//====================================================================================//
//                                                                                    //
//                                Class SAN Element                                   //
//                                                                                    //
//====================================================================================//
//  This File:   elem.C                     Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/feb/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                      Estimation of a SAN element                         //
     //--------------------------------------------------------------------------//
     //    This functions fills the 'val' field of 'this' with an estimation     //
     // made by a specific criteria.                                             //
     //                                                                          //
     //    The input parameters are:                                             //
     //      - the coordinates of where this SAN element appears:                //
     //           'e' is the sync. term (the constant 'no_ev' means local term); //
     //           'a' is the automata;                                           //
     //           'i' is the row;                                                //
     //      - the state maps to obtain the original states coordinates ('s');   //
     //      - function estimation police ('t'):                                 //
     //           'lu_user'  - dialogue with the user;                           //
     //           'lu_max'    - biggest value;                                    //
     //           'lu_min'   - smallest value;                                   //
     //           'lu_ave'   - average value;                                    //
     //           'lu_zero'  - replace the function by zero;                     //
     //--------------------------------------------------------------------------//
     //  This function has two versions, the first just performs the estimation. //
     // The second version also returns in the output parameter 's' the global   //
     // used to the estimation (this version is only used when the estimation    //
     // police is dialogue with the user ('lu_user').                            //
     //--------------------------------------------------------------------------//
void elem::estimate(const ev_id e, const aut_id a, const st_id i,
                    const st_map * m, const f_aprox t)
{
 rp      no_min, no_ave, no_max;
 aut_st  states;
 aut_set known;

 states.create(FTB::ftb.aut());    // creates the object for the values
 states.reset();
 m[a].get(i, states);              // get the automata states corresponding to row 'i'
 known.create(FTB::ftb.aut());     // creates the set of known automata
 known.take_out();
 m[a].get(known);                  // gets the known automata

 switch (t) {
   case lu_user : Ask_Partial_State(states, known, id, e, a);
                  val = FTB::ftb.eval(id, states);
                  break;
   case lu_max  : FTB::ftb.eval(id, states, known, no_min, no_ave, val);
                  break;
   case lu_min  : FTB::ftb.eval(id, states, known, val,    no_ave, no_max);
                  break;
   case lu_ave  : FTB::ftb.eval(id, states, known, no_min, val,    no_max);
                  break;
   case lu_zero : val = zero;
 }
}
     //--------------------------------------------------------------------------//
     //                     Estimation - second version                          //
     //--------------------------------------------------------------------------//
void elem::estimate(const ev_id e, const aut_id a, const st_id i,
                    const st_map * m, const f_aprox t, aut_st & s)
{
 rp      no_min, no_ave, no_max;
 aut_st  states;
 aut_set known;

 states.create(FTB::ftb.aut());    // creates the object for the values
 states.reset();
 m[a].get(i, states);              // get the automata states corresponding to row 'i'
 known.create(FTB::ftb.aut());     // creates the set of known automata
 known.take_out();
 m[a].get(known);                  // gets the known automata

 switch (t) {
   case lu_user : Ask_Partial_State(states, known, id, e, a);
                  val = FTB::ftb.eval(id, states);
                  break;
   case lu_max  : FTB::ftb.eval(id, states, known, no_min, no_ave, val);
                  break;
   case lu_min  : FTB::ftb.eval(id, states, known, val,    no_ave, no_max);
                  break;
   case lu_ave  : FTB::ftb.eval(id, states, known, no_min, val,    no_max);
                  break;
   case lu_zero : val = zero;
 }
 s.copy(states);
}

     //--------------------------------------------------------------------------//
     //              It tries to eliminate a functional element                  //
     //--------------------------------------------------------------------------//
     //   Denote: P   the set of parameters of 'this' (automata);                //
     //           S   the set of automata whose the states are known;            //
     //           SP  the intersection of P and S;                               //
     //           R   the set of automata state values for the automata in SP;   //
     //                                                                          //
     //   This function tests if 'this' is a constant on R.                      //
     //                                                                          //
     //   Input parameters: 's' the set S denoted above;                         //
     //                     'v' the set R (restricted to automata in SP);        //
     //                                                                          //
     //   All the possible values in R of the function are computed. If the same //
     // evaluation is found, the SAN element ('this') is changed to a constant   //
     // and the evaluation is stored in 'val'. Otherwise, 'this' is unchanged.   //
     //                                                                          //
     // Note: if 'this' is constant ('type' = 'Rate') the function does nothing. //
     //                                                                          //
     // Note: the maximum size of 's' and the size of 'v' must be equal to the   //
     // number of automata in the original model. It means that in 'v' only the  //
     // values of the automata in 's' are relevant.                              //
     //--------------------------------------------------------------------------//
