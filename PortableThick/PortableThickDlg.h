
// PortableThickDlg.h: 헤더 파일
//

#pragma once

#include "vLabel.h"
#include "vIconButton.h"
#include "vGridCtrl.h"
#include "vChart.h"

const COLORREF RGB_WEAK_DEEP_GRAY = RGB(25, 30, 34);
const COLORREF RGB_DARK_GRAY = RGB(22, 27, 30);
const COLORREF RGB_LIGHT_GRAY = RGB(52, 57, 50);

const COLORREF RGB_DARK_GREEN = RGB(28, 162, 77);
const COLORREF RGB_DARK_RED = RGB(231, 81, 84);

const COLORREF BUTTON_DOWN_RGB_GRAY = RGB(35, 40, 44);

// CPortableThickDlg 대화 상자
class CPortableThickDlg : public CDialogEx
{
// 생성입니다.
public:
	CPortableThickDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PORTABLETHICK_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.

	

// 구현입니다.
protected:
	HICON m_hIcon;

	vLabel _labelTitleName;
	vLabel _labelMachineStatus;
	vLabel _labelMotionPos;
	vLabel _labelThickValue;
	vLabel _labelLEDMotor;
	vLabel _labelLEDIO;
	vLabel _labelLEDSensor;
	vLabel _labelLEDStart;

	vIconButton _iBtnHistory;
	vIconButton _iBtnProgramExit;
	vIconButton _iBtnCal;
	vIconButton _iBtnReady;
	vIconButton _iBtnReset;
	vIconButton _iBtnStart;
	vIconButton _iBtnStopMotion;
	vIconButton _iBtnSetting;
	vIconButton _iBtnSaveData;
	vIconButton _iBtnDelSel;
	vIconButton _iBtnDelAll;

	vGridCtrl  _gridListThick;
	vChart _vChart;

	void InitLayout();
	void InitLabel(vLabel& label, const std::wstring& text, eLabelAlignH hAlign, eLabelAlignV vAlign, int fontSize, COLORREF fontColor, COLORREF bgColor, const std::wstring& iconPath, int iconMargin);
	void InitGrid();
	void InitChart();
	void InitIconButton(vIconButton& btn, const std::wstring& text, const std::wstring& iconPath, int fontSize, bool isFocus, COLORREF fontColor, COLORREF bgColor, COLORREF bgFocusColor);

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
