#pragma once
#include <fstream>
class CLog
{
public:
	static CLog* GetInstance();
	//CLog& operator<<(std::string&& strErr);
	CLog& operator<<(const std::string& strErr);
	void WriteLog(const std::string& strLog);
	std::string GetNowTime();
private:
	CLog();
	~CLog();
	static CLog* m_instance;
	class CDistroyinstance
	{
	public:
		~CDistroyinstance()
		{
			if ( m_instance != nullptr)
			{
				m_instance->fileout.close();
				delete m_instance;
				m_instance = nullptr;
			}
		}
	};
	static CDistroyinstance dis;
	std::ofstream fileout;

};

