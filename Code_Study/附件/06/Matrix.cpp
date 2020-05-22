#include "StdAfx.h"
#include "Matrix.h"
#include "math.h"



CMatrix::CMatrix(void)
{
	m_nNumColumns = 1;
	m_nNumRows = 1;
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);
}
CMatrix::CMatrix(int nRows, int nCols)
{
	m_nNumRows = nRows;
	m_nNumColumns = nCols;
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);
}
CMatrix::CMatrix(const CMatrix& other)
{
	m_nNumColumns = other.GetNumColumns();
	m_nNumRows = other.GetNumRows();
	m_pData = NULL;
	BOOL bSuccess = Init(m_nNumRows, m_nNumColumns);
	ASSERT(bSuccess);

	// copy the pointer
	memcpy(m_pData, other.m_pData, sizeof(double)*m_nNumColumns*m_nNumRows);
}
CMatrix::~CMatrix(void)
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}
double CMatrix::GetElement(int nRow, int nCol) const
{
	ASSERT(nCol >= 0 && nCol < m_nNumColumns && nRow >= 0 && nRow < m_nNumRows); // array bounds error
	ASSERT(m_pData);							// bad pointer error
	return  m_pData[nCol + nRow * m_nNumColumns] ;

}

BOOL CMatrix::SetElement(int nRow, int nCol, double value)
{
	if (nCol < 0 || nCol >= m_nNumColumns || nRow < 0 || nRow >= m_nNumRows)
		return FALSE;					      	// array bounds error
	if (m_pData == NULL)
		return FALSE;							// bad pointer error

	m_pData[nCol + nRow * m_nNumColumns] = value;

	return TRUE;
}
// 1. int nRows - ָ���ľ������� 2. int nCols - ָ���ľ�������
BOOL CMatrix::Init(int nRows, int nCols)
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}

	m_nNumRows = nRows;
	m_nNumColumns = nCols;
	int nSize = nCols*nRows;
	if (nSize < 0)
		return FALSE;

	// �����ڴ�
	m_pData = new double[nSize];

	if (m_pData == NULL)
		return FALSE;					// �ڴ����ʧ��
	if (IsBadReadPtr(m_pData, sizeof(double) * nSize))
		return FALSE;

	// ����Ԫ��ֵ��0
	memset(m_pData, 0, sizeof(double) * nSize);

	return TRUE;
}
// ��ȡ���������
int	CMatrix::GetNumColumns() const
{
	return m_nNumColumns;
}
// ��ȡ���������
int	CMatrix::GetNumRows() const
{
	return m_nNumRows;
}
// �����ת��
CMatrix CMatrix::Transpose() const
{
	// ����Ŀ�����
	CMatrix	Trans(m_nNumColumns, m_nNumRows);

	// ת�ø�Ԫ��
	double dd=0.0;
	for (int i = 0 ; i < m_nNumRows ; i++)
	{
		for (int j = 0 ; j < m_nNumColumns ;j++)
		{
			Trans.SetElement(j, i, GetElement(i,j)) ;
		}
	}
    
	return Trans;
}

