
//====================================================================================//
//                                                                                    //
//                                Class State Map                                     //
//                                                                                    //
//====================================================================================//
//  This File:   stmap.C                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 23/jun/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"


     //--------------------------------------------------------------------------//
     //     It puts the automata of 's' as the automata that has been grouped    //
     //--------------------------------------------------------------------------//
void st_map::put(const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("bad sized automata subset", 5106)
 if (s.has() != automata)
    Programming_Error("inconsistent state map definition", 5107)
#endif
 int i, j;

 for (i=0, j=0; i<s.size(); i++)
   if (s.has(i)) {
     position[j] = i;
     j++;
   }
}
     //--------------------------------------------------------------------------//
     //    It puts automaton 'a' as the 'p'-th automaton that has been grouped   //
     //--------------------------------------------------------------------------//
void st_map::put(const aut_id a, const int p)
{
#ifdef _PEPS_DEBUG_
  if (a >= FTB::ftb.aut())
    Programming_Error("index out of range", 5108)
 if (p >= automata)
    Programming_Error("index out of range", 5109)
#endif
 position[p] = a;
}
     //--------------------------------------------------------------------------//
     // It fills the array 'states' with a singular order ('0', '1', and so on)  //
     //--------------------------------------------------------------------------//
void st_map::fill()
{
#ifdef _PEPS_DEBUG_
 if (!(is_singular()))
   Programming_Error("invalid fill call", 5110)
#endif
 for (int i=0; i<rows; i++)
   states[i][0] = i;
}
     //--------------------------------------------------------------------------//
     //             It fills the array 'states' with a lexical order             //
     //       (the parameter 's' has the size of each original automaton)        //
     //--------------------------------------------------------------------------//
void st_map::fill(const aut_st & s)
{
 int    j;
 aut_st st;

 st.create(automata);
 st.reset();
 for (int i=0; i<rows; i++) {
   for (j=0; j<automata; j++)
     states[i][j] = st.st(j);
   st.next(s);
 }
}
     //--------------------------------------------------------------------------//
     //  It puts the local states of 's' in the entry 'r' of the array 'states'  //
     //--------------------------------------------------------------------------//
     // Obs.: the size of the state vector 's' is the original number of         //
     // automata. Only the local states of the original automata used by 'this'  //
     // are stored as original states corresponding the state 'r'.               //
     //--------------------------------------------------------------------------//
void st_map::put(const st_id r, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (r >= rows){
    Programming_Error("index out of range", 5111)
  }
  if (s.size() != FTB::ftb.aut())
    Programming_Error("bad sized state vector", 5112)
#endif
 for (int i=0; i<automata; i++)
   states[r][i] = s.st(position[i]);
}

     //--------------------------------------------------------------------------//
     //      It reduces the state map keeping only the states that are in 'v'    //
     //--------------------------------------------------------------------------//
void st_map::reduce(const bools & v)
{
#ifdef _PEPS_DEBUG_
  if (v.size() != rows)
    Programming_Error("bad sized vector of booleans", 5112)
#endif
 st_map temp;
 int j=0;
 aut_id a;

 temp.create(v.has(), automata);          // allocates a new object
 for (a=0; a<automata; a++)               // copies the automata that have been grouped
   temp.position[a] = position[a];
 for (int i=0; i<rows; i++)               // for all states of the grouped automaton
   if (v[i]) {                               // if it is in 'v'
     for (a=0; a<automata; a++)                 // copies its local states
       temp.states[j][a] = states[i][a];
     j++;
   }
 copy(temp);                              // replaces 'this' by the new structure
}

     //--------------------------------------------------------------------------//
     //                           Output on stand stdout                         //
     //--------------------------------------------------------------------------//
void st_map::write() const
{
 aut_id a;

 cout << rows << " " << automata << "\n";
 for (a=0; a<automata; a++)
   cout << position[a] << " ";
 cout << "\n";
 for (int r=0; r<rows; r++) {
   for (a=0; a<automata; a++)
     cout << states[r][a] << " ";
   cout << "\n";
 }
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void st_map::write(ofstream & fout) const
{
 aut_id a;

 fout << rows << " " << automata << "\n";
 for (a=0; a<automata; a++)
   fout << position[a] << " ";
 fout << "\n";
 for (int r=0; r<rows; r++) {
   for (a=0; a<automata; a++)
     fout << states[r][a] << " ";
   fout << "\n";
 }
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void st_map::read(ifstream & fin)
{
 int r, a;

 fin >> r;
 fin >> a;
 create(r, a);
 for (a=0; a<automata; a++)
   fin >> position[a];
 for (r=0; r<rows; r++)
   for (a=0; a<automata; a++)
     fin >> states[r][a];
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int st_map::mem_use()
{
 int answer = 8;
 answer += automata * 4;
 answer += automata * rows * 4;
 return(answer);
}

      //--------------------------------------------------------------------------//
      //                    Internal Function of allocation                       //
      //--------------------------------------------------------------------------//
void st_map::alloc()
{
 position = new aut_id[automata];
 states   = new st_id*[rows];
 for (int i=0; i<rows; i++)
   states[i] = new st_id[automata];
}
      //--------------------------------------------------------------------------//
      //                Internal Function of tranfer contents                     //
      //--------------------------------------------------------------------------//
void st_map::trans(const st_map & m)
{
 int i, j;

 for (j=0; j<automata; j++)
   position[j] = m.position[j];
 for (i=0; i<rows; i++)
   for (j=0; j<automata; j++)
     states[i][j] = m.states[i][j];
}
      //--------------------------------------------------------------------------//
      //                  Internal Function of desallocation                      //
      //--------------------------------------------------------------------------//
void st_map::desal()
{
 for (int i=0; i<rows; i++)
   delete[] states[i];
 delete[] states;
 delete[] position;
}
