//====================================================================================//
//                                                                                    //
//                            Class Probability Vector                                //
//                                                                                    //
//====================================================================================//
//  This File:   vct2.C                     Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Error Codes: 2400                                                                 //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 13/feb/02                        by:     Anne.Benoit@imag.fr         //
//====================================================================================//
#include "peps.h"


     //--------------------------------------------------------------------------//
     //         Multiply by a HBF   -   'this' = 'v' + ('v' * 'HBF::hbf')        //
     //--------------------------------------------------------------------------//
     // Implementation:                                                          //
     //     - for all columns ('j');                                             //
     //       - it initializes the resulting element ('vec[j]') with the element //
     //       with the element of original vector ('v.vec[j]');                  //
     //       - it multiplies each nonzero element by the corresponding vector   //
     //       'v' element, adding to the resulting element of the vector 'this'; //
     //--------------------------------------------------------------------------//
void VCT::mult_HBF(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2401)
  if (HBF::hbf.order() != dimension)
    Programming_Error("bad sized HBF", 2402)
#endif
  int k;                           // counter for the nonzero elements of the HBF matrix

  for (int j=0; j<HBF::hbf.dimension; j++) {                  // for all columns
    vec[j] = v.vec[j];                                          // initializes
    for (k=HBF::hbf.columns[j]; k<HBF::hbf.columns[j+1]; k++)   // multiplies nonzeros
      vec[j] += v.vec[HBF::hbf.rows[k]] * HBF::hbf.elements[k]; // adding to elements
  }
  
}

      //--------------------------------------------------------------------------//
      //      Static variables to the Internal Functions of Multiply by ...       //
      //                                                                          //
      //   The following five variables are defined as static to reduce the cost  //
      // for allocation and release memory structures in the most sensible point  //
      // of the PEPS solvers.                                                     //
      //--------------------------------------------------------------------------//
VCT    VCT::t;       // temporary vector to store the intermediary results
rp   * VCT::z_in;    // input "slice" (a pointer to an array of vectors elements)
rp   * VCT::v_in;    // input "slice" (a pointer to an array of vectors elements) for split
rp   * VCT::z_out;   // output "slice" (a pointer to an array of vectors elements)
aut_st VCT::g_st;    // a global state according to the current descriptor size

vector<gs_pair> VCT::zin;          // input "slice" in sparse format
vector<gs_pair> * VCT::zins;       // array of input  "slice" in sparse format
vector<gs_pair>  VCT::zouts;       // output "slice" in sparse format
//int_pair * VCT::info_zin;        // some informations about the zin
//int_pair * VCT::info_zout;       // some informations about the zout
vector<info_pair> VCT::info2_zin;  // some informations about the zin
vector<info_pair> VCT::info2_zout; // some informations about the zout

aunfs_list *VCT::aunfs;

bool * VCT::used_z;                // the used zin or zout
rp ** VCT::zinf;
int * VCT::indice_r;
int ** VCT::useful_f;
int ** VCT::place;

     //--------------------------------------------------------------------------//
     //       Multiply by a CND  -  Method A  -  'this' = 'v' * 'CND::cnd'       //
     //            this.positions == v.positions (call from mult_CND)            //
     //--------------------------------------------------------------------------//
void VCT::mult_CND_A(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2403)
  if (CND::cnd.ss() != dimension)
    Programming_Error("bad sized CND", 2404)
#endif
    if (is_sparse != v.is_sparse)
        cout << "VCT::mult_CND_A : different vector implementation type\n";

    int k;            // a counter for vector positions

    for (int a = 0; a < CND::cnd.automata; a++) {
        switch (CND::cnd.lf_type[a]) {
            case idnt_mat : //cout << "idnt_mat\n";
                add(v); // The RSS is the same
                break;
            case null_mat : //cout << "null_mat\n"; break;
            case cnst_mat : //cout << "cnst_mat\n";
                mcp(CND::cnd.local_mat[a], v, a);
                break;
            case func_mat : //cout << "func_mat\n";
                mfip(CND::cnd.local_mat[a], CND::cnd.state_map, v, a);
        }
    } 

	//cout << "fin partie locale \n"; //debug();

    // ************* Positive matrices ****************
    for (int e = 0; e < CND::cnd.events; e++) {
        //Part 1/3: for the first matrix...
        int a = CND::cnd.s_D_order[e].aut(0);
        switch (CND::cnd.sf_type[e][a]) {
            case idnt_mat : //cout << "idnt_mat\n";
                if (is_sparse)
                    for (k=0; k<dimension; k++) {
                        t.vec[k] = v.vec[k];
                        t.positions[k] = v.positions[k];
                    }
                else
                    for (k=0; k<dimension; k++)
                        t.vec[k] = v.vec[k];
                break;
            case null_mat : //cout << "null_mat\n";
                if (is_sparse)
                    t.positions[0] = -1;
                else
                    for (k=0; k<dimension; k++)
                        t.vec[k] = zero;
                break;
            case cnst_mat : //cout << "cnst_mat\n";
                t.mcs(CND::cnd.synch_mat[e][a], v, a);
                break;
            case func_mat : //cout << "func_mat\n";
                t.mfis(CND::cnd.synch_mat[e][a],CND::cnd.state_map, v, a);
        }
        //Part 2/3: for the second, until the last minus 1...
        for (int i = 1; i < CND::cnd.automata - 1; i++) {
            a = CND::cnd.s_D_order[e].aut(i);
            switch (CND::cnd.sf_type[e][a]) {
                case idnt_mat : //cout << "idnt_mat\n";
                break;
                case null_mat : //cout << "null_mat\n";
                    if (is_sparse)
                        t.positions[0] = -1;
                    else
                        for (k=0; k<dimension; k++)
                            t.vec[k] = zero;
                    i = CND::cnd.automata-1;   // stops the loop
                break;
                case cnst_mat : //cout << "cnst_mat\n";
                    t.mcs(CND::cnd.synch_mat[e][a], t, a);
                break;
                case func_mat : //cout << "func_mat\n";
                    t.mfis(CND::cnd.synch_mat[e][a],CND::cnd.state_map, t, a);
            }
        }

        //Part 3/3: for the last matrix...
        a = CND::cnd.s_D_order[e].aut(CND::cnd.automata-1);
        switch (CND::cnd.sf_type[e][a]) {
            case idnt_mat : //cout << "idnt_mat\n";
                if (is_sparse) {
                    int k2=0;
                    for (k=0; k<dimension; k++) {
                        if (t.positions[k2]!=-1) {
                            if (t.positions[k2]==positions[k]) {
                                vec[k] += t.vec[k2];
                                k2++;
                            }
                        } else break;
                    }
                } else
                    for (k=0; k<dimension; k++)
                        vec[k] += t.vec[k];
                break;
            case null_mat : //cout << "null_mat\n";
                break;
            case cnst_mat : //cout << "cnst_mat\n";
                mcp(CND::cnd.synch_mat[e][a], t, a);
                break;
            case func_mat : //cout << "func_mat\n";
                mfip(CND::cnd.synch_mat[e][a],CND::cnd.state_map, t, a);
        }
    }
}

     //--------------------------------------------------------------------------//
     //       Multiply by a CND  -  Method S  -  'this' = 'v' * 'CND::cnd'       //
     //            this.positions == v.positions (call from mult_CND) - split    //
     //--------------------------------------------------------------------------//
void VCT::mult_CND_S(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2403)
  if (CND::cnd.ss() != dimension)
    Programming_Error("bad sized CND", 2404)
#endif
    if (is_sparse != v.is_sparse)
        cout << "VCT::mult_CND_S : different vector implementation type\n";

    // split doesnt deal with local matrices, only synchs
    int a, i, e, k;
    bool b = true;
    for (a=0; a<CND::cnd.automata; a++)
        switch (CND::cnd.lf_type[a]) {
            case idnt_mat : add(v); // The RSS is the same
                break;
            case null_mat :
                break;
            case cnst_mat : mcp(CND::cnd.local_mat[a], v, a);
                break;
            case func_mat : t.becomes(v, CND::cnd.l_B_order[a], CND::cnd.l_B_jumps[a]);
                t.mfos(CND::cnd.local_mat[a], CND::cnd.state_map, t, a, true);
                (*this) += t;
    }

    // ************* Positive matrices ****************
    aut_st st;
    san_mat sm;
    int idx, idxo, cut, na, nright_cut, idx_au = 0;
    st.create(CND::cnd.automata);
    for (e = 0; e < CND::cnd.events; e++) {
        cut = CND::cnd.get_fcut(e);
        if (cut == 0) {                     //shuffle approach
            t.becomes(v, CND::cnd.s_B_order[e], CND::cnd.s_B_jumps[e]);
            for (i=0; i<CND::cnd.automata; i++) {
                  a = CND::cnd.s_D_order[e].aut(i);
                  switch (CND::cnd.sf_type[e][a]) {
                     case idnt_mat : //cout << "idnt_mat\n";
                        break;
                     case null_mat : //cout << "null_mat\n";
                        if (is_sparse)
                              t.positions[0] = -1;
                        else
                              for (k=0; k<dimension; k++)
                                 t.vec[k] = zero;
                              i=CND::cnd.automata-1;     // stops the loop
                        break;
                     case cnst_mat :
                        t.mcs(CND::cnd.synch_mat[e][a], t, a);
                        b = false;   // we may have change the positions of t.
                        break;
                     case func_mat :
                        t.mfos(CND::cnd.synch_mat[e][a], CND::cnd.state_map, t, a, b);
                  }
            }
            (*this) += t;
        } else {
            st.reset();
            if (cut == CND::cnd.size()) { //'pure' sparse approach - shuffle is NEVER called...
                for (int i = 0; i < aunfs[e].size; i++) {
                    idxo = aunfs[e].vaunf[i].baseout;
                    idx = aunfs[e].vaunf[i].basein;
                    vec[idxo] += v.vec[idx] * aunfs[e].vaunf[i].scalar;
                    st.next(CND::cnd.s_B_order[e], v.sizes, CND::cnd.orig_jumps, idxo);
                    st.next(CND::cnd.s_B_order[e], v.sizes, CND::cnd.orig_jumps, idx);
                }
            } else {                        //'split' approach
                na = CND::cnd.s_B_order[e].aut(cut);
                nright_cut = CND::cnd.s_B_jumps[e].nright(na) * v.sizes.sz(na); //because s_B_jumps does not include the 'na' matrix...
                v_in  = new rp[nright_cut];
                for (int i = 0; i < aunfs[e].size; i++) {
                    idx = aunfs[e].vaunf[i].basein;
                    for (int l = 0; l < nright_cut; l++) {
                        v_in[l] = v.vec[idx] * aunfs[e].vaunf[i].scalar;
                        st.next(CND::cnd.s_B_order[e], v.sizes, CND::cnd.orig_jumps, idx);
                        //idx++;   //comente a linha de cima e descomente essa para rodar o split sem permutacoes...
                    }
                    for (int j = CND::cnd.size()-1; j>=cut; j--) {
                        int a = CND::cnd.s_B_order[e].aut(j);
                        sm = CND::cnd.synch_mat[e][a];
                        if (CND::cnd.sf_type[e][a] == func_mat)
                            split_func(sm, v.sizes, e, a, cut, CND::cnd.state_map, aunfs[e].vaunf[i].pgs);
                        else
                        if (CND::cnd.sf_type[e][a] != idnt_mat) {
                            split_const(sm, e, a, cut, v.sizes);
                        }
                    }
                    idx = aunfs[e].vaunf[i].baseout;
                    for (int l = 0; l < nright_cut; l++) {
                        vec[idx] += v_in[l];
                        st.next(CND::cnd.s_B_order[e], v.sizes, CND::cnd.orig_jumps, idx);
                        //idx++;   //comente a linha de cima e descomente essa para rodar o split sem permutacoes...
                    }
                }
                delete[] v_in;
            }
        }
    }
}

