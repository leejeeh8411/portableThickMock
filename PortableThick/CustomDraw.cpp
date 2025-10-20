#include "pch.h"
#include "CustomDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CustomDraw, CDialog)

CustomDraw::CustomDraw()
	: m_widthMM(0.0)
	, m_heightMM(0.0)
	, m_scaleX(1.0)
	, m_scaleY(1.0)
	, m_showGuides(true)
	, m_guideDivisions(10)
	, m_circleRadiusMeasurePoint(14)
	, m_backgroundColor(RGB(255, 255, 255))  // 기본 배경색: 흰색
	, m_selectedMeasurementPointIndex(-1)
{
}

CustomDraw::~CustomDraw() 
{
}

BEGIN_MESSAGE_MAP(CustomDraw, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CustomDraw::InitializeDimensions(double widthMM, double heightMM)
{
	m_widthMM = widthMM;
	m_heightMM = heightMM;
	
	// 클라이언트 영역 크기 가져오기
	GetClientRect(&m_clientRect);
	
	// 스케일 계산 (픽셀/mm)
	if (m_widthMM > 0 && m_heightMM > 0)
	{
		m_scaleX = static_cast<double>(m_clientRect.Width()) / m_widthMM;
		m_scaleY = static_cast<double>(m_clientRect.Height()) / m_heightMM;
	}
	
	// 화면 갱신
	Invalidate();
}

void CustomDraw::AddRectangle(double xMM, double yMM, double widthMM, double heightMM, COLORREF color, int penWidth, bool filled)
{
	CRect pixelRect = MMToPixel(xMM, yMM, widthMM, heightMM);
	m_objects.emplace_back(DrawObjectType::Rectangle, pixelRect, color, penWidth, filled);
	Invalidate();
}

void CustomDraw::AddCircle(double centerXMM, double centerYMM, double radiusMM, COLORREF color, int penWidth, bool filled)
{
	CPoint center = MMToPixel(centerXMM, centerYMM);
	int radiusPixels = static_cast<int>(radiusMM * m_scaleX);
	
	CRect circleRect(center.x - radiusPixels, center.y - radiusPixels, 
					 center.x + radiusPixels, center.y + radiusPixels);
	m_objects.emplace_back(DrawObjectType::Circle, circleRect, color, penWidth, filled);
	Invalidate();
}

void CustomDraw::AddLine(double startXMM, double startYMM, double endXMM, double endYMM, COLORREF color, int penWidth)
{
	CPoint start = MMToPixel(startXMM, startYMM);
	CPoint end = MMToPixel(endXMM, endYMM);
	m_objects.emplace_back(DrawObjectType::Line, start, end, color, penWidth);
	Invalidate();
}

void CustomDraw::ClearObjects()
{
	m_objects.clear();
	Invalidate();
}

void CustomDraw::SelectMeasurementPoint(int index)
{
	// 이전 선택 해제
	if (m_selectedMeasurementPointIndex >= 0 && m_selectedMeasurementPointIndex < static_cast<int>(m_measurementPoints.size()))
	{
		m_measurementPoints[m_selectedMeasurementPointIndex].isSelected = false;
	}
	
	// 새 선택
	if (index >= 0 && index < static_cast<int>(m_measurementPoints.size()))
	{
		m_measurementPoints[index].isSelected = true;
		m_selectedMeasurementPointIndex = index;
	}
	else
	{
		m_selectedMeasurementPointIndex = -1;
	}
	
	Invalidate();
}

void CustomDraw::DeselectAllMeasurementPoints()
{
	if (m_selectedMeasurementPointIndex >= 0 && m_selectedMeasurementPointIndex < static_cast<int>(m_measurementPoints.size()))
	{
		m_measurementPoints[m_selectedMeasurementPointIndex].isSelected = false;
	}
	m_selectedMeasurementPointIndex = -1;
	Invalidate();
}

bool CustomDraw::isPointInMeasurementPoint(const CPoint& point, const MeasurementPoint& mp) const
{
    // mm 좌표를 픽셀 좌표로 변환
    CPoint pixelPos = MMToPixel(mp.positionMM.x, mp.positionMM.y);
	
	// 원형 영역 내부인지 확인
	int dx = point.x - pixelPos.x;
	int dy = point.y - pixelPos.y;
	int radius = m_circleRadiusMeasurePoint + 5; // 클릭 영역을 약간 크게
	
	return (dx * dx + dy * dy) <= (radius * radius);
}

void CustomDraw::AddMeasurementPoint(int id, double xMM, double yMM, double targetValue, 
									double measuredValue, const std::string& status)
{
    CPoint positionMM(static_cast<int>(xMM), static_cast<int>(yMM));
    {
        std::lock_guard<std::mutex> lock(m_measurementMutex);
        m_measurementPoints.emplace_back(id, positionMM, targetValue, measuredValue, status);
    }
    Invalidate();
}

void CustomDraw::ClearAllMeasurementPoints()
{
    {
        std::lock_guard<std::mutex> lock(m_measurementMutex);
        m_measurementPoints.clear();
        m_selectedMeasurementPointIndex = -1;  // 선택 해제
    }
	Invalidate();
}

bool CustomDraw::SetMeasurementPointValue(int id, double measuredValue, const std::string& status)
{
    bool found = false;
    {
        std::lock_guard<std::mutex> lock(m_measurementMutex);
        for (auto& point : m_measurementPoints)
        {
            if (point.id == id)
            {
                point.measuredValue = measuredValue;
                point.statusText = status;
                point.hasMeasuredValue = (measuredValue >= 0);

                // 색상 재계산
                if (point.hasMeasuredValue)
                {
                    if (status == "OK")
                    {
                        point.circleColor = RGB(0, 255, 0);
                        point.textColor = RGB(0, 150, 0);
                    }
                    else if (status == "NG")
                    {
                        point.circleColor = RGB(255, 0, 0);
                        point.textColor = RGB(200, 0, 0);
                    }
                    else
                    {
                        point.circleColor = RGB(128, 128, 128);
                        point.textColor = RGB(100, 100, 100);
                    }
                }
                else
                {
                    point.circleColor = RGB(128, 128, 128);
                    point.textColor = RGB(100, 100, 100);
                }
                found = true;
                break;
            }
        }
    }
    if (found) Invalidate();
    return found;
}


CPoint CustomDraw::MMToPixel(double xMM, double yMM) const
{
	int x = static_cast<int>(xMM * m_scaleX);
	int y = static_cast<int>(yMM * m_scaleY);
	return CPoint(x, y);
}

CRect CustomDraw::MMToPixel(double xMM, double yMM, double widthMM, double heightMM) const
{
	CPoint topLeft = MMToPixel(xMM, yMM);
	CPoint bottomRight = MMToPixel(xMM + widthMM, yMM + heightMM);
	return CRect(topLeft, bottomRight);
}

double CustomDraw::PixelToMM_X(int pixelX) const
{
	return static_cast<double>(pixelX) / m_scaleX;
}

double CustomDraw::PixelToMM_Y(int pixelY) const
{
	return static_cast<double>(pixelY) / m_scaleY;
}

void CustomDraw::drawGuides(CDC* pDC)
{
	if (!m_showGuides || m_widthMM <= 0 || m_heightMM <= 0)
		return;
	
	// 점선 펜 생성
	CPen pen(PS_DOT, 1, RGB(200, 200, 200));
	CPen* oldPen = pDC->SelectObject(&pen);
	
	// 가로 가이드라인 그리기
	for (int i = 1; i < m_guideDivisions; ++i)
	{
		int y = static_cast<int>((static_cast<double>(i) / m_guideDivisions) * m_clientRect.Height());
		pDC->MoveTo(0, y);
		pDC->LineTo(m_clientRect.Width(), y);
	}
	
	// 세로 가이드라인 그리기
	for (int i = 1; i < m_guideDivisions; ++i)
	{
		int x = static_cast<int>((static_cast<double>(i) / m_guideDivisions) * m_clientRect.Width());
		pDC->MoveTo(x, 0);
		pDC->LineTo(x, m_clientRect.Height());
	}
	
	pDC->SelectObject(oldPen);
}

void CustomDraw::drawGuideLabels(CDC* pDC)
{
	if (!m_showGuides || m_widthMM <= 0 || m_heightMM <= 0)
		return;
	
	// 폰트 설정
	CFont font;
	font.CreateFont(12, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_SWISS, L"Arial");
	CFont* oldFont = pDC->SelectObject(&font);
	
	pDC->SetTextColor(RGB(100, 100, 100));
	pDC->SetBkMode(TRANSPARENT);
	
	// 가로 라벨 (Y축 값)
	for (int i = 0; i <= m_guideDivisions; ++i)
	{
		int y = static_cast<int>((static_cast<double>(i) / m_guideDivisions) * m_clientRect.Height());
		double valueMM = m_heightMM - (static_cast<double>(i) / m_guideDivisions) * m_heightMM;
		
		CString str;
		str.Format(L"%.1f", valueMM);
		
		CSize textSize = pDC->GetTextExtent(str);
		pDC->TextOut(5, y - textSize.cy / 2, str);
	}
	
	// 세로 라벨 (X축 값)
	for (int i = 0; i <= m_guideDivisions; ++i)
	{
		int x = static_cast<int>((static_cast<double>(i) / m_guideDivisions) * m_clientRect.Width());
		double valueMM = (static_cast<double>(i) / m_guideDivisions) * m_widthMM;
		
		CString str;
		str.Format(L"%.1f", valueMM);
		
		CSize textSize = pDC->GetTextExtent(str);
		pDC->TextOut(x - textSize.cx / 2, m_clientRect.Height() - 20, str);
	}
	
	pDC->SelectObject(oldFont);
}

void CustomDraw::drawObjects(CDC* pDC)
{
	for (const auto& obj : m_objects)
	{
		CPen pen(PS_SOLID, obj.penWidth, obj.color);
		CPen* oldPen = pDC->SelectObject(&pen);
		
		switch (obj.type)
		{
		case DrawObjectType::Rectangle:
			if (obj.isFilled)
			{
				CBrush brush(obj.color);
				CBrush* oldBrush = pDC->SelectObject(&brush);
				pDC->Rectangle(obj.rect);
				pDC->SelectObject(oldBrush);
			}
			else
			{
				pDC->Rectangle(obj.rect);
			}
			break;
			
		case DrawObjectType::Circle:
			if (obj.isFilled)
			{
				CBrush brush(obj.color);
				CBrush* oldBrush = pDC->SelectObject(&brush);
				pDC->Ellipse(obj.rect);
				pDC->SelectObject(oldBrush);
			}
			else
			{
				pDC->Ellipse(obj.rect);
			}
			break;
			
		case DrawObjectType::Line:
			pDC->MoveTo(obj.startPoint);
			pDC->LineTo(obj.endPoint);
			break;
		}
		
		pDC->SelectObject(oldPen);
	}
}

void CustomDraw::drawMeasurementPoints(CDC* pDC)
{
	
	// 폰트 설정
	CFont font;
	font.CreateFont(10, 0, 0, 0, FW_NORMAL, FALSE, FALSE, 0, ANSI_CHARSET,
					OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
					DEFAULT_PITCH | FF_SWISS, L"Arial");
	CFont* oldFont = pDC->SelectObject(&font);
	
    // 안전한 스냅샷을 만들어 잠금 시간을 최소화
    std::vector<MeasurementPoint> snapshot;
    {
        std::lock_guard<std::mutex> lock(m_measurementMutex);
        snapshot = m_measurementPoints;
    }

    for (const auto& point : snapshot)
	{
		// mm 좌표를 픽셀 좌표로 변환
		CPoint pixelPos = MMToPixel(point.positionMM.x, point.positionMM.y);
		
		// 선택된 측정 지점은 파란색으로 그리기
		COLORREF drawColor = point.isSelected ? RGB(0, 0, 255) : point.circleColor;
		
        // 외곽 원(링)과 내부 동심원 계산
        CRect circleRect(pixelPos.x - m_circleRadiusMeasurePoint, pixelPos.y - m_circleRadiusMeasurePoint,
                        pixelPos.x + m_circleRadiusMeasurePoint, pixelPos.y + m_circleRadiusMeasurePoint);

        // 외곽 원: 두께감 있는 빈 원(링)
        int ringThickness = 2; // 펜 두께
        CPen ringPen(PS_SOLID, ringThickness, drawColor);
        CPen* oldPen = pDC->SelectObject(&ringPen);
        HGDIOBJ oldNullBrush = pDC->SelectStockObject(NULL_BRUSH);
        pDC->Ellipse(circleRect);
        // 원복
        pDC->SelectObject(oldNullBrush);
        pDC->SelectObject(oldPen);

        // 내부 동심원: solid 채움
        int innerRadius = max(2, m_circleRadiusMeasurePoint / 2.5);
        CRect innerRect(pixelPos.x - innerRadius, pixelPos.y - innerRadius,
                        pixelPos.x + innerRadius, pixelPos.y + innerRadius);
        CBrush innerBrush(drawColor);
        CBrush* prevBrush = pDC->SelectObject(&innerBrush);
        CPen innerPen(PS_SOLID, 1, drawColor);
        CPen* prevPen = pDC->SelectObject(&innerPen);
        pDC->Ellipse(innerRect);
        pDC->SelectObject(prevBrush);
        pDC->SelectObject(prevPen);
		
        // 텍스트 생성: 본문은 검정, 상태(OK/NG)만 색상 적용
        CString prefix;
        CString statusStr = CString(point.statusText.c_str());
        if (point.hasMeasuredValue)
        {
            prefix.Format(L"%d. %.0f / %.0f um, ", point.id, point.targetValue, point.measuredValue);
        }
        else
        {
            prefix.Format(L"%d. %.0f / um", point.id, point.targetValue);
            statusStr.Empty();
        }

        // 총 폭 계산(중앙 정렬 위해)
        pDC->SetBkMode(TRANSPARENT);
        CSize prefixSize = pDC->GetTextExtent(prefix);
        CSize statusSize = statusStr.IsEmpty() ? CSize(0, 0) : pDC->GetTextExtent(statusStr);
        int totalWidth = prefixSize.cx + statusSize.cx;
        int textY = pixelPos.y + m_circleRadiusMeasurePoint + 5;
        int startX = pixelPos.x - totalWidth / 2;

        // 본문(검정)
        pDC->SetTextColor(RGB(0, 0, 0));
        pDC->TextOut(startX, textY, prefix);

        // 상태 텍스트(색상 적용)
        if (!statusStr.IsEmpty())
        {
            COLORREF statusColor = RGB(0, 0, 0);
            if (statusStr.CompareNoCase(L"OK") == 0)
                statusColor = RGB(0, 200, 0);
            else if (statusStr.CompareNoCase(L"NG") == 0)
                statusColor = RGB(200, 0, 0);

            pDC->SetTextColor(statusColor);
            pDC->TextOut(startX + prefixSize.cx, textY, statusStr);
        }
	}
	
	pDC->SelectObject(oldFont);
}

void CustomDraw::OnLButtonDown(UINT nFlags, CPoint point)
{
	// 마우스 클릭 위치에서 측정 지점 찾기 (뒤에서부터 검사하여 위에 있는 지점 우선)
    int foundIndex = -1;
    // 잠금 범위를 최소화하기 위해 스냅샷
    std::vector<MeasurementPoint> snapshot;
    {
        std::lock_guard<std::mutex> lock(m_measurementMutex);
        snapshot = m_measurementPoints;
    }
    for (int i = static_cast<int>(snapshot.size()) - 1; i >= 0; --i)
    {
        if (isPointInMeasurementPoint(point, snapshot[i]))
        {
            foundIndex = i;
            break;
        }
    }
	
	// 측정 지점 선택
	SelectMeasurementPoint(foundIndex);
	
	CDialog::OnLButtonDown(nFlags, point);
}

void CustomDraw::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);
}

void CustomDraw::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 위치를 mm 단위로 변환하여 표시할 수 있음
	double xMM = PixelToMM_X(point.x);
	double yMM = PixelToMM_Y(point.y);
	
	// TODO: 툴팁이나 상태바에 좌표 표시
}

BOOL CustomDraw::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0) {
		// 줌 인
	}
	else {
		// 줌 아웃
	}

	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CustomDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	// 설정된 배경색으로 칠하기
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, m_backgroundColor);
	
	// 테두리 그리기
	dc.Draw3dRect(&rect, RGB(128, 128, 128), RGB(192, 192, 192));
	
	// 가이드라인 그리기
	drawGuides(&dc);
	
	// 가이드라인 라벨 그리기
	drawGuideLabels(&dc);
	
	// 오브젝트들 그리기
	drawObjects(&dc);
	
	// 측정 지점들 그리기
	drawMeasurementPoints(&dc);
}