%{
//====================================================================================//
//                                                                                    //
//                           Parser and Semantic Analysis                             //
//                                                                                    //
//====================================================================================//
//  This File:   glc.y                       Tool: YACC                               //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    01/Mar/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 07/Nov/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  YACC is a general-purpose parser generator that converts a context-free grammar   //
// a C program.In this module are make the actions to generate the internal SAN's     //
// tables and the others functionalities of this textual interface for PEPS.          //
//====================================================================================//
#include "sanfile.h"

//====================================================================================//
//                                   Global variables                                 //
//====================================================================================//

NET *net; // Network structure
DOM *dom; // Dominion structure
EDIC *edic; // Events dictionary

inf2pos itop;  // Used to convert of infix operator expression form
               //   to postfix operator expression form 

FUNC *t_ident; // Auxiliar table to identifiers

aut_st    replic_map;      // replication map to perform lump

int not_error = 1;

int tour; // Set the times that the parser was called, the first to count the number
          // of automatas, the number of states, the number of transitions and others
int n_aut; // Number of automatons in network, this counter is increased in the first 
           // pass to alloc the automaton structure in second pass
int n_state; // Number of states in each automaton
int n_trans; // Number of transitions in each state
int n_mst; // Number of master events
int n_sync; // Number of synchronizing events
int n_local; // Number of local events
int n_events; // Number of events (local and synchronizing)
int aut_group; // Automaton group number 
//====================================================================================//
//                                      Variables                                     //
//====================================================================================//

char temp[MAX_AUTOMATON_NAME];
char tmp[FUNCTION], tmp1[FUNCTION];  // Variables used to grouping characters
char fnc[FUNCTION]; // Variable to store the functions
char fname[MAX_FUNC_NAME];
char event_name[MAX_EVENT_NAME];
char i1n[MAX_FUNC_NAME];
char i2n[MAX_FUNC_NAME];

int with_id, sum_or_prod;
int r_t;
double v_tmp;
int i_tmp;
int i1, i2, iv, ia, di, dl;
int from_dst, f_t;
int event_type;
int event_rate;
int event_mst;
int queue_p, queue_m;

char aut_replic[MAX_AUTOMATON_NAME];
char state_queue[MAX_STATE_NAME];
char aux_dst[MAX_STATE_NAME];
int aut; // current automaton index
int replic; // number of replics
int state; // current state index
int queue; // queue
int trans; // transition
int mst; // master event
int synch; // synchronizing event
int local; // local event
int **mat_trans; // This matrix store the number of transitions of each automaton-state


//====================================================================================//
//                                   Parser variables                                 //
//====================================================================================//

extern int yynerrs;  // Variable of the YACC generator to count the number of errors.  
int yylineno;        // Variable of the YACC generator to count the current line

extern int yylex(void); // Get the next token
extern void yyrestart(FILE *input_file);
extern char *yytext; 
extern FILE *yyin; // Input file


//====================================================================================//
//                                   Function errors                                  //
//====================================================================================//
//  These functions analysis which error occurred and put the error into a message    //
// more specific.                                                                     //
//====================================================================================//


//====================================================================================
// Function that YACC use to put sintatic message errors                      
//====================================================================================
extern void yyerror(char *s)
{
  yynerrs++; 
  not_error = 0;
  Sintatic_Error(yynerrs, yylineno, s, yytext);
}

//====================================================================================
// Function to put semantic error messages 
//====================================================================================
void type_error(const char* error_number)
{
  yynerrs++;
  not_error = 0;
  Semantic_Error_YACC(yynerrs, yylineno, yytext, error_number);
}


void alloc_mat_aut(int aaut)
{
  mat_trans = new int*[aaut];  
}

void alloc_mat_state(int aaut, int sstate)
{
  mat_trans[aaut] = new int[sstate];  
  for(int i=0; i<sstate; i++)
    mat_trans[aaut][i] = 0;
}

%}

             //-----------------------------------------------------------//
             //  Definition of tokens (terminals) of the grammar.         //
             // YACC used these definitions to generate the file          //
             // y.tab.h that is included in a LEX file (tokens.l)         // 
             //-----------------------------------------------------------//

%token TK_IDENTIFIERS TK_EVENTS TK_NETWORK TK_AUTOMATON TK_STATE TK_TRANS TK_FROM
%token TK_SYN TK_LOC TK_RESULTS TK_CONTINUOUS TK_DISCRETE
%token TK_REACH TK_PARTIAL TK_MIN TK_MAX TK_STATE_NUM TK_NBR TK_REWARD
%token TK_SUM_RW TK_PROD_RW TK_ID TK_INTEGER TK_REAL TK_EXPONENT  
%token TK_LEFT_BRACKET TK_RIGHT_BRACKET 
%token TK_LEFT_PARENTHESIS TK_RIGHT_PARENTHESIS 
%token TK_LEFT_SQUARE_BRACKET TK_RIGHT_SQUARE_BRACKET 
%token TK_SEMICOLON TK_COMMA TK_ATTRIB TK_COLON TK_POINT TK_2DOTS TK_DOTS
%token TK_SIMPLE TK_DOUBLE
%left TK_PLUS TK_PLUS_PLUS TK_MINUS TK_MINUS_MINUS 
%left TK_MULT TK_DIV TK_AND TK_OR TK_XOR TK_NOT
%left TK_EQUAL TK_UNEQUAL TK_MAJOR TK_MAJOR_EQUAL TK_MINOR TK_MINOR_EQUAL

