#pragma once
#include "se/Document.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

namespace se {

class DocExtractor {
public:
	static vector<Document> extract(const string & xmlDir);

private:
	static void parseOne(const string & file, vector<Document> & out);
	static string clean(string s);
};

} // end of namespace se
