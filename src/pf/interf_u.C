//====================================================================================//
//                                                                                    //
//                PEPS User Interface for UNIX versions Implementation                //
//                                                                                    //
//====================================================================================//
//  This File:   interf.C                   Language: C++  (xlC and CC)               //
//  Software:    PEPS 2002                   Advisor: Brigitte.Plateau@imag.fr        //
//  Doc:         interf.tex                                                           //
//====================================================================================//
//  Creation:    19/dec/95                        by: Paulo.Fernandes@imag.fr         //
//  Last Change: 17/feb/03                        by: lbrenner@inf.pucrs.br           //
//====================================================================================//
#include "peps.h"

//====================================================================================//
//   Peps Name Definitions                                                            //
//====================================================================================//

const peps_name no_name = "\0";         // Default empty value for peps_name

//====================================================================================//
//  Communicate Out of Memory                                                         //
//====================================================================================//
void Communicate_Out_of_Memory()
{
  cerr << "\n\n********************************************************************"
       <<   "\nThere is not enough virtual memory to run this operation in PEPS!"
       << "\n\nSometimes this problem can be solved by removing the limits of your "
       <<     "shell. If your using a UNIX system type 'man limit' to see how this "
       <<     "can be done. For example, you may try: 'unlimit vmemoryuse' and run "
       <<     "PEPS again."
       << "\n\nIn any case, this memory shortage is forcing PEPS to exit now!\n"
       <<   "\n********************************************************************\n";
  exit(1);
}
//====================================================================================//
//  Internal Function Add_ExtensionS                                                  //
//====================================================================================//
void Add_ExtensionS(const peps_name file, const file_types ft, peps_name tmp)
{
 strcpy(tmp, file);
 switch (ft) {
 case san_file : strcat(tmp, ".san");
                 break;
 case des_file : strcat(tmp, ".des");
                 break;
 case tft_file : strcat(tmp, ".tft");
                 break;
 case fct_file : strcat(tmp, ".fct");
                 break;
 case dic_file : strcat(tmp, ".dic");
                 break;
 case res_file : strcat(tmp, ".res");
                 break;
 case dtr_file : strcat(tmp, ".sp.dtr");
                 break;
 case rea_file : strcat(tmp, ".sp.rea");
                 break;
 case inf_file : strcat(tmp, ".sp.inf");
                 break;
 case dct_file : strcat(tmp, ".sp.dct");
                 break;
 case dsc_file : strcat(tmp, ".sp.dsc");
                 break;
 case cnd_file : strcat(tmp, ".sp.cnd");
                 break;
 case lud_file : strcat(tmp, ".sp.lud");
                 break;
 case vct_file : strcat(tmp, ".vct");
                 break;
 case rss_file : strcat(tmp, ".sp.rss");
                 break;
 case hbf_file : strcat(tmp, ".hbf");
                 break;
 case ftb_file : strcat(tmp, ".sp.ftb");
                 break;
 case prf_file : strcat(tmp, ".prf");
                 break;
 case ops_file : strcat(tmp, ".sp.ops");
                 break;
 case tim_file : strcat(tmp, ".sp.tim");
                 break;
 case dbg_file : strcat(tmp, ".dbg");
                 break;
 case jit_file : strcat(tmp, ".C");
                 break;
 case obj_file : strcat(tmp, ".o");
                 break;
 case so__file : strcat(tmp, ".so");
                 break;
 case hlp_file : strcat(tmp, ".hlp");
                 break;
 case dot_file : // no extension!
                 break;
 default : strcat(tmp, ".err");
 }
}
//====================================================================================//
//  Internal Function Add_ExtensionF                                                  //
//====================================================================================//
void Add_ExtensionF(const peps_name file, const file_types ft, peps_name tmp)
{
 strcpy(tmp, file);
 switch (ft) {
 case san_file : strcat(tmp, ".san");
                 break;
 case des_file : strcat(tmp, ".des");
                 break;
 case tft_file : strcat(tmp, ".tft");
                 break;
 case fct_file : strcat(tmp, ".fct");
                 break;
 case dic_file : strcat(tmp, ".dic");
                 break;
 case res_file : strcat(tmp, ".res");
                 break;
 case dtr_file : strcat(tmp, ".dtr");
                 break;
 case rea_file : strcat(tmp, ".rea");
                 break;
 case inf_file : strcat(tmp, ".inf");
                 break;
 case dct_file : strcat(tmp, ".dct");
                 break;
 case dsc_file : strcat(tmp, ".dsc");
                 break;
 case cnd_file : strcat(tmp, ".cnd");
                 break;
 case lud_file : strcat(tmp, ".lud");
                 break;
 case vct_file : strcat(tmp, ".vct");
                 break;
 case rss_file : strcat(tmp, ".rss");
                 break;
 case hbf_file : strcat(tmp, ".hbf");
                 break;
 case ftb_file : strcat(tmp, ".ftb");
                 break;
 case prf_file : strcat(tmp, ".prf");
                 break;
 case ops_file : strcat(tmp, ".ops");
                 break;
 case tim_file : strcat(tmp, ".tim");
                 break;
 case dbg_file : strcat(tmp, ".dbg");
                 break;
 case jit_file : strcat(tmp, ".C");
                 break;
 case obj_file : strcat(tmp, ".o");
                 break;
 case so__file : strcat(tmp, ".so");
                 break;
 case hlp_file : strcat(tmp, ".hlp");
                 break;
 case dot_file : // no extension!
                 break;
 default : strcat(tmp, ".err");
 }
}
//====================================================================================//
//  Internal Function Add_Extension                                                   //
//====================================================================================//
void Add_Extension(const peps_name file, const file_types ft, peps_name tmp)
{
  if (PRF::prf.vec_impl() == v_full)
    Add_ExtensionF(file, ft, tmp);
  else
    Add_ExtensionS(file, ft, tmp);
}
//====================================================================================//
//  Open_File (to input)                                                              //
//====================================================================================//
void Open_File(const peps_name file, const file_types ft, ifstream & f_tmp)
{
 peps_name  n_tmp;

 Add_Extension(file, ft, n_tmp);

 // f_tmp.open(n_tmp, ios::nocreate);
 f_tmp.open(n_tmp);
 if (!f_tmp)
   cerr << "System Error - Cannot open the file '" << n_tmp << "' (#### 0001 ####)";
 else
   f_tmp.precision(peps_precision);
}

