#ifndef RBTREE_HPP
# define RBTREE_HPP

# include <algorithm>
# include <limits>
# include <memory>
# include "pair.hpp"
# include "iterator.hpp"
# include "type.hpp"

namespace ft {

	/* tree node 는 parent left right 노드의 포인터를 가지고 해당 노드의 value_type과 색상을 들고있음. */
	template <typename T>
	struct __tree_node
	{
		typedef T value_type;

		__tree_node*	__parent;
		__tree_node*	__left;
		__tree_node*	__right;
		value_type		__value;
		bool			__is_black;

		__tree_node(void) : __parent(ft::NIL), __left(ft::NIL), __right(ft::NIL), __value(value_type()), __is_black(bool()) {}
		__tree_node(const value_type& value) : __parent(ft::NIL), __left(ft::NIL), __right(ft::NIL), __value(value), __is_black(bool()) {}
		__tree_node(const __tree_node& n) : __parent(n.__parent), __left(n.__left), __right(n.__right), __value(n.__value), __is_black(n.__is_black) {}
		~__tree_node(void) {}

		__tree_node& operator=(const __tree_node& n)
		{
			if (this != &n)
			{
				__parent = n.__parent;
				__left = n.__left;
				__right = n.__right;
				__value = n.__value;
				__is_black = n.__is_black;
			}
			return *this;
		}
	};

	template <class NodePtr>
	bool __is_left_child(const NodePtr& ptr)
	{ return ptr == ptr->__parent->__left; }

	template <class NodePtr>
	bool __is_right_child(const NodePtr& ptr)
	{ return ptr == ptr->__parent->__right; }

	template <class NodePtr>
	bool __is_black_color(const NodePtr& ptr)
	{ return ptr->__is_black; }

	template <class NodePtr>
	bool __is_red_color(const NodePtr& ptr)
	{ return !ptr->__is_black; }

	/* 해당 노드 위치에서 가장 왼쪽으로 먼 거리의 노드(가장 작은) 포인터 반환 */
	template <class NodePtr>
	NodePtr __min_node(NodePtr ptr, NodePtr nil)
	{
		while (ptr->__left != nil)
			ptr = ptr->__left;
		return ptr;
	}

	/* 해당 노드 위치에서 가장 오른쪽으로 먼 거리의 노드 포인터 반환 */
	template <class NodePtr>
	NodePtr __max_node(NodePtr ptr, NodePtr nil)
	{
		while (ptr->__right != nil)
			ptr = ptr->__right;
		return ptr;
	}

	/* Red-Black Tree 중위 순회
	다음 노드 : 오른쪽 side 중 가장 가까운 곳에 위치한 노드 포인터 반환
	오른쪽 사이드로 이어지지 않으면 한계층 씩 올라가며 오른쪽 사이드로 빠질 수 있는 노드를 반환*/
	template <class NodePtr>
	NodePtr __next_node(NodePtr ptr, NodePtr nil)
	{
		if (ptr->__right != nil)
			return __min_node(ptr->__right, nil);
		while (!__is_left_child(ptr))
			ptr = ptr->__parent;
		return ptr->__parent;
	}

	/* 이전 노드 : 왼쪽 side 중 가장 가까운 곳에 위치한 노드 포인터 반환
	왼쪽 사이드로 이어지지 않으면 한계층 씩 올라가며 왼쪽 사이드로 빠질 수 있는 노드를 반환*/
	template <class NodePtr>
	NodePtr __prev_node(NodePtr ptr, NodePtr nil)
	{
		if (ptr->__left != nil)
			return __max_node(ptr->__left, nil);
		while (!__is_right_child(ptr))
			ptr = ptr->__parent;
		return ptr->__parent;
	}

	template <typename T, typename U, class Comp>
	bool __is_equal(const T& t, const U& u, Comp comp)
	{ return !comp(t, u) && !comp(u, t); }

	/* Red-Black Tree
	트리 노드를 가지고 이를 해당 자료구조의 이터레이터로 사용
	레드-블랙 트리 자료구조의 private 멤버는
	nil(leaf node)노드 포인터, begin 시작 포인터, end 끝 포인터, compare_type, node 전용 할당자와 size를 가진다.*/
	template <typename T, class Key, class Comp, class Alloc = std::allocator<T> >
	class __rbt{
		public:
			typedef T		value_type;
			typedef Key		key_type;
			typedef Comp	compare_type;

