#pragma once

class CSpatialAnalyzerSDK;

class SDKHelper
{
public:
	SDKHelper(CSpatialAnalyzerSDK& sdkInterface);
	virtual ~SDKHelper(void);

	virtual bool GetMPStepMessagesHelper(CStringArray& messagesOut);
	virtual bool GetTransformArgHelper(const CString& argName, double matrixOut[][4]);
	virtual bool SetTransformArgHelper(const CString& argName, const double matrixIn[][4]);
	virtual bool GetWorldTransformArgHelper(const CString& argName, double matrixOut[][4], double& scale);
	virtual bool SetWorldTransformArgHelper(const CString& argName, const double matrixIn[][4], const double& scale);

	virtual void DebugMatrix(const double T[][4]);
	virtual bool SetCollectionObjectNameRefListArgHelper(const CString& argName, const CStringArray& objects);
	virtual bool GetCollectionObjectNameRefListArgHelper(const CString& argName, CStringArray& objectListOut);
	virtual bool SetPointNameRefListArgHelper(const CString& argName, const CStringArray& points);
	virtual bool GetPointNameRefListArgHelper(const CString& argName, CStringArray& pointListOut);
	virtual bool SetVectorNameRefListArgHelper(const CString& argName, const CStringArray& vectors);
	virtual bool GetVectorNameRefListArgHelper(const CString& argName, CStringArray& vectorListOut);
	virtual bool SetCollectionVectorGroupNameRefListArgHelper(const CString& argName, const CStringArray& objects);
	virtual bool GetCollectionVectorGroupNameRefListArgHelper(const CString& argName, CStringArray& objectListOut);
	virtual bool SetCollectionGroupNameRefListArgHelper(const CString& argName, const CStringArray& groups);
	virtual bool GetCollectionGroupNameRefListArgHelper(const CString& argName, CStringArray& groupListOut);
	virtual bool SetColInstIdRefListArgHelper(const CString& argName, const CStringArray& objects);
	virtual bool GetColInstIdRefListArgHelper(const CString& argName, CStringArray& objectListOut);
	virtual bool SetStringRefListArgHelper(const CString& argName, const CStringArray& stringList);
	virtual bool GetStringRefListArgHelper(const CString& argName, CStringArray& stringListOut);

protected:
	CSpatialAnalyzerSDK& m_interface;
};
