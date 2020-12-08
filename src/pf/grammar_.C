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
#include "grammar_.h"

//------------------------------------------------------------------------------
// Contructor 
//------------------------------------------------------------------------------
GRAMMAR_::GRAMMAR_()
{
  pos         = 0;
  in_mem      = 0;
  new_line    = 1;
  tour        = 1;
  TK          = 0;
  tk[0]       = '\0';
  cline       = NULL;

  name[0]     = '\0';
  type        = CONTINUOUS;
  n_aut       = 0;
  aut         = NULL;

  lineno      = 0;
  errorno     = 0;
}

//------------------------------------------------------------------------------
// Destructor 
//------------------------------------------------------------------------------
GRAMMAR_::~GRAMMAR_()
{
  pos         = 0;
  in_mem      = 0;
  new_line    = 0;
  tour        = 0;
  TK          = 0;
  tk[0]       = '\0';
  if (cline)
    delete [] cline;
  if (fin.is_open())
    fin.close();

  name[0]     = '\0';
  type        = CONTINUOUS;
  n_aut       = 0;
  if (aut)
    delete [] aut;

  aut         = NULL;

  lineno      = 0;
  errorno     = 0;
}

//------------------------------------------------------------------------------
// reset - Reset all auxiliar variables to the initial value 
//------------------------------------------------------------------------------
void GRAMMAR_::reset()
{
  pos      = 0;
  in_mem   = 0;
  new_line = 1;
  tour     = 1;
  TK       = 0;
  tk[0]    = '\0';
  if (cline)
    delete [] cline;

  cline    = NULL;
  lineno   = 0;
  errorno  = 0;

  fin.clear(); 
  fin.seekg(0, ios::beg);
}

//------------------------------------------------------------------------------
//  Open_File (to input)
//------------------------------------------------------------------------------
void GRAMMAR_::open_file(const peps_name file, ifstream & f_tmp)
{
 f_tmp.open(file);
 if (!f_tmp){
   cerr << "System Error - Cannot open the file '" << file << "' (#### 0001 ####)";
   exit (0);
 }
 else
   f_tmp.precision(peps_precision);
}

//------------------------------------------------------------------------------
//  Open_File (to output)
//------------------------------------------------------------------------------
void GRAMMAR_::open_file(const peps_name file, ofstream & f_tmp)
{
 f_tmp.open(file);
 if (!f_tmp){
   cerr << "System Error - Cannot open the file '" << file << "' (#### 0002 ####)";
   exit (0);
 }
 else
   f_tmp.precision(peps_precision);
}

//------------------------------------------------------------------------------
//  Close_File (to input) 
//------------------------------------------------------------------------------
void GRAMMAR_::close_file(const peps_name file, ifstream & f_tmp)
{
 f_tmp.clear();
 f_tmp.close();
 if (!f_tmp){
   cerr << "System Error - Cannot close the file '" << file << "' (#### 0004 ####)";
   exit (0);
 }
}

//------------------------------------------------------------------------------
//  Close_File (to output) 
//------------------------------------------------------------------------------
void GRAMMAR_::close_file(const peps_name file, ofstream & f_tmp)
{
 f_tmp.close();
 if (!f_tmp){
   cerr << "System Error - Cannot close the file '" << file << "' (#### 0005 ####)";
   exit (0);
 }
 else
   cout << " :-) file '" << file << "' saved\n";
}

//------------------------------------------------------------------------------
// compile - compile a san model from a textual format
// in: file - model file
//------------------------------------------------------------------------------
void GRAMMAR_::compile(const peps_name file)
{
  open_file(file, fin);
 
  cout << "Start model compilation\n";
  cout << "First Passage\n";
  id_blk();
  ev_blk();
  reach_blk();
  net_blk();
  res_blk();
  
  cout << "Creating automata and states structures\n";
  create_aut_stt();
  reset();

  tour = 2;

  cout << "Second Passage\n";
  id_blk();
  ev_blk();
  reach_blk();
  for (int i=0; i<n_aut; i++)
    aut[i].info_ev(edic.get_n_ev());
  cout << "Writing events informations\n";
  net_blk();
  res_blk();
  close_file(file, fin);
  
  cout << "Checking events integrity\n";  
  edic.check_ev();
  cout << "Model compiled\n";
  
}

