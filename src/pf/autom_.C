//==============================================================================//
//                                                                              //
//                                  Automaton Class                             //
//                                                                              //
//==============================================================================//
//  This File:   autom.h                  Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to model one automaton. It store the automaton name and  //
// the array of states.                                                         //
//==============================================================================// 
#include "peps.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
AUTOMATON_::AUTOMATON_()
{
  strcpy(name,"");
  n_stt = 0;
  stt   = NULL;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
AUTOMATON_::~AUTOMATON_()
{
  strcpy(name,"");
  n_stt = 0;
  if(stt)
    delete [] stt;
}

//------------------------------------------------------------------------------
// put_name - put the automaton name
// in: _name - automaton name
//------------------------------------------------------------------------------
void AUTOMATON_::put_name(char *_name)
{
  strcpy(name,_name);
}

//------------------------------------------------------------------------------
// info_ev - informe the number of events in the model
// in: _n_ev - number of events 
//------------------------------------------------------------------------------
void AUTOMATON_::info_ev(int _n_ev)
{
  for (int i=0; i<n_stt; i++)
    stt[i].info_ev(_n_ev);              // forward the information to each state
}

//------------------------------------------------------------------------------
// create_stt - create the set of state to the automaton
// in: _n_stt - number of states
//------------------------------------------------------------------------------
void AUTOMATON_::create_stt(int _n_stt)
{
  n_stt = _n_stt;
  stt = new STATE_[n_stt];                                // create set of states
  for(int i=0; i<n_stt; i++)
    stt[i].create_tr(n_stt);                 // create transitions to each state
}

//------------------------------------------------------------------------------
// put_stt - put state informations like: name, index, and reward
// in: _name - state name
//     _stt  - state index
//     _rw   - associated reward
//------------------------------------------------------------------------------
void AUTOMATON_::put_stt(char *_name, int _stt, double _rw)
{
 stt[_stt].put_name(_name);
 stt[_stt].put_reward(_rw);
}

//------------------------------------------------------------------------------
// get_stt_reward - return the reward associated to the state
// in: _stt - state index
//------------------------------------------------------------------------------
double AUTOMATON_::get_stt_reward(int _stt)
{
 return(stt[_stt].get_reward());
}

//------------------------------------------------------------------------------
// put_ev - put transition/event informations like: depart state, target state,
//          associated event, and transition probability
// in: _stt  - depart state index
//     _stto - target state index
//     _ev   - event index
//     _prob - transition probability
//------------------------------------------------------------------------------
void AUTOMATON_::put_ev(int _stt, int _stto, int _ev, int _prob)
{
 stt[_stt].put_ev(_stto, _ev, _prob);
}

//------------------------------------------------------------------------------
// get_ev - return the transition probability associated to the event _ev in a 
//          transition from _stt to _stto
// in: _stt  - depart state index
//     _stto - target state index
//     _ev   - event index
//------------------------------------------------------------------------------
int AUTOMATON_::get_ev(int _stt, int _stto, int _ev)
{
  return(stt[_stt].get_ev(_stto, _ev));
}

//------------------------------------------------------------------------------
// print - print a readable version of the automaton structure
//------------------------------------------------------------------------------
void AUTOMATON_::print()
{
  cout << "Automaton: " << name << endl;
  cout << "State:  " << n_stt << endl;
  for (int i =0; i<n_stt; i++){
    stt[i].print();
  }
}
