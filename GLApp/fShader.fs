#version 440

in vec2 TextCoord;
in vec3 FragPos;
in vec3 TangentLightPosition;
in vec3 TangentEyePosition;
in vec3 dirlight;

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

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform sampler2D texture3;
uniform sampler2D texture4;

uniform int sampleFromTexture = 0;

uniform DirectionalLight directionalLight;

uniform PointLight pointLight;

uniform Material material;

out vec4 color;

vec4 CalculateADS(Light baseLight, vec3 direction, vec3 Normal, vec3 eyePosition, vec3 FragPos, 
                  Material material)
{
   vec4 ambientColor = vec4(0.2, 0.2, 0.2, 1.0);

   float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);       

   vec4 diffuseColor = vec4(baseLight.color, 1.0f) * baseLight.diffuseIntensity * diffuseFactor;

   vec4 specularColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);     

   if(diffuseFactor > 0.0f)
   {       
        vec3 fragToEye = normalize(eyePosition - FragPos);
   		vec3 reflectedVertex = normalize(reflect(-direction, normalize(Normal)));        

   		float specularFactor = dot(normalize(fragToEye), normalize(reflectedVertex));

        if(specularFactor > 0.0f)
   		{             
           specularFactor = pow(specularFactor, material.shininess);
            
  		   specularColor = vec4(baseLight.color * material.specularIntensity * specularFactor, 1.0f);                
        }
   }

   return ambientColor + diffuseColor + specularColor;
}

vec4 CalculateDirectionalLight(DirectionalLight directionalLight, vec3 Normal, vec3 eyePosition, vec3 FragPos, 
                                Material material)
{
   return CalculateADS(directionalLight.baseLight, dirlight , Normal, eyePosition, FragPos, material);
}

vec4 CalculatePointLight(PointLight pointLight, vec3 Normal, vec3 eyePosition, vec3 FragPos, Material material)
{
    vec3 lightDir = normalize(TangentLightPosition - FragPos); 
    float distance = length(lightDir);   

    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance)); 
    return attenuation * CalculateADS(pointLight.baseLight,lightDir, Normal, eyePosition, FragPos, material);
}

void main()
{   
   vec3 Normal = texture(texture2, TextCoord).rgb;
   Normal = normalize(Normal * 2.0 - 1.0);  
   
   vec4 dirLightFact = CalculateDirectionalLight(directionalLight, Normal, TangentEyePosition, FragPos, material);
   vec4 pointLightFact = CalculatePointLight(pointLight, Normal, TangentEyePosition, FragPos, material);
   vec4 lightFact = pointLightFact + dirLightFact;
   vec4 texcolor = texture(texture1, TextCoord);
      
   color = texcolor * lightFact;
}