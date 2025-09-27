#pragma once

#include <QObject>
#include "MainWindow.h"


class SerialProducer : public QObject
{
	Q_OBJECT
public slots:
	void startWatching();          // 线程入口
signals:
	void portListChanged(const QList<QSerialPortInfo>& list);
};


class SerialWatcher  : public QObject
{
	Q_OBJECT

public:
	SerialWatcher(QObject *parent);
	~SerialWatcher();
	void populateComboBox(QComboBox* combo); // 供外部调用

signals:
	void ready();                  // 可连到主界面刷新
private slots:
	void onListChanged(const QList<QSerialPortInfo>& list);
private:
	QThread          m_thread;
	SerialProducer* m_producer;
	QList<QSerialPortInfo> m_lastList;
	QComboBox* m_combo = nullptr;
};

