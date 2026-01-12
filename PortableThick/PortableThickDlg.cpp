
// PortableThickDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "PortableThick.h"
#include "PortableThickDlg.h"
#include "afxdialogex.h"
#include "DlgSetting.h"
#include <random>
#include <chrono>
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

// 랜덤 데이터 생성 함수
std::vector<ChartDataPoint> GenerateRandomData(int count, double minX, double maxX, double minY, double maxY) {
	std::vector<ChartDataPoint> data;

	// 랜덤 시드 설정 (현재 시간 기반)
	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	std::mt19937 gen(seed);

	// X값은 균등하게 분포
	std::uniform_real_distribution<double> xDist(minX, maxX);
	// Y값은 정규 분포 (평균값 주변에 더 많이 분포)
	std::normal_distribution<double> yDist((minY + maxY) / 2.0, (maxY - minY) / 6.0);

	for (int i = 0; i < count; ++i) {
		ChartDataPoint point;
		point.x = minX + i * (maxX - minX) / (count - 1); // X는 균등 분포
		point.y = yDist(gen); // Y는 정규 분포

		// Y값이 범위를 벗어나면 클램핑
		if (point.y < minY) point.y = minY;
		if (point.y > maxY) point.y = maxY;

		data.push_back(point);
	}

	return data;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CPortableThickDlg 대화 상자



CPortableThickDlg::CPortableThickDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PORTABLETHICK_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPortableThickDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);


	// label
	DDX_Control(pDX, IDC_TITLE_NAME, _labelTitleName);
	DDX_Control(pDX, IDC_MACHINE_STATUS, _labelMachineStatus);
	DDX_Control(pDX, IDC_MOTION_POS, _labelMotionPos);
	DDX_Control(pDX, IDC_THICKNESS_VALUE, _labelThickValue);
	DDX_Control(pDX, IDC_LED_MOTOR, _labelLEDMotor);
	DDX_Control(pDX, IDC_LED_IO, _labelLEDIO);
	DDX_Control(pDX, IDC_LED_SENSOR, _labelLEDSensor);
	DDX_Control(pDX, IDC_LED_START, _labelLEDStart);

	// btn
	DDX_Control(pDX, IDC_BTN_HISTORY, _iBtnHistory);
	DDX_Control(pDX, IDC_BTN_PROGRAM_EXIT, _iBtnProgramExit);
	DDX_Control(pDX, IDC_BTN_CAL, _iBtnCal);
	DDX_Control(pDX, IDC_BTN_READY, _iBtnReady);
	DDX_Control(pDX, IDC_BTN_RESET, _iBtnReset);
	DDX_Control(pDX, IDC_BTN_START, _iBtnStart);
	DDX_Control(pDX, IDC_BTN_STOP_MOTION, _iBtnStopMotion);
	DDX_Control(pDX, IDC_BTN_SETTING, _iBtnSetting);
	DDX_Control(pDX, IDC_BTN_SAVE_DATA, _iBtnSaveData);
	DDX_Control(pDX, IDC_BTN_DEL_SEL, _iBtnDelSel);
	DDX_Control(pDX, IDC_BTN_DEL_ALL, _iBtnDelAll);

	// grid
	DDX_Control(pDX, IDC_LIST_THICK2, _gridListThick);

	// chart
	//DDX_Control(pDX, IDC_CHART, _vChart);

	// CustomDraw
	DDX_Control(pDX, IDC_CHART, _customDraw);

	//DDX_Control(pDX, IDC_CHART, _vImage);

}

BEGIN_MESSAGE_MAP(CPortableThickDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SETTING, &CPortableThickDlg::OnBnClickedBtnSetting)
END_MESSAGE_MAP()


// CPortableThickDlg 메시지 처리기

