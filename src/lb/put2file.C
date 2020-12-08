//====================================================================================//
//                                                                                    //
//                                   Network Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   put2file.C                  Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    12/Aug/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 12/Aug/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// network form.                                                                      //
//  This file implements the functions to put the data structure in the auxiliar files//
// as descriptor, functions, dictionary and ect, of the class NET.                    //
//====================================================================================//

#include "sanfile.h"

//#define next_tok 
//====================================================================================//
//  Errors in compilation process                                                     //
//====================================================================================//
/*
enum comp_errors { err_7001 ,     // no space between signs
                   err_7002 ,     // incomplete "min" or "max" sign
                   err_7003 ,     // incomplete "not-equal" sign
                   err_7004 ,     // incomplete "and" sign
                   err_7005 ,     // incomplete "summation" definition
                   err_7006 ,     // incomplete "or" or "xor" sign
                   err_7007 ,     // incomplete "equal" sign
                   err_7008 ,     // incomplete automaton state or reward definition
                   err_7009 ,     // invalid function definition
                   err_7010 ,     // non-contiguous function identifiers
                   err_7011 ,     // incomplete function definition
                   err_7012 ,     // rows too short (misplaced ";")
                   err_7013 ,     // rows too long (misplaced ";")
                   err_7014 ,     // rows missing (misplaced "]")
                   err_7015 ,     // too many rows (misplaced "]")
                   err_7016 ,     // invalid number of automata
                   err_7017 ,     // invalid automaton size
                   err_7018 ,     // bad function definition
                   err_7019 ,     // missing functions
                   err_7020 ,     // unexpected character or end of file
                   err_7021 ,     // invalid function domain
                   err_7022 ,     // missing ":" in rewards definition
                   err_7023 ,     // incomplete double implication sign
                   err_7024 ,     // 
                   err_0000 };    // no error found!
*/

//------------------------------------------------------------------------------
// count_events - Count synchronized events
//------------------------------------------------------------------------------
int NET::count_events()
{
  int ind = 0, count = 0;
  char **dic;
  char aux[MAX_EVENT_NAME];
  int flag;
  
  dic = new char*[MAX_TABLE_SIZE];
  for(int i=0;i<MAX_TABLE_SIZE;i++){
    dic[i] = new char[MAX_EVENT_NAME];
    strcpy(dic[i],"");
  }
  
  for(int i=0; i<n_aut; i++){
    for(int j=0; j<get_aut_n_state(i); j++){
      for(int k=0; k<get_state_n_trans(i, j); k++){
        for(int l=0; l<get_event_n_mst(i, j, k); l++){
          get_mst_name(i, j, k, l, aux);
          flag = 0;
          for(int m=0; m<ind; m++){
            if(!strcmp(aux,dic[m])){
              flag = 1;
            }
          }  
          if(flag==0){
           strcpy(dic[ind],aux);
           count++;
           ind++;
          }
        }
        for(int l=0; l<get_event_n_sync(i, j, k); l++){
          get_sync_name(i, j, k, l, aux);
          flag = 0;
          for(int m=0; m<ind; m++){
            if(!strcmp(aux,dic[m])){
              flag = 1;
            }
          }  
          if(flag==0){
           strcpy(dic[ind],aux);
           count++;
           ind++;
          }
        }
      }
    }
  }  

  for(int i=0;i<MAX_TABLE_SIZE;i++){
    delete [] dic[i];
  }
  delete [] dic;
  return(count);
}

//------------------------------------------------------------------------------
// get_event_name - get the name of event i
// i i - event index
// o _name - event name
//------------------------------------------------------------------------------
int NET::get_event_name(int e, char *_name)
{
  int ind = 0, count = 0;
  char **dic;
  char aux[MAX_EVENT_NAME];
  int flag;
  
  dic = new char*[MAX_TABLE_SIZE];
  for(int i=0;i<MAX_TABLE_SIZE;i++){
    dic[i] = new char[MAX_EVENT_NAME];
    strcpy(dic[i],"");
  }
  
  for(int i=0; i<n_aut; i++){
    for(int j=0; j<get_aut_n_state(i); j++){
      for(int k=0; k<get_state_n_trans(i, j); k++){
        for(int l=0; l<get_event_n_mst(i, j, k); l++){
          get_mst_name(i, j, k, l, aux);
          flag = 0;
          for(int m=0; m<ind; m++){
            if(!strcmp(aux,dic[m])){
              flag = 1;
            }
          }  
          if(flag==0){
           strcpy(dic[ind],aux);
           count++;
           ind++;
          }
        }
        for(int l=0; l<get_event_n_sync(i, j, k); l++){
          get_sync_name(i, j, k, l, aux);
          flag = 0;
          for(int m=0; m<ind; m++){
            if(!strcmp(aux,dic[m])){
              flag = 1;
            }
          }  
          if(flag==0){
           strcpy(dic[ind],aux);
           count++;
           ind++;
          }
        }
      }
    }
  }  
 
  strcpy(_name,dic[e]);
  
  for(int i=0;i<MAX_TABLE_SIZE;i++){
    delete [] dic[i];
  }
  delete [] dic;
  return(count);
}

