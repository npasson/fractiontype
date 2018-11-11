/*==================================================================================*\
 *
 *   Fraction Type
 *   Copyright (C) 2018  Nicholas Passon
 *   Documentation: http://www.npasson.com/fractiontype
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Affero General Public License as published
 *   by the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Affero General Public License for more details.
 *
 *   You should have received a copy of the GNU Affero General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
\*==================================================================================*/

/**
 * \file fraction.hpp
 * Contains the Fraction type.
 */

#ifndef NPASSON_FRACTION_HPP
#define NPASSON_FRACTION_HPP

#if __cplusplus > 201703L
#define NPASSON_IF_CONSTEXPR if constexpr
#else
#define NPASSON_IF_CONSTEXPR if
#endif

#if defined(__GNUC__) || defined(__MINGW32__) || defined(__clang__)
#define NPASSON_MAYBE_UNUSED __attribute__((unused))
#else
#define NPASSON_MAYBE_UNUSED
#endif

#include <string>
#include <typeinfo>

#ifdef NPASSON_DEBUG
#include <cstring>
#include <cxxabi.h>
#endif

#define INVALID_FRACTION Fraction(false)

namespace npasson {

	/**
	 *  The Fraction type. Read more at npasson.com/fractiontype
	 */
	class Fraction {

	private:
		                 long long signed int numerator = 0;
		                 long long signed int denominator = 1;
		                 bool _invalid = false;
		          static long long signed int gcd(long long signed int, long long signed int);
		          static long long signed int lcm(long long signed int, long long signed int);
		constexpr static bool isdigit(char);
		constexpr static bool isdelim(char);
		          static bool isnumber(std::string);
	public:
		/**
		 * @tparam T A numeric type which might be supported.
		 * @return If <tt>T</tt> is supported by the Fraction class.
		 */
		template <typename T>
		static constexpr bool is_supported_type() {
			return std::is_same<T, long long int>::value
			    || std::is_same<T, long int>::value
			    || std::is_same<T, int>::value
			    || std::is_same<T, short>::value
			    || std::is_same<T, unsigned long long int>::value
			    || std::is_same<T, unsigned long int>::value
			    || std::is_same<T, unsigned int>::value
			    || std::is_same<T, unsigned short>::value
			    || std::is_same<T, float>::value
			    || std::is_same<T, double>::value;
		}

		Fraction();
		Fraction(const Fraction&);
		Fraction(long long signed int, long long signed int);
		explicit Fraction(unsigned long long int);
		         Fraction(signed long long int); // NOLINT
		explicit Fraction(unsigned long int);
		explicit Fraction(signed long int);
		explicit Fraction(unsigned int);
		         Fraction(signed int); // NOLINT
		explicit Fraction(unsigned short);
		explicit Fraction(signed short);
		explicit Fraction(std::string);
		explicit Fraction(float);
		         Fraction(double); // NOLINT
		explicit Fraction(long double);
		explicit Fraction(const char*);
		explicit Fraction(char*);
		explicit Fraction(bool);

		~Fraction();

		NPASSON_MAYBE_UNUSED bool valid() const;

		explicit operator long long int() const;
		explicit operator long int() const;
		explicit operator int() const;
		explicit operator short() const;
		explicit operator float() const;
		explicit operator double() const;
		explicit operator bool () const;
		std::string str() const;
		NPASSON_MAYBE_UNUSED const char* c_str() const;
		NPASSON_MAYBE_UNUSED std::string f_str() const;

		/* ********************* OPERATOR OVERLOADINGS ******************** *
		 * **************************************************************** */

		/* *** PLUS *** */
		Fraction& operator += (const Fraction&);
		Fraction  operator +  (const Fraction&) const;

