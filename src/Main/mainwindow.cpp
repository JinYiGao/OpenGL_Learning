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
	ui->comboBox->addItem("4:Transform");
	ui->comboBox->addItem("5:ThreeD");
	ui->comboBox->addItem("6:Color");
	ui->comboBox->addItem("7:Lighting");
	ui->comboBox->addItem("8:Materials");
	ui->comboBox->addItem("9:Lighting Maps");
	ui->comboBox->addItem("10:Light Casters");
	ui->comboBox->addItem("11:Multiple Lights");
	ui->comboBox->addItem("12:Model");
	ui->comboBox->addItem("13:Depth Testing");
	ui->comboBox->addItem("14:Stencil Testing");
	ui->comboBox->addItem("15:Blending");
	ui->comboBox->addItem("16:Face culling");
	ui->comboBox->addItem("17:Framebuffers");
	ui->comboBox->addItem("18:CubeMaps");
	ui->comboBox->addItem("19:UniformBuffer");
	ui->comboBox->addItem("20:Instancing");

	ui->comboBox->blockSignals(false);
	ui->comboBox->setMaxVisibleItems(20);
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
		myGLWidget->resize(800, 600);
		break;
	case 1:
		learnShaderWidget = new LearnShaderWidget(this);
		learnShaderWidget->show();
		learnShaderWidget->move(150, 100);
		learnShaderWidget->resize(800, 600);
		break;
	case 2:
		texturesWidget = new TexturesWidget(this);
		texturesWidget->show();
		texturesWidget->move(150, 100);
		texturesWidget->resize(800, 600);
		break;
	case 3:
		transformWidget = new TransformWidget(this);
		transformWidget->show();
		transformWidget->move(150, 100);
		transformWidget->resize(800, 600);
		break;
	case 4:
		threeDWidget = new ThreeDWidget(this);
		threeDWidget->show();
		threeDWidget->move(150, 100);
		threeDWidget->resize(800, 600);
		break;
	case 5:
		colorWidget = new ColorWidget(this);
		colorWidget->show();
		colorWidget->move(150, 100);
		colorWidget->resize(800, 600);
		break;
	case 6:
		lightingWidget = new LightingWidget(this);
		lightingWidget->show();
		lightingWidget->move(150, 100);
		lightingWidget->resize(800, 600);
		break;
	case 7:
		materialWidget = new MaterialsWidget(this);
		materialWidget->show();
		materialWidget->move(150, 100);
		materialWidget->resize(800, 600);
		break;
	case 8:
		lightingMapsWidget = new LightingMapsWidget(this);
		lightingMapsWidget->show();
		lightingMapsWidget->move(150, 100);
		lightingMapsWidget->resize(800, 600);
		break;
	case 9: 
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("此功能尚未实现!"));
		break;
	case 10:
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("此功能尚未实现!"));
		break;
	case 11:
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("此功能尚未实现!"));
		break;
	case 12:
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("深度测试:本章节内容重在理解,看笔记就行..."));
		break;
	case 13:
		stencilTestingWidget = new StencilTestingWidget(this);
		stencilTestingWidget->show();
		stencilTestingWidget->move(150, 100);
		stencilTestingWidget->resize(800, 600);
		break;
	case 14:
		blendingWidget = new BlendingWidget(this);
		blendingWidget->show();
		blendingWidget->move(150, 100);
		blendingWidget->resize(800, 600);
		break;
	case 15:
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QString::fromLocal8Bit("面剔除:本章节内容重在理解,看笔记就行..."));
		break;
	case 16:
		framebuffersWidget = new FramebuffersWidget(this);
		framebuffersWidget->show();
		framebuffersWidget->move(150, 100);
		framebuffersWidget->resize(800, 600);
		break;
	case 17:
		cubemapsWidget = new CubeMapsWidget(this);
		cubemapsWidget->show();
		cubemapsWidget->move(150, 100);
		cubemapsWidget->resize(800, 600);
		break;
	case 18:
		uniformBufferWidget = new UniformBufferWidget(this);
		uniformBufferWidget->show();
		uniformBufferWidget->move(150, 100);
		uniformBufferWidget->resize(800, 600);
		break;
	case 19:
		instancingWidget = new InstancingWidget(this);
		instancingWidget->show();
		instancingWidget->move(150, 100);
		instancingWidget->resize(800, 600);
		break;
	default:
		break;
	}
}

MainWindow::~MainWindow()
{
    delete ui;
}