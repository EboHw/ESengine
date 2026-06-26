#include "se/Config.h"
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include <string>
#include <string_view>

using std::ifstream;
using std::string;
using std::istringstream;
using std::cerr;
using std::unordered_set;
using std::string_view;

namespace se {

static constexpr const char * kConfPath = "../conf/server.conf";

Config & Config::getInstance() {
	static Config inst;
	return inst;
}

Config::Config() {
	load(kConfPath);
	_cnStop = loadSet(get("cn_stopwords"));
	_enStop = loadSet(get("en_stopwords"));
}

void Config::load(const string & path) {
	ifstream ifs(path);	

	if (!ifs) 
		throw std::runtime_error("Config: cannot open " + path);

	string line;
	while (getline(ifs, line)) {
		if(line.empty() || line[0] == '#')
			continue;

		istringstream iss(line);
		string key, value;
		if (iss >> key >> value)
			_kv[key] = value;

		cerr << "[Config] loaded " << _kv.size()
			 << " entries from " << path << '\n';
	}
}

unordered_set<string> Config::loadSet(const string & path) {
	unordered_set<string> stopWords;
	ifstream ifs(path);
	string words;
	while (getline(ifs, words)) {
		if (!words.empty() && words.back() == '\r')
			words.pop_back();


		if (!words.empty())
			stopWords.insert(std::move(words));
	}

	return stopWords;
} 

const string & Config::get(string_view key) const {
	static const string kEmpty;
	auto it = _kv.find(string(key));

	return it == _kv.end() ? kEmpty : it->second;
}

const unordered_set<string> & Config::stopWords(string_view which) const {
	return which == "cn" ? _cnStop : _enStop;
}

}// end of namespace se
