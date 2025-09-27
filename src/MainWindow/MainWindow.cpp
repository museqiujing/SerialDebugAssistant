#include "MainWindow.h"


//定义一个串口指针
QSerialPort* COM = new QSerialPort();//这是Qt串口类,用于操作串口,实例化一个串口对象

SerialDebugAssistant::SerialDebugAssistant(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);


	/*ui.comboBox_available_COM->clear(); //清除下拉框内容
	foreach(const QSerialPortInfo & info, QSerialPortInfo::availablePorts())
	{
		ui.comboBox_available_COM->addItem(info.portName());//将串口名称添加到下拉框中
	}
	*/  //程序启动时自动枚举串口

	connect(COM, &QSerialPort::readyRead, this, &SerialDebugAssistant::RX_FUN); //连接信号槽 当有数据接收时触发RX_FUN函数
}

SerialDebugAssistant::~SerialDebugAssistant()
{
}

/*按键刷新串口 被点击时触发
void SerialDebugAssistant::on_key_Refresh_COM_clicked()    
{
	ui.comboBox_available_COM->clear(); //清除下拉框内容
	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui.comboBox_available_COM->addItem(info.portName());//将串口名称添加到下拉框中
	}
}
*/

//按键打开串口 被点击时触发 
void SerialDebugAssistant::on_key_open_COM_clicked()
{
	//ui.LED1->setStyleSheet("background-color:green");//设置LED1的背景颜色为绿色
	//串口初始化
	QSerialPort::BaudRate baudRate; //波特率
	QSerialPort::DataBits dataBits; //数据位
	QSerialPort::Parity checkBits;     //奇偶校验位
	QSerialPort::StopBits stopBits; //停止位

	stopBits = QSerialPort::OneStop; //停止位1位
	dataBits = QSerialPort::Data8; //数据位8位
	checkBits = QSerialPort::NoParity; //无奇偶校验

	//设置波特率
	if (ui.comboBox_baudRate->currentText() == "9600") {
		baudRate = QSerialPort::Baud9600;
	}
	else if (ui.comboBox_baudRate->currentText() == "115200") {
		baudRate = QSerialPort::Baud115200;
	}
	else {
		baudRate = QSerialPort::Baud9600; //默认9600
	}

	//设置串口参数
	COM->setPortName(ui.comboBox_available_COM->currentText()); //设置串口名
	COM->setBaudRate(baudRate); //设置波特率
	COM->setDataBits(dataBits); //设置数据位
	COM->setParity(checkBits); //设置奇偶校验位
	COM->setStopBits(stopBits); //设置停止位
	
	//串口操作
	if (ui.key_open_COM->text() == "打开串口")
	{
		if(COM->open(QIODevice::ReadWrite)==true) //判断串口打开成功
		{
			ui.key_open_COM->setText("关闭串口"); //按钮显示为关闭串口
			ui.key_open_COM->setStyleSheet("background-color:green");//设置该按钮的背景颜色为绿色
			ui.comboBox_available_COM->setEnabled(false); //下拉框不可用
			ui.comboBox_baudRate->setEnabled(false); //下拉框不可用
		}
		else
		{
			ui.key_open_COM->setStyleSheet("background-color:red");//设置LED1的背景颜色为红色
			QMessageBox::warning(this, "警告", "串口打开失败！或其他错误。\r\n请选择正确的串口或该串口被占用"); //弹出警告对话框
		}
	}
	else if(ui.key_open_COM->text()=="关闭串口")
	{
		COM->close(); //关闭串口
		ui.key_open_COM->setText("打开串口"); //按钮显示为打开串口
		ui.key_open_COM->setStyleSheet("background-color:white");//设置LED1的背景颜色为白色
		ui.comboBox_available_COM->setEnabled(true); //下拉框可用
		ui.comboBox_baudRate->setEnabled(true); //下拉框可用
	}
}

//按键发送数据 被点击时触发
void SerialDebugAssistant::on_key_COM_Tx_clicked()
{
	if (COM->isOpen() == true) //判断串口是否打开
	{
		QString str = ui.textEdit->toPlainText(); //获取文本框内容
		QByteArray TxData = str.toUtf8(); //转换为UTF-8格式的字节数组
		//QByteArray TxData = str.toLocal8Bit(); //转换为本地格式的字节数组
		//QByteArray TxData = str.toLatin1(); //转换为Latin1格式的字节数组 也就是ASCII格式

		COM->write(TxData); //发送数据
	}
	else
	{
		QMessageBox::warning(this, "警告", "请先打开串口！"); //弹出警告对话框
	}
}

//按键清除发送区域的数据 被点击时触发
void SerialDebugAssistant::on_key_COM_Tx1_clicked()
{
	
	ui.textEdit->clear(); //清除发送区域的数据
}

//串口接收数据处理函数
void SerialDebugAssistant::RX_FUN()
{
	if (COM->isOpen() == true) //判断串口是否打开
	{
		QByteArray RxData = COM->readAll(); //读取接收缓冲区的所有数据
		if (!RxData.isEmpty()) {
			QString str = QString::fromUtf8(RxData); //将字节数组转换为字符串
			//QString str = QString::fromLocal8Bit(RxData); //将字节数组转换为字符串
			//QString str = QString::fromLatin1(RxData); //将字节数组转换为字符串
			ui.textBrowser->append(str); //在接收区域显示接收到的数据
		}
	}
	else
	{
		QMessageBox::warning(this, "警告", "请先打开串口！"); //弹出警告对话框
	}
}