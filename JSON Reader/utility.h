#pragma once
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <map>

#define Str std::string
namespace JSON {
	std::vector<std::string> split(std::string txt, char s);
	void replaceAll(std::string& data, std::string search, std::string raplace);
}