//====================================================================================//
//  Open_File (to output)                                                             //
//====================================================================================//
void Open_File(const peps_name file, const file_types ft, ofstream & f_tmp)
{
 peps_name  n_tmp;

 Add_Extension(file, ft, n_tmp);
 f_tmp.open(n_tmp);
 if (!f_tmp)
   cerr << "System Error - Cannot open the file '" << n_tmp << "' (#### 0002 ####)";
 else
   f_tmp.precision(peps_precision);
}
//====================================================================================//
//  Append_File (to output)                                                           //
//====================================================================================//
void Append_File(const peps_name file, const file_types ft, ofstream & f_tmp)
{
 peps_name  n_tmp;

 Add_Extension(file, ft, n_tmp);
 f_tmp.open(n_tmp, ios::app);
 if (!f_tmp)
   cerr << "System Error - Cannot open the file '" << n_tmp << "' (#### 0003 ####)";
 else
   f_tmp.precision(peps_precision);
}
//====================================================================================//
//  Close_File (to input)                                                             //
//====================================================================================//
void Close_File(const peps_name file, const file_types ft, ifstream & f_tmp)
{
 peps_name  n_tmp;

 Add_Extension(file, ft, n_tmp);
 f_tmp.close();
 if (!f_tmp)
   cerr << "System Error - Cannot close the file '" << n_tmp << "' (#### 0004 ####)";
 else
   cout << " :-) file '" << n_tmp << "' read\n";
}
//====================================================================================//
//  Close_File (to output)                                                            //
//====================================================================================//
void Close_File(const peps_name file, const file_types ft, ofstream & f_tmp)
{
 peps_name  n_tmp;

 Add_Extension(file, ft, n_tmp);
 f_tmp.close();
 if (!f_tmp)
   cerr << "System Error - Cannot close the file '" << n_tmp << "' (#### 0005 ####)";
 else
   cout << " :-) file '" << n_tmp << "' saved\n";
}
//====================================================================================//
//  Exists_File                                                                       //
//====================================================================================//
bool Exists_File(const peps_name file, const file_types ft)
{
 ifstream  f_tmp;
 peps_name n_tmp;

 Add_Extension(file, ft, n_tmp);
 // f_tmp.open(n_tmp, ios::nocreate);
 f_tmp.open(n_tmp);
 if (f_tmp) {
   return(true);
   f_tmp.close();
 }
 else
   return(false);
}
//====================================================================================//
//  Get Current Dir                                                                   //
//====================================================================================//
void Get_Current_Dir(peps_name & name)
{
 ifstream  f_tmp;

 system("pwd > ./peps_tmp");
 // f_tmp.open("./peps_tmp", ios::nocreate);
 f_tmp.open("./peps_tmp");
 if (f_tmp) {
   f_tmp >> name;
   f_tmp.close();
   system("rm -f peps_tmp");
 }
 else
   cerr << "System Error - Cannot create the file './jit/peps_tmp' (#### 0006 ####)";
}
//====================================================================================//
//  Create Temporary Directories                                                      //
//====================================================================================//
void Create_Temporary_Directories()
{
 peps_name cur_dir;

 Get_Current_Dir(cur_dir);
 if (!Exists_File("des/peps_des", dot_file)) {
   system("rm -rf des");
   system("mkdir des");
   system("pwd > des/peps_des");
 }
 if (!Exists_File("dsc/peps_dsc", dot_file)) {
   system("rm -rf dsc");
   system("mkdir dsc");
   system("pwd > dsc/peps_dsc");
 }
 if (!Exists_File("agg/peps_agg", dot_file)) {
   system("rm -rf agg");
   system("mkdir agg");
   system("pwd > agg/peps_agg");
 }
 if (!Exists_File("cnd/peps_cnd", dot_file)) {
   system("rm -rf cnd");
   system("mkdir cnd");
   system("pwd > cnd/peps_cnd");
 }
 if (!Exists_File("jit/peps_jit", dot_file)) {
   system("rm -rf jit");
   system("mkdir jit");
   system("pwd > jit/peps_jit");
 }
}
//====================================================================================//
//  Remove Temporary Directories                                                      //
//====================================================================================//
void Remove_Temporary_Directories()
{
 system("rm -rf des");
 system("rm -rf dsc");
 system("rm -rf agg");
 system("rm -rf cnd");
 system("rm -rf jit");
}
//====================================================================================//
//  Remove Probability Vectors                                                        //
//====================================================================================//
void Remove_Probability_Vectors()
{
 system("rm -f *.vct");
}
//====================================================================================//
//  Remove Tim Files                                                                  //
//====================================================================================//
void Remove_Tim_Files()
{
 system("rm -f *.tim");
}
//====================================================================================//
//  Ask_a_File_Name                                                                   //
//====================================================================================//
void Ask_a_File_Name(peps_name & name, const file_types ft)
{
 cout << "\nEnter ";
 switch (ft) {
 case san_file : cout << "textual SAN";
                 break;
 case des_file : cout << "textual descriptor";
                 break;
 case tft_file : cout << "textual table of functions";
                 break;
 case fct_file : cout << "textual reachability function";
                 break;
 case dic_file : cout << "textual dictionnary";
                 break;
 case res_file : cout << "textual table of integration functions";
                 break;
 case dtr_file : cout << "readable dictionnary";
                 break;
 case rea_file : cout << "reachable states of probability vector";
                 break;
 case inf_file : cout << "table of integration functions";
                 break;
 case dct_file : cout << "dictionnary";
                 break;
 case dsc_file : cout << "descriptor";
                 break;
 case cnd_file : cout << "normalized descriptor";
                 break;
 case lud_file : cout << "ILU descriptor";
                 break;
 case vct_file : cout << "vector";
                 break;
 case rss_file : cout << "state space";
                 break;
 case hbf_file : cout << "sparse matrix";
                 break;
 case ftb_file : cout << "table of functions";
                 break;
 case prf_file : cout << "user preferences";
                 break;
 case ops_file : cout << "misfunctions";
                 break;
 case tim_file : cout << "time report";
                 break;
 case dbg_file : cout << "debugging";
                 break;
 case jit_file : cout << "just-in-time";
                 break;
 case obj_file : cout << "object";
                 break;
 case so__file : cout << "share object";
                 break;
 case hlp_file : cout << "help";
                 break;
 case dot_file : cout << "MARCA vector";
                 break;
 default : cout << "unrecognized";
 }
 cout << " file name: ";
 cin >> name;
}
//====================================================================================//
//  Ask_an_Existing_File_Name                                                         //
//====================================================================================//
void Ask_an_Existing_File_Name(peps_name & name, const file_types ft)
{
 bool exists;

 do {
   Ask_a_File_Name(name, ft);
   exists = Exists_File(name, ft);
     if (!exists)
       cout << "This file can not be found!";
 } while (!exists);
}
//====================================================================================//
//  Permission_to_Proceed                                                             //
//====================================================================================//
bool Permission_to_Proceed(const peps_name name, const file_types ft)
{
 bool      answer = true;
 char      y_n;
 peps_name tmp;

 if (Exists_File(name, ft)) {
  Add_Extension(name, ft, tmp);
  cout << "File '" << tmp << "' already exists and will be replaced. "
       << "Procede anyway (y/n)? ";
  cin >> y_n;
  answer = ((y_n == 'y') || (y_n == 'Y'));
  cout << "\n";
 }
 return(answer);
}
//====================================================================================//
//  Get_Only_the_File_Name                                                            //
//====================================================================================//
void Get_Only_the_File_Name(const peps_name full_name, peps_name & just_the_file_name)
{
 int       last_slash_position = -1;
 int       mark = 0;

 do {
   if (full_name[mark] == '/')
     last_slash_position = mark;
   mark++;
 } while (full_name[mark] != '\0');
 strcpy(just_the_file_name, &full_name[last_slash_position+1]);
}
//====================================================================================//
//  Provide DSC                                                                       //
//====================================================================================//
bool Provide_DSC()
{
 peps_name model_name;     // current directory (just the name, no paths, no extensions)
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 bool      go_on  = true;

 if (DSC::dsc.is_empty() || RSS::rss.is_empty() ||
     DCT::dct.is_empty() || INF::inf.is_empty() || DSC::dsc.is_grouped()) {
   cout << "There is no non-aggregated descriptor (dsc) in memory\n";
   RSS::rss.erase();
   FTB::ftb.erase();
   DSC::dsc.erase();
   DCT::dct.erase();
   INF::inf.erase();
   do { Ask_a_File_Name(model_name, dsc_file);
        strcpy(dsc_level_name, "dsc/");
        strcat(dsc_level_name, model_name);
        go_on = strcmp(model_name,"0");
   } while (!Exists_File(dsc_level_name, dsc_file) || !go_on);
   if (go_on) {
     RSS::rss.read(dsc_level_name);
     FTB::ftb.read(dsc_level_name);
     DSC::dsc.read(dsc_level_name);
     DCT::dct.read(dsc_level_name);
     INF::inf.read(dsc_level_name);
   }
   return(go_on);
 }
 else
   return(true);
}
//====================================================================================//
//  Provide AGG                                                                       //
//====================================================================================//
bool Provide_AGG()
{
 peps_name current_name;   // name of the current model with no path nor extensions
 peps_name original_name;  // original name of the model with no path nor extensions
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 peps_name agg_level_name; // directory to store the aggregated descriptor (./agg/*.dsc)
 bool      go_on  = true;

 if (DSC::dsc.is_empty() || RSS::rss.is_empty()
                         || DCT::dct.is_empty() || INF::inf.is_empty()) {
   cout << "There is no aggregated descriptor (agg) in memory\n";
   RSS::rss.erase();
   FTB::ftb.erase();
   DSC::dsc.erase();
   DCT::dct.erase();
   INF::inf.erase();
   do { Ask_a_File_Name(current_name, dsc_file);
        strcpy(agg_level_name, "agg/");
        strcat(agg_level_name, current_name);
        go_on = strcmp(current_name,"0");
   } while (!Exists_File(agg_level_name, dsc_file) || !Exists_File(agg_level_name, ftb_file) ||
            !Exists_File(agg_level_name, rss_file) || !go_on);
   if (go_on) {
     RSS::rss.read(agg_level_name);
     FTB::ftb.read(agg_level_name);
     DSC::dsc.read(agg_level_name);
     DSC::dsc.o_name(dsc_level_name);     // gets the original name (with the extensions
     go_on = Exists_File(dsc_level_name, dct_file) && Exists_File(dsc_level_name, inf_file);
     if (go_on) {
       DCT::dct.read(dsc_level_name);
       INF::inf.read(dsc_level_name);
     }
   }
   return(go_on);
 }
 else 
   return(true);
}
//====================================================================================//
//  Provide CND                                                                       //
//====================================================================================//
bool Provide_CND()
{
 peps_name current_name;   // name of the current model with no path nor extensions
 peps_name original_name;  // original name of the model with no path nor extensions
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)
 bool      go_on  = true;

 if (CND::cnd.is_empty() || RSS::rss.is_empty() ||
     DCT::dct.is_empty() || INF::inf.is_empty()) {
   cout << "There is no normalized descriptor (cnd) in memory.\n";
   RSS::rss.erase();
   FTB::ftb.erase();
   CND::cnd.erase();
   DCT::dct.erase();
   INF::inf.erase();
   do { Ask_a_File_Name(current_name, cnd_file);
        strcpy(cnd_level_name, "cnd/");
        strcat(cnd_level_name, current_name);
        go_on = strcmp(current_name,"0");
   } while (!Exists_File(cnd_level_name, cnd_file) || !Exists_File(cnd_level_name, ftb_file) ||
            !Exists_File(cnd_level_name, rss_file) || !go_on);
   if (go_on) {
     RSS::rss.read(cnd_level_name);
     FTB::ftb.read(cnd_level_name);
     CND::cnd.read(cnd_level_name);
     CND::cnd.o_name(dsc_level_name);        // gets the original name (with the extensions
     go_on = Exists_File(dsc_level_name, dct_file) && Exists_File(dsc_level_name, inf_file);
     if (go_on) {
       DCT::dct.read(dsc_level_name);
       INF::inf.read(dsc_level_name);
     }
     //else cout << "Debugging: " << dsc_level_name << ".dct and .inf\n";
   }
   return(go_on);
 }
 else
   return(true);
}
//====================================================================================//
//  Provide CND LUD                                                                   //
//====================================================================================//
bool Provide_CND_LUD()
{
 peps_name current_name;   // name of the current model with no path nor extensions
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)
 bool      go_on  = true;

 if (CND::cnd.is_empty() || RSS::rss.is_empty() ||
     LUD::lud.is_empty() || DCT::dct.is_empty() || INF::inf.is_empty()) {
   cout << "There is no normalized descriptor (cnd) in memory.\n";
   RSS::rss.erase();
   FTB::ftb.erase();
   CND::cnd.erase();
   LUD::lud.erase();
   DCT::dct.erase();
   INF::inf.erase();
   do { Ask_a_File_Name(current_name, cnd_file);
        strcpy(cnd_level_name, "cnd/");
        strcat(cnd_level_name, current_name);
        go_on = strcmp(current_name,"0");
   } while (!Exists_File(cnd_level_name, cnd_file) || !Exists_File(cnd_level_name, ftb_file) ||
            !Exists_File(cnd_level_name, rss_file) || !Exists_File(cnd_level_name, lud_file) ||
            !go_on);
   if (go_on) {
     RSS::rss.read(cnd_level_name);
     FTB::ftb.read(cnd_level_name);
     CND::cnd.read(cnd_level_name);
     LUD::lud.read(cnd_level_name);
     CND::cnd.o_name(dsc_level_name);        // gets the original name (with the extensions
     go_on = Exists_File(dsc_level_name, dct_file) && Exists_File(dsc_level_name, inf_file);
     if (go_on) {
       DCT::dct.read(dsc_level_name);
       INF::inf.read(dsc_level_name);
     }
   }
   return(go_on);
 }
 else
   return(true);
}
//====================================================================================//
//  Provide HBF                                                                       //
//====================================================================================//
bool Provide_HBF()
{
 peps_name name;

 if (HBF::hbf.is_empty()) {
   cout << "There is no sparse matrix (hbf) in memory.\n";
   Ask_an_Existing_File_Name(name, hbf_file);
   HBF::hbf.read(name);
 }
 return(true);
}
//====================================================================================//
//  Provide only VCT                                                                  //
//====================================================================================//
bool Provide_VCT()
{
 peps_name vector_name;    // vector name from the current directory
 peps_name current_name;   // name of the current model with no path nor extensions
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)
 bool      go_on  = true;
 int       option;         // option to load a or create a vector


 if (VCT::vct.is_empty())
   cout << "\nThere is no Probability Vector in memory";
 
 cout << "\nDo you wish to: 1) load a vector from a file"
      << "\n                2) create an equiprobable vector\n";
 
 if (!VCT::vct.is_empty()) {
   VCT::vct.name(vector_name);
   cout << "                3) use the current vector " << vector_name << ".vct\n";
 }
 
 cin >> option;
 switch (option) {
 case 1: Ask_an_Existing_File_Name(vector_name, vct_file);
   VCT::vct.read(vector_name);
   break;
 case 2:
   cout << "You need a normalized descriptor (cnd) to create an equiprobable vector!\n";
   RSS::rss.erase();
   FTB::ftb.erase();
   CND::cnd.erase();
   DCT::dct.erase();
   INF::inf.erase();
   do { Ask_a_File_Name(current_name, cnd_file);
   strcpy(cnd_level_name, "cnd/");
   strcat(cnd_level_name, current_name);
   go_on = strcmp(current_name,"0");
   } while (!Exists_File(cnd_level_name, cnd_file) ||
	    !Exists_File(cnd_level_name, ftb_file) ||
	    !Exists_File(cnd_level_name, rss_file) || !go_on);
   if (go_on) {
     RSS::rss.read(cnd_level_name);
     FTB::ftb.read(cnd_level_name);
     CND::cnd.read(cnd_level_name);
     CND::cnd.o_name(dsc_level_name);  // gets the original name (with the extensions
     go_on = Exists_File(dsc_level_name, dct_file) &&
       Exists_File(dsc_level_name, inf_file);
     if (go_on) {
       DCT::dct.read(dsc_level_name);
       INF::inf.read(dsc_level_name);
     }
     else cout << "Debugging: " << dsc_level_name << ".dct and .inf\n";
   }
   
   if (PRF::prf.vec_impl() == v_sparse)
     VCT::vct.create(RSS::rss.reach_size(), CND::cnd.ss());
   else
     VCT::vct.create(CND::cnd.ss(), CND::cnd.ss()); 
   VCT::vct.get_CND();
   VCT::vct.init(eq_vec);
   VCT::vct.baptise("equiprobable");
   break;
 case 3:
   break;
 default : go_on = false;
 }
 return(go_on);
}