%% 

             //-----------------------------------------------------------//
             //                      Begin of grammar                     //
             //-----------------------------------------------------------//

start:blk_id blk_events reach blk_net blk_res
    ;

             //-----------------------------------------------------------//
             //              Block to evaluation of identifiers           //
             //-----------------------------------------------------------//
    
blk_id:
    TK_IDENTIFIERS blk_int_id 
    |
    ;

blk_int_id:
	TK_ID
    { strcpy(fname,(char *) yytext);
      if(tour==1){
	    itop.reset(); 
	    ia = -1; 	
	  }
	  if(tour==4){
        itop.reset();  	
	    ia = -1; 	
      }
	}
    TK_ATTRIB {itop.reset();} blk_exp_dom TK_SEMICOLON blk_int_id2
	;
	
blk_int_id2:
	blk_int_id
	|
	;

             //-----------------------------------------------------------//
             //                 Block to definition of events             //
             //-----------------------------------------------------------//
    
blk_events:
    TK_EVENTS blk_int_ev 
    ;

blk_int_ev:
	TK_LOC 
	{ if(tour==1)
	    n_events++;
	  event_type = 0;
	} 
	TK_ID 
	{ strcpy(fname,(char*)yytext);
	  di = dl = 0;
	} event_replic rate aut_list semi_colon blk_int_ev2
	|TK_SYN 
	{ if(tour==1)
	    n_events++;
	  event_type = 1;
	} 
  TK_ID 
  { strcpy(fname,(char*)yytext);
    di = dl = 0;
  } event_replic rate aut_list semi_colon blk_int_ev2
	;
	
blk_int_ev2:
	blk_int_ev
	|
	;

event_replic:
    TK_LEFT_SQUARE_BRACKET int_event_replic TK_RIGHT_SQUARE_BRACKET
    |
    ;

