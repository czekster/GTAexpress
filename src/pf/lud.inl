//====================================================================================//
//                                                                                    //
//                       LU decomposed Descriptor Structure                           //
//                                                                                    //
//====================================================================================//
//  This File:   lud_inl.h                  Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 5500                                                                 //
//====================================================================================//
//  Creation:    22/jun/96                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 22/oct/97                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//

     //--------------------------------------------------------------------------//
     //                            Empty Constructor                             //
     //--------------------------------------------------------------------------//
inline LUD::LUD()
{
 strcpy(file_name, "\0");
 automata   = 0;
 events     = 0;
 syn_term = no_ev;
 loc_term = fst_aut;
 pm_term  = Mat_plus;
}
     //--------------------------------------------------------------------------//
     //                            Sized Constructor                             //
     //--------------------------------------------------------------------------//
inline LUD::LUD(const aut_id a, const ev_id e)
{
 automata   = a;
 events     = e;
 alloc();
 syn_term = no_ev;
 loc_term = fst_aut;
 pm_term  = Mat_plus;
}
     //--------------------------------------------------------------------------//
     //                             Copy Constructor                             //
     //--------------------------------------------------------------------------//
inline LUD::LUD(const LUD & d)
{
 automata   = d.automata;
 events     = d.events;
 alloc();
 trans(d);
}
     //--------------------------------------------------------------------------//
     //                                Destructor                                //
     //--------------------------------------------------------------------------//
inline LUD::~LUD()
{
 if (automata != 0) {
   desal();
   automata   = 0;
   events     = 0;
 }
 strcpy(file_name, "\0");
}

     //--------------------------------------------------------------------------//
     //                           Empty Intialization                            //
     //--------------------------------------------------------------------------//
inline void LUD::create()
{
 strcpy(file_name, "\0");
 automata   = 0;
 events     = 0;
 syn_term = no_ev;
 loc_term = fst_aut;
 pm_term  = Mat_plus;
}
     //--------------------------------------------------------------------------//
     //                           Sized Intialization                            //
     //--------------------------------------------------------------------------//
inline void LUD::create(const aut_id a, const ev_id e)
{
 erase();
 automata   = a;
 events     = e;
 alloc(); 
 syn_term = no_ev;
 loc_term = fst_aut;
 pm_term  = Mat_plus;
}
     //--------------------------------------------------------------------------//
     //                       Copy and Sized Intialization                       //
     //--------------------------------------------------------------------------//
inline void LUD::copy(const LUD & d)
{
 erase();
 automata   = d.automata;
 events     = d.events;
 alloc();
 trans(d);
}
     //--------------------------------------------------------------------------//
     //                               Destruction                                //
     //--------------------------------------------------------------------------//
inline void LUD::erase()
{
 if (automata != 0) {
   desal();
   automata  = 0;
   events    = 0;
   strcpy(file_name, "\0");
 }
}

     //--------------------------------------------------------------------------//
     //                          It assigns a file name                          //
     //--------------------------------------------------------------------------//
inline void LUD::baptise(const peps_name file)
{
 strcpy(file_name, file);
}
     //--------------------------------------------------------------------------//
     //                         It returns the file name                         //
     //--------------------------------------------------------------------------//
inline void LUD::name(peps_name file) const
{
 strcpy(file, file_name);
}

     //--------------------------------------------------------------------------//
     //                    The number of automata of the model                   //
     //--------------------------------------------------------------------------//
inline int LUD::size() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //                    The number of automata of the model                   //
     //--------------------------------------------------------------------------//
inline aut_id LUD::aut() const
{
 return(automata);
}
     //--------------------------------------------------------------------------//
     //                    The number of synchronizing events                    //
     //--------------------------------------------------------------------------//
inline ev_id LUD::ev() const
{
 return(events);
}
     //--------------------------------------------------------------------------//
     //                         The state space product                          //
     //--------------------------------------------------------------------------//
inline int LUD::ss() const
{
 return(sizes.state_space());
}
     //--------------------------------------------------------------------------//
     //                         The number of valid terms                        //
     //--------------------------------------------------------------------------//
