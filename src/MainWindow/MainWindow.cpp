#include "MainWindow.h"
#include "SerialWatcher.h"

//定义一个串口指针
QSerialPort* COM = new QSerialPort();//这是Qt串口类,用于操作串口,实例化一个串口对象

SerialDebugAssistant::SerialDebugAssistant(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	connect(COM, &QSerialPort::readyRead, this, &SerialDebugAssistant::RX_FUN); //连接信号槽 当有数据接收时触发RX_FUN函数
	//实例化一个串口监视器对象
	m_watcher = new SerialWatcher(this);
	m_watcher->populateComboBox(ui.comboBox_available_COM);
	ui.comboBox_baudRate->setEditable(true);
}

SerialDebugAssistant::~SerialDebugAssistant()
{
}


//按键打开串口 被点击时触发 
void SerialDebugAssistant::on_key_open_COM_clicked()
{
	//串口初始化
	qint32 baudRate = QSerialPort::Baud9600;; //波特率默认9600
	QSerialPort::DataBits dataBits; //数据位
	QSerialPort::Parity checkBits;     //奇偶校验位
	QSerialPort::StopBits stopBits; //停止位

	//设置波特率
	if (ui.comboBox_baudRate->currentText() == "9600") {
		baudRate = QSerialPort::Baud9600;
	}
	else if (ui.comboBox_baudRate->currentText() == "115200") {
		baudRate = QSerialPort::Baud115200;
	}
	else if (ui.comboBox_baudRate->currentText() == "4800") {
		baudRate = QSerialPort::Baud4800;
	}
	else if (ui.comboBox_baudRate->currentText() == "38400") {
		baudRate = QSerialPort::Baud38400;
	}
	else if (ui.comboBox_baudRate->currentText() == "57600") {
		baudRate = QSerialPort::Baud57600;
	}
	else if (ui.comboBox_baudRate->currentText() == "19200") {
		baudRate = QSerialPort::Baud19200;
	}
	else if (ui.comboBox_baudRate->currentText() == "115200") {
		baudRate = QSerialPort::Baud115200;
	}
	else if (ui.comboBox_baudRate->currentText() == "1200") {
		baudRate = QSerialPort::Baud1200;
	}
	else if (ui.comboBox_baudRate->currentText() == "2400") {
		baudRate = QSerialPort::Baud2400;
	}
	else if(baudRate > 110 && baudRate < 4000000){
		baudRate = ui.comboBox_baudRate->currentText().toInt();
	}
	else {
		QMessageBox::warning(this, "错误", "波特率超出范围");
		return;
	}
	
	//设置数据位
	if (ui.comboBox_dataBits->currentText() == "8") {
		dataBits = QSerialPort::Data8;
	}
	else if (ui.comboBox_dataBits->currentText() == "7") {
		dataBits = QSerialPort::Data7;
	}
	else if (ui.comboBox_dataBits->currentText() == "6") {
		dataBits = QSerialPort::Data6;
	}
	else if (ui.comboBox_dataBits->currentText() == "5") {
		dataBits = QSerialPort::Data5;
	}
	else {
		dataBits = QSerialPort::Data8; 
	}

	//设置停止位
	if (ui.comboBox_stopBits->currentText() == "1") {
		stopBits = QSerialPort::OneStop;
	}
	else if (ui.comboBox_stopBits->currentText() == "1.5") {
		stopBits = QSerialPort::OneAndHalfStop;
	}
	else if (ui.comboBox_stopBits->currentText() == "2") {
		stopBits = QSerialPort::TwoStop;
	}
	else {
		stopBits = QSerialPort::OneStop; //默认9600
	}

	//设置奇偶校验位
	if (ui.comboBox_checkBits->currentText() == "无") {
		checkBits = QSerialPort::NoParity;
	}
	else if (ui.comboBox_checkBits->currentText() == "奇校验") {
		checkBits = QSerialPort::OddParity;
	}
	else if (ui.comboBox_checkBits->currentText() == "偶校验") {
		checkBits = QSerialPort::EvenParity;
	}
	else {
		checkBits = QSerialPort::NoParity; //默认无奇偶校验
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
			ui.comboBox_available_COM->setEnabled(false); 
			ui.comboBox_baudRate->setEnabled(false); 
			ui.comboBox_checkBits->setEnabled(false); 
			ui.comboBox_stopBits->setEnabled(false);
			ui.comboBox_dataBits->setEnabled(false);
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
		ui.comboBox_available_COM->setEnabled(true); 
		ui.comboBox_baudRate->setEnabled(true); 
		ui.comboBox_checkBits->setEnabled(true);
		ui.comboBox_stopBits->setEnabled(true);
		ui.comboBox_dataBits->setEnabled(true);
	}
}

//按键发送数据 被点击时触发
void SerialDebugAssistant::on_key_COM_Tx_clicked()
{
	if (!COM->isOpen()) {
		QMessageBox::warning(this, "警告", "请先打开串口！");
		return;
	}

	QString raw = ui.textEdit->toPlainText(); //获取文本框内容
	QByteArray tx;  // 发送数据
	/* ====== 模式+编码选择 ====== */
	if (ui.comboTxMode->currentText() == "文本模式") {
		QString encoding = ui.comboTxEncode->currentText();
		tx = (encoding == "GBK") ? raw.toLocal8Bit()
			: raw.toUtf8();
	}
	else { // HEX
		QString hex = raw.remove(' ');
		if (hex.length() % 2) hex.append('0');   // 补 0
		tx = QByteArray::fromHex(hex.toUtf8());
	}

	COM->write(tx); //发送数据
}

//按键清除发送区域的数据 被点击时触发
void SerialDebugAssistant::on_key_COM_Tx1_clicked()
{
	ui.textEdit->clear(); //清除发送区域的数据
}

//串口接收数据处理函数
void SerialDebugAssistant::RX_FUN()
{
	if (!COM->isOpen()) return;

	QByteArray rx = COM->readAll();
	if (rx.isEmpty()) return;
	
	/* ====== 模式+编码选择 ====== */
	QString text;
	if (ui.comboRxMode->currentText() == "文本模式") {
		QString encoding = ui.comboRxEncode->currentText();
		text = (encoding == "GBK") ? QString::fromLocal8Bit(rx)
			: QString::fromUtf8(rx);
	}
	else { // HEX
		text = rx.toHex(' ').toUpper();
	}

	ui.textBrowser->append(text); //在接收区域显示接收到的数据
}

//按键清除接收区域的数据 被点击时触发
void SerialDebugAssistant::on_key_COM_Tx2_clicked()
{
	ui.textBrowser->clear(); //清除接收区域的数据
}