int_event_replic:
    TK_INTEGER {di = atol((char*) yytext); } dots TK_INTEGER
    { dl = atol((char*) yytext); 
      n_events+= dl-di;
      if((dl-di)<0){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
    }
    | TK_ID 
    { if(dom->get_name((char *) yytext)>=0){
        di = dom->get_first((char *) yytext);
        dl = dom->get_last((char *) yytext);
        n_events+= dl-di;        
      }
      else {
        type_error(Parser_01);
      }
    }
    ;

        
rate: TK_LEFT_PARENTHESIS int_rate TK_RIGHT_PARENTHESIS 
    | int_rate 
    ;

int_rate:
    TK_ID
    { if(tour==4){
        if(net->get_ident_name((char *) yytext)<0){
          type_error(Parser_01);
        }
        else{
          if(dl-di<=0){
            edic->put_event(fname,event_type, "0", net->get_ident_name((char*)yytext));
          }
          else{
            for(int i=di; i<=dl; i++){
              sprintf(event_name, "%s[%d]", fname,i);
              edic->put_event(event_name,event_type, "0", net->get_ident_name((char*)yytext));
            }
          }
        }
      }
    }
    |number
    { if(tour==4){
        if(dl-di<=0){
          edic->put_event(fname,event_type, (char*)yytext, -1);
        }
        else{
          for(int i=di; i<=dl; i++){
            sprintf(event_name, "%s[%d]", fname,i);
            edic->put_event(event_name,event_type, (char*)yytext, -1);
          }
        }
      }
    }
    ;

semi_colon:
  TK_SEMICOLON
  |
  ;
             //-----------------------------------------------------------//
             //              Block to evaluation of expressions           //
             //-----------------------------------------------------------//

blk_exp_dom:
    blk_def_dom
    |blk_exp 
    { if(tour==1){
    	itop.get_func(fnc);
        itop.reset();
	    t_ident->put_name(fname);
        t_ident->put_exp(fnc);
      }
      if(tour==4){
		itop.get_func(fnc);
        itop.reset();
	    net->put_ident_name(fname);
        net->put_ident_exp(fnc);
      }
    }
    ;

blk_def_dom:
    TK_LEFT_SQUARE_BRACKET TK_INTEGER {di = atol((char*)yytext);} dots TK_INTEGER
    { dl = atol((char*)yytext);
      dom->put_name(fname);
      if(dom->put_dom(di, dl)<0){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
    }
    TK_RIGHT_SQUARE_BRACKET
    ;
        
blk_exp:
    elem exp
    ;

blk_exp2:
    blk_exp list_exp
    ;

list_exp:
    TK_COMMA blk_exp2
    |
    ;

exp:op_bin
    { itop.put_op((char*) yytext);}
    elem
    { itop.put_elem();} 
    exp
    |
    ;

elem:
    TK_LEFT_PARENTHESIS blk_exp TK_RIGHT_PARENTHESIS 
    |op_unary elem { itop.put_elem(); }
    |TK_MIN TK_LEFT_PARENTHESIS blk_exp2 TK_RIGHT_PARENTHESIS { itop.put_elem("min");}
    |TK_MAX TK_LEFT_PARENTHESIS blk_exp2 TK_RIGHT_PARENTHESIS { itop.put_elem("max");}
    |TK_REWARD id_replic 
    { strcpy(tmp,"% "); 
      sprintf(tmp,"%s%s ",tmp,tmp1);
      itop.put_elem(tmp);
    }  
    |TK_STATE_NUM id_replic 
    { strcpy(tmp, "@ ");
      sprintf(tmp,"%s%s ",tmp,tmp1);
      itop.put_elem(tmp);
    } 
    |TK_NBR
    { strcpy(tmp, "@$ ");
      i1 = i2 = 0;
    }  
    interval 
    { if(i1>i2 && tour==4){
        Semantic_Error(fname, Parser_105);
        not_error = 0;
      }
    }
    id_replic
    { strcpy(temp,tmp1); 
      if(tour==1){
        sprintf(tmp,"%s%s %s %s ", tmp, temp, i1n, i2n);
        itop.put_elem(tmp);
      }
      if(tour==4){
        sprintf(tmp,"%s%s %d %d ", tmp, temp, i1, i2);
        itop.put_elem(tmp);
        if(net->get_name(temp, i1, i2)==-1){
          Semantic_Warning(fname, temp, Parser_103);
        }
        if(net->get_name(temp, i1, i2)==0){
          Semantic_Warning(fname, temp, Parser_104);
        }
      } 

    }
    |{ tmp[0]='\0';
       i1 = i2 = 0;
    }
    op_rw dominion 
    { if(i1>i2 && tour==4){
        Semantic_Error(fname, Parser_105);
        not_error = 0;
      } 
      if(with_id){
        if(sum_or_prod)
    	  strcpy(tmp1,"%S ");
        else strcpy(tmp1,"%P ");
      }
      else{
        if(sum_or_prod)
          strcpy(tmp1,"%$ ");
        else
          strcpy(tmp1,"%# ");
      }
      if(tour==1){
        sprintf(tmp1,"%s%s %s %s ",tmp1, temp, i1n, i2n);
        itop.put_elem(tmp1);
      }
      if(tour==4){
        sprintf(tmp1,"%s%s %d %d ",tmp1, temp, i1, i2);
        itop.put_elem(tmp1);
      }
      
    } 
    |TK_ID {strcpy(tmp1, (char*) yytext);}
    replic
    { if(tour==4 || tour==1){
        if(net->get_name(tmp1, ia)>=0){
          sprintf(tmp1,"%d ", net->get_name(tmp1, ia)); 
        }
        else{
          if(net->get_name(tmp1)>=0){
            sprintf(tmp1,"%d ", net->get_name(tmp1));
          }
          else{
            if(t_ident->get_name(tmp1)<0){
              strcat(tmp1," ");          
            }
            else{
              t_ident->get_exp(tmp1,tmp1); 
            }
          }
        }
        itop.put_elem(tmp1); 
      }
    }
    |number { itop.put_elem((char*) yytext);}
    ;

op_rw:
    TK_SUM_RW {sum_or_prod = 1; }
    |TK_PROD_RW {sum_or_prod = 0;}
    ;

dominion:
    interval_1 id_stat 
    { strcpy(temp,tmp1); 
      if(tour==4 && with_id==1){
        if(net->get_name(temp, i1, i2)==-1){
          Semantic_Warning(fname, temp, Parser_103);
        }
        if(net->get_name(temp, i1, i2)==0){
          Semantic_Warning(fname, temp, Parser_104);
        }
      } 
    }
    ;


interval:
    interval_1
    |
    { if(tour==4){
        i1 = 0;
        i2 = net->get_net_n_aut()-1;
      }
    }
    ;

interval_1:
    TK_LEFT_SQUARE_BRACKET 
    id_replic { i1 = net->get_name(tmp1); strcpy(i1n,tmp1); tmp1[0] = '\0'; }
    dots 
    id_replic { i2 = net->get_name(tmp1); strcpy(i2n,tmp1); tmp1[0] = '\0'; }
    TK_RIGHT_SQUARE_BRACKET
    ;

id_stat:
    id_replic {with_id = 1;}
    | {with_id = 0;}
    ;

id_replic:
    TK_ID {strcpy(tmp1,(char*) yytext);}
    replic 
    { if(tour==4){
    	if(net->get_name(tmp1)>=0){
          ia = net->get_name(tmp1);
        }
      }
    }
    ;

replic:
    TK_LEFT_SQUARE_BRACKET {strcat(tmp1,(char*) yytext);}
    TK_INTEGER {strcat(tmp1,(char*) yytext);}
    TK_RIGHT_SQUARE_BRACKET {strcat(tmp1,(char*) yytext);}
    |
    ;

number: 
    TK_REAL
    |TK_INTEGER
    |TK_EXPONENT
    ;

op_unary:
    TK_PLUS 
    { itop.put_elem("1");
      itop.put_op("*");
    }
    |TK_MINUS 
    { itop.put_elem("-1");
      itop.put_op("*");
    }
    |TK_NOT { itop.put_op((char*) yytext);}
    ;

op_bin:
    TK_MULT 
    |TK_DIV 
    |TK_PLUS 
    |TK_MINUS 
    |TK_EQUAL 
    |TK_UNEQUAL 
    |TK_MAJOR 
    |TK_MAJOR_EQUAL 
    |TK_MINOR 
    |TK_MINOR_EQUAL 
    |TK_AND 
    |TK_OR
    |TK_XOR  
    |TK_SIMPLE
    |TK_DOUBLE
    ;


             //-----------------------------------------------------------//
             //             Block to evaluation of reachability           //
             //-----------------------------------------------------------//
	
reach: t_reach TK_REACH {itop.reset(); ia = -1; strcpy(fname,(char *) yytext);}
    TK_ATTRIB blk_exp
    { if(tour==4){
    	itop.get_func(fnc);
        itop.reset();
        net->put_reach_exp(fnc);
      }
    }
    TK_SEMICOLON
    ;

t_reach:
    TK_PARTIAL {r_t = 1; PRF::prf.partial_rss();}
    |{r_t = 0; PRF::prf.full_rss();}
    ;
    
             //-----------------------------------------------------------//
             //                 Block to network description              //
             //-----------------------------------------------------------//

blk_net:
    TK_NETWORK TK_ID { net->put_net_name((char*) yytext);} 
    TK_LEFT_PARENTHESIS model_type TK_RIGHT_PARENTHESIS { aut = 0; }
    blk_autom 
    ;

model_type:
	TK_DISCRETE{ net->put_net_model(DISCRETE);}
	|TK_CONTINUOUS {net->put_net_model(CONTINUOUS);}
	;


             //-----------------------------------------------------------//
             //               Block to evaluation of automata             //
             //-----------------------------------------------------------//

blk_autom:
    TK_AUTOMATON
    { if(tour==1)
        n_aut++;
    }
    TK_ID
    { replic = 1;
      strcpy(fname,(char*)yytext);
      if(tour==2){
        n_state = 0;
        if(!net->put_aut_name(aut, (char*) yytext)){
          type_error(Parser_03);
        }
        replic_map.put(aut, aut_group);
        aut++;
      }
      if(tour>=3){
        state = 0;
      }
    }
    blk_replic blk_state 
    { if(tour==2){
        for(int i=1;i<=replic;i++){
          if(!net->put_aut_n_state(aut-i, n_state)){
            not_error = 0;
          }
          alloc_mat_state(aut-i, n_state);
        }
        aut_group++;
      }
    }
    blk_from
    { if(tour>=3){
        aut+= replic;
      }
    } 
    blk_autom2
    ;
    
blk_autom2:
	blk_autom
	|
	;

             //-----------------------------------------------------------//
             //               Block to evaluation of states               //
             //-----------------------------------------------------------//

blk_state:
    TK_STATE 
    { if(tour==2)
        n_state++;
      f_t = 0;
    }
    TK_ID
    { strcpy(fname,(char*)yytext);
      if(tour==3){
        n_trans = 0;
        if(t_ident->get_name((char*) yytext)>0){
          type_error(Parser_06);
        }
        for(int i=0; i<replic; i++){
          if(!net->put_state_name(aut+i, state, (char*) yytext)){
            type_error(Parser_05);
          }
          net->put_state_reward(aut+i, state, state);
        }
        state++;
        queue = 1;
      }	
      if(tour>=4){
        state++;
        queue = 1;	
        trans = 0;
      }
      queue_p = 0;
      queue_m = 0;
    }
    blk_queue blk_reward blk_trans
    { if(tour==3){
        for(int i=0;i<replic;i++){    
          for(int j=1;j<=queue;j++){
            mat_trans[aut+i][state-j] = n_trans;
            if(queue_p>0 && queue_p>=j && queue>1)
              mat_trans[aut+i][state-j]--;
            if(queue_m<0 && ((queue-j)<abs(queue_m))&& queue>1){
              mat_trans[aut+i][state-j]--;
            }
          }
        }
        
      }
      if(tour>=4){
        state+=queue-1;
      }
    }
    blk_state2
    ;
    
blk_state2:
    blk_state
    |
    ;
    

blk_from:
    TK_FROM TK_ID 
    { strcpy(temp,(char*)yytext);
      f_t = 1;
    }
    blk_from_replic 
    { if(tour>=3){
        from_dst = net->get_name(temp,aut);
        if(from_dst<0){
          yynerrs++;
          not_error = 0;
          Semantic_Error_YACC(yynerrs, yylineno, temp, Parser_100);
          from_dst = 0;
        }
      }
    }
    blk_trans blk_from
    |
    ;
    
blk_from_replic:
    TK_LEFT_SQUARE_BRACKET TK_INTEGER
    { sprintf(temp,"%s[%s]",temp,(char*)yytext);
     }
    TK_RIGHT_SQUARE_BRACKET
    |
    ;
        
             //-----------------------------------------------------------//
             //            Block to evaluation of transictions            //
             //-----------------------------------------------------------//
            
blk_trans:
    TK_TRANS 
    { if (tour==3){
        n_trans++;
        if(f_t)
          for(int i=0;i<replic;i++){    
            mat_trans[aut+i][from_dst]++;
          }
      }
      n_mst = 0;
      n_sync = 0;
      n_local = 0;  
    }
    TK_LEFT_PARENTHESIS dst_state TK_RIGHT_PARENTHESIS blk_type_trans 
    { if(tour==4){
        if(f_t==0){
     	for(int i=0; i<replic; i++){
      	  for(int j=abs(queue_m); j<(queue-queue_p); j++){	
              if(!net->put_event_n_mst(aut+i, state+j-1, n_mst))
                not_error = 0;
              if(!net->put_event_n_sync(aut+i, state+j-1, n_sync))
                not_error = 0;
              if(!net->put_event_n_local(aut+i, state+j-1, n_local))
                not_error = 0;
//              cout << "aloc(" << aut+i << "," << state+j-1 << "): m(" << n_mst << ") - s(" << n_sync <<  ") - l(" << n_local << ")\n";
            }
          }
        }
        else{
      	for(int i=0; i<replic; i++){
            if(!net->put_event_n_mst(aut+i, from_dst, n_mst))
              not_error = 0;
            if(!net->put_event_n_sync(aut+i, from_dst, n_sync))
              not_error = 0;
            if(!net->put_event_n_local(aut+i, from_dst, n_local))
              not_error = 0;
//              cout << "+aloc(" << aut+i << "," << from_dst << "): m(" << n_mst << ") - s(" << n_sync <<  ") - l(" << n_local << ")\n";

          }
        }
      }
    }
    blk_trans2
    ;
 
blk_trans2:
    blk_trans
    |
    ;
    
dst_state:
    TK_ID 
    {if(tour>=4)
  	 sprintf(aux_dst,"%s",(char*) yytext);
    }
    dst_replic
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue; j++){	
              from_dst = net->put_trans_dst(aut+i,state+j-1,1,aux_dst);
              if(from_dst<0){
                type_error(Parser_100); 
              }  
            }
          }
        }
        else{
          for(int i=0; i<replic; i++){
            if(net->put_from_dst(aut+i,from_dst,aux_dst)<0){
              type_error(Parser_100); 
            }  
          }
        }
        queue_p = 0;
        queue_m = 0;
      }
    }
    |TK_EQUAL
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue; j++){	
        	  net->put_trans_dst(aut+i,state+j-1,4,(char*) yytext);
        	  from_dst = state-1;
            }
          }
        }
        else{
          type_error(Parser_115); 
        }
        queue_p = 0;
        queue_m = 0;
      }
    }
    |TK_PLUS_PLUS
    { if(tour>=4){
        if(f_t==0){   
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue-1; j++){	
              from_dst = net->put_trans_dst(aut+i,state+j-1,2,(char*) yytext);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }
        }
        else{
          type_error(Parser_115); 
        }
        queue_m = 0;        
      }
      queue_p = 1;
    }
    |TK_MINUS_MINUS
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=1; j<queue; j++){	
              from_dst = net->put_trans_dst(aut+i,state+j-1,3,(char*) yytext);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }
        }
        else{
          type_error(Parser_115); 
        }
        queue_p = 0;                
      }
      queue_m = -1;
    }
    |TK_MINUS TK_INTEGER
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=atol((char*)yytext); j<queue; j++){	
          	sprintf(aux_dst,"-%s",(char*) yytext);
              from_dst = net->put_trans_dst(aut+i,state+j-1,5,aux_dst);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }          
        }
        else{
          type_error(Parser_115); 
        }
        queue_p = 0;
      }
      queue_m = - atol((char*) yytext);
    }
    |TK_PLUS TK_INTEGER
    { if(tour>=4){
        if(f_t==0){
          for(int i=0; i<replic; i++){
            for(int j=0; j<queue-atol((char*)yytext); j++){	
          	sprintf(aux_dst,"-%s",(char*) yytext);
              from_dst = net->put_trans_dst(aut+i,state+j-1,5,aux_dst);
              if(from_dst<0){
                type_error(Parser_101); 
              }  
            }
          }
          if(queue==1){
            type_error(Parser_101); 
          }          
        }
        else{
          type_error(Parser_115); 
        }
        queue_m = 0;
      }
      queue_p = atol((char*) yytext);
    }
    ;

