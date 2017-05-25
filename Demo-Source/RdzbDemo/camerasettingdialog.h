#ifndef CAMERASETTINGDIALOG_H
#define CAMERASETTINGDIALOG_H

#include <QDialog>
#include "ui_camerasettingdialog.h"

class CameraSettingDialog : public QDialog
{
	Q_OBJECT

public:
	CameraSettingDialog(QWidget *parent = 0);
	~CameraSettingDialog();

	void CreateComboBox();
	void LangageChange();//�����л�
private:
	Ui::CameraSettingDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
	private slots:
		//�ر�
		void on_Close_pushButton_clicked();
		//ȡ��
		void on_Cancel_pushButton_clicked();
		//ˢ��
		void on_Refresh_pushButton_clicked();
		//ȷ��
		void on_OK_pushButton_clicked();
};

#endif // CAMERASETTINGDIALOG_H
