#pragma once

#include <string_view>
#include <vector>
#include <functional>

#include "base.h"
namespace chronic {
	using Connected = Connection;
	class Host final : public Connection {
		std::vector<Connected> clients;

		const unsigned int max_clients;
		using Connection::receive;
	public:

		unsigned int getMaxClients() const { return max_clients; }
		
		const Connected* getConnected(size_t) const;
		const Connected* waitClient();
		
		void broadcast(std::string_view) const;

		Host() = delete;
		Host(std::string_view, unsigned int, unsigned int=16);

	};
}