#include <iostream>
#include "json.h"

int main() {
	JSON::JSON json("../example.json");
	if (!json.isValid()) {
		switch (json.getLastError())
		{
		case JSON_FILE_NOT_FOUND:
			std::cerr << "Error! File not found!";
			return EXIT_FAILURE;
		case JSON_INVALID:
			std::cerr << "Error! Invalid JSON provided!";
			return EXIT_FAILURE;
		default:
			break;
		}
	}
	//std::cout << json.getJsonText();
	std::map<std::string, std::string> p = json.getData();
	for (std::map<std::string, std::string>::const_reverse_iterator mp = p.crbegin(); mp != p.crend();mp++) {
		std::cout << mp->first << " " << p[mp->first] << std::endl;
	}
	return 0;
}