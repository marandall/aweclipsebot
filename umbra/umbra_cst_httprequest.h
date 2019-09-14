#pragma once
#include "umbra_cst_winsocket.h"
#include <map>
#include <string>

namespace Umbra
{
	namespace Cst
	{
		class HttpRequest
		{
		public: // socket and configuration
			std::string	user_agent;
			std::string cookie;
			std::string	header_text;
			std::string body;

		protected: // status
			int					_status;
			std::string _status_text;

		public: // headers map
			std::map<std::string, std::string> headers;

			HttpRequest()
			{
				user_agent = "UmbraScript 1.01";
			}

		public: // string handler
			static UrlParseInfo ParseUrl(std::string Host)
			{
				UrlParseInfo url;
				url.source = Host;

				// if begins with http
				if (Host.substr(0, 7).compare("http://") == 0)
				{
					url.protocol = "http";
					Host = Host.substr(7);
				}

				// search until an : or open port
				std::string::size_type slash = Host.find("/");
				std::string::size_type colon = Host.find(":");

				// strip the host
				if (colon != std::string::npos)
				{
					// find the port
					std::string port = Host.substr(colon + 1, slash - colon);
					url.port = atoi(port.c_str());
					url.host = Host.substr(0, colon);
				}
				else
				{
					url.host = Host.substr(0, slash);
				}

				// check whats after
				if (slash != std::string::npos)
					url.path = Host.substr(slash);
				
				// check the path
				return url;
			}

		public:
			// getStatus
			//  retrieves the returned status code
			//

			int getStatus()
			{
				return _status;
			}

			// getHeader
			//  get a specific header from the archive
			//

			std::string getHeader(std::string Key)
			{
				std::map<std::string, std::string>::iterator itr = headers.find(Key);
				if (itr == headers.end())
					return itr->second;
				else
					return "";
			}

			// get
			//  opens a socket connection and requests the file
			//
			int get(std::string Path)
			{
				// create the header
				UrlParseInfo url = HttpRequest::ParseUrl(Path);
				body = "";
				header_text = "";
				headers.clear();

				// get request
				std::string header = "GET ";
				header.append(url.path).append(" HTTP/1.1\r\n")
							.append("host: ").append(url.host).append("\r\n")
							.append("user-agent: ").append(user_agent).append("\r\n")
							.append("connection: close\r\n")
							.append("\r\n");

				// open socket to server
				WinSocket sock;
				int err = sock.connect(url.host, url.port);
				if (err != 0)
					return err;

				// send headers
				err = sock.send(header, 0);
				if (err != 0)
					return err;

				// close send
				sock.shutdownSend();

				// receive everything
				std::string data = sock.recvUntilClose();
				std::string::size_type bpos = data.find("\r\n\r\n");
				if (bpos != std::string::npos)
				{
					header_text = data.substr(0, bpos);
					body = data.substr(bpos + 4);
				}

				// parse the headers
				std::string::size_type pos = 0;
				while (pos < header_text.size())
				{
					std::string::size_type nlpos = header_text.find("\r\n", pos);
					std::string line = header_text.substr(pos, nlpos - pos).c_str();

					// is this line 0?
					if (pos == 0)
					{
						std::string::size_type fs = line.find(" ");
						if (fs != std::string::npos)
						{
							std::string::size_type ls = line.find(" ", fs + 1);
							if (ls != std::string::npos)
							{
								_status = atoi(line.substr(fs + 1, ls - fs - 1).c_str());
								_status_text = line.substr(ls + 1);
								int k = 0;
							}
							else
							{
								_status = atoi(line.substr(fs + 1).c_str());
							}
						}
					}
					else
					{
						// lookup split
						std::string::size_type np = line.find(": ");
						if (np != std::string::npos)
							headers[line.substr(0, np)] = line.substr(np + 2);
					}

					if (nlpos == std::string::npos)
						break;
					pos = nlpos + 2;
				}

				return getStatus() == 200 ? 0 : getStatus();
			}
		};

	}
}