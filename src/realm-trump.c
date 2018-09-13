#include "angband.h"
#include "cmd-spell.h"


/*!
* @brief �g�����v�̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_trump_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	bool fail = (mode == SPELL_FAIL) ? TRUE : FALSE;
	static const char s_random[] = _("�����_��", "random");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�V���[�g�E�e���|�[�g", "Phase Door");
		if (desc) return _("�ߋ����̃e���|�[�g������B", "Teleport short distance.");

		{
			POSITION range = 10;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 1:
		if (name) return _("�w偂̃J�[�h", "Trump Spiders");
		if (desc) return _("�w偂���������B", "Summons spiders.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��͒w偂̃J�[�h�ɏW������...", "You concentrate on the trump of an spider..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_SPIDER, PM_ALLOW_GROUP))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�w偂͓{���Ă���I", "The summoned spiders get angry!"));
					}
				}
			}
		}
		break;

	case 2:
		if (name) return _("�V���b�t��", "Shuffle");
		if (desc) return _("�J�[�h�̐肢������B", "Causes random effects.");

		{
			if (info) return s_random;

			if (cast)
			{
				cast_shuffle();
			}
		}
		break;

	case 3:
		if (name) return _("�t���A�E���Z�b�g", "Reset Recall");
		if (desc) return _("�Ő[�K��ύX����B", "Resets the 'deepest' level for recall spell.");

		{
			if (cast)
			{
				if (!reset_recall()) return NULL;
			}
		}
		break;

	case 4:
		if (name) return _("�e���|�[�g", "Teleport");
		if (desc) return _("�������̃e���|�[�g������B", "Teleport long distance.");

		{
			POSITION range = plev * 4;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 5:
		if (name) return _("���m�̃J�[�h", "Trump Spying");
		if (desc) return _("��莞�ԁA�e���p�V�[�\�͂𓾂�B", "Gives telepathy for a while.");

		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_tim_esp(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 6:
		if (name) return _("�e���|�[�g�E�����X�^�[", "Teleport Away");
		if (desc) return _("�����X�^�[���e���|�[�g������r�[������B��R�����Ɩ����B", "Teleports all monsters on the line away unless resisted.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_AWAY_ALL, dir, power);
			}
		}
		break;

	case 7:
		if (name) return _("�����̃J�[�h", "Trump Animals");
		if (desc) return _("1�̂̓�������������B", "Summons an animal.");

		{
			if (cast || fail)
			{
				int type = (!fail ? SUMMON_ANIMAL_RANGER : SUMMON_ANIMAL);
				msg_print(_("���Ȃ��͓����̃J�[�h�ɏW������...", "You concentrate on the trump of an animal..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, type, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�����͓{���Ă���I", "The summoned animal gets angry!"));
					}
				}
			}
		}
		break;

	case 8:
		if (name) return _("�ړ��̃J�[�h", "Trump Reach");
		if (desc) return _("�A�C�e���������̑����ֈړ�������B", "Pulls a distant item close to you.");

		{
			int weight = plev * 15;

			if (info) return info_weight(weight);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fetch(dir, weight, FALSE);
			}
		}
		break;

	case 9:
		if (name) return _("�J�~�J�[�̃J�[�h", "Trump Kamikaze");
		if (desc) return _("�����̔������郂���X�^�[����������B", "Summons monsters which explode by itself.");

		{
			if (cast || fail)
			{
				int x, y;
				int type;

				if (cast)
				{
					if (!target_set(TARGET_KILL)) return NULL;
					x = target_col;
					y = target_row;
				}
				else
				{
					/* Summons near player when failed */
					x = p_ptr->x;
					y = p_ptr->y;
				}

				if (p_ptr->pclass == CLASS_BEASTMASTER)
					type = SUMMON_KAMIKAZE_LIVING;
				else
					type = SUMMON_KAMIKAZE;

				msg_print(_("���Ȃ��̓J�~�J�[�̃J�[�h�ɏW������...", "You concentrate on several trumps at once..."));
				if (trump_summoning(2 + randint0(plev / 7), !fail, y, x, 0, type, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�����X�^�[�͓{���Ă���I", "The summoned creatures get angry!"));
					}
				}
			}
		}
		break;

	case 10:
		if (name) return _("���쏢��", "Phantasmal Servant");
		if (desc) return _("1�̗̂H�����������B", "Summons a ghost.");

		{
			/* Phantasmal Servant is not summoned as enemy when failed */
			if (cast)
			{
				int summon_lev = plev * 2 / 3 + randint1(plev / 2);

				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, (summon_lev * 3 / 2), SUMMON_PHANTOM, 0L))
				{
					msg_print(_("��p�ł������܂����A���l�l�H", "'Your wish, master?'"));
				}
			}
		}
		break;

	case 11:
		if (name) return _("�X�s�[�h�E�����X�^�[", "Haste Monster");
		if (desc) return _("�����X�^�[1�̂�����������B", "Hastes a monster.");

		{
			if (cast)
			{
				bool result;

				/* Temporary enable target_pet option */
				bool old_target_pet = target_pet;
				target_pet = TRUE;

				result = get_aim_dir(&dir);

				/* Restore target_pet option */
				target_pet = old_target_pet;

				if (!result) return NULL;

				speed_monster(dir, plev);
			}
		}
		break;

	case 12:
		if (name) return _("�e���|�[�g�E���x��", "Teleport Level");
		if (desc) return _("�u���ɏォ���̊K�Ƀe���|�[�g����B", "Teleport to up or down stairs in a moment.");

		{
			if (cast)
			{
				if (!get_check(_("�{���ɑ��̊K�Ƀe���|�[�g���܂����H", "Are you sure? (Teleport Level)"))) return NULL;
				teleport_level(0);
			}
		}
		break;

	case 13:
		if (name) return _("�����̔�", "Dimension Door");
		if (desc) return _("�Z�������̎w�肵���ꏊ�Ƀe���|�[�g����B", "Teleport to given location.");

		{
			POSITION range = plev / 2 + 10;

			if (info) return info_range(range);

			if (cast)
			{
				msg_print(_("�����̔����J�����B�ړI�n��I��ŉ������B", "You open a dimensional gate. Choose a destination."));
				if (!dimension_door()) return NULL;
			}
		}
		break;

	case 14:
		if (name) return _("�A�҂̎���", "Word of Recall");
		if (desc) return _("�n��ɂ���Ƃ��̓_���W�����̍Ő[�K�ցA�_���W�����ɂ���Ƃ��͒n��ւƈړ�����B",
			"Recalls player from dungeon to town, or from town to the deepest level of dungeon.");

		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				if (!word_of_recall()) return NULL;
			}
		}
		break;

	case 15:
		if (name) return _("�����Ǖ�", "Banish");
		if (desc) return _("���E���̑S�Ẵ����X�^�[���e���|�[�g������B��R�����Ɩ����B", "Teleports all monsters in sight away unless resisted.");

		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cast)
			{
				banish_monsters(power);
			}
		}
		break;

	case 16:
		if (name) return _("�ʒu�����̃J�[�h", "Swap Position");
		if (desc) return _("1�̂̃����X�^�[�ƈʒu����������B", "Swap positions of you and a monster.");

		{
			if (cast)
			{
				bool result;

				/* HACK -- No range limit */
				project_length = -1;

				result = get_aim_dir(&dir);

				/* Restore range to default */
				project_length = 0;

				if (!result) return NULL;

				teleport_swap(dir);
			}
		}
		break;

	case 17:
		if (name) return _("�A���f�b�h�̃J�[�h", "Trump Undead");
		if (desc) return _("1�̂̃A���f�b�h����������B", "Summons an undead monster.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��̓A���f�b�h�̃J�[�h�ɏW������...", "You concentrate on the trump of an undead creature..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_UNDEAD, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�A���f�b�h�͓{���Ă���I", "The summoned undead creature gets angry!"));
					}
				}
			}
		}
		break;

	case 18:
		if (name) return _("঒��ނ̃J�[�h", "Trump Reptiles");
		if (desc) return _("1�̂̃q�h������������B", "Summons a hydra.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ���঒��ނ̃J�[�h�ɏW������...", "You concentrate on the trump of a reptile..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_HYDRA, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ঒��ނ͓{���Ă���I", "The summoned reptile gets angry!"));
					}
				}
			}
		}
		break;

	case 19:
		if (name) return _("�����X�^�[�̃J�[�h", "Trump Monsters");
		if (desc) return _("�����̃����X�^�[����������B", "Summons some monsters.");

		{
			if (cast || fail)
			{
				int type;
				msg_print(_("���Ȃ��̓����X�^�[�̃J�[�h�ɏW������...", "You concentrate on several trumps at once..."));
				if (p_ptr->pclass == CLASS_BEASTMASTER)
					type = SUMMON_LIVING;
				else
					type = 0;

				if (trump_summoning((1 + (plev - 15) / 10), !fail, p_ptr->y, p_ptr->x, 0, type, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�����X�^�[�͓{���Ă���I", "The summoned creatures get angry!"));
					}
				}

			}
		}
		break;

	case 20:
		if (name) return _("�n�E���h�̃J�[�h", "Trump Hounds");
		if (desc) return _("1�O���[�v�̃n�E���h����������B", "Summons a group of hounds.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��̓n�E���h�̃J�[�h�ɏW������...", "You concentrate on the trump of a hound..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_HOUND, PM_ALLOW_GROUP))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�n�E���h�͓{���Ă���I", "The summoned hounds get angry!"));
					}
				}
			}
		}
		break;

	case 21:
		if (name) return _("�g�����v�̐n", "Trump Branding");
		if (desc) return _("����Ƀg�����v�̑���������B", "Makes current weapon a Trump weapon.");

		{
			if (cast)
			{
				brand_weapon(5);
			}
		}
		break;

	case 22:
		if (name) return _("�l�ԃg�����v", "Living Trump");
		if (desc) return _("�����_���Ƀe���|�[�g����ˑR�ψق��A�����̈ӎv�Ńe���|�[�g����ˑR�ψق��g�ɂ��B",
			"Gives mutation which makes you teleport randomly or makes you able to teleport at will.");

		{
			if (cast)
			{
				int mutation;

				if (one_in_(7))
					/* Teleport control */
					mutation = 12;
				else
					/* Random teleportation (uncontrolled) */
					mutation = 77;

				/* Gain the mutation */
				if (gain_random_mutation(mutation))
				{
					msg_print(_("���Ȃ��͐����Ă���J�[�h�ɕς�����B", "You have turned into a Living Trump."));
				}
			}
		}
		break;

	case 23:
		if (name) return _("�T�C�o�[�f�[�����̃J�[�h", "Trump Cyberdemon");
		if (desc) return _("1�̂̃T�C�o�[�f�[��������������B", "Summons a cyber demon.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��̓T�C�o�[�f�[�����̃J�[�h�ɏW������...", "You concentrate on the trump of a Cyberdemon..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_CYBER, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�T�C�o�[�f�[�����͓{���Ă���I", "The summoned Cyberdemon gets angry!"));
					}
				}
			}
		}
		break;

	case 24:
		if (name) return _("�\���̃J�[�h", "Trump Divination");
		if (desc) return _("�߂��̑S�Ẵ����X�^�[�A㩁A���A�K�i�A����A�����ăA�C�e�������m����B",
			"Detects all monsters, traps, doors, stairs, treasures and items in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_all(rad);
			}
		}
		break;

	case 25:
		if (name) return _("�m���̃J�[�h", "Trump Lore");
		if (desc) return _("�A�C�e���̎��\�͂����S�ɒm��B", "*Identifies* an item.");

		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 26:
		if (name) return _("�񕜃����X�^�[", "Heal Monster");
		if (desc) return _("�����X�^�[1�̗̂̑͂��񕜂�����B", "Heal a monster.");

		{
			int heal = plev * 10 + 200;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				bool result;

				/* Temporary enable target_pet option */
				bool old_target_pet = target_pet;
				target_pet = TRUE;

				result = get_aim_dir(&dir);

				/* Restore target_pet option */
				target_pet = old_target_pet;

				if (!result) return NULL;

				heal_monster(dir, heal);
			}
		}
		break;

	case 27:
		if (name) return _("�h���S���̃J�[�h", "Trump Dragon");
		if (desc) return _("1�̂̃h���S������������B", "Summons a dragon.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��̓h���S���̃J�[�h�ɏW������...", "You concentrate on the trump of a dragon..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_DRAGON, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�h���S���͓{���Ă���I", "The summoned dragon gets angry!"));
					}
				}
			}
		}
		break;

	case 28:
		if (name) return _("覐΂̃J�[�h", "Trump Meteor");
		if (desc) return _("�����̎��ӂ�覐΂𗎂Ƃ��B", "Makes meteor balls fall down to nearby random locations.");

		{
			HIT_POINT dam = plev * 2;
			int rad = 2;

			if (info) return info_multi_damage(dam);

			if (cast)
			{
				cast_meteor(dam, rad);
			}
		}
		break;

	case 29:
		if (name) return _("�f�[�����̃J�[�h", "Trump Demon");
		if (desc) return _("1�̂̈�������������B", "Summons a demon.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��̓f�[�����̃J�[�h�ɏW������...", "You concentrate on the trump of a demon..."));
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_DEMON, 0L))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�f�[�����͓{���Ă���I", "The summoned demon gets angry!"));
					}
				}
			}
		}
		break;

	case 30:
		if (name) return _("�n���̃J�[�h", "Trump Greater Undead");
		if (desc) return _("1�̂̏㋉�A���f�b�h����������B", "Summons a greater undead.");

		{
			if (cast || fail)
			{
				msg_print(_("���Ȃ��͋��͂ȃA���f�b�h�̃J�[�h�ɏW������...", "You concentrate on the trump of a greater undead being..."));
				/* May allow unique depend on level and dice roll */
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, SUMMON_HI_UNDEAD, PM_ALLOW_UNIQUE))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�㋉�A���f�b�h�͓{���Ă���I", "The summoned greater undead creature gets angry!"));
					}
				}
			}
		}
		break;

	case 31:
		if (name) return _("�Ñ�h���S���̃J�[�h", "Trump Ancient Dragon");
		if (desc) return _("1�̂̌Ñ�h���S������������B", "Summons an ancient dragon.");

		{
			if (cast)
			{
				int type;

				if (p_ptr->pclass == CLASS_BEASTMASTER)
					type = SUMMON_HI_DRAGON_LIVING;
				else
					type = SUMMON_HI_DRAGON;

				msg_print(_("���Ȃ��͌Ñ�h���S���̃J�[�h�ɏW������...", "You concentrate on the trump of an ancient dragon..."));
				/* May allow unique depend on level and dice roll */
				if (trump_summoning(1, !fail, p_ptr->y, p_ptr->x, 0, type, PM_ALLOW_UNIQUE))
				{
					if (fail)
					{
						msg_print(_("�������ꂽ�Ñ�h���S���͓{���Ă���I", "The summoned ancient dragon gets angry!"));
					}
				}
			}
		}
		break;
	}

	return "";
}
