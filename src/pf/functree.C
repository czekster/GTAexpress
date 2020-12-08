//====================================================================================//
//                                                                                    //
//                              Class Function Tree                                   //
//                                                                                    //
//====================================================================================//
//  This File:   functree.C                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 4100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/feb/01                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                 It verifies if 'this' is well defined                    //
     //--------------------------------------------------------------------------//
     //   The input parameter 'verbose' with 'true' causes the output of error   //
     //      messages in the standart error output filestream ('cerr').          //
     //--------------------------------------------------------------------------//
bool func_tree::well_def(const bool verbose) const
{
 bool answer = true;
 int  top = -1;         // the top of the stack
 int  max_top = 0;      // the maximum value reached by 'top'

 for (int i=0; i<last; i++) {
   if (top > max_top)
     max_top = top;
   switch(list[i].op) {
   case Plus    :
   case Minus   :
   case Times   :
   case Slash   :
   case Mini    :
   case Maxi    :
   case Eq      :
   case Neq     :
   case Gret    :
   case Less    :
   case Geq     :
   case Leq     :
   case And     :
   case Or      :
   case Xor     :
   case Imp     :
   case Dimp    : top--; break;  // two pops and one push
   case Not     :        break;  // one pop and one push
   case St      :
   case Rw      :
   case Cs_st   :
   case S_rw    :
   case P_rw    :
   case Cs_rw   :
   case Cp_rw   :
   case Cnst    : top++; break;  // one push
   default      : if (verbose)
                    Bad_Function_Definition("incomplete definition or bad dimension", 1)
                  i = dimension;     // exit the loop
                  answer = false;
   }
 }
 if (top < 0) {
   if (verbose)
     Bad_Function_Semantics("too few arguments (too many operations)")
   answer = false;
 }
 if (top > 0) {
   if (verbose)
     Bad_Function_Semantics("too many arguments (too few operations)")
   answer = false;
 }
 if (max_top >= max_stack_size)
   Function_Stack_Overflow(max_stack_size, max_top)
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                It fills 's' with the parameters of 'this'                //
     //--------------------------------------------------------------------------//
void func_tree::param(aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())    // it tests if the number of automata in 's' is the
                                     // total number of automata of the original model
                                     // (given by the function 'aut' of the class 'FTB'
    Programming_Error("bad parameters call", 4101)
#endif
 aut_id a;

 s.take_out();
 for (int i=0; i<last; i++)
   switch (list[i].op) {
   case St :
   case Rw : s.put_in(aut_id(list[i].val));
             break;
   case Cs_st :
   case S_rw  :
   case P_rw  :
   case Cs_rw :
   case Cp_rw : for (a=list[i].beg; a<=list[i].end; a++)
                  s.put_in(a);
   }
}
     //--------------------------------------------------------------------------//
     //                 It evaluates 'this' for global state 'p'                 //
     //--------------------------------------------------------------------------//
     // The function tree ('this') must be well defined before calling this      //
     // function (see member function 'well_def()' above.                        //
     //--------------------------------------------------------------------------//
