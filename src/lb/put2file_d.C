//====================================================================================//
//                                                                                    //
//                                   Network Structure                                //
//                                                                                    //
//====================================================================================//
//  This File:   put2file_d.C                Tool: C++                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    22/Apr/03                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 22/Apr/03                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This class is used to store the data obtained from SAN textual input file in the  //
// network form.                                                                      //
//  This file implements the functions to put the data structure in the auxiliar files//
// as descriptor, functions and dictionary of the class NET. This file implements the //
// same funtions that the put2file.C file, but to the discrete time.                  //
//====================================================================================//

#include "sanfile.h"

//------------------------------------------------------------------------------
// mount_s - Mount the compatible events (discrete time)
//------------------------------------------------------------------------------
int NET::mount_s(int n_ev)
{
  int n=2;
  int n1,ind,flag;
  int **c_table;
  int **aux_table;
  

  // monta tabela com a combinaçao de eventos
  n = (int) pow((double)2, n_ev);

  c_table = new int*[n];
  aux_table = new int*[n];
  
  for(int i=0;i<n;i++){
    c_table[i] = new int[n_ev];
    aux_table[i] = new int[n_aut];
  }
  
  n1 = n;
  for(int i=0;i<n_ev;i++){//para cada evento
    for(int j=0;j<(n/n1);j++){//numero de vezes
      for(int k=0;k<(n/((n/n1)*2));k++){//numero de preenchimentos em 0
        c_table[(j*n1+k)][i] = 0;
      }
      for(int k=0;k<(n/((n/n1)*2));k++){//numero de preenchimentos em 1
        c_table[((j*n1)+(n1/2)+k)][i] = 1;        
      }
    }
    n1 = n1/2;
  }
  
/*  for(int i=0;i<16;i++){//para cada evento
     cout << i << " [";
     for(int j=0;j<n_ev;j++){//para cada evento
       cout << c_table[i][j] << ", ";
     }
     cout << "]\n";
  }
*/  
  // procura eventos compativeis
  ind = 0;
  for(int i=1;i<n;i++){ // analiza cada combinação
    for(int j=0;j<n_aut;j++) // inicializa posição da tabela com eventos locais
      aux_table[ind][j] = 0;
    for(int j=0;j<n_ev;j++){ // para cada evento
      if(c_table[i][j]){ // faz parte da combinação
        for(int k=0;k<n_aut;k++){
          aux_table[ind][k]+=e_table[j][k];
        }   
      }
    }
    flag = 1;
    for(int j=0;j<n_aut;j++){    
      if(aux_table[ind][j]>1)
        flag = 0;
    }
    if(flag){// achou eventos compativeis
      for(int j=0;j<n_ev;j++){ // para cada evento
        if(c_table[i][j]){ // faz parte da combinação
          for(int k=0;k<n_aut;k++){
            if(e_table[j][k]){
              aux_table[ind][k]=j+1; // coloca o indice do evento
            }
          }   
        }
      }    
      ind++;
    }
  }

  n_s_table = ind;
  s_table = new int*[n_s_table];
  for(int i=0;i<n_s_table;i++){
    s_table[i] = new int[n_aut];
    for(int j=0; j<n_aut; j++)
      s_table[i][j] = aux_table[i][j];
  }

  for(int i=0;i<n_s_table;i++){//para cada evento
     cout << i << " [";
     for(int j=0;j<n_aut;j++){//para cada evento
       cout << s_table[i][j] << ", ";
     }
     cout << "]\n";
  }

  for(int i=0;i<n;i++){
    if(c_table[i])
      delete [] c_table[i];
    if(aux_table[i])
      delete [] aux_table[i];
  }
  
  if(c_table)
    delete [] c_table;
  if(aux_table)
    delete [] aux_table;
  
  return(ind);
}

