/*
 * @Descripttion: 19 - uniform缓冲对象使用示例
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-05-17 21:28:22
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-05-17 21:29:21
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
class UniformBufferWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    UniformBufferWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
	GLuint cubeVAO, cubeVBO;
	GLuint uboMatrices;
	Shader *shaderRed, *shaderGreen, *shaderBlue, *shaderYellow;
    Camera *camera = new Camera();

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
		glEnable(GL_DEPTH_TEST);

		float cubeVertices[] = {
			// positions         
			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,

			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f, -0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,

			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,

			-0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f, -0.5f,
			 0.5f, -0.5f,  0.5f,
			 0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f,  0.5f,
			-0.5f, -0.5f, -0.5f,

			-0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f, -0.5f,
			 0.5f,  0.5f,  0.5f,
			 0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f,  0.5f,
			-0.5f,  0.5f, -0.5f,
		};

		// cube
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		//创建着色器
		shaderRed = new Shader(":/LearningCollection/UniformBuffer_shader/cube.vs",
			":/LearningCollection/UniformBuffer_shader/red.fs");
		shaderGreen = new Shader(":/LearningCollection/UniformBuffer_shader/cube.vs",
			":/LearningCollection/UniformBuffer_shader/green.fs");
		shaderBlue = new Shader(":/LearningCollection/UniformBuffer_shader/cube.vs",
			":/LearningCollection/UniformBuffer_shader/blue.fs");
		shaderYellow = new Shader(":/LearningCollection/UniformBuffer_shader/cube.vs",
			":/LearningCollection/UniformBuffer_shader/yellow.fs");

		// 创建Uniform缓冲对象 绑定到绑定点0
		glGenBuffers(1, &uboMatrices);
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		///\开辟空间
		glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(Eigen::Matrix4f), NULL, GL_STATIC_DRAW); // 64bytes
		glBindBufferBase(GL_UNIFORM_BUFFER, 2, uboMatrices);//绑定到绑定点0

		camera->position = Eigen::Vector3f{ 0.0f, 0.0f, 1.0f };
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

		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//获取camera projection view model
		std::tuple<Eigen::Matrix4f, Eigen::Matrix4f, Eigen::Matrix4f> mvp = camera->get_mvp();
		Eigen::Matrix4f view = std::get<1>(mvp);
		Eigen::Matrix4f proj = std::get<2>(mvp);
		//对Uniform缓冲对象写入数据
		glBindBuffer(GL_UNIFORM_BUFFER, uboMatrices);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Eigen::Matrix4f), proj.data());
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(Eigen::Matrix4f), sizeof(Eigen::Matrix4f), view.data());

		Eigen::Matrix4f model;
		//draw cubes
		glBindVertexArray(cubeVAO);
		shaderRed->bind();
		//camera->model_translation = Eigen::Vector3f{ -0.75f, 0.75f, 0.0f } * camera->zoom; //平移间距也应该缩放
		Eigen::Matrix4f modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ -0.75f, 0.75f, 0.0f }, 1.0);
		mvp = camera->get_mvp();
		model = std::get<0>(mvp);
		shaderRed->setUniformValue("model", model);
		shaderRed->setUniformValue("modelMatrix", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shaderGreen->bind();
		//camera->model_translation = Eigen::Vector3f{ 0.75f, 0.75f, 0.0f } * camera->zoom;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 0.75f, 0.75f, 0.0f }, 1.0);
		mvp = camera->get_mvp();
		model = std::get<0>(mvp);
		shaderGreen->setUniformValue("model", model);
		shaderGreen->setUniformValue("modelMatrix", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shaderBlue->bind();
		//camera->model_translation = Eigen::Vector3f{ -0.75f, -0.75f, 0.0f } * camera->zoom;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ -0.75f, -0.75f, 0.0f }, 1.0);
		mvp = camera->get_mvp();
		model = std::get<0>(mvp);
		shaderBlue->setUniformValue("model", model);
		shaderBlue->setUniformValue("modelMatrix", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		shaderYellow->bind();
		//camera->model_translation = Eigen::Vector3f{ 0.75f, -0.75f, 0.0f } * camera->zoom;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 0.75f, -0.75f, 0.0f }, 1.0);
		mvp = camera->get_mvp();
		model = std::get<0>(mvp);
		shaderYellow->setUniformValue("model", model);
		shaderYellow->setUniformValue("modelMatrix", modelMatrix);
		glDrawArrays(GL_TRIANGLES, 0, 36);
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
		//camera->position.z() = std::max(0.001, camera->position.z() * (e->angleDelta().y() < 0 ? 1.1 : 0.9));
		//camera->position.z() = std::min(1000.0f, camera->position.z());

        // 缩放
		camera->zoom = std::max(0.001, camera->zoom * (e->angleDelta().y() > 0 ? 1.1 : 0.9));
		camera->zoom = std::min(1000.0f, camera->zoom);

        //通过改变相机视野 来缩放 效果好
        //camera->view_angle = std::max(0.001, camera->view_angle * (e->angleDelta().y() < 0 ? 1.1 : 0.9));
        //camera->view_angle = std::min(1000.0f, camera->view_angle);
    }
};