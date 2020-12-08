//====================================================================================//
//                                                                                    //
//                              Class Automata Subset                                 //
//                                                                                    //
//====================================================================================//
//  This File:   autset.C                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code:  1100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 29/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

#include "peps.h"

  //------------------------------------------------------------------------------//
  //         Changes the subset 'this' by the following subset in a list          //
  //-------------------------------------------------------------+----------------//
  // The subsets of E are ordered. The order is defined by two   |   Example of   //
  // criteria. The first criterion is the cardinality of the     |  ordered list  //
  // subsets. For the second criterion, a subset is represented  |                //
  // by the list of its elements ranked according to the         | {}             //
  // automata list 'l'; For a given cardinality, the subsets are | { 2 }          //
  // ranked according to the lexical order. The function changes | { 1 }          //
  // 'this' by the following subset in the ordered list.         | { 3 }          //
  //                                                             | { 0 }          //
  // For example, with E = { 0, 1, 2, 3 } and l = { 2, 1, 3, 0 } | { 2, 1 }       //
  // the order list is represented in the column     ==>         | { 2, 3 }       //
  // If 'this' is { 1, 3 } the function changes it to { 1, 0 }.  | { 2, 0 }       //
  //                                                             | { 1, 3 }       //
  // Restriction: the elements of the automata list 'l' MUST be  | { 1, 0 }       //
  // included in E, but may be a strict subset of E, denoted by  | { 3, 0 }       //
  // El. In this case the order is computed on the subsets of El.| { 2, 1, 3 }    //
  // Then the subset 'this' MUST be included in El.              | { 2, 1, 0 }    //
  //                                                             | { 2, 3, 0 }    //
  // Remark: If 'this' is the last subset of the ordered list,   | { 1, 3, 0 }    //
  // function returns 'this' equal to the empty subset.          | { 2, 1, 3, 0 } //
  //-------------------------------------------------------------+----------------//
  // Implementation:                                                              //
  //                                                                              //
  //  Possible errors:                                                            //
  //                                                                              //
  //    1) 'this' is not included in 'l';                                         //
  //    2) 'l' is not included in E;                                              //
  //                                                                              //
  //  Special cases (first and last subsets of the ordered list):                 //
  //                                                                              //
  //    1) 'this' is an empty subset (first subset of the ordered list):          //
  //        * the function returns a subset with only the first automaton of 'l'  //
  //           (the second subset of the list).                                   //
  //                                                                              //
  //    2) 'this' is equal to 'l' (last subset of the ordered list):              //
  //        * the function returns the empty subset (the first subset of the      //
  //           ordered list).                                                     //
  //                                                                              //
  //  General case:                                                               //
  //                                                                              //
  //    - it copies 'this' in an array of "indexes of the list" called 'hint'     //
  //  (see 'hint' data structure below). 'k' is the index to access to the array  //
  //  elements and the array is of size 'hint_size';                              //
  //    - it compares the respective elements of 'l' and 'hint', starting from    //
  //  the last element of each data structure (main loop);                        //
  //       *** it stops the comparison when the elements are no more equal, that  //
  //  is to say when 'hint' element is smaller than the 'l' element; then it      //
  //  replaces this element, noted 'hint[k]', by its succesor in 'l'. For example //
  //  (using E and 'l' as above), if 'this' = { 2, 0 } , the comparison passes by //
  //  '0' and stops on '2', replacing '2' by its successor in 'l', in this case   //
  //  '1'. The following positions of 'hint' are also replaced by the             //
  //  corresponding sucessors of 'hint[k]' in 'l'. In (in this case only one      //
  //  automaton follows - '0' is replaced by '3', which is the sucessor of        //
  //  automaton '1'). The function returns 'this' = { 1, 3 }.                     //
  //       *** if the loop reaches the first element of 'hint' and all elements   //
  //  are equal, the function adds one to the cardinality of 'this', returning    //
  //  the subset containing the first 'hint_size'+1 elements of 'l'. For example  //
  //  (with the same E and 'l' as above), if 'this' = { 3, 0 }, the function      //
  //  returns 'this' = { 2, 1, 3 }.                                               //
  //                                                                              //
  //  'hint' Data Structure:                                                      //
  //                                                                              //
  //    A special data structure ('hint') is used to represent the 'this' subset  //
  //  inside the function. 'hint' is an array of integers which contains the      //
  //  positions of the elements of 'this' in the list 'l'.  For                   //
  //  example, with 'l' = { 2, 1, 3, 0 } and 'this' = { 1, 0 }, 'hint' is         // 
  //  [ 1, 3 ]. Note that positions in 'l' are coded from 0 to 'dimension'-1.     //
  //                                                                              //
  //    Obs.: With this data structure the change of an automaton by its          //
  //  successor in 'l' is made by a simple increment of an element of 'hint'. For //
  //  example, to pass from the subset 'this' = { 1, 3 } to the { 1, 0 } (the     //
  //  automaton '3' is followed by the automaton '0' in 'l'), 'hint' changes from //
  //  [ 1, 2 ] to [ 1, 3 ].                                                       //
  //------------------------------------------------------------------------------//
