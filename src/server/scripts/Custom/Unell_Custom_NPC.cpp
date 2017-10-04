#include "ScriptPCH.h"
#include "Chat.h"
#include "AccountMgr.h"
#include "ScriptMgr.h"
#include "Common.h"
#include "Player.h"
#include "WorldSession.h"
#include "Language.h"
#include "Log.h"
#include "SpellAuras.h"
#include "World.h"
#include "Config.h"
#include "SystemConfig.h"
#include "revision.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "GitRevision.h"
#include "TicketMgr.h"
#include "Pet.h"

#define GOSSIP_ITEM_STABLE "Stable"
#define GOSSIP_ITEM_NEWPET "New Pet"
#define GOSSIP_ITEM_BOAR "Boar"
#define GOSSIP_ITEM_SERPENT "Serpent"
#define GOSSIP_ITEM_SCRAB "Scrab"
#define GOSSIP_ITEM_LION "Lion"
#define GOSSIP_ITEM_WOLF "Wolf"
#define GOSSIP_ITEM_RAVAGER "Ravenger" 
#define GOSSIP_ITEM_UNTRAINEPET "Restart Pet"

//////////////////////WELCOME MENU/////////////////////////////////
#define Class_Skills  "Изучить классовые навыки и оружия"
#define Talent_Skills "Изучить таланты / сбросить таланты"
#define Riding_Skills "Изучить верховую езду"
#define nevermind 	  "[Закрыть]"		 
#define back 		  "[Назад]"
//SUB WELCOME MENU//
#define SUB_MENU_TALENT_OPTION_1 "Изучить таланты"
#define SUB_MENU_TALENT_OPTION_2 "Сбросить таланты"
/////////////////////NPC ID MENU////////////////////////////////

#define EMOTE_NO_VIP "Извините, этот нпц доступен только для VIP игроков..."

#define EfirID 38186// ид эфириальной монеты
/*  тут настройка по количеству   [все уже настроено с автоматом рачетом приза в умножение на 2] */
#define CostEfir1 10
#define CostEfir2 20
#define CostEfir3 50
#define CostEfir4 100
#define CostEfir5 250
#define CostEfir6 500

enum Enchants
{
    ENCHANT_WEP_BERSERKING              = 3789,
    ENCHANT_WEP_BLADE_WARD              = 3869,
    ENCHANT_WEP_BLOOD_DRAINING          = 3870,
    ENCHANT_WEP_ACCURACY                = 3788,
    ENCHANT_WEP_AGILITY_1H              = 1103,
    ENCHANT_WEP_SPIRIT                  = 3844,
    ENCHANT_WEP_BATTLEMASTER            = 2675,
    ENCHANT_WEP_BLACK_MAGIC             = 3790,
    ENCHANT_WEP_ICEBREAKER              = 3239,
    ENCHANT_WEP_LIFEWARD                = 3241,
    ENCHANT_WEP_MIGHTY_SPELL_POWER      = 3834, // One-hand
    ENCHANT_WEP_EXECUTIONER             = 3225,
    ENCHANT_WEP_POTENCY                 = 3833,
    ENCHANT_WEP_TITANGUARD              = 3851,
    ENCHANT_2WEP_MASSACRE               = 3827,
    ENCHANT_2WEP_SCOURGEBANE            = 3247,
    ENCHANT_2WEP_GIANT_SLAYER           = 3251,
    ENCHANT_2WEP_GREATER_SPELL_POWER    = 3854,
    ENCHANT_2WEP_AGILITY                = 2670,
    ENCHANT_2WEP_MONGOOSE               = 2673,

    ENCHANT_SHIELD_DEFENSE              = 1952,
    ENCHANT_SHIELD_INTELLECT            = 1128,
    ENCHANT_SHIELD_RESILIENCE           = 3229,
    ENCHANT_SHIELD_BLOCK                = 2655,
    ENCHANT_SHIELD_STAMINA              = 1071,
    ENCHANT_SHIELD_TOUGHSHIELD          = 2653,
    ENCHANT_SHIELD_TITANIUM_PLATING     = 3849,

    ENCHANT_HEAD_BLISSFUL_MENDING       = 3819,
    ENCHANT_HEAD_BURNING_MYSTERIES      = 3820,
    ENCHANT_HEAD_DOMINANCE              = 3796,
    ENCHANT_HEAD_SAVAGE_GLADIATOR       = 3842,
    ENCHANT_HEAD_STALWART_PROTECTOR     = 3818,
    ENCHANT_HEAD_TORMENT                = 3817,
    ENCHANT_HEAD_TRIUMPH                = 3795,
    ENCHANT_HEAD_ECLIPSED_MOON          = 3815,
    ENCHANT_HEAD_FLAME_SOUL             = 3816,
    ENCHANT_HEAD_FLEEING_SHADOW         = 3814,
    ENCHANT_HEAD_FROSTY_SOUL            = 3812,
    ENCHANT_HEAD_TOXIC_WARDING          = 3813,

    ENCHANT_SHOULDER_MASTERS_AXE        = 3835,
    ENCHANT_SHOULDER_MASTERS_CRAG       = 3836,
    ENCHANT_SHOULDER_MASTERS_PINNACLE   = 3837,
    ENCHANT_SHOULDER_MASTERS_STORM      = 3838,
    ENCHANT_SHOULDER_GREATER_AXE        = 3808,
    ENCHANT_SHOULDER_GREATER_CRAG       = 3809,
    ENCHANT_SHOULDER_GREATER_GLADIATOR  = 3852,
    ENCHANT_SHOULDER_GREATER_PINNACLE   = 3811,
    ENCHANT_SHOULDER_GREATER_STORM      = 3810,
    ENCHANT_SHOULDER_DOMINANCE          = 3794,
    ENCHANT_SHOULDER_TRIUMPH            = 3793,

    ENCHANT_CLOAK_DARKGLOW_EMBROIDERY   = 3728,
    ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY = 3730,
    ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY = 3722,
    ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE  = 3859,
    ENCHANT_CLOAK_WISDOM                = 3296,
    ENCHANT_CLOAK_TITANWEAVE            = 1951,
    ENCHANT_CLOAK_SPELL_PIERCING        = 3243,
    ENCHANT_CLOAK_SHADOW_ARMOR          = 3256,
    ENCHANT_CLOAK_MIGHTY_ARMOR          = 3294,
    ENCHANT_CLOAK_MAJOR_AGILITY         = 1099,
    ENCHANT_CLOAK_GREATER_SPEED         = 3831,

    ENCHANT_LEG_EARTHEN                 = 3853,
    ENCHANT_LEG_FROSTHIDE               = 3822,
    ENCHANT_LEG_ICESCALE                = 3823,
    ENCHANT_LEG_BRILLIANT_SPELLTHREAD   = 3719,
    ENCHANT_LEG_SAPPHIRE_SPELLTHREAD    = 3721,
    ENCHANT_LEG_DRAGONSCALE             = 3331,
    ENCHANT_LEG_WYRMSCALE               = 3332,

    ENCHANT_GLOVES_GREATER_BLASTING     = 3249,
    ENCHANT_GLOVES_ARMSMAN              = 3253,
    ENCHANT_GLOVES_CRUSHER              = 1603,
    ENCHANT_GLOVES_AGILITY              = 3222,
    ENCHANT_GLOVES_PRECISION            = 3234,
    ENCHANT_GLOVES_EXPERTISE            = 3231,

    ENCHANT_BRACERS_MAJOR_STAMINA       = 3850,
    ENCHANT_BRACERS_SUPERIOR_SP         = 2332,
    ENCHANT_BRACERS_GREATER_ASSUALT     = 3845,
    ENCHANT_BRACERS_MAJOR_SPIRT         = 1147,
    ENCHANT_BRACERS_EXPERTISE           = 3231,
    ENCHANT_BRACERS_GREATER_STATS       = 2661,
    ENCHANT_BRACERS_INTELLECT           = 1119,
    ENCHANT_BRACERS_FURL_ARCANE         = 3763,
    ENCHANT_BRACERS_FURL_FIRE           = 3759,
    ENCHANT_BRACERS_FURL_FROST          = 3760,
    ENCHANT_BRACERS_FURL_NATURE         = 3762,
    ENCHANT_BRACERS_FURL_SHADOW         = 3761,
    ENCHANT_BRACERS_FURL_ATTACK         = 3756,
    ENCHANT_BRACERS_FURL_STAMINA        = 3757,
    ENCHANT_BRACERS_FURL_SPELLPOWER     = 3758,

    ENCHANT_CHEST_POWERFUL_STATS        = 3832,
    ENCHANT_CHEST_SUPER_HEALTH          = 3297,
    ENCHANT_CHEST_GREATER_MAINA_REST    = 2381,
    ENCHANT_CHEST_EXCEPTIONAL_RESIL     = 3245,
    ENCHANT_CHEST_GREATER_DEFENSE       = 1953,

    ENCHANT_BOOTS_GREATER_ASSULT        = 1597,
    ENCHANT_BOOTS_TUSKARS_VITLIATY      = 3232,
    ENCHANT_BOOTS_SUPERIOR_AGILITY      = 983,
    ENCHANT_BOOTS_GREATER_SPIRIT        = 1147,
    ENCHANT_BOOTS_GREATER_VITALITY      = 3244,
    ENCHANT_BOOTS_ICEWALKER             = 3826,
    ENCHANT_BOOTS_GREATER_FORTITUDE     = 1075,
    ENCHANT_BOOTS_NITRO_BOOTS           = 3606,
    ENCHANT_BOOTS_PYRO_ROCKET           = 3603,
    ENCHANT_BOOTS_HYPERSPEED            = 3604,
    ENCHANT_BOOTS_ARMOR_WEBBING         = 3860,

    ENCHANT_RING_ASSULT                 = 3839,
    ENCHANT_RING_GREATER_SP             = 3840,
    ENCHANT_RING_STAMINA                = 3791,
	
	ENCHANT_BRACERS_SOKET               = 3717,
	ENCHANT_GLOVERS_SOKET               = 3723,
	
};

enum CLASSES_MENU_ID
{
	MENU_WARRIOR 		= 1,
	MENU_PALADIN 		= 2,
	MENU_HUNTER  		= 3,
	MENU_ROGUE   		= 4,
	MENU_PRIEST 		= 5,
	MENU_DEATH_K 		= 6,
	MENU_SHAMAN 		= 7,
	MENU_MAGE	 		= 8,
	MENU_WARLOCK 		= 9,
	MENU_DRUID 			= 10
};

enum OPTION_MENU_ID //11->50
{
	OPTION_MENU_ID_1    = 11,
	OPTION_MENU_ID_2	= 12,
	OPTION_MENU_ID_3 	= 13,
	OPTION_MENU_ID_4	= 14
};

enum SUB_OPTION_MENU_ID //51->100
{
	SUB_OPTION_MENU_ID_1    = 51,
	SUB_OPTION_MENU_ID_2	= 52,
	SUB_OPTION_MENU_ID_3 	= 53,
	SUB_OPTION_MENU_ID_4	= 54
};

enum BACK_OPTION //1000->
{	
	BACK_OPTION_MENU_1      = 1000, //Main Menu
	BACK_OPTION_MENU_2      = 1001, //Other
};

enum eIcons
{
    CHAT_ICON               = 0,
    VENDOR_ICON             = 1,
    FLIGHT_ICON             = 2,
    TRAINER_ICON            = 3,
    GEAR_ICON               = 4,
    GEAR_ICON_2             = 5,
    BANK_ICON               = 6,
    CHAT_DOTS_ICON          = 7,
    TABARD_ICON             = 8,
    SWORDS_ICON             = 9,
    GOLD_DOT_ICON           = 10
};

enum war_spell 
{
	war_spell_Overpower = 7384,
	war_spell_Battle_Shout = 47436,
	war_spell_Heroic_Strike = 47450,
	war_spell_Charge = 11578,
	war_spell_Rend = 47465,
	war_spell_Thunder_Clap = 47502,
	war_spell_Victory_Rush = 34428,
	war_spell_Hamstring = 1715,
	war_spell_Bloodrage = 2687,
	war_spell_Defensive_Stance = 71,
	war_spell_Sunder_Armor = 7386,
	war_spell_Taunt = 355,
	war_spell_Shield_Bash = 72,
	war_spell_Demoralizing_Shout = 47437,
	war_spell_Revenge = 57823,
	war_spell_Mocking_Blow = 694,
	war_spell_Shield_Block = 2565,
	war_spell_Disarm = 676,
	war_spell_Cleave = 47520,
	war_spell_Retaliation = 20230,
	war_spell_Stance_Mastery = 12678,
	war_spell_Execute = 47471,
	war_spell_Challenging_Shout = 1161,
	war_spell_Shield_Wall = 871,
	war_spell_Berserker_Stance = 2458,
	war_spell_Intercept = 20252,
	war_spell_Slam = 47475,
	war_spell_Berserker_Rage = 18499,
	war_spell_Whirlwind = 1680,
	war_spell_Pummel = 6552,
	war_spell_Sheild_Slam = 47488,
	war_spell_Recklessness = 1719,
	war_spell_Spell_Reflection = 23920,
	war_spell_Commanding_Shout = 47440,
	war_spell_Intervene = 3411,
	war_spell_Shattering_Throw = 64382,
	war_spell_Enraged_Regeneration = 55694,
	war_spell_Heroic_Throw = 57755,
	war_spell_Dual_Wield = 42459,
	war_spell_Intimidating_Shout = 5246,
	war_spell_Parry = 3127,

	//Talent - War 
	war_spell_Devastate = 47498, // Talent - Proto
	war_spell_Mortal_Strike = 47486 // Talent - Arms

};

enum pala_spell
{
	pala_spell_Parry = 3127,
	pala_spell_Concentration_Aura = 19746,
	pala_spell_Divine_Intervention = 19752,
	pala_spell_Plate_Mail_skill = 750,
	pala_spell_Devotion_Aura = 48942,
	pala_spell_Holy_Light = 48782,
	pala_spell_Blessing_of_Might = 48932,
	pala_spell_Judgement_of_Light = 20271,
	pala_spell_Divine_Protection = 498,
	pala_spell_Hammer_of_Justice = 10308,
	pala_spell_Purify = 1152,
	pala_spell_Hand_of_Protection = 10278,
	pala_spell_Lay_on_Hands = 48788,
	pala_spell_Judgement_of_Wisdom = 53408,
	pala_spell_Redemption = 48950,
	pala_spell_Blessing_of_Wisdom = 48936,
	pala_spell_Righteous_Defense = 31789,
	pala_spell_Hand_of_Reckoning = 62124,
	pala_spell_Retribution_Aura = 54043,
	pala_spell_Righteous_Fury = 25780,
	pala_spell_Hand_of_Freedom = 1044,
	pala_spell_Blessing_of_Kings = 20217,
	pala_spell_Consecration = 48819,
	pala_spell_Exorcism = 48801,
	pala_spell_Flash_of_Light = 48785,
	pala_spell_Sense_Undead = 5502,
	pala_spell_Seal_of_Justice = 20164,
	pala_spell_Turn_Evil = 10326,
	pala_spell_Hand_of_Salvation = 1038,
	pala_spell_Judgement_of_Justice = 53407,
	pala_spell_Shadow_Resistance_Aura = 48943,
	pala_spell_Seal_of_Light = 20165,
	pala_spell_Frost_Resistance_Aura = 48945,
	pala_spell_Divine_Shield = 642,
	pala_spell_Fire_Resistance_Aura = 48947,
	pala_spell_Seal_of_Wisdom = 20166,
	pala_spell_Cleanse = 4987,
	pala_spell_Hammer_of_Wrath = 48806,
	pala_spell_Hand_of_Sacrifice = 6940,
	pala_spell_Holy_Wrath = 48817,
	pala_spell_Greater_Blessing_of_Might = 48934,
	pala_spell_Greater_Blessing_of_Wisdom = 48938,
	pala_spell_Greater_Blessing_of_Kings = 25898,
	pala_spell_Crusader_Aura = 32223,
	pala_spell_Avenging_Wrath = 31884,
	pala_spell_Divine_Plea = 54428,
	pala_spell_Shield_of_Righteousness = 61411,
	pala_spell_Sacred_Shield = 53601,
	pala_spell_Seal_of_Vengeance = 31801, // Alliance
	pala_spell_Warhorse = 13819, // Alliance
	pala_spell_Charger = 23214, // Alliance
	pala_spell_Seal_of_Corruption = 53736, // Horde
	pala_spell_Thalassian_Warhorse = 34769, // Horde
	pala_spell_Thalassian_Charger = 34767, // Horde

	//Talent - Paladin
	pala_spell_Holy_Shield = 48952,  // Talent - Protection
	pala_spell_Avengers_Shield = 48827,  // Talent - Protection
	pala_spell_Greater_Blessing_of_Sanctuary = 25899, // Talents - Protection
	pala_spell_Holy_Shock = 48825, // Talents - Holy



};

enum mage_spell
{
	mage_spell_Arcane_Explosion = 42921,
	mage_spell_Frostbolt = 42842,
	mage_spell_Arcane_Intellect = 42995,
	mage_spell_Fireball = 42833,
	mage_spell_Conjure_Water = 27090,
	mage_spell_Conjure_Food = 33717,
	mage_spell_Fire_Blast = 42873,
	mage_spell_Arcane_Missiles = 42846,
	mage_spell_Polymorph = 12826,
	mage_spell_Polymorph_Turtle = 28271,
	mage_spell_Polymorph_Turkey = 61780,
	mage_spell_Polymorph_Rabbit = 61721,
	mage_spell_Polymorph_Pig = 28272,
	mage_spell_Frost_Nova = 42917,
	mage_spell_Dampen_Magic = 43015,
	mage_spell_Slow_Fall = 130,
	mage_spell_Flamestrike = 42926,
	mage_spell_Amplify_Magic = 43017,
	mage_spell_Remove_Curse = 475,
	mage_spell_Blink = 1953,
	mage_spell_Blizzard = 42940,
	mage_spell_Evocation = 12051,
	mage_spell_Fire_Ward = 43010,
	mage_spell_Mana_Shield = 43020,
	mage_spell_Frost_Ward = 43012,
	mage_spell_Scorch = 42859,
	mage_spell_Counterspell = 2139,
	mage_spell_Cone_of_Cold = 42931,
	mage_spell_Conjure_Mana_Gem = 42985,
	mage_spell_Ice_Armor = 43008,
	mage_spell_Ice_Block = 45438,
	mage_spell_Mage_Armor = 43024,
	mage_spell_Arcane_Brilliance = 43002,
	mage_spell_Molten_Armor = 43046,
	mage_spell_Arcane_Blast = 42897,
	mage_spell_Ice_Lance = 42914,
	mage_spell_Invisibility = 66,
	mage_spell_Ritual_of_Refreshment = 58659,
	mage_spell_Spellsteal = 30449,
	mage_spell_Conjure_Refreshment = 42956,
	mage_spell_Frostfire_Bolt = 47610,
	mage_spell_Dalaran_Brilliance = 61316,
	mage_spell_Dalaran_Intellect = 61024,
	mage_spell_Mirror_Image = 55342,
	mage_spell_Portal_Dalaran = 53142,
	mage_spell_Frost_Armor = 7301,

	mage_spell_Teleport_Exodar = 32271, // Alliance
	mage_spell_Teleport_Theramore = 49359, // Alliance
	mage_spell_Teleport_Darnassus = 3565, // Alliance
	mage_spell_Teleport_Shattrath = 33690, // Alliance
	mage_spell_Teleport_Ironforge = 3562, // Alliance
	mage_spell_Teleport_Stromwind = 3561, // Alliance
	mage_spell_Portal_Darnassus = 11419, // Alliance
	mage_spell_Portal_Exodar = 32266, // Alliance
	mage_spell_Portal_Ironforge = 11416, // Alliance
	mage_spell_Portal_Shattrath = 33691, // Alliance
	mage_spell_Portal_Theramore = 49360, // Alliance

	mage_spell_Teleport_Orgrimmar = 3567, // Horde
	mage_spell_Teleport_Shattrath_H = 35715, // Horde
	mage_spell_Teleport_Thunder_Bluff = 3566, // Horde
	mage_spell_Teleport_Stonard = 49358, // Horde
	mage_spell_Teleport_Silvermoon = 32272, // Horde
	mage_spell_Teleport_Undercity = 3563, // Horde
	mage_spell_Portal_Orgrimmar = 11417, // Horde
	mage_spell_Portal_Shattrath_H = 35717, // Horde
	mage_spell_Portal_Silvermoon = 32267, // Horde
	mage_spell_Portal_Stonard = 49361, // Horde
	mage_spell_Portal_Thunder_Bluff = 11420, // Horde
	mage_spell_Portal_Undercity = 11418, // Horde	


	//Talent - Mage
	mage_spell_Pyroblast = 42891, // Talent - Fire
	mage_spell_Ice_Barrier = 43039, // Talent - Frost
	mage_spell_Living_Bomb = 55360, // Talent - Fire
	mage_spell_Dragons_Breath = 42950, // Talent - Fire
	mage_spell_Blast_Wave = 42945, // Talent - Fire
	mage_spell_Arcane_Barrage = 44781 // Talent - Arcane

};

enum dk_spell
{
	dk_spell_Parry = 3127,
	dk_spell_Pestilence = 50842,
	dk_spell_Blood_Boil = 49941,
	dk_spell_Blood_Strike = 49930,
	dk_spell_Strangulate = 47476,
	dk_spell_Blood_Tap = 45529,
	dk_spell_Path_of_Frost = 3714,
	dk_spell_Dark_Command = 56222,
	dk_spell_Death_Pact = 48743,
	dk_spell_Frost_Presence = 48263,
	dk_spell_Icy_Touch = 49909,
	dk_spell_Mind_Freeze = 47528,
	dk_spell_Chains_of_Ice = 45524,
	dk_spell_Icebound_Fortitude = 48792,
	dk_spell_Horn_of_Winter = 57623,
	dk_spell_Rune_Strike = 56815,
	dk_spell_Empower_Rune_Weapon = 47568,
	dk_spell_Death_Coil = 49895,
	dk_spell_Death_Gate = 50977,
	dk_spell_Plague_Strike = 49921,
	dk_spell_Death_Grip = 49576,
	dk_spell_Raise_Dead = 46584,
	dk_spell_Death_and_Decay = 49938,
	dk_spell_Anti_Magic_Shell = 48707,
	dk_spell_Unholy_Presence = 48265,
	dk_spell_Raise_Ally = 61999,
	dk_spell_Army_of_the_Dead = 42650,
	dk_spell_Runeforging_skill = 53428,
	dk_spell_Uknowas = 53331,
	dk_spell_Uknowah = 54447,
	dk_spell_Uknowai = 53342,
	dk_spell_Uknowaz = 54446,
	dk_spell_Uknowan = 53323,
	dk_spell_Uknowab = 53344,
	dk_spell_Uknowav = 70164,
	dk_spell_Uknowaj = 62158,
	dk_spell_Uknowaq = 33391,
	dk_spell_Acherus_Deathcharger = 48778,
	dk_spell_Obliterate = 51425,
	dk_spell_Death_Strike = 49924,

	//Talent - Deathknight
	dk_spell_Heart_Strike = 55262, // Talent - Blood
	dk_spell_Frost_Strike = 55268, // Talent - Frost
	dk_spell_Howling_Blast = 51411, // Talent - Frost
	dk_spell_Scourge_Strike = 55271, // Talent - Unholy
	dk_spell_Corpse_Explosion = 51328 // Talent - Unholy

};

enum druid_spell
{
	druid_spell_Healing_Touch = 48378,
	druid_spell_Mark_of_the_Wild = 48469,
	druid_spell_Wrath = 48461,
	druid_spell_Moonfire = 48463,
	druid_spell_Rejuvenation = 48441,
	druid_spell_Thorns = 53307,
	druid_spell_Entangling_Roots = 53308,
	druid_spell_Bear_Form = 5487,
	druid_spell_Demoralizing_Roar = 48560,
	druid_spell_Growl = 6795,
	druid_spell_Maul = 48480,
	druid_spell_Natures_Grasp = 53312,
	druid_spell_Teleport_Moonglade = 18960,
	druid_spell_Enrage = 5229,
	druid_spell_Regrowth = 48443,
	druid_spell_Revive = 50763,
	druid_spell_Bash = 8983,
	druid_spell_Cure_Poison = 8946,
	druid_spell_Aquatic_Form = 1066,
	druid_spell_Swipe_Bear = 48562,
	druid_spell_Travel_Form = 783,
	druid_spell_Faerie_Fire = 770,
	druid_spell_Faerie_Fire_Feral = 16857,
	druid_spell_Hibernate = 18658,
	druid_spell_Cat_Form = 768,
	druid_spell_Feral_Charge_Bear = 16979,
	druid_spell_Feral_Charge_Cat = 49376,
	druid_spell_Prowl = 5215,
	druid_spell_Rebirth = 48477,
	druid_spell_Rip = 49800,
	druid_spell_Starfire = 48465,
	druid_spell_Shred = 48572,
	druid_spell_Soothe_Animal = 26995,
	druid_spell_Rake = 48574,
	druid_spell_Remove_Curse = 2782,
	druid_spell_Tigers_Fury = 50213,
	druid_spell_Abolish_Poison = 2893,
	druid_spell_Dash = 33357,
	druid_spell_Challenging_Roar = 5209,
	druid_spell_Cower = 48575,
	druid_spell_Tranquility = 48447,
	druid_spell_Ferocious_Bite = 48577,
	druid_spell_Ravage = 48579,
	druid_spell_Track_Humanoids = 5225,
	druid_spell_Frenzied_Regeneration = 22842,
	druid_spell_Pounce = 49803,
	druid_spell_Dire_Bear_Form = 9634,
	druid_spell_Feline_Grace = 20719,
	druid_spell_Hurricane = 48467,
	druid_spell_Innervate = 29166,
	druid_spell_Savage_Defense = 62600,
	druid_spell_Barkskin = 22812,
	druid_spell_Gift_of_the_Wild = 48470,
	druid_spell_Mangle_Bear = 48564,
	druid_spell_Mangle_Cat = 48566,
	druid_spell_Flight_Form = 33943,
	druid_spell_Maim = 49802,
	druid_spell_Lifebloom = 48451,
	druid_spell_Lacerate = 48568,
	druid_spell_Cyclone = 33786,
	druid_spell_Swift_Flight_Form = 40120,
	druid_spell_Swipe_Cat = 62078,
	druid_spell_Savage_Roar = 52610,
	druid_spell_Nourish = 50464,
	druid_spell_Claw_1 = 1082,
	druid_spell_Claw = 48570,

	//Talent - Druid
	druid_spell_Typhoon = 61384, // Talent - Balance
	druid_spell_Starfall = 53201, // Talent - Balance
	druid_spell_Wild_Growth = 53251, // Talent - Restoration
	druid_spell_Insect_Swarm = 48468 // Talent - Balance

};

enum shaman_spell
{
	shaman_spell_Earth_Elemental_Totem = 2062,
	shaman_spell_Healing_Wave = 49273,
	shaman_spell_Lightning_Bolt = 49238,
	shaman_spell_Rockbiter_Weapon = 10399,
	shaman_spell_Earth_Shock = 49231,
	shaman_spell_Stoneskin_Totem = 58753,
	shaman_spell_Earthbind_Totem = 2484,
	shaman_spell_Lightning_Shield = 49281,
	shaman_spell_Stoneclaw_Totem = 58582,
	shaman_spell_Flame_Shock = 49233,
	shaman_spell_Flametongue_Weapon = 58790,
	shaman_spell_Searing_Totem = 58704,
	shaman_spell_Strength_of_Earth_Totem = 58643,
	shaman_spell_Ancestral_Spirit = 49277,
	shaman_spell_Fire_Nova = 61657,
	shaman_spell_Purge = 8012,
	shaman_spell_Cure_Toxins = 526,
	shaman_spell_Ghost_Wolf = 2645,
	shaman_spell_Wind_Shear = 57994,
	shaman_spell_Tremor_Totem = 8143,
	shaman_spell_Frost_Shock = 49236,
	shaman_spell_Frostbrand_Weapon = 58796,
	shaman_spell_Healing_Stream_Totem = 58757,
	shaman_spell_Lesser_Healing_Wave = 49276,
	shaman_spell_Water_Shield = 57960,
	shaman_spell_Water_Breathing = 131,
	shaman_spell_Frost_Resistance_Totem = 58745,
	shaman_spell_Far_Sight = 6196,
	shaman_spell_Magma_Totem = 58734,
	shaman_spell_Mana_Spring_Totem = 58774,
	shaman_spell_Fire_Resistance_Totem = 58739,
	shaman_spell_Flametongue_Totem = 58656,
	shaman_spell_Water_Walking = 546,
	shaman_spell_Astral_Recall = 556,
	shaman_spell_Call_of_the_Elements = 66842,
	shaman_spell_Earthliving_Weapon = 51994,
	shaman_spell_Grounding_Totem = 8177,
	shaman_spell_Nature_Resistance_Totem = 58749,
	shaman_spell_Reincarnation = 20608,
	shaman_spell_Totemic_Recall = 36936,
	shaman_spell_Windfury_Weapon = 58804,
	shaman_spell_Chain_Lightning = 49271,
	shaman_spell_Windfury_Totem = 8512,
	shaman_spell_Sentry_Totem = 6495,
	shaman_spell_Cleansing_Totem = 8170,
	shaman_spell_Call_of_the_Ancestors = 66843,
	shaman_spell_Chain_Heal = 55459,
	shaman_spell_Call_of_the_Spirits = 66844,
	shaman_spell_Wrath_of_Air_Totem = 3738,
	shaman_spell_Fire_Elemental_Totem = 2894,
	shaman_spell_Lava_Burst = 60043,
	shaman_spell_Hex = 51514,
	shaman_spell_Bloodlust = 2825, // Horde
	shaman_spell_Heroism = 32182, // Alliance

	//Talent - Shaman
	shaman_spell_Riptide = 61301, // Talent - Restoration
	shaman_spell_Earth_Shield = 49284, // Talent - Restoration
	shaman_spell_Totem_of_Wrath = 57722, // Talent - Elemental
	shaman_spell_Thunderstorm = 59159   // Talent - Elemental

};

