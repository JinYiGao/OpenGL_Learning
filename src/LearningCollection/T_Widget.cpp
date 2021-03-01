/*
 * @Descripttion: 
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-02-25 19:01:44
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-02-25 19:02:28
 */
#include <iostream>

#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QDir>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class T_Widget : public QOpenGLWidget,protected QOpenGLFunctions_4_5_Core
{
public :
	T_Widget(QWidget *parent) : QOpenGLWidget(parent) {

	}

protected:
	void initializeGL() {
		initializeOpenGLFunctions();
	}

	void resizeGL(int w, int h)
	{
		//用于更新投影矩阵或者其他相关大小设置
	}

	void paintGL() {
		
	}
};