// GetDataFromWebNew.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ExchangeBase.h"
#include "CCFX.h"
#include "XDCE.h"
#include "XSGE.h"
#include "XZCE.h"
#include <iostream>
#include <memory>
#include "Log.h"
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		string strDate;
		string str;
		std::regex patten("^(19|20)\\d{2}((0[1-9])|(1[0-2]))((0[1-9])|((1|2)[0-9])|30|31)$");

		//char ch = ' ';
		while (true)
		{
			cout << "请输入日期，例如20160314：";
			cin >> strDate;
			cout << "正在下载，请等待.."<<endl;
			while (!std::regex_match(strDate.begin(), strDate.end(), patten))
			{
				cout << "日期格式错误!" << endl;
				cout << "请输入日期：";
				cin >> strDate;
			}


			unique_ptr<CExchangeBase> pCCFX(new CCFX(strDate));
			unique_ptr<CExchangeBase> pXSGE(new CXSGE(strDate));
			unique_ptr<CExchangeBase> pXDCE(new CXDCE(strDate));
			unique_ptr<CExchangeBase> pXZCE(new CXZCE(strDate));
			vector<string> vec;
			CLog* plog = CLog::GetInstance();

			if (!pCCFX->GetDataFromWeb(vec))
			{
				cout << "[ccfx_err]" << pCCFX->m_strErrInfo << endl;
				*plog << "[ccfx_err]" << pCCFX->m_strErrInfo << "\r\n";
				continue;;
			}
			if (!pXDCE->GetDataFromWeb(vec))
			{
				cout << "[xdce_err]" << pXSGE->m_strErrInfo << endl;
				*plog << "[xdce_err]" << pXSGE->m_strErrInfo << "\r\n";
				continue;;
			}
			if (!pXSGE->GetDataFromWeb(vec))
			{
				cout << "[xsge_err]" << pXDCE->m_strErrInfo << endl;
				*plog << "[xsge_err]" << pXSGE->m_strErrInfo << "\r\n";
				continue;;
			}
			if (!pXZCE->GetDataFromWeb(vec))
			{
				cout << "[xzce_err]" << pXZCE->m_strErrInfo << endl;
				*plog << "[xszce_err]" << pXSGE->m_strErrInfo << "\r\n";
				continue;;
			}

			//合并
			ofstream fileout("..\\merge\\merge"+strDate+".txt");
			for ( auto it:vec)
			{
				fileout << it << endl;
			}
			fileout.close();
			cout << strDate << "数据下载成功！" << endl;
			*plog<< strDate<<"数据下载成功\r\n";

		}
		return 0;

	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		::system("pause");
	}

}