//====================================================================================//
//  Provide VCT and CND to handle the probability vector                              //
//====================================================================================//
bool Provide_VCT_CND()
{
 peps_name vector_name;    // vector name from the current directory
 peps_name current_name;   // name of the current model with no path nor extensions
 peps_name dsc_level_name; // directory to store the internal descriptor   (./dsc/*.dsc)
 peps_name cnd_level_name; // directory to store the normalized descriptor (./cnd/*.cnd)
 bool      go_on  = true;
 int       option;         // option to load a or create a vector

 if (CND::cnd.is_empty() || RSS::rss.is_empty() ||
     DCT::dct.is_empty() || INF::inf.is_empty()) {
   cout << "You need a normalized descriptor (cnd) to handle a probability vector!\n";
   RSS::rss.erase();
   FTB::ftb.erase();
   CND::cnd.erase();
   DCT::dct.erase();
   INF::inf.erase();
   do { Ask_a_File_Name(current_name, cnd_file);
   strcpy(cnd_level_name, "cnd/");
   strcat(cnd_level_name, current_name);
   go_on = strcmp(current_name,"0");
   } while (!Exists_File(cnd_level_name, cnd_file) ||
	    !Exists_File(cnd_level_name, ftb_file) ||
	    !Exists_File(cnd_level_name, rss_file) || !go_on);
   if (go_on) {
     RSS::rss.read(cnd_level_name);
     FTB::ftb.read(cnd_level_name);
     CND::cnd.read(cnd_level_name);
     CND::cnd.o_name(dsc_level_name);  // gets the original name (with the extensions
     go_on = Exists_File(dsc_level_name, dct_file) &&
       Exists_File(dsc_level_name, inf_file);
     if (go_on) {
       DCT::dct.read(dsc_level_name);
       INF::inf.read(dsc_level_name);
     }
     else cout << "Debugging: " << dsc_level_name << ".dct and .inf\n";
   }
 }
   
 if (VCT::vct.is_empty() && go_on) {

   cout << "\nDo you wish to: 1) load a vector from a file"
	<< "\n                2) create an equiprobable vector\n";
   cin >> option;
   switch (option) {
   case 1 : Ask_an_Existing_File_Name(vector_name, vct_file);
     VCT::vct.read(vector_name);
     break;
   case 2 :
     if (PRF::prf.vec_impl() == v_sparse)
       VCT::vct.create(RSS::rss.reach_size(), CND::cnd.ss());
     else
       VCT::vct.create(CND::cnd.ss(), CND::cnd.ss()); 
     VCT::vct.get_CND();
     VCT::vct.init(eq_vec);
     VCT::vct.baptise("equiprobable");
     break;
   default : go_on = false;
   }
 }
 return(go_on);
}

