#pragma once
#include "ExchangeBase.h"
class CCFX :
	public CExchangeBase
{
public:
	CCFX(const std::string& strDate);
	virtual ~CCFX();

public:
	bool GetDataFromWeb(std::vector<std::string>& vOut);
};

