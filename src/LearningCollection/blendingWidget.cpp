/*
 * @Descripttion: 15 - 混合 学习
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-10 16:06:51
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-10 16:09:12
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
class BlendingWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    BlendingWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
	GLuint cubeVAO, planeVAO, transparentVAO;
	Shader *shader;
	Texture *texture1, *texture2, *texture3;
    Camera *camera = new Camera();
	Eigen::Matrix4f transform;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
		//开启深度测试
		glEnable(GL_DEPTH_TEST);
		//启用混合
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
			// positions          // texture Coords 
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		float transparentVertices[] = {
			// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

			0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
			1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
			1.0f,  0.5f,  0.0f,  1.0f,  0.0f
		};

		//创建cubeVAO
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);
		GLuint cubeVBO;
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//创建planeVAO
		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);
		GLuint planeVBO;
		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//创建窗户VAO
		glGenVertexArrays(1, &transparentVAO);
		glBindVertexArray(transparentVAO);
		GLuint transparentVBO;
		glGenBuffers(1, &transparentVBO);
		glBindBuffer(GL_ARRAY_BUFFER, transparentVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(transparentVertices), transparentVertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		
		//创建纹理
		texture1 = new Texture(":/LearningCollection/images/marble.jpg");
		texture2 = new Texture(":/LearningCollection/images/metal.png");
		texture3 = new Texture(":/LearningCollection/images/blending_transparent_window.png");

		//创建着色器
		shader = new Shader(":/LearningCollection/Blending_shader/vert.vs",
			":/LearningCollection/Blending_shader/frag.fs");

		//相机初始状态设置
		camera->position = { 0.0f, 0.0f, 1.0f };
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
        camera->size = {w, h};
        camera->arcball.setSize(Vector2i(w, h));
    }

    void paintGL()
    {
		update();
		// 清除颜色缓冲
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->bind();
		//绘制立方体
		glBindVertexArray(cubeVAO);
		glBindTexture(GL_TEXTURE_2D, texture1->ID);
		Eigen::Matrix4f modelMatrix;
		for (int i = 0; i < 2; i++)
		{
			modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 2.0f * float(i),0.0f,0.0f }, 1.0);
			transform = camera->getTransform();
			shader->setUniformValue("transform", transform);
			shader->setUniformValue("modelMatrix", modelMatrix);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ -0.5f,0.0f,0.501f}, 1.0);
		//camera->model_translation = { -0.5f,0.0f,0.501f };
		transform = camera->getTransform();
		shader->setUniformValue("modelMatrix", modelMatrix);
		shader->setUniformValue("transform", transform);
		//绘制平面
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, texture2->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//绘制窗户
		glBindVertexArray(transparentVAO);
		glBindTexture(GL_TEXTURE_2D, texture3->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    //鼠标交互控制
    void mouseReleaseEvent(QMouseEvent *e)
    {

        first = true;
        Eigen::Vector2i screen(e->localPos().x(), e->localPos().y());
        camera->end_rotate(screen);    //结束旋转
        camera->end_translate(screen); //结束平移
    }

    bool first = true; //标志鼠标是否第一次按下
    void mouseMoveEvent(QMouseEvent *e)
    {
        //this->setMouseTracking(true);
        //左键按下
        if (e->buttons() == Qt::LeftButton && first)
        {
            camera->is_rotate = true;
            Vector2i screen(e->localPos().x(), e->localPos().y());
            camera->start_rotate(screen); //开始旋转
            first = false;
        }

        //中键按下
        if (e->buttons() == Qt::MiddleButton && first)
        {
            camera->is_translate = true;
            Vector2f screen(e->localPos().x(), e->localPos().y());
            camera->start_translate(screen); //开始平移
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