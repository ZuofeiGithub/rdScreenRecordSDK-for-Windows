#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>
#include "ui_settingdialog.h"


class SettingDialog : public QDialog
{
	Q_OBJECT

public:
	Ui::SettingDialog ui;

	SettingDialog(QWidget *parent = 0);
	~SettingDialog();
	QDialog * m_pant;//������ָ��

	void SetBitrate();//���ʱ仯����
	void Refresh_Auido();//�����豸ˢ��

	bool CheckMovieSaveDir( BOOL bPopMsg = FALSE );//��Ƶ����Ŀ¼���
	void LangageChange();//�����л�
	void SetLangageList();//

	bool Save( bool isSurface = false );//����
private:
	void keyPressEvent(QKeyEvent *event); //����
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
private slots:
	//����
	void on_Langage_pushButton_clicked();
	//�ر�
	void on_Close_pushButton_clicked();
	//ȡ��
	void on_Cancel_pushButton_clicked();
	//ȷ��
	void on_OK_pushButton_clicked();
	//��Ƶ
	void on_Video_pushButton_clicked();
	//����
	void on_Auido_pushButton_clicked();
	//ֱ������
	void on_Live_pushButton_clicked();
	//�ֱ���
	void on_Resolution_comboBox_currentIndexChanged( int index );
	//֡��
	void on_FPS_comboBox_currentIndexChanged( int index );
	//���
	void on_browse_pushButton_clicked();
	//�鿴
	void on_View_pushButton_clicked();
	//���¼��
	void on_Win7_MOUSE_checkBox_clicked(  bool checked  );
	//Aero��Ч�ر�
	void on_Win7_Aero_checkBox_clicked(  bool checked  );
	//����
	void on_Language_comboBox_currentIndexChanged( int index );

	//��Ƶ�ļ��б���
	void on_File_checkBox_clicked(  bool checked  );
	//Rtmp
	void on_Rtmp_checkBox_clicked(  bool checked  );
};

#endif // SETTINGDIALOG_H
