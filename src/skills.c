/**************************************************************************
 *    ------\        ||                              * The Unknown world- *
 *     |     \       ||                              *  Delanta v0.3      *
 *     |     |       ||                              * Created by Doraemon*
 *     |     |       ||                |             * (C)                   *
 *     |     / /---\ || /---\ \ /--\ --|-- /---\     *                    *
 *     |    /  |---/ || |   |  |   |   |   |   |     *                    *
 *    -----/   \___  || \___\_ |   |   \_| \___\_    *                    *
 **************************************************************************/
 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <fcntl.h>
#include <ctype.h>
#include "merc.h"

#if defined(KEY)
#undef KEY
#endif

#define KEY( literal, field, value )					\
				if ( !str_cmp( word, literal ) )	\
				{					\
				    field  = value;			\
				    fMatch = TRUE;			\
				    break;				\
				}
				
struct	new_skill_type	*	skill_table;
int	MaxSkill;




				
char *gsn_to_name( int *gsn )
{
    if ( gsn == &gsn_backstab )		return "gsn_backstab";
    if ( gsn == &gsn_berserk )		return "gsn_berserk";
    if ( gsn == &gsn_breathe_water )	return "gsn_breathe_water";
    if ( gsn == &gsn_burning_hands )	return "gsn_burning_hands";
    if ( gsn == &gsn_circle )		return "gsn_circle";
    if ( gsn == &gsn_disarm )		return "gsn_disarm";
    if ( gsn == &gsn_dodge )		return "gsn_dodge";
    if ( gsn == &gsn_hide )		return "gsn_hide";
    if ( gsn == &gsn_peek )		return "gsn_peek";
    if ( gsn == &gsn_pick_lock )	return "gsn_pick_lock";
    if ( gsn == &gsn_poison_weapon )	return "gsn_poison_weapon";
    if ( gsn == &gsn_scrolls )		return "gsn_scrolls";
    if ( gsn == &gsn_snare )		return "gsn_snare";
    if ( gsn == &gsn_sneak )		return "gsn_sneak";
    if ( gsn == &gsn_staves )		return "gsn_staves";
    if ( gsn == &gsn_steal )		return "gsn_steal";
    if ( gsn == &gsn_untangle )		return "gsn_untangle";
    if ( gsn == &gsn_wands )		return "gsn_wands";
    if ( gsn == &gsn_bash )		return "gsn_bash";
    if ( gsn == &gsn_dual )		return "gsn_dual";
    if ( gsn == &gsn_enhanced_damage )	return "gsn_enhanced_damage";
    if ( gsn == &gsn_kick )		return "gsn_kick";
    if ( gsn == &gsn_parry )		return "gsn_parry";
    if ( gsn == &gsn_rescue )		return "gsn_rescue";
    if ( gsn == &gsn_second_attack )	return "gsn_second_attack";
    if ( gsn == &gsn_third_attack )	return "gsn_third_attack";
    if ( gsn == &gsn_blindness )	return "gsn_blindness";
    if ( gsn == &gsn_charm_person )	return "gsn_charm_person";
    if ( gsn == &gsn_curse )		return "gsn_curse";
    if ( gsn == &gsn_invis )		return "gsn_invis";
    if ( gsn == &gsn_mass_invis )	return "gsn_mass_invis";
    if ( gsn == &gsn_mute )		return "gsn_mute";
    if ( gsn == &gsn_poison )		return "gsn_poison";
    if ( gsn == &gsn_sleep )		return "gsn_sleep";
    if ( gsn == &gsn_turn_undead )	return "gsn_turn_undead";
    if ( gsn == &gsn_chameleon )	return "gsn_chameleon";
    if ( gsn == &gsn_domination )	return "gsn_domination";
    if ( gsn == &gsn_heighten )		return "gsn_heighten";
    if ( gsn == &gsn_shadow )		return "gsn_shadow";
    if ( gsn == &gsn_stake )		return "gsn_stake";
    if ( gsn == &gsn_vampiric_bite )	return "gsn_vampiric_bite";
    
    
    return "none_gsn";
}

