// DlgSetting.cpp: 구현 파일
//

#include "pch.h"
#include "PortableThick.h"
#include "afxdialogex.h"
#include "DlgSetting.h"
#include "DlgVirtualKeyboard.h"
#include "resource.h"
#include <windows.h>


// DlgSetting 대화 상자

IMPLEMENT_DYNAMIC(DlgSetting, CDialogEx)

DlgSetting::DlgSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_pVirtualKbd(nullptr)
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

void DlgSetting::ShowOnScreenKeyboard(UINT nTargetEditID)
{
	if (m_pVirtualKbd != nullptr && !::IsWindow(m_pVirtualKbd->GetSafeHwnd()))
		m_pVirtualKbd = nullptr;

	HWND hTarget = GetDlgItem(nTargetEditID) ? GetDlgItem(nTargetEditID)->GetSafeHwnd() : NULL;

	if (m_pVirtualKbd == nullptr)
	{
		m_pVirtualKbd = new DlgVirtualKeyboard(this);
		if (m_pVirtualKbd->Create(IDD_VIRTUAL_KEYBOARD, this))
		{
			m_pVirtualKbd->SetTarget(hTarget);
			m_pVirtualKbd->ShowWindow(SW_SHOW);
		}
		else
		{
			delete m_pVirtualKbd;
			m_pVirtualKbd = nullptr;
		}
	}
	else
	{
		m_pVirtualKbd->SetTarget(hTarget);
		m_pVirtualKbd->ShowWindow(SW_SHOW);
	}
}


void DlgSetting::OnEnSetFocusEdit1()
{
	ShowOnScreenKeyboard(IDC_EDIT1);
}

void DlgSetting::OnEnSetFocusEdit2()
{
	ShowOnScreenKeyboard(IDC_EDIT2);
}

void DlgSetting::OnEnSetFocusEdit3()
{
	ShowOnScreenKeyboard(IDC_EDIT3);
}

void DlgSetting::OnEnSetFocusEdit4()
{
	ShowOnScreenKeyboard(IDC_EDIT4);
}

void DlgSetting::OnEnSetFocusEdit5()
{
	ShowOnScreenKeyboard(IDC_EDIT5);
}

void DlgSetting::OnEnSetFocusEdit6()
{
	ShowOnScreenKeyboard(IDC_EDIT6);
}

void DlgSetting::OnEnSetFocusEdit7()
{
	ShowOnScreenKeyboard(IDC_EDIT7);
}

void DlgSetting::OnEnSetFocusEdit8()
{
	ShowOnScreenKeyboard(IDC_EDIT8);
}

void DlgSetting::OnEnSetFocusEdit9()
{
	ShowOnScreenKeyboard(IDC_EDIT9);
}

void DlgSetting::OnBnClickedBtnResetVirKeyboard()
{
	// 기능 제거됨
}

void DlgSetting::OnBnClickedBtnSaveAndExit()
{

}
