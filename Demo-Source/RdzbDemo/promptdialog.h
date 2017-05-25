#ifndef PROMPTDIALOG_H
#define PROMPTDIALOG_H

#include <QDialog>
#include "ui_promptdialog.h"

class PromptDialog : public QDialog
{
	Q_OBJECT

public:
	PromptDialog(QWidget *parent = 0);
	~PromptDialog();
	int m_SchemaSelectSerialNumber;//��ţ� 0Ϊȡ�� 1Ϊ��������  2Ϊ�˳����� 
	void LangageChange();//�����л�
private:
	Ui::PromptDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//ȷ��
	void on_OK_pushButton_clicked();
	//ȡ��
	void on_Cancel_pushButton_clicked();
};

#endif // PROMPTDIALOG_H
