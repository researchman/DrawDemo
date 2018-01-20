#pragma once
#include <string>
#include <vector>
#include <map>

#include "meta/point.h"

// CLoadImage �Ի���

class CLoadImage : public CDialogEx
{
	DECLARE_DYNAMIC(CLoadImage)

public:
	CLoadImage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLoadImage();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD =  };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:
	int backgroundImageWidth;
	int backgroundImageHeight;

	int m_current;
	std::vector<point::YPoint<double>> m_vtPoints;
	std::map<int, point::YPoint<double>> m_vtAllPointsData;

public:
	void LoadImageFile(std::string &filePath);
	void AdjustWndSize(int width,int height);
	void LoadData();
public:
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
};
