#pragma once

#include <string>
#include <vector>
#include <utility>
#include <functional>

// this is just a place to put all the daily exercises that don't really depend on each other
class basic
{
public:

	// first: true if success
	// second: numerical answer
	typedef std::pair<bool, int> result;

	// run a function by name, it'll return its result, -1 probably means error
	static result run(const std::string& name);

private:

	static int day01a();
	static int day01b();
	static int day02a();
	static int day02b();

	typedef std::function<bool(int, int, char, const std::string&)> day02func;
	static int day02worker(day02func);

	static const std::vector<int> day01input;
	static const std::vector<std::string> day02input;
};