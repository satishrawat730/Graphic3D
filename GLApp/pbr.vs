#version 440 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 biTangent;
layout (location = 5) in vec3 offset;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

out vec2 TextCoord;
out vec3 Normal;
out vec3 WorldPos;

void main()
{
    gl_Position =  projection * view * model * vec4(aPos.x + offset.x, aPos.y + offset.y, aPos.z + offset.z, 1.0);

    TextCoord = aTexCoords;

    Normal = mat3(transpose(inverse(model))) * aNormal;
	WorldPos = (model * vec4(aPos, 1.0)).xyz;	
}