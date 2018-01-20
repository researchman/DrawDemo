// LoadImage.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include <fstream>

#include "DrawDemo.h"
#include "LoadImage.h"
#include "afxdialogex.h"
#include "string/facility.h"

using namespace std;

// CLoadImage �Ի���

IMPLEMENT_DYNAMIC(CLoadImage, CDialogEx)

CLoadImage::CLoadImage(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOAD_IMAGE, pParent)
{
	m_current = -1;
	m_vtPoints.resize(0);
}

CLoadImage::~CLoadImage()
{
}

void CLoadImage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLoadImage, CDialogEx)
//	ON_WM_PAINT()
ON_WM_PAINT()
END_MESSAGE_MAP()


// CLoadImage ��Ϣ�������

/*
* ����ͼƬ������ȡͼƬ�Ŀ�Ⱥ͸߶�
*
* param
*		filePath		ͼƬ�ļ�·��
*
* return
*		��
*
*/
void CLoadImage::LoadImageFile(std::string & filePath)
{
	CImage img;
	img.Load(L"./res/png/gongsibuju.png");
	if (img.IsNull())
	{
		MessageBox(_T("����ͼƬʧ�ܣ�"));
		return;
	}

	backgroundImageWidth  = img.GetWidth();
	backgroundImageHeight = img.GetHeight();

	////ֱ�ӻ���
	////img.Draw(dc.GetSafeHdc(), rect);
	////img.Destroy();

	////˫���廭ͼ��ʽ
	//img.Draw(memCDC.GetSafeHdc(), rect);
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memCDC, 0, 0,SRCCOPY);
}

/*
 * ����pngͼƬ�Ĵ�С�������Ի��򴰿ڴ�С
 *
 * param
 *		width		���
 *		height		�߶�
 *
 * return
 *		��
 *
*/
void CLoadImage::AdjustWndSize(int width, int height)
{
	this->MoveWindow(0, 0, width, height);
	CRect rect;
	this->GetClientRect(&rect);
	GetDlgItem(IDC_IMAGE_CTRL)->MoveWindow(0, 0, rect.Width(), rect.Height());
}

void CLoadImage::LoadData()
{
	ifstream in;
	in.open("D:/position.log");

	if (!in.is_open())
	{
		return;
	}

	char pch[200] = { 0 };

	while (!in.eof())
	{
		in.getline(pch, 200);
		string data = pch;

		std::vector<std::string> metaDatas = utility::string_facility::Split(data, ",", true);
		
		std::string card_id = "";
		unsigned int card_ct = 0;
		point::YPoint<double> p;
		for (unsigned int i = 0; i < metaDatas.size(); i++)
		{
			std::string meta = metaDatas[i];
			std::vector<std::string> vtMeta = utility::string_facility::Split(meta, ":", true);
			switch (i) {
			case 0:
			{
				//����
				card_id = vtMeta[1];
				break;
			}
			case 1:
				//����ct��
				card_ct = atoi(vtMeta[1].c_str());
				break;
			case 2:
				//x
				p.x = atof(vtMeta[1].c_str());
				break;
			case 3:
				//y
				p.y = atof(vtMeta[1].c_str());
				break;
			case 4:
				//��վ��
				break;
			default:
				break;
			}
		}

		m_vtAllPointsData.insert(std::make_pair(card_ct,p));
	}
	in.close();
}

void CLoadImage::OnPaint()
{
	CPaintDC dc(GetDlgItem(IDC_IMAGE_CTRL)); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()

	//��ͼ����
	CRect rect;
	GetDlgItem(IDC_IMAGE_CTRL)->GetClientRect(&rect);

	//�����ڴ滭ͼ���,���������ݾ��
	CDC memCDC;
	memCDC.CreateCompatibleDC(&dc);

	//�����ڴ�λͼ
	CBitmap memBitmap;
	//��λͼѡ�뵽�ڴ���ʾ�豸��
	memBitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	CBitmap *pOldBitmap = memCDC.SelectObject(&memBitmap);

	POINT* pts = new POINT[4];
	/*pts[0].x = 421;
	pts[0].y = 16;
	pts[1].x = 7;
	pts[1].y = 197;
	pts[2].x = 480;
	pts[2].y = 320;
	pts[3].x = 30;
	pts[3].y = 350;*/
	/*if (m_current >= 0 && (m_current+3) < m_vtAllPointsData.size())
	{
		pts[0].x = fabs(m_vtAllPointsData[m_current].x*100);
		pts[0].y = fabs(m_vtAllPointsData[m_current].y * 100);
		pts[1].x = fabs(m_vtAllPointsData[m_current+1].x * 100);
		pts[1].y = fabs(m_vtAllPointsData[m_current+1].y * 100);
		pts[2].x = fabs(m_vtAllPointsData[m_current+2].x * 100);
		pts[2].y = fabs(m_vtAllPointsData[m_current+2].y * 100);
		pts[3].x = fabs(m_vtAllPointsData[m_current+3].x * 100);
		pts[3].y = fabs(m_vtAllPointsData[m_current+3].y * 100);
		m_current++;
		PolyBezier(dc.GetSafeHdc(), pts, 4);
		TRACE(_T("current : %d \r\n"), m_current);
	}*/
	srand(time(0));
	for (int i = 0; i < 1; i++)
	{
		SelectObject(dc.GetSafeHdc(), GetStockObject(BLACK_PEN));
		//PolyBezier(dc.GetSafeHdc(), pts, 10);
		for (int j = 0; j < 4; j++)
		{
			pts[j].x = rand() % rect.Width();
			pts[j].y = rand() % rect.Height();
		}
		//hpen = CreatePen(PS_INSIDEFRAME, 3, RGB(rand() % 256, rand() % 256, rand() % 256));
		//DeleteObject(SelectObject(hdc, hpen));
		PolyBezier(dc.GetSafeHdc(), pts, 4);
	}
	
	//CImage img;
	//img.Load(L"./res/png/gongsibuju.png");
	//if (img.IsNull())
	//{
	//	MessageBox(_T("����ͼƬʧ�ܣ�"));
	//	return;
	//}
	////ֱ�ӻ���
	////img.Draw(dc.GetSafeHdc(), rect);
	////img.Destroy();

	////˫���廭ͼ��ʽ
	//img.Draw(memCDC.GetSafeHdc(), rect);
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memCDC, 0, 0,SRCCOPY);

	memCDC.FillSolidRect(0, 0, 100, 100, RGB(255, 255, 255));	//��ͼ��
	memCDC.MoveTo(0,0);
	memCDC.LineTo(100,100);	//���ڴ��е�ͼ��������Ļ�Ͻ�����ʾ
	dc.BitBlt(0, 0, 100, 100, &memCDC, 0, 0, SRCCOPY);

	memCDC.SelectObject(pOldBitmap);
	memBitmap.DeleteObject();
	memCDC.DeleteDC();
}

BOOL CLoadImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	std::string filePath = "ss";
	LoadImageFile(filePath);
	AdjustWndSize(backgroundImageWidth, backgroundImageHeight);
	LoadData();
	m_current = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}