#pragma once
#include "afxwin.h"


// CLTParamDlg 对话框

class CLTParamDlg : public CDialog
{
    DECLARE_DYNAMIC(CLTParamDlg)

public:
    CLTParamDlg(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CLTParamDlg();

// 对话框数据
    enum { IDD = IDD_DIALOG_LTPARAM };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    double m_dTemp;        //温度
    double m_dPress;    //气压
    double m_dHumid;    //湿度
    long m_lTimeSep;    //连续测量时间间隔
    long m_lNumber;        //连续测量次数
    CComboBox m_Combo_RegionType;    //Box or Sphere
    long m_lTime;    //静态测量-时间
    long m_lSearchTime;        //搜索时间
    double m_dSearchRadius;    //搜索半径
    CComboBox m_Combo_CoordSys;    //测量坐标系
    CComboBox m_Combo_MeasMode;    //测量模式
    CButton m_Check_Region;
    CButton m_Check_ADM;
    CComboBox m_Combo_Reflector;    //反射镜

    int m_nLT;    //记录当前的激光跟踪仪
    int m_nPrevMode;    //记录以前的测量模式
    int m_nPrevRefl;//记录以前的反射镜
    
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonCurparam();
    afx_msg void OnBnClickedButtonSetparam();
};