enum hunter_spell
{
	hunter_spell_Scorpid_Sting = 3043,
	hunter_spell_Parry = 3127,
	hunter_spell_Rapid_Fire = 3045,
	hunter_spell_Viper_Sting = 3034,
	hunter_spell_Track_Beasts = 1494,
	hunter_spell_Aspect_of_the_Monkey = 13163,
	hunter_spell_Raptor_Strike = 48996,
	hunter_spell_Serpent_Sting = 49001,
	hunter_spell_Arcane_Shot = 49045,
	hunter_spell_Hunters_Mark = 53338,
	hunter_spell_Concussive_Shot = 5116,
	hunter_spell_Aspect_of_the_Hawk = 27044,
	hunter_spell_Call_Pet = 883,
	hunter_spell_Dismiss_Pet = 2641,
	hunter_spell_Feed_Pet = 6991,
	hunter_spell_Revive_Pet = 982,
	hunter_spell_Tame_Beast = 1515,
	hunter_spell_Track_Humanoids = 19883,
	hunter_spell_Distracting_Shot = 20736,
	hunter_spell_Mend_Pet = 48990,
	hunter_spell_Wing_Clip = 2974,
	hunter_spell_Eagle_Eye = 6197,
	hunter_spell_Eyes_of_the_Beast = 1002,
	hunter_spell_Scare_Beast = 14327,
	hunter_spell_Aspect_of_the_Cheetah = 5118,
	hunter_spell_Immolation_Trap = 49056,
	hunter_spell_Mongoose_Bite = 53339,
	hunter_spell_Multi_Shot = 49048,
	hunter_spell_Track_Undead = 19884,
	hunter_spell_Aspect_of_the_Viper = 34074,
	hunter_spell_Disengage = 781,
	hunter_spell_Freezing_Trap = 14311,
	hunter_spell_Beast_Lore = 1462,
	hunter_spell_Track_Hidden = 19885,
	hunter_spell_Track_Elementals = 19880,
	hunter_spell_Frost_Trap = 13809,
	hunter_spell_Aspect_of_the_Beast = 13161,
	hunter_spell_Feign_Death = 5384,
	hunter_spell_Flare = 1543,
	hunter_spell_Track_Demons = 19878,
	hunter_spell_Explosive_Trap = 49067,
	hunter_spell_Aspect_of_the_Pack = 13159,
	hunter_spell_Track_Giants = 19882,
	hunter_spell_Volley = 58434,
	hunter_spell_Aspect_of_the_Wild = 49071,
	hunter_spell_Steady_Shot = 49052,
	hunter_spell_Track_Dragonkin = 19879,
	hunter_spell_Deterrence = 19263,
	hunter_spell_Tranquilizing_Shot = 19801,
	hunter_spell_Kill_Command = 34026,
	hunter_spell_Snake_Trap = 34600,
	hunter_spell_Misdirection = 34477,
	hunter_spell_Kill_Shot = 61006,
	hunter_spell_Aspect_of_the_Dragonhawk = 61847,
	hunter_spell_Masters_Call = 53271,
	hunter_spell_Freezing_Arrow = 60192,
	hunter_spell_Call_Stabled_Pet = 62757,
	hunter_spell_Dual_Wield = 42459,

	//Talent - Hunter
	hunter_spell_Wyvern_Sting = 49012,  // Talent - Survival
	hunter_spell_Explosive_Shot = 60053, // Talent - Survival
	hunter_spell_Conterattack = 48999, // Talent - Survival
	hunter_spell_Aimed_Shot = 49050  // Talent - Marksmanship


};

enum rogue_spell
{
	rogue_spell_Parry = 3127,
	rogue_spell_Dual_Wield = 42459,
	rogue_spell_Eviscerate = 48668,
	rogue_spell_Sinister_Strike = 48638,
	rogue_spell_Stealth = 1784,
	rogue_spell_Backstab = 48657,
	rogue_spell_Pick_Pocket = 921,
	rogue_spell_Gouge = 1776,
	rogue_spell_Evasion = 26669,
	rogue_spell_Sap = 51724,
	rogue_spell_Slice_and_Dice = 6774,
	rogue_spell_Sprint = 11305,
	rogue_spell_Kick = 1766,
	rogue_spell_Garrote = 48676,
	rogue_spell_Feint = 48659,
	rogue_spell_Lockpicking_skill = 1804,
	rogue_spell_Expose_Armor = 8647,
	rogue_spell_Ambush = 48691,
	rogue_spell_Dismantle = 51722,
	rogue_spell_Rupture = 48672,
	rogue_spell_Distract = 1725,
	rogue_spell_Vanish = 26889,
	rogue_spell_Detect_Traps = 2836,
	rogue_spell_Cheap_Shot = 1833,
	rogue_spell_Disarm_Trap = 1842,
	rogue_spell_Kidney_Shot = 8643,
	rogue_spell_Blind = 2094,
	rogue_spell_Safe_Fall = 1860,
	rogue_spell_Envenom = 57993,
	rogue_spell_Deadly_Throw = 48674,
	rogue_spell_Cloak_of_Shadows = 31224,
	rogue_spell_Shiv = 5938,
	rogue_spell_Tricks_of_the_Trade = 57934,
	rogue_spell_Fan_of_Knives = 51723,

	//Talent - Rogue
	rogue_spell_Hemorrhage = 48660, // Talent - Subtlety
	rogue_spell_Mutilate = 48666 // Talent - Assassination


};

enum priest_spell
{
	priest_spell_Cure_Disease = 528,
	priest_spell_Lesser_Heal = 2053,
	priest_spell_Power_Word_Fortitude = 48161,
	priest_spell_Smite = 48123,
	priest_spell_Shadow_Word_Pain = 48125,
	priest_spell_Power_Word_Shield = 48066,
	priest_spell_Fade = 586,
	priest_spell_Renew = 48068,
	priest_spell_Mind_Blast = 48127,
	priest_spell_Resurrection = 48171,
	priest_spell_Inner_Fire = 48168,
	priest_spell_Psychic_Scream = 10890,
	priest_spell_Heal = 6064,
	priest_spell_Dispel_Magic = 988,
	priest_spell_Devouring_Plague = 48300,
	priest_spell_Fear_Ward = 6346,
	priest_spell_Flash_Heal = 48071,
	priest_spell_Holy_Fire = 48135,
	priest_spell_Holy_Nova = 48078,
	priest_spell_Mind_Soothe = 453,
	priest_spell_Shackle_Undead = 10955,
	priest_spell_Mind_Vision = 10909,
	priest_spell_Mana_Burn = 8129,
	priest_spell_Divine_Spirit = 48073,
	priest_spell_Mind_Control = 605,
	priest_spell_Prayer_of_Healing = 48072,
	priest_spell_Shadow_Protection = 48169,
	priest_spell_Abolish_Disease = 552,
	priest_spell_Levitate = 1706,
	priest_spell_Greater_Heal = 48063,
	priest_spell_Prayer_of_Fortitude = 48162,
	priest_spell_Prayer_of_Shadow_Protection = 48170,
	priest_spell_Prayer_of_Spirit = 48074,
	priest_spell_Shadow_Word_Death = 48158,
	priest_spell_Binding_Heal = 48120,
	priest_spell_Shadowfiend = 34433,
	priest_spell_Prayer_of_Mending = 48113,
	priest_spell_Mass_Dispel = 32375,
	priest_spell_Divine_Hymn = 64843,
	priest_spell_Hymn_of_Hope = 64901,
	priest_spell_Mind_Sear = 53023,

	//Talent - Priest
	priest_spell_Vampiric_Touch = 48160, // Talent - Shadow
	priest_spell_Penance = 53007, // Talent - Discipline
	priest_spell_Lightwell = 48087, // Talent - Holy
	priest_spell_Desperate_Prayer = 48173, // Talent - Holy
	priest_spell_Circle_of_Healing = 48089, // Talent - Holy
	priest_spell_Mind_Flay = 48156 // Talent - Shadow

};

enum warlock_spell
{
	warlock_spell_Demon_Skin = 696,
	warlock_spell_Immolate = 47811,
	warlock_spell_Shadow_Bolt = 47809,
	warlock_spell_Summon_Imp = 688,
	warlock_spell_Corruption = 47813,
	warlock_spell_Curse_of_Weakness = 50511,
	warlock_spell_Life_Tap = 57946,
	warlock_spell_Curse_of_Agony = 47864,
	warlock_spell_Fear = 6215,
	warlock_spell_Create_Healthstone = 47878,
	warlock_spell_Drain_Soul = 47855,
	warlock_spell_Summon_Voidwalker = 697,
	warlock_spell_Health_Funnel = 47856,
	warlock_spell_Drain_Life = 47857,
	warlock_spell_Unending_Breath = 5697,
	warlock_spell_Create_Soulstone = 47884,
	warlock_spell_Searing_Pain = 47815,
	warlock_spell_Demon_Armor = 47889,
	warlock_spell_Rain_of_Fire = 47820,
	warlock_spell_Ritual_of_Summoning = 698,
	warlock_spell_Summon_Succubus = 712,
	warlock_spell_Eye_of_Kilrogg = 126,
	warlock_spell_Drain_Mana = 5138,
	warlock_spell_Sense_Demons = 5500,
	warlock_spell_Curse_of_Tongues = 11719,
	warlock_spell_Detect_Invisibility = 132,
	warlock_spell_Create_Firestone = 60220,
	warlock_spell_Banish = 18647,
	warlock_spell_Enslave_Demon = 61191,
	warlock_spell_Hellfire = 47823,
	warlock_spell_Summon_Felhunter = 691,
	warlock_spell_Curse_of_the_Elements = 47865,
	warlock_spell_Shadow_Ward = 47891,
	warlock_spell_Create_Spellstone = 47888,
	warlock_spell_Howl_of_Terror = 17928,
	warlock_spell_Death_Coil = 47860,
	warlock_spell_Soul_Fire = 47825,
	warlock_spell_Inferno = 1122,
	warlock_spell_Curse_of_Doom = 47867,
	warlock_spell_Ritual_of_Doom = 18540,
	warlock_spell_Fel_Armor = 47893,
	warlock_spell_Incinerate = 47838,
	warlock_spell_Soulshatter = 29858,
	warlock_spell_Ritual_of_Souls = 58887,
	warlock_spell_Seed_of_Corruption = 47836,
	warlock_spell_Shadowflame = 61290,
	warlock_spell_Demonic_Circle_Summon = 48018,
	warlock_spell_Demonic_Circle_Teleport = 48020,
	warlock_spell_Dreadsteed = 23161,

	//Talent - Warlock
	warlock_spell_Shadowburn = 47827, // Talent - Destruction
	warlock_spell_Shadowfury = 47847, // Talent - Destruction
	warlock_spell_Unstable_Affliction = 47843, // Talent - Affliction
	warlock_spell_Chaos_Bolt = 59172, // Talent - Destruction
	warlock_spell_Haunt = 59164, // Talent - Affliction
	warlock_spell_Dark_Pact = 59092 // Talent - Affliction

};

enum mount_spell
{
	//mount_spell_Master_Riding = 90265, cata
	mount_spell_Journeyman_Riding = 34093,
	mount_spell_Cold_Weather_Flying = 54197,
	mount_spell_Flight_Master_License = 90267
};

enum mount_item_id
{	
	Mount_Invincibles_Reins = 49284,
};

enum plate
{
    plate_dd_nesk			= 50728,
	plate_dd_back			= 50677,
	plate_dd_bracers		= 50659,
	plate_dd_belt			= 50620,
	plate_dd_feet			= 54578,
	
	plate_spd_nesk			= 50700,
	plate_spd_back			= 50668,
	plate_spd_bracers		= 50721,
	plate_spd_belt			= 50667,
	plate_spd_feet			= 54586
};

enum mail
{
    mail_dd_nesk			= 50633,
	mail_dd_back			= 50653,
	mail_dd_bracers			= 50655,
	mail_dd_belt			= 50688,
	mail_dd_feet			= 50711,
	
	mail_spd_nesk			= 55555,
	mail_spd_back			= 55555,
	mail_spd_bracers		= 55555,
	mail_spd_belt			= 55555,
	mail_spd_feet			= 55555,
	
	mail_heal_nesk			= 55555,
	mail_heal_back			= 55555,
	mail_heal_bracers		= 55555,
	mail_heal_belt			= 55555,
	mail_heal_feet			= 55555
};

enum leather
{
    leather_dd_nesk			= 50633,
	leather_dd_back			= 50653,
	leather_dd_bracers		= 50670,
	leather_dd_belt			= 50707,
	leather_dd_feet			= 50607,
	
	leather_spd_nesk		= 55555,
	leather_spd_back		= 55555,
	leather_spd_bracers		= 55555,
	leather_spd_belt		= 55555,
	leather_spd_feet		= 55555,
	
	leather_heal_nesk		= 55555,
	leather_heal_back		= 55555,
	leather_heal_bracers	= 55555,
	leather_heal_belt		= 55555,
	leather_heal_feet		= 55555
};

enum cloth
{
    cloth_spd_nesk			= 55555,
	cloth_spd_back			= 55555,
	cloth_spd_bracers		= 55555,
	cloth_spd_belt			= 55555,
	cloth_spd_feet			= 55555,
	
	cloth_heal_nesk			= 50700,
	cloth_heal_back			= 50668,
	cloth_heal_bracers		= 50686,
	cloth_heal_belt			= 50702,
	cloth_heal_feet			= 51850
};

enum otheritem
{
    mili_ring_1				= 50693,
	mili_ring_2				= 50657,	
	mili_trinket_1			= 50363,
	mili_trinket_2			= 54590,
	
	spd_ring_1				= 50614,
	spd_ring_2				= 50714,
	spd_trinket_1			= 54588,
	spd_trinket_2			= 50365,
	
	heal_ring_1				= 54585,
	heal_ring_2				= 50636,
	heal_trinket_1			= 54589,
	heal_trinket_2			= 54588,
	
	agility_ring_1			= 50618,
	agility_ring_2			= 54576
};

void Enchant(Player* player, Item* item, uint32 enchantid)
{
    if (!item)
    {
        player->GetSession()->SendNotification("Вы должны одеть предмет, чтобы зачарить его!");
        return;
    }
    item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
    item->SetEnchantment(PERM_ENCHANTMENT_SLOT, enchantid, 0, 0);
	int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
    const ItemTemplate* temp = item->GetTemplate();
    std::string name = temp->Name1;
    if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
        ObjectMgr::GetLocaleString(il->Name, loc_idx, name);
    player->GetSession()->SendNotification("|cff800080%s |cffFF0000Успешно зачаровано!", name.c_str());
}

void RemoveEnchant(Player* player, Item* item)
{
    if (!item)
    {
        player->GetSession()->SendNotification("У вас нет предметов, чтобы одеть их?");
        return;
    }
	int loc_idx = player->GetSession()->GetSessionDbLocaleIndex();
    const ItemTemplate* temp = item->GetTemplate();
    std::string name = temp->Name1;
    if (ItemLocale const* il = sObjectMgr->GetItemLocale(temp->ItemId))
        ObjectMgr::GetLocaleString(il->Name, loc_idx, name);
    item->ClearEnchantment(PERM_ENCHANTMENT_SLOT);
    player->GetSession()->SendNotification("|cff800080%s |cffFF0000успешно разочарован!", name.c_str());
}

class npc_enchantment : public CreatureScript
{
public:
    npc_enchantment() : CreatureScript("npc_enchantment") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_1H_PVPPandariaS3_C_02:30|t Зачарить Правую руку", GOSSIP_SENDER_MAIN, 1);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_1h_DeathwingRaid_D_01:30|t Зачарить Левую руку", GOSSIP_SENDER_MAIN, 13);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_2h_DeathwingRaidDW_D_01:30|t Зачарить Двуручное Оружие", GOSSIP_SENDER_MAIN, 2);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Shield_DeathwingRaid_D_02:30|t Зачарить Щит", GOSSIP_SENDER_MAIN, 3);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_helmet_cloth_raidwarlock_l_01:30|t Зачарить Голову", GOSSIP_SENDER_MAIN, 4);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Shoulder_Cloth_RaidWarlock_L_01:30|t Зачарить Плечи", GOSSIP_SENDER_MAIN, 5);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Cape_Pandaria_DragonCaster_D_02:30|t Зачарить Спину", GOSSIP_SENDER_MAIN, 6);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Chest_Cloth_RaidWarlock_L_01:30|t Зачарить Грудь", GOSSIP_SENDER_MAIN, 7);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Bracer_Cloth_RaidWarlock_L_01:30|t Зачарить Запястья", GOSSIP_SENDER_MAIN, 8);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Glove_Cloth_RaidWarlock_L_01:30|t Зачарить Руки", GOSSIP_SENDER_MAIN, 9);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Pants_Cloth_RaidWarlock_L_01:30|t Зачарить Ноги", GOSSIP_SENDER_MAIN, 10);
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Boot_Cloth_RaidWarlock_L_01:30|t Зачарить Ступни", GOSSIP_SENDER_MAIN, 11);

            if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
            {
                player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Jewelry_Ring_104:30|t Зачарить Кольцо # 1", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Jewelry_Ring_104:30|t Зачарить Кольцо # 2", GOSSIP_SENDER_MAIN, 1200);
            }
            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_warlock_backdraftgreen:30|t Я хотел бы снять мои чары", GOSSIP_SENDER_MAIN, 14);

            player->PlayerTalkClass->SendGossipMenu(100001, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            Item * item;

            switch (action)
            {

                case 1: // Enchant Weapon
                    if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
                    {
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Отведение удара", GOSSIP_SENDER_MAIN, 102);
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Вытягивание крови", GOSSIP_SENDER_MAIN, 103);
                    }
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 26 к ловкости", GOSSIP_SENDER_MAIN, 100);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 45 к духу", GOSSIP_SENDER_MAIN, 101);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Боевое исступление", GOSSIP_SENDER_MAIN, 104);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к меткости и + 25 к крит. удару", GOSSIP_SENDER_MAIN, 105);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Чёрная магия", GOSSIP_SENDER_MAIN, 106);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Военачальник", GOSSIP_SENDER_MAIN, 107);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Оружие Ледомола", GOSSIP_SENDER_MAIN, 108);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Оберег", GOSSIP_SENDER_MAIN, 109);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к выносливости", GOSSIP_SENDER_MAIN, 110);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 65 к силе атаки", GOSSIP_SENDER_MAIN, 111);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 63 к силе заклинаний", GOSSIP_SENDER_MAIN, 112);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Мангуст", GOSSIP_SENDER_MAIN, 113);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Палач", GOSSIP_SENDER_MAIN, 114);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
                    return true;
                    break;

                case 2: // Enchant 2H Weapon
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                        if (!item)
                        {
                            player->GetSession()->SendNotification("Сначала нужно одеть двуручное оружие!");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                        {
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Боевое исступление", GOSSIP_SENDER_MAIN, 104);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 81 к силе заклинаний", GOSSIP_SENDER_MAIN, 115);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 35 к ловкости", GOSSIP_SENDER_MAIN, 116);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 110 к силе атаки", GOSSIP_SENDER_MAIN, 117);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Мангуст", GOSSIP_SENDER_MAIN, 113);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Палач", GOSSIP_SENDER_MAIN, 114);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                            player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
                            return true;
                        }
                        else
                        {
                            player->GetSession()->SendNotification("У вас не одето двуручное оружие!");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 3: // Enchant Shield
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if (!item)
                        {
                            player->GetSession()->SendNotification("Сначала нужно одеть щит!");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 20 к рейтингу защиты", GOSSIP_SENDER_MAIN, 118);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к интеллекту", GOSSIP_SENDER_MAIN, 119);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 12 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 120);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Титановая обшивка", GOSSIP_SENDER_MAIN, 121);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 18 к выносливости", GOSSIP_SENDER_MAIN, 122);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 36 к блокированию", GOSSIP_SENDER_MAIN, 123);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                            player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
                            return true;
                        }
                        else
                        {
                            player->GetSession()->SendNotification("У вас не одет щит!");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 4: // Enchant Head
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к силе заклинаний и + 10 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 124);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к силе заклинаний и + 20 к крит. удару ", GOSSIP_SENDER_MAIN, 125);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 29 к силе заклинаний и + 20 к устойчивости", GOSSIP_SENDER_MAIN, 126);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к выносливости и + 25 к устойчивости", GOSSIP_SENDER_MAIN, 127);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 37 к выносливости и + 20 к защите", GOSSIP_SENDER_MAIN, 128);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к силе атаки и + 20 к крит. удару", GOSSIP_SENDER_MAIN, 129);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к силе атаки и + 20 к устойчивости", GOSSIP_SENDER_MAIN, 130);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к сопр. тайной магии и + 30 к выносливости", GOSSIP_SENDER_MAIN, 131);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к сопр. огню и + 30 к выносливости", GOSSIP_SENDER_MAIN, 132);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к сопр. тёмной магии и + 30 к выносливости", GOSSIP_SENDER_MAIN, 133);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к сопр. магии льда и + 30 к выносливости", GOSSIP_SENDER_MAIN, 134);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к сопр. силы природы и + 30 к выносливости", GOSSIP_SENDER_MAIN, 135);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100005, creature->GetGUID());
                    return true;
                    break;

                case 5: // Enchant Shoulders
                    if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) > 400)
                    {
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 120 к силе атаки и + 15 к крит. удару", GOSSIP_SENDER_MAIN, 136);
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к силе заклинаний и + 8 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 137);
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 60 к рейтингу уклонения и + 15 к защите", GOSSIP_SENDER_MAIN, 138);
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к силе заклинаний и + 15 к крит. эффекта", GOSSIP_SENDER_MAIN, 139);
                    }
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 40 к силе атаки и + 15 к крит. удару", GOSSIP_SENDER_MAIN, 140);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 24 к силе заклинаний и + 8 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 141);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к выносливости и + 15 к устойчивости", GOSSIP_SENDER_MAIN, 142);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 20 к рейтингу уклонения и + 15 к защите", GOSSIP_SENDER_MAIN, 143);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 24 к силе заклинаний и + 15 к крит. удара", GOSSIP_SENDER_MAIN, 144);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 23 к силе заклинаний и + 15 к устойчивости", GOSSIP_SENDER_MAIN, 145);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 40 к силе атаки и + 15 к устойчивости", GOSSIP_SENDER_MAIN, 146);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100006, creature->GetGUID());
                    return true;
                    break;

                case 6: // Enchant Cloak
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Уменьшение скорости падения", GOSSIP_SENDER_MAIN, 147);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Улучшенная незаметность + 10 к ловкости", GOSSIP_SENDER_MAIN, 148);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Вышивка тёмного сияния", GOSSIP_SENDER_MAIN, 149);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Светлотканная вышивка", GOSSIP_SENDER_MAIN, 150);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Вышивка в виде рукояти меча", GOSSIP_SENDER_MAIN, 151);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 10 к духу и снижение угрозы на 2%", GOSSIP_SENDER_MAIN, 152);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 16 к защите", GOSSIP_SENDER_MAIN, 153);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 35 к проникающей способности заклинаний", GOSSIP_SENDER_MAIN, 154);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 225 к броне", GOSSIP_SENDER_MAIN, 155);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 22 к ловкости" , GOSSIP_SENDER_MAIN, 156);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 23 к скорости", GOSSIP_SENDER_MAIN, 157);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
                    return true;
                    break;

                case 7: //Enchant chest
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 10 ко всему", GOSSIP_SENDER_MAIN, 158);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 275 к здоровью", GOSSIP_SENDER_MAIN, 159);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 10 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 160);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 20 к устойчивости", GOSSIP_SENDER_MAIN, 161);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 22 к защите", GOSSIP_SENDER_MAIN, 162);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
                    return true;
                    break;

                case 8: //Enchant Bracers
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 40 к выносливости", GOSSIP_SENDER_MAIN, 163);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к силе заклинаний", GOSSIP_SENDER_MAIN, 164);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к силе атаки", GOSSIP_SENDER_MAIN, 165);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 18 к духу", GOSSIP_SENDER_MAIN, 166);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 15 к мастерству", GOSSIP_SENDER_MAIN, 167);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 6 ко всему", GOSSIP_SENDER_MAIN, 168);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 16 к интеллекту", GOSSIP_SENDER_MAIN, 169);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к сопротивлению тайной магии", GOSSIP_SENDER_MAIN, 170);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к сопротивлению магии огня", GOSSIP_SENDER_MAIN, 171);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к сопротивлению магии льда", GOSSIP_SENDER_MAIN, 172);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к сопротивлению силам природы", GOSSIP_SENDER_MAIN, 173);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 70 к сопротивлению тёмной магии", GOSSIP_SENDER_MAIN, 174);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 130 к силе атаки", GOSSIP_SENDER_MAIN, 175);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 102 к выносливости", GOSSIP_SENDER_MAIN, 176);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 76 к силе заклинаний", GOSSIP_SENDER_MAIN, 177);
					//player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + Гнездо в наручах", GOSSIP_SENDER_MAIN, 414);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
                    return true;
                    break;

                case 9: //Enchant Gloves
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 16 к крит. удару", GOSSIP_SENDER_MAIN, 178);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 2% угрозы и + 10 к паррированию", GOSSIP_SENDER_MAIN, 179);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 44 к силе атаки", GOSSIP_SENDER_MAIN, 180);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 20 к ловкости", GOSSIP_SENDER_MAIN, 181);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 20 к меткости", GOSSIP_SENDER_MAIN, 182);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 15 к мастерству", GOSSIP_SENDER_MAIN, 183);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Выстрел ракетой", GOSSIP_SENDER_MAIN, 199);
					// player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + Гнездо в перчатках", GOSSIP_SENDER_MAIN, 415);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
                    return true;
                    break;

                case 10: //Enchant legs
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 40 к устойчивости и + 28 к выносливости", GOSSIP_SENDER_MAIN, 184);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 55 к выносливости и + 22 к ловкости", GOSSIP_SENDER_MAIN, 185);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 75 к силе атаки и + 22 к крит. удару", GOSSIP_SENDER_MAIN, 186);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к силе заклинаний + 20 к духу", GOSSIP_SENDER_MAIN, 187);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к силе заклинаний + 30 к выносливости", GOSSIP_SENDER_MAIN, 188);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 72 к выносливости + 35 к ловкости", GOSSIP_SENDER_MAIN, 189);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 100 к силе атаки и + 36 к крит. удару", GOSSIP_SENDER_MAIN, 190);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100011, creature->GetGUID());
                    return true;
                    break;

                case 11: //Enchant feet
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 32 к силе атаки", GOSSIP_SENDER_MAIN, 191);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 15 к выносливости и ув. скорости бега", GOSSIP_SENDER_MAIN, 192);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 16 к ловкости", GOSSIP_SENDER_MAIN, 193);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 18 к духу", GOSSIP_SENDER_MAIN, 194);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 7 ед. здоровья и маны каждые 5 сек.", GOSSIP_SENDER_MAIN, 195);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 12 к рейтингу меткости + 12 к крит. удару", GOSSIP_SENDER_MAIN, 196);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 22  к выносливости", GOSSIP_SENDER_MAIN, 197);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Нитроускоритель", GOSSIP_SENDER_MAIN, 198);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Рейтинг скорости + 340 на 12 сек", GOSSIP_SENDER_MAIN, 200);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 885 к броне", GOSSIP_SENDER_MAIN, 201);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
                    return true;
                    break;

                case 12: //Enchant rings
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 40 к силе атаки", GOSSIP_SENDER_MAIN, 202);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 23 к силе заклинаний", GOSSIP_SENDER_MAIN, 203);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к выносливости", GOSSIP_SENDER_MAIN, 204);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
                    return true;
                    break;
					
				case 1200: //Enchant rings
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 40 к силе атаки", GOSSIP_SENDER_MAIN, 2022);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 23 к силе заклинаний", GOSSIP_SENDER_MAIN, 2032);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 30 к выносливости", GOSSIP_SENDER_MAIN, 2042);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
                    return true;
                    break;

                case 13: //Enchant Off-Hand weapons
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendNotification("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Отведение удара", GOSSIP_SENDER_MAIN, 205);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Вытягивание крови", GOSSIP_SENDER_MAIN, 219);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 26 к ловкости", GOSSIP_SENDER_MAIN, 206);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 45 к духу", GOSSIP_SENDER_MAIN, 207);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Боевое исступление", GOSSIP_SENDER_MAIN, 208);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 25 к меткости и + 25 к крит. удару", GOSSIP_SENDER_MAIN, 209);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Чёрная магия", GOSSIP_SENDER_MAIN, 210);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Военачальник", GOSSIP_SENDER_MAIN, 211);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Оружие Ледомола", GOSSIP_SENDER_MAIN, 212);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Оберег", GOSSIP_SENDER_MAIN, 213);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 50 к выносливости", GOSSIP_SENDER_MAIN, 214);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 65 к силе атаки", GOSSIP_SENDER_MAIN, 215);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t + 63 к силе заклинаний", GOSSIP_SENDER_MAIN, 216);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Мангуст", GOSSIP_SENDER_MAIN, 217);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_misc_enchantedscroll:30|t Палач", GOSSIP_SENDER_MAIN, 218);
                            player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                            player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
                            return true;
                        }
                        else
                        {
                            player->GetSession()->SendNotification("У вас не одето леворучное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;					

                case 14: //Remove enchant menu
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_1H_PVPPandariaS3_C_02:30|t Снять чары с правой руки", GOSSIP_SENDER_MAIN, 400);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_1h_DeathwingRaid_D_01:30|t Снять чары с левой руки", GOSSIP_SENDER_MAIN, 401);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_2h_DeathwingRaidDW_D_01:30|t Снять чары с двуручного оружия", GOSSIP_SENDER_MAIN, 413);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Shield_DeathwingRaid_D_02:30|t Снять чары с щита", GOSSIP_SENDER_MAIN, 412);
					player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_helmet_cloth_raidwarlock_l_01:30|t Снять чары с головы", GOSSIP_SENDER_MAIN, 402);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Shoulder_Cloth_RaidWarlock_L_01:30|t Снять чары с плеч", GOSSIP_SENDER_MAIN, 403);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Cape_Pandaria_DragonCaster_D_02:30|t Снять чары со спины", GOSSIP_SENDER_MAIN, 404);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Chest_Cloth_RaidWarlock_L_01:30|t Снять чары с груди", GOSSIP_SENDER_MAIN, 405);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Bracer_Cloth_RaidWarlock_L_01:30|t Снять чары с запястьев", GOSSIP_SENDER_MAIN, 406);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Glove_Cloth_RaidWarlock_L_01:30|t Снять чары с рук", GOSSIP_SENDER_MAIN, 407);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Pants_Cloth_RaidWarlock_L_01:30|t Снять чары с ног", GOSSIP_SENDER_MAIN, 408);
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Boot_Cloth_RaidWarlock_L_01:30|t Снять чары со ступней", GOSSIP_SENDER_MAIN, 409);
					if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
					{
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Jewelry_Ring_103:30|t Снять чары с кольца # 1", GOSSIP_SENDER_MAIN, 410);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Jewelry_Ring_103:30|t Снять чары с кольца # 2", GOSSIP_SENDER_MAIN, 411);
					}
                    player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/misc_arrowleft:30|t Назад", GOSSIP_SENDER_MAIN, 300);
                    player->PlayerTalkClass->SendGossipMenu(100014, creature->GetGUID());
                    return true;
                    break;

                case 100:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY_1H);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 101:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 102:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLADE_WARD);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 103:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLOOD_DRAINING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 104:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BERSERKING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 105:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ACCURACY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 106:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLACK_MAGIC);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 107:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BATTLEMASTER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 108:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEBREAKER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 109:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFEWARD);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 110:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_TITANGUARD);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 111:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POTENCY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 112:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 113:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MONGOOSE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 114:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_EXECUTIONER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 115:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                        if (!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужно двуручное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_GREATER_SPELL_POWER);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели двуручное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 116:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                        if (!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужно двуручное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели двуручное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 117:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

                        if (!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужно двуручное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MASSACRE);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели двуручное оружие");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 118:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_DEFENSE);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 119:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_INTELLECT);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 120:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_RESILIENCE);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 121:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM_PLATING);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 122:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAMINA);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 123:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TOUGHSHIELD);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 124:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BLISSFUL_MENDING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 125:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BURNING_MYSTERIES);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 126:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_DOMINANCE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 127:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_SAVAGE_GLADIATOR);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 128:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_STALWART_PROTECTOR);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 129:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TORMENT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 130:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TRIUMPH);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 131:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_ECLIPSED_MOON);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 132:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLAME_SOUL);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 133:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLEEING_SHADOW);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 134:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FROSTY_SOUL);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 135:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TOXIC_WARDING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 136:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_AXE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 137:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_CRAG);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 138:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_PINNACLE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 139:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_STORM);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 140:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_AXE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 141:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_CRAG);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 142:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_GLADIATOR);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 143:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_PINNACLE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 144:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_STORM);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 145:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_DOMINANCE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 146:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_TRIUMPH);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 147:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 148:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SHADOW_ARMOR);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 149:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 150:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 151:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 152:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_WISDOM);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 153:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_TITANWEAVE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 154:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPELL_PIERCING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 155:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MIGHTY_ARMOR);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 156:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MAJOR_AGILITY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 157:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPEED);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 158:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_POWERFUL_STATS);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 159:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_SUPER_HEALTH);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 160:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_MAINA_REST);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 161:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_RESIL);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 162:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_DEFENSE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 163:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STAMINA);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 164:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_SP);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 165:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_ASSUALT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 166:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_SPIRT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 167:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXPERTISE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 168:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_STATS);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 169:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 170:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ARCANE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 171:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FIRE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 172:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FROST);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 173:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_NATURE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 174:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SHADOW);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 175:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ATTACK);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 176:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_STAMINA);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 177:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SPELLPOWER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 178:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_BLASTING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 179:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_ARMSMAN);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 180:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_CRUSHER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 181:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 182:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_PRECISION);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 183:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_EXPERTISE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 184:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_EARTHEN);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 185:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_FROSTHIDE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 186:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_ICESCALE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 187:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_BRILLIANT_SPELLTHREAD);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 188:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_SAPPHIRE_SPELLTHREAD);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 189:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_DRAGONSCALE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 190:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_WYRMSCALE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 191:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_ASSULT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 192:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_TUSKARS_VITLIATY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 193:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SUPERIOR_AGILITY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 194:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_SPIRIT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 195:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_VITALITY);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 196:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ICEWALKER);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 197:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_FORTITUDE);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 198:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_NITRO_BOOTS);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 199:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_BOOTS_PYRO_ROCKET);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 200:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_HYPERSPEED);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 201:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ARMOR_WEBBING);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 202:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_ASSULT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 203:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_SP);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 204:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STAMINA);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
					
				case 2022:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_ASSULT);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 2032:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_SP);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 2042:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STAMINA);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 205:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLADE_WARD);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 206:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY_1H);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 207:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_SPIRIT);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 208:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BERSERKING);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 209:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ACCURACY);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 210:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLACK_MAGIC);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 211:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BATTLEMASTER);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 212:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEBREAKER);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 213:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LIFEWARD);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 214:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_TITANGUARD);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 215:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POTENCY);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 216:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 217:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_2WEP_MONGOOSE);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 218:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_EXECUTIONER);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 219:
                    {
                        item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

                        if(!item)
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                            return false;
                        }

                        if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
                        {
                            Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLOOD_DRAINING);
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                        else
                        {
                            player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
                            player->PlayerTalkClass->SendCloseGossip();
                        }
                    }
                    break;

                case 300: //|TInterface/ICONS/misc_arrowleft:30|t Назад menu
                    {
                        player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_1H_PVPPandariaS3_C_02:30|t Зачарить Правую руку", GOSSIP_SENDER_MAIN, 1);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_1h_DeathwingRaid_D_01:30|t Зачарить Левую руку", GOSSIP_SENDER_MAIN, 13);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Sword_2h_DeathwingRaidDW_D_01:30|t Зачарить Двуручное Оружие", GOSSIP_SENDER_MAIN, 2);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Shield_DeathwingRaid_D_02:30|t Зачарить Щит", GOSSIP_SENDER_MAIN, 3);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/inv_helmet_cloth_raidwarlock_l_01:30|t Зачарить Голову", GOSSIP_SENDER_MAIN, 4);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Shoulder_Cloth_RaidWarlock_L_01:30|t Зачарить Плечи", GOSSIP_SENDER_MAIN, 5);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Cape_Pandaria_DragonCaster_D_02:30|t Зачарить Спину", GOSSIP_SENDER_MAIN, 6);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Chest_Cloth_RaidWarlock_L_01:30|t Зачарить Грудь", GOSSIP_SENDER_MAIN, 7);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Bracer_Cloth_RaidWarlock_L_01:30|t Зачарить Запястья", GOSSIP_SENDER_MAIN, 8);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Glove_Cloth_RaidWarlock_L_01:30|t Зачарить Руки", GOSSIP_SENDER_MAIN, 9);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Pants_Cloth_RaidWarlock_L_01:30|t Зачарить Ноги", GOSSIP_SENDER_MAIN, 10);
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Boot_Cloth_RaidWarlock_L_01:30|t Зачарить Ступни", GOSSIP_SENDER_MAIN, 11);

						if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
						{
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Jewelry_Ring_104:30|t Зачарить Кольцо # 1", GOSSIP_SENDER_MAIN, 12);
							player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/INV_Jewelry_Ring_104:30|t Зачарить Кольцо # 2", GOSSIP_SENDER_MAIN, 1200);
						}
						player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/Ability_warlock_backdraftgreen:30|t Я хотел бы снять мои чары", GOSSIP_SENDER_MAIN, 14);
						player->PlayerTalkClass->SendGossipMenu(100001, creature->GetGUID());
						return true;
                    }
                        break;

                case 400: //Remove enchant for mainhand
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 401: //Remove enchant for offhand
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 402: //Remove enchant for head
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 403: //Remove enchant for shoulders
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 404: //remove enchant for cloak
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 405: //remove enchant for chest
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 406: //remove enchant for bracers
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 407: //remove enchant for gloves
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 408: //remove enchant for legs
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 409: //remove enchant for feet
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;

                case 410:
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1));                    
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
					
				case 411:
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
					
				case 412: // Снять чары со щита
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
					
				case 413: // Снять чары с двуручного оружия
                    RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND));
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
					
				/*case 414:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SOKET);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;
					
				case 415:
                    Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVERS_SOKET);
                    player->PlayerTalkClass->SendCloseGossip();
                    break;*/

                }
            return true;
        }
    };

