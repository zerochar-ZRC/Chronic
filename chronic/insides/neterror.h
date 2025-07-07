namespace chronic {

	struct network_exception {
		const std::string what;
		network_exception(std::string_view desc) : what(desc) {}
	};


	struct WSA_failed : public network_exception {
		using network_exception::network_exception;
	};

}