//------------------------------------------------------------------------------
// put_local - print local matrix
// i fout - file 
// i aut - automaton number
//------------------------------------------------------------------------------
void NET::put_local(fstream &fout, int _aut)
{
  char pos[MAX_EXP_SIZE];
  char neg[MAX_EXP_SIZE];
  char aux[MAX_EXP_SIZE];
  int i_aux, flag, flag2;
  char **vet;
  
  vet = new char*[get_aut_n_state(_aut)];
  if(!vet){
    Programming_Error1(Parser_02);	
    exit(0);    
  }
  for(int i=0; i<get_aut_n_state(_aut); i++){
    vet[i] = new char[MAX_FUNC_NAME];
    if(!vet[i]){
      Programming_Error1(Parser_02);	
      exit(0);    
    }
  }

  fout << "[";

  for(int i=0; i<get_aut_n_state(_aut); i++){
    for(int l=0; l<get_aut_n_state(_aut); l++){
      strcpy(vet[l],"0");      
    }	
    flag = 0;
    flag2 = 0;
    for(int j=0; j<get_state_n_trans(_aut, i); j++){
      for(int k=0; k<get_event_n_local(_aut, i, j); k++){
        if(get_local_rate(_aut, i, j, k, aux, &i_aux)){
          get_ident_exp(i_aux, aux); 
        }
        if(k==0){
          sprintf(pos, "%s", aux);
        }
        else{
          sprintf(pos, "%s %s +", pos, aux); 
        }
      }
      if(get_event_n_local(_aut, i, j)>0){
        if(get_ident_exp(pos)>=0){
          sprintf(vet[get_trans_dst(_aut, i, j)],"F%d", get_ident_exp(pos));
        }
        else{
          put_ident_name("new_local");
          i_aux = put_ident_exp(pos);
          sprintf(vet[get_trans_dst(_aut, i, j)],"F%d", i_aux);
        }
        if(!flag2){
          sprintf(neg, "%s", pos);
          flag2 = 1;
        }
        else{
          sprintf(neg, "%s %s +", neg, pos);
        }
        flag = 1;
      }
    }
    if(flag){
  	strcat(neg, " -1 *");
      if(get_ident_exp(neg)>=0){
        sprintf(vet[i],"F%d", get_ident_exp(neg));        
      }
      else{
        put_ident_name("new_local");
        i_aux = put_ident_exp(neg);
        sprintf(vet[i],"F%d", i_aux);              
      }
    }
    for(int l=0; l<get_aut_n_state(_aut); l++){
      fout << " " << vet[l]; 
    }
    if(i<get_aut_n_state(_aut)-1)
      fout << " ;\n";
  }
  
  fout << " ]\n";
  for(int i=0; i<get_aut_n_state(_aut); i++){
    if(vet[i])
      delete[] vet[i];
  }
  
  if(vet)
    delete[] vet;
}

