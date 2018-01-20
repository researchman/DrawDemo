// GdiDraw.cpp : 实现文件
//

#include "stdafx.h"
#include "DrawDemo.h"
#include "GdiDraw.h"
#include "afxdialogex.h"
#include "string/facility.h"
#include <math.h>

// GdiDraw 对话框
const double POS_RATE = 62.62475;
const int ORG_X = 782;
const int ORG_Y = 581;

#define M_PI 3.14159265358979323846

IMPLEMENT_DYNAMIC(GdiDraw, CDialogEx)

GdiDraw::GdiDraw(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_GDI_DRAW, pParent)
{

}

GdiDraw::~GdiDraw()
{
}

void GdiDraw::DrawLineTest(CDC * pDC)
{
	int cx = GetDeviceCaps(pDC->m_hDC, HORZRES);
	int cy = GetDeviceCaps(pDC->m_hDC, VERTRES);

	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pDC, cx, cy);
	CDC MemDC;//创建在内存位图绘制的DC
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(MemBmp);//将内存位图选到内存DC中
	Gdiplus::Graphics gs(MemDC.GetSafeHdc());//根据内存DC创建GDI+的Graphics对象
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//设置绘图质量

	Gdiplus::Pen drawPen2(Gdiplus::Color(255, 50, 150, 255));

	//example1:
	//Gdiplus::Point points[3];
	//points[0].X = 100;
	//points[0].Y = 100;
	//points[1].X = 300;
	//points[1].Y = 300;
	//points[2].X = 200;
	//points[2].Y = 400;
	///*gs.DrawLine(&drawPen2,100,100,300,300,);
	//gs.DrawLine(&drawPen2, 300, 300, 200, 400);*/
	//float tension = 1.0;
	//gs.DrawCurve(&drawPen2, points, 3, tension);

	gs.DrawArc(&drawPen2, 414, 447, 160,160, 30, 90);

	gs.DrawArc(&drawPen2, 100, 100, 160, 160, 30, 270);


	pDC->BitBlt(0, 0, cx, cy, &MemDC, 0, 0, SRCCOPY);
	gs.ReleaseHDC(MemDC.GetSafeHdc());
	MemDC.DeleteDC();
	MemBmp.DeleteObject();
}

