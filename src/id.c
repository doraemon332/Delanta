/*
  * rfc931_user() consults the RFC 931 daemon on the client host to look up
  * the remote user name.
  *
  * Diagnostics are reported through syslog(3).
  *
  * Author: Wietse Venema, Eindhoven University of Technology, The Netherlands.
  *
  * Inspired by the authutil package (comp.sources.unix volume 22) by Dan
  * Bernstein (brnstnd@kramden.acf.nyu.edu).
  */


#include <stdio.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <setjmp.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include "merc.h"


 /*
  * Structure filled in by the fromhost() routine. Prerequisites:
  * <sys/types.h> and <sys/param.h>.
  */

#define FROM_UNKNOWN    "unknown"       /* name or address lookup failed */


#define RFC931_PORT     113             /* Semi-well-known port */
#define TIMEOUT         1               /* wait for at most 30 seconds */

static jmp_buf timebuf;

/* timeout - handle timeouts */
void timeout( int sig )
{
    longjmp( timebuf, sig );
}


/* rfc931_name - return remote user name */
char *rfc931_name( int desc, struct sockaddr_in *there)
{
    struct sockaddr_in   here;          /* local link information */
    struct sockaddr_in   sine;           /* for talking to RFC931 daemon */
    int                  length;
    int                  s;
    unsigned             remote;
    unsigned             local;
    static char          user[256];             /* XXX */
    FILE                *fp;
    char                *cp;
    char                *result=FROM_UNKNOWN;
    
    /* Find out local address and port number of stdin. */

    length = sizeof(here);
    if ( getsockname( desc, (struct sockaddr *) & here, &length ) == -1)
        return result;

    /*
     * The socket that will be used for user name lookups should be bound to
     * the same local IP address as stdin. This will automagically happen on
     * hosts that have only one IP network interface. When the local host has
     * more than one IP network interface we must do an explicit bind() call.
     */

    if ( ( s = socket( AF_INET, SOCK_STREAM, 0 ) ) == -1 )
        return result;

    sine = here;
    sine.sin_port = 0;
    if ( bind( s, (struct sockaddr *)&sine, sizeof(sine) ) < 0 )
        return result;

    /* Set up timer so we won't get stuck. */

    signal( SIGALRM, timeout );
    if ( setjmp(timebuf) )
    {
        close( s );                             /* not: fclose(fp) */
        return result;
    }
    alarm( TIMEOUT );

    /* Connect to the RFC931 daemon. */
    sine = *there;
    sine.sin_port = htons( RFC931_PORT );
    if (connect(s, (struct sockaddr *) &sine, sizeof(sine) ) == -1
        || ( fp = fdopen( s, "w+" ) ) == 0 )
    {
        close( s );
        alarm( 0 );
        return result;
    }

    /* Query the RFC 931 server. Would 13-byte writes ever be broken up? */
    fprintf( fp, "%u,%u\r\n", ntohs(there->sin_port), ntohs(here.sin_port) );
    fflush( fp );

    /* Read response. Kill stdio buffer or we may read back our own query. */
    setbuf( fp, (char *) 0 );
    if ( fscanf( fp, "%u , %u : USERID :%*[^:]:%255s", &remote, &local, user )
== 3 
        && ferror(fp) == 0
        && feof(fp) == 0
        && ntohs(there->sin_port) == remote
        && ntohs(here.sin_port) == local)
    {
        /* Strip trailing carriage return. */
        if ( ( cp = strchr( user, '\r' ) ) )
            *cp = 0;
        result = user;
    }
    alarm( 0 );
    fclose( fp );

    return result;
}


