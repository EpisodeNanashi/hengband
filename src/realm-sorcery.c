#include "angband.h"
#include "cmd-spell.h"
#include "selfinfo.h"

/*!
* @brief ��p�̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_sorcery_spell(SPELL_IDX spell, BIT_FLAGS mode)
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

	case 1:
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

	case 2:
		if (name) return _("㩂Ɣ����m", "Detect Doors and Traps");
		if (desc) return _("�߂��̑S�Ă̔���㩂����m����B", "Detects traps, doors, and stairs in your vicinity.");

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

	case 3:
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

	case 4:
		if (name) return _("�p�j�b�N�E�����X�^�[", "Confuse Monster");
		if (desc) return _("�����X�^�[1�̂�����������B��R�����Ɩ����B", "Attempts to confuse a monster.");

		{
			int power = (plev * 3) / 2;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				confuse_monster(dir, power);
			}
		}
		break;

	case 5:
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

	case 6:
		if (name) return _("�X���[�v�E�����X�^�[", "Sleep Monster");
		if (desc) return _("�����X�^�[1�̂𖰂点��B��R�����Ɩ����B", "Attempts to sleep a monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				sleep_monster(dir, plev);
			}
		}
		break;

	case 7:
		if (name) return _("���͏[�U", "Recharging");
		if (desc) return _("��/���@�_�̏[�U�񐔂𑝂₷���A�[�U���̃��b�h�̏[�U���Ԃ����炷�B", "Recharges staffs, wands or rods.");

		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cast)
			{
				if (!recharge(power)) return NULL;
			}
		}
		break;

	case 8:
		if (name) return _("���@�̒n�}", "Magic Mapping");
		if (desc) return _("���ӂ̒n�`�����m����B", "Maps nearby area.");

		{
			int rad = DETECT_RAD_MAP;

			if (info) return info_radius(rad);

			if (cast)
			{
				map_area(rad);
			}
		}
		break;

	case 9:
		if (name) return _("�Ӓ�", "Identify");
		if (desc) return _("�A�C�e�������ʂ���B", "Identifies an item.");

		{
			if (cast)
			{
				if (!ident_spell(FALSE)) return NULL;
			}
		}
		break;

	case 10:
		if (name) return _("�X���E�E�����X�^�[", "Slow Monster");
		if (desc) return _("�����X�^�[1�̂���������B��R�����Ɩ����B", "Attempts to slow a monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				slow_monster(dir, plev);
			}
		}
		break;

	case 11:
		if (name) return _("���ӃX���[�v", "Mass Sleep");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�𖰂点��B��R�����Ɩ����B", "Attempts to sleep all monsters in sight.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				sleep_monsters(plev);
			}
		}
		break;

	case 12:
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

	case 13:
		if (name) return _("�X�s�[�h", "Haste Self");
		if (desc) return _("��莞�ԁA��������B", "Hastes you for a while.");

		{
			int base = plev;
			int sides = 20 + plev;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_fast(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 14:
		if (name) return _("�^�E���m", "Detection True");
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

	case 15:
		if (name) return _("�^�E�Ӓ�", "Identify True");
		if (desc) return _("�A�C�e���̎��\�͂����S�ɒm��B", "*Identifies* an item.");

		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 16:
		if (name) return _("���̂ƍ��󊴒m", "Detect items and Treasure");
		if (desc) return _("�߂��̑S�ẴA�C�e���ƍ�������m����B", "Detects all treasures and items in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_objects_normal(rad);
				detect_treasure(rad);
				detect_objects_gold(rad);
			}
		}
		break;

	case 17:
		if (name) return _("�`���[���E�����X�^�[", "Charm Monster");
		if (desc) return _("�����X�^�[1�̂𖣗�����B��R�����Ɩ����B", "Attempts to charm a monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				charm_monster(dir, power);
			}
		}
		break;

	case 18:
		if (name) return _("���_���m", "Sense Minds");
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

	case 19:
		if (name) return _("�X�ړ�", "Teleport to town");
		if (desc) return _("�X�ֈړ�����B�n��ɂ���Ƃ������g���Ȃ��B", "Teleport to a town which you choose in a moment. Can only be used outdoors.");

		{
			if (cast)
			{
				if (!tele_town()) return NULL;
			}
		}
		break;

	case 20:
		if (name) return _("���ȕ���", "Self Knowledge");
		if (desc) return _("���݂̎����̏�Ԃ����S�ɒm��B",
			"Gives you useful info regarding your current resistances, the powers of your weapon and maximum limits of your stats.");

		{
			if (cast)
			{
				self_knowledge();
			}
		}
		break;

	case 21:
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

	case 22:
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

	case 23:
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

	case 24:
		if (name) return _("����", "Probing");
		if (desc) return _("�����X�^�[�̑����A�c��̗́A�ő�̗́A�X�s�[�h�A���̂�m��B",
			"Proves all monsters' alignment, HP, speed and their true character.");

		{
			if (cast)
			{
				probing();
			}
		}
		break;

	case 25:
		if (name) return _("�����̃��[��", "Explosive Rune");
		if (desc) return _("�����̂��鏰�̏�ɁA�����X�^�[���ʂ�Ɣ������ă_���[�W��^���郋�[����`���B",
			"Sets a glyph under you. The glyph will explode when a monster moves on it.");

		{
			int dice = 7;
			int sides = 7;
			int base = plev;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				explosive_rune();
			}
		}
		break;

	case 26:
		if (name) return _("�O����", "Telekinesis");
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

	case 27:
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

	case 28:
		if (name) return _("�����̎���", "Charm monsters");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�𖣗�����B��R�����Ɩ����B", "Attempts to charm all monsters in sight.");

		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				charm_monsters(power);
			}
		}
		break;

	case 29:
		if (name) return _("�B���p", "Alchemy");
		if (desc) return _("�A�C�e��1�������ɕς���B", "Turns an item into 1/3 of its value in gold.");

		{
			if (cast)
			{
				if (!alchemy()) return NULL;
			}
		}
		break;

	case 30:
		if (name) return _("�����Ǖ�", "Banishment");
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

	case 31:
		if (name) return _("�����̋�", "Globe of Invulnerability");
		if (desc) return _("��莞�ԁA�_���[�W���󂯂Ȃ��Ȃ�o���A�𒣂�B�؂ꂽ�u�Ԃɏ����^�[���������̂Œ��ӁB",
			"Generates barrier which completely protect you from almost all damages. Takes a few your turns when the barrier breaks or duration time is exceeded.");

		{
			int base = 4;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_invuln(randint1(base) + base, FALSE);
			}
		}
		break;
	}

	return "";
}
