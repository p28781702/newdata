#include "stdafx.h"
#include "XSGE.h"
using namespace std;

CXSGE::CXSGE(std::string strDate):CExchangeBase(strDate)
{
	m_strECchangeName = _T("XSGE");
	m_strPath = _T("..\\data\\XSGE" + strDate + _T(".txt"));

}


CXSGE::~CXSGE()
{
}

bool CXSGE::GetDataFromWeb(std::vector<std::string>& vOut)
{
	string strResult, strUrl, strhost,strErr;
	boost::asio::io_service _io;
	strhost = _T("www.shfe.com.cn");
	string strYear = m_strDate.substr(0, 4);
	//"data/dailydata/kx/kx20160316.dat"
	strUrl = _T("/data/dailydata/kx/kx") + m_strDate + _T(".dat");

	//�õ�����
	if (!CCommonFuc::httpGet(strResult, strhost,strUrl, _io, strErr))
	{
		m_strErrInfo = strErr;
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = _T("����û�н�������");
		return false;
	}
	CCommonFuc::Erasetag(strResult, _T(" "));
	CCommonFuc::Erasetag(strResult, _T("\t"));
	CCommonFuc::Erasetag(strResult, _T("\r\n"));

	//����д��
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
		return false;
	}
	strResult = smtc[1];
	for (std::sregex_token_iterator i(strResult.begin(), strResult.end(), pattern2); i != end; ++i)
	{
		strTmp = *i;
		vector<string> vec;
		vector<string> vec2;
		CCommonFuc::SpliptString(strTmp, _T(","), vec);
		for (auto i = vec.begin(); i != vec.end(); ++i)
		{
			strTmp = *i;
			strTmp = strTmp.substr(strTmp.find(":") + 1, strTmp.size() - 1);
			CCommonFuc::Erasetag(strTmp, _T("\""));
			vec2.push_back(strTmp);
		}
		string sss = vec2[3];
		if (sss[0] < 0 || vec2.size() < 13)
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
		strLine = strName + _T("|XSGE|") + m_strDate + _T("|") + vec2[5] + _T("|") + vec2[6] + _T("|") + vec2[7] + _T("|") + vec2[8] + _T("|")
			+ vec2[12] + _T("|0|") + vec2[9] + _T("|") + vec2[13];
		vOut.push_back(strLine);
		fileout << strLine << endl;

	}
	fileout.close();
	return true;
}