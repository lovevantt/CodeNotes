//#ifndef WINVER
//#if _MSC_VER <= 1200 // up to VC6.0
//    #define WINVER 0x0400
//#else
//    #define WINVER 0x0500
//#endif
//#endif
#include "stdafx.h"
#include <afxdisp.h>
#include <atlbase.h>
#include "LtcCPPEventSink.h"
#include "HorizontalAdjust.h"
#include "MainFrm.h"
#include "DLTMeasureView.h"
#include "IAdjustView.h"
#include "DLTControlBar.h"
#include "math.h"
#include "OutputBar.h"
#include "ADO.h"
#include "CoordTrans.h"
#include "ChildFrm.h"
#include "HSphereCenterCaliView.h"
#include "LocatorAxisCalView.h"
#include "WholeAxisCaliView.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
extern CIAdjustView *pAdjustView;
extern COutputBar* pOutputBar;
extern CHorizontalAdjustApp theApp;
void InitReflectors(LPCTSTR name,long lID,long lTotalRefls,long lReflectorsArrived,CDLTMeasureView* pWnd, ILTCommandSyncPtr pLTCommandSync);

IMPLEMENT_DYNAMIC(CLtcCppEventSink, CCmdTarget)

CLtcCppEventSink::CLtcCppEventSink()
{
    m_nReflectorsArrived = 0;
	m_nMeasCurNum = -1;
	m_dMeasTolNum = 50;
	EnableAutomation();

	pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	pMainWnd=(CDLTMeasureView *)pMain->GetActiveView();
}

CLtcCppEventSink::~CLtcCppEventSink()
{
   // Todo: add remark to LTCppClient2 about MFC event sink !
}


void CLtcCppEventSink::OnFinalRelease()
{
	// When the last reference for an automation object is released OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CLtcCppEventSink, CCmdTarget)

END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CLtcCppEventSink, CCmdTarget)
   DISP_FUNCTION_ID(CLtcCppEventSink, "OnErrorEvent", 1, OnErrorEvent, VT_EMPTY, VTS_I4 VTS_I4)
   DISP_FUNCTION_ID(CLtcCppEventSink, "OnReflectorsData", 2, OnReflectorsData, VT_EMPTY, VTS_I4 VTS_BSTR VTS_I4 VTS_R8 VTS_I4)
   DISP_FUNCTION_ID(CLtcCppEventSink, "OnContinuousPointDataReady", 3, OnContinuousPointDataReady, VT_EMPTY, VTS_I4 VTS_I4)
END_DISPATCH_MAP()


BEGIN_INTERFACE_MAP(CLtcCppEventSink, CCmdTarget)
   INTERFACE_PART(CLtcCppEventSink, DIID__ILTCommandSyncEvents, Dispatch)
END_INTERFACE_MAP()

// CLtcCppEventSink message handlers
void CLtcCppEventSink::OnErrorEvent(ES_Command command, ES_ResultStatus status)
{
   //TRACE("ErrorEvent Event received\n");
	Beep(2000, 50);

	CString str;
	str.Format(_T("command=%d, status=%d"), command, status);
	if(int(command)==64 && int(status)==701)
		pOutputBar->SetInfoText("激光跟踪仪断光");
	else
		pOutputBar->SetInfoText("激光跟踪仪报错,代码:"+str);
	//AfxMessageBox(s);因为弹出式消息框在断光发生后会出现要单击确定才能消失，暂时不知道如何读取激光跟踪仪已经连上光而去控制消息框消失。所以取消这个弹出式消息提示框
}

void CLtcCppEventSink::OnReflectorsData(long reflectorID, BSTR reflectorName, ES_TargetType targetType, double surfaceOffset, long reflectorsTotal)
{
   m_nReflectorsArrived++; // increment with each incoming reflector packet
  
   if (m_nReflectorsArrived)
   {
      TRACE(_T("OnReflectorsData (Event), Name=%s, ID=%ld, Type=%d, Offest=%lf, numRefl=%d\n"), reflectorName, reflectorID, targetType, surfaceOffset, reflectorsTotal);

      DisplayReflectorData((LPCTSTR)reflectorName, reflectorID, reflectorsTotal);
   }

   if (m_nReflectorsArrived == reflectorsTotal) // the last one has arrived
      m_nReflectorsArrived = 0; // reset to 0;
}

