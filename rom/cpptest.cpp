/*
 * yhOS
 * Copyright (c) aceinet
 * License: GPL-2.0
 */
#include <yhos.h>

class Cpptest {
public:
	int x;
	Cpptest(int x) {
		print("Called Cpptest::Cpptest\n");
		this->x = x;
	}

	void test() {
		print("Called Cpptest::test\n");
		char buf[16];
		syscall(SYS_ITOA, this->x, buf, sizeof(buf));
		print((char*)buf);
		syscall(SYS_WRITEC, '\n');
	}
};

_start() {
	print("Performing C++ test...\n");
	Cpptest* cpptest = new Cpptest(123);
	cpptest->test();
	delete cpptest;
	return 0;
}
