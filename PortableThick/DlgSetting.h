#pragma once
#include "afxdialogex.h"

class DlgVirtualKeyboard;

// DlgSetting 대화 상자

class DlgSetting : public CDialogEx
{
	DECLARE_DYNAMIC(DlgSetting)

public:
	DlgSetting(CWnd* pParent = nullptr);
	virtual ~DlgSetting();

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()

private:
	void ShowOnScreenKeyboard(UINT nTargetEditID);

	DlgVirtualKeyboard* m_pVirtualKbd;
	afx_msg void OnEnSetFocusEdit1();
	afx_msg void OnEnSetFocusEdit2();
	afx_msg void OnEnSetFocusEdit3();
	afx_msg void OnEnSetFocusEdit4();
	afx_msg void OnEnSetFocusEdit5();
	afx_msg void OnEnSetFocusEdit6();
	afx_msg void OnEnSetFocusEdit7();
	afx_msg void OnEnSetFocusEdit8();
	afx_msg void OnEnSetFocusEdit9();
public:
	afx_msg void OnBnClickedBtnResetVirKeyboard();
	afx_msg void OnBnClickedBtnSaveAndExit();
};
