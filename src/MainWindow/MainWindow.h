#pragma once 

#include <QtWidgets/QWidget> 
#include "ui_MainWindow.h"   
#include <QSerialPort> //这是Qt串口类
#include <QSerialPortInfo> //这是Qt串口信息类
#include<QMessageBox> //包含消息框头文件
#include <QThread>   //包含线程头文件
#include <QTimer>    //包含定时器头文件
#include <QList>     //包含列表头文件

class SerialDebugAssistant : public QWidget
{
    Q_OBJECT

public:
    SerialDebugAssistant(QWidget *parent = nullptr);
    ~SerialDebugAssistant();

	//私有的槽函数
private slots:
	/*void on_key_Refresh_COM_clicked();  //按键刷新串口*/
	void on_key_open_COM_clicked(); //按键打开串口 
	void on_key_COM_Tx_clicked(); //按键发送数据
	void on_key_COM_Tx1_clicked(); //按键清除发送区域的数据
	void RX_FUN(); //串口接收数据处理函数
private:
	Ui::SerialDebugAssistantClass ui;
};

