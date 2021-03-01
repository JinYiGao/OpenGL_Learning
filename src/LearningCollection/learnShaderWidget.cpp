/*
 * @Descripttion: 2-着色器学习
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-02-25 16:22:09
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-01 21:00:00
 */

#include <iostream>

#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QDir>
#include <QTime>
#include <QVector>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class LearnShaderWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    LearnShaderWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
    GLuint VAO;
    QOpenGLShaderProgram *shaderProgram;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
        //顶点数据
        float vertices[] = {
            -0.5, -0.5, 0.0,
            0.5, -0.5, 0.0,
            0.0, 0.5, 0.0};

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);

        //开辟一个缓冲区，用于存放顶点数据
        GLuint VBO;
        //std::cout << &VBO << "  "<< std::endl;
        glGenBuffers(1, &VBO);

        //设置该缓冲区对应的类型
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        //将数据存到开辟好的指定类型的缓冲区内
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        //创建着色器程序对象
        shaderProgram = new QOpenGLShaderProgram();
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LearningCollection/LearningShader_shader/vert.vs");
        shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/LearningCollection/LearningShader_shader/frag.fs");
        bool success = shaderProgram->link();
        if (success)
        {
            std::cout << "Shader Link Success!" << std::endl;
        }
        //std::cout << seccess << " " << shaderProgram->log().toStdString() << std::endl;

        //告诉opengl如何解析顶点数据
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        //启用顶点属性 ，上面设置解析 默认不生效
        glEnableVertexAttribArray(0);
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

        shaderProgram->bind();

        int timeValue = QTime::currentTime().second();
        //std::cout << timeValue << std::endl;
        float greenValue = sin(timeValue) / 2.0 + 0.5;
        //std::cout << greenValue << std::endl;
        //float ourColor[] = { greenValue, 0.0, 1.0 };
        shaderProgram->setUniformValue("ourColor", 0.0, greenValue, 0.0, 1.0);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
};