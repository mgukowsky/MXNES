#pragma once

#ifdef MXNES_TESTBUILD

#include <common/Core.hpp>

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
			: testName(testNameArg), hasTestPassed(true),
				numTotalTests(0), numPassedTests(0) {}
		std::string testName;
		bool hasTestPassed;
		u32 numTotalTests, numPassedTests;
	};

	template<typename T>
	bool _test_value(const T actual, const T expected, const char * const description) {
		bool retVal = actual == expected;
		bool shouldLogActualExpected = false;
		std::string msgStr;
		TestInfo &currentTest = _testStack.top();
		if (retVal) {
			msgStr = "\xFB ";
			SetConsoleTextAttribute(_hStdout, _TEXT_GREEN);
			++currentTest.numPassedTests;
		}
		else {
			msgStr = "X ";
			SetConsoleTextAttribute(_hStdout, _TEXT_RED);
			shouldLogActualExpected = true;
			currentTest.hasTestPassed = false;
		}
		++currentTest.numTotalTests;

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

	u32 _numAllTests, _numAllPassedTests;
	std::stack<TestInfo> _testStack;
	HANDLE _hStdout;

	static const std::size_t _BORDERSIZ;
	static const std::string _BORDERSTR;

	static constexpr WORD _TEXT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static constexpr WORD _TEXT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY;
	static constexpr WORD _TEXT_YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
	static constexpr WORD _TEXT_WHITE = FOREGROUND_RED | FOREGROUND_GREEN |
		FOREGROUND_BLUE | FOREGROUND_INTENSITY;

};


} //namespace MXNES

#endif //ifdef MXNES_TESTBUILD