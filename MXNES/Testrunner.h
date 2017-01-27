#pragma once

#ifdef MXNES_TESTBUILD

#include "Core.h"

#pragma warning(push, 0)
#include <sstream>
#include <stack>
#include <string>
#pragma warning(pop)

namespace MXNES {

class Testrunner {
public:
	typedef void(*TestFunc)();

	Testrunner();
	~Testrunner() = default;

	MXNES_DISABLE_ALTERNATE_CONSTRUCTORS(Testrunner);

	void run_test_suite();

private:

	struct TestInfo {
		TestInfo(std::string testNameArg)
			: testName(testNameArg), hasTestPassed(true) {}
		std::string testName;
		bool hasTestPassed;

	};

	template<typename T>
	bool _test_value(const T actual, const T expected, const char * const description) {
		bool retVal = actual == expected;
		bool shouldLogActualExpected = false;
		std::string msgStr;
		if (retVal) {
			msgStr = "\xFB ";
			SetConsoleTextAttribute(_hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
		}
		else {
			msgStr = "X ";
			SetConsoleTextAttribute(_hStdout, FOREGROUND_RED | FOREGROUND_INTENSITY);
			shouldLogActualExpected = true;
			_testStack.top().hasTestPassed = false;
		}

		_log(msgStr + description );

		if (shouldLogActualExpected) {
			std::stringstream ssActualExpected;
			ssActualExpected << "\tActual was " << actual << " but expected " << expected;
			_log(ssActualExpected.str());
		}

		return retVal;
	}

	void _start_test(std::string testName);
	void _end_test();

	template<typename T>
	void _log(const T msg) {
		std::cout << std::string(_testStack.size(), '\t') << msg << std::endl;
	}

	template<typename T>
	void _log_decorated(const T msg) {
		std::size_t paddingSize = ((_BORDERSIZ - msg.size()) / 2) - 1;

		if (paddingSize < 0 || paddingSize > _BORDERSIZ / 2) {
			paddingSize = 0;
		}

		const std::string paddingStr(paddingSize, ' ');

		std::string msgStr = std::string("*") + paddingStr + msg + paddingStr + "*";

		_log(_BORDERSTR);
		_log(msgStr);
		_log(_BORDERSTR);
	}

	void _mmu_tests();

	std::stack<TestInfo> _testStack;
	HANDLE _hStdout;

	static const std::size_t _BORDERSIZ;
	static const std::string _BORDERSTR;

};


} //namespace MXNES

#endif //ifdef MXNES_TESTBUILD