#pragma once
#include "afxwin.h"


// CLTParamDlg �Ի���

class CLTParamDlg : public CDialog
{
    DECLARE_DYNAMIC(CLTParamDlg)

public:
    CLTParamDlg(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CLTParamDlg();

// �Ի�������
    enum { IDD = IDD_DIALOG_LTPARAM };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
    double m_dTemp;        //�¶�
    double m_dPress;    //��ѹ
    double m_dHumid;    //ʪ��
    long m_lTimeSep;    //��������ʱ����
    long m_lNumber;        //������������
    CComboBox m_Combo_RegionType;    //Box or Sphere
    long m_lTime;    //��̬����-ʱ��
    long m_lSearchTime;        //����ʱ��
    double m_dSearchRadius;    //�����뾶
    CComboBox m_Combo_CoordSys;    //��������ϵ
    CComboBox m_Combo_MeasMode;    //����ģʽ
    CButton m_Check_Region;
    CButton m_Check_ADM;
    CComboBox m_Combo_Reflector;    //���侵

    int m_nLT;    //��¼��ǰ�ļ��������
    int m_nPrevMode;    //��¼��ǰ�Ĳ���ģʽ
    int m_nPrevRefl;//��¼��ǰ�ķ��侵
    
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButtonCurparam();
    afx_msg void OnBnClickedButtonSetparam();
};
