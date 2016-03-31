#include "stdafx.h"
#include "XZCE.h"
using namespace std;

CXZCE::CXZCE(const std::string& strDate) :CExchangeBase(strDate)
{
	m_strECchangeName = _T("XZCE");
	m_strPath = _T("..\\data\\XZCE" + strDate + _T(".txt"));
	m_strHost= "www.czce.com.cn";
	m_strUrl= "/portal/DFSStaticFiles/Future/" + string(m_strDate.substr(0,4)) + "/" + m_strDate + "/FutureDataDaily.txt";
	m_strPort = "80";
}


CXZCE::~CXZCE()
{
}

bool CXZCE::GetDataFromWeb(std::vector<std::string>& vOut)
{
	//下载数据
	string strResult;
	if (!httpGet(strResult))
	{
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = "当天没有交易数据!";
		return false;
	}
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ',')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ' ')), strResult.end());

	//解析写入
	vector<string> vec;
	SpliptString(strResult, _T("\r\n"), vec);
	string strFilePath =m_strPath;
	ofstream fileout(strFilePath);
	if ( !fileout.is_open() )
	{
		throw runtime_error( m_strECchangeName + "打开文件失败");
	}
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		string strTmp = *it;
		if (strTmp.empty())
		{
			continue;
		}

		int i = strTmp[0];
		if (i < 65 || i>122)
		{
			continue;
		}

		vector<string> vTmp;
		SpliptString(strTmp, "|", vTmp);
		//1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		strTmp = vTmp[0] + "|XZCE|" + m_strDate + "|" + vTmp[2] + "|" + vTmp[3] + "|" + vTmp[4] + "|" + vTmp[5] + "|"
			+ vTmp[9] + "|" + vTmp[12] + "|" + vTmp[6] + "|" + vTmp[10];
		vOut.push_back(strTmp);
		fileout << strTmp << endl;
	}
	fileout.close();
	return true;
}