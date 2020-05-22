#include "stdafx.h"
#include "SDKHelper.h"
#include "CSpatialAnalyzerSDK.h"

SDKHelper::SDKHelper(CSpatialAnalyzerSDK& sdkInterface) :
m_interface(sdkInterface)
{

}

SDKHelper::~SDKHelper(void)
{

}

bool SDKHelper::GetMPStepMessagesHelper(CStringArray& messagesOut)
{
	bool bSuccess = false;

	VARIANT vMsgs;
	VariantInit(&vMsgs);
	if (m_interface.GetMPStepMessages(&vMsgs))
	{
		COleSafeArray msgArray;
		LONG lstart, lend;
		LONG idx = -1;

		msgArray.Attach(vMsgs);
		msgArray.GetLBound(1,&lstart);
		msgArray.GetUBound(1,&lend);

		// loop
		_variant_t vOut;
		for(idx = lstart; idx <= lend; idx++)
		{		
			//Extract the data from the Array
			msgArray.GetElement(&idx,&vOut);
			CString msg = vOut;
			messagesOut.Add(msg);
		}

		msgArray.Detach();

		bSuccess = true;
	}
	VariantClear(&vMsgs);

	return bSuccess;
}

// NOTE: matrixOut is assumed to be a 4x4
bool SDKHelper::GetTransformArgHelper(const CString& argName, double matrixOut[][4])
{
	bool bSuccess = false;

	VARIANT vMatrix;
	VariantInit(&vMatrix);
	if (m_interface.GetTransformArg(argName, &vMatrix))
	{
		COleSafeArray matrixArray;
		matrixArray.Attach(vMatrix);

		LONG lRowStart, lRowEnd;
		LONG lColStart, lColEnd;
		LONG row = -1;
		LONG col = -1;

		matrixArray.GetLBound(1,&lRowStart);
		matrixArray.GetUBound(1,&lRowEnd);
		matrixArray.GetLBound(2,&lColStart);
		matrixArray.GetUBound(2,&lColEnd);

		long aLoc[2];
		for(row = lRowStart; row <= lRowEnd; row++)
		{		
			aLoc[0] = row;
			for(col = lColStart; col <= lColEnd; col++)
			{	
				aLoc[1] = col;
				double val; 
				matrixArray.GetElement(aLoc, &val);
				matrixOut[row][col] = val;
			}
		}
		matrixArray.Detach();

		bSuccess = true;
	}

	VariantClear(&vMatrix);

	return bSuccess;
}

bool SDKHelper::SetTransformArgHelper(const CString& argName, const double matrixIn[][4])
{
	bool bSuccess = false;

	//////////////////////////////////////////////////////////////////////////////
	// matrixIn assumed to be a two dimensional 4x4 array of doubles

	COleSafeArray matrixArray;
	DWORD numElements[] = {4, 4};
	matrixArray.Create(VT_R8, 2, numElements);
	ASSERT(matrixArray.GetDim() == 2);

	LONG row;
	LONG col;

	long index[2];
	for(row = 0; row < 4; row++)
	{		
		index[0] = row;
		for(col = 0; col < 4; col++)
		{	
			index[1] = col;
			double val = matrixIn[row][col];
			matrixArray.PutElement(index, &val);
		}
	}

	VARIANT* vpTemp = matrixArray;
	if (m_interface.SetTransformArg(argName, vpTemp))
	{
		bSuccess = true;
	}

	return bSuccess;
}

// NOTE: matrixOut is assumed to be a 4x4
bool SDKHelper::GetWorldTransformArgHelper(const CString& argName, double matrixOut[][4], double& scale)
{
	bool bSuccess = false;

	VARIANT vMatrix;
	VariantInit(&vMatrix);
	if (m_interface.GetWorldTransformArg(argName, &vMatrix, &scale))
	{
		COleSafeArray matrixArray;
		matrixArray.Attach(vMatrix);

		LONG lRowStart, lRowEnd;
		LONG lColStart, lColEnd;
		LONG row = -1;
		LONG col = -1;

		matrixArray.GetLBound(1,&lRowStart);
		matrixArray.GetUBound(1,&lRowEnd);
		matrixArray.GetLBound(2,&lColStart);
		matrixArray.GetUBound(2,&lColEnd);

		long aLoc[2];
		for(row = lRowStart; row <= lRowEnd; row++)
		{		
			aLoc[0] = row;
			for(col = lColStart; col <= lColEnd; col++)
			{	
				aLoc[1] = col;
				double val; 
				matrixArray.GetElement(aLoc, &val);
				matrixOut[row][col] = val;
			}
		}
		matrixArray.Detach();

		bSuccess = true;
	}

	VariantClear(&vMatrix);

	return bSuccess;
}

