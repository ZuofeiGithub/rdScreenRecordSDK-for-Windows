#include "StdAfx.h"
#include "qdraglistwidget.h"

QDragListWidget::QDragListWidget(QWidget *parent)
: QListWidget(parent)
{
	//setAcceptDrops(true);
}

QDragListWidget::~QDragListWidget()
{

}

bool  QDragListWidget::eventFilter( QObject * watched, QEvent * event )
{	
	if ( watched == this )
	{
		if (event->type() == QEvent::DragEnter) {
			//���Ϸ�ʱ������labelʱ, label�����ϷŵĶ���
			QDragEnterEvent *dee = dynamic_cast<QDragEnterEvent *>(event);	
			dee->acceptProposedAction();
			return true;
		} else if (event->type() == QEvent::Drop) {
			//���Ų���������, ȡ���Ϸŵ�����
			QDropEvent *de = dynamic_cast<QDropEvent *>(event);
			QList<QUrl> urls = de->mimeData()->urls();

			if (urls.isEmpty()) { return true; }
			QString path = urls.first().toLocalFile();

			QFileInfo fileInfo(path);
			QString suffix = fileInfo.suffix();
			HCHIP	hChip	= 0;
			//�ļ��Ƿ���ȷ
			int index = suffix.indexOf("mp4", 0, Qt::CaseInsensitive);
			if( (suffix.indexOf("mp4", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("m4v", 0, Qt::CaseInsensitive) > -1) || 
				(suffix.indexOf("flv", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("f4v", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("3gpp", 0, Qt::CaseInsensitive) > -1)
				)
			{
				hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Movie,(LPCWSTR)path.utf16(), FALSE, TRUE );
			}
			else if ( (suffix.indexOf("png", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("jpg", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("jpeg", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("gif", 0, Qt::CaseInsensitive) > -1) ||
				(suffix.indexOf("bmp", 0, Qt::CaseInsensitive) > -1)
				)
			{
				hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Picture,(LPCWSTR)path.utf16());
			}
			else if ( (suffix.indexOf("swf", 0, Qt::CaseInsensitive) > -1) )
			{
				hChip	= Scene_CreateChip( Render_GetCurScene(), ePinInput_Flash,(LPCWSTR)path.utf16(), FALSE, TRUE );
			}


			if ( hChip )
			{
				Chip_SetRectPercent( hChip, 0.0f, 0.0f, 1.0f, 1.0f, eKeepAspectRatio );
				Chip_SetVisible( hChip, TRUE );
				Chip_SetViewLock( hChip, eLock_AspectRatio, FALSE );
			}
		}
	}

	return QListWidget::eventFilter(watched, event);
}
