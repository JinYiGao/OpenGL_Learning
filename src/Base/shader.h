/*
 * @Descripttion: 自己的shader类 封装了 QOpenGLShaderProgram 的一些方法
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-06 16:06:15
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-06 16:06:49
 */
#pragma once
#include <QOpenGLShaderProgram>
#include <Eigen/Eigen>

class Shader
{
public:
	QOpenGLShaderProgram *shader;//着色器程序
	GLuint programId; //着色器程序ID

	Shader(const QString &vertShaderFileName,const QString &fragShaderFilename);

	///绑定着色器
	bool bind();
	///解绑着色器
	void release();

	///setUniform Matrix4x4
	template <typename T>
	void setUniformValue(const char *name, const Eigen::Matrix<T, 4, 4> &mat)
	{
		shader->bind();
		shader->setUniformValue(name, QMatrix4x4(mat.template cast<float>().data(), 4, 4));
	}

	template <typename T>
	void setUniformValue(const char *name, const Eigen::Matrix<T, 3, 1> &vector)
	{
		shader->bind();
		shader->setUniformValue(name, vector.x(), vector.y(), vector.z());
	}

	///setUniform x,y,z
	void setUniformValue(const char *name, GLfloat x, GLfloat y, GLfloat z)
	{
		shader->bind();
		shader->setUniformValue(name, x, y, z);
	}

	///setUniform int float...
	template <typename T>
	void setUniformValue(const char *name, T data)
	{
		shader->bind();
		shader->setUniformValue(name, data);
	}
};