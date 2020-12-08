//====================================================================================//
//                                                                                    //
//                            Class Scratch Funtion Table                             //
//                                                                                    //
//====================================================================================//
//  This File:   scrft_inl.h                Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 4200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline scr_ft::scr_ft()
{
 dimension = 0;
 number    = 0;
 step      = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline scr_ft::scr_ft(const int dim)
{
 dimension = dim;
 number    = 0;
 step      = dim;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline scr_ft::~scr_ft()
{
 if (dimension != 0) {
  desal();
  dimension = 0;
  number    = 0;
  step      = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void scr_ft::create()
{
 dimension = 0;
 number    = 0;
 step      = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void scr_ft::create(const int dim)
{
 erase();
 dimension = dim;
 step      = dim;
 number    = 0;
 alloc();
}

     //--------------------------------------------------------------------------//
     //                            Copy Intialization                            //
     //--------------------------------------------------------------------------//
inline void scr_ft::create(FTB & ftb)
{
 erase();
 dimension = ftb.size();
 step      = ftb.size();
 number    = 0;
 alloc();

 for(number=0; number<dimension; number++){
   elem_list e(1);          // the list to store the unique function 'f'
   e.tail(number);                          // puts the 'FTB::ftb' function id in the list 'e'
   ftb.get(number, table[number]);        // copies the function to the entry 'number'
   e_list[number].copy(e);              // stores the 'FTB::ftb' function id
   op[number] = No_oper;                //    and the operation
 }
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void scr_ft::erase()
{
 if (dimension != 0) {
   desal();
   dimension = 0;
   number    = 0;
   step      = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                  The current number of functions is zero                 //
     //--------------------------------------------------------------------------//
inline bool scr_ft::is_empty() const
{
 return(number == 0);
}
     //--------------------------------------------------------------------------//
     //                 There is no more place for new functions                 //
     //--------------------------------------------------------------------------//
inline bool scr_ft::is_full() const
{
 return(dimension == number);
}
     //--------------------------------------------------------------------------//
     //                     The maximum number of functions                      //
     //--------------------------------------------------------------------------//
inline int scr_ft::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The current number of functions                      //
     //--------------------------------------------------------------------------//
inline int scr_ft::has() const
{
 return(number);
}

     //--------------------------------------------------------------------------//
     //                It Includes function 't' into the table                   //
     //--------------------------------------------------------------------------//
inline func_id scr_ft::put(const func_tree & t)
{
 if (number == dimension)
   realloc();
 else {
   table[number].copy(t);      // usually the function is taken from 'FTB::ftb', but
   e_list[number].erase();     //   'put' is a special case: the function 't' is stored
   op[number] = No_oper;       //   directly into the 'scrt_ft::sft' (in this case the 
   number++;                   //   function is not obtained by an operation)
 }
 return(number-1);
}
     //--------------------------------------------------------------------------//
     //            It copies the function located in entry 'f' to 't'            //
     //--------------------------------------------------------------------------//
inline void scr_ft::get(const func_id f, func_tree & t) const
{
#ifdef _PEPS_DEBUG_
  if (f >= number)
    Programming_Error("index out of range", 4201)
#endif
 t.copy(table[f]);
}
