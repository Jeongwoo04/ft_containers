#ifndef ITERATOR_HPP
# define ITERATOR_HPP

# include <cstddef>
# include <iterator>
# include "type.hpp"

namespace ft {

	template <class Iterator>
	class iterator_traits
	{
		public:
			typedef typename Iterator::difference_type		difference_type;
			typedef typename Iterator::value_type			value_type;
			typedef typename Iterator::pointer				pointer;
			typedef typename Iterator::reference			reference;
			typedef typename Iterator::iterator_category	iterator_category;
	};

	template <typename T>
	class iterator_traits<T*>
	{
		public:
			typedef ptrdiff_t							difference_type;
			typedef T									value_type;
			typedef T*									pointer;
			typedef T&									reference;
			typedef std::random_access_iterator_tag		iterator_category;
	};

	template <typename T>
	class iterator_traits<const T*>
	{
		public:
			typedef ptrdiff_t							difference_type;
			typedef T									value_type;
			typedef const T*							pointer;
			typedef const T&							reference;
			typedef std::random_access_iterator_tag		iterator_category;
	};

	template <class Iterator>
	class reverse_iterator {
		protected:
			Iterator	__base_it;

		public:
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;

			reverse_iterator(): __base_it(iterator_type()) {}
			explicit reverse_iterator(iterator_type it): __base_it(it) {}
			template <class Iter>
			reverse_iterator(const reverse_iterator<Iter>& __rev_it): __base_it(__rev_it.base()) {}
			~reverse_iterator() {}

			template <class Other>
			reverse_iterator& operator=(const reverse_iterator<Other>& rhs)
			{
				__base_it = rhs.base();
				return (*this);
			}

			iterator_type	base() const { return __base_it; }
			reference operator*(void) const
			{
				iterator_type tmp(__base_it);
				return *--tmp;
			}
			pointer   operator->(void) const { return &(operator*()); }
			reference			operator[](difference_type n) const { return base()[-n-1]; }

			reverse_iterator operator+(difference_type n) const
			{ return (reverse_iterator(base() - n)); }
			reverse_iterator operator-(difference_type n) const
			{ return (reverse_iterator(base() + n)); }

			reverse_iterator& operator++(void)
			{
				--__base_it;
				return (*this);
			}
			reverse_iterator operator++(int)
			{
				reverse_iterator tmp(*this);
				__base_it--;
				return (tmp);
			}
			reverse_iterator& operator--(void)
			{
				++__base_it;
				return (*this);
			}
			reverse_iterator operator--(int)
			{
				reverse_iterator tmp(*this);
				__base_it++;
				return (tmp);
			}

			reverse_iterator& operator+=(difference_type n)
			{
				__base_it -= n;
				return (*this);
			}
			reverse_iterator& operator-=(difference_type n)
			{
				__base_it += n;
				return (*this);
			}
		};

