#pragma once
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <boost//asio.hpp>
class CExchangeBase
{
public:
	CExchangeBase();
	CExchangeBase(const std::string& strDate);
	virtual ~CExchangeBase();


public:
	virtual bool GetDataFromWeb(std::vector<std::string>& vOut)=0;//从网站下载数据
	bool httpGet(/*out*/std::string& strOut);
	static void SpliptString(const std::string& strFull, std::string strpatten, std::vector<std::string>& vec);
	std::string GetErr() const;
	static bool CheckVal(std::vector<std::string>& vec);


protected:
	std::string m_strDate = "";//日期
	std::string m_strPath = "";//路径
	std::string m_strECchangeName = "";//交易所
	std::string m_strErrInfo = "";//错误信息
	std::string m_strHost = "";//域名
	std::string m_strUrl = "";//链接
	std::string m_strPort = "";//端口

};