dst_replic:
    TK_LEFT_SQUARE_BRACKET TK_INTEGER
    { if(tour>=4)
        sprintf(aux_dst,"%s[%s]",aux_dst,(char*) yytext);
    }
    TK_RIGHT_SQUARE_BRACKET
    |
    ;

blk_type_trans:
    TK_ID {strcpy(event_name, (char*)yytext);} e_replic
    { if(tour>=4){
        if(edic->get_type(event_name)<0){
          type_error(Parser_04);
      }
      else{
        event_type = edic->get_type(event_name);
        event_rate = edic->get_f_rate(event_name);
        if(event_rate<0){
          edic->get_rate(event_name, fname);
        }
        switch(event_type){
          case 0: if(tour==4) // local event
                    n_local++;
                  if(tour==5){
                    if(f_t==0){
                      for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                          if(net->put_local_name(aut+i, state+j-1, event_name)==0){
                            type_error(Parser_112);
                          }
                  	    if((from_dst+j)==(state+j-1)){
                            net->put_local_rate(aut+i, state+j-1,  0, "0", -1);
                            cout << "Semantic Warning - " << event_name << " - (** " \
                                 << Parser_111 <<  " **)." \
                                 << endl;
                          }
                          else{
                            if(event_rate>=0){
                              net->put_local_rate(aut+i, state+j-1, 1, "0", event_rate);
                            }
                            else{
                              net->put_local_rate(aut+i, state+j-1, 0, fname, -1);
                            }
                          }
                        }
                      }
                    } 
                    else{
                      for(int i=0; i<replic; i++){
                        if(net->put_local_name(aut+i, from_dst, event_name)==0){
                          type_error(Parser_112);
                        }
                	    if(from_dst==net->get_name(aux_dst,aut+i)){
                          net->put_local_rate(aut+i, from_dst, 0, "0", -1);
                          cout << "Semantic Warning - " << event_name << " - (** " \
                               << Parser_111 <<  " **)." \
                               << endl;
                        }
                        else{
                          if(event_rate>=0){
                            net->put_local_rate(aut+i, from_dst, 1, "0", event_rate);
                          }
                          else{
                            net->put_local_rate(aut+i, from_dst, 0, fname, -1);
                          }
                        }
                      }
                    }
                  }
            break; 
          case 1: if(edic->get_mst(event_name)==aut || edic->get_mst(event_name)<0){ // synchronized event
                    if(tour==4)
                      n_mst++;
                      edic->put_mst(event_name, aut);                      
                    if(tour==5){
                      if(f_t==0){
                      	for(int i=0; i<replic; i++){
          	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                            if(net->put_mst_name(aut+i, state+j-1, event_name)==0){
                              type_error(Parser_112);
                            }
                            if(event_rate>=0){
                              net->put_mst_rate(aut+i, state+j-1, 1, "-1", event_rate);
                            }
                            else{
                              net->put_mst_rate(aut+i, state+j-1, 0, fname, -1);                          
                            }
                            event_mst = 1;
                          }
                        }
                      }
                      else{
                    	  for(int i=0; i<replic; i++){
                          if(net->put_mst_name(aut+i, from_dst, event_name)==0){
                            type_error(Parser_112);
                          }
                          if(event_rate>=0){
                            net->put_mst_rate(aut+i, from_dst, 1, "-1",event_rate);
                          }
                          else{
                            net->put_mst_rate(aut+i, from_dst, 0, fname, -1);                          
                          }
                          event_mst = 1;                    	  
                        }
                      }
                    }  
                  }
                  else{
                    if(tour==4)
                      n_sync++;
                    if(tour==5){
                      if(f_t==0){
                    	for(int i=0; i<replic; i++){ 
        	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                            if(net->put_sync_name(aut+i, state+j-1, event_name)==0){
                              type_error(Parser_112);
                            }            	
                            event_mst = 0;
                          }
                        }
                      }  
                      else{
                      	for(int i=0; i<replic; i++){ 
                          if(net->put_sync_name(aut+i, from_dst, event_name)==0){
                            type_error(Parser_112);
                          }            	
                          event_mst = 0;
                        }
                      }
                    }                  
                  }
            break;
          }
        }
      }
    }
    blk_prob blk_type_trans2
    ;
    
