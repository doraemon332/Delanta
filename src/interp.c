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
 *  Much time and thought has gone into this software and you are          *
 *  benefitting.  We hope that you share your changes too.  What goes      *
 *  around, comes around.                                                  *
 ***************************************************************************/

#if defined( macintosh )
#include <types.h>
#else
#include <sys/types.h>
#endif
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "merc.h"



bool	check_social	args( ( CHAR_DATA *ch, char *command,
			       char *argument ) );

/*
 * Command logging types.
 */
#define LOG_NORMAL              0
#define LOG_ALWAYS              1
#define LOG_NEVER               2

/*
 * God Levels - Check them out in merc.h
*/

#define L_HER                   LEVEL_HERO

/*
 * Log-all switch.
 */
bool				fLogAll		    = FALSE;



/*
 * Command table.
 */
const	struct	cmd_type	cmd_table	[ ] =
{
    /*
     * Common movement commands.
     */
    { "north",		do_north,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "east",		do_east,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "south",		do_south,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "west",		do_west,	POS_STANDING,	 0,  LOG_NORMAL },
    { "up",		do_up,		POS_STANDING,	 0,  LOG_NORMAL },
    { "down",		do_down,	POS_STANDING,	 0,  LOG_NORMAL },

    /*
     * Common other commands.
     * Placed here so one and two letter abbreviations work.
     */
    { "buy",		do_buy,		POS_STANDING,	 0,  LOG_NORMAL	},
    { "cast",		do_cast,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "exits",		do_exits,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "get",		do_get,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "inventory",	do_inventory,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "immtalk",	do_immtalk,	POS_DEAD,    L_HER,  LOG_NORMAL	},
    { "kill",		do_kill,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "look",		do_look,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "order",		do_order,	POS_RESTING,	 0,  LOG_ALWAYS	},
    { "quaff",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "rest",		do_rest,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "stand",		do_stand,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "tell",		do_tell,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "talk",		do_talk,	POS_RESTING,	 0,  LOG_NORMAL },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "wield",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "wizhelp",	do_wizhelp,	POS_DEAD,    L_HER,  LOG_NORMAL	},

    /*
     * Informational commands.
     */
    { "areas",		do_areas,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "bug",		do_bug,		POS_DEAD,	 0,  LOG_NORMAL	},
    { "commands",	do_commands,	POS_DEAD,	 0,  LOG_NORMAL },
    { "compare",	do_compare,	POS_RESTING,	 0,  LOG_NORMAL },
    { "consider",	do_consider,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "credits",	do_credits,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "equipment",	do_equipment,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "examine",	do_examine,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "help",		do_help,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "idea",		do_idea,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "report",		do_report,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "pagelength",     do_pagelen,     POS_DEAD,        0,  LOG_NORMAL },
    { "read",		do_look,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "score",		do_score,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "slist",          do_slist,       POS_DEAD,        0,  LOG_NORMAL },
    { "socials",	do_socials,	POS_DEAD,	 0,  LOG_NORMAL },
    { "time",		do_time,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "typo",		do_typo,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "weather",	do_weather,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "who",		do_who,		POS_DEAD,	 0,  LOG_NORMAL	},
    { "whois",		do_whois,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "wizlist",        do_wizlist,     POS_DEAD,        0,  LOG_NORMAL },

    /*
     * Combat commands.
     */
    { "backstab",	do_backstab,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "bs",		do_backstab,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "berserk",	do_berserk,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "circle",		do_circle,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "disarm",		do_disarm,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "fee",    	do_fee, 	POS_FIGHTING,	 0,  LOG_NORMAL	},    
    { "feed",   	do_feed,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "flee",		do_flee,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "kick",		do_kick,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "murder",		do_murder,	POS_FIGHTING,	 0,  LOG_ALWAYS	},
    { "rescue",		do_rescue,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "stake",		do_stake,	POS_STANDING,	 0,  LOG_NORMAL	},

    /*
     * Configuration commands.
     */
    { "auto",           do_auto,        POS_DEAD,        0,  LOG_NORMAL },
    { "autoexit",       do_autoexit,    POS_DEAD,        0,  LOG_NORMAL },
    { "autogold",       do_autogold,    POS_DEAD,        0,  LOG_NORMAL },
    { "autoloot",       do_autoloot,    POS_DEAD,        0,  LOG_NORMAL },
    { "autosac",        do_autosac,     POS_DEAD,        0,  LOG_NORMAL },
    { "blank",          do_blank,       POS_DEAD,        0,  LOG_NORMAL },
    { "brief",          do_brief,       POS_DEAD,        0,  LOG_NORMAL },
    { "channels",	do_channels,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "combine",        do_combine,     POS_DEAD,        0,  LOG_NORMAL },
    { "config",		do_config,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "description",	do_description,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "password",	do_password,	POS_DEAD,	 0,  LOG_NEVER	},
    { "prompt",         do_prompt,      POS_DEAD,        0,  LOG_NORMAL },
    { "title",		do_title,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "cname",          do_cname,       POS_DEAD,        0,  LOG_NORMAL },
    { "wimpy",		do_wimpy,	POS_DEAD,	 0,  LOG_NORMAL	},

    /*
     * Communication commands.
     */
    { "answer",		do_answer,	POS_SLEEPING,	 3,  LOG_NORMAL },
    { "auction",	do_auction,	POS_SLEEPING,	 3,  LOG_NORMAL	},
    { "beep",		do_beep,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "chat",		do_chat,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { ".",		do_chat,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "emote",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL	},
    { ",",		do_emote,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "gtell",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL	},
    { ";",		do_gtell,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "music",		do_music,	POS_SLEEPING,	 3,  LOG_NORMAL },
    { "note",		do_note,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "pose",		do_pose,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "question",	do_question,	POS_SLEEPING,	 3,  LOG_NORMAL },
    { "reply",		do_reply,	POS_SLEEPING,	 0,  LOG_NORMAL },
    { "say",		do_say,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "'",		do_say,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "shout",		do_shout,	POS_RESTING,	 3,  LOG_NORMAL	},
    { "yell",		do_yell,	POS_RESTING,	 3,  LOG_NORMAL	},

    /*
     * Object manipulation commands.
     */
    { "brandish",	do_brandish,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "close",		do_close,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "donate",         do_donate,      POS_RESTING,     0,  LOG_NORMAL },
    { "drink",		do_drink,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "drop",		do_drop,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "eat",		do_eat,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "fill",		do_fill,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "give",		do_give,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "hold",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "list",		do_list,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "lock",		do_lock,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "open",		do_open,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "pick",		do_pick,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "put",		do_put,		POS_RESTING,	 0,  LOG_NORMAL },
    { "quaff",		do_quaff,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "recite",		do_recite,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "remove",		do_remove,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "sell",		do_sell,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "take",		do_get,		POS_RESTING,	 0,  LOG_NORMAL	},
    { "sacrifice",	do_sacrifice,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "unlock",		do_unlock,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "value",		do_value,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "wear",		do_wear,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "zap",		do_zap,		POS_RESTING,	 0,  LOG_NORMAL	},

    /*
     * Miscellaneous commands.
     */
    { "afk",            do_afk,         POS_SLEEPING,    0,  LOG_NORMAL },
    { "bash",           do_bash,        POS_STANDING,    0,  LOG_NORMAL },
    { "backup",		do_backup,	POS_DEAD,	 0,  LOG_NORMAL },
    { "bet",            do_bet,         POS_STANDING,    0,  LOG_NORMAL },
    { "wager",          do_bet,         POS_STANDING,    0,  LOG_NORMAL },
    { "chameleon power",do_chameleon,   POS_STANDING,    0,  LOG_NORMAL },
    { "follow",		do_follow,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "group",		do_group,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "heighten senses",do_heighten,    POS_STANDING,    0,  LOG_NORMAL },
    { "hide",		do_hide,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "poison weapon",	do_poison_weapon,POS_SLEEPING,	 0,  LOG_NORMAL	},
/*    { "practice",	do_practice,	POS_SLEEPING,	 0,  LOG_NORMAL	},*/
    { "qui",		do_qui,		POS_DEAD,	 0,  LOG_NORMAL	},
    { "quit",		do_quit,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "recall",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "/",		do_recall,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "register",	do_register,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "rent",		do_rent,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "save",		do_save,	POS_DEAD,	 0,  LOG_NORMAL	},
    { "save_skill",	do_save_skill,  POS_DEAD,    L_DIR,  LOG_ALWAYS },
    { "shadow form",    do_shadow,      POS_STANDING,    0,  LOG_NORMAL },
    { "sleep",		do_sleep,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "snare",		do_snare,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "sneak",		do_sneak,	POS_STANDING,	 0,  LOG_NORMAL	},
    { "spells",         do_spells,      POS_SLEEPING,    0,  LOG_NORMAL },
    { "split",		do_split,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "steal",		do_steal,	POS_STANDING,	 0,  LOG_NORMAL	},
/*    { "train",		do_train,	POS_RESTING,	 0,  LOG_NORMAL	},*/
    { "untangle",	do_untangle,	POS_FIGHTING,	 0,  LOG_NORMAL	},
    { "visible",	do_visible,	POS_SLEEPING,	 0,  LOG_NORMAL },
    { "wake",		do_wake,	POS_SLEEPING,	 0,  LOG_NORMAL	},
    { "where",		do_where,	POS_RESTING,	 0,  LOG_NORMAL	},
    { "clear",		do_clear,	POS_DEAD,	 0,  LOG_NORMAL },

    /*
     * Immortal commands.
     */
    { "advance",	do_advance,	POS_DEAD,    L_DIR,  LOG_ALWAYS	},
    { "trust",		do_trust,	POS_DEAD,    L_DIR,  LOG_ALWAYS },

    { "allow",		do_allow,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "ban",		do_ban,		POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "deny",		do_deny,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "disconnect",	do_disconnect,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "force",		do_force,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "freeze",		do_freeze,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "imtlset",	do_imtlset,	POS_DEAD,    L_SEN,  LOG_ALWAYS },
    { "log",		do_log,		POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "mset",		do_mset,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "noemote",	do_noemote,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "notell",		do_notell,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "numlock",	do_numlock,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "oload",		do_oload,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "oset",		do_oset,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "pardon",		do_pardon,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "purge",		do_purge,	POS_DEAD,    L_SEN,  LOG_NORMAL	},
    { "reboo",		do_reboo,	POS_DEAD,    L_SEN,  LOG_NORMAL	},
    { "reboot",		do_reboot,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "restore",	do_restore,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "shutdow",	do_shutdow,	POS_DEAD,    L_SEN,  LOG_NORMAL	},
    { "shutdown",	do_shutdown,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "silence",	do_silence,	POS_DEAD,    L_SEN,  LOG_ALWAYS },
    { "sla",		do_sla,		POS_DEAD,    L_SEN,  LOG_NORMAL	},
    { "slay",		do_slay,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "sset",		do_sset,	POS_DEAD,    L_SEN,  LOG_ALWAYS },
    { "sstime",		do_sstime,	POS_DEAD,    L_SEN,  LOG_ALWAYS },
    { "transfer",	do_transfer,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "users",		do_users,	POS_DEAD,    L_SEN,  LOG_NORMAL	},
    { "wizify", 	do_wizify,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},
    { "wizlock",	do_wizlock,	POS_DEAD,    L_SEN,  LOG_ALWAYS	},

    { "echo",		do_echo,	POS_DEAD,    L_JUN,  LOG_ALWAYS	},
    { "memory",		do_memory,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "mload",		do_mload,	POS_DEAD,    L_JUN,  LOG_ALWAYS	},
    { "mfind",		do_mfind,	POS_DEAD,    L_JUN,  LOG_NORMAL },
    { "mstat",		do_mstat,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "mwhere",		do_mwhere,	POS_DEAD,    L_JUN,  LOG_NORMAL },
    { "newlock",	do_newlock,	POS_DEAD,    L_JUN,  LOG_ALWAYS	},
    { "ofind",		do_ofind,	POS_DEAD,    L_JUN,  LOG_NORMAL },
    { "ostat",		do_ostat,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "owhere",		do_owhere,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "peace",		do_peace,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "recho",		do_recho,	POS_DEAD,    L_JUN,  LOG_ALWAYS	},
    { "return",		do_return,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "rset",		do_rset,	POS_DEAD,    L_JUN,  LOG_ALWAYS	},
    { "rstat",		do_rstat,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "slookup",	do_slookup,	POS_DEAD,    L_JUN,  LOG_NORMAL },
    { "snoop",		do_snoop,	POS_DEAD,    L_JUN,  LOG_NORMAL	},
    { "switch",		do_switch,	POS_DEAD,    L_JUN,  LOG_ALWAYS	},
    { "wizinvis",	do_invis,	POS_DEAD,    L_JUN,  LOG_NORMAL	},

    { "at",		do_at,		POS_DEAD,    L_APP,  LOG_NORMAL	},
    { "bamfin",		do_bamfin,	POS_DEAD,    L_APP,  LOG_NORMAL	},
    { "bamfout",	do_bamfout,	POS_DEAD,    L_APP,  LOG_NORMAL	},
    { "goto",		do_goto,	POS_DEAD,    L_APP,  LOG_NORMAL	},
    { "holylight",	do_holylight,	POS_DEAD,    L_APP,  LOG_NORMAL	},

    { ":",		do_immtalk,	POS_DEAD,    L_HER,  LOG_NORMAL	},

    /*
     * MOBprogram commands.
     */
    { "mpstat",         do_mpstat,      POS_DEAD,    L_JUN,  LOG_NORMAL },
    { "mpasound",       do_mpasound,    POS_DEAD,        0,  LOG_NORMAL },
    { "mpjunk",         do_mpjunk,      POS_DEAD,        0,  LOG_NORMAL },
    { "mpecho",         do_mpecho,      POS_DEAD,        0,  LOG_NORMAL },
    { "mpechoat",       do_mpechoat,    POS_DEAD,        0,  LOG_NORMAL },
    { "mpechoaround",   do_mpechoaround,POS_DEAD,        0,  LOG_NORMAL },
    { "mpkill",         do_mpkill      ,POS_DEAD,        0,  LOG_NORMAL },
    { "mpmload",        do_mpmload     ,POS_DEAD,        0,  LOG_NORMAL },
    { "mpoload",        do_mpoload     ,POS_DEAD,        0,  LOG_NORMAL },
    { "mppurge",        do_mppurge     ,POS_DEAD,        0,  LOG_NORMAL },
    { "mpgoto",         do_mpgoto      ,POS_DEAD,        0,  LOG_NORMAL },
    { "mpat",           do_mpat        ,POS_DEAD,        0,  LOG_NORMAL },
    { "mptransfer",     do_mptransfer  ,POS_DEAD,        0,  LOG_NORMAL },
    { "mpforce",        do_mpforce     ,POS_DEAD,        0,  LOG_NORMAL },
    
    /*
     * OLC 1.1b
     */
    { "aedit",		do_aedit,	POS_DEAD, L_SEN,  LOG_NORMAL },
    { "redit",		do_redit,	POS_DEAD, L_SEN,  LOG_NORMAL },
    { "oedit",		do_oedit,	POS_DEAD, L_SEN,  LOG_NORMAL },
    { "medit",		do_medit,	POS_DEAD, L_SEN,  LOG_NORMAL },
    { "mpedit",		do_mpedit,	POS_DEAD, L_SEN,  LOG_NORMAL },    
    { "asave",          do_asave,	POS_DEAD, L_SEN,  LOG_NORMAL },
    { "alist",		do_alist,	POS_DEAD, L_SEN,  LOG_NORMAL },
    { "resets",		do_resets,	POS_DEAD, L_SEN,  LOG_NORMAL },    
    
    /* For new skill system */
    { "join",		do_join,	POS_DEAD,	1, LOG_NORMAL },
    { "skills",		do_skills,	POS_DEAD,	1, LOG_NORMAL },
    { "reloadskill",	do_reloadskill ,POS_DEAD,   L_DIR, LOG_NORMAL },
    
    /*
     * End of list.
     */
    { "",		0,		POS_DEAD,	 0,  LOG_NORMAL	}
};


