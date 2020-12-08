//==============================================================================//
//                                                                              //
//                             Event Dictionary Class                           //
//                                                                              //
//==============================================================================//
//  This File:   edic.C                   Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store events information extracted from SAN textual   //
// input file.                                                                  //
//  Each event is defined by an identifier, a type, a rate, the master automaton//
// and a vector with the concerned automata.                                    //
//  The event type can be synchronizing (SYN) or local (LOC).                   //
//  The event rate is the function index associated to the event.               //
//  The master automaton is the first concerned automaton.                      //
//==============================================================================// 

#include "peps.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
EDIC_::EDIC_()
{
  id    = NULL;
  type  = NULL;
  rate  = NULL;
  c_aut = NULL;
  n_aut = 0;
  n     = 0;
  ind   = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
EDIC_::~EDIC_()
{
  for (int i=0; i<ind; i++){
    if (id[i])
      delete[] id[i];
    if (c_aut[i])
      delete[] c_aut[i];
  }

  if (id)
    delete[] id;
  if (type)
    delete[] type;
  if (rate)
    delete[] rate;
  if (c_aut)
    delete[] c_aut;
    
  n_aut    = 0; 
  ind      = 0;
  n        = 0;  
}

//------------------------------------------------------------------------------
// put_ev - create a new event
// in: _id   - event identifier
//     _type - event type (LOC or SYN)
//     _rate - event rate (function index associated to the evento)
//------------------------------------------------------------------------------
int EDIC_::put_ev(char *_id, int _type, int _rate)
{
   for (int i=0; i<ind; i++)          //search an event with the same identifier 
    if (!strcmp(id[i], _id))
       return(0);                 // there are an event with the same identifier
  if (ind >= n)
    resize();

  id[ind]    = new char[MAX_ID_NAME];
  c_aut[ind] = new int[n_aut];
  strcpy(id[ind],_id);  
  for (int i=0; i<n_aut; i++)
    c_aut[ind][i] = 0;
  type[ind] = _type;
  rate[ind] = _rate;
  ind++;
  return(1);
}

//------------------------------------------------------------------------------
// concerned_aut - it say that an event is declared in an automaton
// in: _id  - event identifier
//     _aut - automaton index
//------------------------------------------------------------------------------
void EDIC_::concerned_aut(char *_id, int _aut)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i],_id))
      c_aut[i][_aut] = 1;
}

//------------------------------------------------------------------------------
// put_n_aut - put the number of automata in the model
// in: _n_aut - number of automata
//------------------------------------------------------------------------------
void EDIC_::put_n_aut(int _n_aut)
{
  n_aut = _n_aut;
}

//------------------------------------------------------------------------------
// get_n_ev - return the number of events declared in the model
//------------------------------------------------------------------------------
int EDIC_::get_n_ev()
{
  return(ind);
}

//------------------------------------------------------------------------------
// get_ev - return the event index for the _id identifier
// in: _id - event identifier
//------------------------------------------------------------------------------
int EDIC_::get_ev(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i],_id))
      return(i);
}

//------------------------------------------------------------------------------
// get_ev_id - get the event identifier
// in:  i     - event index
// out: _name - event identifier
//------------------------------------------------------------------------------
void EDIC_::get_ev_id(int i, char *_id)
{
 strcpy(_id, id[i]);
}

//------------------------------------------------------------------------------
// get_ev_type - return the event type for the event index _ev
// in: _ev  - event index
//------------------------------------------------------------------------------
int EDIC_::get_ev_type(int _ev)
{
  return(type[_ev]);
}

//------------------------------------------------------------------------------
// get_ev_rate - return the event rate for the event index _ev
// in: _ev  - event index
//------------------------------------------------------------------------------
int EDIC_::get_ev_rate(int _ev)
{
  return(rate[_ev]);
}

//------------------------------------------------------------------------------
// get_n_synch_ev - return the number of synchronizing events
//------------------------------------------------------------------------------
int EDIC_::get_n_synch_ev()
{
  int n=0;
  for (int i=0; i<ind; i++)
    if (type[i] == SYN)
      n++;
  return(n);
}

