//====================================================================================//
//                                                                                    //
//                                 Class Booleans                                     //
//                                                                                    //
//====================================================================================//
//  This File:   bools_inl.h                Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline bools::bools()
{
 dimension = 0;
 words     = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline bools::bools(const int dim)
{
 dimension = dim;
 words     = int((dim - 1) / word_size) + 1;
 values    = new unsigned int[words];
 for (int i=0; i<words; i++)
   values[i] = all_zero;
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline bools::bools(const bools & v)
{
 dimension = v.dimension;
 words     = v.words;
 values    = new unsigned int[words];
 for (int i=0; i<words; i++)
   values[i] = v.values[i];
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline bools::~bools()
{
 if (words != 0) {
   delete[] values;
   dimension = 0;
   words     = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void bools::create()
{
 dimension = 0;
 words     = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void bools::create(const int dim)
{
 if (dimension != dim) {
   erase();
   dimension = dim;
   words     = int((dim - 1) / word_size) + 1;
   values    = new unsigned int[words];
   for (int i=0; i<words; i++)
     values[i] = all_zero;
 }
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void bools::copy(const bools & v)
{
 create(v.dimension);
 for (int i=0; i<words; i++)
   values[i] = v.values[i];
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void bools::erase()
{
 if (words != 0) {
   delete[] values;
   dimension = 0;
   words     = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                      The array size (logical)                            //
     //--------------------------------------------------------------------------//
inline int bools::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                       All booleans are true                              //
     //--------------------------------------------------------------------------//
     //   Implementation: The non used bit of the last word are set to 'true'    //
     // with an "or" with a mask with the superior bits equal to one.            //
     //                                                                          //
     //   The expression 'dimension % word_size' gives the position of the first //
     // non used bit of the last word (the index to choose the correct mask).    //
     //--------------------------------------------------------------------------//
inline bool bools::is_full() const
{
 bool answer = true;

 values[words-1] = values[words-1] | mask1100[dimension % word_size];
 for (int i=0; i<words; i++)
   if (values[i] != all_one) {
     answer = false;
     break;
   }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                       All booleans are false                             //
     //--------------------------------------------------------------------------//
     //   Implementation: The non used bit of the last word are set to 'false'   //
     // with an "and" with a mask with the superior bits equal to zero.          //
     //                                                                          //
     //   The expression 'dimension % word_size' gives the position of the first //
     // non used bit of the last word (the index to choose the correct mask).    //
     //--------------------------------------------------------------------------//
inline bool bools::is_empty() const
{
 bool answer = true;

 values[words-1] = values[words-1] & mask0011[dimension % word_size];
 for (int i=0; i<words; i++)
   if (values[i] != all_zero) {
     answer = false;
     break;
   }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                          The number of trues                             //
     //--------------------------------------------------------------------------//
     //   Implementation: A boolean element of index 'i' is stored in the word   //
     // indexed by 'int(i / word_size)' (the integer division by the size of the //
     // word).                                                                   //
     //   The position of 'i' inside the word is given by 'i % word_size' (the   //
     // rest of the integer division by the size of the word).                   //
     //                                                                          //
     //   The boolean value of the position 'i' is performed by a logical "and"  //
     // between the word which contains the boolean value and the mask having a  //
     // single bit equal to one for the position of the boolean 'i'.             //
     //--------------------------------------------------------------------------//
inline int bools::has() const
{
 int answer=0;

 for (int i=0; i<dimension; i++)
   if ((values[int(i / word_size)] & mask0010[i % word_size]) != all_zero)
     answer++;
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                      Puts true in all positions                          //
     //--------------------------------------------------------------------------//
inline void bools::put()
{
 for (int i=0; i<words; i++)
   values[i] = all_one;
}
     //--------------------------------------------------------------------------//
     //                     Puts false in all positions                          //
     //--------------------------------------------------------------------------//
inline void bools::take()
{
 for (int i=0; i<words; i++)
   values[i] = all_zero;
}
     //--------------------------------------------------------------------------//
     //              Puts the boolean value 'v' in all position                  //
     //--------------------------------------------------------------------------//
inline void bools::assign(const bool v)
{
 unsigned int val;

 if (v)
   val = all_one;
 else
   val = all_zero;
 for (int i=0; i<words; i++)
   values[i] = val;
}

     //--------------------------------------------------------------------------//
     //                   Puts true at the position 'p'                          //
     //--------------------------------------------------------------------------//
     //   Implementation: The boolean element of index 'i' is stored in the word //
     // indexed by 'int(i / word_size)' (the integer division by the size of the //
     // word).                                                                   //
     //   The position of 'i' inside the word is given by 'i % word_size' (the   //
     // rest of the integer division by the size of the word).                   //
     //                                                                          //
     //   The assignment of the boolean in position 'p' is performed by a        //
     // logical "or" between the word which contains the boolean value and the   //
     // the mask having a single bit equal to one for the position of the        //
     // boolean 'p'.                                                             //
     //--------------------------------------------------------------------------//
inline void bools::put(const int p)    // see 'The number of true values' implementation
{
#ifdef _PEPS_DEBUG_
  if (p >= dimension)
    Programming_Error("index out of range", 2101)
#endif
 values[int(p / word_size)] = values[int(p / word_size)] | mask0010[p % word_size];
}
     //--------------------------------------------------------------------------//
     //                   Puts false at the position 'p'                         //
     //--------------------------------------------------------------------------//
     //   Implementation: A boolean element of index 'i' is stored in the word   //
     // indexed by 'int(i / word_size)' (the integer division by the size of the //
     // word).                                                                   //
     //   The position of 'i' inside the word is given by 'i % word_size' (the   //
     // rest of the integer division by the size of the word).                   //
     //                                                                          //
     //   The assignment of the boolean in position 'p' is performed by a        //
     // logical "and" between the word which contains the boolean value and the  //
     // the mask having a single bit equal to zero for the position of the       //
     // boolean 'p'.                                                             //
     //--------------------------------------------------------------------------//
inline void bools::take(const int p)   // see 'The number of true values' implementation
{
#ifdef _PEPS_DEBUG_
  if (p >= dimension)
    Programming_Error("index out of range", 2102)
#endif
 values[int(p / word_size)] = values[int(p / word_size)] & mask1101[p % word_size];
}
     //--------------------------------------------------------------------------//
     //            Puts the boolean value 'v' at the position 'p'                //
     //--------------------------------------------------------------------------//
     //   Implementation: see above.                                             //
     //--------------------------------------------------------------------------//
inline void bools::assign(const bool v, const int p)
{
#ifdef _PEPS_DEBUG_
  if (p >= dimension)
    Programming_Error("index out of range", 2103)
#endif
 if (v)
   values[int(p / word_size)] = values[int(p / word_size)] | mask0010[p % word_size];
 else
   values[int(p / word_size)] = values[int(p / word_size)] & mask1101[p % word_size];
}

     //--------------------------------------------------------------------------//
     //                The boolean value of the position 'p'                     //
     //--------------------------------------------------------------------------//
     //   Implementation: A boolean element of index 'i' is stored in the word   //
     // indexed by 'int(i / word_size)' (the integer division by the size of the //
     // word).                                                                   //
     //   The position of 'i' inside the word is given by 'i % word_size' (the   //
     // rest of the integer division by the size of the word).                   //
     //                                                                          //
     //   The boolean value of the position 'p' is performed by a logical "and"  //
     // between the word which contains the boolean value and the mask having a  //
     // single bit equal to one for the position of the boolean 'p'.             //
     //--------------------------------------------------------------------------//
inline bool bools::operator [] (const int p) const
{
#ifdef _PEPS_DEBUG_
  if (p >= dimension)
    Programming_Error("index out of range", 2104)
#endif
 unsigned int filter = values[int(p / word_size)] & mask0010[p % word_size];
 return(filter != all_zero);
}
