#ifdef MXNES_TESTBUILD

#include <test/Testrunner.hpp>

using namespace MXNES;

const std::size_t Testrunner::_BORDERSIZ = 80;
const std::string Testrunner::_BORDERSTR(Testrunner::_BORDERSIZ, '*');

Testrunner::Testrunner()
	: _hStdout(GetStdHandle(STD_OUTPUT_HANDLE)) {

}

void Testrunner::run_test_suite() {
	_start_test("MXNES Test Suite");
	_mmu_tests();
	_end_test();

	std::string finalMsg = "";

	if (_numAllPassedTests == _numAllTests) {
		SetConsoleTextAttribute(_hStdout, _TEXT_GREEN);
		finalMsg = "; All tests passed :D";
	}
	else {
		SetConsoleTextAttribute(_hStdout, _TEXT_RED);
	}

	std::stringstream ss;
	ss	<< "FINAL RESULTS: Passed "
			<< _numAllPassedTests
			<< " of "
			<< _numAllTests
			<< " total tests"
			<< finalMsg;

	_log_decorated(ss.str());

	SetConsoleTextAttribute(_hStdout, _TEXT_WHITE);
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
	TestInfo &currentTest = _testStack.top();
	const bool didThisTestPass = currentTest.hasTestPassed;
	const u32 numThisTestPassed = currentTest.numPassedTests;
	const u32 numThisTestTotal = currentTest.numTotalTests;

	if (didThisTestPass) {
		SetConsoleTextAttribute(_hStdout, _TEXT_GREEN);
	}
	else {
		SetConsoleTextAttribute(_hStdout, _TEXT_YELLOW);
	}

	std::stringstream ss;
	ss	<< "Ending test \""
			<< currentTest.testName
			<< "\", with "
			<< numThisTestPassed
			<< " of "
			<< numThisTestTotal
			<< " tests passed";

	_log_decorated(ss.str());
	SetConsoleTextAttribute(_hStdout, _TEXT_WHITE);

	_numAllPassedTests += numThisTestPassed;
	_numAllTests += numThisTestTotal;

	_testStack.pop();

	if (!_testStack.empty()) {
		TestInfo &newCurrentTest = _testStack.top();
		newCurrentTest.hasTestPassed = didThisTestPass;
		++newCurrentTest.numTotalTests;
		newCurrentTest.numPassedTests += (didThisTestPass) ? 1 : 0;
	}
}

#endif //ifdef MXNES_TESTBUILD