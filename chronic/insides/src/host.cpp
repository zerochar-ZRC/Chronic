#include "../Host.h"
#include "..\neterror.h"




using namespace chronic;
const Connected* chronic::Host::getConnected(size_t i) const
{
	if (i < 0 || this->clients.size() > i) return nullptr;
	return &this->clients.at(i);;
}
//return nullptr if any error occur
const Connection* Host::waitClient() {
	if (this->clients.size() > this->max_clients) {
		debug::entry(debug::TYPE::FAIL, 
			std::format("{} cancel waiting due to connections limit ({})",
				this->name,
				this->max_clients
			));
		return nullptr;
	}
	/*rewrite dead clients if there are any(deprecated, too heavy for single thread)
	auto it = std::ranges::find_if(clients, [](Client& it)->bool {return !it.isAlive(); });
	if (it != clients.end()) {
		clients.emplace(it, 
			::accept(this->listener, TO_SOCKADDR(&host), &)
		);
		return *clients.at(clients.end()-it);
	}*/
	debug::entry(debug::TYPE::ACT, std::format("{} begin to wait client.", this->name));
	SOCKET sock = ::accept(this->listener, TO_SOCKADDR(&this->connection), &this->conn_size);
	if (sock == INVALID_SOCKET) {
		debug::entry(debug::TYPE::FAIL,
			std::format(
				"{} received connect request but failed to accept it, WSA code: {}",
				this->name,
				std::to_string(WSAGetLastError())
			));
		return nullptr;
	}

	Connection& con = this->clients.emplace_back(this, sock);

	debug::entry(debug::TYPE::ACT,std::format("{} got new connection {} ",this->name,con.name));

	return &con;
}

void Host::broadcast(std::string_view msg) const{
	for (const Connection& client : this->clients)
		client.send(msg);
}

Host::Host(std::string_view ip, 
			unsigned int port, 
			unsigned int max_clients) : Connection(ip,port,"Host"), max_clients(max_clients)
{
	::bind(this->listener, TO_SOCKADDR(&this->connection), this->conn_size);
	::listen(this->listener, SOMAXCONN);
}


