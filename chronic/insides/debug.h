#include <string_view>
#include <functional>
#include <string>

namespace chronic::debug {
	enum TYPE {
		ACT,
		WARN,
		FAIL, //ERROR is occupied by some define
		COUNT
	};

	enum MODE {
		SILENT,
		COUT
	};

	void setMode(MODE);
	void entry(TYPE, std::string_view);
	//void listen(TYPE, callback_t);

	const std::vector<std::string>& getEntries(TYPE);
}