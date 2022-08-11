#include <iostream>
#ifdef STD
	#include <vector>
	#include <map>
	#include <stack>
	#include <set>
	#include <list>
	namespace ft = std;
#else
	#include "vector.hpp"
	#include "map.hpp"
	#include "stack.hpp"
	#include "set.hpp"
#endif

int main(void)
{
	std::cout	<< "                        vector test                         \n"
				<< "============================================================\n";

	std::cout << ("size not (size_t)-1 on empty (undefined behavior)") << std::endl;
	ft::vector<int> ft;
	ft.pop_back();

	bool t = (ft.size() == static_cast<size_t>(-1));
	std::cout << t << std::endl;

	ft::vector<int> vec;

	std::cout << vec.empty();

	for (int i = 0; i < 5; i++)
		vec.push_back(i * 10);

	for (ft::vector<int>::iterator it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	for (ft::vector<int>::reverse_iterator rit = vec.rbegin(); rit != vec.rend(); rit++)
		std::cout << *rit << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	vec.resize(30, 42);
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	vec.pop_back();
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec.at(i) << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	vec.reserve(20);
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	std::cout << vec.front() << "\n";
	std::cout << vec.back() << "\n";

	vec.insert(vec.begin() + 1, 3, 999);
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	vec.erase(vec.end() - 5);
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	vec.assign(5, 10);
	for (size_t i = 0; i < vec.size(); i++)
		std::cout << vec[i] << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	ft::vector<int> copy_vec = vec;
	for (size_t i = 0; i < copy_vec.size(); i++)
		std::cout << copy_vec[i] << " ";
	std::cout << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";

	std::cout << (vec == copy_vec) << " " << (vec != copy_vec) << "\n";
	std::cout << "capacity : " << vec.capacity() << ", size : " << vec.size() << "\n";
	std::cout << "vector max size  : " << vec.max_size() << std::endl;

	std::cout	<< "                         stack test                         \n"
				<< "============================================================\n";

	ft::stack<int> st;
	for (int i = 1; i < 6; i++)
		st.push(i * 11);

	std::cout << st.empty() << "\n";
	std::cout << st.size() << "\n";

	ft::stack<int> copy_st = st;
	std::cout << (st == copy_st) << " " << (st != copy_st) << "\n";

	while (!st.empty())
	{
		std::cout << st.top() << " ";
		st.pop();
	}
	std::cout << "\n";
	std::cout << st.empty() << "\n";


	std::cout	<< "                          map test                          \n"
				<< "============================================================\n";


	ft::map<char, int> mp;
	mp['a'] = 10;
	mp['b'] = 20;
	mp['c'] = 30;
	mp.insert(ft::make_pair('d', 40));
	mp.insert(ft::make_pair('e', 50));

	std::cout << mp.empty() << "\n";
	std::cout << mp.size() << "\n";

	for (ft::map<char, int>::iterator it = mp.begin(); it != mp.end(); it++)
		std::cout << it->first << ", " << it->second << "\n";

	std::cout << "a : " << mp.at('a') << "\n";
	std::cout << "b : " << mp['b'] << "\n";

	mp.erase(mp.find('b'));

	for (ft::map<char, int>::reverse_iterator rit = mp.rbegin(); rit != mp.rend(); rit++)
		std::cout << rit->first << ", " << rit->second << "\n";

	std::cout << mp.count('a') << "\n";

	std::cout << mp.lower_bound('b')->first << "\n";
	std::cout << mp.upper_bound('d')->first << "\n";

	ft::map<char, int> copy_mp = mp;

	std::cout << (mp == copy_mp) << " " << (mp != copy_mp) << "\n";
	return 0;
}
