%{
//====================================================================================//
//                                                                                    //
//                                Lexical  Analyzer                                   //
//                                                                                    //
//====================================================================================//
//  This File:   tokens.l                    Tool: lex                                //
//  Software:    PEPS                        Advisor: Paulof@inf.pucrs.br             //
//  Doc:                                                                              //
//====================================================================================//
//  Creation:    28/Feb/02                   by: lbrenner@inf.pucrs.br                //
//  Last Change: 05/Jun/02                   by: lbrenner@inf.pucrs.br                //
//====================================================================================//
//  This file has the definitions of the tokens that must to be recognized by the     //
// grammar contained in the "glc.y" file. These definitions are used by LEX tool,     //
// which is a generator of lexical parsers.                                           //
//  The lexical analyzer (scanner) is a function that reads an input stream and       //
// returns the correspondents tokens.                                                 //
//====================================================================================//

#include "sanfile.h"

extern int yynerrs;  // Variable of the YACC generator to count the number of errors.
extern int yylineno;

//====================================================================================// 
//  Below are defined the R.E.(Regular Expressions) that defines the format of the    //
// digits (symbols between 0 and 9, inclusively), integers, reals, exponents and      //
// identifies. The symbol "+" indicate one or more occurrences, the symbol "*"        //
// indicate zero or more occurrences and the symbol "?" indicate that the occurrence  //
// is optional.                                                                       //
//====================================================================================//
  
%}

digit    [0-9]
integer  {digit}+
real     {integer}"."{integer}
exponent ({integer}|{real})[Ee][+-]?{integer}
id       [A-Za-z_][A-Za-z0-9_]*

%%

"//"[0-9A-Za-z\-\!\"\#\$\%\&\(\)\*\+\,\-\.\/:\;\<\>\=\?\@\\\]\[\}\{\^\_\~\| ]*
"identifiers"     {return(TK_IDENTIFIERS);}
"events"          {return(TK_EVENTS);}
"network"         {return(TK_NETWORK);}
"aut"             {return(TK_AUTOMATON);}
"stt"             {return(TK_STATE);}
"to"              {return(TK_TRANS);}
"from"            {return(TK_FROM);}
"loc"             {return(TK_LOC);}
"syn"             {return(TK_SYN);}
"results"         {return(TK_RESULTS);}
"continuous"      {return(TK_CONTINUOUS);}
"discrete"        {return(TK_DISCRETE);}
"reachability"    {return(TK_REACH);}  
"partial"         {return(TK_PARTIAL);}  
"min"             {return(TK_MIN);}
"max"             {return(TK_MAX);}
"st"              {return(TK_STATE_NUM);}
"nb"              {return(TK_NBR);}
"rw"              {return(TK_REWARD);}
"sum_rw"          {return(TK_SUM_RW);}
"prod_rw"         {return(TK_PROD_RW);}
{id}              {return(TK_ID);}
{integer}         {return(TK_INTEGER);}
{real}            {return(TK_REAL);}
{exponent}        {return(TK_EXPONENT);}
"@"               {return(TK_STATE_NUM);}
"%"               {return(TK_REWARD);}        
"+"               {return(TK_PLUS);}
"++"              {return(TK_PLUS_PLUS);}
"->"              {return(TK_SIMPLE);}
"-"               {return(TK_MINUS);}
"--"              {return(TK_MINUS_MINUS);}
"*"               {return(TK_MULT);}
">="              {return(TK_MAJOR_EQUAL);}
"=>"              {return(TK_MAJOR_EQUAL);}
">"               {return(TK_MAJOR);}
"<->"             {return(TK_DOUBLE);}
"<="              {return(TK_MINOR_EQUAL);}
"=<"              {return(TK_MINOR_EQUAL);}
"<"               {return(TK_MINOR);}
"!="              {return(TK_UNEQUAL);}
"!"               {return(TK_NOT);}
"=="              {return(TK_EQUAL);}
"&&"              {return(TK_AND);}
"||"              {return(TK_OR);}
"^"               {return(TK_XOR);}
"/"               {return(TK_DIV);}
"{"               {return(TK_LEFT_BRACKET);}
"}"               {return(TK_RIGHT_BRACKET);}
"("               {return(TK_LEFT_PARENTHESIS);}
")"               {return(TK_RIGHT_PARENTHESIS);}
"["               {return(TK_LEFT_SQUARE_BRACKET);}
"]"               {return(TK_RIGHT_SQUARE_BRACKET);}
";"               {return(TK_SEMICOLON);}
","               {return(TK_COMMA);}
"="               {return(TK_ATTRIB);}
":"               {return(TK_COLON);}
"."               {return(TK_POINT);}
".."              {return(TK_2DOTS);}
"..."              {return(TK_DOTS);}
[ \t]*            {/* Ignore the blanks, tabulates and new line */
                  }
[\n]              {yylineno++;}     
.                 { /* If neither token return, means that the input 
                       stream has a lexical error */
                   yynerrs++;
                   Lexical_Error(yynerrs, yylineno, yytext);}
%%

int yywrap ()
{
  return(1); 
}

void go2init()
{
  yy_init = 1;
}