rp func_tree::eval(const aut_st & p) const
{
#ifdef _PEPS_DEBUG_
  if (p.size() != FTB::ftb.aut())  // it tests if the number of automata in 'p' is the
                                     // total number of automata of the original model
    Programming_Error("bad evaluate call", 4102)
#endif
 rp     stack[max_stack_size];   // the stack
 int    top = -1;                // the top of the stack
 aut_id a;                       // to visit a given range of automata

 for (int i=0; i<last; i++) {
#ifdef _PEPS_DEBUG_
   if (top == max_stack_size)
    Programming_Error("missing well-defined verification", 4103)
#endif
   switch(list[i].op) {
   case Plus    : top--; stack[top] += stack[top+1];
                  break;
   case Minus   : top--; stack[top] -= stack[top+1];
                  break;
   case Times   : top--; stack[top] *= stack[top+1];
                  break;
   case Slash   : top--; stack[top] /= stack[top+1]; //aqui
                  break;
   case Mini    : top--; if (stack[top] > stack[top+1])
                           stack[top] = stack[top+1];
                  break;
   case Maxi    : top--; if (stack[top] < stack[top+1])
                           stack[top] = stack[top+1];
                  break;
   case Eq      : top--; stack[top] = rp(stack[top] == stack[top+1]);
                  break;
   case Neq     : top--; stack[top] = rp(stack[top] != stack[top+1]);
                  break;
   case Gret    : top--; stack[top] = rp(stack[top] > stack[top+1]);
                  break;
   case Less    : top--; stack[top] = rp(stack[top] < stack[top+1]);
                  break;
   case Geq     : top--; stack[top] = rp(stack[top] >= stack[top+1]);
                  break;
   case Leq     : top--; stack[top] = rp(stack[top] <= stack[top+1]);
                  break;
   case And     : top--; stack[top] = stack[top] && stack[top+1];
                  break;
   case Or      : top--; stack[top] = stack[top] || stack[top+1];
                  break;
   case Xor     : top--; stack[top] = ( !(stack[top]) &&   stack[top+1]  ) ||
                                        ( stack[top]  && !(stack[top+1]) );
                  break;
   case Imp     : top--; stack[top] = ( stack[top] && stack[top+1] ) || !(stack[top]);
                  break;
   case Dimp    : top--; stack[top] = ( stack[top] && stack[top+1] ) ||
                                      ( !(stack[top]) && !(stack[top+1]) );
                  break;
   case Not     : stack[top] = !(stack[top]) ;
                  break;
   case St      : top++; stack[top] = rp(p.st(int(list[i].val)));
                  break;
   case Rw      : top++; stack[top] = FTB::ftb.reward(int(list[i].val),
                                                      p.st(int(list[i].val)));
                  break;
   case Cs_st   : top++; stack[top] = 0;
                  for (a=list[i].beg; a<=list[i].end; a++)
                    if (p.st(a) == aut_id(list[i].val))
                      stack[top]++;
                  break;
   case S_rw    : top++; stack[top] = 0;
                  for (a=list[i].beg; a<=list[i].end; a++)
                    stack[top] += FTB::ftb.reward(a, p.st(a));
                  break;
   case P_rw   : top++; stack[top] = 1;
                  for (a=list[i].beg; a<=list[i].end; a++)
                    stack[top] *= FTB::ftb.reward(a, p.st(a));
                  break;
   case Cs_rw   : top++; stack[top] = 0;
                  for (a=list[i].beg; a<=list[i].end; a++)
                    if (p.st(a) == aut_id(list[i].val))
                      stack[top] += FTB::ftb.reward(a, p.st(a));
                  break;
   case Cp_rw   : top++; stack[top] = 1;
                  for (a=list[i].beg; a<=list[i].end; a++)
                    if (p.st(a) == aut_id(list[i].val))
                      stack[top] *= FTB::ftb.reward(a, p.st(a));
                  break;
   case Cnst    : top++; stack[top] = list[i].val;
   }
 }
#ifdef _PEPS_DEBUG_
  if (top != 0)
    Programming_Error("missing well-defined verification", 4104)
#endif
 return(stack[0]);
}

     //--------------------------------------------------------------------------//
     //                     It puts 't' at the tail of 'this'                    //
     //--------------------------------------------------------------------------//
void func_tree::tail(const token & t)
{
#ifdef _PEPS_DEBUG_
  if (last == dimension)
    Programming_Error("function tree too small", 4105)
#endif
 list[last] = t;
 last++;
}
     //--------------------------------------------------------------------------//
     //                     It puts 't' at the head of 'this'                    //
     //--------------------------------------------------------------------------//
