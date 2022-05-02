#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 baseTexCoord;
layout (location = 2) in vec2 detailTexCoord;
layout (location = 3) in vec3 normals;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;
uniform float heightScale;
uniform float gridScale;
uniform sampler2D heightTexture;
uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform vec3 lightDir;
uniform int applyTexture;

out vec2 BaseTexCoord;
out vec2 DetailTexCoord;
out vec3 Normals;
out vec3 FragPos;
out vec3 TangentEyePosition;
out vec3 TangentLightPosition;
out vec3 dirlight;

void main()
{
    vec3 newPos;
    newPos.x = pos.x * gridScale;
    newPos.y = texture(heightTexture, baseTexCoord).r * heightScale;
    newPos.z = pos.z * gridScale;

    if(applyTexture != 0){
        gl_Position = projection * view * model * vec4(newPos, 1.0);
    }
    else // without heightmap
        gl_Position = projection * view * model * vec4(pos.x * gridScale, pos.y * heightScale, pos.z * gridScale, 1.0);

    mat3 normalMatrix = transpose(inverse(mat3(model)));

    vec3 tangent = texture(heightTexture, baseTexCoord).rgb;
    vec3 Tangent = normalize(normalMatrix * tangent);
    vec3 norm = texture(heightTexture, baseTexCoord).rgb;
    vec3 Normal = normalize(normalMatrix * norm);
    vec3 biTangent = texture(heightTexture, baseTexCoord).rgb;
    vec3 BiTangent = normalize(normalMatrix * biTangent);

	mat3 TBN = transpose(mat3( Tangent, BiTangent, Normal));
	FragPos = vec3(model * vec4(pos, 1.0));
	FragPos = TBN * FragPos ;

	BaseTexCoord = baseTexCoord;
	DetailTexCoord = detailTexCoord;
	Normals = mat3(transpose(inverse(model))) * newPos;

	TangentEyePosition = TBN * eyePosition;
	TangentLightPosition = TBN * lightPosition;
	dirlight = TBN * lightDir;
}