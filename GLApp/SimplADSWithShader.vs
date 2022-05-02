#version 440

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 tex;
layout (location = 2) in vec3 norm;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 TextCoord;
out vec3 Normal;
out vec3 FragPos;

void main()
{
	gl_Position = projection * view * model * vec4(pos.x , pos.y , pos.z , 1.0);

	TextCoord = tex;
	
	Normal = mat3(transpose(inverse(model))) * norm;
	FragPos = (model * vec4(pos, 1.0)).xyz;	
}