/*
 * The main entry point for executing commands.
 * Can be recursively called from 'at', 'order', 'force'.
 */
void interpret( CHAR_DATA *ch, char *argument )
{
    char command [ MAX_INPUT_LENGTH ];
    char logline [ MAX_INPUT_LENGTH ];
    int  cmd;
    int  trust;
    bool found;

    /*
     * Strip leading spaces.
     */
    while ( isspace( *argument ) )
	argument++;
    if ( argument[0] == '\0' )
	return;

    /*
     * No hiding and remove AFK
     */
    REMOVE_BIT( ch->affected_by, AFF_HIDE );
    if ( !IS_NPC( ch ) )
        REMOVE_BIT( ch->act, PLR_AFK );

    /*
     * Implement freeze command.
     */
    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_FREEZE ) )
    {
	send_to_char( "You're totally frozen!\n\r", ch );
	return;
    }

    /*
     * Grab the command word.
     * Special parsing so ' can be a command,
     *   also no spaces needed after punctuation.
     */
    strcpy( logline, argument );
    if ( !isalpha( argument[0] ) && !isdigit( argument[0] ) )
    {
	command[0] = argument[0];
	command[1] = '\0';
	argument++;
	while ( isspace( *argument ) )
	    argument++;
    }
    else
    {
	argument = one_argument( argument, command );
    }

    /*
     * Look for command in command table.
     */
    found = FALSE;
    trust = get_trust( ch );
    for ( cmd = 0; cmd_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == cmd_table[cmd].name[0]
	    && !str_prefix( command, cmd_table[cmd].name )
	    && ( cmd_table[cmd].level <= trust ) )
	{
	    found = TRUE;
	    break;
	}
    }

    /*
     * Log and snoop.
     */
    if ( cmd_table[cmd].log == LOG_NEVER )
	strcpy( logline, "XXXXXXXX XXXXXXXX XXXXXXXX" );

    if ( ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_LOG ) )
	|| fLogAll
	|| cmd_table[cmd].log == LOG_ALWAYS )
    {
	sprintf( log_buf, "Log %s: %s", ch->name, logline );
	log_string( log_buf );
    }

    if ( ch->desc && ch->desc->snoop_by )
    {
	write_to_buffer( ch->desc->snoop_by, "% ",    2 );
	write_to_buffer( ch->desc->snoop_by, logline, 0 );
	write_to_buffer( ch->desc->snoop_by, "\n\r",  2 );
    }

    if ( !found )
    {
	/*
	 * Look for command in socials table.
	 */
	if ( !check_social( ch, command, argument ) )
	    send_to_char( "Huh?\n\r", ch );
	return;
    }

    /*
     * Character not in position for command?
     */
    if ( ch->position < cmd_table[cmd].position )
    {
	switch( ch->position )
	{
	case POS_DEAD:
	    send_to_char( "Lie still; you are DEAD.\n\r", ch );
	    break;

	case POS_MORTAL:
	case POS_INCAP:
	    send_to_char( "You are hurt far too bad for that.\n\r", ch );
	    break;

	case POS_STUNNED:
	    send_to_char( "You are too stunned to do that.\n\r",    ch );
	    break;

	case POS_SLEEPING:
	    send_to_char( "In your dreams, or what?\n\r",           ch );
	    break;

	case POS_RESTING:
	    send_to_char( "Nah... You feel too relaxed...\n\r",     ch );
	    break;

	case POS_FIGHTING:
	    send_to_char( "No way!  You are still fighting!\n\r",   ch );
	    break;

	}
	return;
    }

    /*
     * Dispatch the command.
     */
    (*cmd_table[cmd].do_fun) ( ch, argument );

    tail_chain( );
    return;
}



