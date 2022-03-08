/*
 * @Descripttion: 6 - 学习颜色的一些技巧 创建一个光照场景
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-06 14:00:48
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-06 14:01:22
 */
#include <iostream>

#include <qevent.h>
#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

#include <Base/camera.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class ColorWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    ColorWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }

	GLuint cubeVAO, lightVAO;
	QOpenGLShaderProgram *cubeShader, *lightShader;
	Camera *camera = new Camera();
	Eigen::Matrix4f transform;

protected:
    void initializeGL()
    {
		Eigen::MatrixXf matrix;
		matrix.resize(4, 3);
		matrix <<
			1, 5, 10,
			4, 3, 6,
			7, 8, 9,
			10, 11, 12;
		Vector3f a = { 1, 2, 3 };
		Vector4f minp = matrix.rowwise().minCoeff();
		std::cout << *(matrix.data() + 2) << std::endl;

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

		glEnable(GL_DEPTH_TEST);//开启z缓冲

		//设置cubeVAO记录状态信息
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);

		GLuint VBO;//设置立方体数据
		glCreateBuffers(1, &VBO);
		//glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		//glNamedBufferStorage(VBO, sizeof(vertices), vertices, GL_DYNAMIC_STORAGE_BIT);
		glNamedBufferData(VBO, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);

		//设置lightVAO记录状态信息
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(0));
		glEnableVertexAttribArray(0);

		//创建着色器程序
		cubeShader = new QOpenGLShaderProgram();
		cubeShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LearningCollection/Color_shader/Objvert.vs");
		cubeShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/LearningCollection/Color_shader/Objfrag.fs");
		cubeShader->link();

		lightShader = new QOpenGLShaderProgram();
		lightShader->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LearningCollection/Color_shader/lightvert.vs");
		lightShader->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/LearningCollection/Color_shader/lightfrag.fs");
		lightShader->link();

		//初始化相机信息
		camera->arcball.setState(Eigen::Quaternionf::Identity());
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
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		Eigen::Matrix4f modelMatrix;
		//camera->model_translation = { 0.0f,0.0f,0.0f };
		//camera->zoom = 1.0f;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 0.0f,0.0f,0.0f }, 1.0);
		transform = camera->getTransform();
		cubeShader->bind();
		cubeShader->setUniformValue("objectColor", 1.0f, 0.5f, 0.31f);
		cubeShader->setUniformValue("lightColor", 1.0f, 1.0f, 1.0f);
		cubeShader->setUniformValue("transform", QMatrix4x4(transform.template cast<float>().data(), 4, 4));
		cubeShader->setUniformValue("modelMatrix", QMatrix4x4(modelMatrix.template cast<float>().data(), 4, 4));
		glBindVertexArray(cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//camera->model_translation = { 1.2f, 2.0f, -2.0f };
		//camera->zoom = 0.5f;
		modelMatrix = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ 1.2f, 2.0f, -2.0f }, 0.5);
		transform = camera->getTransform();
		lightShader->bind();
		lightShader->setUniformValue("transform", QMatrix4x4(transform.template cast<float>().data(), 4, 4));
		lightShader->setUniformValue("modelMatrix", QMatrix4x4(modelMatrix.template cast<float>().data(), 4, 4));
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