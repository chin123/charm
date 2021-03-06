#ifndef _SYMBOL_H
#define _SYMBOL_H

#include "CEntry.h"
#include "EToken.h"
#include "sdag-globals.h"
#include "xi-util.h"

#include "xi-AstNode.h"
#include "xi-Construct.h"
#include "xi-Entry.h"
#include "xi-Member.h"
#include "xi-Message.h"
#include "xi-Module.h"
#include "xi-Parameter.h"
#include "xi-Type.h"
#include "xi-Value.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <list>

namespace xi {

// Reserved words
struct rwentry {
  const char* res;
  int tok;
};

class Chare;
class CStateVar;
class EncapState;

extern void generateVarSignature(XStr& decls, XStr& defs, const Entry* entry,
                                 bool declareStatic, const char* returnType,
                                 const XStr* name, bool isEnd,
                                 std::list<CStateVar*>* params);
extern void generateVarSignature(XStr& decls, XStr& defs, const Chare* chare,
                                 bool declareStatic, const char* returnType,
                                 const XStr* name, bool isEnd,
                                 std::list<CStateVar*>* params);

/******************* Utilities ****************/

extern const char* cur_file;

/****************** Modules, etc. ****************/

extern void RemoveSdagComments(char*);

void generateLocalWrapper(XStr& decls, XStr& defs, int isVoid, XStr& signature,
                          Entry* entry, std::list<CStateVar*>* params, XStr* next, bool isDummy=false);

}  // namespace xi

#endif