bool check_social( CHAR_DATA *ch, char *command, char *argument )
{
    CHAR_DATA *victim;
    char       arg [ MAX_INPUT_LENGTH ];
    int        cmd;
    bool       found;

    found  = FALSE;
    for ( cmd = 0; social_table[cmd].name[0] != '\0'; cmd++ )
    {
	if ( command[0] == social_table[cmd].name[0]
	    && !str_prefix( command, social_table[cmd].name ) )
	{
	    found = TRUE;
	    break;
	}
    }

    if ( !found )
	return FALSE;

    if ( !IS_NPC( ch ) && IS_SET( ch->act, PLR_NO_EMOTE ) )
    {
	send_to_char( "You are anti-social!\n\r", ch );
	return TRUE;
    }

    switch ( ch->position )
    {
    case POS_DEAD:
	send_to_char( "Lie still; you are DEAD.\n\r",             ch );
	return TRUE;

    case POS_INCAP:
    case POS_MORTAL:
	send_to_char( "You are hurt far too badly for that.\n\r", ch );
	return TRUE;

    case POS_STUNNED:
	send_to_char( "You are too stunned to do that.\n\r",      ch );
	return TRUE;

    case POS_SLEEPING:
	/*
	 * I just know this is the path to a 12" 'if' statement.  :(
	 * But two players asked for it already!  -- Furey
	 */
	if ( !str_cmp( social_table[cmd].name, "snore" ) )
	    break;
	send_to_char( "In your dreams, or what?\n\r",             ch );
	return TRUE;

    }

    one_argument( argument, arg );
    victim = NULL;
    if ( arg[0] == '\0' )
    {
	act( social_table[cmd].char_no_arg,   ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].others_no_arg, ch, NULL, victim, TO_ROOM    );
    }
    else if ( !( victim = get_char_world( ch, arg ) ) )
    {
	send_to_char( "They aren't here.\n\r",                    ch );
    }
    else if ( victim == ch )
    {
	act( social_table[cmd].char_auto,     ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].others_auto,   ch, NULL, victim, TO_ROOM    );
    }
    else if ( !get_char_room( ch, arg ) && can_see( ch, victim ) )
    {
	if ( !IS_NPC( victim ) )
        {
	    ROOM_INDEX_DATA *original;
	    char            *ldbase                      = "From far away, ";
	    char             ldmsg [ MAX_STRING_LENGTH ];

	    original = ch->in_room;
	    char_from_room( ch );
	    char_to_room( ch, victim->in_room );

	    strcpy( ldmsg, ldbase );
	    strcat( ldmsg, social_table[cmd].char_found );
	    act( ldmsg,                       ch, NULL, victim, TO_CHAR    );

	    strcpy( ldmsg, ldbase );
	    strcat( ldmsg, social_table[cmd].vict_found );
	    act( ldmsg,                       ch, NULL, victim, TO_VICT    );

	    char_from_room( ch );
	    char_to_room( ch, original );
	}
	else
	{
	    send_to_char( "They aren't here.\n\r",                ch );
	}
    }
    else
    {
	act( social_table[cmd].char_found,    ch, NULL, victim, TO_CHAR    );
	act( social_table[cmd].vict_found,    ch, NULL, victim, TO_VICT    );
	act( social_table[cmd].others_found,  ch, NULL, victim, TO_NOTVICT );

	if ( !IS_NPC( ch )
	    && IS_NPC( victim )
	    && !IS_AFFECTED( victim, AFF_CHARM )
	    && IS_AWAKE( victim ) )
	{
	    switch ( number_bits( 4 ) )
	    {
	    case 0:
	        if ( ( victim->level < ch->level )
		    && !( victim->fighting ) )
		    multi_hit( victim, ch, TYPE_UNDEFINED );
		break;

	    case 1: case 2: case 3: case 4:
	    case 5: case 6: case 7: case 8:
		act( social_table[cmd].char_found,
		    victim, NULL, ch, TO_CHAR    );
		act( social_table[cmd].vict_found,
		    victim, NULL, ch, TO_VICT    );
		act( social_table[cmd].others_found,
		    victim, NULL, ch, TO_NOTVICT );
		break;

	    case 9: case 10: case 11: case 12:
		act( "You slap $N.",  victim, NULL, ch, TO_CHAR    );
		act( "$n slaps you.", victim, NULL, ch, TO_VICT    );
		act( "$n slaps $N.",  victim, NULL, ch, TO_NOTVICT );
		break;
	    }
	}
    }

    return TRUE;
}



