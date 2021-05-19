/*
 * @Descripttion: 8 - 材质学习
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-08 13:37:00
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-08 13:38:14
 */
#include <iostream>

#include <qevent.h>
#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QTime>

#include <Base/shader.h>
#include <Base/camera.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class MaterialsWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    MaterialsWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }

	GLuint cubeVAO, lightVAO;
	Shader *cubeShader;
	Shader *lightShader;
	Camera *camera = new Camera();
	Eigen::Matrix4f transform;

	Eigen::Vector3f lightPos;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();

		float vertices[] = {
			//顶点坐标            //法向量
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
			-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
			-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
			-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
			-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
		};

		glEnable(GL_DEPTH_TEST);
		//设置cubeVAO记录顶点信息
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);//顶点坐标
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));//法向信息
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//设置lightVAO记录顶点信息
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		//创建着色器
		cubeShader = new Shader(":/LearningCollection/Materials_shader/objvert.vs",
			":/LearningCollection/Materials_shader/objfrag.fs");
		lightShader = new Shader(":/LearningCollection/Materials_shader/lightvert.vs",
			":/LearningCollection/Materials_shader/lightfrag.fs");

		//不会变的uniform设置
		cubeShader->bind();
		cubeShader->setUniformValue("lightColor", 1.0f, 1.0f, 1.0f);

		//赋予物体材质信息
		cubeShader->setUniformValue("material.ambient", 1.0f, 0.5f, 0.31f);
		cubeShader->setUniformValue("material.diffuse", 1.0f, 0.5f, 0.31f);
		cubeShader->setUniformValue("material.specular", 0.5f, 0.5f, 0.5f);
		cubeShader->setUniformValue("material.shininess", 32.0f);

		//设置光源信息
		lightPos = { 0.7f, 2.0f, -3.0f };
		cubeShader->setUniformValue("light.position", lightPos);
		cubeShader->setUniformValue("light.ambient", 0.2f, 0.2f, 0.2f);
		cubeShader->setUniformValue("light.diffuse", 0.5f, 0.5f, 0.5f);
		cubeShader->setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
		camera->size = { w,h };
		camera->arcball.setSize(Vector2i(w, h));
    }

    void paintGL()
    {
		update();
		// 清除颜色缓冲
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Eigen::Matrix4f modelMatrix;
		//camera->model_translation = { 0.0f,0.0f,0.0f };
		//camera->zoom = 1.0f;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 0.0f,0.0f,0.0f }, 1.0);
		transform = camera->getTransform();
		cubeShader->bind();
		Eigen::Matrix4f model = std::get<0>(camera->get_mvp());
		cubeShader->setUniformValue("modelMatrix", modelMatrix);
		cubeShader->setUniformValue("model", model);
		Eigen::Matrix4f view = std::get<1>(camera->get_mvp());
		cubeShader->setUniformValue("view", view);
		cubeShader->setUniformValue("transform", transform);

		//随时间改变光源颜色
		float timeValue = QTime::currentTime().second();
		Vector3f lightColor = { sin(timeValue*2.0f),sin(timeValue*0.7f),sin(timeValue*1.3f) };
		Vector3f ambient = 0.2*lightColor;
		Vector3f diffuse = 0.5*lightColor;
		cubeShader->setUniformValue("light.ambient", ambient);
		cubeShader->setUniformValue("light.diffuse", diffuse);
		cubeShader->setUniformValue("light.specular", 1.0f, 1.0f, 1.0f);

		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//camera->model_translation = lightPos;
		//camera->zoom = 0.5f;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 0.7f, 2.0f, -3.0f }, 0.5);
		transform = camera->getTransform();
		lightShader->bind();
		lightShader->setUniformValue("modelMatrix", modelMatrix);
		lightShader->setUniformValue("transform", transform);
		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	//鼠标交互控制
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