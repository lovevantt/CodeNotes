#include "StdAfx.h"
#include "CoordTrans.h"
#include "Matrix.h"
#include "math.h"

CCoordTrans::CCoordTrans(void)
{
}

CCoordTrans::~CCoordTrans(void)
{
}
void CCoordTrans::GetSevenParams(double dCSA[][3], double dCSB[][3], int nCount, double dTran[]) //坐标系拟合 求七参数
{
	ASSERT(nCount>=3);
	int number=0;
	double dDx=0;
	double dDy=0;
	double dDz=0;
	double dm=0;
	double da=0;
	double db=0;
	double dc=0;
	double dCSADist=0.0;
	double dCSBDist=0.0;
	double dErr=0.0;
	for(int i=0;i<nCount;i++)
	{
		for(int j=i+1;j<nCount;j++)
		{
			dCSADist+=GetDistance(dCSA[i],dCSA[j]);
			dCSBDist+=GetDistance(dCSB[i],dCSB[j]);
		}
	}
	dm=dCSBDist/dCSADist-1;
	do
	{
		CMatrix mtxm(3*nCount,6);        
		double m[180]={0}; 
		int i,k = 0;
		for (int i =0;i<nCount;i++)	
		{	
			m[9*i]   =(1+dm)*((cos(da)*sin(db)*cos(dc)-sin(da)*sin(dc))*dCSA[i][1]+(sin(da)*cos(dc)*sin(db)+cos(da)*sin(dc))*dCSA[i][2]);
			m[9*i+1] =(1+dm)*(-sin(db)*cos(dc)*dCSA[i][0]+sin(da)*cos(db)*cos(dc)*dCSA[i][1]-cos(da)*cos(dc)*cos(db)*dCSA[i][2]);
			m[9*i+2] =(1+dm)*(-cos(db)*sin(dc)*dCSA[i][0]+(-sin(da)*sin(db)*sin(dc)+cos(da)*cos(dc))*dCSA[i][1]+(cos(da)*sin(dc)*sin(db)+sin(da)*cos(dc))*dCSA[i][2]);
			m[9*i+3] =(1+dm)*((-cos(da)*sin(db)*sin(dc)-sin(da)*cos(dc))*dCSA[i][1]+(-sin(da)*sin(db)*sin(dc)+cos(da)*cos(dc))*dCSA[i][2]);
			m[9*i+4] =(1+dm)*(sin(db)*sin(dc)*dCSA[i][0]-sin(da)*cos(db)*sin(dc)*dCSA[i][1]+cos(da)*cos(db)*sin(dc)*dCSA[i][2]);
			m[9*i+5] =(1+dm)*(-cos(db)*cos(dc)*dCSA[i][0]+(-sin(da)*sin(db)*cos(dc)-cos(da)*sin(dc))*dCSA[i][1]+(cos(da)*sin(db)*cos(dc)-sin(da)*sin(dc))*dCSA[i][2]);
			m[9*i+6] =(1+dm)*( -cos(da)*cos(db)*dCSA[i][1]-sin(da)*cos(db)*dCSA[i][2]);
			m[9*i+7] =(1+dm)*(cos(db)*dCSA[i][0]+sin(da)*sin(db)*dCSA[i][1]+(-cos(da)*sin(db))*dCSA[i][2]);
			m[9*i+8] = 0.0;	
			mtxm.SetElement(k,0,1);           mtxm.SetElement(k,1,0);           mtxm.SetElement(k,2,0);
			mtxm.SetElement(k,3,m[9*i]);      mtxm.SetElement(k,4,m[9*i+1]);    mtxm.SetElement(k,5,m[9*i+2]);
			mtxm.SetElement(k+1,0,0);         mtxm.SetElement(k+1,1,1);         mtxm.SetElement(k+1,2,0);
			mtxm.SetElement(k+1,3,m[9*i+3]);  mtxm.SetElement(k+1,4,m[9*i+4]);  mtxm.SetElement(k+1,5,m[9*i+5]);
			mtxm.SetElement(k+2,0,0);         mtxm.SetElement(k+2,1,0);         mtxm.SetElement(k+2,2,1);
			mtxm.SetElement(k+2,3,m[9*i+6]);  mtxm.SetElement(k+2,4,m[9*i+7]);  mtxm.SetElement(k+2,5,m[9*i+8]);	
			k += 3;
		}

		CMatrix mtxf(3*nCount,1);
		k = 0;
		for (int i=0;i<nCount;i++)
		{
			mtxf.SetElement(k,0,(1+dm)*(cos(db)*cos(dc)*dCSA[i][0]+(sin(da)*sin(db)*cos(dc)+cos(da)*sin(dc))*dCSA[i][1]+(-cos(da)*cos(dc)*sin(db)+sin(da)*sin(dc))*dCSA[i][2])+dDx-dCSB[i][0]);
			mtxf.SetElement(k+1,0,(1+dm)*(-cos(db)*sin(dc)*dCSA[i][0]+(-sin(da)*sin(db)*sin(dc)+cos(da)*cos(dc))*dCSA[i][1]+(cos(da)*sin(db)*sin(dc)+sin(da)*cos(dc))*dCSA[i][2])+dDy-dCSB[i][1]);
			mtxf.SetElement(k+2,0,(1+dm)*(sin(db)*dCSA[i][0]-sin(da)*cos(db)*dCSA[i][1]+cos(da)*cos(db)*dCSA[i][2])+dDz-dCSB[i][2]);
			k += 3;
		}
		CMatrix mtxb;
		CMatrix mtxc;
		CMatrix mtxx(6,1);
		mtxc = (mtxm.Transpose())*mtxf;	   
		mtxb = (mtxm.Transpose())*mtxm;         
		CMatrix mtest;
		mtest=mtxb;
		if (!mtest.IsInversable()) 
		{
			double p=0.0;
			for (i = 0;	 i < 6;	 i++)        
			{
				p = mtxb.GetElement(i,i) + 0.1;         
				mtxb.SetElement(i,i,p);
			}
		}
		if(!Cholesky(mtxb,mtxc,mtxx))
		{
			AfxMessageBox("请检查测量点坐标值是否有异常，然后重试!",MB_ICONERROR);		
			break;
		}
		double step = 1;   	               
		dDx-=step * mtxx.GetElement(0,0);   
		dDy-=step * mtxx.GetElement(1,0);
		dDz-=step * mtxx.GetElement(2,0);
		da-=step * mtxx.GetElement(3,0);
		db-=step * mtxx.GetElement(4,0);
		dc-=step * mtxx.GetElement(5,0);
		double PI=3.141592653589793;
		while (da>PI)
		{
			da-=2*PI;
		} 
		while (da<-PI)
		{
			da+=2*PI;
		}
		while (db>PI)
		{
			db-=2*PI;
		} 
		while (db<-PI)
		{
			db+=2*PI;
		}
		while (dc>PI)
		{
			dc-=2*PI;
		} 
		while (dc<-PI)
		{
			dc+=2*PI;
		}
		dErr=fabs(step * mtxx.GetElement(0,0))+fabs(step * mtxx.GetElement(1,0))+fabs(step * mtxx.GetElement(2,0))+
			+fabs(step * mtxx.GetElement(3,0))+fabs(step * mtxx.GetElement(4,0))+fabs(step * mtxx.GetElement(5,0));
		number++;
		if (number>10000)
		{
			AfxMessageBox("求解失败,请检查测量点坐标值是否有异常，然后重试!",MB_ICONERROR);
			break;
		}
	} while(dErr>1.0E-6);
	dTran[0]=dDx;dTran[1]=dDy;dTran[2]=dDz;
	dTran[3]=da;dTran[4]=db;dTran[5]=dc;
    dTran[6]=dm;
}

