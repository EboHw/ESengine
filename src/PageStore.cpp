#include "se/PageStore.h"
#include "se/Document.h"
#include <cstdint>
#include <fstream>
#include <ios>
#include <stdexcept>
#include <iostream>

using std::ifstream;
using std::ofstream;
using std::cerr;

namespace se {

void PageStore::build(const vector<Document> & docs,
					  const string & ripepage_path, const string & offset_path) 
{
	ofstream page(ripepage_path, std::ios::binary | std::ios::trunc);
	ofstream off (offset_path, std::ios::trunc);
	if (!page || !off)
		throw std::runtime_error("PageStore: cannot open output files");

	for(const auto & d :docs) {

		string block;
		block.reserve(d.title.size() + d.url.size() + d.content.size() + 64);
		block += "<doc>\n\t<docid>" + std::to_string(d.id) + "</docid>\n";
		block += "\t<url>"          + d.url                + "</url>\n";
		block += "\t<title>"        + d.title              + "</title>\n";
		block += "\t<content>"       + d.content            + "</content>\n";

		// tellp() 拿到 '写之前' 的当前偏移--就是这篇文档的起始位置
		const uint64_t pos = static_cast<uint64_t>(page.tellp());
		page.write(block.data(), static_cast<std::streamsize>(block.size()));
		off << d.id << ' ' << pos << ' ' << block.size() << '\n';	// docid 偏移长度
	}
	cerr << "[PageStore] wrote " << docs.size() << " docs to " << ripepage_path << '\n';
}

unordered_map<int, PageStore::Offset> PageStore::load_offsets(const string & offset_path) {
	unordered_map<int, Offset> table;
	ifstream ifs(offset_path);
	int id;
	uint64_t pos;
	uint32_t len;
	while (ifs >> id >> pos >> len)
		table.emplace(id, Offset{pos, len});
	return table;
}

} // end of namespace set