void CLtcCppEventSink::OnContinuousPointDataReady(long resultsTotal, long bytesTotal)
{
   VARIANT vt;
   VariantInit(&vt);
   
   // pointer must have been set before by calling SetConnectPtr from parent class
   ASSERT(m_pLTConnect != NULL);

   if (m_pLTConnect == NULL)
     return; // SetConnectPtr() was not called

   m_pLTConnect->GetData(&vt);

   ASSERT(vt.parray->rgsabound[0].cElements == (unsigned long)bytesTotal);
   if (/*pMainWnd->m_Combo_MeasMode.GetCurSel()==1*/1)
   {
	   TRACE(_T("OnContinuousPointDataReady (Event), bytesRead=%d\n"), vt.parray->rgsabound[0].cElements);

       MultiMeasResultT *pData = (MultiMeasResultT *)vt.parray->pvData;
       ASSERT(pData->lNumberOfResults == resultsTotal);
       DisplayContinuousMeasurements(pData);
   }
   
   // Cleaning up Variant avoids memory leaks. Very important here since OnContinuousPointDataReady() may get called frequently 
   // and would make the application eat up more and more memory.

   VariantClear(&vt);
}

/////////////////////////////////////////////////////////////////////////////
// Helper functions
void CLtcCppEventSink::DisplayReflectorData(LPCTSTR name, long lID, long lTotalRefls)
{
  InitReflectors(name, lID, lTotalRefls, m_nReflectorsArrived, pMainWnd, pMainWnd->GetSyncObj());
} 

void CLtcCppEventSink::OnContinuousProbeMeasurementAnswer(ProbeContinuousResultT *result) 
{
	TRACE(_T("OnContinuousProbeMeasurementAnswer\n")); // Continuous 6DoF

	CString s;

	for (int i = 0; i < result->lNumberOfResults; i++)
	{
		// same comment as above
		if (result->lNumberOfResults < 10 || i == result->lNumberOfResults - 1)
		{
			// Other than above, here we write out the probeID instead of the temperature.

			s.Format(_T("ProbeID=%d, x=%lf, y=%lf, z=%lf, rx=%lf, ry=%lf, rz=%lf\n"), 
				result->iInternalProbeId, result->data[i].dPosition1, result->data[i].dPosition2, result->data[i].dPosition3, 
				result->data[i].dRotationAngleX, result->data[i].dRotationAngleY, result->data[i].dRotationAngleZ);
			TRACE(s);

			pMainWnd->SetWindowText(s);
		}
	}
}

void CLtcCppEventSink::DisplayContinuousMeasurements(MultiMeasResultT *pData)
{
	static CString s; // static makes it a bit more efficient
	double dVal1,dVal2,dVal3;
	double dTemp,dPress,dHumid;
	for (int i = 0; i < pData->lNumberOfResults; i++)
	{
		s.Format(_T(" %.8lf, %.8lf, %.8lf, %.2lf, %.2lf, %.2lf, (count=%ld)"),
			pData->data[i].dVal1, pData->data[i].dVal2, pData->data[i].dVal3,
			pData->dTemperature, pData->dPressure, pData->dHumidity,
			pData->lNumberOfResults);
		dVal1=pData->data[i].dVal1;
		dVal2=pData->data[i].dVal2;
		dVal3=pData->data[i].dVal3;

		dTemp=pData->dTemperature;
		dPress=pData->dPressure;
		dHumid=pData->dHumidity;

		MultiResultOfMeasure(dVal1,dVal2,dVal3,dTemp,dPress,dHumid);
		// TRACE- out all values
		#ifdef _DEBUG
		CString s2;
		s2.Format(_T("cont meas: (%ld), %ld, %s\n"), pData->lNumberOfResults, i, s);
		TRACE(s2);
		#endif
	} 
}

