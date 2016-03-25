// GetDataFromWebNew.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include <regex>
#include <map>
#include <exception>
#include <math.h>
#include <process.h>
using namespace std;
const string strDataDir = "..\\data\\";
using namespace boost;
//string strInfo="s";
void SpliptString(const string& strFull, string strpatten, vector<string>& vec)
{
	size_t pos;
	string ss = "ss";
	string strtmp = strFull + strpatten;
	string str;
	for (int i = 0; i < strtmp.size();)
	{
		pos = strtmp.find(strpatten, i);
		if (pos != string::npos)
		{
			str = strtmp.substr(i, pos - i);
			if (pos != 0)
			{
				vec.push_back(str);
			}
			i = pos + strpatten.size();
		}
	}


}
void Erasetag(string& strFull, const string& strTag)
{
	while (strFull.find(strTag) != string::npos)
	{
		strFull.erase(strFull.find(strTag), strTag.size());
	}
	return;
}
bool httpGet(/*out*/string& result, const string& host, uint16_t port, const string& url, boost::asio::io_service &_io, string& str)
{
	char ch[256];
	sprintf_s(ch, "%d", port);
	string strPort(ch);
	try
	{
		string domain = host;
		boost::asio::ip::tcp::resolver resolver(_io);
		boost::asio::ip::tcp::resolver::query query(domain, strPort);
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;

		//Try each endpoint until we successfully establish a connection.  
		boost::asio::ip::tcp::socket socket(_io);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);

		//// Form the request. We specify the "Connection: close" header so that the  
		//// server will close the socket after transmitting the response. This will  
		//// allow us to treat all data up until the EOF as the content.  
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << url
			<< " HTTP/1.0\r\n";
		string ss = socket.remote_endpoint().address().to_string();
		request_stream << "Host: " << socket.remote_endpoint().address() << "\r\n";
		request_stream << "Accept: */*\r\n";
		//request_stream << "User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:19.0) Gecko/20100101 Firefox/19.0";
		request_stream << "Connection: close\r\n\r\n";

		// Send the request.  
		boost::asio::write(socket, request);

		// Read the response status line.  
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.  
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			str = "[HTTP]: Invalid response";
			return false;
		}
		if (status_code != 200)
		{
			sprintf_s(ch, "%d", status_code);
			str = string(ch);
			str = "[HTTP]: Response returned with status code " + str;
			return false;
		}

		// Read the response headers, which are terminated by a blank line.  
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.  
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
		{
		}

		// Read until EOF, writing data to output as we go.  
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
		{
			;
		}
		std::ostringstream packetStream;
		packetStream << &response;
		result = packetStream.str();
		//string temp;
		//while (!response_stream.eof()){
		//	std::getline(response_stream, temp);
		//	result.append(temp);
		//}

		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);

		return true;
	}
	catch (std::exception& e)
	{
		str = e.what();
		return false;
	}
}
bool httpGet2(/*out*/string& result, const string& host, uint16_t port, const string& url, boost::asio::io_service &_io, string& str)
{
	char ch[256];
	sprintf_s(ch, "%d", port);
	string strPort(ch);
	try
	{
		string domain = host;
		boost::asio::ip::tcp::resolver resolver(_io);
		boost::asio::ip::tcp::resolver::query query(domain, strPort);
		boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
		boost::asio::ip::tcp::resolver::iterator end;

		//Try each endpoint until we successfully establish a connection.  
		boost::asio::ip::tcp::socket socket(_io);
		boost::system::error_code error = boost::asio::error::host_not_found;
		while (error && endpoint_iterator != end)
		{
			socket.close();
			socket.connect(*endpoint_iterator++, error);
		}
		if (error)
			throw boost::system::system_error(error);

		//// Form the request. We specify the "Connection: close" header so that the  
		//// server will close the socket after transmitting the response. This will  
		//// allow us to treat all data up until the EOF as the content.  
		boost::asio::streambuf request;
		std::ostream request_stream(&request);
		request_stream << "GET " << url
			<< " HTTP/1.1\r\n";
		string ss = socket.remote_endpoint().address().to_string();
		request_stream << "Host:www.czce.com.cn" << "\r\n";
		request_stream << "Accept:text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\n";
		request_stream << "Accept-Encoding:gzip,deflate,sdch\r\n";
		request_stream << "Accept-Language:zh-CN,zh;q=0.8\r\n";
		request_stream << "User-Agent: Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/49.0.2623.75 Safari/537.36";
		request_stream << "Connection: keep-alive\r\n\r\n";

		// Send the request.  
		boost::asio::write(socket, request);

		// Read the response status line.  
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, "\r\n");

		// Check that response is OK.  
		std::istream response_stream(&response);
		std::string http_version;
		response_stream >> http_version;
		unsigned int status_code;
		response_stream >> status_code;
		std::string status_message;
		std::getline(response_stream, status_message);
		if (!response_stream || http_version.substr(0, 5) != "HTTP/")
		{
			str = "[HTTP]: Invalid response";
			return false;
		}
		if (status_code != 200)
		{
			sprintf_s(ch, "%d", status_code);
			str = string(ch);
			str = "[HTTP]: Response returned with status code " + str;
			return false;
		}

		// Read the response headers, which are terminated by a blank line.  
		boost::asio::read_until(socket, response, "\r\n\r\n");

		// Process the response headers.  
		std::string header;
		while (std::getline(response_stream, header) && header != "\r")
		{
		}

		// Read until EOF, writing data to output as we go.  
		while (boost::asio::read(socket, response,
			boost::asio::transfer_at_least(1), error))
		{
			;
		}
		std::ostringstream packetStream;
		packetStream << &response;
		result = packetStream.str();
		//string temp;
		//while (!response_stream.eof()){
		//	std::getline(response_stream, temp);
		//	result.append(temp);
		//}

		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);

		return true;
	}
	catch (std::exception& e)
	{
		str = e.what();
		return false;
	}
}
bool GetDataFromXZCE(const string& strDate, string& str)
{

	string strResult, strUrl, strhost;
	boost::asio::io_service _io;
	strhost = "www.czce.com.cn";
	string strYear = strDate.substr(0, 4);
	//"/portal/DFSStaticFiles/Future/2016/20160314/FutureDataDaily.txt"
	strUrl = "/portal/DFSStaticFiles/Future/" + strYear + "/" + strDate + "/FutureDataDaily.txt";
	int iPort = 80;

	//拿到数据
	if (!httpGet2(strResult, strhost, iPort, strUrl, _io, str))
	{
		return false;
	}
	if (strResult.empty())
	{
		str = "当天没有交易数据!";
		return false;
	}
	//解析写入
	vector<string> vec;
	SpliptString(strResult, "\r\n", vec);
	string strFilePath = strDataDir + "XZCE" + strDate + ".txt";
	ofstream fileout(strFilePath);
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		string strTmp = *it;
		if (strTmp.empty())
		{
			continue;
		}

		int i = strTmp[0];
		if (i<65 || i>122)
		{
			continue;
		}
		Erasetag(strTmp, ",");
		Erasetag(strTmp, " ");

		vector<string> vTmp;
		SpliptString(strTmp, "|", vTmp);
		//1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		strTmp = vTmp[0] + "|XZCE|" + strDate + "|" + vTmp[2] + "|" + vTmp[3] + "|" + vTmp[4] + "|" + vTmp[5] + "|"
			+ vTmp[9] + "|" + vTmp[12] + "|" + vTmp[6] + "|" + vTmp[10];
		fileout << strTmp << endl;
	}
	fileout.close();
	return true;
}
//string ToUTF8(const wchar_t* wideStr)
//{
//	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
//	std::wstring_convert<std::codecvt_>
//	return conv.to_bytes(wideStr);
//}
bool GetDataFromXSGE(const string& strDate, string& str)
{
	string strResult, strUrl, strhost;
	boost::asio::io_service _io;
	strhost = "www.shfe.com.cn";
	string strYear = strDate.substr(0, 4);
	//"data/dailydata/kx/kx20160316.dat"
	strUrl = "/data/dailydata/kx/kx" + strDate + ".dat";
	int iPort = 80;

	//拿到数据
	if (!httpGet(strResult, strhost, iPort, strUrl, _io, str))
	{
		return false;
	}
	if (strResult.empty())
	{
		str = "当天没有交易数据";
		return false;
	}
	Erasetag(strResult, " ");
	Erasetag(strResult, "\t");
	Erasetag(strResult, "\r\n");

	//解析写入
	const std::regex pattern1("^\\{\"o_curinstrument\":(.*)\\}$"); // ^\{"o_curinstrument":(.*)\}
	const std::regex pattern2("\\{.*?\\}"); // {.*?}
	string strTmp;
	smatch smtc;
	const std::sregex_token_iterator end;
	string strFilePath = strDataDir + "XSGE" + strDate + ".txt";
	ofstream fileout(strFilePath);
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
		SpliptString(strTmp, ",", vec);
		for (auto i = vec.begin(); i != vec.end(); ++i)
		{
			strTmp = *i;
			strTmp = strTmp.substr(strTmp.find(":") + 1, strTmp.size() - 1);
			Erasetag(strTmp, "\"");
			vec2.push_back(strTmp);
		}
		string sss = vec2[3];
		if (sss[0]<0 || vec2.size()<13)
		{
			continue;
		}

		//拼凑写入
		string strName;//合约代码
		strName = vec2[0];
		if (strName[strName.size() - 1] != 'f')
		{
			continue;
		}
		strName = strName.substr(0, strName.size() - 2);//去掉_f
		strName += vec2[3];
		//1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		fileout << strName << "|XSGE|" << strDate << "|" << vec2[5] << "|" << vec2[6] << "|" << vec2[7] << "|" << vec2[8] << "|"
			<< vec2[12] << "|0|" << vec2[9] << "|" << vec2[13] << endl;

	}
	fileout.close();
	return true;
}
bool GetDataFromCCFX(const string& strDate, string& str)
{
	string strResult, strUrl, strhost;
	boost::asio::io_service _io;
	strhost = "www.cffex.com.cn";
	string strYear = strDate.substr(0, 4);
	///fzjy/mrhq/201603/17/index.xml
	strUrl = "/fzjy/mrhq/" + strDate.substr(0, 6) + "/" + strDate.substr(6, 7) + "/index.xml";
	int iPort = 80;

	//拿到数据
	if (!httpGet(strResult, strhost, iPort, strUrl, _io, str))
	{
		return false;
	}
	if (strResult.empty())
	{
		str = "当天没有交易数据";
		return false;
	}
	Erasetag(strResult, " ");
	Erasetag(strResult, "\t");
	Erasetag(strResult, "\r");
	Erasetag(strResult, "\n");

	//解析写入
	const std::regex pattern1("<dailydata>(.*?)</dailydata>"); // ^\{"o_curinstrument":(.*)\}
	const std::regex pattern2("<.*?>(.+?)<.*?>"); // <.*?>(.+?)<.*?>
												  //const std::regex pattern3("<.*?>(.+?)<.*?>"); // {.*?}
	string strTmp, strLine;
	smatch smtc;
	const std::sregex_token_iterator end;
	string strFilePath = strDataDir + "CCFX" + strDate + ".txt";
	ofstream fileout(strFilePath);
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
		//写入1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		fileout << std::fixed << vec[0] << "|CCFX|" << strDate << "|" << vec[2] << "|" << vec[3] << "|" << vec[4] << "|" << vec[5] << "|" << vec[10]
			<< "|" << s << "|" << vec[8] << "|" << vec[6] << endl;
	}
	fileout.close();
	return true;
}
bool GetDataFromXDCE(const string& strDate, string& str)
{
	string strResult, strUrl, strhost;
	boost::asio::io_service _io;
	strhost = "www.dce.com.cn";
	string strYear = strDate.substr(0, 4);
	//"/PublicWeb/MainServlet?action=Pu00011_result&Pu00011_Input.trade_date=20160314&Pu00011_Input.variety=all&Pu00011_Input.trade_type=0"
	strUrl = "/PublicWeb/MainServlet?action=Pu00011_result&Pu00011_Input.trade_date=" + strDate + "&Pu00011_Input.variety=all&Pu00011_Input.trade_type=0";
	int iPort = 80;

	//拿到数据
	if (!httpGet(strResult, strhost, iPort, strUrl, _io, str))
	{
		return false;
	}
	if (strResult.empty())
	{
		str = "当天没有交易数据";
		return false;
	}
	//
	Erasetag(strResult, "\r\n");
	Erasetag(strResult, " ");
	Erasetag(strResult, "\t");
	Erasetag(strResult, ",");

	//添加映射
	map<string, string> mName;
	mName["豆一"] = "a"; mName["豆二"] = "b"; mName["豆粕"] = "m"; mName["豆油"] = "y"; mName["鸡蛋"] = "jd";
	mName["胶合板"] = "bb"; mName["焦煤"] = "jm"; mName["焦炭"] = "j"; mName["聚丙烯"] = "pp"; mName["聚氯乙烯"] = "v";
	mName["聚乙烯"] = "l"; mName["铁矿石"] = "i"; mName["纤维板"] = "fb"; mName["玉米"] = "c"; mName["玉米淀粉"] = "cs";
	mName["棕榈油"] = "p";

	//解析写入
	string strFilePath = strDataDir + "XDCE" + strDate + ".txt";
	ofstream fileout(strFilePath);
	const std::tr1::regex patternTR("<tr[\\S]*?</tr>");//<tr >[\s\S]*?</tr>
	const std::regex pattern2("<tdnowrap.*?</td>");//<td nowrap>.*?</td>
												   //const std::regex pattern3("<td nowrap class="td - right">.*?</td>");//<td nowrap class="td-right">.*?</td>
	const std::regex pattern3("<tdnowrap>(.*?)</td>");
	const std::regex pattern4("<tdnowrapclass=\"td-right\">(.*?)</td>");
	const std::sregex_token_iterator end;
	smatch tmpmatch;
	bool n = regex_search(strResult, tmpmatch, patternTR);
	//<tr*</tr>
	for (std::sregex_token_iterator i(strResult.begin(), strResult.end(), patternTR); i != end; ++i)
	{
		string strLine = *i;
		string strTmp;
		sregex_token_iterator j(strLine.begin(), strLine.end(), pattern2);
		if (j == end)
		{
			continue;
		}

		//添加每一项数据
		smatch tmpmatch;
		vector<string> vec;
		map<string, string>::iterator it;
		string strVrt;
		//品种
		strTmp = *j;
		if (!regex_match(strTmp, tmpmatch, pattern3))
		{
			continue;
		}
		strTmp = tmpmatch[1];
		if (mName.find(strTmp) == mName.end())
		{
			continue;
		}
		it = mName.find(strTmp);
		strVrt = it->second;
		j++;

		//交割日期
		strTmp = *j;
		if (!regex_match(strTmp, tmpmatch, pattern3))
		{
			continue;
		}
		strTmp = tmpmatch[1];
		//合约
		strTmp = strVrt + strTmp;
		vec.push_back(strTmp);
		++j;

		for (; j != end; ++j)
		{
			strTmp = *j;
			if (!regex_match(strTmp, tmpmatch, pattern4))
			{
				break;
			}
			strTmp = tmpmatch[1];
			if (strTmp == "-")
			{
				strTmp = "0";
			}
			vec.push_back(strTmp);
		}
		if (vec.size() <13)
		{
			continue;
		}

		//写入1合约代码|2交易所|3日期|4开盘|5最高价|6最低价|7收盘价|8成交量|9成交额|10结算价|11持仓量
		fileout << vec[0] << "|XDCE|" << strDate << "|" << vec[1] << "|" << vec[2] << "|" << vec[3] << "|" << vec[4] << "|" << vec[9] << "|"
			<< vec[12] << "|" << vec[6] << "|" << vec[10] << "|" << endl;
	}

	fileout.close();
	return true;
}
bool MergeFile(string strDate, string& str)
{
	ifstream filein;
	string strPath;
	string strLine;
	vector<string> vec;

	//ccfx
	strPath = strDataDir + "ccfx" + strDate + ".txt";
	filein.open(strPath);
	while (getline(filein, strLine))
	{
		if (strLine.empty())
		{
			continue;
		}
		vec.push_back(strLine);
	}
	filein.close();

	//xdce
	strPath = strDataDir + "xdce" + strDate + ".txt";
	filein.open(strPath);
	while (getline(filein, strLine))
	{
		if (strLine.empty())
		{
			continue;
		}
		vec.push_back(strLine);
	}
	filein.close();

	//xsge
	strPath = strDataDir + "xsge" + strDate + ".txt";
	filein.open(strPath);
	while (getline(filein, strLine))
	{
		if (strLine.empty())
		{
			continue;
		}
		vec.push_back(strLine);
	}
	filein.close();

	//xzce
	strPath = strDataDir + "xzce" + strDate + ".txt";
	filein.open(strPath);
	while (getline(filein, strLine))
	{
		if (strLine.empty())
		{
			continue;
		}
		vec.push_back(strLine);
	}
	filein.close();

	//hebing
	ofstream fileout;
	strPath = "..\\merge\\merge" + strDate + ".txt";
	fileout.open(strPath);
	for (auto it = vec.begin(); it != vec.end(); ++it)
	{
		fileout << *it << endl;
	}
	fileout.close();
	return true;

}
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		string strDate;
		string str;
		std::regex patten("^(19|20)\\d{2}((0[1-9])|(1[0-2]))((0[1-9])|((1|2)[0-9])|30|31)$");

		//char ch = ' ';
		//while (ch != 'q')
		{
			cout << "请输入日期，例如20160314：";
			cin >> strDate;
			while (!std::regex_match(strDate.begin(), strDate.end(), patten))
			{
				cout << "日期格式错误!" << endl;
				cout << "请输入日期：";
				cin >> strDate;
			}
			if (!GetDataFromCCFX(strDate, str))
			{
				cout << "[ccfx_err]" << str << endl;
				return -1;
			}
			if (!GetDataFromXDCE(strDate, str))
			{
				cout << "[xdce_err]" << str << endl;
				return -2;
			}
			if (!GetDataFromXSGE(strDate, str))
			{
				cout << "[xsge_err]" << str << endl;
				return -3;
			}
			if (!GetDataFromXZCE(strDate, str))
			{
				cout << "[xzce_err]" << str << endl;
				return -4;
			}

			//合并
			MergeFile(strDate, str);
			cout << strDate << "数据下载成功！" << endl;

		}
		::system("pause");
		return 0;

	}
	catch (std::exception& e)
	{
		cerr << e.what() << endl;
	}

}