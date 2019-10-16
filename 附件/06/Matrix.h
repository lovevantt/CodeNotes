#pragma once

class CMatrix
{
public:
	CMatrix(void);
	CMatrix(int nRows, int nCols);
	CMatrix(const CMatrix& other);
	~CMatrix(void);
	double	GetElement(int nRow, int nCol) const;         // ��ȡָ��Ԫ�ص�ֵ
	BOOL	SetElement(int nRow, int nCol, double value); // ����ָ��Ԫ�ص�ֵ
	BOOL    Init(int nRows, int nCols);                   // ��ʼ������
	int	    GetNumColumns() const;                        // ��ȡ���������
	int	    GetNumRows() const;                           // ��ȡ���������
	CMatrix Transpose() const;                            // �����ת��
	CMatrix&  operator=(const CMatrix& other);			  // ����=�����
	CMatrix	operator*(const CMatrix& other) const;
	CMatrix	operator+(const CMatrix& other) const;
    CMatrix operator-(const CMatrix& other) const;
	CString ToString(const CString& sDelim /*= " "*/, BOOL bLineBreak /*= TRUE*/) const;

public:
	int	m_nNumColumns;			// ��������
	int	m_nNumRows;				// ��������
	double*	m_pData;			// �������ݻ�����
	CMatrix Inverse(void);
	bool IsInversable(void);
	int FindMain(int nCol);
	void Exchange(int nARow, int nBRow);
	
};