//------------------------------------------------------------------------------
// positive_event - print positive matrix
// o fout - file
// i i - automaton number
// i _event - event name
//------------------------------------------------------------------------------
int NET::positive_event(fstream &fout, int i, char *_event)
{ 
  char pos[MAX_EXP_SIZE];
  char aux[MAX_EXP_SIZE];
  char e_name[MAX_EVENT_NAME];
  int i_aux, i_dst, flag;
  int m, s;
  char ***vet;
  
  vet = new char**[get_aut_n_state(i)];
  if(!vet){
    Programming_Error1(Parser_02);	
    exit(0);    
  }
  for(int j=0; j<get_aut_n_state(i); j++){
    vet[j] = new char*[get_aut_n_state(i)];
    if(!vet[j]){
      Programming_Error1(Parser_02);	
      exit(0);    
    }
  }

  for(int j=0; j<get_aut_n_state(i); j++){
    for(int k=0; k<get_aut_n_state(i); k++){
      vet[j][k] = new char[MAX_FUNC_NAME];
      if(!vet[j][k]){
        Programming_Error1(Parser_02);	
        exit(0);    
      }
    }
  }

  fout << "[";
  flag = 1;
  m = 0;
  s = 0;
  for(int h=0; h<get_aut_n_state(i); h++){
    for(int j=0; j<get_aut_n_state(i); j++){
      for(int l=0; l<get_aut_n_state(i); l++){
        strcpy(vet[h][l],"0");      
      }	
      for(int k=0; k<get_state_n_trans(i, j); k++){
        i_dst = get_trans_dst(i, j, k);
        for(int l=0; l<get_event_n_mst(i, j, k); l++){
          get_mst_name(i, j, k, l, e_name);
          if(!strcmp(_event, e_name)){
            if(get_mst_rate(i, j, k, l, aux, &i_aux)){
              sprintf(pos, "F%d",i_aux);
            }
            else{
              sprintf(pos, "%s", aux); 
            }
            strcpy(vet[j][i_dst], pos);
            flag = 0;
            m = 1;
          }
        }          
        for(int l=0; l<get_event_n_sync(i, j, k); l++){
          get_sync_name(i, j, k, l, e_name);
          if(!strcmp(_event, e_name)){
            if(get_sync_prob(i, j, k, l, aux, &i_aux)){
              sprintf(pos, "F%d",i_aux);
            }
            else{
              sprintf(pos, "%s", aux); 
            }
            strcpy(vet[j][i_dst], pos);
            flag = 0;
            s = 1;
          }
        }          
        if(m==1){
          get_aut_name(i,e_name);	 
          if(strchr(e_name,'[')){
            Semantic_Error5(_event, Parser_102);
            exit(0);
          } 
        }
        if(m==1 && s==1){
          get_aut_name(i,e_name);	
          Semantic_Error2(_event, e_name, Parser_113);
          exit(0);
        }
      }
    }
  }
 
  if(flag){
    for(int h=0; h<get_aut_n_state(i); h++){
      strcpy(vet[h][h],"1"); 
    }
  }
  for(int h=0; h<get_aut_n_state(i); h++){
    for(int l=0; l<get_aut_n_state(i); l++){
      fout << " " << vet[h][l]; 
    }
    if(h<get_aut_n_state(i)-1)
      fout << " ;\n";
  }
  
  fout << " ]\n";
  for(int j=0; j<get_aut_n_state(i); j++){
    for(int k=0; k<get_aut_n_state(i); k++){	
      if(vet[j][k])
        delete[] vet[j][k];
    }
  }

  for(int j=0; j<get_aut_n_state(i); j++){
    if(vet[j])
      delete[] vet[j];
  }
  
  if(vet)
    delete[] vet;
    
  if(m)
    return(1);
  if(s)
    return(2);
  return(0);
}

