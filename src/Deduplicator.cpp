#include "se/Deduplicator.h"
#include "se/Config.h"
#include "se/Document.h"
#include "simhash/include/simhash/Simhasher.hpp"

#include <unordered_set>
#include <bit>
#include <iostream>

using std::unordered_set;
using std::cerr;

namespace se {

// simhash 值汉明距离 <= 3 即认为是重复
struct HammingEqual {
	bool operator()(uint64_t a, uint64_t b) const {
		return std::popcount(a ^ b) <= 3;
	}
};

vector<Document> Deduplicator::dedup(vector<Document> docs) {
	const auto & cfg = Config::getInstance();

	// 构造 simhasher
	simhash::Simhasher hasher(cfg.get("jieba_dict"), cfg.get("jieba_hmm"),
							  cfg.get("jieba_idf"), cfg.get("jieba_stop"));

	// 把 hash 值作为 key 去重
	unordered_set<uint64_t, std::hash<uint64_t>, HammingEqual> seen;

	vector<Document> unique;
	unique.reserve(docs.size());
	const size_t topN = 20;		// 权重最高的 20 个特征词生成指纹

	int next_id = 1;
	for (auto & d :docs) {
		uint64_t fp = 0;
		const string & text = d.content.empty() ? d.title : d.content;
		hasher.make(text, topN, fp);

		// insert 返回 {iterator, bool}；bool=true 表示是新指纹（不与任何已有的近似)
		if (seen.insert(fp).second) {
			d.id = ++next_id;
			unique.push_back(std::move(d));
		}
	}
	cerr << "[Dedup] " << docs.size() << " -> " << unique.size() << " after dedup\n";
	return unique;
}

} // end of namespace se