int *name_to_gsn( char *arg )
{
    if ( !str_cmp( arg, "gsn_backstab" ) )	return &gsn_backstab;
    if ( !str_cmp( arg, "gsn_berserk" )	)	return &gsn_berserk;
    if ( !str_cmp( arg, "gsn_breathe_water" ) )	return &gsn_breathe_water;
    if ( !str_cmp( arg, "gsn_burning_hands" ) )	return &gsn_burning_hands;
    if ( !str_cmp( arg, "gsn_circle" ) )	return &gsn_circle;
    if ( !str_cmp( arg, "gsn_disarm" ) )	return &gsn_disarm;
    if ( !str_cmp( arg, "gsn_dodge" ) )		return &gsn_dodge;
    if ( !str_cmp( arg, "gsn_hide" ) )		return &gsn_hide;
    if ( !str_cmp( arg, "gsn_peek" ) )		return &gsn_peek;
    if ( !str_cmp( arg, "gsn_pick_lock" ) )	return &gsn_pick_lock;
    if ( !str_cmp( arg, "gsn_poison_weapon" ) )	return &gsn_poison_weapon;
    if ( !str_cmp( arg, "gsn_scrolls" ) )	return &gsn_scrolls;
    if ( !str_cmp( arg, "gsn_snare" ) )		return &gsn_snare;
    if ( !str_cmp( arg, "gsn_sneak" ) )		return &gsn_sneak;
    if ( !str_cmp( arg, "gsn_staves" ) )	return &gsn_staves;
    if ( !str_cmp( arg, "gsn_steal" ) )		return &gsn_steal;
    if ( !str_cmp( arg, "gsn_untangle" ) )	return &gsn_untangle;
    if ( !str_cmp( arg, "gsn_wands" ) )		return &gsn_wands;
    if ( !str_cmp( arg, "gsn_bash" ) )		return &gsn_bash;
    if ( !str_cmp( arg, "gsn_dual" ) )		return &gsn_dual;
    if ( !str_cmp( arg, "gsn_enhanced_damage" ) )	return &gsn_enhanced_damage;
    if ( !str_cmp( arg, "gsn_kick" ) )		return &gsn_kick;
    if ( !str_cmp( arg, "gsn_parry" ) )		return &gsn_parry;
    if ( !str_cmp( arg, "gsn_rescue" ) ) 	return &gsn_rescue;
    if ( !str_cmp( arg, "gsn_second_attack" ) )	return &gsn_second_attack;
    if ( !str_cmp( arg, "gsn_third_attack" ) )	return &gsn_third_attack;
    if ( !str_cmp( arg, "gsn_blindness" ) )	return &gsn_blindness;
    if ( !str_cmp( arg, "gsn_charm_person" ) )	return &gsn_charm_person;
    if ( !str_cmp( arg, "gsn_curse" ) )		return &gsn_curse;
    if ( !str_cmp( arg, "gsn_invis" ) )		return &gsn_invis;
    if ( !str_cmp( arg, "gsn_mass_invis" ) )	return &gsn_mass_invis;
    if ( !str_cmp( arg, "gsn_mute" ) )		return &gsn_mute;
    if ( !str_cmp( arg, "gsn_poison" ) )	return &gsn_poison;
    if ( !str_cmp( arg, "gsn_sleep" ) )		return &gsn_sleep;
    if ( !str_cmp( arg, "gsn_turn_undead" ) )	return &gsn_turn_undead;
    if ( !str_cmp( arg, "gsn_chameleon" ) )	return &gsn_chameleon;
    if ( !str_cmp( arg, "gsn_domination" ) )	return &gsn_domination;
    if ( !str_cmp( arg, "gsn_heighten" ) )	return &gsn_heighten;
    if ( !str_cmp( arg, "gsn_shadow" ) )	return &gsn_shadow;
    if ( !str_cmp( arg, "gsn_stake" ) )		return &gsn_stake;
    if ( !str_cmp( arg, "gsn_vampiric_bite" ) )	return &gsn_vampiric_bite;
    if ( !str_cmp( arg, "none_gsn" ) )		return NULL;
    
    
    return NULL;
}