		template<typename T>
		Fraction& operator+=(const T &rhs) {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator += (Fraction, [type])");
			NPASSON_IF_CONSTEXPR (typeid(T) == typeid(long long int)
			                   || typeid(T) == typeid(long int)
			                   || typeid(T) == typeid(int)
			                   || typeid(T) == typeid(short))
			{
				return ((*this) = Fraction((numerator + rhs*denominator),denominator));
			}
			else NPASSON_IF_CONSTEXPR (typeid(T) == typeid(float)
			                        || typeid(T) == typeid(double))
			{
				Fraction rhsf(rhs);
				long long int num, den;
				num = (numerator*rhsf.denominator + rhsf.numerator*denominator);
				den = denominator*rhsf.denominator;
				(*this) = Fraction(num,den);
				return *this;
			}
			else return ((*this) = INVALID_FRACTION);
		}

		template<typename T>
		Fraction  operator+(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator + (Fraction, [type])");
			Fraction temp = (*this);
			return temp += rhs;
		}

		/* *** MINUS *** */
		Fraction& operator -= (const Fraction&);
		Fraction  operator -  (const Fraction&) const;

		template<typename T>
		Fraction& operator-=(const T &rhs) {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator -= (Fraction, [type])");
			NPASSON_IF_CONSTEXPR (typeid(T) == typeid(long long int)
			                   || typeid(T) == typeid(long int)
			                   || typeid(T) == typeid(int)
			                   || typeid(T) == typeid(short))
			{
				return ((*this) = Fraction((numerator - rhs*denominator),denominator));
			}
			else NPASSON_IF_CONSTEXPR (typeid(T) == typeid(float)
			                        || typeid(T) == typeid(double))
			{
				Fraction rhsf(rhs);
				long long int num, den;
				num = (numerator*rhsf.denominator - rhsf.numerator*denominator);
				den = denominator*rhsf.denominator;
				(*this) = Fraction(num,den);
				return *this;
			}
			else return ((*this) = INVALID_FRACTION);
		}

		template<typename T>
		Fraction operator-(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator - (Fraction, [type])");
			Fraction temp = (*this);
			return temp -= rhs;
		}

		/* *** MULTIPLICATION *** */
		Fraction& operator *= (const Fraction&);
		Fraction  operator *  (const Fraction&) const;

		template<typename T>
		Fraction& operator*=(const T &rhs) {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator *= (Fraction, [type])");
			NPASSON_IF_CONSTEXPR (typeid(T) == typeid(long long int)
			                   || typeid(T) == typeid(long int)
			                   || typeid(T) == typeid(int)
			                   || typeid(T) == typeid(short))
			{
				return ((*this) = Fraction(numerator * rhs, denominator));
			}
			else NPASSON_IF_CONSTEXPR (typeid(T) == typeid(float)
			                        || typeid(T) == typeid(double))
			{
				Fraction rhsf(rhs);
				return ((*this) = Fraction(
					numerator * rhsf.numerator,
					denominator * rhsf.denominator
				));
			}
			else return ((*this) = INVALID_FRACTION);
		}

		template<typename T>
		Fraction  operator*(const T &rhs) const {
            static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator * (Fraction, [type])");
			Fraction temp = (*this);
			return temp *= rhs;
		}

		/* *** DIVISION *** */
		Fraction& operator /= (const Fraction&);
		Fraction  operator /  (const Fraction&) const;

		template<typename T>
		Fraction& operator/=(const T &rhs) {
            static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator /= (Fraction, [type])");
			NPASSON_IF_CONSTEXPR (typeid(T) == typeid(long long int)
			                   || typeid(T) == typeid(long int)
			                   || typeid(T) == typeid(int)
			                   || typeid(T) == typeid(short))
			{
				return ((*this) = Fraction(numerator, denominator * rhs));
			}
			else NPASSON_IF_CONSTEXPR (typeid(T) == typeid(float)
			                        || typeid(T) == typeid(double))
			{
				Fraction rhsf(rhs);
				return ((*this) = Fraction(
					numerator * rhsf.denominator,
					denominator * rhsf.numerator
				));
			}
			else return ((*this) = INVALID_FRACTION);
		}

		template<typename T>
		Fraction  operator/(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator / (Fraction, [type])");
			Fraction temp = (*this);
			return temp /= rhs;
		}

		/* *** INCREMENT, DECREMENT, UNARY PLUS/MINUS *** */
		      Fraction& operator ++ ();
		const Fraction  operator ++ (int);
		      Fraction& operator -- ();
		const Fraction  operator -- (int);
		      Fraction  operator +  ()    const;
		      Fraction  operator -  ()    const;

