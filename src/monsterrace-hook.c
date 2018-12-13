﻿#include "angband.h"
#include "monsterrace-hook.h"

/*! 通常pit生成時のモンスターの構成条件ID / Race index for "monster pit (clone)" */
int vault_aux_race;

/*! 単一シンボルpit生成時の指定シンボル / Race index for "monster pit (symbol clone)" */
char vault_aux_char;

/*! ブレス属性に基づくドラゴンpit生成時条件マスク / Breath mask for "monster pit (dragon)" */
BIT_FLAGS vault_aux_dragon_mask4;


/*!
* @brief pit/nestの基準となる単種モンスターを決める /
* @return なし
*/
void vault_prep_clone(void)
{
	/* Apply the monster restriction */
	get_mon_num_prep(vault_aux_simple, NULL);

	/* Pick a race to clone */
	vault_aux_race = get_mon_num(dun_level + 10);

	/* Remove the monster restriction */
	get_mon_num_prep(NULL, NULL);
}


/*!
* @brief pit/nestの基準となるモンスターシンボルを決める /
* @return なし
*/
void vault_prep_symbol(void)
{
	MONRACE_IDX r_idx;

	/* Apply the monster restriction */
	get_mon_num_prep(vault_aux_simple, NULL);

	/* Pick a race to clone */
	r_idx = get_mon_num(dun_level + 10);

	/* Remove the monster restriction */
	get_mon_num_prep(NULL, NULL);

	/* Extract the symbol */
	vault_aux_char = r_info[r_idx].d_char;
}


/*!
* @brief pit/nestの基準となるドラゴンの種類を決める /
* @return なし
*/
void vault_prep_dragon(void)
{
	/* Pick dragon type */
	switch (randint0(6))
	{
	case 0: /* Black */
		vault_aux_dragon_mask4 = RF4_BR_ACID;
		break;
	case 1: /* Blue */
		vault_aux_dragon_mask4 = RF4_BR_ELEC;
		break;
	case 2: /* Red */
		vault_aux_dragon_mask4 = RF4_BR_FIRE;
		break;
	case 3: /* White */
		vault_aux_dragon_mask4 = RF4_BR_COLD;
		break;
	case 4: /* Green */
		vault_aux_dragon_mask4 = RF4_BR_POIS;
		break;
	default: /* Multi-hued */
		vault_aux_dragon_mask4 = (RF4_BR_ACID | RF4_BR_ELEC | RF4_BR_FIRE | RF4_BR_COLD | RF4_BR_POIS);
		break;
	}
}

/*!
* @brief モンスターがクエストの討伐対象に成り得るかを返す / Hook function for quest monsters
* @param r_idx モンスターＩＤ
* @return 討伐対象にできるならTRUEを返す。
*/
bool mon_hook_quest(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	/* Random quests are in the dungeon */
	if (r_ptr->flags8 & RF8_WILD_ONLY) return FALSE;

	/* No random quests for aquatic monsters */
	if (r_ptr->flags7 & RF7_AQUATIC) return FALSE;

	/* No random quests for multiplying monsters */
	if (r_ptr->flags2 & RF2_MULTIPLY) return FALSE;

	/* No quests to kill friendly monsters */
	if (r_ptr->flags7 & RF7_FRIENDLY) return FALSE;

	return TRUE;
}


/*!
* @brief モンスターがダンジョンに出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return ダンジョンに出現するならばTRUEを返す
*/
bool mon_hook_dungeon(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!(r_ptr->flags8 & RF8_WILD_ONLY))
		return TRUE;
	else
	{
		dungeon_info_type *d_ptr = &d_info[dungeon_type];
		if ((d_ptr->mflags8 & RF8_WILD_MOUNTAIN) &&
			(r_ptr->flags8 & RF8_WILD_MOUNTAIN)) return TRUE;
		return FALSE;
	}
}


