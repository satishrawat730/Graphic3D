#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform vec3 lightPosition;
uniform vec3 eyePosition;
uniform vec3 lightDir;

out vec2 TextCoord;
//out vec3 Normal;
out vec3 FragPos;
out vec3 TangentEyePosition;
out vec3 TangentLightPosition;
out vec3 dirlight;
out mat3 TBNMatrix;

void main()
{
	gl_Position = projection * view * model * vec4(pos.x , pos.y , pos.z , 1.0);

	TextCoord = tex;
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));

	vec3 T = normalize(normalMatrix * tangent);
    vec3 N = normalize(normalMatrix * norm);    
    vec3 B = normalize(normalMatrix * biTangent);
    
    mat3 TBN = transpose(mat3(T, B, N));	

	FragPos = vec3(model * vec4(pos, 1.0));	

	FragPos = TBN * FragPos ;
	TangentEyePosition = TBN * eyePosition;
	TangentLightPosition = TBN * lightPosition;	

	dirlight = TBN * lightDir;
}