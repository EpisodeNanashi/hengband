
#include "angband.h"

/*!
* @brief ������A�C�e���������o�� /
* Allocates objects upon opening a chest    -BEN-
* @param scatter TRUE�Ȃ�΃g���b�v�ɂ��A�C�e���̊g�U����
* @param y ���̑��݂���}�X��Y���W
* @param x ���̑��݂���}�X��X���W
* @param o_idx ���̃I�u�W�F�N�gID
* @return �Ȃ�
* @details
* <pre>
* Disperse treasures from the given chest, centered at (x,y).
*
* Small chests often contain "gold", while Large chests always contain
* items.  Wooden chests contain 2 items, Iron chests contain 4 items,
* and Steel chests contain 6 items.  The "value" of the items in a
* chest is based on the "power" of the chest, which is in turn based
* on the level on which the chest is generated.
* </pre>
*/
void chest_death(bool scatter, int y, int x, s16b o_idx)
{
	int number;

	bool small;
	BIT_FLAGS mode = AM_GOOD;

	object_type forge;
	object_type *q_ptr;

	object_type *o_ptr = &o_list[o_idx];


	/* Small chests often hold "gold" */
	small = (o_ptr->sval < SV_CHEST_MIN_LARGE);

	/* Determine how much to drop (see above) */
	number = (o_ptr->sval % SV_CHEST_MIN_LARGE) * 2;

	if (o_ptr->sval == SV_CHEST_KANDUME)
	{
		number = 5;
		small = FALSE;
		mode |= AM_GREAT;
		object_level = o_ptr->xtra3;
	}
	else
	{
		/* Determine the "value" of the items */
		object_level = ABS(o_ptr->pval) + 10;
	}

	/* Zero pval means empty chest */
	if (!o_ptr->pval) number = 0;

	/* Opening a chest */
	opening_chest = TRUE;

	/* Drop some objects (non-chests) */
	for (; number > 0; --number)
	{
		/* Get local object */
		q_ptr = &forge;

		/* Wipe the object */
		object_wipe(q_ptr);

		/* Small chests often drop gold */
		if (small && (randint0(100) < 25))
		{
			/* Make some gold */
			if (!make_gold(q_ptr)) continue;
		}

		/* Otherwise drop an item */
		else
		{
			/* Make a good object */
			if (!make_object(q_ptr, mode)) continue;
		}

		/* If chest scatters its contents, pick any floor square. */
		if (scatter)
		{
			int i;
			for (i = 0; i < 200; i++)
			{
				/* Pick a totally random spot. */
				y = randint0(MAX_HGT);
				x = randint0(MAX_WID);

				/* Must be an empty floor. */
				if (!cave_empty_bold(y, x)) continue;

				/* Place the object there. */
				drop_near(q_ptr, -1, y, x);

				/* Done. */
				break;
			}
		}
		/* Normally, drop object near the chest. */
		else drop_near(q_ptr, -1, y, x);
	}

	/* Reset the object level */
	object_level = base_level;

	/* No longer opening a chest */
	opening_chest = FALSE;

	/* Empty */
	o_ptr->pval = 0;

	/* Known */
	object_known(o_ptr);
}