//====================================================================================//
//  See DSC                                                                           //
//====================================================================================//
void See_DSC()
{
 peps_name bufname;
 bool      go_on;
 bool      normalized;
 int       buf;
 aut_id    a;
 aut_st    bufsizes;

 if ((!CND::cnd.is_empty()) && (!DCT::dct.is_empty()) && (!FTB::ftb.is_empty()) && 
      (!RSS::rss.is_empty()) && (!INF::inf.is_empty())) {
    normalized = true;
    go_on = true;
 }
 else {
    normalized = false;
    go_on = Provide_DSC();
 }
 if (go_on) {
   if (normalized)  CND::cnd.name(bufname);
   else             DSC::dsc.name(bufname);
   cout << "\n-------------------------------------------------------------------------"
        << "\nInspecting the descriptor " << bufname;
   if (PRF::prf.vec_impl() == v_sparse) cout << ".sp";
   if (normalized) cout << ".cnd";
   else            cout << ".dsc";
   cout << "\n-------------------------------------------------------------------------"
        << "\nProduct state space:               " << RSS::rss.orig_size()  << " states";
   if (FTB::ftb.is_grouped())
     cout << "\nState space after aggregation:     " << RSS::rss.size()       << " states";
   cout << "\nReachable state space:             " << RSS::rss.reach_size() << " states"
        << "\nNumber of automata:                " << FTB::ftb.aut()
        << "\nNumber of synchronized events:     ";
   if (normalized) { cout << CND::cnd.ev(); buf = CND::cnd.mem_use(); }
   else            { cout << DSC::dsc.ev(); buf = DSC::dsc.mem_use(); }
   cout << "\n-------------------------------------------------------------------------"
        << "\nMemory used by the descriptor:     " << buf / mem_unit
        << " Kbytes\nMemory used by the function table: " << FTB::ftb.mem_use() / mem_unit
        << " Kbytes\nMemory used by the rss vector:     " << RSS::rss.mem_use() / mem_unit
        << " Kbytes\nMemory used by the results table:  " << INF::inf.mem_use() / mem_unit
        << " Kbytes\nAll memory spent (all structures): "
        << (buf + FTB::ftb.mem_use() +RSS::rss.mem_use() + INF::inf.mem_use()) / mem_unit
        << " Kbytes\nAutomata sizes:\n[ ";
   FTB::ftb.get_domain(bufsizes);
   for (a=0; a<bufsizes.size(); a++)
     cout << bufsizes.sz(a) << " ";
   if (FTB::ftb.is_grouped()) {
     if (normalized) CND::cnd.get_s(bufsizes);
     else            DSC::dsc.get_s(bufsizes);
     cout << "]\nAutomata sizes after aggregation:\n[ ";
     for (a=0; a<bufsizes.size(); a++)
       cout << bufsizes.sz(a) << " ";
   }
   cout << "]\n-------------------------------------------------------------------------";
 }
}
//====================================================================================//
//  See RSS                                                                           //
//====================================================================================//
void See_RSS()
{
 peps_name bufname;
 aut_st    bufsizes, bufstates;
 aut_id    a;
 int       zero_stt; // nunber of states with probabilities under close_to_zero
 int       all_stt;  // original size - the full number of the product state space
 int       informed_reach_stt; // reachable states as informed by the reach function

 if (Provide_VCT_CND()) {
   VCT::vct.name(bufname);
   all_stt            = RSS::rss.orig_size();
   informed_reach_stt = RSS::rss.reach_size();
   zero_stt           = VCT::vct.elem_under(close_to_zero);
   cout << "\n-------------------------------------------------------------------------"
        << "\nInspecting the reachable state space " << bufname << ".rss"
        << "\n-------------------------------------------------------------------------"
        << "\nProduct state space:                  " << all_stt            << " states";
   if (FTB::ftb.is_grouped())
     cout << "\nState space after aggregation:        " << RSS::rss.size()  << " states";
   cout << "\nInformed Reachable state space:       " << informed_reach_stt << " states"
        << "\nNumber of zero probability states:    " << zero_stt           << " states"
        << "\nNumber of nonzero probability states: " << all_stt - zero_stt << " states"
        << "\n-------------------------------------------------------------------------"
        << "\nMemory used by the rss vector: " << RSS::rss.mem_use() / mem_unit << " Kbytes"
        << "\nAutomata sizes:\n[ ";
   FTB::ftb.get_domain(bufsizes);
   for (a=0; a<bufsizes.size(); a++)
     cout << bufsizes.sz(a) << " ";
   if (FTB::ftb.is_grouped()) {
     CND::cnd.get_s(bufsizes);
     cout << "]\nAutomata sizes after aggregation:\n[ ";
     for (a=0; a<bufsizes.size(); a++)
       cout << bufsizes.sz(a) << " ";
   }
   cout << "]\n-------------------------------------------------------------------------";
   if (informed_reach_stt != (all_stt - zero_stt))  // if the informed reach states is
                                                    //  different from the number of
                                                    // non zero states
     if (Ask_an_Answer("\nDo you want to correct the rss vector")) {
       RSS::rss.change_according_to_VCT();         // include as reachable all the
       cout << "rss vector corrected (now "        // states with nonzero probability
            << RSS::rss.reach_size() << " states are reachable)\n"
            << "-------------------------------------------------------------------------\n";
       Generate_a_New_FCT();                       // saves a new FCT (textual rss) file
     }                                             // acording to the newly computed rss
 }
}
//====================================================================================//
//  See VCT                                                                           //
//====================================================================================//
void See_VCT()
{
 peps_name bufname;
 aut_st    bufsizes, bufstates;
 aut_id    a;

 if (Provide_VCT_CND()) {
   VCT::vct.name(bufname);
   cout << "\n-------------------------------------------------------------------------"
        << "\nInspecting the vector " << bufname << ".vct"
        << "\n-------------------------------------------------------------------------"
        << "\nProduct state space:               " << RSS::rss.orig_size()  << " states";
   if (FTB::ftb.is_grouped())
     cout << "\nState space after aggregation:     " << RSS::rss.size()       << " states";
   cout << "\nReachable state space:             " << RSS::rss.reach_size() << " states"
        << "\nNumber of zero probability states: " 
        << VCT::vct.elem_under(close_to_zero) << " states"
        << "\n-------------------------------------------------------------------------"
        << "\nMemory used by the vector: " << VCT::vct.mem_use() / mem_unit << " Kbytes"
        << "\nAutomata sizes:\n[ ";
   FTB::ftb.get_domain(bufsizes);
   for (a=0; a<bufsizes.size(); a++)
     cout << bufsizes.sz(a) << " ";
   if (FTB::ftb.is_grouped()) {
     CND::cnd.get_s(bufsizes);
     cout << "]\nAutomata sizes after aggregation:\n[ ";
     for (a=0; a<bufsizes.size(); a++)
       cout << bufsizes.sz(a) << " ";
   }
   cout << "]\n-------------------------------------------------------------------------";
 }
}
//====================================================================================//
//  See HBF                                                                           //
//====================================================================================//
void See_HBF()
{
 peps_name bufname;

 if (Provide_HBF()) {
   HBF::hbf.name(bufname);
   cout << "\n-------------------------------------------------------------------------"
        << "\nInspecting the sparse matrix " << bufname << ".hbf"
        << "\n-------------------------------------------------------------------------"
        << "\nState space (matrix order):        " << HBF::hbf.order()  << " states"
        << "\nNonzero entries:                   " << HBF::hbf.size()   << " elements"
        << "\nMemory used by the sparse matrix:  " << HBF::hbf.mem_use() / mem_unit
        << " Kbytes\n-------------------------------------------------------------------------";
 }
}
//====================================================================================//
//  Generate a New FCT                                                                //
//====================================================================================//
void Generate_a_New_FCT()
{
 ofstream  out_file;
 peps_name des_level_name;
 peps_name cnd_level_name;
 peps_name model_name;
 aut_st    sizes;

 if (Provide_VCT_CND()) {
   RSS::rss.name(cnd_level_name);                           // generate the correct DES level
   Get_Only_the_File_Name(cnd_level_name, model_name);      //   name, i.e., the name and path
   strcpy(des_level_name, "des/");                          //   of the <model>.res file
   strcat(des_level_name, model_name);
   Open_File(des_level_name, fct_file, out_file);           // open (and erases old) .res file
   out_file << "#domain ";                                  // write the automata number and
   CND::cnd.get_s(sizes);                                   //   sizes
   sizes.write(out_file);
   out_file << "\n#vector ";                                // write the vector (bools) of the
   RSS::rss.dump_vector(out_file);                          //   new reachable state space
   Close_File(des_level_name, fct_file, out_file);          // close the .res file
 }
}
//====================================================================================//
//  Ask an Answer                                                                     //
//====================================================================================//
bool Ask_an_Answer(const peps_name name)
{
 bool answer = true;
 char y_n;

 cout << name << " (y/n)? ";
 cin >> y_n;
 answer = ((y_n == 'y') || (y_n == 'Y') || (y_n == '0'));
 cout << "\n";
 return(answer);
}
//====================================================================================//
//  Ask an Initial Vector Type                                                        //
//====================================================================================//
v_typ Ask_an_Initial_Vector_Type()
{
 int   choice;
 v_typ answer;

 cout << "Enter a choice of initial vector: 1 equiprobable\n"
      << "                                  2 approximated\n"
      << "                                  3 user vector\n";
 cin >> choice;
 switch (choice) {
   case 1 : answer = eq_vec; break;
   case 2 : answer = ap_vec; break;
   case 3 : answer = in_vec; VCT::vct.erase(); break;
   default : cout << "\nAdopting option 1 (equiprobable)\n";
             answer = eq_vec;
 }
 return(answer);
}
//====================================================================================//
//  Ask a Vector Implementation Type                                                  //
//====================================================================================//
v_impl Ask_a_Vector_Implementation()
{
 int   choice;
 v_impl answer;

 cout << "Enter a choice of vector implementation : 1 extended (E)\n"
      << "                                          2 partially reduced (PR)\n"
      << "                                          3 fully reduced (FR)\n";
 cin >> choice;
 switch (choice) {
   case 1 : answer = v_full; break;
   case 2 : answer = v_sparse; PRF::prf.z_impl(zin_full); break;
   case 3 : answer = v_sparse; PRF::prf.z_impl(zin_sparse); break;
   default : cout << "\nAdopting extended implementation\n";
             answer = v_full;
 }

 // Erase all structures to avoid mistakes when it changes
 if (answer != PRF::prf.vec_impl()) {
   DSC::dsc.erase();
   FTB::ftb.erase();
   RSS::rss.erase();
   CND::cnd.erase();
   LUD::lud.erase();
   HBF::hbf.erase();
   VCT::vct.erase();
 }
 
 return(answer);
}
//====================================================================================//
//  Ask a Vector Implementation Type                                                  //
//====================================================================================//
zin_impl Ask_a_Zin_Implementation()
{
 int   choice;
 zin_impl answer;

 cout << "Enter a choice of zin implementation : 1 full\n"
      << "                                       2 sparse\n";
 cin >> choice;
 switch (choice) {
   case 1 : answer = zin_full; break;
   case 2 : answer = zin_sparse; break;
   default : cout << "\nAdopting sparse implementation\n";
             answer = zin_sparse;
 }
 return(answer);
}
//====================================================================================//
//  Ask an Iteration Type                                                             //
//====================================================================================//
i_typ Ask_an_Iteration_Type()
{
 int    choice;
 i_typ answer;

 cout << "Enter a choice: 1 fixed number of iterations\n"
      << "                2 stability test\n"
      << "                3 convergence test\n";
 cin >> choice;
 switch (choice) {
   case 1  : answer = fix_it; break;
   case 2  : answer = stb_it; break;
   case 3  : answer = cnv_it; break;
   default : cout << "\nAdopting option 3 (convergence test)\n";
             answer = cnv_it;
 }
 return(answer);
};
//====================================================================================//
//  Ask a Precision Type                                                             //
//====================================================================================//
e_typ Ask_a_Precision_Type()
{
 int   choice;
 e_typ answer;

 cout << "Enter a choice: 1 maximum absolute individual error\n"
      << "                2 accumulated absolute error\n"
      << "                3 maximum relative individual error\n";
 cin >> choice;
 switch (choice) {
   case 1  : answer = abs_ind_err; break;
   case 2  : answer = abs_acc_err; break;
   case 3  : answer = rel_ind_err; break;
   default : cout << "\nAdopting option 1 (absolute individual error)\n";
             answer = abs_ind_err;
 }
 return(answer);
}
//====================================================================================//
//  Ask a Function Aproximation Type                                                  //
//====================================================================================//
f_aprox Ask_a_Function_Aproximation_Type()
{
 int     choice;
 f_aprox answer;

 cout << "Enter a choice for function elimination policy:\n"
      << "                1 individual function elimination (with dialogue)\n"
      << "                2 replace functions with their maximum values\n"
      << "                3 replace functions with their minimum values\n"
      << "                4 replace functions with their average values\n"
      << "                5 replace functions with zeros\n";
 cin >> choice;
 switch (choice) {
   case 1  : answer = lu_user; break;
   case 2  : answer = lu_max;  break;
   case 3  : answer = lu_min;  break;
   case 4  : answer = lu_ave;  break;
   case 5  : answer = lu_zero; break;
   default : cout << "\nAdopting option 4 (average values)\n";
             answer = lu_ave;
 }
 return(answer);
}
//====================================================================================//
//  Ask a LU Decomposition Type                                                       //
//====================================================================================//
lu_typ Ask_a_LU_Decomposition_Type()
{
 int    choice;
 lu_typ answer;

 cout << "Enter a choice for LU decomposition:\n"
      << "                1 Standard ILU decomposition\n"
      << "                2 SOR-like decomposition\n";
 cin >> choice;
 switch (choice) {
   case 1  : answer = std_lu; break;
   case 2  : answer = sor_lu; break;
   default : cout << "\nAdopting option 1 (Standard ILU decomposition)\n";
             answer = std_lu;
 }
 return(answer);
}
//====================================================================================//
//  Ask a Matrix Regulation Type                                                      //
//====================================================================================//
reg_typ Ask_a_Matrix_Regulation_Type()
{
 int     choice;
 reg_typ answer;

 cout << "Enter a choice for matrix regulation:\n"
      << "              1 no modification      (keeping the matrix as it is)\n"
      << "              2 shift diagonal elements by epsilon         (shift)\n"
      << "              3 equalize diag. to the largest abs. value (winglet)\n"
      << "              4 multiply the matrix by its inverse      (tychonov)\n"
      << "              5 replace null diagonal elements with epsilon\n"
      << "              6 replace null diagonal elements with  one\n";
 cin >> choice;
 switch (choice) {
   case 1  : answer = no_reg;    break;
   case 2  : answer = shift_reg; break;
   case 3  : answer = wingl_reg; break;
   case 4  : answer = tychv_reg; break;
   case 5  : answer = aprox_reg; break;
   case 6  : answer = one_reg;   break;
   default : cout << "\nAdopting option 1 (no regulation)\n";
             answer = no_reg;
 }
 return(answer);
}
//====================================================================================//
//  Ask a LU Solution Type                                                            //
//====================================================================================//
sol_typ Ask_a_LU_Solution_Type()
{
 int     choice;
 sol_typ answer;

 cout << "Enter a choice for LU solution:\n"
      << "              1 Solve with both L and U matrices (when possible)\n"
      << "              2 Solve with L matrices          (always possible)\n"
      << "              3 Solve with U matrices            (when possible)\n";
 cin >> choice;
 switch (choice) {
   case 1  : answer = both_lu; break;
   case 2  : answer = just_l;  break;
   case 3  : answer = just_u;  break;
   default : cout << "\nAdopting option 1 (Solve with both L and U matrices)\n";
             answer = both_lu;
 }
 return(answer);
}
//====================================================================================//
//  Ask Aggregation Map                                                                //
//====================================================================================//
void Ask_Agregation_Map(aut_st & s)
{
 int       g;
 bool      map_not_ok;
 peps_name buf;

 s.create(DSC::dsc.aut());
 do {
   cout << "Enter contiguous integer values to identify the groups starting from '0'\n"
        << " (last group number must be less than '" << DSC::dsc.aut() << "')\n";
   for (int i = 0; i<DSC::dsc.aut(); i++) {
     DCT::dct.a_name(i, buf);
     cout << "Automaton " << buf << " will be in group (0.."
          << DSC::dsc.aut()-1 << "): ";
     cin >> g;
     s.put(i, g);
   }
 if ( map_not_ok = ( !(s.contigous()) || (s.smallest() != 0) ) )
   cout << "Your group numbering is wrong!\n";
 } while (map_not_ok);
}
//====================================================================================//
//  Ask Partial State                                                                 //
//====================================================================================//
//  p - partial state (the values already knew by the program                         //
//  s - which automata are already known                                              //
//====================================================================================//
void Ask_Partial_State(aut_st & p, const aut_set & s,
                       const func_id f, const ev_id e, const aut_id a)
{
#ifdef _PEPS_DEBUG_
  if (s.size() != FTB::ftb.aut())
    Programming_Error("wrong sizes in a Ask_Partial_State call", 0001)
  if (s.size() != p.size())
    Programming_Error("wrong sizes in a Ask_Partial_State call", 0002)
#endif

 aut_id    aut;                    // counter for automata
 st_id     val;                    // buffer to the state of the automata
 rp        min, max;               // minimum and maximum evaluation of the function
 aut_set   to_know;                // the set of automata to be specified by the user
 aut_st    sizes;                  // the size of each automaton
 peps_name buf1, buf2, buf3, buf4; // buffers to names from dictionary

 FTB::ftb.get_domain(sizes);     // gets the size of each original automaton
 to_know.create(FTB::ftb.aut());
 FTB::ftb.params(f, to_know);    // the parameters of 'f' are to know
 to_know.minus(s);               // except if they already are known ('s')

 DCT::dct.gf_name(f, buf1);
 DCT::dct.gf_eexp(f, buf2);
 DCT::dct.a_name(a, buf3);
 // to read the event name use buf4
 cout << "\nElimination of the function " << buf1 << " in the matrix of automaton "
      << buf3;
 if (e != no_ev)
   cout << " of the event term " << e << ".\n";
 else
   cout << " of the local term.\n";
 cout << "Function formulae: " << buf2 << " - "; FTB::ftb.show(f);
 cout << "The values of the automata ( ";
 for (aut=0; aut<s.size(); aut++)
   if (s.has(aut)) {
     DCT::dct.a_name(aut, buf3);
     cout << buf3 << " ";
   }
 cout << ") are known.\nEnter values for the following original automata:\n";
 for (aut=0; aut<s.size(); aut++)
   if (to_know.has(aut)) {
     do {
       DCT::dct.a_name(aut, buf3);
       cout << "Automata " << buf3 << " (from 0 to " << sizes.sz(aut)-1 << "):";
       cin >> val;
     } while ((val < 0) || (val >= sizes.sz(aut)));
     p.put(aut, val);
   }
 cout << "\n------------------------------------\n";
}
//====================================================================================//
//  Ask an Iteration Number                                                           //
//====================================================================================//
int Ask_an_Iteration_Number()
{
 int iteration;

 cout << "Number of iterations to perform? ";
 cin >> iteration;
 if (iteration == 0) {
   iteration = def_iter;
   cout << "Assuming default value: " << def_iter << "\n";
 }
 return(iteration);
}
//====================================================================================//
//  Ask an Iteration Number                                                           //
//====================================================================================//
int Ask_an_Iteration_Number(int & half, int & rest)
{
 int iteration;

 cout << "Number of iterations to perform? ";
 cin >> iteration;
 if (iteration == 0) {
   iteration = def_iter;
   cout << "Assuming default value: " << def_iter << "\n";
 }
 half = iteration / 2;
 rest = iteration % 2;
 return(iteration);
}
//====================================================================================//
//  Ask an Error                                                                      //
//====================================================================================//
rp Ask_an_Error()
{
 rp  error;

 cout << "Enter tolerance: ";
 cin >> error;
 if (error == 0) {
   error = def_err;
   cout << "Assuming default value: " << def_err << "\n";
 }
 return(error);
}
//====================================================================================//
//  Ask an Error                                                                      //
//====================================================================================//
rp Ask_an_Error(int & half, int & rest)
{
 rp  error;
 int iteration;

 cout << "Enter tolerance: ";
 cin >> error;
 if (error == 0) {
   error = def_err;
   cout << "Assuming default value: " << def_err << "\n";
 }
 cout << "Enter maximum number of iterations: ";
 cin >> iteration;
 if (iteration == 0) {
   iteration = def_iter;
   cout << "Assuming default value: " << def_iter << "\n";
 }
 half = iteration / 2;
 rest = iteration % 2; 
 return(error);
}
//====================================================================================//
//  Ask a Product Method                                                              //
//====================================================================================//
m_typ Ask_a_Product_Method()
{
 int   i;
 m_typ answer;

 cout << "Choose a multiplication method:\n"
      << "             1 to avoid permutations    (method A)\n"
      << "             2 to minimize permutations (method B)\n"
      << "             3 to minimize evaluations  (method C)\n"
      << "             4 to used a mixed method\n";
 cin >> i;
 switch (i) {
   case  1 : answer = meth_A; break;
   case  2 : answer = meth_B; break;
   case  3 : answer = meth_C; break;
   default : answer = meth_M;
             cout << "Assuming mixed method\n";
 }
 return(answer);
}
//====================================================================================//
//  Ask a Threshold                                                                   //
//====================================================================================//
rp Ask_a_Threshold()
{
 rp  answer;

 cout << "Enter threshold: ";
 cin >> answer;
 if (answer == zero) {
   answer = def_thresh;
   cout << "Assuming default value: " << def_thresh << "\n";
 }
 return(answer);
}
//====================================================================================//
//  Ask a Krylov Space Size                                                           //
//====================================================================================//
int Ask_a_Krylov_Space_Size()
{
 int answer;

 cout << "Enter dimension of Krylov subspace: ";
 cin >> answer;
 if (answer == 0) {
   answer = def_krylov;
   cout << "Assuming default value: " << def_krylov << "\n";
 }
 return(answer);
}
//====================================================================================//
//  Ask a Polynomial Size                                                            //
//====================================================================================//
int Ask_a_Polynomial_Size()
{
 int answer;

 cout << "Enter number of terms in polynomial: ";
 cin >> answer;
 if (answer == 0) {
   answer = def_poly;
   cout << "Assuming default value: " << def_poly << "\n";
 }
 return(answer);
}
//====================================================================================//
//  Ask a Vector Range                                                                //
//====================================================================================//
void Ask_a_Vector_Range(int & i, int & f)
{
 int first;
 int last;

 cout << "Enter first state: ";
 cin >> first;
 if ((first < i) || (first > (f-1)))
   cout << " (Assuming " << i << ")\n";
 else i = first;
 cout << "Enter last state: ";
 cin >> last;
 if ((last <= i) || (last >= (f)))
   cout << " (Assuming " << f << ")\n";
 else f = last;
}
//====================================================================================//
//  Ask an Automaton                                                                  //
//====================================================================================//
void Ask_an_Automaton(aut_id & a)
{
  peps_name name_aut;
  aut_id    aut;

  cout << "The automata are:";
  for (aut=0; aut<FTB::ftb.aut(); aut++) {
     DCT::dct.a_name(aut, name_aut);
     cout << "\n    " << aut << ") " << name_aut;
   }
   cout << "\n\nChoose an automaton (enter its index): ";
   cin >> a;
   if ( (a < 0) || (a >= FTB::ftb.aut()) ) {
     a = fst_aut;
     cout << "Assuming first automaton\n";
   }
}
//====================================================================================//
//  Ask a Global State                                                                //
//====================================================================================//
int Ask_a_Global_State()
{
 int answer, last;

 last = RSS::rss.orig_size() - 1;
 cout << "Enter the global state index (from 0 to " << last << "): ";
 cin >> answer;
 if ((answer < 0) || (answer > last)) {
   cout << "Assuming the first state (0)\n";
   answer = 0;
 }
 return(answer);
}
//====================================================================================//
//  Ask a Global State                                                                //
//====================================================================================//
int Ask_a_Global_State(aut_st & states)
{
 int    answer;
 aut_st sizes;
 aut_id a;
 st_id  s;

 cout << "Enter the local state of each of the automata\n";
 FTB::ftb.get_domain(sizes);
 for (a=0; a<FTB::ftb.aut(); a++) {
   Ask_a_Local_State(a, sizes.sz(a), s);
   states.put(a, s);
 }
 answer = states.rank(sizes);
 return(answer);
}
//====================================================================================//
//  Ask a Local State                                                                 //
//====================================================================================//
void Ask_a_Local_State(const aut_id a, const st_id size, st_id & s)
{
  peps_name name_aut, name_st;
  st_id     st;

  DCT::dct.a_name(a, name_aut);
  cout << "The local states of automaton " << name_aut << " are:";
  for (st=0; st<size; st++) {
     DCT::dct.s_name(a, st, name_st);
     cout << "\n    " << st << ") " << name_st;
   }
  cout << "\nChoose a state (enter its index): ";
   cin >> s;
   if ( (s < 0) || (s >= size) ) {
     s = fst_st;
     cout << "Assuming the first state\n";
   }
}
//====================================================================================//
//  Ask Local States (accepts no_st as answer)                                        //
//====================================================================================//
void Ask_Local_States(const aut_id a, const st_id size, st_id & s)
{
  peps_name name_aut, name_st;
  st_id     st;

  DCT::dct.a_name(a, name_aut);
  cout << "The local states of automaton " << name_aut << " are:";
  for (st=0; st<size; st++) {
     DCT::dct.s_name(a, st, name_st);
     cout << "\n    " << st << ") " << name_st;
   }
  cout << "\nChoose an option (enter its index or '" << size << "' for all states): ";
   cin >> s;
   if ( (s < 0) || (s >= size) ) {
     s = no_st;
     cout << "Assuming all states\n";
   }
}

