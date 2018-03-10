## Fraction Type

A simple fraction-like type for C++11 and up. Preserves precision in all operations.

## Code Example

The Fraction type can be seamlessly used instead of normal numeric types: 

```
#include <cstdio>
#include "fractiontype/include/fraction.hpp"
using npasson::Fraction;

Fraction f(Fraction x) {
	return 1/x;
}

int main() {

	// initialize like you're used to
	Fraction a = 15;
	Fraction b = 0.2;

	// multiplication and divsion operators implemented
	Fraction c = f(a)*f(b); // (1/15)*(1/0.2) == 1/3
	Fraction d = f(c)/2;	// f(1/3)/2 == 1.5

	// casting to needed types
	printf("d is %f", (double)d);	// prints "d is 1.5"

	return 0;
}		
```

## Motivation

This type will be the base I build my future math projects on, since Matrix inversions involve a lot of divisions and multiplication and floating point has too many precision errors.

## Usage
**1\.**
 `cd` into your project folder, and run
```
git clone https://github.com/npasson/fractiontype.git
```
This will download the code.

**2\.**
Run 
```
g++ -c -std=c++11 ./fractiontype/include/fraction.cpp -o libfraction.o
```
This will add a compiled object file into your project folder. You need to repeat this step every time you make changes to the Fraction code.

**3\.**
Add these two lines at the top of your program:
```
#include "fractiontype/include/fraction.hpp"
using npasson::Fraction;
```
In case you're using two fraction implementations, you can leave out the second line and access the type via the `npasson` namespace (just replace the affected `Fraction`s by `npasson::Fraction`).

**4\.**
From now on, you can compile like normal (and use the Fraction type), but add `-lfraction` to the linker arguments. This means *after* the file names:

`g++ -std=c++14 foo.cpp main.cpp`**`-lfraction`**`-o bar`

## Reference

Positive range:
from `0.00000000000000000010842022`
to `9223372036854775807`

For a documentation see <http://www.npasson.com/fractiontype>.

## License

**Fraction Type**  
**Copyright (C) 2018  Nicholas Passon**  

This program is free software: you can redistribute it and/or modify  it under the terms of the GNU Affero General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version. This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Affero General Public License for more details. 

You should have received a copy of the GNU Affero General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
