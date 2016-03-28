#pragma once
#include <string>
#include <vector>
#include <regex>
#include <fstream>
#include <boost//asio.hpp>
class CCommonFuc
{
public:
	CCommonFuc();
	~CCommonFuc();


public:
	static void Erasetag(std::string& strFull, const std::string& strTag);
	static void SpliptString(const std::string& strFull, std::string strpatten, std::vector<std::string>& vec);
	static bool httpGet(/*out*/std::string& result, const std::string& host, const std::string& url, boost::asio::io_service &_io, std::string& strErrInfo , std::string strPort=_T("80"));



};

