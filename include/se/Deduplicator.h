#pragma once
#include "se/Document.h"
#include <vector>

using std::vector;

namespace se {

class Deduplicator {
public:
	static vector<Document> dedup(vector<Document> docs); 
};

} // end of namespace se
