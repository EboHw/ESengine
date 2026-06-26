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

	// 读取一个配置项；key 不存在时返回空串。string_view 参数对字面量零拷贝。
	const string & get(string_view key) const;

	// 取某个停用词表（中文/英文）。返回 const 引用，避免拷贝大集合。
	const unordered_set<string> & stopWords(string_view which) const;

	Config(const Config &)			   = delete;
	Config & operator=(const Config &) = delete;

private:
	explicit Config();
	~Config() = default;
	void load(const string & path);
	static unordered_set<string> loadSet(const string & path);

private:
	unordered_map<string, string> _kv;
	unordered_set<string> _cnStop;
	unordered_set<string> _enStop;
};

} // end of namespace se
