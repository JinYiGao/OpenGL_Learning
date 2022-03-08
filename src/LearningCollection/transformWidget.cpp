/*
 * @Descripttion: 4-初步涉及矩阵操作---对物体进行旋转和平移
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-01 20:54:04
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-02 20:33:04
 */
#pragma once
#include <iostream>

#include <QOpenGLWidget>
#include <QOpenGlFunctions_4_5_Core>
#include <QOpenGLShaderProgram>
#include <QTime>
#include <QImage>

#include <Eigen/Eigen>
//#include <Base/stb_image.h>
#include <Base/base.h>
#include <Base/common.h>

//继承自QOpenGLFunctions可以避免每次调用opengl函数时使用前缀
class TransformWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_5_Core
{
public:
    TransformWidget(QWidget *parent) : QOpenGLWidget(parent)
    {
    }

	GLuint VAO;
	QOpenGLShaderProgram *shaderProgram;
	GLuint texture, texture1;
	Eigen::Translation3f translation;//平移
	Eigen::AngleAxisf rotation;//旋转
	//Eigen::Matrix4f trans;//模型变换矩阵
	Eigen::Isometry3f trans;//欧式变换矩阵
	int i = 0;

protected:
    void initializeGL()
    {
        initializeOpenGLFunctions();
        //Eigen库学习使用
		printf("--------Eigen库学习使用--------\n");
		//直接赋值
        Eigen::Matrix2i m1;
        m1 << 1, 2,
            3, 4;
		std::cout << m1 << std::endl << std::endl;

		//动态矩阵
        Eigen::Matrix2i m2;
		//m2.resize(2, 2);
		m2(0, 0) = 2;
		m2(0, 1) = 3;
		m2(1, 0) = 4;
		m2(1, 1) = 5;
        /*m2 << 1, 2,
            3, 4;*/
		std::cout << m2 << std::endl << std::endl;

        Eigen::Matrix2i m;
        m = m1 + m2;
        std::cout << m << std::endl;

		Eigen::Vector2i vec;
		vec << 1, 2;
		std::cout << vec << std::endl;

		Eigen::Vector2i pos;
		pos = m * vec;
		std::cout << pos << std::endl;

		Eigen::Matrix3f f1;
		f1 << 1, 2, 3,
			4, 5, 6,
			7, 8, 9;
		Eigen::Matrix3f f2;
		f2 << 11, 12, 13,
			14, 15, 16,
			17, 18, 19;
		Eigen::MatrixXf f3;
		f3.resize(6, 3);
		f3 << f1,
			f2;
		std::cout << "f3: " << f3 << std::endl;
		auto pt = f3.data();
		std::cout << bool(pt) << std::endl;
		int i = 0;
		while (i < 18) {
			std::cout << "next:" << *pt << unsigned(uint8_t(100)) << std::endl;
			pt = pt + 1;
			i++;
		}

		Eigen::MatrixXi_8 i1;
		i1.resize(3, 4);
		i1 << uint8_t(100), uint8_t(10), uint8_t(200), uint8_t(0),
			uint8_t(30), uint8_t(44), uint8_t(12), uint8_t(0),
			uint8_t(100), uint8_t(80), uint8_t(15), uint8_t(1);
		std::cout << "i1(uint8): " << i1.rows() << std::endl;
		for (int i = 0; i < 12; i++) {
			std::cout << "i1(uint8): " << unsigned(*(i1.data() + uint8_t(i))) << " rows:" << i1.rows() << std::endl;
		}

		Eigen::MatrixXf i2f;
		i2f.resize(1, 3);
		memcpy(i2f.data(), i1.data(), 12 * sizeof(uint8_t));

		/*i1.template cast<float>();
		i1.resize(1, 3);
		std::cout << "i1(float): " << i1 << "rows:" << i1.rows() << std::endl;*/

		Eigen::MatrixXi_8 f2i8;
		f2i8.resize(3, 4);
		memcpy(f2i8.data(), i2f.data(), 3 * sizeof(float));
		for (int i = 0; i < 12; i++) {
			std::cout << "i1(int): " << unsigned(*(f2i8.data() + uint8_t(i))) << " rows:" << f2i8.rows() << std::endl;
		}
		std::cout << "sizeof(f2i8): " << f2i8.size() << std::endl;
		std::cout << "sizeof(i1): " << f3.size() << std::endl;

		int8_t *a = new int8_t[4]{ '1','2','3','4' };
		Eigen::Matrix<int8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor> mA = Eigen::Map<Eigen::Matrix<int8_t, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(a, 2, 2);
		std::cout << mA << "size: "<< sizeof(mA) << std::endl;
		printf("-------我是一条分割线-------\n");

		glEnable(GL_DEPTH_TEST);
		float vertices[] = {
			//		 ----位置----       ----颜色---- - 纹理坐标 -
			0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
			-0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // 左上
		};

		//顶点索引指定
		unsigned int indices[] = {
			0, 1, 3, // first triangle
			1, 2, 3  // second triangle
		};

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		//设置数据解析
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		//加载纹理图像数据
		//int width, height, nrChannels;
		//unsigned char *data = stbi_load("D:/Study/Programming/OpenGL_Learning/LearningCollection/src/LearningCollection/images/container.jpg", &width, &height, &nrChannels, 0);
		////std::cout << *data << std::endl << std::endl;
		//QImage img(":/LearningCollection/images/container.jpg");
		//std::cout << img.format() << std::endl;
		//width = img.width();
		//height = img.height();
		////img = img.mirrored();
		//img = img.convertToFormat(QImage::Format_RGB888);
		int width, height,format;
		unsigned char *data = Base::load_img(":/LearningCollection/images/container.jpg", width, height,format);
		//---------------Texture1------------------;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		//设置纹理环绕 过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); //自动生成多层渐进纹理
		width = 0, height = 0;
		//---------------Texture2------------------
		//data = stbi_load("D:/Study/Programming/OpenGL_Learning/LearningCollection/src/LearningCollection/images/awesomeface.png", &width, &height, &nrChannels, 0);
		//QImage img2(":/LearningCollection/images/awesomeface.png");
		//std::cout << img2.format() << std::endl;
		//width = img2.width();
		//height = img2.height();
		////QImage img3 = img2.mirrored();
		//img2 = img2.convertToFormat(QImage::Format_RGBA8888);
		unsigned char *data1 = Base::load_img(":/LearningCollection/images/awesomeface.png", width, height,format);
		//unsigned char *data1 = Base::load_img(":/LearningCollection/images/container2.png", width, height);
		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);
		//设置纹理环绕 过滤方式
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data1);
		glGenerateMipmap(GL_TEXTURE_2D); //自动生成多层渐进纹理

		//创建着色器程序
		shaderProgram = new QOpenGLShaderProgram();
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/LearningCollection/Transform_shader/vert.vs");
		shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/LearningCollection/Transform_shader/frag.fs");
		bool success = shaderProgram->link();
		if (success)
		{
			std::cout << "Shader Link Success!" << std::endl;
		}
		shaderProgram->bind();

		//告诉着色器纹理变量对应的纹理单元
		shaderProgram->setUniformValue("Texture1", 0);
		shaderProgram->setUniformValue("Texture2", 1);

		//设置转换矩阵
		trans = Eigen::Matrix4f::Identity(4, 4);//单位阵
		translation = Eigen::Translation3f(0.0001, 0, 0);
    }

    void resizeGL(int w, int h)
    {
        //用于更新投影矩阵或者其他相关大小设置
    }

    void paintGL()
    {
		update();
		// 清除颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//使用两个纹理
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		//std::cout << translation.vector() << std::endl;
		int timeValue = QTime::currentTime().second();
		
		rotation = Eigen::AngleAxisf(3.1415926 / 180 * 10, Eigen::Vector3f::UnitZ());//设置旋转角 旋转轴
		//trans = (rotation*translation).matrix();
		//每隔一定时间 旋转一次
		if (i % 10 == 0)
		{
			trans.rotate(rotation);
		}
		trans.pretranslate(translation.vector());//平移是在旋转之前的坐标系下进行的
		//std::cout << trans.matrix() << std::endl;
		Eigen::Matrix4f transform = trans.matrix();

		//设置uniform transform
		shaderProgram->setUniformValue("transform", QMatrix4x4(transform.template cast<float>().data(), 4, 4));

		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		i++;
    }
};