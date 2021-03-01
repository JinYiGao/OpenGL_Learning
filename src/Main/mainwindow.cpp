/*
 * @Descripttion: 
 * @version: 
 * @Author: JinYiGao
 * @Date: 2021-01-30 22:47:17
 * @LastEditors: JinYiGao
 * @LastEditTime: 2021-01-30 23:04:17
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	ui->comboBox->blockSignals(true);
	ui->comboBox->addItem("1:Hello_Triangle");
	ui->comboBox->addItem("2:LearnShader");
	ui->comboBox->addItem("3:Textures");
	ui->comboBox->blockSignals(false);
	ui->comboBox->setCurrentIndex(-1);
}

//下拉框选项改变
void MainWindow::on_comboBox_currentIndexChanged(const int &arg1)
{
	switch (arg1)
	{
	case 0:
		myGLWidget = new MyGLWidget(this);
		myGLWidget->show();
		myGLWidget->move(150, 100);
		myGLWidget->resize(450, 450);
		break;
	case 1:
		learnShaderWidget = new LearnShaderWidget(this);
		learnShaderWidget->show();
		learnShaderWidget->move(150, 100);
		learnShaderWidget->resize(450, 450);
		break;
	case 2:
		texturesWidget = new TexturesWidget(this);
		texturesWidget->show();
		texturesWidget->move(150, 100);
		texturesWidget->resize(450, 450);
		break;
	default:
		break;
	}

}

MainWindow::~MainWindow()
{
    delete ui;
}