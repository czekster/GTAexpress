#include "grammar_.h"
//------------------------------------------------------------------------------
// Lexical_error - print a lexical error (unrecongnized symbol)
//------------------------------------------------------------------------------
void GRAMMAR_::Lexical_error()
{
  errorno++;
  cerr << " " << errorno << ":Line(" << lineno << ") - Lexical Error - \"" \
       << tk << "\" is an unrecognized symbol." << endl;
}

//------------------------------------------------------------------------------
// Syntactic_error - Print a syntactical error (unexpected symbol)
//------------------------------------------------------------------------------
void GRAMMAR_::Syntactic_error()
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Syntactical Error - \"" \
      << tk << "\" (not expected)." << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Syntactic_error - Print a syntactical error 
// in: s - expected symbol
//------------------------------------------------------------------------------
void GRAMMAR_::Syntactic_error(char *s)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Syntactical Error - \"" \
      << tk << "\" (not expected). " <<  s << " is expected." << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Dom_semantic_error - Print a semantical error in a domain
// in: s_error    - string error
//     _id        - domain identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Dom_semantic_error(char *s_error, char *_id, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Domain \"" \
      << _id << "\" (** " <<  s_error << " **). Error " << code_error << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Dom_semantic_error - Print a semantical error in a domain
// in: s_error    - string error
//     _id        - domain identifier
//     _tk        - function identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Dom_semantic_error(char *s_error, char *_id, char *_tk, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Domain \"" \
      << _id << "\" (** Funtion \"" << _tk << "\" "<<  s_error << " **). Error " << code_error << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Function_semantic_error - Print a semantical error in a function
// in: s_error    - string error
//     _id        - function identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Function_semantic_error(char *s_error, char *_id, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Function \"" \
      << _id << "\" (** " <<  s_error << " **). Error " << code_error << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Function_semantic_error - Print a semantical error in a function
// in: s_error    - string error
//     _id        - function identifier
//     _tk        - state name
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Function_semantic_error(char *s_error, char *_id,  char *_tk, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Function \"" \
      << _id << "\" (** \"" <<  _tk << "\" " << s_error << " **). Error " << code_error << endl;
 exit(0);
}



//------------------------------------------------------------------------------
// Function_semantic_warning - Print a semantical warning in a function
// in: s_error    - string error
//     _id        - function identifier
//     _tk        - state name
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Function_semantic_warning(char *s_error, char *_id,  char *_tk, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Warning - Function \"" \
      << _id << "\" (** State \"" <<  _tk << "\" " << s_error << " **). Error " << code_error << endl;
}

//------------------------------------------------------------------------------
// Event_semantic_error - Print a semantical error in an event
// in: s_error    - string error
//     _id        - event identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Event_semantic_error(char *s_error, char *_id, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Event \"" \
      << _id << "\" (** " <<  s_error << " **). Error " << code_error << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Event_semantic_error - Print a semantical error in an event
// in: s_error    - string error
//     _id        - event identifier
//     _tk        - function identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Event_semantic_error(char *s_error, char *_id,  char *_tk, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Event \"" \
      << _id << "\" (** \"" <<  _tk << "\" " << s_error << " **). Error " << code_error << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Network_semantic_error - Print a semantical error in the network block
// in: s_error    - string error
//     where      - Automaton, State, Transition, ..
//     _id        - automaton identifier
//     _tk        - identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Network_semantic_error(char *s_error, char *where, char *_id, char *_tk, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - " << where << " \"" \
      << _id << "\" (** \"" <<  _tk << "\" " << s_error << " **). Error " << code_error << endl;
 exit(0);
}

//------------------------------------------------------------------------------
// Network_semantic_error - Print a semantical error in the network block
// in: s_error    - string error
//     _id        - identifier
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Network_semantic_error(char *s_error, char *_id,  int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - (** \"" 
      <<  _id << "\" " << s_error << " **). Error " << code_error << endl;
 exit(0);
}


//------------------------------------------------------------------------------
// Result_semantic_error - Print a semantical error in a result function
// in: s_error    - string error
//     _id        - result function identifier
//     _tk        - state name
//     code_error - unique code error
//------------------------------------------------------------------------------
void GRAMMAR_::Result_semantic_error(char *s_error, char *_id, int code_error)
{
 errorno++;
 cerr << " " << errorno << ":Line(" << lineno << ") - Semantical Error - Result function \"" \
      << _id << "\" (** " << s_error << " **). Error " << code_error << endl;
 exit(0);
}
