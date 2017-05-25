#ifndef QTXTDIALOG_H
#define QTXTDIALOG_H

#include <QDialog>
#include "ui_qtxtdialog.h"

class QTxtDialog : public QDialog
{
	Q_OBJECT

public:
	QTxtDialog(QWidget *parent = 0);
	~QTxtDialog();
	void Init( HCHIP HCip = 0 );
	void LangageChange();//�����л�


private:
	QColor			m_coTextColor;//������ɫ
	HCHIP m_hChip;//Ԫ�����
	Ui::QTxtDialog ui;
	bool winEvent( MSG* msg, long* result );
	bool m_isHM;//�Ƿ��ֶ�
private slots:
	//�ر�
	void on_Close_pushButton_clicked();
	//�����Ļ
	void on_pushButton_clicked();
	//��Ļ�����б仯
	void on_texInput_textChanged();
	//�����޸�
	void on_fontComboBox_currentFontChanged ( const QFont & font );
	//������б
	void on_butTexItalic_clicked( bool checked );
	//����Ӵ�
	void on_butTexBold_clicked( bool checked );
	//����ӿ�
	void on_butTexSizeByWidth_clicked( bool checked );
	//�Ƿ����¹���
	void on_dspScrollCharPerSecond_valueChanged ( double d );
	//������ɫ
	void on_butTexColor_clicked();
};

#endif // QTXTDIALOG_H