/*!
* @brief ���̃g���b�v���� /
* Chests have traps too.
* @param y ���̑��݂���}�X��Y���W
* @param x ���̑��݂���}�X��X���W
* @param o_idx ���̃I�u�W�F�N�gID
* @return �Ȃ�
* @details
* <pre>
* Exploding chest destroys contents (and traps).
* Note that the chest itself is never destroyed.
* </pre>
*/
void chest_trap(POSITION y, POSITION x, KIND_OBJECT_IDX o_idx)
{
	int  i, trap;

	object_type *o_ptr = &o_list[o_idx];

	int mon_level = o_ptr->xtra3;

	/* Ignore disarmed chests */
	if (o_ptr->pval <= 0) return;

	/* Obtain the traps */
	trap = chest_traps[o_ptr->pval];

	/* Lose strength */
	if (trap & (CHEST_LOSE_STR))
	{
		msg_print(_("�d�|�����Ă��������Ȑj�Ɏh����Ă��܂����I", "A small needle has pricked you!"));
		take_hit(DAMAGE_NOESCAPE, damroll(1, 4), _("�Őj", "a poison needle"), -1);
		(void)do_dec_stat(A_STR);
	}

	/* Lose constitution */
	if (trap & (CHEST_LOSE_CON))
	{
		msg_print(_("�d�|�����Ă��������Ȑj�Ɏh����Ă��܂����I", "A small needle has pricked you!"));
		take_hit(DAMAGE_NOESCAPE, damroll(1, 4), _("�Őj", "a poison needle"), -1);
		(void)do_dec_stat(A_CON);
	}

	/* Poison */
	if (trap & (CHEST_POISON))
	{
		msg_print(_("�˔@�����o�����ΐF�̃K�X�ɕ�ݍ��܂ꂽ�I", "A puff of green gas surrounds you!"));
		if (!(p_ptr->resist_pois || IS_OPPOSE_POIS()))
		{
			(void)set_poisoned(p_ptr->poisoned + 10 + randint1(20));
		}
	}

	/* Paralyze */
	if (trap & (CHEST_PARALYZE))
	{
		msg_print(_("�˔@�����o�������F���K�X�ɕ�ݍ��܂ꂽ�I", "A puff of yellow gas surrounds you!"));
		if (!p_ptr->free_act)
		{
			(void)set_paralyzed(p_ptr->paralyzed + 10 + randint1(20));
		}
	}

	/* Summon monsters */
	if (trap & (CHEST_SUMMON))
	{
		int num = 2 + randint1(3);
		msg_print(_("�˔@�����o�������ɕ�ݍ��܂ꂽ�I", "You are enveloped in a cloud of smoke!"));
		for (i = 0; i < num; i++)
		{
			if (randint1(100)<dun_level)
				activate_hi_summon(p_ptr->y, p_ptr->x, FALSE);
			else
				(void)summon_specific(0, y, x, mon_level, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
		}
	}

	/* Elemental summon. */
	if (trap & (CHEST_E_SUMMON))
	{
		msg_print(_("�����邽�߂ɃG�������^�������ꂽ�I", "Elemental beings appear to protect their treasures!"));
		for (i = 0; i < randint1(3) + 5; i++)
		{
			(void)summon_specific(0, y, x, mon_level, SUMMON_ELEMENTAL, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
		}
	}

	/* Force clouds, then summon birds. */
	if (trap & (CHEST_BIRD_STORM))
	{
		msg_print(_("���̌Q�ꂪ���Ȃ�����芪�����I", "A storm of birds swirls around you!"));

		for (i = 0; i < randint1(3) + 3; i++)
			(void)fire_meteor(-1, GF_FORCE, y, x, o_ptr->pval / 5, 7);

		for (i = 0; i < randint1(5) + o_ptr->pval / 5; i++)
		{
			(void)summon_specific(0, y, x, mon_level, SUMMON_BIRD, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
		}
	}

	/* Various colorful summonings. */
	if (trap & (CHEST_H_SUMMON))
	{
		/* Summon demons. */
		if (one_in_(4))
		{
			msg_print(_("���Ɨ����̉_�̒��Ɉ������p���������I", "Demons materialize in clouds of fire and brimstone!"));
			for (i = 0; i < randint1(3) + 2; i++)
			{
				(void)fire_meteor(-1, GF_FIRE, y, x, 10, 5);
				(void)summon_specific(0, y, x, mon_level, SUMMON_DEMON, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
			}
		}

		/* Summon dragons. */
		else if (one_in_(3))
		{
			msg_print(_("�ÈłɃh���S���̉e���ڂ���ƌ��ꂽ�I", "Draconic forms loom out of the darkness!"));
			for (i = 0; i < randint1(3) + 2; i++)
			{
				(void)summon_specific(0, y, x, mon_level, SUMMON_DRAGON, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
			}
		}

		/* Summon hybrids. */
		else if (one_in_(2))
		{
			msg_print(_("��Ȏp�̉������P���ė����I", "Creatures strange and twisted assault you!"));
			for (i = 0; i < randint1(5) + 3; i++)
			{
				(void)summon_specific(0, y, x, mon_level, SUMMON_HYBRID, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
			}
		}

		/* Summon vortices (scattered) */
		else
		{
			msg_print(_("�Q�������̂��A�j�􂵂��I", "Vortices coalesce and wreak destruction!"));
			for (i = 0; i < randint1(3) + 2; i++)
			{
				(void)summon_specific(0, y, x, mon_level, SUMMON_VORTEX, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE | PM_NO_PET));
			}
		}
	}

	/* Dispel player. */
	if ((trap & (CHEST_RUNES_OF_EVIL)) && o_ptr->k_idx)
	{
		/* Determine how many nasty tricks can be played. */
		int nasty_tricks_count = 4 + randint0(3);

		/* Message. */
		msg_print(_("���낵������������:  �u�Èł������܂�I�v", "Hideous voices bid:  'Let the darkness have thee!'"));
		/* This is gonna hurt... */
		for (; nasty_tricks_count > 0; nasty_tricks_count--)
		{
			/* ...but a high saving throw does help a little. */
			if (randint1(100 + o_ptr->pval * 2) > p_ptr->skill_sav)
			{
				if (one_in_(6)) take_hit(DAMAGE_NOESCAPE, damroll(5, 20), _("�j�ł̃g���b�v�̕�", "a chest dispel-player trap"), -1);
				else if (one_in_(5)) (void)set_cut(p_ptr->cut + 200);
				else if (one_in_(4))
				{
					if (!p_ptr->free_act)
						(void)set_paralyzed(p_ptr->paralyzed + 2 +
							randint0(6));
					else
						(void)set_stun(p_ptr->stun + 10 +
							randint0(100));
				}
				else if (one_in_(3)) apply_disenchant(0);
				else if (one_in_(2))
				{
					(void)do_dec_stat(A_STR);
					(void)do_dec_stat(A_DEX);
					(void)do_dec_stat(A_CON);
					(void)do_dec_stat(A_INT);
					(void)do_dec_stat(A_WIS);
					(void)do_dec_stat(A_CHR);
				}
				else (void)fire_meteor(-1, GF_NETHER, y, x, 150, 1);
			}
		}
	}

	/* Aggravate monsters. */
	if (trap & (CHEST_ALARM))
	{
		msg_print(_("�������܂��������苿�����I", "An alarm sounds!"));
		aggravate_monsters(0);
	}

	/* Explode */
	if ((trap & (CHEST_EXPLODE)) && o_ptr->k_idx)
	{
		msg_print(_("�ˑR�A�������������I", "There is a sudden explosion!"));
		msg_print(_("���̒��̕��͂��ׂĕ��X�ɍӂ��U�����I", "Everything inside the chest is destroyed!"));
		o_ptr->pval = 0;
		sound(SOUND_EXPLODE);
		take_hit(DAMAGE_ATTACK, damroll(5, 8), _("�������锠", "an exploding chest"), -1);
	}
	/* Scatter contents. */
	if ((trap & (CHEST_SCATTER)) && o_ptr->k_idx)
	{
		msg_print(_("�󔠂̒��g�̓_���W�������イ�ɎU�������I", "The contents of the chest scatter all over the dungeon!"));
		chest_death(TRUE, y, x, o_idx);
		o_ptr->pval = 0;
	}
}

