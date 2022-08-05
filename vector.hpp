/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeson <jeson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 18:04:54 by jeson             #+#    #+#             */
/*   Updated: 2022/08/05 18:52:17 by jeson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <cstddef>
# include <memory>
# include <limits>
# include "utils/random_access_iterator.hpp"
# include "utils/reverse_iterator.hpp"
# include "utils/enable_if.hpp"
# include "utils/is_integral.hpp"
# include "utils/lexicographical_compare.hpp"

namespace ft {

	template < typename T, typename Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T			value_type;
			typedef Alloc		allocator_type;
			typedef T&			reference;
			typedef const T&	const_reference;
			typedef T*			pointer;
			typedef const T*	const_pointer;
			typedef random_access_iterator<T>				iterator;
			typedef random_access_iterator<const T>			const_iterator;
			typedef ft::reverse_iterator<iterator>			reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;
			typedef ptrdiff_t	difference_type;
			typedef size_t	size_type;

		private:
			T				*_begin;
			size_type		_size;
			size_type		_capacity;
			allocator_type	_alloc;

		public:
			vector(const allocator_type& alloc = allocator_type())
				: _begin(NULL), _size(0), _capacity(0), _alloc(alloc) {}

			vector(size_type n, const value_type& val = value_type(),
				const allocator_type& alloc = allocator_type())
			{
				_alloc = alloc;
				_size = n;
				_capacity = n;
				_begin = _alloc.allocate(n);
				for (size_t i = 0; i < n; i++)
					_alloc.construct(_begin + i, val);
			}

			template < typename InputIterator >
			vector(InputIterator _first,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value,InputIterator>::type _last,
				const allocator_type& alloc = allocator_type())
			{
				size_type n = std::distance(_first, _last);
				_alloc = alloc;
				_size = n;
				_capacity = n;
				_begin = _alloc.allocate(n);
				for (size_type i = 0; i < n; i++)
					_alloc.construct(_begin + i, *(_first++));
			}

			~vector()
			{
				clear();
				if (_begin)
					_alloc.deallocate(_begin, _capacity);
			}

			vector(const vector& rhs)
			{
				_begin = _alloc.allocate(rhs._capacity);
				_size = rhs._size;
				_capacity = rhs._capacity;
				for (size_t i = 0; i < _size; i++)
					_alloc.construct(_begin + i, rhs._begin[i]);
			}

			vector& operator=(const vector& rhs)
			{
				vector lhs(rhs);
				swap(lhs);
				return  *this;
			}

			iterator				begin()			{ return iterator(_begin); }
			iterator				end() 			{ return iterator(_begin + _size); }
			const_iterator			begin() const	{ return const_iterator(_begin); }
			const_iterator			end() const		{ return const_iterator(_begin + _size); }
			reverse_iterator		rbegin()		{ return reverse_iterator(_begin + _size); }
			reverse_iterator		rend()			{ return reverse_iterator(_begin); }
			const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(_begin + _size); }
			const_reverse_iterator	rend() const	{ return cosnt_reverse_iterator(_begin); }

			size_type	size() const { return _size; }
			size_type	max_size() const { return std::numeric_limits<difference_type>::max() / sizeof(T); }

			void	resize(size_type n, value_type val = value_type())
			{
				if (n > _capacity)
				{
					vector lhs(_alloc);
					lhs.reserve((n > _capacity * 2) ? n : _capacity * 2);
					lhs._size = n;
					for (size_t i = 0; i < n; i++)
						if (i < _size)
							_alloc.construct(lhs._begin + i, _begin[i]);
						else
							_alloc.construct(lhs._begin + i, val);
					swap(lhs);
				}
				else
				{
					for (size_t i = _size; i < _capacity; i++)
						_alloc.construct(_begin + i, val);
					_size = n;
				}
			}

			size_type	capacity() const { return _capacity; }
			bool	empty() const { return _size == 0; }
			void	reserve(size_type n)
			{
				if (n > _capacity)
				{
					if (n > max_size())
						throw std::length_error("ft::vector");
					vector lhs(_alloc);
					lhs._begin = _alloc.allocate(n);
					lhs._capacity = n;
					lhs._size = _size;
					for (size_t i = 0; i < _size; i++)
						_alloc.construct(lhs._begin + i, _begin[i]);
					swap(lhs);
			}
			}

			reference		operator[] (size_type n) { return _begin[n]; }
			const_reference	operator[] (size_type n) const { return _begin[n]; }
			reference		at(size_type n)
			{
				if (n >= _size)
					throw std::out_of_range("ft::vector");
				return _begin[n];
			}
			const_reference	at(size_type n) const
			{
				if (n >= _size)
					throw std::out_of_range("ft::vector");
				return _begin[n];
			}

			reference		front()			{ return _begin[0]; }
			reference		back()			{ return _begin[_size - 1]; }
			const_reference	front() const	{ return _begin[0]; }
			const_reference	back() const	{ return _begin[_size - 1]; }

			template <typename InputIterator>
			void assign(InputIterator first,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type last)
			{
				clear();
				size_type n = std::distance(first, last);
				if (n > _capacity)
					reserve(n);
				while (first != last)
					push_back(*first++);
			}

			void 	assign(size_type n, const value_type& val)
			{
				clear();
				if (n > _capacity)
					reserve(n);
				for (size_t i = 0; i < n; i++)
					push_back(val);
			}

			void	push_back(const value_type& val)
			{
				if (_size >= _capacity)
					reserve((_capacity == 0) ? 1 : _capacity * 2);
				_alloc.construct(_begin + _size++, val);
			}

			void	pop_back()
			{
				if (_size)
					_alloc.destroy(_begin + _size - 1);
				_size--;
			}


			iterator	insert(iterator _idx, const value_type& val)
			{
				size_type _insert_idx = _idx - begin();
				insert(_idx, 1, val);
				return begin() + _insert_idx;
			}

			void	insert(iterator _idx, size_type n, const value_type& val)
			{
				size_type _insert_idx = _idx - begin();

				if (_size + n <= _capacity) {
					for (size_type i = _size + n; i > _insert_idx; i--) {
						_alloc.destroy(_begin + i);
						_alloc.construct(_begin + i, _begin[i-n]);
					}
					_size += n;
					while (n) {
						_alloc.destroy(_begin + _insert_idx + n - 1);
						_alloc.construct(_begin + _insert_idx + n-- - 1, val);
					}
				}
				else
				{
					vector lhs(_alloc);
					lhs.reserve(_size + n);
					lhs._size = _size + n;
					for (size_type i = 0; i < _insert_idx; i++)
						_alloc.construct(lhs._begin + i, _begin[i]);
					for (size_type i = _insert_idx; i < _insert_idx + n; i++)
						_alloc.construct(lhs._begin + i, val);
					for (size_type i = _insert_idx + n; i < _size + n; i++)
						_alloc.construct(lhs._begin + i, _begin[i-n]);
					swap(lhs);
				}
			}

			template <typename InputIterator>
			void insert(iterator _idx, InputIterator first,
				typename ft::enable_if< ! ft::is_integral<InputIterator>::value, InputIterator>::type last)
			{
				size_type n = std::distance(first, last);
				size_type _insert_idx = _idx - begin();

				if (_size + n <= _capacity) {
					for (size_type i = _size + n; i > _insert_idx; i--) {
						_alloc.destroy(_begin + i);
						_alloc.construct(_begin + i, _begin[i - n]);
					}
					size_type i = 0;
					while (first != last) {
						_alloc.destroy(_begin + _insert_idx + i);
						_alloc.construct(_begin + _insert_idx + i++, *(first++));
					}
					_size += n;
				}
				else
				{
					vector lhs(_alloc);
					lhs.reserve(_size + n);
					lhs._size = _size + n;
					for (size_type i = 0; i < _insert_idx; i++)
						_alloc.construct(lhs._begin + i, _begin[i]);
					for (size_type i = _insert_idx; i < _insert_idx + n; i++)
						_alloc.construct(lhs._begin + i, *(first++));
					for (size_type i = _insert_idx + n; i < _size + n; i++)
						_alloc.construct(lhs._begin + i, _begin[i-n]);
					swap(lhs);
				}
			}

			iterator	erase(iterator _idx)
			{
				return erase(_idx, _idx + 1);
			}

			iterator	erase(iterator first, iterator last)
			{
				if (last == end()) {
					while (first != last--)
						pop_back();
				}
				else {
					size_type n = last - first;
					for (size_type i = &(*first) - _begin; i < _size - n; i++) {
						_alloc.destroy(_begin + i);
						_alloc.construct(_begin + i, _begin[i+n]);
					}
					while (n--)
						pop_back();
				}
				return first;
			}

			void	swap(vector& v)
			{
				std::swap(_begin, v._begin);
				std::swap(_size, v._size);
				std::swap(_capacity, v._capacity);
				std::swap(_alloc, v._alloc);
			}

			void	clear()
			{
				while (_size)
					pop_back();
			}

			allocator_type	get_allocator() const
			{
				return _alloc;
			}
	};

	template <typename T, typename Alloc>
	bool operator==(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return false;
		typename ft::vector<T>::const_iterator lit = lhs.begin();
		typename ft::vector<T>::const_iterator rit = rhs.begin();
		while (lit != lhs.end()) {
			if (*lit != *rit)
				return false;
			lit++;
			rit++;
		}
		return true;
	}

	template <typename T, typename Alloc>
	bool operator<(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{ return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()); }

	template <typename T, typename Alloc>
	bool operator!=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{ return !(lhs==rhs); }

	template <typename T, typename Alloc>
	bool operator>(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{ return (rhs < lhs); }

	template <typename T, typename Alloc>
	bool operator<=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{ return !(rhs < lhs); }

	template <typename T, typename Alloc>
	bool operator>=(const vector<T, Alloc> &lhs, const vector<T, Alloc> &rhs)
	{ return !(lhs < rhs); }

	template <typename T, typename Alloc>
	void swap(vector<T, Alloc>& x, vector<T, Alloc>& y)
	{ x.swap(y); }
}

#endif
