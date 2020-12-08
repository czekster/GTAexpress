//====================================================================================//
//                                                                                    //
//                            Class SAN Element List                                  //
//                                                                                    //
//====================================================================================//
//  This File:   elemlist.h                 Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 3200                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 14/feb/00                        by: Paulo.Fernandes@imag.fr         //
//====================================================================================//
//                                                                                    //
//     A SAN element list is an array of SAN matrix entries (class 'elem') . This     //
// array has a maximum size (variable 'dimension') and a current size (variable       //
// 'number').                                                                         //
//                                                                                    //
//  Note: If the capacity of the list is exceeded a programming error is produced.    //
//====================================================================================//

class elem_list
{
public:
 inline elem_list();                                                // Empty Constructor
 inline elem_list(const int dim);                                   // Sized Constructor
 inline elem_list(const elem_list & l);                              // Copy Constructor
 inline ~elem_list();                                                      // Destructor

 inline void create();                                            // Empty Intialization
 inline void create(const int dim);                               // Sized Intialization
 inline void copy(const elem_list & l);                  // Copy and Sized Intialization
 inline void erase();                                                     // Destruction

 inline int  size() const;                     // Maximum number of elements in the list
 inline bool is_empty() const;                                 // No element in the list
 inline bool is_full() const;                             // No empty places in the list
 inline int  has() const;                      // Current number of elements in the list
 inline int  has_not() const;              // Current number of empty places in the list
 inline elem pos(const int p) const;                     // The p-th element of the list

 inline void take_out();                        // It removes all elements from the list

        void tail(const elem e);          // It puts element 'e' at the tail of the list
        void tail(const func_id f);      // It puts function 'f' at the tail of the list
        void tail(const rp c);           // It puts constant 'c' at the tail of the list
        void head(const elem e);          // It puts element 'e' at the head of the list

        void change(const int p, const elem e);
           // It puts element 'e' as the p-th element of the list, erasing the old value

        void exchange(const int p, const int q);
                                              // Exchanges the p-th and the q-th element
        void sort();           // Sorts the list according to:
                                    //   - the functional elements with increasing id's;
                                    //   - the constant elements with increasing values;

        bool operator == (const elem_list & l) const;
                                              // It compares if 'this' and 'l' are equal

        void write(ofstream & fout) const;               // Output on 'fout' file stream
        void read(ifstream & fin);                       // Input from 'fin' file stream
        int  mem_use() const;                  // Memory space used by 'this' (in bytes)

        //--------------------------------------------------------------------------//
        //                          Data Structures                                 //
        //--------------------------------------------------------------------------//
private: int    dimension;       // Maximum number of elements in the list
         int    number;          // Current list size
         elem * list;            // array of elements (list information)
};