//------------------------------------------------------------------------------
// count_events_d - Count synchronized events (discrete time)
//------------------------------------------------------------------------------
int NET::count_events_d()
{
  int ind = 0, count = 0;
  char **dic;
  char aux[MAX_EVENT_NAME],aux2[MAX_EVENT_NAME];
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
           ind++;
           count++;
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
           ind++;
           count++;
          }
        }
      }
    }
  }  

  count = count * 2; //cada evento tem o seu evento complementar
  n_e_table = count;
  e_table = new int*[n_e_table];
  for(int i=0;i<n_e_table;i++){
    e_table[i] = new int[n_aut];
    for(int j=0;j<n_aut;j++){
      e_table[i][j] = 0;
    }
  }
  
  for(int c=0; c<n_e_table/2; c++){
	strcpy(aux,dic[c]);    
    for(int i=0; i<n_aut; i++){
      for(int j=0; j<get_aut_n_state(i); j++){
        for(int k=0; k<get_state_n_trans(i, j); k++){
          for(int l=0; l<get_event_n_mst(i, j, k); l++){
            get_mst_name(i, j, k, l, aux2);
            if(!strcmp(aux,aux2)){
              e_table[c][i] = 1;
            }
          }
          for(int l=0; l<get_event_n_sync(i, j, k); l++){
            get_sync_name(i, j, k, l, aux2);
            if(!strcmp(aux,aux2)){
              e_table[c][i] = 1;
            }
          }
        }
      }
    }
  }
  
  // copia cada evento para um evento complementar
  for(int c=0; c<n_e_table/2; c++){
//    cout << "\nevento " << c << " [";
    for(int i=0; i<n_aut; i++){
      e_table[(n_e_table/2)+c][i] = e_table[c][i];
//      cout << e_table[c][i] << ", ";
    }
  }

  // a verificação dos eventos pode ser feita sobre a e_table
  
  for(int i=0;i<MAX_TABLE_SIZE;i++){
    delete [] dic[i];
  }
  delete [] dic;

  if(n_e_table)
    count = mount_s(n_e_table);//number of compatible events
  return(count);
}

