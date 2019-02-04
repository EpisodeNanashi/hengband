#include "angband.h"
#include "avatar.h"
#include "spells-status.h"
#include "projection.h"
#include "spells.h"

/*!
 * @brief �����X�^�[�񕜏���
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param dam �З�
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool heal_monster(DIRECTION dir, HIT_POINT dam)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_HEAL, dir, dam, flg));
}

/*!
 * @brief �����X�^�[��������
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param power ����
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool speed_monster(DIRECTION dir, int power)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_SPEED, dir, power, flg));
}

/*!
 * @brief �����X�^�[��������
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param power ����
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool slow_monster(DIRECTION dir, int power)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_SLOW, dir, power, flg));
}

/*!
 * @brief �����X�^�[�Ö�����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param power ����
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool sleep_monster(DIRECTION dir, int power)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_SLEEP, dir, power, flg));
}

/*!
 * @brief �����X�^�[�S��(STASIS)����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 * @details �З͂̓v���C���[���x��*2�ɌŒ�
 */
bool stasis_monster(DIRECTION dir)
{
	return (fire_ball_hide(GF_STASIS, dir, p_ptr->lev * 2, 0));
}

/*!
 * @brief �׈��ȃ����X�^�[�S��(STASIS)����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 * @details �З͂̓v���C���[���x��*2�ɌŒ�
 */
bool stasis_evil(DIRECTION dir)
{
	return (fire_ball_hide(GF_STASIS_EVIL, dir, p_ptr->lev * 2, 0));
}

/*!
 * @brief �����X�^�[��������
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param plev �v���C���[���x��(=����)
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool confuse_monster(DIRECTION dir, PLAYER_LEVEL plev)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_CONF, dir, plev, flg));
}

/*!
 * @brief �����X�^�[�N�O����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param plev �v���C���[���x��(=����)
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool stun_monster(DIRECTION dir, PLAYER_LEVEL plev)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_STUN, dir, plev, flg));
}

/*!
 * @brief �`�F���W�����X�^�[����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param power ����
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool poly_monster(DIRECTION dir, int power)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	bool tester = (project_hook(GF_OLD_POLY, dir, power, flg));
	if (tester)
		chg_virtue(V_CHANCE, 1);
	return(tester);
}

/*!
 * @brief �N���[�������X�^�[����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool clone_monster(DIRECTION dir)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_OLD_CLONE, dir, 0, flg));
}

/*!
 * @brief �����X�^�[���Q����
 * @param dir ����(5�Ȃ�΃O���[�o���ϐ� target_col/target_row �̍��W��ڕW�ɂ���)
 * @param plev �v���C���[���x��(=����)
 * @return ��p�����ۂɂ������ꍇTRUE��Ԃ�
 */
bool fear_monster(DIRECTION dir, PLAYER_LEVEL plev)
{
	BIT_FLAGS flg = PROJECT_STOP | PROJECT_KILL | PROJECT_REFLECTABLE;
	return (project_hook(GF_TURN_ALL, dir, plev, flg));
}

/*!
* @brief �̂̒�~���������� / Stop singing if the player is a Bard
* @return �Ȃ�
*/
void stop_singing(void)
{
	if (p_ptr->pclass != CLASS_BARD) return;

	/* Are there interupted song? */
	if (INTERUPTING_SONG_EFFECT(p_ptr))
	{
		/* Forget interupted song */
		INTERUPTING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
		return;
	}

	/* The player is singing? */
	if (!SINGING_SONG_EFFECT(p_ptr)) return;

	/* Hack -- if called from set_action(), avoid recursive loop */
	if (p_ptr->action == ACTION_SING) set_action(ACTION_NONE);

	/* Message text of each song or etc. */
	do_spell(REALM_MUSIC, SINGING_SONG_ID(p_ptr), SPELL_STOP);

	SINGING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
	SINGING_SONG_ID(p_ptr) = 0;
	p_ptr->update |= (PU_BONUS);
	p_ptr->redraw |= (PR_STATUS);
}