blk_type_trans2:
    blk_type_trans
    |
    ;

e_replic:
    TK_LEFT_SQUARE_BRACKET TK_INTEGER
    { if(tour>=4)
        sprintf(event_name,"%s[%s]",event_name,(char*) yytext);
    }
    TK_RIGHT_SQUARE_BRACKET
    |
    ;

blk_prob:
    TK_LEFT_PARENTHESIS prob TK_RIGHT_PARENTHESIS
    |
    ;

prob:
    number
    { switch(event_type){
        case 0: if(tour==5){
              	if(event_rate>=0){
              	  net->get_ident_exp(event_rate, fname);
              	}
              	sprintf(tmp1,"%s %s * ", yytext, fname);
              	if(net->get_ident_exp(tmp1)<0){
              	  net->put_ident_name("new_rate");
              	  net->put_ident_exp(tmp1);
              	}
              	if(f_t==0){
                	for(int i=0; i<replic; i++){
    	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                	    if((from_dst+j)==(state+j-1))
                          net->put_local_rate(aut+i, state+j-1, 0, "0", -1);
                        else
                          net->put_local_rate(aut+i, state+j-1, 1, (char *)yytext, net->get_ident_exp(tmp1));
                      }
                    }
                  }
                  else{
                    for(int i=0; i<replic; i++){
               	   if(from_dst==net->get_name(aux_dst,aut+i)){
                        net->put_local_rate(aut+i, from_dst, 0, "0", -1);
                      }
                      else{
                        net->put_local_rate(aut+i, from_dst, 1, (char *)yytext, net->get_ident_exp(tmp1));
                      }
                    }
                  }
                }
          break; 
        case 1: if(tour==5){
        	      if(edic->get_mst(event_name)==aut || edic->get_mst(event_name)<0){
                	if(event_rate>=0){
                	  net->get_ident_exp(event_rate, fname);                  
                	}
                	sprintf(tmp1,"%s %s *", yytext, fname);
                	if(net->get_ident_exp(tmp1)<0){
                	  net->put_ident_name("new_rate");
                	  net->put_ident_exp(tmp1);
                	}
                	if(f_t==0){
                  	for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                  	    net->put_mst_rate(aut+i, state+j-1, 1, (char*) yytext, net->get_ident_exp(tmp1));
                        }
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                	      net->put_mst_rate(aut+i, from_dst, 1, (char*) yytext, net->get_ident_exp(tmp1));
                      }
                    }
                  }
                  else{
                    if(f_t==0){
                  	for(int i=0; i<replic; i++){
    	                for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                          net->put_sync_prob(aut+i, state+j-1, 0, (char*) yytext, -1);
                        }
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                        net->put_sync_prob(aut+i, from_dst, 0, (char*) yytext, -1);
                      }
                    }
                  }
                }
          break;
      }
    }
    |TK_ID        
    { switch(event_type){
        case 0: if(tour==5){
          	    if(net->get_ident_name((char *) yytext)<0){
          	      type_error(Parser_01);
          	    }
          	    else{
                	if(event_rate>=0){
              	    net->get_ident_exp(event_rate, fname);
              	  }
                	net->get_ident_exp((char*) yytext, tmp1);
              	  sprintf(tmp1,"%s %s * ", tmp1, fname);
                	if(net->get_ident_exp(tmp1)<0){
                	  net->put_ident_name("new_rate");
                	  net->put_ident_exp(tmp1);
              	  }
                    if(f_t==0){
                  	for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                  	    if((from_dst+j)==(state+j-1))
                            net->put_local_rate(aut+i, state+j-1, 0, "0", -1);
                          else
                            net->put_local_rate(aut+i, state+j-1, 1, "0", net->get_ident_exp(tmp1));
                        }
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                 	   if(from_dst==net->get_name(aux_dst,aut+i)){
                          net->put_local_rate(aut+i, from_dst, 0, "0", -1);
                        }
                        else{
                          net->put_local_rate(aut+i, from_dst, 1, "0", net->get_ident_exp(tmp1));
                        }
                      }
                    }
                  }
                }
          break; 
        case 1: if(tour==5){
                  if(edic->get_mst(event_name)==aut || edic->get_mst(event_name)<0){
          	      if(net->get_ident_name((char *) yytext)<0){
            	      type_error(Parser_01);
                    }
            	    else{
                  	if(event_rate>=0){
                	    net->get_ident_exp(event_rate, fname);
                	  }
                	  net->get_ident_exp((char*) yytext, tmp1);
                	  sprintf(tmp1,"%s %s *", tmp1, fname);
                  	if(net->get_ident_exp(tmp1)<0){
                  	  net->put_ident_name("new_rate");
                  	  net->put_ident_exp(tmp1);
                	  }
                	  if(f_t==0){
                    	for(int i=0; i<replic; i++){
      	                for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                    	    net->put_mst_rate(aut+i, state+j-1, 1, (char*) yytext, net->get_ident_exp(tmp1));
                          }
                        }
                      }
                      else{
                    	for(int i=0; i<replic; i++){
                  	    net->put_mst_rate(aut+i, from_dst, 1, (char*) yytext, net->get_ident_exp(tmp1));
                        }
                      }
                    }
                  }
                  else{
                    if(f_t==0){
                  	for(int i=0; i<replic; i++){
      	              for(int j=abs(queue_m); j<(queue-queue_p); j++){	
                  	    if(net->get_ident_name((char *) yytext)<0){
    	                    type_error(Parser_01);
                  	    }
                  	    else{
                            net->put_sync_prob(aut+i, state+j-1,  1, (char *) yytext, net->get_ident_name((char *) yytext));
                          }
                        } 
                      }
                    }
                    else{
                  	for(int i=0; i<replic; i++){
                	    if(net->get_ident_name((char *) yytext)<0){
  	                    type_error(Parser_01);
                	    }
                	    else{
                          net->put_sync_prob(aut+i, from_dst, 1, (char *) yytext, net->get_ident_name((char *) yytext));
                        }
                      }
                    } 
                  }
                }
          break;
      }
    }
    ;

             //-----------------------------------------------------------//
             //         Block to evaluation of replics and queue          //
             //-----------------------------------------------------------//

