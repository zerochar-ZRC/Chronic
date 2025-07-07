#pragma once

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#define TO_SOCKADDR(v) reinterpret_cast<SOCKADDR*>(v)

#include <string_view>
#include <string>
#include <format>

#include "debug.h"

namespace chronic {

	const std::string_view empty_msg{ "" };

	class WSA {
		WSAData wsa_data;
		WSA() {
			//could be unititialized.... but i dont get it why and when.
			WSAStartup(MAKEWORD(2, 1), &wsa_data);
		}
	public:
		static const WSA& getData() {
			static WSA instance{};
			return instance;
		}
		~WSA() {
			WSACleanup();
		}
	};

	class Socket {
	protected:
		SOCKADDR_IN connection{};
		SOCKET listener = NULL;
		//however this should be const, winsock require unconst
		static inline int conn_size = sizeof(connection);
	public:
		Socket() {}
		~Socket() {
			closesocket(listener);
		}
	};
	
	class Host;
	struct Connection : public Socket {
		const std::string name;
		const std::string ip;
		const uint16_t port;

		bool isAlive() const {
			return ::send(
						this->listener, 
						empty_msg.data(), 
						0, 
						NULL) != SOCKET_ERROR;
		}
		
		void send(std::string_view) const;
		virtual std::string receive() const;

		Connection(std::string_view ip, uint16_t port,std::string_view name="Unnamed connection")
			: ip(ip),port(port),name(name)
		{
			WSA::getData();
			::inet_pton(AF_INET, ip.data(), &connection.sin_addr);
			connection.sin_port = ::htons(port);
			connection.sin_family = AF_INET;
			this->listener = ::socket(AF_INET, SOCK_STREAM, NULL);
		}
		Connection(Connection*, SOCKET);
		
	};
}