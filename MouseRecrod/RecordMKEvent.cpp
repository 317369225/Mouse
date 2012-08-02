// RecordMKEvent.cpp: implementation of the CRecordMKEvent class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RecordMKEvent.h"
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

CRecordMKEvent::CRecordMKEvent()
{

}

CRecordMKEvent::~CRecordMKEvent()
{

}

// ��ʼ¼��
void CRecordMKEvent::StartRecord(HWND hWnd, int nOnlyKeyboardEvent) 
{
	CHookManager HookManager;

	if ( HookManager.HMStartRecord(hWnd,nOnlyKeyboardEvent))
    {
		return;
	}
}

// ֹͣ¼��
void CRecordMKEvent::StopRecord() 
{
	CHookManager HookManager;
	HookManager.HMStopRecord();
}