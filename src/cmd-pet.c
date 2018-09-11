#include "angband.h"

/*!
* @brief �v���C���[�̋R��/���n��������
* @param c_ptr �v���C���[�̈ړ���}�X�̍\���̎Q�ƃ|�C���^
* @param now_riding TRUE�Ȃ牺�n�����AFALSE�Ȃ�΋R�揈��
* @return �\�Ȃ��TRUE��Ԃ�
*/
bool player_can_ride_aux(cave_type *c_ptr, bool now_riding)
{
	bool p_can_enter;
	bool old_character_xtra = character_xtra;
	MONSTER_IDX old_riding = p_ptr->riding;
	bool old_riding_ryoute = p_ptr->riding_ryoute;
	bool old_old_riding_ryoute = p_ptr->old_riding_ryoute;
	bool old_pf_ryoute = (p_ptr->pet_extra_flags & PF_RYOUTE) ? TRUE : FALSE;

	/* Hack -- prevent "icky" message */
	character_xtra = TRUE;

	if (now_riding) p_ptr->riding = c_ptr->m_idx;
	else
	{
		p_ptr->riding = 0;
		p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
		p_ptr->riding_ryoute = p_ptr->old_riding_ryoute = FALSE;
	}

	calc_bonuses();

	p_can_enter = player_can_enter(c_ptr->feat, CEM_P_CAN_ENTER_PATTERN);

	p_ptr->riding = old_riding;
	if (old_pf_ryoute) p_ptr->pet_extra_flags |= (PF_RYOUTE);
	else p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
	p_ptr->riding_ryoute = old_riding_ryoute;
	p_ptr->old_riding_ryoute = old_old_riding_ryoute;

	calc_bonuses();

	character_xtra = old_character_xtra;

	return p_can_enter;
}


/*!
* @brief �y�b�g�ɂȂ��Ă��郂���X�^�[���\�[�g���邽�߂̔�r����
* @param u �����X�^�[�̍\���̔z��
* @param v ���g�p
* @param a ��r�Ώۂ̃����X�^�[ID1
* @param b ��r�Ώۂ̃����X�^�[ID2
* @return 2�Ԗڂ���Ȃ��TRUE��Ԃ�
*/
static bool ang_sort_comp_pet_dismiss(vptr u, vptr v, int a, int b)
{
	u16b *who = (u16b*)(u);

	int w1 = who[a];
	int w2 = who[b];

	monster_type *m_ptr1 = &m_list[w1];
	monster_type *m_ptr2 = &m_list[w2];
	monster_race *r_ptr1 = &r_info[m_ptr1->r_idx];
	monster_race *r_ptr2 = &r_info[m_ptr2->r_idx];

	/* Unused */
	(void)v;

	if (w1 == p_ptr->riding) return TRUE;
	if (w2 == p_ptr->riding) return FALSE;

	if (m_ptr1->nickname && !m_ptr2->nickname) return TRUE;
	if (m_ptr2->nickname && !m_ptr1->nickname) return FALSE;

	if (!m_ptr1->parent_m_idx && m_ptr2->parent_m_idx) return TRUE;
	if (!m_ptr2->parent_m_idx && m_ptr1->parent_m_idx) return FALSE;

	if ((r_ptr1->flags1 & RF1_UNIQUE) && !(r_ptr2->flags1 & RF1_UNIQUE)) return TRUE;
	if ((r_ptr2->flags1 & RF1_UNIQUE) && !(r_ptr1->flags1 & RF1_UNIQUE)) return FALSE;

	if (r_ptr1->level > r_ptr2->level) return TRUE;
	if (r_ptr2->level > r_ptr1->level) return FALSE;

	if (m_ptr1->hp > m_ptr2->hp) return TRUE;
	if (m_ptr2->hp > m_ptr1->hp) return FALSE;

	return w1 <= w2;
}


/*!
* @brief �y�b�g�̈ێ��R�X�g�v�Z
* @return �ێ��R�X�g(%)
*/
int calculate_upkeep(void)
{
	s32b old_friend_align = friend_align;
	MONSTER_IDX m_idx;
	bool have_a_unique = FALSE;
	s32b total_friend_levels = 0;

	total_friends = 0;
	friend_align = 0;

	for (m_idx = m_max - 1; m_idx >= 1; m_idx--)
	{
		monster_type *m_ptr;
		monster_race *r_ptr;

		m_ptr = &m_list[m_idx];
		if (!m_ptr->r_idx) continue;
		r_ptr = &r_info[m_ptr->r_idx];

		if (is_pet(m_ptr))
		{
			total_friends++;
			if (r_ptr->flags1 & RF1_UNIQUE)
			{
				if (p_ptr->pclass == CLASS_CAVALRY)
				{
					if (p_ptr->riding == m_idx)
						total_friend_levels += (r_ptr->level + 5) * 2;
					else if (!have_a_unique && (r_info[m_ptr->r_idx].flags7 & RF7_RIDING))
						total_friend_levels += (r_ptr->level + 5) * 7 / 2;
					else
						total_friend_levels += (r_ptr->level + 5) * 10;
					have_a_unique = TRUE;
				}
				else
					total_friend_levels += (r_ptr->level + 5) * 10;
			}
			else
				total_friend_levels += r_ptr->level;

			/* Determine pet alignment */
			if (r_ptr->flags3 & RF3_GOOD) friend_align += r_ptr->level;
			if (r_ptr->flags3 & RF3_EVIL) friend_align -= r_ptr->level;
		}
	}
	if (old_friend_align != friend_align) p_ptr->update |= (PU_BONUS);
	if (total_friends)
	{
		int upkeep_factor;
		upkeep_factor = (total_friend_levels - (p_ptr->lev * 80 / (cp_ptr->pet_upkeep_div)));
		if (upkeep_factor < 0) upkeep_factor = 0;
		if (upkeep_factor > 1000) upkeep_factor = 1000;
		return upkeep_factor;
	}
	else
		return 0;
}

