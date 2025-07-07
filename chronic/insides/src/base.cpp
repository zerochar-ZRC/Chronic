#include "../base.h"
using namespace chronic;
unsigned int id = 0;
Connection::Connection(Connection* host, SOCKET sock) 
	: ip(host->ip), port(host->port), name(std::format("{}'s con {}",host->name,std::to_string(id)))
{
	id += 1;
	listener = sock;
}


//get digits count
template<typename T> requires std::is_arithmetic_v<T>
unsigned int getDC(T num) {
	if (num < 0) num *= -1;
	unsigned int digits = 0;
	for (; num > 1; digits++) num /= 10;
	return digits;
}

void Connection::send(std::string_view raw) const {
	size_t len = raw.size();
	std::string msg = std::format("{}{}{}", 
									getDC<decltype(len)>(len),
									len,
									raw);

	if (::send(this->listener, msg.c_str(), msg.size(), NULL)
		!= SOCKET_ERROR) 
		debug::entry(debug::TYPE::ACT, std::format("{} succesfuly send packet with {} len", this->name, std::to_string(len)));
	else
		debug::entry(debug::TYPE::FAIL, std::format("{} failed send packet. WSA code: {}", this->name, WSAGetLastError()));
	
}
//there is no longer way to define len to read
//it was expensive and untrustable
//However there is no limit for send, 
//there is limit for receive in UINT16_MAX msg len
//if there an error happened, will return empty string
std::string Connection::receive() const {
	char digit[6];
	char count = 0;
	sockaddr_in who;
	int size = sizeof(who);
	unsigned int len = 0;

	ZeroMemory(digit, 6);

	debug::entry(debug::ACT, this->name + " is waiting incoming message");

	::recvfrom(this->listener, &count, 1, NULL,TO_SOCKADDR(&who), &size);
	debug::entry(debug::TYPE::ACT, std::format("{} received message from {}", this->name, who.sin_addr.S_un.S_addr));
	count -= '0';
	if (count > 5 || count < 0) {
		debug::entry(debug::TYPE::FAIL, std::format("{} received large or corrupted message. Declined.", this->name));
		return std::string(); 
	}
 	
	::recv(this->listener, &digit[0], count , NULL); // get meta (length)
	debug::entry(debug::ACT, std::format("{} resolved message with {} len", this->name, digit));

	for (char i = 0; i != 5; i++) {
		char c = digit[i];
		if (c == 0) break;
		len += c - '0';
	}

	char* msg = new char[len];
	
	::recv(this->listener, msg, len, NULL);
	std::string msg_s{ msg, &msg[len] };
	delete[] msg;
	return msg_s;
}