			typedef __tree_node<value_type>		node_type;
			typedef __tree_node<value_type>*	node_pointer;
			typedef ft::tree_iterator<value_type, node_type>			iterator;
			typedef ft::tree_iterator<const value_type, node_type>		const_iterator;

			typedef Alloc		allocator_type;
			typedef typename Alloc::template rebind<node_type>::other	node_allocator;
			typedef std::allocator_traits<node_allocator>				node_traits;

			typedef std::size_t		size_type;
			typedef std::ptrdiff_t	diffefence_type;

		private:
			node_pointer	__nil;
			node_pointer	__begin;
			node_pointer	__end;
			compare_type	__comp;
			node_allocator	__alloc;
			size_type		__size;

		public:
		/* 기본생성자 : leaf 노드에 1 만큼 할당 후 leaf 노드 포인터에 value타입으로 allocator::construct
		-> leaf 노드 포인터의 부모와 왼쪽 오른쪽을 전부 leaf 노드로 설정(root)
		-> root 노드는 black 색상을 가짐 end 포인터에 노드를 make_node 후 begin 포인터에 넣어줌
		-> make_node는 노드를 만드는 함수로써 __is_black이 false 값을 가지기때문에 true로 변환.*/
			__rbt(const compare_type& comp, const allocator_type& alloc)
			: __comp(comp), __alloc(alloc), __size(size_type())
			{
				__nil = __alloc.allocate(1);
				__alloc.construct(__nil, value_type());
				__nil->__parent = __nil;
				__nil->__left = __nil;
				__nil->__right = __nil;
				__nil->__is_black = true;
				__end = make_node(value_type());
				__end->__is_black = true;
				__begin = __end;
			}

			/* construct의 과정을 따르고 target rbt의 iterator begin()부터 end()까지 넣어줌 */
			__rbt(const __rbt& rbt)
			: __comp(rbt.__comp), __alloc(rbt.__alloc), __size(size_type())
			{
				__nil = __alloc.allocate(1);
				__alloc.construct(__nil, value_type());
				__nil->__parent = __nil;
				__nil->__left = __nil;
				__nil->__right = __nil;
				__nil->__is_black = true;
				__end = make_node(value_type());
				__end->__is_black = true;
				__begin = __end;
				insert(rbt.begin(), rbt.end());
			}

			/* 모든 노드 삭제 */
			~__rbt()
			{
				del_node_all(__end);
				del_node(__nil);
			}

			__rbt& operator=(const __rbt& rbt)
			{
				if (this != &rbt)
				{
					__rbt tmp(rbt);
					swap(tmp);
				}
				return *this;
			}

			/* iterator 설정 */
			iterator begin()				{ return iterator(__begin, __nil); }
			iterator end()					{ return iterator(__end, __nil); }
			const_iterator begin() const	{ return const_iterator(__begin, __nil); }
			const_iterator end() const		{ return const_iterator(__end, __nil); }

			/* max_size : 사이즈 타입의 max와 allocator::rebind value_type의 max_size 중 작은 값 반환 */
			size_type size() const	{ return __size; }
			size_type max_size() const
			{
				return std::min<size_type>(std::numeric_limits<size_type>::max(),
						node_traits::max_size(node_allocator()));
			}
			bool empty() const { return __size == 0; }

			/* 삽입함수 : 값을 가지고 있을 경우 pair<nd_ptr,false>를 반환
			삽입 가능할 경우 pair<__end,true> 반환 */
			ft::pair<iterator, bool> insert(const value_type& val)
			{
				node_pointer nd_ptr = __search_parent(val);
				if ( nd_ptr != __end && __is_equal(nd_ptr->__value, val, __comp))
					return ft::make_pair(iterator(nd_ptr, __nil), false);
				return ft::make_pair(iterator(__insert_tree(val, nd_ptr), __nil), true);
			}

			/* idx 이터레이터의 base 값과 val 값으로 parent로 올라가면서 값 비교
			반환값 iterator */
			iterator	insert(iterator _idx, const value_type& val)
			{
				node_pointer nd_ptr = __search_parent(val, _idx.base());
				if (nd_ptr != __end && __is_equal(nd_ptr->__value, val, __comp))
					return iterator(nd_ptr, __nil);
				return iterator(__insert_tree(val, nd_ptr), __nil);
			}

