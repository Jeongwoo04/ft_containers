#ifndef is_integral_HPP
# define is_integral_HPP

namespace ft {

	struct true_type {
		static const bool value = true;
		operator bool() const { return value;}
	};

	struct false_type {
		static const bool value = false;
		operator bool() { return value;}
	};


	template <typename T>
	struct remove_cv 					{ typedef T	type;};

	template <typename T>
	struct remove_cv<const T>			{ typedef T	type;};

	template <typename T>
	struct remove_cv<volatile T>		{ typedef T	type;};

	template <typename T>
	struct remove_cv<const volatile T>	{ typedef T	type;};


	template <typename T>
	struct is_integral_helper : public false_type { };

	template <>
	struct is_integral_helper<bool> : public true_type { };

	template <>
	struct is_integral_helper<char> : public true_type { };

	#if 0
	template <>
	struct is_integral_helper<char16_t> : public true_type { };

	template <>
	struct is_integral_helper<char32_t> : public true_type { };
	#endif

	template <>
	struct is_integral_helper<wchar_t> : public true_type { };

	template <>
	struct is_integral_helper<signed char> : public true_type { };

	template <>
	struct is_integral_helper<short int> : public true_type { };

	template <>
	struct is_integral_helper<int> : public true_type { };

	template <>
	struct is_integral_helper<long int> : public true_type { };

	template <>
	struct is_integral_helper<long long int> : public true_type { };

	template <>
	struct is_integral_helper<unsigned char> : public true_type { };

	template <>
	struct is_integral_helper<unsigned short int> : public true_type { };

	template <>
	struct is_integral_helper<unsigned int> : public true_type { };

	template <>
	struct is_integral_helper<unsigned long int> : public true_type { };

	template <>
	struct is_integral_helper<unsigned long long int> : public true_type { };


	template <typename T>
	struct is_integral : public is_integral_helper< typename remove_cv<T>::type > { };


}

#endif