void GdiDraw::DrawArc(CDC * pDC)
{
	//构造数据
	while (m_vtAllPointsData.size() > 0)
	{
		auto it = m_vtAllPointsData.begin();
		m_vtPoints.push_back((it->second));
		it = m_vtAllPointsData.erase(it);
	}

	//坐标数据转像素
	for (auto it = m_vtPoints.begin(); it != m_vtPoints.end(); ++it)
	{
		it->x = abs(-1.0 * it->x * POS_RATE - ORG_X);
		it->y = abs(-1.0 * it->y * POS_RATE - ORG_Y);
	}

	int cx = GetDeviceCaps(pDC->m_hDC, HORZRES);
	int cy = GetDeviceCaps(pDC->m_hDC, VERTRES);

	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pDC, cx, cy);
	CDC MemDC;//创建在内存位图绘制的DC
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(MemBmp);//将内存位图选到内存DC中
	Gdiplus::Graphics gs(MemDC.GetSafeHdc());//根据内存DC创建GDI+的Graphics对象
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//设置绘图质量

	Gdiplus::Pen drawPen2(Gdiplus::Color(255, 50, 150, 255));

	double average_x = 0.0, average_y = 0.0;
	double average_r = 0.0;
	//绘制所有点
	for (size_t i = 0; i < m_vtPoints.size(); i++)
	{
		double start_angle = 0.0;
		double end_angle = 0.0;
		double sweep_angle = 0.0;
		if (!utility::geometry_facility::IsOnStraightLine(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2]))	//三点不在一条直线上
		{
			double radius = 0.0;
			point::YPoint<double> p = utility::geometry_facility::Circle(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2], radius);

			if (radius < 0)
			{
				continue;
			}
			//弧度转角度
			start_angle = asin((m_vtPoints[i + 1].y - p.y) / radius) * 180 / M_PI;
			end_angle = asin((m_vtPoints[i + 2].y - p.y) / radius) * 180 / M_PI;

			sweep_angle = start_angle - end_angle;

			int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
			start_x = p.x - radius;
			start_y = p.y - radius;
			end_x = p.x + radius;
			end_y = p.y + radius;

			//gs.DrawArc(&drawPen2, 0, 0, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			gs.DrawArc(&drawPen2, start_x, start_y, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			//gs.DrawArc(&drawPen2, 0, 0, m_vtPoints[i + 1].x, m_vtPoints[i + 1].y, start_angle, sweep_angle);

			// 方法二
			// 根据角度差平分出两个点，利用平分出的两点做为控制点，使用贝塞
			//TRACE(_T("i = :%d, x0 = %.2f, y0 = %.2f, radius = %.2f \r\n"),i,p.x,p.y,radius);

			//方法三
			//求圆心的平均坐标和平均半径，
			if (i == 0)
			{
				average_x = p.x;
				average_y = p.y;
				average_r = radius;
			}
			else {
				average_x = (average_x + p.x) / 2;
				average_y = (average_y + p.y) / 2;
				average_r = (average_r + radius) / 2;
			}

			if (i > 0)
			{
				//求坐标对应于圆上的坐标，即圆心与坐标直线与圆的交点
			}
		}
		else {
			/*Gdiplus::Point p1, p2;
			p1.X = m_vtPoints[i + 1].x;
			p1.Y = m_vtPoints[i + 1].y;

			p2.X = m_vtPoints[i + 2].x;
			p2.Y = m_vtPoints[i + 2].y;*/
			/*Gdiplus::Point points[2];
			points[0].X = m_vtPoints[i + 1].x;
			points[0].Y = m_vtPoints[i + 1].y;
			points[1].X = m_vtPoints[i + 2].x;
			points[1].Y = m_vtPoints[i + 2].y;

			float tension = 1.0;
			gs.DrawCurve(&drawPen2, points, 2, tension);*/

			Gdiplus::Point points[3];
			points[0].X = m_vtPoints[i].x;
			points[0].Y = m_vtPoints[i].y;
			points[1].X = m_vtPoints[i + 1].x;
			points[1].Y = m_vtPoints[i + 1].y;
			points[2].X = m_vtPoints[i + 2].x;
			points[2].Y = m_vtPoints[i + 2].y;

			float tension = 1.0;
			gs.DrawCurve(&drawPen2, points, 3, tension);
		}

		if (i + 2 >= m_vtPoints.size() - 1) {
			break;
		}
	}
	//double start_angle = 0.0;
	//double end_angle = 0.0;
	//double sweep_angle = 0.0;
	//if (!utility::geometry_facility::IsOnStraightLine(m_vtPoints[0], m_vtPoints[1], m_vtPoints[2]))	//三点不在一条直线上
	//{
	//	double radius = 0.0;
	//	point::YPoint<double> p = utility::geometry_facility::Circle(m_vtPoints[0], m_vtPoints[1], m_vtPoints[2], radius);

	//	//弧度转角度
	//	start_angle = asin((m_vtPoints[1].y - p.y) / radius)*180/M_PI;
	//	end_angle   = asin((m_vtPoints[2].y - p.y) / radius) * 180 / M_PI;

	//	sweep_angle = start_angle - end_angle;
	//}

	//int cx = GetDeviceCaps(dc.m_hDC, HORZRES);
	//int cy = GetDeviceCaps(dc.m_hDC, VERTRES);

	//CBitmap MemBmp;
	//MemBmp.CreateCompatibleBitmap(&dc, cx, cy);
	//CDC MemDC;//创建在内存位图绘制的DC
	//MemDC.CreateCompatibleDC(&dc);
	//MemDC.SelectObject(MemBmp);//将内存位图选到内存DC中
	//Gdiplus::Graphics gs(MemDC.GetSafeHdc());//根据内存DC创建GDI+的Graphics对象
	//gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//设置绘图质量

	////Gdiplus::Pen drawPen(Gdiplus::Color(255, 50, 150, 255));
	////gs.DrawArc(&drawPen, 0, 0, 200, 200, 0, 90);

	//Gdiplus::Pen drawPen2(Gdiplus::Color(255, 255, 0, 0));
	////gs.DrawArc(&drawPen2, 0, 0, 600, 565, 88, 16); //670 565
	////gs.DrawArc(&drawPen2, 0, 0, m_vtPoints[1].x, m_vtPoints[1].y,start_angle,sweep_angle);

	pDC->BitBlt(0, 0, cx, cy, &MemDC, 0, 0, SRCCOPY);
	gs.ReleaseHDC(MemDC.GetSafeHdc());
	MemDC.DeleteDC();
	MemBmp.DeleteObject();
}

