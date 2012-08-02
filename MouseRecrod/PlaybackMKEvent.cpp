// PlaybackMKEvent.cpp: implementation of the CPlaybackMKEvent class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "PlaybackMKEvent.h"
#include "HookManager.h"
#include "CommDef.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlaybackMKEvent::CPlaybackMKEvent()
{

}

CPlaybackMKEvent::~CPlaybackMKEvent()
{

}

// ��ʼ�ط�
void CPlaybackMKEvent::StartPlayBack(HWND hWnd, int nClearPlaybackEvent)
{
	CHookManager HookManager;

	if (HookManager.HMStartPlayBack(hWnd,nClearPlaybackEvent))
    {
		OutputDebugStringA("��û��¼�ƣ������ܻط�!");
		return;
	}
}

// ֹͣ�ط�
void CPlaybackMKEvent::StopPlayBack()
{
	CHookManager HookManager;
	HookManager.HMStopRecord();
}
