/*
 * @Descripttion: 4-初步涉及矩阵操作---对物体进行旋转和平移
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-01 20:54:04
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-01 22:37:22
 */
#include <iostream>

#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QDir>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class TransformWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    TransformWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
    }

    void paintGL()
    {
    }
};