class npc_welcome_new : public CreatureScript
{
public:
    npc_welcome_new() : CreatureScript("npc_welcome_new") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ability_deathknight_desolation:30|t Удалить стартовый хлам в персонаже", GOSSIP_SENDER_MAIN, 18);
		player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/inv_crate_04:30|t Получить 4 Сумки", GOSSIP_SENDER_MAIN, 17);
		player->ADD_GOSSIP_ITEM(10, "Шея", GOSSIP_SENDER_MAIN, 19);
		player->ADD_GOSSIP_ITEM(10, "Спина", GOSSIP_SENDER_MAIN, 20);
		player->ADD_GOSSIP_ITEM(10, "Запястья", GOSSIP_SENDER_MAIN, 21);
		player->ADD_GOSSIP_ITEM(10, "Пояс", GOSSIP_SENDER_MAIN, 22);
		player->ADD_GOSSIP_ITEM(10, "Трини", GOSSIP_SENDER_MAIN, 23);
		player->ADD_GOSSIP_ITEM(10, "Кольца", GOSSIP_SENDER_MAIN, 24);
		player->ADD_GOSSIP_ITEM(10, "Ступни", GOSSIP_SENDER_MAIN, 25);
		player->ADD_GOSSIP_ITEM(10, "Левая рука", GOSSIP_SENDER_MAIN, 26);
		player->ADD_GOSSIP_ITEM(10, "Дальний бой", GOSSIP_SENDER_MAIN, 27);
		player->ADD_GOSSIP_ITEM(10, "Реликвии", GOSSIP_SENDER_MAIN, 28);
		player->ADD_GOSSIP_ITEM(10, "Другое", GOSSIP_SENDER_MAIN, 29);
		
			/*if (player->getClass() == CLASS_WARRIOR)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Warrior:30|t Получить вещи для война", GOSSIP_SENDER_MAIN, 1);
			}
			if (player->getClass() == CLASS_PALADIN)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Paladin:30|t Получить вещи для паладина (хил)", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Paladin:30|t Получить вещи для паладина (дд)", GOSSIP_SENDER_MAIN, 3);
			}
			if (player->getClass() == CLASS_HUNTER)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Hunter:30|t Получить вещи для охотника", GOSSIP_SENDER_MAIN, 4);
			}
			if (player->getClass() == CLASS_ROGUE)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Rogue:30|t Получить вещи для разбойника", GOSSIP_SENDER_MAIN, 5);
			}
			if (player->getClass() == CLASS_PRIEST)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Priest:30|t Получить вещи для жреца (хил)", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Priest:30|t Получить вещи для жреца (рдд) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 7);
			}
			if (player->getClass() == CLASS_DEATH_KNIGHT)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_DeathKnight:30|t Получить вещи для рыцаря смерти |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 8);
			}
			if (player->getClass() == CLASS_SHAMAN)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Shaman:30|t Получить вещи для шамана (рдд) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 9);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Shaman:30|t Получить вещи для шамана (дд) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 10);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Shaman:30|t Получить вещи для шамана (хил) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 11);
			}
			if (player->getClass() == CLASS_MAGE)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Mage:30|t Получить вещи для мага |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 12);
			}
			if (player->getClass() == CLASS_WARLOCK)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Warlock:30|t Получить вещи для чернокнижника |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 13);
			}
			if (player->getClass() == CLASS_DRUID)
			{
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Druid:30|t Получить вещи для друида (спд) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 14);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Druid:30|t Получить вещи для друида (дд) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 15);
				player->ADD_GOSSIP_ITEM(10, "|TInterface/ICONS/ClassIcon_Druid:30|t Получить вещи для друида (хил) |cffff0000(в разработке)", GOSSIP_SENDER_MAIN, 16);
			}*/
			
		player->SEND_GOSSIP_MENU(1,creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();

		switch (action)
		{		
            /*case 1: // Воин
                player->AddItem(51227, 1); 				// Голова
				player->AddItem(plate_dd_nesk, 1); 		// Шея
				player->AddItem(51229, 1); 				// Плечи
				player->AddItem(plate_dd_back, 1); 		// Спина
				player->AddItem(51225, 1); 				// Грудак
				player->AddItem(49, 1); 				// Рубашка
				player->AddItem(plate_dd_bracers, 1); 	// Запястья
				player->AddItem(51226, 1); 				// Руки
				player->AddItem(plate_dd_belt, 1);		// Пояс
				player->AddItem(51228, 1); 				// Ноги
				player->AddItem(plate_dd_feet, 1); 		// Ступни
				player->AddItem(mili_ring_1, 1); 		// Кольцо # 1
				player->AddItem(mili_ring_2, 1); 		// Кольцо # 2
				player->AddItem(mili_trinket_1, 1); 	// Триня # 1
				player->AddItem(mili_trinket_2, 1); 	// Триня # 2
				player->AddItem(50730, 1); 				// Оружие (основа)				
				player->AddItem(50733, 1); 				// 3 слот оружия
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 2: // Пал хил
                player->AddItem(51272, 1); 				// Голова
				player->AddItem(plate_spd_nesk, 1); 	// Шея
				player->AddItem(51273, 1); 				// Плечи
				player->AddItem(plate_spd_back, 1); 	// Спина
				player->AddItem(51274, 1); 				// Грудак
				player->AddItem(42369, 1); 				// Рубашка
				player->AddItem(plate_spd_bracers, 1); 	// Запястья
				player->AddItem(51270, 1); 				// Руки
				player->AddItem(plate_spd_belt, 1);		// Пояс
				player->AddItem(51271, 1); 				// Ноги
				player->AddItem(plate_spd_feet, 1); 	// Ступни
				player->AddItem(heal_ring_1, 1); 		// Кольцо # 1
				player->AddItem(heal_ring_2, 1); 		// Кольцо # 2
				player->AddItem(heal_trinket_1, 1); 	// Триня # 1
				player->AddItem(heal_trinket_2, 1); 	// Триня # 2
				player->AddItem(50734, 1); 				// Оружие (основа)
				player->AddItem(50616, 1); 				// Оружие (левая рука)
				player->AddItem(51472, 1); 				// 3 слот оружия
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 3: // Пал дд
                player->AddItem(51277, 1); 				// Голова
				player->AddItem(plate_dd_nesk, 1); 		// Шея
				player->AddItem(51279, 1); 				// Плечи
				player->AddItem(plate_dd_back, 1); 		// Спина
				player->AddItem(51275, 1); 				// Грудак
				player->AddItem(49, 1); 				// Рубашка
				player->AddItem(plate_dd_bracers, 1); 	// Запястья
				player->AddItem(51276, 1); 				// Руки
				player->AddItem(plate_dd_belt, 1);		// Пояс
				player->AddItem(51278, 1); 				// Ноги
				player->AddItem(plate_dd_feet, 1); 		// Ступни
				player->AddItem(mili_ring_1, 1); 		// Кольцо # 1
				player->AddItem(mili_ring_2, 1); 		// Кольцо # 2
				player->AddItem(mili_trinket_1, 1); 	// Триня # 1
				player->AddItem(mili_trinket_2, 1); 	// Триня # 2
				player->AddItem(50730, 1); 				// Оружие (основа)				
				player->AddItem(51478, 1); 				// 3 слот оружия
				player->CLOSE_GOSSIP_MENU();
                break;
			
            case 4: // Охотник
                player->AddItem(51286, 1); 				// Голова
				player->AddItem(mail_dd_nesk, 1);		// Шея
				player->AddItem(51288, 1);				// Плечи
				player->AddItem(mail_dd_back, 1);		// Спина
				player->AddItem(51289, 1);				// Грудак
				player->AddItem(148, 1);				// Рубашка
				player->AddItem(mail_dd_bracers, 1);	// Запястья
				player->AddItem(51285, 1);				// Руки
				player->AddItem(mail_dd_belt, 1);		// Пояс
				player->AddItem(51287, 1);				// Ноги
				player->AddItem(mail_dd_feet, 1);		// Ступни
				player->AddItem(agility_ring_1, 1);		// Кольцо # 1
				player->AddItem(agility_ring_2, 1);		// Кольцо # 2
				player->AddItem(mili_trinket_1, 1);		// Триня # 1
				player->AddItem(mili_trinket_2, 1);		// Триня # 2
				player->AddItem(50737, 2);				// Оружие (основа)	
				player->AddItem(50733, 1);				// 3 слот оружия
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 5: // Разбойник
                player->AddItem(51252, 1); 				// Голова
				player->AddItem(leather_dd_nesk, 1);	// Шея
				player->AddItem(51254, 1);				// Плечи
				player->AddItem(leather_dd_back, 1);	// Спина
				player->AddItem(51250, 1);				// Грудак
				player->AddItem(148, 1);				// Рубашка
				player->AddItem(leather_dd_bracers, 1);	// Запястья
				player->AddItem(51251, 1);				// Руки
				player->AddItem(leather_dd_belt, 1);	// Пояс
				player->AddItem(51253, 1);				// Ноги
				player->AddItem(leather_dd_feet, 1);	// Ступни
				player->AddItem(agility_ring_1, 1);		// Кольцо # 1
				player->AddItem(agility_ring_2, 1);		// Кольцо # 2
				player->AddItem(mili_trinket_1, 1);		// Триня # 1
				player->AddItem(mili_trinket_2, 1);		// Триня # 2
				player->AddItem(50736, 2);				// Оружие (основа)	
				player->AddItem(50733, 1);				// 3 слот оружия
				player->CLOSE_GOSSIP_MENU();
                break;

			case 6: // Жрец хил 
                player->AddItem(51261, 1); 				// Голова
				player->AddItem(cloth_heal_nesk, 1); 	// Шея
				player->AddItem(51264, 1); 				// Плечи
				player->AddItem(cloth_heal_back, 1); 	// Спина
				player->AddItem(51263, 1); 				// Грудак
				player->AddItem(42369, 1); 				// Рубашка
				player->AddItem(cloth_heal_bracers, 1); 	// Запястья
				player->AddItem(51260, 1); 				// Руки
				player->AddItem(cloth_heal_belt, 1);		// Пояс
				player->AddItem(51262, 1); 				// Ноги
				player->AddItem(cloth_heal_feet, 1); 	// Ступни
				player->AddItem(heal_ring_1, 1); 		// Кольцо # 1
				player->AddItem(heal_ring_2, 1); 		// Кольцо # 2
				player->AddItem(heal_trinket_1, 1); 	// Триня # 1
				player->AddItem(heal_trinket_2, 1); 	// Триня # 2
				player->AddItem(50734, 1); 				// Оружие (основа)
				player->AddItem(50635, 1); 				// Оружие (левая рука)
				player->AddItem(50631, 1); 				// 3 слот оружия
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 7: // Жрец дд
                
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 8: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;
			
            case 9: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;

			case 10: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 11: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 12: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;
			
            case 13: // Варлок
					
				player->AddItem(51231, 1);
				player->AddItem(50724, 1);
				player->AddItem(51234, 1);
				player->AddItem(50628, 1);
				player->AddItem(51233, 1);
				player->AddItem(38, 1);
				player->AddItem(50651, 1);
				player->AddItem(51230, 1);
				player->AddItem(50613, 1);
				player->AddItem(51232, 1);
				player->AddItem(50699, 1);
				player->AddItem(50714, 1);
				player->AddItem(50720, 1);
				player->AddItem(54588, 1);
				player->AddItem(50365, 1);
				player->AddItem(50732, 1);
				player->AddItem(50719, 1);
				player->AddItem(50684, 1);
                player->CLOSE_GOSSIP_MENU();
                break;
				
			case 14: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;
				
            case 15: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;
			
            case 16: // 
                
				player->CLOSE_GOSSIP_MENU();
                break;*/
				
			case 17: // Сумки с проверкой на наличие
			
                if (player->HasItemCount(23162, 4))
				{
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{				
					player->AddItem(23162, 4);
					player->CLOSE_GOSSIP_MENU();
				}
                break;
				
			case 18: // Удаление хлама в персонаже
                if (player->HasItemCount(38145, 1)) // проверка на дк
				{	
					player->DestroyItemCount(38145, 4, true);
					player->DestroyItemCount(34652, 1, true);
					player->DestroyItemCount(34657, 1, true);
					player->DestroyItemCount(34655, 1, true);
					player->DestroyItemCount(34659, 1, true);
					player->DestroyItemCount(34650, 1, true);
					player->DestroyItemCount(34653, 1, true);
					player->DestroyItemCount(34649, 1, true);
					player->DestroyItemCount(34651, 1, true);
					player->DestroyItemCount(34656, 1, true);
					player->DestroyItemCount(34648, 1, true);
					player->DestroyItemCount(34658, 1, true);
					player->DestroyItemCount(38147, 1, true);
					player->DestroyItemCount(41751, 10, true);
				}
				
				if (player->HasItemCount(2101, 1))
				{
					player->DestroyItemCount(2101, 1, true);
					player->DestroyItemCount(2512, 200, true);
				}
				
				if (player->HasItemCount(2102, 1))
				{
					player->DestroyItemCount(2102, 1, true);
					player->DestroyItemCount(2516, 200, true);
				}				
				
				player->CLOSE_GOSSIP_MENU();
                break;
				
				case 19: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123000);}
				
				case 20: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123001);}
				
				case 21: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123002);}
				
				case 22: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123003);}
				
				case 23: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123004);}
				
				case 24: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123005);}
				
				case 25: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123006);}
				
				case 26: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123007);}
				
				case 27: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123008);}
				
				case 28: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123009);}
				
				case 29: //                 
				{player->GetSession()->SendListInventory(creature->GetGUID(), 123010);}
                break;
				
		}
		return true;
	}
};

class ralph : public CreatureScript
{
public:
	ralph() : CreatureScript("ralph") {}	

void CreatePet(Player* player, Creature* creature, uint32 entry)
    {
        Creature* creatureTarget = creature->SummonCreature(entry, player->GetPositionX(), player->GetPositionY() +2, player->GetPositionZ(), player->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 500);
        if(!creatureTarget)
            return;

        Pet* pet = player->CreateTamedPetFrom(creatureTarget, 0);
        if(!pet)
            return;

        creatureTarget->setDeathState(JUST_DIED);
        creatureTarget->RemoveCorpse();
        creatureTarget->SetHealth(0);

        pet->SetPower(POWER_HAPPINESS, 10480000);

        pet->SetUInt64Value(UNIT_FIELD_CREATEDBY, player->GetGUID());
        pet->SetUInt64Value(UNIT_FIELD_FACTIONTEMPLATE, player->getFaction());
        pet->SetUInt64Value(UNIT_FIELD_LEVEL, player->getLevel());

        pet->GetMap()->AddToMap(pet->ToCreature());

        pet->GetCharmInfo()->SetPetNumber(sObjectMgr->GeneratePetNumber(), true);

        pet->UpdateAllStats();
        player->SetMinion(pet, true);

        pet->SavePetToDB(PET_SAVE_AS_CURRENT);
		pet->InitTalentForLevel();
		player->PetSpellInitialize();
				
		player->CLOSE_GOSSIP_MENU();
		creature->MonsterWhisper("Поздравляю, вы получили нового питомца", player->GetGUID());
	}
			
	bool OnGossipHello(Player* player, Creature* creature)
	{
		if(player->getClass() != CLASS_HUNTER)
		{
			creature->MonsterWhisper("Я могу обучать только охотников", player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			return false;
		}
		
		if(player->GetPet())
		{
			creature->MonsterWhisper("Вы должны отпустить или оставить своего текущего питомца, чтобы получить нового", player->GetGUID());
			player->CLOSE_GOSSIP_MENU();
			return false; 
		}
				
		player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/ability_hunter_beasttaming:30|t Получить питомца", GOSSIP_SENDER_MAIN, 30);
		if (player->CanTameExoticPets())
			player->ADD_GOSSIP_ITEM(1, "|TInterface/ICONS/ability_hunter_beastmastery:30|t Получить экзотического питомца", GOSSIP_SENDER_MAIN, 50);
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
			return true;
	}
			
	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
					
		switch(action)
		{
					
			case 100:
				OnGossipHello(player, creature);
				break;
						
			case 150:
				player->CLOSE_GOSSIP_MENU();
				break;
								
			case 30:
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_bat:30|t Летучая мышь", GOSSIP_SENDER_MAIN, 18);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_bear:30|t Медведь", GOSSIP_SENDER_MAIN, 1);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_boar:30|t Вепрь", GOSSIP_SENDER_MAIN, 2);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_cat:30|t Кошка", GOSSIP_SENDER_MAIN, 4);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_vulture:30|t Падальщик", GOSSIP_SENDER_MAIN, 5);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_crab:30|t Краб", GOSSIP_SENDER_MAIN, 6);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_crocolisk:30|t Кроколиск", GOSSIP_SENDER_MAIN, 7);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_dragonhawk:30|t Дракондор", GOSSIP_SENDER_MAIN, 17);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_gorilla:30|t Горилла", GOSSIP_SENDER_MAIN, 8);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_wolf:30|t Волк", GOSSIP_SENDER_MAIN, 9);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_hyena:30|t Гиена", GOSSIP_SENDER_MAIN, 10);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_moth:30|t Мотылёк", GOSSIP_SENDER_MAIN, 11);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_owl:30|t Сова", GOSSIP_SENDER_MAIN, 12);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_raptor:30|t Ящер", GOSSIP_SENDER_MAIN, 20);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_ravager:30|t Опустошитель", GOSSIP_SENDER_MAIN, 19);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_tallstrider:30|t Долгоног", GOSSIP_SENDER_MAIN, 13);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_scorpid:30|t Скорпион", GOSSIP_SENDER_MAIN, 414);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_spider:30|t Паук", GOSSIP_SENDER_MAIN, 16);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/spell_nature_guardianward:30|t Змей", GOSSIP_SENDER_MAIN, 21);  
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_turtle:30|t Черепаха", GOSSIP_SENDER_MAIN, 15);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_wasp:30|t Оса", GOSSIP_SENDER_MAIN, 93);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_warpstalker:30|t Прыгуана", GOSSIP_SENDER_MAIN, 94);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_netherray:30|t Скат пустоты", GOSSIP_SENDER_MAIN, 95);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_sporebat:30|t Спороскат", GOSSIP_SENDER_MAIN, 96);
				player->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:|t Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;

			case 50:
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_chimera:30|t Химера", GOSSIP_SENDER_MAIN, 51);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_corehound:30|t Гончая Недр", GOSSIP_SENDER_MAIN, 52);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_devilsaur:30|t Дьявозавр", GOSSIP_SENDER_MAIN, 53);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/Ability_Hunter_Pet_Rhino:30|t Люторог", GOSSIP_SENDER_MAIN, 54);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_silithid:30|t Силитид", GOSSIP_SENDER_MAIN, 55);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_hunter_pet_worm:30|t Червь", GOSSIP_SENDER_MAIN, 56);  
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_druid_primalprecision:30|t Локе'нахак", GOSSIP_SENDER_MAIN, 57);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_druid_primalprecision:30|t Сколл", GOSSIP_SENDER_MAIN, 58);
				player->ADD_GOSSIP_ITEM(6, "|TInterface/ICONS/ability_druid_primalprecision:30|t Дух зверя.", GOSSIP_SENDER_MAIN, 59);
				player->ADD_GOSSIP_ITEM(4, "|TInterface/ICONS/Achievement_BG_returnXflags_def_WSG:30:|t Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
							
			// Credits to Serifaz who helped me with these ID's.
			case 51: // Химера
				CreatePet(player, creature, 21879);
				break;
									
			case 52: // Гончая Недр
				CreatePet(player, creature, 21108);
				break;
									
			case 53: // Дьявозавр
				CreatePet(player, creature, 20931);
				break;
								
			case 54: // Люторог
				CreatePet(player, creature, 30445);
				break;
								
			case 55: // Silithid
				CreatePet(player, creature, 5460);
				break;
								
			case 56: // Worm
				CreatePet(player, creature, 30148);
				break;
								
			case 57: // Loque'nahak
				CreatePet(player, creature, 32517);
				break;
								
			case 58: // Skoll
				CreatePet(player, creature, 35189);
				break;
								
			case 59: // Gondria
				CreatePet(player, creature, 33776);
				break;
								
			case 16: // Spider
				CreatePet(player, creature, 2349);
				break;
								
			case 17: // Dragonhawk
				CreatePet(player, creature, 27946);
				break;
								
			case 18: // Bat
				CreatePet(player, creature, 28233);
				break;
									
			case 19: // Ravager
				CreatePet(player, creature, 17199);
				break;
									
			case 20: // Raptor
				CreatePet(player, creature, 14821);
				break;
									
			case 21: // Serpent
				CreatePet(player, creature, 28358);
				break;
									
			case 1: // Bear
				CreatePet(player, creature, 29319);
				break;
									
			case 2: // Boar
				CreatePet(player, creature, 29996);
				break;
									
			case 93: // Bug
				CreatePet(player, creature, 28085);
				break;
									
			case 4: // Cat
				CreatePet(player, creature, 28097);
				break;
									
			case 5: // Carrion
				CreatePet(player, creature, 26838);
				break;
									
			case 6: // Crab
				CreatePet(player, creature, 24478);
				break;  
									
			case 7: // Crocolisk
				CreatePet(player, creature, 1417);
				break;  
									
			case 8: // Gorila
				CreatePet(player, creature, 28213);
				break;
									
			case 9: // Hound
				CreatePet(player, creature, 29452);
				break;
									
			case 10: // Hyena
				CreatePet(player, creature, 13036);
				break;
									
			case 11: // Moth
				CreatePet(player, creature, 27421);
				break;
									
			case 12: // Owl
				CreatePet(player, creature, 23136);
				break;
									
			case 13: // Strider
				CreatePet(player, creature, 22807);
				break;
									
			case 414: // Scorpid
				CreatePet(player, creature, 9698);
				break;
									
			case 15: // Turtle
				CreatePet(player, creature, 25482);
				break;
				
			case 94: // Прыгуана
				CreatePet(player, creature, 18464);
				break;
			
			case 95: // Скат пустоты 
				CreatePet(player, creature, 18880);
				break;
				
			case 96: // Скат пустоты 
				CreatePet(player, creature, 17724);
				break;	
		}
					
	return true;
	}
};

