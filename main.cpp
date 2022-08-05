#include <iostream>
#include <typeinfo>
#include <list>
#include <cstdlib>

#ifdef STD
	#include <iterator>
	#include <type_traits>
	#include <vector>
	#include <stack>
	#include <map>
	#include <set>
	namespace ft = std;
#else
	#include "vector.hpp"
	#include "stack.hpp"
	#include "utils/pair.hpp"
	//#include "map.hpp"
	//#include "set.hpp"
#endif

void print_vector(ft::vector<int> &v) {
	std::cout << "(" << v.size() << ", " << v.capacity() << ") ";
	for (size_t i = 0; i < v.size(); i++)
		std::cout << v[i] << ' ';
	std::cout << "\n\n";
}

int main() {
#ifdef STD
	std::cout << "* * *   TESTING\t\tSTD   * * *\n";
#else
	std::cout << "* * *   TESTING\t\tFT   * * *\n";
#endif

	std::cout << "* * *   TESTING\t\tvector   * * *\n";
	std::cout << "빈 벡터 인스턴스 생성 v\n";
	ft::vector<int> v;
	std::cout << "";
	std::cout << "size, capacity = (" << v.size() << ", " << v.capacity() << ")\n";
	std::cout << "max_size v<int>: " << v.max_size() << "\n\n";

	std::cout << "값을 갖는 벡터 인스턴스 생성 v2\n";
	ft::vector<int> v2(5, 42);
	print_vector(v2);

	std::cout << "복사 생성자 v3(v2)\n";
	ft::vector<int> v3(v2);
	print_vector(v3);

	std::cout << "할당 연산자 v = v3\n";
	v = v3;
	print_vector(v);


	std::cout << "push_back() 10 times\n";
	for (size_t i = 1; i <= 10; i++)
		v.push_back(i);
	print_vector(v);

	std::cout << "size보다 작은 값으로 resize(3)\n";
	v.resize(3);
	print_vector(v);
	std::cout << "size보다 큰 값으로 resize(5, 21)후 초기화\n";
	v.resize(5,21);
	print_vector(v);
	std::cout << "capacity보다 큰 값으로 resize(30)\n";
	v.resize(30);
	print_vector(v);
	std::cout << "capacity보다 큰 값으로 reserve(50)\n";
	v.reserve(50);
	print_vector(v);
	std::cout << "capacity보다 작은 값으로 reserve(3)\n";
	v.reserve(3);
	print_vector(v);
	std::cout << "pop_back() 20 times\n";
	for (int i = 0; i < 20; i++)
		v.pop_back();
	print_vector(v);
	std::cout << "clear (size = 0, capacity is not changed)\n";
	v.clear();
	print_vector(v);

	std::cout << "* * *   vector.swap   * * *\n";
	for (int i = 0; i < 10; i++)
		v.push_back(i);
	std::cout << "* * *   before   * * *\n";
	print_vector(v);
	print_vector(v2);
	std::cout << "* * *   after   * * *\n";
	std::cout << "* * *   v.swap(v2)   \n";
	v.swap(v2);
	print_vector(v);
	print_vector(v2);
	std::cout << "* * *   non-member swap   * * *\n";
	ft::swap(v, v2);
	print_vector(v);
	print_vector(v2);

	std::cout << "* * *   assign at()   * * *\n";
	for (int i = 0; i < 10; i++)
		v.at(i) = 9 - i;
	print_vector(v);

	std::cout << "at() check vector's boundary and throw OutOfRange\n";
	try {
		v.at(100) = 20;
	} catch (std::out_of_range &e) { std::cout << "OutOfRange Error: " << e.what() << "\n"; }

	std::cout << "\n\n* * *   TESTING\t\tvector's Iterator   * * *\n";
	ft::vector<int>::iterator it = v.begin();
	ft::vector<int>::iterator ite = v.end();
	for (; it != ite; it++)
		std::cout << *it << " ";
	std::cout << '\n';
	it = v.begin();
	std::cout << "\n *(v.begin() + 3)\t= " << *(v.begin() + 3);
	std::cout << "\n *(3 + v.begin())\t= " << *(3 + v.begin());
	std::cout << "\n *(v.end() - 3)\t\t= " << *(ite - 3);
	std::cout << "\n v.end() - v.begin()\t= " << v.end() - v.begin() << std::endl;

	std::cout << " get_allocator\t= ";
	std::cout << typeid(v.get_allocator()).name() << "\n\n";


	std::cout << "* * *   iterator_traits of vector<int>::iterator   * * *\n";
	typedef ft::vector<int>::iterator itType;
	std::cout << "typename of iterator\t\t: " << typeid(itType).name() << std::endl;
	std::cout << "valuetype of iterator\t\t: " << typeid(ft::iterator_traits<itType>::value_type).name() << std::endl;
	std::cout << "pointer of iterator\t\t: " << typeid(ft::iterator_traits<itType>::pointer).name() << std::endl;
	std::cout << "reference of iterator\t\t: " << typeid(ft::iterator_traits<itType>::reference).name() << std::endl;
	std::cout << "differencetype of iterator\t: " << typeid(ft::iterator_traits<itType>::difference_type).name() << std::endl;
	std::cout << "category of iterator\t\t: " << typeid(ft::iterator_traits<itType>::iterator_category).name() << std::endl;


{
	std::cout << "* * *   reverse iterator   * * *\n";
	for (int i =0; i<10; i++)
		v[i] = i;
	print_vector(v);
	ft::vector<int>::reverse_iterator rev_begin = v.rbegin();
	ft::vector<int>::reverse_iterator rev_end = v.rend();
	std::cout << "*rbegin()\t\t= " << *rev_begin << "\n";
	std::cout << "*(rend()-2)\t= " << *(rev_end-2) << "\n";
	std::cout << "size\t\t\t= " << v.size() << "\n";
	std::cout << "end - begin\t= " << v.end() - v.begin() << "\n";
	std::cout << "rend - rbegin\t= " << v.rend() - v.rbegin() << "\n";
	while (rev_begin != rev_end)
		std::cout << ' ' << *rev_begin++;
	std::cout << "\n\n";
}

	std::cout << "* * *   is_integral   * * *\n";
	std::cout << "int\t\t: " << std::boolalpha << ft::is_integral<int>() << "\n";
	std::cout << "float\t\t: " << ft::is_integral<float>::value << "\n";
	std::cout << "const int\t: " << ft::is_integral<const int>::value << "\n\n";


{
	std::cout << "* * *   initialize with iterator   * * *\n";
	ft::vector<int> v2(v.begin() + 3, v.end() - 4);
	print_vector(v2);
}

{
	std::cout << "* * *   initialize with std::list<>::iterator   * * *\n";
	std::list<int> tmp(5, 42);
	ft::vector<int> v2(tmp.begin(), tmp.end());
	print_vector(v2);
}

{
	std::cout << "* * *   assign(fill)   * * *\n";
	ft::vector<int> tmp(10, 0);
	tmp.assign(3, 42);
	print_vector(tmp);
}

{
	std::cout << "* * *   assign(range) with std::list<>::iterator   * * *\n";
	ft::vector<int> tmp(10, 0);
	std::list<int> lst(15, 42);
	tmp.assign(lst.begin(), lst.end());
	print_vector(tmp);
}
	std::cout << "* * *   erase   * * *\n";
	//ft::vector<int>::iterator it = v.erase(v.end() - 3);
	it = v.erase(v.end() - 3);
	std::cout << "return value points to " << *it << " ";
	print_vector(v);

	std::cout << "* * *   erase range   * * *\n";
	v.erase(v.begin()+3, v.begin()+5);
	print_vector(v);

	std::cout << "* * *   insert(fill)   * * *\n";
	v.insert(v.begin() + 3, 3, 42);
	print_vector(v);

{
	std::cout << "* * *   insert(range) with std::list<>::iterator   * * *\n";
	ft::vector<int> tmp;
	std::list<int> lst(15, 42);
	tmp.insert(tmp.begin(), lst.begin(), lst.end());
	print_vector(tmp);
}

	std::cout << "* * *   compare two vectors   * * *\n";
	{
		ft::vector<int> foo(3, 100);
		ft::vector<int> bar(2, 200);
		std::cout << (foo < bar) << "\n";
	}
	{
		int a1[] = {1,2,3,4,5};
		int a2[] = {1,2,3,4,5};
		ft::vector<int> v1(a1, a1 + 5);
		ft::vector<int> v2(a2, a2 + 6);
		std::cout << (v1==v2) << "\n";
		std::cout << (v1!=v2) << "\n";
		std::cout << (v1<v2) << "\n";
		std::cout << (v1<=v2) << "\n";
		std::cout << (v1>v2) << "\n";
		std::cout << (v1>=v2) << "\n";
	}
	{
		int a1[] = {1,2,3,4,5};
		int a2[] = {1,2,3,4,6};
		ft::vector<int> v1(a1, a1 + 5);
		ft::vector<int> v2(a2, a2 + 6);
		std::cout << (v1==v2) << "\n";
		std::cout << (v1!=v2) << "\n";
		std::cout << (v1<v2) << "\n";
		std::cout << (v1<=v2) << "\n";
		std::cout << (v1>v2) << "\n";
		std::cout << (v1>=v2) << "\n";
	}
	{
		int a1[] = {1,2,3,4,5};
		int a2[] = {1,2,3,4,5,6};
		ft::vector<int> v1(a1, a1 + 5);
		ft::vector<int> v2(a2, a2 + 6);
		std::cout << (v1==v2) << "\n";
		std::cout << (v1!=v2) << "\n";
		std::cout << (v1<v2) << "\n";
		std::cout << (v1<=v2) << "\n";
		std::cout << (v1>v2) << "\n";
		std::cout << (v1>=v2) << "\n";
	}
	{
		int a1[] = {1,2,3,4,5};
		int a2[] = {1,2,3,4};
		ft::vector<int> v1(a1, a1 + 5);
		ft::vector<int> v2(a2, a2 + 4);
		std::cout << (v1==v2) << "\n";
		std::cout << (v1!=v2) << "\n";
		std::cout << (v1<v2) << "\n";
		std::cout << (v1<=v2) << "\n";
		std::cout << (v1>v2) << "\n";
		std::cout << (v1>=v2) << "\n";
	}

	{
		std::cout << "* * *\tTESTING\t* * *\tpair\t* * *\n";
		ft::pair<int,char> foo(10, 'a');
		ft::pair<int,char> bar(10, 'z');
		std::cout << "foo\t= (" << foo.first << ", " << foo.second << ")\n";
		std::cout << "bar\t= (" << bar.first << ", " << bar.second << ")\n";
		std::cout << "foo == bar\t: " << (foo == bar) << "\n";
		std::cout << "foo < bar\t: " << (foo < bar) << "\n";
		ft::pair<std::string, double> p1;
		ft::pair<std::string, double> p2("aaa", 2.3);
		ft::pair<std::string, double> p3(p2);
		p1 = ft::make_pair("bbb", 0.99);
		p2.first = "ccc"; p2.second = 49;
		std::cout << "p1 = (" << p1.first << ", " << p1.second << ")\n";
		std::cout << "p2 = (" << p2.first << ", " << p2.second << ")\n";
		std::cout << "p3 = (" << p3.first << ", " << p3.second << ")\n";
	}

	std::cout << "\n* * * TESTING\t\tstack\t* * *\n";
	ft::stack<int> st;

	std::cout << "size = " << st.size() << ", is_empty()\t= " << st.empty() << "\n";
	st.push(1);
	st.push(2);
	st.push(3);
	st.push(4);
	st.push(5);

	std::cout << "size = " << st.size() << ", is_empty()\t= " << st.empty() << "\n";
	while (st.size()) {
		std::cout << st.top() << "\n";
		st.pop();
	}

	std::cout << "size = " << st.size() << ", is_empty()\t= " << st.empty() << "\n";
}
