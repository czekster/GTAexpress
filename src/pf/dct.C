//====================================================================================//
//                                                                                    //
//                          Dictinaire Table Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   dct.C                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//====================================================================================//
//  Creation:    19/feb/98                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 17/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
#include "peps.h"

     //--------------------------------------------------------------------------//
     //                 static variable: The Dictionaire Table                   //
     //--------------------------------------------------------------------------//
DCT DCT::dct = DCT();

     //--------------------------------------------------------------------------//
     //                          Empty Constructor                               //
     //--------------------------------------------------------------------------//
DCT::DCT()
{
 strcpy(file_name, "\0");
 automata.create();
 events = 0;
 g_funcs = 0;
 s_funcs = 0;
}
     //--------------------------------------------------------------------------//
     //                          Sized Constructor                               //
     //--------------------------------------------------------------------------//
DCT::DCT(const aut_st & s, const ev_id e, const int gf, const int sf)
{
 automata.copy(s);
 events = e;
 g_funcs = gf;
 s_funcs = sf;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                           Copy Constructor                               //
     //--------------------------------------------------------------------------//
DCT::DCT(const DCT & t)
{
 aut_id  a;
 ev_id   e;
 st_id   s;
 func_id f;;

 automata.copy(t.automata);
 events  = t.events;
 g_funcs = t.g_funcs;
 s_funcs = t.s_funcs;
 alloc();
 
 for (a=0; a<automata.size(); a++) {
   strcpy(aut_names[a], t.aut_names[a]);
   for (s=0; s<automata.sz(a); s++)
     strcpy(st_names[a][s], t.st_names[a][s]);
 }
 for (e=0; e<events; e++)
   strcpy(ev_names[e], t.ev_names[e]);
 for (f=0; f<g_funcs; f++) {
   strcpy(gf_names[f], t.gf_names[f]);
   strcpy(gf_exp[f],   t.gf_exp[f]);
 }   
 for (f=0; f<s_funcs; f++) {
   strcpy(sf_names[f], t.sf_names[f]);
   strcpy(sf_exp[f],   t.sf_exp[f]);
 }
}
     //--------------------------------------------------------------------------//
     //                              Destructor                                  //
     //--------------------------------------------------------------------------//
DCT::~DCT()
{
 if (automata.size() != 0) {
   desal();
   events  = 0;
   s_funcs = 0;
   g_funcs = 0;
   automata.erase();
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
void DCT::create()
{
 strcpy(file_name, "\0");
 automata.create();
 events = 0;
 g_funcs = 0;
 s_funcs = 0;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
void DCT::create(const aut_st & s, const ev_id e, const int gf, const int sf)
{
 erase();
 automata.copy(s);
 events  = e;
 g_funcs = gf;
 s_funcs = sf;
 alloc();
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
void DCT::copy(const DCT & t)
{
 aut_id  a;
 ev_id   e;
 st_id   s;
 func_id f;

 erase();
 automata.copy(t.automata);
 g_funcs = t.g_funcs;
 s_funcs = t.s_funcs;
 alloc();
 for (a=0; a<automata.size(); a++) {
   strcpy(aut_names[a], t.aut_names[a]);
   for (s=0; s<automata.sz(a); s++)
     strcpy(st_names[a][s], t.st_names[a][s]);
 }
 for (e=0; e<events; e++)
   strcpy(ev_names[e], t.ev_names[e]);
 for (f=0; f<g_funcs; f++) {
   strcpy(gf_names[f], t.gf_names[f]);
   strcpy(gf_exp[f],   t.gf_exp[f]);
 }   
 for (f=0; f<s_funcs; f++) {
   strcpy(sf_names[f], t.sf_names[f]);
   strcpy(sf_exp[f],   t.sf_exp[f]);
 }
}
     //--------------------------------------------------------------------------//
     //                            Destruction                                   //
     //--------------------------------------------------------------------------//
void DCT::erase()
{
 if (automata.size() != 0) {
   desal();
   events  = 0;
   s_funcs = 0;
   g_funcs = 0;
   automata.erase();
 }
 strcpy(file_name, "\0");
}


     //--------------------------------------------------------------------------//
     //                    The number of automata is zero                        //
     //--------------------------------------------------------------------------//
bool DCT::is_empty() const
{
 return(automata.size() == 0);
}

     //--------------------------------------------------------------------------//
     //                          It assigns a file name                          //
     //--------------------------------------------------------------------------//
void DCT::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                         It returns the file name                         //
     //--------------------------------------------------------------------------//
void DCT::name(peps_name file) const
{
 strcpy(file, file_name);
}
     //--------------------------------------------------------------------------//
     //                          The number of automata                          //
     //--------------------------------------------------------------------------//
int DCT::size() const
{
 return(automata.size());
}
     //--------------------------------------------------------------------------//
     //                          The number of automata                          //
     //--------------------------------------------------------------------------//
aut_id DCT::aut() const
{
 return(automata.size());
}
     //--------------------------------------------------------------------------//
     //                    The number of synchronizing events                    //
     //--------------------------------------------------------------------------//
ev_id DCT::ev() const
{
 return(events);
}
     //--------------------------------------------------------------------------//
     //                         The size of each automata                        //
     //--------------------------------------------------------------------------//
void DCT::a_sz(aut_st & s) const
{
 s.copy(automata);
}
     //--------------------------------------------------------------------------//
     //                      The number of general functions                     //
     //--------------------------------------------------------------------------//
int DCT::gfs() const
{
 return(g_funcs);
}
     //--------------------------------------------------------------------------//
     //                    The number of integration functions                   //
     //--------------------------------------------------------------------------//
int DCT::sfs() const
{
 return(s_funcs);
}

     //--------------------------------------------------------------------------//
     //                     It saves the name of the network                     //
     //--------------------------------------------------------------------------//
void DCT::n_bapt(const peps_name name)
{
 strcpy(san_name, name);
}
     //--------------------------------------------------------------------------//
     //                     It saves the name of automata 'a'                    //
     //--------------------------------------------------------------------------//
void DCT::a_bapt(const aut_id a, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata.size())
    Programming_Error("automaton index out of range", 6201)
#endif
 strcpy(aut_names[a], name);
}
     //--------------------------------------------------------------------------//
     //                       It saves the name of event 'e'                     //
     //--------------------------------------------------------------------------//
void DCT::e_bapt(const ev_id e, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (e >= events)
    Programming_Error("event index out of range", 6202)
#endif
 strcpy(ev_names[e], name);
}
     //--------------------------------------------------------------------------//
     //               It saves the name of state 's' of automata 'a'             //
     //--------------------------------------------------------------------------//
void DCT::s_bapt(const aut_id a, const st_id s, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (a >= automata.size())
    Programming_Error("automaton index out of range", 6203)
  if (s >= automata.sz(a))
    Programming_Error("state index out of range", 6204)
#endif
 strcpy(st_names[a][s], name);
}
     //--------------------------------------------------------------------------//
     //                 It saves the name of general function 'f'                //
     //--------------------------------------------------------------------------//
void DCT::gf_bapt(const func_id f, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (f >= g_funcs)
    Programming_Error("general function index out of range", 6205)
#endif
 strcpy(gf_names[f], name);
}
     //--------------------------------------------------------------------------//
     //               It saves the name of integration function 'f'              //
     //--------------------------------------------------------------------------//
void DCT::sf_bapt(const func_id f, const peps_name name)
{
#ifdef _PEPS_DEBUG_
  if (f >= s_funcs)
    Programming_Error("integration function index out of range", 6206)
#endif
 strcpy(sf_names[f], name);
}
     //--------------------------------------------------------------------------//
     //             It saves the expression of reachability function             //
     //--------------------------------------------------------------------------//
void DCT::r_dexp(const peps_expr expr)
{
 strcpy(reach_func, expr);
}
     //--------------------------------------------------------------------------//
     //              It saves the expression of general function 'f'             //
     //--------------------------------------------------------------------------//
void DCT::gf_dexp(const func_id f, const peps_expr expr)
{
#ifdef _PEPS_DEBUG_
  if (f >= g_funcs)
    Programming_Error("general function index out of range", 6207)
#endif
 strcpy(gf_exp[f], expr);
}
     //--------------------------------------------------------------------------//
     //            It saves the expression of integration function 'f'           //
     //--------------------------------------------------------------------------//
void DCT::sf_dexp(const func_id f, const peps_expr expr)
{
#ifdef _PEPS_DEBUG_
  if (f >= s_funcs)
    Programming_Error("general function index out of range", 6208)
#endif
 strcpy(sf_exp[f], expr);
}

     //--------------------------------------------------------------------------//
     //                    It returns the name of the network                    //
     //--------------------------------------------------------------------------//
void DCT::n_name(peps_name name) const
{
 strcpy(name, san_name);
}
     //--------------------------------------------------------------------------//
     //                    It returns the name of automata 'a'                   //
     //--------------------------------------------------------------------------//
void DCT::a_name(const aut_id a, peps_name name) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata.size())
    Programming_Error("automaton index out of range", 6209)
#endif
 strcpy(name, aut_names[a]);
}
     //--------------------------------------------------------------------------//
     //                      It returns the name of event 'e'                    //
     //--------------------------------------------------------------------------//
void DCT::e_name(const ev_id e, peps_name name) const
{
#ifdef _PEPS_DEBUG_
  if (e >= events)
    Programming_Error("event index out of range", 6210)
#endif
 strcpy(name, ev_names[e]);
}
     //--------------------------------------------------------------------------//
     //              It returns the name of state 's' of automata 'a'            //
     //--------------------------------------------------------------------------//
void DCT::s_name(const aut_id a, const st_id s, peps_name name) const
{
#ifdef _PEPS_DEBUG_
  if (a >= automata.size())
    Programming_Error("automaton index out of range", 6211)
  if (s >= automata.sz(a))
    Programming_Error("state index out of range", 6212)
#endif
 strcpy(name, st_names[a][s]);
}
     //--------------------------------------------------------------------------//
     //                It returns the name of general function 'f'               //
     //--------------------------------------------------------------------------//
void DCT::gf_name(const func_id f, peps_name name) const
{
#ifdef _PEPS_DEBUG_
  if (f >= g_funcs)
    Programming_Error("general function index out of range", 6213)
#endif
 strcpy(name, gf_names[f]);
}
     //--------------------------------------------------------------------------//
     //              It returns the name of integration function 'f'             //
     //--------------------------------------------------------------------------//
void DCT::sf_name(const func_id f, peps_name name) const
{
#ifdef _PEPS_DEBUG_
  if (f >= s_funcs)
    Programming_Error("integration function index out of range", 6214)
#endif
 strcpy(name, sf_names[f]);
}

     //--------------------------------------------------------------------------//
     //          It returns the expression of the reachability function          //
     //--------------------------------------------------------------------------//
void DCT::r_eexp(peps_expr expr) const
{
 strcpy(expr, reach_func);
}
     //--------------------------------------------------------------------------//
     //             It returns the expression of general function 'f'            //
     //--------------------------------------------------------------------------//
void DCT::gf_eexp(const func_id f, peps_expr expr) const
{
#ifdef _PEPS_DEBUG_
  if (f >= g_funcs)
    Programming_Error("general function index out of range", 6215)
#endif
 strcpy(expr, gf_exp[f]);
}
     //--------------------------------------------------------------------------//
     //           It returns the expression of integration function 'f'          //
     //--------------------------------------------------------------------------//
void DCT::sf_eexp(const func_id f, peps_expr expr) const
{
#ifdef _PEPS_DEBUG_
  if (f >= s_funcs)
    Programming_Error("integration function index out of range", 6216)
#endif
 strcpy(expr, sf_exp[f]);
}

     //--------------------------------------------------------------------------//
     //              It prints a readable version of the dictionaire             //
     //--------------------------------------------------------------------------//
void DCT::print() const
{
 ofstream  fout;                // output file
 peps_name model_name;          // dictionnary name whitout the subdirectoty path
 aut_id    a;                   // counter for automata
 ev_id     e;                   // counter for events
 st_id     s;                   // counter for states
 func_id   f;                   // counter for general and integration functions

 Get_Only_the_File_Name(file_name, model_name);
 Open_File("dct", dbg_file, fout);
 fout << "=====================================================================\n"
      << "Dictionary for model " << san_name << "(on file dct.dbg)\n"
      << "\nNumber of Automata:              " << automata.size()
      << "\nNumber of Synchronizing Events:  " << events
      << "\nNumber of Functions:             " << g_funcs
      << "\nNumber of Results:               " << s_funcs
      << "\nReachability Function:           \" " << reach_func << " \"\n"
      << "=====================================================================\n";
 fout << "\nAutomata\n";
 for (a=0; a<automata.size(); a++) {
   fout << "Aut. " << a << " : " << aut_names[a]
        << " (" << automata.sz(a) << " states)\n";
   for (s=0; s<automata.sz(a); s++)
     fout << "   st. " << s << " : " << st_names[a][s] << "\n";
 }
 fout << "=====================================================================\n"
      << "\nSynchronizing Events\n";
 for (e=0; e<events; e++)
   fout << "Event " << e << " : " << ev_names[e] << "\n";
 fout << "=====================================================================\n"
      << "\nFunctions\n";
 for (f=0; f<g_funcs; f++)
   fout << "F" << f << " : " << gf_names[f]
        << " exp: \" " << gf_exp[f] << " \"\n";
 fout << "=====================================================================\n"
      << "\nResults\n";
 for (f=0; f<s_funcs; f++)
   fout << "F" << f << " : " << sf_names[f]
        << " exp: \" " << sf_exp[f] << " \"\n";
 Close_File("dct", dbg_file, fout);
}
     //--------------------------------------------------------------------------//
     //          It shows a readable version of the dictionaire on 'cout'        //
     //--------------------------------------------------------------------------//
void DCT::show() const
{
 peps_name model_name;          // dictionnary name whitout the subdirectoty path
 aut_id    a;                   // counter for automata
 ev_id     e;                   // counter for events
 st_id     s;                   // counter for states
 func_id   f;                   // counter for general and integration functions

 Get_Only_the_File_Name(file_name, model_name);
 cout << "=====================================================================\n"
      << "Dictionary for model " << san_name << "(on file " << model_name << ".dct)\n"
      << "\nNumber of Automata:              " << automata.size()
      << "\nNumber of Synchronizing Events:  " << events
      << "\nNumber of Functions:             " << g_funcs
      << "\nNumber of Results:               " << s_funcs
      << "\nReachability Function:           \" " << reach_func << " \"\n"
      << "=====================================================================\n"
      << "Automata\n";
 for (a=0; a<automata.size(); a++) {
   cout << "Aut. " << a << " : " << aut_names[a]
        << " (" << automata.sz(a) << " states)\n";
   for (s=0; s<automata.sz(a); s++)
     cout << "   st. " << s << " : " << st_names[a][s] << "\n";
 }
 cout << "=====================================================================\n"
      << "Synchronizing Events\n";
 for (e=0; e<events; e++)
   cout << "Event " << e << " : " << ev_names[e] << "\n";
 cout << "=====================================================================\n"
      << "Functions\n";
 for (f=0; f<g_funcs; f++)
   cout << "F" << f << " : " << gf_names[f]
        << " exp: \" " << gf_exp[f] << " \"\n";
 cout << "=====================================================================\n"
      << "Results\n";
 for (f=0; f<s_funcs; f++)
   cout << "F" << f << " : " << sf_names[f]
        << " exp: \" " << sf_exp[f] << " \"\n";
 cout << "=====================================================================\n";
}
     //--------------------------------------------------------------------------//
     //                  It shows the local state names on 'cout'                //
     //--------------------------------------------------------------------------//
void DCT::show_states()
{
 aut_id   a;                      // counter for automata
 st_id    s;                      // counter for states

 cout << "Model " << san_name << "(on file " << file_name << ".dct)\n";
 for (a=0; a<automata.size(); a++) {
   cout << "Aut. " << a << " : " << aut_names[a]
        << " (" << automata.sz(a) << " states)\n";
   for (s=0; s<automata.sz(a); s++)
     cout << "   st. " << s << " : " << st_names[a][s] << "\n";
 }
}

     //--------------------------------------------------------------------------//
     //                      Output on 'fout' file stream                        //
     //--------------------------------------------------------------------------//
void DCT::write() const
{
 ofstream fout;                   // output file
 aut_id   a;                      // counter for automata
 ev_id    e;                      // counter for events
 st_id    s;                      // counter for states
 func_id  f;                      // counter for general and integration functions

 Open_File(file_name, dct_file, fout);
 automata.write(fout);
 fout << events << " " << g_funcs << " " << s_funcs << "\n";
 fout << san_name << " \" " << reach_func << " \"\n";
 for (a=0; a<automata.size(); a++) {
   fout << aut_names[a] << "\n";
   for (s=0; s<automata.sz(a); s++)
     fout << "    " << st_names[a][s] << "\n";
 }
 for (e=0; e<events; e++)
   fout << ev_names[e] << "\n";
 fout << "\n";
 for (f=0; f<g_funcs; f++)
   fout << gf_names[f] << " \" " << gf_exp[f] << " \"\n";
 fout << "\n";
 for (f=0; f<s_funcs; f++)
   fout << sf_names[f] << " \" " << sf_exp[f] << " \"\n";
 Close_File(file_name, dct_file, fout);
}
     //--------------------------------------------------------------------------//
     //                      Input from 'fin' file stream                        //
     //--------------------------------------------------------------------------//
void DCT::read(const peps_name file)
{
 ifstream  fin;
 aut_id    a;                      // counter for automata
 ev_id     e;                      // counter for events
 st_id     s;                      // counter for states
 func_id   f;                      // counter for general and integration functions
 peps_name buf;                    // buffer to read function expression
 bool      reading = true;         // flag to read until a function expression ends

 Open_File(file, dct_file, fin);
 baptise(file);
 automata.read(fin);
 fin >> events;
 fin >> g_funcs;
 fin >> s_funcs;
 fin >> san_name;
 fin >> buf;
 if (strcmp(buf, "\""))
   Compilation_Error("Reading internal dictionary", "bad reachability function expression")
 fin >> reach_func;
 do { fin >> buf;
      if (strcmp(buf, "\"")) {
        strcat(reach_func, " ");
        strcat(reach_func, buf);
      }
      else
        reading = false;
 } while (reading);
 reading = true;
 alloc();                        // allocates 'this'

 for (a=0; a<automata.size(); a++) {
   fin >> aut_names[a];
   for (s=0; s<automata.sz(a); s++)
     fin >> st_names[a][s];
 }
 for (e=0; e<events; e++)
   fin >> ev_names[e];
 for (f=0; f<g_funcs; f++) {
   fin >> gf_names[f];
   fin >> buf;
   if (strcmp(buf, "\""))
     Compilation_Error("Reading internal dictionary", "bad general function expression")
   fin >> gf_exp[f];
   do { fin >> buf;
        if (strcmp(buf, "\"")) {
          strcat(gf_exp[f], " ");
          strcat(gf_exp[f], buf);
        }
        else 
          reading = false;
   } while (reading);
   reading = true;
 }
 for (f=0; f<s_funcs; f++) {
   fin >> sf_names[f];
   fin >> buf;
   if (strcmp(buf, "\""))
     Compilation_Error("Reading internal dictionary", "bad general function expression")
   fin >> sf_exp[f];
   do { fin >> buf;
        if (strcmp(buf, "\"")) {
          strcat(sf_exp[f], " ");
          strcat(sf_exp[f], buf);
        }
        else
          reading = false;
   } while (reading);
   reading = true;
 }
 Close_File(file_name, dct_file, fin);
}

     //--------------------------------------------------------------------------//
     //                       Internal Function of allocation                    //
     //--------------------------------------------------------------------------//
void DCT::alloc()
{
 aut_names = new peps_name[automata.size()];
 st_names  = new peps_name*[automata.size()];
 for (aut_id a=0; a<automata.size(); a++)
   st_names[a] = new peps_name[automata.sz(a)];
 ev_names = new peps_name[events];
 gf_names = new peps_name[g_funcs];
 sf_names = new peps_name[s_funcs];
 gf_exp = new peps_name[g_funcs];
 sf_exp = new peps_name[s_funcs];
}
     //--------------------------------------------------------------------------//
     //                     Internal Function of desallocation                   //
     //--------------------------------------------------------------------------//
void DCT::desal()
{
 delete[] aut_names;
 for (aut_id a=0; a<automata.size(); a++)
   delete[] st_names[a];
 delete[] ev_names;
 delete[] st_names;
 delete[] gf_names;
 delete[] sf_names;
 delete[] gf_exp;
 delete[] sf_exp;
}
