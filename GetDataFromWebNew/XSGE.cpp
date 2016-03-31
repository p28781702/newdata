#include "stdafx.h"
#include "XSGE.h"
using namespace std;

CXSGE::CXSGE(const std::string& strDate):CExchangeBase(strDate)
{
	m_strECchangeName = _T("XSGE");
	m_strPath = _T("..\\data\\XSGE" + strDate + _T(".txt"));
	m_strHost = "www.shfe.com.cn";
	m_strUrl = "/data/dailydata/kx/kx" + m_strDate + ".dat";
	m_strPort = "80";
}


CXSGE::~CXSGE()
{
}

bool CXSGE::GetDataFromWeb(std::vector<std::string>& vOut)
{

	//拿到数据
	string strResult;
	if (!httpGet(strResult))
	{
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = _T("当天没有交易数据");
		return false;
	}
	strResult.erase(remove_if( strResult.begin(),strResult.end(),bind2nd( equal_to<char>(),' ')),strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\t')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\r\n')), strResult.end());


	/////////////////////////////////////////////////////解析写入
	const std::regex pattern1("^\\{\"o_curinstrument\":(.*)\\}$"); // ^\{"o_curinstrument":(.*)\}
	const std::regex pattern2("\\{.*?\\}"); // {.*?}
	string strTmp;
	smatch smtc;
	const std::sregex_token_iterator end;
	ofstream fileout(m_strPath);
	if (!fileout.is_open())
	{
		throw runtime_error(m_strECchangeName + "打开文件失败");
	}
	if (!regex_match(strResult, smtc, pattern1))
	{
		m_strErrInfo = "数据格式出错！";
		return false;
	}
	strResult = smtc[1];
	if ( !regex_search( strResult ,smtc,pattern2))
	{
		m_strErrInfo = "数据格式出错！";
		return false;
	}
	for (std::sregex_token_iterator i(strResult.begin(), strResult.end(), pattern2); i != end; ++i)
	{
		strTmp = *i;
		vector<string> vec;
		vector<string> vec2;
		SpliptString(strTmp, _T(","), vec);
		for (auto i = vec.begin(); i != vec.end(); ++i)
		{
			strTmp = *i;
			strTmp = strTmp.substr(strTmp.find(":") + 1, strTmp.size() - 1);
			strTmp.erase(remove_if(strTmp.begin(), strTmp.end(), bind2nd(equal_to<char>(), '\"')), strTmp.end());
			vec2.push_back(strTmp);
		}
		//string sss = vec2[3];
		if (string(vec2[3])[0]< 0 || vec2.size() < 13)
		{
			continue;
		}

		//拼凑写入
		string strName;//合约代码
		strName = vec2[0];
		if (strName[strName.size() - 1] != 'f')
		{
			continue;
		}
		strName = strName.substr(0, strName.size() - 2);//去掉_f
		strName += vec2[3];
		//1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		string strLine;
		for (auto it = vec2.begin(); it != vec2.end();++it)
		{
			if ( (*it).empty())
			{
				*it = "0";
			}
		}
		strLine = strName + "|XSGE|" + m_strDate + "|" + vec2[5] + "|" + vec2[6] + "|" + vec2[7] + "|" + vec2[8] + "|"
			+ vec2[12] + "|0|" + vec2[9] + "|" + vec2[13];
		vOut.push_back(strLine);
		fileout << strLine << endl;

	}
	fileout.close();
	return true;
}