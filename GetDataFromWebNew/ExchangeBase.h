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
	virtual bool GetDataFromWeb(std::vector<std::string>& vOut)=0;//����վ��������
	bool httpGet(/*out*/std::string& strOut);
	static void SpliptString(const std::string& strFull, std::string strpatten, std::vector<std::string>& vec);
	std::string GetErr() const;
	static bool CheckVal(std::vector<std::string>& vec);


protected:
	std::string m_strDate = "";//����
	std::string m_strPath = "";//·��
	std::string m_strECchangeName = "";//������
	std::string m_strErrInfo = "";//������Ϣ
	std::string m_strHost = "";//����
	std::string m_strUrl = "";//����
	std::string m_strPort = "";//�˿�

};

