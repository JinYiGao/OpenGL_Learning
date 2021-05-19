#version 450 core
layout(location = 0) in vec3 aPos;

//Uniform块 直接设置绑定点为0
layout(std140, binding = 2) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

// uniform mat4 projection;
// uniform mat4 view;

uniform mat4 model;//该矩阵仅用于相机控制 显示旋转缩放状态等

uniform mat4 modelMatrix; //该模型矩阵用于真正修改模型坐标，最后输出坐标应为modelMatrix * Pos

void main(){
    gl_Position = projection * view * model * modelMatrix * vec4(aPos, 1.0);
}