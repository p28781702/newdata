#include "stdafx.h"
#include "CommonFuc.h"
using namespace std;

CCommonFuc::CCommonFuc()
{
}


CCommonFuc::~CCommonFuc()
{
}


void CCommonFuc::Erasetag(std::string& strFull, const std::string& strTag)
{
	while (strFull.find(strTag) != string::npos)
	{
		strFull.erase(strFull.find(strTag), strTag.size());
	}
	return;
}

void CCommonFuc::SpliptString(const std::string& strFull, std::string strpatten, std::vector<std::string>& vec)
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

bool CCommonFuc::httpGet(std::string& result, const std::string& host, const std::string& url, boost::asio::io_service &_io, std::string& strErrInfo, std::string strPort/* =_T("80") */)
{
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
		request_stream << _T("GET ") << url
			<< _T(" HTTP/1.0\r\n");
		string ss = socket.remote_endpoint().address().to_string();
		//request_stream << "Host: " << socket.remote_endpoint().address() << "\r\n";
		request_stream << _T("Host: ") << host.c_str() << _T("\r\n");
		request_stream << _T("Accept: */*\r\n");
		//request_stream << "User-Agent: Mozilla/5.0 (Windows NT 6.1; rv:19.0) Gecko/20100101 Firefox/19.0";
		request_stream << _T("Connection: close\r\n\r\n");

		// Send the request.  
		boost::asio::write(socket, request);

		// Read the response status line.  
		boost::asio::streambuf response;
		boost::asio::read_until(socket, response, _T("\r\n"));

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
			strErrInfo = _T("[HTTP]: Invalid response");
			return false;
		}
		if (status_code != 200)
		{
			strErrInfo = _T("[HTTP]: Response returned with status code ") + to_string(status_code);;
			return false;
		}

		// Read the response headers, which are terminated by a blank line.  
		boost::asio::read_until(socket, response, _T("\r\n\r\n"));

		// Process the response headers.  
		std::string header;
		while (std::getline(response_stream, header) && header != _T("\r"))
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
		strErrInfo = e.what();
		return false;
	}
}