void func_tree::head(const token & t)
{
#ifdef _PEPS_DEBUG_
  if (last == dimension)
    Programming_Error("function tree too small", 4106)
#endif
 for (int i=last; i>0; i--)
   list[i] = list[i-1];
 list[0] = t;
 last++;
}
     //--------------------------------------------------------------------------//
     //        It concatenates 'this' with constant 'v' and operation 'o'        //
     //--------------------------------------------------------------------------//
     //  General case: the constant 'v' and the operation 'o' are concatenated   //
     // at the tail of the list.                                                 //
     //                                                                          //
     //  Special case: if - the last operation of the list and the operation 'o' //
     //                     are the same; and                                    //
     //                   - the penultimate operation is a constant ('cnst')     //
     // Evaluates 'v' and the operand of the penultimate token ('list[last-2]'). //
     //                                                                          //
     // Restriction: Only binary and associative operators are allowed to the    //
     // concatenation with a constant.                                           //
     //--------------------------------------------------------------------------//
void func_tree::concat(const rp v, const oper o)
{
#ifdef _PEPS_DEBUG_
  if ((o == Not)  || (o == St)   || (o == Rw)    || (o == Cs_st) || // non binary
      (o == S_rw) || (o == P_rw) || (o == Cs_rw) || (o == Cp_rw) || //   operators
      (o == Cnst) ||
      (o == Eq)   || (o == Neq)  || (o == Gret) || (o == Less)  ||  // non associative
      (o == Geq)  || (o == Leq)  || (o == Xor)  || (o == Imp)   ||  //   operators
      (o == Dimp))
    Programming_Error("invalid operation (concatenate)", 4110)
#endif
  if ((list[last-1].op == o) && (list[last-2].op == Cnst)) {
    switch (o) {
      case Plus  : list[last-2].val += v;                          break;
      case Minus : list[last-2].val += v;                          break;
      case Times : list[last-2].val *= v;                          break;
      case Slash : list[last-2].val *= v;                          break;
      case Mini  : if (v < list[last-2].val) list[last-2].val = v; break;
      case Maxi  : if (v > list[last-2].val) list[last-2].val = v; break;
      case And   : list[last-2].val = list[last-2].val && v;       break;
      case Or    : list[last-2].val = list[last-2].val || v;       break;
    }
 }
 else {
   func_tree temp(dimension + 2);                  // a temporary to the new 'this'
   for (temp.last=0; temp.last<last; temp.last++)  // copy 'this' list
     temp.list[temp.last] = list[temp.last];
   temp.list[temp.last].op  = Cnst;                // put 'v' at the tail
   temp.list[temp.last].val = v;
   temp.last++;
   temp.list[temp.last].op  = o;                   // put operation 'o' at the tail
   temp.list[temp.last].val = zero;
   temp.last++;
   copy(temp);
 }
}
     //--------------------------------------------------------------------------//
     //   It concatenates two function trees ('this' and 't') and operation 'o'  //
     //--------------------------------------------------------------------------//
     //  General case: the function 't' and the operation 'o' are concatenated   //
     // at the tail of the list.                                                 //
     //                                                                          //
     // Restriction: Only binary operators are allowed to the concatenation with //
     // another function.                                                        //
     //--------------------------------------------------------------------------//
void func_tree::concat(const func_tree & t, const oper o)
{
#ifdef _PEPS_DEBUG_
  if ((o == Not)  || (o == St)   || (o == Rw)    || (o == Cs_st) || // non binary
      (o == S_rw) || (o == P_rw) || (o == Cs_rw) || (o == Cp_rw) || //   operators
      (o == Cnst))
    Programming_Error("invalid operation (concatenate)", 4111)
#endif
 func_tree temp(dimension + t.dimension + 1);         // a temporary to the new 'this'
 for (temp.last=0; temp.last<dimension; temp.last++)  // copy 'this' list
   temp.list[temp.last] = list[temp.last];
 for (int j=0; j<t.dimension; temp.last++, j++)       // copy 't' list
   temp.list[temp.last] = t.list[j];
 temp.list[temp.last].op  = o;                        // put operation 'o' at the tail
 temp.list[temp.last].val = zero;
 temp.last++;
 copy(temp);
}

     //--------------------------------------------------------------------------//
     // It creates 'this' as the concatenation of 't' with 'v' and operation 'o' //
     //--------------------------------------------------------------------------//
     //  General case: the function 't', the constant 'v' and the operation 'o'  //
     // are concatenated in 'this'                                               //
     //                                                                          //
     //  Special case: if - the last operation of the list and the operation 'o' //
     //                     are the same; and                                    //
     //                   - the penultimate operation is a constant ('cnst')     //
     // Evaluates 'v' and the operand of the penultimate token ('list[last-2]'). //
     //                                                                          //
     // Restriction: Only binary and associative operators are allowed to the    //
     // concatenation with a constant.                                           //
     //--------------------------------------------------------------------------//
