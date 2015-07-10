#if defined(macintosh)
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "merc.h"

#if !defined(macintosh)
extern	int	_filbuf		args( (FILE *) );
#endif

QMARK *qmark_free;

void set_qmark( CHAR_DATA *victim, char *arg )
{
    QMARK *qmark;

    if( qmark_free == NULL )
        qmark = alloc_perm( sizeof(*qmark) );
    else
    {
        qmark = qmark_free;    
        qmark_free = qmark_free->next;
    }    
    qmark->next = victim->pcdata->quest_mark;
    victim->pcdata->quest_mark = qmark;
    smash_tilde( arg );
    qmark->mark = str_dup( arg);
    return;
}

bool check_qmark( CHAR_DATA *victim, char *arg )
{
    QMARK *qmark,*qmark_next;

    for( qmark=victim->pcdata->quest_mark; qmark != NULL; qmark=qmark_next )
    {
        qmark_next = qmark->next;
        if( !str_cmp( qmark->mark , arg ) )
            return TRUE;
    }        
    return FALSE;        
}

bool del_qmark( CHAR_DATA *victim, char *arg )
{
    QMARK *qmark, *qmark_last, *qmark_next;

    qmark = victim->pcdata->quest_mark;
    
    if( !str_cmp( qmark->mark, arg ) )
    {
        free_string( qmark->mark );
        victim->pcdata->quest_mark = qmark->next;
        qmark->next = qmark_free;
        qmark_free = qmark;
    }
    else
    {
        qmark_last = qmark;
        for( qmark = qmark->next; qmark != NULL; qmark = qmark_next )
        {
            qmark_next = qmark->next;
            if( !str_cmp( qmark->mark, arg ) )
            {
    		free_string( qmark->mark );
    		qmark_last->next = qmark->next;
    		qmark->next = qmark_free;
    		qmark_free = qmark;            	
            }
            qmark_last = qmark;
        }
    }    

    return TRUE;
}

/* 格式: mpaddmark <victim> <mark> */
void do_mpadd_qmark( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];    
    char *pTmp;
    CHAR_DATA *victim;
    
    if ( !IS_NPC( ch ) )
    {
        send_to_char( "你想幹嘛?\n\r", ch );
	return;
    }
    pTmp = one_argument( argument, arg1);
    one_argument( pTmp, arg2 );
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
	char buf[100];
	if( !str_cmp(arg1,"不明人物") )
           return; /* 這是因為看不見才會這樣 不是出錯*/
	
	sprintf( buf," BUG : MPADDMARK : no VICTIM %s !! mob %d", 
	      arg1 , ch->pIndexData->vnum );
	bug( buf,0);
	return;
    }    
    if ( arg2[0] == '\0' )
    {
	bug( " BUG : MPADDMARK : no MARK string !! mob %d ", ch->pIndexData->vnum );
	return;
    }        
    if ( IS_NPC(victim) )
	return;    
    if( check_qmark( victim, arg2 ) == FALSE ) 
        set_qmark( victim, arg2 );
}

/* 格式: mpdelmark <victim> <mark> */
void do_mpdel_qmark( CHAR_DATA *ch, char *argument )
{
    char arg1[MAX_STRING_LENGTH];
    char arg2[MAX_STRING_LENGTH];    
    char *pTmp;
    CHAR_DATA *victim;
    
    if ( !IS_NPC( ch ) )
    {
        send_to_char( "你想幹嘛?\n\r", ch );
	return;
    }
    pTmp = one_argument( argument, arg1);
    one_argument( pTmp, arg2 );
    if ( ( victim = get_char_room( ch, arg1 ) ) == NULL )
    {
        char buf[100];
        if( !str_cmp(arg1,"不明人物") )
            return; /* 這是因為看不見才會這樣 不是出錯*/
    
	sprintf(buf, " BUG : MPDELMARK : no VICTIM %s !! mob %d",
	         arg1,ch->pIndexData->vnum );
	bug( buf, 0);
	return;
    }    
    if ( arg2[0] == '\0' )
    {
	bug( " BUG : MPDELMARK : no MARK string !! mob %d ",ch->pIndexData->vnum );
	return;
    }        
    if ( IS_NPC(victim) )
	return;    
    if( check_qmark( victim, arg2) == TRUE )
        del_qmark( victim, arg2 );
}

/* 格式: is_qmark <victim> <opr> <mark> */
bool is_qmark( CHAR_DATA *victim, char* opr,char *mark )
{
    bool found;
    char arg[MAX_STRING_LENGTH];
    
    if( IS_NPC( victim ) )
        return FALSE;
    one_argument( mark, arg );
    found = check_qmark( victim, arg );
    
    if( !str_cmp( opr, "!=" ) )
        found = ((found == TRUE) ? FALSE : TRUE);
    else if( str_cmp( opr, "==" ) )   
        bug("Is_qmark: bad opr .", 0 );
    return found;
}