SPELL_FUN *name_to_spellfun( char *arg )
{
    if ( !str_cmp( arg, "spell_acid_blast" ) )		return spell_acid_blast;
    if ( !str_cmp( arg, "spell_armor" ) )		return spell_armor;
    if ( !str_cmp( arg, "spell_bless" ) )		return spell_bless;
    if ( !str_cmp( arg, "spell_blindness" ) )		return spell_blindness;
    if ( !str_cmp( arg, "spell_breathe_water" ) )	return spell_breathe_water;
    if ( !str_cmp( arg, "spell_burning_hands" ) )	return spell_burning_hands;
    if ( !str_cmp( arg, "spell_call_lightning" ) )	return spell_call_lightning;
    if ( !str_cmp( arg, "spell_cause_critical" ) ) 	return spell_cause_critical;
    if ( !str_cmp( arg, "spell_cause_light" ) )		return spell_cause_light;
    if ( !str_cmp( arg, "spell_cause_serious" ) )	return spell_cause_serious;
    if ( !str_cmp( arg, "spell_change_sex" ) )		return spell_change_sex;
    if ( !str_cmp( arg, "spell_charm_person" ) )	return spell_charm_person;
    if ( !str_cmp( arg, "spell_chill_touch" ) )		return spell_chill_touch;
    if ( !str_cmp( arg, "spell_colour_spray" ) )	return spell_colour_spray;
    if ( !str_cmp( arg, "spell_cone_of_silence" ) )	return spell_cone_of_silence;
    if ( !str_cmp( arg, "spell_continual_light" ) )	return spell_continual_light;
    if ( !str_cmp( arg, "spell_control_weather" ) )	return spell_control_weather;
    if ( !str_cmp( arg, "spell_create_food" ) )		return spell_create_food;
    if ( !str_cmp( arg, "spell_create_spring" ) )	return spell_create_spring;
    if ( !str_cmp( arg, "spell_create_water" ) )	return spell_create_water;
    if ( !str_cmp( arg, "spell_cure_blindness" ) )	return spell_cure_blindness;
    if ( !str_cmp( arg, "spell_cure_critical" ) )	return spell_cure_critical;
    if ( !str_cmp( arg, "spell_cure_light" ) )		return spell_cure_light;
    if ( !str_cmp( arg, "spell_cure_poison" ) )		return spell_cure_poison;
    if ( !str_cmp( arg, "spell_cure_serious" ) )	return spell_cure_serious;
    if ( !str_cmp( arg, "spell_curse" ) )		return spell_curse;
    if ( !str_cmp( arg, "spell_destroy_cursed" ) )	return spell_destroy_cursed;
    if ( !str_cmp( arg, "spell_detect_evil" ) )		return spell_detect_evil;
    if ( !str_cmp( arg, "spell_detect_hidden" ) )	return spell_detect_hidden;
    if ( !str_cmp( arg, "spell_detect_invis" ) )	return spell_detect_invis;
    if ( !str_cmp( arg, "spell_detect_magic" ) )	return spell_detect_magic;
    if ( !str_cmp( arg, "spell_detect_poison" ) )	return spell_detect_poison;
    if ( !str_cmp( arg, "spell_dispel_evil" ) )		return spell_dispel_evil;
    if ( !str_cmp( arg, "spell_dispel_magic" ) )	return spell_dispel_magic;
    if ( !str_cmp( arg, "spell_earthquake" ) )		return spell_earthquake;
    if ( !str_cmp( arg, "spell_energy_drain" ) )	return spell_energy_drain;
    if ( !str_cmp( arg, "spell_exorcise" ) )		return spell_exorcise;
    if ( !str_cmp( arg, "spell_faerie_fire" ) )		return spell_faerie_fire;
    if ( !str_cmp( arg, "spell_faerie_fog" ) )		return spell_faerie_fog;
    if ( !str_cmp( arg, "spell_fireball" ) )		return spell_fireball;
    if ( !str_cmp( arg, "spell_flamestrike" ) )		return spell_flamestrike;
    if ( !str_cmp( arg, "spell_flaming" ) )		return spell_flaming;
    if ( !str_cmp( arg, "spell_fly" ) )			return spell_fly;
    if ( !str_cmp( arg, "spell_gate" ) )		return spell_gate;
    if ( !str_cmp( arg, "spell_giant_strength" ) )	return spell_giant_strength;
    if ( !str_cmp( arg, "spell_harm" ) )		return spell_harm;
    if ( !str_cmp( arg, "spell_heal" ) )		return spell_heal;
    if ( !str_cmp( arg, "spell_identify" ) )		return spell_identify;
    if ( !str_cmp( arg, "spell_infravision" ) )		return spell_infravision;
    if ( !str_cmp( arg, "spell_invis" ) ) 		return spell_invis;
    if ( !str_cmp( arg, "spell_know_alighment" ) )	return spell_know_alignment;
    if ( !str_cmp( arg, "spell_lightning_bolt" ) )	return spell_lightning_bolt;
    if ( !str_cmp( arg, "spell_locate_object" ) )	return spell_locate_object;
    if ( !str_cmp( arg, "spell_magic_missile" ) )	return spell_magic_missile;
    if ( !str_cmp( arg, "spell_mass_heal" ) )		return spell_mass_heal;
    if ( !str_cmp( arg, "spell_mass_invis" ) )		return spell_mass_invis;
    if ( !str_cmp( arg, "spell_mute" ) )		return spell_mute;
    if ( !str_cmp( arg, "spell_pass_door" ) )		return spell_pass_door;
    if ( !str_cmp( arg, "spell_poison" ) )		return spell_poison;
    if ( !str_cmp( arg, "spell_polymorph_other" ) )	return spell_polymorph_other;
    if ( !str_cmp( arg, "spell_protection" ) )		return spell_protection;
    if ( !str_cmp( arg, "spell_recharge_item" ) )	return spell_recharge_item;
    if ( !str_cmp( arg, "spell_refresh" ) )		return spell_refresh;
    if ( !str_cmp( arg, "spell_remove_alignment" ) )	return spell_remove_alignment;
    if ( !str_cmp( arg, "spell_remove_curse" ) )	return spell_remove_curse;
    if ( !str_cmp( arg, "spell_remove_silence" ) )	return spell_remove_silence;
    if ( !str_cmp( arg, "spell_sanctuary" ) )		return spell_sanctuary;
    if ( !str_cmp( arg, "spell_shield" ) )		return spell_shield;
    if ( !str_cmp( arg, "spell_shocking_grasp" ) )	return spell_shocking_grasp;
    if ( !str_cmp( arg, "spell_sleep" ) )		return spell_sleep;
    if ( !str_cmp( arg, "spell_stone_skin" ) )		return spell_stone_skin;
    if ( !str_cmp( arg, "spell_summon" ) )		return spell_summon;
    if ( !str_cmp( arg, "spell_teleport" ) )		return spell_teleport;
    if ( !str_cmp( arg, "spell_turn_undead" ) )		return spell_turn_undead;
    if ( !str_cmp( arg, "spell_ventriloquate" ) )	return spell_ventriloquate;
    if ( !str_cmp( arg, "spell_weaken" ) ) 		return spell_weaken;
    if ( !str_cmp( arg, "spell_word_of_recall" ) )	return spell_word_of_recall;
    
    if ( !str_cmp( arg, "spell_acid_breath" ) )		return spell_acid_breath;
    if ( !str_cmp( arg, "spell_fire_breath" ) )		return spell_fire_breath;
    if ( !str_cmp( arg, "spell_frost_breath" ) )	return spell_frost_breath;
    if ( !str_cmp( arg, "spell_gas_breath" ) )		return spell_gas_breath;
    if ( !str_cmp( arg, "spell_lightning_breath" ) )	return spell_lightning_breath;
    
    if ( !str_cmp( arg, "spell_null" ) )		return spell_null;
    if ( !str_cmp( arg, "reserved" ) )			return spell_null;
    
    if ( !str_cmp( arg, "spell_general_purpose" ) )	return spell_general_purpose;
    if ( !str_cmp( arg, "spell_high_explosive" ) )	return spell_high_explosive;
    
    if ( !str_cmp( arg, "spell_adrenaline_control" ) )	return spell_adrenaline_control;
    if ( !str_cmp( arg, "spell_agitation" ) )		return spell_agitation;
    if ( !str_cmp( arg, "spell_aura_sight" ) )		return spell_aura_sight;
    if ( !str_cmp( arg, "spell_awe" ) )			return spell_awe;
    if ( !str_cmp( arg, "spell_ballistic_attack" ) )	return spell_ballistic_attack;
    if ( !str_cmp( arg, "spell_biofeedback" ) )		return spell_biofeedback;
    if ( !str_cmp( arg, "spell_cell_adjustment" ) )	return spell_cell_adjustment;
    if ( !str_cmp( arg, "spell_combat_mind" ) )		return spell_combat_mind;
    if ( !str_cmp( arg, "spell_complete_healing" ) )	return spell_complete_healing;
    if ( !str_cmp( arg, "spell_control_flames" ) )	return spell_control_flames;
    if ( !str_cmp( arg, "spell_create_sound" ) )	return spell_create_sound;
    if ( !str_cmp( arg, "spell_death_field" ) )		return spell_death_field;
    if ( !str_cmp( arg, "spell_detonate" ) )		return spell_detonate;
    if ( !str_cmp( arg, "spell_disintegrate" ) )	return spell_disintegrate;
    if ( !str_cmp( arg, "spell_displacement" ) )	return spell_displacement;
    if ( !str_cmp( arg, "spell_domination" ) )		return spell_domination;
    if ( !str_cmp( arg, "spell_ectoplasmic_form" ) )	return spell_ectoplasmic_form;
    if ( !str_cmp( arg, "spell_ego_whip" ) )		return spell_ego_whip;
    if ( !str_cmp( arg, "spell_energy_containment" ) )	return spell_energy_containment;
    if ( !str_cmp( arg, "spell_enhance_armor" ) )	return spell_enhance_armor;
    if ( !str_cmp( arg, "spell_enhanced_strength" ) )	return spell_enhanced_strength;
    if ( !str_cmp( arg, "spell_flesh_armor" ) )		return spell_flesh_armor;
    if ( !str_cmp( arg, "spell_inertial_barrier" ) )	return spell_inertial_barrier;
    if ( !str_cmp( arg, "spell_inflict_pain" ) )	return spell_inflict_pain;
    if ( !str_cmp( arg, "spell_intellect_fortress" ) )	return spell_intellect_fortress;
    if ( !str_cmp( arg, "spell_lend_health" ) )		return spell_lend_health;
    if ( !str_cmp( arg, "spell_levitation" ) )		return spell_levitation;
    if ( !str_cmp( arg, "spell_mental_barrier" ) )	return spell_mental_barrier;
    if ( !str_cmp( arg, "spell_mind_thrust" ) )		return spell_mind_thrust;
    if ( !str_cmp( arg, "spell_project_force" ) )	return spell_project_force;
    if ( !str_cmp( arg, "spell_psionic_blast" ) )	return spell_psionic_blast;
    if ( !str_cmp( arg, "spell_psychic_crush" ) )	return spell_psychic_crush;
    if ( !str_cmp( arg, "spell_psychic_drain" ) )	return spell_psychic_drain;
    if ( !str_cmp( arg, "spell_psychic_healing" ) )	return spell_psychic_healing;
    if ( !str_cmp( arg, "spell_share_strength" ) )	return spell_share_strength;
    if ( !str_cmp( arg, "spell_thought_shield" ) )	return spell_thought_shield;
    if ( !str_cmp( arg, "spell_ultrablast" ) )		return spell_ultrablast;
    if ( !str_cmp( arg, "spell_vampiric_bite" ) )	return spell_vampiric_bite;

    if ( !str_cmp( arg, "spell_smash" ) )		return spell_smash;    
    return spell_null;
}    
    
    

