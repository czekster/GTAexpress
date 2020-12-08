//====================================================================================//
//                                                                                    //
//                              PEPS Solution Methods                                 //
//                                                                                    //
//====================================================================================//
//  This File:   solve.h                    Language: C++  (xlC and CC)               //
//  Software:    PEPS 3.1                    Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         solve.tex                                                            //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 11/nov/04                        by: Leonardo.Brenner@imag.fr        //
//====================================================================================//

bool Power_CND(VCT & buf, const precond_typ p_type);

bool GMRES_CND(const precond_typ p_type);

bool Arnoldi_CND(const precond_typ p_type);

bool Power_HBF(VCT & buf);

bool Uniformization_CND(VCT & buf, rp _time);
