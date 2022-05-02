#version 440

out vec4 FragColor;

in vec2 BaseTexCoord;
in vec2 DetailTexCoord;
in vec3 Normals;
in vec3 WorldPos;
in vec3 FragPos;
in vec3 TangentLightPosition;
in vec3 TangentEyePosition;
in vec3 dirlight;

struct BaseColor {  float R, G, B; };

struct Light {
    float ambientIntensity;
    vec3 color;
    float diffuseIntensity;
};

struct DirectionalLight {
    Light baseLight;
    vec3 direction;
};

struct PointLight {
    Light baseLight;
    float constant;
    float linear;
    float quadratic;
    vec3 position;
};

struct Material {
    float shininess;
    float specularIntensity;
    BaseColor baseColor;
};

vec4 CalculateADS(Light baseLight, vec3 direction, vec3 Normal, vec3 eyePosition, vec3 WorldPos, Material material)
{
   vec4 ambientColor = vec4(.1);
   vec4 specularColor = vec4(0.1f, 0.1f, 0.1f, 0.1f);

   float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
   vec4 diffuseColor = vec4(baseLight.color, 1.0f) * baseLight.diffuseIntensity * diffuseFactor;

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

vec4 CalculateDirectionalLight(DirectionalLight directionalLight, vec3 Normal, vec3 eyePosition, vec3 FragPos, Material material)
{
   return CalculateADS(directionalLight.baseLight, directionalLight.direction, Normal, eyePosition, FragPos, material);
}

vec4 CalculatePointLight(PointLight pointLight, vec3 Normal, vec3 eyePosition, vec3 FragPos, Material material)
{
    vec3 lightDir = normalize(TangentLightPosition - FragPos); 
    float distance = length(lightDir);

    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance)); 
    return attenuation * CalculateADS(pointLight.baseLight,lightDir, Normal, eyePosition, FragPos, material);
}

uniform sampler2D baseTexture;
uniform sampler2D detailTexture;

uniform sampler2D grassTexture;
uniform sampler2D grassNormalTexture;

uniform sampler2D slopeTexture;
uniform sampler2D slopeNormalTexture;

uniform sampler2D rockTexture;
uniform sampler2D rockNormalTexture;

uniform DirectionalLight directionalLight;
uniform PointLight pointLight;
uniform vec3 eyePosition;
uniform int applyTexture;

void main()
{
    Material material;
    material.shininess = 0.3;
    material.specularIntensity = 0.1;
    BaseColor bColor;
    bColor.R = .5; bColor.G = .5; bColor.B = .5;
    material.baseColor = bColor;

    vec4 finalTexColor;
    if(applyTexture != 0){
    // Slope based texturing
        vec4 grassColor = texture(grassTexture, DetailTexCoord);
        vec3 grassNormal = texture(grassNormalTexture, DetailTexCoord).rgb;
        grassNormal =  normalize(grassNormal* 2.0 - 1.0);

        vec4 slopeColor = texture(slopeTexture, DetailTexCoord);
        vec3 slopeNormal = texture(slopeNormalTexture, DetailTexCoord).rgb;
        slopeNormal = normalize(slopeNormal* 2.0 - 1.0);

        vec4 rockColor = texture(rockTexture, DetailTexCoord);
        vec3 rockNormal = texture(rockNormalTexture, DetailTexCoord).rgb;
        rockNormal = normalize(rockNormal* 2.0 - 1.0);

        vec4 slopeTexColor;
        vec3 slopeNormalTerrain;

        vec3 heights = normalize(Normals);
        float slope = heights.y;

        float blendAmount, low = 0.2f, mid = 0.5f;
        // Determine which texture to use based on height.
        if(slope < low)
        {
            blendAmount = slope /low;
            slopeTexColor = mix(grassColor, slopeColor, blendAmount);
            slopeNormalTerrain = mix(grassNormal, slopeNormal, blendAmount);
        }	
        else if((slope < mid) && (slope >= low))
        {
            blendAmount = (slope - low) * (1.0f / (mid - low));
            slopeTexColor = mix(slopeColor, rockColor, blendAmount);
            slopeNormalTerrain = mix(slopeNormal, rockNormal, blendAmount);
        }
        else if(slope >= mid) 
        {
            slopeTexColor = rockColor;
            slopeNormalTerrain = rockNormal;
        }

        vec4 detailTexColor = texture(detailTexture, DetailTexCoord) * 0.6;
        vec4 baseTexColor = texture(baseTexture, BaseTexCoord);

        // light calculation
        vec4 dirLightFact = CalculateDirectionalLight( directionalLight, slopeNormalTerrain, eyePosition, FragPos, material);
        vec4 pointLightFact = CalculatePointLight(pointLight, slopeNormalTerrain, TangentEyePosition, FragPos, material);
        vec4 lightFact = pointLightFact + dirLightFact;

        // final color
        FragColor = slopeTexColor * lightFact * baseTexColor;// * detailTexColor;
    }
    else
    {
        // light calculation
        vec4 dirLightFact = CalculateDirectionalLight( directionalLight, Normals, eyePosition, FragPos, material);
        vec4 pointLightFact = CalculatePointLight(pointLight, Normals, TangentEyePosition, FragPos, material);
        vec4 lightFact = pointLightFact + dirLightFact;
        // final color
        FragColor = lightFact;
    }
}
