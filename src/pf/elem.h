//====================================================================================//
//                                                                                    //
//                                Class SAN Element                                   //
//                                                                                    //
//====================================================================================//
//  This File:   elem.h                     Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3100                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change:  7/nov/02                        by: paulof@inf.pucrs.br             //
//====================================================================================//
//                                                                                    //
//     A SAN element is the basic storage object of the descriptor matrix entries.    //
// The SAN element can be a constant ('rate') or a function ('func'), therefore it    //
// has three fields:  - 'type' (rate or function);                                    //
//                    - 'val'  (a float value);                                       //
//                    - 'id'   (the function internal identificator);                 //
//                                                                                    //
//     The constant elements do not use the field 'id'.                               //
//                                                                                    //
//     The functional elements use the field 'id' as indexes for the static structure //
// function table 'FTB::ftb'. The field 'value' is also used for functional elements  //
// to store the current result of the function evaluation.                            //
//                                                                                    //
// Note: Several logical and arithmetic operators are defined for this class in order //
// to manipulate SAN element objects in a "normal" way (like floats).                 //
//====================================================================================//

class elem
{
public:
 inline elem();                                                     // Empty Constructor
 inline elem(const elem_type k, const rp v, const func_id i);     // Defined Constructor
 inline elem(const elem & e);                                        // Copy Constructor
 inline ~elem();                                                           // Destructor

 inline bool is_const() const;                     // The SAN element 'this' is constant
 inline bool is_func() const;                    // The SAN element 'this' is functional

 inline void params(aut_set & s) const;    // It fills 's' with the parameters of 'this'

 inline rp   get(const aut_st & s) const;  // It evaluates 'this' for a global state 's'
 inline rp   get() const;                        // It returns the field 'val' of 'this'
                                       // ****************** Warning *******************
                                       // If 'this' is a function, a previous evaluation
                                         // must be done using either 'eval' or 'f_eval'
 inline func_id f_get() const;                    // It returns the field 'id' of 'this'

 inline void put(const elem_type t, const rp v, const func_id i); // It redefines 'this'

        //--------------------------------------------------------------------------//
        //                     Evaluation of a SAN element                          //
        //--------------------------------------------------------------------------//
        //    The next four functions fill the field 'val' with the evaluation of   //
        // 'this' to a given global state. The first two functions receive the      //
        // global state diretcly in the input parameter 's' (the size of 's' must   //
        // be equal to the number of original automata - defined in 'FTB::ftb').    //
        // The last two functions receive the global state in two input parameters. //
        // The parameter 's' indicates the global state according to the current    //
        // number of automata (defined in 'DSC::dsc' or 'CND::cnd'). To generate    //
        // the global state according to the number of original automata (need to   //
        // the evaluation) an array of state maps 'm' is used.                      //
        //--------------------------------------------------------------------------//
        //            Global state with the number of original automata             //
        //--------------------------------------------------------------------------//
 inline void eval(const aut_st & s);                            // (for any SAN element)
 inline void f_eval(const aut_st & s);                  // (for functional SAN elements)
        //--------------------------------------------------------------------------//
        //             Global state with the current number of automata             //
        //--------------------------------------------------------------------------//
 inline void eval(const aut_st & s, const st_map * m);          // (for any SAN element)
 inline void f_eval(const aut_st & s, const st_map * m); //(for functional SAN elements)
 inline void jit_f_eval(const aut_st & s, const st_map * m);
            // same as previous function, but using fast function evaluation using 'jit'

        //--------------------------------------------------------------------------//
        //                      Estimation of a SAN element                         //
        //--------------------------------------------------------------------------//
        //    This functions fills the 'val' field of 'this' with an estimation     //
        // made by a specific criteria.                                             //
        //                                                                          //
        //    The input parameters are:                                             //
        //      - the coordinates of where this SAN element appears:                //
        //           'e' is the sync. term (the constant 'no_ev' means local term); //
        //           'a' is the automata;                                           //
        //           'i' is the row;                                                //
        //      - the state maps to obtain the original states coordinates ('s');   //
        //      - function estimation police ('t'):                                 //
        //           'lu_user'  - dialogue with the user;                           //
        //           'lu_max'   - largest value;                                    //
        //           'lu_min'   - smallest value;                                   //
        //           'lu_ave'   - average value;                                    //
        //           'lu_zero'  - replace the function by zero;                     //
        //--------------------------------------------------------------------------//
        //  This function has two versions, the first just performs the estimation. //
        // The second version also returns in the output parameter 's' the global   //
        // used to the estimation (this version is only used when the estimation    //
        // police is dialogue with the user ('lu_user').                            //
        //--------------------------------------------------------------------------//
        void estimate(const ev_id e, const aut_id a, const st_id i,
                      const st_map * m, const f_aprox t);
        void estimate(const ev_id e, const aut_id a, const st_id i,
                      const st_map * m, const f_aprox t, aut_st & s);