			/* iterator 범위 삽입 */
			template <typename InputIterator>
			void	insert(InputIterator first, InputIterator last)
			{
				for ( ; first != last ; first++)
					insert(*first);
			}

			/* idx iterator 매개변수를 받아 erase
			empty인 경우 nullptr을 갖는 이터레이터 반환
			다음 위치의 노드포인터를 반환
			트리 안의 해당 idx iterator element 를 지우고 tree를 재배치 함 */
			iterator	erase(iterator _idx)
			{
				if (__size == 0)
					return iterator(__nil, __nil);
				iterator tmp(_idx);
				++tmp;
				if (_idx == begin())
					__begin = tmp.base();
				--__size;
				__remove(_idx.base());
				del_node(_idx.base());
				return tmp;
			}

			/* val이 없을 경우 0(false), 있을경우 지우고 tree 재배열 후 1(true) 반환 */
			size_type	erase(const key_type& val)
			{
				iterator it(__find(val), __nil);
				if (it == end())
					return 0;
				if (it == begin())
				{
					iterator tmp(it);
					++tmp;
					__begin = tmp.base();
				}
				--__size;
				__remove(it.base());
				del_node(it.base());
				return 1;
			}

			/* first iterator ~ last iterator 반복문 돌면서 erase */
			void	erase(iterator first, iterator last)
			{
				for ( ; first != last ;)
					first = erase(first);
			}

			void	swap(__rbt& rbt)
			{
				std::swap(__nil, rbt.__nil);
				std::swap(__begin, rbt.__begin);
				std::swap(__end, rbt.__end);
				std::swap(__comp, rbt.__comp);
				std::swap(__alloc, rbt.__alloc);
				std::swap(__size, rbt.__size);
			}

			/* C++ STL 에선 메모리를 완전히 해제하기 위해 swap을 이용해 기본생성자를 호출한 임시의 컨테이너 객체를 이용
			해당 지역에서의 호출이 종료되면 tmp에 옮겨진 tree는 삭제된다. */
			void	clear()
			{
				__rbt tmp(__comp, __alloc);
				swap(tmp);
			}

			iterator	find(const key_type& key)
			{ return iterator(__find(key), __nil); }

			const_iterator	find(const key_type& key) const
			{ return const_iterator(__find(key), __nil); }

			/* lower_bound k : 오른쪽 원소 중 k와 같거나 큰 값 중 가장 왼쪽의 iterator */
			iterator	lower_bound(const key_type& key)
			{ return iterator(__lower_bound(key), __nil); }

			const_iterator	lower_bound(const key_type& key) const
			{ return const_iterator(__lower_bound(key), __nil); }

			/* upper_bound k : 오른쪽 원소 중 k보다 큰 값 중 가장 왼쪽의 iterator */
			iterator upper_bound(const key_type& key)
			{ return iterator(__upper_bound(key), __nil); }

			const_iterator upper_bound(const key_type& key) const
			{ return const_iterator(__upper_bound(key), __nil); }

			/* pair<lower_bound iterator , upper_bound iterator> 객체로 반환*/
			ft::pair<iterator, iterator> equal_range(const key_type& key)
			{ return ft::make_pair(lower_bound(key), upper_bound(key)); }

			ft::pair<const_iterator, const_iterator> equal_range(const key_type& key) const
			{ return ft::make_pair(lower_bound(key), upper_bound(key)); }

			allocator_type get_allocator() const
			{ return __alloc; }

			node_pointer	getRoot() const
			{ return __end->__left; }

			/* 해당 노드 포인터를 root 로 설정 */
			void	setRoot(const node_pointer nd_ptr)
			{
				nd_ptr->__parent = __end;
				__end->__left = nd_ptr;
			}

			/* node 생성 함수 : 포인터로 생성하며 레드-블랙 트리에서 삽입될 노드의 색상은 반드시 red로 시작 */
			node_pointer	make_node(const value_type& val)
			{
				node_pointer nd_ptr = __alloc.allocate(1);
				__alloc.construct(nd_ptr, val);
				nd_ptr->__parent = __nil;
				nd_ptr->__left = __nil;
				nd_ptr->__right = __nil;
				nd_ptr->__is_black = false;
				return nd_ptr;
			}

