#pragma once

#include <map>
#include <string>
#include <functional>

class runner
{
public:
	runner() : m_count(0) { }

	struct new_order
	{
		bool operator()(const std::string& left, const std::string& right) const;
	};
	typedef std::function<long(void)> basic_func_t;
	typedef std::map<std::string, basic_func_t, new_order> method_map_t;

	void add(const method_map_t& map);

	std::string get_keys() const;

	void run(int argc, char** argv);

private:
	void run_impl(const std::string& id, basic_func_t func);
	method_map_t m_map;
	int m_count;
};
