#ifndef REVERSE_ITERATOR_HPP
# define REVERSE_ITERATOR_HPP

# include "iterators_traits.hpp"

namespace ft {

	template <typename Iterator>
	class reverse_iterator {
		public:
			typedef Iterator												iterator_type;
			typedef typename iterator_traits<Iterator>::value_type			value_type;
			typedef typename iterator_traits<Iterator>::pointer				pointer;
			typedef typename iterator_traits<Iterator>::reference			reference;
			typedef typename iterator_traits<Iterator>::difference_type		difference_type;
			typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;

		private:
			iterator_type	__base_it;

		public:
			reverse_iterator() { iterator_type(); }

			reverse_iterator(iterator_type it) { __base_it = it; }

			template <typename Iter>
			reverse_iterator(const reverse_iterator<Iter>& __rev_it) {
				__base_it = __rev_it.__base_it;
			}

			reverse_iterator operator=(const reverse_iterator& rhs) {
				__base_it = rhs.__base_it;
				return *this;
			}

			~reverse_iterator() {}

			iterator_type		base() const { return __base_it; }
			reference			operator*() const { return *(__base_it - 1); }
			pointer				operator->() const { return &(operator*()); }
			reference			operator[](difference_type n) const { return base()[-n-1]; }

			reverse_iterator	operator+(difference_type n) const
			{ return reverse_iterator(__base_it - n); }
			reverse_iterator	operator-(difference_type n) const
			{ return reverse_iterator(__base_it + n); }

			reverse_iterator&	operator++()
			{
				--__base_it;
				return *this;
			}
			reverse_iterator	operator++(int)
			{
				reverse_iterator tmp = *this;
				++(*this);
				return tmp;
			}
			reverse_iterator&	operator--()
			{
				++__base_it;
				return *this;
			}
			reverse_iterator	operator--(int)
			{
				reverse_iterator tmp = *this;
				--(*this);
				return tmp;
			}
			reverse_iterator&	operator+=(difference_type n)
			{
				__base_it -= n;
				return *this;
			}
			reverse_iterator&	operator-=(difference_type n)
			{
				__base_it += n;
				return *this;
			}
	};

	template <typename Iter>
	bool operator==(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return lhs.base() == rhs.base(); }

	template <typename Iter>
	bool operator!=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return lhs.base() != rhs.base(); }

	template <typename Iter>
	bool operator>(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return lhs.base() > rhs.base(); }

	template <typename Iter>
	bool operator>=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return lhs.base() >= rhs.base(); }

	template <typename Iter>
	bool operator<(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return lhs.base() < rhs.base(); }

	template <typename Iter>
	bool operator<=(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return lhs.base() <= rhs.base(); }

	template <typename Iter>
	reverse_iterator<Iter> operator+(typename reverse_iterator<Iter>::difference n, const reverse_iterator<Iter>& rhs)
	{ return rhs + n; }

	template <typename Iter>
	typename reverse_iterator<Iter>::difference_type operator-(const reverse_iterator<Iter>& lhs, const reverse_iterator<Iter>& rhs)
	{ return rhs.base() - lhs.base(); }

}

#endif