		/* *** ASSIGNMENT TO FRACTION *** */
		
		template<typename T>
		inline Fraction& operator=(const T &rhs) {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator = (Fraction, [type])");
			(*this) = Fraction(rhs);
			return (*this);
		}

		/* *** COMPARISON *** */

		bool operator==(const Fraction&) const;
		bool operator!=(const Fraction&) const;
		bool operator< (const Fraction&) const;
		bool operator> (const Fraction&) const;
		bool operator<=(const Fraction&) const;
		bool operator>=(const Fraction&) const;

		template <typename T>
		bool operator==(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator == (Fraction, [type])");
			return (*this) == Fraction(rhs);
		}

		template <typename T>
		bool operator!=(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator != (Fraction, [type])");
			return (*this) != Fraction(rhs);
		}

		template <typename T>
		bool operator<(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator < (Fraction, [type])");
			return (*this) < Fraction(rhs);
		}

		template <typename T>
		bool operator>(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator > (Fraction, [type])");
			return (*this) > Fraction(rhs);
		}

		template <typename T>
		bool operator<=(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator <= (Fraction, [type])");
			return (*this) <= Fraction(rhs);
		}

		template <typename T>
		bool operator>=(const T &rhs) const {
			static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator >= (Fraction, [type])");
			return (*this) >= Fraction(rhs);
		}

		/* *** OTHER OPERATORS *** */

		//TODO root()
		Fraction pow(signed int);

		Fraction invert() const;
		NPASSON_MAYBE_UNUSED static void invert(Fraction&);

