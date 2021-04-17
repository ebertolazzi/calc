# include "calc.hh"
using namespace calc_load;

using std::string;
using std::getline;
using std::cin;
using std::cout;
using std::endl;

typedef Calculator<double> CALC;

int main() {

  string str;
  CALC   ee;

  for (;;) {
    cout << "> ";
    getline(cin, str);
    if ( str == "end"  || str == "bye" ||
         str == "quit" || str == "exit"  ) break;
    if ( str == "list" ) {
      ee . print(cout);
    } else {
      bool err = ee.parse(str.c_str());
      if ( err ) ee.report_error(cout);
      else       cout << ee.get_value() << endl;
    }

  }

  cout << "\nbye\n";
}


