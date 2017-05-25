#ifndef IOSSELECTDIALOG_H
#define IOSSELECTDIALOG_H

#include <QDialog>
#include "ui_iosselectdialog.h"

class IOSSelectDialog : public QDialog
{
	Q_OBJECT

public:
	IOSSelectDialog(QWidget *parent = 0);
	~IOSSelectDialog();
	void LangageChange();//�����л�
private:
	Ui::IOSSelectDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//����Airplay
	void on_IOS_pushButton_clicked();
	//���Airplay
	void on_Download_pushButton_clicked();
};

#endif // IOSSELECTDIALOG_H
