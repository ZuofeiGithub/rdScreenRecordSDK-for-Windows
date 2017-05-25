#include "stdafx.h"
#include "rdzbdemo.h"
#include "promptdialog.h"


#include "androidselectdialog.h"
#include "screenselectdialog.h"
#include "iosselectdialog.h"
#include "gameselectdialog.h"
#include "camerasettingdialog.h"
#include "windowsselectdialog.h"


#include "ChipItem.h"
#include "myradiobutton.h"
#include"opendialog.h"
#include "qtxtdialog.h"
#include "myqradiobutton.h"
RdzbDemo::RdzbDemo(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	m_IsClose = FALSE;
	ui.setupUi(this);

	ui.pushButton->hide();
	//����
	//ui.pushButton->show();

	ui.Speaker_widget->hide();
	ui.MIC_widget->hide();
	ui.Live_Quit_pushButton->hide();
	ui.Add_widget->hide();
	m_UiWidth = width();
	m_ComponentWidth = ui.Component_widget->width();
	ui.listWidget->installEventFilter(ui.listWidget);
	ui.listWidget->setAcceptDrops(true);
	m_bIsClose = false;
	m_bChipListChanging	= FALSE;
	m_trayicon = NULL;
	m_aboutDialog = new AboutDialog(this);
	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint/*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground,true); 
	m_bShow = FALSE;
	//�����ʼ��
	//�ؼ���������

	ui.Preview_widget->SetParent( this );

	m_SettingDialog = new SettingDialog(this);
	m_SettingDialog->m_pant = this;
	
	//�ص�����
	connect( this, SIGNAL(signalsRenderNotifyCB( int, int, int )), this, SLOT(on_signalsRenerNotifyCB( int, int, int )) );
	connect( this, SIGNAL(signalsEncoderNotifyCB( int, ulong, ulong )), this, SLOT(on_signalsEncoderNotifyCB( int, ulong, ulong )) );

	//���Եĳ�ʼ��
	QString strPath( QCoreApplication::applicationDirPath () );
	strPath.replace(QRegExp("/"),"\\");
	strPath.append( "\\language\\language.ini" );
	RdzbDemo::InstanceIniWOrR().CLanguagePath( (LPWSTR)strPath.utf16() );
	QString strLanguageName( QFU(RdzbDemo::InstanceIniWOrR().GetLanguagePrivateProfileString( L"language-default", L"displayname" )) );
	QString strLanguageFileName(  QFU(RdzbDemo::InstanceIniWOrR().GetLanguagePrivateProfileString( L"language-default", L"filename" )) );
	strPath.clear();
	strPath.append( QCoreApplication::applicationDirPath () );
	strPath.replace(QRegExp("/"),"\\");
	strPath.append( "\\language\\" );
	strPath.append( strLanguageFileName );
	RdzbDemo::InstanceIniWOrR().CCurrentLanguagePath( (LPWSTR)strPath.utf16() );
	m_SettingDialog->SetLangageList();
	m_SettingDialog->LangageChange();

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	//��̨��ʼ��
	if ( !RDLive_Init( L"17RD", L"RdzbDemo",RenderNotifyCB, EncoderNotifyCB,this  ) )
	{
		QMessageBox::warning(this,
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PromptInit")),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PromptInitError")),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleModification",L"Acknowledgement")) );
		qApp->quit();
	}
	//�����Ȩ ��Ҫ��дappkey��appsecret���ܳɹ�����
	//appkey��appsecret�����빫˾��̨����Ի��
	if ( !RDLive_ResetAccredit( "xxxxxxxxxxxxxxxxx", 
		"xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" ) )
	{
		QMessageBox::warning(this,
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PromptSignature")),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PromptSignatureError")),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleModification",L"Acknowledgement")) );
		qApp->quit();
	}
	
	//��Ϸ¼��exe����
	Game_InitRecord(FALSE);
	//��ӳ���
	HSCENE		hScene	= Render_CreateScene();
	Scene_SetName( hScene, (LPCWSTR)QFU(L"scene").utf16() );
	//��ȡ������Ϣ
	LoadProfile();
	UpdatePreviewConfig(IGlRender_SPreviewLayout::eNotChangePos);
	//�����������
	LoadScenes();

	//LangageChange();
}	

void RdzbDemo::LangageChange()
{
	//�����˵��б����
	m_menu.clear();
	m_menu.addAction(QFU((LPWSTR)RdzbDemo::InstanceIniWOrR().
						GetCurrentLanguagePrivateProfileString(L"RdzbDemoMain",L"Setting")) )->setData( "Setting" );
	m_menu.addAction( QFU((LPWSTR)RdzbDemo::InstanceIniWOrR().
						GetCurrentLanguagePrivateProfileString(L"RdzbDemoMain",L"ASharpMoveLiveDemo")) )->setData( "prompt" );
	m_menu.addAction( QFU((LPWSTR)RdzbDemo::InstanceIniWOrR().
						GetCurrentLanguagePrivateProfileString(L"RdzbDemoMain",L"DropOut")) )->setData( "Quit" );
	m_menu.setStyleSheet( 
		"QMenu {"

		"background-color: #555555;"
		"font-size:12px;"
		"color:white;   "
		"}"
		"QMenu::item:selected { /* when user selects item using mouse or keyboard */"
		"background-color: #292828;/*��һ�������ò˵�����꾭��ѡ�е���ʽ*/"
		"}"
		);

	//�����б�
	{
		QString Text = QString::fromUtf16( L"<html><head/><body><p align=\"center\"><span style=\" font-size:14pt; color:#ffa118;\">%1</span></p></body></html>" ).arg( QFU((LPWSTR)RdzbDemo::InstanceIniWOrR().
						GetCurrentLanguagePrivateProfileString(L"RdzbDemoMain",L"WindowList")) );
		ui.List_label->setText( Text );
	}

	//ֱ����ѡ
	RdzbDemo::setControlText( ui.slogan_label,L"RdzbDemoMain", L"LivePreferred",ELabel);

	//PCֱ��
	RdzbDemo::setControlText( ui.PC_Live_pushButton,L"RdzbDemoMain", L"PCLive",EButton);

	//��Ϸ
	RdzbDemo::setControlText( ui.Game_label,L"RdzbDemoMain", L"Game",EButton);

	//��ʾ��
	RdzbDemo::setControlText( ui.Display_label,L"RdzbDemoMain", L"Scope",EButton);

	//����
	RdzbDemo::setControlText( ui.Windows_label,L"RdzbDemoMain", L"Windows",EButton);

	//����ͷ
	RdzbDemo::setControlText( ui.Camera_label,L"RdzbDemoMain", L"Camera",EButton);

	//����ֱ��
	RdzbDemo::setControlText( ui.Mobile_Game_pushButton,L"RdzbDemoMain", L"HandTour",EButton);

	//��׿
	RdzbDemo::setControlText( ui.Android_label,L"RdzbDemoMain", L"Android",EButton);

	//IOS
	RdzbDemo::setControlText( ui.IOS_label,L"RdzbDemoMain", L"IOS",EButton);

	//����
	RdzbDemo::setControlText( ui.Other_pushButton,L"RdzbDemoMain", L"Other",EButton);

	//��Ƶ
	RdzbDemo::setControlText( ui.Video_label,L"RdzbDemoMain", L"Video",EButton);

	//RTMP
	RdzbDemo::setControlText( ui.RTMP_label,L"RdzbDemoMain", L"RTMP",EButton);

	//flash
	RdzbDemo::setControlText( ui.flash_label,L"RdzbDemoMain", L"flash",EButton);

	//ͼƬ
	RdzbDemo::setControlText( ui.picture_label,L"RdzbDemoMain", L"Picture",EButton);

	//��Ļ
	RdzbDemo::setControlText( ui.captions_label,L"RdzbDemoMain", L"Captions",EButton);

	//��ʼֱ��
	RdzbDemo::setControlText( ui.Live_Begin_pushButton,L"RdzbDemoMain", L"StartLive",EButton);

	//ֱֹͣ��
	RdzbDemo::setControlText( ui.Live_Quit_pushButton,L"RdzbDemoMain", L"StopLive",EButton);
}

RdzbDemo::~RdzbDemo()
{

}

bool RdzbDemo::winEvent( MSG* msg, long* result )
{
	if ( msg->message == WM_NCHITTEST )
	{
		QPoint	pos( short(msg->lParam & 0xFFFF), short(DWORD(msg->lParam) >> 16) );
		QWidget*	pWid	= childAt( mapFromGlobal( pos ) );
		if ( pWid == NULL || pWid == ui.Rdzb_label_Normal1
			//|| pWid == ui.Rdzb_label 
			||  pWid == ui.slogan_label 
			|| pWid == ui.TitleBarWidget 
			|| pWid == ui.List_label )
		{
				Restore();
			*result	= HTCAPTION;
			return true;
		}
		else
		{
			if ( pWid == ui.widget_4  || pWid == ui.Preview_widget 
				|| pWid == ui.Search_pushButton 
				|| pWid == ui.listWidget )
				Restore();
		}
	}
	return false;
}

