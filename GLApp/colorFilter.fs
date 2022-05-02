#version 440 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D screenTexture;

uniform vec3 colorFilter;

void main()
{
    vec3 col = texture(screenTexture, TexCoords).rgb;

    // make a sepia tone color
    //vec3 colorFilter = vec3(1.34, 1.0, 0.8);
    
    // get gray/intensity scale
    float gray = (col.r + col.g + col.b) / 3.0;
    
    // combine
    vec3 color = gray * colorFilter ;
    
    FragColor= vec4(color, 1.0f);

 } 