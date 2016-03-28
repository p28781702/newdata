#pragma once
#include "ExchangeBase.h"
class CXSGE :
	public CExchangeBase
{
public:
	CXSGE(std::string strDate);
	~CXSGE();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