void RdzbDemo::on_Speaker_pushButton_clicked()
{
	ui.Speaker_widget->show();
}

void RdzbDemo::on_Setting_pushButton_clicked()
{
	m_SettingDialog->Refresh_Auido();
	m_SettingDialog->exec();
}

void RdzbDemo::on_MIC_pushButton_clicked()
{
	ui.MIC_widget->show();	
}

void RdzbDemo::on_Close_pushButton_clicked()
{
	close();
}

void RdzbDemo::closeEvent( QCloseEvent * event )
{
	if ( !m_IsClose )
	{
		PromptDialog promptDialog;
		promptDialog.LangageChange();
		promptDialog.exec();

		if (  promptDialog.m_SchemaSelectSerialNumber == 0  )
		{
			event->ignore();  //�����˳��źţ������������  
			return;
		}
		else if (  promptDialog.m_SchemaSelectSerialNumber == 1 )
		{
			//��������
			hide();
			//��ϵͳ������ʾ������Ϣ��ʾ
			OnbtnCloseClicked();
			m_trayicon->show();
			event->ignore();  //�����˳��źţ������������  
			return;
		}
	}
	m_bIsClose = true;
	//���������ļ�����
	SaveScenes();
	//���������ļ�����
	SaveProfile();
	//����ʼ��
	RDLive_Uninit();
}


void RdzbDemo::on_Min_pushButton_clicked()
{
	showMinimized();
}

void RdzbDemo::on_Operate_pushButton_clicked()
{
	QPoint pint = ui.Operate_pushButton->pos();
	QAction*	act	= m_menu.exec(mapToGlobal(QPoint( 685, pint.y() + ui.Operate_pushButton->height() )));
	if ( NULL == act ) return ;
	QString	szAct	= act->data().toString();

	if ( "Setting" == szAct )
	{
		m_SettingDialog->Refresh_Auido();
		m_SettingDialog->exec();
	} 
	else if( "prompt" == szAct )
	{
		m_aboutDialog->exec();
	}
	else if ( "Quit" == szAct )
	{
		close();
	}
}

void RdzbDemo::on_Search_pushButton_clicked( bool checked )
{
	if ( checked )
	{
		ui.Search_pushButton->setText(">");
		ui.Component_widget->hide();
		setFixedWidth( m_UiWidth -  m_ComponentWidth);
	} 
	else
	{
		ui.Search_pushButton->setText("<");
		ui.Component_widget->show();
		setFixedWidth( m_UiWidth );
	}
}

void RdzbDemo::Restore()
{
	ui.Speaker_widget->hide();
	ui.MIC_widget->hide();

	ui.Add_widget->hide();
	//PC
	ui.Display_label->hide();
	ui.Display_pushButton_select->hide();
	ui.Game_label->hide();
	ui.Game_pushButton_select->hide();
	ui.Windows_label->hide();
	ui.Windows_pushButton_select->hide();
	ui.Camera_label->hide();
	ui.Camera_pushButton_select->hide();
	//����
	ui.IOS_label->hide();
	ui.IOS_pushButton_select->hide();
	ui.Android_label->hide();
	ui.Android_pushButton_select->hide();
	//����
	ui.picture_pushButton_select->hide();
	ui.picture_label->hide();
	ui.flash_pushButton_select->hide();
	ui.flash_label->hide();
	ui.RTMP_pushButton_select->hide();
	ui.RTMP_label->hide();
	ui.RTMP_pushButton_select->hide();
	ui.Video_label->hide();	
	ui.Video_pushButton_select->hide();
	ui.captions_label->hide();
	ui.captions_pushButton_select->hide();
}

void RdzbDemo::changeEvent( QEvent* e )
{
	if(e->type() == QEvent::WindowStateChange)  
	{  
		if( this->windowState() & Qt::WindowMinimized )  
		{ 

		}  
		else	
		{ 
			ui.Add_widget->show();
			ui.Add_widget->hide();
		}  
	}  
}

void RdzbDemo::on_Mobile_Game_pushButton_clicked()
{
	Restore();

	ui.Add_widget->show();
	ui.IOS_label->show();
	ui.IOS_pushButton_select->show();
	ui.Android_label->show();
	ui.Android_pushButton_select->show();
}

void RdzbDemo::on_PC_Live_pushButton_clicked()
{
	Restore();

	ui.Add_widget->show();
	ui.Display_label->show();
	ui.Display_pushButton_select->show();
	ui.Game_label->show();
	ui.Game_pushButton_select->show();
	ui.Windows_label->show();
	ui.Windows_pushButton_select->show();
	ui.Camera_label->show();
	ui.Camera_pushButton_select->show();
}

VOID WINAPI RdzbDemo::RenderNotifyCB( IRender_ENotify eNotify, HSCENE hScene, INT iValue, LPVOID pCbParam )
{
	RdzbDemo * pThis = (RdzbDemo*)pCbParam;
	emit pThis->signalsRenderNotifyCB(int(eNotify),int(hScene),int(iValue));
}
VOID WINAPI RdzbDemo::EncoderNotifyCB( IEncoder_ENotify eNotify, DWORD dwValue, DWORD_PTR ptrValue, LPVOID pCbParam )
{
	RdzbDemo * pThis = (RdzbDemo*)pCbParam;
	emit pThis->signalsEncoderNotifyCB( int(eNotify),dwValue,ptrValue );
}

void RdzbDemo::UpdatePreviewConfig( IGlRender_SPreviewLayout::EReposWnd eMode )
{
	m_sPerviewLayout = *Render_GetPreviewLayout();
	m_sPerviewLayout.fScale	= 0.0f;
	m_sPerviewLayout.hMainWnd	= winId();
	m_sPerviewLayout.siMinimumLimit.cx	= ui.Preview_widget->minimumWidth();
	m_sPerviewLayout.siMinimumLimit.cy	= ui.Preview_widget->minimumHeight();

	//if ( !m_sPerviewLayout.hMainWnd )		//���֮ǰ����ʹ�õ�OpenGL����Ԥ�������ھ���Ҫ����Ϊʹ�ô���Ԥ��
	//{
	//}
	//m_sPerviewLayout.rtPreview.left	= 0;
	//m_sPerviewLayout.rtPreview.right	= ui.Preview_widget->width();
	//m_sPerviewLayout.rtPreview.top	= 0;
	//m_sPerviewLayout.rtPreview.bottom= ui.Preview_widget->height();
	//m_sPerviewLayout.hPreviewWnd	= ui.Preview_widget->winId();
	//m_sPerviewLayout.pCbFunction	= NULL;
	//m_sPerviewLayout.pCbParam	= NULL;

	//ui.Preview_widget->SetUsePreiveImage( FALSE );

	if ( m_sPerviewLayout.hMainWnd )
	{

	}
	m_sPerviewLayout.rtPreview.left	= ui.Preview_widget->geometry().left();
	m_sPerviewLayout.rtPreview.right	= ui.Preview_widget->geometry().right() + 1;
	m_sPerviewLayout.rtPreview.top	= ui.Preview_widget->geometry().top();
	m_sPerviewLayout.rtPreview.bottom= ui.Preview_widget->geometry().bottom() + 1;
	m_sPerviewLayout.hPreviewWnd	= 0;
	m_sPerviewLayout.pCbFunction	= ui.Preview_widget->OnPreviewBufferCB;
	m_sPerviewLayout.pCbParam		= ui.Preview_widget;

	ui.Preview_widget->SetUsePreiveImage( TRUE );

	m_sPerviewLayout.eReposWindow	= eMode;
	m_sPerviewLayout.bOnlyWorkArea	= TRUE;
	//���ݲ˵�������Ԥ������
	m_sPerviewLayout.iBSceneCount	=  0;
	m_sPerviewLayout.bAddSceneBut	= FALSE;
	m_sPerviewLayout.bSceneName		= FALSE;
	m_sPerviewLayout.bCurLikeBScene	= FALSE;
	m_sPerviewLayout.iScrollBarSize	=  12;

	Render_SetPreviewLayout( &m_sPerviewLayout );
}

void RdzbDemo::on_signalsRenerNotifyCB( int eNotify, int hScene, int iValue )
{
	if ( hScene == Render_GetCurScene() )
	{
		m_bChipListChanging	= TRUE;
		ChipItem::OnRenderNotify( this, ui.listWidget, (IRender_ENotify)eNotify, hScene, iValue );
		m_bChipListChanging	= FALSE;
	}
}

