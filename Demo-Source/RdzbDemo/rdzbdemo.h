#ifndef RDZBDEMO_H
#define RDZBDEMO_H

#include <QtGui/QDialog>
#include <QtGui>
#include "ui_rdzbdemo.h"
#include <Windows.h>
#include "settingdialog.h"
#include "ChipItem.h"
#include "aboutdialog.h"



class RdzbDemo : public QDialog
{
	Q_OBJECT

public:
	struct BitrateStatic
	{
		int iBitrate;
		int iBitrateMinimum;
		int iBitrateMaximum;
	};
	//��������Ӧ�ֱ��ʺ�֡���º��ʵ����ʷ�Χ
	static BitrateStatic GetBitrate( int width, int height, int fps );

	RdzbDemo(QWidget *parent = 0, Qt::WFlags flags = 0);
	~RdzbDemo();
	SettingDialog * m_SettingDialog;//���ô���
	void Restore();//�ָ�
	BOOL			m_bChipListChanging;	//��ǣ���ǰ��Ԫ���б���޸��ǳ����Լ����õģ��������û���������˲���Ҫ���¼���Ӧ�н����κδ���

	//�ؼ���ţ� 0ΪQListWidget  1ΪQComboBox
	void GetWindowsList( void * pwidget, int iControlNumber = 0 );//��ȡ�����б�

	void AddPicture();//��Ӳ�����ʾͼƬ

	QDomDocument	m_docOption;//����������Ϣ
	//���ظ�Ԫ��
	QDomElement FindXmlElement( const QString& szPath, BOOL bCreateIfNull = FALSE );

	QString CombinationFilePath( const QString& szDir, const QString& szPrefix, const QString& szExtension );
	//�ѽ����ϵ����ø��µ������ļ���SDK������
	BOOL SaveProfile( BOOL isSet = FALSE);
	bool m_IsClose;//�Ƿ��˳�
	bool m_bIsClose;//�Ƿ�ر�

	static void setControlText( QWidget * Control, LPWSTR lpAppName,LPWSTR lpKeyName , EControlType controlType );
	void LangageChange();//�����л�
	static IniWOrR& InstanceIniWOrR();

	void SwitchingLanguage();//���Ը����л�

	//ֱ����¼������������ļ���ȡ
	BOOL SettingLive();
	//��Ƶ����������ļ���ȡ
	BOOL SettingVideo();

	//��Ļ�޸�
	void CreateSubtitleModification( HCHIP	hChip = NULL );
private:
	QSystemTrayIcon * m_trayicon;//����
	QMenu * m_trayiconMenu;//�����б�
	bool m_bShow;//
	void OnbtnCloseClicked();//���̵ļ���

	void dragEnterEvent(QDragEnterEvent *event);
	void dropEvent( QDropEvent * event );	

	
protected:
	void closeEvent ( QCloseEvent * event );//�ر��¼�
	//
private:
	void keyPressEvent(QKeyEvent *event);

	void OnAcceditDone( BOOL bSuccess, const QString& szErrStr );//��Ȩ��ʾ
	AboutDialog * m_aboutDialog;//��������Ľ��ܵĽ���
	//���������ļ�
	BOOL LoadProfile();
	//�������ļ����������õ������SDK
	BOOL SettingToUiAndSdk();
	//��������������ļ���ȡ
	BOOL SettingAuido();

	//���볡�������ļ�
	BOOL LoadScenes();
	BOOL SaveScenes();

	Ui::RdzbDemoClass ui;
	bool winEvent( MSG* msg, long* result );//�ޱ߿������ƶ�����
	void changeEvent(QEvent* e);//��С�� ��ԭ
	QMenu	m_menu;//�˵�

	int m_UiWidth;//ȫ������
	int m_ComponentWidth;//�б���
	IGlRender_SPreviewLayout m_sPerviewLayout;	//Ԥ������

	//
	void UpdatePreviewConfig( IGlRender_SPreviewLayout::EReposWnd eMode );//�������úͼ���Ԥ����������Ľṹ
	static VOID WINAPI RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam );//��Ⱦ�޸���ʾ�ص�
	static VOID WINAPI EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam );//���������޸���ʾ�ص�
	private slots:
		//�����л�
		void on_pushButton_clicked();
		//���̲˵�����
		void on_UI_clicked();//����������
		void onSystemTrayIconClicked( QSystemTrayIcon::ActivationReason reason );//�������������
		void SelfClose();//�����˳�
		//ֱֹͣ��
		void on_Live_Quit_pushButton_clicked();
		//��ʼֱ��
		void on_Live_Begin_pushButton_clicked();
		//Ԫ���б� ѡ���źŲ�
		void on_listWidget_currentItemChanged( QListWidgetItem * current, QListWidgetItem * previous );
		void on_listWidget_currentRowChanged( int index );
		//����������
		void on_Speaker_pushButton_clicked();
		void on_Checke_Speaker_pushButton_clicked( bool checked );
		void on_Speaker_verticalSlider_valueChanged( int value );
		//��˷�����
		void on_MIC_pushButton_clicked();
		void on_Checke_MIC_pushButton_clicked( bool checked );
		void on_MIC_verticalSlider_valueChanged( int value );
		//����
		void on_Setting_pushButton_clicked();
		//�ر�
		void on_Close_pushButton_clicked();
		//��С��
		void on_Min_pushButton_clicked();
		//���Ͻ�����
		void on_Operate_pushButton_clicked();
		//�б�����
		void on_Search_pushButton_clicked( bool checked );
		//����ֱ��
		void on_Mobile_Game_pushButton_clicked();
		void on_Android_pushButton_select_clicked();//��׿ֱ��
		void on_Android_label_clicked();//��׿ֱ��label

		void on_IOS_pushButton_select_clicked();//IOSֱ��
		void on_IOS_label_clicked();//IOSֱ��label

		//����ֱ��
		void on_Other_pushButton_clicked();//����ѡ����濪��

		void on_Video_pushButton_select_clicked();//��Ƶ�ļ�
		void on_Video_label_clicked();//��Ƶ�ļ�label

		void on_picture_pushButton_select_clicked();//ͼƬ
		void on_picture_label_clicked();//ͼƬlabel

		void on_flash_pushButton_select_clicked();//flash�ļ�
		void on_flash_label_clicked();//flash�ļ�label

		void on_RTMP_pushButton_select_clicked();//rtmpֱ��
		void on_RTMP_label_clicked();//rtmpֱ��label	

		void on_captions_pushButton_select_clicked();//��Ļ���
		void on_captions_label_clicked();//��Ļ���label

		//PCֱ��
		void on_PC_Live_pushButton_clicked();
		void on_Game_pushButton_select_clicked();//��Ϸֱ��
		void on_Game_label_clicked();//��Ϸֱ��label

		void on_Display_pushButton_select_clicked();//��ʾ��ֱ��
		void on_Display_label_clicked();//��ʾ��ֱ��label

		void on_Windows_pushButton_select_clicked();//����ֱ��
		void on_Windows_label_clicked();//����ֱ��label

		void on_Camera_pushButton_select_clicked();//����ͷֱ��
		void on_Camera_label_clicked();//����ͷֱ��label

		void on_signalsRenerNotifyCB( int eNotify, int hScene, int iValue );//��Ⱦ�ص��źŲ�
		void on_signalsEncoderNotifyCB( int eNotify, ulong dwValue, ulong ptrValue  );//����ص��źŲ�
signals:
		void signalsRenderNotifyCB( int eNotify, int hScene, int iValue );//��Ⱦ�ص��źŴ���
		void signalsEncoderNotifyCB( int eNotify, ulong dwValue, ulong ptrValue  );//����ص��źŴ���
};

#endif // RDZBDEMO_H