/*!
* @brief モンスターが海洋に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 海洋に出現するならばTRUEを返す
*/
bool mon_hook_ocean(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_OCEAN)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが海岸に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 海岸に出現するならばTRUEを返す
*/
bool mon_hook_shore(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_SHORE)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが荒地に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 荒地に出現するならばTRUEを返す
*/
bool mon_hook_waste(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_WASTE | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが町に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 荒地に出現するならばTRUEを返す
*/
bool mon_hook_town(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_TOWN | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが森林に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 森林に出現するならばTRUEを返す
*/
bool mon_hook_wood(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_WOOD | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが火山に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 火山に出現するならばTRUEを返す
*/
bool mon_hook_volcano(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_VOLCANO)
		return TRUE;
	else
		return FALSE;
}

/*!
* @brief モンスターが山地に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 山地に出現するならばTRUEを返す
*/
bool mon_hook_mountain(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_MOUNTAIN)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが草原に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 森林に出現するならばTRUEを返す
*/
bool mon_hook_grass(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_GRASS | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}

/*!
* @brief モンスターが深い水地形に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 深い水地形に出現するならばTRUEを返す
*/
bool mon_hook_deep_water(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (r_ptr->flags7 & RF7_AQUATIC)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが浅い水地形に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 浅い水地形に出現するならばTRUEを返す
*/
bool mon_hook_shallow_water(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (r_ptr->flags2 & RF2_AURA_FIRE)
		return FALSE;
	else
		return TRUE;
}


/*!
* @brief モンスターが溶岩地形に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 溶岩地形に出現するならばTRUEを返す
*/
bool mon_hook_lava(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!mon_hook_dungeon(r_idx)) return FALSE;
	if (((r_ptr->flagsr & RFR_EFF_IM_FIRE_MASK) ||
		(r_ptr->flags7 & RF7_CAN_FLY)) &&
		!(r_ptr->flags3 & RF3_AURA_COLD))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief モンスターが通常の床地形に出現するかどうかを返す
* @param r_idx 判定するモンスターの種族ID
* @return 通常の床地形に出現するならばTRUEを返す
*/
bool mon_hook_floor(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!(r_ptr->flags7 & RF7_AQUATIC) || (r_ptr->flags7 & RF7_CAN_FLY))
		return TRUE;
	else
		return FALSE;
}

/*
* Helper function for "glass room"
*/
bool vault_aux_lite(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return FALSE;
	if (!(r_ptr->flags4 & RF4_BR_LITE) && !(r_ptr->a_ability_flags1 & RF5_BA_LITE)) return FALSE;
	if (r_ptr->flags2 & (RF2_PASS_WALL | RF2_KILL_WALL)) return FALSE;
	if (r_ptr->flags4 & RF4_BR_DISI) return FALSE;
	return TRUE;
}

/*
* Helper function for "glass room"
*/
bool vault_aux_shards(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return FALSE;
	if (!(r_ptr->flags4 & RF4_BR_SHAR)) return FALSE;
	return TRUE;
}


/*!
* @brief モンスターがVault生成の最低必要条件を満たしているかを返す /
* Helper monster selection function
* @param r_idx 確認したいモンスター種族ID
* @return Vault生成の最低必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_simple(MONRACE_IDX r_idx)
{
	return (vault_monster_okay(r_idx));
}


/*!
* @brief モンスターがゼリーnestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (jelly)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_jelly(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if ((r_ptr->flags2 & RF2_KILL_BODY) && !(r_ptr->flags1 & RF1_NEVER_BLOW)) return (FALSE);
	if (r_ptr->flags3 & (RF3_EVIL)) return (FALSE);
	if (!my_strchr("ijm,", r_ptr->d_char)) return (FALSE);
	return (TRUE);
}

/*!
* @brief モンスターが動物nestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (animal)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_animal(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!(r_ptr->flags3 & (RF3_ANIMAL))) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターがアンデッドnestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (undead)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_undead(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!(r_ptr->flags3 & (RF3_UNDEAD))) return (FALSE);
	return (TRUE);
}

/*!
* @brief モンスターが聖堂nestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (chapel)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_chapel_g(MONRACE_IDX r_idx)
{
	static int chapel_list[] = {
		MON_NOV_PRIEST, MON_NOV_PALADIN, MON_NOV_PRIEST_G, MON_NOV_PALADIN_G,
		MON_PRIEST, MON_JADE_MONK, MON_IVORY_MONK, MON_ULTRA_PALADIN,
		MON_EBONY_MONK, MON_W_KNIGHT, MON_KNI_TEMPLAR, MON_PALADIN,
		MON_TOPAZ_MONK, 0 };

	int i;
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (r_ptr->flags3 & (RF3_EVIL)) return (FALSE);
	if ((r_idx == MON_A_GOLD) || (r_idx == MON_A_SILVER)) return (FALSE);
	if (r_ptr->d_char == 'A') return TRUE;
	for (i = 0; chapel_list[i]; i++)
		if (r_idx == chapel_list[i]) return TRUE;
	return FALSE;
}

/*!
* @brief モンスターが犬小屋nestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (kennel)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_kennel(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!my_strchr("CZ", r_ptr->d_char)) return (FALSE);
	return (TRUE);
}

/*!
* @brief モンスターがミミックnestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (mimic)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_mimic(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!my_strchr("!$&(/=?[\\|", r_ptr->d_char)) return (FALSE);
	return (TRUE);
}

/*!
* @brief モンスターが単一クローンnestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (clone)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_clone(MONRACE_IDX r_idx)
{
	if (!vault_monster_okay(r_idx)) return (FALSE);
	return (r_idx == vault_aux_race);
}


/*!
* @brief モンスターが邪悪属性シンボルクローンnestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (symbol clone)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_symbol_e(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if ((r_ptr->flags2 & RF2_KILL_BODY) && !(r_ptr->flags1 & RF1_NEVER_BLOW)) return (FALSE);
	if (r_ptr->flags3 & (RF3_GOOD)) return (FALSE);
	if (r_ptr->d_char != vault_aux_char) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターが善良属性シンボルクローンnestの生成必要条件を満たしているかを返す /
* Helper function for "monster nest (symbol clone)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_symbol_g(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if ((r_ptr->flags2 & RF2_KILL_BODY) && !(r_ptr->flags1 & RF1_NEVER_BLOW)) return (FALSE);
	if (r_ptr->flags3 & (RF3_EVIL)) return (FALSE);
	if (r_ptr->d_char != vault_aux_char) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターがオークpitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (orc)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_orc(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!(r_ptr->flags3 & RF3_ORC)) return (FALSE);
	if (r_ptr->flags3 & RF3_UNDEAD) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターがトロルpitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (troll)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_troll(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!(r_ptr->flags3 & RF3_TROLL)) return (FALSE);
	if (r_ptr->flags3 & RF3_UNDEAD) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターが巨人pitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (giant)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_giant(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!(r_ptr->flags3 & RF3_GIANT)) return (FALSE);
	if (r_ptr->flags3 & RF3_GOOD) return (FALSE);
	if (r_ptr->flags3 & RF3_UNDEAD) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターがドラゴンpitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (dragon)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_dragon(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if (!(r_ptr->flags3 & RF3_DRAGON)) return (FALSE);
	if (r_ptr->flags4 != vault_aux_dragon_mask4) return (FALSE);
	if (r_ptr->flags3 & RF3_UNDEAD) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターが悪魔pitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (demon)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_demon(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if ((r_ptr->flags2 & RF2_KILL_BODY) && !(r_ptr->flags1 & RF1_NEVER_BLOW)) return (FALSE);
	if (!(r_ptr->flags3 & RF3_DEMON)) return (FALSE);
	return (TRUE);
}


/*!
* @brief モンスターが狂気pitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (lovecraftian)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_cthulhu(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];
	if (!vault_monster_okay(r_idx)) return (FALSE);
	if ((r_ptr->flags2 & RF2_KILL_BODY) && !(r_ptr->flags1 & RF1_NEVER_BLOW)) return (FALSE);
	if (!(r_ptr->flags2 & (RF2_ELDRITCH_HORROR))) return (FALSE);
	return (TRUE);
}

/*!
* @brief モンスターがダークエルフpitの生成必要条件を満たしているかを返す /
* Helper function for "monster pit (dark elf)"
* @param r_idx 確認したいモンスター種族ID
* @return 生成必要条件を満たしているならTRUEを返す。
*/
bool vault_aux_dark_elf(MONRACE_IDX r_idx)
{
	int i;
	static int dark_elf_list[] =
	{
		MON_D_ELF, MON_D_ELF_MAGE, MON_D_ELF_WARRIOR, MON_D_ELF_PRIEST,
		MON_D_ELF_LORD, MON_D_ELF_WARLOCK, MON_D_ELF_DRUID, MON_NIGHTBLADE,
		MON_D_ELF_SORC, MON_D_ELF_SHADE, 0,
	};
	if (!vault_monster_okay(r_idx)) return FALSE;
	for (i = 0; dark_elf_list[i]; i++) if (r_idx == dark_elf_list[i]) return TRUE;
	return FALSE;
}



/*!
 * @brief モンスターが生命体かどうかを返す
 * Is the monster "alive"?
 * @param r_ptr 判定するモンスターの種族情報構造体参照ポインタ
 * @return 生命体ならばTRUEを返す
 * @details
 * Used to determine the message to print for a killed monster.
 * ("dies", "destroyed")
 */
bool monster_living(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	/* Non-living, undead, or demon */
	if (r_ptr->flags3 & (RF3_DEMON | RF3_UNDEAD | RF3_NONLIVING))
		return FALSE;
	else
		return TRUE;
}

/*!
 * @brief モンスターが特殊能力上、賞金首から排除する必要があるかどうかを返す。
 * Is the monster "alive"? / Is this monster declined to be questor or bounty?
 * @param r_idx モンスターの種族ID
 * @return 賞金首に加えられないならばTRUEを返す
 * @details
 * 実質バーノール＝ルパート用。
 */
bool no_questor_or_bounty_uniques(MONRACE_IDX r_idx)
{
	switch (r_idx)
	{
		/*
		 * Decline them to be questor or bounty because they use
		 * special motion "split and combine"
		 */
	case MON_BANORLUPART:
	case MON_BANOR:
	case MON_LUPART:
		return TRUE;
	default:
		return FALSE;
	}
}
