#
# Makefile of GTAexpress - Kronecker Descriptor Solver
#
# If you prefer, enter a full path to the peps compilation directory
#  replacing the '.' below for the full path (for example: '/home/toto/gtaexpress')

_DIR = .

SRC_PF_DIR = $(_DIR)/src/pf
SRC_LB_DIR = $(_DIR)/src/lb
OBJ_DIR = $(_DIR)/obj
BIN_DIR = $(_DIR)/bin

# The compilers available (xlC for IBM-AIX, CC for SUN-Solaris
#                          egcs++ from GNU for Red Hat version 7.0
#                          or g++ from GNU for everything - recommended)

#CPP=xlC -D_XLC_COMPILER_
#CPP=/opt/SUNWspro/bin/CC -D_CC_COMPILER_
#CPP=egcs++
#CPP=iccbin
CPP=g++

# The options of compilation (debug or optimization)

#CPPFLAGS= -O3 -xW -parallel
#CPPFLAGS= -O3 -Wno-deprecated -Wswitch-bool

# -w flag will DISABLE all warnings!
CPPFLAGS= -O3 -w

# The options of linkage - UNCOMMENT NEXT LINE FOR LINUX
#LDFLAGS= -ldl -i_dynamic
LDFLAGS= -ldl

# The options of parser generation

#LEX = flex
LEX = lex
#YACC = bison
YACC = yacc

# The source files to compile

#$(OBJ_DIR)/aunf.o \

FILES= $(OBJ_DIR)/autset.o \
	$(OBJ_DIR)/autlist.o \
	$(OBJ_DIR)/autst.o \
	$(OBJ_DIR)/dgraph.o \
	$(OBJ_DIR)/bools.o \
	$(OBJ_DIR)/elem.o \
	$(OBJ_DIR)/elemlist.o \
	$(OBJ_DIR)/sanmat.o \
	$(OBJ_DIR)/spmat.o \
	$(OBJ_DIR)/fullmat.o \
	$(OBJ_DIR)/stmap.o \
	$(OBJ_DIR)/jumps.o \
	$(OBJ_DIR)/functree.o \
	$(OBJ_DIR)/scrft.o \
	$(OBJ_DIR)/dsc.o \
	$(OBJ_DIR)/ftb.o \
	$(OBJ_DIR)/rss.o \
	$(OBJ_DIR)/inf.o \
	$(OBJ_DIR)/cnd.o \
	$(OBJ_DIR)/vct.o \
	$(OBJ_DIR)/vct2.o \
	$(OBJ_DIR)/hbf.o \
	$(OBJ_DIR)/lud.o \
	$(OBJ_DIR)/prf.o \
	$(OBJ_DIR)/dct.o \
	$(OBJ_DIR)/interf.o \
	$(OBJ_DIR)/compiler.o \
	$(OBJ_DIR)/solve.o \
	$(OBJ_DIR)/timer.o \
	$(OBJ_DIR)/peps.o \
	$(OBJ_DIR)/lex.o \
	$(OBJ_DIR)/yacc.o \
	$(OBJ_DIR)/net.o \
	$(OBJ_DIR)/inf2pos.o \
	$(OBJ_DIR)/put2file.o \
	$(OBJ_DIR)/put2file_d.o \
	$(OBJ_DIR)/autom.o \
	$(OBJ_DIR)/state.o \
	$(OBJ_DIR)/trans.o \
	$(OBJ_DIR)/event.o \
	$(OBJ_DIR)/func.o \
	$(OBJ_DIR)/dom.o \
	$(OBJ_DIR)/edic.o \
	$(OBJ_DIR)/autom_.o \
	$(OBJ_DIR)/error_msgs_.o \
	$(OBJ_DIR)/dom_.o \
	$(OBJ_DIR)/edic_.o \
	$(OBJ_DIR)/func_.o \
	$(OBJ_DIR)/grammar_.o \
	$(OBJ_DIR)/grammar_lex_.o \
	$(OBJ_DIR)/grammar_syn_ev_.o \
	$(OBJ_DIR)/grammar_syn_id_.o \
	$(OBJ_DIR)/grammar_syn_net_.o \
	$(OBJ_DIR)/grammar_syn_reach_.o \
	$(OBJ_DIR)/grammar_syn_res_.o \
	$(OBJ_DIR)/ndic_.o \
	$(OBJ_DIR)/state_.o \

all: $(BIN_DIR)/gtaexpress 

$(BIN_DIR)/gtaexpress: $(FILES)
#	$(CPP) $(LDFLAGS) $(FILES) $(LOADLIBS) -o $@
	$(CPP) $(FILES) $(LOADLIBS) $(LDFLAGS) -o $@

rebuild_all : parser code

code : all

parser : make_yacc

make_yacc : make_lex $(SRC_LB_DIR)/glc.y
	$(YACC) -tdv -o$(SRC_LB_DIR)/yacc.C $(SRC_LB_DIR)/glc.y

make_lex: $(SRC_LB_DIR)/tokens.l
	$(LEX) -o$(SRC_LB_DIR)/lex.C $(SRC_LB_DIR)/tokens.l

$(OBJ_DIR)/%.o : $(SRC_LB_DIR)/%.C
	$(CPP) -c $(CPPFLAGS) $< -o $@ 

$(OBJ_DIR)/%.o : $(SRC_PF_DIR)/%.C
	$(CPP) -c $(CPPFLAGS) $< -o $@ 

# Facilities

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN_DIR)/gtaexpress

clean_yacc:
	rm -f $(SRC_LB_DIR)/lex.C
	rm -f $(SRC_LB_DIR)/yacc.C

clean_all: clean clean_yacc

