#pragma once
#include <string_view>
#include "base.h"
namespace chronic {
	struct Client : public Connection {

		bool connect() {
			bool success = ::connect(
				this->listener,
				TO_SOCKADDR(&this->connection),
				this->conn_size) == 0;
			if (!success) debug::entry(debug::TYPE::FAIL,
								std::format("{} failed connect to {}:{}. WSA code: {}",
								this->name,
								this->ip,
								std::to_string(this->port),
								WSAGetLastError()
							));
			else debug::entry(debug::TYPE::ACT,
				std::format("{} connected to {}:{}.",
					this->name,
					this->ip,
					std::to_string(this->port),
					WSAGetLastError()
				));
			return success;
		}

		//Creates bounded socket to host's ip and port
		//All actions will be invalid until Client::connect() success
		Client(std::string_view ip, int port,std::string_view name="Unnamed")
			: Connection(ip, port,std::string("Client: ").append(name)) {
			
		}

	};
}