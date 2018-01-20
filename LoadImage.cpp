// LoadImage.cpp : 实现文件
//
#include "stdafx.h"
#include <fstream>

#include "DrawDemo.h"
#include "LoadImage.h"
#include "afxdialogex.h"
#include "string/facility.h"

using namespace std;

// CLoadImage 对话框

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


// CLoadImage 消息处理程序

/*
* 载入图片，并获取图片的宽度和高度
*
* param
*		filePath		图片文件路径
*
* return
*		无
*
*/
void CLoadImage::LoadImageFile(std::string & filePath)
{
	CImage img;
	img.Load(L"./res/png/gongsibuju.png");
	if (img.IsNull())
	{
		MessageBox(_T("加载图片失败！"));
		return;
	}

	backgroundImageWidth  = img.GetWidth();
	backgroundImageHeight = img.GetHeight();

	////直接绘制
	////img.Draw(dc.GetSafeHdc(), rect);
	////img.Destroy();

	////双缓冲画图方式
	//img.Draw(memCDC.GetSafeHdc(), rect);
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memCDC, 0, 0,SRCCOPY);
}

/*
 * 根据png图片的大小，调整对话框窗口大小
 *
 * param
 *		width		宽度
 *		height		高度
 *
 * return
 *		无
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
				//卡号
				card_id = vtMeta[1];
				break;
			}
			case 1:
				//卡的ct号
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
				//分站号
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
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	//画图区域
	CRect rect;
	GetDlgItem(IDC_IMAGE_CTRL)->GetClientRect(&rect);

	//定义内存画图句柄,并创建兼容句柄
	CDC memCDC;
	memCDC.CreateCompatibleDC(&dc);

	//定义内存位图
	CBitmap memBitmap;
	//将位图选入到内存显示设备上
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
	//	MessageBox(_T("加载图片失败！"));
	//	return;
	//}
	////直接绘制
	////img.Draw(dc.GetSafeHdc(), rect);
	////img.Destroy();

	////双缓冲画图方式
	//img.Draw(memCDC.GetSafeHdc(), rect);
	//dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memCDC, 0, 0,SRCCOPY);

	memCDC.FillSolidRect(0, 0, 100, 100, RGB(255, 255, 255));	//绘图　
	memCDC.MoveTo(0,0);
	memCDC.LineTo(100,100);	//将内存中的图拷贝到屏幕上进行显示
	dc.BitBlt(0, 0, 100, 100, &memCDC, 0, 0, SRCCOPY);

	memCDC.SelectObject(pOldBitmap);
	memBitmap.DeleteObject();
	memCDC.DeleteDC();
}

BOOL CLoadImage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	std::string filePath = "ss";
	LoadImageFile(filePath);
	AdjustWndSize(backgroundImageWidth, backgroundImageHeight);
	LoadData();
	m_current = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}