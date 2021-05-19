/*
 * @Descripttion: 5- 进入3D坐标系 摄像机系统 鼠标控制 平移旋转
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-03 17:09:49
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-03 17:10:33
 */
#pragma once

#include <iostream>

#include <qevent.h>
#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

#include <Eigen/Eigen>
#include <Base/base.h>
#include <Base/camera.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class ThreeDWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    ThreeDWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }

	GLuint VAO;
	QOpenGLShaderProgram *shaderProgram;
	GLuint texture1, texture2;
	Camera *camera = new Camera();
	Eigen::Matrix4f transform;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
		
		float vertices[] = {
			//顶点坐标            //纹理坐标
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
			 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
		};

		//开启深度缓冲
		glEnable(GL_DEPTH_TEST);

		GLuint VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		int width, height,format;
		unsigned char * data = Base::load_img(":/LearningCollection/images/container.jpg", width, height,format);
		//创建纹理
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//设置纹理环绕 过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); //自动生成多层渐进纹理

		data = Base::load_img(":/LearningCollection/images/awesomeface.png", width, height,format);
		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);
		//设置纹理环绕 过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); //自动生成多层渐进纹理

		//创建着色器程序
		shaderProgram = new QOpenGLShaderProgram();
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LearningCollection/ThreeD_shader/vert.vs");
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/LearningCollection/ThreeD_shader/frag.fs");
		bool success = shaderProgram->link();
		if (success)
		{
			std::cout << "Shader Link Success!" << std::endl;
		}
		shaderProgram->bind();

		//告诉着色器纹理变量对应的纹理单元
		shaderProgram->setUniformValue("texture1", 0);
		shaderProgram->setUniformValue("texture2", 1);

		//初始化相机参数
		//camera->arcball.setState(Quaternionf::Identity());
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
		camera->size = { w,h };
		camera->arcball.setSize(Vector2i(w, h));
    }

	int i = 0;
    void paintGL()
    {
		update();

		// 清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		
		transform = camera->getTransform();
		std::tuple<Eigen::Matrix4f, Eigen::Matrix4f, Eigen::Matrix4f> mvp = camera->get_mvp();
		Eigen::Matrix4f model = std::get<0>(mvp);
		Eigen::Matrix4f view = std::get<1>(mvp);
		Eigen::Matrix4f proj = std::get<2>(mvp);
		//transform = std::get<1>(camera->get_mvp()).matrix();
		//Eigen::Vector4f aPos = { 0.5f,0.5f,0.5f,1.0f };
		//std::cout << transform*aPos << std::endl;
		glBindVertexArray(VAO);
		shaderProgram->bind();
		shaderProgram->setUniformValue("projection", QMatrix4x4(proj.template cast<float>().data(),4,4));
		shaderProgram->setUniformValue("view", QMatrix4x4(view.template cast<float>().data(),4,4));
		shaderProgram->setUniformValue("model", QMatrix4x4(model.template cast<float>().data(),4,4));
		shaderProgram->setUniformValue("transform", QMatrix4x4(transform.cast<float>().data(),4,4));

		glDrawArrays(GL_TRIANGLES, 0, 36);

    }

	void mouseReleaseEvent(QMouseEvent *e)
	{
		
			first = true;
			Eigen::Vector2i screen(e->localPos().x(), e->localPos().y());
			camera->end_rotate(screen);//结束旋转
			camera->end_translate(screen);//结束平移
	}

	bool first = true;//标志鼠标是否第一次按下
	void mouseMoveEvent(QMouseEvent *e)
	{
		//this->setMouseTracking(true);
		//左键按下
		if (e->buttons() == Qt::LeftButton && first)
		{
			camera->is_rotate = true;
			Vector2i screen(e->localPos().x(), e->localPos().y());
			camera->start_rotate(screen);//开始旋转
			first = false;
		}

		//中键按下
		if (e->buttons() == Qt::MiddleButton && first)
		{
			camera->is_translate = true;
			Vector2f screen(e->localPos().x(), e->localPos().y());
			camera->start_translate(screen);//开始平移
			first = false;
		}

		if (e->type() == QEvent::MouseMove && (e->buttons() == Qt::LeftButton))
		{
			Eigen::Vector2i screen(e->localPos().x(), e->localPos().y());
			camera->motion_rotate(screen);
		}

		if (e->type() == QEvent::MouseMove && (e->buttons() == Qt::MiddleButton))
		{
			Eigen::Vector2f screen(e->localPos().x(), e->localPos().y());
			camera->motion_translate(screen);
		}
		
		//std::cout << 4;
	}

	void wheelEvent(QWheelEvent *e)
	{
		// 缩放 这种效果不好 形变大
		camera->zoom = std::max(0.001, camera->zoom * (e->angleDelta().y() > 0 ? 1.1 : 0.9));
		camera->zoom = std::min(1000.0f, camera->zoom);

		//通过改变相机视野 来缩放 效果好
		//camera->view_angle = std::max(0.001, camera->view_angle * (e->angleDelta().y() < 0 ? 1.1 : 0.9));
		//camera->view_angle = std::min(1000.0f, camera->view_angle);
	}
};