/*!
* @brief �y�b�g���J������R�}���h�̃��C�����[�`��
* @return �Ȃ�
*/
void do_cmd_pet_dismiss(void)
{
	monster_type	*m_ptr;
	bool		all_pets = FALSE;
	MONSTER_IDX pet_ctr;
	int i;
	int Dismissed = 0;

	MONSTER_IDX *who;
	u16b dummy_why;
	int max_pet = 0;
	bool_hack cu, cv;

	cu = Term->scr->cu;
	cv = Term->scr->cv;
	Term->scr->cu = 0;
	Term->scr->cv = 1;

	/* Allocate the "who" array */
	C_MAKE(who, max_m_idx, MONSTER_IDX);

	/* Process the monsters (backwards) */
	for (pet_ctr = m_max - 1; pet_ctr >= 1; pet_ctr--)
	{
		if (is_pet(&m_list[pet_ctr]))
			who[max_pet++] = pet_ctr;
	}

	/* Select the sort method */
	ang_sort_comp = ang_sort_comp_pet_dismiss;
	ang_sort_swap = ang_sort_swap_hook;

	ang_sort(who, &dummy_why, max_pet);

	/* Process the monsters (backwards) */
	for (i = 0; i < max_pet; i++)
	{
		bool delete_this;
		char friend_name[80];
		bool kakunin;

		/* Access the monster */
		pet_ctr = who[i];
		m_ptr = &m_list[pet_ctr];

		delete_this = FALSE;
		kakunin = ((pet_ctr == p_ptr->riding) || (m_ptr->nickname));
		monster_desc(friend_name, m_ptr, MD_ASSUME_VISIBLE);

		if (!all_pets)
		{
			/* Hack -- health bar for this monster */
			health_track(pet_ctr);

			/* Hack -- handle stuff */
			handle_stuff();

			msg_format(_("%s������܂����H [Yes/No/Unnamed (%d��)]", "Dismiss %s? [Yes/No/Unnamed (%d remain)]"), friend_name, max_pet - i);

			if (m_ptr->ml)
				move_cursor_relative(m_ptr->fy, m_ptr->fx);

			while (TRUE)
			{
				char ch = inkey();

				if (ch == 'Y' || ch == 'y')
				{
					delete_this = TRUE;

					if (kakunin)
					{
						msg_format(_("�{���ɂ�낵���ł����H (%s) ", "Are you sure? (%s) "), friend_name);
						ch = inkey();
						if (ch != 'Y' && ch != 'y')
							delete_this = FALSE;
					}
					break;
				}

				if (ch == 'U' || ch == 'u')
				{
					all_pets = TRUE;
					break;
				}

				if (ch == ESCAPE || ch == 'N' || ch == 'n')
					break;

				bell();
			}
		}

		if ((all_pets && !kakunin) || (!all_pets && delete_this))
		{
			if (record_named_pet && m_ptr->nickname)
			{
				char m_name[80];

				monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
				do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_DISMISS, m_name);
			}

			if (pet_ctr == p_ptr->riding)
			{
				msg_format(_("%s����~�肽�B", "You have got off %s. "), friend_name);

				p_ptr->riding = 0;

				/* Update the monsters */
				p_ptr->update |= (PU_BONUS | PU_MONSTERS);
				p_ptr->redraw |= (PR_EXTRA | PR_UHEALTH);
			}

			/* HACK : Add the line to message buffer */
			msg_format(_("%s ��������B", "Dismissed %s."), friend_name);
			p_ptr->window |= (PW_MESSAGE);
			window_stuff();

			delete_monster_idx(pet_ctr);
			Dismissed++;
		}
	}

	Term->scr->cu = cu;
	Term->scr->cv = cv;
	Term_fresh();

	C_KILL(who, max_m_idx, MONSTER_IDX);

#ifdef JP
	msg_format("%d �̂̃y�b�g������܂����B", Dismissed);
#else
	msg_format("You have dismissed %d pet%s.", Dismissed,
		(Dismissed == 1 ? "" : "s"));
