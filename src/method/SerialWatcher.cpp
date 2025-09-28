#include "SerialWatcher.h"



// 子线程入口
SerialWatcher::SerialWatcher(QObject* parent)
	: QObject(parent), m_productor(new SerialProductor)
{
    m_productor->moveToThread(&m_thread);
	connect(&m_thread, &QThread::finished, m_productor, &QObject::deleteLater);    // 线程结束时删除对象
	connect(&m_thread, &QThread::started, m_productor, &SerialProductor::startWatching);  // 线程启动时调用入口
    connect(this, &SerialWatcher::ready,
        this, &SerialWatcher::onListChanged,
		Qt::QueuedConnection);  // 跨线程连接
    bool ok = connect(m_productor, &SerialProductor::portListChanged,
        this, &SerialWatcher::onListChanged,
        Qt::QueuedConnection);
    qDebug() << "connect portListChanged -> onListChanged:" << ok;

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
    qDebug() << "onListChanged received in thread:" << QThread::currentThread()
        << "combo=" << m_combo;
    if (!m_combo) return;
    QString curr = m_combo->currentText();
    m_combo->clear();
    for (const auto& info : list)
        m_combo->addItem(info.portName());
    int idx = m_combo->findText(curr);
    if (idx != -1) m_combo->setCurrentIndex(idx);
}






SerialProductor::SerialProductor(QObject* parent)
    : QObject(parent)
{
	m_lastList = QSerialPortInfo::availablePorts();
}

// 子线程函数，定时轮询串口列表
void SerialProductor::startWatching()
{
    qDebug() << "SerialProductor thread started:" << QThread::currentThread();
    emit portListChanged(QSerialPortInfo::availablePorts());
    m_lastList = QSerialPortInfo::availablePorts();

    // 创建定时器，周期轮询
    QTimer* timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, [this]() {
        QList<QSerialPortInfo> now = QSerialPortInfo::availablePorts();
        qDebug() << "polling: last=" << m_lastList.size() << "now=" << now.size();

        // 大小不同或内容不同 → 更新
        if (now.size() != m_lastList.size() ||
            !std::equal(now.begin(), now.end(), m_lastList.begin(),
                [](const QSerialPortInfo& a, const QSerialPortInfo& b)
                { return a.portName() == b.portName() &&
                a.systemLocation() == b.systemLocation(); }))
        {
            // 不相等 → 更新
            qDebug() << "emitting portListChanged with count=" << now.size();
            m_lastList = now;
            emit portListChanged(now);
        }
        });
    timer->start(1000);
}


SerialProductor::~SerialProductor()
{

}










