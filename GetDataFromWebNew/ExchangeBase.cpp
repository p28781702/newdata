#include "stdafx.h"
#include "ExchangeBase.h"
#include "..\\..\\kCommon\\include\\Log.h"
using namespace std;

CExchangeBase::CExchangeBase()
{
}
CExchangeBase::CExchangeBase(const std::string& strDate):m_strDate(strDate)
{
	m_strPath = "..\\data\\" + strDate + ".txt";
	
}

CExchangeBase::~CExchangeBase()
{
}

std::string CExchangeBase::GetErr() const
{
	return m_strErrInfo;
}

void CExchangeBase::SpliptString(const std::string& strFull, std::string strpatten, std::vector<std::string>& vec)
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

bool CExchangeBase::CheckVal(std::vector<string>& vec)
{
	string strOpen, strClose, strTotal, strHigh, strLow;
	bool bTag = true;
	for (auto it = vec.begin(); it != vec.end();)
	{
		vector<string> vtmp;
		SpliptString(*it, "|", vtmp);
		//1合约代码 | 2交易所 | 3日期 | 4开盘 | 5最高价 | 6最低价 | 7收盘价 | 8成交量 | 9成交额 | 10结算价 | 11持仓量
		strOpen = vtmp[3]; strClose = vtmp[6]; strTotal = vtmp[7]; strHigh = vtmp[4]; strLow = vtmp[5];
		int iHigh = atoi(strHigh.c_str()); int iLow = atoi(strLow.c_str());
		if ( iLow > iHigh		//最高价大于最低价
		   ||( (strTotal != "0") && (strOpen=="0"||strClose=="0"||strHigh=="0"||strLow=="0")) //交易量不为0，且没有开收盘价格
		   || vtmp[0] == ""			//没有合约代码
			)
		{
			LOG_FATAL( *it + "数据有严重错误");
			it = vec.erase(it);
			bTag = false;
			continue;
		}
		++it;
		

	}
	return bTag;
}

bool CExchangeBase::httpGet(string& strOut)
{
	try
	{
		boost::asio::io_service _io;
		string domain = m_strHost;
		boost::asio::ip::tcp::resolver resolver(_io);
		boost::asio::ip::tcp::resolver::query query(domain, m_strPort);
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
		request_stream << "GET " << m_strUrl
			<< " HTTP/1.0\r\n";
		string ss = socket.remote_endpoint().address().to_string();
		//request_stream << "Host: " << socket.remote_endpoint().address() << "\r\n";
		request_stream << "Host: " << m_strHost.c_str() << "\r\n";
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
			m_strErrInfo = "[HTTP]: Invalid response";
			return false;
		}
		if (status_code != 200)
		{
			m_strErrInfo = "[HTTP]: Response returned with status code " + to_string(status_code);;
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
		strOut = packetStream.str();

		if (error != boost::asio::error::eof)
			throw boost::system::system_error(error);

		return true;
	}
	catch (std::exception& e)
	{
		m_strErrInfo = e.what();
		return false;
	}
}