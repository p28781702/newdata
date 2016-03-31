#pragma once
#include "ExchangeBase.h"
class CXZCE :
	public CExchangeBase
{
public:
	CXZCE(const std::string& strDate);
	virtual ~CXZCE();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

