#include "grammar_.h"

void GRAMMAR_::res_blk()
{
  char id[MAX_ID_NAME];
  char exp[MAX_EXP_SIZE];

  cout << " Compiling results block\n";
  if (get_tk() != TK_RESULTS)
    if (TK == 0)
      return;
    else
      Syntactic_error();
  get_tk();
  while (TK == TK_ID){
    strcpy(id, tk);
    if (get_tk() != TK_ATTRIB)
      Syntactic_error("=");
    exp_blk(id);
    if (tour==1) 
        func.put_id(id);
    func.build_exp(id); 
    func.get_exp(id, exp);
    if (tour == 2){
      result.put_exp(id, exp);
      if (!result.check_rate(id))
        Result_semantic_error(Parser_211, id, 500);
    }
    if (get_tk() != TK_SEMICOLON)
      Syntactic_error(";");
    get_tk();
  }
  put_tk();
  return;
}
