/* Generated automatically */
static void zfhandler _((int sig));
static void zfalarm _((int tmout));
static void zfpipe _((void));
static void zfunalarm _((void));
static void zfunpipe _((void));
static int zfmovefd _((int fd));
static void zfsetparam _((char*name,void*val,int flags));
static void zfunsetparam _((char*name));
static char*zfargstring _((char*cmd,char**args));
static int zfgetline _((char*ln,int lnsize,int tmout));
static int zfgetmsg _((void));
static int zfsendcmd _((char*cmd));
static int zfopendata _((char*name,union tcp_sockaddr*zdsockp,int*is_passivep));
static void zfclosedata _((void));
static int zfgetdata _((char*name,char*rest,char*cmd,int getsize));
static int zfstats _((char*fnam,int remote,off_t*retsize,char**retmdtm,int fd));
static void zfstarttrans _((char*nam,int recv,off_t sz));
static void zfendtrans _((void));
static int zfread _((int fd,char*bf,off_t sz,int tmout));
static int zfwrite _((int fd,char*bf,off_t sz,int tmout));
static int zfread_block _((int fd,char*bf,off_t sz,int tmout));
static int zfwrite_block _((int fd,char*bf,off_t sz,int tmout));
static int zfsenddata _((char*name,int recv,int progress,off_t startat));
static int zftp_open _((char*name,char**args,int flags));
static char*zfgetinfo _((char*prompt,int noecho));
static int zftp_params _((UNUSED(char*name),char**args,UNUSED(int flags)));
static int zftp_login _((char*name,char**args,UNUSED(int flags)));
static int zftp_test _((UNUSED(char*name),UNUSED(char**args),UNUSED(int flags)));
static int zftp_dir _((char*name,char**args,int flags));
static int zftp_cd _((UNUSED(char*name),char**args,int flags));
static int zfgetcwd _((void));
static int zfsettype _((int type));
static int zftp_type _((char*name,char**args,int flags));
static int zftp_mode _((char*name,char**args,UNUSED(int flags)));
static int zftp_local _((UNUSED(char*name),char**args,int flags));
static int zftp_getput _((char*name,char**args,int flags));
static int zftp_delete _((UNUSED(char*name),char**args,UNUSED(int flags)));
static int zftp_mkdir _((UNUSED(char*name),char**args,int flags));
static int zftp_rename _((UNUSED(char*name),char**args,UNUSED(int flags)));
static int zftp_quote _((UNUSED(char*name),char**args,int flags));
static void zfclose _((int leaveparams));
static int zftp_close _((UNUSED(char*name),UNUSED(char**args),UNUSED(int flags)));
static void newsession _((char*nm));
static void switchsession _((char*nm));
static void freesession _((Zftp_session sptr));
static int zftp_session _((UNUSED(char*name),char**args,UNUSED(int flags)));
static int zftp_rmsession _((UNUSED(char*name),char**args,UNUSED(int flags)));
static int bin_zftp _((char*name,char**args,UNUSED(Options ops),UNUSED(int func)));