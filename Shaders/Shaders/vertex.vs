#version 330 core

layout (location = 0) in vec3 aPos;

uniform float xOffset;
out vec4 vertexPos;

void main()
{
  gl_Position = vec4(aPos.x + xOffset, aPos.y * -1, aPos.z, 1.0);
  vertexPos = gl_Position;
}