bool SDKHelper::SetWorldTransformArgHelper(const CString& argName, const double matrixIn[][4], const double& scale)
{
	bool bSuccess = false;

	//////////////////////////////////////////////////////////////////////////////
	// matrixIn assumed to be a two dimensional 4x4 array of doubles

	COleSafeArray matrixArray;
	DWORD numElements[] = {4, 4};
	matrixArray.Create(VT_R8, 2, numElements);
	ASSERT(matrixArray.GetDim() == 2);

	LONG row;
	LONG col;

	long index[2];
	for(row = 0; row < 4; row++)
	{		
		index[0] = row;
		for(col = 0; col < 4; col++)
		{	
			index[1] = col;
			double val = matrixIn[row][col];
			matrixArray.PutElement(index, &val);
		}
	}

	VARIANT* vpTemp = matrixArray;
	if (m_interface.SetWorldTransformArg(argName, vpTemp, scale))
	{
		bSuccess = true;
	}

	return bSuccess;
}

void SDKHelper::DebugMatrix(const double T[][4])
{
	int dec = 4;
	for (int r=0;r<4;r++)
	{
		for (int c=0;c<4;c++) TRACE(_T("  %.*f  "),dec,T[r][c]);
		TRACE(_T("\n"));
	}  
}

bool SDKHelper::SetCollectionObjectNameRefListArgHelper(const CString& argName, const CStringArray& objects)
{
	bool bSuccess = false;

	INT_PTR numObjs = objects.GetSize();
	if (numObjs > 0)
	{
		COleSafeArray vObjNameList;
		DWORD numElements[] = { (DWORD) numObjs };
		vObjNameList.Create(VT_VARIANT, 1, numElements);
		ASSERT(vObjNameList.GetDim() == 1);

		long index;
		_variant_t vOut;
		for (LONG row = 0; row < numObjs; row++)
		{
			index = row;
			CString item = objects[row];
			vOut = item;
			vObjNameList.PutElement(&index, &vOut);
		}

		VARIANT* vpTemp = vObjNameList;
		if (m_interface.SetCollectionObjectNameRefListArg(argName, vpTemp))
		{
			bSuccess = true;
		}

		VariantClear(&vObjNameList);
	}

	return bSuccess;
}

bool SDKHelper::GetCollectionObjectNameRefListArgHelper(const CString& argName, CStringArray& objectListOut)
{
	bool bSuccess = false;

	VARIANT vUserObjList;
	VariantInit(&vUserObjList);
	if (m_interface.GetCollectionObjectNameRefListArg(argName, &vUserObjList))
	{
		COleSafeArray objectListArray;
		objectListArray.Attach(vUserObjList);

		LONG lRowStart, lRowEnd;
		LONG row = -1;

		objectListArray.GetLBound(1,&lRowStart);
		objectListArray.GetUBound(1,&lRowEnd);

		long aLoc;
		_variant_t vOut;
		CStringArray objNames;
		for(row = lRowStart; row <= lRowEnd; row++)
		{		
			aLoc = row;
			CString val; 
			objectListArray.GetElement(&aLoc, &vOut);
			val = vOut;
			objectListOut.Add(val);
		}
		objectListArray.Detach();
		bSuccess = true;
	}
	VariantClear(&vUserObjList);

	return bSuccess;
}

bool SDKHelper::SetPointNameRefListArgHelper(const CString& argName, const CStringArray& points)
{
	bool bSuccess = false;

	INT_PTR numObjs = points.GetSize();
	if (numObjs > 0)
	{
		COleSafeArray vPtNameList;
		DWORD numElements[] = { (DWORD) numObjs };
		vPtNameList.Create(VT_VARIANT, 1, numElements);
		ASSERT(vPtNameList.GetDim() == 1);

		long index;
		_variant_t vOut;
		for (LONG row = 0; row < numObjs; row++)
		{
			index = row;
			CString item = points[row];
			vOut = item;
			vPtNameList.PutElement(&index, &vOut);
		}

		VARIANT* vpTemp = vPtNameList;
		if (m_interface.SetCollectionObjectNameRefListArg(argName, vpTemp))
		{
			bSuccess = true;
		}

		VariantClear(&vPtNameList);
	}

	return bSuccess;
}

bool SDKHelper::GetPointNameRefListArgHelper(const CString& argName, CStringArray& pointListOut)
{
	bool bSuccess = false;

	VARIANT vUserPtList;
	VariantInit(&vUserPtList);
	if (m_interface.GetCollectionObjectNameRefListArg(argName, &vUserPtList))
	{
		COleSafeArray ptListArray;
		ptListArray.Attach(vUserPtList);

		LONG lRowStart, lRowEnd;
		LONG row = -1;

		ptListArray.GetLBound(1,&lRowStart);
		ptListArray.GetUBound(1,&lRowEnd);

		long aLoc;
		_variant_t vOut;
		CStringArray objNames;
		for(row = lRowStart; row <= lRowEnd; row++)
		{		
			aLoc = row;
			CString val; 
			ptListArray.GetElement(&aLoc, &vOut);
			val = vOut;
			pointListOut.Add(val);
		}
		ptListArray.Detach();
		bSuccess = true;
	}
	VariantClear(&vUserPtList);

	return bSuccess;
}

