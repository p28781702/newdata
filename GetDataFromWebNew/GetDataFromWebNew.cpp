// GetDataFromWebNew.cpp : �������̨Ӧ�ó������ڵ㡣
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
			///////////////////////////////////////////////////��������//////////////////////////////////////////////
			cout << "���������ڣ�����20160314��";
			cin >> strDate;
			while (!std::regex_match(strDate.begin(), strDate.end(), patten))
			{
				cout << "���ڸ�ʽ����!" << endl;
				cout << "���������ڣ�";
				cin >> strDate;
			}
			cout << "�������أ���ȴ�.." << endl;


			/////////////////////////////////////////////////��������///////////////////////////////////////////////
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


			//////////////////////////////////////////////���ݼ��//////////////////////////////////////////////////
			if ( CExchangeBase::CheckVal(vec) == 0)
			{
				LOG_FATAL("������������Ŷ����ע��鿴");
			}

			//////////////////////////////////////////////���ݺϲ�д��/////////////////////////////////////////////
			ofstream fileout("..\\merge\\merge"+strDate+".txt");
			for ( auto it:vec)
			{
				fileout << it << endl;
			}
			fileout.close();
			LOG_INFO(strDate+" �������سɹ�");
		}
		return 0;

	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
		::system("pause");
	}

}