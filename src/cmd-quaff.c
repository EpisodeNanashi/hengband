/*!
 * @file cmd-quaff.c
 * @brief �v���C���[�̈��ރR�}���h����
 * @date 2018/09/07
 * @details
 * cmd6.c��蕪���B
 */


#include "angband.h"
#include "selfinfo.h"

/*!
 * @brief ������ރR�}���h�̃T�u���[�`�� /
 * Quaff a potion (from the pack or the floor)
 * @param item ���ޖ�I�u�W�F�N�g�̏����iID
 * @return �Ȃ�
 */
void do_cmd_quaff_potion_aux(int item)
{
	int         ident, lev;
	object_type *o_ptr;
	object_type forge;
	object_type *q_ptr;


	/* Take a turn */
	p_ptr->energy_use = 100;

	if (world_player)
	{
		if (flush_failure) flush();
		msg_print(_("�r���琅������o�Ă��Ȃ��I", "The potion doesn't flow out from a bottle."));

		sound(SOUND_FAIL);
		return;
	}

	if (music_singing_any()) stop_singing();
	if (hex_spelling_any())
	{
		if (!hex_spelling(HEX_INHAIL)) stop_hex_spell_all();
	}

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

	/* Get local object */
	q_ptr = &forge;

	/* Obtain a local object */
	object_copy(q_ptr, o_ptr);

	/* Single object */
	q_ptr->number = 1;

	/* Reduce and describe inventory */
	if (item >= 0)
	{
		inven_item_increase(item, -1);
		inven_item_describe(item);
		inven_item_optimize(item);
	}

	/* Reduce and describe floor item */
	else
	{
		floor_item_increase(0 - item, -1);
		floor_item_describe(0 - item);
		floor_item_optimize(0 - item);
	}

	/* Sound */
	sound(SOUND_QUAFF);


	/* Not identified yet */
	ident = FALSE;

	/* Object level */
	lev = k_info[q_ptr->k_idx].level;

	/* Analyze the potion */
	if (q_ptr->tval == TV_POTION)
	{
		switch (q_ptr->sval)
		{
			/* ���݂��������I���W�i�����ׂ����\�� */
		case SV_POTION_WATER:
			msg_print(_("���̒��������ς肵���B", ""));
			msg_print(_("�̂ǂ̊��������������܂����B", "You feel less thirsty."));
			ident = TRUE;
			break;

		case SV_POTION_APPLE_JUICE:
			msg_print(_("�Â��ăT�b�p���Ƃ��Ă��āA�ƂĂ����������B", ""));
			msg_print(_("�̂ǂ̊��������������܂����B", "You feel less thirsty."));
			ident = TRUE;
			break;

		case SV_POTION_SLIME_MOLD:
			msg_print(_("�Ȃ�Ƃ��s�C���Ȗ����B", ""));
			msg_print(_("�̂ǂ̊��������������܂����B", "You feel less thirsty."));
			ident = TRUE;
			break;

		case SV_POTION_SLOWNESS:
			if (set_slow(randint1(25) + 15, FALSE)) ident = TRUE;
			break;

		case SV_POTION_SALT_WATER:
			msg_print(_("�����I�v�킸�f���Ă��܂����B", "The potion makes you vomit!"));

			if (!(prace_is_(RACE_GOLEM) ||
			      prace_is_(RACE_ZOMBIE) ||
			      prace_is_(RACE_DEMON) ||
			      prace_is_(RACE_ANDROID) ||
			      prace_is_(RACE_SPECTRE) ||
			      (mimic_info[p_ptr->mimic_form].MIMIC_FLAGS & MIMIC_IS_NONLIVING)))
			{
				/* Only living creatures get thirsty */
				(void)set_food(PY_FOOD_STARVE - 1);
			}

			(void)set_poisoned(0);
			(void)set_paralyzed(p_ptr->paralyzed + 4);
			ident = TRUE;
			break;

		case SV_POTION_POISON:
			if (!(p_ptr->resist_pois || IS_OPPOSE_POIS()))
			{
				if (set_poisoned(p_ptr->poisoned + randint0(15) + 10))
				{
					ident = TRUE;
				}
			}
			break;

		case SV_POTION_BLINDNESS:
			if (!p_ptr->resist_blind)
			{
				if (set_blind(p_ptr->blind + randint0(100) + 100))
				{
					ident = TRUE;
				}
			}
			break;

		case SV_POTION_CONFUSION: /* Booze */
			if (p_ptr->pclass != CLASS_MONK) chg_virtue(V_HARMONY, -1);
			else if (!p_ptr->resist_conf) p_ptr->special_attack |= ATTACK_SUIKEN;
			if (!p_ptr->resist_conf)
			{
				if (set_confused(randint0(20) + 15))
				{
					ident = TRUE;
				}
			}

			if (!p_ptr->resist_chaos)
			{
				if (one_in_(2))
				{
					if (set_image(p_ptr->image + randint0(150) + 150))
					{
						ident = TRUE;
					}
				}
				if (one_in_(13) && (p_ptr->pclass != CLASS_MONK))
				{
					ident = TRUE;
					if (one_in_(3)) lose_all_info();
					else wiz_dark();
					(void)teleport_player_aux(100, TELEPORT_NONMAGICAL | TELEPORT_PASSIVE);
					wiz_dark();
					msg_print(_("�m��Ȃ��ꏊ�Ŗڂ����߂��B���ɂ�����B", "You wake up somewhere with a sore head..."));
					msg_print(_("�����v���o���Ȃ��B�ǂ�����Ă����֗����̂���������Ȃ��I", "You can't remember a thing, or how you got here!"));
				}
			}
			break;

		case SV_POTION_SLEEP:
			if (!p_ptr->free_act)
			{
				msg_print(_("���Ȃ��͖����Ă��܂����B", "You fall asleep."));

				if (ironman_nightmare)
				{
					msg_print(_("���낵�����i�����ɕ�����ł����B", "A horrible vision enters your mind."));

					/* Have some nightmares */
					sanity_blast(NULL, FALSE);
				}
				if (set_paralyzed(p_ptr->paralyzed + randint0(4) + 4))
				{
					ident = TRUE;
				}
			}
			break;

		case SV_POTION_LOSE_MEMORIES:
			if (!p_ptr->hold_exp && (p_ptr->exp > 0))
			{
				msg_print(_("�ߋ��̋L��������Ă����C������B", "You feel your memories fade."));
				chg_virtue(V_KNOWLEDGE, -5);

				lose_exp(p_ptr->exp / 4);
				ident = TRUE;
			}
			break;

		case SV_POTION_RUINATION:
			msg_print(_("�g���S������Ă��āA���C�������Ă����悤���B", "Your nerves and muscles feel weak and lifeless!"));
			take_hit(DAMAGE_LOSELIFE, damroll(10, 10), _("�j�ł̖�", "a potion of Ruination"), -1);

			(void)dec_stat(A_DEX, 25, TRUE);
			(void)dec_stat(A_WIS, 25, TRUE);
			(void)dec_stat(A_CON, 25, TRUE);
			(void)dec_stat(A_STR, 25, TRUE);
			(void)dec_stat(A_CHR, 25, TRUE);
			(void)dec_stat(A_INT, 25, TRUE);
			ident = TRUE;
			break;

		case SV_POTION_DEC_STR:
			if (do_dec_stat(A_STR)) ident = TRUE;
			break;

		case SV_POTION_DEC_INT:
			if (do_dec_stat(A_INT)) ident = TRUE;
			break;

		case SV_POTION_DEC_WIS:
			if (do_dec_stat(A_WIS)) ident = TRUE;
			break;

		case SV_POTION_DEC_DEX:
			if (do_dec_stat(A_DEX)) ident = TRUE;
			break;

		case SV_POTION_DEC_CON:
			if (do_dec_stat(A_CON)) ident = TRUE;
			break;

		case SV_POTION_DEC_CHR:
			if (do_dec_stat(A_CHR)) ident = TRUE;
			break;

		case SV_POTION_DETONATIONS:
			msg_print(_("�̂̒��Ō������������N�����I", "Massive explosions rupture your body!"));
			take_hit(DAMAGE_NOESCAPE, damroll(50, 20), _("�����̖�", "a potion of Detonation"), -1);

			(void)set_stun(p_ptr->stun + 75);
			(void)set_cut(p_ptr->cut + 5000);
			ident = TRUE;
			break;

		case SV_POTION_DEATH:
			chg_virtue(V_VITALITY, -1);
			chg_virtue(V_UNLIFE, 5);
			msg_print(_("���̗\�����̒����삯�߂������B", "A feeling of Death flows through your body."));
			take_hit(DAMAGE_LOSELIFE, 5000, _("���̖�", "a potion of Death"), -1);
			ident = TRUE;
			break;

		case SV_POTION_INFRAVISION:
			if (set_tim_infra(p_ptr->tim_infra + 100 + randint1(100), FALSE))
			{
				ident = TRUE;
			}
			break;

		case SV_POTION_DETECT_INVIS:
			if (set_tim_invis(p_ptr->tim_invis + 12 + randint1(12), FALSE))
			{
				ident = TRUE;
			}
			break;

		case SV_POTION_SLOW_POISON:
			if (set_poisoned(p_ptr->poisoned / 2)) ident = TRUE;
			break;

		case SV_POTION_CURE_POISON:
			if (set_poisoned(0)) ident = TRUE;
			break;

		case SV_POTION_BOLDNESS:
			if (set_afraid(0)) ident = TRUE;
			break;

		case SV_POTION_SPEED:
			if (!p_ptr->fast)
			{
				if (set_fast(randint1(25) + 15, FALSE)) ident = TRUE;
			}
			else
			{
				(void)set_fast(p_ptr->fast + 5, FALSE);
			}
			break;

		case SV_POTION_RESIST_HEAT:
			if (set_oppose_fire(p_ptr->oppose_fire + randint1(10) + 10, FALSE))
			{
				ident = TRUE;
			}
			break;

		case SV_POTION_RESIST_COLD:
			if (set_oppose_cold(p_ptr->oppose_cold + randint1(10) + 10, FALSE))
			{
				ident = TRUE;
			}
			break;

		case SV_POTION_HEROISM:
			if (set_afraid(0)) ident = TRUE;
			if (set_hero(p_ptr->hero + randint1(25) + 25, FALSE)) ident = TRUE;
			if (hp_player(10)) ident = TRUE;
			break;

		case SV_POTION_BESERK_STRENGTH:
			if (set_afraid(0)) ident = TRUE;
			if (set_shero(p_ptr->shero + randint1(25) + 25, FALSE)) ident = TRUE;
			if (hp_player(30)) ident = TRUE;
			break;

		case SV_POTION_CURE_LIGHT:
			if (hp_player(damroll(2, 8))) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_cut(p_ptr->cut - 10)) ident = TRUE;
			if (set_shero(0,TRUE)) ident = TRUE;
			break;

		case SV_POTION_CURE_SERIOUS:
			if (hp_player(damroll(4, 8))) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_cut((p_ptr->cut / 2) - 50)) ident = TRUE;
			if (set_shero(0,TRUE)) ident = TRUE;
			break;

		case SV_POTION_CURE_CRITICAL:
			if (hp_player(damroll(6, 8))) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			if (set_shero(0,TRUE)) ident = TRUE;
			break;

		case SV_POTION_HEALING:
			if (hp_player(300)) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			if (set_shero(0,TRUE)) ident = TRUE;
			break;

		case SV_POTION_STAR_HEALING:
			if (hp_player(1200)) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			if (set_shero(0,TRUE)) ident = TRUE;
			break;

		case SV_POTION_LIFE:
			chg_virtue(V_VITALITY, 1);
			chg_virtue(V_UNLIFE, -5);
			msg_print(_("�̒��ɐ����͂��������ӂ�Ă����I", "You feel life flow through your body!"));
			restore_level();
			(void)set_poisoned(0);
			(void)set_blind(0);
			(void)set_confused(0);
			(void)set_image(0);
			(void)set_stun(0);
			(void)set_cut(0);
			(void)do_res_stat(A_STR);
			(void)do_res_stat(A_CON);
			(void)do_res_stat(A_DEX);
			(void)do_res_stat(A_WIS);
			(void)do_res_stat(A_INT);
			(void)do_res_stat(A_CHR);
			(void)set_shero(0,TRUE);
			update_stuff();
			hp_player(5000);
			ident = TRUE;
			break;

		case SV_POTION_RESTORE_MANA:
			if (p_ptr->pclass == CLASS_MAGIC_EATER)
			{
				int i;
				for (i = 0; i < EATER_EXT*2; i++)
				{
					p_ptr->magic_num1[i] += (p_ptr->magic_num2[i] < 10) ? EATER_CHARGE * 3 : p_ptr->magic_num2[i]*EATER_CHARGE/3;
					if (p_ptr->magic_num1[i] > p_ptr->magic_num2[i]*EATER_CHARGE) p_ptr->magic_num1[i] = p_ptr->magic_num2[i]*EATER_CHARGE;
				}
				for (; i < EATER_EXT*3; i++)
				{
					KIND_OBJECT_IDX k_idx = lookup_kind(TV_ROD, i-EATER_EXT*2);
					p_ptr->magic_num1[i] -= ((p_ptr->magic_num2[i] < 10) ? EATER_ROD_CHARGE*3 : p_ptr->magic_num2[i]*EATER_ROD_CHARGE/3)*k_info[k_idx].pval;
					if (p_ptr->magic_num1[i] < 0) p_ptr->magic_num1[i] = 0;
				}
				msg_print(_("�����n�b�L���Ƃ����B", "You feel your head clear."));
				p_ptr->window |= (PW_PLAYER);
				ident = TRUE;
			}
			else if (p_ptr->csp < p_ptr->msp)
			{
				p_ptr->csp = p_ptr->msp;
				p_ptr->csp_frac = 0;
				msg_print(_("�����n�b�L���Ƃ����B", "You feel your head clear."));

				p_ptr->redraw |= (PR_MANA);
				p_ptr->window |= (PW_PLAYER);
				p_ptr->window |= (PW_SPELL);
				ident = TRUE;
			}
			if (set_shero(0,TRUE)) ident = TRUE;
			break;

		case SV_POTION_RESTORE_EXP:
			if (restore_level()) ident = TRUE;
			break;

		case SV_POTION_RES_STR:
			if (do_res_stat(A_STR)) ident = TRUE;
			break;

		case SV_POTION_RES_INT:
			if (do_res_stat(A_INT)) ident = TRUE;
			break;

		case SV_POTION_RES_WIS:
			if (do_res_stat(A_WIS)) ident = TRUE;
			break;

		case SV_POTION_RES_DEX:
			if (do_res_stat(A_DEX)) ident = TRUE;
			break;

		case SV_POTION_RES_CON:
			if (do_res_stat(A_CON)) ident = TRUE;
			break;

		case SV_POTION_RES_CHR:
			if (do_res_stat(A_CHR)) ident = TRUE;
			break;

		case SV_POTION_INC_STR:
			if (do_inc_stat(A_STR)) ident = TRUE;
			break;

		case SV_POTION_INC_INT:
			if (do_inc_stat(A_INT)) ident = TRUE;
			break;

		case SV_POTION_INC_WIS:
			if (do_inc_stat(A_WIS)) ident = TRUE;
			break;

		case SV_POTION_INC_DEX:
			if (do_inc_stat(A_DEX)) ident = TRUE;
			break;

		case SV_POTION_INC_CON:
			if (do_inc_stat(A_CON)) ident = TRUE;
			break;

		case SV_POTION_INC_CHR:
			if (do_inc_stat(A_CHR)) ident = TRUE;
			break;

		case SV_POTION_AUGMENTATION:
			if (do_inc_stat(A_STR)) ident = TRUE;
			if (do_inc_stat(A_INT)) ident = TRUE;
			if (do_inc_stat(A_WIS)) ident = TRUE;
			if (do_inc_stat(A_DEX)) ident = TRUE;
			if (do_inc_stat(A_CON)) ident = TRUE;
			if (do_inc_stat(A_CHR)) ident = TRUE;
			break;

		case SV_POTION_ENLIGHTENMENT:
			msg_print(_("�����̒u����Ă���󋵂��]���ɕ�����ł���...", "An image of your surroundings forms in your mind..."));
			chg_virtue(V_KNOWLEDGE, 1);
			chg_virtue(V_ENLIGHTEN, 1);
			wiz_lite(FALSE);
			ident = TRUE;
			break;

		case SV_POTION_STAR_ENLIGHTENMENT:
			msg_print(_("�X�Ȃ�[�ւ�������...", "You begin to feel more enlightened..."));
			chg_virtue(V_KNOWLEDGE, 1);
			chg_virtue(V_ENLIGHTEN, 2);
			msg_print(NULL);
			wiz_lite(FALSE);
			(void)do_inc_stat(A_INT);
			(void)do_inc_stat(A_WIS);
			(void)detect_traps(DETECT_RAD_DEFAULT, TRUE);
			(void)detect_doors(DETECT_RAD_DEFAULT);
			(void)detect_stairs(DETECT_RAD_DEFAULT);
			(void)detect_treasure(DETECT_RAD_DEFAULT);
			(void)detect_objects_gold(DETECT_RAD_DEFAULT);
			(void)detect_objects_normal(DETECT_RAD_DEFAULT);
			identify_pack();
			self_knowledge();
			ident = TRUE;
			break;

		case SV_POTION_SELF_KNOWLEDGE:
			msg_print(_("�������g�̂��Ƃ������͕��������C������...", "You begin to know yourself a little better..."));
			msg_print(NULL);
			self_knowledge();
			ident = TRUE;
			break;

		case SV_POTION_EXPERIENCE:
			if (p_ptr->prace == RACE_ANDROID) break;
			chg_virtue(V_ENLIGHTEN, 1);
			if (p_ptr->exp < PY_MAX_EXP)
			{
				s32b ee = (p_ptr->exp / 2) + 10;
				if (ee > 100000L) ee = 100000L;
				msg_print(_("�X�Ɍo����ς񂾂悤�ȋC������B", "You feel more experienced."));
				gain_exp(ee);
				ident = TRUE;
			}
			break;

		case SV_POTION_RESISTANCE:
			(void)set_oppose_acid(p_ptr->oppose_acid + randint1(20) + 20, FALSE);
			(void)set_oppose_elec(p_ptr->oppose_elec + randint1(20) + 20, FALSE);
			(void)set_oppose_fire(p_ptr->oppose_fire + randint1(20) + 20, FALSE);
			(void)set_oppose_cold(p_ptr->oppose_cold + randint1(20) + 20, FALSE);
			(void)set_oppose_pois(p_ptr->oppose_pois + randint1(20) + 20, FALSE);
			ident = TRUE;
			break;

		case SV_POTION_CURING:
			if (hp_player(50)) ident = TRUE;
			if (set_blind(0)) ident = TRUE;
			if (set_poisoned(0)) ident = TRUE;
			if (set_confused(0)) ident = TRUE;
			if (set_stun(0)) ident = TRUE;
			if (set_cut(0)) ident = TRUE;
			if (set_image(0)) ident = TRUE;
			break;

		case SV_POTION_INVULNERABILITY:
			(void)set_invuln(p_ptr->invuln + randint1(4) + 4, FALSE);
			ident = TRUE;
			break;

		case SV_POTION_NEW_LIFE:
			do_cmd_rerate(FALSE);
			get_max_stats();
			p_ptr->update |= PU_BONUS;
			lose_all_mutations();
			ident = TRUE;
			break;

		case SV_POTION_NEO_TSUYOSHI:
			(void)set_image(0);
			(void)set_tsuyoshi(p_ptr->tsuyoshi + randint1(100) + 100, FALSE);
			ident = TRUE;
			break;

		case SV_POTION_TSUYOSHI:
			msg_print(_("�u�I�N���Z����I�v", "Brother OKURE!"));
			msg_print(NULL);
			p_ptr->tsuyoshi = 1;
			(void)set_tsuyoshi(0, TRUE);
			if (!p_ptr->resist_chaos)
			{
				(void)set_image(50 + randint1(50));
			}
			ident = TRUE;
			break;
		
		case SV_POTION_POLYMORPH:
			if ((p_ptr->muta1 || p_ptr->muta2 || p_ptr->muta3) && one_in_(23))
			{
				lose_all_mutations();
			}
			else
			{
				do
				{
					if (one_in_(2))
					{
						if(gain_random_mutation(0)) ident = TRUE;
					}
					else if (lose_mutation(0)) ident = TRUE;
				} while(!ident || one_in_(2));
			}
			break;
		}
	}

	if (prace_is_(RACE_SKELETON))
	{
		msg_print(_("�t�̂̈ꕔ�͂��Ȃ��̃A�S��f�ʂ肵�ė������I", "Some of the fluid falls through your jaws!"));
		(void)potion_smash_effect(0, p_ptr->y, p_ptr->x, q_ptr->k_idx);
	}

	/* Combine / Reorder the pack (later) */
	p_ptr->notice |= (PN_COMBINE | PN_REORDER);

	if (!(object_is_aware(q_ptr)))
	{
		chg_virtue(V_PATIENCE, -1);
		chg_virtue(V_CHANCE, 1);
		chg_virtue(V_KNOWLEDGE, -1);
	}

	/* The item has been tried */
	object_tried(q_ptr);

	/* An identification was made */
	if (ident && !object_is_aware(q_ptr))
	{
		object_aware(q_ptr);
		gain_exp((lev + (p_ptr->lev >> 1)) / p_ptr->lev);
	}

	/* Window stuff */
	p_ptr->window |= (PW_INVEN | PW_EQUIP | PW_PLAYER);

	/* Potions can feed the player */
	switch (p_ptr->mimic_form)
	{
	case MIMIC_NONE:
		switch (p_ptr->prace)
		{
			case RACE_VAMPIRE:
				(void)set_food(p_ptr->food + (q_ptr->pval / 10));
				break;
			case RACE_SKELETON:
				/* Do nothing */
				break;
			case RACE_GOLEM:
			case RACE_ZOMBIE:
			case RACE_DEMON:
			case RACE_SPECTRE:
				set_food(p_ptr->food + ((q_ptr->pval) / 20));
				break;
			case RACE_ANDROID:
				if (q_ptr->tval == TV_FLASK)
				{
					msg_print(_("�I�C����⋋�����B", "You replenish yourself with the oil."));
					set_food(p_ptr->food + 5000);
				}
				else
				{
					set_food(p_ptr->food + ((q_ptr->pval) / 20));
				}
				break;
			case RACE_ENT:
				msg_print(_("��������荞�񂾁B", "You are moistened."));
				set_food(MIN(p_ptr->food + q_ptr->pval + MAX(0, q_ptr->pval * 10) + 2000, PY_FOOD_MAX - 1));
				break;
			default:
				(void)set_food(p_ptr->food + q_ptr->pval);
				break;
		}
		break;
	case MIMIC_DEMON:
	case MIMIC_DEMON_LORD:
		set_food(p_ptr->food + ((q_ptr->pval) / 20));
		break;
	case MIMIC_VAMPIRE:
		(void)set_food(p_ptr->food + (q_ptr->pval / 10));
		break;
	default:
		(void)set_food(p_ptr->food + q_ptr->pval);
		break;
	}
}


