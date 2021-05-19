/*
 * @Descripttion: 14 - 模板测试 (利用模板绘制物体轮廓)
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-09 20:14:23
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-09 20:15:12
 */
#include <iostream>

#include <qevent.h>
#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

#include <Base/shader.h>
#include <Base/texture.h>
#include <Base/camera.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class StencilTestingWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    StencilTestingWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
	GLuint cubeVAO, cubeVBO;
	GLuint planeVAO, planeVBO;
	Shader *objShader;//用于绘制物体
	Shader *singleShader;//用于绘制轮廓

	Texture *texture1, *texture2;

	Camera *camera = new Camera();
	Eigen::Matrix4f transform;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();

		//开启深度测试
		glEnable(GL_DEPTH_TEST);
		//开启模板测试
		glEnable(GL_STENCIL_TEST);
		//片段模板缓冲值不为1则通过测试
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		//模板测试失败 模板测试通过但深度测试失败均保持模板值不变
		//均通过则更新模板缓冲值
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		float cubeVertices[] = {
			// positions          // texture Coords
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
		float planeVertices[] = {
			// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};

		//创建cube VAO
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//创建Plane VAO
		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);
		
		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//创建纹理
		texture1 = new Texture(":/LearningCollection/images/marble.jpg");
		texture2 = new Texture(":/LearningCollection/images/metal.png");

		//创建着色器
		objShader = new Shader(":/LearningCollection/StencilTesting_shader/objvert.vs",
								":/LearningCollection/StencilTesting_shader/objfrag.fs");
		singleShader = new Shader(":/LearningCollection/StencilTesting_shader/objvert.vs",
								":/LearningCollection/StencilTesting_shader/singlefrag.fs");

		//相机初始状态设置
		camera->position = { 0.0f, 0.0f, 1.0f };
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
		//开启模板缓冲
		glEnable(GL_DEPTH_TEST);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);//所有测试均通过时更新模板缓冲值为ref设置的值

		// 清除颜色缓冲
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_TEST);

		//所有片段均通过模板测试, 且写入模板缓冲值为1
		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);
		objShader->bind();
		//绘制立方体
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, texture1->ID);
		Eigen::Matrix4f modelMatrix;
		for (int i = 0; i < 2; i++)
		{
			//camera->model_translation = { 2.0f * float(i),0.0f,0.0f };
			modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 2.0f * float(i),0.0f,0.0f }, 1.0);
			transform = camera->getTransform();
			objShader->setUniformValue("transform", transform);
			objShader->setUniformValue("modelMatrix", modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//模板缓冲所有写入值为0
		glStencilMask(0x00); // 保证我们在绘制地板的时候不会更新模板缓冲
		//绘制平面
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, texture2->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//片段对应的模板缓冲位置 值不为1则通过模板测试
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		//glDisable(GL_DEPTH_TEST);
		singleShader->bind();//更换着色器 绘制边框
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, texture1->ID);
		for (int i = 0; i < 2; i++)
		{
			//放大1.1倍 则外部片段模板值
			//camera->model_translation = { 2.0f * float(i),0.0f,0.0f };
			modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 2.0f * float(i),0.0f,0.0f }, 1.1);
			transform = camera->getTransform();
			singleShader->setUniformValue("transform", transform);
			singleShader->setUniformValue("modelMatrix", modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		glStencilMask(0xFF);
		glStencilFunc(GL_ALWAYS, 0, 0xFF);
		glEnable(GL_DEPTH_TEST);
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