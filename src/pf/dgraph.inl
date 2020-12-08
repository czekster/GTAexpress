//====================================================================================//
//                                                                                    //
//                            Class Dependency Graph                                  //
//                                                                                    //
//====================================================================================//
//  This File:   dgraph_inl.h               Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Code: 1400                                                                  //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 28/may/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
inline d_graph::d_graph()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
inline d_graph::d_graph(const int dim)
{
 aut_id i,j;

 dimension = dim;
 graph = new bool*[dimension];
 for (i=0; i<dimension; i++)
   graph[i] = new bool[dimension];
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     graph[i][j] = false;
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
inline d_graph::d_graph(const d_graph & g)
{
 aut_id i,j;

 dimension = g.dimension;
 graph = new bool*[dimension];
 for (i=0; i<dimension; i++)
   graph[i] = new bool[dimension];
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     graph[i][j] = g.graph[i][j];
}

     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
inline d_graph::~d_graph()
{
 erase();
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void d_graph::create()
{
 dimension = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void d_graph::create(const int dim)
{
 aut_id i,j;

 if (dimension != dim) {
   erase();
   dimension = dim;
   graph = new bool*[dimension];
   for (i=0; i<dimension; i++)
     graph[i] = new bool[dimension];
 }
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     graph[i][j] = false;
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void d_graph::copy(const d_graph & g)
{
 aut_id i,j;

 create(g.dimension);
 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     graph[i][j] = g.graph[i][j];
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
inline void d_graph::erase()
{
 if (dimension != 0) {
   for (aut_id i=0; i<dimension; i++)
     delete[] graph[i];
   delete[] graph;
   dimension = 0;
 }
}

     //--------------------------------------------------------------------------//
     //                     Number of automata in the model                      //
     //--------------------------------------------------------------------------//
inline int d_graph::size() const
{
 return(dimension);
}

     //--------------------------------------------------------------------------//
     //                    The allocation has not been done                      //
     //--------------------------------------------------------------------------//
inline bool d_graph::is_empty() const
{
 return(dimension == 0);
}
     //--------------------------------------------------------------------------//
     //                         The graph is a clique                            //
     //--------------------------------------------------------------------------//
inline bool d_graph::is_clique() const
{
 aut_id i,j;
 bool   answer = true;

 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     if (!(graph[i]) && (i!=j)) {
       answer = false;
       i = dimension;         // exit loop 'i'
       break;                 // exit loop 'j'
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                    The graph has no dependencies                         //
     //--------------------------------------------------------------------------//
inline bool d_graph::has_no_dep() const
{
 aut_id i,j;
 bool   answer = true;

 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     if ((graph[i]) && (i!=j)) {
       answer = false;
       i = dimension;         // exit loop 'i'
       break;                 // exit loop 'j'
 }
 return(answer);
}

     //--------------------------------------------------------------------------//
     //                        The graph has circuits                            //
     //--------------------------------------------------------------------------//
inline bool d_graph::has_circuits() const
{
 bool    answer;
 aut_set circuits;

 get_circ(circuits);                          // get the automata in the circuits
 if (circuits.is_empty()) answer = false;     // if no automata is in the circuits
 else                     answer = true;      // the graph has no circuits
 return(answer);
}

     //--------------------------------------------------------------------------//
     //               Automaton 'a' is parameter of automaton 'b'                //
     //--------------------------------------------------------------------------//
inline bool d_graph::param_of(const aut_id a, const aut_id b) const
{
#ifdef _PEPS_DEBUG_
  if ((a >= dimension) || (b >= dimension))
    Programming_Error("index(es) out of range", 1401)
#endif
 return(graph[b][a]);   // Element ij means that automaton j is parameter of automaton i
}
     //--------------------------------------------------------------------------//
     //                 Automaton 'a' depends on automaton 'b'                   //
     //--------------------------------------------------------------------------//
inline bool d_graph::dep_on(const aut_id a, const aut_id b) const
{
#ifdef _PEPS_DEBUG_
  if ((a >= dimension) || (b >= dimension))
    Programming_Error("index(es) out of range", 1402)
#endif
 return(graph[a][b]);      // Element ij means that automaton i depends from automaton j
}

     //--------------------------------------------------------------------------//
     //    Fills 's' with the automata that are parameters of automaton 'a'      //
     //--------------------------------------------------------------------------//
inline void d_graph::param_of(const aut_id a, aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1403)
  if (s.size() != dimension)
    Programming_Error("bad param_of call", 1404)
#endif
 for (aut_id i=0; i<dimension; i++)
   if (graph[a][i])
     s.put_in(i);
   else
     s.take_out(i);
}
     //--------------------------------------------------------------------------//
     //         Fills 's' with the automata that depends on automaton 'a'        //
     //--------------------------------------------------------------------------//
inline void d_graph::dep_on(const aut_id a, aut_set & s) const
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1405)
  if (s.size() != dimension)
    Programming_Error("bad depen_on call", 1406)
#endif
 for (aut_id i=0; i<dimension; i++)
   if (graph[i][a])
     s.put_in(i);
   else
     s.take_out(i);
}

     //--------------------------------------------------------------------------//
     //                       Puts that 'a' depends on 'b'                       //
     //--------------------------------------------------------------------------//
inline void d_graph::put(const aut_id a, const aut_id b)
{
#ifdef _PEPS_DEBUG_
  if ((a >= dimension) || (b >= dimension))
    Programming_Error("index(es) out of range", 1407)
#endif
 graph[a][b] = true;
}
     //--------------------------------------------------------------------------//
     //                   Puts that 'a' does not depend on 'b'                   //
     //--------------------------------------------------------------------------//
inline void d_graph::take(const aut_id a, const aut_id b)
{
#ifdef _PEPS_DEBUG_
  if ((a >= dimension) || (b >= dimension))
    Programming_Error("index(es) out of range", 1408)
#endif
 graph[a][b] = false;
}
     //--------------------------------------------------------------------------//
     //                   Puts that 'a' depends on subset 's'                    //
     //--------------------------------------------------------------------------//
inline void d_graph::put(const aut_id a, const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1409)
  if (s.size() != dimension)
    Programming_Error("bad param_of call", 1410)
#endif
 for (aut_id i=0; i<dimension; i++)
   graph[a][i] = s.has(i);
}
     //--------------------------------------------------------------------------//
     //          Puts that 'a' does not depend on any other automata             //
     //--------------------------------------------------------------------------//
inline void d_graph::take(const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (a >= dimension)
    Programming_Error("index out of range", 1411)
#endif
 for (aut_id i=0; i<dimension; i++)
   graph[a][i] = false;
}
     //--------------------------------------------------------------------------//
     //                   No automata depends on the subset 's'                  //
     //--------------------------------------------------------------------------//
inline void d_graph::take(const aut_set & s)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != dimension)
    Programming_Error("bad param_of call", 1412)
#endif
 aut_id i, j;

 for (j=0; j<dimension; j++)
   if (s.has(j))
     for (i=0; i<dimension; i++)
       graph[i][j] = false;
}
     //--------------------------------------------------------------------------//
     //                   Builds a graph with no dependencies                    //
     //--------------------------------------------------------------------------//
inline void d_graph::take()
{
 aut_id i, j;

 for (i=0; i<dimension; i++)
   for (j=0; j<dimension; j++)
     graph[i][j] = false;
}
