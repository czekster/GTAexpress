//====================================================================================//
//                                                                                    //
//                          Dictionaire Table Structure                               //
//                                                                                    //
//====================================================================================//
//  This File:   dct.h                      Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//====================================================================================//
//  Creation:    19/feb/98                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//  This class is used to define the PEPS main structure Function Table ('DCT::dct'). //
//                                                                                    //
//     Identifiers for:                                                               //
//          - the SAN;                                                                //
//          - each automata;                                                          //
//          - each event;                                                             //
//          - states of each automata;                                                //
//          - general functions (functions used in descriptor);                       //
//          - integration functions;                                                  //
//====================================================================================//

class DCT
{
public: DCT();                                                      // Empty Constructor
        DCT(const aut_st & s, const ev_id e, const int gf, const int sf);
                                                                    // Sized Constructor
        DCT(const DCT & t);                                          // Copy Constructor
        ~DCT();                                                            // Destructor

        void create();                                            // Empty Intialization
        void create(const aut_st & s, const ev_id e, const int gf, const int sf);
                                                                  // Sized Intialization
        void copy(const DCT & t);                        // Copy and Sized Intialization
        void erase();                                                     // Destruction

        bool is_empty() const;                         // The number of automata is zero

        void   baptise(const peps_name file);                  // It assigns a file name
        void   name(peps_name file) const;                   // It returns the file name
        int    size() const;                                   // The number of automata
        aut_id aut() const;                                    // The number of automata
        ev_id  ev() const;                         // The number of synchronizing events
        void   a_sz(aut_st & s) const;          // The number of states of each automata
        int    gfs() const;                           // The number of general functions
        int    sfs() const;                       // The number of integration functions

                                          // Those functions saves the name of:
        void n_bapt(const peps_name name);                          // the network (SAN)
        void a_bapt(const aut_id a, const peps_name name);              // automaton 'a'
        void e_bapt(const ev_id e, const peps_name name);                   // event 'e'
        void s_bapt(const aut_id a, const st_id s, const peps_name name);
                                                           // state 's' of automaton 'a'
        void gf_bapt(const func_id f, const peps_name name);     // general function 'f'
        void sf_bapt(const func_id f, const peps_name name); // integration function 'f'
                                          // Those functions saves the expressions of:
        void r_dexp(const peps_expr expr);                      // reachability function
        void gf_dexp(const func_id f, const peps_expr expr);     // general function 'f'
        void sf_dexp(const func_id f, const peps_expr expr); // integration function 'f'

                                         // Those functions returns the name of
        void n_name(peps_name name) const;                          // the network (SAN)
        void a_name(const aut_id a, peps_name name) const;              // automaton 'a'
        void e_name(const ev_id e, peps_name name) const;                   // event 'e'
        void s_name(const aut_id a, const st_id s, peps_name name) const;
                                                           // state 's' of automaton 'a'
        void gf_name(const func_id f, peps_name name) const;     // general function 'f'
        void sf_name(const func_id f, peps_name name) const; // integration function 'f'
                                          // Those functions returs the expressions of:
        void r_eexp(peps_expr expr) const;                      // reachability function
        void gf_eexp(const func_id f, peps_expr expr) const;     // general function 'f'
        void sf_eexp(const func_id f, peps_expr expr) const; // integration function 'f'

        void print() const;         // It prints a readable dictionary in 'dct.dbg' file
        void show() const;   // It shows a readable version of the dictionaire on 'cout'
        void show_states();                  // It shows the local state names on 'cout'
        void write() const;                              // Output on 'fout' file stream
        void read(const peps_name file);                 // Input from 'fin' file stream


static   DCT dct;                     // static variable: The Dictionaire Table

private: peps_name    file_name;      // the file name of this structure
         aut_st       automata;       // the number of automata and their size
         ev_id        events;         // the number of synchronizing events
         int          g_funcs;        // the number of general functions
         int          s_funcs;        // the number of integration functions
         peps_name    san_name;       // the name of the SAN (given in .san file)
         peps_name  * aut_names;      // names of the automata
         peps_name  * ev_names;       // names of synchronizing events
         peps_name ** st_names;       // names of the local states
         peps_name  * gf_names;       // names of general functions
         peps_name  * sf_names;       // names of integration functions

         peps_name    reach_func;     // the expression of the reachability function
         peps_name  * gf_exp;         // the expressions of the general functions
         peps_name  * sf_exp;         // the expressions of the integration functions

         void alloc();                                // Internal Function of allocation
         void desal();                             // Internal Function of desallocation
};
