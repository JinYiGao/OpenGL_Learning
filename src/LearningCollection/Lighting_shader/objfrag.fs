#version 450 core
out vec4 FragColor;

in vec3 Normal;//当前片段法向量
in vec3 FragPos;//片段在世界坐标中的位置

uniform vec3 objectColor;//物体颜色
uniform vec3 lightColor;//光源颜色
uniform vec3 lightPos;//光源位置
uniform mat4 view;//观察者矩阵
uniform mat4 model;
uniform vec3 cameraPos;

void main()
{
    vec3 lightPos = vec3(model * vec4(lightPos, 1.0));
    //环境光照
    float ambientStrength = 0.1;//环境光照强度
    vec3 ambient = ambientStrength * lightColor;

    //漫反射光照计算 在世界坐标下进行
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);//光线方向
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    //镜面反射 在观察空间下进行
    float specularStrength = 0.5;//镜面反射强度
    // vec3 viewDir = normalize(-vec3(view * vec4(FragPos, 1.0)));
    // vec3 reflectDir = reflect(-vec3(normalize(view*vec4(lightPos,1.0) - view*vec4(FragPos,1.0))), 
    //                             mat3(transpose(inverse(view))) * Normal);
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // vec3 specular = specularStrength * spec * lightColor;

    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}