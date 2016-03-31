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
	//��������
	string strResult;
	if (!httpGet(strResult))
	{
		return false;
	}
	if (strResult.empty())
	{
		m_strErrInfo = "����û�н�������!";
		return false;
	}
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ',')), strResult.end());
	strResult.erase(remove_if(strResult.begin(), strResult.end(), bind2nd(equal_to<char>(), ' ')), strResult.end());

	//����д��
	vector<string> vec;
	SpliptString(strResult, _T("\r\n"), vec);
	string strFilePath =m_strPath;
	ofstream fileout(strFilePath);
	if ( !fileout.is_open() )
	{
		throw runtime_error( m_strECchangeName + "���ļ�ʧ��");
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
		//1��Լ����|2������|3����|4����|5��߼�|6��ͼ�|7���̼�|8�ɽ���|9�ɽ���|10�����|11�ֲ���
		strTmp = vTmp[0] + "|XZCE|" + m_strDate + "|" + vTmp[2] + "|" + vTmp[3] + "|" + vTmp[4] + "|" + vTmp[5] + "|"
			+ vTmp[9] + "|" + vTmp[12] + "|" + vTmp[6] + "|" + vTmp[10];
		vOut.push_back(strTmp);
		fileout << strTmp << endl;
	}
	fileout.close();
	return true;
}