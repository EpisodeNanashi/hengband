#include "angband.h"

/*!
* @brief �g�����v���@�Ǝ��̏����������s�� / Handle summoning and failure of trump spells
* @param num summon_specific()�֐����Ăяo����
* @param pet �y�b�g���Ƃ��ď�������邩�ۂ�
* @param y �����ʒu��y���W
* @param x �����ʒu��x���W
* @param lev �������x��
* @param type ��������ID
* @param mode �����X�^�[���������t���O
* @return �����X�^�[���i�G�΂��܂߂āj���҂��ꂽ�Ȃ��TRUE��Ԃ��B
*/
bool trump_summoning(int num, bool pet, POSITION y, POSITION x, DEPTH lev, int type, BIT_FLAGS mode)
{
	PLAYER_LEVEL plev = p_ptr->lev;

	MONSTER_IDX who;
	int i;
	bool success = FALSE;

	/* Default level */
	if (!lev) lev = plev * 2 / 3 + randint1(plev / 2);

	if (pet)
	{
		/* Become pet */
		mode |= PM_FORCE_PET;

		/* Only sometimes allow unique monster */
		if (mode & PM_ALLOW_UNIQUE)
		{
			/* Forbid often */
			if (randint1(50 + plev) >= plev / 10)
				mode &= ~PM_ALLOW_UNIQUE;
		}

		/* Player is who summons */
		who = -1;
	}
	else
	{
		/* Prevent taming, allow unique monster */
		mode |= PM_NO_PET;

		/* Behave as if they appear by themselfs */
		who = 0;
	}

	for (i = 0; i < num; i++)
	{
		if (summon_specific(who, y, x, lev, type, mode))
			success = TRUE;
	}

	if (!success)
	{
		msg_print(_("�N�����Ȃ��̃J�[�h�̌Ăѐ��ɓ����Ȃ��B", "Nobody answers to your Trump call."));
	}

	return success;
}



/*!
* @brief �����̈�̃O���[�^�[�f�[���������ɗ��p�\�Ȏ��̂��ǂ�����Ԃ��B / An "item_tester_hook" for offer
* @param o_ptr �I�u�W�F�N�g�\���̂̎Q�ƃ|�C���^
* @return ���тɎg�p�\�Ȏ��̂Ȃ��TRUE��Ԃ��B
*/
bool item_tester_offer(object_type *o_ptr)
{
	/* Flasks of oil are okay */
	if (o_ptr->tval != TV_CORPSE) return (FALSE);
	if (o_ptr->sval != SV_CORPSE) return (FALSE);

	if (my_strchr("pht", r_info[o_ptr->pval].d_char)) return (TRUE);

	/* Assume not okay */
	return (FALSE);
}

/*!
* @brief �����̈�̃O���[�^�[�f�[������������������ / Daemon spell Summon Greater Demon
* @return ���������s�����Ȃ��TRUE��Ԃ��B
*/
bool cast_summon_greater_demon(void)
{
	PLAYER_LEVEL plev = p_ptr->lev;
	OBJECT_IDX item;
	cptr q, s;
	int summon_lev;
	object_type *o_ptr;

	item_tester_hook = item_tester_offer;
	q = _("�ǂ̎��̂�����܂���? ", "Sacrifice which corpse? ");
	s = _("�������鎀�̂������Ă��Ȃ��B", "You have nothing to scrifice.");
	if (!get_item(&item, q, s, (USE_INVEN | USE_FLOOR))) return FALSE;

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

	summon_lev = plev * 2 / 3 + r_info[o_ptr->pval].level;

	if (summon_specific(-1, p_ptr->y, p_ptr->x, summon_lev, SUMMON_HI_DEMON, (PM_ALLOW_GROUP | PM_FORCE_PET)))
	{
		msg_print(_("�����̈��L���[�������B", "The area fills with a stench of sulphur and brimstone."));
		msg_print(_("�u���p�ł������܂����A����l�l�v", "'What is thy bidding... Master?'"));

		/* Decrease the item (from the pack) */
		if (item >= 0)
		{
			inven_item_increase(item, -1);
			inven_item_describe(item);
			inven_item_optimize(item);
		}

		/* Decrease the item (from the floor) */
		else
		{
			floor_item_increase(0 - item, -1);
			floor_item_describe(0 - item);
			floor_item_optimize(0 - item);
		}
	}
	else
	{
		msg_print(_("�����͌���Ȃ������B", "No Greater Demon arrive."));
	}

	return TRUE;
}