rp VCT::get_scalar(const int e, aut_st & gs, int *basein, int *baseout, aut_st *gs_aux, const int cut, jumps j) {
    //discovers the scalar
    double scalar = 1.0f;
    san_mat sm;
    *basein = *baseout = 0;
    for (int i = cut-1; i >= 0; i--) {
        int a = CND::cnd.s_B_order[e].aut(i);
        san_mat mat = CND::cnd.synch_mat[e][a];
        int nr = j.nright(a);
        int lin = mat.get_elem_lin(gs.val(a));
        int col = mat.get_elem_col(gs.val(a));
        if (nr != 1) {
            *basein += lin * nr;
            *baseout += col * nr;
        } else {
            *basein += lin;
            *baseout += col;
        }
        gs_aux->stamp(i, lin);
        double val = mat.get_elem_value(gs.val(a), gs);
        if (val != 1) { //a small optimization here
            scalar *= val;
        }
    }
    return scalar;
}

     //--------------------------------------------------------------------------//
     //       Multiply by a CND  -  Method B  -  'this' = 'v' * 'CND::cnd'       //
     //            this.positions == v.positions (call from mult_CND)            //
     //--------------------------------------------------------------------------//
void VCT::mult_CND_B(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2405)
  if (CND::cnd.ss() != dimension)
    Programming_Error("bad sized CND", 2406)
#endif
 int a;            // counter for the automata
 int e;            // counter for the synchronizing events
 int i;            // a counter for positions in a list of automata
 int k;            // a counter for vector positions
 bool b = true;

 if (is_sparse != v.is_sparse)
   cout << "VCT::mult_CND_B : different vector implementation type\n";


 for (a=0; a<CND::cnd.automata; a++)
   switch (CND::cnd.lf_type[a]) {
     case idnt_mat : add(v); // The RSS is the same
       break;
     case null_mat : break;
     case cnst_mat : mcp(CND::cnd.local_mat[a], v, a);
                     break;
     case func_mat : t.becomes(v, CND::cnd.l_B_order[a], CND::cnd.l_B_jumps[a]);
                     t.mfos(CND::cnd.local_mat[a], CND::cnd.state_map, t, a, true);
                     (*this) += t;
   }
    for (e = 0; e < CND::cnd.events; e++) {
        /*
         cout << "order for event " << e << endl;
         for (i=0;i<CND::cnd.automata;i++) {
             cout << " " << CND::cnd.s_B_order[e].aut(i);
         }
         cout << endl;
        /**/
        t.becomes(v, CND::cnd.s_B_order[e], CND::cnd.s_B_jumps[e]);
        for (i=0; i<CND::cnd.automata; i++) {
            a = CND::cnd.s_D_order[e].aut(i);
            switch (CND::cnd.sf_type[e][a]) {
                case idnt_mat : //cout << "idnt_mat\n";
                    break;
                case null_mat : //cout << "null_mat\n";
                    if (is_sparse)
                        t.positions[0] = -1;
                    else
                        for (k=0; k<dimension; k++)
                            t.vec[k] = zero;
                        i=CND::cnd.automata-1;     // stops the loop
                    break;
                case cnst_mat : //cout << "cnst_mat\n";
                    t.mcs(CND::cnd.synch_mat[e][a], t, a);
                    b = false;   // we may have change the positions of t.
                    break;
                case func_mat : //cout << "func_mat\n";
                    t.mfos(CND::cnd.synch_mat[e][a], CND::cnd.state_map, t, a, b);
            }
        }
        (*this) += t;
    }

}

     //--------------------------------------------------------------------------//
     //       Multiply by a CND  -  Method C  -  'this' = 'v' * 'CND::cnd'       //
     //            this.positions == v.positions (call from mult_CND)            //
     //--------------------------------------------------------------------------//
void VCT::mult_CND_C(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2407)
  if (CND::cnd.ss() != dimension)
    Programming_Error("bad sized CND", 2408)
#endif
 int a;            // counter for the automata
 int e;            // counter for the synchronizing events
 int i;            // a counter for positions in a list of automata
 int k;            // a counter for vector positions
 bool b = true;

 if (is_sparse != v.is_sparse)
   cout << "VCT::mult_CND_C : different vector implementation type\n";

 for (a=0; a<CND::cnd.automata; a++)
   switch (CND::cnd.lf_type[a]) {
     case idnt_mat : add(v); // The RSS is the same
       break;
     case null_mat : break;
     case cnst_mat : mcp(CND::cnd.local_mat[a], v, a);
                     break;
     case func_mat :
       //cout << "avant becomes\n";
       t.becomes(v, CND::cnd.l_C_order[a], CND::cnd.l_C_jumps[a]);
       //cout << "apres becomes \n"; t.debug();
       t.mnps(CND::cnd.local_mat[a], CND::cnd.state_map, t, a,
                            CND::cnd.l_C_left[a], CND::cnd.l_C_right[a],
                            CND::cnd.l_C_fst[a], true);
       //cout << "apres mnps\n"; t.debug();
       (*this) += t;
       //cout << "apres +=\n"; debug();
   }
 for (e=0; e<CND::cnd.events; e++) {
   //------------------------------
   t.get_CND();  // it copies the permutation info of the original vector ('v')
   //------------------------------
     a = CND::cnd.s_D_order[e].aut(0);
     switch (CND::cnd.sf_type[e][a]) {
     case idnt_mat : //cout << "idnt_mat\n";
       if (is_sparse)
	 for (k=0; k<dimension; k++) {
	   t.vec[k] = v.vec[k];
	   t.positions[k] = v.positions[k];
	 }
       else
	 for (k=0; k<dimension; k++)
	   t.vec[k] = v.vec[k];
       break;
     case null_mat : //cout << "null_mat\n";
       if (is_sparse)
	 t.positions[0] = -1;
       else
	 for (k=0; k<dimension; k++)
	   t.vec[k] = zero;
       break;
     case cnst_mat : //cout << "cnst_mat\n";
       t.mcs(CND::cnd.synch_mat[e][a], v, a);
       b = false;  // We may have change the positions of t
       break;
     case func_mat : //cout << "func_mat\n";
       t.becomes(v, CND::cnd.s_C_order[e][a], CND::cnd.s_C_jumps[e][a]);
       t.mnps(CND::cnd.synch_mat[e][a],
	      CND::cnd.state_map, t, a,
	      CND::cnd.s_C_left[e][a],
	      CND::cnd.s_C_right[e][a],
	      CND::cnd.s_C_fst[e][a],
	      true);
     }
     for (i=1; i<CND::cnd.automata; i++) {
       a = CND::cnd.s_D_order[e].aut(i);
       switch (CND::cnd.sf_type[e][a]) {
       case idnt_mat : //cout << "idnt_mat\n";
	 break;
       case null_mat : //cout << "null_mat\n";
	 if (is_sparse)
	   t.positions[0] = -1;
	 else
	   for (k=0; k<dimension; k++)
	     t.vec[k] = zero;
	 i=CND::cnd.automata-1;   // stops the loop
	 break;
       case cnst_mat : //cout << "cnst_mat\n";
	 t.mcs(CND::cnd.synch_mat[e][a], t, a);
	 b = false;  // We may have change the positions of t
	 break;
       case func_mat : //cout << "func_mat\n";
	 t.permut(CND::cnd.s_C_order[e][a], CND::cnd.s_C_jumps[e][a]);
	 t.mnps(CND::cnd.synch_mat[e][a],
		CND::cnd.state_map, t, a,
		CND::cnd.s_C_left[e][a],
		CND::cnd.s_C_right[e][a],
		CND::cnd.s_C_fst[e][a],
		b);
       }
     }
     
     (*this) += t;
 }

}

 
     //--------------------------------------------------------------------------//
     //     Multiply by a CND  -  Mixed Method  -  'this' = 'v' * 'CND::cnd'     //
     //            this.positions == v.positions (call from mult_CND)            //
     //--------------------------------------------------------------------------//
