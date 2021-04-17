/*--------------------------------------------------------------------------*\
 |                                                                          |
 |  This program is free software; you can redistribute it and/or modify    |
 |  it under the terms of the GNU General Public License as published by    |
 |  the Free Software Foundation; either version 2, or (at your option)     |
 |  any later version.                                                      |
 |                                                                          |
 |  This program is distributed in the hope that it will be useful,         |
 |  but WITHOUT ANY WARRANTY; without even the implied warranty of          |
 |  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the           |
 |  GNU General Public License for more details.                            |
 |                                                                          |
 |  You should have received a copy of the GNU General Public License       |
 |  along with this program; if not, write to the Free Software             |
 |  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.               |
 |                                                                          |
 |  Copyright (C) 1999                                                      |
 |                                                                          |
 |            ___    ____  ___  __  __        ___    ____  ___  __  __      |
 |           /   \  /     /   \  \  /        /   \  /     /   \  \  /       |
 |          /____/ /__   /____/   \/        /____/ /__   /____/   \/        |
 |         /   \  /     /   \     /        /   \  /     /   \     /         |
 |        /____/ /____ /    /    /        /____/ /____ /    /    /          |
 |                                                                          |
 |      Enrico Bertolazzi                                                   |
 |      Dipartimento di Ingegneria Meccanica e Strutturale                  |
 |      Universita` degli Studi di Trento                                   |
 |      Via Mesiano 77, I-38050 Trento, Italy                               |
 |                                                                          |
\*--------------------------------------------------------------------------*/

#ifndef CALC_HH
#define CALC_HH

#include <cmath>

// standard includes I/O
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

// STL lib
#include <string>
#include <map>

/*!
 * This namespace is used to shield the class definitions
 */
namespace calc_defs {
  
  using namespace ::std;
   
  /*!
   * This class implement the expression evaluator
   */
  template <typename T_type = double> 
  class Calculator {
  
    // Calculator type
  public:
  
    typedef T_type              value_type;
    typedef value_type*         pointer;
    typedef const value_type*   const_pointer;
    typedef value_type&         reference;
    typedef const value_type&   const_reference;
  
    typedef Calculator<value_type> CALCULATOR;
  
    typedef value_type (*Func1)(value_type);
    typedef value_type (*Func2)(value_type,value_type);
  
    typedef map<string,Func1>      map_fun1;
    typedef map<string,Func2>      map_fun2;
    typedef map<string,value_type> map_real;
  
    typedef typename map_fun1::iterator       map_fun1_iterator;
    typedef typename map_fun1::const_iterator map_fun1_const_iterator;
    typedef typename map_fun2::iterator       map_fun2_iterator;
    typedef typename map_fun2::const_iterator map_fun2_const_iterator;
    typedef typename map_real::iterator       map_real_iterator;
    typedef typename map_real::const_iterator map_real_const_iterator;
  
  private:

    static value_type internal_abs (value_type x) { return x > 0 ? x : - x; }
    static value_type internal_pos (value_type x) { return x > 0 ? x : 0; }
    static value_type internal_neg (value_type x) { return x > 0 ? 0 : x; }
    static value_type internal_max (value_type a, value_type b) { return a > b ? a : b; }
    static value_type internal_min (value_type a, value_type b) { return a < b ? a : b; }
  
    map_fun1 unary_fun;
    map_fun2 binary_fun;
    map_real variables;
  
    typedef enum {
      Number, Variable, Parameter,
      Plus, Minus, Times, Divide, Power,
      OpenPar, ClosePar,
      Assign, Comma, Unrecognized,
      EndOfExpression, EndOfString
    } Token_Type;
  
    typedef enum {
      No_Error,
      Divide_By_Zero,
      Expected_OpenPar, Expected_ClosePar, Expected_Comma,
      Unknown_Variable, Bad_Position, Unknown_Error
    } ErrorCode;
  
  private:
    // error handling
    ErrorCode  error_found;
  
    // token management
    Token_Type token_type;
    string     token_string;
    value_type last_evaluated;
  
