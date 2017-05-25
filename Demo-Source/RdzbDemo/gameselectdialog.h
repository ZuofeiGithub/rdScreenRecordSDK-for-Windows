#ifndef GAMESELECTDIALOG_H
#define GAMESELECTDIALOG_H

#include <QDialog>
#include "ui_gameselectdialog.h"

class GameSelectDialog : public QDialog
{
	Q_OBJECT

public:
	GameSelectDialog(QWidget *parent = 0);
	~GameSelectDialog();
	void LangageChange();//�����л�
private:
	Ui::GameSelectDialog ui;
	
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
	QDialog * m_pant;//������ָ��
private slots:
	//�б�ѡ��
	void on_listWidget_currentRowChanged( int index );
	//�ر�
	void on_Close_pushButton_clicked();
	//ˢ��
	void on_Refresh_pushButton_clicked();
	//��
	void on_Open_pushButton_clicked();
};

#endif // GAMESELECTDIALOG_H