//------------------------------------------------------------------------------
// negative_event - print negative matrix
// o fout - file
// i i - automaton number
// i _event - event name
// i m_s - master or synchronized
//------------------------------------------------------------------------------
int NET::negative_event(fstream &fout, int i, char *_event, int m_s)
{ 
  char pos[MAX_EXP_SIZE];
  char aux[MAX_EXP_SIZE];
  char e_name[MAX_EVENT_NAME];
  int i_aux, i_dst, flag, first;
  double v;
  char ***vet;
  
  vet = new char**[get_aut_n_state(i)];
  if(!vet){
    Programming_Error1(Parser_02);	
    exit(0);    
  }
  for(int j=0; j<get_aut_n_state(i); j++){
    vet[j] = new char*[get_aut_n_state(i)];
    if(!vet[j]){
      Programming_Error1(Parser_02);	
      exit(0);    
    }
  }

  for(int j=0; j<get_aut_n_state(i); j++){
    for(int k=0; k<get_aut_n_state(i); k++){
      vet[j][k] = new char[MAX_FUNC_NAME];
      if(!vet[j][k]){
        Programming_Error1(Parser_02);	
        exit(0);    
      }
    }
  }


  fout << "[";
  flag = 1;
  for(int h=0; h<get_aut_n_state(i); h++){
    for(int j=0; j<get_aut_n_state(i); j++){
      for(int l=0; l<get_aut_n_state(i); l++){
        strcpy(vet[h][l],"0");      
      }	
      for(int k=0; k<get_state_n_trans(i, j); k++){
        i_dst = get_trans_dst(i, j, k);
        for(int l=0; l<get_event_n_mst(i, j, k); l++){
          get_mst_name(i, j, k, l, e_name);
          if(!strcmp(_event, e_name)){
            if(get_mst_rate(i, j, k, l, aux, &i_aux)){
              get_ident_exp(i_aux,aux);
            }
            sprintf(pos, "%s", aux); 
            strcpy(vet[j][i_dst], pos);
            flag = 0;
          }
        }          
        for(int l=0; l<get_event_n_sync(i, j, k); l++){
          get_sync_name(i, j, k, l, e_name);
          if(!strcmp(_event, e_name)){
            if(get_sync_prob(i, j, k, l, aux, &i_aux)){
              get_ident_exp(i_aux, aux);
            }
            sprintf(pos, "%s", aux); 
            strcpy(vet[j][i_dst], pos);
            flag = 0;
          }
        }          
      }
    }
  }

  for(int h=0; h<get_aut_n_state(i); h++){
    strcpy(pos,"");	
    first = 1;
    for(int j=0; j<get_aut_n_state(i); j++){
      if(strcmp(vet[h][j],"0")){
        if(first){
          strcpy(pos,vet[h][j]);
          first = 0;
        }	
        else{
          sprintf(pos,"%s %s +", pos, vet[h][j]);
        }
      }
    }
    if(first==0){
      if(m_s==1)	
        strcat(pos, " -1 *");
      if(get_ident_exp(pos)<0){
        put_ident_name("new_rate");
        put_ident_exp(pos);
      }
      for(int j=0; j<get_aut_n_state(i); j++){
        strcpy(vet[h][j],"0");
      }
      i_aux = get_ident_exp(pos);
      if(ident.exp_value(i_aux, &v))
        sprintf(vet[h][h],"%s", pos);
      else
        sprintf(vet[h][h],"F%d", get_ident_exp(pos));
    }
  }
 
  if(flag){
    for(int h=0; h<get_aut_n_state(i); h++){
      strcpy(vet[h][h],"1"); 
    }
  }
  for(int h=0; h<get_aut_n_state(i); h++){
    for(int l=0; l<get_aut_n_state(i); l++){
      fout << " " << vet[h][l]; 
    }
    if(h<get_aut_n_state(i)-1)
      fout << " ;\n";
  }
  
  fout << " ]\n";
  for(int j=0; j<get_aut_n_state(i); j++){
    for(int k=0; k<get_aut_n_state(i); k++){	
      if(vet[j][k])
        delete[] vet[j][k];
    }
  }

  for(int j=0; j<get_aut_n_state(i); j++){
    if(vet[j])
      delete[] vet[j];
  }
  
  if(vet)
    delete[] vet;
  return(flag);
}

//------------------------------------------------------------------------------
// print_events - print synchronized event matrix
// i _n - number of synchronized events
// o fout - file
//------------------------------------------------------------------------------
void NET::print_events(fstream &fout, int _n)
{ 
  int m, s, aux, m_s;
  char event[MAX_EVENT_NAME];
  
  get_event_name(_n, event);  
 
  m = s = 0;
  m_s = 1;
  fout << "#event " << _n << "\n#positive\n";
  for(int i=0; i<n_aut; i++){
    aux = positive_event(fout, i, event);
    if(aux==1)
      m++;
    if(aux==2)
      s++;
  }

  if(m>1){
    Semantic_Error5(event, Parser_107);
    exit(0);
  }
    
  if((m==1 && s==0) || m==0 && s>=1){
    Semantic_Error5(event, Parser_106);
    exit(0);
  }
  	
  fout << "#negative\n";
  for(int i=0; i<n_aut; i++){
    if(!negative_event(fout, i, event, m_s))
      m_s = 0;
  }
}

//------------------------------------------------------------------------------
// next_tok - get the nextt token
// i aux - expression
// o buf - token
//------------------------------------------------------------------------------
int NET::next_tok(char *buf, char *aux)
{ 
  int p,x;

  p=0;
  x=0;
  while(aux[0]==' '){ 
    x++;
    aux++;
  }
  while(aux[0]!=' ' && aux[0]!='\0'){
    buf[p] = aux[0];
    aux++;
    p++;
  }
  buf[p] = '\0';
  return(p+x);
}

