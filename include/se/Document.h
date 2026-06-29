#pragma once
#include <string>

using std::string;

namespace se {

struct Document {
	int id = 0;
	string title;
	string url;
	string content;  // 已清洗正文
};

} // end of namespace se
