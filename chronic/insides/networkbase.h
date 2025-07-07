#pragma once
#ifdef _WIN32

#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <ws2tcpip.h>
#include <Windows.h>

#define TO_SOCKADDR(v) reinterpret_cast<network::SOCKADDR*>(v)

#endif
namespace chronic {

	const std::string_view empty_msg{ "" };

	class WSA {
		WSAData wsa_data;
		WSA() {
			//could be unititialized.... but i dont get it why and when.
			WSAStartup(MAKEWORD(2, 1), &wsa_data);
		}
	public:
		//also initialize if haven't
		static const WSA& getData() {
			static WSA instance{};
			return instance;
		}

	};

	class Socket {
		network::SOCKADDR_IN connection{};
		network::SOCKET ptr = NULL;
	public:

		bool isAlive() const {
			return network::send(ptr, empty_msg.data(), 0, NULL) != SOCKET_ERROR;
		}
		void send(std::string_view) const;
		virtual std::string receive() const;

		//blank. Waiting for connect method call
		Client() {}
	};
}