#endif
	if (Dismissed == 0 && all_pets)
		msg_print(_("'U'nnamed �́A��n�ȊO�̖��O�̂Ȃ��y�b�g������S�ĉ�����܂��B", "'U'nnamed means all your pets except named pets and your mount."));
}



/*!
* @brief �y�b�g����R��/���n����R�}���h�̃��C�����[�`�� /
* @param force �����I�ɋR��/���n����Ȃ��TRUE
* @return �R��/���n�ł�����TRUE
*/
bool do_riding(bool force)
{
	int x, y, dir = 0;
	cave_type *c_ptr;
	monster_type *m_ptr;

	if (!get_rep_dir2(&dir)) return FALSE;
	y = p_ptr->y + ddy[dir];
	x = p_ptr->x + ddx[dir];
	c_ptr = &cave[y][x];

	if (p_ptr->special_defense & KATA_MUSOU) set_action(ACTION_NONE);

	if (p_ptr->riding)
	{
		/* Skip non-empty grids */
		if (!player_can_ride_aux(c_ptr, FALSE))
		{
			msg_print(_("������ɂ͍~����܂���B", "You cannot go to that direction."));
			return FALSE;
		}

		if (!pattern_seq(p_ptr->y, p_ptr->x, y, x)) return FALSE;

		if (c_ptr->m_idx)
		{
			/* Take a turn */
			p_ptr->energy_use = 100;

			/* Message */
			msg_print(_("�����X�^�[�������ӂ������Ă���I", "There is a monster in the way!"));

			py_attack(y, x, 0);
			return FALSE;
		}

		p_ptr->riding = 0;
		p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
		p_ptr->riding_ryoute = p_ptr->old_riding_ryoute = FALSE;
	}
	else
	{
		if (p_ptr->confused)
		{
			msg_print(_("�������Ă��ď��Ȃ��I", "You are too confused!"));
			return FALSE;
		}

		m_ptr = &m_list[c_ptr->m_idx];

		if (!c_ptr->m_idx || !m_ptr->ml)
		{
			msg_print(_("���̏ꏊ�ɂ̓����X�^�[�͂��܂���B", "Here is no monster."));
			return FALSE;
		}
		if (!is_pet(m_ptr) && !force)
		{
			msg_print(_("���̃����X�^�[�̓y�b�g�ł͂���܂���B", "That monster is not a pet."));
			return FALSE;
		}
		if (!(r_info[m_ptr->r_idx].flags7 & RF7_RIDING))
		{
			msg_print(_("���̃����X�^�[�ɂ͏��Ȃ��������B", "This monster doesn't seem suitable for riding."));
			return FALSE;
		}

		if (!pattern_seq(p_ptr->y, p_ptr->x, y, x)) return FALSE;

		if (!player_can_ride_aux(c_ptr, TRUE))
		{
			/* Feature code (applying "mimic" field) */
			feature_type *f_ptr = &f_info[get_feat_mimic(c_ptr)];
#ifdef JP
			msg_format("���̃����X�^�[��%s��%s�ɂ���B", f_name + f_ptr->name,
				((!have_flag(f_ptr->flags, FF_MOVE) && !have_flag(f_ptr->flags, FF_CAN_FLY)) ||
					(!have_flag(f_ptr->flags, FF_LOS) && !have_flag(f_ptr->flags, FF_TREE))) ?
				"��" : "��");
#else
			msg_format("This monster is %s the %s.",
				((!have_flag(f_ptr->flags, FF_MOVE) && !have_flag(f_ptr->flags, FF_CAN_FLY)) ||
					(!have_flag(f_ptr->flags, FF_LOS) && !have_flag(f_ptr->flags, FF_TREE))) ?
				"in" : "on", f_name + f_ptr->name);
#endif

			return FALSE;
		}
		if (r_info[m_ptr->r_idx].level > randint1((p_ptr->skill_exp[GINOU_RIDING] / 50 + p_ptr->lev / 2 + 20)))
		{
			msg_print(_("���܂����Ȃ������B", "You failed to ride."));
			p_ptr->energy_use = 100;
			return FALSE;
		}

		if (MON_CSLEEP(m_ptr))
		{
			char m_name[80];
			monster_desc(m_name, m_ptr, 0);
			(void)set_monster_csleep(c_ptr->m_idx, 0);
			msg_format(_("%s���N�������B", "You have waked %s up."), m_name);
		}

		if (p_ptr->action == ACTION_KAMAE) set_action(ACTION_NONE);

		p_ptr->riding = c_ptr->m_idx;

		/* Hack -- remove tracked monster */
		if (p_ptr->riding == p_ptr->health_who) health_track(0);
	}

	p_ptr->energy_use = 100;

	/* Mega-Hack -- Forget the view and lite */
	p_ptr->update |= (PU_UN_VIEW | PU_UN_LITE);

	/* Update the monsters */
	p_ptr->update |= (PU_BONUS);

	/* Redraw map */
	p_ptr->redraw |= (PR_MAP | PR_EXTRA);

	p_ptr->redraw |= (PR_UHEALTH);

	/* Move the player */
	(void)move_player_effect(y, x, MPE_HANDLE_STUFF | MPE_ENERGY_USE | MPE_DONT_PICKUP | MPE_DONT_SWAP_MON);

	return TRUE;
}

