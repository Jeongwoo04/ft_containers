/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeson <jeson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 18:04:54 by jeson             #+#    #+#             */
/*   Updated: 2022/08/11 16:46:05 by jeson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef VECTOR_HPP
# define VECTOR_HPP

# include <memory>
# include "utils/iterator.hpp"
# include "utils/type.hpp"
# include "utils/algorithm.hpp"

/* std::allocator
 * 메모리 할당과 해제 클래스
 1. namespace std { ... } 안에서 정의하라.

 2. 템플릿으로 만들어야 한다. 템플릿 매개 변수에는 할당하고자 하는 객체의 타입 (T)을 사용한다.

 3. pointer(T*)와 reference(T&)라는 typedef를 제공해야 한다.

 4. 비정적 데이터 멤버를 가질 수 없다.

 5. allocate/deallocate 함수를 가지며, allocate 인자로는 바이트 수가 아닌, 필요한 (T) 객체의 개수를 매겨변수로 넘긴다.
 	또한, 이 함수는 void*가 아닌 pointer(T*)를 반환한다.

 6. 표준 컨테이너에서 필요로 하는 rebind라는 중첩(nested) 구조체 템플릿을 반드시 제공해야 한다.

 * std::allocator::allocate(n)			: n의 크기를 갖는 초기화 되지 않은 메모리블록 할당후 시작 주소 반환.
 * std::allocator::deallocate(p, n)		: p 부터 p+n 까지의 메모리블록을 해제.
 * std::allocator::construct(p, val)	: p가 가리키는 곳에 object val의 생성자를 대입
 * std::allocator::destroy(p)			: p가 가리키는 object의 소멸자 호출
 */
# include <vector>

namespace ft {

	template <typename T, class Alloc = std::allocator<T> >
	class vector
	{
		public:
			typedef T 					value_type;
			typedef value_type&			reference; // value_type& = T&
			typedef const value_type&	const_reference; // const value_type& = cont T&

			typedef Alloc	allocator_type;
			typedef typename allocator_type::template rebind<value_type>::other	type_allocator;
			typedef std::allocator_traits<type_allocator>		type_traits;
			typedef typename type_traits::pointer				pointer;
			typedef typename type_traits::const_pointer			const_pointer;

			typedef ft::random_access_iterator<value_type>				iterator;
			typedef ft::random_access_iterator<const value_type>		const_iterator;
			typedef ft::reverse_iterator<iterator>						reverse_iterator;
			typedef ft::reverse_iterator<const_iterator>				const_reverse_iterator;
			typedef typename std::ptrdiff_t		difference_type;
			typedef typename std::size_t		size_type;

		private:
			pointer			_begin;
			pointer			_end;
			pointer			_capacity;
			allocator_type	_alloc;

		public:
			explicit vector(const allocator_type& alloc = allocator_type())
				: _begin(NULL), _end(NULL), _capacity(NULL), _alloc(alloc) {}
			explicit vector(size_type n, const value_type& val = value_type(), const allocator_type& alloc = allocator_type())
				: _alloc(alloc)
			{
				_vector_init(n);
				_alloc_construct(n, val);
			}
			template <class InputIterator>
			vector(InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
				typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = ft::NIL) : _alloc(alloc)
			{
				size_type n = std::distance(first, last);
				_vector_init(n);
				_alloc_construct(n);
				std::copy(first, last, _begin);
			}
			vector(const vector& v) : _alloc(v._alloc)
			{
				size_type n = v.size();
				_vector_init(v.capacity());
				_alloc_construct(n);
				std::copy(v._begin, v._end, _begin);
			}
			~vector()
			{
				if (_begin == NULL)
					return;
				size_type x_capacity = capacity();
				_alloc_destroy(_begin);
				_alloc.deallocate(_begin, x_capacity);
			}

			vector& operator=(const vector& v)
			{
				if (this != &v)
					assign(v._begin, v._end);
				return *this;
			}

			iterator begin()						{ return iterator(_begin); }
			iterator end()							{ return iterator(_end); }
			const_iterator begin() const			{ return const_iterator(_begin); }
			const_iterator end() const				{ return const_iterator(_end); }
			reverse_iterator rbegin()				{ return reverse_iterator(end()); }
			reverse_iterator rend()					{ return reverse_iterator(begin()); }
			const_reverse_iterator rbegin() const	{ return const_reverse_iterator(end()); }
			const_reverse_iterator rend() const		{ return const_reveres_iterator(begin()); }

