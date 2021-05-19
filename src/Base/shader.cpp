/*
 * @Descripttion: 
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-03-06 16:06:20
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-03-06 16:06:20
 */
#include <Base/shader.h>

Shader::Shader(const QString &vertShaderFileName, const QString &fragShaderFilename)
{
	shader = new QOpenGLShaderProgram();
	shader->addShaderFromSourceFile(QOpenGLShader::Vertex, vertShaderFileName);
	shader->addShaderFromSourceFile(QOpenGLShader::Fragment, fragShaderFilename);
	bool success = shader->link();
	if (!success)
	{
		qDebug() << shader->log() << endl;
	}
	else
	{
		programId = shader->programId();
	}
}

bool Shader::bind()
{
	return shader->bind();
}

void Shader::release()
{
	shader->release();
}

//void Shader::setUniformValue(const char *name, GLfloat x, GLfloat y, GLfloat z)
//{
//	shader->setUniformValue(name, x, y, z);
//}
