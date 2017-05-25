#ifndef OPENDIALOG_H
#define OPENDIALOG_H

#include <QDialog>
#include "ui_opendialog.h"

class OpenDialog : public QDialog
{
	Q_OBJECT

public:
	OpenDialog(QWidget *parent = 0);
	~OpenDialog();
	void SetTitleText( QString Text );//���ñ�����
	enum OpenFileFmt
	{
		OPFilefmtMove, //��Ƶ�ļ� ��ʽ��mp4,m4v,flv,f4v,3gpp
		OPFilefmtPicture, //ͼƬ�ļ� ��ʽ��jpg,png,bmp
		OPFilefmtFlash, //flsh�ļ� ��ʽ��
		OPFilefmtRtmp //rtmp��ʽ
	};

	void SetModel( OpenDialog::OpenFileFmt  openFileFmt );//���ý���򿪵�����
	void LangageChange();//�����л�
private:
	Ui::OpenDialog ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
	OpenFileFmt m_OpenFileFmt;//��Ҫ�򿪵��ļ�
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//���
	void on_Browse_pushButton_clicked();
	//ȷ��
	void on_OK_pushButton_clicked();
};

#endif // OPENDIALOG_H