		template <class Iter1, class Iter2>
		bool operator==(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
		{ return lhs.base() == rhs.base(); }

		template <class Iter1, class Iter2>
		bool operator!=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
		{ return lhs.base() != rhs.base(); }

		template <class Iter1, class Iter2>
		bool operator>(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
		{ return lhs.base() < rhs.base(); }

		template <class Iter1, class Iter2>
		bool operator>=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
		{ return lhs.base() <= rhs.base(); }

		template <class Iter1, class Iter2>
		bool operator<(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
		{ return lhs.base() > rhs.base(); }

		template <class Iter1, class Iter2>
		bool operator<=(const reverse_iterator<Iter1>& lhs, const reverse_iterator<Iter2>& rhs)
		{ return lhs.base() >= rhs.base(); }

		template <class Iter>
		reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference_type n, const reverse_iterator<Iter>& rhs)
		{ return (rhs + n); }

		template <class Iter1, class Iter2>
		typename reverse_iterator<Iter1>::difference_type operator-(const reverse_iterator<Iter1>& lhs,
			const reverse_iterator<Iter2>& rhs)
		{ return rhs.base() - lhs.base(); }

		template <typename T>
		class random_access_iterator : public std::iterator<std::random_access_iterator_tag, T>
		{
			public:
			typedef T*		iterator_type;
			typedef typename ft::iterator_traits<iterator_type>::value_type			value_type;
			typedef typename ft::iterator_traits<iterator_type>::difference_type	difference_type;
			typedef typename ft::iterator_traits<iterator_type>::pointer			pointer;
			typedef typename ft::iterator_traits<iterator_type>::reference			reference;
			typedef typename ft::iterator_traits<iterator_type>::iterator_category	iterator_category;

			private:
				pointer	__it;

			public:
				random_access_iterator() : __it(ft::NIL) {}
				random_access_iterator(pointer ptr) : __it(ptr) {}
				random_access_iterator(const random_access_iterator<T>& rhs) : __it(rhs.__it) {}
				virtual	~random_access_iterator() {}
				operator random_access_iterator<const value_type>() const
				{ return random_access_iterator<const value_type>(__it); }
				random_access_iterator &operator=(const random_access_iterator& rhs)
				{
					if (this != &rhs)
						__it = rhs.__it;
					return *this;
				}

				pointer base() const { return __it; }
				pointer operator->() const { return &operator*(); }
				reference operator*() const { return *__it; }
				reference operator[](difference_type n) const { return __it[n]; }

				random_access_iterator &operator++()
				{
					++__it;
					return *this;
				}
				random_access_iterator operator++(int)
				{
					random_access_iterator lhs(*this);
					__it++;
					return lhs;
				}
				random_access_iterator &operator--()
				{
					--__it;
					return *this;
				}
				random_access_iterator operator--(int)
				{
					random_access_iterator lhs(*this);
					__it--;
					return lhs;
				}

				random_access_iterator &operator+=(difference_type n)
				{
					__it += n;
					return *this;
				}
				random_access_iterator &operator-=(difference_type n)
				{
					__it -= n;
					return *this;
				}
				random_access_iterator operator+(difference_type n) const
				{ return __it + n; }
				random_access_iterator operator-(difference_type n) const
				{ return __it - n; }
			};

			template <typename Iter1, typename Iter2>
			bool operator==(const random_access_iterator<Iter1>& __x, const random_access_iterator<Iter2>& __y)
			{ return (__x.base() == __y.base()); }

			template <typename Iter1, typename Iter2>
			bool operator!=(const random_access_iterator<Iter1>& __x, const random_access_iterator<Iter2>& __y)
			{ return (__x.base() != __y.base()); }

			template <typename Iter1, typename Iter2>
			bool operator>(const random_access_iterator<Iter1>& __x, const random_access_iterator<Iter2>& __y)
			{ return (__x.base() > __y.base()); }

			template <typename Iter1, typename Iter2>
			bool operator>=(const random_access_iterator<Iter1>& __x, const random_access_iterator<Iter2>& __y)
			{ return (__x.base() >= __y.base()); }

			template <typename Iter1, typename Iter2>
			bool operator<(const random_access_iterator<Iter1>& __x, const random_access_iterator<Iter2>& __y)
			{ return (__x.base() < __y.base()); }

			template <typename Iter1, typename Iter2>
			bool operator<=(const random_access_iterator<Iter1>& __x, const random_access_iterator<Iter2>& __y)
			{ return (__x.base() <= __y.base()); }


			template <typename T>
			random_access_iterator<T> operator+(typename random_access_iterator<T>::difference_type n, const random_access_iterator<T>& _it)
			{ return (_it + n); }

			template <typename T1, typename T2>
			typename random_access_iterator<T1>::difference_type operator-(const random_access_iterator<T1>& lhs, const random_access_iterator<T2>& rhs)
			{ return (lhs.base() - rhs.base()); }

	template <typename T, typename U>
	class tree_iterator {
		public:
		typedef T	value_type;
		typedef T*	pointer;
		typedef T&	reference;
		typedef U*	iterator_type;
		typedef typename iterator_traits<iterator_type>::difference_type	difference_type;
		typedef typename iterator_traits<iterator_type>::value_type			node_type;
		typedef typename iterator_traits<iterator_type>::pointer			node_pointer;
		typedef typename iterator_traits<iterator_type>::reference			node_reference;
		typedef typename iterator_traits<iterator_type>::iterator_category	iterator_category;

		private:
		node_pointer	__cur;
		node_pointer	__nil;

		public:
		tree_iterator() : __cur(ft::NIL), __nil(ft::NIL) {}
		tree_iterator(node_pointer cur, node_pointer nil) : __cur(cur), __nil(nil) {}
		tree_iterator(const tree_iterator& tit) : __cur(tit.__cur), __nil(tit.__nil) {}
		~tree_iterator() {}

		tree_iterator& operator=(const tree_iterator& rhs)
		{
			if (this != &rhs)
			{
				__cur = rhs.__cur;
				__nil = rhs.__nil;
			}
			return (*this);
		}

		node_pointer	base() const		{ return (__cur); }
		pointer			operator->() const	{ return (&__cur->__value); }
		reference		operator*() const	{ return (__cur->__value); }

		tree_iterator& operator++()
		{
			__cur = __next_node(__cur, __nil);
			return (*this);
		}

		tree_iterator operator++(int)
		{
			tree_iterator tmp(*this);
			++(*this);
			return (tmp);
		}

		tree_iterator& operator--()
		{
			__cur = __prev_node(__cur, __nil);
			return (*this);
		}

		tree_iterator operator--(int)
		{
			tree_iterator tmp(*this);
			--(*this);
			return (tmp);
		}

		template <typename V>
		bool operator==(const tree_iterator<V, node_type>& x) const
		{ return (__cur == x.base()); }

		template <typename V>
		bool operator!=(const tree_iterator<V, node_type>& x) const
		{ return !(*this == x); }

		operator tree_iterator<const value_type, node_type>(void) const
		{ return (tree_iterator<const value_type, node_type>(__cur, __nil)); }

		friend bool operator==(const tree_iterator& lhs, const tree_iterator& rhs)
		{ return (lhs.__cur == rhs.__cur); }

		friend bool operator!=(const tree_iterator& lhs, const tree_iterator& rhs)
		{ return (!(lhs == rhs)); }
	};

}

#endif
