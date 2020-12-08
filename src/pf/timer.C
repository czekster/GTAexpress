// ======================================================================= //
// Class Timer :
// (c) copyright PAC++ 1994
// date: 1994
// ======================================================================= //

#include <math.h>

#ifndef WIN32
extern "C" {
#include <sys/time.h>
#include <sys/resource.h>
  // int getrusage (int, struct rusage*) ; *** removed for g++ compiler
}
#endif

#include <iostream>
#include "timer.h"

/****************************************************/
/*  Windows NT (32 bit) dtime() routine             */
/*  Provided by: Piers Haken, piersh@microsoft.com  */
/****************************************************/
#ifdef WIN32
#include <windows.h>

double Timer::dtime(void) {
 double q;

 q = (double)GetTickCount() * 1.0e-03;
	
 return q;
}
#endif

   // Clear timer :
void Timer::clear() { ut = st = rt = 0 ; }

   // Start timer
void Timer::start()
{  
#ifndef WIN32
   struct rusage  tmp1 ;  // to getrusage (sys+user times)
   struct timeval tmp2 ;  // to gettimeofday  (real time)
   getrusage (RUSAGE_SELF, &tmp1) ; 
   gettimeofday (&tmp2, 0) ;

   // user time 
   ut = (double) tmp1.ru_utime.tv_sec + 
         ((double) tmp1.ru_utime.tv_usec)/ (double)MSPSEC ;
   // system time 
   st = (double) tmp1.ru_stime.tv_sec + 
         ((double) tmp1.ru_stime.tv_usec)/ (double)MSPSEC ;
   // real time 
   rt = (double) tmp2.tv_sec + 
         ((double) tmp2.tv_usec)/ (double)MSPSEC ; 
#else
   ut = st = rt = dtime();
#endif
}

   // Stop timer 
void Timer::stop()
{ 
   double uts = ut, sts = st , rts = rt ;
   start() ;
   ut -= uts ; st -= sts ; rt -= rts ;
}

   // Return a value to initialize random generator 
long Timer::seed()   
{
#ifndef WIN32
   struct timeval tp;
   gettimeofday(&tp, 0) ;
   return(tp.tv_usec);
   return 1 ;
#endif
  return 1;//ERR_VISUAL_C++
}

   // Output the value of the timer :
std::ostream& operator<<(std::ostream& o, const Timer& T) 
{
   o << "user time: " << T.usertime() << '\n' ;
   o << "sys. time: " << T.systime() << '\n' ;
   o << "real time: " << T.realtime() << std::endl ;
   return o ;
}
       
   // Some arithmetic operator :
Timer& Timer::operator = (const Timer & T) 
{  
   ut = T.ut ; st = T.st ; rt = T.rt ; 
   return *this ; 
}
      
   // Computes and returns interval of time
   // beteween *this and T
const Timer Timer::operator - (const Timer & T)  const
{
   Timer Tmp ;
   Tmp.ut = ut - T.ut ; 
   Tmp.st = st - T.st ; 
   Tmp.rt = rt - T.rt ; 
   return Tmp ;
}

const Timer Timer::operator - () 
{
   Timer Tmp ;
   Tmp.ut = -ut ; 
   Tmp.st = -st ; 
   Tmp.rt = -rt ; 
   return Tmp ;
}

const Timer Timer::operator + (const Timer & T)  const
{
   Timer Tmp ;
   Tmp.ut = ut + T.ut ; 
   Tmp.st = st + T.st ; 
   Tmp.rt = rt + T.rt ; 
   return Tmp ;
}