inline int LUD::v_terms() const
{
 int answer = 0;

 for (aut_id a=0; a<automata; a++)
   if (l_valid[a])
     answer++;
 for (ev_id e=0; e<events; e++) {
   if (s_valid[Mat_plus][e])
     answer++;
   if (s_valid[Mat_minus][e])
     answer++;
 }
 return(answer);
}
     //--------------------------------------------------------------------------//
     //                      The number of automata is zero                      //
     //--------------------------------------------------------------------------//
inline bool LUD::is_empty() const
{
 return(automata == 0);
}
     //--------------------------------------------------------------------------//
     //            The regularization performed during the generation            //
     //--------------------------------------------------------------------------//
inline reg_typ LUD::regular() const
{
 return(regular_type);
}
     //--------------------------------------------------------------------------//
     //          It sets the "term of the turn" to the first valid term          //
     //--------------------------------------------------------------------------//
     //    The possible values of the "term of the turn" variables ('syn_term',  //
     // 'loc_term' and 'pm_term') and their corresponding semantics are:         //
     //                                                                          //
     // +--------+--------+---------+------------------------------------------+ //
     // |syn_term|loc_term| pm_term |               indicates                  | //
     // +--------+--------+---------+------------------------------------------+ //
     // | no_ev  | fst_aut|Mat_plus | the first local term                     | //
     // | no_ev  |   i    |Mat_plus | the i+1th local term                     | //
     // | fst_ev | no_aut |Mat_plus | the occurence term (+) of the first event| //
     // | fst_ev | no_aut |Mat_minus| the adj. term (-) of the first event     | //
     // |   i    | no_aut |Mat_plus | the occurence term of the i+1th event    | //
     // |   i    | no_aut |Mat_minus| the adjustement term of the i+1 th event | //
     // +--------+--------+---------+------------------------------------------+ //
     //                                                                          //
     // Note: the automata and the events are numbered from 0 to 'automata'-1    //
     // and 0 to 'events'-1, respectively.                                       //
     //--------------------------------------------------------------------------//
inline void LUD::reset_turn()
{
 syn_term = no_ev;
 loc_term = fst_aut;
 pm_term  = Mat_plus;
 if (turn_is_not_valid()) {
   next_turn();
 }
}
     //--------------------------------------------------------------------------//
     //           It sets the "term of the turn" to the next valid term          //
     //--------------------------------------------------------------------------//
     //  The terms are visited in the order:                                     //
     //                                                                          //
     //     - first local term;                                                  //
     //         ...                                                              //
     //     - last local term;                                                   //
     //     - the occurence (positive) term of the first synchronizing event;    //
     //     - the adjustement (negative) term of the first synchronizing event;  //
     //         ...                                                              //
     //     - the occurence (positive) term of the last synchronizing event;     //
     //     - the adjustement (negative) term of the last synchronizing event;   //
     //--------------------------------------------------------------------------//
inline void LUD::next_turn()
{
 do {
   if (syn_term == no_ev) {        // if it points to a local term
     loc_term++;                      // points to the next local term
     if (loc_term == automata) {      // if it was the last local term
       if (events != 0) {                // if there are synchronizing events
         syn_term = fst_ev;                 // points to the occurence term
         loc_term = no_aut;                 //    of the first synchronizing event
       }
       else {                            // if there is no synchronizing event
         loc_term = fst_aut;                // points to the first local term
       }
     }
   }
   else {                          // if it points to a synchronizing event term
     if (pm_term == Mat_plus) {       // if it points to an occurence term
       pm_term = Mat_minus;              // points to the adjustment term
     }
     else {                           // if it points to an adjustement term
       pm_term = Mat_plus;               // points to the occurence term of
       syn_term++;                       //    the next synchronizing event
       if (syn_term == events) {         // if it was the last synchronizing event
         syn_term = no_ev;                  // points to the first local term
         loc_term = fst_aut;
       }
     }
   }
 } while (turn_is_not_valid());
}
    //--------------------------------------------------------------------------//
     //                           Internal Function                              //
     //--------------------------------------------------------------------------//
     //         It returns 'true' if the "term of the turn" is not valid         //
     //--------------------------------------------------------------------------//
inline bool LUD::turn_is_not_valid()
{
 bool answer;

 if (syn_term == no_ev)
   answer = !(l_valid[loc_term]);
 else
   answer = !(s_valid[pm_term][syn_term]);
 return(answer);
}
