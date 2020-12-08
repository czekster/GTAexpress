#include "grammar_.h"

void GRAMMAR_::net_blk()
{
  cout << " Compiling network block\n";
  if (get_tk() != TK_NETWORK)
    Syntactic_error("network");
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  put_model_name(tk);
  if (get_tk() != TK_LEFT_PARENTHESIS)
    Syntactic_error("(");
  get_tk();
  if (TK != TK_CONTINUOUS && TK != TK_DISCRETE)
    Syntactic_error("continuous or discrete"); 
  TK != TK_CONTINUOUS ? put_model_type(CONTINUOUS) : put_model_type(DISCRETE);
  if (get_tk() != TK_RIGHT_PARENTHESIS)
    Syntactic_error(")");
  aut_blk();
}

void GRAMMAR_::aut_blk()
{
  char id[MAX_ID_NAME];
  char repA[MAX_ID_NAME];
  char aut[MAX_ID_NAME];
  int  faut, laut;
  int  n = 0;

  if (get_tk() != TK_AUTOMATON) 
    Syntactic_error("aut");

  while (TK == TK_AUTOMATON){
    if (get_tk() != TK_ID)
      Syntactic_error("An identifier");
    strcpy(id, tk);
    faut = n;
    if (aut_rep_blk(repA)){
      if (!dom.is_dom(repA))
        Network_semantic_error(Parser_600, "Automaton", id, repA, 600);
      for (int i=0; i<dom.get_length(repA); i++){
        sprintf(aut, "%s[%d]", id, dom.get_pos(repA, i));
        if (tour == 1) 
          ndic.put_aut(aut, n); 
        n++;
      }
    }
    else{
      if (tour == 1) 
        ndic.put_aut(id, n); 
      n++;
    }
    laut = n;
    stt_blk(faut, laut);
    get_tk();
  }
  put_tk();
}

int GRAMMAR_::aut_rep_blk(char *_repA)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ 
    put_tk();                              
    return(0);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repA, tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  return (1);
}

void GRAMMAR_::stt_blk(int _faut, int _laut)
{
  char id[MAX_ID_NAME];
  char repS[MAX_ID_NAME];
  char stt[MAX_ID_NAME];
  char rw[MAX_ID_NAME];
  int  fstt, lstt;
  int  n = 0;
 
  get_tk();
  if (TK != TK_STATE && TK != TK_FROM) 
    Syntactic_error("stt");
  while (TK == TK_STATE || TK == TK_FROM){
    if (TK == TK_STATE){
      if (get_tk() != TK_ID)
        Syntactic_error("An identifier");
      strcpy(id, tk);
      fstt = n;
      if(stt_rep_blk(repS)){
        if (!dom.is_dom(repS))
          Network_semantic_error(Parser_600, "State", id, repS, 601);
        if (stt_reward(rw)){
          for (int i=0; i<dom.get_length(repS); i++){
            for (int j=_faut; j<_laut; j++){
              sprintf(stt, "%s[%d]", id, dom.get_pos(repS, i));
              if (tour == 1) 
                ndic.put_stt(stt, j, n);
              else 
                ((_laut - _faut) > 1) ? aut[j].put_stt(stt, n, ident.eval(rw, (j-_faut), i)) : aut[j].put_stt(stt, n, ident.eval(rw, j, i));
            }
            n++;
          }
        }
        else{
          for (int i=0; i<dom.get_length(repS); i++){
            for (int j=_faut; j<_laut; j++){
              sprintf(stt, "%s[%d]", id, dom.get_pos(repS, i));
              if (tour == 1) 
                ndic.put_stt(stt, j, n);
              else
                aut[j].put_stt(stt, n, n);              
            }
            n++;
          }
        }
      }
      else{
        if (stt_reward(rw)){
          for (int j=_faut; j<_laut; j++)
            if (tour == 1) 
              ndic.put_stt(id, j, n);
            else
              ((_laut - _faut) > 1) ? aut[j].put_stt(id, n, ident.eval(rw, (j-_faut), n)) : aut[j].put_stt(id, n, ident.eval(rw, j, n));
          n++;        
        }
        else{
          for (int j=_faut; j<_laut; j++)
            if (tour == 1) 
              ndic.put_stt(id, j, n);
            else
              aut[j].put_stt(id, n, n);              
          n++;        
        }
      }
      lstt = n;
    }
    else{
      if (get_tk() != TK_ID)
        Syntactic_error("An identifier");
      strcpy(id, tk);
      if(from_rep_blk(repS)){
        if (tour == 2){
          for (int i=0; i<dom.get_length(repS); i++)
            if (ndic.get_stt_rep(_faut, id, dom.get_pos(repS, i)) < 0){
              sprintf(stt, "%s[%d]", id, i);
              ndic.get_aut_name(_faut, id);
              Network_semantic_error(Parser_601, "Automaton", id, stt, 602);
            }
          fstt = ndic.get_stt_rep(_faut, id, dom.get_first_pos(repS));
          lstt = ndic.get_stt_rep(_faut, id, dom.get_last_pos(repS)) + 1;
        }
      }
      else{
        if (tour == 2){
          if (ndic.get_stt_rep(_faut, id, ident.eval(repS)) < 0){
            sprintf(stt, "%s[%d]", id, ident.eval(repS));
            ndic.get_aut_name(_faut, id);
            Network_semantic_error(Parser_601, "Automaton", id, stt, 603);
          }
          fstt = ndic.get_stt_rep(_faut, id, ident.eval(repS));
          lstt = fstt+1;
        }
      }
    }
    to_blk(_faut, _laut, fstt, lstt);
    get_tk();
  }
  put_tk();
}

