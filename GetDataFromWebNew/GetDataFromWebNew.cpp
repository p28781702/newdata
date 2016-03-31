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
			///////////////////////////////////////////////////输入日期//////////////////////////////////////////////
			cout << "请输入日期，例如20160314：";
			cin >> strDate;
			while (!std::regex_match(strDate.begin(), strDate.end(), patten))
			{
				cout << "日期格式错误!" << endl;
				cout << "请输入日期：";
				cin >> strDate;
			}
			cout << "正在下载，请等待.." << endl;


			/////////////////////////////////////////////////下载数据///////////////////////////////////////////////
			unique_ptr<CExchangeBase> pCCFX(new CCFX(strDate));
			unique_ptr<CExchangeBase> pXSGE(new CXSGE(strDate));
			unique_ptr<CExchangeBase> pXDCE(new CXDCE(strDate));
			unique_ptr<CExchangeBase> pXZCE(new CXZCE(strDate));
			vector<string> vec;
			if (!pCCFX->GetDataFromWeb(vec))
			{
				LOG_ERROR("[ccfx_err]" + pCCFX->GetErr());
				continue;;
			}
			if (!pXDCE->GetDataFromWeb(vec))
			{
				LOG_ERROR("[ccfx_err]" + pXDCE->GetErr());
				continue;;
			}
			if (!pXSGE->GetDataFromWeb(vec))
			{
				LOG_ERROR("[ccfx_err]" + pXSGE->GetErr());
				continue;;
			}
			if (!pXZCE->GetDataFromWeb(vec))
			{
				LOG_ERROR("[ccfx_err]" + pXZCE->GetErr());
				continue;;
			}


			//////////////////////////////////////////////数据检查//////////////////////////////////////////////////
			if ( CExchangeBase::CheckVal(vec) == 0)
			{
				LOG_FATAL("部分数据严重哦错误，注意查看");
			}

			//////////////////////////////////////////////数据合并写入/////////////////////////////////////////////
			ofstream fileout("..\\merge\\merge"+strDate+".txt");
			for ( auto it:vec)
			{
				fileout << it << endl;
			}
			fileout.close();
			LOG_INFO(strDate+" 数据下载成功");
		}
		return 0;

	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		::system("pause");
	}

}