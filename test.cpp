
#include "vector.hpp"
#include <vector>

#include <iostream>

int main(int argc, char const *argv[])
{
	std::vector<int> sv;
	ft::vector<int> fv;


	for (int i=0; i<20; i++)
	{
		sv.push_back(i);
		fv.push_back(i);
	}

	for (std::vector<int>::iterator it = sv.begin(); it != sv.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	for (ft::vector<int>::iterator it = fv.begin(); it != fv.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "sv size: " << sv.size() << std::endl;
	std::cout << "sv capacity: " << sv.capacity() << std::endl;
	std::cout << "fv size: " << fv.size() << std::endl;
	std::cout << "fv capacity: " << fv.capacity() << std::endl;
	std::cout << "============================" << std::endl;

	sv.insert(sv.begin()+10, sv.begin()+3, sv.begin()+15);
	fv.insert(fv.begin()+10, fv.begin()+3, fv.begin()+15);

	for (std::vector<int>::iterator it = sv.begin(); it != sv.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	for (ft::vector<int>::iterator it = fv.begin(); it != fv.end(); it++)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "sv size: " << sv.size() << std::endl;
	std::cout << "sv capacity: " << sv.capacity() << std::endl;
	std::cout << "fv size: " << fv.size() << std::endl;
	std::cout << "fv capacity: " << fv.capacity() << std::endl;
	std::cout << "============================" << std::endl;

	return 0;
}
