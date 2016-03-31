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
	//��������
	string strResult;
	if (!httpGet(strResult))
	{
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = "����û�н�������";
		return false;
	}
	//
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\n')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\r')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ' ')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\t')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ',')), strResult.end());

	//���ӳ��
	map<string, string> mName;
	mName["��һ"] = "a"; mName["����"] = "b"; mName["����"] = "m"; mName["����"] = "y"; mName["����"] = "jd";
	mName["���ϰ�"] = "bb"; mName["��ú"] = "jm"; mName["��̿"] = "j"; mName["�۱�ϩ"] = "pp"; mName["������ϩ"] = "v";
	mName["����ϩ"] = "l"; mName["����ʯ"] = "i"; mName["��ά��"] = "fb"; mName["����"] = "c"; mName["���׵��"] = "cs";
	mName["�����"] = "p";

	//����д��
	ofstream fileout(m_strPath);
	if (!fileout.is_open())
	{
		throw runtime_error(m_strECchangeName + "���ļ�ʧ��");
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
		m_strErrInfo = "���ݸ�ʽ����";
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

		//���ÿһ������
		smatch tmpmatch;
		vector<string> vec;
		map<string, string>::iterator it;
		string strVrt;
		//Ʒ��
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

		//��������
		strTmp = *j;
		if (!regex_match(strTmp, tmpmatch, pattern3))
		{
			continue;
		}
		strTmp = tmpmatch[1];
		//��Լ
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

		//д��1��Լ����|2������|3����|4����|5��߼�|6��ͼ�|7���̼�|8�ɽ���|9�ɽ���|10�����|11�ֲ���
		strLine = vec[0] + "|XDCE|" + m_strDate + "|" + vec[1] + "|" + vec[2] + "|" + vec[3] + "|" + vec[4] + "|" + vec[9] + "|"
			+ vec[12] + "|" + vec[6] + "|" + vec[10] + "|" ;
		fileout <<strLine << endl;
		vOut.push_back(strLine);
	}

	fileout.close();
	return true;
}