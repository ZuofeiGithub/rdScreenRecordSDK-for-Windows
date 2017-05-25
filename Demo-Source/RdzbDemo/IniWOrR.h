#pragma once

class IniWOrR
{
public:
	IniWOrR(void);
	~IniWOrR(void);

	void CLanguagePath( LPWSTR LanguagePath );//����ѡ���·��
	int Count();//���Ե�����
	LPCTSTR GetLanguagePrivateProfileString( LPCTSTR lpAppName,LPCTSTR lpKeyName );//��ȡ����ѡ��ini�ļ�string
	BOOL  WriteLanguagePrivateProfileString( LPCTSTR lpAppName,LPCTSTR lpKeyName, LPCTSTR lpString );//д�뵱ǰ���Ե�

	void CCurrentLanguagePath( LPWSTR CurrentLanguagePath  );//��ǰ���Ե�·��
	LPCTSTR GetCurrentLanguagePrivateProfileString( LPCTSTR lpAppName,LPCTSTR lpKeyName );//��ȡ����ini�ļ�string

private: 
	wstring m_LanguagePath;//����ѡ��ı����ļ��������б�
	wstring m_CurrentLanguagePath;//��ǰ���Ե�
};
