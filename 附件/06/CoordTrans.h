#pragma once
#include "Matrix.h"

class CCoordTrans
{
public:
	CCoordTrans(void);
	~CCoordTrans(void);
public:
	bool Cholesky(CMatrix& matA, CMatrix& matB, CMatrix& matX);
	double GetDistance(double dPtsA[3],double dPtsB[3]);
	void GetSevenParams(double dCSA[][3],double dCSB[][3],int nCount,double dTran[7]);
	void GetValueSetByTrans(double dCSA[][3],double dCSB[][3],int nCount,double dTran[7]);
    void GetValueByTrans(double dCSA[3],double dCSB[3],double dTran[7]);
	void GetRotationMatrixByTrans(double dTrans[7], double dMatrix[9]);
};
