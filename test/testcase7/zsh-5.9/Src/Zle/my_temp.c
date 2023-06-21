#include "myHeader.h"

int
my_magic_123do_completion(UNUSED(Hookdef dummy), Compldat dat)
{
    int ret = 0, lst = dat->lst, incmd = dat->incmd, osl = showinglist;
    char *s = dat->s;
    char *opm;
    LinkNode n;

    METACHECK();

    pushheap();

    ainfo = fainfo = NULL;
    matchers = newlinklist();

    zsfree(compqstack);
    compqstack = zalloc(2);
    /*
     * It looks like we may need to do stuff with backslashes even
     * if instring is QT_NONE.
     */
    *compqstack = (instring == QT_NONE) ? QT_BACKSLASH : (char)instring;
    compqstack[1] = '\0';

    hasunqu = 0;
    useline = (wouldinstab ? -1 : (lst != COMP_LIST_COMPLETE));
    useexact = isset(RECEXACT);
    zsfree(compexactstr);
    compexactstr = ztrdup("");
    uselist = (useline ?
	       ((isset(AUTOLIST) && !isset(BASHAUTOLIST)) ? 
		(isset(LISTAMBIGUOUS) ? 3 : 2) : 0) : 1);
    zsfree(comppatmatch);
    opm = comppatmatch = ztrdup(useglob ? "*" : "");
    zsfree(comppatinsert);
    comppatinsert = ztrdup("menu");
    forcelist = 0;
    haspattern = 0;
    complistmax = getiparam("LISTMAX");
    zsfree(complastprompt);
    complastprompt = ztrdup(isset(ALWAYSLASTPROMPT) ? "yes" : "");
    dolastprompt = 1;
    zsfree(complist);
    complist = ztrdup(isset(LISTROWSFIRST) ?
		      (isset(LISTPACKED) ? "packed rows" : "rows") :
		      (isset(LISTPACKED) ? "packed" : ""));
    startauto = isset(AUTOMENU);
    movetoend = ((zlemetacs == we || isset(ALWAYSTOEND)) ? 2 : 1);
    showinglist = 0;
    hasmatched = hasunmatched = 0;
    minmlen = 1000000;
    maxmlen = -1;
    compignored = 0;
    nmessages = 0;
    hasallmatch = 0;

    /* Make sure we have the completion list and compctl. */
    if (my_magic_123makecomplist(s, incmd, lst)) {
	/* Error condition: feeeeeeeeeeeeep(). */
	zlemetacs = 0;
	foredel(zlemetall, CUT_RAW);
	inststr(origline);
	zlemetacs = origcs;
	clearlist = 1;
	ret = 1;
	minfo.cur = NULL;
	if (useline < 0) {
	    /* unmetafy line before calling ZLE */
	    unmetafy_line();
	    ret = selfinsert(zlenoargs);
	    metafy_line();
	}
	goto compend;
    }
    zsfree(lastprebr);
    zsfree(lastpostbr);
    lastprebr = lastpostbr = NULL;

    if (comppatmatch && *comppatmatch && comppatmatch != opm)
	haspattern = 1;
    if (iforcemenu) {
	if (nmatches)
            do_ambig_menu();
	ret = !nmatches;
    } else if (useline < 0) {
	/* unmetafy line before calling ZLE */
	unmetafy_line();
	ret = selfinsert(zlenoargs);
	metafy_line();
    } else if (!useline && uselist) {
	/* All this and the guy only wants to see the list, sigh. */
	zlemetacs = 0;
	foredel(zlemetall, CUT_RAW);
	inststr(origline);
	zlemetacs = origcs;
	showinglist = -2;
    } else if (useline == 2 && nmatches > 1) {
	do_allmatches(1);

	minfo.cur = NULL;

	if (forcelist)
	    showinglist = -2;
	else
	    invalidatelist();
    } else if (useline) {
	/* We have matches. */
	if (nmatches > 1 && diffmatches) {
	    /* There is more than one match. */
	    ret = do_ambiguous();

	    if (!showinglist && uselist && listshown && (usemenu == 2 || oldlist))
		showinglist = osl;
	} else if (nmatches == 1 || (nmatches > 1 && !diffmatches)) {
	    /* Only one match. */
	    Cmgroup m = amatches;
#ifdef ZSH_HEAP_DEBUG
	    if (memory_validate(m->heap_id)) {
		HEAP_ERROR(m->heap_id);
	    }
#endif

	    while (!m->mcount)
		m = m->next;
	    minfo.cur = NULL;
	    minfo.asked = 0;
	    do_single(m->matches[0]);
	    if (forcelist) {
		if (uselist)
		    showinglist = -2;
		else
		    clearlist = 1;
	    } else
		invalidatelist();
	} else if (nmessages && forcelist) {
	    if (uselist)
		showinglist = -2;
	    else
		clearlist = 1;
	}
    } else {
	invalidatelist();
	lastambig = isset(BASHAUTOLIST);
	if (forcelist)
	    clearlist = 1;
	zlemetacs = 0;
	foredel(zlemetall, CUT_RAW);
	inststr(origline);
	zlemetacs = origcs;
    }
    /* Print the explanation strings if needed. */
    if (!showinglist && validlist && usemenu != 2 && uselist &&
	(nmatches != 1 || diffmatches) &&
	useline >= 0 && useline != 2 && (!oldlist || !listshown)) {
	onlyexpl = 3;
	showinglist = -2;
    }
 compend:
    for (n = firstnode(matchers); n; incnode(n))
	freecmatcher((Cmatcher) getdata(n));

    zlemetall = strlen(zlemetaline);
    if (zlemetacs > zlemetall)
	zlemetacs = zlemetall;
    popheap();

    return ret;
}