void func_tree::create_cat(const func_tree & t, const rp v, const oper o)
{
#ifdef _PEPS_DEBUG_
  if ((o == Not)  || (o == St)   || (o == Rw)    || (o == Cs_st) || // non binary
      (o == S_rw) || (o == P_rw) || (o == Cs_rw) || (o == Cp_rw) || //   operators
      (o == Cnst) ||
      (o == Eq)   || (o == Neq)  || (o == Gret)  || (o == Less)  || // non associative
      (o == Geq)  || (o == Leq)  || (o == Xor)   || (o == Imp)   || //   operators
      (o == Dimp))
    Programming_Error("invalid operation (concatenate)", 4112)
#endif
  if ((t.list[last-1].op == o) && (t.list[last-2].op == Cnst)) {
    copy(t);
    switch (o) {
      case Plus  : list[last-2].val += v;                          break;
      case Minus : list[last-2].val += v;                          break;
      case Times : list[last-2].val *= v;                          break;
      case Slash : list[last-2].val *= v;                          break;
      case Mini  : if (v < list[last-2].val) list[last-2].val = v; break;
      case Maxi  : if (v > list[last-2].val) list[last-2].val = v; break;
      case And   : list[last-2].val = list[last-2].val && v;       break;
      case Or    : list[last-2].val = list[last-2].val || v;       break;
    }
 }
 else {
   create(t.dimension + 2);                // allocate 'this'
   for (last=0; last<t.dimension; last++)  // copy 't' list
     list[last] = t.list[last];
   list[last].op  = Cnst;
   list[last].val = v;                     // put constant 'v' at the tail
   last++;
   list[last].op  = o;                     // put operation 'o' at the tail
   list[last].val = zero;
   last++;
 }
}
     //--------------------------------------------------------------------------//
     //It creates 'this' as the concatenation of 't1' with 't2' and operation 'o'//
     //--------------------------------------------------------------------------//
     //  General case: the function 't' and the operation 'o' are concatenated   //
     // at the tail of the list.                                                 //
     //                                                                          //
     // Restriction: Only binary operators are allowed to the concatenation with //
     // another function.                                                        //
     //--------------------------------------------------------------------------//
void func_tree::create_cat(const func_tree & t1, const func_tree & t2, const oper o)
{
#ifdef _PEPS_DEBUG_
  if ((o == Not)  || (o == St)   || (o == Rw)    || (o == Cs_st) || // non binary
      (o == S_rw) || (o == P_rw) || (o == Cs_rw) || (o == Cp_rw) || //   operators
      (o == Cnst))
    Programming_Error("invalid operation (concatenate)", 4113)
#endif
 create(t1.dimension + t2.dimension + 1);      // allocate 'this'
 for (last=0; last<t1.dimension; last++)       // copy 't1' list
   list[last] = t1.list[last];
 for (int j=0; j<t2.dimension; last++, j++)    // copy 't2' list
   list[last] = t2.list[j];
 list[last].op  = o;                           // put operation 'o' at the tail
 list[last].val = zero;
 last++;
}

     //--------------------------------------------------------------------------//
     //               It writes the function on the standard output              //
     //--------------------------------------------------------------------------//
