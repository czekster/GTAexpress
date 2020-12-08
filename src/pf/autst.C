//====================================================================================//
//                                                                                    //
//                              Class Automata States                                 //
//                                                                                    //
//====================================================================================//
//  This File:   autst.C                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 1350                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/jan/02                        by: Anne.Benoit@imag.fr             //
//====================================================================================//
#include "peps.h"


     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void aut_st::write(ofstream & fout) const
{
 fout << dimension << "\n";
 for (aut_id d=0; d<dimension; d++)
   fout << info[d] << " ";
 fout << "\n";
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void aut_st::read(ifstream & fin)
{
 aut_id d;

 fin >> d;
 create(d);
 for (d=0; d<dimension; d++)
   fin >> info[d];
}
     //--------------------------------------------------------------------------//
     //              Memory space used by 'this' (in memory units)               //
     //--------------------------------------------------------------------------//
int aut_st::mem_use() const
{
 int answer = 4;
 answer += dimension * 4;
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                      The rank of the global state                        //
     //--------------------------------------------------------------------------//
int aut_st::rank(const aut_st & s) const {
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad index call", 1350)
#endif
 int answer = 0;
 int base = 1;
 for (aut_id i=dimension-1; i>=0; i--) {
   answer += info[i] * base;
   base   *= s.info[i];
 }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                    Sets 'this' according to rank 'r'                     //
     //--------------------------------------------------------------------------//
     // Principle: in the rank 'r' there are multiples of the local states.      //
     // Algorithm: just keep the rest of integer division by each automata size  //
     // and remove (integer division) the part corresponding to each discovered  //
     // automaton local state.                                                   //
     //--------------------------------------------------------------------------//
void aut_st::st_rank(const int r, const aut_st & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad index call", 1351)
#endif
 aut_id a;               // the automaton which the local state is being extracted
 int    rest  = r;       // the part of the rank to extract the local state

 // for (a=0; a<dimension; a++) { // to all automata 'a'
 for (a=dimension-1; a>=0; a--) { // to all automata 'a'
   info[a] = (rest % s.info[a]); // compute the local state
   rest    /= s.info[a];         // remove the part corresponding to this automaton
 }
}

     //--------------------------------------------------------------------------//
     //      The rank of the state, the list 'l' being used to change the        //
     //       lexical order and also to mask E according to an order 'l'         //
     //--------------------------------------------------------------------------//
int aut_st::rank(const aut_list & l, const aut_st & s) const
{
#ifdef _PEPS_DEBUG_
  if (l.size() != dimension)
    Programming_Error("bad index call", 1352)
  if (s.size() != dimension)
    Programming_Error("bad index call", 1353)
#endif
 int answer = 0;
 int base   = 1;
 for (int i=l.has()-1; i>=0; i--) {
   answer += info[l.aut(i)] * base;
   base   *= s.info[l.aut(i)];
 }
 return(answer);
}


     //--------------------------------------------------------------------------//
     // It verifies if a group of global states has at least one reacheable state//
     //--------------------------------------------------------------------------//
     //   Given a state vector x of automata in E and a subset s of E, denote    //
     // St(x,s) the set of states y such that y(i) = x(i) for all automata i in  //
     // E-s. The function returns false if St(x,s) is included in the            //
     // non-reacheable state space and true otherwise.                           //
     //   In the 'verify' function, the state vector x is coded in 'this' and    //
     // the s subset is the input parameter 's'. The reacheable state space is   //
     // the static variable 'RRS::rss' and the size of the automata is taken     //
     // from the static variable 'FTB::ftb' (y(i) domain when i belongs to s).   //
     //                                                                          //
     // Obs.: this function (usually called in the grouping process) suposes     //
     // that the model was not grouped before.                                   //
     //--------------------------------------------------------------------------//
bool aut_st::verify(const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("wrong sized parameter in verify", 1354)
  if (FTB::ftb.aut() != dimension)
    Programming_Error("inconsistency on FTB", 1355)
#endif
 bool     answer = false;    // true if at least one state is reacheable
 aut_st   sizes;             // the size of each original automata
 aut_list list;              // a list with the automata in 's'
 int      sss;               // size of the product state space St(x,s)

 FTB::ftb.get_domain(sizes); // gets the original automata sizes
 sss = sizes.state_space(s); // gets the product state space of the automata in 's'
 list.put(s);                // puts 's' in an aut_list object
 reset(list);                // resets the automata in the subset 's'
 for (int i=0; i<sss; i++) {          // for all global states in the subspace St(x,s)
   if (RSS::rss.has(rank(sizes))) {   //   verifies if the global state is reacheable
     answer = true;                   //   if it is reacheable
     break;                           //     stops the loop
   }
   else                               //   if it is not reacheable
     next(list, sizes);               //      proceeds to the next global state
 }                                    //      of the subspace
 return(answer);
}

//====================================================================================//
//                     Functions used only to automata states                         //
//====================================================================================//


     //--------------------------------------------------------------------------//
     //              Counts the number of different values in 'this'             //
     //--------------------------------------------------------------------------//
int aut_st::count_groups() const
{
 aut_st buff;          // a buffer to store the different values of 'this'
 aut_id i, j;          // counter for 'this' and 'diff' elements
 bool   new_value;     // a flag to indicate that a new value was found
 int    answer = 0;    // the answer (the number of different values)

 buff.create(dimension);              // creates a second automata state object
 for (i=0; i<dimension; i++) {        // for all values of 'this'
   new_value = true;
   for (j=0; j<answer; j++)           //   search for this same values into the 'buff'
     if (info[i] == buff.info[j]) {   //   if this value already exists
       new_value = false;             //     - it is not a new value and
       break;                         //     - exit the inner (search) loop
     }                                //   after the search loop
   if (new_value) {                   //   if a new value is found
     buff.info[answer] = info[i];     //     - stores this new value into the 'buff' and
     answer++;                        //     - adds one to the answer
   }                 
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //              Verifies if the values in 'this' are contigous              //
     //--------------------------------------------------------------------------//
bool aut_st::contigous() const
{
 bool   answer = true;       // 'this' has contigous values
 bool * values;              // an array of boolean to indicate if a value is present
 int    first = smallest();  // the smallest value in 'this'
 int    last  = biggest();   // the biggest value in 'this'
 int    i;                   // a counter to visit 'this' and 'values'

 values = new bool[last+1];     // it allocates booleans to each values
 for (i=0; i<last; i++)         // it resets the array 'values'
   values[i] = false;
 for (i=0; i<dimension; i++)    // it fills the array 'values' with the values in 'this'
   values[info[i]] = true;
 for (i=first+1; i<last; i++)   // for all values between the first and the last
   if (!(values[i])) {             // it verifies if the value is in 'this'
     answer = false;                  // if one of the values is missing
     break;                              // the answer is 'false' and stops the loop
   }
 delete [] values;
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                         Sort the automata by states                      //
     //--------------------------------------------------------------------------//
void aut_st::sort(aut_id f, aut_id l)
{
  quick_sort(f, l); 
}

void aut_st::quick_sort(aut_id i, aut_id s)
{
 int e=i, d=s;
 int item = info[ ((e+d)/2) ];
 while(e <= d){
   while(info[e] < item)
     e++;
   while(info[d] > item)
     d--;
   if(e <= d){
     int aux; 
     aux      = info[e];
     info[e] = info[d];
     info[d] = aux;
     d--;
     e++;
   }
 }
 if(d-i > 0) quick_sort(i,d);
 if(s-e > 0) quick_sort(e,s);
}

      //--------------------------------------------------------------------------//
      //                    Get state map of a specific state                     //
      //--------------------------------------------------------------------------//
void aut_st::create_map(const int i, const int sz)
{
  int x;
  int y;
  
  y = i;
  for(int t=0; t<size(); t++){
    x = y % sz;
    y /= sz;
    info[(size()-t-1)] = x;
  }
}

      //--------------------------------------------------------------------------//
      //                    Get state map of a specific state                     //
      //--------------------------------------------------------------------------//
bool aut_st::is_sorted(aut_id f, aut_id l)
{
 for(int i=f; i<l; i++)
   if(info[i] > info[i+1])
     return(false);
 return(true);
}
