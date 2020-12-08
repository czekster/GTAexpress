//====================================================================================//
//                                                                                    //
//                                Class SAN Element                                   //
//                                                                                    //
//====================================================================================//
//  This File:   elem.inl                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 07/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline elem::elem()
{}
     //--------------------------------------------------------------------------//
     //                        Defined Constructor                               //
     //--------------------------------------------------------------------------//
inline elem::elem(const elem_type t, const rp v, const func_id i)
{
 type = t;
 val  = v;      // for a functional element the default value used is 'zero'
 id   = i;      // for a constant element the default value used is 'no_id'
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline elem::elem(const elem & e)
{
 type = e.type;
 val  = e.val;
 id   = e.id;
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline elem::~elem()
{}

     //--------------------------------------------------------------------------//
     //                   The SAN element 'this' is constant                     //
     //--------------------------------------------------------------------------//
inline bool elem::is_const() const
{
 return(type != Func);
}
     //--------------------------------------------------------------------------//
     //                  The SAN element 'this' is functional                    //
     //--------------------------------------------------------------------------//
inline bool elem::is_func() const
{
 return(type == Func);
}

     //--------------------------------------------------------------------------//
     //                Fills 's' with the dependencies of 'this'                 //
     //--------------------------------------------------------------------------//
inline void elem::params(aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.g_aut())   // it tests if the number of automata in 's' the
                                      // current number of automata of the model (given
                                      // by the function 'g_aut' of the class 'FTB')
    Programming_Error("bad params call", 3101)
#endif
 if (type == Func)
   FTB::ftb.g_params(id, s);
 else
   s.take_out();
}

     //--------------------------------------------------------------------------//
     //                Evaluates 'this' for a global state 's'                   //
     //--------------------------------------------------------------------------//
inline rp elem::get(const aut_st & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("bad get call", 3102)
#endif
 rp answer;
 if (type == Func)
   answer = FTB::ftb.eval(id, s);
 else
   answer = val;
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                 It returns the field 'val' of 'this'                     //
     //--------------------------------------------------------------------------//
inline rp elem::get() const
{
 return(val);
}
     //--------------------------------------------------------------------------//
     //                 It returns the field 'id' of 'this'                      //
     //--------------------------------------------------------------------------//
inline func_id elem::f_get() const
{
 return(id);
}
     //--------------------------------------------------------------------------//
     //                       It re-defines 'this'                               //
     //--------------------------------------------------------------------------//
inline void elem::put(const elem_type t, const rp v, const any_id i)
{
 type = t;
 val  = v;
 id   = i;
}

     //--------------------------------------------------------------------------//
     //                     Evaluation of a SAN element                          //
     //--------------------------------------------------------------------------//
     //    The next four functions fill the field 'val' with the evaluation of   //
     // 'this' to a given global state. The first two functions receive the      //
     // global state diretcly in the input parameter 's' (the size of 's' must   //
     // be equal to the number of original automata - defined in 'FTB::ftb').    //
     // The last two functions receive the global state in two input parameters. //
     // The parameter 's' indicates the global state according to the current    //
     // number of automata (defined in 'DSC::dsc' or 'CND::cnd'). To generate    //
     // the global state according to the number of original automata (need to   //
     // the evaluation) an array of state maps 'm' is used.                      //
     //--------------------------------------------------------------------------//
     //            Global state with the number of original automata             //
     //                         (for any SAN element)                            //
     //--------------------------------------------------------------------------//
inline void elem::eval(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong global state size", 3103)
#endif
 if (type == Func)
   val = FTB::ftb.eval(id, s);
}
     //--------------------------------------------------------------------------//
     //            Global state with the number of original automata             //
     //                     (for functional SAN elements)                        //
     //--------------------------------------------------------------------------//
inline void elem::f_eval(const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong global state size", 3104)
  if (type != Func)
    Programming_Error("bad function evaluate call", 3105)
#endif
 val = FTB::ftb.eval(id, s);
}
     //--------------------------------------------------------------------------//
     //             Global state with the current number of automata             //
     //                         (for any SAN element)                            //
     //--------------------------------------------------------------------------//
inline void elem::eval(const aut_st & s, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong global state size", 3106)
#endif
 if (type == Func)
   val = FTB::ftb.g_eval(id, s, m);
}
     //--------------------------------------------------------------------------//
     //             Global state with the current number of automata             //
     //                     (for functional SAN elements)            Traditional //
     //--------------------------------------------------------------------------//
inline void elem::f_eval(const aut_st & s, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong global state size", 3107)
  if (type != Func)
    Programming_Error("bad function evaluate call", 3108)
  if (id >= FTB::ftb.has())
    Programming_Error("function index out of range", 3109)
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong global state size", 3110)
#endif
 val = FTB::ftb.g_eval(id, s, m);
}
     //--------------------------------------------------------------------------//
     //             Global state with the current number of automata             //
     //                     (for functional SAN elements)                  'jit' //
     //--------------------------------------------------------------------------//
inline void elem::jit_f_eval(const aut_st & s, const st_map * m)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong global state size", 3111)
  if (type != Func)
    Programming_Error("bad function evaluate call", 3112)
  if (id >= FTB::ftb.has())
    Programming_Error("function index out of range", 3113)
  if (s.size() != FTB::ftb.g_aut())
    Programming_Error("wrong global state size", 3114)
#endif
 val = FTB::ftb.jit_g_eval(id, s, m);
}

     //--------------------------------------------------------------------------//
     //              Assign Operators  -  It assigns a rate                      //
     //--------------------------------------------------------------------------//
inline void elem::operator = (const rp v)
{
 type = Rate;
 val  = v;
 id   = no_id;
}
     //--------------------------------------------------------------------------//
     //              Assign Operators  -  It assigns a function id.              //
     //--------------------------------------------------------------------------//
inline void elem::operator = (const func_id i)
{
 type = Func;
 val  = 0.0;
 id   = i;
}
     //--------------------------------------------------------------------------//
     //              Assign Operators  -  It assigns another SAN Element         //
     //--------------------------------------------------------------------------//
inline void elem::operator = (const elem e)
{
 type = e.type;
 val  = e.val;
 id   = e.id;
}
     //--------------------------------------------------------------------------//
     //                          Logical Operators                               //
     //--------------------------------------------------------------------------//
     //   Principles: - Two elements of different types are different;           //
     //               - Less than or Greater than operators compares 'this' to   //
     //               'zero',i.e., 'v' must be equal to 'zero';                  //
     //               - A functional element is greater than zero if and only if //
     //               one of its values is strictly positive. Remember that each //
     //               entry of a rate matrix must be either always positive or   //
     //               always negative (if diagonal element);                     //
     //--------------------------------------------------------------------------//

     //--------------------------------------------------------------------------//
     //              Logical Operators  -  Equality                              //
     //--------------------------------------------------------------------------//
inline bool elem::operator == (const rp v) const
{
 bool answer = false;

 if (type != Func)                // Two elements of different types cannot be equal
   answer = (val == v);
 return(answer);
}
     //--------------------------------------------------------------------------//
     //              Logical Operators  -  Equality                              //
     //--------------------------------------------------------------------------//
inline bool elem::operator == (const elem e) const
{
 bool answer = false;

 if (type == e.type)              // Two elements of different types cannot be equal
   if (type == Func)
     answer = (id == e.id);                      // functional elements
   else
     answer = (val == e.val);                    // constant elements
 return(answer);
}
     //--------------------------------------------------------------------------//
     //              Logical Operators  -  Inequality                            //
     //--------------------------------------------------------------------------//
inline bool elem::operator != (const rp v) const
{
 bool answer = true;

 if (type != Func)                // Two elements of different types cannot be equal
   answer = (val != v);
 return(answer);
}
     //--------------------------------------------------------------------------//
     //              Logical Operators  -  Inequality                            //
     //--------------------------------------------------------------------------//
inline bool elem::operator != (const elem e) const
{
 bool answer = true;

 if (type == e.type)              // Two elements of different types cannot be equal
   if (type == Func)
     answer = (id != e.id);                      // functional elements
   else
     answer = (val != e.val);                    // constant elements
 return(answer);
}
     //--------------------------------------------------------------------------//
     //              Logical Operators  -  Less than (negative)                  //
     //--------------------------------------------------------------------------//
inline bool elem::operator < (const rp v) const
{
#ifdef _PEPS_DEBUG_
  if (v != zero)
    Programming_Error("bad less than operator call", 3115)
#endif
 bool answer = true;

 if (type != Func)
   answer = (val < v);                          // constant elements
 else
   answer = (FTB::ftb.nonzero_eval(id) < v);    // functional elements
 return(answer);
}
     //--------------------------------------------------------------------------//
     //              Logical Operators  -  Greater than (positive)               //
     //--------------------------------------------------------------------------//
inline bool elem::operator > (const rp v) const
{
#ifdef _PEPS_DEBUG_
  if (v != zero)
    Programming_Error("bad greater than operator call", 3116)
#endif
 bool answer = true;

 if (type != Func)                              // constant elements
   answer = (val > v);
 else
   answer = (FTB::ftb.nonzero_eval(id) > v);    // functional elements
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                        Arithmetic Operators                              //
     //--------------------------------------------------------------------------//
     //   The following eight operators perform operations on the SAN elements   //
     // regardless if the elements are constant or functional. To do that these  //
     // functions MUST use the static structures Table of Functions ('FTB::ftb') //
     // and Scratch Table of Functions ('scr_ft::sft'). Basically, the operators //
     // search the function tree object in the original table ('FTB::ftb') and   //
     // store the result into the new table ('scr_ft::sft'), i.e., the operators //
     // applyied to functional elements create new functions which must be       //
     // stored into a new table ('scr_ft::sft').                                 //
     //--------------------------------------------------------------------------//

     //--------------------------------------------------------------------------//
     //          Arithmetic Operators  -  Add  -  it returns 'this' + 'v'        //
     //--------------------------------------------------------------------------//
inline elem elem::operator + (const rp v) const
{
 elem answer;

 if (type == Func)
   if (v == zero)
     answer = scr_ft::sft.sto(id);            // if 'this' is functional and 'v' == zero
   else
     answer = scr_ft::sft.add(id, v);         // if 'this' is functional and 'v' != zero
 else
  answer.val = val + v;                       // if 'this' is constant
 return(answer);
}
     //--------------------------------------------------------------------------//
     //           Arithmetic Operators  -  Add  -  it returns 'this' + 'e'       //
     //--------------------------------------------------------------------------//
inline elem elem::operator + (const elem e) const
{
 func_id temp[2];
 elem answer;

 if (type == Func)
   if (e == zero)
     answer = scr_ft::sft.sto(id);            // if 'this' is functional and 'e' == zero
   else
     if (e.type == Func){
       temp[0] = id;
       temp[1] = e.id;
       answer = scr_ft::sft.add(temp, 2, 0);    // if 'this' and 'e' are functional
     }
     else
       answer = scr_ft::sft.add(id, e.val);   // if 'this' is funct. and 'e' is const.
 else
   if (e.type == Func)
     if (val == zero)
       answer = scr_ft::sft.sto(e.id);        // if 'e' is funct. and 'this' is const.
     else
       answer = scr_ft::sft.add(e.id, val);   // if 'e' is functional and 'this' == zero
   else
     answer.val = val + e.val;                // if 'this' and 'e' are constant
 return(answer);
}
     //--------------------------------------------------------------------------//
     //         Arithmetic Operators  -  Multiply  -  it returns 'this' * 'v'    //
     //--------------------------------------------------------------------------//
inline elem elem::operator * (const rp v) const
{
 elem answer(Rate, zero, no_id);              // if 'this' or 'v' is equal to zero

 if ((v != zero) && (*this != zero))
   if (type == Func)
     if (v == one)
       answer = scr_ft::sft.sto(id);          // if 'this' is functional and 'v' == one
     else
       answer = scr_ft::sft.mul(id, v);       // if 'this' is functional and 'v' != one
   else
     answer.val = val * v;                    // if 'this' is constant
 return(answer);
}
     //--------------------------------------------------------------------------//
     //         Arithmetic Operators  -  Multiply  -  it returns 'this' * 'e'    //
     //--------------------------------------------------------------------------//
inline elem elem::operator * (const elem e) const
{
 func_id temp[2];
 elem answer(Rate, zero, no_id);              // if 'this' or 'e' is equal to zero

 if ((*this != zero) && (e != zero))
   if (type == Func)
     if (e == one)
       answer = scr_ft::sft.sto(id);          // if 'this' is functional and 'e' == one
     else
       if (e.type == Func){
         temp[0] = id;
         temp[0] = e.id;
         answer = scr_ft::sft.mul(temp, 2, 1);      // if 'this' and 'e' are functional
       }
       else
         answer = scr_ft::sft.mul(id, e.val); // if 'this' is funct. and 'e' is const.
   else
     if (e.type == Func)
       if (val == one)
         answer = scr_ft::sft.sto(e.id);      // if 'e' is functional and 'this' == one
       else
         answer = scr_ft::sft.mul(e.id, val); // if 'e' is funct. and 'this' is const.
     else
       answer.val = val * e.val;              // if 'this' and 'e' are constant
 return(answer);
}
     //--------------------------------------------------------------------------//
     //        Arithmetic Operators  -  Acumulator Add   -  'this' += 'v'        //
     //--------------------------------------------------------------------------//
inline void elem::operator += (const rp v)
{
 if (type == Func)
   if (v == zero)
     id = scr_ft::sft.sto(id);                // if 'this' is functional and 'v' == zero
   else
     id = scr_ft::sft.add(id, v);             // if 'this' is functional and 'v' != zero
 else
   val += v;                                  // if 'this' is constant
}
     //--------------------------------------------------------------------------//
     //         Arithmetic Operators  -  Acumulator Add  -  'this' += 'e'        //
     //--------------------------------------------------------------------------//
inline void elem::operator += (const elem e)
{
 func_id temp[2];
 
 if (type == Func)
   if (e == zero)
     id = scr_ft::sft.sto(id);                // if 'this' is functional and 'e' == zero
   else
     if (e.type == Func){
       temp[0] = id;
       temp[1] = e.id;
       id = scr_ft::sft.add(temp, 2, 0);    // if 'this' and 'e' are functional
     }
     else
       id = scr_ft::sft.add(id, e.val);       // if 'this' is funct. and 'e' is const.
 else
   if (e.type == Func) {
     type = Func;
     if (val == zero)
       id = scr_ft::sft.sto(e.id);            // if 'e' is functional and 'this' == zero
     else
       id = scr_ft::sft.add(e.id, val);       // if 'e' is funct. and 'this' is const.
     val = zero;
   }
   else
     val += e.val;                            // if 'this' and 'e' are constant
}
     //--------------------------------------------------------------------------//
     //      Arithmetic Operators  -  Acumulator Multiply  -  'this' *= 'v'      //
     //--------------------------------------------------------------------------//
inline void elem::operator *= (const rp v)
{
 if ((*this == zero) || (v == zero)) {        // if 'this' or 'v' == zero
   type = Rate;
   val  = zero;
   id   = no_id;
 }
 else
   if (type == Func)
     if (v == one)
       id = scr_ft::sft.sto(id);              // if 'this' is functional and 'v' == one
     else
       id = scr_ft::sft.mul(id, v);           // if 'this' is functional and 'v' != one
   else
     val *= v;                                // if 'this' is constant
}
     //--------------------------------------------------------------------------//
     //       Arithmetic Operators  -  Acumulator Multiply  -  'this' *= 'e'     //
     //--------------------------------------------------------------------------//
inline void elem::operator *= (const elem e)
{
 func_id temp[2];
 
 if ((*this == zero) || (e == zero)) {        // if 'this' or 'e' == zero
   type = Rate;
   val  = zero;
   id   = no_id;
 }
 else
   if (type == Func)
     if (e == one)
       id = scr_ft::sft.sto(id);              // if 'this' is functional and 'e' == one
     else
       if (e.type == Func){
         temp[0] = id;
         temp[0] = e.id;
         id = scr_ft::sft.mul(temp, 2, 1);      // if 'this' and 'e' are functional
       }
       else
         id = scr_ft::sft.mul(id, e.val);     // if 'this' is funct. and 'e' is const.
   else
     if (e.type == Func) {
       type = Func;
       if (val == one)
         id = scr_ft::sft.sto(e.id);          // if 'e' is functional and 'this' == one
       else
         id = scr_ft::sft.mul(e.id, val);     // if 'e' is funct. and 'this' is const.
       val  = zero;
     }
     else
       val *= e.val;                          // if 'this' and 'e' are constant
}

