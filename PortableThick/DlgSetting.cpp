// DlgSetting.cpp: 구현 파일
//

#include "pch.h"
#include "PortableThick.h"
#include "afxdialogex.h"
#include "DlgSetting.h"
#include <shellapi.h>
#include <windows.h>


// DlgSetting 대화 상자

IMPLEMENT_DYNAMIC(DlgSetting, CDialogEx)

DlgSetting::DlgSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

DlgSetting::~DlgSetting()
{
}

void DlgSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgSetting, CDialogEx)
	ON_EN_SETFOCUS(IDC_EDIT1, &DlgSetting::OnEnSetFocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, &DlgSetting::OnEnSetFocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &DlgSetting::OnEnSetFocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT4, &DlgSetting::OnEnSetFocusEdit4)
	ON_EN_SETFOCUS(IDC_EDIT5, &DlgSetting::OnEnSetFocusEdit5)
	ON_EN_SETFOCUS(IDC_EDIT6, &DlgSetting::OnEnSetFocusEdit6)
	ON_EN_SETFOCUS(IDC_EDIT7, &DlgSetting::OnEnSetFocusEdit7)
	ON_EN_SETFOCUS(IDC_EDIT8, &DlgSetting::OnEnSetFocusEdit8)
	ON_EN_SETFOCUS(IDC_EDIT9, &DlgSetting::OnEnSetFocusEdit9)
	ON_BN_CLICKED(IDC_BTN_RESET_VIR_KEYBOARD, &DlgSetting::OnBnClickedBtnResetVirKeyboard)
	ON_BN_CLICKED(IDC_BTN_SAVE_AND_EXIT, &DlgSetting::OnBnClickedBtnSaveAndExit)
END_MESSAGE_MAP()


// DlgSetting 메시지 처리기

void DlgSetting::ShowOnScreenKeyboard()
{
	// osk.exe (On-Screen Keyboard) 실행
	ShellExecute(NULL, _T("open"), _T("osk.exe"), NULL, NULL, SW_SHOWNORMAL);
}

void DlgSetting::OnEnSetFocusEdit1()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit2()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit3()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit4()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit5()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit6()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit7()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit8()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnEnSetFocusEdit9()
{
	ShowOnScreenKeyboard();
}

void DlgSetting::OnBnClickedBtnResetVirKeyboard()
{
	// 기능 제거됨
}

void DlgSetting::OnBnClickedBtnSaveAndExit()
{
	// OSK (화상 키보드) 종료
	HWND hwndOSK = ::FindWindow(_T("OSKMainClass"), NULL);
	if (hwndOSK != NULL && ::IsWindow(hwndOSK))
	{
		// 창 핸들에서 프로세스 ID 얻기
		DWORD dwProcessId = 0;
		::GetWindowThreadProcessId(hwndOSK, &dwProcessId);
		
		if (dwProcessId != 0)
		{
			// 프로세스 핸들 열기
			HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcessId);
			if (hProcess != NULL)
			{
				// 프로세스 종료
				::TerminateProcess(hProcess, 0);
				::CloseHandle(hProcess);
			}
		}
	}
	
	// 다이얼로그 닫기
	EndDialog(IDOK);
}