char *spell_to_name( SPELL_FUN *spell )
{
    if ( spell == spell_acid_blast )		return "spell_acid_blast";
    if ( spell == spell_armor )			return "spell_armor";
    if ( spell == spell_bless )			return "spell_bless";
    if ( spell == spell_blindness )		return "spell_blindness";
    if ( spell == spell_breathe_water )		return "spell_breathe_water";
    if ( spell == spell_burning_hands )		return "spell_burning_hands";
    if ( spell == spell_call_lightning )	return "spell_call_lightning";
    if ( spell == spell_cause_critical )	return "spell_cause_critical";
    if ( spell == spell_cause_light )		return "spell_cause_light";
    if ( spell == spell_cause_serious )		return "spell_cause_serious";
    if ( spell == spell_change_sex )		return "spell_changed_sex";
    if ( spell == spell_charm_person )		return "spell_charm_person";
    if ( spell == spell_chill_touch )		return "spell_chill_touch";
    if ( spell == spell_colour_spray )		return "spell_colour_spray";
    if ( spell == spell_cone_of_silence )	return "spell_cone_of_silence";
    if ( spell == spell_continual_light )	return "spell_continual_light";
    if ( spell == spell_control_weather )	return "spell_control_weather";
    if ( spell == spell_create_food )		return "spell_create_food";
    if ( spell == spell_create_spring )		return "spell_create_spring";
    if ( spell == spell_create_water )		return "spell_create_water";
    if ( spell == spell_cure_blindness )	return "spell_cure_blindness";
    if ( spell == spell_cure_critical )		return "spell_cure_critical";
    if ( spell == spell_cure_light )		return "spell_cure_light";
    if ( spell == spell_cure_poison )		return "spell_cure_poison";
    if ( spell == spell_cure_serious )		return "spell_cure_serious";
    if ( spell == spell_curse )			return "spell_curse";
    if ( spell == spell_destroy_cursed )	return "spell_destroy_cursed";
    if ( spell == spell_detect_evil )		return "spell_detect_evil";
    if ( spell == spell_detect_hidden )		return "spell_detect_hidden";
    if ( spell == spell_detect_invis )		return "spell_detect_invis";
    if ( spell == spell_detect_magic )		return "spell_detect_magic";
    if ( spell == spell_detect_poison )		return "spell_detect_poison";
    if ( spell == spell_dispel_evil )		return "spell_dispel_evil";
    if ( spell == spell_dispel_magic )		return "spell_dispel_magic";
    if ( spell == spell_earthquake )		return "spell_earthquake";
    if ( spell == spell_enchant_weapon )	return "spell_enchant_weapon";
    if ( spell == spell_energy_drain )		return "spell_energy_drain";
    if ( spell == spell_exorcise )		return "spell_exorcise";
    if ( spell == spell_faerie_fire )		return "spell_faerie_fire";
    if ( spell == spell_faerie_fog )		return "spell_faerie_fog";
    if ( spell == spell_fireball )		return "spell_fireball";
    if ( spell == spell_flamestrike )		return "spell_flamestrike";
    if ( spell == spell_flaming )		return "spell_flaming";
    if ( spell == spell_fly )			return "spell_fly";
    if ( spell == spell_gate )			return "spell_gate";
    if ( spell == spell_giant_strength )	return "spell_giant_strength";
    if ( spell == spell_harm )			return "spell_harm";
    if ( spell == spell_heal )			return "spell_heal";
    if ( spell == spell_identify )		return "spell_identify";
    if ( spell == spell_infravision )		return "spell_infravision";
    if ( spell == spell_invis )			return "spell_invis";
    if ( spell == spell_know_alignment )	return "spell_know_alignment";
    if ( spell == spell_lightning_bolt )	return "spell_lightning_bolt";
    if ( spell == spell_locate_object )		return "spell_locate_object";
    if ( spell == spell_magic_missile )		return "spell_magic_missile";
    if ( spell == spell_mass_heal )		return "spell_mass_heal";
    if ( spell == spell_mass_invis )		return "spell_mass_invis";
    if ( spell == spell_mute )			return "spell_mute";
    if ( spell == spell_pass_door )		return "spell_pass_door";
    if ( spell == spell_poison )		return "spell_poison";
    if ( spell == spell_polymorph_other )	return "spell_polymorph_other";
    if ( spell == spell_protection )		return "spell_protection";
    if ( spell == spell_recharge_item )		return "spell_recharge_item";
    if ( spell == spell_refresh )		return "spell_refresh";
    if ( spell == spell_remove_alignment )	return "spell_remove_alignment";
    if ( spell == spell_remove_curse )		return "spell_remove_curse";
    if ( spell == spell_remove_silence )	return "spell_remove_silence";
    if ( spell == spell_sanctuary )		return "spell_sanctuary";
    if ( spell == spell_shield )		return "spell_shield";
    if ( spell == spell_shocking_grasp )	return "spell_shocking_grasp";
    if ( spell == spell_sleep )			return "spell_sleep";
    if ( spell == spell_stone_skin )		return "spell_stone_skin";
    if ( spell == spell_summon )		return "spell_summon";
    if ( spell == spell_teleport )		return "spell_teleport";
    if ( spell == spell_turn_undead )		return "spell_turn_undead";
    if ( spell == spell_ventriloquate )		return "spell_ventriloquate";
    if ( spell == spell_weaken )		return "spell_weaken";
    if ( spell == spell_word_of_recall )	return "spell_word_of_recall";
    
    if ( spell == spell_acid_breath )		return "spell_acid_breath";
    if ( spell == spell_fire_breath )		return "spell_fire_breath";
    if ( spell == spell_frost_breath )		return "spell_frost_breath";
    if ( spell == spell_gas_breath )		return "spell_gas_breath";
    if ( spell == spell_lightning_breath )	return "spell_lightning_breath";
    
    if ( spell == spell_null )			return "spell_null";
    
    if ( spell == spell_general_purpose )	return "spell_general_purpose";
    if ( spell == spell_high_explosive )	return "spell_high_explosive";
    
    if ( spell == spell_adrenaline_control )	return "spell_adrenaline_control";
    if ( spell == spell_agitation )		return "spell_agitation";
    if ( spell == spell_aura_sight )		return "spell_aura_sight";
    if ( spell == spell_awe )			return "spell_awe";
    if ( spell == spell_ballistic_attack )	return "spell_ballistic_attack";
    if ( spell == spell_biofeedback )		return "spell_biofeedback";
    if ( spell == spell_cell_adjustment )	return "spell_cell_adjustment";
    if ( spell == spell_combat_mind )		return "spell_combat_mind";
    if ( spell == spell_complete_healing )	return "spell_complete_healing";
    if ( spell == spell_control_flames )	return "spell_control_flames";
    if ( spell == spell_create_sound )		return "spell_create_sound";
    if ( spell == spell_death_field )		return "spell_death_field";
    if ( spell == spell_detonate )		return "spell_detonate";
    if ( spell == spell_disintegrate )		return "spell_disintegrate";
    if ( spell == spell_displacement )		return "spell_displacement";
    if ( spell == spell_domination )		return "spell_domination";
    if ( spell == spell_ectoplasmic_form )	return "spell_ectoplasmic_form";
    if ( spell == spell_ego_whip )		return "spell_ego_whip";
    if ( spell == spell_energy_containment )	return "spell_energy_containment";
    if ( spell == spell_enhance_armor )		return "spell_enhance_armor";
    if ( spell == spell_enhanced_strength )	return "spell_enhanced_strength";
    if ( spell == spell_flesh_armor )		return "spell_flesh_armor";
    if ( spell == spell_inertial_barrier )	return "spell_inertial_barrier";
    if ( spell == spell_inflict_pain )		return "spell_inflict_pain";
    if ( spell == spell_intellect_fortress )	return "spell_intellect_fortress";
    if ( spell == spell_lend_health )		return "spell_lend_health";
    if ( spell == spell_levitation )		return "spell_levitation";
    if ( spell == spell_mental_barrier )	return "spell_mental_barrier";
    if ( spell == spell_mind_thrust )		return "spell_mind_thrust";
    if ( spell == spell_project_force )		return "spell_project_force";
    if ( spell == spell_psionic_blast )		return "spell_psionic_blast";
    if ( spell == spell_psychic_crush )		return "spell_psychic_crush";
    if ( spell == spell_psychic_drain )		return "spell_psychic_drain";
    if ( spell == spell_psychic_healing )	return "spell_psychic_healing";
    if ( spell == spell_share_strength )	return "spell_share_strength";
    if ( spell == spell_thought_shield )	return "spell_thought_shield";
    if ( spell == spell_ultrablast )		return "spell_ultrablast";
    if ( spell == spell_vampiric_bite )		return "spell_vampiric_bite";                        
    if ( spell == spell_smash )			return "spell_smash";
            
    
    return "reserved";
}