int GRAMMAR_::stt_rep_blk(char *_repS)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ 
    put_tk();                              
    return(0);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repS, tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  return (1);
}

int GRAMMAR_::from_rep_blk(char *_repS)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET)
    Syntactic_error("[");
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repS, tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (!ident.is_func(_repS) && !dom.is_dom(_repS))
    Network_semantic_error(Parser_602, _repS, 604);
  if (ident.is_func(_repS))
    if (ident.is_const(_repS))
      return(0);
    else
      Network_semantic_error(Parser_603, _repS, 605);
  return (1);
}

int GRAMMAR_::stt_reward(char *_rw)
{
  if (get_tk() != TK_LEFT_PARENTHESIS){
    put_tk();                              
    return(0);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_rw, tk);
  if (get_tk() != TK_RIGHT_PARENTHESIS)
    Syntactic_error(")");
  if (!ident.is_func(_rw))
    Network_semantic_error(Parser_604, _rw, 606);
  if (!ident.is_const(_rw))
    Network_semantic_error(Parser_603, _rw, 607);
   return(1);
}

void GRAMMAR_::to_blk(int _faut, int _laut, int _fstt, int _lstt)
{
  char id[MAX_ID_NAME];
  char repT[MAX_ID_NAME];
  char restr[MAX_ID_NAME];
  int left_par = 0;

  if (get_tk() != TK_TRANS) 
    Syntactic_error("to");

  while (TK == TK_TRANS){
    id[0] = '\0';
    repT[0] = '\0';
    left_par = 0;
    get_tk() == TK_LEFT_PARENTHESIS ? left_par++ : put_tk();
    switch(get_tk()){
      case TK_ID:          strcpy(id, tk);
                           to_rep_blk(repT);
                           restr_blk(restr);
                           get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
                           ev_lst(_faut, _laut, _fstt, _lstt, id, repT, restr, 0);
                           break;
      case TK_PLUS_PLUS:   restr_blk(restr);
                           get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
                           ev_lst(_faut, _laut, _fstt, _lstt, id, repT, restr, 1);
                           break;
      case TK_PLUS:        if (get_tk() != TK_INTEGER)
                             Syntactic_error("A number");
                           restr_blk(restr);
                           get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
                           ev_lst(_faut, _laut, _fstt, _lstt, id, repT, restr, atol(tk));
                           break;
      case TK_MINUS_MINUS: restr_blk(restr);
                           get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
                           ev_lst(_faut, _laut, _fstt, _lstt, id, repT, restr, -1);
                           break;
      case TK_MINUS:       if (get_tk() != TK_INTEGER)
                             Syntactic_error("A number");
                           restr_blk(restr);
                           get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
                           ev_lst(_faut, _laut, _fstt, _lstt, id, repT, restr, (-atol(tk)));
                           break;
      case TK_EQUAL:       restr_blk(restr);
                           get_tk() == TK_RIGHT_PARENTHESIS ? left_par-- : put_tk();
                           ev_lst(_faut, _laut, _fstt, _lstt, id, repT, restr, 0);
                           break;
      default:             Syntactic_error();
    }
    if (left_par)
      Syntactic_error();
    get_tk();
  }
  put_tk();
}

int GRAMMAR_::to_rep_blk(char *_repT)
{
  _repT[0] = '\0';
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ 
    put_tk();                              
    return(0);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repT, tk);
  if (!ident.is_func(_repT) && !dom.is_dom(_repT))
    Network_semantic_error(Parser_602, _repT, 608);
  if (ident.is_func(_repT) && !ident.is_descr(_repT))
    Network_semantic_error(Parser_605, _repT, 609);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  return (1);
}


