#include "stdafx.h"
#include "ExchangeBase.h"


CExchangeBase::CExchangeBase()
{
}
CExchangeBase::CExchangeBase(std::string strDate):m_strDate(strDate)
{
	m_strPath = _T("..\\data\\") + strDate + _T(".txt");
}

CExchangeBase::~CExchangeBase()
{
}
