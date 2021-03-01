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

#include <LearningCollection/myWidget.cpp>
#include <LearningCollection/learnShaderWidget.cpp>
#include <LearningCollection/texturesWidget.cpp>
#include <LearningCollection/transformWidget.cpp>

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

private slots:
	void on_comboBox_currentIndexChanged(const int &arg1);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
