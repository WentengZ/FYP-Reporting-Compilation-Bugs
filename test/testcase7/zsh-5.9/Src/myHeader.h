#include "zsh.mdh"
#include "subst.pro"
#define LF_ARRAY	1
#define Nularg  ((char) 0xa1)
#ifdef DEBUG
#else
#endif
#define isend(c) ( !(c) || (c)=='/' || (c)==Inpar || (assign && (c)==':') )
#define isend2(c) ( !(c) || (c)==Inpar || (assign && (c)==':') )
#ifdef MULTIBYTE_SUPPORT
#define WCPADWIDTH(cchar, mw)	wcpadwidth(cchar, mw)
#else
#define WCPADWIDTH(cchar, mw)	(1)
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#define	isstring(c) ((c) == '$' || (char)(c) == String || (char)(c) == Qstring)
#define isbrack(c)  ((c) == '[' || (char)(c) == Inbrack)
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#if 0
#else
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#endif
#ifdef MULTIBYTE_SUPPORT
#define WCPADWIDTH(cchar, mw)	wcpadwidth(cchar, mw)

/*
 * Width of character for padding purposes.
 * 0: all characters count 1.
 * 1: use width of multibyte character.
 * 2: non-zero width characters count 1, zero width 0.
 */
static int
wcpadwidth(wchar_t wc, int multi_width)
{
    int width;

    switch (multi_width)
    {
    case 0:
	return 1;

    case 1:
	width = WCWIDTH(wc);
	if (width >= 0)
	    return width;
	return 0;

    default:
	return WCWIDTH(wc) > 0 ? 1 : 0;
    }
}

#else
#define WCPADWIDTH(cchar, mw)	(1)
#endif
extern char nulstring[] ;

LinkNode my_magic_123keyvalpairelement(LinkList list, LinkNode node)
;

mod_export void my_magic_123prefork(LinkList list, int flags, int *ret_flags)
;

char * my_magic_123stringsubstquote(char *strstart, char **pstrdpos)
;

LinkNode my_magic_123stringsubst(LinkList list, LinkNode node, int pf_flags, int *ret_flags,
	    int asssub)
;

char * my_magic_123quotesubst(char *str)
;

mod_export void my_magic_123globlist(LinkList list, int flags)
;

mod_export void my_magic_123singsub(char **s)
;

int my_magic_123multsub(char **s, int pf_flags, char ***a, int *isarr, char *sep,
	int *ms_flags)
;

mod_export void my_magic_123filesub(char **namptr, int assign)
;

char * my_magic_123equalsubstr(char *str, int assign, int nomatch)
;

mod_export int my_magic_123filesubstr(char **namptr, int assign)
;

char * my_magic_123strcatsub(char **d, char *pb, char *pe, char *src, int l, char *s, int glbsub,
	  int copied)
;

char * my_magic_123dopadding(char *str, int prenum, int postnum, char *preone, char *postone,
	  char *premul, char *postmul
#ifdef MULTIBYTE_SUPPORT
	  , int multi_width
#endif
    )
;

char * my_magic_123get_strarg(char *s, int *lenp)
;

int my_magic_123get_intarg(char **s, int *delmatchp)
;

int my_magic_123subst_parse_str(char **sp, int single, int err)
;

char * my_magic_123substevalchar(char *ptr)
;

char * my_magic_123untok_and_escape(char *s, int escapes, int tok_arg)
;

char * my_magic_123check_colon_subscript(char *str, char **endp)
;

LinkNode my_magic_123paramsubst(LinkList l, LinkNode n, char **str, int qt, int pf_flags,
	   int *ret_flags)
;

char * my_magic_123arithsubst(char *a, char **bptr, char *rest)
;

void my_magic_123modify(char **str, char **ptr, int inbrace)
;

char * my_magic_123dstackent(char ch, int val)
;

