#pragma once

#include <afxwin.h>
#include <memory>
#include <vector>
#include <mutex>
#include <string>

// 오브젝트 타입 정의
enum class DrawObjectType
{
	Rectangle,
	Circle,
	Line
};

// 그리기 오브젝트 구조체
struct DrawObject
{
	DrawObjectType type;
	CRect rect;           // 사각형이나 원의 경계
	CPoint startPoint;    // 선의 시작점
	CPoint endPoint;      // 선의 끝점
	COLORREF color;
	int penWidth;
	bool isFilled;
	
	DrawObject(DrawObjectType t, CRect r, COLORREF c = RGB(0, 0, 0), int pw = 1, bool filled = false)
		: type(t), rect(r), color(c), penWidth(pw), isFilled(filled) {}
	
	DrawObject(DrawObjectType t, CPoint start, CPoint end, COLORREF c = RGB(0, 0, 0), int pw = 1)
		: type(t), startPoint(start), endPoint(end), color(c), penWidth(pw), isFilled(false) {}
};

// 측정 지점 정보를 담는 구조체
struct MeasurementPoint
{
	CPoint positionMM;                    // 측정 지점의 실제 좌표 (mm 단위)
	double targetValue;                   // 목표 두께 값 (예: 45)
	double measuredValue;                 // 측정된 두께 값 (예: 50, 48), -1이면 미측정
	std::string statusText;               // "OK", "NG" 또는 빈 문자열
	COLORREF circleColor;                 // 원형 마커의 색상
	COLORREF textColor;                   // 텍스트의 색상
	int id;                               // 지점의 고유 ID (1, 2, 3, 4)
	bool hasMeasuredValue;                // 측정값이 있는지 여부
	bool isSelected;                      // 선택 상태
	
	// 생성자
	MeasurementPoint(int id, CPoint pos, double target, double measured = -1.0, const std::string& status = "")
		: id(id), positionMM(pos), targetValue(target), measuredValue(measured), statusText(status), 
		  hasMeasuredValue(measured >= 0), isSelected(false)
	{
		// 색상 결정
		if (hasMeasuredValue)
		{
			if (status == "OK")
			{
				circleColor = RGB(0, 255, 0);  // 녹색
				textColor = RGB(0, 150, 0);
			}
			else if (status == "NG")
			{
				circleColor = RGB(255, 0, 0);  // 빨간색
				textColor = RGB(200, 0, 0);
			}
			else
			{
				circleColor = RGB(128, 128, 128);  // 회색
				textColor = RGB(100, 100, 100);
			}
		}
		else
		{
			circleColor = RGB(128, 128, 128);  // 회색 (측정값 없음)
			textColor = RGB(100, 100, 100);
		}
	}
};

class CustomDraw : public CDialog
{
	DECLARE_DYNAMIC(CustomDraw)

public:
	CustomDraw();
	~CustomDraw();

	CustomDraw(const CustomDraw&) = delete;
	CustomDraw& operator=(const CustomDraw&) = delete;
	CustomDraw(CustomDraw&&) = delete;
	CustomDraw& operator=(CustomDraw&&) = delete;

	// 초기화 함수
	void InitializeDimensions(double widthMM, double heightMM);
	
	// 오브젝트 관리 함수
	void AddRectangle(double xMM, double yMM, double widthMM, double heightMM, COLORREF color = RGB(0, 0, 0), int penWidth = 1, bool filled = false);
	void AddCircle(double centerXMM, double centerYMM, double radiusMM, COLORREF color = RGB(0, 0, 0), int penWidth = 1, bool filled = false);
	void AddLine(double startXMM, double startYMM, double endXMM, double endYMM, COLORREF color = RGB(0, 0, 0), int penWidth = 1);
	void ClearObjects();
	
	// 측정 지점 관리 함수
	void AddMeasurementPoint(int id, double xMM, double yMM, double targetValue, 
							double measuredValue = -1.0, const std::string& status = "");
	void ClearAllMeasurementPoints();
	bool SetMeasurementPointValue(int id, double measuredValue, const std::string& status);

	
	// 표시 옵션
	void SetCircleRadius(int radius) { m_circleRadiusMeasurePoint = radius; Invalidate(); }
	void SetBackgroundColor(COLORREF color) { m_backgroundColor = color; Invalidate(); }
	COLORREF GetBackgroundColor() const { return m_backgroundColor; }

	void SetShowGuides(bool show) { m_showGuides = show; Invalidate(); }
	bool GetShowGuides() const { return m_showGuides; }
	
	// 선택 관련 함수
	void SelectMeasurementPoint(int index);
	void DeselectAllMeasurementPoints();
	int GetSelectedMeasurementPointIndex() const { return m_selectedMeasurementPointIndex; }
	
	// 좌표 변환 함수
	CPoint MMToPixel(double xMM, double yMM) const;
	CRect MMToPixel(double xMM, double yMM, double widthMM, double heightMM) const;
	double PixelToMM_X(int pixelX) const;
	double PixelToMM_Y(int pixelY) const;

private:
	// 그리기 관련 멤버 변수
	double m_widthMM;          // 실제 너비 (mm)
	double m_heightMM;         // 실제 높이 (mm)
	double m_scaleX;           // X축 스케일 (픽셀/mm)
	double m_scaleY;           // Y축 스케일 (픽셀/mm)
	CRect m_clientRect;        // 클라이언트 영역
	
	// 오브젝트 저장소
	std::vector<DrawObject> m_objects;
	
	// 측정 지점 저장소
	std::vector<MeasurementPoint> m_measurementPoints;
	mutable std::mutex m_measurementMutex;
	
	// 가이드라인 설정
	bool m_showGuides;
	int m_guideDivisions;      // 등분 수
	
	// 측정 지점 설정
	int m_circleRadiusMeasurePoint;        // 원형 마커의 반지름 (픽셀)
	
	// 배경 설정
	COLORREF m_backgroundColor; // 배경색
	
	// 선택 관련
	int m_selectedMeasurementPointIndex; // 선택된 측정 지점의 인덱스 (-1이면 선택 없음)
	
	// 내부 함수
	void drawGuides(CDC* pDC);
	void drawGuideLabels(CDC* pDC);
	void drawObjects(CDC* pDC);
	void drawMeasurementPoints(CDC* pDC);
	// 버튼클릭 지점이 측정 포인트인지 확인
	bool isPointInMeasurementPoint(const CPoint& point, const MeasurementPoint& mp) const;

protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};

