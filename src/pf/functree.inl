//====================================================================================//
//                                                                                    //
//                              Class Function Tree                                   //
//                                                                                    //
//====================================================================================//
//  This File:   functree_inl.h             Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 4100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 21/mar/98                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline func_tree::func_tree()
{
 dimension = 0;
 last      = 0;
 mark      = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline func_tree::func_tree(const int dim)
{
 dimension = dim;
 last      = 0;
 mark      = 0;
 list      = new token[dimension];
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline func_tree::func_tree(const func_tree & t)
{
 dimension = t.dimension;
 list      = new token[dimension];
 for (last=0; last<t.last; last++) {
   list[last].op  = t.list[last].op;
   list[last].val = t.list[last].val;
   list[last].beg = t.list[last].beg;
   list[last].end = t.list[last].end;
 }
 mark = t.mark;
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline func_tree::~func_tree()
{
 erase();
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void func_tree::create()
{
 dimension = 0;
 last      = 0;
 mark      = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void func_tree::create(const int dim)
{
 if (dimension != dim) {
   erase();
   dimension = dim;
   list = new token[dimension];
 }
 last = 0;
 mark = 0;
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void func_tree::copy(const func_tree & t)
{
 create(t.dimension);
 for (last=0; last<t.last; last++) {
   list[last].op  = t.list[last].op;
   list[last].val = t.list[last].val;
   list[last].beg = t.list[last].beg;
   list[last].end = t.list[last].end;
 }
 mark = t.mark;
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void func_tree::erase()
{
 if (dimension != 0) {
   delete[] list;
   dimension = 0;
   last      = 0;
   mark      = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                     The maximum number of tokens                         //
     //--------------------------------------------------------------------------//
inline int func_tree::size() const
{
 return(dimension);
}
     //--------------------------------------------------------------------------//
     //                     The current number of tokens                         //
     //--------------------------------------------------------------------------//
inline int func_tree::has() const
{
 return(last);
}
     //--------------------------------------------------------------------------//
     //                 The current number of tokens is zero                     //
     //--------------------------------------------------------------------------//
inline bool func_tree::is_empty() const
{
 return(last == 0);
}
     //--------------------------------------------------------------------------//
     //                  'mark' points to the head of 'this'                     //
     //--------------------------------------------------------------------------//
inline void func_tree::reset()
{
 mark = 0; 
}
     //--------------------------------------------------------------------------//
     //                'mark' points to the next token of 'this'                 //
     //--------------------------------------------------------------------------//
inline void func_tree::next()
{
#ifdef _PEPS_DEBUG_
  if (mark == last)
    Programming_Error("bad next call", 4107)
#endif
 mark++; 
}

     //--------------------------------------------------------------------------//
     //         It returns the operation of the token pointed by 'mark'          //
     //--------------------------------------------------------------------------//
inline oper func_tree::get_op() const
{
#ifdef _PEPS_DEBUG_
  if (mark == last)
    Programming_Error("bad get operation call", 4108)
#endif
 return(list[mark].op);
}
     //--------------------------------------------------------------------------//
     //           It returns the operand of the token pointed by 'mark'          //
     //--------------------------------------------------------------------------//
inline rp func_tree::get_val() const
{
#ifdef _PEPS_DEBUG_
  if (mark == last)
    Programming_Error("bad get operand call", 4109)
#endif
 return(list[mark].val);
}
     //--------------------------------------------------------------------------//
     //        It returns the 'beg' field of the token pointed by 'mark'         //
     //--------------------------------------------------------------------------//
inline int func_tree::get_beg() const
{
#ifdef _PEPS_DEBUG_
  if (mark == last)
    Programming_Error("bad get operation call", 4110)
#endif
 return(list[mark].beg);
}
     //--------------------------------------------------------------------------//
     //         It returns the 'end' field of the token pointed by 'mark'        //
     //--------------------------------------------------------------------------//
inline int func_tree::get_end() const
{
#ifdef _PEPS_DEBUG_
  if (mark == last)
    Programming_Error("bad get operand call", 4111)
#endif
 return(list[mark].end);
}