CString CMatrix::ToString(const CString& sDelim /*= " "*/, BOOL bLineBreak /*= TRUE*/) const
{
	CString s="";

	for (int i=0; i<m_nNumRows; ++i)
	{
		for (int j=0; j<m_nNumColumns; ++j)
		{
			CString ss;
			ss.Format("%f", GetElement(i, j));
			s += ss;

			if (bLineBreak)
			{
				if (j != m_nNumColumns-1)
					s += sDelim;
			}
			else
			{
				if (i != m_nNumRows-1 || j != m_nNumColumns-1)
					s += sDelim;
			}
		}
		if (bLineBreak)
			if (i != m_nNumRows-1)
				s += "\r\n";
	}

	return s;
}
// ���������=,������ֵ
CMatrix& CMatrix::operator=(const CMatrix& other)
{
	if (&other != this)
	{
		BOOL bSuccess = Init(other.GetNumRows(), other.GetNumColumns());
		ASSERT(bSuccess);

		// copy the pointer
		memcpy(m_pData, other.m_pData, sizeof(double)*m_nNumColumns*m_nNumRows);
	}
	// finally return a reference to ourselves
	return *this ;
}
//////////////////////////////////////////////////////////////////////
// ���������*��ʵ�־���ĳ˷�
//
// ������
// 1. const CMatrix& other - ��ָ��������˵ľ���
//
// ����ֵ��CMatrix�ͣ�ָ��������other���֮��
//////////////////////////////////////////////////////////////////////
CMatrix	CMatrix::operator*(const CMatrix& other) const
{
	// ���ȼ���������Ƿ����Ҫ��
	ASSERT (m_nNumColumns == other.GetNumRows());

	// construct the object we are going to return
	CMatrix	result(m_nNumRows, other.GetNumColumns()) ;

	// ����˷�����
	//
	// [A][B][C]   [G][H]     [A*G + B*I + C*K][A*H + B*J + C*L]
	// [D][E][F] * [I][J] =   [D*G + E*I + F*K][D*H + E*J + F*L]
	//             [K][L]
	//
	double	value ;
	for (int i = 0 ; i < result.GetNumRows() ; i++)
	{
		for (int j = 0 ; j < other.GetNumColumns() ; j++)
		{
			value = 0.0 ;
			for (int k = 0 ; k < m_nNumColumns ; k++)
			{
				value += GetElement(i, k) * other.GetElement(k, j) ;
			}

			result.SetElement(i, j, value) ;
		}
	}

	return result ;
}
//////////////////////////////////////////////////////////////////////
// ���������+��ʵ�־���ļӷ�
//
// ������
// 1. const CMatrix& other - ��ָ��������ӵľ���
//
// ����ֵ��CMatrix�ͣ�ָ��������other���֮��
//////////////////////////////////////////////////////////////////////
CMatrix	CMatrix::operator+(const CMatrix& other) const
{
	// ���ȼ���������Ƿ����
	ASSERT (m_nNumColumns == other.GetNumColumns() && m_nNumRows == other.GetNumRows());

	// ����������
	CMatrix	result(*this) ;		// ��������
	// ����ӷ�
	for (int i = 0 ; i < m_nNumRows ; ++i)
	{
		for (int j = 0 ; j <  m_nNumColumns; ++j)
			result.SetElement(i, j, result.GetElement(i, j) + other.GetElement(i, j)) ;
	}

	return result ;
}
CMatrix CMatrix::operator-(const CMatrix& other)const
{
	ASSERT (m_nNumColumns == other.GetNumColumns() && m_nNumRows == other.GetNumRows());

	// ����������
	CMatrix	result(*this) ;		// ��������
	// ����ӷ�
	for (int i = 0 ; i < m_nNumRows ; ++i)
	{
		for (int j = 0 ; j <  m_nNumColumns; ++j)
			result.SetElement(i, j, result.GetElement(i, j) - other.GetElement(i, j)) ;
	}
	return result ;
}
CMatrix CMatrix::Inverse(void)  //��������
{
    ASSERT(m_nNumRows==m_nNumColumns);
	CMatrix Inver(m_nNumRows,m_nNumColumns);
	CMatrix matT(m_nNumRows,2*m_nNumColumns);
	//������������
    for(int i=0;i<m_nNumRows;i++)
	{
		for(int j=0;j<2*m_nNumColumns;j++)
		{
			if(i==j-m_nNumColumns)
			{
				matT.SetElement(i,j,1);
			}
			if(j<m_nNumColumns)
			{
				matT.SetElement(i,j,GetElement(i,j));
			}
		}
	}
	int i=0;
	for(int j=0;j<m_nNumColumns;j++)
	{
		int k=matT.FindMain(j);
		matT.Exchange(k,i);
		double dTim;
		dTim=matT.GetElement(i,i);
		for(int n=0;n<2*m_nNumColumns;n++)
		{
			matT.SetElement(i,n,matT.GetElement(i,n)/dTim);
		}
		for(int m=0;m<m_nNumRows;m++)
		{
			if(m==i) continue;
			double dRowT;
			dRowT=matT.GetElement(m,i)/matT.GetElement(i,i);
			for(int p=0;p<2*m_nNumColumns;p++)
			{
                 matT.SetElement(m,p,matT.GetElement(m,p)-dRowT*matT.GetElement(i,p));
			}
		}
		i++;
	}
	for(int i=0;i<m_nNumRows;i++)
	{
		for(int j=0;j<m_nNumColumns;j++)
		{
			Inver.SetElement(i,j,matT.GetElement(i,j+m_nNumColumns));
		}
	}
	return Inver;
}

bool CMatrix::IsInversable(void) //�жϾ����Ƿ���棬����Ԫ��˹��ȥ��
{
	ASSERT(m_nNumRows==m_nNumColumns);
	int k;
	double dFac,dElement;
	for(int j=0;j<m_nNumColumns;j++)
	{
        k=FindMain(j);
		if(GetElement(k,j)==0)
			return false;
		if(k!=0)
		{
			Exchange(k,j);
		}
		for(int i=j+1;i<m_nNumRows;i++)
		{
			dFac=GetElement(i,j)/GetElement(j,j);
			if(dFac!=0)
			{
				for(int m=j+1;m<m_nNumColumns;m++)
				{
					dElement=GetElement(i,m)-dFac*GetElement(j,m);
					SetElement(i,m,dElement);
				}
			}    
		}
	}
	return true;
}

int CMatrix::FindMain(int nCol)  //Ѱ����Ԫ���������У�������Ԫ���ڵ���
{
	double dMax;                 //��¼��������Ԫ��ֵ
	int nRow=nCol;                          //��¼��Ԫ���ڵ���
    dMax=GetElement(nRow,nCol);
    for(int i=0;i<m_nNumRows;i++)
	{
		if(i==nRow) continue;
		if(dMax<GetElement(i,nCol))
		{
			dMax=GetElement(i,nCol);
			nRow=i;
		}
	}
	return nRow;
}

void CMatrix::Exchange(int nARow, int nBRow) // ���� 
{
     double dElem=0;
     for(int j=0;j<m_nNumColumns;j++)
	 {
        dElem=GetElement(nARow,j);
		SetElement(nARow,j,GetElement(nBRow,j));
		SetElement(nBRow,j,dElem);
	 }
}


