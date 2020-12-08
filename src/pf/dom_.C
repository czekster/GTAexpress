//==============================================================================//
//                                                                              //
//                                    Domain Class                              //
//                                                                              //
//==============================================================================//
//  This File:   dom.C                    Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is used to store the domain data obtained from SAN textual input //
// file.                                                                        //
//  A domain is a list of intervals. A domain can a simples interval ([0..3]),  //
// a list of number ([0, 1, 2, 3, 5, 7, 8, 9]), or a combination of simples     //
// intervals and number ([0..3], 5, [7..9]). The domain order must to increase. //
//==============================================================================// 

#include "peps.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
DOM_::DOM_()
{
  id       = NULL;
  elem     = NULL;
  length   = NULL;
  interv   = NULL;
  n_interv = NULL;
  n        = 0;
  ind      = 0;
  t_elem   = NULL;
  t_length = 0;
  t_n_int  = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
DOM_::~DOM_()
{
  for (int i=0; i<ind; i++){
    if (id[i])
      delete[] id[i];
    if (elem[i])
      delete[] elem[i];
    if (interv[i])
      delete[] interv[i];
  }

  if (id)
    delete[] id;
  if (elem)
    delete[] elem;
  if (length)
    delete[] length;
  if (n_interv)
    delete[] n_interv;
  if (t_elem)
    delete[] t_elem;
  
  ind      = 0;
  n        = 0;  
  t_length = 0;
  t_n_int  = 0;
}

//------------------------------------------------------------------------------
// build_dom - build a domain based in the temporary data
// in: _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::build_dom(char *_id)
{
  for (int i=0; i<ind; i++)           //search a domain with the same identifier 
    if (!strcmp(id[i], _id))
       return(0);                 // there are a domain with the same identifier
  if (ind >= n)
    resize();
  id[ind]     = new char[MAX_ID_NAME];
  elem[ind]   = new int[t_length];
  interv[ind] = new int[t_n_int+1];
  strcpy(id[ind],_id);                                 // copy domain identifier
  
  for (int i=0; i<t_length; i++)
    elem[ind][i] = t_elem[i];                                   // copy elements
  
  length[ind]    = t_length;                               // copy domain length
  interv[ind][0] = 0;                                // begin intervals building 
  n_interv[ind]  = 1;
  
  for (int j=1; j<t_length; j++)
    if (elem[ind][j] > (elem[ind][j-1]+1)){ // each time that difference between 
      interv[ind][n_interv[ind]] = j;       //  two elements is bigger than one, 
      n_interv[ind]++;                      // a new interval is created.
    }
  
  interv[ind][n_interv[ind]] = t_length;               // end intervals building
  
  delete [] t_elem;
  t_elem   =  NULL;    
  t_length = 0;
  t_n_int  = 0;
  ind++;
  return(1);
}

//------------------------------------------------------------------------------
// put_elem - put an element in the temporary domain
// in: _elem - integer number
//------------------------------------------------------------------------------
void DOM_::put_elem(int _elem)
{
  int *_t_elem;

  _t_elem = new int[t_length+1];                           // create a new array
  
  for (int i=0; i<t_length; i++)
    _t_elem[i] = t_elem[i];                             // copy the old elements
  _t_elem[t_length] = _elem;                              // put the new element
  
  if (t_elem)
    delete [] t_elem;
  t_elem = _t_elem;
  t_length++;
  t_n_int++;
}

//------------------------------------------------------------------------------
// put_elem - put an element in the temporary domain
// in: _elem - number (char format)
//------------------------------------------------------------------------------
void DOM_::put_elem(char *_elem)
{
  int *_t_elem;

  _t_elem = new int[t_length+1];
  
  for (int i=0; i<t_length; i++)
    _t_elem[i] = t_elem[i];
  _t_elem[t_length] = atol(_elem);                            // need convertion
  
  if (t_elem)
    delete [] t_elem;
  t_elem = _t_elem;
  t_length++;
  t_n_int++;
}

//------------------------------------------------------------------------------
// put_interv - put an array of elements in the temporary domain
// in: _first - first element
//     _last  - last element
//------------------------------------------------------------------------------
int DOM_::put_interv(int _first, int _last)
{
  int *_t_elem;

  if (_first > _last)                     // decreasing interval is not accepted
    return(0);
    
  _t_elem = new int[t_length+(_last-_first+1)];
  
  for (int i=0; i<t_length; i++)
    _t_elem[i] = t_elem[i];
  for (int i=_first; i<=_last; i++)
    _t_elem[t_length+(i-_first)] = i;        // put each element of the interval
    
  if (t_elem)
    delete [] t_elem;
  t_elem = _t_elem;
  t_length+=(_last-_first+1);
  t_n_int++;
  return(1);
}

//------------------------------------------------------------------------------
// put_interv - put an array of elements in the temporary domain
// in: _first - first element (char format)
//     _last  - last element  (char format)
//------------------------------------------------------------------------------
int DOM_::put_interv(char *_first, char *_last)
{
  int *_t_elem;
  int first, last;
  
  first = atoi(_first);
  last  = atoi(_last);
  
  if (first > last)                       // decreasing interval is not accepted
    return(0);
   
  _t_elem = new int[t_length+(last-first+1)];
  
  for (int i=0; i<t_length; i++)
    _t_elem[i] = t_elem[i];
  for (int i=first; i<=last; i++)
    _t_elem[t_length+(i-first)] = i;         // put each element of the interval
    
  if (t_elem)
    delete [] t_elem;
  t_elem = _t_elem;
  t_length+=(last-first+1);
  t_n_int++;
  return(1);
}

//------------------------------------------------------------------------------
// get_interv - get the pos-th interval of the domain _id
// in:  _id - domain identifier
//      pos - interval number
// out: _first - first element of the pos-th interval
//      _last  - last element of the pos-th interval
//------------------------------------------------------------------------------
int DOM_::get_interv(char *_id, int pos, int _first, int _last)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))                                // search the domain
      if (pos >= 0  && pos < n_interv[i]){
        _first = elem[i][interv[i][pos]];
        _last  = elem[i][interv[i][pos+1]-1];
        return(1);
      }
  return(0);
}