void aut_set::next(const aut_list & l)
{
//---------------------------------------------------------------------- Possible Errors
#ifdef _PEPS_DEBUG_
  aut_set set_l;            // the automata of the list 'l' as a subset object
  aut_set copy(*this);      // a working copy of 'this'
  set_l.copy(l);            // copy 'l' to a subset object
  copy.minus(set_l);        // removes 'l' elements from 'this'

  if (!(copy.is_empty()))                     // 'this' is not included in 'l' (error 1)
    Programming_Error("bad next call", 1110)
  if (l.biggest() >= size())                       // 'l' is not included in E (error 2)
    Programming_Error("bad next call", 1111)
#endif
//---------------------------------------------------------------------------- Variables
 int     i,                // counter for 'hint' and list 'l' positions
         k;                // the current position of 'hint' (in the main loop)
 bool    not_found = true; // condition to continue in the main loop (next not found)
 int   * hint;             // a copy of 'this' in an array of integers
 int hint_size = has();    // the cardinality of 'this'              ('hint' array size)
 int list_size = l.has();  // size of the list 'l'      (maximum cardinality for 'this')

//------------------------------------------------------------------------ Special Cases
  if (hint_size == 0)                   // if 'this' is an empty subset (special case 1)
    set[l.aut(0)] = true;               // then puts the first automata of 'l' in 'this'
  else {
    if (hint_size == list_size)            // if 'this' is equal to 'l' (special case 2)
      take_out();                          // then 'this' becomes the empty subset
    else {
//------------------------------------------------------------------------- General case
      hint = new int[hint_size];               // creates the 'hint' array
      for (i=0, k=-1; i<list_size; i++)        // copy 'this' elements to 'hint'
        if (set[l.aut(i)]) {                   //  (according to the list 'l' positions)
           k++;        
           hint[k] = i;                         // exiting the loop,
      }                                         // 'k' points to the last 'hint' element
      take_out();            // makes 'this' empty ('this' elements are coded in 'hint')
      do {                                                                  // Main loop
        // compares the respective elements of 'l' and 'hint'
        if (hint[k] < (list_size - (hint_size - k))) {
          not_found = false;                                     // stops the comparison
          hint[k]++;                  // replaces 'hint' element by its successor in 'l'
          for (i=k+1; i<hint_size; i++)    // The following positions of 'hint' are also
            hint[i] = hint[i-1] + 1;       // replaced by the corresponding sucessors of
                                           // 'hint[k]'
          for (i=0; i<hint_size; i++)                           // copy 'hint' to 'this'
            set[l.aut(hint[i])] = true;
        }                                      // the comparison leeds to equal elements
        else {                                 // until the first element of 'hint' then
          if (k == 0) {                        // adds one to the cardinality of 'this':
            not_found = false;                                   // stops the comparison
            for (i=0; i<hint_size+1; i++)                   // 'this' becomes the subset
              set[l.aut(i)] = true;                         //  with the 'hint_size'+1
          }                                                 //  first elements of 'l'
        }
        k--;                                           // Main loop index decrementation
      } while (not_found);
      delete[] hint;                                 // destructs temporary array 'hint'
    }
  }
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void aut_set::write(ofstream & fout) const
{
 fout << dimension << "\n";
 for (aut_id d=0; d<dimension; d++)
   fout << set[d] << " ";
 fout << "\n";
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void aut_set::read(ifstream & fin)
{
 aut_id d;
 fin >> d;
 create(d);
 for (d=0; d<dimension; d++)
   fin >> set[d];
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int aut_set::mem_use() const
{
 int answer = 4;
 answer += dimension * 4;
 return(answer);
}