			void	del_node(node_pointer nd_ptr)
			{
				__alloc.destroy(nd_ptr);
				__alloc.deallocate(nd_ptr, 1);
			}

			/* 재귀 반복문으로 Tree 내 모든 노드 삭제 및 해제 */
			void	del_node_all(node_pointer nd_ptr)
			{
				if (nd_ptr == __nil)
					return;
				del_node_all(nd_ptr->__left);
				del_node_all(nd_ptr->__right);
				del_node(nd_ptr);
			}

			/* _idx(hint) 포인터 위치부터 val 비교 탐색 */
			node_pointer	__search_parent(const value_type& val, node_pointer _idx = ft::NIL)
			{
				/* _idx 위치의 iterator + 1 과 - 1 비교 후 부모를 찾으면 해당 포인터 리턴 */
				if (_idx && _idx != __end)
				{
					if (__comp(val, _idx->__value) && _idx->__left == __nil)
					{
						iterator it_prev = iterator(_idx, __nil);
						if (it_prev == begin() || __comp(*--it_prev, val))
							return _idx;
					}
					else if (_idx->__right == __nil)
					{
						iterator it_next = iterator(_idx, __nil);
						if (it_next == end() || __comp(val, *++it_next))
							return _idx;
					}
				}
				/* insert 해야할 위치 root 노드 설정(left or right) 후 root 노드 반환
				같을경우 root가 노드로 설정된 상태로만 반환 */
				node_pointer cur = getRoot();
				node_pointer tmp = __end;
				for ( ; cur != __nil ; )
				{
					tmp = cur;
					if (__comp(val, cur->__value))
						cur = cur->__left;
					else if (__comp(cur->__value, val))
						cur = cur->__right;
					else
						return cur;
				}
				/* 루트 노드의 leaf가 비어있을경우 그대로 반환 */
				return tmp;
			}
			/* parent value보다 작으면 왼쪽 크면 오른쪽 삽입
			-> 생성한 노드를 각 값들로 초기화
			-> 해당 노드를 삽입 후 Tree 재정렬 */
			node_pointer	__insert_tree(const value_type& val, node_pointer parent)
			{
				node_pointer nd_ptr = make_node(val);
				if (parent == __end)
					setRoot(nd_ptr);
				else if (__comp(val, parent->__value))
					parent->__left = nd_ptr;
				else
					parent->__right = nd_ptr;
				nd_ptr->__parent = parent;
				__insert_fix(nd_ptr);
				__insert_init(nd_ptr);
				return nd_ptr;
			}
			/* 부모 컬러가 red 일 경우 !! double red !!
			-> parent 노드의 위치를 보고 fix_left or fix_right 실행
			-> root노드의 색상을 black으로 설정*/
			void	__insert_fix(node_pointer nd_ptr)
			{
				while (__is_red_color(nd_ptr->__parent))
				{
					if (__is_left_child(nd_ptr->__parent))
					{
						node_pointer uncle = nd_ptr->__parent->__parent->__right;
						if (__is_red_color(uncle))
							__recoloring(nd_ptr, uncle);
						else
							__restructing_left(nd_ptr);
					}
					else
					{
						node_pointer uncle = nd_ptr->__parent->__parent->__left;
						if (__is_red_color(uncle))
							__recoloring(nd_ptr, uncle);
						else
							__restructing_right(nd_ptr);
					}
				}
				getRoot()->__is_black = true;
			}

			void	__recoloring(node_pointer& nd_ptr, node_pointer& uncle)
			{
				nd_ptr->__parent->__is_black = true;
				uncle->__is_black = true;
				uncle->__parent->__is_black = false;
				nd_ptr = uncle->__parent;
			}

			void	__restructing_left(node_pointer nd_ptr)
			{
				if (__is_right_child(nd_ptr))
				{
					nd_ptr = nd_ptr->__parent;
					__rot_left(nd_ptr);
				}
				nd_ptr->__parent->__is_black = true;
				nd_ptr->__parent->__parent->__is_black = false;
				__rot_right(nd_ptr->__parent->__parent);
			}