BOOL CPortableThickDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	InitLayout();
	InitGrid();
	//InitChart();
	InitCustomDraw();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CPortableThickDlg::InitLayout()
{
	InitLabel(_labelTitleName, L"포터블 두께 측정기", eLabelAlignH::Center, eLabelAlignV::Center, 38, RGB(255, 255, 255), RGB_WEAK_DEEP_GRAY, L"", 0);

	InitLabel(_labelMachineStatus, L"측정 대기 중", eLabelAlignH::Center, eLabelAlignV::Center, 30, RGB(255, 255, 255), RGB_DARK_GREEN, L"", 0);

	InitLabel(_labelMotionPos, L"실시간 모션 위치", eLabelAlignH::Center, eLabelAlignV::Center, 30, RGB(255, 255, 255), RGB_LIGHT_GRAY, L"", 0);

	InitLabel(_labelThickValue, L"평균 두께 : 0.0 um", eLabelAlignH::Center, eLabelAlignV::Center, 30, RGB(255, 255, 255), RGB_LIGHT_GRAY, L"", 0);

	InitLabel(_labelLEDMotor, L"모터", eLabelAlignH::Center, eLabelAlignV::Center, 20, RGB(255, 255, 255), RGB_DARK_GREEN, L"", 0);

	InitLabel(_labelLEDIO, L"I/O Board", eLabelAlignH::Center, eLabelAlignV::Center, 20, RGB(255, 255, 255), RGB_DARK_GREEN, L"", 0);

	InitLabel(_labelLEDSensor, L"측정 센서", eLabelAlignH::Center, eLabelAlignV::Center, 20, RGB(255, 255, 255), RGB_DARK_GREEN, L"", 0);

	InitLabel(_labelLEDStart, L"인터락", eLabelAlignH::Center, eLabelAlignV::Center, 20, RGB(255, 255, 255), RGB_DARK_GREEN, L"", 0);

	InitIconButton(_iBtnHistory, L"이력 조회", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnProgramExit, L"프로그램 종료", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnCal, L"영점 조정", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnReady, L"측정 준비", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnReset, L"재 준비", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnStart, L"측정 시작", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnStopMotion, L"모션 정지", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnSetting, L"셋팅", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnSaveData, L"평균 데이터 저장", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnDelSel, L"선택 삭제", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
	InitIconButton(_iBtnDelAll, L"전체 삭제", L"", 28, true, RGB(255, 255, 255), BUTTON_DOWN_RGB_GRAY, RGB_WEAK_DEEP_GRAY);
}

void CPortableThickDlg::InitLabel(vLabel& label, const std::wstring& text, eLabelAlignH hAlign, eLabelAlignV vAlign, int fontSize, COLORREF fontColor, COLORREF bgColor, const std::wstring& iconPath, int iconMargin)
{
	std::wstring wfont = L"SUIT";

	//폰트 및 얼라인 설정
	label.SetTextType(fontSize, fontColor, wfont, eLabelAlignH::Center, eLabelAlignV::Center);
	label.SetTransparent(false);
	label.SetBackground(bgColor);
	label.SetText(text.c_str());

	//아이콘 경로가 존재할때 얼라인
	if (!iconPath.empty()) {
		label.SetIconAlign(eLabelAlignH::Center, eLabelAlignV::Center);
		label.SetIconMargin(iconMargin);
		label.SetPngIcon(iconPath.c_str());
	}

	label.Draw();
}

void CPortableThickDlg::InitChart()
{
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	_vChart.SetChartType(eChartType::Line);
	_vChart.GetSettings().SetTitle(L"Thick Profile");
	_vChart.GetSettings().SetTitleFontSize(18);
	_vChart.GetSettings().SetTitleFont(L"consolas");
	_vChart.GetSettings().SetXAxisTitle(L"시간");
	_vChart.GetSettings().SetYAxisTitle(L"값");
	_vChart.GetSettings().SetChartWidth(750);
	_vChart.GetSettings().SetChartHeight(528);
	_vChart.GetSettings().SetBackgroundColor(RGB(240, 240, 240));
	_vChart.GetSettings().SetChartFont(L"consolas");
	_vChart.GetSettings().SetChartFontSize(10);
	_vChart.GetSettings().SetPlotAreaColor(RGB(240, 240, 240));

	// 첫 번째 시리즈 - 100개 랜덤 데이터 (X: 0~100, Y: 50~150)
	ChartSeries series1;
	series1.title = "Data";
	series1.color = RGB_DARK_GREEN; // 빨간색
	series1.data = GenerateRandomData(100, 0, 100, 20, 40);
	_vChart.AddSeries(series1);


	_vChart.DrawChart();
}

void CPortableThickDlg::InitIconButton(vIconButton& btn, const std::wstring& text, const std::wstring& iconPath, int fontSize, bool isFocus, COLORREF fontColor, COLORREF bgColor, COLORREF bgFocusColor)
{
	std::wstring wfont = L"SUIT";

	btn.SetButtonType(eButtonType::kNormal);
	//btn.SetPngIcon(eBgType::kBgNormal, iconPath.c_str());
	btn.SetBackground(eBgType::kBgNormal, bgColor);

	if (isFocus)
		btn.SetBackground(eBgType::kBgFocus, bgFocusColor);

	//btn.SetIconAlign(eIconButtonAlignH::Left, eIconButtonAlignV::Center);
	//btn.SetIconMargin(10);
	btn.SetTextType(fontSize, fontColor, wfont, eIconButtonAlignH::Center, eIconButtonAlignV::Center);
	btn.SetTextOffset(0, 0);
	btn.SetText(text.c_str());

}

void CPortableThickDlg::InitGrid()
{
	const int cols = 4; // 그룹, 키, 값, 설명
	const int headerRows = 1;

	_gridListThick.SetEditable(TRUE);
	_gridListThick.SetRowCount(headerRows);
	_gridListThick.SetColumnCount(cols);
	_gridListThick.SetFixedRowCount(1);
	_gridListThick.SetFixedColumnCount(0);

	// 헤더
	const CStringW headers[cols] = { L"Count", L"Thickness Data(um)", L"값", L"설명" };
	for (int c = 0; c < cols; ++c)
	{
		GV_ITEM it{};
		it.mask = GVIF_TEXT | GVIF_FORMAT;
		it.row = 0; it.col = c;
		it.nFormat = DT_CENTER | DT_VCENTER | DT_SINGLELINE;
		it.strText = headers[c];
		_gridListThick.SetItem(&it);
		_gridListThick.SetItemState(0, c, _gridListThick.GetItemState(0, c) | GVIS_READONLY);
		_gridListThick.SetItemBkColour(0, c, RGB(230, 230, 230));
	}
}

void CPortableThickDlg::InitCustomDraw()
{
	// CustomDraw 초기화
	double width = 400.0;
	double height = 400.0;
	// 100mm x 100mm 영역
	_customDraw.InitializeDimensions(width, height);
	
	// 배경색 설정 (연한 회색)
	_customDraw.SetBackgroundColor(RGB(10, 10, 10));

	// 가이드선 표시 유무
	_customDraw.SetShowGuides(false);
	
	// 예제 오브젝트들 추가
	// 사각형 추가 (10mm, 10mm 위치에 20mm x 15mm 크기)
	//_customDraw.AddRectangle(10.0, 10.0, 20.0, 15.0, RGB(255, 0, 0), 2, true);
	
	// 원 추가 (50mm, 50mm 중심에 반지름 10mm)
	double centerPosX = 200.0;
	double centerPosY = 200.0;
	double radius = 100.0;
	_customDraw.AddCircle(centerPosX, centerPosY, radius, RGB(180, 180, 180), 2, true);
	
	// 선 추가 (0mm, 0mm에서 100mm, 100mm까지)
	//_customDraw.AddLine(0.0, 0.0, 100.0, 100.0, RGB(0, 0, 255), 1);
	
	// 측정 지점들 추가 (그림에 맞게 배치)
	double target = 45.0;
	_customDraw.AddMeasurementPoint(1, 135.0, 150.0, target, 50.0, "NG"); 
	_customDraw.AddMeasurementPoint(2, 250.0, 125.0, target, 48.0, "OK"); 
	_customDraw.AddMeasurementPoint(3, 250.0, 275.0, target);             
	_customDraw.AddMeasurementPoint(4, 165.0, 250.0, target);             
}


void CPortableThickDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CPortableThickDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CPortableThickDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


HBRUSH CPortableThickDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	if (nCtlColor == CTLCOLOR_DLG)
	{
		static CBrush brush(RGB_WEAK_DEEP_GRAY);
		return brush;
	}

	return CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CPortableThickDlg::OnBnClickedBtnSetting()
{
	DlgSetting dlg;
	dlg.DoModal();
}