bool time_walk(player_type *creature_ptr)
{
	if (world_player)
	{
		msg_print(_("���Ɏ��͎~�܂��Ă���B", "Time is already stopped."));
		return (FALSE);
	}
	world_player = TRUE;
	msg_print(_("�u����I�v", "You yell 'Time!'"));
	msg_print(NULL);

	/* Hack */
	p_ptr->energy_need -= 1000 + (100 + p_ptr->csp - 50)*TURNS_PER_TICK / 10;
	p_ptr->redraw |= (PR_MAP);
	p_ptr->update |= (PU_MONSTERS);
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
	handle_stuff();
	return TRUE;
}

/*!
 * @brief �v���C���[�̃q�b�g�_�C�X��U�� / Role Hitpoints
 * @param options �X�y�����ʃI�v�V����
 * @return �Ȃ�
 */
void roll_hitdice(SPOP_FLAGS options)
{
	PERCENTAGE percent;

	/* Minimum hitpoints at highest level */
	HIT_POINT min_value = p_ptr->hitdie + ((PY_MAX_LEVEL + 2) * (p_ptr->hitdie + 1)) * 3 / 8;

	/* Maximum hitpoints at highest level */
	HIT_POINT max_value = p_ptr->hitdie + ((PY_MAX_LEVEL + 2) * (p_ptr->hitdie + 1)) * 5 / 8;

	int i;

	/* Rerate */
	while (1)
	{
		/* Pre-calculate level 1 hitdice */
		p_ptr->player_hp[0] = (HIT_POINT)p_ptr->hitdie;

		for (i = 1; i < 4; i++)
		{
			p_ptr->player_hp[0] += randint1(p_ptr->hitdie);
		}

		/* Roll the hitpoint values */
		for (i = 1; i < PY_MAX_LEVEL; i++)
		{
			p_ptr->player_hp[i] = p_ptr->player_hp[i - 1] + randint1(p_ptr->hitdie);
		}

		/* Require "valid" hitpoints at highest level */
		if ((p_ptr->player_hp[PY_MAX_LEVEL - 1] >= min_value) &&
			(p_ptr->player_hp[PY_MAX_LEVEL - 1] <= max_value)) break;
	}

	percent = (int)(((long)p_ptr->player_hp[PY_MAX_LEVEL - 1] * 200L) /
		(2 * p_ptr->hitdie + ((PY_MAX_LEVEL - 1 + 3) * (p_ptr->hitdie + 1))));

	/* Update and redraw hitpoints */
	p_ptr->update |= (PU_HP);
	p_ptr->redraw |= (PR_HP);
	p_ptr->window |= (PW_PLAYER);

	if (!(options & SPOP_NO_UPDATE)) handle_stuff();

	if (options & SPOP_DISPLAY_MES)
	{
		if (options & SPOP_DEBUG)
		{
			msg_format(_("���݂̗̑̓����N�� %d/100 �ł��B", "Your life rate is %d/100 now."), percent);
			p_ptr->knowledge |= KNOW_HPRATE;
		}
		else
		{
			msg_print(_("�̗̓����N���ς�����B", "Life rate is changed."));
			p_ptr->knowledge &= ~(KNOW_HPRATE);
		}
	}
}

bool_hack life_stream(bool_hack message, bool_hack virtue_change)
{
	if (virtue_change)
	{
		chg_virtue(V_VITALITY, 1);
		chg_virtue(V_UNLIFE, -5);
	}
	if (message)
	{
		msg_print(_("�̒��ɐ����͂��������ӂ�Ă����I", "You feel life flow through your body!"));
	}
	restore_level();
	(void)set_poisoned(0);
	(void)set_blind(0);
	(void)set_confused(0);
	(void)set_image(0);
	(void)set_stun(0);
	(void)set_cut(0);
	(void)restore_all_status();
	(void)set_shero(0, TRUE);
	handle_stuff();
	hp_player(5000);

	return TRUE;
}

