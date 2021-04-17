# include "calc.hh"

using namespace calc_load;

using std::string;
using std::cin;
using std::cout;
using std::endl;

typedef Calculator<double> CALC;

static
double power2(double const a)
{ return a*a; }

static
double add(double const a, double const b)
{ return a+b; }

int
main() {

  string str;
  CALC   ee;
  bool   ok;

  ee.set("abc", 2.5 );
  if ( ee.exist("abc") )
    cout << "found abc, value = " << ee.get("abc",ok) << endl;
 
  ee.set_unary_fun("power2",power2);
  ee.set_binary_fun("add",add);
  
  // test new function
  ee.parse("power2(add(abc,1))");
  
  // chech for error
  ee.report_error(cout);

  // print last computed value
  cout << "last computed value " << ee.get_value() << endl;
  
  // produce an error
  ee.parse("12+23/0");
  ee.report_error(cout);
   
  ee.parse_file("calc.test",true);

  // print variables
  ee.print(cout);
  
  cout << "\nbye\n";

  return 0;
}

