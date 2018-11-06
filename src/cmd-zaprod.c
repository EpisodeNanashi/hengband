﻿#include "angband.h"

/*!
 * @brief ロッドの効果を発動する
 * @param sval オブジェクトのsval
 * @param dir 発動目標の方向ID
 * @param use_charge チャージを消費したかどうかを返す参照ポインタ
 * @param powerful 強力発動上の処理ならばTRUE
 * @param magic 魔道具術上の処理ならばTRUE
 * @return 発動により効果内容が確定したならばTRUEを返す
 */
int rod_effect(OBJECT_SUBTYPE_VALUE sval, DIRECTION dir, bool *use_charge, bool powerful, bool magic)
{
	int ident = FALSE;
	int lev = powerful ? p_ptr->lev * 2 : p_ptr->lev;
	int detect_rad = powerful ? DETECT_RAD_DEFAULT * 3 / 2 : DETECT_RAD_DEFAULT;
	POSITION rad = powerful ? 3 : 2;

	/* Unused */
	(void)magic;

	/* Analyze the rod */
	switch (sval)
	{
	case SV_ROD_DETECT_TRAP:
	{
		if (detect_traps(detect_rad, (bool)(dir ? FALSE : TRUE))) ident = TRUE;
		break;
	}

	case SV_ROD_DETECT_DOOR:
	{
		if (detect_doors(detect_rad)) ident = TRUE;
		if (detect_stairs(detect_rad)) ident = TRUE;
		break;
	}

	case SV_ROD_IDENTIFY:
	{
		if (powerful) {
			if (!identify_fully(FALSE)) *use_charge = FALSE;
		}
		else {
			if (!ident_spell(FALSE)) *use_charge = FALSE;
		}
		ident = TRUE;
		break;
	}

	case SV_ROD_RECALL:
	{
		if (!word_of_recall()) *use_charge = FALSE;
		ident = TRUE;
		break;
	}

	case SV_ROD_ILLUMINATION:
	{
		if (lite_area(damroll(2, 8), (powerful ? 4 : 2))) ident = TRUE;
		break;
	}

	case SV_ROD_MAPPING:
	{
		map_area(powerful ? DETECT_RAD_MAP * 3 / 2 : DETECT_RAD_MAP);
		ident = TRUE;
		break;
	}

	case SV_ROD_DETECTION:
	{
		detect_all(detect_rad);
		ident = TRUE;
		break;
	}

	case SV_ROD_PROBING:
	{
		probing();
		ident = TRUE;
		break;
	}

	case SV_ROD_CURING:
	{
		if (true_healing(0)) ident = TRUE;
		if (set_shero(0, TRUE)) ident = TRUE;
		break;
	}

	case SV_ROD_HEALING:
	{
		if(cure_critical_wounds(powerful ? 750 : 500)) ident = TRUE;
		break;
	}

	case SV_ROD_RESTORATION:
	{
		if(restore_level()) ident = TRUE;
		if(restore_all_status()) ident = TRUE;
		break;
	}

	case SV_ROD_SPEED:
	{
		if (set_fast(randint1(30) + (powerful ? 30 : 15), FALSE)) ident = TRUE;
		break;
	}

	case SV_ROD_PESTICIDE:
	{
		if (dispel_monsters(powerful ? 8 : 4)) ident = TRUE;
		break;
	}

	case SV_ROD_TELEPORT_AWAY:
	{
		int distance = MAX_SIGHT * (powerful ? 8 : 5);
		if (teleport_monster(dir, distance)) ident = TRUE;
		break;
	}

	case SV_ROD_DISARMING:
	{
		if (disarm_trap(dir)) ident = TRUE;
		if (powerful && disarm_traps_touch()) ident = TRUE;
		break;
	}

	case SV_ROD_LITE:
	{
		HIT_POINT dam = damroll((powerful ? 12 : 6), 8);
		msg_print(_("青く輝く光線が放たれた。", "A line of blue shimmering light appears."));
		(void)lite_line(dir, dam);
		ident = TRUE;
		break;
	}

	case SV_ROD_SLEEP_MONSTER:
	{
		if (sleep_monster(dir, lev)) ident = TRUE;
		break;
	}

	case SV_ROD_SLOW_MONSTER:
	{
		if (slow_monster(dir, lev)) ident = TRUE;
		break;
	}

	case SV_ROD_HYPODYNAMIA:
	{
		if (hypodynamic_bolt(dir, 70 + 3 * lev / 2)) ident = TRUE;
		break;
	}

	case SV_ROD_POLYMORPH:
	{
		if (poly_monster(dir, lev)) ident = TRUE;
		break;
	}

	case SV_ROD_ACID_BOLT:
	{
		fire_bolt_or_beam(10, GF_ACID, dir, damroll(6 + lev / 7, 8));
		ident = TRUE;
		break;
	}

	case SV_ROD_ELEC_BOLT:
	{
		fire_bolt_or_beam(10, GF_ELEC, dir, damroll(4 + lev / 9, 8));
		ident = TRUE;
		break;
	}

	case SV_ROD_FIRE_BOLT:
	{
		fire_bolt_or_beam(10, GF_FIRE, dir, damroll(7 + lev / 6, 8));
		ident = TRUE;
		break;
	}

	case SV_ROD_COLD_BOLT:
	{
		fire_bolt_or_beam(10, GF_COLD, dir, damroll(5 + lev / 8, 8));
		ident = TRUE;
		break;
	}

	case SV_ROD_ACID_BALL:
	{
		fire_ball(GF_ACID, dir, 60 + lev, rad);
		ident = TRUE;
		break;
	}

	case SV_ROD_ELEC_BALL:
	{
		fire_ball(GF_ELEC, dir, 40 + lev, rad);
		ident = TRUE;
		break;
	}

	case SV_ROD_FIRE_BALL:
	{
		fire_ball(GF_FIRE, dir, 70 + lev, rad);
		ident = TRUE;
		break;
	}

	case SV_ROD_COLD_BALL:
	{
		fire_ball(GF_COLD, dir, 50 + lev, rad);
		ident = TRUE;
		break;
	}

	case SV_ROD_HAVOC:
	{
		call_chaos();
		ident = TRUE;
		break;
	}

	case SV_ROD_STONE_TO_MUD:
	{
		HIT_POINT dam = powerful ? 40 + randint1(60) : 20 + randint1(30);
		if (wall_to_mud(dir, dam)) ident = TRUE;
		break;
	}

	case SV_ROD_AGGRAVATE:
	{
		aggravate_monsters(0);
		ident = TRUE;
		break;
	}
	}
	return ident;
}

