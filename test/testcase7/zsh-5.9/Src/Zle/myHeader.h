#include "complete.mdh"
#include "compcore.pro"
#if 0
#endif
#define inststr(X) inststrlen((X),1,-1)
#ifdef ZSH_HEAP_DEBUG
#endif
#ifdef ZSH_HEAP_DEBUG
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#if 0
#endif
#ifdef ZSH_HEAP_DEBUG
#endif
#ifdef DEBUG
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef ZSH_HEAP_DEBUG
#endif
#ifdef ZSH_HEAP_DEBUG
#endif
#define matchstreq(a, b) ((!(a) && !(b)) || ((a) && (b) && !strcmp((a), (b))))
#ifdef ZSH_HEAP_DEBUG
#endif
#ifdef ZSH_HEAP_DEBUG
#endif
extern int useexact;

extern int useexact, useline;

extern int useexact, useline, uselist;

extern int useexact, useline, uselist, forcelist;

extern int useexact, useline, uselist, forcelist, startauto;

extern mod_export int iforcemenu;

extern mod_export int dolastprompt;

extern mod_export int oldlist;

extern int origlpre;

extern int origlpre, origlsuf;

extern int origlpre, origlsuf, lenchanged;

extern int movetoend;

extern mod_export int insmnum;

extern mod_export struct menuinfo minfo;

extern mod_export int menuacc;

extern int hasunqu;

extern int hasunqu, useqbr;

extern int hasunqu, useqbr, brpcs;

extern int hasunqu, useqbr, brpcs, brscs;

extern mod_export int ispar;

extern char *parpre;

extern int parflags;

extern mod_export int mflags;

extern int parq;

extern int parq, eparq;

extern mod_export char *ipre;

extern mod_export LinkList;

extern LinkList fmatches;

extern mod_export Cmgroup;

extern mod_export int hasoldlist;

extern int hasallmatch;

extern mod_export int newmatches;

extern mod_export int permmnum;

extern mod_export int nmatches;

extern mod_export int smatches;

extern mod_export int diffmatches;

extern mod_export int nmessages;

extern mod_export int onlyexpl;

extern mod_export struct cldata listdat;

extern mod_export int ispattern;

extern mod_export int hasmatched;

extern Cmgroup mgroup;

extern mod_export int mnum;

extern mod_export int unambig_mnum;

extern int maxmlen;

extern int maxmlen, minmlen;

extern LinkList expls;

extern mod_export Cexpl;

extern mod_export Cmlist;

extern mod_export Cmlist;

extern mod_export LinkList;

extern mod_export Cline;

extern mod_export Aminfo;

extern mod_export Heap;

extern mod_export LinkList;

extern int fromcomp;

extern mod_export int lastend;

int my_magic_123do_completion(UNUSED(Hookdef dummy), Compldat dat)
;

extern  int oldmenucmp;

int my_magic_123before_complete(UNUSED(Hookdef dummy), int *lst)
;

int my_magic_123after_complete(UNUSED(Hookdef dummy), int *dat)
;

extern  int parwb;

extern  int parwb, parwe;

extern  int parwb, parwe, paroffs;

void my_magic_123callcompfunc(char *s, char *fn)
;

int my_magic_123makecomplist(char *s, int incmd, int lst)
;

mod_export char * my_magic_123multiquote(char *s, int ign)
;

mod_export char * my_magic_123tildequote(char *s, int ign)
;

mod_export char * my_magic_123check_param(char *s, int set, int test)
;

mod_export char * my_magic_123rembslash(char *s)
;

mod_export int my_magic_123remsquote(char *s)
;

mod_export char * my_magic_123ctokenize(char *p)
;

mod_export char * my_magic_123comp_str(int *ipl, int *pl, int untok)
;

mod_export char * my_magic_123comp_quoting_string(int stype)
;

int my_magic_123set_comp_sep(void)
;

mod_export void my_magic_123set_list_array(char *name, LinkList l)
;

mod_export char ** my_magic_123get_user_var(char *nam)
;

char ** my_magic_123get_data_arr(char *name, int keys)
;

void my_magic_123addmatch(char *str, int flags, char ***dispp, int line)
;

int my_magic_123addmatches(Cadata dat, char **argv)
;

extern mod_export Cmatch;

int my_magic_123matchcmp(Cmatch *a, Cmatch *b)
;

int my_magic_123matcheq(Cmatch a, Cmatch b)
;

Cmatch * my_magic_123makearray(LinkList l, int type, int flags, int *np, int *nlp, int *llp)
;

Cmatch my_magic_123dupmatch(Cmatch m, int nbeg, int nend)
;

mod_export int my_magic_123permmatches(int last)
;

void my_magic_123freematch(Cmatch m, int nbeg, int nend)
;

mod_export void my_magic_123freematches(Cmgroup g, int cm)
;

