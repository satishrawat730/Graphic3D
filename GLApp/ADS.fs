#version 440

in vec2 TextCoord;
in vec3 Normal;
in vec3 WorldPos;

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

uniform sampler2D albedoMap;

uniform int sampleFromTexture = 0;

uniform DirectionalLight directionalLight;

uniform PointLight pointLight;

uniform Material material;

uniform vec3 eyePosition;

out vec4 color;

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

vec4 CalculateDirectionalLight(DirectionalLight directionalLight, vec3 Normal, vec3 eyePosition, vec3 FragPos, 
                                Material material)
{
   return CalculateADS(directionalLight.baseLight,directionalLight.direction, Normal, eyePosition, FragPos, material);
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
   vec4 dirLightFact = CalculateDirectionalLight(directionalLight, Normal, eyePosition, WorldPos, material);
   vec4 pointLightFact = CalculatePointLight(pointLight, Normal, eyePosition, WorldPos, material);
   vec4 lightFact = pointLightFact + dirLightFact ;
   vec4 texcolor = vec4(material.baseColor.R, material.baseColor.G, material.baseColor.B, 1.0) * texture(albedoMap, TextCoord);
      
   color =  texcolor  * lightFact;  
}