			/* element 접근자 */
			reference operator[](size_type n)				{ return _begin[n]; }
			const_reference operator[](size_type n) const	{ return _begin[n]; }

			/* operator[]와는 다르게 n 값이 element 범위를 넘어가는지 확인 후 throw 를 함. */
			reference at(size_type n)
			{
				if (n >= size())
					throw std::out_of_range("ft::vector::at");
				return _begin[n];
			}
			const_reference at(size_type n) const
			{
				if (n >= size())
					throw std::out_of_range("ft::vector::at");
				return _begin[n];
			}

			reference front()				{ return *_begin; }
			reference back() 				{ return *(_end - 1); }
			const_reference front() const	{ return *_begin; }
			const_reference back() const	{ return *(_end - 1); }

			// vector.data() 형이 오는 test가 있어 추가
			T* data() throw() 				{ return reinterpret_cast<T*>(_begin); }
			const T* data() const throw()	{ return reinterpret_cast<const T*>(_begin); }

			size_type size() const	{ return (_end - _begin); }
			// if (!vector.size() == static_cast<size_type>(-1)) 테스트문이 있음.
			// size_type size() const		{ return static_cast<size_type>(_end - _begin); }

			// type에 따른 allocator 의 max_size()와 std::numeric_limits<size_type>dml max() 중 작은 값 반환
			size_type max_size() const
			{
				return std::min<size_type>(std::numeric_limits<size_type>::max(),
					type_traits::max_size(type_allocator()));
			}
			// 인자로 받을 n의 크기가 size 보다 작으면 나머지 공간은 삭제
			// size 보다 크면 capacity와 비교 -> reserve로 늘릴지 판단 후 할당
			void resize(size_type n, value_type val = value_type())
			{
				if (size() > n)
				{
					size_type space = size() - n;
					_alloc_destroy(space);
				}
				else if (size() < n)
				{
					size_type space = n - size();
					if (capacity() < n)
						reserve(n);
					_alloc_construct(space, val);
				}
			}
			size_type capacity() const	{ return (_capacity - _begin); }
			// size_type capacity() const	{ return static_cast<size_type>(_capacity - _begin); }
			bool empty() const { return _begin == _end; }

			// reserve >> insert , assign , push_back 등 각 함수에서 n 만큼 받을 인자를 매개변수로 취함
			// 받아올 인자 n이, capacity < n < capacity * 2 일경우 capacity를 2배로 늘려주고 각 멤버 값을 정리 후 재할당.
			void reserve(size_type n)
			{
				if (n <= size() || n <= capacity())
					return;
				if (n < capacity() * 2)
				{
					if (n > size() * 2)
						;
					else
						n = capacity() * 2;
				}
				size_type x_size = size();
				size_type x_capacity = capacity();
				pointer _ptr = _alloc.allocate(n);
				std::uninitialized_copy(_begin, _end, _ptr);
				_alloc_destroy(_begin);
				_alloc.deallocate(_begin, x_capacity);
				_begin = _ptr;
				_end = _begin + x_size;
				_capacity = _begin + n;
			}
			// uninitialized_copy >> 메모리 할당을 객체 생성에서 분리.
			// container 저장소 안의 초기화 되지 않은 범위에 복사. parameter(first, last, dest)
			// reserve에서 capacity 보다 큰 값이 들어올경우 새로운 메모리 블럭을 지정해 기존을 해제하고
			// 할당시켜주기 때문에 밑에 assign 에서의 copy와 쓰임이 다름.