char *target_to_name( int target )
{
    if ( target == TAR_IGNORE )	return "TAR_IGNORE";
    if ( target == TAR_CHAR_OFFENSIVE )	return "TAR_CHAR_OFFENSIVE";
    if ( target == TAR_CHAR_DEFENSIVE )	return "TAR_CHAR_DEFENSIVE";
    if ( target == TAR_CHAR_SELF )	return "TAR_CHAR_SELF";
    if ( target == TAR_OBJ_INV )	return "TAR_OBJ_INV";
    
    return "TAR_IGNORE";
}

int name_to_target( char *arg )
{
    if ( !str_cmp( arg, "TAR_IGNORE" ) )		return TAR_IGNORE;
    if ( !str_cmp( arg, "TAR_CHAR_OFFENSIVE" ) )	return TAR_CHAR_OFFENSIVE;
    if ( !str_cmp( arg, "TAR_CHAR_DEFENSIVE" ) )	return TAR_CHAR_DEFENSIVE;
    if ( !str_cmp( arg, "TAR_CHAR_SELF" ) )		return TAR_CHAR_SELF;
    if ( !str_cmp( arg, "TAR_OBJ_INV" ) )		return TAR_OBJ_INV;
    
    return TAR_IGNORE;
}

char *position_to_name( int pos )
{
    if ( pos == POS_DEAD )	return "POS_DEAD";
    if ( pos == POS_MORTAL )	return "POS_MORTAL";
    if ( pos == POS_INCAP )	return "POS_INCAP";
    if ( pos == POS_STUNNED )	return "POS_STUNNED";
    if ( pos == POS_SLEEPING )	return "POS_SLEEPING";
    if ( pos == POS_RESTING )	return "POS_RESTING";
    if ( pos == POS_FIGHTING )	return "POS_FIGHTING";
    if ( pos == POS_STANDING )	return "POS_STANDING";
    
    return "POS_DEAD";
}

