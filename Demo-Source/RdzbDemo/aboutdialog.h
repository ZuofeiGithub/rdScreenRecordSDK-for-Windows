#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>
#include "ui_aboutdialog.h"

class AboutDialog : public QDialog
{
	Q_OBJECT

public:
	AboutDialog(QWidget *parent = 0);
	~AboutDialog();
	Ui::AboutDialog ui;

	bool m_bSuccess;
	QString m_szErrStr;
private:
	
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//ȷ��
	void on_OK_pushButton_clicked();
};

#endif // ABOUTDIALOG_H
