#include "grammar_.h"

int GRAMMAR_::get_ln()
{
  string  sline;

  if (fin.eof()){
    return(0);
  }
  getline(fin,sline);
  if (cline)
    delete [] cline;
  cline = new char[sline.length()+1];
  strcpy(cline,sline.c_str());
  pos = 0;
  lineno++;
  return(1);
}

int GRAMMAR_::classify_id()
{
  if (!strcmp(tk,"at"))            return(TK_AUT);
  if (!strcmp(tk,"aut"))           return(TK_AUTOMATON);
  if (!strcmp(tk,"continuous"))    return(TK_CONTINUOUS);
  if (!strcmp(tk,"discrete"))      return(TK_DISCRETE);
  if (!strcmp(tk,"div"))           return(TK_DIVI);
  if (!strcmp(tk,"events"))        return(TK_EVENTS);
  if (!strcmp(tk,"from"))          return(TK_FROM);
  if (!strcmp(tk,"identifiers"))   return(TK_IDENTIFIERS);
  if (!strcmp(tk,"loc"))           return(TK_LOC);
  if (!strcmp(tk,"max"))           return(TK_MAX);  
  if (!strcmp(tk,"min"))           return(TK_MIN);
  if (!strcmp(tk,"mod"))           return(TK_MOD);
  if (!strcmp(tk,"nb"))            return(TK_NBR);
  if (!strcmp(tk,"network"))       return(TK_NETWORK);
  if (!strcmp(tk,"partial"))       return(TK_PARTIAL);
  if (!strcmp(tk,"prod_rw"))       return(TK_PROD_RW);
  if (!strcmp(tk,"reachability"))  return(TK_REACH);
  if (!strcmp(tk,"results"))       return(TK_RESULTS);
  if (!strcmp(tk,"rw"))            return(TK_REWARD);
  if (!strcmp(tk,"st"))            return(TK_STATE_NUM);          
  if (!strcmp(tk,"std"))           return(TK_STD);
  if (!strcmp(tk,"sts"))           return(TK_STS);
  if (!strcmp(tk,"stt"))           return(TK_STATE);
  if (!strcmp(tk,"sum_rw"))        return(TK_SUM_RW);  
  if (!strcmp(tk,"syn"))           return(TK_SYN);
  if (!strcmp(tk,"to"))            return(TK_TRANS);

  return(TK_ID);
}

int GRAMMAR_::classify_number()
{
 int  pos_tk = 0;
 int  class_num = TK_INTEGER;
 
 while (cline[pos] >= '0' && cline[pos] <= '9'){ // integer
   tk[pos_tk] = cline[pos];   
   pos_tk++;
   pos++;
 } // integer
 if ((cline[pos] == '.') && (cline[pos+1] >= '0' && cline[pos+1] <= '9')){
   tk[pos_tk] = cline[pos];   
   pos_tk++;
   pos++;
   while (cline[pos] >= '0' && cline[pos] <= '9'){ // real
     tk[pos_tk] = cline[pos];   
     pos_tk++;
     pos++;
   } // real
   class_num = TK_REAL;
 }
 if ((cline[pos] == 'e' || cline[pos] == 'E') && 
     ((cline[pos+1] >= '0' && cline[pos+1] <= '9') || 
      (cline[pos+1] == '-' || cline[pos+1] == '+'))){ // expo
   tk[pos_tk] = cline[pos];   
   pos_tk++;
   pos++;
   tk[pos_tk] = cline[pos];   
   pos_tk++;
   pos++;
   while (cline[pos] >= '0' && cline[pos] <= '9'){ 
     tk[pos_tk] = cline[pos];   
     pos_tk++;
     pos++;
   } 
   class_num = TK_EXPONENT;
 }// expo
 tk[pos_tk] = '\0';
  
 return(class_num);
}

