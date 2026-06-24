#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <string_view>

using std::string;
using std::unordered_map;
using std::unordered_set;
using std::string_view;
namespace se {

class Config {
public:
	static Config & getInstance();

	const string & get(string_view key) const;

	const unordered_set<string> & stopWords(string_view which) const;

	Config(const Config &)			   = delete;
	Config & operator=(const Config &) = delete;

private:
	explicit Config();
	~Config() = default;
	void load(const string & path);
	static unordered_set<string> loadSet(const string path);

private:
	unordered_map<string, string> _kv;
	unordered_set<string> _cnStop;
	unordered_set<string> _enStop;
};

} // end of namespace se