class npc_welcome : public CreatureScript
{
public:
	npc_welcome() : CreatureScript("npc_welcome") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		if (player->getLevel() >= 80)
		{
			{
				player->PrepareQuestMenu(creature->GetGUID());
				player->SendPreparedQuest(creature->GetGUID());
			}
			if (player->getClass() == CLASS_WARRIOR)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Warrior:30|t Пройти обучение война", GOSSIP_SENDER_MAIN, MENU_WARRIOR);
			}
			if (player->getClass() == CLASS_PALADIN)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Paladin:30|t Пройти обучение паладина", GOSSIP_SENDER_MAIN, MENU_PALADIN);
			}
			if (player->getClass() == CLASS_HUNTER)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Hunter:30|t Пройти обучение охотника", GOSSIP_SENDER_MAIN, MENU_HUNTER);
			}
			if (player->getClass() == CLASS_ROGUE)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Rogue:30|t Пройти обучение разбойника", GOSSIP_SENDER_MAIN, MENU_ROGUE);
			}
			if (player->getClass() == CLASS_PRIEST)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Priest:30|t Пройти обучение жреца", GOSSIP_SENDER_MAIN, MENU_PRIEST);
			}
			if (player->getClass() == CLASS_DEATH_KNIGHT)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_DeathKnight:30|t Пройти обучение рыцаря смерти", GOSSIP_SENDER_MAIN, MENU_DEATH_K);
			}
			if (player->getClass() == CLASS_SHAMAN)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Shaman:30|t Пройти обучение шамана", GOSSIP_SENDER_MAIN, MENU_SHAMAN);
			}
			if (player->getClass() == CLASS_MAGE)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Mage:30|t Пройти обучение мага", GOSSIP_SENDER_MAIN, MENU_MAGE);
			}
			if (player->getClass() == CLASS_WARLOCK)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Warlock:30|t Пройти обучение чернокнижника", GOSSIP_SENDER_MAIN, MENU_WARLOCK);
			}
			if (player->getClass() == CLASS_DRUID)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Druid:30|t Пройти обучение друида", GOSSIP_SENDER_MAIN, MENU_DRUID);
			}

			player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ability_rogue_dirtydeeds:30|t Меню талантов", GOSSIP_SENDER_MAIN, OPTION_MENU_ID_1);
			player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/Ability_Mount_FelDrake:30|t Изучить верховую езду", GOSSIP_SENDER_MAIN, OPTION_MENU_ID_2);			
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		}
		else
		{
			player->PlayerTalkClass->SendCloseGossip();
			return false;
		}
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
	{
		WorldSession * PGS = player->GetSession();

		player->PlayerTalkClass->ClearMenus();
		{
			//	/**DUAL SPEC*////////////////////////////////////////////////
			player->CastSpell(player,63680,true,NULL,NULL,player->GetGUID());
			player->CastSpell(player,63624,true,NULL,NULL,player->GetGUID());
			//	/**//////////////////////////////////////////////////////////
			switch(action)
			{
				//=============================================WARRIOR=================================================//
			case MENU_WARRIOR:
				player->learnSpell(war_spell_Overpower, false),
					player->learnSpell(war_spell_Battle_Shout, false),
					player->learnSpell(war_spell_Heroic_Strike, false),
					player->learnSpell(war_spell_Charge, false),
					player->learnSpell(war_spell_Rend, false),
					player->learnSpell(war_spell_Thunder_Clap, false),
					player->learnSpell(war_spell_Victory_Rush, false),
					player->learnSpell(war_spell_Hamstring, false),
					player->learnSpell(war_spell_Bloodrage, false),
					player->learnSpell(war_spell_Defensive_Stance, false),
					player->learnSpell(war_spell_Sunder_Armor, false),
					player->learnSpell(war_spell_Taunt, false),
					player->learnSpell(war_spell_Shield_Bash, false),
					player->learnSpell(war_spell_Demoralizing_Shout, false),
					player->learnSpell(war_spell_Revenge, false),
					player->learnSpell(war_spell_Mocking_Blow, false),
					player->learnSpell(war_spell_Shield_Block, false),
					player->learnSpell(war_spell_Disarm, false),
					player->learnSpell(war_spell_Cleave, false),
					player->learnSpell(war_spell_Retaliation, false),
					player->learnSpell(war_spell_Stance_Mastery, false),
					player->learnSpell(war_spell_Execute, false),
					player->learnSpell(war_spell_Challenging_Shout, false),
					player->learnSpell(war_spell_Shield_Wall, false),
					player->learnSpell(war_spell_Berserker_Stance, false),
					player->learnSpell(war_spell_Intercept, false),
					player->learnSpell(war_spell_Slam, false),
					player->learnSpell(war_spell_Berserker_Rage, false),
					player->learnSpell(war_spell_Whirlwind, false),
					player->learnSpell(war_spell_Pummel, false),
					player->learnSpell(war_spell_Sheild_Slam, false),
					player->learnSpell(war_spell_Recklessness, false),
					player->learnSpell(war_spell_Spell_Reflection, false),
					player->learnSpell(war_spell_Commanding_Shout, false),
					player->learnSpell(war_spell_Intervene, false),
					player->learnSpell(war_spell_Shattering_Throw, false),
					player->learnSpell(war_spell_Enraged_Regeneration, false),
					player->learnSpell(war_spell_Heroic_Throw, false),
					player->learnSpell(war_spell_Dual_Wield, false),
					player->learnSpell(war_spell_Intimidating_Shout, false);
				player->learnSpell(war_spell_Parry, false);
				player->learnSpell(54197, false); // ?
				/////////WEAPON/////////////
				player->learnSpell(750, false); // ?
				player->learnSpell(196, false); // ?
				player->learnSpell(197, false); // ?
				player->learnSpell(198, false); // ?
				player->learnSpell(199, false); // ?
				player->learnSpell(200, false); // ?
				player->learnSpell(201, false); // ?
				player->learnSpell(202, false); // ?
				player->learnSpell(264, false); // ?
				player->learnSpell(5011, false); // ?
				player->learnSpell(266, false); // ?
				player->learnSpell(2567, false); // ?
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================PALADIN=================================================//
			case MENU_PALADIN:
				player->learnSpell(pala_spell_Parry, false);
				player->learnSpell(pala_spell_Concentration_Aura, false);
				player->learnSpell(pala_spell_Divine_Intervention, false);
				player->learnSpell(pala_spell_Plate_Mail_skill,   false);
				player->learnSpell(pala_spell_Devotion_Aura, false);
				player->learnSpell(pala_spell_Holy_Light, false);
				player->learnSpell(pala_spell_Blessing_of_Might, false);
				player->learnSpell(pala_spell_Judgement_of_Light, false);
				player->learnSpell(pala_spell_Divine_Protection,   false);
				player->learnSpell(pala_spell_Hammer_of_Justice, false);
				player->learnSpell(pala_spell_Purify,  false);
				player->learnSpell(pala_spell_Hand_of_Protection, false);
				player->learnSpell(pala_spell_Lay_on_Hands, false);
				player->learnSpell(pala_spell_Judgement_of_Wisdom, false);
				player->learnSpell(pala_spell_Redemption, false);
				player->learnSpell(pala_spell_Blessing_of_Wisdom, false);
				player->learnSpell(pala_spell_Righteous_Defense, false);
				player->learnSpell(pala_spell_Hand_of_Reckoning, false);
				player->learnSpell(pala_spell_Retribution_Aura, false);
				player->learnSpell(pala_spell_Righteous_Fury, false);
				player->learnSpell(pala_spell_Hand_of_Freedom,  false);
				player->learnSpell(pala_spell_Blessing_of_Kings, false);
				player->learnSpell(pala_spell_Consecration, false);
				player->learnSpell(pala_spell_Exorcism, false);
				player->learnSpell(pala_spell_Flash_of_Light, false);
				player->learnSpell(pala_spell_Sense_Undead,  false);
				player->learnSpell(pala_spell_Seal_of_Justice, false);
				player->learnSpell(pala_spell_Turn_Evil, false);
				player->learnSpell(pala_spell_Hand_of_Salvation,  false);
				player->learnSpell(pala_spell_Judgement_of_Justice, false);
				player->learnSpell(pala_spell_Shadow_Resistance_Aura, false);
				player->learnSpell(pala_spell_Seal_of_Light, false);
				player->learnSpell(pala_spell_Frost_Resistance_Aura, false);
				player->learnSpell(pala_spell_Divine_Shield,   false);
				player->learnSpell(pala_spell_Fire_Resistance_Aura, false);
				player->learnSpell(pala_spell_Seal_of_Wisdom, false);
				player->learnSpell(pala_spell_Cleanse,  false);
				player->learnSpell(pala_spell_Hammer_of_Wrath, false);
				player->learnSpell(pala_spell_Hand_of_Sacrifice,  false);
				player->learnSpell(pala_spell_Holy_Wrath, false);
				player->learnSpell(pala_spell_Greater_Blessing_of_Might, false);
				player->learnSpell(pala_spell_Greater_Blessing_of_Wisdom, false);
				player->learnSpell(pala_spell_Greater_Blessing_of_Kings, false);
				player->learnSpell(pala_spell_Crusader_Aura, false);
				player->learnSpell(pala_spell_Avenging_Wrath, false);
				player->learnSpell(pala_spell_Divine_Plea, false);
				player->learnSpell(pala_spell_Shield_of_Righteousness, false);
				player->learnSpell(pala_spell_Sacred_Shield, false);
				player->learnSpell(33388, false); //?
				player->learnSpell(33391, false); //?
				/////////WEAPONS/////////////
				player->learnSpell(750, false); //?
				player->learnSpell(196, false); //?
				player->learnSpell(197, false); //?
				player->learnSpell(198, false); //?
				player->learnSpell(199, false); //?
				player->learnSpell(200, false); //?
				player->learnSpell(201, false); //?
				player->learnSpell(202, false);	//?	 		
				//Spell Learned for different factions..
				if(player->GetTeam() == ALLIANCE){
					player->learnSpell(pala_spell_Seal_of_Vengeance, false);
					player->learnSpell(pala_spell_Warhorse, false);
					player->learnSpell(pala_spell_Charger, false);
				}
				else
				{
					player->learnSpell(pala_spell_Seal_of_Corruption, false);
					player->learnSpell(pala_spell_Thalassian_Warhorse, false);
					player->learnSpell(pala_spell_Thalassian_Charger, false);
				}
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================HUNTER=================================================//
			case MENU_HUNTER:
				player->learnSpell(hunter_spell_Scorpid_Sting, false);
				player->learnSpell(hunter_spell_Parry, false);
				player->learnSpell(hunter_spell_Rapid_Fire,  false);
				player->learnSpell(hunter_spell_Viper_Sting,  false);
				player->learnSpell(hunter_spell_Track_Beasts,  false);
				player->learnSpell(hunter_spell_Aspect_of_the_Monkey, false);
				player->learnSpell(hunter_spell_Raptor_Strike, false);
				player->learnSpell(hunter_spell_Serpent_Sting, false);
				player->learnSpell(hunter_spell_Arcane_Shot, false);
				player->learnSpell(hunter_spell_Hunters_Mark, false);
				player->learnSpell(hunter_spell_Concussive_Shot,  false);
				player->learnSpell(hunter_spell_Aspect_of_the_Hawk, false);
				player->learnSpell(hunter_spell_Call_Pet,   false);
				player->learnSpell(hunter_spell_Dismiss_Pet,  false);
				player->learnSpell(hunter_spell_Feed_Pet,  false);
				player->learnSpell(hunter_spell_Revive_Pet,   false);
				player->learnSpell(hunter_spell_Tame_Beast,  false);
				player->learnSpell(hunter_spell_Track_Humanoids, false);
				player->learnSpell(hunter_spell_Distracting_Shot, false);
				player->learnSpell(hunter_spell_Mend_Pet, false);
				player->learnSpell(hunter_spell_Wing_Clip,  false);
				player->learnSpell(hunter_spell_Eagle_Eye,  false);
				player->learnSpell(hunter_spell_Eyes_of_the_Beast,  false);
				player->learnSpell(hunter_spell_Scare_Beast, false);
				player->learnSpell(hunter_spell_Aspect_of_the_Cheetah,  false);
				player->learnSpell(hunter_spell_Immolation_Trap, false);
				player->learnSpell(hunter_spell_Mongoose_Bite, false);
				player->learnSpell(hunter_spell_Multi_Shot, false);
				player->learnSpell(hunter_spell_Track_Undead, false);
				player->learnSpell(hunter_spell_Aspect_of_the_Viper, false);
				player->learnSpell(hunter_spell_Disengage,   false);
				player->learnSpell(hunter_spell_Freezing_Trap, false);
				player->learnSpell(hunter_spell_Beast_Lore,  false);
				player->learnSpell(hunter_spell_Track_Hidden, false);
				player->learnSpell(hunter_spell_Track_Elementals, false);
				player->learnSpell(hunter_spell_Frost_Trap, false);
				player->learnSpell(hunter_spell_Aspect_of_the_Beast, false);
				player->learnSpell(hunter_spell_Feign_Death,  false);
				player->learnSpell(hunter_spell_Flare,  false);
				player->learnSpell(hunter_spell_Track_Demons, false);
				player->learnSpell(hunter_spell_Explosive_Trap, false);
				player->learnSpell(hunter_spell_Aspect_of_the_Pack, false);
				player->learnSpell(hunter_spell_Track_Giants, false);
				player->learnSpell(hunter_spell_Volley, false);
				player->learnSpell(hunter_spell_Aspect_of_the_Wild, false);
				player->learnSpell(hunter_spell_Steady_Shot, false);
				player->learnSpell(hunter_spell_Track_Dragonkin, false);
				player->learnSpell(hunter_spell_Deterrence, false);
				player->learnSpell(hunter_spell_Tranquilizing_Shot, false);
				player->learnSpell(hunter_spell_Kill_Command, false);
				player->learnSpell(hunter_spell_Snake_Trap, false);
				player->learnSpell(hunter_spell_Misdirection, false);
				player->learnSpell(hunter_spell_Kill_Shot, false);
				player->learnSpell(hunter_spell_Aspect_of_the_Dragonhawk, false);
				player->learnSpell(hunter_spell_Masters_Call, false);
				player->learnSpell(hunter_spell_Freezing_Arrow, false);
				player->learnSpell(hunter_spell_Call_Stabled_Pet, false);
				player->learnSpell(hunter_spell_Dual_Wield, false);
				player->learnSpell(8737,  false); //?
				/////////ARRMES/////////////
				player->learnSpell(15590, false); //?
				player->learnSpell(8737, false); //?
				player->learnSpell(196, false); //?
				player->learnSpell(197, false); //?
				player->learnSpell(200, false); //?
				player->learnSpell(201, false); //?
				player->learnSpell(202, false); //?
				player->learnSpell(264, false); //?
				player->learnSpell(266, false); //?
				player->learnSpell(5011, false); //?
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================ROGUE=================================================//
			case MENU_ROGUE:
				player->learnSpell(rogue_spell_Parry, false);
				player->learnSpell(rogue_spell_Dual_Wield, false); //  ambi
				player->learnSpell(rogue_spell_Eviscerate, false);
				player->learnSpell(rogue_spell_Sinister_Strike, false);
				player->learnSpell(rogue_spell_Stealth, false);
				player->learnSpell(rogue_spell_Backstab, false);
				player->learnSpell(rogue_spell_Pick_Pocket, false);
				player->learnSpell(rogue_spell_Gouge, false);
				player->learnSpell(rogue_spell_Evasion, false);
				player->learnSpell(rogue_spell_Sap, false);
				player->learnSpell(rogue_spell_Slice_and_Dice, false);
				player->learnSpell(rogue_spell_Sprint, false);
				player->learnSpell(rogue_spell_Kick, false);
				player->learnSpell(rogue_spell_Garrote, false);
				player->learnSpell(rogue_spell_Feint, false);
				player->learnSpell(rogue_spell_Lockpicking_skill, false);
				player->learnSpell(rogue_spell_Expose_Armor, false);
				player->learnSpell(rogue_spell_Ambush, false);
				player->learnSpell(rogue_spell_Dismantle, false);
				player->learnSpell(rogue_spell_Rupture, false);
				player->learnSpell(rogue_spell_Distract, false);
				player->learnSpell(rogue_spell_Vanish, false);
				player->learnSpell(rogue_spell_Detect_Traps, false);
				player->learnSpell(rogue_spell_Cheap_Shot, false);
				player->learnSpell(rogue_spell_Disarm_Trap, false);
				player->learnSpell(rogue_spell_Kidney_Shot, false);
				player->learnSpell(rogue_spell_Blind, false);
				player->learnSpell(rogue_spell_Safe_Fall, false);
				player->learnSpell(rogue_spell_Envenom, false);
				player->learnSpell(rogue_spell_Deadly_Throw, false);
				player->learnSpell(rogue_spell_Cloak_of_Shadows, false);
				player->learnSpell(rogue_spell_Shiv, false);
				player->learnSpell(rogue_spell_Tricks_of_the_Trade, false);
				player->learnSpell(rogue_spell_Fan_of_Knives, false);
				///////////WEAPON/////////////
				player->learnSpell(196, false); //? 
				player->learnSpell(198, false); //? 
				player->learnSpell(201, false); //? 
				player->learnSpell(1180, false); //? 
				player->learnSpell(2567, false); //? 
				player->learnSpell(264, false); //? 
				player->learnSpell(266, false); //? 
				player->learnSpell(5011, false); //? 
				player->learnSpell(15590, false); //? 
				//======================================//
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
			case MENU_PRIEST:
				player->learnSpell(priest_spell_Cure_Disease, false);
				player->learnSpell(priest_spell_Lesser_Heal, false);
				player->learnSpell(priest_spell_Power_Word_Fortitude, false);
				player->learnSpell(priest_spell_Smite, false);
				player->learnSpell(priest_spell_Shadow_Word_Pain, false);
				player->learnSpell(priest_spell_Power_Word_Shield, false);
				player->learnSpell(priest_spell_Fade, false);
				player->learnSpell(priest_spell_Renew, false);
				player->learnSpell(priest_spell_Mind_Blast, false);
				player->learnSpell(priest_spell_Resurrection, false);
				player->learnSpell(priest_spell_Inner_Fire, false);
				player->learnSpell(priest_spell_Psychic_Scream, false);
				player->learnSpell(priest_spell_Heal, false);
				player->learnSpell(priest_spell_Dispel_Magic, false);
				player->learnSpell(priest_spell_Devouring_Plague, false);
				player->learnSpell(priest_spell_Fear_Ward, false);
				player->learnSpell(priest_spell_Flash_Heal, false);
				player->learnSpell(priest_spell_Holy_Fire, false);
				player->learnSpell(priest_spell_Holy_Nova, false);
				player->learnSpell(priest_spell_Mind_Soothe, false);
				player->learnSpell(priest_spell_Shackle_Undead, false);
				player->learnSpell(priest_spell_Mind_Vision, false);
				player->learnSpell(priest_spell_Mana_Burn, false);
				player->learnSpell(priest_spell_Divine_Spirit, false);
				player->learnSpell(priest_spell_Mind_Control, false);
				player->learnSpell(priest_spell_Prayer_of_Healing, false);
				player->learnSpell(priest_spell_Shadow_Protection, false);
				player->learnSpell(priest_spell_Abolish_Disease, false);
				player->learnSpell(priest_spell_Levitate, false);
				player->learnSpell(priest_spell_Greater_Heal, false);
				player->learnSpell(priest_spell_Prayer_of_Fortitude, false);
				player->learnSpell(priest_spell_Prayer_of_Shadow_Protection, false);
				player->learnSpell(priest_spell_Prayer_of_Spirit, false);
				player->learnSpell(priest_spell_Shadow_Word_Death, false);
				player->learnSpell(priest_spell_Binding_Heal, false);
				player->learnSpell(priest_spell_Shadowfiend, false);
				player->learnSpell(priest_spell_Prayer_of_Mending, false);
				player->learnSpell(priest_spell_Mass_Dispel, false);
				player->learnSpell(priest_spell_Divine_Hymn, false);
				player->learnSpell(priest_spell_Hymn_of_Hope, false);
				player->learnSpell(priest_spell_Mind_Sear, false);
				///////////WEAPON/////////////
				player->learnSpell(196, false); //?
				player->learnSpell(198, false); //?
				player->learnSpell(227, false); //?
				player->learnSpell(1180, false); //?
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================DEATH KNGIHTS=================================================//
			case MENU_DEATH_K:
				player->learnSpell(dk_spell_Parry, false);
				player->learnSpell(dk_spell_Pestilence, false);
				player->learnSpell(dk_spell_Blood_Boil, false);
				player->learnSpell(dk_spell_Blood_Strike, false);
				player->learnSpell(dk_spell_Strangulate, false);
				player->learnSpell(dk_spell_Blood_Tap, false);
				player->learnSpell(dk_spell_Path_of_Frost, false);
				player->learnSpell(dk_spell_Dark_Command, false);
				player->learnSpell(dk_spell_Death_Pact, false);
				player->learnSpell(dk_spell_Frost_Presence, false);
				player->learnSpell(dk_spell_Icy_Touch, false);
				player->learnSpell(dk_spell_Mind_Freeze, false);
				player->learnSpell(dk_spell_Chains_of_Ice, false);
				player->learnSpell(dk_spell_Icebound_Fortitude, false);
				player->learnSpell(dk_spell_Horn_of_Winter, false);
				player->learnSpell(dk_spell_Rune_Strike, false);
				player->learnSpell(dk_spell_Empower_Rune_Weapon, false);
				player->learnSpell(dk_spell_Death_Coil, false);
				player->learnSpell(dk_spell_Death_Gate, false);
				player->learnSpell(dk_spell_Plague_Strike, false);
				player->learnSpell(dk_spell_Death_Grip, false);
				player->learnSpell(dk_spell_Raise_Dead, false);
				player->learnSpell(dk_spell_Death_and_Decay, false);
				player->learnSpell(dk_spell_Anti_Magic_Shell, false);
				player->learnSpell(dk_spell_Unholy_Presence, false);
				player->learnSpell(dk_spell_Raise_Ally, false);
				player->learnSpell(dk_spell_Army_of_the_Dead, false);
				player->learnSpell(dk_spell_Runeforging_skill, false);
				player->learnSpell(dk_spell_Acherus_Deathcharger, false);
				player->learnSpell(dk_spell_Obliterate, false);
				player->learnSpell(dk_spell_Death_Strike, false);
				player->learnSpell(war_spell_Dual_Wield, false);
				//================RuneForge================//
				player->learnSpell(dk_spell_Uknowas, false);
				player->learnSpell(dk_spell_Uknowah, false);
				player->learnSpell(dk_spell_Uknowai, false);
				player->learnSpell(dk_spell_Uknowaz, false);
				player->learnSpell(dk_spell_Uknowan, false);
				player->learnSpell(dk_spell_Uknowab, false);
				player->learnSpell(dk_spell_Uknowav, false);
				player->learnSpell(dk_spell_Uknowaj, false);
				player->learnSpell(dk_spell_Uknowaq, false);
				///////////WEAPON/////////////
				player->learnSpell(750, false);
				player->learnSpell(196, false);
				player->learnSpell(197, false);
				player->learnSpell(198, false);
				player->learnSpell(199, false);
				player->learnSpell(200, false);
				player->learnSpell(201, false);
				player->learnSpell(202, false);
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================SHAMAN=================================================//
			case MENU_SHAMAN:
				player->learnSpell(8737, false);//?
				player->learnSpell(shaman_spell_Earth_Elemental_Totem, false);
				player->learnSpell(shaman_spell_Healing_Wave, false);
				player->learnSpell(shaman_spell_Lightning_Bolt, false);
				player->learnSpell(shaman_spell_Rockbiter_Weapon, false);
				player->learnSpell(shaman_spell_Earth_Shock, false);
				player->learnSpell(shaman_spell_Stoneskin_Totem, false);
				player->learnSpell(shaman_spell_Earthbind_Totem, false);
				player->learnSpell(shaman_spell_Lightning_Shield, false);
				player->learnSpell(shaman_spell_Stoneclaw_Totem, false);
				player->learnSpell(shaman_spell_Flame_Shock, false);
				player->learnSpell(shaman_spell_Flametongue_Weapon, false);
				player->learnSpell(shaman_spell_Searing_Totem, false);
				player->learnSpell(shaman_spell_Strength_of_Earth_Totem, false);
				player->learnSpell(shaman_spell_Ancestral_Spirit, false);
				player->learnSpell(shaman_spell_Fire_Nova, false);
				player->learnSpell(shaman_spell_Purge, false);
				player->learnSpell(shaman_spell_Cure_Toxins, false);
				player->learnSpell(shaman_spell_Ghost_Wolf, false);
				player->learnSpell(shaman_spell_Wind_Shear, false);
				player->learnSpell(shaman_spell_Tremor_Totem, false);
				player->learnSpell(shaman_spell_Frost_Shock, false);
				player->learnSpell(shaman_spell_Frostbrand_Weapon, false);
				player->learnSpell(shaman_spell_Healing_Stream_Totem, false);
				player->learnSpell(shaman_spell_Lesser_Healing_Wave, false);
				player->learnSpell(shaman_spell_Water_Shield, false);
				player->learnSpell(shaman_spell_Water_Breathing, false);
				player->learnSpell(shaman_spell_Frost_Resistance_Totem, false);
				player->learnSpell(shaman_spell_Far_Sight, false);
				player->learnSpell(shaman_spell_Magma_Totem, false);
				player->learnSpell(shaman_spell_Mana_Spring_Totem, false);
				player->learnSpell(shaman_spell_Fire_Resistance_Totem, false);
				player->learnSpell(shaman_spell_Flametongue_Totem, false);
				player->learnSpell(shaman_spell_Water_Walking, false);
				player->learnSpell(shaman_spell_Astral_Recall, false);
				player->learnSpell(shaman_spell_Call_of_the_Elements, false);
				player->learnSpell(shaman_spell_Earthliving_Weapon, false);
				player->learnSpell(shaman_spell_Grounding_Totem, false);
				player->learnSpell(shaman_spell_Nature_Resistance_Totem, false);
				player->learnSpell(shaman_spell_Reincarnation, false);
				player->learnSpell(shaman_spell_Totemic_Recall, false);
				player->learnSpell(shaman_spell_Windfury_Weapon, false);
				player->learnSpell(shaman_spell_Chain_Lightning, false);
				player->learnSpell(shaman_spell_Windfury_Totem, false);
				player->learnSpell(shaman_spell_Sentry_Totem, false);
				player->learnSpell(shaman_spell_Cleansing_Totem, false);
				player->learnSpell(shaman_spell_Call_of_the_Ancestors, false);
				player->learnSpell(shaman_spell_Chain_Heal, false);
				player->learnSpell(shaman_spell_Call_of_the_Spirits, false);
				player->learnSpell(shaman_spell_Wrath_of_Air_Totem, false);
				player->learnSpell(shaman_spell_Fire_Elemental_Totem, false);
				player->learnSpell(shaman_spell_Lava_Burst, false);
				player->learnSpell(shaman_spell_Hex, false);
				///////////WEAPONS/////////////
				player->learnSpell(8737, false);
				player->learnSpell(196, false);
				player->learnSpell(197, false);
				player->learnSpell(198, false);
				player->learnSpell(199, false);
				player->learnSpell(227, false);
				player->learnSpell(1180, false);
				player->learnSpell(15590, false);
				//Faction
				if(player->GetTeam() == ALLIANCE){
					player->learnSpell(shaman_spell_Heroism, false);
				}
				else
				{
					player->learnSpell(shaman_spell_Bloodlust, false);
				}
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================MAGE=================================================//
			case MENU_MAGE:
				player->learnSpell(mage_spell_Arcane_Explosion, false);
				player->learnSpell(mage_spell_Frostbolt, false);
				player->learnSpell(mage_spell_Arcane_Intellect, false);
				player->learnSpell(mage_spell_Fireball, false);
				player->learnSpell(mage_spell_Conjure_Water, false);
				player->learnSpell(mage_spell_Conjure_Food, false);
				player->learnSpell(mage_spell_Fire_Blast, false);
				player->learnSpell(mage_spell_Arcane_Missiles, false);
				player->learnSpell(mage_spell_Polymorph, false);
				player->learnSpell(mage_spell_Polymorph_Turtle, false);
				player->learnSpell(mage_spell_Polymorph_Turkey, false);
				player->learnSpell(mage_spell_Polymorph_Rabbit, false);
				player->learnSpell(mage_spell_Polymorph_Pig, false);
				player->learnSpell(mage_spell_Frost_Nova, false);
				player->learnSpell(mage_spell_Dampen_Magic, false);
				player->learnSpell(mage_spell_Slow_Fall,	  false);
				player->learnSpell(mage_spell_Flamestrike, false);
				player->learnSpell(mage_spell_Amplify_Magic, false);
				player->learnSpell(mage_spell_Remove_Curse,   false);
				player->learnSpell(mage_spell_Blink,  false);
				player->learnSpell(mage_spell_Blizzard, false);
				player->learnSpell(mage_spell_Evocation, false);
				player->learnSpell(mage_spell_Fire_Ward, false);
				player->learnSpell(mage_spell_Mana_Shield, false);
				player->learnSpell(mage_spell_Frost_Ward, false);
				player->learnSpell(mage_spell_Scorch, false);
				player->learnSpell(mage_spell_Counterspell,  false);
				player->learnSpell(mage_spell_Cone_of_Cold, false);
				player->learnSpell(mage_spell_Conjure_Mana_Gem, false);
				player->learnSpell(mage_spell_Ice_Armor, false);
				player->learnSpell(mage_spell_Ice_Block, false);
				player->learnSpell(mage_spell_Mage_Armor, false);
				player->learnSpell(mage_spell_Arcane_Brilliance, false);
				player->learnSpell(mage_spell_Molten_Armor, false);
				player->learnSpell(mage_spell_Arcane_Blast, false);
				player->learnSpell(mage_spell_Ice_Lance, false);
				player->learnSpell(mage_spell_Invisibility,    false); 
				player->learnSpell(mage_spell_Ritual_of_Refreshment, false);
				player->learnSpell(mage_spell_Spellsteal, false);
				player->learnSpell(mage_spell_Conjure_Refreshment, false);
				player->learnSpell(mage_spell_Frostfire_Bolt, false);
				player->learnSpell(mage_spell_Dalaran_Brilliance, false);
				player->learnSpell(mage_spell_Dalaran_Intellect, false);
				player->learnSpell(mage_spell_Mirror_Image, false);
				player->learnSpell(mage_spell_Portal_Dalaran, false);
				player->learnSpell(mage_spell_Frost_Armor,  false);
				player->learnSpell(61305, false);//?
				player->SaveToDB();
				/////////WEAPON/////////////
				player->learnSpell(201,  false); //?
				player->learnSpell(196,  false); //?
				player->learnSpell(227,  false); //?
				player->learnSpell(1180, false); //?
				player->SaveToDB();
				/////////RESTRICTION/////////////
				if(player->GetTeam() == ALLIANCE){
					player->learnSpell(mage_spell_Teleport_Exodar, false);
					player->learnSpell(mage_spell_Teleport_Theramore, false);
					player->learnSpell(mage_spell_Teleport_Darnassus,  false);
					player->learnSpell(mage_spell_Teleport_Shattrath, false);
					player->learnSpell(mage_spell_Teleport_Ironforge,  false);
					player->learnSpell(mage_spell_Teleport_Stromwind,  false);
					player->learnSpell(mage_spell_Portal_Darnassus, false);
					player->learnSpell(mage_spell_Portal_Exodar, false);
					player->learnSpell(mage_spell_Portal_Ironforge, false);
					player->learnSpell(mage_spell_Portal_Shattrath, false);
					player->learnSpell(mage_spell_Portal_Theramore, false);
				}else{
					player->learnSpell(mage_spell_Teleport_Orgrimmar,  false);
					player->learnSpell(mage_spell_Teleport_Shattrath_H, false);
					player->learnSpell(mage_spell_Teleport_Thunder_Bluff,  false);
					player->learnSpell(mage_spell_Teleport_Stonard, false);
					player->learnSpell(mage_spell_Teleport_Silvermoon, false);
					player->learnSpell(mage_spell_Teleport_Undercity,  false);
					player->learnSpell(mage_spell_Portal_Orgrimmar, false);
					player->learnSpell(mage_spell_Portal_Shattrath_H, false);
					player->learnSpell(mage_spell_Portal_Silvermoon, false);
					player->learnSpell(mage_spell_Portal_Stonard, false);
					player->learnSpell(mage_spell_Portal_Thunder_Bluff, false);
					player->learnSpell(mage_spell_Portal_Undercity, false);
				}
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================WARLOCK=================================================//
			case MENU_WARLOCK:
				player->learnSpell(warlock_spell_Demon_Skin, false);
				player->learnSpell(warlock_spell_Immolate, false);
				player->learnSpell(warlock_spell_Shadow_Bolt, false);
				player->learnSpell(warlock_spell_Summon_Imp, false);
				player->learnSpell(warlock_spell_Corruption, false);
				player->learnSpell(warlock_spell_Curse_of_Weakness, false);
				player->learnSpell(warlock_spell_Life_Tap, false);
				player->learnSpell(warlock_spell_Curse_of_Agony, false);
				player->learnSpell(warlock_spell_Fear, false);
				player->learnSpell(warlock_spell_Create_Healthstone, false);
				player->learnSpell(warlock_spell_Drain_Soul, false);
				player->learnSpell(warlock_spell_Summon_Voidwalker, false);
				player->learnSpell(warlock_spell_Health_Funnel, false);
				player->learnSpell(warlock_spell_Drain_Life, false);
				player->learnSpell(warlock_spell_Unending_Breath, false);
				player->learnSpell(warlock_spell_Create_Soulstone, false);
				player->learnSpell(warlock_spell_Searing_Pain, false);
				player->learnSpell(warlock_spell_Demon_Armor, false);
				player->learnSpell(warlock_spell_Rain_of_Fire, false);
				player->learnSpell(warlock_spell_Ritual_of_Summoning, false);
				player->learnSpell(warlock_spell_Summon_Succubus, false);
				player->learnSpell(warlock_spell_Eye_of_Kilrogg, false);
				player->learnSpell(warlock_spell_Drain_Mana, false);
				player->learnSpell(warlock_spell_Sense_Demons, false);
				player->learnSpell(warlock_spell_Curse_of_Tongues, false);
				player->learnSpell(warlock_spell_Detect_Invisibility, false);
				player->learnSpell(warlock_spell_Create_Firestone, false);
				player->learnSpell(warlock_spell_Banish, false);
				player->learnSpell(warlock_spell_Enslave_Demon, false);
				player->learnSpell(warlock_spell_Hellfire, false);
				player->learnSpell(warlock_spell_Summon_Felhunter, false);
				player->learnSpell(warlock_spell_Curse_of_the_Elements, false);
				player->learnSpell(warlock_spell_Shadow_Ward, false);
				player->learnSpell(warlock_spell_Create_Spellstone, false);
				player->learnSpell(warlock_spell_Howl_of_Terror, false);
				player->learnSpell(warlock_spell_Death_Coil, false);
				player->learnSpell(warlock_spell_Soul_Fire, false);
				player->learnSpell(warlock_spell_Inferno, false);
				player->learnSpell(warlock_spell_Curse_of_Doom, false);
				player->learnSpell(warlock_spell_Ritual_of_Doom, false);
				player->learnSpell(warlock_spell_Fel_Armor, false);
				player->learnSpell(warlock_spell_Incinerate, false);
				player->learnSpell(warlock_spell_Soulshatter, false);
				player->learnSpell(warlock_spell_Ritual_of_Souls, false);
				player->learnSpell(warlock_spell_Seed_of_Corruption, false);
				player->learnSpell(warlock_spell_Shadowflame, false);
				player->learnSpell(warlock_spell_Demonic_Circle_Summon, false);
				player->learnSpell(warlock_spell_Demonic_Circle_Teleport, false);
				player->learnSpell(warlock_spell_Dreadsteed, false);
				player->learnSpell(33388, false); //?
				player->learnSpell(33391, false); //?
				///////////ARMES/////////////
				player->learnSpell(196, false);//?
				player->learnSpell(227, false);//?
				player->learnSpell(201, false);
				player->learnSpell(1180, false);//?
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;
				//=============================================DRUID=================================================//
			case MENU_DRUID:
				if (player->getClass() == CLASS_DRUID)
				{
					player->learnSpell(druid_spell_Healing_Touch, false);
					player->learnSpell(druid_spell_Mark_of_the_Wild, false);
					player->learnSpell(druid_spell_Wrath, false);
					player->learnSpell(druid_spell_Moonfire, false);
					player->learnSpell(druid_spell_Rejuvenation, false);
					player->learnSpell(druid_spell_Thorns, false);
					player->learnSpell(druid_spell_Entangling_Roots, false);
					player->learnSpell(druid_spell_Bear_Form, false);
					player->learnSpell(druid_spell_Demoralizing_Roar, false);
					player->learnSpell(druid_spell_Growl, false);
					player->learnSpell(druid_spell_Maul, false);
					player->learnSpell(druid_spell_Natures_Grasp, false);
					player->learnSpell(druid_spell_Teleport_Moonglade, false);
					player->learnSpell(druid_spell_Enrage, false);
					player->learnSpell(druid_spell_Regrowth, false);
					player->learnSpell(druid_spell_Revive, false);
					player->learnSpell(druid_spell_Bash, false);
					player->learnSpell(druid_spell_Cure_Poison, false);
					player->learnSpell(druid_spell_Aquatic_Form, false);
					player->learnSpell(druid_spell_Swipe_Bear, false);
					player->learnSpell(druid_spell_Travel_Form, false);
					player->learnSpell(druid_spell_Faerie_Fire, false);
					player->learnSpell(druid_spell_Faerie_Fire_Feral, false); 
					player->learnSpell(druid_spell_Hibernate, false);
					player->learnSpell(druid_spell_Cat_Form, false);
					//	player->learnSpell(druid_spell_Feral_Charge_Bear, false); // charge farouche 2
					//	player->learnSpell(druid_spell_Feral_Charge_Cat, false); // charge farouche
					player->learnSpell(druid_spell_Prowl, false);
					player->learnSpell(druid_spell_Rebirth, false);
					player->learnSpell(druid_spell_Rip, false);
					player->learnSpell(druid_spell_Starfire, false);
					player->learnSpell(druid_spell_Shred, false);
					player->learnSpell(druid_spell_Soothe_Animal, false);
					player->learnSpell(druid_spell_Rake, false);
					player->learnSpell(druid_spell_Remove_Curse, false);
					player->learnSpell(druid_spell_Tigers_Fury, false);
					player->learnSpell(druid_spell_Abolish_Poison, false);
					player->learnSpell(druid_spell_Dash, false);
					player->learnSpell(druid_spell_Challenging_Roar, false);
					player->learnSpell(druid_spell_Cower, false);
					player->learnSpell(druid_spell_Tranquility, false);
					player->learnSpell(druid_spell_Ferocious_Bite, false);
					player->learnSpell(druid_spell_Ravage, false);
					player->learnSpell(druid_spell_Track_Humanoids, false);
					player->learnSpell(druid_spell_Frenzied_Regeneration, false);
					player->learnSpell(druid_spell_Pounce, false);
					player->learnSpell(druid_spell_Dire_Bear_Form, false);
					player->learnSpell(druid_spell_Feline_Grace, false);
					player->learnSpell(druid_spell_Hurricane, false);
					player->learnSpell(druid_spell_Innervate, false);
					player->learnSpell(druid_spell_Savage_Defense, false);
					player->learnSpell(druid_spell_Barkskin, false);
					player->learnSpell(druid_spell_Gift_of_the_Wild, false);
					//	player->learnSpell(druid_spell_Mangle_Bear, false); mutilation farouche 2
					//	player->learnSpell(druid_spell_Mangle_Cat, false); mutilation farouche
					player->learnSpell(druid_spell_Flight_Form, false);
					player->learnSpell(druid_spell_Maim, false);
					player->learnSpell(druid_spell_Lifebloom, false);
					player->learnSpell(druid_spell_Lacerate, false);
					player->learnSpell(druid_spell_Cyclone, false);
					player->learnSpell(druid_spell_Swift_Flight_Form, false);
					player->learnSpell(druid_spell_Swipe_Cat, false);
					player->learnSpell(druid_spell_Savage_Roar, false);
					player->learnSpell(druid_spell_Nourish, false);
					player->learnSpell(druid_spell_Claw, false);
					player->learnSpell(druid_spell_Claw_1, false);//?
					///////////ARMES/////////////
					player->learnSpell(198, false); //?
					player->learnSpell(199, false); // ?
					player->learnSpell(200, false); //?
					player->learnSpell(227, false); //?
					player->SaveToDB();
				}
				player->UpdateSkillsToMaxSkillsForLevel();
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case OPTION_MENU_ID_1:
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, SUB_MENU_TALENT_OPTION_1, GOSSIP_SENDER_MAIN, SUB_OPTION_MENU_ID_1);
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, SUB_MENU_TALENT_OPTION_2, GOSSIP_SENDER_MAIN, SUB_OPTION_MENU_ID_2);
				player->ADD_GOSSIP_ITEM(CHAT_ICON, back, GOSSIP_SENDER_MAIN, BACK_OPTION_MENU_1);
				player->SEND_GOSSIP_MENU(1, creature->GetGUID());
				break;
				
			case SUB_OPTION_MENU_ID_1:
				//=====WARRIOR TALENT=====//
				//Arms
				if(player->HasSpell(12294)) //Mortal Strike Rank 1 
				{
					player->learnSpell(war_spell_Mortal_Strike, false);
				}
				//Prot
				if(player->HasSpell(20243)) //Devastate Rank 1
				{
					player->learnSpell(war_spell_Devastate, false);
				}
				//=====PALADIN TALENT=====//
				//Holy
				if(player->HasSpell(20925)) //Holy Shield Rank 1
				{
					player->learnSpell(pala_spell_Holy_Shield, false);
				}
				if(player->HasSpell(20473)) //Holy Shock Rank 1
				{
					player->learnSpell(pala_spell_Holy_Shock, false);
				}
				//Prot
				if(player->HasSpell(31935)) //Avengers Shield Rank 1
				{
					player->learnSpell(pala_spell_Avengers_Shield, false);
				}
				if(player->HasSpell(20911)) //Blessing of Sanctuary Rank 1
				{
					player->learnSpell(pala_spell_Greater_Blessing_of_Sanctuary, false);
				}
				//=====HUNTER  TALENT=====//
				//Survival
				if(player->HasSpell(19386)) //Wyvern String Rank 1
				{
					player->learnSpell(hunter_spell_Wyvern_Sting, false);
				}
				if(player->HasSpell(53301)) //Explosive Shot Rank 1
				{
					player->learnSpell(hunter_spell_Explosive_Shot, false);
				}
				if(player->HasSpell(19306)) //Conter Attack Rank 1
				{
					player->learnSpell(hunter_spell_Conterattack, false);
				}
				//Marksmanship
				if(player->HasSpell(19434)) //Aimed Shot Rank 1
				{
					player->learnSpell(hunter_spell_Aimed_Shot, false);
				}
				//=====ROGUE   TALENT=====//
				//Subtlety
				if(player->HasSpell(16511)) //Hemorrhage Rank 1
				{
					player->learnSpell(rogue_spell_Hemorrhage, false);
				}
				//Assassination
				if(player->HasSpell(1329)) //Mutilate Rank 1
				{
					player->learnSpell(rogue_spell_Mutilate, false);
				}
				//=====PRIEST  TALENT=====//
				//Shadow
				if(player->HasSpell(34914)) //Vampiric Touch Rank 1
				{
					player->learnSpell(priest_spell_Vampiric_Touch, false);
				}
				//Discipline
				if(player->HasSpell(47540)) //Penance Rank 1
				{
					player->learnSpell(priest_spell_Penance, false);
				}
				//Holy
				if(player->HasSpell(724)) //LightWell Rank 1
				{
					player->learnSpell(priest_spell_Lightwell, false);
				}
				if(player->HasSpell(19236)) //Desperate Prayer Rank 1
				{
					player->learnSpell(priest_spell_Desperate_Prayer, false);
				}
				if(player->HasSpell(34861)) //Circle Of healing Rank 1
				{
					player->learnSpell(priest_spell_Circle_of_Healing, false);
				}
				//Shadow
				if(player->HasSpell(15407)) //Mind Flay Rank 1
				{
					player->learnSpell(priest_spell_Mind_Flay, false);
				}
				//=====DEATH_K TALENT=====//
				//Blood
				if(player->HasSpell(55050)) //Heart Strike Rank 1
				{
					player->learnSpell(dk_spell_Heart_Strike, false);
				}
				//Frost
				if(player->HasSpell(49143)) //Frost Strike Rank 1
				{
					player->learnSpell(dk_spell_Frost_Strike, false);
				}
				if(player->HasSpell(49184)) //Howling Blast Rank 1
				{
					player->learnSpell(dk_spell_Howling_Blast, false);
				}
				//UnHoly
				if(player->HasSpell(55090)) //Scourge Strike Rank 1
				{
					player->learnSpell(dk_spell_Scourge_Strike, false);
				}
				if(player->HasSpell(49158)) //Corpse Explosion Rank 1
				{
					player->learnSpell(dk_spell_Corpse_Explosion, false);
				}
				//=====SHAMAN  TALENT=====//
				//Restoration
				if(player->HasSpell(61295)) //Riptide Rank 1
				{
					player->learnSpell(shaman_spell_Riptide, false);
				}
				if(player->HasSpell(974)) //Earth Shield Rank 1
				{
					player->learnSpell(shaman_spell_Earth_Shield, false);
				}
				//Elemental
				if(player->HasSpell(30706)) //Totem of Wrath Rank 1
				{
					player->learnSpell(shaman_spell_Totem_of_Wrath, false);
				}
				if(player->HasSpell(51490)) //TunderStorm Rank 1
				{
					player->learnSpell(shaman_spell_Thunderstorm, false);
				}
				//=====MAGE    TALENT=====//
				//Arcane
				if(player->HasSpell(44425)) //Arcane Barrage Rank 1
				{
					player->learnSpell(mage_spell_Arcane_Barrage, false);
				}
				//Fire
				if(player->HasSpell(11113)) //Blast Wave Rank 1
				{
					player->learnSpell(mage_spell_Blast_Wave, false);
				}
				if(player->HasSpell(31661)) //Dragon Breath  Rank 1
				{
					player->learnSpell(mage_spell_Dragons_Breath, false);
				}
				if(player->HasSpell(44457)) //Living Bomb Rank 1
				{
					player->learnSpell(mage_spell_Living_Bomb, false);
				}
				if(player->HasSpell(11366)) //Pyroblast Rank 1
				{
					player->learnSpell(mage_spell_Pyroblast, false);
				}
				//Frost
				if(player->HasSpell(11426)) //Ice Barrier Rank 1
				{
					player->learnSpell(mage_spell_Ice_Barrier, false);
				}
				//=====WARLOCK TALENT=====//
				//Destruction
				if(player->HasSpell(17877)) //ShadowBurn Rank 1
				{
					player->learnSpell(warlock_spell_Shadowburn, false);
				}
				if(player->HasSpell(30283)) //ShadowFury Rank 1
				{
					player->learnSpell(warlock_spell_Shadowfury, false);
				}
				if(player->HasSpell(50796)) //Chaos Bolt Rank 1
				{
					player->learnSpell(warlock_spell_Chaos_Bolt, false);
				}
				//Affliction
				if(player->HasSpell(30108)) //Unstable Affliction Rank 1
				{
					player->learnSpell(warlock_spell_Unstable_Affliction, false);
				}
				if(player->HasSpell(48181)) //Spell Haunt Rank 1
				{
					player->learnSpell(warlock_spell_Haunt, false);
				}
				if(player->HasSpell(18220)) //Dark Pact Rank 1
				{
					player->learnSpell(warlock_spell_Dark_Pact, false);
				}
				//=====DRUID   TALENT=====//
				//Balance
				if(player->HasSpell(50516)) //Typhoon Rank 1
				{
					player->learnSpell(druid_spell_Typhoon, false);
				}
				if(player->HasSpell(48505)) //Starfall Rank 1
				{
					player->learnSpell(druid_spell_Starfall, false);
				}
				if(player->HasSpell(5570)) //Insect Swarm Rank 1
				{
					player->learnSpell(druid_spell_Insect_Swarm, false);
				}
				//Restoration
				if(player->HasSpell(48438)) //Wild Growth Rank 1
				{
					player->learnSpell(druid_spell_Wild_Growth, false);
				}
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case SUB_OPTION_MENU_ID_2:
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->GetSession()->SendNotification("Ваши таланты были сброшены!");
				player->SaveToDB();
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case OPTION_MENU_ID_2:
				player->learnSpell(mount_spell_Journeyman_Riding,false);
				player->learnSpell(mount_spell_Cold_Weather_Flying,false);
				
				if (player->HasSpell(42777))
				{	
					player->CLOSE_GOSSIP_MENU();
				}
				else
				{	
					player->CLOSE_GOSSIP_MENU();					
					player->AddItem(Mount_Invincibles_Reins, 1);
				}
				player->PlayerTalkClass->SendCloseGossip();
				break;
				
				
		case BACK_OPTION_MENU_1://Main Menu
		if (player->getLevel() >= 80)
		{
			{
				player->PrepareQuestMenu(creature->GetGUID());
				player->SendPreparedQuest(creature->GetGUID());
			}
			if (player->getClass() == CLASS_WARRIOR)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Warrior:30|t Пройти обучение война", GOSSIP_SENDER_MAIN, MENU_WARRIOR);
			}
			if (player->getClass() == CLASS_PALADIN)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Paladin:30|t Пройти обучение паладина", GOSSIP_SENDER_MAIN, MENU_PALADIN);
			}
			if (player->getClass() == CLASS_HUNTER)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Hunter:30|t Пройти обучение охотника", GOSSIP_SENDER_MAIN, MENU_HUNTER);
			}
			if (player->getClass() == CLASS_ROGUE)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Rogue:30|t Пройти обучение разбойника", GOSSIP_SENDER_MAIN, MENU_ROGUE);
			}
			if (player->getClass() == CLASS_PRIEST)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Priest:30|t Пройти обучение жреца", GOSSIP_SENDER_MAIN, MENU_PRIEST);
			}
			if (player->getClass() == CLASS_DEATH_KNIGHT)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_DeathKnight:30|t Пройти обучение рыцаря смерти", GOSSIP_SENDER_MAIN, MENU_DEATH_K);
			}
			if (player->getClass() == CLASS_SHAMAN)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Shaman:30|t Пройти обучение шамана", GOSSIP_SENDER_MAIN, MENU_SHAMAN);
			}
			if (player->getClass() == CLASS_MAGE)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Mage:30|t Пройти обучение мага", GOSSIP_SENDER_MAIN, MENU_MAGE);
			}
			if (player->getClass() == CLASS_WARLOCK)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Warlock:30|t Пройти обучение чернокнижника", GOSSIP_SENDER_MAIN, MENU_WARLOCK);
			}
			if (player->getClass() == CLASS_DRUID)
			{
				player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ClassIcon_Druid:30|t Пройти обучение друида", GOSSIP_SENDER_MAIN, MENU_DRUID);
			}

			player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/ability_rogue_dirtydeeds:30|t Меню талантов", GOSSIP_SENDER_MAIN, OPTION_MENU_ID_1);
			player->ADD_GOSSIP_ITEM(TRAINER_ICON, "|TInterface/ICONS/Ability_Mount_FelDrake:30|t Изучить верховую езду", GOSSIP_SENDER_MAIN, OPTION_MENU_ID_2);			
			player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		}
		else
		{
			player->PlayerTalkClass->SendCloseGossip();
			return false;
		}
				break;

			}
		}
		return true;
	}
};