/*!
* @brief �y�b�g�ɖ��O������R�}���h�̃��C�����[�`��
* @return �Ȃ�
*/
static void do_name_pet(void)
{
	monster_type *m_ptr;
	char out_val[20];
	char m_name[80];
	bool old_name = FALSE;
	bool old_target_pet = target_pet;

	target_pet = TRUE;
	if (!target_set(TARGET_KILL))
	{
		target_pet = old_target_pet;
		return;
	}
	target_pet = old_target_pet;

	if (cave[target_row][target_col].m_idx)
	{
		m_ptr = &m_list[cave[target_row][target_col].m_idx];

		if (!is_pet(m_ptr))
		{
			/* Message */
			msg_print(_("���̃����X�^�[�̓y�b�g�ł͂Ȃ��B", "This monster is not a pet."));
			return;
		}
		if (r_info[m_ptr->r_idx].flags1 & RF1_UNIQUE)
		{
			msg_print(_("���̃����X�^�[�̖��O�͕ς����Ȃ��I", "You cannot change name of this monster!"));
			return;
		}
		monster_desc(m_name, m_ptr, 0);

		/* Message */
		msg_format(_("%s�ɖ��O������B", "Name %s."), m_name);
		msg_print(NULL);

		/* Start with nothing */
		strcpy(out_val, "");

		/* Use old inscription */
		if (m_ptr->nickname)
		{
			/* Start with the old inscription */
			strcpy(out_val, quark_str(m_ptr->nickname));
			old_name = TRUE;
		}

		/* Get a new inscription (possibly empty) */
		if (get_string(_("���O: ", "Name: "), out_val, 15))
		{
			if (out_val[0])
			{
				/* Save the inscription */
				m_ptr->nickname = quark_add(out_val);
				if (record_named_pet)
				{
					monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
					do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_NAME, m_name);
				}
			}
			else
			{
				if (record_named_pet && old_name)
				{
					monster_desc(m_name, m_ptr, MD_INDEF_VISIBLE);
					do_cmd_write_nikki(NIKKI_NAMED_PET, RECORD_NAMED_PET_UNNAME, m_name);
				}
				m_ptr->nickname = 0;
			}
		}
	}
}


