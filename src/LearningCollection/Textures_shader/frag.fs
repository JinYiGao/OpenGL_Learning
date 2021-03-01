#version 450 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

//uniform sampler2D ourTexture;//内置数据类型 用于传输存储纹理数据

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;

void main()
{
    //内置函数texture用于根据纹理数据以及纹理坐标来采样颜色
    //FragColor=texture(ourTexture,TexCoord) * vec4(ourColor,1.0);
    FragColor=mix(texture(ourTexture1,TexCoord),texture(ourTexture2,TexCoord),0.2);
}