#pragma once
#include <string>
#include <vector>
#include <string_view>

using std::string_view;
using std::vector;
using std::string;

namespace se {
// 目录扫描器: 递归列出该目录下所有[普通文件]的完整路径
// 不持有数据, 线程安全, 工具类
class DirScanner {
public:
	// 现代实现, 基于 std::filesystem。recursive=true 时递归子目录。
	static vector<string> scan(string_view dir, bool recursive = false);

	// 原生实现: 基于 opendir/readdir, 学习目的(非递归)
	static vector<string> scan_posix(const string & dir);

	DirScanner() = delete;
};

} // end of namesapce se
