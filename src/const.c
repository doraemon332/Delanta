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
#include <stdio.h>
#include <time.h>
#include "merc.h"



/*
 * Class table.
 */
const	struct	class_type	class_table	[MAX_CLASS]	=
{
    {
	"Mag", "魔法師", APPLY_INT,  OBJ_VNUM_SCHOOL_DAGGER,
	3018,  95,  18,  6,   60,  80, 4, TRUE
    },

    {
	"Cle", "聖術師", APPLY_WIS,  OBJ_VNUM_SCHOOL_MACE,
	3003,  95,  18,  9,  70,  100, 5, TRUE, TRUE
    },

    {
	"Thi", "盜賊", APPLY_DEX,  OBJ_VNUM_SCHOOL_DAGGER,
	3028,  85,  18,  3,  80,  150, 3, FALSE
    },

    {
	"War", "戰士", APPLY_STR,  OBJ_VNUM_SCHOOL_SWORD,
	3022,  85,  18,  0,  100, 300, 10, FALSE
    },

    {
        "Psi", "邪術師", APPLY_WIS,  OBJ_VNUM_SCHOOL_DAGGER,
        3151,  95,  18,  9,   60,  90, 7, TRUE, TRUE
    },
    
    {
        "Non", "無職業", APPLY_STR, OBJ_VNUM_SCHOOL_SWORD,
        3022, 95, 18, 8, 50, 100, 20, FALSE
    },       
    
    {
        "Phy", "物理學家", APPLY_WIS, OBJ_VNUM_SCHOOL_REFERENCE_BOOK,
        3022, 88, 18, 8, 30,  80, 3, TRUE
    },
    
    {
        "Che", "化學家",  APPLY_INT, OBJ_VNUM_SCHOOL_REFERENCE_BOOK,
        3022, 88, 18, 8, 40, 90, 2, TRUE
    },
    
    {
        "Bio", "生物學家", APPLY_CON, OBJ_VNUM_SCHOOL_REFERENCE_BOOK,
        3022, 85, 18, 8, 50, 100, 5, TRUE
    },
    
    {   
        "Her", "勇者",   APPLY_STR, OBJ_VNUM_SCHOOL_SWORD,
        3022, 85, 18, 8, 80, 200, 50, FALSE
    },
        
    
};

/*
 * Immort Levels
*/
#define L_HER            LEVEL_HERO


/* 
 * Race types
 */
