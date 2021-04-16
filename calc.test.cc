# include "calc.hh"

using calc::Calculator ;
using namespace std ;

typedef Calculator<double,unsigned> CALC ;

int main() {

  string str ;
  CALC * parse = new CALC() ;
  
  while (true) {
    cout << "> " ;
    getline(cin, str) ;
    if ( str == "end"  || str == "bye" ||
         str == "quit" || str == "exit"  ) break ;
    if ( str == "list" ) {
      cout << *parse << endl ;
    } else {
      bool err ;
      double value = parse -> parse(str,err) ;
      if ( err ) parse -> report_error(cout) ;
      else       cout << value << endl ;
    }	
    
  }
  
  cout << endl << "bye" << endl ;
  delete parse ;
}
