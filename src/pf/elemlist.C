//====================================================================================//
//                                                                                    //
//                            Class SAN Element List                                  //
//                                                                                    //
//====================================================================================//
//  This File:   elemlist.C                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //              It puts element 'e' at the tail of the list                 //
     //--------------------------------------------------------------------------//
void elem_list::tail(const elem e)
{
#ifdef _PEPS_DEBUG_
  if (dimension == number)
    Programming_Error("elements list full", 3202)
#endif
 list[number] = e;
 number++;
}
     //--------------------------------------------------------------------------//
     //              It puts function 'f' at the tail of the list                //
     //--------------------------------------------------------------------------//
void elem_list::tail(const func_id f)
{
#ifdef _PEPS_DEBUG_
  if (dimension == number)
    Programming_Error("elements list full", 3203)
#endif
 list[number] = f;
 number++;
}
     //--------------------------------------------------------------------------//
     //              It puts constant 'c' at the tail of the list                //
     //--------------------------------------------------------------------------//
void elem_list::tail(const rp c)
{
#ifdef _PEPS_DEBUG_
  if (dimension == number)
    Programming_Error("elements list full", 3204)
#endif
 list[number] = c;
 number++;
}
     //--------------------------------------------------------------------------//
     //              It puts element 'e' at the head of the list                 //
     //--------------------------------------------------------------------------//
void elem_list::head(const elem e)
{
#ifdef _PEPS_DEBUG_
  if (dimension == number)
    Programming_Error("elements list full", 3205)
#endif
 for (int i=number; i>0; i--)
   list[i] = list[i-1];
 list[0] = e;
 number++;
}

     //--------------------------------------------------------------------------//
     //It puts element 'e' as the p-th element of the list, erasing the old value//
     //--------------------------------------------------------------------------//
void elem_list::change(const int p, const elem e)
{
#ifdef _PEPS_DEBUG_
  if (p > number)
    Programming_Error("index out of range", 3206)
#endif
 if (p == number)    // inclusion of a new element at the tail of the list
   number++;
 list[p] = e;        // real change of the element in position 'p'
}

     //--------------------------------------------------------------------------//
     //                Exchanges the p-th and the q-th element                   //
     //--------------------------------------------------------------------------//
void elem_list::exchange(const int p, const int q)
{
#ifdef _PEPS_DEBUG_
  if ((p >= number) || (q >= number))
    Programming_Error("index(es) out of range", 3207)
#endif
 elem temp;
 temp    = list[p];
 list[p] = list[q];
 list[q] = temp;
}
     //--------------------------------------------------------------------------//
     //         Sorts the list according to:                                     //
     //                - the functional elements with increasing id's;           //
     //                - the constant elements with increasing values;           //
     //--------------------------------------------------------------------------//
void elem_list::sort()
{
 int  last, i;
 elem temp;
 for (last=number-1; last>0; last--)           // bubble sort
   for (i=0; i<last; i++)
     if (list[i].is_const()) {
       if ((list[i+1].is_func()) || (list[i].get() > list[i+1].get())) {
         temp      = list[i+1];
         list[i+1] = list[i];
         list[i]   = temp;
       }
     }
     else if ((list[i+1].is_func()) && (list[i].f_get() > list[i+1].f_get())) {
            temp      = list[i+1];
            list[i+1] = list[i];
            list[i]   = temp;
          }
}

     //--------------------------------------------------------------------------//
     //               It compares if 'this' and 'l' are equal                    //
     //--------------------------------------------------------------------------//
bool elem_list::operator == (const elem_list & l) const
{
 bool answer = true;

 if (number != l.number)
   answer = false;
 else
   for (int i=0; i<number; i++)
     if (list[i] != l.list[i]) {
       answer = false;
       break;
     }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void elem_list::write(ofstream & fout) const
{
 fout << dimension << " " << number << "\n";
 for (int i=0; i<number; i++)
   list[i].write(fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void elem_list::read(ifstream & fin)
{
 int d;

 fin >> d;
 create(d);
 fin >> number;
 for (d=0; d<number; d++)
   list[d].read(fin);
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int elem_list::mem_use() const
{
 int answer = 8;
 answer += dimension * 16;
 return(answer);
}
