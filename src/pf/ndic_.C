//==============================================================================//
//                                                                              //
//                             Network Dictionary Class                         //
//                                                                              //
//==============================================================================//
//  This File:   ndic.C                   Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store the network dictionary (automata and states     //
// identifiers).                                                                //
//  Each automaton has an index associated to an identifier. Each state has two //
// indices associated, one to the state identifier and other to the automaton   //
// where it appears.                                                            //
//==============================================================================// 
#include "peps.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
NDIC_::NDIC_()
{
  id   = NULL;
  type = NULL;
  aut  = NULL;
  stt  = NULL;
  n    = 0;
  ind  = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
NDIC_::~NDIC_()
{
  for (int i=0; i<ind; i++)
    if (id[i])
      delete[] id[i];

  if (id)
    delete[] id;
  if (type)
    delete[] type;
  if (aut)
    delete[] aut;
  if (stt)
    delete[] stt;
 
  ind      = 0;
  n        = 0;  
}


//------------------------------------------------------------------------------
// put_aut - put a new automaton
// in: _id  - automaton identifier
//     _aut - automaton index 
//------------------------------------------------------------------------------
int NDIC_::put_aut(char *_id, int _aut)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return (0);

  if (ind >= n)
    resize();

  id[ind]  = new char[MAX_ID_NAME];
  strcpy(id[ind],_id);  
  type[ind] = AUT;
  aut[ind]  = _aut;
  stt[ind]  = -1;
  ind++;
  return (1);
}

//------------------------------------------------------------------------------
// put_stt - put a new state
// in: _id  - state identifier
//     _aut - concerned automaton index
//     _stt - state index
//------------------------------------------------------------------------------
int NDIC_::put_stt(char *_id, int _aut, int _stt)
{
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], _id) && (_aut == aut[i])))
      return (0);
   
  if (ind >= n)
    resize();
  
  id[ind]   = new char[MAX_ID_NAME];
  strcpy(id[ind],_id);  
  type[ind] = STT;
  aut[ind]  = _aut;
  stt[ind]  = _stt;
  ind++;
  return (1);
}

//------------------------------------------------------------------------------
// get_n_aut - return number of automata
//------------------------------------------------------------------------------
int NDIC_::get_n_aut()
{
  int biggest = 0;
  for (int i=0; i<ind; i++)
    if (type[i] == AUT && aut[i]>biggest)
      biggest = aut[i];

  return (biggest+1);
}

//------------------------------------------------------------------------------
// get_n_stt - return number of state of an automaton
// in: _aut - automaton index
//------------------------------------------------------------------------------
int NDIC_::get_n_stt(int _aut)
{
  int biggest = 0;
  for (int i=0; i<ind; i++)
    if (type[i] == STT && aut[i]==_aut && stt[i]>biggest)
      biggest = stt[i];
  
  return(biggest+1);
}

//------------------------------------------------------------------------------
// is_aut - return true or false if the identifier is an automaton identifier
// in: _id - identifier
//------------------------------------------------------------------------------
int NDIC_::is_aut(char *_id)
{
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], _id)) && (type[i] == AUT))
      return(1);
  return(0);       
}

//------------------------------------------------------------------------------
// get_aut - return the automaton index 
// in: _id - automaton identifier
//------------------------------------------------------------------------------
int NDIC_::get_aut(char *_id)
{
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], _id)) && (type[i] == AUT))
      return(aut[i]);
  return(-1);       
}

//------------------------------------------------------------------------------
// get_aut_rep - return the automaton index to a replicated automaton 
// in: _id  - automaton identifier
//     _pos - replicated position
//------------------------------------------------------------------------------
int NDIC_::get_aut_rep(char *_id, int _pos)
{
  char aux[MAX_ID_NAME];
  
  sprintf(aux, "%s[%d]", _id, _pos);
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], aux)) &&  (type[i] == AUT))
      return(aut[i]);
  return (-1);       
}

//------------------------------------------------------------------------------
// get_aut_name - get the automaton name
// in: _aut  - automaton index
//     _name - automaton name
//------------------------------------------------------------------------------
int NDIC_::get_aut_name(int _aut, char *_name)
{
  for (int i=0; i<ind; i++)
    if (type[i] == AUT && aut[i] == _aut){
      strcpy(_name, id[i]);
      return (1);
    }
  return (0);
}

