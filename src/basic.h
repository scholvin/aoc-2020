#pragma once

#include <string>
#include <vector>
#include <utility>
#include <functional>
#include <map>

// this is just a place to put all the daily exercises that don't really depend on each other
// everything is static, could probably be a namespace
class basic
{
public:
	// the methods that run in this "class"
	typedef std::function<long(void)> basic_func;

	// run a function by name, print answer to std::cout, complain if not valid
	static void run(const std::string& name);

	// run them all in order
	static void run_all();

	// return all the keys that we know as a space-delimited list
	static std::string get_keys();

private:

	static long day01a();
	static long day01b();
	static long day02a();
	static long day02b();
	static long day03a(int right, int down);
	static long day03b();

	// use a lambda for the two different validation functions on day 2
	typedef std::function<bool(int, int, char, const std::string&)> day02func;
	static long day02worker(day02func);

	// data structure for the run tracker
	static const std::map<std::string, std::function<long(void)>> method_map;

    // input data
	static const std::vector<int> day01input;
	static const std::vector<std::string> day02input;
	static const std::vector<std::string> day03input;
};