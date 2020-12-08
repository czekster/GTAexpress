//====================================================================================//
//                                                                                    //
//                            Class SAN Element List                                  //
//                                                                                    //
//====================================================================================//
//  This File:   elemlist_inl.h             Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
  

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline elem_list::elem_list()
{
 dimension = 0;
 number    = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline elem_list::elem_list(const int dim)
{
 dimension = dim;
 number    = 0;
 list      = new elem[dimension];
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline elem_list::elem_list(const elem_list & l)
{
 dimension = l.dimension;
 number    = l.number;
 list      = new elem[dimension];
 for (int i=0; i<number; i++)
   list[i] = l.list[i];
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline elem_list::~elem_list()
{
 erase();
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void elem_list::create()
{
 dimension = 0;
 number    = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void elem_list::create(const int dim)
{
 if (dimension != dim) {
   erase();
   dimension = dim;
   list = new elem[dimension];
 }
 number = 0;
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void elem_list::copy(const elem_list & l)
{
 create(l.dimension);
 number = l.number;
 for (int i=0; i<number; i++)
   list[i] = l.list[i];
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void elem_list::erase()
{
 if (dimension != 0) {
   delete[] list;
   dimension = 0;
   number    = 0;
 }
}

     //--------------------------------------------------------------------------//
     //               Maximum number of elements in the list                     //
     //--------------------------------------------------------------------------//
inline int elem_list::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                       No element in the list                             //
     //--------------------------------------------------------------------------//
inline bool elem_list::is_empty() const
{
 return(number == 0);
}
     //--------------------------------------------------------------------------//
     //                    No empty places in the list                           //
     //--------------------------------------------------------------------------//
inline bool elem_list::is_full() const
{
 return(number == dimension);
}
     //--------------------------------------------------------------------------//
     //               Current number of elements in the list                     //
     //--------------------------------------------------------------------------//
inline int elem_list::has() const
{
 return(number);
}
     //--------------------------------------------------------------------------//
     //              Current number of empty places in the list                  //
     //--------------------------------------------------------------------------//
inline int elem_list::has_not() const
{
 return(dimension - number);
}
     //--------------------------------------------------------------------------//
     //                     The p-th element of the list                         //
     //--------------------------------------------------------------------------//
inline elem elem_list::pos(const int p) const
{
#ifdef _PEPS_DEBUG_
  if (p >= number)
    Programming_Error("index out of range", 3201)
#endif
 return(list[p]);
}

     //--------------------------------------------------------------------------//
     //                 It removes all elements from the list                    //
     //--------------------------------------------------------------------------//
inline void elem_list::take_out()
{
 number = 0;
}
