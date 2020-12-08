//============================================================================//
//                                                                            //
//                 INFix TO POStfix translator Structure                      //
//                                                                            //
//============================================================================//
//  This File: inf2pos.C            Language: C++                             //
//  Software:  PEPS                 Advisor: Brigitte.Plateau@imag.fr         //
//============================================================================//
//  Creation: 12/05/97                 by: Roberta.Jungblut-Hessel@imag.fr    //
//  Last Change: 07/Aug/02             by: lbrenner@inf.pucrs.br              //
//============================================================================//
// This class translate operators of infix to postfix form.                   //
//============================================================================//

#include "sanfile.h"


     //==============================================================//
     //                      Constructor                             //
     //==============================================================//
inf2pos::inf2pos()
{
  ind_func = 0;
  ind_op = 0;

}

     //==============================================================//
     //                       Destructor                             //
     //==============================================================//
inf2pos::~inf2pos()
{
/*  for(int i=0; i< ind_func; i++)
    free(func[i]);
  for(int i=0; i< ind_op; i++)
    free(op[i]);*/
}

     //==============================================================//
     //              Reset the vectors                               //
     //==============================================================//
void inf2pos::reset()
{
  for(int i=0; i< ind_func; i++)
    delete [] func[i];

  for(int i=0; i< ind_op; i++)
    delete [] op[i];
  
  ind_func = 0;
  ind_op = 0;
}

     //==============================================================//
     //            Put an element in the func vector                 //
     //==============================================================//
void inf2pos::put_elem(char *elem)
{
  func[ind_func] = strdup(elem);
  ind_func++;
}

     //==============================================================//
     //  Put an operator (in the correct order) in the func vector   //
     //==============================================================//
void inf2pos::put_elem()
{
  func[ind_func] = strdup(op[ind_op-1]);
  ind_func++;
  free(op[ind_op-1]);
  ind_op--;
}

     //==============================================================//
     // Store temporarily an operator, this is necessary to make the //
     // translation                                                  //
     //==============================================================//
void inf2pos::put_op(char *o)
{
  op[ind_op] = strdup(o);
  ind_op++;
}

     //==============================================================//
     //    Return the function already translated to the exterior    //
     //==============================================================//
void inf2pos::get_func(char *fnc)
{
  strcpy(fnc, func[0]);

  for(int i=1; i<ind_func; i++)
    {
      strcat(fnc," ");
      strcat(fnc,func[i]);
    }
}