int GRAMMAR_::get_tk()
{
  int  pos_tk;
  
  if (in_mem){
    in_mem = 0;
    return(TK);
  }
  
  while (true){
    if (new_line) // get a new line
      if (get_ln())
        new_line = 0;
      else{
        TK = 0;  // end of file
        return (TK);
      }

    while (cline[pos] == ' ' || cline[pos] == '\t') pos++;    // skip blankspaces
    if((cline[pos] == '\n') || (cline[pos] == '\0')){              // end of line
      new_line = 1;
    }

    if ((cline[pos] >= 'a' && cline[pos] <= 'z') || // characters (reserved words 
        (cline[pos] >= 'A' && cline[pos] <= 'Z') || //           and identifiers)
        (cline[pos] == '_')){ 
      pos_tk = 0;
      while ((cline[pos] >= 'a' && cline[pos] <= 'z') || 
             (cline[pos] >= 'A' && cline[pos] <= 'Z') || 
             (cline[pos] == '_') || (cline[pos] >= '0' && 
              cline[pos] <= '9')) {
        tk[pos_tk] = cline[pos];
        pos_tk++;
        pos++;
      }
      tk[pos_tk] = '\0';
      TK = classify_id();
      return (TK);
    }
    else{
      if ((cline[pos] >= '0' && cline[pos] <= '9') ||  // numbers (integer, real,
          (cline[pos] == '.' && (cline[pos+1] >= '0' && //          and exponent)
           cline[pos+1] <= '9'))){ 
        TK = classify_number();
        return (TK);
      }  
      else{                               // classify simbols and ignore comments
        switch (cline[pos]){
          case '(': strcpy(tk,"("); pos++; 
                    TK = TK_LEFT_PARENTHESIS;
                    return (TK);
          case ')': strcpy(tk,")"); pos++; 
                    TK = TK_RIGHT_PARENTHESIS;
                    return (TK);
          case '[': strcpy(tk,"["); pos++; 
                    TK = TK_LEFT_SQUARE_BRACKET;
                    return (TK);
          case ']': strcpy(tk,"]"); pos++; 
                    TK = TK_RIGHT_SQUARE_BRACKET;
                    return (TK);
          case ';': strcpy(tk,";"); pos++; 
                    TK = TK_SEMICOLON;
                    return (TK);
          case ',': strcpy(tk,","); pos++; 
                    TK = TK_COMMA;
                    return (TK);
          case '^': strcpy(tk,"^"); pos++;
                    TK = TK_XOR;
                    return (TK);
          case '&': if (cline[pos+1] == '&'){
                      strcpy(tk,"&&"); pos+=2; 
                      TK = TK_AND;
                      return (TK);
                    }
                    else {
                      strcpy(tk,"&"); pos++;  
                      Lexical_error();
                    }
                    break;
          case '|': if (cline[pos+1] == '|'){
                      strcpy(tk,"||"); pos+=2; 
                      TK = TK_OR;
                      return (TK);
                    }
                    else {
                      strcpy(tk,"|"); pos++;  
                      Lexical_error();
                    }
                    break;
          case '.': if (cline[pos+1] == '.'){
                      if (cline[pos+2] == '.'){
                        strcpy(tk,"..."); pos+=3; 
                      }
                      else{
                        strcpy(tk,".."); pos+=2; 
                      }
                      TK = TK_DOTS;
                      return (TK);
                    }
                    else {
                      strcpy(tk,"."); pos++;  
                      Lexical_error();
                    }
                    break;
          case '=': switch (cline[pos+1]){
                      case '=': strcpy(tk,"=="); pos+=2; 
                                TK = TK_EQUAL;
                                return (TK);
                      case '<': strcpy(tk,"=<"); pos+=2; 
                                TK = TK_MINOR_EQUAL;
                                return (TK);
                      case '>': strcpy(tk,"=>"); pos+=2; 
                                TK = TK_MAJOR_EQUAL;
                                return (TK);
                      default:  strcpy(tk,"=");  pos++;  
                                TK = TK_ATTRIB;
                                return (TK);
                    }
          case '>': if (cline[pos+1] == '='){
                      strcpy(tk,">="); pos+=2; 
                      TK = TK_MAJOR_EQUAL;
                      return (TK);
                    }
                    else{
                      strcpy(tk,">");  pos++; 
                      TK = TK_MAJOR;
                      return (TK);
                    }
          case '<': switch (cline[pos+1]){
                      case '=': strcpy(tk,"<="); pos+=2; 
                                TK = TK_MINOR_EQUAL;
                                return (TK);
                      case '-': if (cline[pos+2] == '>'){
                                  strcpy(tk,"<->"); pos+=3; 
                                  TK = TK_DOUBLE;
                                  return (TK);
                                }
                      default : strcpy(tk,"<");  pos++; 
                                TK = TK_MINOR;
                                return (TK);
                    }
          case '!': if (cline[pos+1] == '='){
                      strcpy(tk,"!="); pos+=2;
                      TK = TK_UNEQUAL;
                      return (TK);
                    }
                    else {
                      strcpy(tk,"!"); pos++; 
                      TK = TK_NOT;
                      return (TK);
                    }
          case '*': strcpy(tk,"*"); pos++; 
                    TK = TK_MULT;
                    return (TK);
          case '/': switch (cline[pos+1]){
                      case '/': new_line = 1; break;
                      case '*': pos++; 
                                while (true){
                                  pos++;
                                  if (cline[pos] == '*' && cline[pos+1] == '/'){
                                    pos+=2;
                                    break;
                                  }
                                  if (cline[pos+1] == '\0') {
                                    if (!get_ln()){
                                      TK = 0;
                                      return(TK);
                                    }
                                  }
                                }
                                break; 
                      default : strcpy(tk,"/"); pos++; 
                                TK = TK_DIV;
                                return (TK);
                    }
                    break;
          case '-': switch (cline[pos+1]){
                      case '>': strcpy(tk,"->"); pos+=2; 
                                TK = TK_SIMPLE;
                                return (TK);
                      case '-': strcpy(tk,"--"); pos+=2; 
                                TK = TK_MINUS_MINUS;
                                return (TK);
                      default : strcpy(tk,"-");  pos++ ; 
                                TK = TK_MINUS;
                                return (TK);
                    }
          case '+': if (cline[pos+1] == '+'){
                      strcpy(tk,"++"); pos+=2; 
                      TK = TK_PLUS_PLUS;
                      return (TK);
                    }
                    else{
                      strcpy(tk,"+"); pos++; 
                      TK = TK_PLUS;
                      return (TK);
                    }
          case '\0': new_line = 1; 
                     break;
          default: sprintf(tk,"%c",cline[pos]); pos++;  
                   Lexical_error();        
        }
      }
    }
  }
}

int GRAMMAR_::put_tk()
{
  in_mem = 1;
  return -1; //cz: had to put this, because linux doesnt like it
}