    // internal use
    char const * string_in;
    char const * ptr;
  
    char const & get(void)       { return *ptr++; }
    char const & see(void) const { return *ptr; }
    int          pos(void) const { return static_cast<int>(ptr - string_in); }

    void get_number( value_type & res, string const & s ) const {
      stringstream str(s);
      str >> res;
    }

    void to_string( value_type const & res, string & s ) const {
      stringstream str;
      str << res;
      s = str . str();
    }

    CALCULATOR const & operator = (CALCULATOR const &);
    // { init(); return *this; }

    Calculator(CALCULATOR const &);
    // { init(); }
  
  public:
  
    Calculator(void) 
    : unary_fun()
    , binary_fun()
    , variables()
    , error_found()
    , token_type()
    , token_string()
    , last_evaluated(0)
    , string_in(0)
    , ptr(0)
    { init(); };

    ~Calculator(void) { };
  
    void init(void);
    
    /*!
     *  This method do a parsing of an input string
     *  \param str the input string to be parsed
     *  \return true if no error parsing errors are found
     */
    bool parse( char const * str );
    bool parse( string const & str ) { return parse(str.c_str()); }

    /*!
     *  This method do a parsing of a whole file
     *  \param name     the name of the input file
     *  \param show_err true if you wand message on error parsing
     *  \param stream   stream for the error messages
     */
    void
    parse_file(
      char const * name,
      bool         show_err = false,
      ostream &    stream = cerr
    );

    void
    parse_file(
      string const & name,
      bool           show_err = false,
      ostream &      stream = cerr
    ) {
      parse_file( name.c_str(), show_err, stream);
    }
  
    /*!
     *  Print the last error found, noe if no error are found
     *  \param s the object stream of output
     */
    void report_error( ostream & s );

    /*!
     *  @\eturn the value of the last evaluated expression
     */
    const_reference get_value() const { return last_evaluated; };

    /*!
     *  Set or define a variable
     *  \param name the name of the variable
     *  \param val the value to be stored
     */
    void set( string const & name, value_type val );
    void set( char const * name, value_type val ) { set(string(name),val); }

    /*!
     *  Look if a variable exists and remove from the list
     *  \param name the name of the variable
     *  \return true if the variable exists and is dropped
     */
    bool
    drop( string const & name ) {
      map_real_const_iterator ii = variables.find(name);
      bool ex = ii != variables.end();
      if ( ex ) variables . erase( ii );
      return ex;
	  }

    bool drop( char const * name ) { return drop(string(name)); }

    /*!
     *  Look if a variable exists
     *  \param name the name of the variable
     *  \return true if the variable exists
     */
    bool exist(string const & name) const
    { return variables.find(name) != variables.end(); }
    bool exist(char const name[]) const { return exist(string(name)); }
  
    /*!
     * \param name the name of the variable
     * \param ok   true  if the variable exists
     * \return the value of the variable
     */
    value_type get( string const & name, bool & ok );
    value_type get( char const * name, bool & ok ) { return get(string(name),ok); }

    /*!
     * Print internal status of the parser: the list of the variables
     * constants and functions
     *
     * \param s the object stream of output
     */
    void print( ostream & s ) const;
  
    /**
     *  Add unary function to the parser
     *  @param f_name function name
     *  @param f_ptr  pointer to the function routine
     */
    void
    set_unary_fun( char const * f_name, Func1 f_ptr )
    { unary_fun[f_name] = f_ptr; }

    void
    set_unary_fun( string const & f_name, Func1 f_ptr )
    { unary_fun[f_name] = f_ptr; }
  
    /*!
     *  Add binary function to the parser
     *  \param f_name function name
     *  \param f_ptr  pointer to the function routine
     */
    void
    set_binary_fun( char const * f_name, Func2 f_ptr )
    { binary_fun[f_name] = f_ptr; }

    void
    set_binary_fun( string const & f_name, Func2 f_ptr )
    { binary_fun[f_name] = f_ptr; }