//------------------------------------------------------------------------------
// put_false_local - print false local matrix (discrete time)
// i fout - file 
// i aut - automaton number
//------------------------------------------------------------------------------
void NET::put_false_local(fstream &fout, int _aut)
{
  ///char pos[MAX_EXP_SIZE];
  ///char neg[MAX_EXP_SIZE];
  ///char aux[MAX_EXP_SIZE];
  ///int i_aux;
  ///int flag, flag2;
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
      fout << " 0"; 
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
// put_local_d - print local matrix (discrete time)
// i fout - file 
// i aut - automaton number
//------------------------------------------------------------------------------
void NET::put_local_d(fstream &fout, int _aut)
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
          strcpy(pos, aux);
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
  	strcpy(pos, neg);
  	sprintf(neg, "1 %s - ", pos);
      if(get_ident_exp(neg)>=0){
        sprintf(vet[i],"F%d", get_ident_exp(neg));        
      }
      else{
        put_ident_name("new_local");
        i_aux = put_ident_exp(neg);
        sprintf(vet[i],"F%d", i_aux);              
      }
    }
    else{
      strcpy(vet[i],"1");
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
// positive_c_event - print positive complementar event
// o fout - file
// i i - automaton number
// i _event - event name
//------------------------------------------------------------------------------
int NET::positive_c_event(fstream &fout, int i, char *_event)
{ 
  char pos[MAX_EXP_SIZE];
  char aux[MAX_EXP_SIZE];
  char e_name[MAX_EVENT_NAME];
  int i_aux, i_dst, flag, first, m;
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
            m = 1;
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
            m = 0;
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
      if(m){	
        strcpy(aux,pos);
        sprintf(pos, "1 %s -", aux);
      }
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
  return(1);
}

//------------------------------------------------------------------------------
// negative_c_event - print negative complementar matrix
// o fout - file
// i i - automaton number
// i _event - event name
// i m_s - master or slave
//------------------------------------------------------------------------------
int NET::negative_c_event(fstream &fout, int i, char *_event, int m_s)
{ 
  char pos[MAX_EXP_SIZE];
  char aux[MAX_EXP_SIZE];
  char e_name[MAX_EVENT_NAME];
  int i_aux, i_dst, flag, first, m;
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
            m = 1;
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
            m = 0;
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
      if(m){	
        strcpy(aux,pos);
        sprintf(pos, "1 %s -", aux);
      }
      if(m_s){
        strcpy(aux,pos);
        sprintf(pos, "%s -1 *", aux);
      }
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
  return(1);
}

//------------------------------------------------------------------------------
// print_events_d - print compatible events matrix
// i _n - number of compatible event
// o fout - file
//------------------------------------------------------------------------------
void NET::print_events_d(fstream &fout, int _n)
{ 
  ///int aux;
  int m_s;
  char event[MAX_EVENT_NAME];
  
  fout << "#event " << _n+1 << "\n#positive\n";
  cout << "#event " << _n << "\n#positive\n";
  m_s = 1;
  for(int i=0; i<n_aut; i++){
    if(s_table[_n][i]){
      if(s_table[_n][i]<=(n_e_table/2)){
        cout << s_table[_n][i]-1 << " 1\n";
        get_event_name(s_table[_n][i]-1, event);  
        positive_event(fout, i, event); // mesmo da versão continua
      }
      else{
        cout << (s_table[_n][i]-(n_e_table/2)-1) << " 2\n";
        get_event_name((s_table[_n][i]-(n_e_table/2)-1), event);  
        positive_c_event(fout, i, event);
      }
    }
    else{
      put_local_d(fout, i);     
    }
  }

  fout << "#negative\n";
  for(int i=0; i<n_aut; i++){
    if(s_table[_n][i]){
      if(s_table[_n][i]<=(n_e_table/2)){
        cout << s_table[_n][i]-1 << " 3\n";
        get_event_name(s_table[_n][i]-1, event);  
        negative_event(fout, i, event, m_s);
        m_s = 0;
      }
      else{
        cout << (s_table[_n][i]-(n_e_table/2)-1) << " 4\n";
        get_event_name((s_table[_n][i]-(n_e_table/2)-1), event);  
        negative_c_event(fout, i, event, m_s);
        m_s = 0;
      }
    }
    else{
      negative_event(fout, i, "@", 1);
    }
  }
}

//------------------------------------------------------------------------------
// put2file_des_d - put the network to discrete time to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_des_d(char *_path)
{
  char path[MAX_NAME];
  fstream fout;

  sprintf(path,"%s.des", _path);
  
  fout.open(path, ios::out);
  count_events_d();
  
  fout << "#type " << get_net_model() << endl;
  fout << "#automata " << get_net_n_aut() << endl;
  fout << "#events " << n_s_table+1 << endl; 
  fout << "#sizes " << get_net_n_aut() << "   ";
  for(int i=0; i<get_net_n_aut(); i++)
    fout << get_aut_n_state(i) << " ";
  fout << endl;
  fout << "#locals\n";

  for(int i=0; i<get_net_n_aut(); i++){ // matrizes locais, não utilizadas
    put_false_local(fout, i);
  }

  fout << "#event " << 0 << "\n#positive\n";
  for(int i=0; i<get_net_n_aut(); i++){ // matrizes locais, reais
    put_local_d(fout, i);
  }
  fout << "#negative\n";
  for(int i=0; i<get_net_n_aut(); i++){ // ajuste das matrizes locais
    put_false_local(fout, i);
  }
  
  for(int i=0; i<n_s_table+1; i++){
    print_events_d(fout, i);
  }
  fout.close();
  cout << " :-) file '" << path << "' saved\n";
  return(1);
}

//------------------------------------------------------------------------------
// put2file_dic_d - put the dictionary (discrete time) to a file
// i path - file name
//------------------------------------------------------------------------------
int NET::put2file_dic_d(char *_path)
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

  fout << "\n#events " << n_s_table+1 << endl; 
  
  fout << "\n#functions " << ident.get_n_exp() << endl; 
  
  fout << "\n#results " << result.get_n_exp() << endl; 

  fout << "\n#automata" << endl; 
  for(int i=0; i<get_net_n_aut(); i++){
    get_aut_name(i, aux);
    fout << i << "  " << aux << endl;
  }

  fout << "\n#events" << endl; 
  for(int i=0; i<n_s_table+1; i++){
//    get_event_name(i, aux);
    fout << i << "  s_" << i << endl;
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
