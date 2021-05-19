#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

uniform mat4 transform;
uniform mat4 modelMatrix;

out vec2 TexCoords;

void main()
{
    gl_Position = transform * modelMatrix * vec4(aPos, 1.0);
    TexCoords = aTexCoords;
}