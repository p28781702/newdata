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

	//�õ�����
	string strResult;
	if (!httpGet(strResult))
	{
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = _T("����û�н�������");
		return false;
	}
	strResult.erase(remove_if( strResult.begin(),strResult.end(),bind2nd( equal_to<char>(),' ')),strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\t')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\r\n')), strResult.end());


	/////////////////////////////////////////////////////����д��
	const std::regex pattern1("^\\{\"o_curinstrument\":(.*)\\}$"); // ^\{"o_curinstrument":(.*)\}
	const std::regex pattern2("\\{.*?\\}"); // {.*?}
	string strTmp;
	smatch smtc;
	const std::sregex_token_iterator end;
	ofstream fileout(m_strPath);
	if (!fileout.is_open())
	{
		throw runtime_error(m_strECchangeName + "���ļ�ʧ��");
	}
	if (!regex_match(strResult, smtc, pattern1))
	{
		m_strErrInfo = "���ݸ�ʽ����";
		return false;
	}
	strResult = smtc[1];
	if ( !regex_search( strResult ,smtc,pattern2))
	{
		m_strErrInfo = "���ݸ�ʽ����";
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

		//ƴ��д��
		string strName;//��Լ����
		strName = vec2[0];
		if (strName[strName.size() - 1] != 'f')
		{
			continue;
		}
		strName = strName.substr(0, strName.size() - 2);//ȥ��_f
		strName += vec2[3];
		//1��Լ����|2������|3����|4����|5��߼�|6��ͼ�|7���̼�|8�ɽ���|9�ɽ���|10�����|11�ֲ���
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