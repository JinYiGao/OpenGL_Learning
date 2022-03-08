/*
 * @Descripttion: 17 - 帧缓冲学习
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-11 16:55:44
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-11 16:56:29
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
class FramebuffersWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    FramebuffersWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }
	unsigned int cubeVAO, cubeVBO;
	unsigned int planeVAO, planeVBO;
	unsigned int quadVAO, quadVBO;
	unsigned int framebuffer;
	unsigned int rbo;
	Shader *shader, *screenShader;
	Texture *cubeTexture;
	Texture *floorTexture;
	Texture *textureColorbuffer;
    Camera *camera = new Camera();

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();

		//立方体
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
		//平面
		float planeVertices[] = {
			// positions          // texture Coords 
			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

			 5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
			-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
			 5.0f, -0.5f, -5.0f,  2.0f, 2.0f
		};
		float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
			// positions   // texCoords
			-1.0f,  1.0f,  0.0f, 1.0f,
			-1.0f, -1.0f,  0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,

			-1.0f,  1.0f,  0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f, 1.0f
		};

		//cube VAO
		glGenVertexArrays(1, &cubeVAO);
		glBindVertexArray(cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//plane VAO
		glGenVertexArrays(1, &planeVAO);
		glBindVertexArray(planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		//quad VAO
		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
		glEnableVertexAttribArray(1);

		cubeTexture = new Texture(":/LearningCollection/images/container.jpg");
		floorTexture = new Texture(":/LearningCollection/images/metal.png");

		//创建着色器
		shader = new Shader(":/LearningCollection/Framebuffers_shader/framebuffers.vs",
			":/LearningCollection/Framebuffers_shader/framebuffers.fs");
		screenShader = new Shader(":/LearningCollection/Framebuffers_shader/framebuffers_screen.vs",
			":/LearningCollection/Framebuffers_shader/framebuffers_screen.fs");

		//设置着色器纹理变量对应的纹理索引
		shader->setUniformValue("texture1", 0);
		screenShader->setUniformValue("screenTexture", 0);

		//创建帧缓冲对象
		glGenFramebuffers(1, &framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

		//-----------------------创建一个纹理作为颜色附件加到帧缓冲上-----------------------
		textureColorbuffer = new Texture(800, 600);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer->ID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//附加纹理到帧缓冲
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer->ID, 0);

		//------------------------------创建渲染缓冲对象附件--------------------------------
		glGenRenderbuffers(1, &rbo);
		glBindRenderbuffer(GL_RENDERBUFFER, rbo);
		//写入深度模板缓冲
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		//附加到帧缓冲上
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//初始化相机参数
		camera->arcball.setState(Quaternionf::Identity());
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

		//使用帧缓冲 将以下内容全都渲染到该帧缓冲上 ----离屏渲染
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);//启用深度测试

		// 清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader->bind();
		Eigen::Matrix4f transform = camera->getTransform();
		shader->setUniformValue("transform", transform);
		//std::cout << transform * Eigen::Vector4f(5.0f, -0.5f, 5.0f, 1.0f).matrix()<<std::endl <<std::endl;
		//draw cubes
		glBindVertexArray(cubeVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, cubeTexture->ID);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//draw floor
		glBindVertexArray(planeVAO);
		glBindTexture(GL_TEXTURE_2D, floorTexture->ID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glBindVertexArray(0);

		//绑定到屏幕帧缓冲
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFramebufferObject());
		glDisable(GL_DEPTH_TEST); //关闭深度测试
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		screenShader->bind();//切换着色器
		glBindVertexArray(quadVAO);
		glBindTexture(GL_TEXTURE_2D, textureColorbuffer->ID); //在离屏渲染时 已经写入了颜色纹理
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