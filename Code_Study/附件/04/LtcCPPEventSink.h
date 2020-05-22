// CLtcCppEventSink1 created by MFC Class wizard
// This is a different approach from the ATL- based on used for LTCPPClient Sample and CPServer and
//may be preferable for MFC client applications

#ifndef _LTCPPEVENTSINK_H
#define _LTCPPEVENTSINK_H
#define MY_NOTIFY_MSG (WM_USER+99)
#pragma once
#import "LTControl.dll" no_namespace, named_guids, inject_statement("#pragma pack(4)")

#include "stdafx.h"
#include "MainFrm.h"
#include "HorizontalAdjust.h"
#include "DLTMeasureView.h"

class CLtcCppEventSink : public CCmdTarget
{
	DECLARE_DYNAMIC(CLtcCppEventSink)

public:
	CLtcCppEventSink();
	virtual ~CLtcCppEventSink();
	virtual void OnFinalRelease();

   void SetConnectPtr(ILTConnectPtr ptr)
   {
      m_pLTConnect=ptr;
   }

protected:
	DECLARE_MESSAGE_MAP()


   afx_msg void OnErrorEvent(ES_Command command, ES_ResultStatus status);
   afx_msg void OnReflectorsData(long reflectorID, BSTR reflectorName, ES_TargetType targetType, double surfaceOffset, long reflectorsTotal);
   afx_msg void OnContinuousPointDataReady(long resultsTotal, long bytesTotal);
   
 
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

private:
   void DisplayReflectorData(LPCTSTR name, long lID, long lTotalRefls);
   void DisplayContinuousMeasurements(MultiMeasResultT *pData);
   void OnContinuousProbeMeasurementAnswer(ProbeContinuousResultT *result);


   ILTConnectPtr m_pLTConnect;
   int m_nReflectorsArrived;
public:
	void ResultAnswer(double dValue[3],double dRMS[3],double dTemp,double dPress,double dHumid);
	void CalcuMeasResult(double dVal[][3],double dValue[3],double dRMS[3],int nCount);
    void MultiResultOfMeasure(double x, double y, double z,double dTemp,double dPress,double dHumid);
	double m_dVal[1000][3];
	int  m_nMeasCurNum;
	long m_dMeasTolNum;

public:
	CMainFrame *pMain;
	CDLTMeasureView *pMainWnd;


	SOCKET socket;
};

#endif // _LTCPPEVENTSINK_H


