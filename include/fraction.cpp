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
 * \file fraction.cpp
 * The code of the Fraction class.
 */

#include <sstream>

#ifndef NPASSON_EXPERIMENTAL_COMPILE
#include "fraction.hpp"
#endif
#ifdef NPASSON_DEBUG
#	include <iostream>
#endif

// yeah yeah, implementation-defined, but really, there's no reason
// to include <climits> just for one value, especially in a library
#define MAX_VAL 9223372036854775807

namespace npasson {

	/**
	 * Tests if given char is a numeric digit. Compares it against the ASCII table digits.
	 *
	 * @param c
	 * @return true if <tt>c</tt> is a numeric digit
	 */
	bool Fraction::isdigit(char c) {
		return c >= 48 && c <= 57;
	}

	/**
	 * Returns true if the character passed is a numeric delimiter, i.e. a dot or a comma.
	 *
	 * @param c
	 * @return true if <tt>c</tt> is a delimiter
	 */
	bool Fraction::isdelim(char c) {
		return c == '.' || c == ',';
	}

	/**
	 * \brief Tests if the string is a number.
	 *
	 * Tests if the string can be processed as a number. Currently only matches
	 * decimal numbers with a dot or a commas as decimal delimiter. Runs in O(n).
	 *
	 * @param str A string representing a number (or not).
	 * @retval <b><tt>true</tt></b> when the string represents a decimal number
	 * @retval <b><tt>false</tt></b> otherwise
	 */
	bool Fraction::isnumber(std::string str) {
		if(str.empty()) return false;

		if(str[0] == '-') {
			str = str.substr(1); // remove the minus from the front
		} else if (!isdigit(str[0])) {
			return false; // if first char is no digit
		} else if (isdelim(str[str.size() - 1])) {
			return false; // if last char is a delimiter
		}

		int i;
		int d = 0;
		char c = str[0];
		for(i = 0;
			i<str.size() // while not at the end of the string
			&& (d += (isdelim(c)) ? 1 : 0) < 2 // and we haven't had more than one delimiter yet
			&& (isdigit(c) || isdelim(c)); // and everything's still a digit or delimiter,
			c = (i+1 == str.size()) ? (char)++i : str[++i]) // set c to the next character if still in string
															// (else increase i anyway)
		{}

		return i == str.size(); // if we went through once without breaking rules, it's a number.
	}

	/**
	 * This constructor initializes the Fraction to be 0/1.
	 */
	Fraction::Fraction() {
		this->numerator = 0;
		this->denominator = 1;
	}

	Fraction::Fraction(const Fraction &frac) = default;

	Fraction::Fraction(long long signed int numerator, long long signed int denominator) {
		if (denominator == 0) {
			this->numerator = 0;
			this->denominator = 1;
			this->_invalid = true;
			return;
		} else if (numerator == 0) {
			this->numerator = 0;
			this->denominator = 1;
			return;
		} else if (denominator == 1) {
			this->numerator = numerator;
			this->denominator = 1;
			return;
		}
		short sign = 1;
		if (numerator < 1) {
			sign *= -1;
			numerator *= -1;
		}
		if (denominator < 1) {
			sign *= -1;
			denominator *= -1;
		}
		long long signed int divisor = Fraction::gcd(numerator, denominator);
		this->numerator = numerator/divisor*sign;
		this->denominator = denominator/divisor;
	}

	Fraction::Fraction(unsigned long long int numerator) {
		if(numerator < MAX_VAL) {
			this->numerator = static_cast<long long int>(numerator);
		}
		else {
			this->numerator = MAX_VAL;
			this->denominator = 1;
		}
	}

	Fraction::Fraction(signed long long int numerator) : numerator(numerator) {}

	Fraction::Fraction(unsigned long int numerator) : numerator(static_cast<long long signed int>(numerator)) {}

	Fraction::Fraction(signed long int numerator) : numerator(numerator) {}

	Fraction::Fraction(unsigned int numerator) : numerator(static_cast<long long signed int>(numerator)) {}

