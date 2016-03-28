#pragma once
#include "ExchangeBase.h"
class CXDCE :
	public CExchangeBase
{
public:
	CXDCE(std::string strDate);
	~CXDCE();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

