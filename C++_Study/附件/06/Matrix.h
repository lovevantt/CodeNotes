#pragma once

class CMatrix
{
public:
	CMatrix(void);
	CMatrix(int nRows, int nCols);
	CMatrix(const CMatrix& other);
	~CMatrix(void);
	double	GetElement(int nRow, int nCol) const;         // 获取指定元素的值
	BOOL	SetElement(int nRow, int nCol, double value); // 设置指定元素的值
	BOOL    Init(int nRows, int nCols);                   // 初始化矩阵
	int	    GetNumColumns() const;                        // 获取矩阵的列数
	int	    GetNumRows() const;                           // 获取矩阵的行数
	CMatrix Transpose() const;                            // 矩阵的转置
	CMatrix&  operator=(const CMatrix& other);			  // 重载=运算符
	CMatrix	operator*(const CMatrix& other) const;
	CMatrix	operator+(const CMatrix& other) const;
    CMatrix operator-(const CMatrix& other) const;
	CString ToString(const CString& sDelim /*= " "*/, BOOL bLineBreak /*= TRUE*/) const;

public:
	int	m_nNumColumns;			// 矩阵列数
	int	m_nNumRows;				// 矩阵行数
	double*	m_pData;			// 矩阵数据缓冲区
	CMatrix Inverse(void);
	bool IsInversable(void);
	int FindMain(int nCol);
	void Exchange(int nARow, int nBRow);
	
};
