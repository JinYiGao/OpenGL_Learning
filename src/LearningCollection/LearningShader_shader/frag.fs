#version 450 core
out vec4 FragColor;

//---1.颜色数据从顶点着色器传入----
//in vec4 vertexColor;//从顶点着色器接受的变量 (类型 名称必须相同)

//---2.颜色数据由应用程序直接设置----
uniform vec4 ourColor;

void main()
{
    //FragColor=vertexColor;
    FragColor=ourColor;
}