    /*!
     *  \return true if no error found
     */
    bool no_error() const { return No_Error == error_found; }

    /*!
     *  \return a reference of the variables map.
     */
    map_real const & variables_map() const { return variables; }
    
    /*!
     *  \return a reference of the variables map.
     */
    void
    variables_merge( map_real const & ee_vars ) {
      for ( map_real_const_iterator ii = ee_vars . begin();
            ii != ee_vars . end(); ++ii ) {
        variables[ii -> first] = ii -> second;
      }
    }

  private:
  
    value_type G0(void);
    value_type G1(void);
    value_type G2(void);
    value_type G3(void);
    value_type G4(void);
    value_type G5(void);
    void Next_Token(void);
  
  };
  
  template <typename T_type>
  inline
  void
  Calculator<T_type>::init() {
    last_evaluated      = 0;

    unary_fun["abs"]    = internal_abs;
    unary_fun["pos"]    = internal_pos;
    unary_fun["neg"]    = internal_neg;

    unary_fun["cos"]    = cos;
    unary_fun["sin"]    = sin;
    unary_fun["tan"]    = tan;
  
    unary_fun["acos"]   = acos;
    unary_fun["asin"]   = asin;
    unary_fun["atan"]   = atan;
  
    unary_fun["cosh"]   = cosh;
    unary_fun["sinh"]   = sinh;
    unary_fun["tanh"]   = tanh;
  
    unary_fun["exp"]    = exp;
    unary_fun["log"]    = log;
    unary_fun["log10"]  = log10;
    unary_fun["sqrt"]   = sqrt;
    unary_fun["ceil"]   = ceil;
    unary_fun["floor"]  = floor;
  
    binary_fun["atan2"] = atan2;
    binary_fun["pow"]   = pow;
    binary_fun["max"]   = internal_max;
    binary_fun["min"]   = internal_min;
  
    // predefined variables
    variables["pi"]     = 3.14159265358979323846;
    variables["e"]      = 2.71828182845904523536;
  }
  
  template <typename T_type>
  void
  Calculator<T_type>::set( string const & name, value_type val ) {
    int pos = name . find('@');
    if ( pos != string::npos ) {
      // split variable in 2
      string var1 = name . substr(0,pos);
      string var2 = name . substr(pos+1);

      map_real_const_iterator ii = variables . find(var2);
      if ( ii != variables . end() ) {
		// build variable
		to_string( ii -> second, var2 );
		var1 += var2;
		variables[var1] = val;
      } else {
        token_string = var2;
        throw Unknown_Variable;
      }
    } else {
      variables[name] = val;
    }
  }

  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::get( string const & name, bool & ok ) {
    int pos = name . find('@');
    string var1, var2;
    if ( pos != string::npos ) {
      // split variable in 2
      var1 = name . substr(0,pos);
      var2 = name . substr(pos+1);
      map_real_const_iterator ii = variables . find(var2);
      if ( ii != variables . end() ) {
		// build variable
		to_string( ii -> second, var2 );
		var1 += var2;
      } else {
        token_string = var2;
        throw Unknown_Variable;
      }
    } else {
      var1 = name;
    }
    map_real_const_iterator ii = variables . find(var1);
    ok = ii != variables . end();
    if ( ok ) return ii -> second;
    else      return 0;
  }

  template <typename T_type>
  bool
  Calculator<T_type>::parse( char const * str ) {
    string_in = ptr = str;
    error_found = No_Error;
    try {
      do { Next_Token(); } while ( token_type == EndOfExpression );
      while ( token_type != EndOfString && error_found == No_Error ) {
        last_evaluated = G0();
        while ( token_type == EndOfExpression ) Next_Token();
      }
    }
    catch (ErrorCode err) {
      error_found = err;
    }
    catch (...) {
      error_found = Unknown_Error;
    }
    return error_found != No_Error;
  }
  