//------------------------------------------------------------------------------
// get_real_synch_ev - return the event index for the i-th synchronizing event
// in: _synch_ev - synchronizing event number
//------------------------------------------------------------------------------
int EDIC_::get_real_synch_ev(int synch_ev)
{
  int n = 0;
  for (int i=0; i<ind; i++){
    if (type[i] == SYN)
      n++;
    if ((n - 1) == synch_ev)
      return (i);
  }
}

//------------------------------------------------------------------------------
// get_mst_aut - return the master automaton index 
// in: _ev  - event index
//------------------------------------------------------------------------------
int EDIC_::get_mst_aut(int _ev)
{
  for (int i=0; i<n_aut; i++)
    if (c_aut[_ev][i] == 1)
      return(i);
}

//------------------------------------------------------------------------------
// is_ev - return true if the identifier _id is an event and false if it is not
// in: _id - event identifier
//------------------------------------------------------------------------------
int EDIC_::is_ev(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i],_id))
      return(1);
  return(0);
}

//------------------------------------------------------------------------------
// is_concerned - return true or false if the event _ev if declared in the 
//                automaton _aut
// in: _ev  - event index
//     _aut - automaton index 
//------------------------------------------------------------------------------
int EDIC_::is_concerned(int _ev, int _aut)
{
  return(c_aut[_ev][_aut]);
}

//------------------------------------------------------------------------------
// check_ev - check if local events have only one automaton concerned and 
//            synchronizing events have two or more automata concerned
//------------------------------------------------------------------------------
void EDIC_::check_ev()
{
  int n;
  
  for (int i=0; i<ind; i++){
    n = 0;
    for (int j=0; j<n_aut; j++)
      if (c_aut[i][j])
        n++;
    if (n == 0)
      cout << "Warning: Event \"" << id[i] << "\" is declared but it is not used!" << endl;
    if ((type[i] == SYN) && (n == 1)){
      cout << "Warning: Event \"" << id[i] << "\" is a synchroning event but it is " 
           << "declared in only one automaton. It will be changed to local event.\n" 
           << "Please, check your model to correct your mistake." << endl;
      type[i] = LOC;
    }
    else{
      if ((type[i] == LOC) && (n > 1)){
        cout << "Warning: Event \"" << id[i] << "\" is a local event but it is " 
             << "declared in more than one automaton. It will be changed to "
             << "synchronizing event.\nPlease, check your model to correct your mistake." << endl;
        type[i] = SYN;
      }
    }
  }
}

//------------------------------------------------------------------------------
// print - print a readable version of the domain structure
//------------------------------------------------------------------------------
void EDIC_::print()
{
  cout << "EVENT TABLE\n";
  cout << "Allocated positions: " << n << endl;
  cout << "Used positions: " << ind << endl << endl;
  cout << "Position" << "Identifier" << "Type" << "Rate" << endl;
  for (int i=0; i<ind; i++){
    cout << i << "\t\t" << id[i] << "\t" << type[i] << "\t" << rate[i] << endl;
  }
  // concerned
}

//------------------------------------------------------------------------------
// resize - alloc space for more identifiers
//------------------------------------------------------------------------------
void EDIC_::resize()
{ 
  char **_id;
  int   *_type;
  int   *_rate;
  int  **_c_aut;

  n = n + 20;
  _id    = new char*[n];
  _type  = new int[n];
  _rate  = new int[n];
  _c_aut = new int*[n];

  for (int i=0; i<ind; i++){ 
    _id[i]    = new char[MAX_ID_NAME];
    _c_aut[i] = new int[n_aut];
    strcpy(_id[i],id[i]);
    for (int j=0; j<n_aut; j++)
      _c_aut[i][j] = c_aut[i][j];
    _type[i] = type[i];
    _rate[i] = rate[i];
  }
  // delete old structures
  for (int i=0; i<ind; i++){
    if (id[i])
      delete[] id[i];
    if (c_aut[i])
      delete[] c_aut[i];
  }
  if (id)
    delete[] id;
  if (c_aut)
    delete[] c_aut;
  if (type)
    delete[] type;
  if (rate)
    delete[] rate;
      
  id    = _id;
  c_aut = _c_aut;
  type  = _type;
  rate  = _rate;
}

