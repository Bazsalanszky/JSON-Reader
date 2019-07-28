#include <iostream>
#include "json.h"

int main() {
	JSON::JSON json("../example.json",true);
	if (!json.isValid()) {
		switch (json.getLastError())
		{
		case JSON_FILE_NOT_FOUND:
			std::cerr << "Error! File not found!";
			return EXIT_FAILURE;
		case JSON_INVALID:
			std::cerr << "Error! Invalid JSON provided!";
			return EXIT_FAILURE;
		case JSON_IS_NUMBER:
			std::cerr << "Error! Expecting object or array, not number!";
			return EXIT_FAILURE;
		case JSON_IS_BOOL:
			std::cerr << "Error! Expecting object or array, not bool!";
			return EXIT_FAILURE;
		case JSON_IS_STRING:
			std::cerr << "Error! Expecting object or array, not string!";
			return EXIT_FAILURE;
		default:
			break;
		}
	}
	std::cout << json.dump();
	
	return 0;
}