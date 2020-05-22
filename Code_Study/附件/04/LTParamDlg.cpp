// LTParamDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../PointMaesure.h"
#include "LTParamDlg.h"
#include "../PointMeasView.h"
extern CPointMeasView *pPointMeasView;

// CLTParamDlg �Ի���

IMPLEMENT_DYNAMIC(CLTParamDlg, CDialog)

CLTParamDlg::CLTParamDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CLTParamDlg::IDD, pParent)
{
    m_dTemp=0;
    m_dPress=0;
    m_dHumid=0;
    m_lTimeSep=0;
    m_lNumber=0;
    m_lTime=0;
    m_lSearchTime=0;
    m_dSearchRadius=0;
    m_nLT=-1;
}

CLTParamDlg::~CLTParamDlg()
{

}

void CLTParamDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_TEMP, m_dTemp);
    DDX_Text(pDX, IDC_EDIT_PRESS, m_dPress);
    DDX_Text(pDX, IDC_EDIT_HUMID, m_dHumid);
    DDX_Text(pDX, IDC_EDIT_TIMESEP, m_lTimeSep);
    DDX_Text(pDX, IDC_EDIT_NUMBER, m_lNumber);
    DDX_Control(pDX, IDC_COMBO_REGIONTYPE, m_Combo_RegionType);
    DDX_Text(pDX, IDC_EDIT_TIME, m_lTime);
    DDX_Text(pDX, IDC_EDIT_SEARCHTIME, m_lSearchTime);
    DDX_Text(pDX, IDC_EDIT_SEARCHRADIUS, m_dSearchRadius);
    DDX_Control(pDX, IDC_COMBO_COORDSYS, m_Combo_CoordSys);
    DDX_Control(pDX, IDC_COMBO_MEASMODE, m_Combo_MeasMode);
    DDX_Control(pDX, IDC_CHECK_REGION, m_Check_Region);
    DDX_Control(pDX, IDC_CHECK_ADM, m_Check_ADM);
    DDX_Control(pDX, IDC_COMBO_REFLECTOR, m_Combo_Reflector);
}


BEGIN_MESSAGE_MAP(CLTParamDlg, CDialog)
    ON_BN_CLICKED(IDC_BUTTON_CURPARAM, &CLTParamDlg::OnBnClickedButtonCurparam)
    ON_BN_CLICKED(IDC_BUTTON_SETPARAM, &CLTParamDlg::OnBnClickedButtonSetparam)
END_MESSAGE_MAP()


// CLTParamDlg ��Ϣ�������

BOOL CLTParamDlg::OnInitDialog()
{
    CDialog::OnInitDialog();

    // TODO:  �ڴ���Ӷ���ĳ�ʼ��
    m_Combo_MeasMode.SetCurSel(0);
    m_Combo_RegionType.SetCurSel(0);
    m_Combo_CoordSys.SetCurSel(0);
//     m_nLT=pPointMeasView->GetListLTItemSelected();
    CString strLTName;
//     strLTName=pPointMeasView->m_List_LT.GetItemText(m_nLT,0);
    this->SetWindowText(strLTName+"��������");    //���öԻ������

    UpdateData(FALSE);
    return TRUE;  // return TRUE unless you set the focus to a control
    // �쳣: OCX ����ҳӦ���� FALSE
}

