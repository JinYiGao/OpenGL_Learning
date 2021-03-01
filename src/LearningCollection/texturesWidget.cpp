/*
 * @Descripttion: 3-纹理变量使用学习
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-02-25 19:01:04
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-01 21:00:10
 */
#include <iostream>

#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QDir>

#include <Base/stb_image.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class TexturesWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    TexturesWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
    GLuint VAO;
    QOpenGLShaderProgram *shaderProgram;
    GLuint texture, texture1;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();

        float vertices[] = {
            //		 ----位置----       ----颜色---- - 纹理坐标 -
            0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
            -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
        };

        //顶点索引指定
        unsigned int indices[] = {
            0, 1, 3, // first triangle
            1, 2, 3  // second triangle
        };

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        GLuint EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        GLuint VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //设置数据解析
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        //加载纹理图像数据
        int width, height, nrChannels;
        unsigned char *data = stbi_load("D:/Study/Programming/OpenGL_Learning/LearningCollection/src/LearningCollection/images/container.jpg", &width, &height, &nrChannels, 0);
        //std::cout << width << std::endl;

        //---------------Texture1------------------
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        //设置纹理环绕 过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //自动生成多层渐进纹理
        width = 0, height = 0;
        //---------------Texture2------------------
        data = stbi_load("D:/Study/Programming/OpenGL_Learning/LearningCollection/src/LearningCollection/images/awesomeface.png", &width, &height, &nrChannels, 0);
        glGenTextures(1, &texture1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        //设置纹理环绕 过滤方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //自动生成多层渐进纹理

        //创建着色器程序
        shaderProgram = new QOpenGLShaderProgram();
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LearningCollection/Textures_shader/vert.vs");
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/LearningCollection/Textures_shader/frag.fs");
        bool success = shaderProgram->link();
        if (success)
        {
            std::cout << "Shader Link Success!" << std::endl;
        }
        shaderProgram->bind();

        //告诉着色器纹理变量对应的纹理单元
        shaderProgram->setUniformValue("ourTexture1", 0);
        shaderProgram->setUniformValue("ourTexture2", 1);
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
    }

    void paintGL()
    {
        update();
        // 清除颜色缓冲
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //----- 一个纹理时，默认激活GL_TEXTURE0 -----
        //glBindTexture(GL_TEXTURE_2D, texture1);//会自动传入纹理到着色器

        //----- 多个纹理时，需要确定哪个纹理对应哪个纹理单元，并且需要设置着色器纹理变量所对应的纹理单元
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 6);
    }
};