#include "se/DirScanner.h"

#include <cstdio>
#include <filesystem>
#include <iostream>
#include <algorithm>

// 原生实现需要的头文件
#include <dirent.h>
#include <cstring>
#include <vector>

using std::string;
using std::string_view;
using std::vector;

namespace se {
namespace fs = std::filesystem;

// ============ 现代实现：std::filesystem ============
vector<string> DirScanner::scan(string_view dir, bool recursive) {
	vector<string> files;
	std::error_code ec;  // 用错误码而不是异常，以至于遍历到坏文件不会奔溃

	if (recursive) {
		for (const auto & entry : fs::recursive_directory_iterator(dir, ec)) {
			if (entry.is_regular_file(ec))
				files.push_back(entry.path().string());
		}
	} else {
		for (const auto & entry : fs::directory_iterator(dir, ec)) {
			if (entry.is_regular_file(ec))
				files.push_back(entry.path().string());
		}
	}
	if (ec) std::cerr << "[DirScanner] warn: " << ec.message() << '\n';

	std::sort(files.begin(), files.end());
	return files;
}


// ============ 原生实现：opendir/readdir + RAII 守卫 ============
vector<string> DirScanner::scan_posix(const string & dir) {
	vector<string> files;
	DIR * dirp = ::opendir(dir.c_str());
	if (dirp == nullptr) {
		std::perror(("opendir " + dir).c_str());
		return files;
	}

	// RAII: 用一个局部对象的析构函数兜底 closedir
	struct DirGuard {
		DIR * d;
		~DirGuard() {
			if (d)
				closedir(d);
		}
	} dirguard{dirp};

	struct dirent * ent;	// readdir的返回
	while((ent = ::readdir(dirp)) != nullptr) {
		// 跳过 ./ 和 ../ 目录
		if(strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0)
			continue;

		files.push_back(dir + "/" + ent->d_name);
	}
	return files;
}

} // end of namespace se
