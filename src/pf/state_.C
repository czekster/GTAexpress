//==============================================================================//
//                                                                              //
//                                    State Class                               //
//                                                                              //
//==============================================================================//
//  This File:   state.h                  Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store a state with its transitions and events.        //
//  The state is defined by a name, a reward value, and a transition matrix. In //
// the transition matrix, each row represent a transition to one state, the     //
// number of rows is equal to the number of state in the automaton. The columns //
// represent the events. Each position (row, column) represent a transition to  //
// the state index row with the event index column.                             //
//  For exemple: one automaton with 3 states and 5 event in the model. The state//
// 1 has transtions to state 2 with event 1, 2, and 3 and to state 3 with event //
// 2 and 5. The transitions with event 2 have probabilities function 3 and 0    //
// associated to the transition state 2 and 3, respectively. The event 4 is not //
// declared in this state. The matrix to this state is following:               //
//   _|__1__2__3__4__5                                                          //
//   1|                                                                         //
//   2| -1 3 -1 -2 -2                                                           //
//   3| -2 0 -2 -2 -1                                                           //
//  In the matrix, the value -2 represent the no event occurrence in transition //
// The value -1 represent the event occurrence with probability 1 and the posi- //
// tive values represent a transition with probability functions. The value     //
// represent the function index. If no exist transition to a state, the row is  //
// NULL.                                                                        //
//==============================================================================// 
#include "peps.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
STATE_::STATE_()
{
  strcpy(name,"");
  rw    = 0.0;
  n_stt = 0;
  n_ev  = 0;
  tr_ev = NULL;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
STATE_::~STATE_()
{
  strcpy(name,"");
  rw    = 0.0;
  n_stt = 0;
  n_ev  = 0;

  for (int i=0; i<n_stt; i++)
    if (tr_ev[i])
      delete [] tr_ev[i];
  if (tr_ev)
    delete [] tr_ev;
}

//------------------------------------------------------------------------------
// create_tr - create the transition matrix
// in: _n_stt - number of states in the automaton
//------------------------------------------------------------------------------
void STATE_::create_tr(int _n_stt)
{
  n_stt = _n_stt;
  tr_ev = new int *[n_stt];                               // create states vetor
  for (int i=0; i<n_stt; i++)
    tr_ev[i] = NULL;                  // the event vector are create only if the 
                                      //     state has a transition to the state
}

//------------------------------------------------------------------------------
// put_name - put state name
// in: _name - state name
//------------------------------------------------------------------------------
void STATE_::put_name(char *_name)
{
  strcpy(name,_name);
}

//------------------------------------------------------------------------------
// put_reward - put state reward
// in: _rw - reward value
//------------------------------------------------------------------------------
void STATE_::put_reward(double _rw)
{
  rw = _rw;
}

//------------------------------------------------------------------------------
// put_ev - put event information
// in: _stto - target state index
//     _ev   - event index
//     _prob - probability function index 
//------------------------------------------------------------------------------
int STATE_::put_ev(int _stto, int _ev, int _prob)
{
  if (!tr_ev[_stto]){                  // create event vector if it do not exist
    tr_ev[_stto] = new int[n_ev];
    for (int i=0; i<n_ev; i++)
       tr_ev[_stto][i] = -2;              // set all position as undeclared (-2)
  }
  if (tr_ev[_stto][_ev] > -2)       // event already declared in this transition
    return(0);
  tr_ev[_stto][_ev] = _prob;
  return(1);
} 

//------------------------------------------------------------------------------
// info_ev - informe the number of events in the model
// in: _n_events - number of events 
//------------------------------------------------------------------------------
void STATE_::info_ev(int _n_ev)
{
  n_ev = _n_ev;
}

//------------------------------------------------------------------------------
// get_reward - return reward value associated to the state
//------------------------------------------------------------------------------
double STATE_::get_reward()
{
  return(rw);
}

//------------------------------------------------------------------------------
// get_ev - return the event information 
// in: _stto - target state index
//     _ev   - event index
//------------------------------------------------------------------------------
int STATE_::get_ev(int _stto, int _ev)
{
  if (tr_ev[_stto])
    return(tr_ev[_stto][_ev]);
  else
    return(-2);
}
    
//------------------------------------------------------------------------------
// print - print a readable version of the state structure
//------------------------------------------------------------------------------
void STATE_::print()
{
  cout << "\tState: " << name << "\t reward: " << rw << endl;
  for (int i=0; i<n_stt; i++){
     if (tr_ev[i]){
       cout << "\t\t" << i << ": ";
       for (int j=0; j<n_ev; j++){
         if (tr_ev[i][j] > -2)
           if (tr_ev[i][j] == -1)
             cout << j << " ";
           else
           cout << j << "(" << tr_ev[i][j] << ") ";
       }
       cout << endl;
     }
  }
}
