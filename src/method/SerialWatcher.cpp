#include "SerialWatcher.h"



// 子线程入口
SerialWatcher::SerialWatcher(QObject* parent)
	: QObject(parent), m_productor(new SerialProductor)
{
    m_productor->moveToThread(&m_thread);
    connect(&m_thread, &QThread::started, m_productor, &SerialProductor::startWatching);  // 线程启动时调用入口
	connect(&m_thread, &QThread::finished, m_productor, &QObject::deleteLater);    // 线程结束时删除对象
    connect(this, &SerialWatcher::ready,this, &SerialWatcher::onListChanged,Qt::QueuedConnection);  // 跨线程连接
    bool ok = connect(m_productor, &SerialProductor::portListChanged,this, &SerialWatcher::onListChanged,Qt::QueuedConnection);
    m_thread.start();
}

// 析构时退出线程
SerialWatcher::~SerialWatcher()
{
    m_thread.quit();
    m_thread.wait();
}

// 初始化时填充一次串口列表
void SerialWatcher::populateComboBox(QComboBox* combo)
{
    m_combo = combo;
    connect(this, &SerialWatcher::ready, this, &SerialWatcher::onListChanged);
    emit ready(QSerialPortInfo::availablePorts()); // 立即刷一次
}

// 主线程槽函数，更新串口列表
void SerialWatcher::onListChanged(const QList<QSerialPortInfo>& list)
{
    if (!m_combo) return;
	QString curr = m_combo->currentText();   // 记录当前选择
	m_combo->clear();                       // 清空
	for (const auto& info : list)           // 重新填充
		m_combo->addItem(info.portName());  // 这里只显示端口名
	int idx = m_combo->findText(curr);      // 尝试恢复选择
	if (idx != -1) m_combo->setCurrentIndex(idx);  // 找到则恢复
}


SerialProductor::SerialProductor(QObject* parent)
    : QObject(parent)
{
	m_lastList = QSerialPortInfo::availablePorts();
}

// 子线程函数，定时轮询串口列表
void SerialProductor::startWatching()
{
	emit portListChanged(QSerialPortInfo::availablePorts());  // 线程启动时先发一次
	m_lastList = QSerialPortInfo::availablePorts();   // 记录初始列表

    // 创建定时器，周期轮询
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        QList<QSerialPortInfo> now = QSerialPortInfo::availablePorts();
        // 大小不同或内容不同 → 更新
        if (now.size() != m_lastList.size() ||
            !std::equal(now.begin(), now.end(), m_lastList.begin(),
                [](const QSerialPortInfo& a, const QSerialPortInfo& b)
                { return a.portName() == b.portName() &&
                a.systemLocation() == b.systemLocation(); }))
        {
            // 不相等 → 更新
            m_lastList = now;
            emit portListChanged(now);
        }
        });
    timer->start(1000);
}


SerialProductor::~SerialProductor()
{

}