/*!
* @brief �y�b�g�Ɋւ���R�}���h���X�g�̃��C�����[�`�� /
* Issue a pet command
* @return �Ȃ�
*/
void do_cmd_pet(void)
{
	COMMAND_CODE i = 0;
	int			num;
	int			powers[36];
	cptr			power_desc[36];
	bool			flag, redraw;
	char			choice;
	char			out_val[160];
	int			pet_ctr;
	monster_type	*m_ptr;

	PET_COMMAND_IDX mode = 0;

	char buf[160];
	char target_buf[160];

	int menu_line = use_menu ? 1 : 0;

	num = 0;

	power_desc[num] = _("�y�b�g�����", "dismiss pets");
	powers[num++] = PET_DISMISS;

#ifdef JP
	sprintf(target_buf, "�y�b�g�̃^�[�Q�b�g���w�� (���݁F%s)",
		(pet_t_m_idx ? (p_ptr->image ? "������ȕ�" : (r_name + r_info[m_list[pet_t_m_idx].ap_r_idx].name)) : "�w��Ȃ�"));
#else
	sprintf(target_buf, "specify a target of pet (now:%s)",
		(pet_t_m_idx ? (p_ptr->image ? "something strange" : (r_name + r_info[m_list[pet_t_m_idx].ap_r_idx].name)) : "nothing"));
#endif
	power_desc[num] = target_buf;
	powers[num++] = PET_TARGET;
	power_desc[num] = _("�߂��ɂ���", "stay close");

	if (p_ptr->pet_follow_distance == PET_CLOSE_DIST) mode = num;
	powers[num++] = PET_STAY_CLOSE;
	power_desc[num] = _("���ė���", "follow me");

	if (p_ptr->pet_follow_distance == PET_FOLLOW_DIST) mode = num;
	powers[num++] = PET_FOLLOW_ME;
	power_desc[num] = _("�G�������ē|��", "seek and destroy");

	if (p_ptr->pet_follow_distance == PET_DESTROY_DIST) mode = num;
	powers[num++] = PET_SEEK_AND_DESTROY;
	power_desc[num] = _("��������Ă���", "give me space");

	if (p_ptr->pet_follow_distance == PET_SPACE_DIST) mode = num;
	powers[num++] = PET_ALLOW_SPACE;
	power_desc[num] = _("����Ă���", "stay away");

	if (p_ptr->pet_follow_distance == PET_AWAY_DIST) mode = num;
	powers[num++] = PET_STAY_AWAY;

	if (p_ptr->pet_extra_flags & PF_OPEN_DOORS)
	{
		power_desc[num] = _("�h�A���J���� (����:ON)", "pets open doors (now On)");
	}
	else
	{
		power_desc[num] = _("�h�A���J���� (����:OFF)", "pets open doors (now Off)");
	}
	powers[num++] = PET_OPEN_DOORS;

	if (p_ptr->pet_extra_flags & PF_PICKUP_ITEMS)
	{
		power_desc[num] = _("�A�C�e�����E�� (����:ON)", "pets pick up items (now On)");
	}
	else
	{
		power_desc[num] = _("�A�C�e�����E�� (����:OFF)", "pets pick up items (now Off)");
	}
	powers[num++] = PET_TAKE_ITEMS;

	if (p_ptr->pet_extra_flags & PF_TELEPORT)
	{
		power_desc[num] = _("�e���|�[�g�n���@���g�� (����:ON)", "allow teleport (now On)");
	}
	else
	{
		power_desc[num] = _("�e���|�[�g�n���@���g�� (����:OFF)", "allow teleport (now Off)");
	}
	powers[num++] = PET_TELEPORT;

	if (p_ptr->pet_extra_flags & PF_ATTACK_SPELL)
	{
		power_desc[num] = _("�U�����@���g�� (����:ON)", "allow cast attack spell (now On)");
	}
	else
	{
		power_desc[num] = _("�U�����@���g�� (����:OFF)", "allow cast attack spell (now Off)");
	}
	powers[num++] = PET_ATTACK_SPELL;

	if (p_ptr->pet_extra_flags & PF_SUMMON_SPELL)
	{
		power_desc[num] = _("�������@���g�� (����:ON)", "allow cast summon spell (now On)");
	}
	else
	{
		power_desc[num] = _("�������@���g�� (����:OFF)", "allow cast summon spell (now Off)");
	}
	powers[num++] = PET_SUMMON_SPELL;

	if (p_ptr->pet_extra_flags & PF_BALL_SPELL)
	{
		power_desc[num] = _("�v���C���[���������ޔ͈͖��@���g�� (����:ON)", "allow involve player in area spell (now On)");
	}
	else
	{
		power_desc[num] = _("�v���C���[���������ޔ͈͖��@���g�� (����:OFF)", "allow involve player in area spell (now Off)");
	}
	powers[num++] = PET_BALL_SPELL;

	if (p_ptr->riding)
	{
		power_desc[num] = _("�y�b�g����~���", "get off a pet");
	}
	else
	{
		power_desc[num] = _("�y�b�g�ɏ��", "ride a pet");
	}
	powers[num++] = PET_RIDING;
	power_desc[num] = _("�y�b�g�ɖ��O������", "name pets");
	powers[num++] = PET_NAME;

	if (p_ptr->riding)
	{
		if ((p_ptr->migite && (empty_hands(FALSE) == EMPTY_HAND_LARM) &&
			object_allow_two_hands_wielding(&inventory[INVEN_RARM])) ||
			(p_ptr->hidarite && (empty_hands(FALSE) == EMPTY_HAND_RARM) &&
				object_allow_two_hands_wielding(&inventory[INVEN_LARM])))
		{
			if (p_ptr->pet_extra_flags & PF_RYOUTE)
			{
				power_desc[num] = _("�����Ў�Ŏ���", "use one hand to control a riding pet");
			}
			else
			{
				power_desc[num] = _("����𗼎�Ŏ���", "use both hands for a weapon");
			}

			powers[num++] = PET_RYOUTE;
		}
		else
		{
			switch (p_ptr->pclass)
			{
			case CLASS_MONK:
			case CLASS_FORCETRAINER:
			case CLASS_BERSERKER:
				if (empty_hands(FALSE) == (EMPTY_HAND_RARM | EMPTY_HAND_LARM))
				{
					if (p_ptr->pet_extra_flags & PF_RYOUTE)
					{
						power_desc[num] = _("�Ў�Ŋi������", "use one hand to control a riding pet");
					}
					else
					{
						power_desc[num] = _("����Ŋi������", "use both hands for melee");
					}

					powers[num++] = PET_RYOUTE;
				}
				else if ((empty_hands(FALSE) != EMPTY_HAND_NONE) && !buki_motteruka(INVEN_RARM) && !buki_motteruka(INVEN_LARM))
				{
					if (p_ptr->pet_extra_flags & PF_RYOUTE)
					{
						power_desc[num] = _("�i�����s��Ȃ�", "use one hand to control a riding pet");
					}
					else
					{
						power_desc[num] = _("�i�����s��", "use one hand for melee");
					}

					powers[num++] = PET_RYOUTE;
				}
				break;
			}
		}
	}

#ifdef ALLOW_REPEAT
	if (!(repeat_pull(&i) && (i >= 0) && (i < num)))
	{
#endif /* ALLOW_REPEAT */

		/* Nothing chosen yet */
		flag = FALSE;

		/* No redraw yet */
		redraw = FALSE;

		if (use_menu)
		{
			/* Save the screen */
			screen_save();

			/* Build a prompt */
			strnfmt(out_val, 78, _("(�R�}���h�AESC=�I��) �R�}���h��I��ł�������:", "(Command, ESC=exit) Choose command from menu."));
		}
		else
		{
			/* Build a prompt */
			strnfmt(out_val, 78,
				_("(�R�}���h %c-%c�A'*'=�ꗗ�AESC=�I��) �R�}���h��I��ł�������:", "(Command %c-%c, *=List, ESC=exit) Select a command: "),
				I2A(0), I2A(num - 1));
		}

		choice = (always_show_list || use_menu) ? ESCAPE : 1;

		/* Get a command from the user */
		while (!flag)
		{
			int ask = TRUE;

			if (choice == ESCAPE) choice = ' ';
			else if (!get_com(out_val, &choice, TRUE)) break;

			if (use_menu && (choice != ' '))
			{
				switch (choice)
				{
				case '0':
					screen_load();
					return;

				case '8':
				case 'k':
				case 'K':
					menu_line += (num - 1);
					break;

				case '2':
				case 'j':
				case 'J':
					menu_line++;
					break;

				case '4':
				case 'h':
				case 'H':
					menu_line = 1;
					break;

				case '6':
				case 'l':
				case 'L':
					menu_line = num;
					break;

				case 'x':
				case 'X':
				case '\r':
				case '\n':
					i = menu_line - 1;
					ask = FALSE;
					break;
				}
				if (menu_line > num) menu_line -= num;
			}

			/* Request redraw */
			if ((choice == ' ') || (choice == '*') || (choice == '?') || (use_menu && ask))
			{
				/* Show the list */
				if (!redraw || use_menu)
				{
					byte y = 1, x = 0;
					PET_COMMAND_IDX ctr = 0;

					/* Show list */
					redraw = TRUE;

					/* Save the screen */
					if (!use_menu) screen_save();

					prt("", y++, x);

					/* Print list */
					for (ctr = 0; ctr < num; ctr++)
					{
						/* Letter/number for power selection */
						if (use_menu)
							sprintf(buf, "%c%s ", (ctr == mode) ? '*' : ' ', (ctr == (menu_line - 1)) ? _("�t", "> ") : "  ");
						else
							sprintf(buf, "%c%c) ", (ctr == mode) ? '*' : ' ', I2A(ctr));

						strcat(buf, power_desc[ctr]);

						prt(buf, y + ctr, x);
					}

					prt("", y + MIN(ctr, 17), x);
				}

				/* Hide the list */
				else
				{
					/* Hide list */
					redraw = FALSE;

					/* Restore the screen */
					screen_load();
				}

				/* Redo asking */
				continue;
			}

			if (!use_menu)
			{
				/* Note verify */
				ask = (isupper(choice));

				/* Lowercase */
				if (ask) choice = (char)tolower(choice);

				/* Extract request */
				i = (islower(choice) ? A2I(choice) : -1);
			}

			/* Totally Illegal */
			if ((i < 0) || (i >= num))
			{
				bell();
				continue;
			}

			/* Verify it */
			if (ask)
			{
				/* Prompt */
				strnfmt(buf, 78, _("%s���g���܂����H ", "Use %s? "), power_desc[i]);

				/* Belay that order */
				if (!get_check(buf)) continue;
			}

			/* Stop the loop */
			flag = TRUE;
		}

		/* Restore the screen */
		if (redraw) screen_load();

		/* Abort if needed */
		if (!flag)
		{
			p_ptr->energy_use = 0;
			return;
		}

#ifdef ALLOW_REPEAT
		repeat_push(i);
	}
#endif /* ALLOW_REPEAT */

	switch (powers[i])
	{
	case PET_DISMISS: /* Dismiss pets */
	{
		/* Check pets (backwards) */
		for (pet_ctr = m_max - 1; pet_ctr >= 1; pet_ctr--)
		{
			/* Player has pet */
			if (is_pet(&m_list[pet_ctr])) break;
		}

		if (!pet_ctr)
		{
			msg_print(_("�y�b�g�����Ȃ��I", "You have no pets!"));
			break;
		}
		do_cmd_pet_dismiss();
		(void)calculate_upkeep();
		break;
	}
	case PET_TARGET:
	{
		project_length = -1;
		if (!target_set(TARGET_KILL)) pet_t_m_idx = 0;
		else
		{
			cave_type *c_ptr = &cave[target_row][target_col];
			if (c_ptr->m_idx && (m_list[c_ptr->m_idx].ml))
			{
				pet_t_m_idx = cave[target_row][target_col].m_idx;
				p_ptr->pet_follow_distance = PET_DESTROY_DIST;
			}
			else pet_t_m_idx = 0;
		}
		project_length = 0;

		break;
	}
	/* Call pets */
	case PET_STAY_CLOSE:
	{
		p_ptr->pet_follow_distance = PET_CLOSE_DIST;
		pet_t_m_idx = 0;
		break;
	}
	/* "Follow Me" */
	case PET_FOLLOW_ME:
	{
		p_ptr->pet_follow_distance = PET_FOLLOW_DIST;
		pet_t_m_idx = 0;
		break;
	}
	/* "Seek and destoy" */
	case PET_SEEK_AND_DESTROY:
	{
		p_ptr->pet_follow_distance = PET_DESTROY_DIST;
		break;
	}
	/* "Give me space" */
	case PET_ALLOW_SPACE:
	{
		p_ptr->pet_follow_distance = PET_SPACE_DIST;
		break;
	}
	/* "Stay away" */
	case PET_STAY_AWAY:
	{
		p_ptr->pet_follow_distance = PET_AWAY_DIST;
		break;
	}
	/* flag - allow pets to open doors */
	case PET_OPEN_DOORS:
	{
		if (p_ptr->pet_extra_flags & PF_OPEN_DOORS) p_ptr->pet_extra_flags &= ~(PF_OPEN_DOORS);
		else p_ptr->pet_extra_flags |= (PF_OPEN_DOORS);
		break;
	}
	/* flag - allow pets to pickup items */
	case PET_TAKE_ITEMS:
	{
		if (p_ptr->pet_extra_flags & PF_PICKUP_ITEMS)
		{
			p_ptr->pet_extra_flags &= ~(PF_PICKUP_ITEMS);
			for (pet_ctr = m_max - 1; pet_ctr >= 1; pet_ctr--)
			{
				/* Access the monster */
				m_ptr = &m_list[pet_ctr];

				if (is_pet(m_ptr))
				{
					monster_drop_carried_objects(m_ptr);
				}
			}
		}
		else p_ptr->pet_extra_flags |= (PF_PICKUP_ITEMS);

		break;
	}
	/* flag - allow pets to teleport */
	case PET_TELEPORT:
	{
		if (p_ptr->pet_extra_flags & PF_TELEPORT) p_ptr->pet_extra_flags &= ~(PF_TELEPORT);
		else p_ptr->pet_extra_flags |= (PF_TELEPORT);
		break;
	}
	/* flag - allow pets to cast attack spell */
	case PET_ATTACK_SPELL:
	{
		if (p_ptr->pet_extra_flags & PF_ATTACK_SPELL) p_ptr->pet_extra_flags &= ~(PF_ATTACK_SPELL);
		else p_ptr->pet_extra_flags |= (PF_ATTACK_SPELL);
		break;
	}
	/* flag - allow pets to cast attack spell */
	case PET_SUMMON_SPELL:
	{
		if (p_ptr->pet_extra_flags & PF_SUMMON_SPELL) p_ptr->pet_extra_flags &= ~(PF_SUMMON_SPELL);
		else p_ptr->pet_extra_flags |= (PF_SUMMON_SPELL);
		break;
	}
	/* flag - allow pets to cast attack spell */
	case PET_BALL_SPELL:
	{
		if (p_ptr->pet_extra_flags & PF_BALL_SPELL) p_ptr->pet_extra_flags &= ~(PF_BALL_SPELL);
		else p_ptr->pet_extra_flags |= (PF_BALL_SPELL);
		break;
	}

	case PET_RIDING:
	{
		(void)do_riding(FALSE);
		break;
	}

	case PET_NAME:
	{
		do_name_pet();
		break;
	}

	case PET_RYOUTE:
	{
		if (p_ptr->pet_extra_flags & PF_RYOUTE) p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
		else p_ptr->pet_extra_flags |= (PF_RYOUTE);
		p_ptr->update |= (PU_BONUS);
		handle_stuff();
		break;
	}
	}
}


