#include "se/DocExtractor.h"
#include "se/DirScanner.h"
#include "se/Document.h"
#include "tinyxml2-master/tinyxml2.h"

#include <regex>
#include <iostream>

using std::cerr;

namespace se {
using namespace tinyxml2;

string DocExtractor::clean(string s) {
	static const std::regex noise(R"(( | |&nbsp;|[\r\n]|<[^>]+>|showPlayer\([^)]*\); ))");
	return std::regex_replace(s, noise, "");
}

void DocExtractor::parseOne(const string & file, vector<Document> & out) {
	XMLDocument doc;
	if (doc.LoadFile(file.c_str()) != XML_SUCCESS) {
		// 解析失败就跳过这个文件
		cerr << "[DocExtractor] skip bad xml: " << file << '\n';
		return;
	}

	XMLElement * rss = doc.FirstChildElement("rss");
	if (!rss)
		return;
	XMLElement * channel = rss->FirstChildElement("channel");
	if (!channel)
		return;

	for (XMLElement * item = channel->FirstChildElement("item");
		 item != nullptr; item = item->NextSiblingElement("item")) {
		XMLElement * eTitle = item->FirstChildElement("title");
		XMLElement * eLink  = item->FirstChildElement("link");
		// 优先 <conntent> 没有就退化到 <description> 都没有就丢弃该 item
		XMLElement * eBody  = item->FirstChildElement("content");
		if (!eBody)
			eBody = item->FirstChildElement("description");
		if (!eBody)
			continue;

		Document d;
		// GetText() 对空标签返回 nullptr，用三元兜底成空串
		d.title   = clean(eTitle && eTitle->GetText() ? eTitle->GetText() : "");
		d.url     =      (eLink && eLink->GetText() ? eLink->GetText() : "");
		d.content = clean(eBody->GetText() ? eBody->GetText() : "");
		if (d.content.empty())	// 清洗后如果变空也丢弃
			continue;
		out.push_back(std::move(d));
	} 

}
vector<Document> DocExtractor::extract(const string & xmlDir) {
	vector<Document> docs;
	for(const auto & f : DirScanner::scan(xmlDir))
		if (f.size() >= 4 && f.substr(f.size() - 4) == ".xml")
			parseOne(f, docs);
	cerr <<  "[DocExtractor] extracted " << docs.size() << " raw documents\n";
	return docs;
}

} // end of namespace se