	Fraction::Fraction(signed int numerator) : numerator(numerator) {}

	Fraction::Fraction(unsigned short numerator) : numerator(static_cast<long long signed int>(numerator)) {}

	Fraction::Fraction(signed short numerator) : numerator(numerator) {}

	Fraction::Fraction(std::string str_val) {
		if(!isnumber(str_val)) {
			//std::cerr << "Passed string () is not a number. Fraction initialized to zero." << std::endl;
			this->numerator = 0;
			this->denominator = 1;
			this->_invalid = true;
			return;
		}

		bool neg = (str_val.find('-') != std::string::npos);
		if (neg) str_val.erase(0,1);

		// basically: if you're using a dot, find that, otherwise find a comma
		unsigned int dec_pos = (str_val.find('.') != std::string::npos)?
							   str_val.find('.'):
							   str_val.find(',');

		// get the right hand side of the decimal point
		std::string right_decimal_str = str_val.substr(dec_pos+1);
		long long int right_decimal = std::stoll(right_decimal_str);
		unsigned int denom_len = right_decimal_str.length();
		std::string denom_str(denom_len+1, '0');
		denom_str[0] = '1';
		long long int right_denom_int = std::stoll(denom_str);
		Fraction right_fraction(right_decimal, right_denom_int);

		// get the left hand side of the decimal point
		unsigned int left_len = str_val.length()-denom_len-1;
		std::string left_decimal_str = str_val.substr(0, left_len);
		long long int left_decimal = std::stoll(left_decimal_str);
		Fraction left_fraction(left_decimal, 1ll); // 1 long long

		// combine them
		Fraction final_frac = left_fraction+right_fraction;
		if(neg) final_frac *= -1;

		this->numerator = final_frac.numerator;
		this->denominator = final_frac.denominator;
	}

	Fraction::Fraction(float val) : Fraction(std::to_string(val)) {} // screw floating point, we'll do it via string

	Fraction::Fraction(double val) : Fraction(std::to_string(val)) {} // cast it to string again I guess

	Fraction::Fraction(long double val) {
		std::stringstream tempss;
		tempss << val;
		Fraction temp (tempss.str());
		this->numerator = temp.numerator;
		this->denominator = temp.denominator;
	} // why does std:: not have a long double constructor for string?

	Fraction::Fraction(char* val) : Fraction(std::string(val)) {} // another cast, just like the other two times

	Fraction::Fraction(const char* val) : Fraction(std::string(val)) {} // ...I will probably not accept merge requests about this

	/**
	 * This constructor has two effects:
	 *
	 * <b><tt>Fraction(true)</tt></b> is equivalent to <tt>Fraction()</tt>.
	 *
	 * <b><tt>Fraction(false)</tt></b> creates a Fraction where the <tt>_invalid</tt> bit is triggered, indicating
	 * undefined behavior on <i>all</i> operations. If you forget to check for that, it is also initialized as 0/0 to
	 * remind you.
	 *
	 * @param valid A bool indicating if the Fraction should be valid.
	 */
	Fraction::Fraction(bool valid) {
		if(valid) {
			return;
		} else {
			this->_invalid = true;
			this->denominator = 0;
			this->numerator = 0;
		}
	}

	Fraction::~Fraction() = default; // nothing to destroy

	bool Fraction::valid() const {
		return !(this->_invalid);
	}

	/* === GENERIC RETURNS === */

	Fraction::operator long long int()	const {return (			 numerator)/		  denominator;}
	Fraction::operator long int()		const {return (long int)(((double)	 numerator)/(double)  denominator);}
	Fraction::operator int()			const {return (int)(((double)	 numerator)/(double)  denominator);}
	Fraction::operator short()			const {return (short)(((double)	 numerator)/(double)  denominator);}
	Fraction::operator float()			const {return (float)(((double)	 numerator)/(double)  denominator);}
	Fraction::operator double()			const {return ((double)	 numerator)/(double)  denominator;}
	Fraction::operator bool() 			const {return (numerator != 0);}
	/**
	 * \brief Returns <tt>this</tt> as a <tt>std::string</tt>.
	 *
	 * @return A decimal std::string representation of the Fraction.
	 */
	std::string Fraction::str()			const {return std::to_string((double)(*this));}
	const char* Fraction::c_str()		const {return (*this).str().c_str();}
	std::string Fraction::f_str() 		const
			{return (std::to_string((*this).numerator) + "/" + std::to_string((*this).denominator));}

