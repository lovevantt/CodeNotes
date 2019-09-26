// LTVideoCameraDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "../PointMaesure.h"
#include "LTVideoCameraDlg.h"
#include "../MeasDeviceView.h"
#include "../OutputBar.h"
extern CMeasDeviceView *pMeasDeviceView;
extern COutputBar *pOutputBar;
// CLTVideoCameraDlg 对话框

IMPLEMENT_DYNAMIC(CLTVideoCameraDlg, CDialog)

CLTVideoCameraDlg::CLTVideoCameraDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLTVideoCameraDlg::IDD, pParent)
{
	m_nLT=-1;
}

CLTVideoCameraDlg::~CLTVideoCameraDlg()
{
}

void CLTVideoCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LTVIDEO2CTRL, m_LTVideo2Ctrl);
}


BEGIN_MESSAGE_MAP(CLTVideoCameraDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_ACTIVATECAMERAVIEW, &CLTVideoCameraDlg::OnBnClickedButtonActivatecameraview)
	ON_BN_CLICKED(IDC_BUTTON_STARTLIVE, &CLTVideoCameraDlg::OnBnClickedButtonStartlive)
	ON_BN_CLICKED(IDC_BUTTON_STOPLIVE, &CLTVideoCameraDlg::OnBnClickedButtonStoplive)
	ON_BN_CLICKED(IDC_BUTTON_FRAMERATE_UP, &CLTVideoCameraDlg::OnBnClickedButtonFramerateUp)
	ON_BN_CLICKED(IDC_BUTTON_FRAMERATE_DOWN, &CLTVideoCameraDlg::OnBnClickedButtonFramerateDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, &CLTVideoCameraDlg::OnBnClickedButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, &CLTVideoCameraDlg::OnBnClickedButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_LEFT, &CLTVideoCameraDlg::OnBnClickedButtonMoveLeft)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_RIGHT, &CLTVideoCameraDlg::OnBnClickedButtonMoveRight)
	ON_BN_CLICKED(IDC_BUTTON_STOPMOVE, &CLTVideoCameraDlg::OnBnClickedButtonStopmove)
	ON_BN_CLICKED(IDC_BUTTON_FINDREFLECTOR, &CLTVideoCameraDlg::OnBnClickedButtonFindreflector)
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CLTVideoCameraDlg, CDialog)
	ON_EVENT(CLTVideoCameraDlg,IDC_LTVIDEO2CTRL,1/* Click */,OnBnClickedLtvideo2ctrl,VTS_R8 VTS_R8 VTS_I4 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()

// CLTVideoCameraDlg 消息处理程序

BOOL CLTVideoCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
    //m_nLT=pMeasDeviceView->GetListLTItemSelected();
    m_nLT=-1;
	CString strLTName,strLTIP;
	//strLTName=pMeasDeviceView->m_List_LT.GetItemText(m_nLT,0);
	//strLTIP=pMeasDeviceView->m_List_LT.GetItemText(m_nLT,1);
	this->SetWindowText(strLTName+"全景相机视图");
	//m_LTVideo2Ctrl.SetServerAddress(strLTIP);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CLTVideoCameraDlg::OnBnClickedButtonActivatecameraview()
{
	// TODO: 在此添加控件通知处理程序代码
	HRESULT hr=0;
	try
	{
		hr=pMeasDeviceView->m_pLTCommandSync[m_nLT]->ActivateCameraView();//激活全景相机
		if(SUCCEEDED(hr))
		{
			pOutputBar->SetInfoText("全景相机已激活");
			OnBnClickedButtonStartlive();
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		pOutputBar->SetInfoText("全景相机激活失败");
		return;
	}
}
void CLTVideoCameraDlg::OnBnClickedButtonStartlive()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LTVideo2Ctrl.StartLiveImage();
}

void CLTVideoCameraDlg::OnBnClickedButtonStoplive()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LTVideo2Ctrl.StopLiveImage();
}

void CLTVideoCameraDlg::OnBnClickedButtonFramerateUp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LTVideo2Ctrl.FrameRateStepUp();
}

void CLTVideoCameraDlg::OnBnClickedButtonFramerateDown()
{
	// TODO: 在此添加控件通知处理程序代码
	m_LTVideo2Ctrl.FrameRateStepDown();
}
void CLTVideoCameraDlg::OnBnClickedLtvideo2ctrl(double deltaHz,double deltaVt,long posX,long posY,long flags)
{
	try
	{
		pMeasDeviceView->m_pLTCommandSync[m_nLT]->PositionRelativeHV(deltaHz,deltaVt);
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		return;
	}
}
void CLTVideoCameraDlg::OnBnClickedButtonMoveUp()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		pMeasDeviceView->m_pLTCommandSync[m_nLT]->MoveUp();
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		return;
	}
}

void CLTVideoCameraDlg::OnBnClickedButtonMoveDown()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		pMeasDeviceView->m_pLTCommandSync[m_nLT]->MoveDown();
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		return;
	}
}

void CLTVideoCameraDlg::OnBnClickedButtonMoveLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		pMeasDeviceView->m_pLTCommandSync[m_nLT]->MoveLeft();
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		return;
	}
}

void CLTVideoCameraDlg::OnBnClickedButtonMoveRight()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		pMeasDeviceView->m_pLTCommandSync[m_nLT]->MoveRight();
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		return;
	}
}

void CLTVideoCameraDlg::OnBnClickedButtonStopmove()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		pMeasDeviceView->m_pLTCommandSync[m_nLT]->StopMove();
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		return;
	}
}

void CLTVideoCameraDlg::OnBnClickedButtonFindreflector()
{
	// TODO: 在此添加控件通知处理程序代码
	HRESULT hr=0;
	try
	{
		hr=pMeasDeviceView->m_pLTCommandSync[m_nLT]->FindReflector(4000);
		if(SUCCEEDED(hr))
		{
			pOutputBar->SetInfoText("已搜索到靶球");
			if(IDYES==AfxMessageBox("已搜索到靶球，请先退出全景相机，然后开始测量",MB_YESNO)) 
			{
				OnOK();
			}
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		pOutputBar->SetInfoText("未搜索到靶球");
		return;
	}
}
