#pragma once
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "utility.h"

#define JSON_FILE_NOT_FOUND 19001
#define JSON_INVALID 19002
#define JSON_IS_NUMBER 19101
#define JSON_IS_STRING 19102
#define JSON_IS_BOOL 19103

namespace JSON {

	class JSON {
	public:
		JSON(std::string path, bool b) {
			std::ifstream file(path);
			//File not found
			if (!file.good()) { error = JSON_FILE_NOT_FOUND; return; }
			std::string line;
			std::ostringstream ss;
			while (std::getline(file, line)) {
				ss << line;
			}
			line.clear();
			json = ss.str();
			ss.clear();

			clearVoid();
			if (check() == JSON_INVALID) { error = JSON_INVALID; return; }
			parts = getParts();
			data = separate();


			valid = true;
		}

		

		bool isValid() { return valid; }
		std::string getJsonText() { return json; }
		std::map<std::string, std::string> getData() { return data; }

		int getLastError() { return error; }

		std::string& operator[](const std::string& key)
		{
			return data[key];
		}
	private:

		std::map<std::string, std::string> data;
		std::map<std::string, JSON> j_map;
		std::vector<std::string> parts;
		std::string json;
		bool valid = false;
		int error = 0;
		/*Type codes
		0 = object
		1 = array
		2 = string
		3 = int
		4 = float
		5 = bool
		-1 = null
		*/
		int type = 0;

		JSON(std::string json_c) {
			json = json_c;
			clearVoid();
			type = getType();
			if (type == JSON_INVALID) return;


			valid = true;
		}


		int check() {
			if (isObject() || isArray()) return 0;
			if (isInt()) return JSON_IS_NUMBER;
			if (isBool()) return JSON_IS_BOOL;
			if (isString()) return JSON_IS_STRING;
			else return JSON_INVALID;
		}

		int getType() {
			if (isObject()) return 0;
			if (isArray()) return 1;
			if (isString()) return 2;
			if (isInt()) return 3;
			if (isBool()) return 5;
			if (isNull()) return -1;
			else return JSON_INVALID;
		}

		bool isObject() {
			if (json[0] != '{' && json.back() != '}') return true;
			return false;
		}
		bool isArray() {
			if (json[0] != '[' && json.back() != ']') return true;
			return false;
		}
		bool isString() {
			if (json[0] != '"' && json.back() != '"') return true;
			return false;
		}
		bool isBool() {
			if (json == "true" || json == "false") return true;
			return false;
		}
		bool isInt()
		{
			return !json.empty() && std::find_if(json.begin(),
				json.end(), [](char c) { return !std::isdigit(c); }) == json.end();
		}
		bool isNull()
		{
			if (json == "null") return true;
			return false;
		}
		

		void clearVoid() {
			bool a = false;
			std::ostringstream ss;
			for (unsigned int i = 0; i < json.size(); i++) {
				if (json[i] == '"') {
					if (a) {
						a = false;
						ss << json[i];
						continue;
					}
					else {
						a = true;
						ss << json[i];
						continue;
					}
				}
				if (json[i] == ' ' and !a) continue;
				ss << json[i];
			}
			json = ss.str();
		}
		std::vector<std::string> getParts() {
			std::vector<std::string> result;
			std::string buf = "";
			int fcount = 0;
			for (unsigned int i = 1; i < json.size() - 1; i++) {
				char c = json[i];
				//Out of index? Maybe later :P
				if (c == '[' || c == '{') {
					buf += c;
					fcount++;
					continue;
				}
				if (c == '}' || c == ']') {
					buf += c;
					fcount--;
					continue;
				}
				if (c == ',' && fcount == 0) {
					result.push_back(buf);
					buf = "";
					continue;
				}
				buf += c;
			}
			return result;
		}

		std::map<std::string, std::string> separate() {
			std::map<std::string, std::string> result;
			for (unsigned int i = 0; i < parts.size(); i++) {
				bool first = true;
				std::string key = "", value = "";
				std::string s = parts[i];
				for (unsigned int j = 0; j < s.size(); j++) {
					if (s[j] == ':' && first) { first = false; continue; }
					if (first) {
						if (s[j] == '"' && j == 0 || s[j] == '"' && s[j - 1] != '\\') continue;
						key += s[j];
					}
					else {
						//if (s[j] == '"' && s[j - 1] != '\\') continue;
						value += s[j];
					}
				}
				result[key] = value;
			}
			return result;
		}
	};
}