//====================================================================================//
//  Show User Preferences                                                             //
//====================================================================================//
void Show_User_Preferences()
{
 peps_name buf;

 cout << "User Preferences\n\n";
 PRF::prf.sverb(buf);
 cout << "Verbose mode:                       " << buf                << "\n";
 cout << "Iterations:\n"
      << "  Maximum number of iterations:     " << PRF::prf.max_it()  << "\n";
 PRF::prf.sit_type(buf);
 cout << "  Stop iteration criterion:         " << buf                << "\n";
 cout << "  Tolerance accepted:               " << PRF::prf.min_err() << "\n";
 PRF::prf.serr_type(buf);
 cout << "  Error Test:                       " << buf                << "\n";
 PRF::prf.si_vec(buf);
 cout << "  Initial vector:                   " << buf                << "\n";
 PRF::prf.svec_impl(buf);
 cout << "  Vector Implementation type:       " << buf                << "\n";
 PRF::prf.smethod(buf);
 cout << "  Vector-Descriptor product method: " << buf                << "\n";
 cout << "ILU Decomposition:\n";
 cout << "  Threshold:                        " << PRF::prf.lu_thrs() << "\n";
 PRF::prf.slu_apx(buf);
 cout << "  Function elimination policy:      " << buf                << "\n";
 PRF::prf.sreg_type(buf);
 cout << "  Matrix modification:              " << buf                << "\n";
 PRF::prf.slu_type(buf);
 cout << "  ILU decomposition type:           " << buf                << "\n";
 cout << "Projection Methods:\n";
 cout << "  Krylov subspace size:             " << PRF::prf.k_size()  << "\n";
 cout << "Preconditioning:\n";
 PRF::prf.ssol_type(buf);
 cout << "  ILU solution type:                " << buf                << "\n";
 cout << "  Number of terms in polynomials:   " << PRF::prf.poly_k()  << "\n";
}

