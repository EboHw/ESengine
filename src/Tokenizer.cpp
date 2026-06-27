#include "se/Config.h"
#include "se/Tokenizer.h"
#include "cppjieba/Jieba.hpp"

namespace se {

struct JiebaTokenizer::Impl {
	cppjieba::Jieba jieba;
	Impl(const string & dict, const string & hmm,
         const string & user, const string & idf, const string& stop)
    : jieba(dict, hmm, user, idf, stop) 
	{}
};

JiebaTokenizer & JiebaTokenizer::getInstance() {
	static JiebaTokenizer inst;
	return inst;
}

JiebaTokenizer::JiebaTokenizer() {
	const auto & cfg = Config::getInstance();

	// 构造内部的 cppjieba
	_impl = std::make_unique<Impl>(
		cfg.get("jieba_dict"), cfg.get("jieba_hmm"), 
		cfg.get("jieba_user"), cfg.get("jieba_idf"),
		cfg.get("jieba_stop"));
	_stopWords = &cfg.stopWords("cn");
}

JiebaTokenizer::~JiebaTokenizer() = default;

vector<string> JiebaTokenizer::cut(string_view text) const {
	vector<string> raw;
	_impl->jieba.CutForSearch(string(text), raw);	// 搜索引擎模式, 粒度细、召回高
	vector<string> result;
	result.reserve(raw.size());
	for(auto & w :raw) {
		if(w == " " || w.empty())
			continue;
		if(_stopWords->count(w))
			continue;
		result.push_back(std::move(w));
	}
	return result;
}

} // end of namespace se
