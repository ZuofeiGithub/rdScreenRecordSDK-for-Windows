#ifndef SCREENSELECTDIALOG_H
#define SCREENSELECTDIALOG_H

#include <QDialog>
#include "ui_screenselectdialog.h"

class ScreenSelectDialog : public QDialog
{
	Q_OBJECT

public:
	ScreenSelectDialog(QWidget *parent = 0);
	~ScreenSelectDialog();

	void Refresh_Screen();
	void LangageChange();//�����л�
private:
	Ui::ScreenSelectDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//ȡ��
	void on_Cancel_pushButton_clicked();
	//ȷ��
	void on_OK_pushButton_clicked();
	//ˢ��
	void on_Refresh_pushButton_clicked();
};

#endif // SCREENSELECTDIALOG_H