  template <typename T_type>
  void
  Calculator<T_type>::parse_file(
    char const * name,
    bool const   show_err,
    ostream &    stream_error
  ) {
    string        line;
    unsigned long nline = 0;
    ifstream      stream;
    stream . open(name);
    if ( stream.is_open() ) {
      while ( stream.good() ) {
        ++nline;
        getline( stream, line);
        if ( parse(line.c_str()) && show_err ) {
          stream_error << "in file '" << name
                       << "' on line " << nline
                       << " found an error\n";
          report_error(stream_error);
        }
      }
      stream . close();
    } else {
      if ( show_err ) {
        stream_error << "ERROR in opening file '" << name << "'\n";
      }
    }
  }
  
  template <typename T_type>
  void
  Calculator<T_type>::report_error( ostream & s ) {
    switch (error_found) {
    case No_Error:
      s << "No error found\n";
      return;
  
    case Divide_By_Zero:
      s << "divide by 0\n";
      break;
  
    case Expected_OpenPar:
      s << "expect ``('' found ``" << token_string << "''\n";
      break;
  
    case Expected_ClosePar:
      s << "expect ``)'' found ``" << token_string << "''\n";
      break;
  
    case Expected_Comma:
      s << "expect ``,'' found ``" << token_string << "''\n";
      break;
  
    case Unknown_Variable:
      s << "unknown variable: " << token_string << "\n";
      break;
  
    case Bad_Position:
      s << "bad position for token: ``" << token_string << "''\n";
      break;

    case Unknown_Error:
      s << "Unknown error for token: ``" << token_string << "''\n";
      break;
    }
    s << "\t: " << string_in << "\n"
      << "\t: " << setfill('-') << setw(pos()-1) << "^\n";
  }
  
  template <typename T_type>
  void
  Calculator<T_type>::print( ostream & s ) const {
    map_real_const_iterator ii;
    map_fun1_const_iterator f1;
    map_fun2_const_iterator f2;
  
    s << "\nUNARY FUNCTIONS\n";
    for ( f1 = unary_fun . begin(); f1 != unary_fun . end(); ++f1 )
      s << f1 -> first << ", ";
  
    s << "\n\nBINARY FUNCTIONS\n";
    for ( f2 = binary_fun . begin(); f2 != binary_fun . end(); ++f2 )
      s << f2 -> first << ", ";
  
    s << "\n\nVARIABLES\n";
    for ( ii = variables . begin(); ii != variables . end(); ++ii )
      s << ii -> first << " = " << ii -> second << "\n";
  
    s << "END LIST\n";
  }
  
  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::G0() {
  
    if ( token_type == EndOfExpression || token_type == EndOfString ) return 0;
  
    char const * const bf_ptr = ptr; // save pointer
    string     name  = token_string;
    Token_Type token = token_type;
  
    if ( token_type == Variable ) {
      Next_Token();
      if ( token_type == Assign ) { // handle assign
        Next_Token();              // eat assign
        value_type res = G1();
        set(name,res);
        return res; // assign value
      }
    }
  
    ptr = bf_ptr; // restore pointer
    token_string = name;
    token_type   = token;
  
    return G1(); // handle immediate evaluation
  }
  
  // handle binary + and -
  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::G1() {
    value_type lval = G2(), rval;
    while ( token_type == Plus || token_type == Minus ) {
      bool do_plus = token_type == Plus;
      Next_Token();
      rval = G2();
      if ( do_plus ) lval += rval;
      else           lval -= rval;
    };
    return lval;
  }
  
  // handles * and /
  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::G2() {
    value_type lval = G3(), rval;
    while ( token_type == Times || token_type == Divide ) {
      bool do_times = token_type == Times;
      Next_Token();
      rval = G3();
      if ( do_times ) lval *= rval;
      else {
        if ( rval == 0 ) throw Divide_By_Zero;
        lval /= rval;
      }
    };
    return lval;
  }
  
  // handles ^ operator
  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::G3() {
    value_type lval = G4();
    if ( token_type == Power )
      { Next_Token(); lval = pow(lval,G4()); }
    return lval;
  }
  