void VCT::mult_CND_M(const VCT & v)
{
#ifdef _PEPS_DEBUG_
  if (dimension != v.dimension)
    Programming_Error("different sized vectors", 2409)
  if (CND::cnd.ss() != dimension)
    Programming_Error("bad sized CND", 2410)
#endif
 int a;            // counter for the automata
 int e;            // counter for the synchronizing events
 int i;            // a counter for positions in a list of automata
 int k;            // a counter for vector positions

 bool b = true;

 if (is_sparse != v.is_sparse)
   cout << "VCT::mult_CND_M : different vector implementation type\n";


 //cout << "mult_CND_M : nb d'automates = " << CND::cnd.automata << "\n";
 // cout << "debut mult_CND_M: this = \n"; debug();

 for (a=0; a<CND::cnd.automata; a++) {

   //cout << "automata " << a+1 << " / " << CND::cnd.automata << "\n";

   switch (CND::cnd.l_type[a]) {
   case idnt_term :  //cout << "  idnt_term \n";
                       for (k=0; k<dimension; k++)
                        vec[k] += one;
                      break;
   case null_term : //cout << "  null_term \n";
                    break;
   case cnst_term : //cout << "  cnst_term \n";
       mcp(CND::cnd.local_mat[a], v, a);
       break;
   case A_term    : //cout << "  A_term \n";
       mfip(CND::cnd.local_mat[a], CND::cnd.state_map, v, a);
       break;
   case B_term    : //cout << "  B_term \n";
       t.becomes(v, CND::cnd.l_B_order[a], CND::cnd.l_B_jumps[a]);
       t.mfos(CND::cnd.local_mat[a], CND::cnd.state_map, t, a, true);
       (*this) += t;
       break;
   case C_term    : //cout << "  C_term \n";
       t.becomes(v, CND::cnd.l_C_order[a], CND::cnd.l_C_jumps[a]);
       t.mnps(CND::cnd.local_mat[a], CND::cnd.state_map, t, a,
	      CND::cnd.l_C_left[a], CND::cnd.l_C_right[a],
	      CND::cnd.l_C_fst[a], true);
       (*this) += t;
   }
   
   //debug();

 }

 for (e=0; e<CND::cnd.events; e++) {
   //------------------------------
   t.get_CND();  // it copies the permutation info of the original vector ('v')
   //------------------------------
   //cout << "event " << e+1 << " / " << CND::cnd.events << "\n";
   switch (CND::cnd.s_type[e]) {
   
   case idnt_term : //cout << "idnt_term\n";
     for (k=0; k<dimension; k++)
       vec[k] += one;
     break;
     
   case null_term : //cout << "null_term\n";
     break;
   
   case cnst_term : //cout << "cnst_term\n";
   
   case A_term    : //cout << "A_term\n";
     a = CND::cnd.s_D_order[e].aut(0);
     switch (CND::cnd.sf_type[e][a]) {
     case idnt_mat : //cout << "idnt_mat\n";
       if (is_sparse)
	 for (k=0; k<dimension; k++) {
	   t.vec[k] = v.vec[k];
	   t.positions[k] = v.positions[k];
	 }
       else
	 for (k=0; k<dimension; k++)
	   t.vec[k] = v.vec[k];
       break;
     case null_mat : //cout << "null_mat\n";
       if (is_sparse)
	 t.positions[0] = -1;
       else
	 for (k=0; k<dimension; k++)
	   t.vec[k] = zero;
       break;
     case cnst_mat : //cout << "cnst_mat\n";
       t.mcs(CND::cnd.synch_mat[e][a], v, a);
       break;
     case func_mat : //cout << "func_mat\n";
       t.mfis(CND::cnd.synch_mat[e][a],CND::cnd.state_map, v, a);
     }
     //cout << "fin autom 0 \n";
     //t.debug();
     //     e=CND::cnd.events; break;
     
     for (i=1; i<CND::cnd.automata-1; i++) {
       a = CND::cnd.s_D_order[e].aut(i);
       switch (CND::cnd.sf_type[e][a]) {
       case idnt_mat : //cout << "idnt_mat\n";
	 break;
       case null_mat : //cout << "null_mat\n";
	 if (is_sparse)
	   t.positions[0] = -1;
	 else
	   for (k=0; k<dimension; k++)
	     t.vec[k] = zero;
	 i=CND::cnd.automata-1;   // stops the loop
	 break;
       case cnst_mat : //cout << "cnst_mat\n";
	 t.mcs(CND::cnd.synch_mat[e][a], t, a);
	 break;
       case func_mat : //cout << "func_mat\n";
	 t.mfis(CND::cnd.synch_mat[e][a],CND::cnd.state_map, t, a);
       }
       //cout << "fin autom " << i << " \n";
       //t.debug();
     }

     a = CND::cnd.s_D_order[e].aut(CND::cnd.automata-1);
     switch (CND::cnd.sf_type[e][a]) {
     case idnt_mat : //cout << "idnt_mat\n";
       if (is_sparse) {
	 int k2=0;
	 for (k=0; k<dimension; k++) {
	   if (t.positions[k2]!=-1) {
	     if (t.positions[k2]==positions[k]) {
	       vec[k] += t.vec[k2];
	       k2++;
	     }
	   }
	   else
	     k=dimension;
	 }
       }
       else
	 for (k=0; k<dimension; k++)
	   vec[k] += t.vec[k];
       break;
     case null_mat : //cout << "null_mat\n";
       break;
     case cnst_mat : //cout << "cnst_mat\n";
       //debug();
       mcp(CND::cnd.synch_mat[e][a], t, a);
       break;
     case func_mat : //cout << "func_mat\n";
       mfip(CND::cnd.synch_mat[e][a],CND::cnd.state_map, t, a);
     }
     //cout << "fin autom n \n";
     //debug();
     //e=CND::cnd.events;
     
     break;
 
    case B_term    : //cout << "B_term\n";
        t.becomes(v, CND::cnd.s_B_order[e], CND::cnd.s_B_jumps[e]);
        for (i=0; i<CND::cnd.automata; i++) {
            a = CND::cnd.s_D_order[e].aut(i);
            switch (CND::cnd.sf_type[e][a]) {
                case idnt_mat :  //cout << "idnt_mat\n";
                    break;
                case null_mat : //cout << "null_mat\n";
                    if (is_sparse)
                        t.positions[0] = -1;
                    else
                        for (k=0; k<dimension; k++)
                            t.vec[k] = zero;
                    i=CND::cnd.automata-1;     // stops the loop
                    break;
                case cnst_mat : //cout << "cnst_mat\n";
                    t.mcs(CND::cnd.synch_mat[e][a], t, a);
                    b = false; // We may have change the positions of t
                    break;
                case func_mat : //cout << "func_mat\n";
                    t.mfos(CND::cnd.synch_mat[e][a], CND::cnd.state_map, t, a, b);
            }
        }
        (*this) += t;
        break;
   
   case C_term    : //cout << "C_term\n";
     a = CND::cnd.s_D_order[e].aut(0);
     switch (CND::cnd.sf_type[e][a]) {
     case idnt_mat : //cout << "idnt_mat\n";
       if (is_sparse)
	 for (k=0; k<dimension; k++) {
	   t.vec[k] = v.vec[k];
	   t.positions[k] = v.positions[k];
	 }
       else
	 for (k=0; k<dimension; k++)
	   t.vec[k] = v.vec[k];
       break;
     case null_mat : //cout << "null_mat\n";
       if (is_sparse)
	 t.positions[0] = -1;
       else
	 for (k=0; k<dimension; k++)
	   t.vec[k] = zero;
       break;
     case cnst_mat : //cout << "cnst_mat\n";
       t.mcs(CND::cnd.synch_mat[e][a], v, a);
       b = false;
       break;
     case func_mat : //cout << "func_mat\n";
       t.becomes(v, CND::cnd.s_C_order[e][a], CND::cnd.s_C_jumps[e][a]);
       t.mnps(CND::cnd.synch_mat[e][a],
	      CND::cnd.state_map, t, a,
	      CND::cnd.s_C_left[e][a],
	      CND::cnd.s_C_right[e][a],
	      CND::cnd.s_C_fst[e][a], true);
     }
     for (i=1; i<CND::cnd.automata; i++) {
       a = CND::cnd.s_B_order[e].aut(i);
       switch (CND::cnd.sf_type[e][a]) {
       case idnt_mat : //cout << "idnt_mat\n";
	 break;
       case null_mat : //cout << "null_mat\n";
	 if (is_sparse)
	   t.positions[0] = -1;
	 else
	   for (k=0; k<dimension; k++)
	     t.vec[k] = zero;
	 i=CND::cnd.automata-1;   // stops the loop
	 break;
       case cnst_mat : //cout << "cnst_mat\n";
	 t.mcs(CND::cnd.synch_mat[e][a], t, a);
	 b = false;
	 break;
       case func_mat : //cout << "func_mat\n";
	 t.permut(CND::cnd.s_C_order[e][a], CND::cnd.s_C_jumps[e][a]);
	 t.mnps(CND::cnd.synch_mat[e][a],
		CND::cnd.state_map, t, a,
		CND::cnd.s_C_left[e][a],
		CND::cnd.s_C_right[e][a],
		CND::cnd.s_C_fst[e][a], b);
       }
     }
     (*this) += t;
   }
 } 
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                                                          //
     //     It multiplies a vector 'v' by a normal factor without functions (non //
     // identity matrix in 'm') adding the result to the vector 'this'.          //
     //--------------------------------------------------------------------------//
void VCT::mcp(san_mat & m, const VCT & v, const aut_id a)
{
  //cout << "begin mcp\n";

 int i, ni    = v.sizes.sz(a);        // size of the automaton 'a'
 int l, nleft  = v.p_jumps.nleft(a);   // space size before the automaton 'a'
 int r, nright = v.p_jumps.nright(a);  // space size after the automaton 'a'
 int njump     = v.p_jumps.njump(a);   // 'nright' times 'ni'

 int idx, base = 0;              // pointer to pick vector elements for the slices

 //cout << "ni, nleft, nright : " << ni << " " << nleft << " " << nright << "\n";

 if (is_sparse) { // sparse vector
   div_t zin_pos;
   int j, j2, dim;
   ///int rr, k2;
   ///int jinit, jinit2;

   gs_pair tmp;
   int_pair tmp2;
   
   j=0; j2=0;
   dim = v.size();
   
   if (PRF::prf.z_impl() == zin_sparse) { // sparse zin

     info_pair tmpin, tmpout;
     vector<info_pair>::const_iterator izin, izout;
        // iterators to traverse the 'info_zin' and 'info_zout' vectors

     zin.reserve(ni);
     info2_zin.reserve(dim);
     info2_zout.reserve(dimension);


     for (l=0; l<nleft; l++) {                            // for the left sub-space:

       // We create the info_zin array (informations on zin)
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j;
	 tmpin.first=zin_pos.rem;
	 tmpin.second=tmp2;
	 info2_zin.push_back(tmpin);
	 j++;
       }
       
       // We create the info_zout array (informations on zout)
       while ((j2<dimension) && (positions[j2]<(l+1)*njump) && (positions[j2]!=-1)) {
	 zin_pos = div(positions[j2]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j2;
	 tmpout.first=zin_pos.rem;
	 tmpout.second=tmp2;
	 info2_zout.push_back(tmpout);
	 j2++;
       }
       
       // We sort info_zin and info_zout
       sort(info2_zin.begin(), info2_zin.end(), comp2());
       sort(info2_zout.begin(), info2_zout.end(), comp2());
       izin  = info2_zin.begin();
       izout = info2_zout.begin();

       for (r=0;r<nright;r++) {
	 if ((*izout).first == r) {
	   zin.erase(zin.begin(), zin.end());
	   while ((*izin).first < r) ++izin; // we go to the good zin
	   while ((izin != info2_zin.end()) && ((*izin).first == r)) {
	     // Extraction of zin
	     tmp2 = (*izin).second;
	     tmp.first = tmp2.first;
	     tmp.second = v.vec[tmp2.second];
	     zin.push_back(tmp);
	     ++izin;
	   }
	   // sort the current zin
	   sort(zin.begin(),zin.end(), comp());

	   // Now that we have the current zin, we compute each element
	   // of the resulting vector, and store the results in vec.
	   while ((izout != info2_zout.end()) && ((*izout).first == r)) {
	     tmp2 = (*izout).second;
	     vec[tmp2.second] += m.sparse_elem_left_mult2(zin, tmp2.first);
	     ++izout;
	   }
	 }
       }

       // Erase everything
       info2_zin.erase(info2_zin.begin(), info2_zin.end());
       info2_zout.erase(info2_zout.begin(), info2_zout.end());
     }
   } // end sparse zin
 /* ----------------------------------------------------------------------------------*/
   else { // zin in full implementation

     //rp zinf[nright][ni];   // array of the full zin

     for (l=0; l<nleft; l++) {                            // for the left sub-space:
       
       // Initialisations
       for (r=0; r<nright; r++)
	 for (i=0; i<ni; i++)
	   zinf[r][i]=0;
       
       // We create the zinf array for the sparse multiplication
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 zinf[zin_pos.rem][zin_pos.quot] = v.vec[j];
	 j++;
       }
       
       // Here, we have all the zinf for the l portion ; we can make the 
       // vector-matrix multiplication in sparse : we compute each element
       // of the resulting vector, and store the results in vec.
       while ((j2<dimension) && (positions[j2]<(l+1)*njump)) {
	 zin_pos = div(positions[j2]-l*njump,nright);
	 vec[j2] += m.sparse_elem_left_mult(zinf[zin_pos.rem], zin_pos.quot);
	 j2++;
       }

     } //end of left sub-space    
   } //end of zin in full implementation
 } // end of sparse vector implementation
 
 else {  // full vector implementation
    for (l=0; l<nleft; l++) {                            // for the left sub-space:
        for (r=0; r<nright; r++) {                         // for the right sub-space:
            idx = base + r;                                  //
            for (i=0; i<ni; i++) {                           //
                z_in[i] = v.vec[idx];                          // gets the z_in slice
                idx += nright;                                 //
            }                                                //
            m.left_mult(z_out, z_in);                        // z_out = z_in * matrix
            idx = base + r;                                  //
            for (i=0; i<ni; i++) {                           //
                vec[idx] += z_out[i];                          // stores the z_out slice
                idx += nright;                                 //
            }                                                //
        }
        base += njump;                                     // pass to the next
    }                                                    // left-space block
 } 

 // cout << "end mcp\n";
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                (Split)                                   //
     //                                                                          //
     //     It multiplies a vector 'v' by a normal factor without functions (non //
     // identity matrix in 'm') storing the result in the vector 'this'.         //
     //--------------------------------------------------------------------------//
void VCT::split_const(san_mat & m, const int e, const aut_id a, const int cut, const aut_st & vsizes) {
 int c           = CND::cnd.s_B_order[e].aut(cut);
 int i, j, ni    = vsizes.sz(a);                     // size of the automaton 'a'
 int l, nleft_i  = CND::cnd.s_B_jumps[e].nleft(a);   // space size before the automaton 'a'
 int nleft_cut   = CND::cnd.s_B_jumps[e].nleft(c);   // space size before the automaton 'a'
 int nright_i    = CND::cnd.s_B_jumps[e].nright(a);  // space size after the automaton 'a'
 int njump       = CND::cnd.s_B_jumps[e].njump(a);   // 'nright' times 'ni'

 int index = 0;
 int base = 0;
 for (i = 0; i < (nleft_i / nleft_cut); i++) {
     for (j = 0; j < nright_i; j++) {
        index = base + j;
        for (l = 0; l < ni; l++) {
            z_in[l] = v_in[index];
            index = index + nright_i;
        }
        m.left_mult(z_out, z_in);
        index = base + j;
        for (l = 0; l < ni; l++) {
            v_in[index] = z_out[l];
            index = index + nright_i;
        }
     }
     base += njump;
 }
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                (Split)                                   //
     //                                                                          //
     //     It multiplies a vector 'v' by a normal factor without functions (non //
     // identity matrix in 'm') storing the result in the vector 'this'.         //
     //--------------------------------------------------------------------------//

void VCT::split_func(san_mat & m, const aut_st & vsizes, const int e, const aut_id a, const int cut, const st_map * sm, const aut_st & pgs) {
 int c           = CND::cnd.s_B_order[e].aut(cut);
 int i, j, ni    = vsizes.sz(a);                     // size of the automaton 'a'
 int l, nleft_i  = CND::cnd.s_B_jumps[e].nleft(a);   // space size before the automaton 'a'
 int nleft_cut   = CND::cnd.s_B_jumps[e].nleft(c);   // space size before the automaton 'a'
 int nright_i    = CND::cnd.s_B_jumps[e].nright(a);  // space size after the automaton 'a'
 int njump       = CND::cnd.s_B_jumps[e].njump(a);   // 'nright' times 'ni'
 int pos         = CND::cnd.s_B_order[e].pos(a);     // position of automaton 'a' in the list
 int val = 0;

 g_st.reset();
 /**/
 for (i = 0; i < pgs.size(); i++) {
     val = CND::cnd.s_B_order[e].aut(i);
     g_st.stamp(val, pgs.val(i));
 }
 /**/
 int index = 0;
 int base = 0;
 for (i = 0; i < (nleft_i / nleft_cut); i++) {
     m.jit_eval(g_st, a, sm);    // evaluates the matrix
     for (j = 0; j < nright_i; j++) {
        index = base + j;
        for (l = 0; l < ni; l++) {
            z_in[l] = v_in[index];
            index = index + nright_i;
        }
        m.left_mult(z_out, z_in);
        index = base + j;
        for (l = 0; l < ni; l++) {
            v_in[index] = z_out[l];
            index = index + nright_i;
        }
     }
     base += njump;
     g_st.next(CND::cnd.s_B_order[e], vsizes, pos, Left);          // left-space block
 }
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                                                          //
     //      It multiplies a vector 'v' by a normal factor with functions (non   //
     // identity matrix in 'm') adding the result to the vector 'this'.          //
     //                                                                          //
     // Obs: The function evaluations are performed inside the inner most loop   //
     // (no permutations are need).                                              //
     //--------------------------------------------------------------------------//
void VCT::mfip(san_mat & m, const st_map * sm, const VCT & v, const aut_id a)
{
  //  cout << "begin mfip\n";;

 int i, ni     = v.sizes.sz(a);        // size of the automaton 'a'
 int l, nleft  = v.p_jumps.nleft(a);   // space size before the automaton 'a'
 int r, nright = v.p_jumps.nright(a);  // space size after the automaton 'a'
 int njump     = v.p_jumps.njump(a);   // 'nright' times 'ni'
 int idx, base = 0;               // pointer to pick vector elements for the slices

 // cout << "ni, nleft, nright : " << ni << " " << nleft << " " << nright << "\n";

 g_st.reset();                                        // global state reset

 if (is_sparse) { // sparse vectors
   div_t zin_pos;
   int j, j2, rr, dim;
   ///int k2;
   ///int jinit, jinit2;
   
   gs_pair tmp;
   int_pair tmp2;
   
   j=0;j2=0;
   dim = v.size();
   
   if (PRF::prf.z_impl() == zin_sparse) { // sparse zin

     info_pair tmpin, tmpout;
     vector<info_pair>::const_iterator izin, izout;
        // iterators to traverse the 'info_zin' and 'info_zout' vectors

     zin.reserve(ni);
     info2_zin.reserve(dim);
     info2_zout.reserve(dim);


     for (l=0; l<nleft; l++) {                            // for the left sub-space:

       // We create the info_zin array (informations on zin)
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j;
	 tmpin.first=zin_pos.rem;
	 tmpin.second=tmp2;
	 info2_zin.push_back(tmpin);
	 j++;
       }
       
       // We create the info_zout array (informations on zout)
       while ((j2<dimension) && (positions[j2]<(l+1)*njump) && (positions[j2]!=-1)) {
	 zin_pos = div(positions[j2]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j2;
	 tmpout.first=zin_pos.rem;
	 tmpout.second=tmp2;
	 info2_zout.push_back(tmpout);
	 j2++;
       }
       
       // We sort info_zin and info_zout
       sort(info2_zin.begin(), info2_zin.end(), comp2());
       sort(info2_zout.begin(), info2_zout.end(), comp2());
       izin  = info2_zin.begin();
       izout = info2_zout.begin();

       for (r=0;r<nright;r++) {
	 if ((*izout).first == r) {
	   zin.erase(zin.begin(), zin.end());
	   while ((*izin).first < r) ++izin; // we go to the good zin
	   while ((izin != info2_zin.end()) && ((*izin).first == r)) {
	     // Extraction of zin
	     tmp2 = (*izin).second;
	     tmp.first = tmp2.first;
	     tmp.second = v.vec[tmp2.second];
	     zin.push_back(tmp);
	     ++izin;
	   }
	   // sort the current zin
	   sort(zin.begin(),zin.end(), comp());

	   // Now that we have the current zin, we compute each element
	   // of the resulting vector, and store the results in vec.
	   m.jit_eval(g_st, a, sm);                           // evaluates the matrix
	   while ((izout != info2_zout.end()) && ((*izout).first == r)) {
	     tmp2 = (*izout).second;
	     vec[tmp2.second] += m.sparse_elem_left_mult2(zin, tmp2.first);
	     ++izout;
	   }
	 }
	 g_st.next(v.sizes, a, Right);                       // changes the state right
       }

       // Erase everything
       info2_zin.erase(info2_zin.begin(), info2_zin.end());
       info2_zout.erase(info2_zout.begin(), info2_zout.end());

       g_st.next(v.sizes, a, Left);                       // changes the state left     
     }
   } // end sparse zin
 /* ----------------------------------------------------------------------------------*/
   else { // full zin implementation
     
     /*     
     int indice_r[nright];
     int useful_f[nright][ni];
     int place[nright][ni];
     bool used_z[nright];
     rp zinf[nright][ni];
     */
     
     for (l=0; l<nleft; l++) {                        // for the left subspace
       
       // Initialisations of the array : null
       for (r=0; r<nright; r++) {
	 used_z[r] = false;
	 indice_r[r] = 0;
	 for (i=0; i<ni; i++)
	   zinf[r][i] = 0;
       }
       
       // We create the zinf array for the sparse multiplication
       while ((j<dim) && (v.positions[j]<(l+1)*njump)  && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 rr = zin_pos.rem;
	 zinf[rr][zin_pos.quot] = v.vec[j];
	 j++;
       }
       while ((j2<dimension) && (positions[j2]<(l+1)*njump)&& (positions[j2]!=-1)) {
	 zin_pos = div(positions[j2]-l*njump,nright);
	 rr = zin_pos.rem;
	 used_z[rr] = true;
	 useful_f[rr][indice_r[rr]] = zin_pos.quot;
	 place[rr][indice_r[rr]] = j2;
	 indice_r[rr]++;
	 j2++;
       }
       
       // Here, we have all the zinf for the l portion ; we can make the 
       // vector-matrix multiplication in sparse : we compute each element
       // of the resulting vector, and store the results in vec.
       for (r=0; r<nright; r++) {                         // for the right sub-space:
	 if (used_z[r]) {
	   m.jit_eval(g_st, a, sm);                           // evaluates the matrix
	   for (i=0; i<indice_r[r]; i++)
	     vec[place[r][i]] += m.sparse_elem_left_mult(zinf[r], useful_f[r][i]);
	 }
	 g_st.next(v.sizes, a, Right);                       // changes the state right
       }     
       g_st.next(v.sizes, a, Left);                       // changes the state left     
     } //end left subspace
     
     //   delete everything;
     //delete[] used_z;
     
   } // end full zin
 } // end sparse vector
 
 else { // full vector implementation
   for (l=0; l<nleft; l++) {                            // for the left sub-space:
     for (r=0; r<nright; r++) {                            // for the right sub-space:
       idx = base + r;                                     //
       for (i=0; i<ni; i++) {                              //
	 z_in[i] = v.vec[idx];                             // gets the z_in slice
	 idx += nright;                                    //
       }                                                   //
       m.jit_eval(g_st, a, sm);                                // evaluates the matrix
       m.left_mult(z_out, z_in);                           // z_out = z_in * matrix
       idx = base + r;                                     //
       for (i=0; i<ni; i++) {                              //
	 vec[idx] += z_out[i];                             // stores the z_out slice
	 idx += nright;                                    //
       }                                                   //
       g_st.next(v.sizes, a, Right);                       // changes the state right
     }
     g_st.next(v.sizes, a, Left);                        // changes the state left
     base += njump;                                      // pass to the next
   }                                                     // left-space block
 }

 //cout << "End mfip\n";

}
     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                                                          //
     //     It multiplies a vector 'v' by a normal factor without functions (non //
     // identity matrix in 'm') storing the result in the vector 'this'.         //
     //--------------------------------------------------------------------------//
void VCT::mcs(san_mat & m, const VCT & v, const aut_id a)
{

  //cout << "begin mcs\n";

 int i, ni     = v.sizes.sz(a);        // size of the automaton 'a'
 int l, nleft  = v.p_jumps.nleft(a);   // space size before the automaton 'a'
 int r, nright = v.p_jumps.nright(a);  // space size after the automaton 'a'
 int njump     = v.p_jumps.njump(a);   // 'nright' times 'ni'
 int idx, base = 0;                    // pointer to pick vector elements for the slices
 
 //cout << "ni, nleft, nright : " << ni << " " << nleft << " " << nright << "\n";

 if (is_sparse) { // sparse vector
   div_t zin_pos;
   int k2, j, j2, dim;
   ///int rr;
   ///int jinit, jinit2;

   vector<gs_pair>::const_iterator iter; // an iterator to traverse the 'zouts' vector
   gs_pair tmp;
   int_pair tmp2;

   j=0; j2=0; k2=0;
   dim = v.size();
   zouts.reserve(dimension);

   //if (PRF::prf.z_impl() == zin_sparse) { // sparse zin

     info_pair tmpin;
     vector<info_pair>::const_iterator izin;
        // iterators to traverse the 'info_zin' vector

     zin.reserve(ni);
     info2_zin.reserve(dim);
     for (l=0; l<nleft; l++) {                            // for the left sub-space:

       // We create the info_zin array (informations on zin)
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j;
	 tmpin.first=zin_pos.rem;
	 tmpin.second=tmp2;
	 info2_zin.push_back(tmpin);
	 j++;
       }

       // We sort info_zin
       sort(info2_zin.begin(), info2_zin.end(), comp2());
       izin  = info2_zin.begin();

       for (r=0;r<nright;r++) {
	 if ((*izin).first == r) {
	   zin.erase(zin.begin(), zin.end());
	   while ((izin != info2_zin.end()) && ((*izin).first == r)) {
	     // Extraction of zin
	     tmp2 = (*izin).second;
	     tmp.first = tmp2.first;
	     tmp.second = v.vec[tmp2.second];
	     zin.push_back(tmp);
	     ++izin;
	   }
	   // sort the current zin
	   sort(zin.begin(),zin.end(), comp());
	   
	   // We have the current zin ; we multiply it with the matrix and
	   // find which element are not nul. Those elements are stored in zouts.
	   m.sparse_left_mult2(zouts, zin, r, nright);
	 }
       }
       
       // put the zouts results in vec and positions
       sort(zouts.begin(), zouts.end(), comp());
       iter = zouts.begin();
       
       while ((iter != zouts.end()) && (k2<dimension)) {
	 //cout << "element non nul a la position " << (*iter).first + l*njump << "\n";
	 positions[k2]=(*iter).first + l*njump;
	 vec[k2]=(*iter).second;
	 ++iter; k2++;
       }
       if (k2==dimension)
	 cout << "*** Got out of RSS : you should use full vector implementation ***\n";

       // Erase everything
       info2_zin.erase(info2_zin.begin(), info2_zin.end());
       info2_zout.erase(info2_zout.begin(), info2_zout.end());      
       zouts.erase(zouts.begin(), zouts.end());
 
     }
     
     if (k2<dimension)
       positions[k2]=-1;

     // } // end of sparse zin
 //-----------------------------------------------------------------------------//
   /*  else {  // "full" zin   
     zins = new vector<gs_pair>[nright];

     for (l=0; l<nleft; l++) {                            // for the left sub-space:
       
       // Initialisations
       for (r=0; r<nright; r++)
	 zins[r].erase(zins[r].begin(), zins[r].end());
       
       // We create the zins array for the sparse multiplication
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 rr = zin_pos.rem;
	 tmp.first = zin_pos.quot;
	 tmp.second = v.vec[j];
	 if (zins[rr].size() == zins[rr].capacity())
	   zins[rr].reserve(zins[rr].size()+1);
	 zins[rr].push_back(tmp);
	 j++;
       }
       
       // Here, we have all the zins for the l portion ; we can make the 
       // vector-matrix multiplication in sparse : we compute each element
       // of the resulting vector, and store the results in vec.
       for (r=0; r<nright; r++)
	 m.sparse_left_mult2(zouts, zins[r], r, nright);
       
       // put the zouts results in vec and positions
       sort(zouts.begin(), zouts.end(), comp());
       iter = zouts.begin();
       
       while ((iter != zouts.end()) && (k2<dimension)) {
	 //cout << "element non nul a la position " << (*iter).first + l*njump << "\n";
	 positions[k2]=(*iter).first + l*njump;
	 vec[k2]=(*iter).second;
	 ++iter; k2++;
       }
       if (k2==dimension)
	 cout << "*** Got out of RSS : you should use full vector implementation ***\n";
      
       zouts.erase(zouts.begin(), zouts.end());
     }
     
     if (k2<dimension)
       positions[k2]=-1;
     
     //   delete[] zins and zouts;
     delete[] zins;
     } // end of full zin
    */
 }  // end of sparse vector implementation
 //-----------------------------------------------------------------------------//
 else {  // full vector implementation
   for (l=0; l<nleft; l++) {                            // for the left sub-space:
     for (r=0; r<nright; r++) {                            // for the right sub-space:
       idx = base + r;                                     //
       for (i=0; i<ni; i++) {                              //
	 z_in[i] = v.vec[idx];                             // gets the z_in slice
	 idx += nright;                                    //
       }                                                   //
       m.left_mult(z_out, z_in);                           // z_out = z_in * matrix
       idx = base + r;                                     //
       for (i=0; i<ni; i++) {                              //
	 vec[idx] = z_out[i];                              // stores the z_out slice
	 idx += nright;                                    //
       }                                                   //
     }
     base += njump;                                     // pass to the next
   }                                                    // left-space block
 }
//cout << "end mcs\n";
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                                                          //
     //      It multiplies a vector 'v' by a normal factor with functions (non   //
     // identity matrix in 'm') storing the result in the vector 'this'.         //
     //                                                                          //
     // Obs: The function evaluations are performed inside the inner most loop   //
     // (no permutations are need).                                              //
     //--------------------------------------------------------------------------//
void VCT::mfis(san_mat & m, const st_map * sm, const VCT & v, const aut_id a)
{

// cout << "begin mfis\n";
 int i, ni     = v.sizes.sz(a);        // size of the automaton 'a'
 int l, nleft  = v.p_jumps.nleft(a);   // space size before the automaton 'a'
 int r, nright = v.p_jumps.nright(a);  // space size after the automaton 'a'
 int njump     = v.p_jumps.njump(a);   // 'nright' times 'ni'
 int pos       = v.p_order.pos(a);     // position of automaton 'a' in the list
 int idx, base = 0;                    // pointer to pick vector elements for the slices

 //cout << "ni, nleft, nright : " << ni << " " << nleft << " " << nright << "\n";

 g_st.reset();                                                  // global state reset

 if (is_sparse) { // sparse vectors
   div_t zin_pos;
   int k2, j, j2, dim;
   ///int rr;
   ///int jinit, jinit2;
   
   vector<gs_pair>::const_iterator iter;  // an iterator to traverse the 'zouts' vector
   gs_pair tmp;
   int_pair tmp2;
   
   j=0; j2=0; k2=0;
   dim = v.size();
   zouts.reserve(dimension);

//if (PRF::prf.z_impl() == zin_sparse) { // sparse zin

     info_pair tmpin;
     vector<info_pair>::const_iterator izin;
        // iterators to traverse the 'info_zin' vector

     zin.reserve(ni);
     info2_zin.reserve(dim);
     for (l=0; l<nleft; l++) {                            // for the left sub-space:

       // We create the info_zin array (informations on zin)
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j;
	 tmpin.first=zin_pos.rem;
	 tmpin.second=tmp2;
	 info2_zin.push_back(tmpin);
	 j++;
       }


       // We sort info_zin
       sort(info2_zin.begin(), info2_zin.end(), comp2());
       izin  = info2_zin.begin();

       for (r=0;r<nright;r++) {
	 if ((*izin).first == r) {
	   zin.erase(zin.begin(), zin.end());
	   while ((izin != info2_zin.end()) && ((*izin).first == r)) {
	     // Extraction of zin
	     tmp2 = (*izin).second;
	     tmp.first = tmp2.first;
	     tmp.second = v.vec[tmp2.second];
	     zin.push_back(tmp);
	     ++izin;
	   }
	   // sort the current zin
	   sort(zin.begin(),zin.end(), comp());
	   
	   // We have the current zin ; we multiply it with the matrix and
	   // find which element are not nul. Those elements are stored in zouts.
	   m.jit_eval(g_st, a, sm);                           // evaluates the matrix
	   m.sparse_left_mult2(zouts, zin, r, nright);
	 }
	 g_st.next(v.sizes, a, Right);                       // changes the state right
       }
       
       // put the zouts results in vec and positions
       sort(zouts.begin(), zouts.end(), comp());
       iter = zouts.begin();
       
       while ((iter != zouts.end()) && (k2<dimension)) {
	 //cout << "element non nul a la position " << (*iter).first + l*njump << "\n";
	 positions[k2]=(*iter).first + l*njump;
	 vec[k2]=(*iter).second;
	 ++iter; k2++;
       }
       if (k2==dimension)
	 cout << "*** Got out of RSS : you should use full vector implementation ***\n";

       // Erase everything
       info2_zin.erase(info2_zin.begin(), info2_zin.end());
       info2_zout.erase(info2_zout.begin(), info2_zout.end());      
       zouts.erase(zouts.begin(), zouts.end());
 
       g_st.next(v.sizes, a, Left);                       // changes the state left
     }
     
     if (k2<dimension)
       positions[k2]=-1;

 }   // end of sparse vector implementation
 //-----------------------------------------------------------------------------//
 else {  // full vector implementation
   for (l=0; l<nleft; l++) {                            // for the left sub-space:
     for (r=0; r<nright; r++) {                            // for the right sub-space:
       idx = base + r;                                     //
       for (i=0; i<ni; i++) {                              //
	 z_in[i] = v.vec[idx];                             // gets the z_in slice
	 idx += nright;                                    //
       }                                                   //
       m.jit_eval(g_st, a, sm);                                // evaluates the matrix
       m.left_mult(z_out, z_in);                           // z_out = z_in * matrix
       idx = base + r;                                     //
       for (i=0; i<ni; i++) {                              //
	 vec[idx] = z_out[i];                              // stores the z_out slice
	 idx += nright;                                    //
       }                                                   //
       g_st.next(v.sizes, pos, Right);                     // pass to the next
     }                                                     // right-space block
     base += njump;                                     // pass to the next
     g_st.next(v.sizes, pos, Left);                     // left-space block
   }
 } // end full vectors
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                                                          //
     //      It multiplies a vector 'v' by a normal factor with functions (non   //
     // identity matrix in 'm') storing the result in the vector 'this'.         //
     //                                                                          //
     // Obs: The function evaluations are performed outside the inner most loop  //
     // (permutations are need).                                                 //
     //--------------------------------------------------------------------------//
void VCT::mfos(san_mat & m, const st_map * sm, const VCT & v, const aut_id a,
	       const bool b)
{
  //cout << "begin mfos\n";
  
 int i, ni     = v.sizes.sz(a);        // size of the automaton 'a'
 int l, nleft  = v.p_jumps.nleft(a);   // space size before the automaton 'a'
 int r, nright = v.p_jumps.nright(a);  // space size after the automaton 'a'
 int njump     = v.p_jumps.njump(a);   // 'nright' times 'ni'
 int pos       = v.p_order.pos(a);     // position of automaton 'a' in the list
 int idx, base = 0;                    // pointer to pick vector elements for the slices

 //cout << "ni, nleft, nright : " << ni << " " << nleft << " " << nright << "\n";

 g_st.reset();                                        // global state reset

 if (is_sparse) { // sparse vectors
   div_t zin_pos;
   int k2, j, j2, rr, dim;
   ///int jinit, jinit2;

   vector<gs_pair>::const_iterator iter;  // an iterator to traverse the 'zouts' vector
   gs_pair tmp;
   int_pair tmp2;
 
   j=0; j2=0;
   dim = v.size();
  
   if (!b) {
     zouts.reserve(dimension);
     k2=0;
   }
   
   if ((PRF::prf.z_impl() == zin_sparse)||(!b)) { // sparse zin

     info_pair tmpin, tmpout;
     vector<info_pair>::const_iterator izin, izout;
        // iterators to traverse the 'info_zin' and 'info_zout' vectors

     zin.reserve(ni);
     info2_zin.reserve(dim);
     if (b) info2_zout.reserve(dim);

     for (l=0; l<nleft; l++) {                            // for the left sub-space:
       m.jit_eval(g_st, a, sm);                               // evaluates the matrix
       
       // We create the info_zin array (informations on zin)
       while ((j<dim) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 tmp2.first=zin_pos.quot;
	 tmp2.second=j;
	 tmpin.first=zin_pos.rem;
	 tmpin.second=tmp2;
	 info2_zin.push_back(tmpin);
	 j++;
       }

       // We sort info_zin
       sort(info2_zin.begin(), info2_zin.end(), comp2());
       izin  = info2_zin.begin();

       if (b) { // b is true -> take into account the zout positions (that gives
	        // the non nul values
	 // We create the info_zout array (informations on zout)
	 while ((j2<dimension) && (positions[j2]<(l+1)*njump) && (positions[j2]!=-1)) {
	   zin_pos = div(positions[j2]-l*njump,nright);
	   tmp2.first=zin_pos.quot;
	   tmp2.second=j2;
	   tmpout.first=zin_pos.rem;
	   tmpout.second=tmp2;
	   info2_zout.push_back(tmpout);
	   j2++;
	 }
	 // We sort info_zout
	 sort(info2_zout.begin(), info2_zout.end(), comp2());
	 izout = info2_zout.begin();
	 
	 for (r=0;r<nright;r++) {
	   if ((*izout).first == r) {
	     zin.erase(zin.begin(), zin.end());
	     while ((*izin).first < r) ++izin; // we go to the good zin
	     while ((izin != info2_zin.end()) && ((*izin).first == r)) {
	       // Extraction of zin
	       tmp2 = (*izin).second;
	       tmp.first = tmp2.first;
	       tmp.second = v.vec[tmp2.second];
	       zin.push_back(tmp);
	       ++izin;
	     }
	     // sort the current zin
	     sort(zin.begin(),zin.end(), comp());
	     
	     // Now that we have the current zin, we compute each element
	     // of the resulting vector, and store the results in vec.
	     while ((izout != info2_zout.end()) && ((*izout).first == r)) {
	       tmp2 = (*izout).second;
	       vec[tmp2.second] = m.sparse_elem_left_mult2(zin, tmp2.first);
	       ++izout;
	     }
	   }
	 }
	 info2_zout.erase(info2_zout.begin(), info2_zout.end());      
       }
	 
       else {// b is false -> we have to find the non null values

	 for (r=0;r<nright;r++) {
	   if ((*izin).first == r) {
	     zin.erase(zin.begin(), zin.end());
	     while ((izin != info2_zin.end()) && ((*izin).first == r)) {
	       // Extraction of zin
	       tmp2 = (*izin).second;
	       tmp.first = tmp2.first;
	       tmp.second = v.vec[tmp2.second];
	       zin.push_back(tmp);
	       ++izin;
	     }
	     // sort the current zin
	     sort(zin.begin(),zin.end(), comp());
	     
	     // We have the current zin ; we multiply it with the matrix and
	     // find which element are not nul. Those elements are stored in zouts.
	     m.sparse_left_mult2(zouts, zin, r, nright);
	   }
	 }
	 
	 // put the zouts results in vec and positions
	 sort(zouts.begin(), zouts.end(), comp());
	 iter = zouts.begin();
	 
	 while ((iter != zouts.end()) && (k2<dimension)) {
	   //cout<< "element non nul a la position " <<(*iter).first + l*njump << "\n";
	   positions[k2]=(*iter).first + l*njump;
	   vec[k2]=(*iter).second;
	   ++iter; k2++;
	 }
	 if (k2==dimension)
	   cout<<"*** Got out of RSS : you should use full vector implementation ***\n";
	
	 zouts.erase(zouts.begin(), zouts.end());
       }

       // Erase everything
       info2_zin.erase(info2_zin.begin(), info2_zin.end());
       g_st.next(v.p_order, v.sizes, pos, Left);          // left-space block
     }
   
     if ((!b) && (k2<dimension))
       positions[k2]=-1;

   } // end sparse zin
   //----------------------------------------------------------------------------//
   else { // full zin implementation

     for (l=0; l<nleft; l++) {                        // for the left subspace
       m.jit_eval(g_st, a, sm);                           // evaluates the matrix
       
       // Initialisations of the array : null
       for (r=0; r<nright; r++) {
	 indice_r[r] = 0;
	 for (i=0; i<ni; i++)
	   zinf[r][i] = 0;
       }
       
       // We create the zinf array for the sparse multiplication
       while ((j<dimension) && (v.positions[j]<(l+1)*njump) && (v.positions[j]!=-1)) {
	 zin_pos = div(v.positions[j]-l*njump,nright);
	 rr = zin_pos.rem;
	 zinf[rr][zin_pos.quot] = v.vec[j];
	 j++;
       }
      
       while ((j2<dimension) && (positions[j2]<(l+1)*njump) && (positions[j2]!=-1)) {
	 zin_pos = div(positions[j2]-l*njump,nright);
	 rr = zin_pos.rem;
	 useful_f[rr][indice_r[rr]] = zin_pos.quot;
	 place[rr][indice_r[rr]] = j2;
	 indice_r[rr]++;
	 j2++;
       }
       
       // Here, we have all the zinf for the l portion ; we can make the 
       // vector-matrix multiplication in sparse : we compute each element
       // of the resulting vector, and store the results in vec.
       for (r=0; r<nright; r++)                         // for the right sub-space:
	 for (i=0; i<indice_r[r]; i++)
	   vec[place[r][i]] = m.sparse_elem_left_mult(zinf[r], useful_f[r][i]);
       
       g_st.next(v.p_order, v.sizes, pos, Left);          // left-space block
     } //end left subspace
   } // end full zin
 } // end sparse vector
 
  else { // full vector implementation
    for (l=0; l<nleft; l++) {                            // for the left sub-space:
        m.jit_eval(g_st, a, sm);                               // evaluates the matrix
        for (r=0; r<nright; r++) {                            // for the right sub-space:
            idx = base + r;                                     //
            for (i=0; i<ni; i++) {                              //
                z_in[i] = v.vec[idx];                             // gets the z_in slice
                idx += nright;                                    //
            }                                                   //
            m.left_mult(z_out, z_in);                           // z_out = z_in * matrix
            idx = base + r;                                     //
            for (i=0; i<ni; i++) {                              //
                vec[idx] = z_out[i];                              // stores the z_out slice
                idx += nright;                                    //
            }                                                   //
        }
        base += njump;                                     // pass to the next
        g_st.next(v.p_order, v.sizes, pos, Left);          // left-space block
    }
  }
}

     //--------------------------------------------------------------------------//
     //                  Internal Function of Multiply by a CND                  //
     //                                                                          //
     //      It multiplies a vector 'v' by a normal factor with functions (non   //
     // identity matrix in 'm') storing the result in the vector 'this'.         //
     //                                                                          //
     // Obs: The permutation of the input vector is performed before this        //
     // function is called, the function evaluation is performed outside the     //
     // inner most loop (no permutations are need in this function).             //
     //--------------------------------------------------------------------------//
void VCT::mnps(san_mat & m, const st_map * sm, const VCT & v, const aut_id a,
               const int nleft, const int nright, const aut_id pos, const bool b)
{
  
  //cout << "begin mnps\n";

 int i, ni = v.sizes.sz(a);    // size of the automaton 'a'
 int l; // nleft               // space size of the parameters of automaton 'a'
 int r; // nright              // space size of the non parameters of automaton 'a'
 int njump = nright * ni;      // 'nright' times 'ni'
        // pos                 // position of the first non parameter of 'a' in the list
 int base = 0;                 // pointer to pick vector elements for the slices
 
 //cout << "ni, nleft, nright : " << ni << " " << nleft << " " << nright << "\n";

 g_st.reset();                                        // global state reset

 if (is_sparse) { // sparse vectors
   ///div_t zin_pos;
   int dim = v.size();
   int j=0, j2=0, k2=0;
   
   vector<gs_pair>::const_iterator iter; // an iterator to traverse zouts
   
   if (!b) zouts.reserve(dimension);
   
   for (l=0; l<nleft; l++) {                            // for the left sub-space:
     m.jit_eval(g_st, a, sm);                               // evaluates the matrix
          
     for (r=0; r<nright; r++) {                            // for the right sub-space:
       // initialize the z_in
       for (i=0;i<ni;i++)
	 z_in[i] = 0;
       
       // extract the z_in
       while ((j<dim)&&(v.positions[j]<(base+ni)) && (v.positions[j]!=-1)) {
	 z_in[v.positions[j]-base] = v.vec[j];
	 j++;
       }
       
       if (b)       
	 // vector/matrix multiplication (elt/column)
	 while ((j2<dimension)&&(positions[j2]<(base+ni)) && (positions[j2]!=-1)) {
	   vec[j2] = m.sparse_elem_left_mult(z_in, positions[j2]-base);
	   j2++;
	 }
       else {
	 //cout << "l = " << l << " , base = " << base << "\n";
	 m.sparse_left_mult2(zouts, z_in, base);       
       }
       
       base += ni;
     } // end of right space
     
     if (!b) {
       // Get the result of zouts in the vector 'this'
       iter = zouts.begin();
       while ((iter != zouts.end()) && (k2<dimension)) {
	 positions[k2] = (*iter).first;
	 vec[k2] = (*iter).second;
	 ++iter; k2++;
       }
       if (k2==dimension)
	 cout << "*** Got out of RSS : you should use full vector implementation ***\n";
     }

     if (!b) zouts.erase(zouts.begin(), zouts.end());     
     g_st.next(v.p_order, v.sizes, pos, Left);          // pass to the next
   }                                                  // left-space block
   if ((!b) && (k2<dimension))
     positions[k2] = -1;
   
 } // end sparse vectors
 //-------------------------------------------------------------------------------//
else { // full vector implementation
   for (l=0; l<nleft; l++) {                            // for the left sub-space:
     m.jit_eval(g_st, a, sm);                               // evaluates the matrix
     for (r=0; r<nright; r++) {                            // for the right sub-space:
       m.left_mult(z_out, &(v.vec[base]));                 // z_out = slice * matrix
       for (i=0; i<ni; i++) {                               //
	 vec[base+i] = z_out[i];                           // stores the z_out slice
       }
       base += ni;                                         //
     }
     g_st.next(v.p_order, v.sizes, pos, Left);          // pass to the next
   }                                                    // left-space block
 }

 //cout << "end of mnps : 'this' \n";
 //debug(); 
}

     //--------------------------------------------------------------------------//
     //             Multiply by a LUD term  -   'this = this * LUD^1'            //
     //--------------------------------------------------------------------------//
void VCT::solv_LUD()
{
 aut_id a;                          // counter for the automata matrices
 ev_id  e;                          // pointer for the event term (if not local)
 // int    k;                          // counter for the vector positions

 if (LUD::lud.syn_term == no_ev) {            // the "term of the turn" is local
     a = LUD::lud.loc_term;
     solv(LUD::lud.local_mat[a], LUD::lud.local_mat2[a], LUD::lud.sizes.sz(a), LUD::lud.p_jumps.nleft(a),
          LUD::lud.p_jumps.nright(a), LUD::lud.p_jumps.njump(a));
 }
 else {                                       // the "term of the turn" is synch.
   e = LUD::lud.syn_term;
   for (a=0; a<LUD::lud.automata; a++)
     if (LUD::lud.pm_term == Mat_plus)           // the "term of the turn" is Mat_plus
       solv(LUD::lud.synch_mat[e][a],
            LUD::lud.synch_mat2[e][a],  LUD::lud.sizes.sz(a),
            LUD::lud.p_jumps.nleft(a), LUD::lud.p_jumps.nright(a),
            LUD::lud.p_jumps.njump(a));
     else                                        // the "term of the turn" is Mat_minus
       solv(LUD::lud.synch_adj[e][a],
            LUD::lud.synch_adj2[e][a],  LUD::lud.sizes.sz(a),
            LUD::lud.p_jumps.nleft(a), LUD::lud.p_jumps.nright(a),
            LUD::lud.p_jumps.njump(a));
 }
}

     //--------------------------------------------------------------------------//
     //             Multiply by a LUD term  -   'this = v * LUD^1'               //
     //--------------------------------------------------------------------------//
void VCT::solv_LUD(const VCT & v)
{
 aut_id a;                          // counter for the automata matrices
 ev_id  e;                          // pointer for the event term (if not local)
 // int    k;                          // counter for the vector positions

 if (LUD::lud.syn_term == no_ev) {            // the "term of the turn" is local
     a = LUD::lud.loc_term;
     solv(v, LUD::lud.local_mat[a], LUD::lud.local_mat2[a],
             LUD::lud.sizes.sz(a), LUD::lud.p_jumps.nleft(a),
             LUD::lud.p_jumps.nright(a), LUD::lud.p_jumps.njump(a));
 }
 else {                                       // the "term of the turn" is synch.
   e = LUD::lud.syn_term;
   for (a=0; a<LUD::lud.automata; a++)
     if (LUD::lud.pm_term == Mat_plus)           // the "term of the turn" is Mat_plus
       solv(v, LUD::lud.synch_mat[e][a],
               LUD::lud.synch_mat2[e][a], LUD::lud.sizes.sz(a),
               LUD::lud.p_jumps.nleft(a), LUD::lud.p_jumps.nright(a),
               LUD::lud.p_jumps.njump(a));
     else                                        // the "term of the turn" is Mat_minus
       solv(v, LUD::lud.synch_adj[e][a],
               LUD::lud.synch_adj2[e][a], LUD::lud.sizes.sz(a),
               LUD::lud.p_jumps.nleft(a), LUD::lud.p_jumps.nright(a),
               LUD::lud.p_jumps.njump(a));
 }
}

     //--------------------------------------------------------------------------//
     //             Multiply by a LUD term  -   'this += v * LUD^1'              //
     //--------------------------------------------------------------------------//
void VCT::solv_LUD_P(const VCT & v)
{
 aut_id a;                          // counter for the automata matrices
 ev_id  e;                          // pointer for the event term (if not local)
 //int    k;                          // counter for the vector positions

 if (LUD::lud.syn_term == no_ev) {            // the "term of the turn" is local
   a = LUD::lud.loc_term;
   solv_P(LUD::lud.local_mat[a], LUD::lud.local_mat2[a], v, LUD::lud.sizes.sz(a), LUD::lud.p_jumps.nleft(a),
          LUD::lud.p_jumps.nright(a), LUD::lud.p_jumps.njump(a));
 }
 else {                                       // the "term of the turn" is synch.
   e = LUD::lud.syn_term;
   for (a=0; a<LUD::lud.automata; a++)
     if (LUD::lud.pm_term == Mat_plus)           // the "term of the turn" is Mat_plus
       solv_P(LUD::lud.synch_mat[e][a],
              LUD::lud.synch_mat2[e][a], v,  LUD::lud.sizes.sz(a),
              LUD::lud.p_jumps.nleft(a), LUD::lud.p_jumps.nright(a),
              LUD::lud.p_jumps.njump(a));
     else                                        // the "term of the turn" is Mat_minus
       solv_P(LUD::lud.synch_adj[e][a],
              LUD::lud.synch_adj2[e][a], v,  LUD::lud.sizes.sz(a),
              LUD::lud.p_jumps.nleft(a), LUD::lud.p_jumps.nright(a),
              LUD::lud.p_jumps.njump(a));
 }
}

     //--------------------------------------------------------------------------//
     //                Internal Function of Multiply by a LUD term               //
     //                                                                          //
     //      It solves 'this' by a normal factor (non identity matrix in 'm')    //
     // storing the result in the same vector 'this'.                            //
     //--------------------------------------------------------------------------//
void VCT::solv(const sp_mat & m, const sp_mat & m2, const int ni,
               const int nleft, const int nright, const int njump)
{
 int i;                     // counters for space size of the automaton 'a'
 int l;                     // counters for space size before the automaton 'a'
 int r;                     // counters for space size after the automaton 'a'
 int idx, base = 0;         // pointers to pick vector elements for the slices

 for (l=0; l<nleft; l++) {                            // for the left sub-space:
   for (r=0; r<nright; r++) {                            // for the right sub-space:
     idx = base + r;                                     //
     for (i=0; i<ni; i++) {                              //
       z_in[i] = vec[idx];                               // gets the z_in slice
       idx += nright;                                    //
     }                                                   //
     if (PRF::prf.reg_type() == tychv_reg) {
       m2.left_mult(z_out, z_in);                           // tychonov option
       m.LU_solve(z_out, z_out);                            // 
     }
     else
       m.LU_solve(z_in, z_out);                          // solve z_in = z_out * matrix
     idx = base + r;                                     // 
     for (i=0; i<ni; i++) {                              //
       vec[idx] = z_out[i];                              // stores the z_out slice
       idx += nright;                                    //
     }                                                   //
   }
   base += njump;                                     // pass to the next
 }                                                    // left-space block
}
     //--------------------------------------------------------------------------//
     //                Internal Function of Multiply by a LUD term               //
     //                                                                          //
     //      It solves 'this' by a normal factor (non identity matrix in 'm')    //
     // storing the result in the same vector 'this'.                            //
     //--------------------------------------------------------------------------//
void VCT::solv(const VCT v, const sp_mat & m, const sp_mat & m2, const int ni,
               const int nleft, const int nright, const int njump)
{
 int i;                     // counters for space size of the automaton 'a'
 int l;                     // counters for space size before the automaton 'a'
 int r;                     // counters for space size after the automaton 'a'
 int idx, base = 0;         // pointers to pick vector elements for the slices

 for (l=0; l<nleft; l++) {                            // for the left sub-space:
   for (r=0; r<nright; r++) {                            // for the right sub-space:
     idx = base + r;                                     //
     for (i=0; i<ni; i++) {                              //
       z_in[i] = v.vec[idx];                             // gets the z_in slice
       idx += nright;                                    //
     }                                                   //
     if (PRF::prf.reg_type() == tychv_reg) {
       m2.left_mult(z_out, z_in);                           // tychonov option
       m.LU_solve(z_out, z_out);                            // 
     }
     else
       m.LU_solve(z_in, z_out);                          // solve z_in = z_out * matrix
     idx = base + r;                                     // 
     for (i=0; i<ni; i++) {                              //
       vec[idx] = z_out[i];                              // stores the z_out slice
       idx += nright;                                    //
     }                                                   //
   }
   base += njump;                                     // pass to the next
 }                                                    // left-space block
}
     //--------------------------------------------------------------------------//
     //                Internal Function of Multiply by a LUD term               //
     //                                                                          //
     //      It solves 'v' by a normal factor (non identity matrix in 'm')       //
     // adding the result in the vector 'this'.                                  //
     //--------------------------------------------------------------------------//
void VCT::solv_P(const sp_mat & m, const sp_mat & m2, const VCT & v, const int ni,
                 const int nleft, const int nright, const int njump)
{
 int i;                     // counters for space size of the automaton 'a'
 int l;                     // counters for space size before the automaton 'a'
 int r;                     // counters for space size after the automaton 'a'
 int idx, base = 0;         // pointers to pick vector elements for the slices

 for (l=0; l<nleft; l++) {                            // for the left sub-space:
   for (r=0; r<nright; r++) {                            // for the right sub-space:
     idx = base + r;                                     //
     for (i=0; i<ni; i++) {                              //
       z_in[i] = v.vec[idx];                             // gets the z_in slice
       idx += nright;                                    //
     }                                                   //
     if (PRF::prf.reg_type() == tychv_reg) {
       m2.left_mult(z_out, z_in);                           // tychonov option
       m.LU_solve(z_out, z_out);                            // 
     }
     else
       m.LU_solve(z_in, z_out);                          // solve z_in = z_out * matrix
     idx = base + r;                                     // 
     for (i=0; i<ni; i++) {                              //
       vec[idx] += z_out[i];                             // adds the z_out slice
       idx += nright;                                    //
     }                                                   //
   }
   base += njump;                                     // pass to the next
 }                                                    // left-space block
}

      //--------------------------------------------------------------------------//
      //      Multiply by a CND  -  generic switch among methods A, B, C and M    //
      //--------------------------------------------------------------------------//
void VCT::mult_CND(const VCT & v)
{
  int k;

  if (v.is_sparse != is_sparse)
    cout << "mult_CND : different implementation type for vectors\n";

  //cout << "debut de mult_CND\n";

  if (is_sparse)
    for (k=0; k<dimension; k++) {                    // dot product of the diagonal
      positions[k] = v.positions[k];
      vec[k] = v.vec[k] * CND::cnd.diagonal[k];
      //      cout << "position " << positions[k]
      //	   << "  -  elt diagonal " << CND::cnd.diagonal[k] << "\n";
   }
  else
    for (k=0; k<CND::cnd.state_space; k++) {         // dot product of the diagonal
      vec[k] = v.vec[k] * CND::cnd.diagonal[k];
      //      cout << "elt diagonal " << CND::cnd.diagonal[k] << "\n";
    }

  //cout << "Appel a mult_CND_x\n";

  switch(PRF::prf.method()) {                        // switch methods A, B, C or M
  case meth_A : mult_CND_A(v); break;
  case meth_B : mult_CND_B(v); break;
  case meth_C : mult_CND_C(v); break;
  case meth_S : mult_CND_S(v); break;
  case meth_M : mult_CND_M(v);
  }
  
}



     //--------------------------------------------------------------------------//
     //                Multiply by the inverse of the CND diagonal               //
     //--------------------------------------------------------------------------//
void VCT::solv_D()
{
  int k;

  if (is_sparse)
    for (k=0; k<dimension; k++)       // dot product of the inverse of
      vec[k] /= CND::cnd.diagonal[positions[k]];                //    the diagonal
  else
    for (k=0; k<dimension; k++)       // dot product of the inverse of
      vec[k] /= CND::cnd.diagonal[k];                //    the diagonal
}

     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // Alternated    - uses the term of the turn of the 'LUD::lud' structure as //
     //                 preconditionner.                                         //
     //                                             'this = (v * CND) * LUD^-1'  //
     //--------------------------------------------------------------------------//
void VCT::pre_altn(const VCT & v)
{
 mult_CND(v);                          // it performs 'this = v * CND'
 solv_LUD();                           // it performs 'this = (v * CND) * LUD^-1'
}
     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // Aditive        - uses the aditive combination of all 'LUD::lud' terms as //
     //                  preconditionner.                                        //
     //                                  'this' = 'v' * 'CND' * sum of 'LUD^-1'  //
     //--------------------------------------------------------------------------//
void VCT::pre_adtv(const VCT & v)
{
 int n_terms = LUD::lud.v_terms();

 mult_CND(v);                          // it performs 't = v * CND'
 t.copy(*this);
 LUD::lud.reset_turn();                // for the first valid 'LUD::lud' term
 solv_LUD(t);                             // it performs 'this = (v * CND) * LUD^-1'
 for (int i=1; i<n_terms; i++) {       // for the other valid 'LUD::lud' terms
   LUD::lud.next_turn();
   solv_LUD_P(t);                         // it performs 'this += (v * CND) * LUD^-1'
 }
}
     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // Multiplicative - uses the multiplicative combination of all 'LUD::lud'   //
     //                  terms as preconditionner.                               //
     //                                 'this' = 'v' * 'CND' * prod of 'LUD^-1'  //
     //--------------------------------------------------------------------------//
void VCT::pre_mult(const VCT & v)
{
 int n_terms = LUD::lud.v_terms();

 LUD::lud.reset_turn();
 mult_CND(v);                          // it performs 'this = v * CND'
 solv_LUD();                           // 'this = (v * CND) * LUD^-1' to the first term
 for (int i=1; i<n_terms; i++) {       // for each term from the second to the last
   LUD::lud.next_turn();
   mult_CND(*this);                       // it performs 'this *= CND' 
   solv_LUD_P(*this);                     // it performs 'this += (this * CND) * LUD^-1'
 }
}
     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // Diagonal       - uses the diagonal of the 'CND::cnd' descriptor as       //
     //                  preconditionner.                                        //
     //                                          'this = (v * CND) * diag(CND)'  //
     //--------------------------------------------------------------------------//
void VCT::pre_diag(const VCT & v)
{
 mult_CND(v);                          // it performs 'this = v * CND'
 solv_D();                             // it performs 'this = (v * CND) * diag(CND)'
}
     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // Polynomial     - uses the 'CND::cnd' as the preconditionner and          //
     //                  aproximates its inverse by the polynome:                //
     //                  'I + CND + CND^2 + CND^3 + ...'                         //
     //                              'this = v * CND * (I + CND + CND^2 + ...')  //
     //--------------------------------------------------------------------------//
void VCT::pre_plyn(const VCT & v)
{
 int n_terms = PRF::prf.poly_k();
 VCT zz(dimension, real_dim);

 mult_CND(v);                          // it performs 'zz = this = v * CND'
 zz.copy(*this);
 for (int i=1; i<n_terms; i++) {       // for all polynomial terms
   mult_CND(*this);                       // it performs 'this *= CND'
   add(zz);                               // it performs 'this = (v * CND) + this'
 }
}
     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // I-A Polynomial - uses the Identity - 'CND::cnd' as the preconditionner   //
     //                  and aproximates its inverse by the polynome:            //
     //                  'I + (I - CND) + (I - CND)^2 + (I - CND)^3 + ...'       //
     //                  'this = v * CND * (I + (I - CND) + (I - CND)^2 + ...')  //
     //--------------------------------------------------------------------------//
void VCT::pre_ipln(const VCT & v)
{
 int n_terms = PRF::prf.poly_k();
 int k;
 rp  buf;
 VCT zz(dimension, real_dim);
 VCT ww(dimension, real_dim);

 if (is_sparse)
   cout << "pre_ipln not yet implemented in sparse\n";

 mult_CND(v);                             // it performs 'this = v * CND'
 for (k=0; k<dimension; k++)              // it performs 'zz = v * CND * (I - CND)'
   zz.vec[k] = v.vec[k] - vec[k];
 for (int i=0; i<n_terms; i++) {          // for all polynomial terms 
   ww.mult_CND(zz);                          // it performs 'ww = zz * CND'
   for (k=0; k<dimension; k++) {
     buf = zz.vec[k] - ww.vec[k];
     vec[k] += buf;                          // it performs 'this += zz * (I - CND)'
     zz.vec[k] = buf;                        // it performs 'zz = zz * (I - CND)'
   }
 }
}
     //--------------------------------------------------------------------------//
     // Preconditionned Multiplications (multiplications + solutions)            //
     //--------------------------------------------------------------------------//
     // Chebyshev      - uses ??????????????????????????                         //
     //                                    'this = v * (I + CND + CND^2 + ...')  //
     //--------------------------------------------------------------------------//
void VCT::pre_cheb(const VCT & v)
{
 int n_terms = PRF::prf.poly_k();
 VCT zz(dimension, real_dim);

 if (is_sparse)
   cout << "pre_cheb not yet implemented in sparse\n";

 mult_CND(v);                          // it performs 'this = v * CND'
 for (int i=1; i<n_terms; i++) {       // for all polynomial terms
   zz.copy(*this);                        // it copies 'x^(k-1)'
   mult_CND(*this);                       // it performs 'this *= CND'
   scale_add(zz, -one);                   // it performs 'this = (v * CND) + this'
 }
}

      //--------------------------------------------------------------------------//
      //      Multiply by a CND  -  generic switch among methods A, B, C and M    //
      //--------------------------------------------------------------------------//
void VCT::setup_aunfs() {
    aunfs = new aunfs_list[CND::cnd.ev()];
    for (int e = 0; e < CND::cnd.ev(); e++) {
        int cut = CND::cnd.get_fcut(e);
        if (cut > 0) {
            create_aunfs(e, cut);
        }
    }
    //print_aunfs();
}

      //--------------------------------------------------------------------------//
      //   For all partial global states in the sparse side, creates all aunfs    //
      //--------------------------------------------------------------------------//
void VCT::create_aunfs(const int e, const int cut) {
    vector<aunf> vaunfs;
    rp scalar;
    int basein, baseout;
    aut_st vv(cut);               // an auxiliary vector to save the non-zeroed elements
    aut_st pgs(cut);              // a partial global state of size 'cut'
    int nz, a = cut - 1;

    //initialize the nonzero counter for each k initial matrices
    int nzaunf = 1;
    int id_aunf = 0;

    for (int i = 0; i < cut; i++) {
        int ai = CND::cnd.s_B_order[e].aut(i);
        int nz = (i == 0) ? CND::cnd.get_synch_mat(e, ai).nz() : CND::cnd.get_synch_mat(e, ai).nz() - 1;
        vv.stamp(i, nz);
        pgs.stamp(i, 0);
        nzaunf *= CND::cnd.get_synch_mat(e, ai).nz(); //saves total number of aunfs for this event
    }

    aunfs[e].size = nzaunf;
    aunfs[e].vaunf = new aunf[nzaunf];
    //TODO: improve the algorithm...
    //discovers the AUNFs
    pgs.stamp(cut - 1, -1); // :(((
    //other positions...
    while (pgs.val(0) != vv.val(0)) {
        if (pgs.val(a) < vv.val(a)) {
            pgs.stamp(a, pgs.val(a) + 1);
            a = cut - 1;
            if (pgs.val(0) != vv.val(0)) {
                aut_st pgs_aux(cut); // an auxiliary vector to save the partial global state defined inside get_scalar()
                aut_st gs(CND::cnd.automata);
                for (int k = 0; k < cut; k++) { //create a global state with the partial global state
                    int ai = CND::cnd.s_B_order[e].aut(k);
                    gs.stamp(ai, pgs.val(k));
                }
                scalar = get_scalar(e, gs, &basein, &baseout, &pgs_aux, cut, CND::cnd.orig_jumps);
                aunfs[e].vaunf[id_aunf].scalar = scalar;
                aunfs[e].vaunf[id_aunf].basein = basein;
                aunfs[e].vaunf[id_aunf].baseout = baseout;
                aunfs[e].vaunf[id_aunf].pgs = pgs_aux;
                id_aunf++;
            }
        } else {
            a--;
            //restart from a+1 till the end with zeroes
            for (int i = a + 1; i < cut; i++) {
                nz = (pgs.val(i) != -1) ? 0 : -1;
                pgs.stamp(i, nz);
            }
        }
    }
    CND::cnd.add_aunf(aunfs[e].vaunf, aunfs[e].size);
}

      //--------------------------------------------------------------------------//
      //      Print all aunfs for all events                                      //
      //--------------------------------------------------------------------------//
void VCT::print_aunfs() {   
    string name;

    name.append(file_name)  ;
    name.append(".aunfslist");
    ofstream out(name.c_str());

    for (int e = 0; e < CND::cnd.ev(); e++) {
        if (CND::cnd.get_fcut(e) != 0) {
            out << endl << "event = ";
            out << left << setw(3) << e << endl;
            for (int i = 0; i < aunfs[e].size; i++) {
                out << " -- basein  = ";
                out << left << setw(15) << aunfs[e].vaunf[i].basein;
                out << " baseout = ";
                out << left << setw(15) << aunfs[e].vaunf[i].baseout;
                out << " scalar = ";
                out << left << setw(15) << aunfs[e].vaunf[i].scalar;
                out << " partialglobalstate = [ ";
                for (int j = 0; j < aunfs[e].vaunf[i].pgs.size(); j++) {
                    out << " " << left << setw(2) << aunfs[e].vaunf[i].pgs.val(j);
                }
                out << " ]" << endl;
            }
        }
    }
}