			void	__restructing_right(node_pointer nd_ptr)
			{
				if (__is_left_child(nd_ptr))
				{
					nd_ptr = nd_ptr->__parent;
					__rot_right(nd_ptr);
				}
				nd_ptr->__parent->__is_black = true;
				nd_ptr->__parent->__parent->__is_black = false;
				__rot_left(nd_ptr->__parent->__parent);
			}

			// /* insert로 변화된 트리 초기값 설정 */
			void	__insert_init(const node_pointer nd_ptr)
			{
				if (__begin == __end || __comp(nd_ptr->__value, __begin->__value))
					__begin = nd_ptr;
				__size++;
			}
			/* recoloring 노드 포인터설정 매개변수로 받은 노드 포인터를 nd_fixup으로 설정
			nd_ptr : 삭제될 노드 포인터
			nd_fixup : 옮겨 심어질 노드 포인터
			nd_recoloring : recoloring을 고려할 노드 포인터 */
			void	__remove(node_pointer nd_ptr)
			{
				node_pointer nd_recoloring;
				node_pointer nd_fixup = nd_ptr;
				bool origin_color = __is_black_color(nd_ptr);
				/* nd_ptr -> left leaf만 비어있을 경우 nd_ptr->right를 옮겨 link됨 */
				if (nd_ptr->__left == __nil)
				{
					nd_recoloring = nd_ptr->__right;
					__link_parent_child(nd_ptr, nd_ptr->__right);
				}
				/* nd_ptr -> right leaf만 비어있을 경우 */
				else if (nd_ptr->__right == __nil)
				{
					nd_recoloring = nd_ptr->__left;
					__link_parent_child(nd_ptr, nd_ptr->__left);
				}
				/* left & right 모두 있을 경우 */
				else
				{
					/* tree 특성상 해당 위치의 오른쪽 min_node가 가장 작음. */
					nd_fixup = __min_node(nd_ptr->__right, __nil);
					origin_color = __is_black_color(nd_fixup);
					/* nd_fixup은 left가 없으므로 옮길때 고려할 nd_recoloring은 right */
					nd_recoloring = nd_fixup->__right;
					if (nd_fixup->__parent == nd_ptr)
						nd_recoloring->__parent = nd_fixup;
					else
					{
						__link_parent_child(nd_fixup, nd_fixup->__right);
						nd_fixup->__right = nd_ptr->__right;
						nd_fixup->__right->__parent = nd_fixup;
					}
					/* 옮겨 심어질 경우 right는 연결이 됨. left 연결 시켜줌. */
					__link_parent_child(nd_ptr, nd_fixup);
					nd_fixup->__left = nd_ptr->__left;
					nd_fixup->__left->__parent = nd_fixup;
					nd_fixup->__is_black = __is_black_color(nd_ptr);
				}
				/* 삭제한 노드가 red 색상이면 그냥 지워도 상관 없음. */
				if (origin_color)
					__remove_fixup(nd_recoloring);
			}
			/* nd_ptr이 루트노드가 아니고 black 일 경우 fixup 반복문 */
			void	__remove_fixup(node_pointer nd_ptr)
			{
				while (nd_ptr != getRoot() && __is_black_color(nd_ptr))
				{
					if (__is_left_child(nd_ptr))
						__remove_fixup_left(nd_ptr);
					else
						__remove_fixup_right(nd_ptr);
				}
				nd_ptr->__is_black = true;
			}

