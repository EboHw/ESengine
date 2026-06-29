#pragma once
#include "se/Document.h"
#include <string>
#include <vector>
#include <cstdint>
#include <unordered_map>

using std::string;
using std::vector;
using std::unordered_map;

namespace se {

class PageStore {
public:
	static void build(const vector<Document> & docs,
					  const string & ripepage_path, const string & offset_path);
	struct Offset {
		uint64_t pos;
		uint32_t len;
	}; // 文档在网页库中的位置
	
	static unordered_map<int, Offset> load_offsets(const string & offset_path);
};

} // end of namespace se
