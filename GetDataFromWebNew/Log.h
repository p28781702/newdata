#pragma once

#include <iostream>

#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/configurator.h"

class CLog
{
public:
	static CLog& GetLog()
	{
		static CLog g_log;
		return g_log;
	}

	CLog(void)
		:m_isInit(false)
	{
		try
		{
			log4cplus::initialize();
			log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties"));
			m_root = log4cplus::Logger::getRoot();
			m_isInit = true;
		}
		catch (std::exception& e)
		{
			std::cerr << e.what();
			m_isInit = false;
		}
		catch (...)
		{
			std::cerr << "CLog unknown exception";
			m_isInit = false;
		}
	}
	~CLog(void){};
	bool isInit(){ return m_isInit; };
	log4cplus::Logger& getLogger(){ return m_root; };
private:
	bool m_isInit;
	log4cplus::Logger m_root;
};




#define LOG_FATAL(msg)  {if(CLog::GetLog().isInit()) LOG4CPLUS_FATAL(CLog::GetLog().getLogger(), msg);else std::cerr<<msg<<std::endl;}
#define LOG_ERROR(msg)  {if(CLog::GetLog().isInit()) LOG4CPLUS_ERROR(CLog::GetLog().getLogger(), msg);else std::cerr<<msg<<std::endl;}           
#define LOG_WARN(msg)   {if(CLog::GetLog().isInit()) LOG4CPLUS_WARN(CLog::GetLog().getLogger(),msg);  else std::cerr<<msg<<std::endl;}           
#define LOG_INFO(msg)   LOG4CPLUS_INFO(CLog::GetLog().getLogger(), msg)
#define LOG_DEBUG(msg)  LOG4CPLUS_DEBUG(CLog::GetLog().getLogger(), msg)
