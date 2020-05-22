// LTVideoCameraDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PointMaesure.h"
#include "LTVideoCameraDlg.h"
#include "../MeasDeviceView.h"
#include "../OutputBar.h"
extern CMeasDeviceView *pMeasDeviceView;
extern COutputBar *pOutputBar;
// CLTVideoCameraDlg �Ի���

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

// CLTVideoCameraDlg ��Ϣ�������

BOOL CLTVideoCameraDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
    //m_nLT=pMeasDeviceView->GetListLTItemSelected();
    m_nLT=-1;
	CString strLTName,strLTIP;
	//strLTName=pMeasDeviceView->m_List_LT.GetItemText(m_nLT,0);
	//strLTIP=pMeasDeviceView->m_List_LT.GetItemText(m_nLT,1);
	this->SetWindowText(strLTName+"ȫ�������ͼ");
	//m_LTVideo2Ctrl.SetServerAddress(strLTIP);

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
void CLTVideoCameraDlg::OnBnClickedButtonActivatecameraview()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HRESULT hr=0;
	try
	{
		hr=pMeasDeviceView->m_pLTCommandSync[m_nLT]->ActivateCameraView();//����ȫ�����
		if(SUCCEEDED(hr))
		{
			pOutputBar->SetInfoText("ȫ������Ѽ���");
			OnBnClickedButtonStartlive();
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		pOutputBar->SetInfoText("ȫ���������ʧ��");
		return;
	}
}
void CLTVideoCameraDlg::OnBnClickedButtonStartlive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LTVideo2Ctrl.StartLiveImage();
}

void CLTVideoCameraDlg::OnBnClickedButtonStoplive()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LTVideo2Ctrl.StopLiveImage();
}

void CLTVideoCameraDlg::OnBnClickedButtonFramerateUp()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_LTVideo2Ctrl.FrameRateStepUp();
}

void CLTVideoCameraDlg::OnBnClickedButtonFramerateDown()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	HRESULT hr=0;
	try
	{
		hr=pMeasDeviceView->m_pLTCommandSync[m_nLT]->FindReflector(4000);
		if(SUCCEEDED(hr))
		{
			pOutputBar->SetInfoText("������������");
			if(IDYES==AfxMessageBox("�����������������˳�ȫ�������Ȼ��ʼ����",MB_YESNO)) 
			{
				OnOK();
			}
		}
	}
	catch (_com_error &e)
	{
		AfxMessageBox((LPCTSTR)e.Description());
		pOutputBar->SetInfoText("δ����������");
		return;
	}
}
