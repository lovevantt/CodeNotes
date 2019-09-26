#pragma once

// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被覆盖。

/////////////////////////////////////////////////////////////////////////////
// CLtvideo2ctrl 包装类

class CLtvideo2ctrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CLtvideo2ctrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x21E4BE41, 0x79E1, 0x4C35, { 0x9F, 0xC2, 0xC9, 0x1E, 0xEA, 0x64, 0x68, 0x7D } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// 属性
public:


// 操作
public:

// _DLTVideo

// Functions
//

	void StartLiveImage()
	{
		InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void StopLiveImage()
	{
		InvokeHelper(0x7, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void FrameRateStepUp()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void FrameRateStepDown()
	{
		InvokeHelper(0x9, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void GetCameraParameters()
	{
		InvokeHelper(0xa, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void SetFrameRate(long frameRate)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_EMPTY, NULL, parms, frameRate);
	}
	void GetFrameRate()
	{
		InvokeHelper(0xc, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

long GetCHColorR()
{
	long result;
	GetProperty(0x1, VT_I4, (void*)&result);
	return result;
}
void SetCHColorR(long propVal)
{
	SetProperty(0x1, VT_I4, propVal);
}
long GetCHColorG()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}
void SetCHColorG(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}
long GetCHColorB()
{
	long result;
	GetProperty(0x3, VT_I4, (void*)&result);
	return result;
}
void SetCHColorB(long propVal)
{
	SetProperty(0x3, VT_I4, propVal);
}
CString GetServerAddress()
{
	CString result;
	GetProperty(0x4, VT_BSTR, (void*)&result);
	return result;
}
void SetServerAddress(CString propVal)
{
	SetProperty(0x4, VT_BSTR, propVal);
}
long GetPortNumber()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}
void SetPortNumber(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}


};