/*
 * Return true if an argument is completely numeric.
 */
bool is_number( char *arg )
{
    if ( *arg == '\0' )
	return FALSE;

    if ( *arg == '+' || *arg == '-' )
        arg++;

    for ( ; *arg != '\0'; arg++ )
    {
	if ( !isdigit( *arg ) )
	    return FALSE;
    }

    return TRUE;
}



/*
 * Given a string like 14.foo, return 14 and 'foo'
 */
int number_argument( char *argument, char *arg )
{
    char *pdot;
    int   number;
    
    for ( pdot = argument; *pdot != '\0'; pdot++ )
    {
	if ( *pdot == '.' )
	{
	    *pdot = '\0';
	    number = atoi( argument );
	    *pdot = '.';
	    strcpy( arg, pdot+1 );
	    return number;
	}
    }

    strcpy( arg, argument );
    return 1;
}



/*
 * Pick off one argument from a string and return the rest.
 * Understands quotes.
 */
char *one_argument( char *argument, char *arg_first )
{
    char cEnd;

    while ( isspace( *argument ) )
	argument++;

    cEnd = ' ';
    if ( *argument == '\'' || *argument == '"' )
	cEnd = *argument++;

    while ( *argument != '\0' )
    {
	if ( *argument == cEnd )
	{
	    argument++;
	    break;
	}
	*arg_first = LOWER( *argument );
	arg_first++;
	argument++;
    }
    *arg_first = '\0';

    while ( isspace( *argument ) )
	argument++;

    return argument;
}