			template <class InputIterator>
			void assign(InputIterator first, InputIterator last,
				typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = ft::NIL)
			{
				size_type n = std::distance(first, last);
				if (capacity() < n)
					reserve(n);
				std::copy(first, last, _begin);
				_end = _begin + n;
			}
			void assign(size_type n, const value_type& val)
			{
				if (capacity() < n)
					reserve(n);
				std::fill_n(_begin, n, val);
				_end = _begin + n;
			}
			void push_back(const value_type& val)
			{
				size_type n = size() + 1;
				if (capacity() < n)
					reserve(n);
				_alloc_construct(1);
				*(_end - 1) = val;
			}
			void pop_back() { _alloc_destroy(1); }
			// copy 차이 copy_backward
			// 0 1 2 3 4 5 6 7 8 9
			// lst   2 3 4 5 6    을 해당 위치에 대응되게 복사
			// 0 1 2 2 2 2 2 2 8 9 copy
			// 0 1 2 2 3 4 5 6 8 9 copy_backward
			// copy 는 원본이 복사가 이루어지기 전에 변함.
			// copy_backward 는 원본을 먼저 옮겨두고 복사.
			iterator insert(iterator _idx, const value_type& val)
			{
				difference_type space = _idx - begin();
				if (capacity() < size() + 1)
					reserve(size() + 1);
				pointer ptr = _begin + space;
				_alloc_construct(1);
				std::copy_backward(ptr, _end - 1, _end);
				*ptr = val;
				return iterator(ptr);
			}
			void insert(iterator _idx, size_type n, const value_type& val)
			{
				difference_type space = _idx - begin();
				if (capacity() < size() + n)
					reserve(size() + n);
				pointer ptr = _begin + space;
				_alloc_construct(n);
				std::copy_backward(ptr, _end - n, _end);
				for (size_type i = 0 ; i < n ; i++)
					ptr[i] = val;
			}
			template <class InputIterator>
			void insert(iterator _idx, InputIterator first, InputIterator last,
					typename ft::enable_if<!ft::is_integral<InputIterator>::value>::type* = ft::NIL)
			{
				difference_type n = std::distance(first, last);
				difference_type space = _idx - begin();
				if (capacity() < size() + n)
					reserve(size() + n);
				pointer ptr = _begin + space;
				_alloc_construct(n);
				std::copy_backward(ptr, _end - n, _end);
				for (InputIterator i = first ; i != last ; i++, ptr++)
					*ptr = *i;
			}
			iterator erase(iterator _idx)
			{
				difference_type space = _idx - begin();
				pointer ptr = _begin + space;
				std::copy(ptr + 1, _end, ptr);
				_alloc_destroy(1);
				return iterator(ptr);
			}
			iterator erase(iterator first, iterator last)
			{
				difference_type n = std::distance(first, last);
				std::copy(last, end(), first);
				_alloc_destroy(n);
				return first;
			}
			void swap(vector& v)
			{
				std::swap(_begin, v._begin);
				std::swap(_end, v._end);
				std::swap(_capacity, v._capacity);
				std::swap(_alloc, v._alloc);
			}
			void clear() { _alloc_destroy(_begin); }

			allocator_type get_allocator() const { return _alloc; }

			void	_vector_init(size_type n)
			{
				if (n > max_size())
					{ throw std::length_error("ft::vector::_vector_init"); }
				_begin = _alloc.allocate(n);
				_end = _begin;
				_capacity = _begin + n;
			}

			void _alloc_construct(size_type n, T val)
			{
				for ( ; n > 0 ; _end++, n--)
				{
					_alloc.construct(_end);
					*_end = val;
				}
			}

			void _alloc_construct(size_type n)
			{
				for ( ; n > 0 ; _end++, n--)
					_alloc.construct(_end);
			}

			void _alloc_destroy(size_type n)
			{
				for ( ; n > 0 && _end-- ; n--)
					_alloc.destroy(_end);
			}

			void _alloc_destroy(pointer until)
			{
				for ( ; _end != until && _end-- ; )
					_alloc.destroy(_end);
			}
		};

			template <typename T, class Alloc>
			bool operator==(const ft::vector<T, Alloc>& x, const ft::vector<T, Alloc>& y)
			{ return x.size() == y.size() && ft::equal(x.begin(), x.end(), y.begin()); }

			template <typename T, class Alloc>
			bool operator!=(const ft::vector<T, Alloc>& x, const ft::vector<T, Alloc>& y)
			{ return !(x == y); }

			template <typename T, class Alloc>
			bool operator<(const ft::vector<T, Alloc>& x, const ft::vector<T, Alloc>& y)
			{ return ft::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end()); }

			template <typename T, class Alloc>
			bool operator>(const ft::vector<T, Alloc>& x, const ft::vector<T, Alloc>& y)
			{ return (y < x); }

			template <typename T, class Alloc>
			bool operator<=(const ft::vector<T, Alloc>& x, const ft::vector<T, Alloc>& y)
			{ return !(x > y); }

			template <typename T, class Alloc>
			bool operator>=(const ft::vector<T, Alloc>& x, const ft::vector<T, Alloc>& y)
			{ return !(x < y); }

			template <typename T, class Alloc>
			void swap(ft::vector<T, Alloc>& x, ft::vector<T, Alloc>& y)
			{ x.swap(y); }

}

#endif
