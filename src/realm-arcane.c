#include "angband.h"
#include "cmd-spell.h"

/*!
* @brief ��p�̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_arcane_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�d��", "Zap");
		if (desc) return _("�d���̃{���g�������̓r�[������B", "Fires a bolt or beam of lightning.");

		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 3;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("���@�̎{��", "Wizard Lock");
		if (desc) return _("���Ɍ���������B", "Locks a door.");

		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				wizard_lock(dir);
			}
		}
		break;

	case 2:
		if (name) return _("�����̊��m", "Detect Invisibility");
		if (desc) return _("�߂��̓����ȃ����X�^�[�����m����B", "Detects all invisible monsters in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_invis(rad);
			}
		}
		break;

	case 3:
		if (name) return _("�����X�^�[���m", "Detect Monsters");
		if (desc) return _("�߂��̑S�Ă̌����郂���X�^�[�����m����B", "Detects all monsters in your vicinity unless invisible.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_normal(rad);
			}
		}
		break;

	case 4:
		if (name) return _("�V���[�g�E�e���|�[�g", "Blink");
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

	case 5:
		if (name) return _("���C�g�E�G���A", "Light Area");
		if (desc) return _("�������Ƃ炵�Ă���͈͂������S�̂��i�v�ɖ��邭����B", "Lights up nearby area and the inside of a room permanently.");

		{
			int dice = 2;
			int sides = plev / 2;
			int rad = plev / 10 + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 6:
		if (name) return _("㩂Ɣ� �j��", "Trap & Door Destruction");
		if (desc) return _("�꒼����̑S�Ă�㩂Ɣ���j�󂷂�B", "Fires a beam which destroy traps and doors.");

		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				destroy_door(dir);
			}
		}
		break;

	case 7:
		if (name) return _("�y���̎���", "Cure Light Wounds");
		if (desc) return _("����Ƒ̗͂������񕜂�����B", "Heals cut and HP a little.");

		{
			int dice = 2;
			int sides = 8;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut(p_ptr->cut - 10);
			}
		}
		break;

	case 8:
		if (name) return _("㩂Ɣ� ���m", "Detect Doors & Traps");
		if (desc) return _("�߂��̑S�Ă�㩂Ɣ��ƊK�i�����m����B", "Detects traps, doors, and stairs in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);
			}
		}
		break;

	case 9:
		if (name) return _("�R�f", "Phlogiston");
		if (desc) return _("�����ɔR����⋋����B", "Adds more turns of light to a lantern or torch.");

		{
			if (cast)
			{
				phlogiston();
			}
		}
		break;

	case 10:
		if (name) return _("���󊴒m", "Detect Treasure");
		if (desc) return _("�߂��̍�������m����B", "Detects all treasures in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_treasure(rad);
				detect_objects_gold(rad);
			}
		}
		break;

	case 11:
		if (name) return _("���@ ���m", "Detect Enchantment");
		if (desc) return _("�߂��̖��@�����������A�C�e�������m����B", "Detects all magical items in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_objects_magic(rad);
			}
		}
		break;

	case 12:
		if (name) return _("�A�C�e�����m", "Detect Objects");
		if (desc) return _("�߂��̑S�ẴA�C�e�������m����B", "Detects all items in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_objects_normal(rad);
			}
		}
		break;

	case 13:
		if (name) return _("���", "Cure Poison");
		if (desc) return _("�ł�̓����犮�S�Ɏ�菜���B", "Cures poison status.");

		{
			if (cast)
			{
				set_poisoned(0);
			}
		}
		break;

	case 14:
		if (name) return _("�ϗ�", "Resist Cold");
		if (desc) return _("��莞�ԁA��C�ւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B", "Gives resistance to cold. This resistance can be added to which from equipment for more powerful resistance.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
			}
		}
		break;

	case 15:
		if (name) return _("�ω�", "Resist Fire");
		if (desc) return _("��莞�ԁA���ւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to fire. This resistance can be added to which from equipment for more powerful resistance.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 16:
		if (name) return _("�ϓd", "Resist Lightning");
		if (desc) return _("��莞�ԁA�d���ւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to electricity. This resistance can be added to which from equipment for more powerful resistance.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_elec(randint1(base) + base, FALSE);
			}
		}
		break;

	case 17:
		if (name) return _("�ώ_", "Resist Acid");
		if (desc) return _("��莞�ԁA�_�ւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to acid. This resistance can be added to which from equipment for more powerful resistance.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_acid(randint1(base) + base, FALSE);
			}
		}
		break;

	case 18:
		if (name) return _("�d���̎���", "Cure Medium Wounds");
		if (desc) return _("����Ƒ̗͂𒆒��x�񕜂�����B", "Heals cut and HP more.");

		{
			int dice = 4;
			int sides = 8;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut((p_ptr->cut / 2) - 50);
			}
		}
		break;

	case 19:
		if (name) return _("�e���|�[�g", "Teleport");
		if (desc) return _("�������̃e���|�[�g������B", "Teleport long distance.");

		{
			POSITION range = plev * 5;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 20:
		if (name) return _("�Ӓ�", "Identify");
		if (desc) return _("�A�C�e�������ʂ���B", "Identifies an item.");

		{
			if (cast)
			{
				if (!ident_spell(FALSE)) return NULL;
			}
		}
		break;

	case 21:
		if (name) return _("��Ηn��", "Stone to Mud");
		if (desc) return _("�ǂ�n�����ď��ɂ���B", "Turns one rock square to mud.");

		{
			int dice = 1;
			int sides = 30;
			int base = 20;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				wall_to_mud(dir, 20 + randint1(30));
			}
		}
		break;

	case 22:
		if (name) return _("�M��", "Ray of Light");
		if (desc) return _("��������B��������������X�^�[�Ɍ��ʂ�����B", "Fires a beam of light which damages to light-sensitive monsters.");

		{
			int dice = 6;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				msg_print(_("�����������ꂽ�B", "A line of light appears."));
				lite_line(dir, damroll(6, 8));
			}
		}
		break;

	case 23:
		if (name) return _("�󕠏[��", "Satisfy Hunger");
		if (desc) return _("�����ɂ���B", "Satisfies hunger.");

		{
			if (cast)
			{
				set_food(PY_FOOD_MAX - 1);
			}
		}
		break;

	case 24:
		if (name) return _("�������F", "See Invisible");
		if (desc) return _("��莞�ԁA�����Ȃ��̂�������悤�ɂȂ�B", "Gives see invisible for a while.");

		{
			int base = 24;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_invis(randint1(base) + base, FALSE);
			}
		}
		break;

	case 25:
		if (name) return _("�G�������^������", "Conjure Elemental");
		if (desc) return _("1�̂̃G�������^������������B", "Summons an elemental.");

		{
			if (cast)
			{
				if (!summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_ELEMENTAL, (PM_ALLOW_GROUP | PM_FORCE_PET)))
				{
					msg_print(_("�G�������^���͌���Ȃ������B", "No Elementals arrive."));
				}
			}
		}
		break;

	case 26:
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

	case 27:
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

	case 28:
		if (name) return _("���f�̋�", "Elemental Ball");
		if (desc) return _("���A�d���A��C�A�_�̂ǂꂩ�̋�����B", "Fires a ball of some elements.");

		{
			HIT_POINT dam = 75 + plev;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				int type;

				if (!get_aim_dir(&dir)) return NULL;

				switch (randint1(4))
				{
				case 1:  type = GF_FIRE; break;
				case 2:  type = GF_ELEC; break;
				case 3:  type = GF_COLD; break;
				default: type = GF_ACID; break;
				}

				fire_ball(type, dir, dam, rad);
			}
		}
		break;

	case 29:
		if (name) return _("�S���m", "Detection");
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

	case 30:
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

	case 31:
		if (name) return _("�痢��", "Clairvoyance");
		if (desc) return _("���̊K�S�̂��i�v�ɏƂ炵�A�_���W���������ׂẴA�C�e�������m����B����ɁA��莞�ԃe���p�V�[�\�͂𓾂�B",
			"Maps and lights whole dungeon level. Knows all objects location. And gives telepathy for a while.");

		{
			int base = 25;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				chg_virtue(V_KNOWLEDGE, 1);
				chg_virtue(V_ENLIGHTEN, 1);

				wiz_lite(FALSE);

				if (!p_ptr->telepathy)
				{
					set_tim_esp(randint1(sides) + base, FALSE);
				}
			}
		}
		break;
	}

	return "";
}
