#include "stdafx.h"
#include "XDCE.h"
using namespace std;

CXDCE::CXDCE(const std::string& strDate) :CExchangeBase(strDate)
{
	m_strECchangeName = "XDCE";
	m_strPath = "..\\data\\XDCE" + strDate + ".txt";
	m_strHost= "www.dce.com.cn";
	m_strUrl= "/PublicWeb/MainServlet?action=Pu00011_result&Pu00011_Input.trade_date=" + m_strDate + "&Pu00011_Input.variety=all&Pu00011_Input.trade_type=0";
	m_strPort = "80";
}


CXDCE::~CXDCE()
{
}

bool CXDCE::GetDataFromWeb(std::vector<std::string>& vOut)
{
	//下载数据
	string strResult;
	if (!httpGet(strResult))
	{
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = "当天没有交易数据";
		return false;
	}
	//
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\n')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\r')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ' ')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\t')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ',')), strResult.end());

	//添加映射
	map<string, string> mName;
	mName["豆一"] = "a"; mName["豆二"] = "b"; mName["豆粕"] = "m"; mName["豆油"] = "y"; mName["鸡蛋"] = "jd";
	mName["胶合板"] = "bb"; mName["焦煤"] = "jm"; mName["焦炭"] = "j"; mName["聚丙烯"] = "pp"; mName["聚氯乙烯"] = "v";
	mName["聚乙烯"] = "l"; mName["铁矿石"] = "i"; mName["纤维板"] = "fb"; mName["玉米"] = "c"; mName["玉米淀粉"] = "cs";
	mName["棕榈油"] = "p";

	//解析写入
	ofstream fileout(m_strPath);
	if (!fileout.is_open())
	{
		throw runtime_error(m_strECchangeName + "打开文件失败");
	}
	const std::tr1::regex patternTR("<tr[\\S]*?</tr>");//<tr >[\s\S]*?</tr>
	const std::regex pattern2("<tdnowrap.*?</td>");//<td nowrap>.*?</td>
	//const std::regex pattern3("<td nowrap class="td - right">.*?</td>");//<td nowrap class="td-right">.*?</td>
	const std::regex pattern3("<tdnowrap>(.*?)</td>");
	const std::regex pattern4("<tdnowrapclass=\"td-right\">(.*?)</td>");
	const std::sregex_token_iterator end;
	smatch tmpmatch;
	bool n = regex_search(strResult, tmpmatch, patternTR);
	if ( !n)
	{
		m_strErrInfo = "数据格式出错！";
		return false;
	}
	//<tr*</tr>
	for (std::sregex_token_iterator i(strResult.begin(), strResult.end(), patternTR); i != end; ++i)
	{
		string strLine = *i;
		string strTmp;
		sregex_token_iterator j(strLine.begin(), strLine.end(), pattern2);
		if (j == end)
		{
			continue;
		}

		//添加每一项数据
		smatch tmpmatch;
		vector<string> vec;
		map<string, string>::iterator it;
		string strVrt;
		//品种
		strTmp = *j;
		if (!regex_match(strTmp, tmpmatch, pattern3))
		{
			continue;
		}
		strTmp = tmpmatch[1];
		if (mName.find(strTmp) == mName.end())
		{
			continue;
		}
		it = mName.find(strTmp);
		strVrt = it->second;
		j++;

		//交割日期
		strTmp = *j;
		if (!regex_match(strTmp, tmpmatch, pattern3))
		{
			continue;
		}
		strTmp = tmpmatch[1];
		//合约
		strTmp = strVrt + strTmp;
		vec.push_back(strTmp);
		++j;

		for (; j != end; ++j)
		{
			strTmp = *j;
			if (!regex_match(strTmp, tmpmatch, pattern4))
			{
				break;
			}
			strTmp = tmpmatch[1];
			if (strTmp == "-")
			{
				strTmp = "0";
			}
			vec.push_back(strTmp);
		}
		if (vec.size() < 13)
		{
			continue;
		}

		//写入1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		strLine = vec[0] + "|XDCE|" + m_strDate + "|" + vec[1] + "|" + vec[2] + "|" + vec[3] + "|" + vec[4] + "|" + vec[9] + "|"
			+ vec[12] + "|" + vec[6] + "|" + vec[10] + "|" ;
		fileout <<strLine << endl;
		vOut.push_back(strLine);
	}

	fileout.close();
	return true;
}