     //--------------------------------------------------------------------------//
     //              It compiles 'this' into the ascii file "fout"               //
     //--------------------------------------------------------------------------//
     // The function tree ('this') must be well defined before calling this      //
     // function (see member function 'well_def()' above.                        //
     //--------------------------------------------------------------------------//
#include "peps.h"
/**/
rp func_tree::compile(const aut_st & p, const func_id f) const
{
#ifdef _PEPS_DEBUG_
  if (p.size() != FTB::ftb.aut())  // it tests if the number of automata in 'p' is the
                                     // total number of automata of the original model
    Programming_Error("bad evaluate call", 4102)
#endif
      // to delete *************************************************************************************V
 rp     stack[max_stack_size];   // the stack
 int    top = -1;                // the top of the stack
 aut_id a;                       // to visit a given range of automata
      // to delete *************************************************************************************^
 ofstream  fout;                 // the output file peps_jit.C

 if (!Exists_File("peps_jit.C", dot_file))
   Programming_Error("function jit file missing", 4444)
 else {
   Append_File("peps_jit.C", dot_file, fout);
   fout << "//////////////////////////////////////////////////////////\n"
        << "rp FX__" << f << "__FX(const aut_st & p) {\n";
 }
 for (int i=0; i<last; i++) {
   switch(list[i].op) {
   case Plus    : top--; stack[top] += stack[top+1];
                  break;
   case Minus   : top--; stack[top] -= stack[top+1];
                  break;
   case Times   : top--; stack[top] *= stack[top+1];
                  break;
   case Slash   : top--; stack[top] /= stack[top+1];
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
/**/