//====================================================================================//
//  Change User Preferences                                                           //
//====================================================================================//
void Change_User_Preferences()
{
 peps_name name;

 PRF::prf.verb(Ask_an_Answer("Do you want verbose mode"));
 PRF::prf.max_it(Ask_an_Iteration_Number());
 PRF::prf.it_type(Ask_an_Iteration_Type());
 PRF::prf.min_err(Ask_an_Error());
 PRF::prf.err_type(Ask_a_Precision_Type());
 PRF::prf.i_vec(Ask_an_Initial_Vector_Type());
 PRF::prf.vec_impl(Ask_a_Vector_Implementation());
 PRF::prf.method(Ask_a_Product_Method());
 PRF::prf.lu_thrs(Ask_a_Threshold());
 PRF::prf.lu_apx(Ask_a_Function_Aproximation_Type());
 PRF::prf.lu_type(Ask_a_LU_Decomposition_Type());
 PRF::prf.reg_type(Ask_a_Matrix_Regulation_Type());
 PRF::prf.sol_type(Ask_a_LU_Solution_Type());
 PRF::prf.k_size(Ask_a_Krylov_Space_Size());
 PRF::prf.poly_k(Ask_a_Polynomial_Size());
 if (Ask_an_Answer("Do you wish to save these preferences")) {
   Ask_a_File_Name(name, prf_file);
   PRF::prf.baptise(name);
   if (Permission_to_Proceed(name, prf_file))
     PRF::prf.write();
 }
}
//====================================================================================//
//  Notify_Time_Spend (stdout) - solutions version                                    //
//====================================================================================//
void Notify_Time_Spend(const Timer T,
                       const peps_name op, const int iteration, const rp resid)
{
 ofstream out_file;

 cout << "\n" << op << "\n"
      <<   "Number of iterations:   " << iteration << "\n"
      <<   " - user time spent:     " << T.usertime() << " seconds\n"
      <<   " - system time spent:   " << T.systime()  << " seconds\n"
      <<   " - real time spent:     " << T.realtime() << " seconds\n";
 if (PRF::prf.it_type() == cnv_it) {
   cout << "Residual Error: " << resid;
   if (resid < PRF::prf.min_err())
     cout << " - The method converged (solution found)!\n";
   else
     cout << " - The method did not converge\n";
 }
//  if (PRF::prf.verb()) {
//    Open_File("peps", tim_file, out_file);
//    out_file << T.usertime() << " (time)\n"
//             << iteration    << " (iterations)\n"
//             << op           << " (solution)\n";
//    if (PRF::prf.it_type() == cnv_it)
//      out_file << resid        << " (residue)\n";
//      if (resid < PRF::prf.min_err())
//        out_file << " - The method converged (solution found)!\n";
//      else
//        out_file << " - The method did not converge\n";
//  }
}
//====================================================================================//
//  Notify_Time_Spend (stdout) - translations version                                 //
//====================================================================================//
void Notify_Time_Spend(const Timer T, const peps_name op)
{
 ofstream out_file;

 cout << "\nTranslation performed:  " << op
      << "\n  (largest element in reachable states: " << 1/CND::cnd.n_factor() << ")\n"
      <<   " - user time spent:     " << T.usertime() << " seconds\n"
      <<   " - system time spent:   " << T.systime()  << " seconds\n"
      <<   " - real time spent:     " << T.realtime() << " seconds\n";
//  if (PRF::prf.verb()) {
//    Open_File("peps", tim_file, out_file);
//    out_file << T.usertime() << " (time)\n"
//             << op           << " (translation)\n";
//  }
}
//====================================================================================//
//  Notify_Time_Spend (in a file) - solutions version                                 //
//====================================================================================//
void Notify_Time_Spend(const Timer T, const peps_name name,
                       const peps_name op, const int iteration, const rp resid)
{
 ofstream out_file;

 Open_File(name, tim_file, out_file);
 out_file << "File " << name << ".tim\n\n"
          << op << " of "
          << name << ".cnd\n  stored in the file " << name << ".vct\n"
          << "Number of iterations:   " << iteration << "\n"
          << " - user time spent:     " << T.usertime() << " seconds\n"
          << " - system time spent:   " << T.systime()  << " seconds\n"
          << " - real time spent:     " << T.realtime() << " seconds\n";
 if (PRF::prf.it_type() == cnv_it) {
   out_file << "Residual Error: " << resid;
   if (resid < PRF::prf.min_err())
     out_file << " - The method converged (solution found)!\n";
   else
     out_file << " - The method did not converge\n";
 }
 Close_File(name, tim_file, out_file);
}
//====================================================================================//
//  Notify_Time_Spend (in a file) - translations version                              //
//====================================================================================//
void Notify_Time_Spend(const Timer T, const peps_name name, const peps_name op)
{
 ofstream out_file;

 Open_File(name, tim_file, out_file);
 out_file << "File " << name << ".tim\n\n"
          << "Translation Report (" << op << ")\n\n"
          << name << ".dsc translated into " << name << ".cnd\n"
          << " - user time spent:     " << T.usertime() << " seconds\n"
          << " - system time spent:   " << T.systime()  << " seconds\n"
          << " - real time spent:     " << T.realtime() << " seconds\n";
 Close_File(name, tim_file, out_file);
}
//====================================================================================//
//  Log Out Solution                                                                  //
//====================================================================================//
void Log_Out_Solution(const Timer T, const int iteration,
                      const solution_typ s, const precond_typ p,
                      const rp resid)
{
 peps_name cnd_level_name, descriptor_name, model_name, v_name;
 ofstream  out_file;
 aut_st    sizes;
 aut_id    a;
 rp        acc, big,  sml;
 int            bpos, spos;
 int       kry_size = PRF::prf.k_size();
 int       polynom_size  = PRF::prf.poly_k();
 int       vector_i = PRF::prf.vec_impl();

 CND::cnd.name(cnd_level_name);
 Get_Only_the_File_Name(cnd_level_name, descriptor_name);
 DCT::dct.n_name(model_name);

 VCT::vct.name(v_name);

 if (Exists_File(descriptor_name, tim_file))
   Append_File(descriptor_name, tim_file, out_file);
 else {
   Open_File(descriptor_name, tim_file, out_file);
   out_file << "===============================================================\n"
            << "File " << descriptor_name;
   if (PRF::prf.vec_impl() == v_sparse) out_file << ".sp";
   out_file << ".tim\n"
            << "===============================================================\n"
            << descriptor_name << ".san -- A model with " << DCT::dct.aut()
            << " automata and " << DCT::dct.ev() << " events"
            << "\nUser name: '" << model_name << "'"
            << "\n------------\nProblem Size\n------------"
            << "\nProduct state space:           " << RSS::rss.orig_size() << " states";
   if (FTB::ftb.is_grouped())
     out_file << "\nState space after aggregation:     " << RSS::rss.size()       << " states";
   out_file << "\nReachable state space:         " << RSS::rss.reach_size() << " states"
            << "\nAutomata sizes:                   [ ";
   FTB::ftb.get_domain(sizes);
   for (a=0; a<sizes.size(); a++)
     out_file << sizes.sz(a) << " ";
   out_file << "]\nAutomata sizes after aggregation: [ ";
   if (FTB::ftb.is_grouped()) {
     CND::cnd.get_s(sizes);
     for (a=0; a<sizes.size(); a++)
       out_file << sizes.sz(a) << " ";
   }
   out_file << "]\nCurrent Number of Functions: " << FTB::ftb.size();
   out_file << "\nSize of the Normalized Descriptor: "
            << CND::cnd.mem_use() / mem_unit << " Kbytes";
   out_file << "\n===============================================================\n";
 }
 out_file << "Solution performed: ";
 switch (s) {
   case Power     : out_file << "power";   break;
   case Arnoldi   : out_file << "arnoldi"; break;
   case Gmres     : out_file << "gmres";   break;
 }
 out_file << " method with ";
 switch (p) {
   case no_precond : out_file << "no";                      break;
   case adtv       : out_file << "partial aditive";         break;
   case altn       : out_file << "partial alternate";       break;
   case mult       : out_file << "partial multiplicative";  break;
   case diag       : out_file << "diagonal";                break;
   case plyn       : out_file << "polynomial (" 
                              << polynom_size << " terms)"; break;
   case ipln       : out_file << "inverse polynonial (" 
                              << polynom_size << " terms)"; break;
 }
 out_file << " preconditionning";
 out_file << "\nThe multiplication method ";
   switch (PRF::prf.method()) {
   case meth_A : out_file << " avoids permutations (method A)"; break;
   case meth_B : out_file << " minimizes permutations (method B)"; break;
   case meth_C : out_file << " minimizes evaluations (method C)"; break;
   case meth_M : out_file << " can be different for each term (mixed method)"; break;
   }
 if ( (s == Arnoldi) || (s == Gmres) )
   out_file << "\nSize of the Krylov subspace for projection: " << kry_size << " vectors";

 if (vector_i == v_full)
   out_file << "\nExtended (E) Probability Vectors";
 else {
   if (PRF::prf.z_impl() == zin_full)
     out_file << "\nPartially Reduced (PR) Probability Vectors";
   else
     out_file << "\nFully Reduced (FR) Probability Vectors";
 }
 
 out_file << "\n===============================================================";
 if (!(LUD::lud.is_empty())) {
   out_file << "\nRegulation used on LU decomposed matrices: ";
   switch (LUD::lud.regular()) {
     case no_reg    : out_file << "none";
                      break;
     case shift_reg : out_file << "shift (add a small value to diagonal elements)";
                      break;
     case wingl_reg : out_file << "winglet (minimize diagonal elements)";
                      break;
     case tychv_reg : out_file << "tychonov (transposition)";
                      break;
     case aprox_reg : out_file << "forcing LU decomposition replacing 0 by a small value";
                      break;
     case one_reg   : out_file << "forcing LU decomposition replacing 0 by 1";
   }
 }
 out_file << "\nNumber of iterations performed: " << iteration
          << "\nCPU average time per iteration: "
          << (rp(int(T.usertime()*100))/100)/iteration << " seconds"
          << "\nTotal CPU time:                 "
          << rp(int(T.usertime()*100))/100 << " seconds";

 VCT::vct.info(acc, big, bpos, sml, spos);

 out_file << "\n----------------------\nVector characteristics\n----------------------"
          << "\nName:                      " << v_name
          << ".vct\nSum of all elements:       " << acc;
 if (absv(one - acc) > round_zero)
   out_file << " - warning - this vector is not a probability vector :-(";
 out_file <<  "\nLargest element:           " << big << " (in position " << bpos
          << ")\nSmallest element:          " << sml << " (in position " << spos
          << ")\nResidue after convergence: " << resid << "\n  (the vector " << v_name;
 if (resid < PRF::prf.min_err())
   out_file << ".vct is the solution of the model " << descriptor_name
            << ".san)";
 else
   out_file << ".vct is NOT be the solution of the model " << descriptor_name
          << ".san)\n===============================================================\n";
 Close_File(descriptor_name, tim_file, out_file);
}
//====================================================================================//
//  Log Out a Solution Function                                                       //
//====================================================================================//
void Log_Out_a_Solution_Function(const func_id f, const rp v)
{
 peps_name cnd_level_name, descriptor_name, model_name, v_name, f_name;
 ofstream  out_file;
 v_impl  vv;

 CND::cnd.name(cnd_level_name);
 Get_Only_the_File_Name(cnd_level_name, descriptor_name);
 DCT::dct.n_name(model_name);
 if (Exists_File(descriptor_name, tim_file))
   Append_File(descriptor_name, tim_file, out_file);
 else {
   Open_File(descriptor_name, tim_file, out_file);
   out_file << "===============================================================\n"
            << "File " << descriptor_name;
   if (PRF::prf.vec_impl() == v_sparse) out_file << ".sp";
   out_file << ".tim\n"
	    << "===============================================================\n"
            << descriptor_name << ".san -- A model with " << DCT::dct.aut()
            << " automata and " << DCT::dct.ev() << " events called " << model_name
            << "\nProblem Size: " << RSS::rss.orig_size() << " where "
            << RSS::rss.reach_size() << " are reachable\n"
            << "\n---------------------------------------------------------------"
            << "\nResolution details unknown (probably a programming error)"
            << "\n---------------------------------------------------------------";
 }
 if (f == fst_func)
   out_file << "\n---------------------\nResults\n---------------------";
 
 DCT::dct.sf_name(f, f_name);
 out_file << "\n" << f_name << ": " << v;
 if ((f+1) == DCT::dct.sfs())
   out_file << "\n===============================================================\n";
 out_file.close();// Close_File(model_name, tim_file, out_file);-- to close it quietly
}
//====================================================================================//
//  Show Credits                                                                      //
//====================================================================================//
void Show_Credits()
{
 cout << "\n\n     PEPS - Performance Evaluation of Parallel Systems - version 2003"
      << "\n\n  PEPS is a software tool for solving Stochastic Automata Networks (SAN)"
      << "\n\n     The current version (2009) is an experimental version. Mathematical"
      <<   "\n basis for SANs may be obtained in the literature.  The main limitations"
      <<   "\n of this version are:"
      <<   "\n          - the function compilation procedure has size limitations;"
      <<   "\n          - the grouping procedure is not fully tested;"
      <<   "\n          - the saved files are large (ascii format);" 
      <<   "\n          - only continuous-time models are accepted;" 
      <<   "\n          - only numerical solutions are available" 
      <<   "\n          - large functions may lead to memory problems"
      << "\n\n     PEPS is a collective software effort and its authors may be reached"
      <<   "\n at:"
      <<   "\n         peps@imag.fr or http://www-apache.imag.fr/software/peps/"
      << "\n\n     The distribution of this software is free,  but any changes in  the" 
      <<   "\n source code is under the full responsability of the user. Any comercial"
      <<   "\n use of  PEPS  or modified versions of  PEPS cannot be performed without"
      <<   "\n the  formal autorization of the authors.  Any scientific benefit of the"
      <<   "\n use of the PEPS tool must mention it."
      << "\n\n     This version of PEPS was released on: " << __RELEASE_DATE__
      <<   "\n              and it has been compiled on: " << __DATE__
#ifdef _PEPS_DEBUG_
      << " (debug version)\n\n";
#else
      << " (optimized version)\n\n";
#endif
}