//------------------------------------------------------------------------------
// put2file_des - put the network to a file
// out: file - model file
//------------------------------------------------------------------------------
void GRAMMAR_::put2file_des(const peps_name file)
{
///  char path[max_peps_name];
  ofstream fout;

  open_file(file, fout);

  fout << "#type " << type << endl;
  fout << "#automata " << n_aut << endl;
  fout << "#events " << edic.get_n_synch_ev() << endl; 
  fout << "#sizes " << n_aut << "   ";
  for(int i=0; i<n_aut; i++)
    fout << ndic.get_n_stt(i) << " ";
  fout << endl;
  fout << "#locals\n";
  
  for(int i=0; i<n_aut; i++){
    fout << "[";
    write_local_events(fout, i);
    fout << " ]\n";
  }
  
  for(int i=0; i<edic.get_n_synch_ev(); i++){
    write_synch_event(fout, i);
  }
  close_file(file, fout);
}


//------------------------------------------------------------------------------
// put2file_dic - put the dictionary to a file
// out: file - model file
//------------------------------------------------------------------------------
void GRAMMAR_::put2file_dic(const peps_name file)
{
  char aux[MAX_EXP_SIZE];
  ofstream fout;

  open_file(file, fout);

  fout << "#model   " << name << endl;
  
  fout << "\n#reach_func   \" ";
  reach.get_exp(0,aux);
  fout << aux << " \" " << endl;
  
  fout << "\n#automata " << n_aut << "   ";
  for(int i=0; i<n_aut; i++)
    fout << ndic.get_n_stt(i) << " ";
  fout << endl;

  fout << "\n#events " << edic.get_n_synch_ev() << endl; 
  
  fout << "\n#functions " << ident.get_n_func() << endl; 
  
  fout << "\n#results " << result.get_n_func() << endl; 

  fout << "\n#automata" << endl; 
  for(int i=0; i<n_aut; i++){
    ndic.get_aut_name(i, aux);
    fout << i << "  " << aux << endl;
  }

  fout << "\n#events" << endl; 
  for(int i=0; i<edic.get_n_synch_ev(); i++){
    edic.get_ev_id(edic.get_real_synch_ev(i), aux);
    fout << i << "  " << aux << endl;
  }
  
  fout << "\n#states" << endl;
  for(int i=0; i<n_aut; i++){
    for(int j=0; j<ndic.get_n_stt(i); j++){
      ndic.get_stt_name(i, j, aux);
      fout << i << "   " << j << "  " << aux << endl;
    }
  }

  fout << "\n#functions" << endl; 
  for(int i=0; i<ident.get_n_func(); i++){
    ident.get_name(i, aux);
    fout << i << "   " << aux << "   \"  ";
    if (ident.get_exp_to_file(i,aux))
      fout << aux << "  \"" << endl;
    else
      fout << "1  \"" << endl;
  }

  fout << "\n#results" << endl; 
  for(int i=0; i<result.get_n_func(); i++){
    result.get_name(i, aux);
    fout << i << "   " << aux << "   \"  ";
    result.get_exp_to_file(i,aux);
    fout << aux << "  \"" << endl;
  }
  close_file(file, fout);
}

//------------------------------------------------------------------------------
// put2file_fct - put the reachability function to a file
// out: file - model file
//------------------------------------------------------------------------------
void GRAMMAR_::put2file_fct(const peps_name file)
{
  char aux[MAX_EXP_SIZE];
  ofstream fout;

  open_file(file, fout);

  fout << "#domain " << n_aut << "   ";
  for(int i=0; i<n_aut; i++)
    fout << ndic.get_n_stt(i) << " ";
  fout << endl;

  fout << "#function " << endl << endl;

  reach.get_exp_to_file(0,aux);
  
  fout << aux << " ;"; 
  
  close_file(file, fout);
}

//------------------------------------------------------------------------------
// put2file_tft - put the functions to a file
// out: file - model file
//------------------------------------------------------------------------------
void GRAMMAR_::put2file_tft(const peps_name file)
{
  char aux[MAX_EXP_SIZE];
  int pos;
  ofstream fout;

  open_file(file, fout);
  
  fout << "#domain " << n_aut << "   ";
  for(int i=0; i<n_aut; i++)
    fout << ndic.get_n_stt(i) << " ";
  fout << endl;

  fout << "#functions " << ident.get_n_func() << endl << endl;

  for(int i=0; i<ident.get_n_func(); i++){
    pos = 0;
    if (ident.get_exp_to_file(i,aux))
      fout << "F" << i << " : " << aux << "  ;\n";
    else
      fout << "F" << i << " : 1  ; \n";
  }
 
  fout << "\n#rewards" << endl;
  for(int i=0; i<n_aut; i++){
    for(int j=0; j<ndic.get_n_stt(i); j++){
      fout << i << "   " << j << " :  " << aut[i].get_stt_reward(j) << endl;
    }
    fout << endl;
  }
  fout << endl;
  close_file(file, fout);
}

