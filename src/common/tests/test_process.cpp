#include "test_common.h"

#include <fstream>
#include <string>

#include "../Process.h"

void test_process() {
    require_throws(
		auto pr = WD::Process{ "non-existing" };
	);

	std::remove("test_file.txt");

	require_nothrow(
        auto pr = WD::Process{ "test_common --child 0" };
        require(pr.WaitForCompletion() == 0);
	);

	std::ifstream in_file("test_file.txt", std::ios::binary);
	std::string str;
	std::getline(in_file, str);

	require(str == "test_process_child");
}

void test_process_child() {
	std::ofstream out_file("test_file.txt", std::ios::binary);
	out_file << "test_process_child\n";
}