bool SDKHelper::SetVectorNameRefListArgHelper(const CString& argName, const CStringArray& vectors)
{
	bool bSuccess = false;

	INT_PTR numObjs = vectors.GetSize();
	if (numObjs > 0)
	{
		COleSafeArray vVectorNameList;
		DWORD numElements[] = { (DWORD) numObjs };
		vVectorNameList.Create(VT_VARIANT, 1, numElements);
		ASSERT(vVectorNameList.GetDim() == 1);

		long index;
		_variant_t vOut;
		for (LONG row = 0; row < numObjs; row++)
		{
			index = row;
			CString item = vectors[row];
			vOut = item;
			vVectorNameList.PutElement(&index, &vOut);
		}

		VARIANT* vpTemp = vVectorNameList;
		if (m_interface.SetCollectionObjectNameRefListArg(argName, vpTemp))
		{
			bSuccess = true;
		}

		VariantClear(&vVectorNameList);
	}

	return bSuccess;
}

bool SDKHelper::GetVectorNameRefListArgHelper(const CString& argName, CStringArray& vectorListOut)
{
	bool bSuccess = false;

	VARIANT vUserVectorList;
	VariantInit(&vUserVectorList);
	if (m_interface.GetCollectionObjectNameRefListArg(argName, &vUserVectorList))
	{
		COleSafeArray vectorListArray;
		vectorListArray.Attach(vUserVectorList);

		LONG lRowStart, lRowEnd;
		LONG row = -1;

		vectorListArray.GetLBound(1,&lRowStart);
		vectorListArray.GetUBound(1,&lRowEnd);

		long aLoc;
		_variant_t vOut;
		CStringArray objNames;
		for(row = lRowStart; row <= lRowEnd; row++)
		{		
			aLoc = row;
			CString val; 
			vectorListArray.GetElement(&aLoc, &vOut);
			val = vOut;
			vectorListOut.Add(val);
		}
		vectorListArray.Detach();
		bSuccess = true;
	}
	VariantClear(&vUserVectorList);

	return bSuccess;
}

bool SDKHelper::SetCollectionVectorGroupNameRefListArgHelper(const CString& argName, const CStringArray& objects)
{
	return SetCollectionObjectNameRefListArgHelper(argName, objects);
}

bool SDKHelper::GetCollectionVectorGroupNameRefListArgHelper(const CString& argName, CStringArray& objectListOut)
{
	return GetCollectionObjectNameRefListArgHelper(argName, objectListOut);
}

bool SDKHelper::SetCollectionGroupNameRefListArgHelper(const CString& argName, const CStringArray& groups)
{
	return SetCollectionObjectNameRefListArgHelper(argName, groups);
}

bool SDKHelper::GetCollectionGroupNameRefListArgHelper(const CString& argName, CStringArray& groupListOut)
{
	return GetCollectionObjectNameRefListArgHelper(argName, groupListOut);
}

bool SDKHelper::SetColInstIdRefListArgHelper(const CString& argName, const CStringArray& objects)
{
	return SetCollectionObjectNameRefListArgHelper(argName, objects);
}

bool SDKHelper::GetColInstIdRefListArgHelper(const CString& argName, CStringArray& objectListOut)
{
	return GetCollectionObjectNameRefListArgHelper(argName, objectListOut);
}

bool SDKHelper::SetStringRefListArgHelper(const CString& argName, const CStringArray& stringList)
{
	bool bSuccess = false;

	INT_PTR numItems = stringList.GetSize();
	if (numItems > 0)
	{
		COleSafeArray vStringList;
		DWORD numElements[] = { (DWORD) numItems };
		vStringList.Create(VT_VARIANT, 1, numElements);
		ASSERT(vStringList.GetDim() == 1);

		long index;
		_variant_t vOut;
		for (LONG row = 0; row < numItems; row++)
		{
			index = row;
			CString item = stringList[row];
			vOut = item;
			vStringList.PutElement(&index, &vOut);
		}

		VARIANT* vpTemp = vStringList;
		if (m_interface.SetStringRefListArg(argName, vpTemp))
		{
			bSuccess = true;
		}

		VariantClear(&vStringList);
	}

	return bSuccess;
}

bool SDKHelper::GetStringRefListArgHelper(const CString& argName, CStringArray& stringListOut)
{
	bool bSuccess = false;

	VARIANT vUserStringList;
	VariantInit(&vUserStringList);
	if (m_interface.GetStringRefListArg(argName, &vUserStringList))
	{
		COleSafeArray stringArray;
		stringArray.Attach(vUserStringList);

		LONG lRowStart, lRowEnd;
		LONG row = -1;

		stringArray.GetLBound(1,&lRowStart);
		stringArray.GetUBound(1,&lRowEnd);

		long aLoc;
		_variant_t vOut;
		CStringArray objNames;
		for(row = lRowStart; row <= lRowEnd; row++)
		{		
			aLoc = row;
			CString val; 
			stringArray.GetElement(&aLoc, &vOut);
			val = vOut;
			stringListOut.Add(val);
		}
		stringArray.Detach();
		bSuccess = true;
	}
	VariantClear(&vUserStringList);

	return bSuccess;
}