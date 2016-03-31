#include "stdafx.h"
#include "CCFX.h"
using namespace std;

CCFX::CCFX(const std::string& strDate) :CExchangeBase(strDate)
{
	m_strECchangeName = _T("CCFX");
	m_strPath = _T("..\\data\\CCFX" + strDate + _T(".txt"));
	m_strHost = "www.cffex.com.cn";
	m_strUrl = "/fzjy/mrhq/" + m_strDate.substr(0, 6) + "/" + m_strDate.substr(6, 7) + "/index.xml";
	m_strPort = "80";
}


CCFX::~CCFX()
{
}

bool CCFX::GetDataFromWeb(std::vector<std::string>& vOut)
{
	//�õ�����
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
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ' ')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\t')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\r')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), '\n')), strResult.end());

	//����д��
	const std::regex pattern1("<dailydata>(.*?)</dailydata>"); // ^\{"o_curinstrument":(.*)\}
	const std::regex pattern2("<.*?>(.+?)<.*?>"); // <.*?>(.+?)<.*?>
												  //const std::regex pattern3("<.*?>(.+?)<.*?>"); // {.*?}
	string strTmp, strLine;
	smatch smtc;
	const std::sregex_token_iterator end;
	ofstream fileout(m_strPath);
	if (!fileout.is_open())
	{
		throw runtime_error(m_strECchangeName + "���ļ�ʧ��");
	}
	if (!regex_search( strResult , smtc , pattern1))
	{
		m_strErrInfo = "���ݸ�ʽ����";
		return false;
	}
	for (std::sregex_token_iterator i(strResult.begin(), strResult.end(), pattern1); i != end; ++i)
	{
		strTmp = *i;
		regex_match(strTmp, smtc, pattern1);
		strTmp = smtc[1];
		vector<string> vec;
		for (std::sregex_token_iterator j(strTmp.begin(), strTmp.end(), pattern2); j != end; ++j)
		{
			strLine = *j;
			regex_match(strLine, smtc, pattern2);
			strLine = smtc[1];
			vec.push_back(strLine);
		}
		double s = atof(vec[11].c_str());
		//д��1��Լ����|2������|3����|4����|5��߼�|6��ͼ�|7���̼�|8�ɽ���|9�ɽ���|10�����|11�ֲ���
		strLine = vec[0] + "|CCFX|" + m_strDate + "|" + vec[2] + "|" + vec[3] + "|" + vec[4] + "|" + vec[5] + "|" + vec[10]
			+ "|" + to_string(s) + "|" + vec[8] + "|" + vec[6];
		vOut.push_back(strLine);
		fileout << std::fixed <<strLine << endl;
	}
	fileout.close();
	return true;
}