//------------------------------------------------------------------------------
// put_func - put just in time functions in file
// i i - function number
// i aux - expression
// o fout - file
//------------------------------------------------------------------------------
void NET::put_func(fstream &fout, int i, char *expr)
{ 
  ///char tok[MAX_EXP_SIZE];
  char *aux, buf[MAX_EXP_SIZE];
  char stack[MAX_TABLE_SIZE][MAX_FUNC_NAME];
  int index, count=0;
  ///int p;
  comp_errors error = err_0000;

  double as=1;  
  aux = expr;
  index = 0;

  while(aux[0]!='\0' || buf[0]!='\0' || error==err_0000){
   aux +=  next_tok(buf,aux);
   switch (buf[0]) {
   case '!' : if(buf[1] == '='){ 
                if(buf[2] == '\0'){
                  fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                       << " != " << stack[index-1] << ");\n";           	
                  index--;
                }
                else 
                  error = err_7001; 
              }
              else{
                if(buf[1] == '\0') { 
                  fout << "  " << stack[index-1] << " = rp(!" << stack[index-1] << ");\n";           	
                }
                else 
                  error = err_7003;
              }
              break;
   case '%' : switch (buf[1]) {
              case '#' : fout << "  rp var" << count << " = t_r";
                         aux +=  next_tok(buf,aux);
                         fout << buf << "[p.st(" << buf << ")];\n";
                         strcpy(stack[index],buf);
                         aux +=  next_tok(buf,aux);
                         strcpy(stack[index+1],buf);
                         for(int i=(atoi(stack[index])+1); i<=(atoi(stack[index+1])); i++){
                           fout << "  var" << count << " *= t_r" << i << "[p.st(" << i << ")];\n";
                         }
                         sprintf(stack[index], "var%d", count);
                         count++;
                         index++;
                         break;
              case '$' : fout << "  rp var" << count << " = t_r";
                         aux +=  next_tok(buf,aux);
                         fout << buf << "[p.st(" << buf << ")];\n";
                         strcpy(stack[index],buf);
                         aux +=  next_tok(buf,aux);
                         strcpy(stack[index+1],buf);
                         for(int i=(atoi(stack[index])+1); i<=(atoi(stack[index+1])); i++){
                           fout << "  var" << count << " += t_r" << i << "[p.st(" << i << ")];\n";
                         }
                         sprintf(stack[index], "var%d", count);
                         count++;
                         index++;
                         break;
              case '0' : case '1' : case '2' :
              case '3' : case '4' : case '5' :
              case '6' : case '7' : case '8' :
              case '9' : fout << "  rp var" << count << " = t_r" << buf+1 << "[p.st(" 
                              << buf+1 << ")];\n";
                         sprintf(stack[index], "var%d", count);
                         index++;
                         count++;
                         break;
              case 'P' : if (buf[2] == '\0') 
                           error = err_7008;
                         else {
                           fout << "  rp var" << count << " = 1.0;\n";
                           strcpy(stack[index],buf+2);
                           aux +=  next_tok(buf,aux);
                           strcpy(stack[index+1],buf);
                           aux +=  next_tok(buf,aux);
                           strcpy(stack[index+2],buf);
                           for(int i=atoi(stack[index+1]); i<=atoi(stack[index+2]); i++){
                             fout << "  if(" << stack[index] << " == p.st(" << i << "))\n" 
                                  << "    var" << count << " *= t_r" << i << "[p.st(" 
                                  << i << ")];\n";
                           }
                           sprintf(stack[index], "var%d", count);
                           count++;
                           index++;
                         }
                         break;
              case 'S' : if (buf[2] == '\0') 
                           error = err_7008;
                         else {
                           fout << "  rp var" << count << " = 0.0;\n";
                           strcpy(stack[index],buf+2);
                           aux +=  next_tok(buf,aux);
                           strcpy(stack[index+1],buf);
                           aux +=  next_tok(buf,aux);
                           strcpy(stack[index+2],buf);
                           for(int i=atoi(stack[index+1]); i<=atoi(stack[index+2]); i++){
                             fout << "  if(" << stack[index] << " == p.st(" << i << "))\n" 
                                  << "    var" << count << " += t_r" << i << "[p.st(" 
                                  << i << ")];\n";
                           }
                           sprintf(stack[index], "var%d", count);
                           count++;
                           index++;
                         }
                         break;
              default  : err_7001; }
              break;
   case '&' : if(buf[1] == '&') { 
                if (buf[2] == '\0') {
                  fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                       << " && " << stack[index-1] << ");\n";           	
                  index--;
                }
                else 
                  error = err_7001; 
              }
              else 
                error = err_7004;
              break;
   case '*' : case '+' :
   case '/' : if (buf[1] == '\0') { 
                fout << "  " << stack[index-2] << " = " << stack[index-2] << " " << buf[0] 
                     << " " << stack[index-1] << ";\n";
                index--;
              }
              else 
                error = err_7001;
              break;
   case '-' : switch(buf[1]){
   	         case '\0': fout << "  " << stack[index-2] << " = " << stack[index-2] 
   	                         << " - " << buf[0] << stack[index-1] << ";\n";
                           index--;
                           break;
              
                case '>': if (buf[2] == '\0') {
                           fout << "  " << stack[index-2] << " = ( " << stack[index-2] 
                                << " && " << stack[index-1] << ") || !(" << stack[index-2]
                                << ");\n";
                          }
                          else 
                            error = err_7001; 
                          break;
                default: fout << "  rp var" << count << " = " << buf << ";\n";
                         sprintf(stack[index],"var%d",count);
                         count++;
                         index++;
              }
              break;
   case '0' : case '1' : case '2' : case '3' : case '4' :                   // 'x'
   case '5' : case '6' : case '7' : case '8' :
   case '9' : fout << "  rp var" << count << " = " << buf << ";\n";
              sprintf(stack[index],"var%d",count);
              count++;
              index++;
              break;
   case '<' : switch (buf[1]) {
              case '\0' : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                               << " < " << stack[index-1] << ");\n";           	
                          index--;  
                          break;
              case '-'  : if (buf[2] == '>') { 
                            if (buf[3] == '\0') {
                              fout << "  " << stack[index-2] << " = ( " << stack[index-2] 
                                   << " && " << stack[index-1] << ") || (!(" 
                                   << stack[index-2] << ") && !(" << stack[index-1] << "));\n";
                            }
                            else 
                              error = err_7001; 
                          }
                          else error = err_7023;
                          break;
              case '='  : if (buf[2] == '\0'){ 
                            fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                                 << " <= " << stack[index-1] << ");\n";           	
                            index--;  
                          }
                          else 
                            error = err_7001;
                          break;
              case '>'  : if (buf[2] == '\0') { 
                            fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                                 << " != " << stack[index-1] << ");\n";           	
                            index--;  
                          }
                          else 
                            error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '=' : switch (buf[1]) {
              case '\0' : error = err_7007;
                          break;
              case '='  : if (buf[2] == '\0') {
                            fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                                 << " == " << stack[index-1] << ");\n";           	
                            index--;  
              	        }
                          else 
                            error = err_7001;
                          break;
              case '<'  : if (buf[2] == '\0') {
                            fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                                 << " <= " << stack[index-1] << ");\n";           	
                            index--;  
                          }
                          else 
                            error = err_7001;
                          break;
              case '>'  : if (buf[2] == '\0') {
                            fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                                 << " >= " << stack[index-1] << ");\n";           	
                            index--;  
                          }
                          else 
                            error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '>' : switch (buf[1]) {
              case '\0' : fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                               << " > " << stack[index-1] << ");\n";           	
                          index--;  
                          break;
              case '='  : if (buf[2] == '\0') {
             	           fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                                 << " >= " << stack[index-1] << ");\n";           	
                           index--;  
                          }
                          else 
                            error = err_7001;
                          break;
              default   : error = err_7001; }
              break;
   case '@' : switch (buf[1]) {
              case '$' : if(buf[2] == '\0') 
                           error = err_7008;
                         else {
                           fout << "  rp var" << count << " = 0;\n  for(int i=";
                           strcpy(stack[index],buf+2);
                           aux +=  next_tok(buf,aux);
                           fout << buf << "; i<=";
                           aux +=  next_tok(buf,aux);
                           fout << buf << "; i++){\n    if(p.st(i)==" << stack[index] << "){\n"
                                << "      var" << count << "++;\n    }\n  }\n";
                           sprintf(stack[index], "var%d", count);
                           count++;
                           index++;
                         }
                         break;
              case '0' : case '1' : case '2' :
              case '3' : case '4' : case '5' :
              case '6' : case '7' : case '8' :
              case '9' : fout << "  rp var" << count << " = p.st(" << buf+1 << ");\n";
                         sprintf(stack[index], "var%d", count);
                         index++;
                         count++;
                         break;
              default  : error = err_7008; }
              break;
   case '^' : if(buf[1] == '^') { 
                if(buf[2] == '\0') { 
                  fout << "  " << stack[index-2] << " = rp(" << stack[index-2] << " xor " 
                       << stack[index-1] << ");\n";
                  index--;  
                }
                else 
                  error = err_7001; }
              else 
                error = err_7006;
              break;
   case 'm' : if (buf[3] != '\0')
                error = err_7001;
              else{
                if ((buf[1] == 'i') && (buf[2] == 'n')) {                   // 'min'
                  fout << "  if(" << stack[index-1] << " < " << stack[index-2] << "){\n"
                       << "    " << stack[index-2] << " = " << stack[index-1] << ";\n  }\n";
                  index--;
                }
                else{
                  if ((buf[1] == 'a') && (buf[2] == 'x')) {                // 'max'
                    fout << "  if(" << stack[index-1] << " > " << stack[index-2] << "){\n"
                         << "    " << stack[index-2] << " = " << stack[index-1] << ";\n  }\n";
                    index--;
                  }
                  else 
                    error = err_7002;
                }
              }
              break;
   case '|' : if(buf[1] == '|'){ 
                if(buf[2] == '\0'){ 
                  fout << "  " << stack[index-2] << " = rp(" << stack[index-2] 
                      << " || " << stack[index-1] << ");\n";           	
                  index--;  
                }
                else 
                  error = err_7001;
              }
              else 
                error = err_7006;
              break;
   default  : error = err_7020;
   }
  }
  fout << "  return(" << stack[0] << ");\n";  
}

//------------------------------------------------------------------------------
// put2file_des - put the network to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_des(char *_path)
{
  char path[MAX_NAME];
  fstream fout;

  sprintf(path,"%s.des", _path);
  
  fout.open(path, ios::out);

  fout << "#type " << get_net_model() << endl;
  fout << "#automata " << get_net_n_aut() << endl;
  fout << "#events " << count_events() << endl; 
  fout << "#sizes " << get_net_n_aut() << "   ";
  for(int i=0; i<get_net_n_aut(); i++)
    fout << get_aut_n_state(i) << " ";
  fout << endl;
  fout << "#locals\n";

  for(int i=0; i<get_net_n_aut(); i++){
    put_local(fout, i);
  }
  for(int i=0; i<count_events(); i++){
    print_events(fout, i);
  }
  fout.close();
  cout << " :-) file '" << path << "' saved\n";
  return(1);
}

//------------------------------------------------------------------------------
// put2file_dic - put the dictionary to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_dic(char *_path)
{
  char aux[MAX_EXP_SIZE];
  char path[MAX_NAME];
  fstream fout;

  sprintf(path,"%s.dic", _path);
  
  fout.open(path, ios::out);

  get_net_name(aux);
  fout << "#model   " << aux << endl;
  
  fout << "\n#reach_func   \" ";
  reach.get_exp(0,aux);
  fout << aux << " \" " << endl;
  
  fout << "\n#automata " << get_net_n_aut() << "   ";
  for(int i=0; i<get_net_n_aut(); i++){
    fout << get_aut_n_state(i) << " ";
  }
  fout << endl;

  fout << "\n#events " << count_events() << endl; 
  
  fout << "\n#functions " << ident.get_n_exp() << endl; 
  
  fout << "\n#results " << result.get_n_exp() << endl; 

  fout << "\n#automata" << endl; 
  for(int i=0; i<get_net_n_aut(); i++){
    get_aut_name(i, aux);
    fout << i << "  " << aux << endl;
  }

  fout << "\n#events" << endl; 
  for(int i=0; i<count_events(); i++){
    get_event_name(i, aux);
    fout << i << "  " << aux << endl;
  }
  
  fout << "\n#states" << endl;
  for(int i=0; i<get_net_n_aut(); i++){
    for(int j=0; j<get_aut_n_state(i); j++){
      get_state_name(i, j, aux);
      fout << i << "   " << j << "  " << aux << endl;
    }
  }

  fout << "\n#functions" << endl; 
  for(int i=0; i<ident.get_n_exp(); i++){
    ident.get_name(i, aux);
    fout << i << "   " << aux << "   \"  ";
    ident.get_exp(i,aux);
    fout << aux << "  \"" << endl;
  }

  fout << "\n#results" << endl; 
  for(int i=0; i<result.get_n_exp(); i++){
    result.get_name(i, aux);
    fout << i << "   " << aux << "   \"  ";
    result.get_exp(i,aux);
    fout << aux << "  \"" << endl;
  }
  fout.close();
  cout << " :-) file '" << path << "' saved\n";
  return(1);
}

//------------------------------------------------------------------------------
// put2file_fct - put the reachability function to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_fct(char *_path)
{
  char aux[MAX_EXP_SIZE];
  char path[MAX_NAME];
  fstream fout;

  sprintf(path,"%s.fct", _path);
  
  fout.open(path, ios::out);

  fout << "#domain " << get_net_n_aut() << "   ";
  for(int i=0; i<get_net_n_aut(); i++)
    fout << get_aut_n_state(i) << " ";
  fout << endl;

  fout << "#function " << endl << endl;
  
  get_reach_exp(aux);
  
  fout << aux << " ;"; 
  
  fout.close();
  cout << " :-) file '" << path << "' saved";
  return(1);
}

//------------------------------------------------------------------------------
// put2file_tft - put the functions to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_tft(char *_path)
{
  char aux[MAX_EXP_SIZE];
  char path[MAX_NAME];
  fstream fout;

  sprintf(path,"%s.tft", _path);
  
  fout.open(path, ios::out);

  fout << "#domain " << get_net_n_aut() << "   ";
  for(int i=0; i<get_net_n_aut(); i++)
    fout << get_aut_n_state(i) << " ";
  fout << endl;

  fout << "#functions " << get_ident_n_exp() << endl << endl;

  for(int i=0; i<get_ident_n_exp(); i++){
    ident.get_exp(i,aux);
    fout << "F" << i << " : " << aux << "  ;\n";
  }
 
  fout << "\n#rewards" << endl;
  for(int i=0; i<get_net_n_aut(); i++){
    for(int j=0; j<get_aut_n_state(i); j++){
      fout << i << "   " << j << " :  " << get_state_reward(i, j) << endl;
    }
    fout << endl;
  }
  fout << endl;
  fout.close();
  cout << " :-) file '" << path << "' saved\n";  
  return(1);
}

//------------------------------------------------------------------------------
// put2file_res - put the result functions to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_res(char *_path)
{
  char aux[MAX_EXP_SIZE];
  char path[MAX_NAME];
  fstream fout;

  sprintf(path,"%s.res", _path);
  
  fout.open(path, ios::out);

  fout << "#domain " << get_net_n_aut() << "   ";
  for(int i=0; i<get_net_n_aut(); i++)
    fout << get_aut_n_state(i) << " ";
  fout << endl;

  fout << "#functions " << get_result_n_exp() << endl << endl;
  
  for(int i=0; i<get_result_n_exp(); i++){
    result.get_exp(i,aux);
    fout << "F" << i << " : " << aux << " ;\n";
  }
  fout.close();
  cout << " :-) file '" << path << "' saved\n";  
  return(1);
}

//------------------------------------------------------------------------------
// put2file_jit - put the just in time functions in a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_jit(char *_path)
{
  char path[MAX_NAME];
  char exp[MAX_EXP_SIZE];

  fstream fout;

  sprintf(path,"jit/%s.C", _path);
  
  fout.open(path, ios::out);
  fout << "#include \"../../src/lb/sanfile.h\"\n";
  fout << "extern \"C\" {\n";

  for(int i=0; i<get_net_n_aut(); i++){
    fout << "rp t_r" << i << "[" << get_aut_n_state(i) <<  "] ={";
    for(int j=0; j<get_aut_n_state(i)-1; j++){
      fout << get_state_reward(i, j) << ", ";
    }
    fout << get_state_reward(i, get_aut_n_state(i)-1) << "};\n";
  }
  fout << endl;

  for(int i=0; i<get_ident_n_exp(); i++){
    fout << "rp F" << i << "(const aut_st &p)\n{\n";
    ident.get_exp(i, exp);
    put_func(fout, i, exp);
    fout << "}\n";
  }
  fout << "}\n";  
  for(int i=0; i<get_result_n_exp(); i++){
    fout << "rp R" << i << "(const aut_st &p)\n{\n";
    result.get_exp(i, exp);
    put_func(fout, i, exp);
    fout << "}\n";
  
  }
  
  fout.close();
  cout << " :-) file '" << path << "' saved\n";  
  return(1);
}