int name_to_position( char *argu )
{
    if ( !str_cmp( argu, "POS_DEAD" ) )		return POS_DEAD;
    if ( !str_cmp( argu, "POS_MORTAL" ) )	return POS_MORTAL;
    if ( !str_cmp( argu, "POS_INCAP" ) )	return POS_INCAP;
    if ( !str_cmp( argu, "POS_STUNNED" ) )	return POS_STUNNED;
    if ( !str_cmp( argu, "POS_SLEEPING" ) )	return POS_SLEEPING;
    if ( !str_cmp( argu, "POS_RESTING" ) )	return POS_RESTING;
    if ( !str_cmp( argu, "POS_FIGHTING" ) )	return POS_FIGHTING;
    if ( !str_cmp( argu, "POS_STANDING" ) )	return POS_STANDING;
    
    return POS_DEAD;
}


void fwrite_skill( OLD_SKILL_TYPE skill, FILE *fp )
{
    fprintf( fp, "#SKILL\n" );
    fprintf( fp, "Name      %s~\n" , skill.name );
    fprintf( fp, "CName     %s~\n", skill.name );
    fprintf( fp, "Class     %d\n", 0 );
    fprintf( fp, "SplFun    %s~\n", spell_to_name( skill.spell_fun ) );
    fprintf( fp, "Target    %s~\n", target_to_name( skill.target ) );
    fprintf( fp, "MinPos    %s~\n", position_to_name ( skill.minimum_position ) );
    fprintf( fp, "Gsn       %s~\n", gsn_to_name( skill.pgsn ) );
    fprintf( fp, "MinMana   %d\n", skill.min_mana );
    fprintf( fp, "Beats     %d\n", skill.beats );
    fprintf( fp, "DamMsg    %s~\n", skill.noun_damage );
    fprintf( fp, "WearOff   %s~\n", skill.msg_off );
    
    fprintf( fp, "End\n\n" );
    
    if ( ferror( fp ) )
    	bug( "Something wrong on fwrite_skill. %d", ferror( fp ) );
    
    return;
}



    
void do_save_skill( CHAR_DATA *ch, char *argument )
{   
    FILE *skill_fp;
    char	log_buf [ MAX_STRING_LENGTH ];
    int		i;
    
    
    if ( !( skill_fp = fopen( SKILL_FILE, "w" ) ) )
    {
        strcpy( log_buf, "Àx¦s§Þ¯àÀÉ®× skills.dat µo¥Í»ÙÃª¡C" );
        bug( log_buf, 0 );
        perror( SKILL_FILE );
    }

    fprintf( skill_fp, "MAX_SKILL      %d\n\n", MAX_SKILL );
    
    for ( i = 0; i < MAX_SKILL; i++ )
/*        fwrite_skill( skill_table[i], skill_fp )*/;

    fprintf( skill_fp, "#END~" );
    
    fclose( skill_fp );
    
    send_to_char( "Àx¦s§Þ¯àÀÉ®× SKILLS.TXT §¹¦¨¡C\n\r", ch );
    
}