//------------------------------------------------------------------------------
// get_n_interv - return the number of intervals of the domain _id
// in:  _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::get_n_interv(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(n_interv[i]);
  return(0);
}

//------------------------------------------------------------------------------
// get_length - return the number of elemens of the domain _id
// in:  _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::get_length(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(length[i]);
  return(0);
}

//------------------------------------------------------------------------------
// get_first_pos - return the element of the first position 
// in:  _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::get_first_pos(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(elem[i][0]);
  return(-1);
}


//------------------------------------------------------------------------------
// get_last_pos - return the element of the last position 
// in:  _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::get_last_pos(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(elem[i][length[i]-1]);
  return(-1);
}


//------------------------------------------------------------------------------
// get_pos - return the element of the pos-th position 
// in:  _id - domain identifier
//      pos - element position
//------------------------------------------------------------------------------
int DOM_::get_pos(char *_id, int pos)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(elem[i][pos]);
}

//------------------------------------------------------------------------------
// is_dom - return true if _id is a domain identifier  
// in:  _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::is_dom(char *_id)
{
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id))
      return(1);
  return(0);
}

//------------------------------------------------------------------------------
// check_dom - return true if the _i-th domain has an increasing order 
// in:  _id - domain identifier
//------------------------------------------------------------------------------
int DOM_::check_dom(char *_id)
{
  int _i;
  
  for (int i=0; i<ind; i++)
    if (!strcmp(id[i], _id)){
      _i = i;
      break;
    }
    
  for (int j=1; j<length[_i]; j++)
    if (elem[_i][j] <= elem[_i][j-1])
      return(0);
  return(1);
}

//------------------------------------------------------------------------------
// print - print a readable version of the domain structure
//------------------------------------------------------------------------------
void DOM_::print()
{
///  int k;
  cout << "DOMAIN TABLE\n";
  cout << "Allocated positions: " << n << endl;
  cout << "Used position: " << ind << endl << endl;
  for (int i=0; i<ind; i++){
    cout << "Position: " << i << "\tIdentifier: " << id[i] << "\tLength: " 
         << length[i] << "\tIntervals: " << n_interv[i] << endl;
    cout << "Elements: ";
    for (int j=0; j<length[i]; j++)
      cout << elem[i][j] << " ";
    cout << "\nIntervals: ";
    for (int j=0; j<=n_interv[i]; j++)
      cout << interv[i][j] << " ";  
    cout << endl;
  }
}


//------------------------------------------------------------------------------
// resize - alloc space for more domains
//------------------------------------------------------------------------------
void DOM_::resize()
{ 
  char **_id;
  int  **_elem;
  int   *_length;
  int  **_interv;
  int   *_n_interv;

  n = n + 20;
  _id       = new char*[n];
  _elem     = new int*[n];
  _length   = new int[n];
  _interv   = new int*[n];
  _n_interv = new int[n];
  
  // alloc new strutures and copy domains
  for (int i=0; i<ind; i++){ 
    _id[i]     = new char[MAX_ID_NAME];
    _elem[i]   = new int[length[i]];
    _interv[i] = new int[n_interv[i]+1];
    strncpy(_id[i],id[i],MAX_ID_NAME);
    _length[i] = length[i];
    for (int j=0; j<length[i]; j++)
      _elem[i][j] = elem[i][j];
    _n_interv[i] = n_interv[i];
    for (int j=0; j<=n_interv[i]; j++)
      _interv[i][j] = interv[i][j];
  }
  
  // delete old structures
  for (int i=0; i<ind; i++){
    if (id[i])
      delete[] id[i];
    if (elem[i])
      delete[] elem[i];
    if (interv[i])
      delete[] interv[i];
  }

  if (id)
    delete[] id;
  if (elem)
    delete[] elem;
  if (length)
    delete[] length;
  if (interv)
    delete[] interv;
  if (n_interv)
    delete[] n_interv;
     
  id     = _id;
  elem   = _elem;
  length = _length;
  interv = _interv;
  n_interv = _n_interv;
}
