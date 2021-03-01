#version 450 core
layout (location=0) in vec3 aPos; //位置变量的属性位置为0

//out vec4 vertexColor;//输出颜色变量

void main(){
    gl_Position=vec4(aPos,1.0);
    //vertexColor=vec4(0.5,0.0,0.0,1.0);
}