class Professions_NPC : public CreatureScript
{
    public:
	
    Professions_NPC () : CreatureScript("Professions_NPC") {}
	
    void CreatureWhisperBasedOnBool(const char *text, Creature *_creature, Player *pPlayer, bool value)
    {
        if (value)
			_creature->MonsterWhisper(text, pPlayer->GetGUID());
			}
			uint32 PlayerMaxLevel() const
        {
			return sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
        }
     
        bool PlayerHasItemOrSpell(const Player *plr, uint32 itemId, uint32 spellId) const
        {
            return plr->HasItemCount(itemId, 1, true) || plr->HasSpell(spellId);
        }
     
		bool OnGossipHello(Player *pPlayer, Creature* _creature)
		{
			pPlayer->ADD_GOSSIP_ITEM(9, "[Professions] ->", GOSSIP_SENDER_MAIN, 196);
			pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
			return true;
		}
                   
		bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
		{
			uint32 skillCount = 0;

			if (pPlayer->HasSkill(SKILL_MINING))
					skillCount++;
			if (pPlayer->HasSkill(SKILL_SKINNING))
					skillCount++;
			if (pPlayer->HasSkill(SKILL_HERBALISM))
					skillCount++;

			if (skillCount >= 2)
					return true;

			for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
			{
					SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
					if (!SkillInfo)
							continue;

					if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
							continue;

					if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
							continue;

					const uint32 skillID = SkillInfo->id;
					if (pPlayer->HasSkill(skillID))
							skillCount++;

					if (skillCount >= 2)
							return true;
			}
			return false;
		}
     
		bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
		{
			ChatHandler handler(pPlayer->GetSession());
			char* skill_name;

			SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
									skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];

			// if (!SkillInfo)
			// {
			// TC_LOG_ERROR("server.loading", "Profession NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
			// }      

			LearnSkillRecipesHelper(pPlayer, SkillInfo->id);

			pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), 450, 450);
			handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);
   
			return true;
		}
           
		void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
		{
			uint32 classmask = player->getClassMask();

			for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
			{
				SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
				if (!skillLine)
						continue;

				// wrong skill
				if (skillLine->skillId != skill_id)
						continue;

				// not high rank
				if (skillLine->forward_spellid)
						continue;

				// skip racial skills
				if (skillLine->racemask != 0)
						continue;

				// skip wrong class skills
				if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
						continue;

				SpellInfo const * spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
				if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
						continue;
			   
				player->learnSpell(skillLine->spellId, false);
			}
		}
     
		bool IsSecondarySkill(SkillType skill) const
		{
			return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
		}

		void CompleteLearnProfession(Player *pPlayer, Creature *pCreature, SkillType skill)
		{
			if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
					pCreature->MonsterWhisper("Больше 2-ух професси иметь нельзя!", pPlayer->GetGUID());
			else
			{
					if (!LearnAllRecipesInProfession(pPlayer, skill))
							pCreature->MonsterWhisper("Internal error occured!", pPlayer->GetGUID());
			}
		}
           
		bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 uiAction)
		{
			pPlayer->PlayerTalkClass->ClearMenus();

			if (uiSender == GOSSIP_SENDER_MAIN)
			{
   
				switch (uiAction)
				{
					case 196:
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_alchemy:30|t Алхимия.", GOSSIP_SENDER_MAIN, 1);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Ingot_05:30|t Кузнечное дело.", GOSSIP_SENDER_MAIN, 2);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_LeatherScrap_02:30|t Кожевничество.", GOSSIP_SENDER_MAIN, 3);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Fabric_Felcloth_Ebon:30|t Портняжное дело.", GOSSIP_SENDER_MAIN, 4);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_wrench_01:30|t Инженерное дело.", GOSSIP_SENDER_MAIN, 5);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engraving:30|t Наложение чар.", GOSSIP_SENDER_MAIN, 6);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_gem_01:30|t Ювелирное дело.", GOSSIP_SENDER_MAIN, 7);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Scroll_08:30|t Начертание.", GOSSIP_SENDER_MAIN, 8);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_Herb_07:30|t Травничество.", GOSSIP_SENDER_MAIN, 9);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_pelt_wolf_01:30|t Снятие шкур.", GOSSIP_SENDER_MAIN, 10);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Горное дело.", GOSSIP_SENDER_MAIN, 11);
							pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_fishing:30|t Рыболовное дело.", GOSSIP_SENDER_MAIN, 13);
							pPlayer->PlayerTalkClass->SendGossipMenu(1, _creature->GetGUID());
							break;
					case 1:
							if(pPlayer->HasSkill(SKILL_ALCHEMY))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}

							CompleteLearnProfession(pPlayer, _creature, SKILL_ALCHEMY);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 2:
							if(pPlayer->HasSkill(SKILL_BLACKSMITHING))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}
							CompleteLearnProfession(pPlayer, _creature, SKILL_BLACKSMITHING);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 3:
							if(pPlayer->HasSkill(SKILL_LEATHERWORKING))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}
							CompleteLearnProfession(pPlayer, _creature, SKILL_LEATHERWORKING);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 4:
							if(pPlayer->HasSkill(SKILL_TAILORING))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}
							CompleteLearnProfession(pPlayer, _creature, SKILL_TAILORING);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 5:
							if(pPlayer->HasSkill(SKILL_ENGINEERING))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}
							CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINEERING);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 6:
							if(pPlayer->HasSkill(SKILL_ENCHANTING))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}
							CompleteLearnProfession(pPlayer, _creature, SKILL_ENCHANTING);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 7:
							if(pPlayer->HasSkill(SKILL_JEWELCRAFTING))
							{
									pPlayer->PlayerTalkClass->SendCloseGossip();
									break;
							}
							CompleteLearnProfession(pPlayer, _creature, SKILL_JEWELCRAFTING);

							pPlayer->PlayerTalkClass->SendCloseGossip();
							break;
					case 8:
						if(pPlayer->HasSkill(SKILL_INSCRIPTION))
						{
								pPlayer->PlayerTalkClass->SendCloseGossip();
								break;
						}
						CompleteLearnProfession(pPlayer, _creature, SKILL_INSCRIPTION);

						pPlayer->PlayerTalkClass->SendCloseGossip();
						break;
					case 9:
						if(pPlayer->HasSkill(SKILL_HERBALISM))
						{
								pPlayer->PlayerTalkClass->SendCloseGossip();
								break;
						}
					   
						CompleteLearnProfession(pPlayer, _creature, SKILL_HERBALISM);
						pPlayer->PlayerTalkClass->SendCloseGossip();
						break;
					case 10:
						if(pPlayer->HasSkill(SKILL_SKINNING))
						{
								pPlayer->PlayerTalkClass->SendCloseGossip();
								break;
						}
					   
						CompleteLearnProfession(pPlayer, _creature, SKILL_SKINNING);
						pPlayer->PlayerTalkClass->SendCloseGossip();
						break;
					case 11:
						if(pPlayer->HasSkill(SKILL_MINING))
						{
								pPlayer->PlayerTalkClass->SendCloseGossip();
								break;
						}
					   
						CompleteLearnProfession(pPlayer, _creature, SKILL_MINING);
						pPlayer->PlayerTalkClass->SendCloseGossip();
						break;
					case 12:
						pPlayer->PlayerTalkClass->SendCloseGossip();
						break;
							
					case 13:
						if(pPlayer->HasSkill(SKILL_FISHING))
						{
								pPlayer->PlayerTalkClass->SendCloseGossip();
								break;
						}
					   
						CompleteLearnProfession(pPlayer, _creature, SKILL_FISHING);
						pPlayer->PlayerTalkClass->SendCloseGossip();
						break;
					}

				}
			return true;
		}
    };
	
class VIP_Professions_NPC : public CreatureScript
{
    public:
	
    VIP_Professions_NPC () : CreatureScript("VIP_Professions_NPC") {}
	
    void CreatureWhisperBasedOnBool(const char *text, Creature *_creature, Player *pPlayer, bool value)
    {
        if (value)
        _creature->MonsterWhisper(text, pPlayer->GetGUID());
        }
		uint32 PlayerMaxLevel() const
        {
        return sWorld->getIntConfig(CONFIG_MAX_PLAYER_LEVEL);
        }
     
        bool PlayerHasItemOrSpell(const Player *plr, uint32 itemId, uint32 spellId) const
        {
            return plr->HasItemCount(itemId, 1, true) || plr->HasSpell(spellId);
        }
     
        bool OnGossipHello(Player *pPlayer, Creature* _creature)
        {
		if (!pPlayer->GetSession()->IsPremium() && !pPlayer->isGameMaster())
		{
			pPlayer->CLOSE_GOSSIP_MENU();
			_creature->MonsterWhisper(EMOTE_NO_VIP, pPlayer->GetGUID());
			return true;
		}
        pPlayer->ADD_GOSSIP_ITEM(9, "[Professions] ->", GOSSIP_SENDER_MAIN, 196);
        pPlayer->PlayerTalkClass->SendGossipMenu(907, _creature->GetGUID());
		return true;
		}
		
		bool PlayerAlreadyHasTwoProfessions(const Player *pPlayer) const
		{
                            uint32 skillCount = 0;
     
                            if (pPlayer->HasSkill(SKILL_MINING))
                                    skillCount++;
                            if (pPlayer->HasSkill(SKILL_SKINNING))
                                    skillCount++;
                            if (pPlayer->HasSkill(SKILL_HERBALISM))
                                    skillCount++;
     
                            if (skillCount >= 11)
                                    return true;
     
                            for (uint32 i = 1; i < sSkillLineStore.GetNumRows(); ++i)
                            {
                                    SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(i);
                                    if (!SkillInfo)
                                            continue;
     
                                    if (SkillInfo->categoryId == SKILL_CATEGORY_SECONDARY)
                                            continue;
     
                                    if ((SkillInfo->categoryId != SKILL_CATEGORY_PROFESSION) || !SkillInfo->canLink)
                                            continue;
     
                                    const uint32 skillID = SkillInfo->id;
                                    if (pPlayer->HasSkill(skillID))
                                            skillCount++;
     
                                    if (skillCount >= 11)
                                            return true;
                            }
                            return false;
                    }
     
                    bool LearnAllRecipesInProfession(Player *pPlayer, SkillType skill)
                    {
                            ChatHandler handler(pPlayer->GetSession());
                            char* skill_name;
     
                            SkillLineEntry const *SkillInfo = sSkillLineStore.LookupEntry(skill);
                                                    skill_name = SkillInfo->name[handler.GetSessionDbcLocale()];
     
                            // if (!SkillInfo)
                            // {
                            // TC_LOG_ERROR("server.loading", "Profession NPC: received non-valid skill ID (LearnAllRecipesInProfession)");
                            // }      
     
                            LearnSkillRecipesHelper(pPlayer, SkillInfo->id);
     
                            pPlayer->SetSkill(SkillInfo->id, pPlayer->GetSkillStep(SkillInfo->id), 450, 450);
                            handler.PSendSysMessage(LANG_COMMAND_LEARN_ALL_RECIPES, skill_name);
                   
                            return true;
                    }
           
