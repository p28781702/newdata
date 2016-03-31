#pragma once
#include "ExchangeBase.h"
class CXDCE :
	public CExchangeBase
{
public:
	CXDCE(const std::string& strDate);
	virtual ~CXDCE();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