			void	__remove_fixup_left(node_pointer& nd_ptr)
			{
				node_pointer sibling = nd_ptr->__parent->__right;
				if (__is_red_color(sibling))
				{
					sibling->__is_black = true;
					nd_ptr->__parent->__is_black = false;
					__rot_left(nd_ptr->__parent);
					sibling = nd_ptr->__parent->__right;
				}
				if (__is_black_color(sibling->__left) && __is_black_color(sibling->__right))
				{
					sibling->__is_black = false;
					nd_ptr = nd_ptr->__parent;
				}
				else if (__is_black_color(sibling->__right))
				{
					sibling->__left->__is_black = true;
					sibling->__is_black = false;
					__rot_right(sibling);
					sibling = nd_ptr->__parent->__right;
				}
				if (__is_red_color(sibling->__right))
				{
					sibling->__is_black = __is_black_color(nd_ptr->__parent);
					nd_ptr->__parent->__is_black = true;
					sibling->__right->__is_black = true;
					__rot_left(nd_ptr->__parent);
					nd_ptr = getRoot();
				}
			}
			void	__remove_fixup_right(node_pointer& nd_ptr)
			{
				node_pointer sibling = nd_ptr->__parent->__left;
				if (__is_red_color(sibling))
				{
					sibling->__is_black = true;
					nd_ptr->__parent->__is_black = false;
					__rot_right(nd_ptr->__parent);
					sibling = nd_ptr->__parent->__left;
				}
				if (__is_black_color(sibling->__right) && __is_black_color(sibling->__left))
				{
					sibling->__is_black = false;
					nd_ptr = nd_ptr->__parent;
				}
				else if (__is_black_color(sibling->__left))
				{
					sibling->__right->__is_black = true;
					sibling->__is_black = false;
					__rot_left(sibling);
					sibling = nd_ptr->__parent->__left;
				}
				if (__is_red_color(sibling->__left))
				{
					sibling->__is_black = __is_black_color(nd_ptr->__parent);
					nd_ptr->__parent->__is_black = true;
					sibling->__left->__is_black = true;
					__rot_right(nd_ptr->__parent);
					nd_ptr = getRoot();
				}
			}

			/* 삭제할 노드의 부모가 end 노드일 경우와 아닐경우의 link 함수 */
			void	__link_parent_child(node_pointer before, node_pointer after)
			{
				/* __end 일 경우 오른편에 존재하는 노드나 leaf 가 없기때문에 setRoot */
				if (before->__parent == __end)
					setRoot(after);
				/* 아닐경우 직접 옮김 */
				else if (__is_left_child(before))
					before->__parent->__left = after;
				else
					before->__parent->__right = after;
				/* 삭제 이전의 부모와 link */
				after->__parent = before->__parent;
			}

			void	__rot_left(node_pointer nd_ptr)
			{
				node_pointer ch = nd_ptr->__right;
				nd_ptr->__right = ch->__left;
				if (nd_ptr->__right != __nil)
					nd_ptr->__right->__parent = nd_ptr;
				node_pointer parent = nd_ptr->__parent;
				ch->__parent = parent;
				if (parent == __end)
					setRoot(ch);
				else if (__is_left_child(nd_ptr))
					parent->__left = ch;
				else
					parent->__right = ch;
				ch->__left = nd_ptr;
				nd_ptr->__parent = ch;
			}
			void	__rot_right(node_pointer nd_ptr)
			{
				node_pointer ch = nd_ptr->__left;
				nd_ptr->__left = ch->__right;
				if (nd_ptr->__left != __nil)
					nd_ptr->__left->__parent = nd_ptr;
				node_pointer parent = nd_ptr->__parent;
				ch->__parent = parent;
				if (parent == __end)
					setRoot(ch);
				else if (__is_left_child(nd_ptr))
					parent->__left = ch;
				else
					parent->__right = ch;
				ch->__right = nd_ptr;
				nd_ptr->__parent = ch;
			}

			node_pointer __find(const key_type& val) const
			{
				node_pointer nd_ptr = getRoot();
				while (nd_ptr != __nil)
				{
					if (__comp(val, nd_ptr->__value))
						nd_ptr = nd_ptr->__left;
					else if (__comp(nd_ptr->__value, val))
						nd_ptr = nd_ptr->__right;
					else
						return nd_ptr;
				}
				return __end;
			}

			node_pointer	__lower_bound(const key_type& key) const
			{
				node_pointer nd_ptr = getRoot();
				node_pointer tmp = __end;
				while (nd_ptr != __nil)
				{
					if (!__comp(nd_ptr->__value, key))
					{
						tmp = nd_ptr;
						nd_ptr = nd_ptr->__left;
					}
					else
						nd_ptr = nd_ptr->__right;
				}
				return tmp;
			}
			node_pointer	__upper_bound(const key_type& key) const
			{
				node_pointer nd_ptr = getRoot();
				node_pointer tmp = __end;
				while (nd_ptr != __nil)
				{
					if (__comp(key, nd_ptr->__value))
					{
						tmp = nd_ptr;
						nd_ptr = nd_ptr->__left;
					}
					else
						nd_ptr = nd_ptr->__right;
				}
				return tmp;
			}

	};

}
#endif
