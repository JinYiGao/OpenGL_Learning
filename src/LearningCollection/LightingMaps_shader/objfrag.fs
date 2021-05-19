#version 450 core
out vec4 FragColor;

in vec3 Normal;//当前片段法向量
in vec3 FragPos;//片段在世界坐标中的位置
in vec2 TexCoords;//纹理坐标

uniform mat4 view;//观察矩阵

//光源信息
struct Light
{
    vec3 position;

    vec3 ambient;//环境光
    vec3 diffuse;//漫反射光
    vec3 specular;//镜面反射光
};
uniform Light light;

//材质贴图---漫反射贴图 镜面光贴图
struct Material
{
    sampler2D diffuse;//漫反射贴图
    sampler2D specular;//镜面光贴图
    float shininess;
};
uniform Material material;

void main()
{
    vec3 lightPos = light.position;
    //环境光照
    vec3 ambient = vec3(texture(material.diffuse, TexCoords)) * light.ambient;

    //漫反射光照计算 在世界坐标下进行
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//光线方向
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * vec3(texture(material.diffuse, TexCoords))) * light.diffuse;

    //镜面反射 在观察空间下进行
    vec3 viewDir = normalize(-vec3(view * vec4(FragPos, 1.0)));
    vec3 reflectDir = reflect(-vec3(normalize(view*vec4(lightPos,1.0) - view * vec4(FragPos,1.0))), 
                                mat3(transpose(inverse(view))) * Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (vec3(texture(material.specular, TexCoords)) * spec) * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}