#pragma once
#include "ExchangeBase.h"
class CCFX :
	public CExchangeBase
{
public:
	CCFX(std::string strDate);
	~CCFX();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

