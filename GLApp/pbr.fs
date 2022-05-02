#version 440 core

out vec4 FragColor;

in vec2 TextCoord;
in vec3 Normal;
in vec3 WorldPos;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform vec3 eyePosition;

//ads lights -----------------------------------------------------------------
struct BaseColor
{
    float R, G, B;
};

struct Light
{
    float ambientIntensity;
    vec3 color;
    float diffuseIntensity;
};

struct DirectionalLight
{    
    Light baseLight;
    vec3 direction;    
};

struct PointLight
{
    Light baseLight;
    float constant;
    float linear;
    float quadratic;
    vec3 position;
};

struct Material
{
    float shininess;
    float specularIntensity;
    BaseColor baseColor;
};
uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform Material material;

// pbr  lights -----------------------------------------------------------------------------
vec3 lightPositions[] = 
{
	vec3(-10.0f,  10.0f,  10.0f),
	vec3(10.0f,  10.0f,  10.0f),
	vec3(-10.0f, -10.0f,  10.0f),
	vec3(10.0f, -10.0f,  10.0f),
};
vec3 lightColors[] =
{
    vec3( 340, 340, 340),
    vec3( 340, 340, 340),
    vec3( 340, 340, 340),
    vec3( 340, 340, 340),
};
//--------------------------------------------------------------------------------------------
const float PI = 3.14159265359;
// ----------------------------------------------------------------------------


// replace this normal map code with the one inside NormalMap shader
vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TextCoord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TextCoord);
    vec2 st2 = dFdy(TextCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------

vec4 CalculateADS(Light baseLight, vec3 direction, vec3 Normal, vec3 eyePosition, vec3 WorldPos, 
                  Material material)
{
   vec4 ambientColor = vec4(0.1, 0.1, 0.1, 1.0);

   float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);       

   vec4 diffuseColor = vec4(baseLight.color, 1.0f) * baseLight.diffuseIntensity * diffuseFactor;

   vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

   if(diffuseFactor > 0.0f)
   {       
        vec3 fragToEye = normalize(eyePosition - WorldPos);
   		vec3 reflectedVertex = normalize(reflect(-direction, normalize(Normal)));        

   		float specularFactor = dot(normalize(fragToEye), normalize(reflectedVertex));        

        if(specularFactor > 0.0f)
   		{   			
            float specularBeforePower = specularFactor;
            specularFactor = pow(specularFactor, material.shininess);
            
   			specularColor = vec4(baseLight.color * material.specularIntensity * specularFactor, 1.0f);                
        }       
   }

   return ambientColor + diffuseColor + specularColor;
}
vec4 CalculatePointLight(PointLight pointLight, vec3 Normal, vec3 eyePosition, vec3 WorldPos, Material material)
{
    vec3 lightDir = normalize(pointLight.position - WorldPos); 
     float distance = length(pointLight.position - WorldPos);  
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance)); 
    return attenuation * CalculateADS(pointLight.baseLight,lightDir, Normal, eyePosition, WorldPos, material);
}
void main()
{		
    vec3 albedo     = pow(texture(albedoMap, TextCoord).rgb, vec3(2.2));
    float metallic  = texture(metallicMap, TextCoord).r;
    float roughness = texture(roughnessMap, TextCoord).r;
    float ao        = texture(aoMap, TextCoord).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(eyePosition - WorldPos);

    // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
    // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    for(int i = 0; i < 4; ++i) 
    {
        // calculate per-light radiance
        vec3 L = normalize(lightPositions[i] - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - WorldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);   
        float G   = GeometrySmith(N, V, L, roughness);      
        vec3 F    = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);
           
        vec3 numerator    = NDF * G * F; 
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001; // + 0.0001 to prevent divide by zero
        vec3 specular = numerator / denominator;
        
        // kS is equal to Fresnel
        vec3 kS = F;
        // for energy conservation, the diffuse and specular light can't
        // be above 1.0 (unless the surface emits light); to preserve this
        // relationship the diffuse component (kD) should equal 1.0 - kS.
        vec3 kD = vec3(1.0) - kS;
        // multiply kD by the inverse metalness such that only non-metals 
        // have diffuse lighting, or a linear blend if partly metal (pure metals
        // have no diffuse light).
        kD *= 1.0 - metallic;	  

        // scale light by NdotL
        float NdotL = max(dot(N, L), 0.0);        

        // add to outgoing radiance Lo
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
    }   
    
    // ambient lighting (note that the next IBL tutorial will replace 
    // this ambient lighting with environment lighting).
    vec3 ambient = vec3(0.03) * albedo * ao;

    vec3 color = ambient + Lo;
    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correct
    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);

    //==== ads 
//    vec4 cl2;
//   
//    vec4 pointLightFact = CalculatePointLight(pointLight, Normal, eyePosition, WorldPos, material);
//
//    cl2 = pointLightFact;
//   
//    FragColor = vec4(.5,0,0,1) * cl2;
}
