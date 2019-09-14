#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

namespace Umbra
{
	namespace Cst
	{
		class WinSocket
		{
		private: // socket data 
			struct SocketStatus
			{
				bool initialized;

				SocketStatus() {
					initialized = false;
				}

				~SocketStatus() {
					WSACleanup();
				}
			};

			static WSAData& getWsData() { static WSAData wsd; return wsd; }
			static SocketStatus& GetStatus() { static SocketStatus ss; return ss; }

		protected: // connection data
			SOCKET _socket;

		public:
			// init
			//  initialises winsock2 static method use first
			//
			static int Init()
			{
				if (GetStatus().initialized == false)
				{
					GetStatus().initialized = true;
					int res = WSAStartup(MAKEWORD(2,2), &getWsData());
					return res;
				}
				else
				{
					return 1;
				}
			}

			static int Shutdown()
			{
				if (GetStatus().initialized == true)
				{
					GetStatus().initialized = false;
					WSACleanup();
				}

			}

			// constructor
			//  sets up the basics
			//

			WinSocket()
			{
				Init();
			}

			// connect
			//  connects to a remote socket
			//
			int connect(std::string Host, short Port)
			{
				addrinfo *result = 0, *ptr = 0, hints;
				
				// configure hints
				memset(&hints, 0, sizeof(hints));
				hints.ai_family = AF_UNSPEC;
				hints.ai_socktype = SOCK_STREAM;
				hints.ai_protocol = IPPROTO_TCP;

				// lookup the request
				int res = getaddrinfo(Host.c_str(), "80", &hints, &result);
				if (res != 0) {
					return res;
				}


				// create socket
				ptr = result;
				_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
				if (_socket == INVALID_SOCKET) {
					freeaddrinfo(result);
					return GetLastError();
				}

				// connect
				res = ::connect(_socket, ptr->ai_addr, ptr->ai_addrlen);
				if (res == SOCKET_ERROR) {
					close();
					return WSAGetLastError();
				}

				// all is good
				return 0;
			}

			// send
			//  sends a certain amount of text in the buffer
			//
			int send(std::string text, int hints = 0)
			{
				// attempt to send socket data
				int res = ::send(_socket, text.c_str(), (int)text.length(), hints);
				if (res == SOCKET_ERROR) {
					close();
					return WSAGetLastError();
				}

				// no problem
				return 0;
			}

			int close()
			{
				closesocket(_socket);
				_socket = INVALID_SOCKET;
				return 0;
			}

			// shutdownSend
			//  shuts down the sending method for the socket
			//
			int shutdownSend()
			{
				int res = shutdown(_socket, SD_SEND);
				if (res == SOCKET_ERROR) {
					close();
					return WSAGetLastError();
				}
				else {
					return 0;
				}
			}

			// shutdownReceive
			//  shuts down the receiving method for the socket
			//
			int shutdownReceive()
			{
				int res = shutdown(_socket, SD_RECEIVE);
				if (res == SOCKET_ERROR) {
					close();
					return WSAGetLastError();
				}
				else {
					return 0;
				}
			}

			// recvUntilClose
			//  receive loop gets all of the data from the socket
			//
			std::string recvUntilClose(int* err = 0)
			{
				std::string flow;
				char buffer[1024 * 32];
				int res = 0;

				do {
					res = recv(_socket, buffer, sizeof(buffer), 0);
					if (res > 0) {
						std::string buf(buffer, res);
						flow.append(buf);
					}
					else if (res == 0) {
						close();
					}
					else {
						*err = WSAGetLastError();
					}
				} while (res > 0);

				return flow;
			}
		};

		struct UrlParseInfo
		{
			std::string source;
			std::string protocol;
			std::string host;
			std::string path;
			int					port;

			UrlParseInfo()
			{
				protocol	= "http";
				port			= 80;
				path			= "/";
			}
		};
	}
}