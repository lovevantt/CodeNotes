#pragma once
#include "ltvideo2ctrl.h"

// CLTVideoCameraDlg �Ի���

class CLTVideoCameraDlg : public CDialog
{
	DECLARE_DYNAMIC(CLTVideoCameraDlg)

public:
	CLTVideoCameraDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLTVideoCameraDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_LT_VIDEOCAMERA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

	afx_msg void OnBnClickedLtvideo2ctrl(double deltaHz,double deltaVt,long posX,long posY,long flags);
	DECLARE_EVENTSINK_MAP()
public:
	CLtvideo2ctrl m_LTVideo2Ctrl;
	int m_nLT;	//��¼��ǰ�ļ��������

	virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonActivatecameraview();
	afx_msg void OnBnClickedButtonStartlive();
	afx_msg void OnBnClickedButtonStoplive();
	afx_msg void OnBnClickedButtonFramerateUp();
	afx_msg void OnBnClickedButtonFramerateDown();
	afx_msg void OnBnClickedButtonMoveUp();
	afx_msg void OnBnClickedButtonMoveDown();
	afx_msg void OnBnClickedButtonMoveLeft();
	afx_msg void OnBnClickedButtonMoveRight();
	afx_msg void OnBnClickedButtonStopmove();
	afx_msg void OnBnClickedButtonFindreflector();
};