void elem::solve(const aut_set & s, const aut_st & v)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong sized parameters on a solve call", 3109)
  if (v.size() != FTB::ftb.aut())
    Programming_Error("wrong sized parameters on a solve call", 3110)
#endif
 if (type == Func)
   if (FTB::ftb.eval(id, v, s, val)) {       // 'FTB::ftb.eval' return 'true'
     id   = no_id;                           // if the conditions above are fulfilled
     type = Rate;                            // and 'val' is the constant value
   }
   else
     val = 0.0;
}

     //--------------------------------------------------------------------------//
     //    Array Operators  -  It assigns another SAN Element  -  'this' = 'e'   //
     //--------------------------------------------------------------------------//
void elem::sto(const elem e)
{
 if (e.type == Func) {                // 'e' is functional
   type = Func;
   val  = zero;
   id   = scr_ft::sft.sto(e.id);
 }
 else {                               // 'e' is constant
   type = e.type;
   val  = e.val;
   id   = no_id;
 }
}
     //--------------------------------------------------------------------------//
     //   Array Operators  -  It adds 'n' SAN elements  -  'this' = sum of e[]   //
     //--------------------------------------------------------------------------//
void elem::add(const elem e[], const int n)
{
 rp        acc = zero;                  // the sum of all constant 'e[]' elements
 int       f = 0;                       // the number of functional elements in 'e[]'
 func_id * func_list = new func_id[n];  // the array of function id's in 'e[]'

 for (int i=0; i<n; i++)                // for all 'e[]' elements
   if (e[i].type == Func) {                // if 'e[i]' functional
     func_list[f] = e[i].id;               //   count the functional elements
     f++;                                  //   and stores their id's in 'func_list'
   }
   else                                    // if 'e[i]' constant
     acc += e[i].val;                      //   adds it to the accumulator 'acc'

 if (f > 0) {                           // if 'e[]' has at least one functional element
   type = Func;                         //   'this' becomes a functional element
   val  = zero;
   id   = scr_ft::sft.add(func_list, f, acc);  // creates the new function
 }
 else {                                 // if 'e[]' does not have functional elements
   type = Rate;                         //   'this' becomes a constant element
   val  = acc;
   id   = no_id;
 }
 delete[] func_list;
}
     //--------------------------------------------------------------------------//
     //Array Operators - It multiplies 'n' SAN elements - 'this' = product of e[]//
     //--------------------------------------------------------------------------//
void elem::mul(const elem e[], const int n)
{
 rp        acc = one;                  // the product of all constant 'e[]' elements
 int       f = 0;                      // the number of functional elements in 'e[]'
 func_id * func_list = new func_id[n]; // the array of function id's in 'e[]'

 for (int i=0; i<n; i++)               // for all 'e[]' elements
   if (e[i].type == Func) {               // if 'e[i]' functional
     func_list[f] = e[i].id;              //   count the functional elements
     f++;                                 //   and stores their id's in 'func_list'
   }
   else                                   // if 'e[i]' constant
     acc *= e[i].val;                     //   multiplies it to the accumulator 'acc'

 if (acc == zero) {                    // if 'acc' is equal to zero
   type = Rate;                        //   'this' becomes zero (constant)
   val  = zero;
   id   = no_id;
 }
 else
   if (f > 0) {                        // if 'e[]' has at least one functional element
     type = Func;                      //   'this' becomes a functional element
     val = zero;
     id = scr_ft::sft.mul(func_list, f, acc);   // creates the new function
   }
   else {                              // if 'e[]' does not have functional elements
     type = Rate;                      //   'this' becomes a constant element
     val  = acc;
     id   = no_id;
   }
 delete[] func_list;
}

     //--------------------------------------------------------------------------//
     //                        Print on 'fout' file stream                       //
     //--------------------------------------------------------------------------//
void elem::print(ofstream & fout) const
{
  if (type == Func) {
    FTB::ftb.print(id, fout);
    fout << " (F" << id << ")\n";
  }
  else {
    fout << val;
    if (!CND::cnd.is_empty())
      fout << " (" << val / CND::cnd.n_factor() << ")\n";
  }
}

     //--------------------------------------------------------------------------//
     //                       Output on 'fout' file stream                       //
     //--------------------------------------------------------------------------//
void elem::write(ofstream & fout) const
{
 fout << type << " " << val << " " << id << "\n";
}
     //--------------------------------------------------------------------------//
     //                       Output on cout                                     //
     //--------------------------------------------------------------------------//
void elem::write() const
{
 cout << type << " " << val << " " << id << "\n";
}
     //--------------------------------------------------------------------------//
     //                       Input from 'fin' file stream                       //
     //--------------------------------------------------------------------------//
void elem::read(ifstream & fin)
{
 int buf;

 fin >> buf;
 type = elem_type(buf);
 fin >> val;
 fin >> id;
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int elem::mem_use() const
{
 int answer = 16;
 return(answer);
}
