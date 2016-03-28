#pragma once
#include "ExchangeBase.h"
class CXZCE :
	public CExchangeBase
{
public:
	CXZCE(std::string strDate);
	~CXZCE();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