		friend std::ostream& operator << (std::ostream&, const Fraction&);
		std::string operator()();

#ifdef NPASSON_DEBUG
		template<typename T, typename U>
		static void test (T& a, U& b) {
			printf("==========================\n");
			printf("|   Operator test run    |\n");
			printf("==========================\n");
			printf("| a is  %6.6g of type %s\n", (double)a, abi::__cxa_demangle(typeid(a).name(),NULL,NULL,0));
			printf("| b is  %6.6g of type %s\n", (double)b, abi::__cxa_demangle(typeid(b).name(),NULL,NULL,0));
			printf("| a+b is  %6.6g of type %s\n", (double)(a+b), abi::__cxa_demangle(typeid(a+b).name(),NULL,NULL,0));
			printf("| b+a is  %6.6g of type %s\n", (double)(b+a), abi::__cxa_demangle(typeid(b+a).name(),NULL,NULL,0));
			printf("| a-b is  %6.6g of type %s\n", (double)(a-b), abi::__cxa_demangle(typeid(a-b).name(),NULL,NULL,0));
			printf("| b-a is  %6.6g of type %s\n", (double)(b-a), abi::__cxa_demangle(typeid(b-a).name(),NULL,NULL,0));
			printf("| a*b is  %6.6g of type %s\n", (double)(a*b), abi::__cxa_demangle(typeid(a*b).name(),NULL,NULL,0));
			printf("| b*a is  %6.6g of type %s\n", (double)(b*a), abi::__cxa_demangle(typeid(b*a).name(),NULL,NULL,0));
			printf("| a/b is  %6.6g of type %s\n", (double)(a/b), abi::__cxa_demangle(typeid(a/b).name(),NULL,NULL,0));
			printf("| b/a is  %6.6g of type %s\n", (double)(b/a), abi::__cxa_demangle(typeid(b/a).name(),NULL,NULL,0));
			printf("==========================\n");
			printf("| (operators are reset before\n");
			printf("|  each following operation)\n");
			printf("--------------------------\n");
			T at;
			U bt;
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("| a+=b is    %6.6g of type %s\n", (double)(at+=bt), abi::__cxa_demangle(typeid(at).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| b+=a is    %6.6g of type %s\n", (double)(bt+=at), abi::__cxa_demangle(typeid(bt).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| a-=b is    %6.6g of type %s\n", (double)(at-=bt), abi::__cxa_demangle(typeid(at).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| b-=a is    %6.6g of type %s\n", (double)(bt-=at), abi::__cxa_demangle(typeid(bt).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| a*=b is    %6.6g of type %s\n", (double)(at*=bt), abi::__cxa_demangle(typeid(at).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| b*=a is    %6.6g of type %s\n", (double)(bt*=at), abi::__cxa_demangle(typeid(bt).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt,&b,sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| a/=b is    %6.6g of type %s\n", (double)(at/=bt), abi::__cxa_demangle(typeid(at).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("| b/=a is    %6.6g of type %s\n", (double)(bt/=at), abi::__cxa_demangle(typeid(bt).name(),NULL,NULL,0));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("---------=RESET=----------\n");
			std::memmove(&at, &a, sizeof(a));
			std::memmove(&bt, &b, sizeof(b));
			printf("|   a is now %6.6g\n", (double)(at));
			printf("|   b is now %6.6g\n", (double)(bt));
			printf("==========================\n");
			printf("| Comparison\n");
			printf("| a is  %6.6g\n", (double)a);
			printf("| b is  %6.6g\n", (double)b);
			printf("| a<b is %s\n", (a<b)?"true":"false");
			printf("| a>b is %s\n", (a>b)?"true":"false");
			printf("| a<=b is %s\n", (a<=b)?"true":"false");
			printf("| a>=b is %s\n", (a>=b)?"true":"false");
			printf("| a==b is %s\n", (a==b)?"true":"false");
			printf("| a!=b is %s\n", (a!=b)?"true":"false");
			printf("==========================\n");
		}
#endif
	};

	/* *** Addition *** */
	template<typename T>
	T& operator += (T &lhs, const Fraction& rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator += ([type], Fraction)");
		return (lhs = (T)(rhs + lhs));
	}

	template <typename T>
	Fraction operator + (const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator + ([type], Fraction)");
		return Fraction(lhs) + rhs;
	}

	/* *** Subtraction *** */
	template<typename T>
	T& operator -= (T &lhs, const Fraction& rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator -= ([type], Fraction)");
		return (lhs = (T)(-rhs + lhs));
	}

	template <typename T>
	Fraction operator - (const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator - ([type], Fraction)");
		return Fraction(lhs) - rhs;
	}

	/* *** Multiplication *** */
	template<typename T>
	T& operator *= (T &lhs, const Fraction& rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator *= ([type], Fraction)");
		return (lhs = (T)(rhs * lhs));
	}

	template <typename T>
	Fraction operator * (const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator * ([type], Fraction)");
		return Fraction(lhs) * rhs;
	}

	/* *** Division *** */
	template<typename T>
	T& operator /= (T &lhs, const Fraction& rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator /= ([type], Fraction)");
		return (lhs = (T)((rhs / lhs).invert()));
	}

	template <typename T>
	Fraction operator / (const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator / ([type], Fraction)");
		return Fraction(lhs) / rhs;
	}

	/* *** COMPARISONS *** */

	template <typename T>
	bool operator==(const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator == (Fraction, [type])");
		return rhs == Fraction(lhs);
	}

	template <typename T>
	bool operator!=(const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator != (Fraction, [type])");
		return rhs != Fraction(lhs);
	}

	template <typename T>
	bool operator<(const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator < (Fraction, [type])");
		return Fraction(lhs) < rhs;
	}

	template <typename T>
	bool operator>(const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator > (Fraction, [type])");
		return Fraction(lhs) > rhs;
	}

	template <typename T>
	bool operator<=(const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator <= (Fraction, [type])");
		return Fraction(lhs) <= rhs;
	}

	template <typename T>
	bool operator>=(const T &lhs, const Fraction &rhs) {
		static_assert(Fraction::is_supported_type<T>(), "Error: unsupported type for operator >= (Fraction, [type])");
		return Fraction(lhs) >= rhs;
	}
}

#undef NPASSON_MAYBE_UNUSED
#undef NPASSON_IF_CONSTEXPR

#ifdef NPASSON_EXPERIMENTAL_COMPILE
#include "fraction.cpp"
#endif

#endif //NPASSON_FRACTION_HPP