  // handles any unary + or - signs
  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::G4() {
    if ( token_type == Minus ) { Next_Token(); return - G5(); }
    if ( token_type == Plus  ) Next_Token();
    return G5();
  }
  
  // handles numbers, variables, functions and parentesis
  template <typename T_type>
  typename Calculator<T_type>::value_type
  Calculator<T_type>::G5() {
    if ( token_type == OpenPar ) { // handle ( ... )
      Next_Token(); // eat (
      value_type val = G0();
      if ( token_type != ClosePar ) throw Expected_ClosePar;
      Next_Token(); // eat )
      return val;
    }
  
    if ( token_type == Number ) {
      value_type val;
      get_number(val,token_string);
      Next_Token();
      return val;
    }
  
    if ( token_type == Variable ) {
  
      bool ok;
      value_type res = get( token_string, ok );
      if ( ok ) {
        Next_Token();
        return res;
      }
  
      map_fun1_iterator f1 = unary_fun . find(token_string);
      if ( f1 != unary_fun . end() ) {
        Next_Token(); // expect (
        if ( token_type != OpenPar ) throw Expected_OpenPar;
        Next_Token(); // eat (
        value_type v1 = G0();
        if ( token_type != ClosePar ) throw Expected_ClosePar;
        Next_Token(); // eat )
        return f1 -> second(v1);
      }
  
      map_fun2_iterator f2 = binary_fun . find(token_string);
      if ( f2 != binary_fun . end() ) {
        Next_Token(); // expect (
        if ( token_type != OpenPar ) throw Expected_OpenPar;
        Next_Token(); // eat (
        value_type v1 = G0();
        if ( token_type != Comma ) throw Expected_Comma;
        Next_Token(); // eat ,
        value_type v2 = G0();
        if ( token_type != ClosePar ) throw Expected_ClosePar;
        Next_Token(); // eat )
        return f2 -> second(v1,v2);
      }
  
      throw Unknown_Variable;
    }
    throw Bad_Position;
  }
  
  template <typename T_type>
  void
  Calculator<T_type>::Next_Token() { // eat separators
    token_type   = EndOfExpression;
    token_string = "";
  
    // EAT SEPARATORS
    while ( isspace(see()) ) get();
  
    // SKIP COMMENTS
    if ( see() == '#' ) {
      while ( see() != '\n' && see() != '\0' ) get();
      return;
    }
  
    if ( isalpha(see()) ) {
      token_type = Variable;
      do { token_string += get(); } while ( isalnum(see()) || see() == '_' || see() == '@' );
      return;
    }
    
    if ( isdigit(see()) ) {
      token_type = Number;
      do { token_string += get(); } while ( isdigit(see()) );
      if ( see() == '.' ) {
        do { token_string += get(); } while ( isdigit(see()) );
      }
      if ( see() == 'e' || see() == 'E' ) {
        token_string += get();
        if ( see() == '+' || see() == '-' ) token_string += get();
        if ( isdigit(see()) ) {
          do { token_string += get(); } while ( isdigit(see()) );
        } else {
          token_type = Unrecognized;
        }
      }
      return;
    }
  
    token_string = get();
    switch (token_string[0]) {
    case '+' : token_type = Plus;
               break;
    case '-' : token_type = Minus;
               break;
    case '*' : token_type = Times;
               break;
    case '/' : token_type = Divide;
               break;
    case '^' : token_type = Power;
               break;
    case '(' : token_type = OpenPar;
               break;
    case ')' : token_type = ClosePar;
               break;
    case '=' : token_type = Assign;
               break;
    case ',' : token_type = Comma;
               break;
    case '\0': token_type   = EndOfString;
               token_string = "EndOfString";
               break;
    case ';' : token_type   = EndOfExpression;
               token_string = "EndOfExpression";
               break;
    default  : token_type   = Unrecognized;
               token_string = "Unrecognized";
               break;
    }
  }
  
  // end class Calculator

} // end namespace


namespace calc_load {
  using calc_defs::Calculator;
}

#endif

// end of file: calc.hh