//------------------------------------------------------------------------------
// put2file_res - put the result functions to a file
// out: file - model file
//------------------------------------------------------------------------------
void GRAMMAR_::put2file_res(const peps_name file)
{
  char aux[MAX_EXP_SIZE];
  ofstream fout;

  open_file(file, fout);
  
  fout << "#domain " << n_aut << "   ";
  for(int i=0; i<n_aut; i++)
    fout << ndic.get_n_stt(i) << " ";
  fout << endl;

  fout << "#functions " << result.get_n_func() << endl << endl;
  
  for(int i=0; i<result.get_n_func(); i++){
    result.get_exp_to_file(i,aux);
    fout << "F" << i << " : " << aux << " ;\n";
  }
  close_file(file, fout);
}

//------------------------------------------------------------------------------
// print - print a readable version of the model structure
//------------------------------------------------------------------------------
void GRAMMAR_::print()
{
  cout << "Model name: " << name << "\ttype: " << type << endl;
  cout << "Automata: " << n_aut << endl;
  for (int i=0; i<n_aut; i++){
    aut[i].print();
    cout << endl;  
  }
}

//------------------------------------------------------------------------------
// create_aut_stt - create the automata and states structure
//------------------------------------------------------------------------------
void GRAMMAR_::create_aut_stt()
{
  char aut_name[MAX_ID_NAME];

  n_aut = ndic.get_n_aut();
//  cout << "automata: " << n_aut << endl;
  aut = new AUTOMATON_[n_aut];
  for (int i=0; i<n_aut; i++){
    ndic.get_aut_name(i, aut_name);
    aut[i].put_name(aut_name);
    aut[i].create_stt(ndic.get_n_stt(i));
//    aut[i].info_ev(edic.get_n_ev());
//    cout << "automaton: " << i << " states: " << ndic.get_n_stt(i) << endl;
  }
  edic.put_n_aut(n_aut);
}

//------------------------------------------------------------------------------
// put_model_name - put the model name
// in: _id - model name
//------------------------------------------------------------------------------
void GRAMMAR_::put_model_name(char *_id)
{
  strcpy(name, _id);
}

//------------------------------------------------------------------------------
// put_model_type - put the model type. It must be CONTINUOUS or DISCRETE
// in: _type - model type
//------------------------------------------------------------------------------
void GRAMMAR_::put_model_type(int _type)
{
  type = _type;
}

//------------------------------------------------------------------------------
// cut_dom_id - extract from an identifier with replic (domain) only the domain
//              identifier
// in:  _id  - identifier with replic
// out: _dom - domain identifier
//------------------------------------------------------------------------------
void GRAMMAR_::cut_dom_id(char *_id, char *_dom)
{
 int i=0;
 int j=0;
 int flag=0;

 while (i< MAX_ID_NAME){
   if (_id[i] == ']')
     break;
   if (flag == 1){
     _dom[j] = _id[i];
     j++;
   }
   if (_id[i] == '['){
     _id[i] = '\0';
     flag = 1;
   }
   i++;
 }
 _dom[j] = '\0';
}