void fread_skill( FILE *fp, struct new_skill_type *skill )
{
    char buf[MAX_STRING_LENGTH];
    char *word;
    char letter;
    bool fMatch;
    int stat;
    
    letter = fread_letter( fp );
    if ( letter != '#' )
    {
        bug( "¦bSKILLªºÀÉ®×¸Ì§ä¤£¨ì # ¸¹¡C", 0 );
        exit( 1 );
    }
    
    word = fread_word( fp, &stat );
    if ( str_cmp( word, "SKILL" ) )
    {
        bug( "¦bSKILLªºÀÉ®×¸Ì§ä¤£¨ì SKILL ³o­Ó¦r¡C", 0 );
        
        exit( 1 );
    }    
    
    
    for ( ; ; )
    {
	word   = feof( fp ) ? "End" : fread_word( fp, &stat );
	fMatch = FALSE;

	switch ( UPPER(word[0]) )
	{
	case '*':
	    fMatch = TRUE;
	    fread_to_eol( fp );
	    break;
	    
	case 'B':
	    KEY( "Beats",	skill->beats,	fread_number( fp, &stat ) );
	    break;

	case 'C':
	    KEY( "Class",	skill->for_which_class, fread_number( fp, &stat ) );
            KEY( "CName",	skill->cname, fread_string( fp, &stat ) );	    
            break;
 
	case 'D':
            KEY( "DamMsg",	skill->noun_damage,	fread_string( fp, &stat ) );
	    break;

	case 'E':
	    if ( !str_cmp( word, "End" ) )
		return;
	    break;
	    
	case 'G':
	    if ( !str_cmp( word, "Gsn" ) )
	    {
	         int *pointer = name_to_gsn( fread_string( fp, &stat ) );
	         
	         fMatch = TRUE;
	         if ( pointer )
     	             *pointer = *skill->pgsn;
     	         break;
	    }
	    break;

	case 'M':
	    KEY( "MinPos",	skill->minimum_position, (int)name_to_position( fread_string( fp, &stat ) ) );
	    KEY( "MinMana",	skill->min_mana,	fread_number( fp, &stat ) );
	    break;
	
	case 'N':
            KEY( "Name",	skill->name,		fread_string( fp, &stat ) );
	    break;

	case 'S':
	    KEY( "SplFun",	skill->spell_fun,	(SPELL_FUN *)name_to_spellfun( fread_string( fp, &stat ) ) );
	    break;

	case 'T':
	    KEY( "Target",	skill->target,		(int)name_to_target( fread_string( fp, &stat ) ) );
	    break;

	case 'W':
	    KEY( "WearOff",	skill->msg_off,		fread_string( fp, &stat ) );
	    break;
	}
	
	if ( !fMatch )
	{
            sprintf( buf, "Fread_skill: no match: %s", word );
	    bug( buf, 0 );
	}
    }
    return;
}        
    
        
    