/*!
* @brief �y�b�g�̑P�������ɉ������ێ��R�X�g�̓r���v�Z����
* @param m_ptr �v�Z��ƂȂ郂���X�^�[�̍\���̎Q�ƃ|�C���^
* @param inc m_ptr�Ŏw�肵�������X�^�[���ێ��R�X�g�v�Z�ɉ�����Ȃ�TRUE�A�O���Ȃ�FALSE���w��
* @return �Ȃ�
*/
void check_pets_num_and_align(monster_type *m_ptr, bool inc)
{
	s32b old_friend_align = friend_align;
	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	if (inc)
	{
		total_friends++;
		if (r_ptr->flags3 & RF3_GOOD) friend_align += r_ptr->level;
		if (r_ptr->flags3 & RF3_EVIL) friend_align -= r_ptr->level;
	}
	else
	{
		total_friends--;
		if (r_ptr->flags3 & RF3_GOOD) friend_align -= r_ptr->level;
		if (r_ptr->flags3 & RF3_EVIL) friend_align += r_ptr->level;
	}

	if (old_friend_align != friend_align) p_ptr->update |= (PU_BONUS);
}



/*!
* @brief �v���C���[�̗��n���菈��
* @param dam ���n����𔭂����ۂɎ󂯂��_���[�W��
* @param force TRUE�Ȃ�΋����I�ɗ��n����
* @return ���ۂɗ��n������TRUE��Ԃ�
*/
bool rakuba(HIT_POINT dam, bool force)
{
	int i, y, x, oy, ox;
	int sn = 0, sy = 0, sx = 0;
	char m_name[80];
	monster_type *m_ptr = &m_list[p_ptr->riding];
	monster_race *r_ptr = &r_info[m_ptr->r_idx];
	bool fall_dam = FALSE;

	if (!p_ptr->riding) return FALSE;
	if (p_ptr->wild_mode) return FALSE;

	if (dam >= 0 || force)
	{
		if (!force)
		{
			int cur = p_ptr->skill_exp[GINOU_RIDING];
			int max = s_info[p_ptr->pclass].s_max[GINOU_RIDING];
			int ridinglevel = r_ptr->level;

			/* ���n�̂��₷�� */
			int rakubalevel = r_ptr->level;
			if (p_ptr->riding_ryoute) rakubalevel += 20;

			if ((cur < max) && (max > 1000) &&
				(dam / 2 + ridinglevel) > (cur / 30 + 10))
			{
				int inc = 0;

				if (ridinglevel > (cur / 100 + 15))
					inc += 1 + (ridinglevel - cur / 100 - 15);
				else
					inc += 1;

				p_ptr->skill_exp[GINOU_RIDING] = MIN(max, cur + inc);
			}

			/* ���x���̒Ⴂ��n����͗��n���ɂ��� */
			if (randint0(dam / 2 + rakubalevel * 2) < cur / 30 + 10)
			{
				if ((((p_ptr->pclass == CLASS_BEASTMASTER) || (p_ptr->pclass == CLASS_CAVALRY)) && !p_ptr->riding_ryoute) || !one_in_(p_ptr->lev*(p_ptr->riding_ryoute ? 2 : 3) + 30))
				{
					return FALSE;
				}
			}
		}

		/* Check around the player */
		for (i = 0; i < 8; i++)
		{
			cave_type *c_ptr;

			/* Access the location */
			y = p_ptr->y + ddy_ddd[i];
			x = p_ptr->x + ddx_ddd[i];

			c_ptr = &cave[y][x];

			if (c_ptr->m_idx) continue;

			/* Skip non-empty grids */
			if (!cave_have_flag_grid(c_ptr, FF_MOVE) && !cave_have_flag_grid(c_ptr, FF_CAN_FLY))
			{
				if (!player_can_ride_aux(c_ptr, FALSE)) continue;
			}

			if (cave_have_flag_grid(c_ptr, FF_PATTERN)) continue;

			/* Count "safe" grids */
			sn++;

			/* Randomize choice */
			if (randint0(sn) > 0) continue;

			/* Save the safe location */
			sy = y; sx = x;
		}
		if (!sn)
		{
			monster_desc(m_name, m_ptr, 0);
#ifdef JP
			msg_format("%s����U�藎�Ƃ��ꂻ���ɂȂ��āA�ǂɂԂ������B", m_name);
			take_hit(DAMAGE_NOESCAPE, r_ptr->level + 3, "�ǂւ̏Փ�", -1);
#else
			msg_format("You have nearly fallen from %s, but bumped into wall.", m_name);
			take_hit(DAMAGE_NOESCAPE, r_ptr->level + 3, "bumping into wall", -1);
#endif
			return FALSE;
		}

		oy = p_ptr->y;
		ox = p_ptr->x;

		p_ptr->y = sy;
		p_ptr->x = sx;

		/* Redraw the old spot */
		lite_spot(oy, ox);

		/* Redraw the new spot */
		lite_spot(p_ptr->y, p_ptr->x);

		/* Check for new panel */
		verify_panel();
	}

	p_ptr->riding = 0;
	p_ptr->pet_extra_flags &= ~(PF_RYOUTE);
	p_ptr->riding_ryoute = p_ptr->old_riding_ryoute = FALSE;

	calc_bonuses();

	p_ptr->update |= (PU_BONUS);

	/* Update stuff */
	p_ptr->update |= (PU_VIEW | PU_LITE | PU_FLOW | PU_MON_LITE | PU_MONSTERS);

	/* Window stuff */
	p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

	p_ptr->redraw |= (PR_EXTRA);

	/* Update health track of mount */
	p_ptr->redraw |= (PR_UHEALTH);

	if (p_ptr->levitation && !force)
	{
		monster_desc(m_name, m_ptr, 0);
		msg_format(_("%s���痎�������A�󒆂ł��܂��̐��𗧂Ē����Ē��n�����B", "You are thrown from %s, but make a good landing."), m_name);
	}
	else
	{
		take_hit(DAMAGE_NOESCAPE, r_ptr->level + 3, _("���n", "Falling from riding"), -1);
		fall_dam = TRUE;
	}

	/* Move the player */
	if (sy && !p_ptr->is_dead)
		(void)move_player_effect(p_ptr->y, p_ptr->x, MPE_DONT_PICKUP | MPE_DONT_SWAP_MON);

	return fall_dam;
}