                    void LearnSkillRecipesHelper(Player *player, uint32 skill_id)
                    {
                            uint32 classmask = player->getClassMask();
     
                            for (uint32 j = 0; j < sSkillLineAbilityStore.GetNumRows(); ++j)
                            {
                                    SkillLineAbilityEntry const *skillLine = sSkillLineAbilityStore.LookupEntry(j);
                                    if (!skillLine)
                                            continue;
     
                                    // wrong skill
                                    if (skillLine->skillId != skill_id)
                                            continue;
     
                                    // not high rank
                                    if (skillLine->forward_spellid)
                                            continue;
     
                                    // skip racial skills
                                    if (skillLine->racemask != 0)
                                            continue;
     
                                    // skip wrong class skills
                                    if (skillLine->classmask && (skillLine->classmask & classmask) == 0)
                                            continue;
     
                                    SpellInfo const * spellInfo = sSpellMgr->GetSpellInfo(skillLine->spellId);
                                    if (!spellInfo || !SpellMgr::IsSpellValid(spellInfo, player, false))
                                            continue;
                                   
                                    player->learnSpell(skillLine->spellId, false);
                            }
                    }
     
                    bool IsSecondarySkill(SkillType skill) const
                    {
                            return skill == SKILL_COOKING || skill == SKILL_FIRST_AID;
                    }
     
                    void CompleteLearnProfession(Player *pPlayer, Creature *pCreature, SkillType skill)
                    {
                           /* if (PlayerAlreadyHasTwoProfessions(pPlayer) && !IsSecondarySkill(skill))
                                    pCreature->MonsterWhisper("Больше 2-ух професси иметь нельзя!", pPlayer->GetGUID());
                            else
                            {*/
                                    if (!LearnAllRecipesInProfession(pPlayer, skill))
                                            pCreature->MonsterWhisper("Internal error occured!", pPlayer->GetGUID());
                            // }*/
                    }
           
                    bool OnGossipSelect(Player* pPlayer, Creature* _creature, uint32 uiSender, uint32 uiAction)
                    {
                            pPlayer->PlayerTalkClass->ClearMenus();
           
                            if (uiSender == GOSSIP_SENDER_MAIN)
                            {
                   
                                    switch (uiAction)
                                    {
                                            case 196:
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_alchemy:30|t Алхимия.", GOSSIP_SENDER_MAIN, 1);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Ingot_05:30|t Кузнечное дело.", GOSSIP_SENDER_MAIN, 2);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_LeatherScrap_02:30|t Кожевничество.", GOSSIP_SENDER_MAIN, 3);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Fabric_Felcloth_Ebon:30|t Портняжное дело.", GOSSIP_SENDER_MAIN, 4);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_wrench_01:30|t Инженерное дело.", GOSSIP_SENDER_MAIN, 5);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_engraving:30|t Наложение чар.", GOSSIP_SENDER_MAIN, 6);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_gem_01:30|t Ювелирное дело.", GOSSIP_SENDER_MAIN, 7);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Scroll_08:30|t Начертание.", GOSSIP_SENDER_MAIN, 8);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\INV_Misc_Herb_07:30|t Травничество.", GOSSIP_SENDER_MAIN, 9);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\inv_misc_pelt_wolf_01:30|t Снятие шкур.", GOSSIP_SENDER_MAIN, 10);
                                                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_mining:30|t Горное дело.", GOSSIP_SENDER_MAIN, 11);
													pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_INTERACT_2, "|TInterface\\icons\\trade_fishing:30|t Рыболовное дело.", GOSSIP_SENDER_MAIN, 13);
                                                    pPlayer->PlayerTalkClass->SendGossipMenu(1, _creature->GetGUID());
                                                    break;
                                            case 1:
                                                    if(pPlayer->HasSkill(SKILL_ALCHEMY))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
     
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_ALCHEMY);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 2:
                                                    if(pPlayer->HasSkill(SKILL_BLACKSMITHING))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_BLACKSMITHING);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 3:
                                                    if(pPlayer->HasSkill(SKILL_LEATHERWORKING))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_LEATHERWORKING);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 4:
                                                    if(pPlayer->HasSkill(SKILL_TAILORING))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_TAILORING);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 5:
                                                    if(pPlayer->HasSkill(SKILL_ENGINEERING))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_ENGINEERING);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 6:
                                                    if(pPlayer->HasSkill(SKILL_ENCHANTING))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_ENCHANTING);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 7:
                                                    if(pPlayer->HasSkill(SKILL_JEWELCRAFTING))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_JEWELCRAFTING);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 8:
                                                    if(pPlayer->HasSkill(SKILL_INSCRIPTION))
                                                    {
                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                            break;
                                                    }
                                                    CompleteLearnProfession(pPlayer, _creature, SKILL_INSCRIPTION);
     
                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                    break;
                                            case 9:
                                            if(pPlayer->HasSkill(SKILL_HERBALISM))
                                            {
                                                pPlayer->PlayerTalkClass->SendCloseGossip();
											break;
                                                                                            }
                                                                                           
                                                                                            CompleteLearnProfession(pPlayer, _creature, SKILL_HERBALISM);
                                                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                            break;
                                                                                    case 10:
                                                                                            if(pPlayer->HasSkill(SKILL_SKINNING))
                                                                                            {
                                                                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                                    break;
                                                                                            }
                                                                                           
                                                                                            CompleteLearnProfession(pPlayer, _creature, SKILL_SKINNING);
                                                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                            break;
                                                                                    case 11:
                                                                                            if(pPlayer->HasSkill(SKILL_MINING))
                                                                                            {
                                                                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                                    break;
                                                                                            }
                                                                                           
                                                                                            CompleteLearnProfession(pPlayer, _creature, SKILL_MINING);
                                                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                            break;
                                                                                    case 12:
                                                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                            break;
																							
																					case 13:
                                                                                            if(pPlayer->HasSkill(SKILL_FISHING))
                                                                                            {
                                                                                                    pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                                    break;
                                                                                            }
                                                                                           
                                                                                            CompleteLearnProfession(pPlayer, _creature, SKILL_FISHING);
                                                                                            pPlayer->PlayerTalkClass->SendCloseGossip();
                                                                                            break;
                                    }
     
           
                            }
                            return true;
                    }
};

class Unell_Lottery : public CreatureScript
{
public:
    Unell_Lottery() : CreatureScript("Unell_Lottery") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		player->ADD_GOSSIP_ITEM(10, "Ну всё, хочу всё про*бать!!!", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Test", GOSSIP_SENDER_MAIN, 2);
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN)
            return true; // wrong sender, something fisy

        switch(action)
        {
        // case 1: ChatHandler(player->GetSession()).PSendSysMessage("123");       
		case 1:
                player->CLOSE_GOSSIP_MENU(); // Shattered Sun Magi
                player->AddItem(44113, 1);
                break;
		
		break;
		
        return true; // invalid class, something fishy
        }
       
        return true;
    }
};

class Unell_NPC_Spell : public CreatureScript
{
public:
    Unell_NPC_Spell() : CreatureScript("Unell_NPC_Spell") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		player->PlayerTalkClass->ClearMenus();
		player->ADD_GOSSIP_ITEM(10, "Узнать статус моих заклинаний", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Выносливость", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Интеллект", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(10, "Снижение получаемого урона", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(10, "Дух", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(10, "Сила заклинаний", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(10, "Сила атаки", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(10, "Ловкость", GOSSIP_SENDER_MAIN, 8);
        player->SEND_GOSSIP_MENU(1, creature->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
        if (sender != GOSSIP_SENDER_MAIN)
            return true; // wrong sender, something fisy

        switch(action)
        {
			case 100:
			OnGossipHello(player, creature);
			break;
			
			case 1:
			player->CLOSE_GOSSIP_MENU(); //      
			if(player->HasAura(92002)) // Стамина 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВыносливость |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}
				
			if(player->HasAura(92003)) // Стамина 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВыносливость |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}
				
			if(player->HasAura(92004)) // Стамина 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВыносливость |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}
				
			if(player->HasAura(92005)) // Стамина 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВыносливость |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}

			if(player->HasAura(92006)) // Стамина 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВыносливость |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92007)) // Стамина 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFВыносливость |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92008)) // Интеллект 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFИнтеллект |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92009)) // Интеллект 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFИнтеллект |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92010)) // Интеллект 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFИнтеллект |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92011)) // Интеллект 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFИнтеллект |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92012)) // Интеллект 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFИнтеллект |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92013)) // Интеллект 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFИнтеллект |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92014)) // Снижение получаемого урона 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСнижение получаемого урона |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92015)) // Снижение получаемого урона 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСнижение получаемого урона |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92016)) // Снижение получаемого урона 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСнижение получаемого урона |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92017)) // Снижение получаемого урона 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСнижение получаемого урона |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92018)) // Снижение получаемого урона 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСнижение получаемого урона |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92019)) // Снижение получаемого урона 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСнижение получаемого урона |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92020)) // Дух 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFДух |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92021)) // Дух 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFДух |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92022)) // Дух 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFДух |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92023)) // Дух 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFДух |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92024)) // Дух 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFДух |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92025)) // Дух 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFДух |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92026)) // Сила заклинаний 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила заклинаний |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92027)) // Сила заклинаний 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила заклинаний |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92028)) // Сила заклинаний 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила заклинаний |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92029)) // Сила заклинаний 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила заклинаний |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92030)) // Сила заклинаний 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила заклинаний |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92031)) // Сила заклинаний 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила заклинаний |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92032)) // Сила атаки 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила атаки |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92033)) // Сила атаки 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила атаки |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92034)) // Сила атаки 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила атаки |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92035)) // Сила атаки 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила атаки |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92036)) // Сила атаки 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила атаки |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92037)) // Сила атаки 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFСила атаки |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92038)) // Ловкость 1
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFЛовкость |cffff00001 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				
			if(player->HasAura(92039)) // Ловкость 2
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFЛовкость |cffff00002 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
			
			if(player->HasAura(92040)) // Ловкость 3
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFЛовкость |cffff00003 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	

			if(player->HasAura(92041)) // Ловкость 4
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFЛовкость |cffff00004 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}			
			
			if(player->HasAura(92042)) // Ловкость 5
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFЛовкость |cffff00005 Уровень из 5");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	

			if(player->HasAura(92043)) // Ловкость 6
				{
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000# |cff14ECCFЛовкость |cffff00006 Уровень (Тестовый)");
					// player->learnSpell(druid_spell_Typhoon, false);
				}	
				break;		
			
			case 2: // Меню выносливости 
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92002) && !player->HasAura(92003) && !player->HasAura(92004) && !player->HasAura(92005) && !player->HasAura(92006) && !player->HasAura(92007))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 10);
				}
				if(player->HasAura(92002))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 11);
				}
				if(player->HasAura(92003))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 12);
				}
				if(player->HasAura(92004))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 13);
				}
				if(player->HasAura(92005))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 14);
				}
				if(player->HasAura(92006))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 15);
				}
				if(player->HasAura(92007))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 15);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
				
			case 3: // Меню интеллекта 
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92008) && !player->HasAura(92009) && !player->HasAura(92010) && !player->HasAura(92011) && !player->HasAura(92012) && !player->HasAura(92013))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 20);
				}
				if(player->HasAura(92008))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 21);
				}
				if(player->HasAura(92009))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 22);
				}
				if(player->HasAura(92010))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 23);
				}
				if(player->HasAura(92011))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 24);
				}
				if(player->HasAura(92012))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 25);
				}
				if(player->HasAura(92013))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 25);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			
			case 4: // Меню снижения 
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92014) && !player->HasAura(92015) && !player->HasAura(92016) && !player->HasAura(92017) && !player->HasAura(92018) && !player->HasAura(92019))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 30);
				}
				if(player->HasAura(92014))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 31);
				}
				if(player->HasAura(92015))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 32);
				}
				if(player->HasAura(92016))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 33);
				}
				if(player->HasAura(92017))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 34);
				}
				if(player->HasAura(92018))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 35);
				}
				if(player->HasAura(92019))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 35);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			
			case 5: // Меню Духа
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92020) && !player->HasAura(92021) && !player->HasAura(92022) && !player->HasAura(92023) && !player->HasAura(92024) && !player->HasAura(92025))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 40);
				}
				if(player->HasAura(92020))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 41);
				}
				if(player->HasAura(92021))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 42);
				}
				if(player->HasAura(92022))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 43);
				}
				if(player->HasAura(92023))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 44);
				}
				if(player->HasAura(92024))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 45);
				}
				if(player->HasAura(92025))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 45);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
				
			case 6: // Меню СПД 
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92026) && !player->HasAura(92027) && !player->HasAura(92028) && !player->HasAura(92029) && !player->HasAura(92030) && !player->HasAura(92031))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 50);
				}
				if(player->HasAura(92026))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 51);
				}
				if(player->HasAura(92027))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 52);
				}
				if(player->HasAura(92028))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 53);
				}
				if(player->HasAura(92029))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 54);
				}
				if(player->HasAura(92030))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 55);
				}
				if(player->HasAura(92031))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 55);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
				
			case 7: // Меню АП 
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92032) && !player->HasAura(92033) && !player->HasAura(92034) && !player->HasAura(92035) && !player->HasAura(92036) && !player->HasAura(92037))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 60);
				}
				if(player->HasAura(92032))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 61);
				}
				if(player->HasAura(92033))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 62);
				}
				if(player->HasAura(92034))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 63);
				}
				if(player->HasAura(92035))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 64);
				}
				if(player->HasAura(92036))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 65);
				}
				if(player->HasAura(92037))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 65);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
			
			case 8: // Меню Агилы 
				player->PlayerTalkClass->ClearMenus();
				if(!player->HasAura(92038) && !player->HasAura(92039) && !player->HasAura(92040) && !player->HasAura(92041) && !player->HasAura(92042) && !player->HasAura(92043))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 1. + 1%.", GOSSIP_SENDER_MAIN, 70);
				}
				if(player->HasAura(92038))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 2. + 3%.", GOSSIP_SENDER_MAIN, 71);
				}
				if(player->HasAura(92039))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 3. + 5%.", GOSSIP_SENDER_MAIN, 72);
				}
				if(player->HasAura(92040))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 7%.", GOSSIP_SENDER_MAIN, 73);
				}
				if(player->HasAura(92041))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 5. + 10%.", GOSSIP_SENDER_MAIN, 74);
				}
				if(player->HasAura(92042))
				{
					player->ADD_GOSSIP_ITEM(10, "Изучить заклинание. Уровень 6. + 15%. (Тест версия спелла!)", GOSSIP_SENDER_MAIN, 75);
				}
				if(player->HasAura(92043))
				{
					player->ADD_GOSSIP_ITEM(10, "У вас уже 6 уровень заклинания, это максимальный и то тестовый.", GOSSIP_SENDER_MAIN, 75);
				}
				player->ADD_GOSSIP_ITEM(10, "Назад", GOSSIP_SENDER_MAIN, 100);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
				break;
				
			
			case 10:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92002, false);			
			break;
			
			case 11:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92003, false);			
			break;
			
			case 12:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92004, false);			
			break;
			
			case 13:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92005, false);			
			break;
			
			case 14:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92006, false);			
			break;
			
			case 15:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92007, false);			
			break;
			
			case 20:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92008, false);			
			break;
			
			case 21:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92009, false);			
			break;
			
			case 22:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92010, false);			
			break;
			
			case 23:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92011, false);			
			break;
			
			case 24:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92012, false);			
			break;
			
			case 25:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92013, false);			
			break;
			
			case 30:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92014, false);			
			break;
			
			case 31:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92015, false);			
			break;
			
			case 32:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92016, false);			
			break;
			
			case 33:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92017, false);			
			break;
			
			case 34:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92018, false);			
			break;
			
			case 35:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92019, false);			
			break;
			
			case 40:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92020, false);			
			break;
			
			case 41:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92021, false);			
			break;
			
			case 42:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92022, false);			
			break;
			
			case 43:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92023, false);			
			break;
			
			case 44:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92024, false);			
			break;
			
			case 45:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92025, false);			
			break;
			
			case 50:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92026, false);			
			break;
			
			case 51:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92027, false);			
			break;
			
			case 52:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92028, false);			
			break;
			
			case 53:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92029, false);			
			break;
			
			case 54:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92030, false);			
			break;
			
			case 55:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92031, false);			
			break;
			
			case 60:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92032, false);			
			break;
			
			case 61:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92033, false);			
			break;
			
			case 62:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92034, false);			
			break;
			
			case 63:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92035, false);			
			break;
			
			case 64:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92036, false);			
			break;
			
			case 65:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92037, false);			
			break;
			
			case 70:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92038, false);			
			break;
			
			case 71:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92039, false);			
			break;
			
			case 72:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92040, false);			
			break;
			
			case 73:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92041, false);			
			break;
			
			case 74:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92042, false);			
			break;
			
			case 75:
			player->CLOSE_GOSSIP_MENU();     
			player->learnSpell(92043, false);			
			break;		
			
			break;
		
        return true; // 
        }
       
        return true;
    }
};

class Gamble_npc : public  CreatureScript
{
public:
	Gamble_npc() : CreatureScript("Gamble_npc") {}
	

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->PlayerTalkClass->ClearMenus();
		
		uint32 coutitem = player->GetItemCount(EfirID);
		
		std::ostringstream gossipvalue;
		gossipvalue << "У вас " << coutitem << " Монет";
		player->ADD_GOSSIP_ITEM(10, gossipvalue.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);
		
		if (player->HasItemCount(EfirID, CostEfir1))
		{
			std::ostringstream gossiptext1;
			gossiptext1 << "|TInterface\\Icons\\INV_Ingot_01:25:25:-17:0|t Испытать шанс [" << CostEfir1 << " Монет]";
			player->ADD_GOSSIP_ITEM(10, gossiptext1.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
		}
			
		if (player->HasItemCount(EfirID, CostEfir2))
		{
			std::ostringstream gossiptext2;
			gossiptext2 << "|TInterface\\Icons\\INV_Ingot_01:25:25:-17:0|t Испытать шанс [" << CostEfir2 << " Монет]";
			player->ADD_GOSSIP_ITEM(10, gossiptext2.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
		}

		if (player->HasItemCount(EfirID, CostEfir3))
		{
			std::ostringstream gossiptext3;
			gossiptext3 << "|TInterface\\Icons\\INV_Ingot_01:25:25:-17:0|t Испытать шанс [" << CostEfir3 << " Монет]";
			player->ADD_GOSSIP_ITEM(10, gossiptext3.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
		}
		
		if (player->HasItemCount(EfirID, CostEfir4))
		{
			std::ostringstream gossiptext4;
			gossiptext4 << "|TInterface\\Icons\\INV_Ingot_01:25:25:-17:0|t Испытать шанс [" << CostEfir4 << " Монет]";
			player->ADD_GOSSIP_ITEM(10, gossiptext4.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
		}
		
		if (player->HasItemCount(EfirID, CostEfir5))
		{
			std::ostringstream gossiptext5;
			gossiptext5 << "|TInterface\\Icons\\INV_Ingot_01:25:25:-17:0|t Испытать шанс [" << CostEfir5 << " Монет]";
			player->ADD_GOSSIP_ITEM(10, gossiptext5.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
		}
		
		if (player->HasItemCount(EfirID, CostEfir6))
		{
			std::ostringstream gossiptext6;
			gossiptext6 << "|TInterface\\Icons\\INV_Ingot_01:25:25:-17:0|t Испытать шанс [" << CostEfir6 << " Монет]";
			player->ADD_GOSSIP_ITEM(10, gossiptext6.str(), GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
		}
		player->SEND_GOSSIP_MENU(1, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature * creature, uint32 Sender, uint32 action)
	{
		uint32 number = urand(1, 3); // 33% шанса

		player->PlayerTalkClass->ClearMenus();
		if (Sender == GOSSIP_SENDER_MAIN)
		switch (action)
		{
			case GOSSIP_ACTION_INFO_DEF + 1:
            {			
				if (number == 3)
				{
					uint32 reward = CostEfir1 * 2;
					player->AddItem(EfirID, reward);
					ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00Поздравляем! Вы выйграли %u Эфириальных монет", reward);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
				else
				{
					player->DestroyItemCount(EfirID, CostEfir1, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000К сожелению вы проиграли %u Эфириальных монет.", CostEfir1);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
			}
		    break;
			
			case GOSSIP_ACTION_INFO_DEF + 2:
            {			
				if (number == 3)
				{
					uint32 reward = CostEfir2 * 2;
					player->AddItem(EfirID, reward);
					ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00Поздравляем! Вы выйграли %u Эфириальных монет", reward);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
				else
				{
					player->DestroyItemCount(EfirID, CostEfir2, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000К сожелению вы проиграли %u Эфириальных монет.", CostEfir2);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
			}
		    break;
			
			case GOSSIP_ACTION_INFO_DEF + 3:
            {			
				if (number == 3)
				{
					uint32 reward = CostEfir3 * 2;
					player->AddItem(EfirID, reward);
					ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00Поздравляем! Вы выйграли %u Эфириальных монет", reward);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
				else
				{
					player->DestroyItemCount(EfirID, CostEfir3, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000К сожелению вы проиграли %u Эфириальных монет.", CostEfir3);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
			}
		    break;
			
			case GOSSIP_ACTION_INFO_DEF + 4:
            {			
				if (number == 3)
				{
					uint32 reward = CostEfir4 * 2;
					player->AddItem(EfirID, reward);
					ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00Поздравляем! Вы выйграли %u Эфириальных монет", reward);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
				else
				{
					player->DestroyItemCount(EfirID, CostEfir4, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000К сожелению вы проиграли %u Эфириальных монет.", CostEfir4);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
			}
		    break;
			
			case GOSSIP_ACTION_INFO_DEF + 5:
            {			
				if (number == 3)
				{
					uint32 reward = CostEfir5 * 2;
					player->AddItem(EfirID, reward);
					ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00Поздравляем! Вы выйграли %u Эфириальных монет", reward);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
				else
				{
					player->DestroyItemCount(EfirID, CostEfir5, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000К сожелению вы проиграли %u Эфириальных монет.", CostEfir5);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
			}
		    break;
			
			case GOSSIP_ACTION_INFO_DEF + 6:
            {			
				if (number == 3)
				{
					uint32 reward = CostEfir6 * 2;
					player->AddItem(EfirID, reward);
					ChatHandler(player->GetSession()).PSendSysMessage("|cff00ff00Поздравляем! Вы выйграли %u Эфириальных монет", reward);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
				else
				{
					player->DestroyItemCount(EfirID, CostEfir6, true);
					ChatHandler(player->GetSession()).PSendSysMessage("|cffff0000К сожелению вы проиграли %u Эфириальных монет.", CostEfir6);
					player->PlayerTalkClass->SendCloseGossip();
					return true;
				}
			}
		    break;
			
            case GOSSIP_ACTION_INFO_DEF + 7:
            OnGossipHello(player, creature);
            break;			

		 }
		return true;
	}
};

class Unell_Changeclass_NPC : public CreatureScript
{
public:
    Unell_Changeclass_NPC() : CreatureScript("Unell_Changeclass_NPC") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		
		player->PlayerTalkClass->ClearMenus();
		uint32 accid = player->GetSession()->GetAccountId();
		uint32 selfguid = sObjectMgr->GetPlayerGUIDByName(player->GetName());
		
		player->ADD_GOSSIP_ITEM(10, "Информация", GOSSIP_SENDER_MAIN, 10001);
		QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM player_changeclass WHERE account = %u AND status = 0", accid);
		if(result)
		{
			Field * field = result->Fetch();	
			uint32 guidlow = field[0].GetUInt32();
			if (selfguid != guidlow)
			{
				
				player->ADD_GOSSIP_ITEM(10, "Одобрить смену класса", GOSSIP_SENDER_MAIN, 999);
			}
			else
				player->ADD_GOSSIP_ITEM(10, "Вам нужно зайти на друго персонажа и подтвердить смену класса с него", GOSSIP_SENDER_MAIN, 10000);
			
		}
		
		QueryResult resultstatus = CharacterDatabase.PQuery("SELECT status FROM player_changeclass WHERE guid = %u", selfguid);
		if(resultstatus)
		{
			Field * field = resultstatus->Fetch();	
			uint32 status = field[0].GetUInt32();
			
			if (status == 0)
			{
				player->ADD_GOSSIP_ITEM(10, "Отменить заявку на смену класса", GOSSIP_SENDER_MAIN, 1100);
			}
			
			if (status == 1)
			{
				player->ADD_GOSSIP_ITEM(10, "Я сменил класс и хочу поменять вещи под свой класс!", GOSSIP_SENDER_MAIN, 700);
				player->ADD_GOSSIP_ITEM(10, "Мне не нужно менять вещи, мне эти подходят", GOSSIP_SENDER_MAIN, 910);
			}

			if (status == 2)
			{
				if (player->getClass() == CLASS_PALADIN || player->getClass() == CLASS_SHAMAN || player->getClass() == CLASS_DRUID)
				player->ADD_GOSSIP_ITEM(10, "Мне нужно поменять мои новые вещи под другую ветку", GOSSIP_SENDER_MAIN, 900);
				else
				player->ADD_GOSSIP_ITEM(10, "Я поменял все вещи которые мне нужно", GOSSIP_SENDER_MAIN, 910);
			}

			if (status == 3)
			{
				if (player->getClass() == CLASS_PALADIN || player->getClass() == CLASS_SHAMAN || player->getClass() == CLASS_DRUID)
				{
					player->ADD_GOSSIP_ITEM(10, "Я не так поменял вещи, хочу другие :(", GOSSIP_SENDER_MAIN, 900);
					player->ADD_GOSSIP_ITEM(10, "Я поменял все вещи которые мне нужно", GOSSIP_SENDER_MAIN, 910);
				}
				
			}
			
		}
		if(!resultstatus)
		player->ADD_GOSSIP_ITEM(10, "Я хочу сменить свой класс", GOSSIP_SENDER_MAIN, 1);
	
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		uint32 plguid = sObjectMgr->GetPlayerGUIDByName(player->GetName());
		uint32 accid = player->GetSession()->GetAccountId();
		ChatHandler handler(player);
		player->PlayerTalkClass->ClearMenus();		
		uint32 oldclass = 0;
		
		uint32 Entry = creature->GetEntry();
		CreatureTemplate const* cInfo = creature->GetCreatureTemplate();
		CreatureTemplate const* CreatureTemplate = sObjectMgr->GetCreatureTemplate(Entry);
		CreatureLocale const* CreatureLocale = sObjectMgr->GetCreatureLocale(Entry);
		char const* crename = NULL;
			if (CreatureLocale)
				crename = CreatureLocale->Name[handler.GetSessionDbcLocale()].c_str();
			if ((!crename || !*crename) && CreatureTemplate)
				crename = CreatureTemplate->Name.c_str();
			if (!crename)
				crename = "Unknown creature";
					
		switch(player->getClass()) 
		{
			case CLASS_DEATH_KNIGHT:	oldclass = 6;				break;
			case CLASS_DRUID:			oldclass = 11; 				break;
			case CLASS_HUNTER:			oldclass = 3; 				break;
			case CLASS_MAGE:			oldclass = 8; 				break;
			case CLASS_PALADIN:			oldclass = 2; 				break;
			case CLASS_PRIEST:			oldclass = 5; 				break;
			case CLASS_ROGUE:			oldclass = 4;				break;
			case CLASS_SHAMAN:			oldclass = 7; 				break;
			case CLASS_WARLOCK:			oldclass = 9; 				break;
			case CLASS_WARRIOR:			oldclass = 1; 				break;
		}
		
		uint32 faction = 0;
		switch (player->GetTeam())
		{
			case ALLIANCE:	faction = 1;	break;
			case HORDE:		faction = 2;	break;
		}		

		switch (action)
		{		
			case 10000:
				OnGossipHello(player, creature);
				break;
			case 10001:				
				player->CLOSE_GOSSIP_MENU();
				handler.PSendSysMessage("|cFFFF0000#|r Перед вами нпц для смены класса: |cffff0000%s", crename);
				handler.PSendSysMessage("|cFFFF0000#|r Для начала выберите какой класс вам по душе.");
				handler.PSendSysMessage("|cFFFF0000#|r Затем нужно будет подтвердить смену класса с альтернативного персонажа на вашем аккаунте.");
				handler.PSendSysMessage("|cFFFF0000#|r Далее, у этого же нпц сменить вещи под новый класс.");
				handler.PSendSysMessage("|cFFFF0000#|r После всех проделанных пунктов можно играть уже новым классом :)");
				break;
			case 1: // Выбор класса	
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Война", GOSSIP_SENDER_MAIN, 1000);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Паладина", GOSSIP_SENDER_MAIN, 1001);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Охотника", GOSSIP_SENDER_MAIN, 1002);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Разбойника", GOSSIP_SENDER_MAIN, 1003);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Жреца", GOSSIP_SENDER_MAIN, 1004);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Рыцаря Смерти", GOSSIP_SENDER_MAIN, 1005);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Шамана", GOSSIP_SENDER_MAIN, 1006);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Мага", GOSSIP_SENDER_MAIN, 1007);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Чернокнижника", GOSSIP_SENDER_MAIN, 1008);
				player->ADD_GOSSIP_ITEM(10, "Сменить класс на Друида", GOSSIP_SENDER_MAIN, 1009);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());				
				break;
			
			case 900: // Выбор ветки
				player->PlayerTalkClass->ClearMenus();
				
								
				if (oldclass == 2) // Пал
				{
					player->ADD_GOSSIP_ITEM(10, "Выбрать вещи на силу атаки", GOSSIP_SENDER_MAIN, 400);
					player->ADD_GOSSIP_ITEM(10, "Выбрать вещи на силу заклинаний", GOSSIP_SENDER_MAIN, 401);
				}
				
				if (oldclass == 7) // Шам
				{
					player->ADD_GOSSIP_ITEM(10, "Выбрать вещи на силу атаки", GOSSIP_SENDER_MAIN, 500);
					player->ADD_GOSSIP_ITEM(10, "Выбрать вещи на силу заклинаний", GOSSIP_SENDER_MAIN, 501);
				}
				
				if (oldclass == 11) // Друид
				{
					player->ADD_GOSSIP_ITEM(10, "Выбрать вещи на силу атаки", GOSSIP_SENDER_MAIN, 600);
					player->ADD_GOSSIP_ITEM(10, "Выбрать вещи на силу заклинаний", GOSSIP_SENDER_MAIN, 601);
					
				}
				
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,creature->GetGUID());				
				break;			
			
			case 700: // Замена вещей подтверждение		
				player->PlayerTalkClass->ClearMenus();
				player->ADD_GOSSIP_ITEM(10, "Все мои старые вещи в сумке и я готов сделать замену вещей", GOSSIP_SENDER_MAIN, 800);
				player->ADD_GOSSIP_ITEM(10, "Не-не-не, погоди, посмотрю ещё почту, вроде там чего-то есть", GOSSIP_SENDER_MAIN, 10000);
				player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());				
				break;
				
			case 400: // Замена вещей пал сила атаки	
				player->CLOSE_GOSSIP_MENU();
				{
					QueryResult result = WorldDatabase.PQuery("SELECT paladindd, paladinspd FROM player_changetalent_items");
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 ddid = fields[0].GetUInt32();
							uint32 spdid = fields[1].GetUInt32();					
							if (player->HasItemCount(spdid, 1))
							{
								player->DestroyItemCount(spdid, 1, true);
								player->AddItem(ddid, 1);
							}
						} 
					while(result->NextRow());
					}
						
					if(!result)
					handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
				
					CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 3 WHERE guid = %u AND status = 2", plguid);				
				}
				break;
				
			case 401: // Замена вещей пал сила заклинаний
				player->CLOSE_GOSSIP_MENU();
				{
					QueryResult result = WorldDatabase.PQuery("SELECT paladindd, paladinspd FROM player_changetalent_items");
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 ddid = fields[0].GetUInt32();
							uint32 spdid = fields[1].GetUInt32();					
							if (player->HasItemCount(ddid, 1))
							{
								player->DestroyItemCount(ddid, 1, true);
								player->AddItem(spdid, 1);
							}
						} 
					while(result->NextRow());
					}
						
					if(!result)
					handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
				
					CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 3 WHERE guid = %u AND status = 2", plguid);				
				}				
				break;
				
			case 500: // Замена вещей шаман сила атаки	
				player->CLOSE_GOSSIP_MENU();
				{
					QueryResult result = WorldDatabase.PQuery("SELECT shamandd, shamanspd FROM player_changetalent_items");
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 ddid = fields[0].GetUInt32();
							uint32 spdid = fields[1].GetUInt32();					
							if (player->HasItemCount(spdid, 1))
							{
								player->DestroyItemCount(spdid, 1, true);
								player->AddItem(ddid, 1);
							}
						} 
					while(result->NextRow());
					}
						
					if(!result)
					handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
				
					CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 3 WHERE guid = %u AND status = 2", plguid);				
				}
				break;
				
			case 501: // Замена вещей шаман сила заклинаний
				player->CLOSE_GOSSIP_MENU();
				{
					QueryResult result = WorldDatabase.PQuery("SELECT shamandd, shamanspd FROM player_changetalent_items");
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 ddid = fields[0].GetUInt32();
							uint32 spdid = fields[1].GetUInt32();					
							if (player->HasItemCount(ddid, 1))
							{
								player->DestroyItemCount(ddid, 1, true);
								player->AddItem(spdid, 1);
							}
						} 
					while(result->NextRow());
					}
						
					if(!result)
					handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
				
					CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 3 WHERE guid = %u AND status = 2", plguid);				
				}				
				break;
				
			case 600: // Замена вещей друид сила атаки	
				player->CLOSE_GOSSIP_MENU();
				{
					QueryResult result = WorldDatabase.PQuery("SELECT druiddd, druidspd FROM player_changetalent_items");
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 ddid = fields[0].GetUInt32();
							uint32 spdid = fields[1].GetUInt32();					
							if (player->HasItemCount(spdid, 1))
							{
								player->DestroyItemCount(spdid, 1, true);
								player->AddItem(ddid, 1);
							}
						} 
					while(result->NextRow());
					}
						
					if(!result)
					handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
				
					CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 3 WHERE guid = %u AND status = 2", plguid);				
				}
				break;
				
			case 601: // Замена вещей друид сила заклинаний
				player->CLOSE_GOSSIP_MENU();
				{
					QueryResult result = WorldDatabase.PQuery("SELECT druiddd, druidspd FROM player_changetalent_items");
					if(result)
					{
						do
						{
							Field * fields = result->Fetch();
							uint32 ddid = fields[0].GetUInt32();
							uint32 spdid = fields[1].GetUInt32();					
							if (player->HasItemCount(ddid, 1))
							{
								player->DestroyItemCount(ddid, 1, true);
								player->AddItem(spdid, 1);
							}
						} 
					while(result->NextRow());
					}
						
					if(!result)
					handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
				
					CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 3 WHERE guid = %u AND status = 1", plguid);				
				}				
				break;
				
			case 800:
			player->CLOSE_GOSSIP_MENU();
			player->resetTalents(true);
			player->SendTalentsInfoData(false);
			player->resetSpells(false);
			{
				QueryResult resultnewclass = CharacterDatabase.PQuery("SELECT newclass FROM player_changeclass WHERE guid = %u AND status = 1", plguid);
				QueryResult resultoldclass = CharacterDatabase.PQuery("SELECT oldclass FROM player_changeclass WHERE guid = %u AND status = 1", plguid);
				
				uint32 newclass = resultnewclass->Fetch()->GetUInt32();
				uint32 oldclass = resultoldclass->Fetch()->GetUInt32();
				
				std::string strnewclass;
				std::string stroldclass;
				
				switch (newclass)
				{
					case 1:		strnewclass = "warrior";	break;
					case 2:		strnewclass = "paladin";	break;
					case 3:		strnewclass = "hunter";		break;
					case 4:		strnewclass = "rogue";		break;
					case 5:		strnewclass = "priest";		break;
					case 6:		strnewclass = "dk";			break;
					case 7:		strnewclass = "shaman";		break;
					case 8:		strnewclass = "mage";		break;
					case 9:		strnewclass = "warlock";	break;
					case 11:	strnewclass = "druid";		break;
				}
				
				switch (oldclass)
				{
					case 1:		stroldclass = "warrior";	break;
					case 2:		stroldclass = "paladin";	break;
					case 3:		stroldclass = "hunter";		break;
					case 4:		stroldclass = "rogue";		break;
					case 5:		stroldclass = "priest";		break;
					case 6:		stroldclass = "dk";			break;
					case 7:		stroldclass = "shaman";		break;
					case 8:		stroldclass = "mage";		break;
					case 9:		stroldclass = "warlock";	break;
					case 11:	stroldclass = "druid";		break;
				}
				
			QueryResult result = WorldDatabase.PQuery("SELECT %s, %s FROM player_changeclass_items", stroldclass.c_str(), strnewclass.c_str());
			if(result)
			{
				do
				{
					Field * fields = result->Fetch();
					uint32 oldid = fields[0].GetUInt32();
					uint32 newid = fields[1].GetUInt32();					
					if (player->HasItemCount(oldid, 1))
					{
						player->DestroyItemCount(oldid, 1, true);
						player->AddItem(newid, 1);
					}
				} 
			while(result->NextRow());
			}
				
			if(!result)
			handler.PSendSysMessage("|cffff0000# |cff00ff00Нет ответа от базы...");
		
			CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 2 WHERE guid = %u AND status = 1", plguid);						
			}
				break;
				
			case 999: // Сама смена класса
                player->CLOSE_GOSSIP_MENU();				
				{
					QueryResult result = CharacterDatabase.PQuery("SELECT guid, newclass, faction FROM player_changeclass WHERE account = %u AND status = 0", accid);
					if(result)
					{
						Field * field = result->Fetch();
						uint32 playerguid = field[0].GetUInt32();
						uint32 newclass = field[1].GetUInt32();
						uint32 factionchar = field[2].GetUInt32();
						
						if (newclass == 1)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 6 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 2)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 10 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 3)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 4 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 6 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 4)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 5 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 5)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 10 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 6)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 2 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 7)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 11 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 6 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 8)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 10 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 9)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 1 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 10 WHERE guid = %u", newclass, playerguid);
						}
						
						if (newclass == 11)
						{
							if (factionchar == 1)
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 4 WHERE guid = %u", newclass, playerguid);
							else
								CharacterDatabase.PExecute("UPDATE `characters` SET class = %u, race = 6 WHERE guid = %u", newclass, playerguid);
						}
						CharacterDatabase.PExecute("UPDATE `characters` SET `at_login`='128' WHERE (`guid`='%u')", playerguid);
						CharacterDatabase.PExecute("UPDATE `player_changeclass` SET status = 1 WHERE guid = %u AND status = 0", playerguid);				
					}
				}					
				handler.PSendSysMessage("|cFFFF0000#|cff6C8CD5 Вы подтвердили смену класса. Вам добавлена дополнительно смена рассы.");
                break;
			
			case 1000:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
				
				if (oldclass == 1)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так воин, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '1', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}				
                break;
			case 1001:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 2)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так паладин, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '2', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
				break;
			case 1002:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 3)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так охотник, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '3', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
				break;				
			case 1003:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
				if (oldclass == 4)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так разбойник, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '4', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1004:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 5)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так жрец, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '5', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1005:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 6)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так рыцарь смерти, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '6', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1006:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 7)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так шаман, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '7', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1007:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 8)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так маг, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '8', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1008:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
               if (oldclass == 9)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так чернокнижник, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '9', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1009:
                player->CLOSE_GOSSIP_MENU();
				player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
                if (oldclass == 11)
					handler.PSendSysMessage("|cFFFF0000#|r Вы уже и так друид, куда ещё?");
				else
				{
					CharacterDatabase.PExecute("INSERT INTO `player_changeclass` (`guid`, `account`, `oldclass`, `newclass`, `status`, `faction`) VALUES ('%u', '%u', '%u', '11', '0', '%u')", plguid, accid, oldclass, faction);
					handler.PSendSysMessage("|cFFFF0000#|r Вы подали заявку на сменну класса. Подтвердите смену с другого персонажа на этом же аккаунте.");
				}
                break;
			case 1100:
                player->CLOSE_GOSSIP_MENU();
                {
					QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM player_changeclass WHERE guid = %u", plguid);
					if (result)
					{
						CharacterDatabase.PExecute("DELETE FROM `player_changeclass` WHERE (`guid`='%u')", plguid);
						handler.PSendSysMessage("|cFFFF0000#|r Вы удалили свою заявку на смену класса.");
					}
					else
						handler.PSendSysMessage("|cFFFF0000#|r Вы не подавали заявку на смену класса");
				}				
                break;
			case 910:
                player->CLOSE_GOSSIP_MENU();
                player->resetTalents(true);
				player->SendTalentsInfoData(false);
				player->resetSpells(false);
				{
					QueryResult result = CharacterDatabase.PQuery("SELECT guid FROM player_changeclass WHERE guid = %u", plguid);
					if (result)
					{
						CharacterDatabase.PExecute("DELETE FROM `player_changeclass` WHERE (`guid`='%u')", plguid);	
						handler.PSendSysMessage("|cFFFF0000#|r Тогда всё готово, можете начинать играть.");
					}					
				}				
                break;
	}
		return true;
	}
};

