// HookManager.h: interface for the CHookManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HOOKMANAGER_H__CDFFD3CF_937D_4B4E_A77E_351F2B6CA2F9__INCLUDED_)
#define AFX_HOOKMANAGER_H__CDFFD3CF_937D_4B4E_A77E_351F2B6CA2F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// �¼��б�ṹ��
struct S_ENODE 
{
	EVENTMSG Event;
	S_ENODE	 *lpNext;
};

// �¼�ͷָ��
extern S_ENODE *g_lpEventHead;  

// ¼�ƺͻطŹ���
extern HHOOK g_hHookRecord;
extern HHOOK g_hHookPlayBack;

static	S_ENODE *g_lpEventTail = NULL;  // ��¼�Ƶ��¼���ĩβ
static	S_ENODE *g_lpPlayEvent = NULL;  // ��ǰ���ڲ��ŵ��¼�

class CHookManager  
{
public:
	CHookManager();
	virtual ~CHookManager();

public:
	int HMStartRecord(HWND hWnd,BOOL bNoMouse);
	int	HMStopRecord(int nNotifyCode = 0);

	int	HMStartPlayBack(HWND hWnd,BOOL bDoClear);
	int	HMStopPlayBack(int nNotifyCode = 0);
	
	void HMFreeAllEvents();
};

#endif // !defined(AFX_HOOKMANAGER_H__CDFFD3CF_937D_4B4E_A77E_351F2B6CA2F9__INCLUDED_)