void func_tree::show() const
{
 for (int i=0; i<last; i++)
   switch (list[i].op) {
     case Plus    : cout << "+ ";                      break;
     case Minus   : cout << "- ";                      break;
     case Times   : cout << "* ";                      break;
     case Slash   : cout << "/ ";                      break;
     case Mini    : cout << "min ";                    break;
     case Maxi    : cout << "max ";                    break;
     case Eq      : cout << "== ";                     break;
     case Neq     : cout << "!= ";                     break;
     case Gret    : cout << "> ";                      break;
     case Less    : cout << "< ";                      break;
     case Geq     : cout << ">= ";                     break;
     case Leq     : cout << "<= ";                     break;
     case And     : cout << "&& ";                     break;
     case Or      : cout << "|| ";                     break;
     case Xor     : cout << "^^ ";                     break;
     case Imp     : cout << "-> ";                     break;
     case Dimp    : cout << "<-> ";                    break;
     case Not     : cout << "! ";                      break;
     case St      : cout << "@" << int(list[i].val) << " "; break;
     case Rw      : cout << "%" << int(list[i].val) << " "; break;
     case Cs_st   : cout << "@$(" << int(list[i].val) << ","
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case S_rw    : cout << "%$("
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case P_rw    : cout << "%#("
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case Cs_rw   : cout << "%%$(" << int(list[i].val) << ","
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case Cp_rw   : cout << "%%#(" << int(list[i].val) << ","
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case Cnst    : if ((list[i].val - int(list[i].val)) < visual_zero)
                      cout << int(list[i].val) << " ";
                    else
                      cout << list[i].val << " ";
                    break;
     default      : cout << "??? ";
   }
 cout << "\n(" << dimension+1 << " tokens, using until the " << last << "-th token)\n";
}
     //--------------------------------------------------------------------------//
     //                       Print on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void func_tree::print(ofstream & fout) const
{
 for (int i=0; i<last; i++)
   switch (list[i].op) {
     case Plus    : fout << "+ ";                      break;
     case Minus   : fout << "- ";                      break;
     case Times   : fout << "* ";                      break;
     case Slash   : fout << "/ ";                      break;
     case Mini    : fout << "min ";                    break;
     case Maxi    : fout << "max ";                    break;
     case Eq      : fout << "== ";                     break;
     case Neq     : fout << "!= ";                     break;
     case Gret    : fout << "> ";                      break;
     case Less    : fout << "< ";                      break;
     case Geq     : fout << ">= ";                     break;
     case Leq     : fout << "<= ";                     break;
     case And     : fout << "&& ";                     break;
     case Or      : fout << "|| ";                     break;
     case Xor     : fout << "^^ ";                     break;
     case Imp     : fout << "-> ";                     break;
     case Dimp    : fout << "<-> ";                    break;
     case Not     : fout << "! ";                      break;
     case St      : fout << "@" << int(list[i].val) << " "; break;
     case Rw      : fout << "%" << int(list[i].val) << " "; break;
     case Cs_st   : fout << "@$(" << int(list[i].val) << ","
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case S_rw    : fout << "%$("
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case P_rw    : fout << "%#("
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case Cs_rw   : fout << "%%$(" << int(list[i].val) << ","
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case Cp_rw   : fout << "%%#(" << int(list[i].val) << ","
                         << int(list[i].beg) << "," << int(list[i].end) << ") "; break;
     case Cnst    : if ((list[i].val - int(list[i].val)) < visual_zero)
                      fout << int(list[i].val) << " ";
                    else
                      fout << list[i].val << " ";
                    break;
     default      : fout << "??? ";
   }
}
     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void func_tree::write(ofstream & fout) const
{
 fout << dimension << " " << last << "\n";
 for (int d=0; d<last; d++)
   fout << int(list[d].op) << " " << list[d].val << " "
        << " " << list[d].beg << " " << " " << list[d].end << " ";
 fout << "\n";
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void func_tree::read(ifstream & fin)
{
 int d;
 int buf;

 fin >> d;
 create(d);
 fin >> last;
 for (d=0; d<last; d++) {
   fin >> buf; list[d].op = oper(buf);
   fin >> list[d].val;
   fin >> list[d].beg;
   fin >> list[d].end;
 }
 mark = 0;
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int func_tree::mem_use() const
{
 int answer = 12;
 answer += dimension * 20;
 return(answer);
}
