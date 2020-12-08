#ifndef _TIMER_H_
#define _TIMER_H_
// ======================================================================= //
// Class Timer :
// (c) copyright PAC++ 1994
// date: 1994
// ======================================================================= //

//class ostream ;

class Timer {
enum { 
  MSPSEC = 1000000  // microsecond per second
} ;
public :

   // Clear timer :
  void clear() ;
  
#ifdef WIN32
  double dtime(void);
#endif

   // Start timer
  void start() ;

  // Stop timer 
  void stop() ;

  // total amount of second spent in user mode
  double usertime() const { return ut ; }

  // total amount of second spent in system mode
  double systime () const { return st ; }

  // real total amount of second spent.  
  double realtime () const { return rt ; }

  // retourne une petite graine
  long seed()    ;

  // Output the value of the timer :
  friend std::ostream& operator<<(std::ostream& o, const Timer& T)  ;
       
  // Some arithmetic operators to compute cumulative time :
  Timer& operator = (const Timer & T)  ;
  const Timer operator - (const Timer & T)  const ;
  const Timer operator - ()  ;
  const Timer operator + (const Timer & T)  const ;
  const Timer operator += (const Timer & T) { return *this = *this + T; } ;
  const Timer operator -= (const Timer & T) { return *this = *this - T; } ;


private :
   double ut ;	// user time 
   double st ;	// system time 
   double rt ;  // real time  
} ;



#endif 