blk_replic:
    TK_LEFT_SQUARE_BRACKET replic_number TK_RIGHT_SQUARE_BRACKET
    |
    ;

replic_number:
    TK_INTEGER {di = atol((char*) yytext);} dots TK_INTEGER
    { dl = atol((char*) yytext);
      replic = dl - di+1;
      if(replic<1){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
      if(tour==1){
        n_aut+= replic - 1;
      }
      if(tour==2){
        aut--;
        net->get_aut_name(aut, temp);
        for(int i=di;i<=dl;i++){
          sprintf(aut_replic,"%s[%d]",temp,i);
          net->put_aut_name(aut, aut_replic);
          replic_map.put(aut, aut_group);
          aut++;
        }
      }
    }
    |TK_ID 
    { if(dom->get_name((char *) yytext)>=0){
        di = dom->get_first((char *) yytext);
        dl = dom->get_last((char *) yytext);
        replic = dl-di+1;
        if(tour==1){
          n_aut+= replic-1;
        }
        if(tour==2){
          aut--;
          net->get_aut_name(aut, temp);
          for(int i=di;i<=dl;i++){
            sprintf(aut_replic,"%s[%d]",temp,i);
            net->put_aut_name(aut, aut_replic);
            replic_map.put(aut, aut_group);
            aut++;
          }
        }
      }
      else {
        type_error(Parser_01);
      }
    }
    ;
    
blk_queue:
    TK_LEFT_SQUARE_BRACKET queue_number TK_RIGHT_SQUARE_BRACKET
    |
    ;
    
queue_number:
    TK_INTEGER {di = atol((char*) yytext);} dots TK_INTEGER
    { dl = atol((char*) yytext);
      queue = dl - di+1;
      if(queue<1){
        Semantic_Error(fname, Parser_114);
        not_error = 0;
      }
      if(tour==2){
        n_state+= queue-1;
      }
      if(tour==3){
        state--;
        net->get_state_name(aut, state, temp);
        for(int i=di;i<=dl;i++){
          sprintf(state_queue,"%s[%d]",temp,i);
          if(t_ident->get_name(state_queue)>0){
            type_error(Parser_06);
          }
          for(int j=0; j<replic; j++){
            net->put_state_name(aut+j, state, state_queue);
            net->put_state_reward(aut+j, state, state);
          }
          state++;
        }
      }
    }
    |TK_ID
    { if(dom->get_name((char *) yytext)>=0){
        di = dom->get_first((char *) yytext);
        dl = dom->get_last((char *) yytext);
        queue = dl - di+1;
        if(tour==2){
          n_state+= queue-1;
        }
        if(tour==3){
          state--;
          net->get_state_name(aut, state, temp);
          for(int i=di;i<=dl;i++){
            sprintf(state_queue,"%s[%d]",temp,i);
            if(t_ident->get_name(state_queue)>0){
              type_error(Parser_06);
            }
            for(int j=0; j<replic; j++){
              net->put_state_name(aut+j, state, state_queue);
              net->put_state_reward(aut+j, state, state);
            }
            state++;
          }
        }
      }
      else {
        type_error(Parser_01);
      }
    }
    ;


blk_reward:
    TK_LEFT_PARENTHESIS reward_number TK_RIGHT_PARENTHESIS
    |
    ; 
   
reward_number:
    signal number
    { strcat(temp, (char*) yytext);
      if(tour==3){
        for(int i=0; i<replic;i++){
          for(int j=0; j<queue; j++){
    	    net->put_state_reward(aut+i,state-1-j,atof(temp));
    	  }
        }
      }   	
    }
    |TK_ID
    { if(t_ident->get_name((char *) yytext)>=0){
    	if(t_ident->exp_value(t_ident->get_name((char *) yytext), &v_tmp)){
          if(tour==3){
            for(int i=0; i<replic;i++){
              for(int j=0; j<queue; j++){
    	        net->put_state_reward(aut+i,state-1-j,v_tmp);
    	      }
            }
          }
        }
        else{
          type_error(Parser_02);
        }
      }
      else{
        type_error(Parser_01);
      }   	
    }
    ;

             //-----------------------------------------------------------//
             //               Block to evaluation of results              //
             //-----------------------------------------------------------//

blk_res:
    TK_RESULTS {itop.reset(); }
    blk_def_res 
	;
	
blk_def_res:
    TK_ID
    { if(tour==4){
    	net->put_result_name((char *) yytext);
	    strcpy(fname,(char *) yytext);    	
	    ia = -1;
      }
    }
    TK_ATTRIB blk_exp
    { if(tour==4){
    	itop.get_func(fnc);
        itop.reset();
        net->put_result_exp(fnc);
      }
    }
    TK_SEMICOLON blk_def_res
    |
    ;
    
    
             //-----------------------------------------------------------//
             //                Block to othres definitions                //
             //-----------------------------------------------------------//


signal:
    TK_PLUS {strcpy(temp, "");}
    |TK_MINUS {strcpy(temp, "-");}
    |{strcpy(temp, "");}
    ;

dots:
    TK_2DOTS
    |TK_DOTS
    ;
        
aut_list: 
    TK_ID aut_dom list_member
    |
    ;
    
list_member:
    TK_COMMA TK_ID aut_dom list_member
    |
    ;
    
aut_dom:
    TK_LEFT_BRACKET aut_dom_int TK_RIGHT_BRACKET
    |
    ;

aut_dom_int:
    TK_ID
    |TK_INTEGER dots TK_INTEGER    
    ;
%%


void Init_Variables()
{
  yylineno = 0;
}


//int main()
//void Compile_Network(peps_name name, const char* path_, const char* results_path_)
void Compile_Network(const peps_name san_level_name, const peps_name des_level_name)
{ 
  peps_name n_tmp;
  peps_name n_tmp1;

  net = new NET;
  t_ident = new FUNC;
  dom = new DOM;
  edic = new EDIC;

  sprintf(n_tmp, "%s.san", san_level_name);

  n_aut = 0;
  aut = 0;
  state = 0;
  tour = 0;
  n_events = 0;

  t_ident->alloc(2); // auxiliar table to identifiers
  dom->alloc(1); // auxiliar table to dominion
  net->reset();
   
  Init_Variables();
  tour++; // first pass
  yyin = fopen(n_tmp, "r");
  yyparse();
  fclose(yyin);


  if(not_error){
    aut_group = 0;
    not_error = net->put_net_n_aut(n_aut); // alloc n_aut automata
    replic_map.create(n_aut); // create replic map to n_aut automata    
    alloc_mat_aut(n_aut); // alloc matrix to store automata-state
    edic->put_n_event(n_events);
    Init_Variables();
    net->reset();    
    tour++; // second pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
  }

  if(not_error){
    Init_Variables();
    net->reset();
    tour++; // third pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
    if(!net->put_net_n_ident(2))// alloc n_ident identifiers structure
      not_error = 0; 
    if(!net->put_net_n_result(2)) // alloc n_result results structure
      not_error = 0; 

    for(int i=0; i<net->get_net_n_aut(); i++){
      for(int j=0; j<net->get_aut_n_state(i); j++){
        if(!net->put_state_n_trans(i, j, mat_trans[i][j]))
          not_error = 0;
      }
    }
  }

  if(not_error){
    Init_Variables();
    net->reset();
    tour++; // fourth pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
    if(!net->rm_ident_cnst()){
      not_error = 0;
    }
    if(!net->rm_reach_cnst()){
      not_error = 0;     
    }        
    if(!net->rm_result_cnst()){
      not_error = 0;     
    }
  }

  if(not_error){
    Init_Variables();
    net->reset();
    tour++; // fifth pass
    yyin = fopen(n_tmp, "r");
    yyrestart(yyin);
    yyparse();
    fclose(yyin);
  }

  if(not_error){
    strcpy(n_tmp, des_level_name);  	
    net->put2file_des(n_tmp);
    net->put2file_dic(n_tmp);
    net->put2file_tft(n_tmp);
    if(!r_t){
      net->put2file_fct(n_tmp);
      cout << endl;
    }
    else{
      if(!Exists_File(n_tmp, fct_file)){
        net->put2file_fct(n_tmp);
        cout << " (partial reachable state space)\n";
      }
      else{
        cout << " :-) existing file '" << n_tmp << ".fct ' used as reachable state space\n";  
      }
    } 
    net->put2file_res(n_tmp);
  } 

  itop.reset();

  if(net)
    delete net;
  if(t_ident)
    delete t_ident;
  if(dom)
    delete dom;
  if(edic)
    delete edic;

  if(!not_error)
    exit(0);
}