#define DEFAULT_MESSAGE 1
 
struct VisualData
{
    uint32 Menu;
    uint32 Submenu;
    uint32 Icon;
    uint32 Id;
    std::string Name;
};
 
VisualData vData[] =
{
    { 1, 0, GOSSIP_ICON_BATTLE, 3789, "|TInterface\\icons\\Spell_chargepositive:30:|tBerserking" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3854, "|TInterface\\icons\\Spell_chargepositive:30:|tSpell Power" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3273, "|TInterface\\icons\\Spell_chargepositive:30:|tDeathfrost" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3225, "|TInterface\\icons\\Spell_chargepositive:30:|tExecutioner" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3870, "|TInterface\\icons\\Spell_chargepositive:30:|tBlood Draining" },
    { 1, 0, GOSSIP_ICON_BATTLE, 1899, "|TInterface\\icons\\Spell_chargepositive:30:|tUnholy Weapon" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2674, "|TInterface\\icons\\Spell_chargepositive:30:|tSpellsurge" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2675, "|TInterface\\icons\\Spell_chargepositive:30:|tBattlemaster" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2671, "|TInterface\\icons\\Spell_chargepositive:30:|tArcane and Fire Spell Power" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2672, "|TInterface\\icons\\Spell_chargepositive:30:|tShadow and Frost Spell Power" },
    { 1, 0, GOSSIP_ICON_BATTLE, 3365, "|TInterface\\icons\\Spell_chargepositive:30:|tRune of Swordshattering" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2673, "|TInterface\\icons\\Spell_chargepositive:30:|tMongoose" },
    { 1, 0, GOSSIP_ICON_BATTLE, 2343, "|TInterface\\icons\\Spell_chargepositive:30:|tSpell Power" },
    { 1, 2, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargepositive:30:|tСлед страница" },
 
    { 2, 0, GOSSIP_ICON_BATTLE, 425, "|TInterface\\icons\\Spell_chargepositive:30:|tBlack Temple Dummy" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3855, "|TInterface\\icons\\Spell_chargepositive:30:|tSpell Power III" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1894, "|TInterface\\icons\\Spell_chargepositive:30:|tIcy Weapon" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1103, "|TInterface\\icons\\Spell_chargepositive:30:|tAgility" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1898, "|TInterface\\icons\\Spell_chargepositive:30:|tLifestealing" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3345, "|TInterface\\icons\\Spell_chargepositive:30:|tEarthliving I" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1743, "|TInterface\\icons\\Spell_chargepositive:30:|tMHTest02" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3093, "|TInterface\\icons\\Spell_chargepositive:30:|tAttack Power vs Undead and Demons" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1900, "|TInterface\\icons\\Spell_chargepositive:30:|tCrusader" },
    { 2, 0, GOSSIP_ICON_BATTLE, 3846, "|TInterface\\icons\\Spell_chargepositive:30:|tSpell Power II" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1606, "|TInterface\\icons\\Spell_chargepositive:30:|tAttack Power" },
    { 2, 0, GOSSIP_ICON_BATTLE, 283, "|TInterface\\icons\\Spell_chargepositive:30:|tWindfury I" },
    { 2, 0, GOSSIP_ICON_BATTLE, 1, "|TInterface\\icons\\Spell_chargepositive:30:|tRockbiter III" },
    { 2, 3, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargepositive:30:|tСлед страница" },
    { 2, 1, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargenegative:30:|tНазад" },
 
    { 3, 0, GOSSIP_ICON_BATTLE, 3265, "|TInterface\\icons\\Spell_chargepositive:30:|tBlessed Weapon Coating" },
    { 3, 0, GOSSIP_ICON_BATTLE, 2, "|TInterface\\icons\\Spell_chargepositive:30:|tFrostbrand I" },
    { 3, 0, GOSSIP_ICON_BATTLE, 3, "|TInterface\\icons\\Spell_chargepositive:30:|tFlametongue III" },
    { 3, 0, GOSSIP_ICON_BATTLE, 3266, "|TInterface\\icons\\Spell_chargepositive:30:|tRighteous Weapon Coating" },
    { 3, 0, GOSSIP_ICON_BATTLE, 1903, "|TInterface\\icons\\Spell_chargepositive:30:|tSpirit" },
    { 3, 0, GOSSIP_ICON_BATTLE, 13, "|TInterface\\icons\\Spell_chargepositive:30:|tSharpened" },
    { 3, 0, GOSSIP_ICON_BATTLE, 26, "|TInterface\\icons\\Spell_chargepositive:30:|tFrost Oil" },
    { 3, 0, GOSSIP_ICON_BATTLE, 7, "|TInterface\\icons\\Spell_chargepositive:30:|tDeadly Poison" },
    { 3, 0, GOSSIP_ICON_BATTLE, 803, "|TInterface\\icons\\Spell_chargepositive:30:|tFiery Weapon" },
    { 3, 0, GOSSIP_ICON_BATTLE, 1896, "|TInterface\\icons\\Spell_chargepositive:30:|tWeapon Damage" },
    { 3, 0, GOSSIP_ICON_BATTLE, 2666, "|TInterface\\icons\\Spell_chargepositive:30:|tIntellect" },
    { 3, 0, GOSSIP_ICON_BATTLE, 25, "|TInterface\\icons\\Spell_chargepositive:30:|tShadow Oil" },
    { 3, 2, GOSSIP_ICON_TALK, 0, "|TInterface\\icons\\Spell_chargenegative:30:|tНазад" },
};
 
class NPC_VisualWeapon : public CreatureScript
{
public:
    NPC_VisualWeapon() : CreatureScript("VisualWeapon") { }
 
    bool MainHand;
 
    void SetVisual(Player* player, uint32 visual)
    {
        uint8 slot = MainHand ? EQUIPMENT_SLOT_MAINHAND : EQUIPMENT_SLOT_OFFHAND;
 
        Item* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, slot);
 
        if (!item)
        {
            ChatHandler(player->GetSession()).PSendSysMessage("Нету оружия для этого слота.");
            return;
        }
 
        const ItemTemplate* itemTemplate = item->GetTemplate();
 
        if (itemTemplate->SubClass == ITEM_SUBCLASS_ARMOR_SHIELD ||
            itemTemplate->SubClass == ITEM_SUBCLASS_ARMOR_BUCKLER ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_SPEAR ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_BOW ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_GUN ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_EXOTIC2 ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_THROWN ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_CROSSBOW ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_WAND ||
            itemTemplate->SubClass == ITEM_SUBCLASS_WEAPON_FISHING_POLE)
            return;
 
        player->SetUInt16Value(PLAYER_VISIBLE_ITEM_1_ENCHANTMENT + (item->GetSlot() * 2), 0, visual);
    }
 
    void GetMenu(Player* player, Creature* creature, uint32 menuId)
    {
        for (uint8 i = 0; i < (sizeof(vData) / sizeof(*vData)); i++)
        {
            if (vData[i].Menu == menuId)
                player->ADD_GOSSIP_ITEM(vData[i].Icon, vData[i].Name, GOSSIP_SENDER_MAIN, i);
        }
 
        player->SEND_GOSSIP_MENU(DEFAULT_MESSAGE, creature->GetGUID());
    }
 
    bool OnGossipHello(Player* player, Creature* creature)
    {
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/PaperDoll/UI-PaperDoll-Slot-MainHand:30:|t Правая Рука", GOSSIP_SENDER_MAIN, 1);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/PaperDoll/UI-PaperDoll-Slot-SecondaryHand:30:|t Левая Рука", GOSSIP_SENDER_MAIN, 2);
        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "|TInterface/PaperDollInfoFrame/UI-GearManager-Undo:30:|t Выйти", GOSSIP_SENDER_MAIN, 3);
 
        player->SEND_GOSSIP_MENU(DEFAULT_MESSAGE, creature->GetGUID());
        return true;
    }
 
    bool OnGossipSelect(Player* player, Creature* creature, uint32 sender, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();
 
        switch (action)
        {
        case 1:
            MainHand = true;
            GetMenu(player, creature, 1);
            return false;
 
        case 2:
            MainHand = false;
            GetMenu(player, creature, 1);
            return false;
 
        case 3:
            player->CLOSE_GOSSIP_MENU();
            return false;
        }
 
        uint32 menuData = vData[action].Submenu;
 
        if (menuData == 0)
        {
            SetVisual(player, vData[action].Id);
            menuData = vData[action].Menu;
        }
 
        GetMenu(player, creature, menuData);
        return true;
    }
};

class Unell_Vendor_NPC_ILVL_Start : public CreatureScript
{
public:
    Unell_Vendor_NPC_ILVL_Start() : CreatureScript("Unell_Vendor_NPC_ILVL_Start") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
		
		/*uint32 avgILvl = player->GetAverageItemLevel();	
		
		if (avgILvl < 300)
		{
			player->CLOSE_GOSSIP_MENU();
			handler.PSendSysMessage("|cFFFF0000#|r |cff00ff00Cредний уровень предметов не позволяет покупать вам такие вещи. Нужно как минимум|r 300|cff00ff00. А у вас|r %u|cff00ff00.", avgILvl);
			return true;
		}*/
		
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Старт Сет", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Старт НоН Сет", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Старт Оружия", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		ChatHandler handler(player);
		
		switch (action)
		{		
			case 1: // Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300000);
				break;
			case 2: // НоН Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300012);
				break;
			case 3: // Оружия		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300001);
				break;
		}
		return true;
	}
};

class Unell_Vendor_NPC_ILVL_1 : public CreatureScript
{
public:
    Unell_Vendor_NPC_ILVL_1() : CreatureScript("Unell_Vendor_NPC_ILVL_1") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
		
		uint32 avgILvl = player->GetAverageItemLevel();	
		
		if (avgILvl < 300)
		{
			player->CLOSE_GOSSIP_MENU();
			handler.PSendSysMessage("|cFFFF0000#|r |cff00ff00Cредний уровень предметов не позволяет покупать вам такие вещи. Нужно как минимум|r 300|cff00ff00. А у вас|r %u|cff00ff00.", avgILvl);
			return true;
		}
		
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Тир 1 Сет", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Тир 1 НоН Сет", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Тир 1 Оружия", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		ChatHandler handler(player);
		
		switch (action)
		{		
			case 1: // Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300013);
				break;
			case 2: // НоН Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300002);
				break;
			case 3: // Оружия		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300003);
				break;
		}
		return true;
	}
};

class Unell_Vendor_NPC_ILVL_2 : public CreatureScript
{
public:
    Unell_Vendor_NPC_ILVL_2() : CreatureScript("Unell_Vendor_NPC_ILVL_2") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
		
		uint32 avgILvl = player->GetAverageItemLevel();	
		
		if (avgILvl < 350)
		{
			player->CLOSE_GOSSIP_MENU();
			handler.PSendSysMessage("|cFFFF0000#|r |cff00ff00Cредний уровень предметов не позволяет покупать вам такие вещи. Нужно как минимум|r 350|cff00ff00. А у вас|r %u|cff00ff00.", avgILvl);
			return true;
		}
		
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Тир 2 Сет", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Тир 2 НоН Сет", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Тир 2 Оружия", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		ChatHandler handler(player);
		
		switch (action)
		{		
			case 1: // Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300004);
				break;
			case 2: // НоН Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300014);
				break;
			case 3: // Оружия		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300005);
				break;
		}
		return true;
	}
};

class Unell_Vendor_NPC_ILVL_3 : public CreatureScript
{
public:
    Unell_Vendor_NPC_ILVL_3() : CreatureScript("Unell_Vendor_NPC_ILVL_3") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
		
		uint32 avgILvl = player->GetAverageItemLevel();	
		
		if (avgILvl < 400)
		{
			player->CLOSE_GOSSIP_MENU();
			handler.PSendSysMessage("|cFFFF0000#|r |cff00ff00Cредний уровень предметов не позволяет покупать вам такие вещи. Нужно как минимум|r 400|cff00ff00. А у вас|r %u|cff00ff00.", avgILvl);
			return true;
		}
		
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Тир 3 Сет", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Тир 3 НоН Сет", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Тир 3 Оружия", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		ChatHandler handler(player);
		
		switch (action)
		{		
			case 1: // Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300006);
				break;
			case 2: // НоН Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300016);
				break;
			case 3: // Оружия		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300007);
				break;
		}
		return true;
	}
};

class Unell_Vendor_NPC_ILVL_4 : public CreatureScript
{
public:
    Unell_Vendor_NPC_ILVL_4() : CreatureScript("Unell_Vendor_NPC_ILVL_4") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
		
		uint32 avgILvl = player->GetAverageItemLevel();	
		
		if (avgILvl < 450)
		{
			player->CLOSE_GOSSIP_MENU();
			handler.PSendSysMessage("|cFFFF0000#|r |cff00ff00Cредний уровень предметов не позволяет покупать вам такие вещи. Нужно как минимум|r 450|cff00ff00. А у вас|r %u|cff00ff00.", avgILvl);
			return true;
		}
		
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Тир 4 Сет", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Тир 4 НоН Сет", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Тир 4 Оружия", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		ChatHandler handler(player);
		
		switch (action)
		{		
			case 1: // Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300008);
				break;
			case 2: // НоН Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300015);
				break;
			case 3: // Оружия		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300009);
				break;
		}
		return true;
	}
};

class Unell_Vendor_NPC_ILVL_5 : public CreatureScript
{
public:
    Unell_Vendor_NPC_ILVL_5() : CreatureScript("Unell_Vendor_NPC_ILVL_5") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
		
		uint32 avgILvl = player->GetAverageItemLevel();	
		
		if (avgILvl < 500)
		{
			player->CLOSE_GOSSIP_MENU();
			handler.PSendSysMessage("|cFFFF0000#|r |cff00ff00Cредний уровень предметов не позволяет покупать вам такие вещи. Нужно как минимум|r 500|cff00ff00. А у вас|r %u|cff00ff00.", avgILvl);
			return true;
		}
		
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Тир 5 Сет", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Тир 5 НоН Сет", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(10, "Тир 5 Оружия", GOSSIP_SENDER_MAIN, 3);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		
		ChatHandler handler(player);
		
		switch (action)
		{		
			case 1: // Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300010);
				break;
			case 2: // НоН Сет		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300019);
				break;
			case 3: // Оружия		
				player->GetSession()->SendListInventory(creature->GetGUID(), 300011);
				break;
		}
		return true;
	}
};

class Unell_Prize_OBT : public CreatureScript
{
public:
    Unell_Prize_OBT() : CreatureScript("Unell_Prize_OBT") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
		ChatHandler handler(player);	
				
		player->PlayerTalkClass->ClearMenus();
        player->ADD_GOSSIP_ITEM(10, "Информация", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(10, "Получить награду", GOSSIP_SENDER_MAIN, 2);
		player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, creature->GetGUID());
		return true;
	}
	
	
	bool OnGossipSelect(Player* player, Creature* creature, uint32 uiSender, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();		
		ChatHandler handler(player);
		uint32 accid = player->GetSession()->GetAccountId();
		uint32 dateOBT = ConfigMgr::GetIntDefault("Unell.OBT.Prize.Date", 1504213200);
		time_t curTime = time(NULL);
		uint32 localtime = curTime;
		
		switch (action)
		{		
			case 1: // Инфа	
				player->CLOSE_GOSSIP_MENU();
				handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Перед вами нпц для выдачи наград, для тех кто помогал администрации тестировать игровой мир");
				handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Данный нпц выдаёт награды тому, кто был зарегистрирован, когда сервер находился в режиме ОБТ");
				break;
			case 2: // Получение плюшки		
				player->CLOSE_GOSSIP_MENU();
				{
					// SELECT UNIX_TIMESTAMP(joindate) from account where id = 2;
					QueryResult res = LoginDatabase.PQuery("SELECT UNIX_TIMESTAMP(joindate) FROM account WHERE id = %u", accid);	
					if (res)
					{
						uint32 joindate = res->Fetch()->GetUInt32();
						
						QueryResult resdate = LoginDatabase.PQuery("SELECT FROM_UNIXTIME(%u)", dateOBT);
						std::string resdateOBT = resdate->Fetch()->GetCString();
						
						QueryResult resdatejoin = LoginDatabase.PQuery("SELECT FROM_UNIXTIME(%u)", joindate);
						std::string datejoin = resdatejoin->Fetch()->GetCString();
						
						
						QueryResult resacc = CharacterDatabase.PQuery("SELECT account FROM `prize_obt` WHERE `account` = '%u' ", accid);
						
						if (resacc)
						{
							handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Вы уже получали награду. Она выдаётся только один раз, увы...");
						}
						
						if (!resacc)
						{
							if (joindate > dateOBT)
							handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Ваш аккаунт зарегистрирован пожже: |cffff0000%s|cff00ff00. Следовательно, вы мне могли быть на ОБТ", resdateOBT.c_str());
							else
							{
								handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Ваш аккаунт зарегистрирован: |cffff0000%s|cff00ff00. Следовательно, вы могли быть на ОБТ", datejoin.c_str());
								handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Поэтому вы получаете награду от администрации. Приятной игры.");
								CharacterDatabase.PExecute("INSERT INTO `prize_obt` (`account`, `date`) VALUES ('%u', FROM_UNIXTIME(%u))", accid, localtime);
								player->AddItem(120000, 500);								
							}
						}			

					}
					if (!res)
						handler.PSendSysMessage("|cFFFF0000#|r|cff00ff00 Проблемы с аккаунтом о.о");
				}
				player->GetSession()->SendListInventory(creature->GetGUID(), 400018);
				break;
		}
		return true;
	}
};

enum Spellsvuran
{
	SPELL_STRELA = 71130,
	Spell_Pokrov = 43897,
};

class Unell_boss : public CreatureScript
{
public:

	Unell_boss() : CreatureScript("Unell_boss"){}

	struct Unell_bossAI : public ScriptedAI
	{
		Unell_bossAI(Creature* creature) : ScriptedAI(creature){}
		uint32 SPELL_STRELA_Timer;
		uint8 Phase;
		uint32 Phase1_Timer;
		uint32 Phase2_Timer;

		void Reset()
		{
			SPELL_STRELA_Timer = 3000;
			DoCast(me, 43897);
			Phase1_Timer = 5000;
			Phase2_Timer = 5000;
			Phase = 1;
			SetCombatMovement(true);
		}

		void JustDied(Unit * /* killer */)
		{
			me->MonsterYell("Я... умираю...", LANG_UNIVERSAL, 0);
		}
		
		void KilledUnit(Unit* victim)
		{
			me->MonsterYell("Ещё одна жертва!", LANG_UNIVERSAL, 0);			
		}
		
		void EnterCombat(Unit* who)
		{
			me->MonsterYell("Кто вы? Вы совершаете большую ошибку. Тут вы встретите только свою СМЕРТЬ!...", LANG_UNIVERSAL, 0);
			SummonValkira(me->getVictim());
            me->RemoveAurasDueToSpell(43897);
		}
		
		void SummonValkira(Unit* victim)
        {
			if (Creature* Valkira = DoSpawnCreature(330000, float(irand(-9, 9)), float(irand(-9, 9)), 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
				Valkira->AI()->AttackStart(victim);
		}

		void UpdateAI(uint32 diff)
		{
			if (!UpdateVictim())
				return;
			
            SetCombatMovement(false);

			DoMeleeAttackIfReady();
		}
	};

	CreatureAI* GetAI(Creature* creature) const
	{
		return new Unell_bossAI(creature);
	}

};

class npc_enchantment_private : public CreatureScript
{
public:
    npc_enchantment_private() : CreatureScript("npc_enchantment_private") { }

