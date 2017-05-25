#include "stdafx.h"
#include "rdzbdemo.h"
#include "androidselectdialog.h"

AndroidSelectDialog::AndroidSelectDialog(QWidget *parent)
: QDialog(parent)
{
	ui.setupUi(this);

	setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint/*| Qt::WindowStaysOnTopHint*/);
	//setAttribute(Qt::WA_TranslucentBackground,true); 
}

AndroidSelectDialog::~AndroidSelectDialog()
{

}

void AndroidSelectDialog::on_Close_pushButton_clicked()
{
	close();
}

void AndroidSelectDialog::on_Download_pushButton_clicked()
{
	QDesktopServices::openUrl(QUrl("http://www.bluestacks.cn/"));
}

bool AndroidSelectDialog::winEvent( MSG* msg, long* result )
{
	if ( msg->message == WM_NCHITTEST )
	{

		QPoint	pos( short(msg->lParam & 0xFFFF), short(DWORD(msg->lParam) >> 16) );
		QWidget*	pWid	= childAt( mapFromGlobal( pos ) );
		if ( pWid == NULL || pWid == ui.label_2 || pWid == ui.label ||  pWid == ui.widget_2)
		{
			*result	= HTCAPTION;
			return true;
		}
	}
	return false;
}

void AndroidSelectDialog::on_DownWindow_pushButton_clicked()
{
	HWND			hWnd	= GetTopWindow( NULL );
	WCHAR			szWindow[MAX_PATH]	= {0};
	DWORD			dwStyle				= 0;
	DWORD			dwStyleEx			= 0;
	BOOL			bAddWindow			= FALSE;
	HWND			hMyWnd				= winId();
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
				szItem	= QFU( szWindow );
				BOOL isBeginGameAndroid = FALSE;
				if ( szItem == QFU(L"�����") )
				{
					isBeginGameAndroid = TRUE;
					//51��׿ģ����
				} 
				else if( szItem == QFU(L"Bluestacks App Player") )
				{
					isBeginGameAndroid = TRUE;
					//����
				}
				else if( szItem == QFU(L"��������") )
				{
					isBeginGameAndroid = TRUE;
					//��������
				}
				else if ( szItem.contains(QFU(L"������ģ����"), Qt::CaseInsensitive))
				{
					isBeginGameAndroid = TRUE;
					//������ģ����
				}
				else if ( szItem.contains(QFU(L"MuMuģ����"), Qt::CaseInsensitive) )
				{
					isBeginGameAndroid = TRUE;
					//����mumu
				}
				else if ( szItem.contains(QFU(L"��������ģ����"), Qt::CaseInsensitive) )
				{
					isBeginGameAndroid = TRUE;
					//����ģ����
				}
				else if ( szItem.contains(QFU(L"���ɰ�׿ģ����"), Qt::CaseInsensitive) )
				{
					isBeginGameAndroid = TRUE;
					//���ɰ�׿ģ����
				}
				if ( isBeginGameAndroid )
				{
					IScreen_SCapParams	sCapParams	= {0};
					sCapParams.hWindow = hWnd;
					HCHIP	hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Screen, Screen_AssembleSource( &sCapParams ) );
					if ( hChip )
					{
						Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
						//����ӵ�Ԫ������Ҫ����������ʾ��
						Chip_SetVisible( hChip, TRUE );
						Chip_SetViewLock( hChip, eLock_AspectRatio, FALSE );
					}
					close();
					return;
				}
			}
		}
		hWnd	= GetWindow( hWnd, GW_HWNDNEXT );
	}
	QMessageBox::warning(this,
		QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"AndroidSimulation")),
		QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleErrorPointOut",L"AndroidSimulationError")),
		QFU(RdzbDemo::InstanceIniWOrR().GetCurrentLanguagePrivateProfileString(L"RdzbDemoSubtitleModification",L"Acknowledgement")) );
}

void AndroidSelectDialog::LangageChange()
{
	//��׿ֱ��
	RdzbDemo::setControlText( ui.label,L"RdzbDemoAndroidLive", L"AndroidLive",ELabel);

	//ģ����
	RdzbDemo::setControlText( ui.pushButton,L"RdzbDemoAndroidLive", L"Simulator",EButton);

	//����ģ����
	RdzbDemo::setControlText( ui.pushButton,L"RdzbDemoAndroidLive", L"DownloadSimulator",EButton);

	//���
	RdzbDemo::setControlText( ui.DownWindow_pushButton,L"RdzbDemoAndroidLive", L"Superinduce",EButton);

	//����
	//RdzbDemo::setControlText( ui.textBrowser,L"RdzbDemoAndroidLive", L"DisplayPointOut1",ETextEdit);
	QString str = QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut1"));
	str.append("\t\n");
	str.append(QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut2")));
	str.append("\t\n");
	str.append(QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut3")));
	str.append("\t\n");
	str.append(QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut4")));
	str.append("\t\n");
	str.append(QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut5")));
	str.append("\t\n");
	str.append(QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut6")));
	str.append("\t\n");
	str.append(QFU(RdzbDemo::InstanceIniWOrR().
		GetCurrentLanguagePrivateProfileString(L"RdzbDemoAndroidLive",L"DisplayPointOut7")));
	
	 ui.textBrowser->setText(str);
}
