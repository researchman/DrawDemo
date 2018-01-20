#pragma once

#include <map>
#include <vector>
#include "meta/point.h"
// GdiDraw �Ի���

class GdiDraw : public CDialogEx
{
	DECLARE_DYNAMIC(GdiDraw)

public:
	GdiDraw(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GdiDraw();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD =  };
#endif
public:
	std::vector<point::YPoint<double>> m_vtPoints;
	std::map<int, point::YPoint<double>> m_vtAllPointsData;

public:
	void DrawLineTest(CDC* pDC);
	void DrawArc(CDC* pDC);
	void DrawCircle(CDC* pDC);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