bool_hack heroism(int base)
{
	bool_hack ident = FALSE;
	if (set_afraid(0)) ident = TRUE;
	if (set_hero(p_ptr->hero + randint1(base) + base, FALSE)) ident = TRUE;
	if (hp_player(10)) ident = TRUE;
	return ident;
}

bool_hack berserk(int base)
{
	bool_hack ident = FALSE;
	if (set_afraid(0)) ident = TRUE;
	if (set_shero(p_ptr->hero + randint1(base) + base, FALSE)) ident = TRUE;
	if (hp_player(30)) ident = TRUE;
	return ident;
}

bool_hack cure_light_wounds(DICE_NUMBER dice, DICE_SID sides)
{
	bool_hack ident = FALSE;
	if (hp_player(damroll(dice, sides))) ident = TRUE;
	if (set_blind(0)) ident = TRUE;
	if (set_cut(p_ptr->cut - 10)) ident = TRUE;
	if (set_shero(0, TRUE)) ident = TRUE;
	return ident;
}

bool_hack cure_serious_wounds(DICE_NUMBER dice, DICE_SID sides)
{
	bool_hack ident = FALSE;
	if (hp_player(damroll(dice, sides))) ident = TRUE;
	if (set_blind(0)) ident = TRUE;
	if (set_confused(0)) ident = TRUE;
	if (set_cut((p_ptr->cut / 2) - 50)) ident = TRUE;
	if (set_shero(0, TRUE)) ident = TRUE;
	return ident;
}

bool_hack cure_critical_wounds(HIT_POINT pow)
{
	bool_hack ident = FALSE;
	if (hp_player(pow)) ident = TRUE;
	if (set_blind(0)) ident = TRUE;
	if (set_confused(0)) ident = TRUE;
	if (set_poisoned(0)) ident = TRUE;
	if (set_stun(0)) ident = TRUE;
	if (set_cut(0)) ident = TRUE;
	if (set_shero(0, TRUE)) ident = TRUE;
	return ident;
}

bool_hack true_healing(HIT_POINT pow)
{
	bool_hack ident = FALSE;
	if (hp_player(pow)) ident = TRUE;
	if (set_blind(0)) ident = TRUE;
	if (set_confused(0)) ident = TRUE;
	if (set_poisoned(0)) ident = TRUE;
	if (set_stun(0)) ident = TRUE;
	if (set_cut(0)) ident = TRUE;
	if (set_image(0)) ident = TRUE;
	return ident;
}

bool_hack restore_mana(bool_hack magic_eater)
{
	bool_hack ident = FALSE;

	if (p_ptr->pclass == CLASS_MAGIC_EATER && magic_eater)
	{
		int i;
		for (i = 0; i < EATER_EXT * 2; i++)
		{
			p_ptr->magic_num1[i] += (p_ptr->magic_num2[i] < 10) ? EATER_CHARGE * 3 : p_ptr->magic_num2[i] * EATER_CHARGE / 3;
			if (p_ptr->magic_num1[i] > p_ptr->magic_num2[i] * EATER_CHARGE) p_ptr->magic_num1[i] = p_ptr->magic_num2[i] * EATER_CHARGE;
		}
		for (; i < EATER_EXT * 3; i++)
		{
			KIND_OBJECT_IDX k_idx = lookup_kind(TV_ROD, i - EATER_EXT * 2);
			p_ptr->magic_num1[i] -= ((p_ptr->magic_num2[i] < 10) ? EATER_ROD_CHARGE * 3 : p_ptr->magic_num2[i] * EATER_ROD_CHARGE / 3)*k_info[k_idx].pval;
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

	return ident;
}

bool restore_all_status(void)
{
	bool ident = FALSE;
	if (do_res_stat(A_STR)) ident = TRUE;
	if (do_res_stat(A_INT)) ident = TRUE;
	if (do_res_stat(A_WIS)) ident = TRUE;
	if (do_res_stat(A_DEX)) ident = TRUE;
	if (do_res_stat(A_CON)) ident = TRUE;
	if (do_res_stat(A_CHR)) ident = TRUE;
	return ident;
}
