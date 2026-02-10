// DlgVirtualKeyboard.cpp: 구현 파일 (아이폰 스타일 QWERTY)

#include "pch.h"
#include "PortableThick.h"
#include "afxdialogex.h"
#include "DlgVirtualKeyboard.h"
#include "DlgSetting.h"
#include "resource.h"


IMPLEMENT_DYNAMIC(DlgVirtualKeyboard, CDialogEx)

DlgVirtualKeyboard::DlgVirtualKeyboard(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_VIRTUAL_KEYBOARD, pParent)
	, m_hwndTarget(nullptr)
	, m_bShiftState(false)
	, m_bNumberMode(false)
	, m_pBtnClose(nullptr)
{
}

DlgVirtualKeyboard::~DlgVirtualKeyboard()
{
	// 창 파괴 시 자식 HWND는 이미 없음. CButton delete 시 CWnd 소멸자에서 크래시 가능
	// → delete 하지 않고 포인터만 비움 (의도적 누수, 키보드 닫을 때만 발생)
	m_keyButtons.clear();
	m_pBtnClose = nullptr;
}

void DlgVirtualKeyboard::PostNcDestroy()
{
	CDialogEx::PostNcDestroy();
}

void DlgVirtualKeyboard::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgVirtualKeyboard, CDialogEx)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDOK, &DlgVirtualKeyboard::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &DlgVirtualKeyboard::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_CLOSE_KEYBOARD, &DlgVirtualKeyboard::OnBnClickedClose)
	ON_COMMAND_RANGE(IDC_KEY_FIRST, IDC_KEY_LAST, &DlgVirtualKeyboard::OnKeyPressed)
END_MESSAGE_MAP()


BOOL DlgVirtualKeyboard::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	CreateKeyButtons();
	// 닫기 버튼 (우측 상단, QWERTY/넘패드 전환해도 항상 표시)
	CRect rcClient;
	GetClientRect(&rcClient);
	const int BTN_W = 50, BTN_H = 24, MARGIN = 8;
	int x = rcClient.Width() - MARGIN - BTN_W;
	m_pBtnClose = new CButton();
	m_pBtnClose->Create(_T("Close"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(x, MARGIN, x + BTN_W, MARGIN + BTN_H), this, IDC_BTN_CLOSE_KEYBOARD);
	return TRUE;
}

void DlgVirtualKeyboard::OnDestroy()
{
	// CButton은 delete하지 않음 (delete 시 CWnd 소멸자에서 크래시/Assert)
	// 포인터만 비워서 소멸자에서 이중 접근 방지
	m_keyButtons.clear();
	m_pBtnClose = nullptr;
	CDialogEx::OnDestroy();
}


