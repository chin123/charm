#ifndef _INIT_H
#define _INIT_H

#include <charm.h> // For CkNumPes
#include <new.h>   // for in-place new operator
#include "ckhashtable.h"

typedef CkQ<void *> PtrQ;
class envelope;
typedef CkVec<CkZeroPtr<envelope> > PtrVec;

class IrrGroup;
class TableEntry {
    IrrGroup *obj;
    PtrQ *pending; //Buffers msgs recv'd before group is created
    char gName[256];
    int defCtor,migCtor; // the index of default and migration constructors in _entryTable,
			 // to be fed in CkCreateLocalGroup
  public:
    TableEntry(void) {init();}
    void init(void) { obj=0; pending=0; defCtor=migCtor=-1; }
    inline IrrGroup* getObj(void) { return obj; }
    void setObj(void *_obj) { obj=(IrrGroup *)_obj; }
    PtrQ* getPending(void) { return pending; }
    void enqMsg(void *msg) {
      if (pending==0)
        pending=new PtrQ();
      pending->enq(msg);
    }
    inline int getDefCtor(void){ return defCtor; }
    void setDefCtor(int defCtor_){ defCtor = defCtor_; }
    inline int getMigCtor(void){ return migCtor; }
    void setMigCtor(int migCtor_){ migCtor = migCtor_; }
    inline char* getName(void){ return gName; }
    void setName(const char* gName_){ strcpy(gName,gName_); }
};

template <class dtype>
class GroupIdxArray {
  enum {MAXBINSPE0=256};

  dtype *tab;                           // direct entry table for processor 0
  CkHashtable_c hashTab;
  int max;

  //This non-inline version of "find", below, allows the (much simpler)
  // common case to be inlined.
  dtype& nonInlineFind(CkGroupID n) {
      dtype *ret;
#ifndef CMK_OPTIMIZE
      if (n.idx==0) CkAbort("Group ID is zero-- invalid!\n");
      else if (n.idx>=max) { CkAbort("Group ID is too large!\n");}
      else
#endif
      /*n.idx < 0*/
      { /*Groups created on processors other than 0 go into a hashtable:*/
        if(hashTab == NULL)
          hashTab = CkCreateHashtable_int(sizeof(dtype),17);

        ret = (dtype *)CkHashtableGet(hashTab,&(n.idx));

        if(ret == NULL)               // insert data into the table
        {
          ret = (dtype *)CkHashtablePut(hashTab,&(n.idx));
          new (ret) dtype; //Call dtype's constructor (ICK!)
        }
      }
      return *ret;
   }

  public:
     GroupIdxArray() {tab=NULL;max=0;hashTab=NULL;}
     ~GroupIdxArray() {delete[] tab; if (hashTab!=NULL) CkDeleteHashtable(hashTab);}
     void init(void) {
      max = MAXBINSPE0;
      tab = new dtype[max];
      for(int i=0;i<max;i++)
       tab[i].init();
      hashTab=NULL;
     }

     inline dtype& find(CkGroupID n) {

// TODO: make the table extensible. i.e. if (unsigned)n.idx<max then return tab[n.idx]
// else if (n.idx<0)    then hashtable things
// else extend the table

      if(n.idx>0 && n.idx<MAXBINSPE0)
        return tab[n.idx];
      else
        return nonInlineFind(n);
    }
};

typedef GroupIdxArray<TableEntry> GroupTable;
typedef CkVec<CkGroupID> GroupIDTable;


/*********************************************************/
/**
\addtogroup CkInit
These are implemented in init.C.
*/
/*@{*/
extern unsigned int    _printCS;
extern unsigned int    _printSS;

extern int     _infoIdx;
extern int     _charmHandlerIdx;
extern int     _initHandlerIdx;
extern int     _bocHandlerIdx;
extern int     _nodeBocHandlerIdx;
extern int     _qdHandlerIdx;
extern unsigned int   _numInitMsgs;

CksvExtern(unsigned int,  _numInitNodeMsgs);
CksvExtern(CmiNodeLock, _nodeLock);
CksvExtern(GroupTable*,  _nodeGroupTable);
CksvExtern(GroupIDTable, _nodeGroupIDTable);
CksvExtern(unsigned int, _numNodeGroups);
CkpvExtern(int, _charmEpoch);

CkpvExtern(CkGroupID,_currentGroup);
CkpvExtern(CkGroupID, _currentGroupRednMgr);

CkpvExtern(GroupTable*, _groupTable);
CkpvExtern(GroupIDTable*, _groupIDTable);
CkpvExtern(unsigned int, _numGroups);

CpvExtern(char **,Ck_argv);

static inline IrrGroup *_localBranch(CkGroupID gID)
{
  return CkpvAccess(_groupTable)->find(gID).getObj();
}

extern void _initCharm(int argc, char **argv);

/** This routine registers the user's main module.  It is normally
    generated by the translator, but for FEM and AMPI may actually be 
    the "fallback" version in compat_regmm.c. */
extern "C" void CkRegisterMainModule(void);

typedef  void  (*CkExitFn) (void);

extern CkQ<CkExitFn> CkExitFnVec;
extern void registerExitFn(CkExitFn);

/*@}*/

#endif