	/* === OPERATORS === */

	/* **** PLUS **** */
	Fraction& Fraction::operator+=(const Fraction &rhs) {
		long long int num, den;
		num = (numerator*rhs.denominator + rhs.numerator*denominator);
		den = denominator*rhs.denominator;
		(*this) = Fraction(num,den);
		return *this;
	}
	Fraction  Fraction::operator+ (const Fraction &rhs) const {
		Fraction temp = (*this);
		return temp += rhs;
	}

	/* **** MINUS **** */
	Fraction& Fraction::operator-=(const Fraction &rhs) {
		long long int num, den;
		num = (numerator*rhs.denominator - rhs.numerator*denominator);
		den = denominator*rhs.denominator;
		(*this) = Fraction(num,den);
		return *this;
	}
	Fraction  Fraction::operator- (const Fraction &rhs) const {
		Fraction temp = (*this);
		return temp -= rhs;
	}


	/* MULTIPLICATION */
	Fraction& Fraction::operator*=(const Fraction &rhs) {
		return ((*this) = Fraction(
				numerator * rhs.numerator,
				denominator * rhs.denominator
		));
	}
	Fraction  Fraction::operator* (const Fraction &rhs) const {
		Fraction temp = (*this);
		return temp *= rhs;
	}


	/* DIVISION */
	Fraction& Fraction::operator/=(const Fraction &rhs) {
		return ((*this) = Fraction(
				numerator * rhs.denominator,
				denominator * rhs.numerator
		));
	}
	Fraction  Fraction::operator/ (const Fraction &rhs) const {
		Fraction temp = (*this);
		return temp /= rhs;
	}

	/* MISC OPERATORS */

	Fraction &Fraction::operator++()	{return ((*this) = Fraction(numerator + denominator, denominator));}
	Fraction  Fraction::operator++(int) {Fraction temp = (*this); ++(*this); return temp;}

	Fraction &Fraction::operator--()	{return ((*this) = Fraction(numerator - denominator, denominator));}
	Fraction  Fraction::operator--(int) {Fraction temp = (*this); --(*this); return temp;}

	Fraction  Fraction::operator+ () const 	{return Fraction(*this);}
	Fraction  Fraction::operator- () const	{return Fraction(-numerator, denominator);}

	std::ostream&       operator<<(std::ostream &os, const Fraction &frac) {os << (double)frac; return os;}
	std::string	Fraction::operator()(){return this->str();}

	bool Fraction::operator==(const Fraction &rhs) const {
		return (this->numerator == 0)? rhs.numerator == 0 :
			   ((this->numerator/gcd(this->numerator,this->denominator) == rhs.numerator/gcd(rhs.numerator, rhs.denominator))
			   && (this->denominator/gcd(this->numerator,this->denominator) == rhs.denominator/gcd(rhs.numerator, rhs.denominator)));
	}
	bool Fraction::operator!=(const Fraction &rhs) const {
		return !((*this) == rhs);
	}

	bool Fraction::operator< (const Fraction &rhs) const {
		long long int multiple = lcm(denominator, rhs.denominator);
		return (
					(	 numerator)*(multiple/    denominator)
				  < (rhs.numerator)*(multiple/rhs.denominator)
		);
	}
	bool Fraction::operator> (const Fraction &rhs) const {
		long long int multiple = lcm(denominator, rhs.denominator);
		return (
				(	   numerator)*(multiple/    denominator)
				> (rhs.numerator)*(multiple/rhs.denominator)
		);
	}
	bool Fraction::operator<=(const Fraction &rhs) const {return !((*this)>rhs);}
	bool Fraction::operator>=(const Fraction &rhs) const {return !((*this)<rhs);}

