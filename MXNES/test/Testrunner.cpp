#ifdef MXNES_TESTBUILD

#include <test/Testrunner.hpp>

using namespace MXNES;

const std::size_t Testrunner::_BORDERSIZ = 60;
const std::string Testrunner::_BORDERSTR(Testrunner::_BORDERSIZ, '*');

Testrunner::Testrunner()
	: _hStdout(GetStdHandle(STD_OUTPUT_HANDLE)) {

}

void Testrunner::run_test_suite() {
	_start_test("MXNES Test Suite");
	_mmu_tests();
	_end_test();
}

void Testrunner::_start_test(std::string testName) {
	if (testName.size() & 1)
		testName += " ";

	SetConsoleTextAttribute(_hStdout, _TEXT_WHITE);
	const std::string testNameStr = std::string("Starting test \"") + testName + "\"";
	_log_decorated(testNameStr);
	_testStack.push(testName);
}

void Testrunner::_end_test() {
	if (_testStack.top().hasTestPassed) {
		SetConsoleTextAttribute(_hStdout, _TEXT_WHITE);
	}
	else {
		SetConsoleTextAttribute(_hStdout, _TEXT_YELLOW);
	}

	const std::string completedTestStr = std::string("Ending test \"") 
		+ _testStack.top().testName 
		+ "\"";

	_testStack.pop();
	_log_decorated(completedTestStr);
	SetConsoleTextAttribute(_hStdout, _TEXT_WHITE);
}

#endif //ifdef MXNES_TESTBUILD