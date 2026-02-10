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

	CWnd* pEdit = GetDlgItem(nTargetEditID);
	HWND hTarget = pEdit ? pEdit->GetSafeHwnd() : NULL;

	if (m_pVirtualKbd == nullptr)
	{
		m_pVirtualKbd = new DlgVirtualKeyboard(this);
		if (m_pVirtualKbd->Create(IDD_VIRTUAL_KEYBOARD, this))
		{
			m_pVirtualKbd->SetTarget(hTarget);
			// 먼저 위치 설정 후 표시 (첫 표시 시에도 한 번에 올바른 위치에 뜨도록)
			if (pEdit)
			{
				const int offsetX = 8, offsetY = 8;
				CRect rcEdit;
				pEdit->GetWindowRect(&rcEdit);
				int x = rcEdit.right + offsetX;
				int y = rcEdit.bottom + offsetY;
				CRect rcKbd;
				m_pVirtualKbd->GetWindowRect(&rcKbd);
				int screenRight = GetSystemMetrics(SM_CXSCREEN);
				int screenBottom = GetSystemMetrics(SM_CYSCREEN);
				if (x + rcKbd.Width() > screenRight) x = screenRight - rcKbd.Width();
				if (y + rcKbd.Height() > screenBottom) y = screenBottom - rcKbd.Height();
				if (x < 0) x = 0;
				if (y < 0) y = 0;
				m_pVirtualKbd->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			}
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
		if (pEdit)
		{
			// 선택한 에디트 컨트롤 위치 기준으로 가상키보드 창을 이동시킨다.
			const int offsetX = 8, offsetY = 8;
			CRect rcEdit;
			pEdit->GetWindowRect(&rcEdit);
			int x = rcEdit.right + offsetX;
			int y = rcEdit.bottom + offsetY;
			CRect rcKbd;
			m_pVirtualKbd->GetWindowRect(&rcKbd);
			int screenRight = GetSystemMetrics(SM_CXSCREEN);
			int screenBottom = GetSystemMetrics(SM_CYSCREEN);
			if (x + rcKbd.Width() > screenRight) x = screenRight - rcKbd.Width();
			if (y + rcKbd.Height() > screenBottom) y = screenBottom - rcKbd.Height();
			if (x < 0) x = 0;
			if (y < 0) y = 0;
			m_pVirtualKbd->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}
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