void InitReflectors(LPCTSTR name, long lID, long lTotalRefls, long nReflectorsArrived, 
                    CDLTMeasureView* pMWnd, ILTCommandSyncPtr pLTCommandSync)
{
   TRACE(_T("InitReflectors\n"));

   // This is helper function to avoid duplicate code.
   // However, this is only for demonstration/tests since in a real application we would decide either for the Sync or the Async
   // interface - never for both simultaneously

   // beep for each incoming reflector
   Beep(300, 50);

   if (nReflectorsArrived == 1) // first one has arrived - clear previous map
   {
      pMWnd->m_ComboD_Reflector.EnableWindow(false);
      pMWnd->GetReflectorMap().RemoveAll();
   }

   pMWnd->GetReflectorMap().SetAt(CString(name), (void*)((INT_PTR)lID));

   CString sName;
   POSITION pos = NULL;
   void *pa = NULL;
   long id = -1;

   if (nReflectorsArrived == lTotalRefls)
   {
      for (pos = pMWnd->GetReflectorMap().GetStartPosition(); pos != NULL; )
      {
         pMWnd->GetReflectorMap().GetNextAssoc(pos, sName, pa);
         TRACE(_T("ID=%ld, NAME=%s\n"), (INT_PTR)pa, sName);

         pMWnd->m_ComboD_Reflector.AddString(sName);
      }

      pLTCommandSync->GetReflector(&id); // get current reflector

      // set current reflecotr - if any
      for (pos = pMWnd->GetReflectorMap().GetStartPosition(); pos != NULL; )
      {
         pMWnd->GetReflectorMap().GetNextAssoc(pos, sName, pa);

         if (id >= 0 && (INT_PTR)pa == id) 
            pMWnd->m_ComboD_Reflector.SelectString(-1, sName);
      }

      pMWnd->m_ComboD_Reflector.EnableWindow(TRUE);
   }
}

