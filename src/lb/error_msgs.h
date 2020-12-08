//====================================================================================//
//                                                                                    //
//                                   Error Messages                                   //
//                                                                                    //
//====================================================================================//
//  This File:   error_msgs.h                Language: C++                            //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    06/Mar/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 06/Mar/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
// This file has macro definitions to put error messages of PEPS parser               //
//====================================================================================//

#ifndef _ERROR_MSGS_H_
#define _ERROR_MSGS_H_

#define Lexical_Error(yacc_number, yacc_line, id) \
        cout << " " << yacc_number  \
             << ":Line(" << yacc_line \
             << ") - Lexical Error - \"" << id \
             << "\" is an unrecognized symbol." \
             << endl;

#define Sintatic_Error(yacc_number, yacc_line, str, id) \
        cout << " " << yacc_number  \
             << ":Line(" << yacc_line \
             << ") - " << str << " - \"" << id \
             << "\" (not expected)." \
             << endl;

#define Semantic_Error_YACC(yacc_number, yacc_line, id, number) \
        cout << " " << yacc_number  \
             << ":Line(" << yacc_line \
             << ") - Semantic Error - " << id \
             << "  (" << number << ")." \
             << endl;

#define Semantic_Error(text, number) \
        cout << "Semantic Error - " << number << " (** " \
             << text << " **).\n";

#define Semantic_Error2(text, text2, text3) \
        cout << "Semantic Error - Event " << text << " in automaton " << text2 << " (** " \
             << text3 << " **).\n";

#define Semantic_Error3(text_1, text_2, number) \
        cout << "Semantic Error - " << number << " (** " \
             << text_1 << " - " << text_2 << " **).\n";

#define Semantic_Error4(text_1, text_2, text_3) \
        cout << "Semantic Error - " << " (** " \
             << text_1 << " - " << text_2 << " in function " << text_3 << " **)." << endl;

#define Semantic_Error5(text, text2) \
        cout << "Semantic Error - Event " << text << " (** " \
             << text2 << " **).\n";

#define Semantic_Warning(text_1, text_2, number) \
        cout << "Semantic Warning - " << number << " (** " \
             << text_1 << " - " << text_2 << " **)." \
             << endl;

#define Semantic_Warning4(text_1, text_2, text_3, number) \
        cout << "Semantic Warning - " << number << " (** Function: " \
             << text_1 << " - State: " << text_2 << " - internal id is: " << text_3 << " **)." \
             << endl;

#define Programming_Error1(text) \
        cout << "Programming Error - at \"" << text \
             << "\" (file '" << __FILE__ << "' - line " << __LINE__ << ")" \
             << endl;

#define Programming_Error2(text, number) \
        cout << "Programming Error - " << number << " (at " \
             << text << ")\n" \
             << "(file '" << __FILE__ << "' - line " << __LINE__ << ")" \
             << endl;

// Semantic Errors - YACC (00 .. 99)
#define Parser_00 "already defined in this scope."
#define Parser_01 "identifier or dominion not declared."
#define Parser_02 "can not solve identifier."
#define Parser_03 "automaton identifier already defined."
#define Parser_04 "event not declared."
#define Parser_05 "state identifier already defined for this automaton."
#define Parser_06 "state identifier already defined as function identifier."

// Semantic Errors - ordinary files (100 - 199)
#define Parser_100 "Bad destination state (unknow state)"
#define Parser_101 "Bad destination state (state index out of range or state is not replicated)"  
#define Parser_102 "Replication of master automaton produce inconsistency"
#define Parser_103 "State name occurs in different positions for different automata. Please verify that your function takes this into account"
#define Parser_104 "State name does not occurs in all automata. Please verify that your function takes this into account" 
#define Parser_105 "Wrong automata interval (final automaton appears before initial or automaton identifiers do not exist)."
#define Parser_106 "Inconsistent synchronization (only one automaton synchronized)"
#define Parser_107 "Inconsistent synchronization (more than one master automaton)" 
#define Parser_108 "Bad function (unexpected error)"
#define Parser_109 "Non existent automaton"
#define Parser_110 "not declared"
#define Parser_111 "Destin state of the local event is to same state"
#define Parser_112 "Event name already defined for this transition"
#define Parser_113 "Inconsistent synchronization (master automaton and synchronized automaton are the same one)"
#define Parser_114 "Wrong interval (final limit appears before initial)."
#define Parser_115 "Bad destination state (this operation cannot be used with the \'from\' primitive)"
#define Parser_116 "Bad destination state (this operation can be used only in a replicated state)"

// Programming Errors (200 - 299)
#define Parser_200 "Out of range (see FUNCTION define)."
#define Parser_201 "Index out of range."
#define Parser_202 "Can not alloc memory."


#endif