	bool OnGossipHello(Player* player, Creature* creature)
	{
		player->ADD_GOSSIP_ITEM(1, "Зачарить Правую руку", GOSSIP_SENDER_MAIN, 1);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Левую руку", GOSSIP_SENDER_MAIN, 13);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Двуручное Оружие", GOSSIP_SENDER_MAIN, 2);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Щит", GOSSIP_SENDER_MAIN, 3);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Голову", GOSSIP_SENDER_MAIN, 4);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Плечи", GOSSIP_SENDER_MAIN, 5);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Спину", GOSSIP_SENDER_MAIN, 6);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Грудь", GOSSIP_SENDER_MAIN, 7);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Запястья", GOSSIP_SENDER_MAIN, 8);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Руки", GOSSIP_SENDER_MAIN, 9);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Ноги", GOSSIP_SENDER_MAIN, 10);
		player->ADD_GOSSIP_ITEM(1, "Зачарить Ступни", GOSSIP_SENDER_MAIN, 11);

		if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
		{
			player->ADD_GOSSIP_ITEM(1, "Зачарить Кольцо # 1", GOSSIP_SENDER_MAIN, 12);
			player->ADD_GOSSIP_ITEM(1, "Зачарить Кольцо # 2", GOSSIP_SENDER_MAIN, 1200);
		}
		player->ADD_GOSSIP_ITEM(1, "Я хотел бы снять мои чары", GOSSIP_SENDER_MAIN, 14);

		player->PlayerTalkClass->SendGossipMenu(100001, creature->GetGUID());
		return true;
	}

	bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
	{
		player->PlayerTalkClass->ClearMenus();
		Item * item;

		switch (action)
		{

			case 1: // Enchant Weapon
				if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
				{
					player->ADD_GOSSIP_ITEM(1, "Отведение удара", GOSSIP_SENDER_MAIN, 102);
					player->ADD_GOSSIP_ITEM(1, "Вытягивание крови", GOSSIP_SENDER_MAIN, 103);
				}
				player->ADD_GOSSIP_ITEM(1, "+ 26 к ловкости", GOSSIP_SENDER_MAIN, 100);
				player->ADD_GOSSIP_ITEM(1, "+ 45 к духу", GOSSIP_SENDER_MAIN, 101);
				player->ADD_GOSSIP_ITEM(1, "Боевое исступление", GOSSIP_SENDER_MAIN, 104);
				player->ADD_GOSSIP_ITEM(1, "+ 25 к меткости и + 25 к крит. удару", GOSSIP_SENDER_MAIN, 105);
				player->ADD_GOSSIP_ITEM(1, "Чёрная магия", GOSSIP_SENDER_MAIN, 106);
				player->ADD_GOSSIP_ITEM(1, "Военачальник", GOSSIP_SENDER_MAIN, 107);
				player->ADD_GOSSIP_ITEM(1, "Оружие Ледомола", GOSSIP_SENDER_MAIN, 108);
				player->ADD_GOSSIP_ITEM(1, "Оберег", GOSSIP_SENDER_MAIN, 109);
				player->ADD_GOSSIP_ITEM(1, "+ 50 к выносливости", GOSSIP_SENDER_MAIN, 110);
				player->ADD_GOSSIP_ITEM(1, "+ 65 к силе атаки", GOSSIP_SENDER_MAIN, 111);
				player->ADD_GOSSIP_ITEM(1, "+ 63 к силе заклинаний", GOSSIP_SENDER_MAIN, 112);
				player->ADD_GOSSIP_ITEM(1, "Мангуст", GOSSIP_SENDER_MAIN, 113);
				player->ADD_GOSSIP_ITEM(1, "Палач", GOSSIP_SENDER_MAIN, 114);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
				return true;
				break;

			case 2: // Enchant 2H Weapon
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

					if (!item)
					{
						player->GetSession()->SendNotification("Сначала нужно одеть двуручное оружие!");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
					{
						player->ADD_GOSSIP_ITEM(1, "Боевое исступление", GOSSIP_SENDER_MAIN, 104);
						player->ADD_GOSSIP_ITEM(1, "+ 81 к силе заклинаний", GOSSIP_SENDER_MAIN, 115);
						player->ADD_GOSSIP_ITEM(1, "+ 35 к ловкости", GOSSIP_SENDER_MAIN, 116);
						player->ADD_GOSSIP_ITEM(1, "+ 110 к силе атаки", GOSSIP_SENDER_MAIN, 117);
						player->ADD_GOSSIP_ITEM(1, "Мангуст", GOSSIP_SENDER_MAIN, 113);
						player->ADD_GOSSIP_ITEM(1, "Палач", GOSSIP_SENDER_MAIN, 114);
						player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
						player->PlayerTalkClass->SendGossipMenu(100003, creature->GetGUID());
						return true;
					}
					else
					{
						player->GetSession()->SendNotification("У вас не одето двуручное оружие!");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 3: // Enchant Shield
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if (!item)
					{
						player->GetSession()->SendNotification("Сначала нужно одеть щит!");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						player->ADD_GOSSIP_ITEM(1, "+ 20 к рейтингу защиты", GOSSIP_SENDER_MAIN, 118);
						player->ADD_GOSSIP_ITEM(1, "+ 25 к интеллекту", GOSSIP_SENDER_MAIN, 119);
						player->ADD_GOSSIP_ITEM(1, "+ 12 к рейтингу устойчивости", GOSSIP_SENDER_MAIN, 120);
						player->ADD_GOSSIP_ITEM(1, "Титановая обшивка", GOSSIP_SENDER_MAIN, 121);
						player->ADD_GOSSIP_ITEM(1, "+ 18 к выносливости", GOSSIP_SENDER_MAIN, 122);
						player->ADD_GOSSIP_ITEM(1, "+ 36 к блокированию", GOSSIP_SENDER_MAIN, 123);
						player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
						player->PlayerTalkClass->SendGossipMenu(100004, creature->GetGUID());
						return true;
					}
					else
					{
						player->GetSession()->SendNotification("У вас не одет щит!");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 4: // Enchant Head
				player->ADD_GOSSIP_ITEM(1, "+ 30 к силе заклинаний и + 10 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 124);
				player->ADD_GOSSIP_ITEM(1, "+ 30 к силе заклинаний и + 20 к крит. удару ", GOSSIP_SENDER_MAIN, 125);
				player->ADD_GOSSIP_ITEM(1, "+ 29 к силе заклинаний и + 20 к устойчивости", GOSSIP_SENDER_MAIN, 126);
				player->ADD_GOSSIP_ITEM(1, "+ 30 к выносливости и + 25 к устойчивости", GOSSIP_SENDER_MAIN, 127);
				player->ADD_GOSSIP_ITEM(1, "+ 37 к выносливости и + 20 к защите", GOSSIP_SENDER_MAIN, 128);
				player->ADD_GOSSIP_ITEM(1, "+ 50 к силе атаки и + 20 к крит. удару", GOSSIP_SENDER_MAIN, 129);
				player->ADD_GOSSIP_ITEM(1, "+ 50 к силе атаки и + 20 к устойчивости", GOSSIP_SENDER_MAIN, 130);
				player->ADD_GOSSIP_ITEM(1, "+ 25 к сопр. тайной магии и + 30 к выносливости", GOSSIP_SENDER_MAIN, 131);
				player->ADD_GOSSIP_ITEM(1, "+ 25 к сопр. огню и + 30 к выносливости", GOSSIP_SENDER_MAIN, 132);
				player->ADD_GOSSIP_ITEM(1, "+ 25 к сопр. тёмной магии и + 30 к выносливости", GOSSIP_SENDER_MAIN, 133);
				player->ADD_GOSSIP_ITEM(1, "+ 25 к сопр. магии льда и + 30 к выносливости", GOSSIP_SENDER_MAIN, 134);
				player->ADD_GOSSIP_ITEM(1, "+ 25 к сопр. силы природы и + 30 к выносливости", GOSSIP_SENDER_MAIN, 135);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100005, creature->GetGUID());
				return true;
				break;

			case 5: // Enchant Shoulders
				if (player->HasSkill(SKILL_INSCRIPTION) && player->GetSkillValue(SKILL_INSCRIPTION) > 400)
				{
					player->ADD_GOSSIP_ITEM(1, "+ 120 к силе атаки и + 15 к крит. удару", GOSSIP_SENDER_MAIN, 136);
					player->ADD_GOSSIP_ITEM(1, "+ 70 к силе заклинаний и + 8 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 137);
					player->ADD_GOSSIP_ITEM(1, "+ 60 к рейтингу уклонения и + 15 к защите", GOSSIP_SENDER_MAIN, 138);
					player->ADD_GOSSIP_ITEM(1, "+ 70 к силе заклинаний и + 15 к крит. эффекта", GOSSIP_SENDER_MAIN, 139);
				}
				player->ADD_GOSSIP_ITEM(1, "+ 40 к силе атаки и + 15 к крит. удару", GOSSIP_SENDER_MAIN, 140);
				player->ADD_GOSSIP_ITEM(1, "+ 24 к силе заклинаний и + 8 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 141);
				player->ADD_GOSSIP_ITEM(1, "+ 30 к выносливости и + 15 к устойчивости", GOSSIP_SENDER_MAIN, 142);
				player->ADD_GOSSIP_ITEM(1, "+ 20 к рейтингу уклонения и + 15 к защите", GOSSIP_SENDER_MAIN, 143);
				player->ADD_GOSSIP_ITEM(1, "+ 24 к силе заклинаний и + 15 к крит. удара", GOSSIP_SENDER_MAIN, 144);
				player->ADD_GOSSIP_ITEM(1, "+ 23 к силе заклинаний и + 15 к устойчивости", GOSSIP_SENDER_MAIN, 145);
				player->ADD_GOSSIP_ITEM(1, "+ 40 к силе атаки и + 15 к устойчивости", GOSSIP_SENDER_MAIN, 146);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100006, creature->GetGUID());
				return true;
				break;

			case 6: // Enchant Cloak
				player->ADD_GOSSIP_ITEM(1, "Уменьшение скорости падения", GOSSIP_SENDER_MAIN, 147);
				player->ADD_GOSSIP_ITEM(1, "Улучшенная незаметность + 10 к ловкости", GOSSIP_SENDER_MAIN, 148);
				player->ADD_GOSSIP_ITEM(1, "Вышивка тёмного сияния", GOSSIP_SENDER_MAIN, 149);
				player->ADD_GOSSIP_ITEM(1, "Светлотканная вышивка", GOSSIP_SENDER_MAIN, 150);
				player->ADD_GOSSIP_ITEM(1, "Вышивка в виде рукояти меча", GOSSIP_SENDER_MAIN, 151);
				player->ADD_GOSSIP_ITEM(1, "+ 10 к духу и снижение угрозы на 2%", GOSSIP_SENDER_MAIN, 152);
				player->ADD_GOSSIP_ITEM(1, "+ 16 к защите", GOSSIP_SENDER_MAIN, 153);
				player->ADD_GOSSIP_ITEM(1, "+ 35 к проникающей способности заклинаний", GOSSIP_SENDER_MAIN, 154);
				player->ADD_GOSSIP_ITEM(1, "+ 225 к броне", GOSSIP_SENDER_MAIN, 155);
				player->ADD_GOSSIP_ITEM(1, "+ 22 к ловкости" , GOSSIP_SENDER_MAIN, 156);
				player->ADD_GOSSIP_ITEM(1, "+ 23 к скорости", GOSSIP_SENDER_MAIN, 157);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100007, creature->GetGUID());
				return true;
				break;

			case 7: //Enchant chest
				player->ADD_GOSSIP_ITEM(1, "+ 10 ко всему", GOSSIP_SENDER_MAIN, 158);
				player->ADD_GOSSIP_ITEM(1, "+ 275 к здоровью", GOSSIP_SENDER_MAIN, 159);
				player->ADD_GOSSIP_ITEM(1, "+ 10 ед. маны каждые 5 сек", GOSSIP_SENDER_MAIN, 160);
				player->ADD_GOSSIP_ITEM(1, "+ 20 к устойчивости", GOSSIP_SENDER_MAIN, 161);
				player->ADD_GOSSIP_ITEM(1, "+ 22 к защите", GOSSIP_SENDER_MAIN, 162);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100008, creature->GetGUID());
				return true;
				break;

			case 8: //Enchant Bracers
				player->ADD_GOSSIP_ITEM(1, "+ 40 к выносливости", GOSSIP_SENDER_MAIN, 163);
				player->ADD_GOSSIP_ITEM(1, "+ 30 к силе заклинаний", GOSSIP_SENDER_MAIN, 164);
				player->ADD_GOSSIP_ITEM(1, "+ 50 к силе атаки", GOSSIP_SENDER_MAIN, 165);
				player->ADD_GOSSIP_ITEM(1, "+ 18 к духу", GOSSIP_SENDER_MAIN, 166);
				player->ADD_GOSSIP_ITEM(1, "+ 15 к мастерству", GOSSIP_SENDER_MAIN, 167);
				player->ADD_GOSSIP_ITEM(1, "+ 6 ко всему", GOSSIP_SENDER_MAIN, 168);
				player->ADD_GOSSIP_ITEM(1, "+ 16 к интеллекту", GOSSIP_SENDER_MAIN, 169);
				player->ADD_GOSSIP_ITEM(1, "+ 70 к сопротивлению тайной магии", GOSSIP_SENDER_MAIN, 170);
				player->ADD_GOSSIP_ITEM(1, "+ 70 к сопротивлению магии огня", GOSSIP_SENDER_MAIN, 171);
				player->ADD_GOSSIP_ITEM(1, "+ 70 к сопротивлению магии льда", GOSSIP_SENDER_MAIN, 172);
				player->ADD_GOSSIP_ITEM(1, "+ 70 к сопротивлению силам природы", GOSSIP_SENDER_MAIN, 173);
				player->ADD_GOSSIP_ITEM(1, "+ 70 к сопротивлению тёмной магии", GOSSIP_SENDER_MAIN, 174);
				player->ADD_GOSSIP_ITEM(1, "+ 130 к силе атаки", GOSSIP_SENDER_MAIN, 175);
				player->ADD_GOSSIP_ITEM(1, "+ 102 к выносливости", GOSSIP_SENDER_MAIN, 176);
				player->ADD_GOSSIP_ITEM(1, "+ 76 к силе заклинаний", GOSSIP_SENDER_MAIN, 177);
				//player->ADD_GOSSIP_ITEM(1, "+ Гнездо в наручах", GOSSIP_SENDER_MAIN, 414);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100009, creature->GetGUID());
				return true;
				break;

			case 9: //Enchant Gloves
				player->ADD_GOSSIP_ITEM(1, "+ 16 к крит. удару", GOSSIP_SENDER_MAIN, 178);
				player->ADD_GOSSIP_ITEM(1, "+ 2% угрозы и + 10 к паррированию", GOSSIP_SENDER_MAIN, 179);
				player->ADD_GOSSIP_ITEM(1, "+ 44 к силе атаки", GOSSIP_SENDER_MAIN, 180);
				player->ADD_GOSSIP_ITEM(1, "+ 20 к ловкости", GOSSIP_SENDER_MAIN, 181);
				player->ADD_GOSSIP_ITEM(1, "+ 20 к меткости", GOSSIP_SENDER_MAIN, 182);
				player->ADD_GOSSIP_ITEM(1, "+ 15 к мастерству", GOSSIP_SENDER_MAIN, 183);
				player->ADD_GOSSIP_ITEM(1, "Выстрел ракетой", GOSSIP_SENDER_MAIN, 199);
				// player->ADD_GOSSIP_ITEM(1, "+ Гнездо в перчатках", GOSSIP_SENDER_MAIN, 415);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100010, creature->GetGUID());
				return true;
				break;

			case 10: //Enchant legs
				player->ADD_GOSSIP_ITEM(1, "+ 40 к устойчивости и + 28 к выносливости", GOSSIP_SENDER_MAIN, 184);
				player->ADD_GOSSIP_ITEM(1, "+ 55 к выносливости и + 22 к ловкости", GOSSIP_SENDER_MAIN, 185);
				player->ADD_GOSSIP_ITEM(1, "+ 75 к силе атаки и + 22 к крит. удару", GOSSIP_SENDER_MAIN, 186);
				player->ADD_GOSSIP_ITEM(1, "+ 50 к силе заклинаний + 20 к духу", GOSSIP_SENDER_MAIN, 187);
				player->ADD_GOSSIP_ITEM(1, "+ 50 к силе заклинаний + 30 к выносливости", GOSSIP_SENDER_MAIN, 188);
				player->ADD_GOSSIP_ITEM(1, "+ 72 к выносливости + 35 к ловкости", GOSSIP_SENDER_MAIN, 189);
				player->ADD_GOSSIP_ITEM(1, "+ 100 к силе атаки и + 36 к крит. удару", GOSSIP_SENDER_MAIN, 190);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100011, creature->GetGUID());
				return true;
				break;

			case 11: //Enchant feet
				player->ADD_GOSSIP_ITEM(1, "+ 32 к силе атаки", GOSSIP_SENDER_MAIN, 191);
				player->ADD_GOSSIP_ITEM(1, "+ 15 к выносливости и ув. скорости бега", GOSSIP_SENDER_MAIN, 192);
				player->ADD_GOSSIP_ITEM(1, "+ 16 к ловкости", GOSSIP_SENDER_MAIN, 193);
				player->ADD_GOSSIP_ITEM(1, "+ 18 к духу", GOSSIP_SENDER_MAIN, 194);
				player->ADD_GOSSIP_ITEM(1, "+ 7 ед. здоровья и маны каждые 5 сек.", GOSSIP_SENDER_MAIN, 195);
				player->ADD_GOSSIP_ITEM(1, "+ 12 к рейтингу меткости + 12 к крит. удару", GOSSIP_SENDER_MAIN, 196);
				player->ADD_GOSSIP_ITEM(1, "+ 22  к выносливости", GOSSIP_SENDER_MAIN, 197);
				player->ADD_GOSSIP_ITEM(1, "Нитроускоритель", GOSSIP_SENDER_MAIN, 198);
				player->ADD_GOSSIP_ITEM(1, "Рейтинг скорости + 340 на 12 сек", GOSSIP_SENDER_MAIN, 200);
				player->ADD_GOSSIP_ITEM(1, "+ 885 к броне", GOSSIP_SENDER_MAIN, 201);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100012, creature->GetGUID());
				return true;
				break;

			case 12: //Enchant rings
				player->ADD_GOSSIP_ITEM(1, "+ 40 к силе атаки", GOSSIP_SENDER_MAIN, 202);
				player->ADD_GOSSIP_ITEM(1, "+ 23 к силе заклинаний", GOSSIP_SENDER_MAIN, 203);
				player->ADD_GOSSIP_ITEM(1, "+ 30 к выносливости", GOSSIP_SENDER_MAIN, 204);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
				return true;
				break;
				
			case 1200: //Enchant rings
				player->ADD_GOSSIP_ITEM(1, "+ 40 к силе атаки", GOSSIP_SENDER_MAIN, 2022);
				player->ADD_GOSSIP_ITEM(1, "+ 23 к силе заклинаний", GOSSIP_SENDER_MAIN, 2032);
				player->ADD_GOSSIP_ITEM(1, "+ 30 к выносливости", GOSSIP_SENDER_MAIN, 2042);
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100013, creature->GetGUID());
				return true;
				break;

			case 13: //Enchant Off-Hand weapons
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendNotification("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						player->ADD_GOSSIP_ITEM(1, "Отведение удара", GOSSIP_SENDER_MAIN, 205);
						player->ADD_GOSSIP_ITEM(1, "Вытягивание крови", GOSSIP_SENDER_MAIN, 219);
						player->ADD_GOSSIP_ITEM(1, "+ 26 к ловкости", GOSSIP_SENDER_MAIN, 206);
						player->ADD_GOSSIP_ITEM(1, "+ 45 к духу", GOSSIP_SENDER_MAIN, 207);
						player->ADD_GOSSIP_ITEM(1, "Боевое исступление", GOSSIP_SENDER_MAIN, 208);
						player->ADD_GOSSIP_ITEM(1, "+ 25 к меткости и + 25 к крит. удару", GOSSIP_SENDER_MAIN, 209);
						player->ADD_GOSSIP_ITEM(1, "Чёрная магия", GOSSIP_SENDER_MAIN, 210);
						player->ADD_GOSSIP_ITEM(1, "Военачальник", GOSSIP_SENDER_MAIN, 211);
						player->ADD_GOSSIP_ITEM(1, "Оружие Ледомола", GOSSIP_SENDER_MAIN, 212);
						player->ADD_GOSSIP_ITEM(1, "Оберег", GOSSIP_SENDER_MAIN, 213);
						player->ADD_GOSSIP_ITEM(1, "+ 50 к выносливости", GOSSIP_SENDER_MAIN, 214);
						player->ADD_GOSSIP_ITEM(1, "+ 65 к силе атаки", GOSSIP_SENDER_MAIN, 215);
						player->ADD_GOSSIP_ITEM(1, "+ 63 к силе заклинаний", GOSSIP_SENDER_MAIN, 216);
						player->ADD_GOSSIP_ITEM(1, "Мангуст", GOSSIP_SENDER_MAIN, 217);
						player->ADD_GOSSIP_ITEM(1, "Палач", GOSSIP_SENDER_MAIN, 218);
						player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
						player->PlayerTalkClass->SendGossipMenu(100002, creature->GetGUID());
						return true;
					}
					else
					{
						player->GetSession()->SendNotification("У вас не одето леворучное оружие");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;					

			case 14: //Remove enchant menu
				player->ADD_GOSSIP_ITEM(1, " Снять чары с правой руки", GOSSIP_SENDER_MAIN, 400);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с левой руки", GOSSIP_SENDER_MAIN, 401);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с двуручного оружия", GOSSIP_SENDER_MAIN, 413);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с щита", GOSSIP_SENDER_MAIN, 412);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с головы", GOSSIP_SENDER_MAIN, 402);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с плеч", GOSSIP_SENDER_MAIN, 403);
				player->ADD_GOSSIP_ITEM(1, "Снять чары со спины", GOSSIP_SENDER_MAIN, 404);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с груди", GOSSIP_SENDER_MAIN, 405);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с запястьев", GOSSIP_SENDER_MAIN, 406);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с рук", GOSSIP_SENDER_MAIN, 407);
				player->ADD_GOSSIP_ITEM(1, "Снять чары с ног", GOSSIP_SENDER_MAIN, 408);
				player->ADD_GOSSIP_ITEM(1, "Снять чары со ступней", GOSSIP_SENDER_MAIN, 409);
				if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
				{
					player->ADD_GOSSIP_ITEM(1, "Снять чары с кольца # 1", GOSSIP_SENDER_MAIN, 410);
					player->ADD_GOSSIP_ITEM(1, "Снять чары с кольца # 2", GOSSIP_SENDER_MAIN, 411);
				}
				player->ADD_GOSSIP_ITEM(1, "Назад", GOSSIP_SENDER_MAIN, 300);
				player->PlayerTalkClass->SendGossipMenu(100014, creature->GetGUID());
				return true;
				break;

			case 100:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_AGILITY_1H);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 101:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_SPIRIT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 102:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLADE_WARD);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 103:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLOOD_DRAINING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 104:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BERSERKING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 105:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ACCURACY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 106:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BLACK_MAGIC);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 107:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_BATTLEMASTER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 108:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_ICEBREAKER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 109:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_LIFEWARD);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 110:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_TITANGUARD);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 111:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_POTENCY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 112:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 113:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MONGOOSE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 114:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_WEP_EXECUTIONER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 115:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

					if (!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужно двуручное оружие");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_GREATER_SPELL_POWER);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели двуручное оружие");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 116:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

					if (!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужно двуручное оружие");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_AGILITY);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели двуручное оружие");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 117:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND);

					if (!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужно двуручное оружие");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_2HWEAPON)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND), ENCHANT_2WEP_MASSACRE);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели двуручное оружие");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 118:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_DEFENSE);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 119:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_INTELLECT);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 120:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_RESILIENCE);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 121:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TITANIUM_PLATING);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 122:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_STAMINA);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 123:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого нужен щит");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_SHIELD)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_SHIELD_TOUGHSHIELD);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Вы не одели щит");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 124:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BLISSFUL_MENDING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 125:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_BURNING_MYSTERIES);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 126:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_DOMINANCE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 127:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_SAVAGE_GLADIATOR);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 128:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_STALWART_PROTECTOR);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 129:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TORMENT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 130:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TRIUMPH);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 131:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_ECLIPSED_MOON);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 132:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLAME_SOUL);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 133:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FLEEING_SHADOW);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 134:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_FROSTY_SOUL);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 135:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD), ENCHANT_HEAD_TOXIC_WARDING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 136:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_AXE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 137:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_CRAG);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 138:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_PINNACLE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 139:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_MASTERS_STORM);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 140:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_AXE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 141:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_CRAG);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 142:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_GLADIATOR);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 143:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_PINNACLE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 144:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_GREATER_STORM);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 145:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_DOMINANCE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 146:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS), ENCHANT_SHOULDER_TRIUMPH);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 147:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPRINGY_ARACHNOWEAVE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 148:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SHADOW_ARMOR);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 149:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_DARKGLOW_EMBROIDERY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 150:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_LIGHTWEAVE_EMBROIDERY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 151:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SWORDGUARD_EMBROIDERY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 152:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_WISDOM);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 153:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_TITANWEAVE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 154:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_SPELL_PIERCING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 155:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MIGHTY_ARMOR);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 156:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_MAJOR_AGILITY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 157:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK), ENCHANT_CLOAK_GREATER_SPEED);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 158:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_POWERFUL_STATS);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 159:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_SUPER_HEALTH);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 160:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_MAINA_REST);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 161:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_EXCEPTIONAL_RESIL);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 162:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST), ENCHANT_CHEST_GREATER_DEFENSE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 163:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_STAMINA);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 164:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_SUPERIOR_SP);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 165:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_ASSUALT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 166:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_MAJOR_SPIRT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 167:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_EXPERTISE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 168:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_GREATER_STATS);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 169:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_INTELLECT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 170:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ARCANE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 171:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FIRE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 172:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_FROST);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 173:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_NATURE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 174:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SHADOW);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 175:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_ATTACK);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 176:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_STAMINA);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 177:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS), ENCHANT_BRACERS_FURL_SPELLPOWER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 178:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_GREATER_BLASTING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 179:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_ARMSMAN);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 180:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_CRUSHER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 181:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_AGILITY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 182:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_PRECISION);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 183:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_GLOVES_EXPERTISE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 184:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_EARTHEN);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 185:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_FROSTHIDE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 186:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_ICESCALE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 187:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_BRILLIANT_SPELLTHREAD);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 188:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_SAPPHIRE_SPELLTHREAD);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 189:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_DRAGONSCALE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 190:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS), ENCHANT_LEG_WYRMSCALE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 191:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_ASSULT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 192:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_TUSKARS_VITLIATY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 193:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_SUPERIOR_AGILITY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 194:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_SPIRIT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 195:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_VITALITY);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 196:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ICEWALKER);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 197:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_GREATER_FORTITUDE);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 198:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_NITRO_BOOTS);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 199:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS), ENCHANT_BOOTS_PYRO_ROCKET);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 200:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_HYPERSPEED);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 201:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET), ENCHANT_BOOTS_ARMOR_WEBBING);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 202:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_ASSULT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 203:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_GREATER_SP);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 204:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1), ENCHANT_RING_STAMINA);
				player->PlayerTalkClass->SendCloseGossip();
				break;
				
			case 2022:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_ASSULT);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 2032:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_GREATER_SP);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 2042:
				Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2), ENCHANT_RING_STAMINA);
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 205:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLADE_WARD);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 206:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_AGILITY_1H);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 207:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_SPIRIT);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 208:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BERSERKING);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 209:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ACCURACY);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 210:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLACK_MAGIC);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 211:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BATTLEMASTER);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 212:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_ICEBREAKER);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 213:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_LIFEWARD);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 214:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_TITANGUARD);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 215:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_POTENCY);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 216:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_MIGHTY_SPELL_POWER);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 217:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_2WEP_MONGOOSE);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 218:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_EXECUTIONER);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 219:
				{
					item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND);

					if(!item)
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
						return false;
					}

					if (item->GetTemplate()->InventoryType == INVTYPE_WEAPONOFFHAND)
					{
						Enchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND), ENCHANT_WEP_BLOOD_DRAINING);
						player->PlayerTalkClass->SendCloseGossip();
					}
					else
					{
						player->GetSession()->SendAreaTriggerMessage("Для этого должно быть одето оружие левой руки");
						player->PlayerTalkClass->SendCloseGossip();
					}
				}
				break;

			case 300: //Назад menu
				{
					player->ADD_GOSSIP_ITEM(1, "Зачарить Правую руку", GOSSIP_SENDER_MAIN, 1);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Левую руку", GOSSIP_SENDER_MAIN, 13);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Двуручное Оружие", GOSSIP_SENDER_MAIN, 2);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Щит", GOSSIP_SENDER_MAIN, 3);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Голову", GOSSIP_SENDER_MAIN, 4);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Плечи", GOSSIP_SENDER_MAIN, 5);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Спину", GOSSIP_SENDER_MAIN, 6);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Грудь", GOSSIP_SENDER_MAIN, 7);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Запястья", GOSSIP_SENDER_MAIN, 8);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Руки", GOSSIP_SENDER_MAIN, 9);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Ноги", GOSSIP_SENDER_MAIN, 10);
					player->ADD_GOSSIP_ITEM(1, "Зачарить Ступни", GOSSIP_SENDER_MAIN, 11);

					if (player->HasSkill(SKILL_ENCHANTING) && player->GetSkillValue(SKILL_ENCHANTING) > 400)
					{
						player->ADD_GOSSIP_ITEM(1, "Зачарить Кольцо # 1", GOSSIP_SENDER_MAIN, 12);
						player->ADD_GOSSIP_ITEM(1, "Зачарить Кольцо # 2", GOSSIP_SENDER_MAIN, 1200);
					}
					player->ADD_GOSSIP_ITEM(1, "Я хотел бы снять мои чары", GOSSIP_SENDER_MAIN, 14);
					player->PlayerTalkClass->SendGossipMenu(100001, creature->GetGUID());
					return true;
				}
					break;

			case 400: //Remove enchant for mainhand
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 401: //Remove enchant for offhand
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 402: //Remove enchant for head
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HEAD));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 403: //Remove enchant for shoulders
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_SHOULDERS));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 404: //remove enchant for cloak
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_BACK));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 405: //remove enchant for chest
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_CHEST));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 406: //remove enchant for bracers
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_WRISTS));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 407: //remove enchant for gloves
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_HANDS));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 408: //remove enchant for legs
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_LEGS));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 409: //remove enchant for feet
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FEET));
				player->PlayerTalkClass->SendCloseGossip();
				break;

			case 410:
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER1));                    
				player->PlayerTalkClass->SendCloseGossip();
				break;
				
			case 411:
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_FINGER2));
				player->PlayerTalkClass->SendCloseGossip();
				break;
				
			case 412: // Снять чары со щита
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_OFFHAND));
				player->PlayerTalkClass->SendCloseGossip();
				break;
				
			case 413: // Снять чары с двуручного оружия
				RemoveEnchant(player, player->GetItemByPos(INVENTORY_SLOT_BAG_0, EQUIPMENT_SLOT_MAINHAND));
				player->PlayerTalkClass->SendCloseGossip();
				break;
			}
		return true;
	}
};

void AddSC_Unell_Custom_NPC()
{
    new npc_enchantment();
	new npc_welcome_new();
	new ralph();
	new npc_welcome();
	new Professions_NPC();
	new VIP_Professions_NPC();
	new Unell_Lottery();
	new Unell_NPC_Spell();
	new Gamble_npc();
	new Unell_Changeclass_NPC();
	new NPC_VisualWeapon();
	new Unell_Vendor_NPC_ILVL_Start();
	new Unell_Vendor_NPC_ILVL_1();
	new Unell_Vendor_NPC_ILVL_2();
	new Unell_Vendor_NPC_ILVL_3();
	new Unell_Vendor_NPC_ILVL_4();
	new Unell_Vendor_NPC_ILVL_5();
	new Unell_Prize_OBT();
	new Unell_boss();
	new npc_enchantment_private();
}