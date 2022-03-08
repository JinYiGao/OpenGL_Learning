/*
 * @Descripttion: 
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-01-30 22:47:22
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-01-30 23:09:45
 */
 
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <LearningCollection/myWidget.cpp>
#include <LearningCollection/learnShaderWidget.cpp>
#include <LearningCollection/texturesWidget.cpp>
#include <LearningCollection/transformWidget.cpp>
#include <LearningCollection/threeDWidget.cpp>
#include <LearningCollection/colorWidget.cpp>
#include <LearningCollection/lightingWidget.cpp>
#include <LearningCollection/materialsWidget.cpp>
#include <LearningCollection/lightingMapsWidget.cpp>
#include <LearningCollection/stencilTestingWidget.cpp>
#include <LearningCollection/blendingWidget.cpp>
#include <LearningCollection/framebuffersWidget.cpp>
#include <LearningCollection/cubemapsWidget.cpp>
#include <LearningCollection/uniformBufferWidget.cpp>
#include <LearningCollection/instancingWidget.cpp>

namespace Ui {
class MainWindow;
}
 
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	MyGLWidget *myGLWidget = nullptr;
	LearnShaderWidget *learnShaderWidget = nullptr;
	TexturesWidget *texturesWidget = nullptr;
	TransformWidget *transformWidget = nullptr;
	ThreeDWidget *threeDWidget = nullptr;
	ColorWidget *colorWidget = nullptr;
	LightingWidget *lightingWidget = nullptr;
	MaterialsWidget *materialWidget = nullptr;
	LightingMapsWidget *lightingMapsWidget = nullptr;
	StencilTestingWidget *stencilTestingWidget = nullptr;
	BlendingWidget *blendingWidget = nullptr;
	FramebuffersWidget *framebuffersWidget = nullptr;
	CubeMapsWidget * cubemapsWidget = nullptr;
	UniformBufferWidget *uniformBufferWidget = nullptr;
	InstancingWidget *instancingWidget = nullptr;

private slots:
	void on_comboBox_currentIndexChanged(const int &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