void CLTParamDlg::OnBnClickedButtonCurparam()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    double dTemp,dPress,dHumid;//�¶ȡ���ѹ��ʪ��

    VARIANT_BOOL bUseRegion;
    ES_RegionType regType;
    long lNumPoints, lTimeSep;

    VARIANT_BOOL bUseADM;
    long lMeasTime;

    double dSearchRadius = m_dSearchRadius;  //��������
    long lSearchTime = m_lSearchTime;

    ES_CoordinateSystemType nSelCoord; 
    ES_MeasMode nMeasMode;

    long lReflector=-1;

    try
    {
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetEnvironmentParams(&dTemp, &dPress, &dHumid);
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetContinuousTimeModeParams(&lTimeSep, &lNumPoints, &bUseRegion, &regType);
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetStationaryModeParams(&lMeasTime, &bUseADM);
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetSearchParams(&dSearchRadius, &lSearchTime);
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetCoordinateSystemType(&nSelCoord);
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetMeasurementMode(&nMeasMode);
        pPointMeasView->m_pLTCommandSync[m_nLT]->GetReflector(&lReflector);
    }
    catch(_com_error &e)
    {
        AfxMessageBox("��ȡ��ǰ����ʧ�ܣ�");
        return;
    }        
    m_dTemp=dTemp;
    m_dPress=dPress;
    m_dHumid=dHumid;
    m_lTimeSep = lTimeSep;
    m_lNumber = lNumPoints;
    m_Check_Region.SetCheck(bUseRegion ? 1 : 0);
    m_Combo_RegionType.SetCurSel(regType);
    m_lTime = lMeasTime;
    m_Check_ADM.SetCheck(bUseADM ? 1 : 0);
    m_dSearchRadius=dSearchRadius;
    m_lSearchTime=lSearchTime;
    m_Combo_CoordSys.SetCurSel(nSelCoord);
    m_Combo_MeasMode.SetCurSel(nMeasMode);

    //��ȡLT����ʱ���Ѿ���õķ��侵��������ʾ��������
    CString sName;
    POSITION pos = NULL;
    void *pa = NULL;
    m_Combo_Reflector.EnableWindow(FALSE);
    for (pos=pPointMeasView->GetReflectorMap(m_nLT).GetStartPosition();pos!=NULL;)
    {
        pPointMeasView->GetReflectorMap(m_nLT).GetNextAssoc(pos, sName, pa);
        m_Combo_Reflector.AddString(sName);
    } 
    for (pos=pPointMeasView->GetReflectorMap(m_nLT).GetStartPosition();pos!=NULL;)
    {
        pPointMeasView->GetReflectorMap(m_nLT).GetNextAssoc(pos, sName, pa);
        if (lReflector>= 0 && (INT_PTR)pa==lReflector) 
            m_Combo_Reflector.SelectString(-1, sName);
    }
    m_Combo_Reflector.EnableWindow(TRUE);
    m_nPrevRefl=m_Combo_Reflector.GetCurSel();

    UpdateData(FALSE);
}

void CLTParamDlg::OnBnClickedButtonSetparam()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
    UpdateData(TRUE);
    double dTemp,dPress,dHumid;                          //�¶ȡ���ѹ��ʪ��
    dTemp=m_dTemp;
    dPress=m_dPress;
    dHumid=m_dHumid;
   
    VARIANT_BOOL bUseRegion;               //ʱ������������Region
    ES_RegionType regType;
    long lTimeSep, lNumPoints;
    lTimeSep=m_lTimeSep;
    lNumPoints=m_lNumber;
    bUseRegion=m_Check_Region.GetCheck(); 
    regType=(ES_RegionType)m_Combo_RegionType.GetCurSel();

    VARIANT_BOOL bUseADM;                 //��̬��������
    bUseADM=m_Check_ADM.GetCheck();
    long lTime;
    lTime=m_lTime;

    double dSearchRadius=m_dSearchRadius;  //��������
    long lSearchTime=m_lSearchTime;

    int nSelMode=m_Combo_MeasMode.GetCurSel();    //������ʽ
    int nSelCoord=m_Combo_CoordSys.GetCurSel();   //����ϵ


    CString strSelReflector;
    void *id;
    m_Combo_Reflector.GetLBText(m_Combo_Reflector.GetCurSel(), strSelReflector);
    VERIFY(pPointMeasView->m_reflectorMap[m_nLT].Lookup(strSelReflector, id));

    try 
    {
        //pPointMeasView->m_pLTCommandSync[m_nLT]->SetEnvironmentParams(dTemp, dPress, dHumid);//�˴���֪Ϊ��ִ�л��б���
        pPointMeasView->m_pLTCommandSync[m_nLT]->SetContinuousTimeModeParams(lTimeSep, lNumPoints, bUseRegion, regType);    
        pPointMeasView->m_pLTCommandSync[m_nLT]->SetStationaryModeParams(lTime, bUseADM);
        pPointMeasView->m_pLTCommandSync[m_nLT]->SetSearchParams(dSearchRadius, lSearchTime);
        pPointMeasView->m_pLTCommandSync[m_nLT]->SetMeasurementMode((ES_MeasMode)nSelMode);
        pPointMeasView->m_pLTCommandSync[m_nLT]->SetCoordinateSystemType((ES_CoordinateSystemType)nSelCoord);
        pPointMeasView->m_pLTCommandSync[m_nLT]->SetReflector((long)((INT_PTR)id));
        AfxMessageBox("��������ǲ������óɹ���");
    } 
    catch(_com_error &e)
    {
        if (m_nPrevMode>=0)        //������ʧ�ܣ���ʾ��һ��������ʽ
            m_Combo_MeasMode.SetCurSel(m_nPrevMode);
        if(m_nPrevRefl>=0)
            m_Combo_Reflector.SetCurSel(m_nPrevRefl);

        AfxMessageBox("��������ǲ�������ʧ�ܣ�");        
    }

    UpdateData(FALSE);
}