/*!
* @brief ロッドを使うコマンドのサブルーチン /
* Activate (zap) a Rod
* @param item 使うオブジェクトの所持品ID
* @return なし
* @details
* <pre>
* Unstack fully charged rods as needed.
* Hack -- rods of perception/genocide can be "cancelled"
* All rods can be cancelled at the "Direction?" prompt
* pvals are defined for each rod in k_info. -LM-
* </pre>
*/
void do_cmd_zap_rod_aux(INVENTORY_IDX item)
{
	int ident, chance, lev, fail;
	DIRECTION dir = 0;
	object_type *o_ptr;
	bool success;

	/* Hack -- let perception get aborted */
	bool use_charge = TRUE;

	object_kind *k_ptr;

	/* Get the item (in the pack) */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Get the item (on the floor) */
	else
	{
		o_ptr = &o_list[0 - item];
	}


	/* Mega-Hack -- refuse to zap a pile from the ground */
	if ((item < 0) && (o_ptr->number > 1))
	{
		msg_print(_("まずはロッドを拾わなければ。", "You must first pick up the rods."));
		return;
	}


	/* Get a direction (unless KNOWN not to need it) */
	if (((o_ptr->sval >= SV_ROD_MIN_DIRECTION) && (o_ptr->sval != SV_ROD_HAVOC) && (o_ptr->sval != SV_ROD_AGGRAVATE) && (o_ptr->sval != SV_ROD_PESTICIDE)) ||
		!object_is_aware(o_ptr))
	{
		/* Get a direction, allow cancel */
		if (!get_aim_dir(&dir)) return;
	}


	/* Take a turn */
	p_ptr->energy_use = 100;

	/* Extract the item level */
	lev = k_info[o_ptr->k_idx].level;

	/* Base chance of success */
	chance = p_ptr->skill_dev;

	/* Confusion hurts skill */
	if (p_ptr->confused) chance = chance / 2;

	fail = lev + 5;
	if (chance > fail) fail -= (chance - fail) * 2;
	else chance -= (fail - chance) * 2;
	if (fail < USE_DEVICE) fail = USE_DEVICE;
	if (chance < USE_DEVICE) chance = USE_DEVICE;

	if (world_player)
	{
		if (flush_failure) flush();
		msg_print(_("止まった時の中ではうまく働かないようだ。", "Nothing happen. Maybe this rod is freezing too."));
		sound(SOUND_FAIL);
		return;
	}

	if (p_ptr->pclass == CLASS_BERSERKER) success = FALSE;
	else if (chance > fail)
	{
		if (randint0(chance * 2) < fail) success = FALSE;
		else success = TRUE;
	}
	else
	{
		if (randint0(fail * 2) < chance) success = TRUE;
		else success = FALSE;
	}

	/* Roll for usage */
	if (!success)
	{
		if (flush_failure) flush();
		msg_print(_("うまくロッドを使えなかった。", "You failed to use the rod properly."));
		sound(SOUND_FAIL);
		return;
	}

	k_ptr = &k_info[o_ptr->k_idx];

	/* A single rod is still charging */
	if ((o_ptr->number == 1) && (o_ptr->timeout))
	{
		if (flush_failure) flush();
		msg_print(_("このロッドはまだ魔力を充填している最中だ。", "The rod is still charging."));
		return;
	}
	/* A stack of rods lacks enough energy. */
	else if ((o_ptr->number > 1) && (o_ptr->timeout > k_ptr->pval * (o_ptr->number - 1)))
	{
		if (flush_failure) flush();
		msg_print(_("そのロッドはまだ充填中です。", "The rods are all still charging."));
		return;
	}

	sound(SOUND_ZAP);

	ident = rod_effect(o_ptr->sval, dir, &use_charge, FALSE, FALSE);

	/* Increase the timeout by the rod kind's pval. -LM- */
	if (use_charge) o_ptr->timeout += k_ptr->pval;

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	if (!(object_is_aware(o_ptr)))
	{
		chg_virtue(V_PATIENCE, -1);
		chg_virtue(V_CHANCE, 1);
		chg_virtue(V_KNOWLEDGE, -1);
	}

	/* Tried the object */
	object_tried(o_ptr);

	/* Successfully determined the object function */
	if (ident && !object_is_aware(o_ptr))
	{
		object_aware(o_ptr);
		gain_exp((lev + (p_ptr->lev >> 1)) / p_ptr->lev);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);
}

/*!
* @brief ロッドを使うコマンドのメインルーチン /
* @return なし
*/
void do_cmd_zap_rod(void)
{
	OBJECT_IDX item;
	cptr q, s;

	if (p_ptr->special_defense & (KATA_MUSOU | KATA_KOUKIJIN))
	{
		set_action(ACTION_NONE);
	}

	/* Restrict choices to rods */
	item_tester_tval = TV_ROD;

	/* Get an item */
	q = _("どのロッドを振りますか? ", "Zap which rod? ");
	s = _("使えるロッドがない。", "You have no rod to zap.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return;

	/* Zap the rod */
	do_cmd_zap_rod_aux(item);
}