double CCoordTrans::GetDistance(double dPtsA[], double dPtsB[]) //求两点之间距离
{
	return sqrt((dPtsA[0]-dPtsB[0])*(dPtsA[0]-dPtsB[0])+(dPtsA[1]-dPtsB[1])*(dPtsA[1]-dPtsB[1])+(dPtsA[2]-dPtsB[2])*(dPtsA[2]-dPtsB[2]));
}
bool CCoordTrans::Cholesky(CMatrix &matA, CMatrix &matB, CMatrix &matX)		//乔列斯基分解
{
	CMatrix matL(matA.GetNumRows(),matA.GetNumColumns());  //下三角矩阵L
	double dData,dElem;                                                     
	for(int j=0;j<matA.GetNumColumns();j++)                //求L矩阵各元素值
	{
		dData=dElem=0;
		for(int k=0;k<=j-1;k++)
		{
			dData+=matL.GetElement(j,k)*matL.GetElement(j,k);
		}
		if((matA.GetElement(j,j)-dData)<0)
		{
			return false;
		}
		dElem=sqrt(matA.GetElement(j,j)-dData);
		matL.SetElement(j,j,dElem);

		for(int i=j+1;i<matA.GetNumRows();i++)
		{
			dData=dElem=0;
			for(int k=0;k<=j-1;k++)
			{
				dData+=matL.GetElement(i,k)*matL.GetElement(j,k);
			}
			dElem=(matA.GetElement(i,j)-dData)/matL.GetElement(j,j);
			matL.SetElement(i,j,dElem);
		}
	}

	CMatrix matY(matX.GetNumRows(),1);
	for(int i=0;i<matB.GetNumRows();i++)           //求LY=B中的Y
	{
		dData=dElem=0;
		for(int k=0;k<=i-1;k++)
		{
			dData+=matL.GetElement(i,k)*matY.GetElement(k,0);
		}
		dElem=(matB.GetElement(i,0)-dData)/matL.GetElement(i,i);
		matY.SetElement(i,0,dElem);
	}

	for(int i=matX.GetNumRows()-1;i>=0;i--)
	{
		dData=dElem=0;
		for(int k=i+1;k<matX.GetNumRows();k++)
		{
			dData+=matL.GetElement(k,i)*matX.GetElement(k,0);
		}
		dElem=(matY.GetElement(i,0)-dData)/matL.GetElement(i,i);
		matX.SetElement(i,0,dElem);
	}
	return true;
}
//下面两函数为利用七参数求A转换后的值B
void CCoordTrans::GetValueSetByTrans(double dCSA[][3],double dCSB[][3],int nCount,double dTran[7]) //n*3数组A*七参数=数组B 
{
	for(int i=0;i<nCount;i++)
	{
		dCSB[i][0]=(1+dTran[6])*(cos(dTran[4])*cos(dTran[5])*dCSA[i][0]+(sin(dTran[3])*sin(dTran[4])*cos(dTran[5])+cos(dTran[3])*sin(dTran[5]))*dCSA[i][1]
		+(-cos(dTran[3])*sin(dTran[4])*cos(dTran[5])+sin(dTran[3])*sin(dTran[5]))*dCSA[i][2])+dTran[0];
		dCSB[i][1]=(1+dTran[6])*(-cos(dTran[4])*sin(dTran[5])*dCSA[i][0]+(-sin(dTran[3])*sin(dTran[4])*sin(dTran[5])+cos(dTran[3])*cos(dTran[5]))*dCSA[i][1]
		+(cos(dTran[3])*sin(dTran[4])*sin(dTran[5])+sin(dTran[3])*cos(dTran[5]))*dCSA[i][2])+dTran[1];
		dCSB[i][2]=(1+dTran[6])*(sin(dTran[4])*dCSA[i][0]-sin(dTran[3])*cos(dTran[4])*dCSA[i][1]+cos(dTran[3])*cos(dTran[4])*dCSA[i][2])+dTran[2];
	}
}
void CCoordTrans::GetValueByTrans(double dCSA[3],double dCSB[3],double dTran[7]) //3维数组A*七参数=数组B 
{
	dCSB[0]=(1+dTran[6])*(cos(dTran[4])*cos(dTran[5])*dCSA[0]+(sin(dTran[3])*sin(dTran[4])*cos(dTran[5])+cos(dTran[3])*sin(dTran[5]))*dCSA[1]
	+(-cos(dTran[3])*sin(dTran[4])*cos(dTran[5])+sin(dTran[3])*sin(dTran[5]))*dCSA[2])+dTran[0];
	dCSB[1]=(1+dTran[6])*(-cos(dTran[4])*sin(dTran[5])*dCSA[0]+(-sin(dTran[3])*sin(dTran[4])*sin(dTran[5])+cos(dTran[3])*cos(dTran[5]))*dCSA[1]
	+(cos(dTran[3])*sin(dTran[4])*sin(dTran[5])+sin(dTran[3])*cos(dTran[5]))*dCSA[2])+dTran[1];
	dCSB[2]=(1+dTran[6])*(sin(dTran[4])*dCSA[0]-sin(dTran[3])*cos(dTran[4])*dCSA[1]+cos(dTran[3])*cos(dTran[4])*dCSA[2])+dTran[2];
}
void CCoordTrans::GetRotationMatrixByTrans(double dTrans[7], double dMatrix[9])
{
	//X轴
	dMatrix[0]=cos(dTrans[5])*cos(dTrans[4]);//[0][0]
	dMatrix[1]=sin(dTrans[5])*cos(dTrans[4]);//[1][0]
	dMatrix[2]=-sin(dTrans[4]);//[2][0]
	//Y轴
	dMatrix[3]=cos(dTrans[5])*sin(dTrans[4])*sin(dTrans[3])-sin(dTrans[5])*cos(dTrans[3]);//[0][1]
	dMatrix[4]=sin(dTrans[5])*sin(dTrans[4])*sin(dTrans[3])+cos(dTrans[5])*cos(dTrans[3]);//[1][1]
	dMatrix[5]=cos(dTrans[4])*sin(dTrans[3]);//[2][1]
	//Z轴
	dMatrix[6]=cos(dTrans[5])*sin(dTrans[4])*cos(dTrans[3])+sin(dTrans[5])*sin(dTrans[3]);//[0][2]
	dMatrix[7]=sin(dTrans[5])*sin(dTrans[4])*cos(dTrans[3])-cos(dTrans[5])*sin(dTrans[3]);//[1][2]
	dMatrix[8]=cos(dTrans[4])*cos(dTrans[3]);//[2][2]
}
