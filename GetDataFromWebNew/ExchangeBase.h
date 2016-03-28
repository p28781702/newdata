#pragma once
#include <string>
#include <vector>
#include "CommonFuc.h"
class CExchangeBase
{
public:
	CExchangeBase();
	CExchangeBase(std::string strDate);
	virtual ~CExchangeBase();

public:
	virtual bool GetDataFromWeb(std::vector<std::string>& vOut)=0;//从网站下载数据
	std::string m_strDate=_T("");
	std::string m_strPath=_T("");
	std::string m_strECchangeName=_T("");
	std::string m_strErrInfo = _T("");

};