const   struct    race_type       race_table      [ MAX_RACE ]    =
{
    { "Human",     RACE_PC_AVAIL | RACE_WEAPON_WIELD,      3, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, "空手一擊", "Githyanki Vampire Werewolf Mindflayer"      },
    { "Elf",       RACE_PC_AVAIL | RACE_INFRAVISION | RACE_DETECT_HIDDEN      |
	           RACE_WEAPON_WIELD,                      2, 0, 1, 0, 1, -1,
	0, 4, 0, 0, 0, "punch",
	"Drow Ogre Orc Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin Halfkobold"                                                                    },
    { "Halfelf",   RACE_PC_AVAIL | RACE_INFRAVISION | RACE_WEAPON_WIELD,
	                                                   3, 0, 0, 0, 1, 0,
	0, 2, 0, 0, 0, "punch",
	"Drow Ogre Orc Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin"},
    { "Drow",      RACE_PC_AVAIL | RACE_INFRAVISION | RACE_DETECT_HIDDEN      |
	           RACE_WEAPON_WIELD,                      2, 0, 0, 1, 1, 0,
	0, 4, 0, 0, 0, "punch",
	"Elf Halfelf Hobbit Githyanki Vampire Werewolf"             },
    { "Dwarf",     RACE_PC_AVAIL | RACE_INFRAVISION | RACE_DETECT_HIDDEN      |
	           RACE_WEAPON_WIELD,                      2, 0, 0, 0, -1, 1,
	0, 0, 0, 0, 1, "punch",
	"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin Halfkobold"                                                          },
    { "Halfdwarf", RACE_PC_AVAIL | RACE_INFRAVISION | RACE_WEAPON_WIELD,
	                                                   2, 0, 0, 0, 0, 1,
	0, 0, 0, 0, 0, "punch",
	"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin"                                                                     },
    { "Hobbit",    RACE_PC_AVAIL | RACE_INFRAVISION | RACE_DETECT_HIDDEN      |
	           RACE_WEAPON_WIELD,                      2, 0, 0, 0, 1, -1,
	0, 0, 0, 0, 0, "punch",
	"Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Dragon Vampire Werewolf Goblin Halfkobold"                                                          },
    { "Gnome",     RACE_PC_AVAIL | RACE_INFRAVISION | RACE_WEAPON_WIELD,
	                                                   2, -1, 0, 1, 1, -1,
	0, 4, 0, 0, 0, "punch",
	"Drow Ogre Orc Kobold Troll Hobgoblin Dragon Vampire Werewolf Goblin"},
    { "Halfkobold", RACE_PC_AVAIL | RACE_INFRAVISION | RACE_WEAPON_WIELD,
	                                                  2, -2, -1, -2, 3, -2,
	0, 0, 0, 0, 0, "punch", "Ogre Orc Giant Troll Hobgoblin"             },
    { "Giant",     RACE_WEAPON_WIELD,                      6, 2, -1, 0, -1, 1,
	3, -5, -2, 2, 4, "fist",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"          },
    { "Ogre",      RACE_WEAPON_WIELD,                      6, 1, -1, 0, -1, 1,
	2, -3, -1, 0, 2, "fist",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"          },
    { "Orc",       RACE_INFRAVISION | RACE_WEAPON_WIELD,   4, 1, -1, 0, 0, 0,
	2, -1, -1, 0, 1, "punch",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"          },
    { "Kobold",    RACE_INFRAVISION | RACE_WEAPON_WIELD,   2, -1, -1, 0, 1, 0,
	0, 0, 0, 0, 0, "punch",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome Halfkobold"},
    { "Minotaur",  RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD, 5, 2, 0, 0, -1, 1,
	3, 0, -1, 0, 3, "fist",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"          },
    { "Troll",     RACE_INFRAVISION | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
	                                                   7, 2, -1, 0, 0, 1,
	10, 0, 0, 0, 8, "fist",
	"Human Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"    },
    { "Hobgoblin", RACE_INFRAVISION | RACE_WEAPON_WIELD,   3, 1, 0, -1, 0, 1,
	0, 0, 0, 0, 0, "punch",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"          },
    { "Insect",    RACE_MUTE,                              0, 0, 0, 0, 0, -1,
	0, 0, 5, 0, 0, "bite", ""                                            },
    { "Dragon",    RACE_FLY | RACE_INFRAVISION | RACE_DETECT_ALIGN |
	           RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
	                                                   9, 2, 2, 1, -3, 2,
	15, 0, -10, 0, 10, "claw", ""                                        },
    { "Animal",    RACE_DETECT_HIDDEN | RACE_MUTE,         2, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, "bite", "Kobold Halfkobold"                           },
    { "God",       RACE_WATERBREATH | RACE_FLY | RACE_SWIM | RACE_WATERWALK   |
	           RACE_PASSDOOR | RACE_INFRAVISION | RACE_DETECT_ALIGN       |
		   RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_PROTECTION   |
		   RACE_SANCT | RACE_WEAPON_WIELD,         8, 3, 3, 3, 3, 3,
	20, 0, 10, 0, 0, "空手一擊", ""                                         },
    { "Undead",    RACE_PASSDOOR | RACE_INFRAVISION | RACE_DETECT_ALIGN       |
	           RACE_DETECT_INVIS | RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD,
	                                                   3, 1, 0, 0, -2, 1,
	0, 0, 0, -1, -1, "touch",
	"Human Elf Halfelf Drow Dwarf Halfdwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Goblin Faerie Gnome"                                  },
    { "Harpy",     RACE_FLY | RACE_DETECT_INVIS | RACE_MUTE,
	                                                   3, 0, 0, 0, 2, 0,
	0, 0, 6, 0, 0, "claw",
	"Human Elf Halfelf Dwarf Halfdwarf Hobbit Gnome"                     },
    { "Bear",      RACE_SWIM | RACE_DETECT_HIDDEN | RACE_MUTE,
	                                                   3, 1, 0, 0, -1, 1,
	0, 0, 0, 0, 3, "swipe", ""                                           },
    { "Githyanki", RACE_WEAPON_WIELD,                      3, 0, 1, 0, 0, 0,
	0, 4, 0, 0, 0, "punch", "Mindflayer"                                 },
    { "Elemental", RACE_MUTE,                              4, 1, 0, 0, 0, 1,
	0, 0, 0, -1, -1, "punch", ""                                         },
    { "Bat",       RACE_FLY | RACE_INFRAVISION | RACE_MUTE,
	                                                   1, -1, 0, 0, 2, -1,
	0, 0, 4, 0, 0, "bite", ""                                            },
    { "Plant",     RACE_MUTE,                              1, 0, 0, 0, -1, 1,
	0, 0, 0, -1, 0, "swipe", ""                                          },
    { "Rat",       RACE_PASSDOOR | RACE_MUTE,              0, -1, 0, 0, 2, -1,
	0, 0, 0, 0, 0, "bite", ""                                            },
    { "Vampire",   RACE_FLY | RACE_PASSDOOR | RACE_INFRAVISION                |
	           RACE_DETECT_ALIGN | RACE_DETECT_INVIS | RACE_DETECT_HIDDEN |
		   RACE_WEAPON_WIELD,                      3, 1, 1, 0, 1, 2,
	10, 3, 3, -1, 10, "claw",
	"Human Elf Halfelf Drow Dwarf Halfdwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Werewolf Goblin Faerie Gnome"                         },
    { "Werewolf",  RACE_INFRAVISION | RACE_DETECT_ALIGN | RACE_DETECT_INVIS   |
	           RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD, 3, 2, 0, 1, 2, 3,
	10, 0, 5, 0, 5, "claw",
	"Human Elf Halfelf Drow Dwarf Halfdwarf Hobbit Giant Ogre Orc Kobold Minotaur Troll Hobgoblin Vampire Goblin Faerie Gnome"                          },
    { "Goblin",    RACE_INFRAVISION | RACE_WEAPON_WIELD,   2, -1, -1, -1, 1, 0,
	0, 0, 0, 0, 0, "punch",
	"Elf Halfelf Dwarf Halfdwarf Hobbit Vampire Werewolf Gnome"          },
    { "Faerie",    RACE_FLY | RACE_INFRAVISION | RACE_DETECT_INVIS            |
	           RACE_DETECT_HIDDEN | RACE_WEAPON_WIELD, 1, -2, 1, 1, 1, -1,
	0, 0, 7, 0, 0, "punch", ""                                           },
    { "Arachnid",  RACE_NO_ABILITIES | RACE_WEAPON_WIELD | RACE_MUTE,
	                                                   2, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, "bite", ""                                            },
    { "Mindflayer",RACE_INFRAVISION | RACE_WEAPON_WIELD,   3, 1, 2, 1, -1, 0,
	0, 6, 0, 0, 0, "punch", "Githyanki"                                  },
    { "Object",    RACE_WATERBREATH | RACE_MUTE,           3, 3, 0, 0, 0, 3,
	0, 0, 0, -1, -1, "swing", ""                                         },
    { "Mist",      RACE_FLY | RACE_PASSDOOR | RACE_MUTE,   2, -1, 0, 0, 3, 0,
	0, 0, 20, -1, -1, "gas", ""                                          },
    { "Snake",     RACE_MUTE,                              1, 0, 0, 0, 1, 0,
	0, 0, 0, 0, 0, "bite", ""                                            },
    { "Worm",      RACE_PASSDOOR | RACE_MUTE,              0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, "slime", ""                                           },
    { "Fish",      RACE_WATERBREATH | RACE_SWIM | RACE_MUTE,
	                                                   1, 0, 0, 0, 2, 0,
	0, 0, 0, -1, 0, "slap", ""                                           },
    { "Hydra",     RACE_DETECT_HIDDEN | RACE_MUTE,         8, 2, 0, 0, -1, 2,
	0, 0, 0, 0, 0, "bite", ""                                            },
    { "Lizard",    RACE_MUTE,                              1, -1, 0, 0, 1, 0,
	0, 0, 0, 0, 0, "lash", ""                                            },
    { "Ghost",    RACE_DETECT_HIDDEN | RACE_PASSDOOR | RACE_FLY,
    							   1, 2, 3, 3, 4, 2,
        0, 0, 0, 0, 0, "憤力一擊", ""					     }
};


/*
 * Drunkeness string changes.
 */
const   struct  struckdrunk     drunk           [  ]            =
{
    { 3, 10,
	{ "a", "a", "a", "A", "aa", "ah", "Ah", "ao", "aw", "oa", "ahhhh" }  },
    { 8, 5,
	{ "b", "b", "b", "B", "B", "vb" }                                    },
    { 3, 5,
	{ "c", "c", "C", "ch", "sj", "zj" }                                  },
    { 5, 2,
	{ "d", "d", "D" }                                                    },
    { 3, 3,
	{ "e", "e", "eh", "E" }                                              },
    { 4, 5,
	{ "f", "f", "ff", "fff", "fFf", "F" }                                },
    { 8, 3,
	{ "g", "g", "G", "jeh" }                                             },
    { 9, 6,
	{ "h", "h", "hh", "hhh", "Hhh", "HhH", "H" }                         },
    { 7, 6,
	{ "i", "i", "Iii", "ii", "iI", "Ii", "I" }                           },
    { 9, 5,
	{ "j", "j", "jj", "Jj", "jJ", "J" }                                  },
    { 7, 3,
	{ "k", "k", "K", "kah" }                                             },
    { 3, 2, 
	{ "l", "l", "L" }                                                    },
    { 5, 8,
	{ "m", "m", "mm", "mmm", "mmmm", "mmmmm", "MmM", "mM", "M" }         },
    { 6, 6,
	{ "n", "n", "nn", "Nn", "nnn", "nNn", "N" }                          },
    { 3, 6,
	{ "o", "o", "ooo", "ao", "aOoo", "Ooo", "ooOo" }                     },
    { 3, 2,
	{ "p", "p", "P" }                                                    },
    { 5, 5,
	{ "q", "q", "Q", "ku", "ququ", "kukeleku" }                          },
    { 4, 2,
	{ "r", "r", "R" }                                                    },
    { 2, 5,
	{ "s", "ss", "zzZzssZ", "ZSssS", "sSzzsss", "sSss" }                 },
    { 5, 2,
	{ "t", "t", "T" }                                                    },
    { 3, 6,
	{ "u", "u", "uh", "Uh", "Uhuhhuh", "uhU", "uhhu" }                   },
    { 4, 2,
	{ "v", "v", "V" }                                                    },
    { 4, 2,
	{ "w", "w", "W" }                                                    },
    { 5, 6,
	{ "x", "x", "X", "ks", "iks", "kz", "xz" }                           },
    { 3, 2,
	{ "y", "y", "Y" }                                                    },
    { 2, 8,
	{ "z", "z", "ZzzZz", "Zzz", "Zsszzsz", "szz", "sZZz", "ZSz", "zZ"}   }
};
      
/*
 * Attribute bonus tables.
 */
const	struct	str_app_type	str_app		[26]		=
{
    { -5, -4,   0,  0 },  /* 0  */
    { -5, -4,   3,  1 },  /* 1  */
    { -3, -2,   3,  2 },
    { -3, -1,  10,  3 },  /* 3  */
    { -2, -1,  25,  4 },
    { -2, -1,  55,  5 },  /* 5  */
    { -1,  0,  80,  6 },
    { -1,  0,  90,  7 },
    {  0,  0, 100,  8 },
    {  0,  0, 100,  9 },
    {  0,  0, 115, 10 }, /* 10  */
    {  0,  0, 115, 11 },
    {  0,  0, 140, 12 },
    {  0,  0, 140, 13 }, /* 13  */
    {  0,  1, 170, 14 },
    {  1,  1, 170, 15 }, /* 15  */
    {  1,  2, 195, 16 },
    {  2,  3, 220, 22 },
    {  2,  4, 250, 25 }, /* 18  */
    {  3,  5, 400, 30 },
    {  3,  6, 500, 35 }, /* 20  */
    {  4,  7, 600, 40 },
    {  5,  7, 700, 45 },
    {  6,  8, 800, 50 },
    {  8, 10, 900, 55 },
    { 10, 12, 999, 60 }  /* 25   */
};



const	struct	int_app_type	int_app		[26]		=
{
    {  3 },	/*  0 */
    {  5 },	/*  1 */
    {  7 },
    {  8 },	/*  3 */
    {  9 },
    { 10 },	/*  5 */
    { 11 },
    { 12 },
    { 13 },
    { 15 },
    { 17 },	/* 10 */
    { 19 },
    { 22 },
    { 25 },
    { 28 },
    { 31 },	/* 15 */
    { 34 },
    { 37 },
    { 40 },	/* 18 */
    { 44 },
    { 49 },	/* 20 */
    { 55 },
    { 60 },
    { 70 },
    { 85 },
    { 99 }	/* 25 */
};



const	struct	wis_app_type	wis_app		[26]		=
{
    { 0 },	/*  0 */
    { 0 },	/*  1 */
    { 0 },
    { 0 },	/*  3 */
    { 0 },
    { 1 },	/*  5 */
    { 1 },
    { 1 },
    { 1 },
    { 2 },
    { 2 },	/* 10 */
    { 2 },
    { 2 },
    { 2 },
    { 2 },
    { 3 },	/* 15 */
    { 3 },
    { 4 },
    { 4 },	/* 18 */
    { 5 },
    { 5 },	/* 20 */
    { 6 },
    { 7 },
    { 7 },
    { 7 },
    { 8 }	/* 25 */
};



const	struct	dex_app_type	dex_app		[26]		=
{
    {   60 },   /* 0 */
    {   50 },   /* 1 */
    {   50 },
    {   40 },
    {   30 },
    {   20 },   /* 5 */
    {   10 },
    {    0 },
    {    0 },
    {    0 },
    {    0 },   /* 10 */
    {    0 },
    {    0 },
    {    0 },
    {    0 },
    { - 10 },   /* 15 */
    { - 15 },
    { - 20 },
    { - 30 },
    { - 40 },
    { - 50 },   /* 20 */
    { - 65 },
    { - 75 },
    { - 90 },
    { -105 },
    { -120 }    /* 25 */
};



const	struct	con_app_type	con_app		[26]		=
{
    { -4, 20 },   /*  0 */
    { -3, 25 },   /*  1 */
    { -2, 30 },
    { -2, 35 },	  /*  3 */
    { -1, 40 },
    { -1, 45 },   /*  5 */
    { -1, 50 },
    {  0, 55 },
    {  0, 60 },
    {  0, 65 },
    {  0, 70 },   /* 10 */
    {  0, 75 },
    {  0, 80 },
    {  0, 85 },
    {  0, 88 },
    {  1, 90 },   /* 15 */
    {  2, 95 },
    {  2, 97 },
    {  3, 99 },   /* 18 */
    {  3, 99 },
    {  4, 99 },   /* 20 */
    {  4, 99 },
    {  5, 99 },
    {  6, 99 },
    {  7, 99 },
    {  8, 99 }    /* 25 */
};



/*
 * Liquid properties.
 * Used in world.obj.
 */
const	struct	liq_type	liq_table	[LIQ_MAX]	=
{
    { "water",			"clear",	{  0, 0, 10 }	},  /*  0 */
    { "beer",			"amber",	{  3, 2,  5 }	},
    { "wine",			"rose",		{  5, 2,  5 }	},
    { "ale",			"brown",	{  2, 2,  5 }	},
    { "dark ale",		"dark",		{  1, 2,  5 }	},

    { "whisky",			"golden",	{  6, 1,  4 }	},  /*  5 */
    { "lemonade",		"pink",		{  0, 1,  8 }	},
    { "firebreather",		"boiling",	{ 10, 0,  0 }	},
    { "local specialty",	"everclear",	{  3, 3,  3 }	},
    { "slime mold juice",	"green",	{  0, 4, -8 }	},

    { "milk",			"white",	{  0, 3,  6 }	},  /* 10 */
    { "tea",			"tan",		{  0, 1,  6 }	},
    { "coffee",			"black",	{  0, 1,  6 }	},
    { "blood",			"red",		{  0, 2, -1 }	},
    { "salt water",		"clear",	{  0, 1, -2 }	},

    { "cola",			"cherry",	{  0, 1,  5 }	}   /* 15 */
};



