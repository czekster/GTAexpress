#include "grammar_.h"

void GRAMMAR_::reach_blk()
{
  char exp[MAX_EXP_SIZE];

  cout << " Compiling reachability function\n";
  if (get_tk() != TK_PARTIAL)
    put_tk();
  
  if (get_tk() != TK_REACH)
    Syntactic_error();
  if (get_tk() != TK_ATTRIB)
    Syntactic_error("=");
  exp_blk("reachability");
  if (tour == 1) 
    func.put_id("reachability");
  func.build_exp("reachability"); 
  func.get_exp("reachability", exp);
  if (tour == 2){
    reach.put_exp("reachability", exp);
    if (!reach.check_reach("reachability"))
      Function_semantic_error(Parser_210, "reachability", 400);
  }
  if (get_tk() != TK_SEMICOLON)
    Syntactic_error(";");
}