void RdzbDemo::on_signalsEncoderNotifyCB( int eNotify, ulong dwValue, ulong ptrValue )
{
	IEncoder_ENotify Notify =  (IEncoder_ENotify)eNotify;
	switch( Notify )
	{
	case eEncNotifySuccess:
		break;
	case eEncNotifyStarted:
		ui.Live_Begin_pushButton->hide();
		ui.Live_Quit_pushButton->show();
		ui.Setting_pushButton->setEnabled(false);
	//	ui.Speaker_pushButton->setEnabled(false);
	//	ui.MIC_pushButton->setEnabled(false);
		break;
	case eEncNotifyStoped:
		ui.Live_Begin_pushButton->show();
		ui.Live_Quit_pushButton->hide();
		ui.Setting_pushButton->setEnabled(TRUE);
	//	ui.Speaker_pushButton->setEnabled(TRUE);
	//	ui.MIC_pushButton->setEnabled(TRUE);
		if ( dwValue )
		{
			QMessageBox::critical( this, 
				QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PrompLliveRecording1")), 
				QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PrompLliveRecording1Error"),).arg( dwValue ) ,
				QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleModification",L"Acknowledgement")) );
		}
		break;
	case eEncNotifyEncodeError:
		break;
	case eEncNotifyEncodeFps:
		break;
	case eEncNotifyWriteFileFail:
		break;
	case eEncNotifyDisconnected:
		break;
	case eEncNotifyReConnectStart:
		break;
	case eEncNotifyReConnectDone:
		break;
	case eEncNotifyReConnectFail:
		break;
	case eEncNotifyUploadBitrate:
		break;
	case eEncNotifyDiscardPacks:
		break;
	case eEncNotifyDiscardFrame:
		break;
	case eEncNotifyResetBitrate:
		break;
	case eEncNotifyResetPreset:
		break;
	case eEncNotify_AcceditDone:
		OnAcceditDone( dwValue ? TRUE : FALSE, QFU( (PWORD)ptrValue ) );
		break;
	case eEncNotify_GetCloudFail:
		QMessageBox::critical( this, 
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PrompLliveRecording2") ), 
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PrompLliveRecording2Error"),).arg( QFU( PWORD(ptrValue) ) ),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleModification",L"Acknowledgement")) );
		break;
	}
}

