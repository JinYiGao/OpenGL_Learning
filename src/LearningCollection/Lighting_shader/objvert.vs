#version 450 core
layout(location=0) in vec3 aPos;
layout(location=1) in vec3 aNormal;//法向量

uniform mat4 transform;
uniform mat4 proj;
uniform mat4 view;
uniform mat4 model;//模型矩阵
uniform mat4 modelMatrix;

out vec3 Normal;
out vec3 FragPos;//物体在世界坐标中的位置

void main()
{
    mat4 model = model * modelMatrix;
    Normal =  mat3(transpose(inverse(model))) * aNormal;
    //片段光照理论上应该在世界坐标下进行计算
    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = proj * view * vec4(FragPos, 1.0);
}