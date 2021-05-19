#version 450 core
out vec4 FragColor;

in vec3 Normal;//当前片段法向量
in vec3 FragPos;//片段在世界坐标中的位置

//uniform vec3 objectColor;//物体颜色
//uniform vec3 lightColor;//光源颜色
//uniform vec3 lightPos;//光源位置
uniform mat4 view;//观察者矩阵

//物体材质信息
struct Material{
    vec3 ambient;//环境光照下物体反射的颜色，通常和物体本身颜色一致
    vec3 diffuse;//漫反射光照
    vec3 specular;//镜面光照
    float shininess;//反光度
};
uniform Material material;//uniform结构体变量

struct Light{
    vec3 position;

    vec3 ambient;//环境光
    vec3 diffuse;//漫反射光
    vec3 specular;//镜面反射光
};
uniform Light light;

void main()
{
    vec3 lightPos = light.position;
    //环境光照
    vec3 ambient = material.ambient * light.ambient;

    //漫反射光照计算 在世界坐标下进行
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//光线方向
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * material.diffuse) * light.diffuse;

    //镜面反射 在观察空间下进行
    vec3 viewDir = normalize(-vec3(view * vec4(FragPos, 1.0)));
    vec3 reflectDir = reflect(-vec3(normalize(view*vec4(lightPos,1.0) - view*vec4(FragPos,1.0))), 
                                mat3(transpose(inverse(view))) * Normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = (material.specular * spec) * light.specular;

    vec3 result = (ambient + diffuse + specular);
    FragColor = vec4(result, 1.0);
}