BOOL RdzbDemo::LoadProfile()
{
	//�û������ļ���·�������û������ļ����ڣ��Ұ汾��Ĭ�������ļ�һ�£���ʹ���û������ļ���
	QString		szOptUser		= QFU( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Option.xml";
	szOptUser	= QDir::fromNativeSeparators( szOptUser );

	QFile		fileUser( szOptUser );
	if ( !fileUser.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		return SettingToUiAndSdk();		//�ļ���ʧ��
	}
	else
	{
		if ( !m_docOption.setContent( &fileUser ) )
		{
			return SettingToUiAndSdk();		//���� XML ��ʽʧ��
		}
		else
		{
			QDomElement		eleUser	= m_docOption.documentElement();
			if ( eleUser.tagName() != "Option" )
			{
				return SettingToUiAndSdk();	//XML ���ݴ���
				fileUser.close();
			}
		}
		fileUser.close();
	}

	return SettingToUiAndSdk();
}

BOOL RdzbDemo::SaveProfile( BOOL isSet )
{
	QString		szOptScenes		= QFU( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Option.xml";
	szOptScenes	= QDir::fromNativeSeparators( szOptScenes );
	QDomDocument	docScenes;

	QDomProcessingInstruction	domProc	= docScenes.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	docScenes.appendChild( domProc );
	QDomElement		eleSceneAll	= docScenes.createElement( "Option" );
	docScenes.appendChild( eleSceneAll );

	//��������
	//����
	if( isSet )
	{
		Audio_SetCurrent( eAudCap_Speaker, (LPCWSTR)m_SettingDialog->ui.Speaker_comboBox->itemData(m_SettingDialog->ui.Speaker_comboBox->currentIndex()).toString ().utf16() );
		Audio_SetCurrent( eAudCap_Microphone, (LPCWSTR)m_SettingDialog->ui.MIC_comboBox->itemData(m_SettingDialog->ui.MIC_comboBox->currentIndex()).toString ().utf16() );
	}
	Audio_Enable( eAudCap_Speaker,  !ui.Checke_Speaker_pushButton->isChecked()? TRUE : FALSE );
	Audio_SetVolume( eAudCap_Speaker, (FLOAT(ui.Speaker_verticalSlider->value()))/100.0f  );
	Audio_Enable( eAudCap_Microphone,  !ui.Checke_MIC_pushButton->isChecked()? TRUE : FALSE );
	Audio_SetVolume( eAudCap_Microphone, (FLOAT(ui.MIC_verticalSlider->value()))/100.0f  );

	//��Ƶ��������
	Encoder_SetAudioParams( 2, Aud_Inp_Samp_44100, 16, Aud_Enc_AAC, 0 );

	QDomElement	eleAuidoCapture	= docScenes.createElement( "AuidoCapture" );  
	eleSceneAll.appendChild( eleAuidoCapture );
	//������
	QDomElement	eleSpeaker	= docScenes.createElement( "Speaker" );  
	eleSpeaker.setAttribute( "DevId",QFU(Audio_GetCurrent(eAudCap_Speaker)) );
	eleSpeaker.setAttribute( "Mutex",Audio_IsEnabled(eAudCap_Speaker) );
	eleSpeaker.setAttribute( "Volume",Audio_GetVolume(eAudCap_Speaker) );
	eleAuidoCapture.appendChild(eleSpeaker);
	//��˷�
	QDomElement	eleMicrophone	= docScenes.createElement( "Microphone" );  
	eleMicrophone.setAttribute( "DevId",QFU(Audio_GetCurrent(eAudCap_Microphone)) );
	eleMicrophone.setAttribute( "Mutex",Audio_IsEnabled(eAudCap_Microphone) );
	eleMicrophone.setAttribute( "Volume",Audio_GetVolume(eAudCap_Microphone) );
	eleAuidoCapture.appendChild(eleMicrophone);

	//��Ƶ
	QDomElement	eleVideoCapture	= docScenes.createElement( "VideoCapture" ); 
	//����
	int width = 1920;
	int height = 1080;
	int fps   = 25;
	//��ȡ���
	if( isSet )
	{
		QString szResolution = m_SettingDialog->ui.Resolution_comboBox->currentText();
		int iNumber = szResolution.indexOf("x"); 
		QString szWidth =  szResolution.mid(0,iNumber);
		QString szHeight = szResolution.mid(iNumber+1);
		width = szWidth.toInt();
		height = szHeight.toInt();
		if ( width <= 0 )
		{
			width = 1920;
		}
		if ( height <= 0 )
		{
			height = 1080;
		}
		Render_SetSize(width,height);
		UpdatePreviewConfig(IGlRender_SPreviewLayout::eNotChangePos);
		//fps
		fps = m_SettingDialog->ui.FPS_comboBox->currentText().toInt();
		if ( fps <= 0 )
		{
			fps = 25;
		}
		Render_SetFps( FLOAT(fps) );
		//����
		EVideoRateMode	eRateMode = VR_VariableBitrate;
		INT				 iBitrateMax, iVbvSize;
		iBitrateMax = int((float)m_SettingDialog->ui.Bitrate_spinBox->value() * 1.2f);
		iVbvSize = iBitrateMax;
		Encoder_SetBitrate(eRateMode,m_SettingDialog->ui.Bitrate_spinBox->value(),iBitrateMax,iVbvSize);

	}
	fps = (int)Render_GetFps();
	SIZE size = Render_GetSize();

	//��Ƶ���ݱ���
	eleVideoCapture.setAttribute( "width",size.cx );
	eleVideoCapture.setAttribute( "height",size.cy );
	eleVideoCapture.setAttribute( "Fps",fps );
	eleVideoCapture.setAttribute("AutoAero",m_SettingDialog->ui.Win7_Aero_checkBox->isChecked());
	eleVideoCapture.setAttribute("CursorCapture",m_SettingDialog->ui.Win7_MOUSE_checkBox->isChecked());
	eleSceneAll.appendChild(eleVideoCapture );

	//Live
	DWORD	dwError	= 0;
	QDomElement	eleLiveCapture	= docScenes.createElement( "LiveCapture" ); 
	SEncoderSaveFile	sSave;
	ZeroMemory( &sSave, sizeof( sSave ) );
	QString				szTitle	= m_SettingDialog->ui.TiletelineEdit->text();
	if ( isSet && szTitle.isEmpty() )
	{
		QMessageBox::warning( window(), 
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PromptCodingOutput")),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"PromptCodingOutputError")),
			QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleModification",L"Acknowledgement")) );
		return false;
	}
	Encoder_RemoveSaveFile( -1 );	//ɾ�����еı����ļ�����
	LPCSTR				szExtNames[]	= { ".flv", ".mp4", ".avi" };
	QString				szFilePath;
	eleLiveCapture.setAttribute( "Title", szTitle );
	//�ļ�
	eleLiveCapture.setAttribute( "File",  m_SettingDialog->ui.File_checkBox->isChecked() ? 1 : 0 );
	sSave.eContainer	= (EFileContainer) m_SettingDialog->ui.File_comboBox->currentIndex();
	if ( !m_SettingDialog->CheckMovieSaveDir( isSet ) )
	{
		return false;
	}

	szFilePath			= CombinationFilePath( m_SettingDialog->ui.File_lineEdit->text(), m_SettingDialog->ui.TiletelineEdit->text(), szExtNames[ sSave.eContainer ] );
	sSave.szSavePath	= (LPCWSTR)szFilePath.utf16();
	sSave.iSplitSize	= INT64( m_SettingDialog->ui.File_spinBox->value() ) * 1024 * 1024;

	eleLiveCapture.setAttribute( "Dir", m_SettingDialog->ui.File_lineEdit->text() );
	eleLiveCapture.setAttribute( "Fmt", int( sSave.eContainer ) );
	eleLiveCapture.setAttribute( "Split", m_SettingDialog->ui.File_spinBox->value() );

	if (  m_SettingDialog->ui.File_checkBox->isChecked() )
	{
		if ( -1 == Encoder_AddSaveFile( &sSave ) )
		{
			dwError	= GetLastError();
		}
	}

	//Rtmp
	eleLiveCapture.setAttribute( "Rtmp",  m_SettingDialog->ui.Rtmp_checkBox->isChecked() ? 1 : 0 );
	eleLiveCapture.setAttribute( "RtmpFlow", m_SettingDialog->ui.Rtmp_lineEdit->text() );
	if (  m_SettingDialog->ui.Rtmp_checkBox->isChecked() )
	{
		if( !eleLiveCapture.attribute( "RtmpFlow" ).isEmpty() )
		{
			sSave.eContainer	= Mix_RTMP;
			sSave.szSavePath	= (LPCWSTR)eleLiveCapture.attribute( "RtmpFlow" ).utf16();
			if ( -1 == Encoder_AddSaveFile( &sSave ) )
			{
				dwError	= GetLastError();
			}
		}

	}
	eleSceneAll.appendChild( eleLiveCapture );

	if ( !isSet )
	{
		QFile			fileScenes( szOptScenes );
		if ( !fileScenes.open( QIODevice::WriteOnly | QIODevice::Text ) )
		{
			return FALSE;		//�ļ���ʧ��
		}
		QTextStream	steXml( &fileScenes );
		steXml.setCodec( "UTF-8" );
		docScenes.save( steXml, 4, QDomNode::EncodingFromTextStream );
		fileScenes.close();
	}
	return TRUE;
}

void RdzbDemo::on_Android_pushButton_select_clicked()
{
	AndroidSelectDialog androidSelectDialog(this);
	androidSelectDialog.LangageChange();
	androidSelectDialog.exec();
}
void RdzbDemo::on_Android_label_clicked()
{
	on_Android_pushButton_select_clicked();
}

void RdzbDemo::on_IOS_pushButton_select_clicked()
{
	IOSSelectDialog iosSelectDialog(this);
	iosSelectDialog.LangageChange();
	iosSelectDialog.exec();
}

void RdzbDemo::on_IOS_label_clicked()
{
	on_IOS_pushButton_select_clicked();
}

void RdzbDemo::on_Game_pushButton_select_clicked()
{
	GameSelectDialog gameSelectDialog(this);
	gameSelectDialog.LangageChange();
	gameSelectDialog.exec();
}

void RdzbDemo::on_Game_label_clicked()
{
	on_Game_pushButton_select_clicked();
}

void RdzbDemo::on_Display_pushButton_select_clicked()
{
	ScreenSelectDialog screenSelectDialog(this);
	screenSelectDialog.LangageChange();
	screenSelectDialog.Refresh_Screen();
	screenSelectDialog.exec();
}

void RdzbDemo::on_Display_label_clicked()
{
	on_Display_pushButton_select_clicked();
}

void RdzbDemo::on_Windows_pushButton_select_clicked()
{
	WindowsSelectDialog windowsSelectDialog(this);
	windowsSelectDialog.LangageChange();
	windowsSelectDialog.exec();
}


void RdzbDemo::on_Windows_label_clicked()
{
	on_Windows_pushButton_select_clicked();
}

void RdzbDemo::on_Camera_pushButton_select_clicked()
{
	CameraSettingDialog cameraSettingDialog(this);
	cameraSettingDialog.LangageChange();
	cameraSettingDialog.exec();
}

void RdzbDemo::on_Camera_label_clicked()
{
	on_Camera_pushButton_select_clicked();
}

void RdzbDemo::GetWindowsList( void * pwidget, int iControlNumber /*= 0 */ )
{
	HWND			hWnd	= GetTopWindow( NULL );
	WCHAR			szWindow[MAX_PATH]	= {0};
	DWORD			dwStyle				= 0;
	DWORD			dwStyleEx			= 0;
	BOOL			bAddWindow			= FALSE;
	HWND			hMyWnd				= winId();
	QFontMetrics	fm( ((QWidget*)pwidget)->font() );
	QString			szItem;
	while( hWnd )
	{
		bAddWindow	= FALSE;
		dwStyle		= GetWindowLong( hWnd, GWL_STYLE );
		dwStyleEx	= GetWindowLong( hWnd, GWL_EXSTYLE );
		while( !bAddWindow )
		{
			if (  hMyWnd == hWnd ) break;
			if ( dwStyle & ( WS_ICONIC | WS_CHILD ) ) break;
			if ( ( dwStyle & WS_VISIBLE ) == 0 ) break;
			if ( GetWindow( hWnd, GW_OWNER ) ) break;
			if ( GetWindowLong( hWnd, GWL_HWNDPARENT ) ) break;
			bAddWindow	= TRUE;
		}

		if ( bAddWindow )
		{
			if ( InternalGetWindowText( hWnd, szWindow, ARRAYSIZE( szWindow ) ) && szWindow[0] )
			{
				szItem	= fm.elidedText( QFU( szWindow ), Qt::ElideRight, 200 );
				if ( iControlNumber == 0 )
				{
					QListWidget * pListWidget = (QListWidget*)pwidget;
					MyRadioButton * pRadioButton = new MyRadioButton( pListWidget );

					pRadioButton->setText( szItem );
					pRadioButton->m_hWnd = hWnd;
				} 
				else if( iControlNumber == 1  )
				{
					QComboBox * pComboBox = (QComboBox*) pwidget;
					pComboBox->addItem( szItem,UINT(hWnd));
				}
			}
		}
		hWnd	= GetWindow( hWnd, GW_HWNDNEXT );
	}
}

BOOL RdzbDemo::LoadScenes()
{
	BOOL		bReplaceUserOpt	= FALSE;	//�Ƿ���Ҫ�滻�����û��������ļ�
	//Ĭ�������ļ���·����������ĳ�ʼ���ã����û������ļ�������ʱ������Ĭ�����õİ汾�и���ʱ�����滻���û������ļ���
	QString		szOptDefault	= QFU( (PWORD)RDLive_DefaultDir( RDir_Application ) ) + "Scenes.xml";
	//ȡ�������ļ���·����ʹ���� RDLive_DefaultDir() �õ�ָ�����ļ��С�
	QString		szOptScenes		= QFU( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Scenes.xml";
	szOptDefault	= QDir::fromNativeSeparators( szOptDefault );
	szOptScenes	= QDir::fromNativeSeparators( szOptScenes );

	QDomDocument	docScenes;
	QFile			fileScenes( szOptScenes );
	if ( !fileScenes.open( QIODevice::ReadOnly | QIODevice::Text ) )
	{
		bReplaceUserOpt	= TRUE;		//���� XML ��ʽʧ��
	}
	else
	{
		if ( !docScenes.setContent( &fileScenes ) )
		{
			bReplaceUserOpt	= TRUE;		//���� XML ��ʽʧ��
		}
		fileScenes.close();
	}
	if ( bReplaceUserOpt )
	{
		//return FALSE;
		QFile::remove( szOptScenes );
		if ( !QFile::copy( szOptDefault, szOptScenes ) )
		{
			return FALSE;		//�ļ�����ʧ��
		}
		if ( !fileScenes.open( QIODevice::ReadOnly | QIODevice::Text ) )
		{
			return FALSE;		//�ļ���ʧ��
		}
		if ( !docScenes.setContent( &fileScenes ) )
		{
			return FALSE;		//���� XML ��ʽʧ��
		}
		QDomElement		eleUser	= docScenes.documentElement();
		if ( eleUser.tagName() != "Scenes" )
		{
			return FALSE;		//XML ���ݴ���
		}

	}

	QDomElement		eleSceneAll	= docScenes.documentElement();
	if ( eleSceneAll.tagName() != "Scenes" )
	{
		return FALSE;		//XML ���ݴ���
	}
	QDomNode		nodeScene	= eleSceneAll.firstChild();


	while( !nodeScene.isNull() )
	{
		if ( !nodeScene.isElement() )
		{
			nodeScene	= nodeScene.nextSibling();
			continue;
		}
		QDomElement		eleScene	= nodeScene.toElement();
		if ( eleScene.tagName() != "Scene" )
		{
			nodeScene	= nodeScene.nextSibling();
			continue;
		}
		HSCENE	hScene	= Render_GetScene(0);
		QDomNode	nodeChip	= eleScene.firstChild();
		while( !nodeChip.isNull() )
		{
			if ( !nodeChip.isElement() )
			{
				nodeChip	= nodeChip.nextSibling();
				continue;
			}
			QDomElement		eleChip		= nodeChip.toElement();
			if ( eleChip.tagName() != "Chip" )
			{
				nodeChip	= nodeChip.nextSibling();
				continue;
			}
			int				iSelChip	= eleScene.attribute( "Current", "-1" ).toInt();
			HCHIP			hChip;
			IPinInput_EClass	eClass		= (IPinInput_EClass)eleChip.attribute( "Type" ).toInt();
			QString sz  = eleChip.attribute( "Source" );

			sz = sz.replace(QRegExp("\\/"), "\\");
			if ( sz.contains(QFU(L"TipsImage\\OperatingHints.png"), Qt::CaseInsensitive) )
			{
				QString		szOptDefault	= QFU( (PWORD)RDLive_DefaultDir( RDir_Application ) ) + "TipsImage\\OperatingHints.png";
				sz = szOptDefault;
			}
			switch( eClass )
			{
			case ePinInput_Picture:
				hChip	= Scene_CreateChip( hScene, eClass, (LPCWSTR)sz.utf16(),
					eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE );
				break;
			case ePinInput_Camera:
			case ePinInput_Screen:
				hChip	= Scene_CreateChip( hScene, eClass, (LPCWSTR)eleChip.attribute( "Source" ).utf16(),
					eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE );
				break;
			case ePinInput_Flash:
			case ePinInput_Movie:
				hChip	= Scene_CreateChip( hScene, eClass, (LPCWSTR)eleChip.attribute( "Source" ).utf16(),
					eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE, 
					eleChip.attribute( "Playing" ).toInt() ? TRUE : FALSE );
				break;
			case ePinInput_Game:
				hChip	= Scene_CreateChip( hScene, eClass, (LPCWSTR)eleChip.attribute( "Source" ).utf16(),
					eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE );
				break;
			case  ePinInput_Text:
				hChip	= Scene_CreateChip( hScene, eClass, (LPCWSTR)eleChip.attribute( "Source" ).utf16(),
					eleChip.attribute( "CannotReuse" ).toInt() ? TRUE : FALSE );
				break;
			}
			if ( hChip == NULL )
			{
				nodeChip	= nodeChip.nextSibling();
				continue;
			}

			QDomElement	eleColor	= eleChip.firstChildElement( "Color" );
			if ( !eleColor.isNull() )
			{
				Chip_SetBaseShaderParam( hChip, eShader_Hue, eleColor.attribute( "Hue" ).toFloat() );
				Chip_SetBaseShaderParam( hChip, eShader_UseFixedHue, eleColor.attribute( "HueFixed" ).toFloat() );
				Chip_SetBaseShaderParam( hChip, eShader_Saturation, eleColor.attribute( "Saturation" ).toFloat() );
				Chip_SetBaseShaderParam( hChip, eShader_Lighteness, eleColor.attribute( "Ligtheness" ).toFloat() );
				Chip_SetBaseShaderParam( hChip, eShader_Contrast, eleColor.attribute( "Contrast" ).toFloat() );
				Chip_SetBaseShaderParam( hChip, eShader_Transparency, eleColor.attribute( "Transparency" ).toFloat() );
			}

			QDomElement		eleRect		= eleChip.firstChildElement( "Rect" );
			if ( !eleRect.isNull() )
			{
				Chip_SetRectPercent( hChip, eleRect.attribute( "X" ).toFloat(), eleRect.attribute( "Y" ).toFloat(),
					eleRect.attribute( "W" ).toFloat(), eleRect.attribute( "H" ).toFloat(), eIgnoreAspectRatio );
			}
			QDomElement		eleClip		= eleChip.firstChildElement( "Clip" );
			if ( !eleClip.isNull() )
			{
				Chip_SetClipPercent( hChip, eleClip.attribute( "L" ).toFloat(), eleClip.attribute( "T" ).toFloat(),
					eleClip.attribute( "R" ).toFloat(), eleClip.attribute( "B" ).toFloat() );
			}
			Chip_SetViewLock( hChip, eLock_AspectRatio, FALSE );

			Chip_SetVisible( hChip, eleChip.attribute( "Visible" ).toInt() ? TRUE : FALSE );
			Chip_SetVolume( hChip, eleChip.attribute( "Volume" ).toFloat() );
			Chip_SetMute( hChip, eleChip.attribute( "Mutex" ).toInt() ? TRUE : FALSE );
			Chip_SetLoop( hChip, eleChip.attribute( "LoopPlay" ).toInt() ? TRUE : FALSE );
			if ( eleChip.attribute( "RangePlay" ).toInt() )
			{
				Chip_SetRange( hChip, INT64( eleChip.attribute( "RangeStart" ).toFloat() * 1000.0 ),
					INT64( eleChip.attribute( "RangeEnd" ).toFloat() * 1000.0 ), FALSE );
			}

			if ( iSelChip == Scene_GetChipCount( hScene ) - 1 )
			{
				Chip_SetCurent( hChip );
			}

			nodeChip	= nodeChip.nextSibling();
		}
		nodeScene	= nodeScene.nextSibling();
	}
	return TRUE;
}

BOOL RdzbDemo::SaveScenes()
{
	QString		szOptScenes		= QFU( (PWORD)RDLive_DefaultDir( RDir_Profile ) ) + "Scenes.xml";
	szOptScenes	= QDir::fromNativeSeparators( szOptScenes );

	QDomDocument	docScenes;

	QDomProcessingInstruction	domProc	= docScenes.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
	docScenes.appendChild( domProc );
	QDomElement		eleSceneAll	= docScenes.createElement( "Scenes" );
	docScenes.appendChild( eleSceneAll );

	for ( int iScene = 0; iScene < Render_GetSceneCount(); ++iScene )
	{
		HSCENE		hScene		= Render_GetScene( iScene );
		QDomElement	eleScene	= docScenes.createElement( "Scene" );
		for ( int iChip = 0; iChip < Scene_GetChipCount( hScene ); ++iChip )
		{
			HCHIP		hChip	= Scene_GetChip( hScene, iChip );
			QDomElement	eleChip	= docScenes.createElement( "Chip" );
			IPinInput_SStatusInfo	sStatus;
			Chip_GetStatusInfo( hChip, &sStatus );

			eleChip.setAttribute( "Type", Chip_GetClassType( hChip ) );
			eleChip.setAttribute( "Source", QFU( Chip_GetSourceName( hChip ) ) );
			eleChip.setAttribute( "CannotReuse", sStatus.bCannotReuse ? 1 : 0 );
			eleChip.setAttribute( "Paused", sStatus.eStatus == ePin_Paused ? 1 : 0 );
			eleChip.setAttribute( "Visible", Chip_IsVisible( hChip ) );
			eleChip.setAttribute( "Volume", sStatus.fVolume );
			eleChip.setAttribute( "Mutex", sStatus.bIsMute );
			if ( sStatus.eStatus == ePin_Played || sStatus.eStatus == ePin_Loading )
				eleChip.setAttribute( "Playing", 1 );
			else
				eleChip.setAttribute( "Playing", 0 );
			eleChip.setAttribute( "LoopPlay", sStatus.bIsLoop );
			eleChip.setAttribute( "RangePlay", sStatus.bIsRange ? 1 : 0 );
			eleChip.setAttribute( "RangeStart", sStatus.fSecondStart );
			eleChip.setAttribute( "RangeEnd", sStatus.fSecondEnd );

			QDomElement	eleColor	= docScenes.createElement( "Color" );
			eleColor.setAttribute( "Hue", Chip_GetBaseShaderParam( hChip, eShader_Hue ) );
			eleColor.setAttribute( "HueFixed", Chip_GetBaseShaderParam( hChip, eShader_UseFixedHue ) );
			eleColor.setAttribute( "Saturation", Chip_GetBaseShaderParam( hChip, eShader_Saturation ) );
			eleColor.setAttribute( "Ligtheness", Chip_GetBaseShaderParam( hChip, eShader_Lighteness ) );
			eleColor.setAttribute( "Contrast", Chip_GetBaseShaderParam( hChip, eShader_Contrast ) );
			eleColor.setAttribute( "Transparency", Chip_GetBaseShaderParam( hChip, eShader_Transparency ) );
			eleChip.appendChild( eleColor );

			QDomElement	eleRect	= docScenes.createElement( "Rect" );
			FLOAT	fX, fY, fW, fH;
			BOOL	bKeepAspectRatio;
			Chip_GetRectPercent( hChip, &fX, &fY, &fW, &fH, FALSE );
			eleRect.setAttribute( "X", fX );
			eleRect.setAttribute( "Y", fY );
			eleRect.setAttribute( "W", fW );
			eleRect.setAttribute( "H", fH );
			eleChip.appendChild( eleRect );

			QDomElement	eleClip	= docScenes.createElement( "Clip" );
			FLOAT	fL, fT, fR, fB;
			Chip_GetClipPercent( hChip, &fL, &fT, &fR, &fB );
			eleClip.setAttribute( "L", fL );
			eleClip.setAttribute( "T", fT );
			eleClip.setAttribute( "R", fR );
			eleClip.setAttribute( "B", fB );
			eleChip.appendChild( eleClip );

			eleScene.appendChild( eleChip );
		}
		int	iCurrent	= Chip_GetIndex( Sceen_GetCurChip( hScene ), NULL );
		eleSceneAll.appendChild( eleScene );
	}

	QFile			fileScenes( szOptScenes );
	if ( !fileScenes.open( QIODevice::WriteOnly | QIODevice::Text ) )
	{
		return FALSE;		//�ļ���ʧ��
	}
	QTextStream	steXml( &fileScenes );
	steXml.setCodec( "UTF-8" );
	docScenes.save( steXml, 4, QDomNode::EncodingFromTextStream );
	fileScenes.close();
	return TRUE;
}

void RdzbDemo::AddPicture()
{
	QString		szOptDefault	= QFU( (PWORD)RDLive_DefaultDir( RDir_Application ) ) + "TipsImage\\OperatingHints.png";
	HCHIP	hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Picture,(LPCWSTR)szOptDefault.utf16()  );
	if ( hChip )
	{
		Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
		Chip_SetVisible( hChip, TRUE );
		if ( Chip_Open( hChip, (LPCWSTR)szOptDefault.utf16() ) )
		{

		}
	}
}

BOOL RdzbDemo::SettingToUiAndSdk()
{	
	SettingAuido();
	SettingVideo();
	SettingLive();
	return TRUE;
}

QDomElement RdzbDemo::FindXmlElement( const QString& szPath, BOOL bCreateIfNull /*= FALSE */ )
{
	QStringList		lstNode		= szPath.split( '/' );

	QDomElement		eleFind		= m_docOption.documentElement();  //���ظ�Ԫ��
	for ( int i = 0; i < lstNode.count(); ++i )
	{
		QDomElement	eleParent	= eleFind;
		eleFind		= eleParent.firstChildElement( lstNode[i] );
		if ( eleFind.isNull() )
		{
			if ( bCreateIfNull )
			{
				eleFind	= m_docOption.createElement( lstNode[i]);
				eleParent.appendChild( eleFind );
			}
			else
			{
				break;
			}
		}
	}
	return eleFind;
}

BOOL RdzbDemo::SettingAuido()
{
	//������
	int			iCount		= Audio_GetDevCount( eAudCap_Speaker );
	if ( iCount > 0 )
	{
		QDomElement	eleSpeaker	= FindXmlElement( "AuidoCapture/Speaker" );
		if ( !eleSpeaker.isNull() )
		{
			Audio_SetCurrent( eAudCap_Speaker, (LPCWSTR)eleSpeaker.attribute( "DevId" ).utf16() );
			Audio_Enable( eAudCap_Speaker, eleSpeaker.attribute( "Mutex" ).toInt() ? TRUE : FALSE );
			Audio_SetVolume( eAudCap_Speaker, eleSpeaker.attribute( "Volume" ).toFloat() );
		}
		else
		{
			Audio_Enable( eAudCap_Speaker, TRUE );
			Audio_SetVolume( eAudCap_Speaker, 1.0f );
		}
		ui.Checke_Speaker_pushButton->setChecked(!Audio_IsEnabled(eAudCap_Speaker));
		ui.Speaker_verticalSlider->setValue( int( Audio_GetVolume(eAudCap_Speaker) * 100.0f ) );
		ui.Speaker_verticalSlider->setEnabled(Audio_IsEnabled(eAudCap_Speaker));
	}
	else
	{
		ui.Checke_Speaker_pushButton->setEnabled(false);
		ui.Speaker_verticalSlider->setEnabled(false);
	}
	//��˷�
	iCount		= Audio_GetDevCount( eAudCap_Microphone );
	if ( iCount > 0 )
	{
		QDomElement	eleMicro	= FindXmlElement( "AuidoCapture/Microphone" );
		if ( !eleMicro.isNull() )
		{
			Audio_SetCurrent( eAudCap_Microphone, (LPCWSTR)eleMicro.attribute( "DevId" ).utf16() );
			Audio_Enable( eAudCap_Microphone, eleMicro.attribute( "Mutex" ).toInt() ? TRUE : FALSE );
			Audio_SetVolume( eAudCap_Microphone, eleMicro.attribute( "Volume" ).toFloat() );
		}
		else
		{
			Audio_Enable( eAudCap_Microphone, TRUE );
			Audio_SetVolume( eAudCap_Microphone, 1.0f );
		}
		ui.Checke_MIC_pushButton->setChecked(!Audio_IsEnabled(eAudCap_Microphone));
		ui.MIC_verticalSlider->setValue( int( Audio_GetVolume(eAudCap_Microphone) * 100.0f ) );
		ui.MIC_verticalSlider->setEnabled(Audio_IsEnabled(eAudCap_Microphone));
	}
	else
	{
		ui.Checke_MIC_pushButton->setEnabled(false);
		ui.MIC_verticalSlider->setEnabled(false);
	}
	return TRUE;
}

BOOL RdzbDemo::SettingVideo()
{
	QDomElement		eleVideo	= FindXmlElement("VideoCapture");
	if ( !eleVideo.isNull() )
	{
		//Aero ��Ч AutoAero
		BOOL bAero = eleVideo.attribute("AutoAero").toInt()?TRUE:false;
		Screen_SetAutoEnableAero( bAero );
		m_SettingDialog->ui.Win7_Aero_checkBox->setChecked(bAero);

		//���¼�� CursorCapture
		BOOL bCursorCapture = eleVideo.attribute("CursorCapture").toInt()?TRUE:false;
		Cursor_EnableCapture( bCursorCapture );
		m_SettingDialog->ui.Win7_MOUSE_checkBox->setChecked(bCursorCapture);

		//�ֱ���
		int width = eleVideo.attribute("width").toInt()?eleVideo.attribute("width").toInt():1920;
		int height= eleVideo.attribute("height").toInt()?eleVideo.attribute("height").toInt():1080;
		Render_SetSize(width,height);

		//֡��
		int fps = eleVideo.attribute("Fps").toInt()?eleVideo.attribute("Fps").toInt():25;
		Render_SetFps( FLOAT(fps) );
		//����
		EVideoRateMode	eRateMode = VR_VariableBitrate;
		INT				 iBitrateMax, iVbvSize;

		BitrateStatic bitrateStatic = GetBitrate(width,height,fps);

		iBitrateMax = int((float)bitrateStatic.iBitrate * 1.2f);
		iVbvSize = iBitrateMax;
		Encoder_SetBitrate(eRateMode,bitrateStatic.iBitrate,iBitrateMax,iVbvSize);
		if ( m_SettingDialog )
		{
			QString szResolution = QString::number(width) + "x" + QString::number(height);
			for( int i = 0; i < m_SettingDialog->ui.Resolution_comboBox->count(); i++ )
			{
				if ( szResolution == m_SettingDialog->ui.Resolution_comboBox->itemText(i)  )
				{
					m_SettingDialog->ui.Resolution_comboBox->setCurrentIndex( i );
				}
			}

			QString szFps = QString::number(fps);
			for( int i = 0; i < m_SettingDialog->ui.FPS_comboBox->count(); i++ )
			{
				if ( szFps == m_SettingDialog->ui.FPS_comboBox->itemText(i)  )
				{
					m_SettingDialog->ui.FPS_comboBox->setCurrentIndex( i );
				}
			}

			m_SettingDialog->ui.Bitrate_spinBox->setMaximum(bitrateStatic.iBitrateMaximum);
			m_SettingDialog->ui.Bitrate_spinBox->setMinimum(bitrateStatic.iBitrateMinimum);
			m_SettingDialog->ui.Bitrate_spinBox->setValue(bitrateStatic.iBitrate);

		}

	}
	else
	{
		//Aero ��Ч AutoAero
		Screen_SetAutoEnableAero( TRUE );
		m_SettingDialog->ui.Win7_Aero_checkBox->setChecked(TRUE);
		//���¼�� CursorCapture
		Cursor_EnableCapture( TRUE );
		m_SettingDialog->ui.Win7_MOUSE_checkBox->setChecked(TRUE);
		//�ֱ���
		int width = 1920;
		int height= 1080;
		Render_SetSize(width,height);
		m_SettingDialog->ui.Resolution_comboBox->setCurrentIndex( 0 );
		//֡��
		Render_SetFps( FLOAT(25) );
		m_SettingDialog->ui.FPS_comboBox->setCurrentIndex( 2 );
		//����
		EVideoRateMode	eRateMode = VR_VariableBitrate;
		INT				 iBitrateMax, iVbvSize;

		BitrateStatic bitrateStatic = GetBitrate(width,height,25);

		iBitrateMax = int((float)bitrateStatic.iBitrate * 1.2f);
		iVbvSize = iBitrateMax;
		Encoder_SetBitrate(eRateMode,bitrateStatic.iBitrate,iBitrateMax,iVbvSize);

		m_SettingDialog->ui.Bitrate_spinBox->setMaximum(bitrateStatic.iBitrateMaximum);
		m_SettingDialog->ui.Bitrate_spinBox->setMinimum(bitrateStatic.iBitrateMinimum);
		m_SettingDialog->ui.Bitrate_spinBox->setValue(bitrateStatic.iBitrate);

	}

	return TRUE;
}

BOOL RdzbDemo::SettingLive()
{
	QDomElement		eleVideo	= FindXmlElement("LiveCapture");
	if ( !eleVideo.isNull() )
	{
		//�ļ�����
		m_SettingDialog->ui.TiletelineEdit->setText(eleVideo.attribute( "Title" ));
		m_SettingDialog->ui.File_lineEdit->setText(eleVideo.attribute( "Dir" ));
		m_SettingDialog->ui.File_checkBox->setChecked( eleVideo.attribute( "File",0).toInt() ? TRUE : FALSE );
		m_SettingDialog->ui.File_comboBox->setCurrentIndex( eleVideo.attribute( "Fmt",0 ).toInt() );
		m_SettingDialog->ui.File_spinBox->setValue( eleVideo.attribute( "Split", 0 ).toInt() );

		if( m_SettingDialog->ui.File_checkBox->isChecked() )
		{
			m_SettingDialog->ui.File_lineEdit->setEnabled(TRUE);
			m_SettingDialog->ui.browse_pushButton->setEnabled(TRUE);
			m_SettingDialog->ui.View_pushButton->setEnabled(TRUE);
			m_SettingDialog->ui.File_comboBox->setEnabled(TRUE);
			m_SettingDialog->ui.File_spinBox->setEnabled(TRUE);
		}		

		m_SettingDialog->CheckMovieSaveDir();
		//Rtmpֱ��
		m_SettingDialog->ui.Rtmp_checkBox->setChecked( eleVideo.attribute( "Rtmp",0).toInt() ? TRUE : FALSE );
		m_SettingDialog->ui.Rtmp_lineEdit->setText( eleVideo.attribute( "RtmpFlow",0) );

		if( m_SettingDialog->ui.Rtmp_checkBox->isChecked() )
		{
			m_SettingDialog->ui.Rtmp_lineEdit->setEnabled(TRUE);
		}
	}
	return TRUE;
}

void RdzbDemo::on_listWidget_currentItemChanged( QListWidgetItem * current, QListWidgetItem * previous )
{
	if ( !m_bChipListChanging && current )
	{
		ChipItem*	pChip	= (ChipItem*)ui.listWidget->itemWidget( current );
		if ( pChip ) Chip_SetCurent( pChip->GetHChip() );
	}
}

RdzbDemo::BitrateStatic RdzbDemo::GetBitrate( int width, int height, int fps )
{	
	BitrateStatic bitrateStatic = {0};
	bitrateStatic.iBitrate = int((FLOAT)width*(FLOAT)height*0.25f*(FLOAT)sqrt((double)fps)/1000.0f + 0.5f);
	bitrateStatic.iBitrateMinimum = int((FLOAT)width*(FLOAT)height*0.15f*(FLOAT)sqrt((double)fps)/1000.0f + 0.5f);
	bitrateStatic.iBitrateMaximum = int((FLOAT)width*(FLOAT)height*0.5f*(FLOAT)sqrt((double)fps)/1000.0f + 0.5f);

	return bitrateStatic;
}

void RdzbDemo::on_listWidget_currentRowChanged( int index )
{
	ChipItem*	pChip = NULL;
	for ( int i = 0 ; i <  ui.listWidget->count(); i++)
	{
		pChip = NULL;
		pChip	= (ChipItem*)ui.listWidget->itemWidget( ui.listWidget->	item  (i) );
		if ( pChip )  pChip->OnChipSelected( TRUE );
	}
	pChip = NULL;
	pChip = (ChipItem*)ui.listWidget->itemWidget( ui.listWidget->currentItem () );
	if ( pChip )  pChip->OnChipSelected( FALSE );
}

QString RdzbDemo::CombinationFilePath( const QString& szDir, const QString& szPrefix, const QString& szExtension )
{
	QString	szFilePath	= szDir;
	SYSTEMTIME	stTime;
	if ( !szFilePath.endsWith( "\\" ) ) szFilePath += "\\";

	GetLocalTime( &stTime );
	QString	szPre	= szPrefix;

	szPre.replace( QChar('\\'), QChar('-') );
	szPre.replace( QChar('/'), QChar('-') );
	szPre.replace( QChar(':'), QChar('-') );
	szPre.replace( QChar('*'), QChar('-') );
	szPre.replace( QChar('?'), QChar('-') );
	szPre.replace( QChar('\"'), QChar('\'') );
	szPre.replace( QChar('<'), QChar('(') );
	szPre.replace( QChar('>'), QChar(')') );
	szPre.replace( QChar('|'), QChar('!') );

	szFilePath	+= QString( "%1-%2-%3\\" ).arg( stTime.wYear, 2, 10, QChar('0') )
		.arg( stTime.wMonth, 2, 10, QChar('0') ).arg( stTime.wDay, 2, 10, QChar('0') );
	szFilePath	+= QString( "%1(%2%3%4)%5" ).arg( szPre )
		.arg( stTime.wHour, 2, 10, QChar('0') ).arg( stTime.wMinute, 2, 10, QChar('0') )
		.arg( stTime.wSecond, 2, 10, QChar('0') ).arg( szExtension );
	return szFilePath;
}

void RdzbDemo::on_Live_Begin_pushButton_clicked()
{
	bool isBegin = FALSE;
	if( m_SettingDialog != NULL )
		isBegin = m_SettingDialog->Save();
	else
		isBegin = SaveProfile(TRUE);

	if( isBegin )
		Encoder_Start();
}

void RdzbDemo::on_Live_Quit_pushButton_clicked()
{
	Encoder_End();
}

void RdzbDemo::OnbtnCloseClicked()
{
	//ÿ�����пͻ��ˣ������ε�����Ͻǹرպ���ʾ��С����֮�����ظ���ʾ��
	m_bShow = TRUE; 
	if (m_bShow)
	{
		if( m_trayicon != NULL )
		{
			delete m_trayicon;
			m_trayicon = NULL;

		}

		m_trayicon = new QSystemTrayIcon(this);
		//����QIcon���󣬲�����ͼ����Դ��ֵΪ��Ŀ����Դ�ļ���ͼ��ĵ�ַ
		QIcon icon(":/ICON/Resources/HeadPortrait.png");
		m_trayiconMenu = new QMenu(this);

		//Ϊ���̲˵���Ӳ˵���
		QAction* startAction = new QAction( QIcon(""),QFU(RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoSalver",L"OpenMainInterface")), this);
		connect(startAction, SIGNAL(triggered()), this, SLOT(on_UI_clicked()));
		m_trayiconMenu->addAction(startAction);
		//Ϊ���̲˵���ӷָ���
		m_trayiconMenu->addSeparator();
		//Ϊ���̲˵���Ӳ˵���
		QAction* ShowAction = new QAction( QIcon(""),QFU(RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoSalver",L"DropOut")), this);
		connect(ShowAction, SIGNAL(triggered()), this, SLOT( SelfClose() ));
		m_trayiconMenu->addAction(ShowAction);
		//m_trayiconMenu->addAction(quitAction);
		m_trayiconMenu->setStyleSheet(
			"QMenu {"
			"background-color: #555555;"
			"font-size:12px;"
			"color:white;   "
			"}"
			"QMenu::item:selected { /* when user selects item using mouse or keyboard */"
			"background-color: #292828;/*��һ�������ò˵�����꾭��ѡ�е���ʽ*/"
			"}"
			);

		//��������QIcon������Ϊϵͳ����ͼ��
		m_trayicon->setIcon(icon);
		//��ʾ����ͼ��
		/*m_trayicon->show();*/
		//����ϵͳ������ʾ
		m_trayicon->setToolTip(QFU(RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoAnenst",L"YuedongLiveDemo")));
		//�������˵���Ϊϵͳ���̲˵�   
		m_trayicon->setContextMenu(m_trayiconMenu);

		m_trayicon->showMessage(QFU(RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoClosePrompt",L"Prompt")), QFU(RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoAnenst",L"YuedongLiveDemo")), QSystemTrayIcon::Information, 7000);
		//Ϊϵͳ���̰󶨵����źŵĲ� ��ͼ�꼤��ʱ
		connect(m_trayicon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(onSystemTrayIconClicked(QSystemTrayIcon::ActivationReason)));
		m_bShow = FALSE;
	}
}

void RdzbDemo::on_UI_clicked()
{
	HWND hPrevWnd = this->winId();
	m_trayicon->hide();
	if ( !isVisible() ) show();	// ��������Ѿ���С�������У���ô������ʾ
	if ( isMinimized() ) ShowWindow( hPrevWnd, SW_RESTORE ); //ʹ����������ʾ  
	SetForegroundWindow( hPrevWnd ); 
	SetForegroundWindow( GetLastActivePopup(hPrevWnd) ); 
	////ǿ���ػ���С����ť����Ϊ�˽��������С��֮��ԭ���ڣ���С����ť������ʾ����״̬�����
}

void RdzbDemo::onSystemTrayIconClicked( QSystemTrayIcon::ActivationReason reason )
{
	switch(reason)
	{
		//����
	case QSystemTrayIcon::Trigger:
		//OnShowSelfClicked();    //������ǰ����
		break;
		//˫��
	case QSystemTrayIcon::DoubleClick:
		//�ָ�������ʾ
		on_UI_clicked();
		break;
	default:
		break;
	}
}

void RdzbDemo::SelfClose()
{
	m_trayicon->hide();
	m_IsClose = true;
	close();
}

void RdzbDemo::on_Other_pushButton_clicked()
{
	Restore();

	ui.Add_widget->show();
	ui.picture_pushButton_select->show();
	ui.picture_label->show();
	ui.flash_pushButton_select->show();
	ui.flash_label->show();
	ui.RTMP_pushButton_select->show();
	ui.RTMP_label->show();
	ui.RTMP_pushButton_select->show();
	ui.Video_label->show();	
	ui.Video_pushButton_select->show();
	ui.captions_label->show();
	ui.captions_pushButton_select->show();
}

void RdzbDemo::on_Video_pushButton_select_clicked()
{
	OpenDialog openDialog(this);
	openDialog.LangageChange();
	openDialog.SetModel(OpenDialog::OPFilefmtMove);
	openDialog.exec();
}



void RdzbDemo::on_Video_label_clicked()
{
	on_Video_pushButton_select_clicked();
}

void RdzbDemo::on_RTMP_pushButton_select_clicked()
{
	OpenDialog openDialog(this);
	openDialog.SetModel(OpenDialog::OPFilefmtRtmp);
	openDialog.exec();
}

void RdzbDemo::on_RTMP_label_clicked()
{
	on_RTMP_pushButton_select_clicked();
}


void RdzbDemo::on_flash_pushButton_select_clicked()
{
	OpenDialog openDialog(this);
	openDialog.LangageChange();
	openDialog.SetModel(OpenDialog::OPFilefmtFlash);
	openDialog.exec();
}

void RdzbDemo::on_flash_label_clicked()
{
	on_flash_pushButton_select_clicked();
}

void RdzbDemo::on_picture_pushButton_select_clicked()
{
	OpenDialog openDialog(this);
	openDialog.LangageChange();
	openDialog.SetModel(OpenDialog::OPFilefmtPicture);
	openDialog.exec();
}

void RdzbDemo::on_picture_label_clicked()
{
	on_picture_pushButton_select_clicked();
}

void RdzbDemo::OnAcceditDone( BOOL bSuccess, const QString& szErrStr )
{
	m_aboutDialog->ui.labAcceditLocalDays->setAlignment ( Qt::AlignHCenter );
	m_aboutDialog->ui.labAcceditLiveDays->setAlignment ( Qt::AlignHCenter );
	if ( bSuccess )
	{
		FLOAT	fLocalDays	= RDLive_GetAccreditDays( eAccreditLocalSave );
		FLOAT	fRtmpDays	= RDLive_GetAccreditDays( eAccreditRtmpLive );
		FLOAT	fCloudDays	= RDLive_GetAccreditDays( eAccreditCloudLive );

		QString	szTilte	= QFU(RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoAnenst",L"DisplayPointOut1") ).arg( fLocalDays, 0, 'f', 1 );
		m_aboutDialog->ui.labAcceditLocalDays->setText( szTilte );

		szTilte	= QFU( RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoAnenst",L"DisplayPointOut2") ).arg( fRtmpDays, 0, 'f', 1 );
		QString szTilte1;
		szTilte1 = QFU( RdzbDemo::InstanceIniWOrR().
			GetCurrentLanguagePrivateProfileString(L"RdzbDemoAnenst",L"DisplayPointOut6") ).arg( fCloudDays, 0, 'f', 1 );
		szTilte.append('\n');
		szTilte.append(szTilte1);
		m_aboutDialog->ui.labAcceditLiveDays->setText( szTilte );

		ui.Live_Begin_pushButton->setEnabled(true);
	}
	else
	{
		m_aboutDialog->ui.labAcceditLocalDays->setText( QFU( L"��Ȩʧ�ܣ�%1").arg( szErrStr ) );
		m_aboutDialog->ui.labAcceditLiveDays->setText( QFU( L"��Ȩʧ�ܣ�%1").arg( szErrStr ) );
		ui.Live_Begin_pushButton->setEnabled(FALSE);
	}
	
	m_aboutDialog->m_bSuccess = bSuccess;
	m_aboutDialog->m_szErrStr = szErrStr;
	//��ֱ��Demo
	RdzbDemo::setControlText( m_aboutDialog->ui.label,L"RdzbDemoAnenst", L"YuedongLiveDemo",ELabel);
	this->setWindowTitle( QFU( RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAnenst",L"YuedongLiveDemo")  ) );
	RdzbDemo::setControlText( ui.Rdzb_label_Normal1,L"RdzbDemoAnenst", L"YuedongLiveDemo",ELabel);
	
	//��ϵ��ʽ
	RdzbDemo::setControlText( m_aboutDialog->ui.label_2,L"RdzbDemoAnenst", L"DisplayPointOut4",ELabel);
		
	//���ⷴ����ʽ
	RdzbDemo::setControlText( m_aboutDialog->ui.label_3,L"RdzbDemoAnenst", L"DisplayPointOut5",ELabel);
	
	//ȷ��
	RdzbDemo::setControlText( m_aboutDialog->ui.OK_pushButton,L"RdzbDemoAnenst", L"Acknowledgement",EButton);
	

}

void RdzbDemo::on_captions_pushButton_select_clicked()
{
	CreateSubtitleModification();
}

void RdzbDemo::on_captions_label_clicked()
{
	on_captions_pushButton_select_clicked();
}



void RdzbDemo::dragEnterEvent( QDragEnterEvent * event )
{
	//���Ϊ�ļ�����֧���Ϸ�
	if (event->mimeData()->hasFormat("text/uri-list"))
	{
		event->acceptProposedAction();
	}
}

void RdzbDemo::dropEvent( QDropEvent * event )
{
	//���ı�����׷���ļ���
	QList<QUrl> urls = event->mimeData()->urls();
	if(urls.isEmpty())
		return;

	foreach(QUrl url, urls) {
		QString file_name = url.toLocalFile();
	}
}

void RdzbDemo::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Escape:
		close();
		break;
	default:
		QDialog::keyPressEvent(event);
	}
}

void RdzbDemo::on_Checke_Speaker_pushButton_clicked( bool checked )
{
	ui.Speaker_verticalSlider->setEnabled(!checked);
	Audio_Enable( eAudCap_Speaker,  !checked?true:FALSE );
}

void RdzbDemo::on_Checke_MIC_pushButton_clicked( bool checked )
{
	ui.MIC_verticalSlider->setEnabled(!checked);
	Audio_Enable( eAudCap_Microphone,  !checked?true:FALSE  );
}

IniWOrR& RdzbDemo::InstanceIniWOrR()
{
	static IniWOrR m_IniWOrR;
	return m_IniWOrR;
}

void RdzbDemo::setControlText( QWidget * Control, LPWSTR lpAppName,LPWSTR lpKeyName , EControlType controlType )
{
	LPWSTR Str = (LPWSTR)RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(lpAppName,lpKeyName);
	switch( controlType )
	{
	case ELabel:
		((QLabel*)Control)->setText( QString::fromUtf16(Str) );
		break;
	case EButton:
		((QPushButton*)Control)->setText( QString::fromUtf16(Str) );
		break;
	case ECheckBox:
		((QCheckBox*)Control)->setText( QString::fromUtf16(Str) );
		break;
	case ETextEdit:
		((QTextEdit*)Control)->setText( QString::fromUtf16(Str) );
		break;
	case ETextBrowser:
		((QTextBrowser*)Control)->setText( QString::fromUtf16(Str) );
		break;
	case EPushButtonToolTip:
		((QPushButton*)Control)->setToolTip( QString::fromUtf16(Str) );
		break;
	}
	delete [] Str;
}

void RdzbDemo::SwitchingLanguage()
{
	LangageChange();

	m_SettingDialog->LangageChange();

	//OnbtnCloseClicked();

	for ( int  i = 0;  i < ui.listWidget->count(); i++ )
	{
		QListWidgetItem*	pItem	= ui.listWidget->item( i );
		ChipItem*	pChip	= (ChipItem*)ui.listWidget->itemWidget( pItem );
		pChip->LangageChange();
	}

	OnAcceditDone( m_aboutDialog->m_bSuccess,m_aboutDialog->m_szErrStr );
}

void RdzbDemo::on_pushButton_clicked()
{
	SwitchingLanguage();
}

void RdzbDemo::on_MIC_verticalSlider_valueChanged( int value )
{
	Audio_SetVolume( eAudCap_Speaker, (FLOAT(value))/100.0f  );
}

void RdzbDemo::on_Speaker_verticalSlider_valueChanged( int value )
{
	Audio_SetVolume( eAudCap_Microphone, (FLOAT(value))/100.0f  );
}

void RdzbDemo::CreateSubtitleModification( HCHIP	hChip )
{
	QTxtDialog txtDialog( this );
	txtDialog.LangageChange();
	txtDialog.Init(hChip);
	txtDialog.exec();
}




