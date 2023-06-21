#include "myHeader.h"

mod_export void
my_magic_123strmetasort(char **array, int sortwhat, int *unmetalenp)
{
    char **arrptr;
    /*
     * Array of elements containing stuff to sort.  Note sortptrarr
     * is an array of pointers, since that's more efficient
     * for qsort() to manipulate.  sortarr is the array of
     * structures.
     */
    SortElt *sortptrarr, *sortptrarrptr;
    SortElt sortarr, sortarrptr;
    int oldsortdir, oldsortnumeric, nsort;

    nsort = arrlen(array);
    if (nsort < 2)
	return;

    pushheap();

    sortptrarr = (SortElt *) zhalloc(nsort * sizeof(SortElt));
    sortarr = (SortElt) zhalloc(nsort * sizeof(struct sortelt));
    for (arrptr = array, sortptrarrptr = sortptrarr, sortarrptr = sortarr;
	 *arrptr; arrptr++, sortptrarrptr++, sortarrptr++) {
	char *metaptr;
	int needlen, needalloc;
	*sortptrarrptr = sortarrptr;
	sortarrptr->orig = *arrptr;

	if (unmetalenp) {
	    /*
	     * Already unmetafied.  We just need to check for
	     * embedded nulls.
	     */
	    int count = unmetalenp[arrptr-array];
	    /* Remember this length for sorted array */
	    sortarrptr->origlen = count;
	    for (metaptr = *arrptr; *metaptr != '\0' && count--; metaptr++)
		;
	    /* *metaptr must now be \0, even if we reached the end */
	    needlen = (count != 0);
	} else {
	    /*
	     * Not yet unmetafied.  See if it needs unmetafying.
	     * If it doesn't, there can't be any embedded nulls,
	     * since these are metafied.
	     */
	    needlen = 0;
	    for (metaptr = *arrptr; *metaptr && *metaptr != Meta;
		 metaptr++);
	}
	/*
	 * See if we need to do some special checking.
	 * Either we're going to need to copy it to transform it,
	 * or we need to unmetafy it.
	 */
	if ((needalloc = (sortwhat &
			  (SORTIT_IGNORING_CASE|SORTIT_IGNORING_BACKSLASHES)))
	    || *metaptr == Meta) {
	    char *s, *t, *src = *arrptr, *dst;
	    int len;
	    sortarrptr->cmp = dst =
		(char *)zhalloc(((sortwhat & SORTIT_IGNORING_CASE)?2:1)*strlen(src)+1);

	    if (unmetalenp) {
		/* Already unmetafied and we have the length. */
		len = unmetalenp[arrptr-array];
	    } else if (*metaptr != '\0') {
		/*
		 * Needs unmetafying.  We need to check for
		 * embedded nulls while we do this.
		 */
		char *t = dst + (metaptr - src);

		if (metaptr != src)
		    memcpy(dst, src, metaptr - src);
		while ((*t = *metaptr++)) {
		    if (*t++ == Meta) {
			if ((t[-1] = *metaptr++ ^ 32) == '\0')
			    needlen = 1;
		    }
		}
		len = t - dst;
		src = dst;
	    } else {
		/*
		 * Doesn't need unmetafying.
		 * This means metaptr is the NULL at the
		 * end of the string, so we have the length, and
		 * there are no embedded nulls, so we don't
		 * need the length later.
		 * We're only copying the string to transform it
		 * below.
		 */
		len = metaptr - src;
	    }
	    if (sortwhat & SORTIT_IGNORING_CASE) {
		char *send = src + len;
#ifdef MULTIBYTE_SUPPORT
		if (isset(MULTIBYTE)) {
		    /*
		     * Lower the case the hard way.  Convert to a wide
		     * character, process that, and convert back.  We
		     * don't assume the characters have the same
		     * multibyte length.  We can't use casemodify()
		     * because we have unmetafied data, which may have
		     * been passed down to use.
		     */
		    mbstate_t mbsin, mbsout;
		    int clen;
		    wchar_t wc;
		    memset(&mbsin, 0, sizeof(mbstate_t));
		    memset(&mbsout, 0, sizeof(mbstate_t));

		    for (s = src, t = dst; s < send; ) {
			clen = mbrtowc(&wc, s, send-s, &mbsin);
			if (clen < 0) {
			    /* invalid or unfinished: treat as single bytes */
			    while (s < send)
				*t++ = tulower(*s++);
			    break;
			}
			if (clen == 0) {
			    /* embedded null */
			    *t++ = '\0';
			    s++;
			    continue;
			}
			s += clen;
			wc = towlower(wc);
			clen = wcrtomb(t, wc, &mbsout);
			t += clen;
			DPUTS(clen < 0, "Bad conversion when lowering case");
		    }
		    *t = '\0';
		    len = t - dst;
		} else
#endif
		    for (s = src, t = dst; s < send; )
			*t++ = tulower(*s++);
		src = dst;
	    }
	    if (sortwhat & SORTIT_IGNORING_BACKSLASHES) {
                char *end = src + len + 1;
		/* copy null byte, so increment length */
		for (s = src, t = dst; s < end; ) {
		    if (*s == '\\') {
			s++;
			len--;
		    }
		    *t++ = *s++;
		}
	    }
	    /* Do we need to store the length (embedded null)? */
	    sortarrptr->len = needlen ? len : -1;
	} else {
	    /*
	     * We can use the string as is, although it's possible
	     * we still need to take account of an embedded null.
	     */
	    sortarrptr->cmp = *arrptr;
	    sortarrptr->len = needlen ? unmetalenp[arrptr-array] : -1;
	}
    }
    /*
     * We probably don't need to restore the following, but it's pretty cheap.
     */
    oldsortdir = sortdir;
    oldsortnumeric = sortnumeric;

    sortdir = (sortwhat & SORTIT_BACKWARDS) ? -1 : 1;
    sortnumeric = (sortwhat & SORTIT_NUMERICALLY_SIGNED) ? -1 :
	(sortwhat & SORTIT_NUMERICALLY) ? 1 : 0;

    qsort(sortptrarr, nsort, sizeof(SortElt), eltpcmp);

    sortnumeric = oldsortnumeric;
    sortdir = oldsortdir;
    for (arrptr = array, sortptrarrptr = sortptrarr; nsort--; ) {
	if (unmetalenp)
	    unmetalenp[arrptr-array] = (*sortptrarrptr)->origlen;
	*arrptr++ = (*sortptrarrptr++)->orig;
    }

    popheap();
}