        //--------------------------------------------------------------------------//
        //   It tries to eliminate a functional element in the process of grouping  //
        //--------------------------------------------------------------------------//
        //   Denote: P   the set of parameters of 'this' (automata);                //
        //           S   the set of automata whose states are known;                //
        //           SP  the intersection of P and S;                               //
        //           R   the set of automata state values for the automata in SP;   //
        //                                                                          //
        //   This function tests if 'this' is a constant on R.                      //
        //                                                                          //
        //   Input parameters: 's' the set S denoted above;                         //
        //                     'v' the set R (restricted to automata in SP);        //
        //                                                                          //
        //   All the possible values in R of the function are computed. If the same //
        // evaluation is found, the SAN element ('this') is changed to a constant   //
        // and the evaluation is stored in 'val'. Otherwise, 'this' is unchanged.   //
        //                                                                          //
        // Note: if 'this' is constant ('type' = 'rate') the function does nothing. //
        //                                                                          //
        // Note: the maximum size of 's' and the size of 'v' must be equal to the   //
        // number of automata in the original model. It means that in 'v' only the  //
        // values of the automata in 's' are relevant.                              //
        //--------------------------------------------------------------------------//
        void solve(const aut_set & s, const aut_st & v);

        //--------------------------------------------------------------------------//
        //                           Assign Operators                               //
        //--------------------------------------------------------------------------//
 inline void operator =  (const rp      v);                       // It assigns a rate
 inline void operator =  (const func_id i);               // It assigns a function id.
 inline void operator =  (const elem    e);          // It assigns another SAN Element

        //--------------------------------------------------------------------------//
        //                          Logical Operators                               //
        //--------------------------------------------------------------------------//
        //   Principles: - Two elements of different types are different;           //
        //               - Less than or Greater than operators compares 'this' to   //
        //               'zero',i.e., 'v' must be equal to 'zero';                  //
        //               - A functional element is greater than zero if and only if //
        //               one of its values is strictly positive. Remember that each //
        //               entry of a rate matrix must be either always positive or   //
        //               always negative (if diagonal element);                     //
        //--------------------------------------------------------------------------//
 inline bool operator == (const rp      v) const;                          // Equality
 inline bool operator == (const elem    e) const;                          // Equality
 inline bool operator != (const rp      v) const;                        // Inequality
 inline bool operator != (const elem    e) const;                        // Inequality
 inline bool operator <  (const rp      v) const;            // Lesser than (negative)
 inline bool operator >  (const rp      v) const;           // Greater than (positive)

        //--------------------------------------------------------------------------//
        //                        Arithmetic Operators                              //
        //--------------------------------------------------------------------------//
        //   The following eight operators perform operations on the SAN elements   //
        // regardless if the elements are constant or functional. To do that these  //
        // functions MUST use the static structures Table of Functions ('FTB::ftb') //
        // and Scratch Table of Functions ('scr_ft::sft'). Basically, the operators //
        // search the function tree object in the original table ('FTB::ftb') and   //
        // store the result into the new table ('scr_ft::sft'), i.e., the operators //
        // applyied to functional elements create new functions which must be       //
        // stored into a new table ('scr_ft::sft').                                 //
        //--------------------------------------------------------------------------//
 inline elem operator +  (const rp   v) const;           // Add: it returns 'this + v'
 inline elem operator +  (const elem e) const;           // Add: it returns 'this + e'
 inline elem operator *  (const rp   v) const;      // Multiply: it returns 'this * v'
 inline elem operator *  (const elem e) const;      // Multiply: it returns 'this * e'
 inline void operator += (const rp   v);          // Acumulator Add:       'this += v'
 inline void operator += (const elem e);          // Acumulator Add:       'this += e'
 inline void operator *= (const rp   v);          // Acumulator Multiply:  'this *= v'
 inline void operator *= (const elem e);          // Acumulator Multiply:  'this *= e'

        //--------------------------------------------------------------------------//
        //                         Array Operators                                  //
        //--------------------------------------------------------------------------//
        //   The following three functions perform operations over the SAN elements //
        // regardless if the elements are constant or functional. To do that these  //
        // functions MUST use the static structures Table of Functions ('FTB::ftb') //
        // and Scratch Table of Functions ('scr_ft::sft'). Basically, the functions //
        // search the function tree object in the original table ('FTB::ftb') and   //
        // store the result into the new table ('scr_ft::sft'), i.e., the operators //
        // applyied to functional elements creates new functions which must be      //
        // stored into a new table ('scr_ft::sft').                                 //
        //                                                                          //
        // Note: The 'sto' function, which just copies a SAN element, copies, when  //
        // 'this' is a function, the corresponding function tree object of the      //
        // table 'FTB::ftb' to the table 'scr_ft::sft'.                             //
        //--------------------------------------------------------------------------//
        void sto(const elem e);     // It assigns another SAN Element  -  'this' = 'e'
        void add(const elem e[], const int n);
                                 // It adds 'n' SAN elements  -  'this' = sum of 'e[]'
        void mul(const elem e[], const int n);
                       // It multiplies 'n' SAN elements  -  'this' = product of 'e[]'

        void print(ofstream & fout) const;                // Print on 'fout' file stream
        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void write() const;                              // Output on cout
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: elem_type type;     // element type ('rate' for constants 'func' for functions)
         func_id   id;       // the internal id of functional elements
         rp        val;      // the numeric value of the constant element or
                             // the current evaluation of the functional element
};


