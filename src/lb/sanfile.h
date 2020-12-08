//#ifndef _SANFILE_H
//#define _SANFILE_H

//====================================================================================//
//                                                                                    //
//                           SAN File Declarations and includes                       //
//                                                                                    //
//====================================================================================//
//  This File:   sanfile.h                   Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    06/Mar/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 18/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This file has the declarations and includes that must to be used in all files of  //
// related with the recognition of SAN files.                                         //
//====================================================================================//

#include <stdlib.h>
//#include <stream.h>

#include <iostream>             // std i/o with streams (cin, cout, cerr)
#include <fstream>              // file streams (i/o)
#include <stdio.h>
#include <string.h> 
#include <math.h>
#if defined(WIN32)
  #include <windows.h>
  #define dlopen(a,b) LoadLibrary(a)
  #define find_dlsym(a,b,c) (a = (Funcadd*)GetProcAddress(b,c))
  #define dlsym(handle, func) GetProcAddress ((HMODULE)handle, func)
  #define dlclose(x) FreeLibrary((HMODULE)x)
  #define NO_DLERROR
  #define reg_file(x) 1
  #ifdef _WIN32_WCE
    #define dlsym(handle, func) GetProcAddress ((HMODULE)handle, TEXT (func))
  #else
    #define dlsym(handle, func) GetProcAddress ((HMODULE)handle, func)
  #endif
#else
  #include <dlfcn.h>             // handler for dinamic libraries (used for 'jit')
#endif

#ifndef _SANFILE_H_
#define _SANFILE_H_
#include "../pf/peps.h"
#endif

#include "net.h"
#include "autom.h"
#include "state.h"
#include "trans.h"
#include "event.h"
#include "inf2pos.h"
#include "func.h"
#include "dom.h"
#include "edic.h"
#include "yacc.H"
#include "error_msgs.h"

class NET;
class AUTOMATON;
class STATE;
class TRANS;
class EVENT;
class FUNC;
class DOM;
class EDIC;

//===========================================================================//
//    Defines                                                                //
//===========================================================================//

#define MAX_NAME     256                       // Maximun length of file names
#define FUNCTION     16384                     // Maximun length of functions
#define MAX_SIZE     16384                     // Maximun length of vectors and
                                               // matrices dimentions 
//#define DISCRETE 0
//#define CONTINUOUS 1
#define MAX_MODEL_NAME 256
#define MAX_AUTOMATON_NAME 256
#define MAX_STATE_NAME 256
#define MAX_EVENT_NAME 256
#define MAX_FUNC_NAME 256
//#define MAX_EXP_SIZE 16384
#define MAX_TABLE_SIZE 16384 

//#endif