void CLtcCppEventSink::ResultAnswer(double dValue[3],double dRMS[3],double dTemp,double dPress,double dHumid)
{
    
	double dFit[7];	//按顺序分别代表XTran-0,YTran-1,ZTran-2;XRot-3,YRot-4,ZRot-5;Scale-6
	CADO adoact;
	CStringArray strArray;
	strArray.RemoveAll();
	adoact.GetSevenTransferParam("LT_TransferSevenParam",strArray,0);
	for(int i=0;i<7;i++)
		dFit[i] = atof(strArray.GetAt(i+9));//由测量坐标系到全局坐标系
// 	for(int i=0;i<7;i++)  
// 		m_dInvFit[i] = atof(strArray.GetAt(i+1));//由全局坐标系到测量坐标系
	double dGloVal[3],dMValue[1][3],dMGloVal[1][3];
	CCoordTrans MyCoordTrans;
	dMValue[0][0] = dValue[0];	dMValue[0][1] = dValue[1];	dMValue[0][2] = dValue[2];
	MyCoordTrans.GetValueSetByTransSeven(dMValue,dMGloVal,1,dFit);		//求全局值dGloVal
	dGloVal[0] = dMGloVal[0][0];	dGloVal[1] = dMGloVal[0][1];	dGloVal[2] = dMGloVal[0][2];
   
    double dRetVal[3];
	pMainWnd->UpdateData(TRUE);
    if(pMainWnd->m_bIsLocal)
	{
		for(int i=0;i<3;i++)
			dRetVal[i]=dValue[i];
	}
	else
	{
		for(int i=0;i<3;i++)
			dRetVal[i]=dGloVal[i];
	}
	CString strPtsName;
	CString str;
	strPtsName=pMainWnd->m_EditD_PtsName;
	int nItemNO = -1 ;

	for(int i=0;i<pMainWnd->m_ctrlDList_LTMeasPts.GetItemCount();i++)
	{
		CString strItem;
		strItem=pMainWnd->m_ctrlDList_LTMeasPts.GetItemText(i,0);
		if(strItem == strPtsName)//如果有重名，返回序号
		{
			nItemNO = i;
		}
	}

	if(nItemNO == -1) //原列表中没这个点
	{
		nItemNO=pMainWnd->m_ctrlDList_LTMeasPts.GetItemCount();
		pMainWnd->m_ctrlDList_LTMeasPts.InsertItem(nItemNO,"");
	}
    pMainWnd->m_ctrlDList_LTMeasPts.SetItemText(nItemNO,0,strPtsName);

	CString strX,strY,strZ;
	strX.Format("%.6f",dRetVal[0]);	pMainWnd->m_ctrlDList_LTMeasPts.SetItemText(nItemNO,1,strX);	
	pMainWnd->XYZ[0]=atof(strX);	pMainWnd->MeansCommand.xyz[0]=atof(strX);
	strY.Format("%.6f",dRetVal[1]);	pMainWnd->m_ctrlDList_LTMeasPts.SetItemText(nItemNO,2,strY);	
	pMainWnd->XYZ[1]=atof(strY);	pMainWnd->MeansCommand.xyz[1]=atof(strY);
	strZ.Format("%.6f",dRetVal[2]);	pMainWnd->m_ctrlDList_LTMeasPts.SetItemText(nItemNO,3,strZ);	
	pMainWnd->XYZ[2]=atof(strZ);	pMainWnd->MeansCommand.xyz[2]=atof(strZ);

	if (theApp.nListNum != -1)
	{
		if (theApp.nListNum == 1 || theApp.nListNum == 2)//水平调整对话框
		{
			CChildFrame* pChildFrame = (CChildFrame*)pMain->GetActiveFrame();
			CIAdjustView *pAdjustView=(CIAdjustView *)pChildFrame->GetActiveView();

			if (theApp.nListNum == 1)//传感器安装底座上的基准孔列表
			{
				pAdjustView->m_ListCtrlI_BasicHole.SetItemText(theApp.m_nSelectBasicHoleNum,1,strX);
				pAdjustView->m_ListCtrlI_BasicHole.SetItemText(theApp.m_nSelectBasicHoleNum,2,strY);
				pAdjustView->m_ListCtrlI_BasicHole.SetItemText(theApp.m_nSelectBasicHoleNum,3,strZ);
			}
			else if (theApp.nListNum == 2)//水平测量点初始位置列表
			{
				double dLTMeas[1][3],dLocalPt[1][3],dHorizontalPt[1][3];//IGPS测量值，局部坐标值，水平坐标值
				double dCATrans[6]={0};

				dLTMeas[0][0]=dRetVal[0];dLTMeas[0][1]=dRetVal[1];dLTMeas[0][2]=dRetVal[2];
				//LT测量坐标转换到标定块局部坐标
				adoact.GetRowContentsInPtsTable_Six("TransferParamLT_to_Local","正转换",-1,dCATrans,0);
				MyCoordTrans.GetValueSetByTransSix(dLTMeas,dLocalPt,1,dCATrans);
				//标定块局部坐标转换到水平坐标
				adoact.GetRowContentsInPtsTable_Six("TransferParamLocal_to_Horizon","正转换",-1,dCATrans,0);
				MyCoordTrans.GetValueSetByTransSix(dLocalPt,dHorizontalPt,1,dCATrans);

				strX.Format("%.6f",dHorizontalPt[0][0]);	pAdjustView->m_ListCtrlI_InitialMeas.SetItemText(theApp.m_nSelectInitialMeasNum,1,strX);
				strY.Format("%.6f",dHorizontalPt[0][1]);	pAdjustView->m_ListCtrlI_InitialMeas.SetItemText(theApp.m_nSelectInitialMeasNum,2,strY);
				strZ.Format("%.6f",dHorizontalPt[0][2]);	pAdjustView->m_ListCtrlI_InitialMeas.SetItemText(theApp.m_nSelectInitialMeasNum,3,strZ);
			}
			theApp.m_nSelectBasicHoleNum=theApp.m_nSelectInitialMeasNum=-1;
			theApp.LTMeasSuccess=TRUE;
			theApp.nListNum = -1;
		}
		else if (theApp.nListNum == 3 || theApp.nListNum == 4)//球心位置标定对话框
		{
			CChildFrame* pChildFrame = (CChildFrame*)pMain->GetActiveFrame();
			CHSphereCenterCaliView *pCenterCaliView=(CHSphereCenterCaliView *)pChildFrame->GetActiveView();
			if (theApp.nListNum == 3)//六点位置列表中的水平测量点
			{
				pCenterCaliView->m_ListCtrlH_SixHPts.SetItemText(theApp.m_nSelectHtipNum,1,strX);
				pCenterCaliView->m_ListCtrlH_SixHPts.SetItemText(theApp.m_nSelectHtipNum,2,strY);
				pCenterCaliView->m_ListCtrlH_SixHPts.SetItemText(theApp.m_nSelectHtipNum,3,strZ);
			}
			else if (theApp.nListNum == 4)//测量球心辅助测量杆上的点
			{
				pCenterCaliView->m_ListCtrlH_Sphere.SetItemText(theApp.m_nSelectSphereNum,1,strX);
				pCenterCaliView->m_ListCtrlH_Sphere.SetItemText(theApp.m_nSelectSphereNum,2,strY);
				pCenterCaliView->m_ListCtrlH_Sphere.SetItemText(theApp.m_nSelectSphereNum,3,strZ);
			}
			theApp.m_nSelectHtipNum=theApp.m_nSelectSphereNum=-1;
			theApp.LTMeasSuccess=TRUE;
			theApp.nListNum = -1;
		}
		else if (theApp.nListNum == 5 || theApp.nListNum == 6 || theApp.nListNum == 7)
		{
			CChildFrame* pChildFrame = (CChildFrame*)pMain->GetActiveFrame();
			CLocatorAxisCalView *pLocatorCaliView=(CLocatorAxisCalView *)pChildFrame->GetActiveView();
			if (theApp.nListNum == 5)//#1定位器
			{
				pLocatorCaliView->m_ListLocator1Meas.SetItemText(theApp.m_nSelectLocator1Num,1,strX);
				pLocatorCaliView->m_ListLocator1Meas.SetItemText(theApp.m_nSelectLocator1Num,2,strY);
				pLocatorCaliView->m_ListLocator1Meas.SetItemText(theApp.m_nSelectLocator1Num,3,strZ);
			}
			else if (theApp.nListNum == 6)//#2定位器
			{
				pLocatorCaliView->m_ListLocator2Meas.SetItemText(theApp.m_nSelectLocator2Num,1,strX);
				pLocatorCaliView->m_ListLocator2Meas.SetItemText(theApp.m_nSelectLocator2Num,2,strY);
				pLocatorCaliView->m_ListLocator2Meas.SetItemText(theApp.m_nSelectLocator2Num,3,strZ);
			}
			else if (theApp.nListNum == 7)//#3定位器
			{
				pLocatorCaliView->m_ListLocator3Meas.SetItemText(theApp.m_nSelectLocator3Num,1,strX);
				pLocatorCaliView->m_ListLocator3Meas.SetItemText(theApp.m_nSelectLocator3Num,2,strY);
				pLocatorCaliView->m_ListLocator3Meas.SetItemText(theApp.m_nSelectLocator3Num,3,strZ);
			}
			theApp.m_nSelectLocator1Num=theApp.m_nSelectLocator2Num=theApp.m_nSelectLocator3Num=-1;
			theApp.LTMeasSuccess=TRUE;
			theApp.nListNum = -1;
		}
		else if (theApp.nListNum == 10 || theApp.nListNum == 11 || theApp.nListNum == 12 || theApp.nListNum == 13 || theApp.nListNum == 15 || theApp.nListNum == 16|| theApp.nListNum == 17)
		{
			CChildFrame* pChildFrame = (CChildFrame*)pMain->GetActiveFrame();
			CWholeAxisCaliView *pWholeAxisCaliView=(CWholeAxisCaliView *)pChildFrame->GetActiveView();
			if (theApp.nListNum == 10)//机翼坐标系
			{
				pWholeAxisCaliView->m_List_WingCoordMeas.SetItemText(theApp.m_nSelWingNum,1,strX);
				pWholeAxisCaliView->m_List_WingCoordMeas.SetItemText(theApp.m_nSelWingNum,2,strY);
				pWholeAxisCaliView->m_List_WingCoordMeas.SetItemText(theApp.m_nSelWingNum,3,strZ);
			}
			else if (theApp.nListNum == 11)//#1定位器
			{
				pWholeAxisCaliView->m_List_Locator1Meas.SetItemText(theApp.m_nSelLocatorOneNum,1,strX);
				pWholeAxisCaliView->m_List_Locator1Meas.SetItemText(theApp.m_nSelLocatorOneNum,2,strY);
				pWholeAxisCaliView->m_List_Locator1Meas.SetItemText(theApp.m_nSelLocatorOneNum,3,strZ);
			}
			else if (theApp.nListNum == 12)//#2定位器
			{
				pWholeAxisCaliView->m_List_Locator2Meas.SetItemText(theApp.m_nSelLocatorTwoNum,1,strX);
				pWholeAxisCaliView->m_List_Locator2Meas.SetItemText(theApp.m_nSelLocatorTwoNum,2,strY);
				pWholeAxisCaliView->m_List_Locator2Meas.SetItemText(theApp.m_nSelLocatorTwoNum,3,strZ);
			}
			else if (theApp.nListNum == 13)//#3定位器
			{
				pWholeAxisCaliView->m_List_Locator3Meas.SetItemText(theApp.m_nSelLocatorThreeNum,1,strX);
				pWholeAxisCaliView->m_List_Locator3Meas.SetItemText(theApp.m_nSelLocatorThreeNum,2,strY);
				pWholeAxisCaliView->m_List_Locator3Meas.SetItemText(theApp.m_nSelLocatorThreeNum,3,strZ);
			}
			else if (theApp.nListNum == 15)//平台坐标系
			{
				pWholeAxisCaliView->m_List_GlobalCoordMeas.SetItemText(theApp.m_nSelGlobalNum,1,strX);
				pWholeAxisCaliView->m_List_GlobalCoordMeas.SetItemText(theApp.m_nSelGlobalNum,2,strY);
				pWholeAxisCaliView->m_List_GlobalCoordMeas.SetItemText(theApp.m_nSelGlobalNum,3,strZ);
			}
			else if (theApp.nListNum == 16)//球心坐标
			{
				pWholeAxisCaliView->m_List_SphereCoordMeas.SetItemText(theApp.m_nSelSphereNum,1,strX);
				pWholeAxisCaliView->m_List_SphereCoordMeas.SetItemText(theApp.m_nSelSphereNum,2,strY);
				pWholeAxisCaliView->m_List_SphereCoordMeas.SetItemText(theApp.m_nSelSphereNum,3,strZ);
			}
			else if (theApp.nListNum == 17)//机身坐标
			{
				pWholeAxisCaliView->m_List_BodyCoordMeas.SetItemText(theApp.m_nSelSphereNum,1,strX);
				pWholeAxisCaliView->m_List_BodyCoordMeas.SetItemText(theApp.m_nSelSphereNum,2,strY);
				pWholeAxisCaliView->m_List_BodyCoordMeas.SetItemText(theApp.m_nSelSphereNum,3,strZ);
			}

			theApp.m_nSelWingNum=theApp.m_nSelGlobalNum=theApp.m_nSelLocatorOneNum=theApp.m_nSelLocatorTwoNum=theApp.m_nSelLocatorThreeNum=theApp.m_nSelSphereNum=-1;
			theApp.LTMeasSuccess=TRUE;
			theApp.nListNum = -1;
		}
	}

	pMainWnd->m_ctrlDList_LTMeasPts.EnsureVisible(nItemNO,TRUE);//保证该行可见（即不用滑动滚动条）
	pMainWnd->SetStaticBitmap(IDC_STATICD_LTStatus,IDB_Signal_Green);
	if ((pMainWnd->m_ComboD_MeasMode.GetCurSel()==2))
	{
		pMainWnd->m_pLTCommandSync->SetMeasurementMode(ES_MM_6DStationary);
	}
	if ((pMainWnd->m_ComboD_MeasMode.GetCurSel()==3))
	{
		pMainWnd->m_pLTCommandSync->SetMeasurementMode(ES_MM_6DContinuousTime);
		pMainWnd->m_pLTCommandSync->SetContinuousTimeModeParams(500, 0, FALSE, (ES_RegionType)0);
		m_pLTConnect->SelectNotificationMethod(LTC_NM_WM_Notify,(long)pMainWnd->m_hWnd,MY_NOTIFY_MSG);
	}

	CString strRMSX,strRMSY,strRMSZ,strTOLRMS;
	strRMSX.Format("%.3f",dRMS[0]);	pMainWnd->dXYZ[0]=atof(strRMSX);	pMainWnd->MeansCommand.dxyz[0]=atof(strRMSX);
    strRMSY.Format("%.3f",dRMS[1]);	pMainWnd->dXYZ[1]=atof(strRMSY);	pMainWnd->MeansCommand.dxyz[1]=atof(strRMSY);
    strRMSZ.Format("%.3f",dRMS[2]);	pMainWnd->dXYZ[2]=atof(strRMSZ);	pMainWnd->MeansCommand.dxyz[2]=atof(strRMSZ);
	strTOLRMS.Format("%.3f",sqrt(dRMS[0]*dRMS[0]+dRMS[1]*dRMS[1]+dRMS[2]*dRMS[2]));	pMainWnd->dXYZ[3]=atof(strTOLRMS);	pMainWnd->MeansCommand.dxyz[3]=atof(strTOLRMS);
	pOutputBar->SetInfoText(strPtsName+"测量完成,RMS误差:X向"+strRMSX+" Y向"+strRMSY+" Z向"+strRMSZ+" 总"+strTOLRMS);
	
	str.Format("%d,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,%.6f,11,0",pMainWnd->MeasStation,pMainWnd->MeansCommand.xyz[0],pMainWnd->MeansCommand.xyz[1],pMainWnd->MeansCommand.xyz[2],pMainWnd->MeansCommand.dxyz[0],pMainWnd->MeansCommand.dxyz[1],pMainWnd->MeansCommand.dxyz[2],pMainWnd->MeansCommand.dxyz[3]);
	if (pMainWnd->m_BConnect == TRUE)
	{
		pMainWnd->SENDOne();
	}

    pMainWnd->OnBnClickedButtondNameadd();
	pMainWnd->UpdateData(FALSE);
}
void CLtcCppEventSink::MultiResultOfMeasure(double x, double y, double z,double dTemp,double dPress,double dHumid)
{
	++m_nMeasCurNum;
	m_dVal[m_nMeasCurNum][0] = x;
	m_dVal[m_nMeasCurNum][1] = y;
	m_dVal[m_nMeasCurNum][2] = z;
	if(m_nMeasCurNum == m_dMeasTolNum-1)
	{
		double dVal[3];
		double dRms[3];
		CalcuMeasResult(m_dVal,dVal,dRms,m_dMeasTolNum);
		ResultAnswer(dVal,dRms,dTemp,dPress,dHumid);
		m_nMeasCurNum = -1;
		if ((pMainWnd->nAutoMeas<4)&&(pMainWnd->m_bAutoMeas==TRUE))
		{
			pMainWnd->OnBnClickedButtondMeasauto();
		}
		else
		{
            pMainWnd->nAutoMeas=0;
            pMainWnd->m_bAutoMeas=FALSE;
		}
	}
}
void CLtcCppEventSink::CalcuMeasResult(double dVal[][3],double dValue[3],double dRMS[3],int nCount)
{
	dValue[0]=dValue[1]=dValue[2]=0.0;
	dRMS[0]=dRMS[1]=dRMS[2]=0.0;
	for(int j=0;j<3;j++)
	{
		for(int i=0;i<nCount;i++)
		{
			dValue[j]+=dVal[i][j]/double(nCount); //求平均值
		}
		double dSum=0;
		for(int i=0;i<nCount;i++)
		{
			dSum+=(dVal[i][j]-dValue[j])*(dVal[i][j]-dValue[j])/double(nCount); //求RMS误差 暂时没用到
		}
		dRMS[j]=sqrt(dSum);
	}
}