//------------------------------------------------------------------------------
// write_local_events - write the local events of an automata in a file
// in:  _aut - automaton index
// out: fout - file handler
//------------------------------------------------------------------------------
void GRAMMAR_::write_local_events(ofstream &fout, int _aut)
{
  char new_func[MAX_ID_NAME];
///  char old_func[MAX_ID_NAME];
  char new_exp[MAX_EXP_SIZE];
  char old_exp[MAX_EXP_SIZE];

  for(int j=0; j<ndic.get_n_stt(_aut); j++){
    for(int k=0; k<ndic.get_n_stt(_aut); k++){                       // positive
      new_func[0] = '\0';
      new_exp[0] = '\0';
      if (k != j){      
        for (int ev=0; ev < edic.get_n_ev(); ev++){ 
          if (edic.get_ev_type(ev) == LOC && aut[_aut].get_ev(j, k, ev) > -2){
            if (aut[_aut].get_ev(j, k, ev) == -1){ 
              if (new_exp[0] == '\0'){
                sprintf(new_func, "_F%d", edic.get_ev_rate(ev));
                ident.get_exp(edic.get_ev_rate(ev), new_exp);
              }
              else{
                sprintf(new_func, "%s_F%d", new_func, edic.get_ev_rate(ev));
                ident.get_exp(edic.get_ev_rate(ev), old_exp);
                sprintf(new_exp, "%s %s +", new_exp, old_exp);
              }
            }
            else{
              if (new_exp[0] == '\0'){
                sprintf(new_func, "_F%dP%d", edic.get_ev_rate(ev), aut[_aut].get_ev(j, k, ev));
                ident.get_exp(edic.get_ev_rate(ev), new_exp);
                ident.get_exp(aut[_aut].get_ev(j, k, ev), old_exp);
                sprintf(new_exp, "%s %s *", new_exp, old_exp);
              }
              else{
                sprintf(new_func, "%s_F%dP%d", new_func, edic.get_ev_rate(ev), aut[_aut].get_ev(j, k, ev));
                ident.get_exp(edic.get_ev_rate(ev), old_exp);
                sprintf(new_exp, "%s %s ", new_exp, old_exp);
                ident.get_exp(aut[_aut].get_ev(j, k, ev), old_exp);
                sprintf(new_exp, "%s %s * +", new_exp, old_exp);
              }
            }
          }
        }
      }
      else{
        for(int l=0; l<ndic.get_n_stt(_aut); l++){ 
          for (int ev=0; ev < edic.get_n_ev(); ev++){ 
            if (edic.get_ev_type(ev) == LOC && aut[_aut].get_ev(j, l, ev) > -2){
              if (aut[_aut].get_ev(j, l, ev) == -1){ 
                if (new_exp[0] == '\0'){
                  sprintf(new_func, "_F%d", edic.get_ev_rate(ev));
                  ident.get_exp(edic.get_ev_rate(ev), new_exp);
                }
                else{
                  sprintf(new_func, "%s_F%d", new_func, edic.get_ev_rate(ev));
                  ident.get_exp(edic.get_ev_rate(ev), old_exp);
                  sprintf(new_exp, "%s %s +", new_exp, old_exp);
                }
              }
              else{
                if (new_exp[0] == '\0'){
                  sprintf(new_func, "_F%dP%d", edic.get_ev_rate(ev), aut[_aut].get_ev(j, l, ev));
                  ident.get_exp(edic.get_ev_rate(ev), new_exp);
                  ident.get_exp(aut[_aut].get_ev(j, l, ev), old_exp);
                  sprintf(new_exp, "%s %s *", new_exp, old_exp);
                }
                else{
                  sprintf(new_func, "%s_F%dP%d", new_func, edic.get_ev_rate(ev), aut[_aut].get_ev(j, l, ev));
                  ident.get_exp(edic.get_ev_rate(ev), old_exp);
                  sprintf(new_exp, "%s %s ", new_exp, old_exp);
                  ident.get_exp(aut[_aut].get_ev(j, l, ev), old_exp);
                  sprintf(new_exp, "%s %s * +", new_exp, old_exp);
                }
              }
            }
          } 
        }
        if (new_func[0] != '\0'){
          sprintf(new_func, "%s_Neg", new_func);
          sprintf(new_exp, "%s -1 *", new_exp);
        }
      }
      if (new_func[0] != '\0'){
        if (ident.has_identical(new_exp) < 0)
          ident.put_exp(new_func, new_exp);
        fout << " F" << ident.has_identical(new_exp);
      }
      else
        fout << " 0";
    }
    if (j < (ndic.get_n_stt(_aut)-1)) 
      fout << " ;\n ";
  }
}

