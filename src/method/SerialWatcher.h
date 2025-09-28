#pragma once

#include <QObject>
#include <qcombobox.h>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <QThread>
#include <QList>
#include <QTimer>
#include <QDebug>

class SerialProductor;

class SerialWatcher  : public QObject
{
	Q_OBJECT

public:
	explicit SerialWatcher(QObject* parent = nullptr);
	~SerialWatcher();
	void populateComboBox(QComboBox* combo); // 初始化时填充一次串口列表
private slots:
	void onListChanged(const QList<QSerialPortInfo>& list);
signals:
	void ready(const QList<QSerialPortInfo>& list); // 立即刷一次
private:
	QThread          m_thread;
	SerialProductor* m_productor;
	QComboBox* m_combo = nullptr;
};


class SerialProductor : public QObject
{
	Q_OBJECT
public:
	SerialProductor(QObject* parent = nullptr);
	~SerialProductor();

public slots:
	void startWatching();          // 子线程入口

signals:
	void portListChanged(const QList<QSerialPortInfo>& list); // 通知主线程串口列表变化

private:
	QList<QSerialPortInfo> m_lastList;
};