/*!
 * @brief �I�u�W�F�N�g���v���C���[�����ނ��Ƃ��ł��邩�𔻒肷�� /
 * Hook to determine if an object can be quaffed
 * @param o_ptr ���肵�����I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
 * @return ���ނ��Ƃ��\�Ȃ��TRUE��Ԃ�
 */
static bool item_tester_hook_quaff(object_type *o_ptr)
{
	if (o_ptr->tval == TV_POTION) return TRUE;

	if (prace_is_(RACE_ANDROID))
	{
		if (o_ptr->tval == TV_FLASK && o_ptr->sval == SV_FLASK_OIL)
			return TRUE;
	}
	return FALSE;
}


/*!
 * @brief ������ރR�}���h�̃��C�����[�`�� /
 * Quaff some potion (from the pack or floor)
 * @return �Ȃ�
 */
void do_cmd_quaff_potion(void)
{
	OBJECT_IDX item;
	cptr q, s;

	if (p_ptr->special_defense & (KATA_MUSOU | KATA_KOUKIJIN))
	{
		set_action(ACTION_NONE);
	}

	/* Restrict choices to potions */
	item_tester_hook = item_tester_hook_quaff;

	/* Get an item */
	q = _("�ǂ̖�����݂܂���? ", "Quaff which potion? ");
	s = _("���߂�򂪂Ȃ��B", "You have no potions to quaff.");

	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return;

	/* Quaff the potion */
	do_cmd_quaff_potion_aux(item);
}
