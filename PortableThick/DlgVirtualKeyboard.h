#pragma once
#include "afxdialogex.h"
#include <vector>

// DlgVirtualKeyboard 전용 리소스 ID (버튼/키는 코드에서 생성)
#define IDC_BTN_CLOSE_KEYBOARD         1032

// 가상 키보드 키 버튼 ID 범위 (동적 생성용, 이 클래스 전용)
#define IDC_KEY_FIRST                  2000
#define IDC_KEY_LAST                   2040

// DlgVirtualKeyboard 대화 상자 (아이폰 스타일 QWERTY)

class DlgVirtualKeyboard : public CDialogEx
{
	DECLARE_DYNAMIC(DlgVirtualKeyboard)

public:
	DlgVirtualKeyboard(CWnd* pParent = nullptr);
	virtual ~DlgVirtualKeyboard();

	// 입력을 보낼 대상 에디트(또는 컨트롤) 설정. 버튼 클릭 시 포커스가 옮겨져도 이 쪽으로 전송됨.
	void SetTarget(HWND hwndTarget) { m_hwndTarget = hwndTarget; }

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_VIRTUAL_KEYBOARD };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();

	void CreateKeyButtons();
	void CreateNumpadButtons();
	void DestroyKeyButtons();
	void OnKeyPressed(UINT nID);
	void SendKeyInput(WCHAR ch, bool isBackspace, bool isReturn);

	std::vector<CButton*> m_keyButtons;
	CButton* m_pBtnClose;  // 닫기 버튼 (항상 표시)
	HWND m_hwndTarget;
	bool m_bShiftState;
	bool m_bNumberMode;
	static const int KEY_COUNT = 31;
	static const int NUMPAD_KEY_COUNT = 13;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedClose();
};
