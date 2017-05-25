#ifndef ANDROIDSELECTDIALOG_H
#define ANDROIDSELECTDIALOG_H

#include <QDialog>
#include "ui_androidselectdialog.h"

class AndroidSelectDialog : public QDialog
{
	Q_OBJECT

public:
	AndroidSelectDialog(QWidget *parent = 0);
	~AndroidSelectDialog();
	void LangageChange();//�����л�
private:
	Ui::AndroidSelectDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//����ģ����
	void on_Download_pushButton_clicked();
	//��Ӱ�׿ģ����
	void on_DownWindow_pushButton_clicked();
};

#endif // ANDROIDSELECTDIALOG_H
