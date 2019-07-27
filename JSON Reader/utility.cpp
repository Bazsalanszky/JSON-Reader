#pragma once
#include "utility.h"
namespace JSON {
	std::vector<std::string> split(std::string txt, char s = ' ') {
		std::string buf = "";
		std::vector<std::string> v;
		bool first = true;
		for (unsigned int i = 0; i < txt.length(); i++) {
			if (txt[i] == s) {
				if (buf != "")v.push_back(buf);
				buf = "";
			}
			else {
				buf += txt[i];
			}
		}
		v.push_back(buf);
		return v;
	}
	void replaceAll(std::string& data, std::string search, std::string replace) {

		size_t pos = data.find(search);
		while (pos != std::string::npos) {
			data.replace(pos, search.size(), replace);
			pos = data.find(search, pos + replace.size());
		}
	}
}