/*
 * @Descripttion: 20 - 实例化学习 批量渲染物体
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-05-19 18:14:34
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-05-19 18:15:35
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
class InstancingWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    InstancingWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
	GLuint quadVAO, quadVBO, quadColorVBO;
	GLuint instanceVBO;
	Shader *quadShader;
    Camera *camera = new Camera();

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();

		glEnable(GL_DEPTH_TEST);

		// 四边形
		float quadVertices[] = {
			// positions     
			-0.05f,  0.05f,  
			 0.05f, -0.05f,  
			-0.05f, -0.05f,  

			-0.05f,  0.05f,  
			 0.05f, -0.05f,  
			 0.05f,  0.05f,  
		};
		float quadColors[] = {
			// colors
			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 1.0f,

			1.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 1.0f
		};

		Eigen::Vector2f translations[100];
		int index = 0;
		float offset = 0.1f;
		Eigen::Vector2f translate{ 0.0, 0.0 };
		for (int y = -10; y < 10; y += 2)
		{
			for (int x = -10; x < 10; x += 2)
			{
				translate.x() = (float)x / 10.0f + offset;
				translate.y() = (float)y / 10.0f + offset;
				translations[index++] = translate;
			}
		}

		//quadVAO, quadVBO
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		/*glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(2 * sizeof(float)));*/

		glGenBuffers(1, &quadColorVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadColorVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadColors), &quadColors, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);

		//偏移数据缓冲区
		glGenBuffers(1, &instanceVBO);
		glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Eigen::Vector2f) * 100, translations[0].data(), GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(2);
		//告诉OpenGL什么时候更新顶点属性
		glVertexAttribDivisor(2, 1);

		//shader
		quadShader = new Shader(":/LearningCollection/Instancing_shader/instancing.vs",
			":/LearningCollection/Instancing_shader/instancing.fs");
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
        camera->size = {w, h};
        camera->arcball.setSize(Vector2i(w, h));
    }

    void paintGL()
    {
		//update();
		//std::cout << glGetString(GL_VERSION) << std::endl;
		// 清除颜色缓冲
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		quadShader->bind();
		glBindVertexArray(quadVAO);
		glDrawArraysInstanced(GL_TRIANGLES, 0, 6, 100);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		//std::cout << glGetError() << std::endl;
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