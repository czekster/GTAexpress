//==============================================================================//
//                                                                              //
//                                Main Grammar Class                            //
//                                                                              //
//==============================================================================//
//  This File:   grammar.h                Tool: C++                             //
//  Software:    PEPS                     Advisor: Brigitte.Plateau@imag.fr     //
//  Doc:                                                                        //
//==============================================================================//
//  Creation:    30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//  Last Change: 30/Aug/05                by: Leonardo.Brenner@imag.fr          //
//==============================================================================//
//  This class is the parser to recognize the SAN textual file, to store all    //
// datas and to generate the DES files set (des, dic, fct, tft, and res).       //
//  This class is composed by the files:                                        //
//   - grammar.C and .h    - main files                                         //
//   - grammar_lex.C       - all lexical functions                              //
//   - grammar_syn_ev.C    - syntactical functions to event block               //
//   - grammar_syn_id.C    - syntactical functions to identifiers block         //
//   - grammar_syn_net.C   - syntactical functions to network block             //
//   - grammar_syn_reach.C - syntactical functions to reachability block        //
//   - grammar_syn_res.C   - syntactical functions to results block             //
//   - error_msgsC and .h  - error messages                                     //
//==============================================================================// 
#ifndef _GRAMMAR__H_
#define _GRAMMAR__H_

#include "error_msgs_.h"
#include "../lb/error_msgs.h"
#include "../lb/yacc.H"
#include "peps.h"
#include "dom_.h"
#include "edic_.h"
#include "func_.h"
#include "ndic_.h"

class GRAMMAR_ {
  public:
    GRAMMAR_();
    ~GRAMMAR_();
    void reset();

    void open_file(const peps_name file, ifstream & f_tmp);
    void open_file(const peps_name file, ofstream & f_tmp);
    void close_file(const peps_name file, ifstream & f_tmp);
    void close_file(const peps_name file, ofstream & f_tmp);

    void compile(const peps_name file);

    void put2file_des(const peps_name file);       // write pos and neg matrices
    void put2file_dic(const peps_name file);        // write the data dictionary
    void put2file_fct(const peps_name file);      // write reachability function
    void put2file_tft(const peps_name file);                  // write functions
    void put2file_res(const peps_name file);           // write result functions

    void print();
    
  private:
    // lexical functions
    int get_ln();
    int classify_id();
    int classify_number();
    int get_tk();
    int put_tk();
    
    // syntatical functions
    // identifiers
    void id_blk();
    void dom_blk(char *_id);
    void dom_blk2(char *_id);
    void list_dom(char *_id);
    void dom_int(char *_id);
    void exp_blk(char *id_func);
    void exp_blk2(char *id_func);
    void list_blk(char *id_func);
    void exp(char *id_func);
    void elem(char *id_func);
    int  bin_op();
    int  id_aut(char *_id, char *id_func);
    int  id_int_aut(char *_id, char *id_func);    
    void id_stt(char *_id, char *id_func);
    int  replic(char *_rep, char *id_func);
    int  interval(char *_first, char *_last, char *id_func);
    int  interval1(char *_first, char *_last, char *id_func);
    int  id_stat(char *_id, char *id_func);
    
    // events
    int ev_blk();
    int ev_rep_blk(char *_ev, char *_repM, char *_repN, char *_repO);
    
    // reachability
    void reach_blk();
    
    // network
    void net_blk();
    void aut_blk();
    int  aut_rep_blk(char *_repA);
    void stt_blk(int _faut, int _laut);
    int  stt_rep_blk(char *_repS);
    int  from_rep_blk(char *_repS);
    int  stt_reward(char *_rw);
    void to_blk(int _faut, int _laut, int _fstt, int _lstt);  
    int  to_rep_blk(char *_repT);
    void ev_lst(int _faut, int _laut, int _fstt, int _lstt, char *_idT, 
                char *_repT, char *_restrT, int inc_dec);
    int  ev_rep_lst(char *_repEM, char *_repEN, char *_repEO);
    int  restr_blk(char *_restr);
       
    // results
    void res_blk();
    
    // data create procedures
    void create_aut_stt();
    void put_model_name(char *_id);
    void put_model_type(int _type);
     
    // auxiliar procedures
    void cut_dom_id(char *_id, char *_dom);    
    
    // internal write procedures
    void write_local_events(ofstream &fout, int _aut);
    void write_synch_event(ofstream &fout, int synch_ev);
    
    // error procedures
    void Lexical_error();
    void Syntactic_error();
    void Syntactic_error(char *s);
    void Dom_semantic_error(char *s_error, char *_id, int code_error);
    void Dom_semantic_error(char *s_error, char *_id, char *_tk, int code_error);
    void Function_semantic_error(char *s_error, char *_id, int code_error);
    void Function_semantic_error(char *s_error, char *_id,  char *_tk, int code_error);
    void Function_semantic_warning(char *s_error, char *_id,  char *_tk, int code_error);
    void Event_semantic_error(char *s_error, char *_id, int code_error);
    void Event_semantic_error(char *s_error, char *_id,  char *_tk, int code_error);
    void Network_semantic_error(char *s_error, char *where, char *_id, char *_tk, int code_error);
    void Network_semantic_error(char *s_error, char *_id,  int code_error);
    void Result_semantic_error(char *s_error, char *_id, int code_error);

    // variables to lexical and syntatic procedures
    ifstream  fin;
    int       pos;
    int       in_mem;
    int       new_line;
    int       tour;
    int       TK;
    char      tk[MAX_ID_NAME];
    char     *cline;

    // dictionaries 
    DOM_       dom;                                         // domains dictionary
    NDIC_      ndic;                                       // network  dictionary
    EDIC_      edic;                                         // events dictionary
    FUNC_      ident;                                     // functions dictionary
    FUNC_      reach;
    FUNC_      result;
    FUNC_      func;                              // auxiliar function dictionary
    
    // network
    char       name[MAX_ID_NAME];
    int        type;
    AUTOMATON_ *aut;
    int        n_aut;
    
    // error control
    int       lineno;
    int       errorno;
};

#endif