// 아이폰 스타일 레이아웃: Row0 10키, Row1 9키, Row2 Shift+7+Back, Row3 123+Space+Return
void DlgVirtualKeyboard::CreateKeyButtons()
{
	const int KEY_W = 60;
	const int KEY_H = 66;
	const int GAP = 5;
	const int MARGIN = 10;
	const int SPECIAL_W = 60;   // Shift, Backspace
	const int ROW3_SPECIAL_W = 60;  // 123, Return

	CRect rcClient;
	GetClientRect(&rcClient);
	const int CW = rcClient.Width();
	const int CH = rcClient.Height();

	// 키 라벨 (아이폰 스타일: ⇧ Shift, ⌫ Back)
	static const TCHAR* s_labels[KEY_COUNT] = {
		_T("Q"), _T("W"), _T("E"), _T("R"), _T("T"), _T("Y"), _T("U"), _T("I"), _T("O"), _T("P"),
		_T("A"), _T("S"), _T("D"), _T("F"), _T("G"), _T("H"), _T("J"), _T("K"), _T("L"),
		_T("Shift"),
		_T("Z"), _T("X"), _T("C"), _T("V"), _T("B"), _T("N"), _T("M"),
		_T("Back"),
		_T("123"),
		_T("Space"),
		_T("Return")
	};

	int idx = 0;

	// Row 0: Q W E R T Y U I O P (10 keys)
	const int row0_total = 10 * KEY_W + 9 * GAP;
	int row0_left = (CW - row0_total) / 2;
	int y0 = MARGIN;
	for (int i = 0; i < 10 && idx < KEY_COUNT; i++, idx++)
	{
		int x = row0_left + i * (KEY_W + GAP);
		CButton* pBtn = new CButton();
		pBtn->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			CRect(x, y0, x + KEY_W, y0 + KEY_H), this, IDC_KEY_FIRST + idx);
		m_keyButtons.push_back(pBtn);
	}

	// Row 1: A S D F G H J K L (9 keys)
	const int row1_total = 9 * KEY_W + 8 * GAP;
	int row1_left = (CW - row1_total) / 2;
	int y1 = y0 + KEY_H + GAP;
	for (int i = 0; i < 9 && idx < KEY_COUNT; i++, idx++)
	{
		int x = row1_left + i * (KEY_W + GAP);
		CButton* pBtn = new CButton();
		pBtn->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			CRect(x, y1, x + KEY_W, y1 + KEY_H), this, IDC_KEY_FIRST + idx);
		m_keyButtons.push_back(pBtn);
	}

	// Row 2: Shift(SPECIAL_W) + Z X C V B N M + Backspace(SPECIAL_W)
	const int row2_total = SPECIAL_W + 7 * KEY_W + 7 * GAP + SPECIAL_W;
	int row2_left = (CW - row2_total) / 2;
	int y2 = y1 + KEY_H + GAP;
	// Shift
	CButton* pShift = new CButton();
	pShift->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(row2_left, y2, row2_left + SPECIAL_W, y2 + KEY_H), this, IDC_KEY_FIRST + idx);
	m_keyButtons.push_back(pShift);
	idx++;
	// Z~M
	for (int i = 0; i < 7 && idx < KEY_COUNT; i++, idx++)
	{
		int x = row2_left + SPECIAL_W + GAP + i * (KEY_W + GAP);
		CButton* pBtn = new CButton();
		pBtn->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			CRect(x, y2, x + KEY_W, y2 + KEY_H), this, IDC_KEY_FIRST + idx);
		m_keyButtons.push_back(pBtn);
	}
	// Backspace
	int back_x = row2_left + SPECIAL_W + GAP + 7 * (KEY_W + GAP);
	CButton* pBack = new CButton();
	pBack->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(back_x, y2, back_x + SPECIAL_W, y2 + KEY_H), this, IDC_KEY_FIRST + idx);
	m_keyButtons.push_back(pBack);
	idx++;

	// Row 3: 123 | Space | Return
	int y3 = y2 + KEY_H + GAP;
	int row3_123_x = MARGIN;
	int row3_return_w = ROW3_SPECIAL_W;
	int row3_return_x = CW - MARGIN - row3_return_w;
	int row3_space_x = row3_123_x + ROW3_SPECIAL_W + GAP;
	int row3_space_w = row3_return_x - GAP - row3_space_x;

	CButton* p123 = new CButton();
	p123->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(row3_123_x, y3, row3_123_x + ROW3_SPECIAL_W, y3 + KEY_H), this, IDC_KEY_FIRST + idx);
	m_keyButtons.push_back(p123);
	idx++;

	CButton* pSpace = new CButton();
	pSpace->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(row3_space_x, y3, row3_space_x + row3_space_w, y3 + KEY_H), this, IDC_KEY_FIRST + idx);
	m_keyButtons.push_back(pSpace);
	idx++;

	CButton* pReturn = new CButton();
	pReturn->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(row3_return_x, y3, row3_return_x + row3_return_w, y3 + KEY_H), this, IDC_KEY_FIRST + idx);
	m_keyButtons.push_back(pReturn);
}


// 넘패드 레이아웃: 전체 가운데, 7,8,9 / 4,5,6 / 1,2,3 / 0,.,Back / ABC(하단, 열 맞춤, 같은 높이)
void DlgVirtualKeyboard::CreateNumpadButtons()
{
	const int KEY_W = 80;
	const int KEY_H = 76;
	const int GAP = 6;
	const int MARGIN = 10;

	CRect rcClient;
	GetClientRect(&rcClient);
	const int CW = rcClient.Width();

	static const TCHAR* s_labels[NUMPAD_KEY_COUNT] = {
		_T("7"), _T("8"), _T("9"), _T("4"), _T("5"), _T("6"),
		_T("1"), _T("2"), _T("3"), _T("0"), _T("."), _T("Back"), _T("ABC")
	};

	// 3열 그리드 가운데 배치
	const int block_w = 3 * KEY_W + 2 * GAP;
	int left = (CW - block_w) / 2;
	int y = MARGIN;

	// Row 0~2: 7,8,9 / 4,5,6 / 1,2,3
	for (int row = 0; row < 3; row++)
	{
		for (int col = 0; col < 3; col++)
		{
			int idx = row * 3 + col;
			int x = left + col * (KEY_W + GAP);
			CButton* pBtn = new CButton();
			pBtn->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
				CRect(x, y, x + KEY_W, y + KEY_H), this, IDC_KEY_FIRST + idx);
			m_keyButtons.push_back(pBtn);
		}
		y += KEY_H + GAP;
	}

	// Row 3: ABC(0번 키 좌측), 0, ., Back
	int abc_x = left - KEY_W - GAP;
	if (abc_x < MARGIN) abc_x = MARGIN;
	CButton* pABC = new CButton();
	pABC->Create(s_labels[12], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(abc_x, y, abc_x + KEY_W, y + KEY_H), this, IDC_KEY_FIRST + 12);
	m_keyButtons.push_back(pABC);
	for (int col = 0; col < 3; col++)
	{
		int idx = 9 + col;  // 0, ., Back
		int x = left + col * (KEY_W + GAP);
		CButton* pBtn = new CButton();
		pBtn->Create(s_labels[idx], WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			CRect(x, y, x + KEY_W, y + KEY_H), this, IDC_KEY_FIRST + idx);
		m_keyButtons.push_back(pBtn);
	}
}