int GRAMMAR_::restr_blk(char *_restr)
{
  _restr[0] = '\0';
  if (get_tk() == TK_DIV){
    if (get_tk() != TK_ID)
      Syntactic_error("An identifier");
    strcpy(_restr, tk);
    if (!ident.is_descr(_restr))
      Network_semantic_error(Parser_605, _restr, 610);
    return(1);
  }
  else
    put_tk();
  return (0);
}


void GRAMMAR_::ev_lst(int _faut, int _laut, int _fstt, int _lstt, char *_idT, char *_repT, char *_restrT, int inc_dec)
{
  int fto, lto;
  int fem, lem, iem;
  int fen, len, ien;
  int feo, leo, ieo;
  int ii, ij, ik;
  int n_rep;
  int idP;

  char aux[MAX_ID_NAME];
  char ev[MAX_ID_NAME];
  char idE[MAX_ID_NAME];
  char repEM[MAX_ID_NAME];
  char repEN[MAX_ID_NAME];
  char repEO[MAX_ID_NAME];  
  char restrE[MAX_ID_NAME]; 
  
  repEM[0]='\0'; repEN[0]='\0'; repEO[0]='\0'; restrE[0]='\0'; 

//  cout << "identifier: " << _idT << endl;
//  cout << "replic: " << _repT << endl;
//  cout << "restriction: " << _restrT << endl;

  if (get_tk() != TK_ID) 
    Syntactic_error("An identifier");

  while (TK == TK_ID){
    strcpy(idE, tk);
    n_rep = ev_rep_lst(repEM, repEN, repEO);
    idP = -1;
    if (get_tk() == TK_LEFT_PARENTHESIS){
      if (get_tk() != TK_ID)
        Syntactic_error("An identifier");
      if (!ident.is_func(tk))
        Network_semantic_error(Parser_604, tk, 611);
      if (!ident.check_rate(tk))
        Network_semantic_error(Parser_606, tk, 612);
      idP = ident.get_id(tk);  
      if (get_tk() != TK_RIGHT_PARENTHESIS)
        Syntactic_error(")");
    }
    else
      put_tk();
      
    restr_blk(restrE);    
    
    if (tour == 2){
      // parser ok
      for (int i=_faut; i<_laut; i++){
        ((_laut - _faut) > 1) ? ii = i-_faut : ii = i;
        for (int j=_fstt; j<_lstt; j++){
        ((_lstt - _fstt) > 1) ? ij = j-_fstt : ij = j;        
          if (_idT[0] == '\0'){ // jumps ++ -- == 
            if (_lstt-_fstt <= 1){
              ndic.get_stt_name(i, j, aux);
              Network_semantic_error(Parser_607, aux, 613);
            }
            fto = j + inc_dec; 
            if (fto < _fstt || fto >= _lstt)
              lto = fto; // do not insert an event out of a domain 
            else
              lto = fto + 1;
          }
          else{
            if (_repT[0] == '\0'){ // simple transition
              fto = ndic.get_stt(i, _idT);
              if (fto == -1)
                Network_semantic_error(Parser_601, _idT, 614);
              lto = fto + 1;
            }
            else{ // replicated transition
              if (dom.is_dom(_repT)){ 
                fto = ndic.get_stt_rep(i, _idT, dom.get_first_pos(_repT));
                if (fto == -1)
                   Network_semantic_error(Parser_601, _idT, 615);
                lto = ndic.get_stt_rep(i, _idT, dom.get_last_pos(_repT)) + 1;  
                if (lto == -1)
                  Network_semantic_error(Parser_601, _idT, 616);
              }
              else{
                fto = ndic.get_stt_rep(i, _idT, ident.eval(_repT, ii, ij));
                if (fto == -1)
                  Network_semantic_error(Parser_601, _idT, 617);
                lto = fto + 1; 
              }
            }
          }
          
          for (int k=fto; k<lto; k++){
            ((lto - fto) > 1) ? ik = k-fto : ik = k;     
            switch (n_rep){
              case 3: if(dom.is_dom(repEO)){    
                        feo = 0;
                        leo = dom.get_length(repEO);
                      }
                      else{
                        feo = ident.eval(repEO, ii, ij, ik);
                        leo = feo + 1;;
                      }
              case 2: if(dom.is_dom(repEN)){    
                        fen = 0;
                        len = dom.get_length(repEN);
                      }
                      else{
                        fen = ident.eval(repEN, ii, ij, ik);
                        len = fen + 1;;
                      }
              case 1: if(dom.is_dom(repEM)){    
                        fem = 0;
                        lem = dom.get_length(repEM);
                      }
                      else{
                        fem = ident.eval(repEM, ii, ij, ik);
                        lem = fem + 1;;
                      }
              case 0: break;
            }     
            switch (n_rep){
              case 0: if ((_restrT[0] == '\0' || ident.eval(_restrT, ii, ij, ik)) && (restrE[0]=='\0' || ident.eval(restrE, ii, ij, ik))){
                        if (!edic.is_ev(idE))
                          Network_semantic_error(Parser_608, idE, 618);
                        aut[i].put_ev(j, k, edic.get_ev(idE), idP);
                        edic.concerned_aut(idE, i);
                      }
                      break;
              case 1: for (int em=fem; em<lem; em++){
                        dom.is_dom(repEM) ? iem = dom.get_pos(repEM, em) : iem = em;
                        if ((_restrT[0]=='\0' || ident.eval(_restrT, ii, ij, ik)) && (restrE[0]=='\0' || ident.eval(restrE, ii, ij, ik))){
                          sprintf(ev,"%s[%d]", idE, iem);
                          if (!edic.is_ev(ev))
                            Network_semantic_error(Parser_608, ev, 619);
                          aut[i].put_ev(j, k, edic.get_ev(ev), idP);
                          edic.concerned_aut(ev, i);
                        }
                      }
                      break;
              case 2: for (int em=fem; em<lem; em++){
                        dom.is_dom(repEM) ? iem = dom.get_pos(repEM, em) : iem = em;
                        for (int en=fen; en<len; en++){
                          dom.is_dom(repEN) ? ien = dom.get_pos(repEN, en) : ien = en;
                          if ((_restrT[0]=='\0' || ident.eval(_restrT, ii, ij, ik)) && (restrE[0]=='\0' || ident.eval(restrE, ii, ij, ik))){
                            sprintf(ev,"%s[%d][%d]", idE, iem, ien); 
                            if (!edic.is_ev(ev))
                              Network_semantic_error(Parser_608, ev, 620);
                            aut[i].put_ev(j, k, edic.get_ev(ev), idP);
                            edic.concerned_aut(ev, i);
                          }
                        }
                      }
                      break;
              case 3: for (int em=fem; em<lem; em++){
                        dom.is_dom(repEM) ? iem = dom.get_pos(repEM, em) : iem = em;
                        for (int en=fen; en<len; en++){
                          dom.is_dom(repEN) ? ien = dom.get_pos(repEN, en) : ien = en;
                          for (int eo=feo; eo<leo; eo++){
                            dom.is_dom(repEO) ? ieo = dom.get_pos(repEO, eo) : ieo = eo;
                            if ((_restrT[0]=='\0' || ident.eval(_restrT, ii, ij, ik)) && (restrE[0]=='\0' || ident.eval(restrE, ii, ij, ik))){
                              sprintf(ev,"%s[%d][%d][%d]", idE, iem, ien, ieo); 
                              if (!edic.is_ev(ev))
                                Network_semantic_error(Parser_608, ev, 621);
                              aut[i].put_ev(j, k, edic.get_ev(ev), idP);
                              edic.concerned_aut(ev, i);
                            }
                          }
                        }
                      }
                      break;
            }
          }
        }
      }
    }
    get_tk();
  }
  put_tk();
}

