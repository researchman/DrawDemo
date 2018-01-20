#pragma once

#include <map>
#include <vector>
#include "meta/point.h"
// GdiDraw 对话框

class GdiDraw : public CDialogEx
{
	DECLARE_DYNAMIC(GdiDraw)

public:
	GdiDraw(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GdiDraw();

// 对话框数据
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