//------------------------------------------------------------------------------
// is_stt - return true or false if the identifier is a state identifier
// in: _id - identifier
//------------------------------------------------------------------------------
int NDIC_::is_stt(char *_id)
{
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], _id)) && (type[i] == STT))
      return (1);
  return (0);       
}

//------------------------------------------------------------------------------
// get_stt - return the state index 
// in: _aut - automaton index
//     _id  - state identifier
//------------------------------------------------------------------------------
int NDIC_::get_stt(int _aut, char *_id)
{
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], _id)) && (aut[i] == _aut) && (type[i] == STT))
      return (stt[i]);
  
  return (-1);       
}


//------------------------------------------------------------------------------
// get_stt_rep - return the state index to a replicated state 
// in: _aut - automaton index
//     _id  - state identifier
//     _pos - replicated position
//------------------------------------------------------------------------------
int NDIC_::get_stt_rep(int _aut, char *_id, int _pos)
{
  char aux[MAX_ID_NAME];
  
  sprintf(aux, "%s[%d]", _id, _pos);
  for (int i=0; i<ind; i++)
    if ((!strcmp(id[i], aux)) && (aut[i] == _aut) && (type[i] == STT))
      return (stt[i]);
  return (-1);       
}

//------------------------------------------------------------------------------
// get_stt_name - get the stt name
// in: _aut  - automaton index
//     _stt  - state index
//     _name - automaton name
//------------------------------------------------------------------------------
int NDIC_::get_stt_name(int _aut, int _stt, char *_name)
{
  for (int i=0; i<ind; i++)
    if (type[i] == STT && aut[i] == _aut && stt[i] == _stt){
      strcpy(_name, id[i]);
      return (1);
    }
  return (0);
}

//------------------------------------------------------------------------------
// check_stt_in_aut - return true or false if the state identifier exists in an 
//                    automata interval
// in: _id   - state identifier
//     _faut - first automaton index
//     _laut - last automaton index
//------------------------------------------------------------------------------
int NDIC_::check_stt_in_aut(char *_id, int _faut, int _laut)
{
  int flag;
  
  for (int i=_faut; i<=_laut; i++){
    flag = 1;
    for (int j=0; j<ind; j++)
      if ((!strcmp(id[j], _id)) && (aut[j] == i) && (type[j] == STT))
        flag = 0;
    if (flag)
      return (0); 
  }
  return (1);
}


//------------------------------------------------------------------------------
// print - print a readable version of the domain structure
//------------------------------------------------------------------------------
void NDIC_::print() // refazer
{ 
  cout << "\tAUTOMATA/STATES TABLE\n";
  cout << "Allocated positions: " << n << endl;
  cout << "Used positions: " << ind << endl << endl;
  for (int i=0; i<ind; i++){
    cout << i << "\t" << id[i] << "\t" << aut[i] << "\t" << stt[i] << "\ttipo: " << type[i] << endl;
  }
}

//------------------------------------------------------------------------------
// resize - alloc space for more identifiers
//------------------------------------------------------------------------------
void NDIC_::resize()
{ 
  char **_id;
  int   *_type;
  int   *_aut;
  int   *_stt;

  n = n + 20;
  _id   = new char*[n];
  _type = new int[n];
  _aut  = new int[n];
  _stt  = new int[n];
  
  // alloc new strutures and copy identifiers
  for (int i=0; i<ind; i++){ 
    _id[i] = new char[MAX_ID_NAME];
    strncpy(_id[i], id[i], MAX_ID_NAME);
    _type[i] = type[i];
    _aut[i]  = aut[i];
    _stt[i]  = stt[i];
  }
  // delete old structures
  for (int i=0; i<ind; i++)
    if (id[i])
      delete[] id[i];
  if (id)
    delete[] id;
  if (type)
    delete[] type;
  if (aut)
    delete[] aut;
  if (stt)
    delete[] stt;
      
  id   = _id;
  type = _type;
  aut  = _aut;
  stt  = _stt;
}

