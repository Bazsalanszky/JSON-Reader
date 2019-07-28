#pragma once
#include <fstream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>

#define JSON_FILE_NOT_FOUND 19001
#define JSON_INVALID 19002
#define JSON_IS_NUMBER 19101
#define JSON_IS_STRING 19102
#define JSON_IS_BOOL 19103

#define JSON_TYPE_OBJECT 0
#define JSON_TYPE_ARRAY 1
#define JSON_TYPE_STRING 2
#define JSON_TYPE_INT 3
#define JSON_TYPE_DOUBLE 4
#define JSON_TYPE_BOOL 5
#define JSON_TYPE_NULL -1

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
			if (type == JSON_TYPE_ARRAY) {
				parts = getParts();
				for (int i = 0; i < parts.size(); i++) {

					JSON j(parts[i]);
					j_map.insert(std::make_pair(std::to_string(i), j));
				}
			}
			if (type == JSON_TYPE_OBJECT) {
				parts = getParts();
				j_map = separate();
			}


			valid = true;
		}

		

		bool isValid() { return valid; }
		std::string getJsonText() { return json; }
		std::map<std::string, std::string> getData() { return data; }

		int getLastError() { return error; }
		
		JSON& operator[](const std::string& key)
		{
			if (j_map.find(key) != j_map.end())
				return j_map.at(key);
			else { JSON j(""); return j; };
		}
		friend std::ostream& operator<<(std::ostream& os, JSON& js);

		std::string dump(int t=0) {
			std::ostringstream ss;
			if (!isValid()) return std::to_string(JSON_INVALID);
			for (std::map<std::string, JSON>::const_reverse_iterator mp = j_map.crbegin(); mp != j_map.crend(); mp++) {
				JSON j = mp->second;
				for (int i = 0; i < t; i++) ss << "\t";
				if (j.isArray() || j.isObject()) ss << mp->first << std::endl << j.dump(t+1);
				else ss << mp->first << " " << j.getJsonText() << std::endl;
			}
			return ss.str();
		}

		int getInt() {
			if (type == JSON_TYPE_INT) return stoi(json);
			else return 0;
		}
		double getDouble() {
			if (type == JSON_TYPE_DOUBLE) return atof(json.c_str());
			else return 0;
		}
		bool getBool() {
			if (type == JSON_TYPE_DOUBLE) 
				if (json == "true") return true;
				else return false;
			else return false;
		}
		JSON* getArray() {
			JSON* result;
			int i = 0;
			for (std::map<std::string, JSON>::iterator mp = j_map.begin(); mp != j_map.end();mp++) {
				result[i] = mp->second;
				i++;
			}
		}
		std::map<std::string, JSON> getJmap() { return j_map; }

		std::string getString() {
			if (type == JSON_TYPE_STRING) return json;
			else return  "";
		}
		


		int getType() {
			if (isObject()) return JSON_TYPE_OBJECT;
			if (isArray()) return JSON_TYPE_ARRAY;
			if (isString()) return JSON_TYPE_STRING;
			if (isInt()) return JSON_TYPE_INT;
			if (isDouble()) return JSON_TYPE_DOUBLE;
			if (isBool()) return JSON_TYPE_BOOL;
			if (isNull()) return JSON_TYPE_NULL;
			else return JSON_INVALID;
		}

		bool isObject() {
			if (json[0] == '{' && json.back() == '}') return true;
			return false;
		}
		bool isArray() {
			if (json[0] == '[' && json.back() == ']') return true;
			return false;
		}
		bool isString() {
			if (json[0] == '"' && json.back() == '"') return true;
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
		bool isDouble()
		{
			const char* str = json.c_str();
			char* endptr = 0;
			strtod(str, &endptr);

			if (*endptr != '\0' || endptr == str)
				return false;
			return true;
		}
		bool isNull()
		{
			if (json == "null") return true;
			return false;
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
			if (type == JSON_TYPE_ARRAY) {
				parts = getParts();
				for (int i = 0; i < parts.size(); i++) {
					
					JSON j(parts[i]);
					j_map.insert(std::make_pair(std::to_string(i),j));
				}
			}
			if (type == JSON_TYPE_OBJECT) {
				parts = getParts();
				j_map = separate();
			}
			if (type == JSON_TYPE_STRING) {
				json.erase(0, 1);
				json.erase(json.begin() + json.size() - 1);
			}
			valid = true;
		}


		int check() {
			if (isObject() || isArray()) return 0;
			if (isInt()) return JSON_IS_NUMBER;
			if (isBool()) return JSON_IS_BOOL;
			if (isString()) return JSON_IS_STRING;
			else return JSON_INVALID;
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
			result.push_back(buf);
			return result;
		}
		
		std::map<std::string, JSON> separate() {
			std::map<std::string, JSON> result;
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
				JSON j(value);
				result.insert(std::make_pair(key,j));	
			}
			return result;
		}
		
	};
	std::ostream& operator<<(std::ostream& os, JSON& js) {
		os << js.getJsonText();
		return os;
	}
}