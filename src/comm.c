/***************************************************************************
 *  Original Diku Mud copyright (C) 1990, 1991 by Sebastian Hammer,        *
 *  Michael Seifert, Hans Henrik St{rfeldt, Tom Madsen, and Katja Nyboe.   *
 *                                                                         *
 *  Merc Diku Mud improvments copyright (C) 1992, 1993 by Michael          *
 *  Chastain, Michael Quan, and Mitchell Tse.                              *
 *                                                                         *
 *  Envy Diku Mud improvements copyright (C) 1994 by Michael Quan, David   *
 *  Love, Guilherme 'Willie' Arnold, and Mitchell Tse.                     *
 *                                                                         *
 *  EnvyMud 2.0 improvements copyright (C) 1995 by Michael Quan and        *
 *  Mitchell Tse.                                                          *
 *                                                                         *
 *  EnvyMud 2.2 improvements copyright (C) 1996, 1997 by Michael Quan.     *
 *                                                                         *
 *  In order to use any part of this Envy Diku Mud, you must comply with   *
 *  the original Diku license in 'license.doc', the Merc license in        *
 *  'license.txt', as well as the Envy license in 'license.nvy'.           *
 *  In particular, you may not remove either of these copyright notices.   *
 *                                                                         *
 *  Thanks to abaddon for proof-reading our comm.c and pointing out bugs.  *
 *  Any remaining bugs are, of course, our work, not his.  :)              *
 *                                                                         *
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

/*
 * This file contains all of the OS-dependent stuff:
 *   startup, signals, BSD sockets for tcp/ip, i/o, timing.
 *
 * The data flow for input is:
 *    Game_loop ---> Read_from_descriptor ---> Read
 *    Game_loop ---> Read_from_buffer
 *
 * The data flow for output is:
 *    Game_loop ---> Process_Output ---> Write_to_descriptor -> Write
 *
 * The OS-dependent functions are Read_from_descriptor and Write_to_descriptor.
 * -- Furey  26 Jan 1993
 */

#if defined( WIN32 )
char version_str[] = "$VER: EnvyMud 2.0 Windows 32 Bit Version";
/*
 * Provided by Mystro <http://www.cris.com/~Kendugas/mud.shtml>
 */
#endif

#if defined( AmigaTCP )
char version_str[] = "$VER: Merc/Diku Mud Envy2.0 AmiTCP Version";
/*
 * You must rename or delete the sc:sys/types.h, so the 
 * amitcp:netinclude/sys/types.h will be used instead.
 * Also include these assigns in your user-startup (After the SC assigns)
 *    assign lib: Amitcp:netlib add 
 *    assign include: Amitcp:netinclude add
 * If you haven't allready :)
 * Compilled with SasC 6.56 and AmiTCP 4.2
 * http://www.geocities.com/SiliconValley/1411
 * dkaupp@netcom.com (May be defunct soon)
 * or davek@megnet.net
 * 4-16-96
 */
#endif

#if !defined( WIN32 ) && !defined( AmigaTCP )
char version_str[] = "$VER: EnvyMud 2.0 *NIX";
#endif

#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#if !defined( WIN32 )
#include <sys/time.h>
#endif
#endif

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#if defined( WIN32 )
#include <winsock.h>
#include <sys/timeb.h> /*for _ftime(), uses _timeb struct*/
#endif

#include "merc.h"



/*
 * Malloc debugging stuff.
 */
#if defined( sun )
#undef MALLOC_DEBUG
#endif

#if defined( MALLOC_DEBUG )
#include <malloc.h>
extern	int	malloc_debug	args( ( int  ) );
extern	int	malloc_verify	args( ( void ) );
#endif



/*
 * Signal handling.
 * Apollo has a problem with __attribute( atomic ) in signal.h,
 *   I dance around it.
 */
#if defined( apollo )
#define __attribute( x )
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
#include <signal.h>
#endif

#if defined( apollo )
#undef __attribute
#endif



/*
 * Socket and TCP/IP stuff.
 */
#if	defined( macintosh )
const	char	echo_off_str	[] = { '\0' };
const	char	echo_on_str	[] = { '\0' };
const	char 	go_ahead_str	[] = { '\0' };
#endif

#if	defined( unix ) || defined( AmigaTCP )
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/telnet.h>
const	char	echo_off_str	[] = { IAC, WILL, TELOPT_ECHO, '\0' };
const	char	echo_on_str	[] = { IAC, WONT, TELOPT_ECHO, '\0' };
const	char 	go_ahead_str	[] = { IAC, GA, '\0' };
#endif



/*
 * OS-dependent declarations.
 */