//====================================================================================//
//  Say Goodbye                                                                       //
//====================================================================================//
void Say_Goodbye()
{
 cout << "\n\nThanks for using PEPS!\n\n";
}
//====================================================================================//
//  Say Sorry                                                                         //
//====================================================================================//
void Say_Sorry()
{
 cout << "\nI'm sorry this option does not work right now!\n";
}
//====================================================================================//
//  Welcome_Get_Peps_Option                                                           //
//====================================================================================//
peps_options Welcome_Get_Peps_Option()
{
 char         option;
 peps_options answer = nothing;
 peps_name    buf;

 do
{cout << "\n\n     +--------------------------------------------------------+"
      <<   "\n     |            This is PEPS 2009 - the SAN tool            |"
      <<   "\n     |  released on: " << __RELEASE_DATE__  << " -- compiled on: " << __DATE__ << "  |"
#ifdef _PEPS_DEBUG_
      <<   "\n     |                  *** debug version ***                 |"
#endif
      <<   "\n     +--------------------------------------------------------+"
      << "\n\n  1) Compile a SAN model             5) Load/Show/Remove data structures"
      <<   "\n  2) Solve a compiled SAN model      6) Inspect data structures"
      <<   "\n  3) Probability vector facilities   7) Sparse matrix facilities"
      <<   "\n  4) Preferences                     8) About this version"
      << "\n\n     0) Exit PEPS (Option 0 always exits the current menu)\n";

 cin >> option;
 switch (option) {
 case '1': cout << "\n\n             ******* Compiling a SAN Model *******"
                << "\n\nWith Aggregation:                      Without Aggregation:"
                << "\n\n 1) Compile, Aggregate and Normalize    6) Compile and Normalize"
                <<   "\n    (san -> des -> dsc -> agg -> cnd)      (san -> des -> dsc -> cnd)"
                <<   "\n 2) Compile and Aggregate               7) Compile"
                <<   "\n    (san -> des -> dsc -> agg)             (san -> des -> dsc)"
                <<   "\n 3) Aggregate and Normalize             8) Normalize"
                <<   "\n    (dsc -> agg -> cnd)                    (dsc -> cnd)"
                <<   "\n 4) Aggregate"
                <<   "\n    (dsc -> agg)                      If you don't know what to do"
                <<   "\n 5) Normalize                          you should try option 6!!"
                <<   "\n    (agg -> cnd)\n";
           cin >> option;
           switch (option) {
           case '1': answer = comp_aggr_norm; break;
           case '2': answer = comp_aggr;      break;
           case '3': answer = aggr_norm;      break;
           case '4': answer = aggregate;      break;
           case '5': answer = normalize2;     break;
           case '6': answer = comp_norm;      break;
           case '7': answer = compile;        break;
           case '8': answer = normalize1;     } break;
 case '2': cout << "\n\n       ******* Solving a SAN model *******"
                << "\n\nNo Preconditioning:      Diagonal Preconditioning:"
                << "\n\n 1) Power Method             4) Power Method"
                <<   "\n 2) Arnoldi Method           5) Arnoldi Method"
                <<   "\n 3) GMRES Method             6) GMRES Method\n";
           cin >> option;
           switch (option) {
           case '1': answer = power_std;    break;
           case '2': answer = arnoldi_std;  break;
           case '3': answer = gmres_std;    break;
           case '4': answer = power_diag;   break;
           case '5': answer = arnoldi_diag; break;
           case '6': answer = gmres_diag;   } break;
 case '3': cout << "\n\n         ******* Probability Vector Facilities *******"
                << "\n\n 1) local state probabilities   5) global state probabilities"
                <<   "\n 2) integrate results           6) compute residue"
                <<   "\n 3) compare two vectors         7) vector characteristics"
                <<   "\n 4) inspect probability vector  8) inspect reachable state space\n"
                <<   "\n                 9) import a MARCA vector\n";
         cin >> option;
         switch (option) {
         case  '1': answer =   integrate_local; break;
         case  '2': answer = integrate_results; break;
         case  '3': answer =       compare_vct; break;
         case  '4': answer =         slice_vct; break;
         case  '5': answer =   look_global_vct; break;
         case  '6': answer =       residue_vct; break;
         case  '7': answer =        charac_vct; break;
         case  '8': answer = analyse_reachable; break;
         case  '9': answer =  import_marca_vct; } break;
 case '4': cout << "\n\n         ******* Changing Preferences *******"
                << "\n\n 1) Verbose Mode:                                  ";
           PRF::prf.sverb(buf); cout << buf;
           cout <<   "\n 2) Maximum number of iterations:                  "
                << PRF::prf.max_it()
                <<   "\n 3) Stop iteration criterion:                      ";
           PRF::prf.sit_type(buf); cout << buf;
           cout <<   "\n 4) Tolerance accepted:                            "
                << PRF::prf.min_err()
                <<   "\n 5) Error Test:                                    ";
           PRF::prf.serr_type(buf); cout << buf;
           cout <<   "\n 6) Initial vector:                                ";
           PRF::prf.si_vec(buf); cout << buf;
           cout <<   "\n 7) Krylov subspace size (for projection methods): "
                << PRF::prf.k_size();
	   cout <<   "\n 8) Vector-Descriptor product method :             ";
           PRF::prf.smethod(buf); cout << buf;
	   cout <<   "\n 9) Vector implementation :                        ";
	     PRF::prf.svec_impl(buf); cout << buf;

           cout << "\n\nEnter a number to change the corresponding parameter: ";
         cin >> option;
         switch (option) {
         case  '1': PRF::prf.verb(Ask_an_Answer("Do you want verbose mode")); break;
         case  '2': PRF::prf.max_it(Ask_an_Iteration_Number());               break;
         case  '3': PRF::prf.it_type(Ask_an_Iteration_Type());                break;
         case  '4': PRF::prf.min_err(Ask_an_Error());                         break;
         case  '5': PRF::prf.err_type(Ask_a_Precision_Type());                break;
         case  '6': PRF::prf.i_vec(Ask_an_Initial_Vector_Type());             break;
         case  '7': PRF::prf.k_size(Ask_a_Krylov_Space_Size());               break;
	 case  '8': PRF::prf.method(Ask_a_Product_Method());                  break;
	 case  '9': PRF::prf.vec_impl(Ask_a_Vector_Implementation());
	 } break;


 case '5': cout << "\n\n            ******* Load/Show/Remove Data Structures *******"
                << "\n\n 1) Show/Remove Structures           4) Load Aggregated Descriptor (agg)"
                <<   "\n 2) Load Textual Descriptor (dsc)    5) Load Probability Vector    (vct)"
                <<   "\n 3) Load Normalized Descriptor (cnd) 6) Load Sparse Matrix (hbf)\n";
           cin >> option;
           switch (option) {
           case '1': Show_Structures(); break;
           case '2': answer = read_dsc; break;
           case '3': answer = read_cnd; break;
           case '4': answer = read_agg; break;
           case '5': answer = read_vct; break;
           case '6': answer = read_hbf; } break;
 case '6': cout << "\n\n              ******* Inspect Data Structures *******"
                << "\n\n 1) Descriptor in Memory   (dsc or agg or cnd)"
                <<   "\n 2) Reachable State Space  (rss)"
                <<   "\n 3) Probability Vector     (vct)"
                <<   "\n 4) Sparse Matrix          (hbf)\n";
           cin >> option;
           switch (option) {
           case '1': answer = see_dsc; break;
           case '2': answer = see_rss; break;
           case '3': answer = see_vct; break;
           case '4': answer = see_hbf; } break;
 case '7': cout << "\n\n         ******* Sparse matrix operations *******"
                << "\n\n 1) generate a .hbf file from a SAN"
                <<   "\n 2) solve a SAN model using the sparse algorithm"
                <<   "\n 3) convert a .hbf file to/from MARCA format"
                <<   "\n 4) generate the state numbering of a SAN\n";
           cin >> option;
           switch (option) {
           case '1': answer = generate_hbf; break;
           case '2': answer = power_hbf; break;
           case '3': answer = convert_hbf; break;
           case '4': answer = state_hbf;  } break;
 case '8': answer = credits; break;
 case 'd': cout << "\n\n         ******* Entering PEPS 2000 debug mode *******"
                <<   "\n              *** i.e., you shouldn't be here ***"
                << "\n\n 1) edit inner preferences   5) print rss"
                <<   "\n 2) print dsc                6) print cnd"
                <<   "\n 3) print dct                7) print cnd-diag"
                <<   "\n 4) print ftb                8) print inf"
                <<   "\n             9) Show Structures\n";
           cin >> option;
           switch (option) {
           case '1': Preferences();      break;
           case '2': DSC::dsc.print();   break;
           case '3': DCT::dct.print();   break;
           case '4': FTB::ftb.print();   break;
           case '5': RSS::rss.print();   break;
           case '6': CND::cnd.print();   break;
           case '7': CND::cnd.dprint();  break;
           case '8': INF::inf.print();   break;
           case '9': Show_Structures();  } break;
 default : cout << "\n\n            ******* Before Leaving PEPS *******"
                << "\n\n 1) Do nothing, i.e., keep the all created files in the disk"
                <<   "\n 2) Erase temporary files (des, dsc, agg, cnd) from the disk"
                <<   "\n 3) Erase probability vector files (vct) from the disk"
                <<   "\n 4) Erase solutions report files (tim) from the disk"
                <<   "\n 5) Erase vector and report files (vct, tim) from the disk"
                <<   "\n 6) Erase vector, report and temporary files from the disk\n"
                <<   "\n 0) Don't leave PEPS, i.e., return to main menu\n";
           answer = goodbye;
           cin >> option;
           switch (option) {
           case '1': break;
           case '2': Remove_Temporary_Directories(); break;
           case '3': Remove_Probability_Vectors();   break;
           case '4': Remove_Tim_Files();             break;
           case '5': Remove_Probability_Vectors();
                     Remove_Tim_Files();             break;
           case '6': Remove_Temporary_Directories();
                     Remove_Probability_Vectors();
                     Remove_Tim_Files();             break;
           default : answer = nothing;               } break;
 }
}
 while (answer == nothing);
 return(answer);
}

