// GdiDraw.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DrawDemo.h"
#include "GdiDraw.h"
#include "afxdialogex.h"
#include "string/facility.h"
#include <math.h>

// GdiDraw �Ի���
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
	CDC MemDC;//�������ڴ�λͼ���Ƶ�DC
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(MemBmp);//���ڴ�λͼѡ���ڴ�DC��
	Gdiplus::Graphics gs(MemDC.GetSafeHdc());//�����ڴ�DC����GDI+��Graphics����
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//���û�ͼ����

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
	//��������
	while (m_vtAllPointsData.size() > 0)
	{
		auto it = m_vtAllPointsData.begin();
		m_vtPoints.push_back((it->second));
		it = m_vtAllPointsData.erase(it);
	}

	//��������ת����
	for (auto it = m_vtPoints.begin(); it != m_vtPoints.end(); ++it)
	{
		it->x = abs(-1.0 * it->x * POS_RATE - ORG_X);
		it->y = abs(-1.0 * it->y * POS_RATE - ORG_Y);
	}

	int cx = GetDeviceCaps(pDC->m_hDC, HORZRES);
	int cy = GetDeviceCaps(pDC->m_hDC, VERTRES);

	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pDC, cx, cy);
	CDC MemDC;//�������ڴ�λͼ���Ƶ�DC
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(MemBmp);//���ڴ�λͼѡ���ڴ�DC��
	Gdiplus::Graphics gs(MemDC.GetSafeHdc());//�����ڴ�DC����GDI+��Graphics����
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//���û�ͼ����

	Gdiplus::Pen drawPen2(Gdiplus::Color(255, 50, 150, 255));

	double average_x = 0.0, average_y = 0.0;
	double average_r = 0.0;
	//�������е�
	for (size_t i = 0; i < m_vtPoints.size(); i++)
	{
		double start_angle = 0.0;
		double end_angle = 0.0;
		double sweep_angle = 0.0;
		if (!utility::geometry_facility::IsOnStraightLine(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2]))	//���㲻��һ��ֱ����
		{
			double radius = 0.0;
			point::YPoint<double> p = utility::geometry_facility::Circle(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2], radius);

			if (radius < 0)
			{
				continue;
			}
			//����ת�Ƕ�
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

			// ������
			// ���ݽǶȲ�ƽ�ֳ������㣬����ƽ�ֳ���������Ϊ���Ƶ㣬ʹ�ñ���
			//TRACE(_T("i = :%d, x0 = %.2f, y0 = %.2f, radius = %.2f \r\n"),i,p.x,p.y,radius);

			//������
			//��Բ�ĵ�ƽ�������ƽ���뾶��
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
				//�������Ӧ��Բ�ϵ����꣬��Բ��������ֱ����Բ�Ľ���
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
	//if (!utility::geometry_facility::IsOnStraightLine(m_vtPoints[0], m_vtPoints[1], m_vtPoints[2]))	//���㲻��һ��ֱ����
	//{
	//	double radius = 0.0;
	//	point::YPoint<double> p = utility::geometry_facility::Circle(m_vtPoints[0], m_vtPoints[1], m_vtPoints[2], radius);

	//	//����ת�Ƕ�
	//	start_angle = asin((m_vtPoints[1].y - p.y) / radius)*180/M_PI;
	//	end_angle   = asin((m_vtPoints[2].y - p.y) / radius) * 180 / M_PI;

	//	sweep_angle = start_angle - end_angle;
	//}

	//int cx = GetDeviceCaps(dc.m_hDC, HORZRES);
	//int cy = GetDeviceCaps(dc.m_hDC, VERTRES);

	//CBitmap MemBmp;
	//MemBmp.CreateCompatibleBitmap(&dc, cx, cy);
	//CDC MemDC;//�������ڴ�λͼ���Ƶ�DC
	//MemDC.CreateCompatibleDC(&dc);
	//MemDC.SelectObject(MemBmp);//���ڴ�λͼѡ���ڴ�DC��
	//Gdiplus::Graphics gs(MemDC.GetSafeHdc());//�����ڴ�DC����GDI+��Graphics����
	//gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//���û�ͼ����

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
	//��������
	while (m_vtAllPointsData.size() > 0)
	{
		auto it = m_vtAllPointsData.begin();
		m_vtPoints.push_back((it->second));
		it = m_vtAllPointsData.erase(it);
	}

	//��������ת����
	for (auto it = m_vtPoints.begin(); it != m_vtPoints.end(); ++it)
	{
		it->x = abs(-1.0 * it->x * POS_RATE - ORG_X);
		it->y = abs(-1.0 * it->y * POS_RATE - ORG_Y);
	}

	int cx = GetDeviceCaps(pDC->m_hDC, HORZRES);
	int cy = GetDeviceCaps(pDC->m_hDC, VERTRES);

	CBitmap MemBmp;
	MemBmp.CreateCompatibleBitmap(pDC, cx, cy);
	CDC MemDC;//�������ڴ�λͼ���Ƶ�DC
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(MemBmp);//���ڴ�λͼѡ���ڴ�DC��
	Gdiplus::Graphics gs(MemDC.GetSafeHdc());//�����ڴ�DC����GDI+��Graphics����
	gs.SetSmoothingMode(Gdiplus::SmoothingModeAntiAlias);//���û�ͼ����

	Gdiplus::Pen drawPen2(Gdiplus::Color(255, 50, 150, 255));

	double average_x = 0.0, average_y = 0.0;
	double average_r = 0.0;
	//�������е�
	for (size_t i = 0; i < m_vtPoints.size(); i++)
	{
		double start_angle = 0.0;
		double end_angle = 0.0;
		double sweep_angle = 0.0;
		if (!utility::geometry_facility::IsOnStraightLine(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2]))	//���㲻��һ��ֱ����
		{
			double radius = 0.0;
			point::YPoint<double> p = utility::geometry_facility::Circle(m_vtPoints[i], m_vtPoints[i + 1], m_vtPoints[i + 2], radius);

			if (radius < 0)
			{
				continue;
			}
			//����ת�Ƕ�
			start_angle = asin((m_vtPoints[i + 1].y - p.y) / radius) * 180 / M_PI;
			end_angle = asin((m_vtPoints[i + 2].y - p.y) / radius) * 180 / M_PI;

			sweep_angle = start_angle - end_angle;

			//gs.DrawArc(&drawPen2, 0, 0, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			//gs.DrawArc(&drawPen2, start_x, start_y, end_x - start_x, end_y - start_y, start_angle, sweep_angle);
			//gs.DrawArc(&drawPen2, 0, 0, m_vtPoints[i + 1].x, m_vtPoints[i + 1].y, start_angle, sweep_angle);

			// ������
			// ���ݽǶȲ�ƽ�ֳ������㣬����ƽ�ֳ���������Ϊ���Ƶ㣬ʹ�ñ���
			//TRACE(_T("i = :%d, x0 = %.2f, y0 = %.2f, radius = %.2f \r\n"),i,p.x,p.y,radius);

			//������
			//��Բ�ĵ�ƽ�������ƽ���뾶��
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
			//	//�������Ӧ��Բ�ϵ����꣬��Բ��������ֱ����Բ�Ľ���
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


// GdiDraw ��Ϣ�������


void GdiDraw::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	//DrawLineTest(&dc);
	//DrawArc(&dc);
	DrawCircle(&dc);
}

BOOL GdiDraw::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	this->MoveWindow(0,0,1000,1000);

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_vtAllPointsData = utility::file_facility::LoadData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
