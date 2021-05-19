/*
 * @Descripttion: 18 - 立方体贴图之——天空盒
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-05-16 15:05:44
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-05-16 15:06:33
 */
#include <iostream>
#include <vector>

#include <qevent.h>
#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>

#include <Base/shader.h>
#include <Base/texture.h>
#include <Base/camera.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class CubeMapsWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    CubeMapsWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
	unsigned int cubeVAO, cubeVBO;
	unsigned int skyboxVAO, skyboxVBO;
	Texture *cubemapTexture;
	Shader *shader, *skyboxShader;
    Camera *camera = new Camera();

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
		
		glEnable(GL_DEPTH_TEST);

		float cubeVertices[] = {
			// positions          // normals
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
		float skyboxVertices[] = {
			// positions          
			-1.0f,  1.0f, -1.0f,
			-1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f, -1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,

			-1.0f, -1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f,
			-1.0f, -1.0f,  1.0f,

			-1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f, -1.0f,
			 1.0f,  1.0f,  1.0f,
			 1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f,  1.0f,
			-1.0f,  1.0f, -1.0f,

			-1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f, -1.0f,
			 1.0f, -1.0f, -1.0f,
			-1.0f, -1.0f,  1.0f,
			 1.0f, -1.0f,  1.0f
		};

		//cube
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		//skybox
		glGenVertexArrays(1, &skyboxVAO);
		glBindVertexArray(skyboxVAO);
		glGenBuffers(1, &skyboxVBO);
		glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);

		//cubeTexture
		std::vector<std::string> faces
		{
			":/LearningCollection/images/right.jpg",
			":/LearningCollection/images/left.jpg",
			":/LearningCollection/images/top.jpg",
			":/LearningCollection/images/bottom.jpg",
			":/LearningCollection/images/front.jpg",
			":/LearningCollection/images/back.jpg",
		};
		cubemapTexture = new Texture();
		cubemapTexture->createCubeMap(faces);

		//创建着色器
		shader = new Shader(":/LearningCollection/CubeMaps_shader/cubemaps.vs",
			":/LearningCollection/CubeMaps_shader/cubemaps.fs");
		skyboxShader = new Shader(":/LearningCollection/CubeMaps_shader/skybox.vs",
			":/LearningCollection/CubeMaps_shader/skybox.fs");

		shader->bind();
		shader->setUniformValue("skybox", 0);
		skyboxShader->bind();
		skyboxShader->setUniformValue("skybox", 0);

		camera->position = Eigen::Vector3f{ 0.0f,0.0f,0.8f };
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

		//draw cube
		shader->bind();
		//get mvp
		std::tuple<Eigen::Matrix4f, Eigen::Matrix4f, Eigen::Matrix4f> mvp = camera->get_mvp();
		Eigen::Matrix4f model = std::get<0>(mvp);
		Eigen::Matrix4f view = std::get<1>(mvp);
		Eigen::Matrix4f proj = std::get<2>(mvp);
		shader->setUniformValue("model", model);
		shader->setUniformValue("view", view);
		shader->setUniformValue("projection", proj);
		shader->setUniformValue("cameraPos", camera->position);
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture->ID); //这里设置纹理是为了采样反射和折射纹理
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//draw skybox
		glDepthFunc(GL_LEQUAL); //在深度值小于等于1.0时通过深度测试
		skyboxShader->bind();
		//std::cout << camera->position << std::endl;
		model = camera->createModel(Eigen::Matrix4f::Identity(), Eigen::Vector3f{ camera->position.x(),camera->position.y(),0.0f }, 1.0);
		skyboxShader->setUniformValue("model", model);
		skyboxShader->setUniformValue("view", view);
		skyboxShader->setUniformValue("projection", proj);
		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture->ID); //这里设置的纹理是正常天空盒纹理
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
        // 缩放 这种效果不好 形变大
        camera->zoom = std::max(0.001, camera->zoom * (e->angleDelta().y() > 0 ? 1.1 : 0.9));
		camera->zoom = std::min(1000.0f, camera->zoom);

        //通过改变相机视野 来缩放 效果好
        /*camera->view_angle = std::max(0.001, camera->view_angle * (e->angleDelta().y() < 0 ? 1.1 : 0.9));
        camera->view_angle = std::min(1000.0f, camera->view_angle);*/
    }
};