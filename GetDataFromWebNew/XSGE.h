#pragma once
#include "ExchangeBase.h"
class CXSGE :
	public CExchangeBase
{
public:
	CXSGE(const std::string& strDate);
	virtual ~CXSGE();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