void GdiDraw::DrawCircle(CDC * pDC)
{
	//构造数据
	while (m_vtAllPointsData.size() > 0)
	{
		auto it = m_vtAllPointsData.begin();
		m_vtPoints.push_back((it->second));
		it = m_vtAllPointsData.erase(it);
	}

	//坐标数据转像素
	for (auto it = m_vtPoints.begin(); it != m_vtPoints.end(); ++it)
	{
		it->x = abs(-1.0 * it->x * POS_RATE - ORG_X);
		it->y = abs(-1.0 * it->y * POS_RATE - ORG_Y);
	}

	int cx = GetDeviceCaps(pDC->m_hDC, HORZRES);
	int cy = GetDeviceCaps(pDC->m_hDC, VERTRES);

	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pDC, cx, cy);
	CDC MemDC;//创建在内存位图绘制的DC
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(MemBmp);//将内存位图选到内存DC中
	Gdiplus::Graphics gs(MemDC.GetSafeHdc());//根据内存DC创建GDI+的Graphics对象
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//设置绘图质量

	Gdiplus::Pen drawPen2(Gdiplus::Color(255, 50, 150, 255));

	double average_x = 0.0, average_y = 0.0;
	double average_r = 0.0;
	//绘制所有点
	for (size_t i = 0; i < m_vtPoints.size(); i++)
	{
		double start_angle = 0.0;
		double end_angle = 0.0;
		double sweep_angle = 0.0;
		if (!utility::geometry_facility::IsOnStraightLine(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2]))	//三点不在一条直线上
		{
			double radius = 0.0;
			point::YPoint<double> p = utility::geometry_facility::Circle(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2], radius);

			if (radius < 0)
			{
				continue;
			}
			//弧度转角度
			start_angle = asin((m_vtPoints[i + 1].y - p.y) / radius) * 180 / M_PI;
			end_angle = asin((m_vtPoints[i + 2].y - p.y) / radius) * 180 / M_PI;

			sweep_angle = start_angle - end_angle;

			//gs.DrawArc(&drawPen2, 0, 0, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			//gs.DrawArc(&drawPen2, start_x, start_y, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			//gs.DrawArc(&drawPen2, 0, 0, m_vtPoints[i + 1].x, m_vtPoints[i + 1].y, start_angle, sweep_angle);

			// 方法二
			// 根据角度差平分出两个点，利用平分出的两点做为控制点，使用贝塞
			//TRACE(_T("i = :%d, x0 = %.2f, y0 = %.2f, radius = %.2f \r\n"),i,p.x,p.y,radius);

			//方法三
			//求圆心的平均坐标和平均半径，
			if (i == 0)
			{
				average_x = p.x;
				average_y = p.y;
				average_r = radius;
			}
			else {
				average_x = (average_x + p.x) / 2;
				average_y = (average_y + p.y) / 2;
				average_r = (average_r + radius) / 2;
			}

			int start_x = 0, start_y = 0, end_x = 0, end_y = 0;
			start_x = average_x - average_r;
			start_y = average_y - average_r;
			end_x = average_x + average_r;
			end_y = average_y + average_r;

			gs.DrawArc(&drawPen2, start_x, start_y, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			//if (i > 0)
			//{
			//	point::YPoint<double> circle_center;
			//	circle_center.x = average_x;
			//	circle_center.y = average_y;
			//	//求坐标对应于圆上的坐标，即圆心与坐标直线与圆的交点
			//	point::YPoint<double> inter_plot;
			//	std::vector<point::YPoint<double>> points = utility::geometry_facility::calc_intersection_by_line_cycle(circle_center,m_vtPoints[1],circle_center,average_r);
			//	
			//	for (auto it = points.begin();it != points.end();++it) {
			//		double d = 0.0;
			//		d = sqrt(pow(circle_center.x - (*it).x,2) + pow(circle_center.y - (*it).y,2));
			//		if (fabs(d - utility::geometry_facility::calc_distance((*it), circle_center, m_vtPoints[1])) < 1E-2)
			//		{
			//			inter_plot.x = (*it).x;
			//			inter_plot.y = (*it).y;
			//			break;
			//		}
			//	}
			//}
		}
		else {
			Gdiplus::Point points[3];
			points[0].X = m_vtPoints[i].x;
			points[0].Y = m_vtPoints[i].y;
			points[1].X = m_vtPoints[i + 1].x;
			points[1].Y = m_vtPoints[i + 1].y;
			points[2].X = m_vtPoints[i + 2].x;
			points[2].Y = m_vtPoints[i + 2].y;

			float tension = 1.0;
			gs.DrawCurve(&drawPen2, points, 3, tension);
		}

		if (i + 2 >= m_vtPoints.size() - 1) {
			break;
		}
	}

	pDC->BitBlt(0, 0, cx, cy, &MemDC, 0, 0, SRCCOPY);
	gs.ReleaseHDC(MemDC.GetSafeHdc());
	MemDC.DeleteDC();
	MemBmp.DeleteObject();
}

void GdiDraw::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(GdiDraw, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// GdiDraw 消息处理程序


void GdiDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()

	//DrawLineTest(&dc);
	//DrawArc(&dc);
	DrawCircle(&dc);
}

BOOL GdiDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->MoveWindow(0,0,1000,1000);

	// TODO:  在此添加额外的初始化
	m_vtAllPointsData = utility::file_facility::LoadData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
