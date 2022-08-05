#ifndef RANDOM_ACCESS_ITERATOR_HPP
# define RANDOM_ACCESS_ITERATOR_HPP

# include <cstddef>
# include "iterators_traits.hpp"

namespace ft {

	template <typename T>
	class random_access_iterator {
		public:
			typedef T							value_type;
			typedef T*							pointer;
			typedef T&							reference;
			typedef ptrdiff_t					difference_type;
			typedef std::random_access_iterator_tag	iterator_category;

		private:
			pointer	__p;

		public:
			random_access_iterator() : __p(NULL) {}
			random_access_iterator(pointer ptr) : __p(ptr) {}
			~random_access_iterator() {}
			random_access_iterator(const random_access_iterator &rhs) : __p(rhs.__p) {}
			random_access_iterator &operator=(const random_access_iterator &rhs)
			{
				__p = rhs.__p;
				return *this;
			}

			reference operator*() const
			{ return *__p; }
			pointer operator->() const
			{ return __p; }
			random_access_iterator &operator++()
			{
				__p++;
				return *this;
			}
			random_access_iterator operator++(int)
			{
				random_access_iterator lhs(*this);
				++__p;
				return lhs;
			}
			random_access_iterator &operator--()
			{
				__p--;
				return *this;
			}
			random_access_iterator operator--(int)
			{
				random_access_iterator lhs(*this);
				--__p;
				return lhs;
			}

			random_access_iterator &operator+=(difference_type n)
			{
				__p += n;
				return *this;
			}
			random_access_iterator &operator-=(difference_type n)
			{
				__p -= n;
				return *this;
			}

			bool operator==(const random_access_iterator &rhs) const
			{ return (__p == rhs.__p); }
			bool operator!=(const random_access_iterator &rhs) const
			{ return (__p != rhs.__p); }
			bool operator>(const random_access_iterator &rhs) const
			{ return (__p > rhs.__p); }
			bool operator<(const random_access_iterator &rhs) const
			{ return (__p < rhs.__p); }
			bool operator>=(const random_access_iterator &rhs) const
			{ return (__p >= rhs.__p); }
			bool operator<=(const random_access_iterator &rhs) const
			{ return (__p <= rhs.__p); }

			reference operator[](difference_type n) const
			{ return *(*this + n); }
			random_access_iterator operator+(difference_type n) const
			{ return __p + n; }
			random_access_iterator operator-(difference_type n) const
			{ return __p - n; }
			difference_type operator-(const random_access_iterator &rhs) const
			{ return __p - rhs.__p; }
	};

	template <typename T>
	ft::random_access_iterator<T> operator+(typename ft::random_access_iterator<T>::difference_type n, const ft::random_access_iterator<T> &__it)
	{ return __it + n; }

}

#endif
