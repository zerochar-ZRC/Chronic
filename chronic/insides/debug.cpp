#include "debug.h"
#include <iostream>

using namespace chronic;
std::unordered_map<debug::TYPE, std::vector<std::string>> entries{};
debug::MODE mode = debug::MODE::COUT;


void debug::setMode(MODE mode) {
	::mode = mode;
}

const std::vector<std::string>& debug::getEntries(TYPE t)
{
	return entries.at(t);
}

void debug::entry(debug::TYPE type, std::string_view msg) {
	entries[type].emplace_back(msg);
	if (mode == debug::MODE::COUT) {
		std::cout << '[' << type << "] " << msg << '\n';
	}
}