void load_skill_table(void)
{
    FILE *fp;
    int i, stat;
    
    
    if ( !( fp = fopen( SKILL_FILE, "r" ) ) )
    {
        strcpy( log_buf, "Åª¨ú§Þ¯àÀÉ®× SKILLS.TXT µo¥Í»ÙÃª¡C" );
        bug( log_buf, 0 );
        perror( SKILL_FILE );
    }
    
    fread_word( fp, &stat );
    fscanf( fp, "%d\n\n", &MaxSkill );
    
    skill_table = alloc_perm( sizeof(struct new_skill_type) * ( MaxSkill + 1 ) );

    log_string( "  Assigning skill number.." );
    for ( i = 0; i < MaxSkill; i++ )
        skill_table[i].pgsn = &i;

    log_string( "  Read in skill..." );
    for ( i = 0; i < MaxSkill; i++ )
        fread_skill( fp, &skill_table[i] );
            
    if ( str_cmp( fread_string( fp, &stat ), "#END" ) )
        bug( "load_skill_table: Cannot find #END in skill file.", 0 );
        
    skill_table[MaxSkill].name = str_dup( "" );
    fclose( fp );
    
    return;
}

    
void do_join( CHAR_DATA *ch, char *argument )
{
    bool fMatch = FALSE;
    bool fMobIn = FALSE;
    CHAR_DATA *join_mob;
    char buf [ MAX_STRING_LENGTH ];

    if ( IS_NPC( ch ) )
    {
        send_to_char( "§A·Q¦³Â¾·~??·Q±o¬ü°Ú!!\n\r", ch );
        return;
    }
    
    if ( strlen( argument ) < 3 && ch->class == CLASS_NONE )
    {
        int i;
        
        send_to_char( "Â¾·~ªº¦r¤¸¦Ü¤Ö¦³¤T­Ó¦r¡C¨Ã½Ð¿é¤J­^¤å¡C\n\r§A¥i¥H¥[¤J¤U­±ªºÂ¾·~¡G\n\r", ch );
        for ( i = 0; i < MAX_CLASS; i++ )
        {
            if ( !class_table[i].fAdvance && i != CLASS_NONE )
            {
                sprintf( buf, "    [1;36m%s  [1;33m%s[0;37;40m\n\r", class_table[i].who_name, class_table[i].chinese );
                send_to_char( buf, ch );
            }
        }
        send_to_char( "\n\r½Ð¾Ü¤@¦Ó¤J¤§¡C\n\r", ch );
        return;
    }
    else if ( strlen( argument ) < 3 )
    {
        send_to_char( "Â¾·~ªº¦r¤¸¦Ü¤Ö­n¦³¤T­Ó¦r¡C\n\r", ch );
        return;
    }    
    
    argument[0] = UPPER( argument[0] );
    
    if ( ch->class == CLASS_NONE )
    {
        int iClass;
        
        for ( iClass = 0; iClass < MAX_CLASS; iClass++ )
            if ( !str_cmp( argument, class_table[iClass].who_name ) && !class_table[iClass].fAdvance )
            {
                fMatch = TRUE;
                break;
            }
            
        for ( join_mob = ch->in_room->people; join_mob; join_mob = join_mob->next_in_room )
        {
             switch ( iClass )
             {
                 case CLASS_MAGE:
                     if ( IS_NPC( join_mob ) 
                         && IS_SET( join_mob->act, ACT_JOIN_MAG ) )
                         fMobIn = TRUE;
                     break;
                 case CLASS_THIEF:
                     if ( IS_NPC( join_mob ) 
                         && IS_SET( join_mob->act, ACT_JOIN_THI ) )
                         fMobIn = TRUE;
                     break;                         
                 case CLASS_WARRIOR:
                     if ( IS_NPC( join_mob ) 
                         && IS_SET( join_mob->act, ACT_JOIN_WAR ) )
                         fMobIn = TRUE;
                     break;                                    
                 case CLASS_PHYSICALIST:
                     if ( IS_NPC( join_mob )
                         && IS_SET( join_mob->act, ACT_JOIN_PHY ) )
                         fMobIn = TRUE;
                     break;
                 case CLASS_CHEMICALIST:
                     if ( IS_NPC( join_mob )
                         && IS_SET( join_mob->act, ACT_JOIN_CHE ) )
                         fMobIn = TRUE;
                     break;
                 case CLASS_BIOLOGIST:
                     if ( IS_NPC( join_mob )
                         && IS_SET( join_mob->act, ACT_JOIN_BIO ) )
                         fMobIn = TRUE;
                     break;
                 case CLASS_HERCULES:
                     if ( IS_NPC( join_mob )
                         && IS_SET( join_mob->act, ACT_JOIN_HER ) )
                         fMobIn = TRUE;
                     break;
                 default: 
                     fMobIn = FALSE;
                     break;
             }
        }          
        
        if ( fMatch && fMobIn && ch->class != iClass )
        {
            ch->class = iClass;
	    switch ( class_table[ch->class].attr_prime )
	    {
	    case APPLY_STR: ch->pcdata->perm_str = 22; break;
	    case APPLY_INT: ch->pcdata->perm_int = 22; break;
	    case APPLY_WIS: ch->pcdata->perm_wis = 22; break;
	    case APPLY_DEX: ch->pcdata->perm_dex = 22; break;
	    case APPLY_CON: ch->pcdata->perm_con = 22; break;
	    }            
            sprintf( buf, "§A¥[¤J¤F%s³o­ÓÂ¾·~¡C\n\r", class_table[iClass].chinese );
            send_to_char( buf, ch );
            return;
        }
        else if ( !fMobIn )
        {
            send_to_char( "³o¸Ì¨S¦³¤H¥i¥H§ïÅÜ§AªºÂ¾·~¡C\n\r", ch );
            return;
        }
        else 
        {
            send_to_char( "§ä¤£¨ì¨ººØÂ¾·~!!\n\r", ch );
            return;
        }
    }
    
    send_to_char( "§A¨S¦³¸ê®æ´«Â¾·~!!\n\r", ch );
    return;
}
        
int can_learn( CHAR_DATA *ch, int sn )
{
    int confirm_bit;
    
    if ( IS_NPC( ch ) )
    	return FALSE;
            
    switch ( ch->class )
    {
        case CLASS_MAGE:
            confirm_bit = SKILL_ACT_MAG;
            break;
        case CLASS_WARRIOR:
            confirm_bit = SKILL_ACT_WAR;
            break;
        case CLASS_THIEF:
            confirm_bit = SKILL_ACT_THI;
            break;
        case CLASS_CLERIC:
            confirm_bit = SKILL_ACT_CLE;
            break;
        case CLASS_PSIONICIST:
            confirm_bit = SKILL_ACT_PSI;
            break;
        default:
            confirm_bit = SKILL_ACT_NONE;
    }
    
    return IS_SET( skill_table[sn].for_which_class, confirm_bit );
}        

void do_reloadskill( CHAR_DATA *ch, char *argument )
{
    if ( !authorized( ch, "load_skill" ) || ch->level < L_DIR )
    	return;
    	
    load_skill_table( );
    send_to_char( "§Þ¯à°t¸mªí­«·sÅª¹L¤F¡C", ch );
    
    return;
}

    
void do_skills( CHAR_DATA *ch, char *argument )
{
    char buf [ MAX_STRING_LENGTH ];
    char buf1 [ MAX_STRING_LENGTH ];
    int sn;

    if ( IS_NPC( ch ) )
    	return;
    
    buf[0] = '\0';
    buf1[0] = '\0';
    	
    strcpy( buf, "§A¾Ç¹Lªº§Þ¯à¡G\n\r" );
    	
    for ( sn = 0; sn < MaxSkill; sn++ )
    {
        int number;
        
        if ( !skill_table[sn].name || !str_cmp( skill_table[sn].name, "(null)" ) || !skill_table[sn].cname )
            break;

        if ( IS_LEARNED( ch, sn ) && ( ch->pcdata ) )
        {
            number = ch->pcdata->learned[sn];
            
            if ( ( number >= 0 )&&(number <= 100))
            {
                sprintf( buf1, "[0;36m%18s  [1;34m%18s[m  %d%%  ¸gÅç­È %d\n\r", skill_table[sn].name, skill_table[sn].cname, ch->pcdata->learned[sn], ch->skill_level[sn] );
                strcat( buf, buf1 );
            }
        }
    }
    
    send_to_char( buf, ch );
    
    return;
}            
             
             
            
    
    
        
