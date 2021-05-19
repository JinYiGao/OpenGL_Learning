#version 450 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;//法向量
layout(location=2) in vec2 aTexCoords;//纹理坐标

uniform mat4 transform;
uniform mat4 model;//模型矩阵
uniform mat4 modelMatrix;

out vec3 Normal;
out vec3 FragPos;//物体在世界坐标中的位置
out vec2 TexCoords;

void main()
{
    mat4 model = model * modelMatrix;
    gl_Position=transform * vec4(aPos,1.0);
    Normal = aNormal;
    //片段光照理论上应该在世界坐标下进行计算
    FragPos = vec3(model * vec4(aPos,1.0));
    TexCoords = aTexCoords;
}