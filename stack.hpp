/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stack.hpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jeson <jeson@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/17 18:12:41 by jeson             #+#    #+#             */
/*   Updated: 2022/08/05 18:52:21 by jeson            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STACK_HPP
# define STACK_HPP

# include "vector.hpp"

namespace ft {

	template <typename T, typename Container = ft::vector<T> >
	class stack
	{
		public:
			typedef T			value_type;
			typedef Container	container_type;
			typedef size_t		size_type;

		protected:
			container_type	c;

		public:
			stack(const container_type& __con = container_type()): c(__con){}
			stack(const stack& rhs): c(rhs.c){}
			~stack(){}
			stack& operator=(const stack& rhs)
			{
				c = rhs.c;
				return *this;
			}

			bool				empty() const { return c.empty(); }
			size_type			size() const { return c.size(); }
			value_type			top() { return c.back(); }
			const value_type&	top() const {return c.back(); }
			void				push(const value_type& __val) { c.push_back(__val); }
			void				pop() { c.pop_back(); }
	};

	template <typename T, typename Con>
	bool operator==(const stack<T, Con>& lhs, const stack<T, Con>& rhs){ return (lhs == rhs); }

	template <typename T, typename Con>
	bool operator!=(const stack<T, Con>& lhs, const stack<T, Con>& rhs){ return (lhs != rhs); }

	template <typename T, typename Con>
	bool operator<(const stack<T, Con>& lhs, const stack<T, Con>& rhs){ return (lhs < rhs); }

	template <typename T, typename Con>
	bool operator<=(const stack<T, Con>& lhs, const stack<T, Con>& rhs){ return (lhs <= rhs); }

	template <typename T, typename Con>
	bool operator>(const stack<T, Con>& lhs, const stack<T, Con>& rhs){ return (lhs > rhs); }

	template <typename T, typename Con>
	bool operator>=(const stack<T, Con>& lhs, const stack<T, Con>& rhs){ return (lhs >= rhs); }


}

#endif
