CALC
----

Minimal expression evaluator on C++.

A minimal example odf use is the following:

```cpp
# include "calc.hh"

using calc::Calculator;
using namespace std;

typedef Calculator<double,unsigned> CALC;

int
main() {

  string str;
  CALC * parse = new CALC();
  
  while (true) {
    cout << "> ";
    getline(cin, str) ;
    if ( str == "end"  || str == "bye" ||
         str == "quit" || str == "exit"  ) break;
    if ( str == "list" ) {
      cout << *parse << '\n';
    } else {
      bool err;
      double value = parse->parse(str,err);
      if ( err ) parse->report_error(cout);
      else       cout << value << '\n';
    }	
    
  }
  
  cout << "\nbye\n";
  delete parse;
  return 0;
}
}
```

when run

```bash
> 1+1
2
> a = 2
2
> b = pi
3.14159
> a*b
6.28319
>
```

can have a list of commands

```bash
> list

UNARY FUNCTIONS
acos, asin, atan, ceil, cos, cosh, exp, floor, log, log10, sin, sinh, sqrt, tan, tanh,

BINARY FUNCTIONS
atan2, max, min, pow,

VARIABLES
a = 2
b = 3.14159
e = 2.71828
pi = 3.14159
END LIST
>
```

can be used in any C++ code for minimalistic
symbolic computation with expressions.

Developer
---------

    Enrico Bertolazzi  
    Dipartimento di Ingegneria Industriale  
    Università degli Studi di Trento  
    mailto:enrico.bertolazzi@unitn.it  
    homepage: https://e.bertolazzi.dii.unitn.it  


This code is inspired by the ``Expression Evaluator'' of Mark Morley (c) 1992.