void DlgVirtualKeyboard::DestroyKeyButtons()
{
	for (CButton* pBtn : m_keyButtons)
	{
		if (pBtn->GetSafeHwnd())
			pBtn->DestroyWindow();
		delete pBtn;
	}
	m_keyButtons.clear();
	// 닫기 버튼은 그대로 둠 (다이얼로그와 함께 유지)
}


void DlgVirtualKeyboard::OnKeyPressed(UINT nID)
{
	if (nID < IDC_KEY_FIRST || nID > IDC_KEY_LAST)
		return;
	int index = nID - IDC_KEY_FIRST;

	// 숫자(넘패드) 모드
	if (m_bNumberMode)
	{
		if (index < 0 || index >= NUMPAD_KEY_COUNT)
			return;
		if (index == 11) { SendKeyInput(0, true, false); return; }   // Back
		if (index == 12)   // ABC → 문자 모드로 전환
		{
			m_bNumberMode = false;
			DestroyKeyButtons();
			CreateKeyButtons();
			return;
		}
		// 순서: 0=7,1=8,2=9, 3=4,4=5,5=6, 6=1,7=2,8=3, 9=0,10=.
		TCHAR ch = 0;
		if (index <= 2) ch = (TCHAR)(_T('7') + index);
		else if (index <= 5) ch = (TCHAR)(_T('4') + index - 3);
		else if (index <= 8) ch = (TCHAR)(_T('1') + index - 6);
		else if (index == 9) ch = _T('0');
		else if (index == 10) ch = _T('.');
		if (ch)
			SendKeyInput(ch, false, false);
		return;
	}

	// QWERTY 모드
	if (index < 0 || index >= KEY_COUNT)
		return;
	if (index == 27) { SendKeyInput(0, true, false); return; }   // Backspace
	if (index == 29) { SendKeyInput(_T(' '), false, false); return; }  // Space
	if (index == 30) { SendKeyInput(0, false, true); return; }  // Return
	if (index == 19) { m_bShiftState = !m_bShiftState; return; }  // Shift 토글
	if (index == 28)   // 123 → 넘패드 모드로 전환
	{
		m_bNumberMode = true;
		DestroyKeyButtons();
		CreateNumpadButtons();
		return;
	}

	// 문자키: Shift 상태에 따라 대/소문자
	TCHAR ch = 0;
	if (index <= 9)
		ch = _T("QWERTYUIOP")[index];
	else if (index <= 18)
		ch = _T("ASDFGHJKL")[index - 10];
	else if (index >= 20 && index <= 26)
		ch = _T("ZXCVBNM")[index - 20];
	if (ch)
	{
		if (!m_bShiftState)
			ch = (TCHAR)_totlower((int)ch);
		SendKeyInput(ch, false, false);
	}
}


void DlgVirtualKeyboard::SendKeyInput(WCHAR ch, bool isBackspace, bool isReturn)
{
	// 버튼 클릭 시 포커스가 키보드로 옮겨지므로, 미리 저장한 대상 컨트롤로 전송
	HWND hTarget = m_hwndTarget ? m_hwndTarget : ::GetFocus();
	if (!hTarget || !::IsWindow(hTarget))
		return;
	if (!isBackspace && !isReturn && ch == 0)
		return;

	if (isBackspace)
	{
		// 에디트에서 커서 앞 한 글자 삭제 (WM_KEYDOWN만으로는 동작 안 할 수 있음)
		DWORD start = 0, end = 0;
		::SendMessage(hTarget, EM_GETSEL, (WPARAM)&start, (LPARAM)&end);
		if (start > 0 && start == end)
		{
			::SendMessage(hTarget, EM_SETSEL, (WPARAM)(start - 1), (LPARAM)start);
			::SendMessage(hTarget, EM_REPLACESEL, TRUE, (LPARAM)_T(""));
		}
		else if (start != end)
		{
			::SendMessage(hTarget, EM_REPLACESEL, TRUE, (LPARAM)_T(""));
		}
	}
	else if (isReturn)
	{
		::SendMessage(hTarget, WM_KEYDOWN, VK_RETURN, 0);
		::SendMessage(hTarget, WM_KEYUP, VK_RETURN, 0);
	}
	else
		::SendMessage(hTarget, WM_CHAR, (WPARAM)ch, 0);
}


void DlgVirtualKeyboard::OnBnClickedOk()
{

}

void DlgVirtualKeyboard::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}

void DlgVirtualKeyboard::OnBnClickedClose()
{
	// 1) 키보드 창 숨기기
	ShowWindow(SW_HIDE);

	// 2) 포커스를 에디트가 아닌, 부모의 다른 컨트롤로 옮겨서
	//    EN_SETFOCUS(IDC_EDITx) 가 바로 다시 발생하지 않도록 한다.
	if (CWnd* pParent = GetParent())
	{
		// 설정 다이얼로그라면 저장/종료 버튼 쪽으로 포커스를 보내는 식으로 처리
		if (CWnd* pBtn = pParent->GetDlgItem(IDC_BTN_SAVE_AND_EXIT))
		{
			//pBtn->SetFocus();
		}
		else
		{
			//pParent->SetFocus();
		}	
	}
}