#if	defined( _AIX )
#include <sys/select.h>
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			       int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if     defined( irix )
void	bzero		args( ( char *b, int length ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
int	close		args( ( int fd ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
#endif

#if	defined( __hpux )
int	accept		args( ( int s, void *addr, int *addrlen ) );
int	bind		args( ( int s, const void *addr, int addrlen ) );
void	bzero		args( ( char *b, int length ) );
int	getpeername	args( ( int s, void *addr, int *addrlen ) );
int	getsockname	args( ( int s, void *name, int *addrlen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	setsockopt	args( ( int s, int level, int optname,
			       const void *optval, int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
#endif

#if     defined( interactive )
#include <net/errno.h>
#include <sys/fcntl.h>
#endif

#if	defined( linux )
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			    fd_set *exceptfds, struct timeval *timeout ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined( macintosh )
#include <console.h>
#include <fcntl.h>
#include <unix.h>
struct	timeval
{
	time_t	tv_sec;
	time_t	tv_usec;
};
#if	!defined( isascii )
#define	isascii( c )		( ( c ) < 0200 )
#endif
static	long			theKeys	[4];

int	gettimeofday	args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined( MIPS_OS )
extern	int		errno;
#endif

#if	defined( WIN32 )
const   char echo_off_str[]  = { '\0' };
const   char echo_on_str[]   = { '\0' };
const   char go_ahead_str[]  = { '\0' };
void    gettimeofday    args( ( struct timeval *tp, void *tzp ) );
#endif

#if	defined( NeXT )
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
#if	!defined( htons )
u_short	htons		args( ( u_short hostshort ) );
#endif
#if	!defined( ntohl )
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			       fd_set *exceptfds, struct timeval *timeout ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if	defined( sequent )
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
int	close		args( ( int fd ) );
int	fcntl		args( ( int fd, int cmd, int arg ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#if	!defined( htons )
u_short	htons		args( ( u_short hostshort ) );
#endif
int	listen		args( ( int s, int backlog ) );
#if	!defined( ntohl )
u_long	ntohl		args( ( u_long hostlong ) );
#endif
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			       fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, caddr_t optval,
			       int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

/*
 * This includes Solaris SYSV as well
 */

#if defined( sun )
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
#if !defined( SYSV )
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
#endif
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			       fd_set *exceptfds, struct timeval *timeout ) );
#if defined( SYSV )
int     setsockopt      args( ( int s, int level, int optname,
			       const char *optval, int optlen ) );
#else
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			       int optlen ) );
#endif
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif

#if defined( ultrix )
int	accept		args( ( int s, struct sockaddr *addr, int *addrlen ) );
int	bind		args( ( int s, struct sockaddr *name, int namelen ) );
void	bzero		args( ( char *b, int length ) );
int	close		args( ( int fd ) );
int	getpeername	args( ( int s, struct sockaddr *name, int *namelen ) );
int	getsockname	args( ( int s, struct sockaddr *name, int *namelen ) );
int	gettimeofday	args( ( struct timeval *tp, struct timezone *tzp ) );
int	listen		args( ( int s, int backlog ) );
int	read		args( ( int fd, char *buf, int nbyte ) );
int	select		args( ( int width, fd_set *readfds, fd_set *writefds,
			       fd_set *exceptfds, struct timeval *timeout ) );
int	setsockopt	args( ( int s, int level, int optname, void *optval,
			       int optlen ) );
int	socket		args( ( int domain, int type, int protocol ) );
int	write		args( ( int fd, char *buf, int nbyte ) );
#endif


#if defined( sun )
int     system          args( ( const char *string ) );
#endif




/*
 * Global variables.
 */
DESCRIPTOR_DATA *   descriptor_free;	/* Free list for descriptors	*/
DESCRIPTOR_DATA *   descriptor_list;	/* All open descriptors		*/
DESCRIPTOR_DATA *   d_next;		/* Next descriptor in loop	*/
FILE *		    fpReserve;		/* Reserved file handle		*/
bool		    merc_down;		/* Shutdown                     */
bool		    wizlock;		/* Game is wizlocked		*/
int                 numlock = 0;        /* Game is numlocked at <level> */
char		    str_boot_time [ MAX_INPUT_LENGTH ];
time_t		    current_time;	/* Time of this pulse		*/
int		    players_count = 0;
int		    directs_count = 0;
int		    immortals_count = 0;


/*
 * OS-dependent local functions.
 */
#if defined( macintosh )
void	game_loop_mac_msdos	args( ( void ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
void	game_loop_unix		args( ( int control ) );
int	init_socket		args( ( u_short port ) );
__uid_t getuid			args( ( void ) );
void	new_descriptor		args( ( int control ) );
bool	read_from_descriptor	args( ( DESCRIPTOR_DATA *d ) );
bool	write_to_descriptor	args( ( int desc, char *txt, int length ) );
#endif




/*
 * Other local functions (OS-independent).
 */
bool	check_parse_name	args( ( char *name ) );
bool	check_reconnect		args( ( DESCRIPTOR_DATA *d, char *name,
				       bool fConn ) );
bool	check_playing		args( ( DESCRIPTOR_DATA *d, char *name ) );
int	main			args( ( int argc, char **argv ) );
void	nanny			args( ( DESCRIPTOR_DATA *d, char *argument ) );
bool	process_output		args( ( DESCRIPTOR_DATA *d, bool fPrompt ) );
void	read_from_buffer	args( ( DESCRIPTOR_DATA *d ) );
void	stop_idling		args( ( CHAR_DATA *ch ) );
void    bust_a_prompt           args( ( DESCRIPTOR_DATA *d ) );
char	*rfc931_name		args( ( int desc, struct sockaddr_in *there ) );


int main( int argc, char **argv )
{
    struct  timeval now_time;
    u_short port;
    u_short www_port;

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
    int control;
    int www_control;
#endif

    /*
     * Memory debugging if needed.
     */
#if defined( MALLOC_DEBUG )
    malloc_debug( 2 );
#endif

    /*
     * Init time.
     */
    gettimeofday( &now_time, NULL );
    current_time = (time_t) now_time.tv_sec;
    strcpy( str_boot_time, ctime( &current_time ) );

    /*
     * Macintosh console initialization.
     */
#if defined( macintosh )
    console_options.nrows = 31;
    cshow( stdout );
    csetmode( C_RAW, stdin );
    cecho2file( "log file", 1, stderr );
#endif

    /*
     * Reserve one channel for our use.
     */
    if ( !( fpReserve = fopen( NULL_FILE, "r" ) ) )
    {
	perror( NULL_FILE );
	exit( 1 );
    }

    /*
     * Get the port number.
     */
    port = 4000;
    www_port = 8000;
    
    if ( argc > 1 )
    {
	if ( !is_number( argv[1] ) )
	{
	    fprintf( stderr, "Usage: %s [port #] [www port #]\n", argv[0] );
	    exit( 1 );
	}
	/* If you are the root, you can run the mud in any port. */
	else if ( ( port = atoi( argv[1] ) ) <= 1024 && ( ( int )getuid() != 0 ) )
	{
	    fprintf( stderr, "Port number must be above 1024.\n" );
	    exit( 1 );
	}    
	
        if ( argc > 2 )
        {
            if ( !is_number( argv[2] ) )
            {
                fprintf( stderr, "Usage: %s [port #] [www port #]\n", argv[0] );
                exit( 1 );
            }
        
            else if ( ( www_port = atoi( argv[2] ) ) <= 1024 && ( ( int )getuid() != 0 ) ) 
            {
	        fprintf( stderr, "The port number of WWW server must be above 1024.\n" );
	        exit( 1 );
            }
        }
    }

    /*
     * Run the game.
     */
#if defined( macintosh )
    boot_db( );
    log_string( "Delanta is ready to rock." );
    game_loop_mac_msdos( );
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
    control = init_socket( port );
    www_control = init_socket( www_port );
    boot_db( );
    sprintf( log_buf, "Delanta is ready to rock on port %d.", port );
    log_string( log_buf );
    game_loop_unix( control );
#if !defined( WIN32 )
    close( control );
    close( www_control );
#else
    closesocket( control );
    closesocket( www_control );    
    WSACleanup();
#endif
#endif

    /*
     * That's all, folks.
     */
    log_string( "Normal termination of game." );
    exit( 0 );
    return 0;
}



#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
int init_socket( u_short port )
{
    static struct sockaddr_in sa_zero;
           struct sockaddr_in sa;
                  int         x        = 1; 
                  int         fd;

#if !defined( WIN32 )
    system( "touch SHUTDOWN.TXT" );
    if ( ( fd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
	perror( "Init_socket: socket" );
	exit( 1 );
    }
#else
    WORD wVersionRequested = MAKEWORD( 1, 1 );
    WSADATA wsaData;
    int err = WSAStartup( wVersionRequested, &wsaData ); 
    if ( err != 0 )
    {
	perror("No useable WINSOCK.DLL");
	exit(1);
    }

    if ( ( fd = socket( PF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        perror( "Init_socket: socket" );
	exit( 1 );
    }
#endif

    if ( setsockopt( fd, SOL_SOCKET, SO_REUSEADDR,
    (char *) &x, sizeof( x ) ) < 0 )
    {
	perror( "Init_socket: SO_REUSEADDR" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

#if defined( SO_DONTLINGER ) && !defined( SYSV )
    {
	struct	linger	ld;

	ld.l_onoff  = 1;
	ld.l_linger = 1000;

	if ( setsockopt( fd, SOL_SOCKET, SO_DONTLINGER,
	(char *) &ld, sizeof( ld ) ) < 0 )
	{
	    perror( "Init_socket: SO_DONTLINGER" );
#if !defined( WIN32 )
	    close( fd );
#else
	    closesocket( fd );
#endif
	    exit( 1 );
	}
    }
#endif

    sa		    = sa_zero;
#if !defined( WIN32 )
    sa.sin_family   = AF_INET;
#else
    sa.sin_family   = PF_INET;
#endif
    sa.sin_port	    = htons( port );

    if ( bind( fd, (struct sockaddr *) &sa, sizeof( sa ) ) < 0 )
    {
	perror( "Init_socket: bind" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

    if ( listen( fd, 3 ) < 0 )
    {
	perror( "Init_socket: listen" );
#if !defined( WIN32 )
	close( fd );
#else
	closesocket( fd );
#endif
	exit( 1 );
    }

#if !defined( WIN32 )
    system( "rm SHUTDOWN.TXT" );
#endif
    return fd;
}
#endif



#if defined( macintosh )
void game_loop_mac_msdos( void )
{
    static        DESCRIPTOR_DATA dcon;
           struct timeval         last_time;
           struct timeval         now_time;

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /*
     * New_descriptor analogue.
     */
    dcon.descriptor	= 0;
    dcon.character      = NULL;
    dcon.connected	= CON_GET_NAME;
    dcon.host		= str_dup( "localhost" );
    dcon.outsize	= 2000;
    dcon.outbuf		= alloc_mem( dcon.outsize );
    dcon.showstr_head   = str_dup( "" );
    dcon.showstr_point  = 0;
    dcon.pEdit		= NULL;
    dcon.pString	= NULL;
    dcon.editor		= 0;
    dcon.next		= descriptor_list;
    descriptor_list	= &dcon;

    /*
     * Send the greeting.
     */
    {
        extern char * os_version;
	extern char * help_greeting;        
	
    	write_to_buffer( &dcon, os_version, 0 );
	
	if ( help_greeting[0] == '.' )
	    write_to_buffer( &dcon, help_greeting+1, 0 );
	else
	    write_to_buffer( &dcon, help_greeting  , 0 );
    }

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( d->character )
	        d->character->timer = 0;
	    if ( !read_from_descriptor( d ) )
	    {
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
		continue;
	    }

	    if ( d->character && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );
		
		/* OLC */
		if ( d->showstr_point )
		    show_string( d, d->incomm );
		else
		if ( d->pString )
		    string_add( d->character, d->incomm );
		else
		    if ( d->connected == CON_PLAYING )
		    {
			if ( !run_olc_editor( d ) )
			    interpret( d->character, d->incomm );
		    }
		    else
			nanny( d, d->incomm );
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Busy wait (blargh).
	 */
	now_time = last_time;
	for ( ; ; )
	{
	    int delta;

	    if ( dcon.character )
	        dcon.character->timer = 0;
	    if ( !read_from_descriptor( &dcon ) )
	    {
		if ( dcon.character )
		    save_char_obj( d->character );
		dcon.outtop	= 0;
		close_socket( &dcon );
	    }

	    gettimeofday( &now_time, NULL );
	    delta = ( now_time.tv_sec  - last_time.tv_sec  ) * 1000 * 1000
		  + ( now_time.tv_usec - last_time.tv_usec );
	    if ( delta >= 1000000 / PULSE_PER_SECOND )
		break;
	}
	last_time    = now_time;
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
void game_loop_unix( int control )
{
    static struct timeval null_time;
           struct timeval last_time;

#if !defined( AmigaTCP ) && !defined( WIN32 )
    signal( SIGPIPE, SIG_IGN );
#endif

    gettimeofday( &last_time, NULL );
    current_time = (time_t) last_time.tv_sec;

    /* Main loop */
    while ( !merc_down )
    {
	DESCRIPTOR_DATA *d;
	fd_set           in_set;
	fd_set           out_set;
	fd_set           exc_set;
	int              maxdesc;

#if defined( MALLOC_DEBUG )
	if ( malloc_verify( ) != 1 )
	    abort( );
#endif

	/*
	 * Poll all active descriptors.
	 */
	FD_ZERO( &in_set  );
	FD_ZERO( &out_set );
	FD_ZERO( &exc_set );
	FD_SET( control, &in_set );
	maxdesc	= control;
	for ( d = descriptor_list; d; d = d->next )
	{
	    maxdesc = UMAX( (unsigned) maxdesc, d->descriptor );
	    FD_SET( d->descriptor, &in_set  );
	    FD_SET( d->descriptor, &out_set );
	    FD_SET( d->descriptor, &exc_set );
	}

	if ( select( maxdesc+1, &in_set, &out_set, &exc_set, &null_time ) < 0 )
	{
	    perror( "Game_loop: select: poll" );
	    exit( 1 );
	}

	/*
	 * New connection?
	 */
	if ( FD_ISSET( control, &in_set ) )
	    new_descriptor( control );

	/*
	 * Kick out the freaky folks.
	 */
	for ( d = descriptor_list; d; d = d_next )
	{
	    d_next = d->next;   
	    if ( FD_ISSET( d->descriptor, &exc_set ) )
	    {
		FD_CLR( d->descriptor, &in_set  );
		FD_CLR( d->descriptor, &out_set );
		if ( d->character )
		    save_char_obj( d->character );
		d->outtop	= 0;
		close_socket( d );
	    }
	}

	/*
	 * Process input.
	 */
	for ( d = descriptor_list; d; d = d_next )
	{
	    d_next	= d->next;
	    d->fcommand	= FALSE;

	    if ( FD_ISSET( d->descriptor, &in_set ) )
	    {
		if ( d->character )
		    d->character->timer = 0;
		if ( !read_from_descriptor( d ) )
		{
		    FD_CLR( d->descriptor, &out_set );
		    if ( d->character )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		    continue;
		}
	    }

	    if ( d->character && d->character->wait > 0 )
	    {
		--d->character->wait;
		continue;
	    }

	    read_from_buffer( d );
	    if ( d->incomm[0] != '\0' )
	    {
		d->fcommand	= TRUE;
		stop_idling( d->character );

		/* OLC */
		if ( d->showstr_point )
		{
		    show_string( d, d->incomm );
		}
		else if ( d->pString )
		{
		    string_add( d->character, d->incomm );
		}
		else if ( d->connected == CON_PLAYING )
		{
		    if ( !run_olc_editor( d ) )
		        interpret( d->character, d->incomm );
		}
		else nanny( d, d->incomm );
		
		d->incomm[0]	= '\0';
	    }
	}



	/*
	 * Autonomous game motion.
	 */
	update_handler( );



	/*
	 * Output.
	 */
	for ( d = descriptor_list; d; d = d_next )
	{
	    d_next = d->next;

	    if ( ( d->fcommand || d->outtop > 0 )
		&& FD_ISSET( d->descriptor, &out_set ) )
	    {
		if ( !process_output( d, TRUE ) )
		{
		    if ( d->character )
			save_char_obj( d->character );
		    d->outtop	= 0;
		    close_socket( d );
		}
	    }
	}



	/*
	 * Synchronize to a clock.
	 * Sleep( last_time + 1/PULSE_PER_SECOND - now ).
	 * Careful here of signed versus unsigned arithmetic.
	 */
#if !defined( WIN32 )
	{
	    struct timeval now_time;
	    long secDelta;
	    long usecDelta;

	    gettimeofday( &now_time, NULL );
	    usecDelta	= ( (int) last_time.tv_usec )
	                - ( (int)  now_time.tv_usec )
			+ 1000000 / PULSE_PER_SECOND;
	    secDelta	= ( (int) last_time.tv_sec )
	                - ( (int)  now_time.tv_sec );
	    while ( usecDelta < 0 )
	    {
		usecDelta += 1000000;
		secDelta  -= 1;
	    }

	    while ( usecDelta >= 1000000 )
	    {
		usecDelta -= 1000000;
		secDelta  += 1;
	    }

	    if ( secDelta > 0 || ( secDelta == 0 && usecDelta > 0 ) )
	    {
		struct timeval stall_time;

		stall_time.tv_usec = usecDelta;
		stall_time.tv_sec  = secDelta;
		if ( select( 0, NULL, NULL, NULL, &stall_time ) < 0 )
		{
		    perror( "Game_loop: select: stall" );
		    exit( 1 );
		}
	    }
	}
#else
	{
	    int times_up;
	    int nappy_time;
	    struct _timeb start_time;
	    struct _timeb end_time;
	    _ftime( &start_time );
	    times_up = 0;

	    while( times_up == 0 )
	    {
		_ftime( &end_time );
		if ( ( nappy_time =
		      (int) ( 1000 *
			     (double) ( ( end_time.time - start_time.time ) +
				       ( (double) ( end_time.millitm -
						   start_time.millitm ) /
					1000.0 ) ) ) ) >=
		    (double)( 1000 / PULSE_PER_SECOND ) )
		  times_up = 1;
		else
		{
		    Sleep( (int) ( (double) ( 1000 / PULSE_PER_SECOND ) -
				  (double) nappy_time ) );
		    times_up = 1;
		}
	    }
	}
#endif

	gettimeofday( &last_time, NULL );
	current_time = (time_t) last_time.tv_sec;
    }

    return;
}
#endif



#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
void new_descriptor( int control )
{
    static DESCRIPTOR_DATA  d_zero;
           DESCRIPTOR_DATA *dnew;
    struct sockaddr_in      sock;
    struct hostent         *from;
           BAN_DATA        *pban;
    char                    buf [ MAX_STRING_LENGTH ];
    struct sockaddr_in	   *sine = (struct sockaddr_in *)(&sock);
    int                     desc;
    int                     size;
    int                     addr;
    char		   *user;

    size = sizeof( sock );
    if ( ( desc = accept( control, (struct sockaddr *) &sock, &size) ) < 0 )
    {
	perror( "New_descriptor: accept" );
	return;
    }

#if !defined( FNDELAY )
#if defined( __hpux )
#define FNDELAY O_NONBLOCK
#else
#define FNDELAY O_NDELAY
#endif
#endif

#if !defined( AmigaTCP ) && !defined( WIN32 )
    if ( fcntl( desc, F_SETFL, FNDELAY ) == -1 )
    {
	perror( "New_descriptor: fcntl: FNDELAY" );
	return;
    }
#endif

    /*
     * Cons a new descriptor.
     */
    if ( !descriptor_free )
    {
	dnew		= alloc_perm( sizeof( *dnew ) );
    }
    else
    {
	dnew		= descriptor_free;
	descriptor_free	= descriptor_free->next;
    }

    *dnew		= d_zero;
    dnew->descriptor	= desc;
    dnew->character     = NULL;
    dnew->connected	= CON_GET_NAME;
    dnew->showstr_head  = str_dup( "" );
    dnew->showstr_point = 0;
    dnew->pEdit		= NULL;			/* OLC */
    dnew->pString	= NULL;			/* OLC */
    dnew->editor	= 0;			/* OLC */    
    dnew->outsize	= 2000;
    dnew->outbuf	= alloc_mem( dnew->outsize );

    size = sizeof( sock );

    /*
     * Would be nice to use inet_ntoa here but it takes a struct arg,
     * which ain't very compatible between gcc and system libraries.
     */
     
    user = (char *)rfc931_name( (int)desc, (struct sockaddr_in *)sine);
    dnew->remote_user = str_dup( user );
    
    addr = ntohl( sock.sin_addr.s_addr );
    sprintf( buf, "%d.%d.%d.%d",
	( addr >> 24 ) & 0xFF, ( addr >> 16 ) & 0xFF,
	( addr >>  8 ) & 0xFF, ( addr       ) & 0xFF
	);
    sprintf( log_buf, "Sock.sinaddr:  %s", buf );
    log_string( log_buf );
    from = gethostbyaddr( (char *) &sock.sin_addr,
			 sizeof(sock.sin_addr), AF_INET );
    dnew->host = str_dup( from ? from->h_name : buf );

	
    /*
     * Swiftest: I added the following to ban sites.  I don't
     * endorse banning of sites, but Copper has few descriptors now
     * and some people from certain sites keep abusing access by
     * using automated 'autodialers' and leaving connections hanging.
     *
     * Furey: added suffix check by request of Nickel of HiddenWorlds.
     */
    for ( pban = ban_list; pban; pban = pban->next )
    {
	if ( !str_suffix( pban->name, dnew->host ) )
	{
	    write_to_descriptor( desc,
		"Your site has been banned from this Mud.\n\r", 0 );
#if !defined( WIN32 )
	    close( desc );
#else
	    closesocket( desc );
#endif
	    free_string( dnew->host );
	    free_mem( dnew->outbuf, dnew->outsize );
	    dnew->next		= descriptor_free;
	    descriptor_free	= dnew;
	    return;
	}
    }

    /*
     * Init descriptor data.
     */
    dnew->next			= descriptor_list;
    descriptor_list		= dnew;

    /*
     * Send the greeting.
     */
    {
        char buf [ MAX_STRING_LENGTH ];
        extern char * os_version;
        extern char * sys_ann;
        extern char * help_greeting;
        extern int players_count;
        extern int directs_count;
        extern int immortals_count;
        
        write_to_buffer( dnew, "[H[J[1;34mDoraemon's[m MUD -- Delanta v0.4Alpha\n\r[1;37m", 0 );
    	write_to_buffer( dnew, os_version, 0 );    
    	write_to_buffer( dnew, "[1;37mConnecting to the [1;33mMUDS[m......[m\n\r\n\r", 0 );

	if ( help_greeting[0] == '.' )
	    write_to_buffer( dnew, help_greeting+1, 0 );
	else
	    write_to_buffer( dnew, help_greeting  , 0 );
	
	write_to_buffer( dnew, "\n\r--------------------------------------------------------------------------------\n\r", 0 );
	sprintf( buf, "�ثe��[1;33m%3d[m�쪱�a�b�u�W, ��[1;32m%3d[m��O�j��, ��[1;31m%3d[m��O��߯�\n\r[1;33m�t�Τ��i: %s[m\n\r", players_count, directs_count, immortals_count, sys_ann );
	write_to_buffer( dnew, buf, 0 );
	write_to_buffer( dnew, "�п�J�A���W�r: ", 0 );
    }

    return;
}
#endif



void close_socket( DESCRIPTOR_DATA *dclose )
{
    CHAR_DATA *ch;

    if ( dclose->outtop > 0 )
	process_output( dclose, FALSE );

    if ( dclose->snoop_by )
    {
	write_to_buffer( dclose->snoop_by,
	    "Your victim has left the game.\n\r", 0 );
    }

    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d; d = d->next )
	{
	    if ( d->snoop_by == dclose )
		d->snoop_by = NULL;
	}
    }

    if ( ( ch = dclose->character ) )
    {
	sprintf( log_buf, "Closing link to %s.", ch->name );
	log_string( log_buf );
	if ( dclose->connected == CON_PLAYING )
	{
	    act( "$c���h�F$s���s�u�C", ch, NULL, NULL, TO_ROOM );
	    ch->desc = NULL;
	}
	else
	{
	    free_char( dclose->character );
	}
    }

    if ( d_next == dclose )
	d_next = d_next->next;   

    if ( dclose == descriptor_list )
    {
	descriptor_list = descriptor_list->next;
    }
    else
    {
	DESCRIPTOR_DATA *d;

	for ( d = descriptor_list; d && d->next != dclose; d = d->next )
	    ;
	if ( d )
	    d->next = dclose->next;
	else
	    bug( "Close_socket: dclose not found.", 0 );
    }

#if !defined( WIN32 )
    close( dclose->descriptor );
#else
    closesocket( dclose->descriptor );
#endif
    free_string( dclose->host );

    /* RT socket leak fix */
    free_mem( dclose->outbuf, dclose->outsize );

    dclose->next	= descriptor_free;
    descriptor_free	= dclose;
#if defined( macintosh )
    exit( 1 );
#endif
    return;
}



bool read_from_descriptor( DESCRIPTOR_DATA *d )
{
    int iStart;

    /* Hold horses if pending command already. */
    if ( d->incomm[0] != '\0' )
	return TRUE;

    /* Check for overflow. */
    iStart = strlen( d->inbuf );
    if ( iStart >= sizeof( d->inbuf ) - 10 )
    {
	sprintf( log_buf, "%s input overflow!", d->host );
	log_string( log_buf );
	write_to_descriptor( d->descriptor,
	    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
	return FALSE;
    }

    /* Snarf input. */
#if defined( macintosh )
    for ( ; ; )
    {
	int c;
	c = getc( stdin );
	if ( c == '\0' || c == EOF )
	    break;
	putc( c, stdout );
	if ( c == '\r' )
	    putc( '\n', stdout );
	d->inbuf[iStart++] = c;
	if ( iStart > sizeof( d->inbuf ) - 10 )
	    break;
    }
#endif

#if defined( unix ) || defined( AmigaTCP ) || defined( WIN32 )
    for ( ; ; )
    {
	int nRead;

#if !defined( WIN32 )
	nRead = read( d->descriptor, d->inbuf + iStart,
		     sizeof( d->inbuf ) - 10 - iStart );
#else
	nRead = recv( d->descriptor, d->inbuf + iStart,
		     sizeof( d->inbuf ) - 10 - iStart, 0 );
#endif
	if ( nRead > 0 )
	{
	    iStart += nRead;
	    if ( d->inbuf[iStart-1] == '\n' || d->inbuf[iStart-1] == '\r' )
		break;
	}
	else if ( nRead == 0 )
	{
	    log_string( "EOF encountered on read." );
	    return FALSE;
	}
#if !defined( AmigaTCP ) && !defined( WIN32 )
        else if ( errno == EWOULDBLOCK || errno == EAGAIN )
	    break;
#endif
#if defined( WIN32 )
        else if ( WSAGetLastError() == WSAEWOULDBLOCK || errno == EAGAIN )
	    break;
#endif
	else
	{
	    perror( "Read_from_descriptor" );
	    return FALSE;
	}
    }
#endif

    d->inbuf[iStart] = '\0';
    return TRUE;
}



/*
 * Transfer one line from input buffer to input line.
 */
void read_from_buffer( DESCRIPTOR_DATA *d )
{
    int i;
    int j;
    int k;

    /*
     * Hold horses if pending command already.
     */
    if ( d->incomm[0] != '\0' )
	return;

    /*
     * Look for at least one new line.
     */
    for ( i = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( d->inbuf[i] == '\0' )
	    return;
    }

    /*
     * Canonical input processing.
     */
    for ( i = 0, k = 0; d->inbuf[i] != '\n' && d->inbuf[i] != '\r'; i++ )
    {
	if ( k >= MAX_INPUT_LENGTH - 2 )
	{
	    write_to_descriptor( d->descriptor, "Line too long.\n\r", 0 );

	    /* skip the rest of the line */
	    for ( ; d->inbuf[i] != '\0'; i++ )
	    {
		if ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
		    break;
	    }
	    d->inbuf[i]   = '\n';
	    d->inbuf[i+1] = '\0';
	    break;
	}

	if ( d->inbuf[i] == '\b' && k > 0 )
	    --k;
	/*else if ( isascii( d->inbuf[i] ) && isprint( d->inbuf[i] ) )*/
	/* For chinese */
	else d->incomm[k++] = d->inbuf[i];
    }

    /*
     * Finish off the line.
     */
    if ( k == 0 )
	d->incomm[k++] = ' ';
    d->incomm[k] = '\0';

    /*
     * Deal with bozos with #repeat 1000 ...
     */
    if ( k > 1 || d->incomm[0] == '!' )
    {
    	if ( d->incomm[0] != '!' && strcmp( d->incomm, d->inlast ) )
	{
	    d->repeat = 0;
	}
	else
	{
	    if ( ++d->repeat >= 20 )
	    {
		sprintf( log_buf, "%s input spamming!", d->host );
		log_string( log_buf );
		write_to_descriptor( d->descriptor,
		    "\n\r*** PUT A LID ON IT!!! ***\n\r", 0 );
		strcpy( d->incomm, "quit" );
	    }
	}
    }

    /*
     * Do '!' substitution.
     */
    if ( d->incomm[0] == '!' )
	strcpy( d->incomm, d->inlast );
    else
	strcpy( d->inlast, d->incomm );

    /*
     * Shift the input buffer.
     */
    while ( d->inbuf[i] == '\n' || d->inbuf[i] == '\r' )
	i++;
    for ( j = 0; ( d->inbuf[j] = d->inbuf[i+j] ) != '\0'; j++ )
	;
    return;
}



/*
 * Low level output function.
 */
bool process_output( DESCRIPTOR_DATA *d, bool fPrompt )
{
    extern bool merc_down;

    /*
     * Bust a prompt.
     */
    if ( fPrompt && !merc_down && d->connected == CON_PLAYING )
    {
        if ( d->showstr_point )
	{
	    write_to_buffer( d,
  "[Please type (c)ontinue, (r)efresh, (b)ack, (q)uit, or RETURN]:  ", 0 );
	}
	else if ( d->pString )
		write_to_buffer( d, "> ", 2 );
	else
	{
	    CHAR_DATA *ch;

	    ch = d->original ? d->original : d->character;
	    if ( IS_SET( ch->act, PLR_BLANK     ) )
	        write_to_buffer( d, "\n\r", 2 );

	    if ( IS_SET( ch->act, PLR_PROMPT    ) )
	        bust_a_prompt( d );

	    if ( IS_SET( ch->act, PLR_TELNET_GA ) )
	        write_to_buffer( d, go_ahead_str, 0 );
	}
    }

    /*
     * Short-circuit if nothing to write.
     */
    if ( d->outtop == 0 )
	return TRUE;

    /*
     * Snoop-o-rama.
     */
    if ( d->snoop_by )
    {
	write_to_buffer( d->snoop_by, "% ", 2 );
	write_to_buffer( d->snoop_by, d->outbuf, d->outtop );
    }

    /*
     * OS-dependent output.
     */
    if ( !write_to_descriptor( d->descriptor, d->outbuf, d->outtop ) )
    {
	d->outtop = 0;
	return FALSE;
    }
    else
    {
	d->outtop = 0;
	return TRUE;
    }
}

/*
 * Bust a prompt (player settable prompt)
 * coded by Morgenes for Aldara Mud
 */
void bust_a_prompt( DESCRIPTOR_DATA *d )
{
         CHAR_DATA *ch;
   const char      *str;
   const char      *i;
         char      *point;
         char       buf  [ MAX_STRING_LENGTH ];
         char       buf2 [ MAX_STRING_LENGTH ];

   /* Will always have a pc ch after this */
   ch = ( d->original ? d->original : d->character );
   if( !ch->pcdata->prompt || ch->pcdata->prompt[0] == '\0' )
   {
      send_to_char( "\n\r\n\r", ch );
      return;
   }

   point = buf;
   str = ch->pcdata->prompt;

   while( *str != '\0' )
   {
      if( *str != '%' )
      {
         *point++ = *str++;
         continue;
      }
      ++str;
      switch( *str )
      {
         default :
            i = " "; break;
         case 'h' :
            sprintf( buf2, "%d", ch->hit                               );
            i = buf2; break;
         case 'H' :
            sprintf( buf2, "%d", ch->max_hit                           );
            i = buf2; break;
         case 'm' :
            sprintf( buf2, "%d", ch->mana                              );
            i = buf2; break;
         case 'M' :
            sprintf( buf2, "%d", ch->max_mana                          );
            i = buf2; break;
         case 'v' :
            sprintf( buf2, "%d", ch->move                              ); 
            i = buf2; break;
         case 'V' :
            sprintf( buf2, "%d", ch->max_move                          );
            i = buf2; break;
         case 'x' :
            sprintf( buf2, "%d", ch->exp                               );
            i = buf2; break;
         case 'g' :
            sprintf( buf2, "%d", ch->gold                              );
            i = buf2; break;
         case 'w' :
	    sprintf( buf2, "%d", ch->wait                              );
	    i = buf2; break;
         case 'a' :
            if( ch->level > 10 )
               sprintf( buf2, "%d", ch->alignment                      );
            else
               sprintf( buf2, "%s", IS_GOOD( ch ) ? "good"
		                  : IS_EVIL( ch ) ? "evil" : "neutral" );
            i = buf2; break;
         case 'r' :
            if( ch->in_room )
               sprintf( buf2, "%s", ch->in_room->name                  );
            else
               sprintf( buf2, " "                                      );
            i = buf2; break;
         case 'R' :
            if( IS_IMMORTAL( ch ) && ch->in_room )
               sprintf( buf2, "%d", ch->in_room->vnum                  );
            else
               sprintf( buf2, " "                                      );
            i = buf2; break;
         case 'z' :
            if( IS_IMMORTAL( ch ) && ch->in_room )
               sprintf( buf2, "%s", ch->in_room->area->name            );
            else
               sprintf( buf2, " "                                      );
            i = buf2; break;
         case 'i' :  /* Invisible notification on prompt sent by Kaneda */
	    sprintf( buf2, "%s", IS_AFFECTED( ch, AFF_INVISIBLE ) ?
		                 "invisible" : "visible" );
	    i = buf2; break;
         case 'I' :
            if( IS_IMMORTAL( ch ) )
               sprintf( buf2, "(wizinv: %s)", IS_SET( ch->act, PLR_WIZINVIS ) ?
		                              "on" : "off" );
            else
               sprintf( buf2, " "                                      );
            i = buf2; break;
         case 'A' :
            if( !IS_NPC( ch ) )
                sprintf( buf2, "%d", ch->pcdata->angry_point );
            i = buf2; break;
         case '%' :
            sprintf( buf2, "%%"                                        );
            i = buf2; break;
         case 'c' :		/* OLC */
	    i = olc_ed_name( ch );
	    break;
         case 'C' :		/* OLC */
	    i = olc_ed_vnum( ch );
	    break;         
      } 
      ++str;
      while( ( *point = *i ) != '\0' )
         ++point, ++i;      
   }
   write_to_buffer( d, buf, point - buf );
   return;
}

/*
 * Append onto an output buffer.
 */
void write_to_buffer( DESCRIPTOR_DATA *d, const char *txt, int length )
{
    /*
     * Find length in case caller didn't.
     */
    if ( length <= 0 )
	length = strlen( txt );
	
	
    /*
     * Initial \n\r if needed.
     */
    if ( d->outtop == 0 && !d->fcommand )
    {
	d->outbuf[0]	= '\n';
	d->outbuf[1]	= '\r';
	d->outtop	= 2;
    }

    /*
     * Expand the buffer as needed.
     */
    while ( d->outtop + length >= d->outsize )
    {
	char *outbuf;

	outbuf      = alloc_mem( 2 * d->outsize );
	strncpy( outbuf, d->outbuf, d->outtop );
	free_mem( d->outbuf, d->outsize );
	d->outbuf   = outbuf;
	d->outsize *= 2;
    }

    /*
     * Copy.  Modifications sent in by Zavod.
     */
    strncpy( d->outbuf + d->outtop, txt, length );
    d->outtop += length;
    return;
}



/*
 * Lowest level output function.
 * Write a block of text to the file descriptor.
 * If this gives errors on very long blocks (like 'ofind all'),
 *   try lowering the max block size.
 */
bool write_to_descriptor( int desc, char *txt, int length )
{
    int iStart;
    int nWrite;
    int nBlock;

#if defined( macintosh )
    if ( desc == 0 )
	desc = 1;
#endif

    if ( length <= 0 )
	length = strlen( txt );

    for ( iStart = 0; iStart < length; iStart += nWrite )
    {
	nBlock = UMIN( length - iStart, 2048 );
#if !defined( WIN32 )
	if ( ( nWrite = write( desc, txt + iStart, nBlock ) ) < 0 )
#else
	if ( ( nWrite = send( desc, txt + iStart, nBlock , 0) ) < 0 )
#endif
	    { perror( "Write_to_descriptor" ); return FALSE; }
    } 

    return TRUE;
}



/*
 * Deal with sockets that haven't logged in yet.
 */
void nanny( DESCRIPTOR_DATA *d, char *argument )
{
    CHAR_DATA *ch;
    NOTE_DATA *pnote;
    char      *pwdnew;
    char      *p;
    char       buf [ MAX_STRING_LENGTH ];
    int        lines;
    int        notes;
    bool       fOld;

    while ( isspace( *argument ) 
           || !isascii(*argument) 
           || !isprint(*argument) )
	argument++;

    /* This is here so we wont get warnings.  ch = NULL anyways - Kahn */
    ch          = d->character;

    switch ( d->connected )
    {

    default:
	bug( "Nanny: bad d->connected %d.", d->connected );
	close_socket( d );
	return;

    case CON_GET_NAME:
	if ( argument[0] == '\0' )
	{
	    close_socket( d );
	    return;
	}

	argument[0] = UPPER( argument[0] );

	if ( !check_parse_name( argument ) )
	{
	    write_to_buffer( d, "���X�k���W�r�A���@�ӸոաC\n\r�п�J�A���W�r: ", 0 );
	    return;
	}

	fOld = load_char_obj( d, argument );
	ch   = d->character;

	if ( IS_SET( ch->act, PLR_DENY ) )
	{
	    sprintf( log_buf, "Denying access to %s@%s.", argument, d->host );
	    log_string( log_buf );
	    write_to_buffer( d, "[1;33m�A�Q�T��s�u�i�J��MUD�C[m\n\r", 0 );
	    close_socket( d );
	    return;
	}

	if ( check_reconnect( d, argument, FALSE ) )
	{
	    fOld = TRUE;
	}
	else
	{
	    /* Must be immortal with wizbit in order to get in */
	    if ( wizlock
		&& !IS_HERO( ch )
		&& !IS_SET( ch->act, PLR_WIZBIT ) )
	    {
		write_to_buffer( d, "[1;31m�{�b�o��MUD�u��[1;33m�j��[1;31m�~�i�H�i�J�C[m\n\r", 0 );
		close_socket( d );
		return;
	    }
	    if ( ch->level <= numlock
		&& !IS_SET( ch->act, PLR_WIZBIT )
		&& numlock != 0 )
	    {
		write_to_buffer( d,
			"�o�ӹC���{�b���}�񵹱z�o�ӵ��Ū����a�ϥΡC\n\r\n\r",
				0 );
		if ( ch->level == 0 )
		{
		    write_to_buffer( d,
			"�s�����a�{�b�O�H Email �{�Ҥ覡�ӵ��U�@�Ө���C\n\r\n\r",
				    0 );
		    write_to_buffer( d,
			"�еo�@�� email �� mudadm@mud.ck.tp.edu.tw �ӵ��U�A������C\n\r", 0 );
		    write_to_buffer( d,
			"�@�� email ��}�@�Ө���, �ж�J�z���ӤH��Ƥ@�ֱH��, �j���N�|���z�B�z�C\n\r\n\r", 0 );
		}
		close_socket( d ) ;
		return;
	    }
	}

	if ( check_playing( d, ch->name ) )
	    return;

	if ( fOld )
	{
	    /* Old player */
	    if ( !IS_NPC( d->character ) )
	    {
	        sprintf( buf, "�A�W���O�q %s �s�u�i�Ӫ��C\n\r", d->character->pcdata->lastFrom	         );
	        write_to_buffer( d, buf, 0 );
	    }
	    write_to_buffer( d, "[m�п�J�z���K�X: ", 0 );
	    write_to_buffer( d, echo_off_str, 0 );
	    d->connected = CON_GET_OLD_PASSWORD;
	    return;
	}
	else
	{
	    /* New player */
	    sprintf( buf, "��..�ڨS�ݿ��a, �A���W�r�O[1;34m %s [m(Y/N)? ", argument );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_CONFIRM_NEW_NAME;
	    return;
	}
	break;

    case CON_GET_OLD_PASSWORD:
#if defined( unix ) || defined( AmigaTCP )
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "[1;31m�K�X���~�C[m\n\r", 0 );
	    write_to_buffer( d, "[0;32m�p��������D, �w��ӫH mudadm@mud.ck.tp.edu.tw �C[m\n\r", 0 );
	    close_socket( d );
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );

	if ( check_reconnect( d, ch->name, TRUE ) )
	    return;

	sprintf( log_buf, "%s@%s has connected with character %s.", d->remote_user, d->host, ch->name );
	
	log_string( log_buf );
	lines = ch->pcdata->pagelen;
	ch->pcdata->pagelen = 20;
	if ( IS_HERO( ch ) )
	    do_help( ch, "imotd" );
	do_help( ch, "motd" );
	ch->pcdata->pagelen = lines;
	d->connected = CON_READ_MOTD;
	break;

    case CON_CONFIRM_NEW_NAME:
	switch ( *argument )
	{
	case 'y': case 'Y':
	    sprintf( buf, "��..�ڨS���L�A, �O[0;33m�s�����a[m��!!\n\r�е���[0;32m %s [m���K�X:%s",
		    ch->name, echo_off_str );
	    write_to_buffer( d, buf, 0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    break;

	case 'n': case 'N':
	    write_to_buffer( d, "�n�a, ���M�O����O? ", 0 );
	    free_char( d->character );
	    d->character = NULL;
	    d->connected = CON_GET_NAME;
	    break;

	default:
	    write_to_buffer( d, "�п�J[1;34m Yes [m��[1;35m No [m? ", 0 );
	    break;
	}
	break;

    case CON_GET_NEW_PASSWORD:
#if defined( unix ) || defined( AmigaTCP )
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strlen( argument ) < 5 )
	{
	    write_to_buffer( d,
	       "�K�X�ܤ֭n�����Ӧr���C�·бz�A��J�@��\n\r�z���K�X: ",
		0 );
	    return;
	}

	pwdnew = crypt( argument, ch->name );
	for ( p = pwdnew; *p != '\0'; p++ )
	{
	    if ( *p == '~' )
	    {
		write_to_buffer( d,
		    "�������A�s���K�X, �A�ոը�L���ݬݡC\n\r�z���K�X: ",
		    0 );
		return;
	    }
	}

	free_string( ch->pcdata->pwd );
	ch->pcdata->pwd	= str_dup( pwdnew );
	write_to_buffer( d, "�ЦA��J�@���K�X: ", 0 );
	d->connected = CON_CONFIRM_NEW_PASSWORD;
	break;

    case CON_CONFIRM_NEW_PASSWORD:
#if defined( unix ) || defined( AmigaTCP )
	write_to_buffer( d, "\n\r", 2 );
#endif

	if ( strcmp( crypt( argument, ch->pcdata->pwd ), ch->pcdata->pwd ) )
	{
	    write_to_buffer( d, "�x..�⦸�K�X���@�ˡC\n\r�ЦA�����@���K�X: ",
		0 );
	    d->connected = CON_GET_NEW_PASSWORD;
	    return;
	}

	write_to_buffer( d, echo_on_str, 0 );
	write_to_buffer( d, "\n\r�����U [1;33mENTER[m ���~��:\n\r", 0 );
	write_to_buffer( d, "\n\r�аݧA���ʧO�O( [1;37mM[male(��)/[1;37mF[memale(��)/[1;37mN[meutral(��) )? ", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;

/*    case CON_DISPLAY_RACE:
	strcpy( buf, "��ܺر� [" );
	for ( iRace = 0; iRace < MAX_RACE; iRace++ )
	{
	    if ( !IS_SET( race_table[ iRace ].race_abilities, RACE_PC_AVAIL ) )
	        continue;
	    if ( iRace > 0 )
	        strcat( buf, " " );
	    strcat( buf, race_table[iRace].name );
	}
	strcat( buf, "]:  " );
	write_to_buffer( d, buf, 0 );
	d->connected = CON_GET_NEW_RACE;
	break;

    case CON_GET_NEW_RACE:
        log_string( "Get_NEW_RACE" );
	for ( iRace = 0; iRace < MAX_RACE; iRace++ )
	    if ( !str_prefix( argument, race_table[iRace].name )
		&& IS_SET( race_table[ iRace ].race_abilities,
			  RACE_PC_AVAIL ) )
	    {
		ch->race = race_lookup( race_table[iRace].name );
		break;
	    }

	if ( iRace == MAX_RACE )
	{
	    write_to_buffer( d,
			    "�����O�@�ӺرڡC\n\r�A�Q�n����ر�? ", 0 );
	    return;
	}

	write_to_buffer( d, "\n\r", 0 );
	do_help( ch, race_table[ch->race].name );
	write_to_buffer( d, "�A�T�w�A�n�o�Ӻرڶ�? ", 0 );
	d->connected = CON_CONFIRM_NEW_RACE;
	break;

    case CON_CONFIRM_NEW_RACE:
        log_string( "Confirm_NEW_RACE" );
	switch ( argument[0] )
	{
	  case 'y': case 'Y': break;
	  default:
	      write_to_buffer( d, "\n\r�����U [1;34mENTER[m �~��:\n\r", 0 );
	      d->connected = CON_DISPLAY_RACE;
	      return;
	}

	write_to_buffer( d, "\n\r�аݧA���ʧO�O( [1;37mM[male(��)/[1;37mF[memale(��)/[1;37mN[meutral(��) )? ", 0 );
	d->connected = CON_GET_NEW_SEX;
	break;*/

    case CON_GET_NEW_SEX:
	switch ( argument[0] )
	{
	case 'm': case 'M': ch->sex = SEX_MALE;    break;
	case 'f': case 'F': ch->sex = SEX_FEMALE;  break;
	case 'n': case 'N': ch->sex = SEX_NEUTRAL; break;
	default:
	    write_to_buffer( d, "�����O�@�өʧO�C\n\r�аݧA���ʧO? ", 0 );
	    return;
	}

	d->connected = CON_READ_MOTD;
	write_to_buffer( d, "\n\r�����U [1;36mENTER[m �~��:\n\r", 0 );
	break;
	

    case CON_READ_MOTD:
	ch->next	= char_list;
	char_list	= ch;
	d->connected	= CON_PLAYING;

	send_to_char(
    "[H[J\n\r�A���Mı�o[1;37m�Y��[m�{�F�@�U�C�b�@������, �A���̪�[0;34m��ı�ϸ��[m�w�Q��s�C\n\r",
	    ch );
	send_to_char(
    "�M�J�A���e��, �O�@��[1;31m���i��ĳ���@��[m�C�ڷQ�B�Ʊ�R���b�o�̡C\n\r", ch );
        send_to_char(
    "�o�O��[1;33m�s�@�N���ѯ�[m, �ҳгy�X�Ӫ��@�ɡ�[1;32m���U���йF����[m\n\r", ch );
        send_to_char(
    "-----------------------------------------------------------------------------\n\r\n\r\n\r", ch );

	if ( ch->level == 0 )
	{
	    OBJ_DATA *obj;
	    
	    ch->class = CLASS_NONE;
	    switch ( class_table[ch->class].attr_prime )
	    {
	    case APPLY_STR: ch->pcdata->perm_str = 18; break;
	    case APPLY_INT: ch->pcdata->perm_int = 18; break;
	    case APPLY_WIS: ch->pcdata->perm_wis = 18; break;
	    case APPLY_DEX: ch->pcdata->perm_dex = 18; break;
	    case APPLY_CON: ch->pcdata->perm_con = 18; break;
	    }

	    ch->level	= 1;
	    ch->class	= CLASS_NONE;
	    ch->cname	= str_dup( ch->name );
	    ch->exp	= 1000;
	    ch->gold    = 5500 + number_fuzzy( 3 )
	                * number_fuzzy( 4 ) * number_fuzzy( 5 ) * 9;
	    ch->hit	= ch->max_hit;
	    ch->mana	= ch->max_mana;
	    ch->move	= ch->max_move;
	    ch->pcdata->security = 0;
	    free_string( ch->pcdata->prompt );
	    ch->pcdata->prompt = str_dup( "<��[0;35m%h[m��[1;34m%m[m��[0;32m%v[m��[0;33m%A[m> " );
	    ch->pcdata->angry_point = 30;

	    obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_BANNER ), 0 );
	    obj_to_char( obj, ch );
	    equip_char( ch, obj, WEAR_LIGHT );

	    obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_VEST   ), 0 );
	    obj_to_char( obj, ch );
	    equip_char( ch, obj, WEAR_BODY );

	    obj = create_object( get_obj_index( OBJ_VNUM_SCHOOL_SHIELD ), 0 );
	    obj_to_char( obj, ch );
	    equip_char( ch, obj, WEAR_SHIELD );

	    obj = create_object( 
				get_obj_index( OBJ_VNUM_SCHOOL_SWORD ),
				0 );
	    obj_to_char( obj, ch );
	    equip_char( ch, obj, WEAR_WIELD );

	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );	    
	}
	else if ( ch->in_room )
	{
	    char_to_room( ch, ch->in_room );
	}
	else if ( IS_IMMORTAL( ch ) )
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_CHAT ) );
	}
	else
	{
	    char_to_room( ch, get_room_index( ROOM_VNUM_TEMPLE ) );
	}
	
	if ( players_count < 0 )
	    players_count = 0;
	if ( directs_count < 0 )
	    directs_count = 0;
	if ( immortals_count < 0 )
	    immortals_count = 0;
	    
	players_count++;
	if ( IS_IMMORTAL( ch ) && !IS_DIRECT( ch ) )
	    immortals_count++;
	if ( IS_DIRECT( ch ) )
	    directs_count++;

        /* Sort the descriptors */
        sort_descriptor( (int)_sort_descriptor_by_level );
        
	if ( !IS_SET( ch->act, PLR_WIZINVIS )
	    && !IS_AFFECTED( ch, AFF_INVISIBLE ) )
	    act( "[1;35m$c[1;35m�i�J�F�o�ӻ��C[m", ch, NULL, NULL, TO_ROOM );

	do_look( ch, "auto" );
	/* check for new notes */
	notes = 0;

	for ( pnote = note_list; pnote; pnote = pnote->next )
	    if ( is_note_to( ch, pnote ) && str_cmp( ch->name, pnote->sender )
		&& pnote->date_stamp > ch->last_note )
	        notes++;

	if ( notes == 1 )
	    send_to_char( "\n\r[1;32m�A���@�ʷs�쪺�l��C[m\n\r", ch );
	else
	    if ( notes > 1 )
	    {
		sprintf( buf, "\n\r[1;32m�A��%3d�ʷs�쪺�l��C[m\n\r",
			notes );
		send_to_char( buf, ch );
	    }

	break;
    }

    return;
}



/*
 * Parse a name for acceptability.
 */
bool check_parse_name( char *name )
{
    /*
     * Reserved words.
     */
    if ( is_name( name, "all auto imm immortal self someone none . .. ./ ../ /" ) )
	return FALSE;

    /*
     * Obsenities
     */
    if ( is_name( name, "damn fuck screw shit ass asshole bitch bastard gay lesbian pussy fart vagina penis" ) )
        return FALSE;

    /*
     * Length restrictions.
     */
    if ( strlen( name ) <  3 )
	return FALSE;

#if defined( macintosh ) || defined( unix ) || defined( AmigaTCP )
    if ( strlen( name ) > 12 )
	return FALSE;
#endif

    /*
     * Alphanumerics only.
     * Lock out IllIll twits.
     */
    {
	char *pc;
	bool fIll;

	fIll = TRUE;
	for ( pc = name; *pc != '\0'; pc++ )
	{
	    if ( !isalpha( *pc ) )
		return FALSE;
	    if ( LOWER( *pc ) != 'i' && LOWER( *pc ) != 'l' )
		fIll = FALSE;
	}

	if ( fIll )
	    return FALSE;
    }

    /*
     * Prevent players from naming themselves after mobs.
     */
    {
	extern MOB_INDEX_DATA *mob_index_hash [ MAX_KEY_HASH ];
	       MOB_INDEX_DATA *pMobIndex;
	       int             iHash;

	for ( iHash = 0; iHash < MAX_KEY_HASH; iHash++ )
	{
	    for ( pMobIndex  = mob_index_hash[iHash];
		  pMobIndex;
		  pMobIndex  = pMobIndex->next )
	    {
		if ( is_name( name, pMobIndex->player_name ) )
		    return FALSE;
	    }
	}
    }

    return TRUE;
}



/*
 * Look for link-dead player to reconnect.
 */
bool check_reconnect( DESCRIPTOR_DATA *d, char *name, bool fConn )
{
    CHAR_DATA *ch;

    for ( ch = char_list; ch; ch = ch->next )
    {
        if ( ch->deleted )
	    continue;

	if ( !IS_NPC( ch )
	    && ( !fConn || !ch->desc )
	    && !str_cmp( d->character->name, ch->name ) )
	{
	    if ( fConn == FALSE )
	    {
		free_string( d->character->pcdata->pwd );
		d->character->pcdata->pwd = str_dup( ch->pcdata->pwd );
	    }
	    else
	    {
		free_char( d->character );
		d->character = ch;
		ch->desc     = d;
		ch->timer    = 0;
		send_to_char( "Reconnecting.\n\r", ch );
		act( "$n has reconnected.", ch, NULL, NULL, TO_ROOM );
		sprintf( log_buf, "%s@%s reconnected.", ch->name, d->host );
		log_string( log_buf );
		d->connected = CON_PLAYING;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



/*
 * Check if already playing.
 */
bool check_playing( DESCRIPTOR_DATA *d, char *name )
{
    DESCRIPTOR_DATA *dold;

    for ( dold = descriptor_list; dold; dold = dold->next )
    {
	if ( dold != d
	    && dold->character
	    && dold->connected != CON_GET_NAME
	    && !str_cmp( name, dold->original
			? dold->original->name : dold->character->name )
	    && !dold->character->deleted )
	{
	    write_to_buffer( d, "�w�g�b�C�����F�C\n\r�ЦA��J�z���W�r: ", 0 );
	    d->connected = CON_GET_NAME;
	    if ( d->character )
	    {
		free_char( d->character );
		d->character = NULL;
	    }
	    return TRUE;
	}
    }

    return FALSE;
}



void stop_idling( CHAR_DATA *ch )
{
    if (   !ch
	|| !ch->desc
	||  ch->desc->connected != CON_PLAYING
	|| !ch->was_in_room
	||  ch->in_room != get_room_index( ROOM_VNUM_LIMBO ) )
	return;

    ch->timer = 0;
    char_from_room( ch );
    char_to_room( ch, ch->was_in_room );
    ch->was_in_room	= NULL;
    act( "$n has returned from the void.", ch, NULL, NULL, TO_ROOM );
    return;
}

/*
 * Write to all in the room.
 */
void send_to_room( const char *txt, ROOM_INDEX_DATA *room )
{
    DESCRIPTOR_DATA *d;
    
    for ( d = descriptor_list; d; d = d->next )
        if ( d->character != NULL )
	    if ( d->character->in_room == room )
	        act( txt, d->character, NULL, NULL, TO_CHAR );
}

/*
 * Write to all characters.
 */
void send_to_all_char( const char *text )
{
    DESCRIPTOR_DATA *d;

    if ( !text )
        return;
    for ( d = descriptor_list; d; d = d->next )
        if ( d->connected == CON_PLAYING )
	    send_to_char( text, d->character );

    return;
}

/*
 * Write to one char.
 */
void send_to_char( const char *txt, CHAR_DATA *ch )
{
    if ( !txt || !ch->desc )
        return;

    /*
     * Bypass the paging procedure if the text output is small
     * Saves process time.
     */
    if ( strlen( txt ) < 600 )
	write_to_buffer( ch->desc, txt, strlen( txt ) );
    else
    {
        free_string( ch->desc->showstr_head );
	ch->desc->showstr_head  = str_dup( txt );
	ch->desc->showstr_point = ch->desc->showstr_head;
	show_string( ch->desc, "" );
    }

    return;
}

 /* The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom
    for porting this SillyMud code for MERC 2.0 and laying down the groundwork.
    Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which
    the improvements to the pager was modeled from.  - Kahn */
 /* 12/1/94 Fixed bounds and overflow bugs in pager thanks to BoneCrusher
    of EnvyMud Staff - Kahn */

void show_string( struct descriptor_data *d, char *input )
{
    register char *scan;
             char  buffer[ MAX_STRING_LENGTH*6 ];
             char  buf   [ MAX_INPUT_LENGTH    ];
             int   line      = 0;
             int   toggle    = 0;
             int   pagelines = 20;

    one_argument( input, buf );

    switch( UPPER( buf[0] ) )
    {
    case '\0':
    case 'C': /* show next page of text */
	break;

    case 'R': /* refresh current page of text */
	toggle = 1;
	break;

    case 'B': /* scroll back a page of text */
	toggle = 2;
	break;

    default: /*otherwise, stop the text viewing */
	if ( d->showstr_head )
	{
	    free_string( d->showstr_head );
	    d->showstr_head = str_dup( "" );
	}
	d->showstr_point = 0;
	return;

    }

    if ( d->original )
        pagelines = d->original->pcdata->pagelen;
    else
        pagelines = d->character->pcdata->pagelen;

    if ( toggle )
    {
	if ( d->showstr_point == d->showstr_head )
	    return;
	if ( toggle == 1 )
	    line = -1;
	do
	{
	    if ( *d->showstr_point == '\n' )
	      if ( ( line++ ) == ( pagelines * toggle ) )
		break;
	    d->showstr_point--;
	} while( d->showstr_point != d->showstr_head );
    }
    
    line    = 0;
    *buffer = 0;
    scan    = buffer;
    if ( *d->showstr_point )
    {
	do
	{
	    *scan = *d->showstr_point;
	    if ( *scan == '\n' )
	      if ( ( line++ ) == pagelines )
		{
		  scan++;
		  break;
		}
	    scan++;
	    d->showstr_point++;
	    if( *d->showstr_point == 0 )
	      break;
	} while( 1 );
    }

    /* On advice by Scott Mobley and others */
    *scan++ = '\n';
    *scan++ = '\r';

    *scan = 0;

    write_to_buffer( d, buffer, strlen( buffer ) );
    if ( *d->showstr_point == 0 )
    {
      free_string( d->showstr_head );
      d->showstr_head  = str_dup( "" );
      d->showstr_point = 0;
    }

    return;
}

/* OLC, new pager for editing long descriptions. */
/* ========================================================================= */
/* - The heart of the pager.  Thanks to N'Atas-Ha, ThePrincedom for porting  */
/*   this SillyMud code for MERC 2.0 and laying down the groundwork.         */
/* - Thanks to Blackstar, hopper.cs.uiowa.edu 4000 for which the improvements*/
/*   to the pager was modeled from.  - Kahn                                  */
/* - Safer, allows very large pagelen now, and allows to page while switched */
/*   Zavod of jcowan.reslife.okstate.edu 4000.                               */
/* ========================================================================= */

void new_show_string( DESCRIPTOR_DATA *d, char *input )
{
    char               *start, *end;
    char                arg[MAX_INPUT_LENGTH];
    int                 lines = 0, pagelen;

    /* Set the page length */
    /* ------------------- */

    pagelen = d->original ? d->original->pcdata->pagelen
                          : d->character->pcdata->pagelen;

    /* Check for the command entered */
    /* ----------------------------- */

    one_argument( input, arg );

    switch( UPPER( *arg ) )
    {
        /* Show the next page */

        case '\0':
        case 'C': lines = 0;
                  break;
        
        /* Scroll back a page */

        case 'B': lines = -2 * pagelen;
                  break;

        /* Help for show page */

        case 'H': write_to_buffer( d, "B     - Scroll back one page.\n\r", 0 );
                  write_to_buffer( d, "C     - Continue scrolling.\n\r", 0 );
                  write_to_buffer( d, "H     - This help menu.\n\r", 0 );
                  write_to_buffer( d, "R     - Refresh the current page.\n\r",
                                   0 );
                  write_to_buffer( d, "Enter - Continue Scrolling.\n\r", 0 );
                  return;

        /* refresh the current page */

        case 'R': lines = -1 - pagelen;
                  break;

        /* stop viewing */

        default:  free_string( d->showstr_head );
                  d->showstr_head  = NULL;
                  d->showstr_point = NULL;
                  return;
    }

    /* do any backing up necessary to find the starting point */
    /* ------------------------------------------------------ */

    if ( lines < 0 )
    {
        for( start= d->showstr_point; start > d->showstr_head && lines < 0;
             start-- )
            if ( *start == '\r' )
                lines++;
    }
    else
        start = d->showstr_point;

    /* Find the ending point based on the page length */
    /* ---------------------------------------------- */

    lines  = 0;

    for ( end= start; *end && lines < pagelen; end++ )
        if ( *end == '\r' )
            lines++;

    d->showstr_point = end;

    if ( end - start )
        write_to_buffer( d, start, end - start );

    /* See if this is the end (or near the end) of the string */
    /* ------------------------------------------------------ */

    for ( ; isspace( *end ); end++ );

    if ( !*end )
    {
        free_string( d->showstr_head );
        d->showstr_head  = NULL;
        d->showstr_point = NULL;
    }

    return;
}

/*
 * The primary output interface for formatted output.
 */
void act( const char *format, CHAR_DATA *ch, const void *arg1,
	 const void *arg2, int type )
{
           OBJ_DATA        *obj1        = (OBJ_DATA  *) arg1;
	   OBJ_DATA        *obj2        = (OBJ_DATA  *) arg2;
	   CHAR_DATA       *to;
	   CHAR_DATA       *vch         = (CHAR_DATA *) arg2;
    static char *    const  he_she  [ ] = { "��",  "�L",  "�o" };
    static char *    const  him_her [ ] = { "��",  "�L", "�o" };
    static char *    const  his_her [ ] = { "����", "�L��", "�o��" };
    const  char            *str;
    const  char            *i;
           char            *point;
           char             buf     [ MAX_STRING_LENGTH ];
           char             buf1    [ MAX_STRING_LENGTH ];
           char             fname   [ MAX_INPUT_LENGTH  ];

    /*
     * Discard null and zero-length messages.
     */
    if ( !format || format[0] == '\0' )
	return;

    to = ch->in_room->people;
    if ( type == TO_VICT )
    {
	if ( !vch )
	{
	    bug( "Act: null vch with TO_VICT.", 0 );
	    sprintf( buf1, "Bad act string:  %s", format );
	    bug( buf1, 0 );
	    return;
	}
	to = vch->in_room->people;
    }
    
    for ( ; to; to = to->next_in_room )
    {
	if ( ( to->deleted )
	    || ( !to->desc && IS_NPC( to ) )
	    || !IS_AWAKE( to ) )
	    continue;

	if ( type == TO_CHAR    && to != ch )
	    continue;
	if ( type == TO_VICT    && ( to != vch || to == ch ) )
	    continue;
	if ( type == TO_ROOM    && to == ch )
	    continue;
	if ( type == TO_NOTVICT && (to == ch || to == vch) )
	    continue;

	point	= buf;
	str	= format;
	while ( *str != '\0' )
	{
	    if ( *str != '$' )
	    {
		*point++ = *str++;
		continue;
	    }
	    ++str;

	    if ( !arg2 && *str >= 'A' && *str <= 'Z' )
	    {
		bug( "Act: missing arg2 for code %d.", *str );
		sprintf( buf1, "Bad act string:  %s", format );
		bug( buf1, 0 );
		i = " <@@@> ";
	    }
	    else
	    {
		switch ( *str )
		{
		default:  bug( "Act: bad code %d.", *str );
		          sprintf( buf1, "Bad act string:  %s", format );
		          bug( buf1, 0 );
			  i = " <@@@> ";				break;
		/* Thx alex for 't' idea */
		case 't': i = (char *) arg1;				break;
		case 'T': i = (char *) arg2;          			break;
		case 'n': i = PERS( ch,  to  );				break;
		case 'N': i = PERS( vch, to  );				break;
		case 'c': i = CPERS( ch, to  );				break;
		case 'C': i = CPERS( vch, to );				break;
		case 'e': i = he_she  [URANGE( 0, ch  ->sex, 2 )];	break;
		case 'E': i = he_she  [URANGE( 0, vch ->sex, 2 )];	break;
		case 'm': i = him_her [URANGE( 0, ch  ->sex, 2 )];	break;
		case 'M': i = him_her [URANGE( 0, vch ->sex, 2 )];	break;
		case 's': i = his_her [URANGE( 0, ch  ->sex, 2 )];	break;
		case 'S': i = his_her [URANGE( 0, vch ->sex, 2 )];	break;

		case 'p':
		    i = can_see_obj( to, obj1 )
			    ? obj1->short_descr
			    : "�������~";
		    break;

		case 'P':
		    i = can_see_obj( to, obj2 )
			    ? obj2->short_descr
			    : "�������~";
		    break;

		case 'h':
		    i = can_see_obj( to, obj1 )
			    ? obj1->cname
			    : "�������~";
		    break;

		case 'H':
		    i = can_see_obj( to, obj2 )
			    ? obj2->cname
			    : "�������~";
		    break;		    

		case 'd':
		    if ( !arg2 || ( (char *) arg2 )[0] == '\0' )
		    {
			i = "door";
		    }
		    else
		    {
			one_argument( (char *) arg2, fname );
			i = fname;
		    }
		    break;
		}
	    }
		
	    ++str;
	    while ( ( *point = *i ) != '\0' )
		++point, ++i;
	}

	*point++ = '\n';
	*point++ = '\r';
	buf[0]   = UPPER( buf[0] );
	if ( to->desc )
	    write_to_buffer( to->desc, buf, point - buf );
	if ( MOBtrigger )
	    mprog_act_trigger( buf, to, ch, obj1, vch );
    }

    MOBtrigger = TRUE;
    return;
}



/*
 * Macintosh support functions.
 */
#if defined( macintosh )
int gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
}
#endif

/*
 * Windows 95 and Windows NT support functions
 */
#if defined( WIN32 )
void gettimeofday( struct timeval *tp, void *tzp )
{
    tp->tv_sec  = time( NULL );
    tp->tv_usec = 0;
  }
#endif

