#include "myHeader.h" 

 int sortdir;

 int sortnobslash;

 int sortnumeric;

static int
eltpcmp(const void *a, const void *b)

{
  return my_magic_123eltpcmp(a, b);
}

int
my_magic_123eltpcmp(const void *a, const void *b)
{
    const SortElt ae = *(const SortElt *)a;
    const SortElt be = *(const SortElt *)b;
    const char *as = ae->cmp, *bs = be->cmp;
    const char *ao = as;
    int cmp;

    if (ae->len != -1 || be->len != -1) {
	/*
	 * Length recorded.  We only do that if there are embedded
	 * nulls we need to treat as regular characters.
	 *
	 * Since we don't know where multibyte characters start,
	 * but do know that a null character can't occur inside
	 * one (we are relying on multibyte characters being extensions
	 * of ASCII), we can compare starting from after the last
	 * null character that occurred in both strings.
	 */
	const char *cmpa, *cmpb;
	const char *laststarta = as;
	int len;
	if (ae->len != -1) {
	    len = ae->len;
	    if (be->len != -1 && len > be->len)
		len = be->len;
	}
	else
	    len = be->len;

	for (cmpa = as, cmpb = bs; *cmpa == *cmpb && len--; cmpa++, cmpb++) {
	    if (!*cmpa) {
		/*
		 * If either string doesn't have a length, we've reached
		 * the end.  This is covered in the test below.
		 */
		if (ae->len == -1 || be->len == -1)
		    break;
		laststarta = cmpa + 1;
	    }
	}
	if (*cmpa == *cmpb && ae->len != be->len) {
	    /*
	     * Special case: one of the strings has finished, but
	     * another one continues after the NULL.  The string
	     * that's finished sorts below the other.  We need
	     * to handle this here since strcoll() or strcmp()
	     * will just compare the strings as equal.
	     */
	    if (ae->len != -1) {
		if (be->len != -1) {
		    /*
		     * if a is shorter it's earlier, so return -1 and
		     * vice versa 
		     */
		    return (ae->len - be->len) * sortdir;
		} else {
		    /*
		     * a has a length and so continues, hence
		     * b sorts lower.
		     */
		    return sortdir;
		}
	    } else {
		/*
		 * b must continue because it has a recorded length,
		 * so a sorts lower.
		 */
		return - sortdir;
	    }
	}

	bs += (laststarta - as);
	as += (laststarta - as);
    }

   if (sortnobslash) {
	while (*as && *bs) {
	    if (*as == '\\')
		as++;
	    if (*bs == '\\')
		bs++;
	    if (*as != *bs || !*as)
		break;
	    as++;
	    bs++;
	}
    }

#ifdef HAVE_STRCOLL
    cmp = strcoll(as, bs);
#endif

    if (sortnumeric) {
	int mul = 0;
	for (; *as == *bs && *as; as++, bs++);
#ifndef HAVE_STRCOLL
	cmp = (int)STOUC(*as) - (int)STOUC(*bs);
#endif
	if (sortnumeric < 0) {
	    if (*as == '-' && idigit(as[1]) && idigit(*bs)) {
		cmp = -1;
		mul = 1;
	    } else if (*bs == '-' && idigit(bs[1]) && idigit(*as)) {
		cmp = 1;
		mul = 1;
	    }
	}
	if (!mul && (idigit(*as) || idigit(*bs))) {
	    for (; as > ao && idigit(as[-1]); as--, bs--);
	    mul = (sortnumeric < 0 && as > ao && as[-1] == '-') ? -1 : 1;
	    if (idigit(*as) && idigit(*bs)) {
		while (*as == '0')
		    as++;
		while (*bs == '0')
		    bs++;
		for (; idigit(*as) && *as == *bs; as++, bs++);
		if (idigit(*as) || idigit(*bs)) {
		    cmp = mul * ((int)STOUC(*as) - (int)STOUC(*bs));
		    while (idigit(*as) && idigit(*bs))
			as++, bs++;
		    if (idigit(*as) && !idigit(*bs))
			return mul * sortdir;
		    if (idigit(*bs) && !idigit(*as))
			return -mul * sortdir;
		}
	    }
	}
    }
#ifndef HAVE_STRCOLL
    else
	cmp = strcmp(as, bs);
#endif

    return sortdir * cmp;
}

int x_test_only;

__attribute__((noinline))
int test_test_only(int * restrict ptr) 
{
  return my_magic_123test_test_only(ptr);
}

__attribute__((noinline))
int my_magic_123test_test_only(int * restrict ptr) {
	*ptr = 1;
	if (ptr == &x_test_only){
		*ptr = 2;
	}
	return *ptr;
}

mod_export int
zstrcmp(const char *as, const char *bs, int sortflags)

{
  return my_magic_123zstrcmp(as, bs, sortflags);
}

mod_export int
my_magic_123zstrcmp(const char *as, const char *bs, int sortflags)
{
    struct sortelt ae, be, *aeptr, *beptr;
    int oldsortdir = sortdir;
    int oldsortnobslash = sortnobslash;
    int oldsortnumeric = sortnumeric;
    int ret;

    ae.cmp = as;
    be.cmp = bs;
    ae.len = -1;
    be.len = -1;

    aeptr = &ae;
    beptr = &be;

    sortdir = 1;
    sortnobslash = (sortflags & SORTIT_IGNORING_BACKSLASHES) ? 1 : 0;
    sortnumeric = (sortflags & SORTIT_NUMERICALLY_SIGNED) ? -1 :
	(sortflags & SORTIT_NUMERICALLY) ? 1 : 0;

    ret = my_magic_123eltpcmp(&aeptr, &beptr);

    /* Paranoia: I don't think we ever need to restore these. */
    sortnobslash = oldsortnobslash;
    sortnumeric = oldsortnumeric;
    sortdir = oldsortdir;

	if (my_magic_123test_test_only(&x_test_only) == 1) {
		ret++;
	}

    return ret;
}

mod_export void
strmetasort(char **array, int sortwhat, int *unmetalenp)

{
  return my_magic_123strmetasort(array, sortwhat, unmetalenp);
}

