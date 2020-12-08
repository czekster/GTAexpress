#include "grammar_.h"

int GRAMMAR_::ev_blk()
{
  char id[MAX_ID_NAME];
  char ev[MAX_ID_NAME];
  char repN[MAX_ID_NAME];
  char repM[MAX_ID_NAME];
  char repO[MAX_ID_NAME];  
  int left_par = 0;
  int type     = 0;
  int n_rep    = 0;
  
  //id[0] = '\0';
  //ev[0] = '\0';
  repN[0] = '\0';
  repM[0] = '\0';
  repO[0] = '\0';

  cout << " Compiling event block\n";
  if (get_tk() != TK_EVENTS)
    Syntactic_error();
  get_tk();
  if (TK != TK_SYN && TK != TK_LOC)
    Syntactic_error("syn or loc"); 
  while (TK == TK_SYN || TK == TK_LOC){
    TK == TK_SYN ? type = SYN : type = LOC;
    if (get_tk() != TK_ID)
      Syntactic_error("An identifier");
    strcpy(id, tk);
    n_rep = ev_rep_blk(id, repM, repN, repO);
    left_par = 0;
    get_tk() == TK_LEFT_PARENTHESIS ? left_par++ : put_tk();
    if (get_tk() != TK_ID) 
      Syntactic_error("An identifier");
    sprintf(ev,"%s[%s][%s][%s]",id, repM, repN, repO); 
    if (!ident.is_func(tk))
      Event_semantic_error(Parser_300, ev, tk, 300);
    if (!ident.check_rate(tk))
      Event_semantic_error(Parser_301, ev, tk, 301);
    if (tour == 2){ 
      switch(n_rep){
        case 0: if (!edic.put_ev(id, type, ident.get_id(tk)))
                  Event_semantic_error(Parser_302, ev, 302);
                break;
        case 1: for (int i=0; i<dom.get_length(repM); i++){
                  sprintf(ev,"%s[%d]", id, dom.get_pos(repM,i)); 
                  if (!edic.put_ev(ev, type, ident.get_id(tk)))
                    Event_semantic_error(Parser_302, ev, 303);
                }
                break;
        case 2: for (int i=0; i<dom.get_length(repM); i++){
                  for (int j=0; j<dom.get_length(repN); j++){
                    sprintf(ev,"%s[%d][%d]",id, dom.get_pos(repM, i), dom.get_pos(repN, j)); 
                    if (!edic.put_ev(ev, type, ident.get_id(tk)))
                      Event_semantic_error(Parser_302, ev, 304);
                  }
                }
                break;
        case 3: for (int i=0; i<dom.get_length(repM); i++){
                  for (int j=0; j<dom.get_length(repN); j++){
                    for (int k=0; k<dom.get_length(repO); k++){
                      sprintf(ev,"%s[%d][%d][%d]",id, dom.get_pos(repM, i), dom.get_pos(repN, j), dom.get_pos(repO, k)); 
                      if (!edic.put_ev(ev, type, ident.get_id(tk)))
                        Event_semantic_error(Parser_302, ev, 305);
                    }
                  }
                }
                break;
      }
    }
    get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
    if (left_par)
      Syntactic_error();
      
    if (get_tk() != TK_SEMICOLON)
      put_tk();
    get_tk();
  }
  put_tk();
  return(1);
}

int GRAMMAR_::ev_rep_blk(char *_ev, char *_repM, char *_repN, char *_repO)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ // First replication. Replication is 
    put_tk();                              //            in only one dimension.
    return(0);                             //                        Ex.: EV[M]
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repM,tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");

  if (!dom.is_dom(_repM))
    Event_semantic_error(Parser_303, _ev, _repM, 306);

  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ // Second replication. Replication is 
    put_tk();                              //                 in two dimensions.
    return(1);                             //                      Ex.: EV[M][N]
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repN,tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (!dom.is_dom(_repN))
    Event_semantic_error(Parser_303, _ev, _repN, 307);
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){  // third replication. Replication is 
    put_tk();                               //              in three dimensions.
    return(2);                              //                  Ex.: EV[M][N][O]
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repO,tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (!dom.is_dom(_repO))
    Event_semantic_error(Parser_303, _ev, _repO, 308);
  return (3);
}