//====================================================================================//
//                                Show Structures                                     //
//====================================================================================//
void Show_Structures()
{
 int option;
 peps_name name;

 cout << "--------------------------------------------------------------------------\n"
      << " Structures in Memory\n"
      << "--------------------------------------------------------------------------\n";
// NET::net.name(name); cout << " 1) network:               " << name;
// if (NET::net.is_empty()) cout << " (no network in memory)\n";
// else cout << ".net - automata: " << NET::net.aut()
//           << "- events: " << NET::net.ev() << "\n"
//           << "memory space: " << NET.net.mem_use() / mem_unit << " kbytes\n";
 DSC::dsc.name(name); cout << " 1) descriptor:            " << name;
 if (DSC::dsc.is_empty()) cout << " (no descriptor in memory)\n";
 else cout << ".dsc - automata: " << DSC::dsc.aut()
           << " - events: " << DSC::dsc.ev() << "\n"
           << "            memory space: " << DSC::dsc.mem_use() / mem_unit << " Kbytes\n";
 FTB::ftb.name(name); cout << " 2) functions table:       " << name;
 if (FTB::ftb.is_empty()) cout << " (no table in memory)\n";
 else cout << ".ftb " << "- functions: " << FTB::ftb.size() << "\n"
           << "            memory space: " << FTB::ftb.mem_use() / mem_unit << " Kbytes\n";
 RSS::rss.name(name); cout << " 3) reachability vector:   " << name;
 if (RSS::rss.is_empty()) cout << " (no vector in memory)\n";
 else cout << ".rss - states: " << RSS::rss.size() << "\n"
           << "            memory space: " << RSS::rss.mem_use() / mem_unit << " Kbytes\n";
 CND::cnd.name(name); cout << " 4) cont.-time descriptor: " << name;
 if (CND::cnd.is_empty()) cout << " (no norm. desc. in memory)\n";
 else cout << ".cnd - automata: " << CND::cnd.aut()
           << " - events: " << CND::cnd.ev() << "\n"
           << "            memory space: " << CND::cnd.mem_use() / mem_unit << " Kbytes\n";
 LUD::lud.name(name); cout << " 5) LU descriptor:         " << name;
 if (LUD::lud.is_empty()) cout << " (no LU in memory)\n";
 else cout << ".lud - automata: " << LUD::lud.aut()
           << " - events: " << LUD::lud.ev() << "\n"
           << "            memory space: " << LUD::lud.mem_use() / mem_unit << " Kbytes\n";
 HBF::hbf.name(name); cout << " 6) sparse matrix:         " << name;
 if (HBF::hbf.is_empty()) cout << " (no matrix in memory)\n";
 else cout << ".hbf - order: " << HBF::hbf.order()
           << " - nonzeros: " << HBF::hbf.size() << "\n"
           << "            memory space: " << HBF::hbf.mem_use() / mem_unit << " Kbytes\n";
 VCT::vct.name(name); cout << " 7) probability vector:    " << name;
 if (VCT::vct.is_empty()) cout << " (no vector in memory)\n";
 else cout << ".vct - states: " << VCT::vct.real_size() << "\n"
           << "            memory space: " << VCT::vct.mem_use() / mem_unit << " Kbytes\n";
 cout << "--------------------------------------------------------------------------\n"
      << "To free one of these structures enter its number,\n "
      << "to free all of these structures enter number 8 : ";
 cin >> option;
 switch (option) {
// case  1: NET::net.name(name); NET::net.erase();
//          cout << " :-) " << name << ".net took out from the memory\n"; break;
 case  1: DSC::dsc.name(name); DSC::dsc.erase();
          cout << " :-) " << name << ".dsc removed from memory\n"; break;
 case  2: FTB::ftb.name(name); FTB::ftb.erase();
          cout << " :-) " << name << ".ftb removed from memory\n"; break;
 case  3: RSS::rss.name(name); RSS::rss.erase();
          cout << " :-) " << name << ".rss removed from memory\n"; break;
 case  4: CND::cnd.name(name); CND::cnd.erase();
          cout << " :-) " << name << ".cnd removed from memory\n"; break;
 case  5: LUD::lud.name(name); LUD::lud.erase();
          cout << " :-) " << name << ".lud removed from memory\n"; break;
 case  6: HBF::hbf.name(name); HBF::hbf.erase();
          cout << " :-) " << name << ".hbf removed from memory\n"; break;
 case  7: VCT::vct.name(name); VCT::vct.erase();
          cout << " :-) " << name << ".vct removed from memory\n"; break;
 case  8: DSC::dsc.erase();
          FTB::ftb.erase();
          RSS::rss.erase();
          CND::cnd.erase();
          LUD::lud.erase();
          HBF::hbf.erase();
          VCT::vct.erase();
          cout << " :-) all structures removed from memory\n";
default  : cout << " :-) keeping all structures in memory\n";
 }
}



//====================================================================================//
//                                  Preferences                                       //
//====================================================================================//
void Preferences()
{
 peps_name name;

 Show_User_Preferences();
 if (Ask_an_Answer("Do you wish to change these parameters"))
   if (Ask_an_Answer("Do you wish to load preferences from a .prf file")) {
     Ask_an_Existing_File_Name(name, prf_file);
     PRF::prf.read(name);
     Show_User_Preferences();
   }
   else
     Change_User_Preferences();
}
