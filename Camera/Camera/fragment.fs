#version 330 core

in vec2 fTexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float mixFactor;

void main()
{
    FragColor = mix(texture(texture1, fTexCoord), 
                    texture(texture2, fTexCoord),
                    mixFactor);
}