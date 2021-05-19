#version 450 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 transform;

void main(){
    TexCoords = aTexCoords;
    gl_Position = transform * vec4(aPos, 1.0);
}