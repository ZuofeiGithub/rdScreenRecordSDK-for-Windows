#ifndef WINDOWSSELECTDIALOG_H
#define WINDOWSSELECTDIALOG_H

#include <QDialog>
#include "ui_windowsselectdialog.h"

class WindowsSelectDialog : public QDialog
{
	Q_OBJECT

public:
	WindowsSelectDialog(QWidget *parent = 0);
	~WindowsSelectDialog();
	QRadioButton * CreateRadioButton( QString & name );
	void LangageChange();//�����л�
private:
	Ui::WindowsSelectDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
	QDialog * m_pant;//������
private slots:
	//�б�ѡ��
	void on_listWidget_currentRowChanged( int index );
	//�ر�
	void on_Close_pushButton_clicked();
	//ȡ��
	void on_Cancel_pushButton_clicked();
	//ˢ��
	void on_Refresh_pushButton_clicked();
	//ȷ��
	void on_OK_pushButton_clicked();
};

#endif // WINDOWSSELECTDIALOG_H