//------------------------------------------------------------------------------
// write_synch_event - write a synchronizing event in a file
// in:  synch_ev - synchronizing event index
// out: fout - file handler
//------------------------------------------------------------------------------
void GRAMMAR_::write_synch_event(ofstream &fout, int synch_ev)
{
  int flag;
  int ev = edic.get_real_synch_ev(synch_ev);
  char new_func[MAX_ID_NAME];
  char new_exp[MAX_EXP_SIZE];
  char old_exp[MAX_EXP_SIZE];
  
  fout << "#event " <<  synch_ev << "\n#positive\n";
  for(int i=0; i<n_aut; i++){
    fout << "[";
    if (edic.is_concerned(ev, i)){
      for(int j=0; j<ndic.get_n_stt(i); j++){
        for(int k=0; k<ndic.get_n_stt(i); k++){
          if (aut[i].get_ev(j, k, ev) > -2){
            if (aut[i].get_ev(j, k, ev) == -1){ 
              if (edic.get_mst_aut(ev)==i){ 
                fout << " F" << edic.get_ev_rate(ev);
              }
              else{
                fout << " 1";
              }  
            }
            else{ 
              if (edic.get_mst_aut(ev)==i){
                sprintf(new_func, "_F%dP%d", edic.get_ev_rate(ev), aut[i].get_ev(j, k, ev));
                ident.get_exp(edic.get_ev_rate(ev), new_exp);
                ident.get_exp(aut[i].get_ev(j, k, ev), old_exp);
                sprintf(new_exp, "%s %s *", new_exp, old_exp);
                if (ident.has_identical(new_exp) < 0)
                  ident.put_exp(new_func, new_exp);
                fout << " F" << ident.has_identical(new_exp);
              }
              else{
                fout << " F" << aut[i].get_ev(j, k, ev);
              }  
            }
          }
          else{
            fout << " 0";
          }
        }
        if (j < (ndic.get_n_stt(i)-1)) 
          fout << " ;\n ";
      }
    }
    else{ 
      for(int j=0; j<ndic.get_n_stt(i); j++){
        for(int k=0; k<ndic.get_n_stt(i); k++){
          j == k ? fout << " 1" : fout << " 0";
        }
        if (j < (ndic.get_n_stt(i)-1)) 
          fout << " ;\n ";
      }
    }
    fout << " ]\n";
  }
  
  fout << "#negative\n";
  for(int i=0; i<n_aut; i++){
    fout << "[";
    if (edic.is_concerned(ev, i)){
      for(int j=0; j<ndic.get_n_stt(i); j++){
        new_func[0] = '\0';
        new_exp[0] = '\0';
        old_exp[0] = '\0';
        flag = 0;
        for(int k=0; k<ndic.get_n_stt(i); k++){ 
          if (aut[i].get_ev(j, k, ev) == -1) 
            flag = 1;
          if (aut[i].get_ev(j, k, ev) >= 0){
            flag = 2;
            break;
          }
        }
        switch (flag){
          case 0: for(int k=0; k<ndic.get_n_stt(i); k++) 
                    fout << " 0";
                  break;
          case 1: for(int k=0; k<ndic.get_n_stt(i); k++){
                    if (j==k){
                      if (edic.get_mst_aut(ev)==i){ 
                        sprintf(new_func, "_Neg_F%d", edic.get_ev_rate(ev));
                        ident.get_exp(edic.get_ev_rate(ev), old_exp);
                        sprintf(new_exp, "%s -1 *", old_exp);
                        if (ident.has_identical(new_exp) < 0)
                          ident.put_exp(new_func, new_exp);
                        fout << " F" << ident.has_identical(new_exp);
                      }
                      else
                        fout << " 1";
                    }
                    else
                      fout << " 0";
                  }
                  break;
          case 2: for(int k=0; k<ndic.get_n_stt(i); k++){ 
//                    if (aut[i].get_ev(j, k, ev) == -1){
//                      new_exp[0] == '\0' ? strcpy(new_exp, "1") : strcat(new_exp, " 1 +");
//                      sprintf(new_func, "%s_1", new_func);
//                    }
                    if (aut[i].get_ev(j, k, ev) >= 0){
                      ident.get_exp(aut[i].get_ev(j, k, ev), old_exp);                    
                      if (new_exp[0] == '\0')
                        strcpy(new_exp, old_exp);
                      else
                        sprintf(new_exp, "%s %s +", new_exp, old_exp);
                      sprintf(new_func, "%s_P%d", new_func, aut[i].get_ev(j, k, ev));
                    }
                  }
                  if (edic.get_mst_aut(ev) == i){
                      ident.get_exp(edic.get_ev_rate(ev), old_exp);                    
                      sprintf(new_exp, "%s %s * -1 *", new_exp, old_exp);
                      sprintf(new_func, "%s_Neg_F%d", new_func, edic.get_ev_rate(ev));
                  }
                  if (ident.has_identical(new_exp) < 0)
                    ident.put_exp(new_func, new_exp);
                  for(int k=0; k<ndic.get_n_stt(i); k++) 
                    j==k ? fout << " F" << ident.has_identical(new_exp)  : fout << " 0";
                  break;
        }
        if (j < (ndic.get_n_stt(i)-1)) 
          fout << " ;\n ";
      }
    }
    else{
      for(int j=0; j<ndic.get_n_stt(i); j++){
        for(int k=0; k<ndic.get_n_stt(i); k++){
          j == k ? fout << " 1" : fout << " 0";
        }
        if (j < (ndic.get_n_stt(i)-1)) 
          fout << " ;\n";
      }
    }
    fout << " ]\n";    
  }
}



