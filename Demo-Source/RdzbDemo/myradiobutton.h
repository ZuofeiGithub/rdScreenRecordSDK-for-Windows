#ifndef MYRADIOBUTTON_H
#define MYRADIOBUTTON_H

#include <QRadioButton>

class MyRadioButton : public QRadioButton
{
	Q_OBJECT

public:
	MyRadioButton(QWidget *parent);
	~MyRadioButton();
	DWORD m_dwProcessId;//��ϷID
	HWND m_hWnd;//��Ӧ�Ĵ��ھ��
	void OnChipSelected();
private:
	QListWidget*		m_listWidChips;
	QListWidgetItem*	m_lwItem;

	QSize			m_siBase;
};

#endif // MYRADIOBUTTON_H