	/* non-atomic OPERATORS */

	/**
	 * \brief Calculates <tt>this^exp</tt>
	 *
	 * Calculates <tt>this</tt> raised to the power of <tt>exp</tt>. Follows the normal mathematical rules.
	 * The argument has to be an integer, non-integer values are not supported currently.
	 *
	 * @param exp An integer exponent.
	 * @return
	 */
	Fraction Fraction::pow(signed int exp) {
		if (exp == 0) return Fraction(1);
		if(exp<0) return (this->invert()).pow(-exp);
		exp = (exp<0)?(-exp):(exp);
		Fraction tfrac(*this);
		Fraction mult(*this);
		for(int i=1; i<exp; ++i) {
			tfrac *= mult;
		}
		return tfrac;
	}

	/* === MANIPULATION === */

	/**
	 * \brief Returns an inverted version of the Fraction.
	 *
	 * Returns an inverted version of a Fraction. This will not modify the Fraction. If you want to invert the
	 * original Fraction, use <tt>invert(Fraction&)</tt> which is also faster.
	 *
	 * @return The inverted Fraction.
	 */
	Fraction Fraction::invert() const {
		return Fraction(this->denominator, this->numerator);
	}

	/**
	 * \brief Inverts the Fraction.
	 *
	 * Inverts a Fraction. This will directly modify the Fraction, but is faster than using <tt>f = f.invert()</tt>.
	 * If you need to invert a <tt>const Fraction</tt> or just get the value without modifying the Fraction, use
	 * <tt>invert()</tt>.
	 *
	 * @param frac The Fraction to be inverted.
	 * \sa invert()
	 */
	void Fraction::invert(Fraction &frac) {
		// XOR swap algorithm
		frac.numerator = (frac.numerator)^(frac.denominator);
		frac.denominator = (frac.denominator)^(frac.numerator);
		frac.numerator = (frac.numerator)^(frac.denominator);
	}

	/**
 	 * \brief Returns the absolute value.
 	 *
 	 * Implementation of the mathematical abs() function. Returns <tt>t</tt> if <tt>t</tt> is positive and
 	 * <tt>-t</tt> if <tt>t</tt> is negative,
 	 * so that the end result is always positive.
 	 *
 	 * @attention The type needs to support <tt>operator-()</tt> and <tt>operator<(T, T)</tt>.
 	 *
 	 * @param t Any numeric type.
 	 * @return The absolute value of the type.
 	 */
	template <typename T> T abs(T t) {
		if (t<T(0)) return -t;
		else return t;
	}

	/**
	 * \brief Returns the greatest common denominator.
	 *
	 * Returns the Greatest Common Denominator of <tt>a</tt> and <tt>b</tt> using Euclid's method. The result will always
	 * be positive. The function does not have a built-in check for negative types to increase speed, so beware that
	 * negative inputs are undefined behavior even though the arguments are signed.
	 *
	 * @param a A <i>positive</i> integer.
	 * @param b A <i>positive</i> integer.
	 * @return The GCD of a and b.
	 */
	long long signed int Fraction::gcd(long long signed int a, long long signed int b) {
		long long signed int t;
		while (b!=0) {
			t = b;
			b = a%b;
			a = t;
		}
		return a;
	}

	/**
	 * \brief Returns the least common multiple.
	 *
	 * Returns the Least Common Multiple of <tt>a</tt> and <tt>b</tt> using calls to <tt>gcd()</tt>. The result will always
	 * be positive. The function does not have a built-in check for negative types to increase speed, so beware that
	 * negative inputs are undefined behavior even though the arguments are signed.
	 *
	 * @param a A <i>positive</i> integer.
	 * @param b A <i>positive</i> integer.
	 * @return The LCM of a and b.
	 */
	long long signed int Fraction::lcm(long long signed int a, long long signed int b) {
		return (abs(a) / gcd(a, b)) * abs(b);
	}

}

#undef MAX_VAL