#include "stdafx.h"
#include "Log.h"
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <sstream>
using namespace std;
CLog* CLog::m_instance = nullptr;
CLog::CLog()
{
	fileout.open("..\\mylog.txt",ios::app);
	if ( !fileout)
	{
		throw std::runtime_error(_T("打开日志文件失败"));
	}
	string strTime = GetNowTime();
	fileout << strTime << endl;
}


CLog::~CLog()
{
}


CLog *CLog::GetInstance()
{
	//没有考虑线程安全
	if ( m_instance ==nullptr )
	{
		m_instance = new CLog;
	}
	return m_instance;
	
}

void CLog::WriteLog(const string& strLog)
{
	fileout << strLog.c_str()<<endl;
}

//CLog& CLog::operator<<(std::string&& strErr)
//{
//	fileout << strErr.c_str();
//	return *this;
//}

CLog& CLog::operator<<(const std::string& strErr)
{
	fileout.write(strErr.c_str(), strErr.size());
	fileout.flush();
	return *this;
}

string CLog::GetNowTime()
{
	time_t  t;
	char  buf[128];
	memset(buf, 0, sizeof(buf));
	struct tm tmp = {0};
	t = time(NULL);
	localtime_s(&tmp,&t);
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tmp);
	stringstream str;
	str << buf;
	return str.str();
}