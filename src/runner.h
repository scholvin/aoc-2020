#pragma once

#include <map>
#include <string>
#include <functional>

class runner
{
public:
	typedef std::function<long(void)> basic_func_t;
	typedef std::map<std::string, basic_func_t> method_map_t;

	void add(const method_map_t& map);

	std::string get_keys() const;

	void run(const std::string& id) const;

	void run_all() const;

private:
	void run_impl(const std::string& id, basic_func_t func) const;
	method_map_t m_map;
};