int GRAMMAR_::ev_rep_lst(char *_repEM, char *_repEN, char *_repEO)
{
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ 
    put_tk();                              
    return(0);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repEM, tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (!ident.is_func(_repEM) && !dom.is_dom(_repEM))
    Network_semantic_error(Parser_602, _repEM, 622);
  if (ident.is_func(_repEM) && !ident.is_descr(_repEM))
    Network_semantic_error(Parser_605, _repEM, 623);
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ 
    put_tk();                              
    return(1);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repEN, tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (!ident.is_func(_repEN) && !dom.is_dom(_repEN))
    Network_semantic_error(Parser_602, _repEN, 624);
  if (ident.is_func(_repEN) && !ident.is_descr(_repEN))
    Network_semantic_error(Parser_605, _repEN, 625);
  if (get_tk() != TK_LEFT_SQUARE_BRACKET){ 
    put_tk();                              
    return(2);                             
  }
  if (get_tk() != TK_ID)
    Syntactic_error("An identifier");
  strcpy(_repEO, tk);
  if (get_tk() != TK_RIGHT_SQUARE_BRACKET)
    Syntactic_error("]");
  if (!ident.is_func(_repEO) && !dom.is_dom(_repEO))
    Network_semantic_error(Parser_602, _repEO, 626);
  if (ident.is_func(_repEO) && !ident.is_descr(_repEO))
    Network_semantic_error(Parser_605, _repEO, 627);
  return(3);
}
