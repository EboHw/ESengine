#pragma once
#include <string>
#include <vector>
#include <string_view>
#include <unordered_set>
#include <memory>

using std::string;
using std::string_view;
using std::vector;
using std::unordered_set;

namespace se {

// 分词器的抽象接(策略基类)。上层只依赖这个接口。
class Tokenizer {
public:
	virtual ~Tokenizer() = default;
	// 切词；返回过滤掉停用词后的词序列
	virtual vector<string> cut(string_view text) const = 0;
};

// 中文分词器: 基于 cppjieba 的具体策略
// 单例: Jieba 构造时要加载数十 MB 词典, 全局构建一次
class JiebaTokenizer final
: public Tokenizer 
{
public:
	static JiebaTokenizer & getInstance();
	vector<string> cut(string_view text) const override;

	JiebaTokenizer(const JiebaTokenizer &) = delete;
	JiebaTokenizer & operator=(const JiebaTokenizer &) = delete;

private:
	JiebaTokenizer();
	~JiebaTokenizer() override;

private:
	struct Impl;
	std::unique_ptr<Impl> _impl;
	const unordered_set<string> * _stopWords;
};

} // end of namespace se
