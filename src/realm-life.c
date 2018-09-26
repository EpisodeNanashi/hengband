#include "angband.h"
#include "cmd-spell.h"

/*!
* @brief �����̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_life_spell(SPELL_IDX spell, BIT_FLAGS mode)
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
		if (name) return _("�y���̎���", "Cure Light Wounds");
		if (desc) return _("����Ƒ̗͂������񕜂�����B", "Heals cut and HP a little.");
		{
			int dice = 2;
			int sides = 10;
			if (info) return info_heal(dice, sides, 0);
			if (cast) (void)cure_light_wounds(dice, sides);			
		}
		break;

	case 1:
		if (name) return _("�j��", "Bless");
		if (desc) return _("��莞�ԁA��������AC�Ƀ{�[�i�X�𓾂�B", "Gives bonus to hit and AC for a few turns.");
		{
			int base = 12;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_blessed(randint1(base) + base, FALSE);
			}
		}
		break;

	case 2:
		if (name) return _("�y��", "Cause Light Wounds");
		if (desc) return _("1�̂̃����X�^�[�ɏ��_���[�W��^����B��R�����Ɩ����B", "Wounds a monster a little unless resisted.");
		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball_hide(GF_WOUNDS, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 3:
		if (name) return _("���̏���", "Call Light");
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
		if (name) return _("� & �B�������m", "Detect Doors & Traps");
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

	case 5:
		if (name) return _("�d���̎���", "Cure Medium Wounds");
		if (desc) return _("����Ƒ̗͂𒆒��x�񕜂�����B", "Heals cut and HP more.");
		{
			int dice = 4;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);
			if (cast) (void)cure_serious_wounds(dice, sides);
		}
		break;

	case 6:
		if (name) return _("���", "Cure Poison");
		if (desc) return _("�̓��̓ł���菜���B", "Cure poison status.");
		{
			if (cast)
			{
				set_poisoned(0);
			}
		}
		break;

	case 7:
		if (name) return _("�󕠏[��", "Satisfy Hunger");
		if (desc) return _("�����ɂ���B", "Satisfies hunger.");
		{
			if (cast)
			{
				set_food(PY_FOOD_MAX - 1);
			}
		}
		break;

	case 8:
		if (name) return _("����", "Remove Curse");
		if (desc) return _("�A�C�e���ɂ��������ア�􂢂���������B", "Removes normal curses from equipped items.");
		{
			if (cast)
			{
				if (remove_curse())
				{
					msg_print(_("�N���Ɍ�����Ă���悤�ȋC������B", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 9:
		if (name) return _("�d��", "Cause Medium Wounds");
		if (desc) return _("1�̂̃����X�^�[�ɒ��_���[�W��^����B��R�����Ɩ����B", "Wounds a monster unless resisted.");
		{
			int sides = 8 + (plev - 5) / 4;
			int dice = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball_hide(GF_WOUNDS, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 10:
		if (name) return _("�v�����̎���", "Cure Critical Wounds");
		if (desc) return _("�̗͂�啝�ɉ񕜂����A�����ƞN�O��Ԃ��S������B", "Heals cut, stun and HP greatly.");
		{
			int dice = 8;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 11:
		if (name) return _("�ϔM�ϊ�", "Resist Heat and Cold");
		if (desc) return _("��莞�ԁA�Ή��Ɨ�C�ɑ΂���ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to fire and cold. These resistances can be added to which from equipment for more powerful resistances.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 12:
		if (name) return _("���ӊ��m", "Sense Surroundings");
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

	case 13:
		if (name) return _("�p�j�b�N�E�A���f�b�h", "Turn Undead");
		if (desc) return _("���E���̃A���f�b�h�����|������B��R�����Ɩ����B", "Attempts to scare undead monsters in sight.");

		{
			if (cast)
			{
				turn_undead();
			}
		}
		break;

	case 14:
		if (name) return _("�̗͉�", "Healing");
		if (desc) return _("�ɂ߂ċ��͂ȉ񕜎����ŁA�����ƞN�O��Ԃ��S������B", "Much powerful healing magic, and heals cut and stun completely.");

		{
			int heal = 300;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				hp_player(heal);
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 15:
		if (name) return _("���E�̖��", "Glyph of Warding");
		if (desc) return _("�����̂��鏰�̏�ɁA�����X�^�[���ʂ蔲�����菢�����ꂽ�肷�邱�Ƃ��ł��Ȃ��Ȃ郋�[����`���B",
			"Sets a glyph on the floor beneath you. Monsters cannot attack you if you are on a glyph, but can try to break glyph.");

		{
			if (cast)
			{
				warding_glyph();
			}
		}
		break;

	case 16:
		if (name) return _("*����*", "Dispel Curse");
		if (desc) return _("�A�C�e���ɂ����������͂Ȏ􂢂���������B", "Removes normal and heavy curse from equipped items.");

		{
			if (cast)
			{
				if (remove_all_curse())
				{
					msg_print(_("�N���Ɍ�����Ă���悤�ȋC������B", "You feel as if someone is watching over you."));
				}
			}
		}
		break;

	case 17:
		if (name) return _("�ӎ�", "Perception");
		if (desc) return _("�A�C�e�������ʂ���B", "Identifies an item.");

		{
			if (cast)
			{
				if (!ident_spell(FALSE)) return NULL;
			}
		}
		break;

	case 18:
		if (name) return _("�A���f�b�h�ގU", "Dispel Undead");
		if (desc) return _("���E���̑S�ẴA���f�b�h�Ƀ_���[�W��^����B", "Damages all undead monsters in sight.");

		{
			int dice = 1;
			int sides = plev * 5;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				dispel_undead(damroll(dice, sides));
			}
		}
		break;

	case 19:
		if (name) return _("��̍�", "Day of the Dove");
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

	case 20:
		if (name) return _("�v����", "Cause Critical Wounds");
		if (desc) return _("1�̂̃����X�^�[�ɑ�_���[�W��^����B��R�����Ɩ����B", "Wounds a monster critically unless resisted.");

		{
			int dice = 5 + (plev - 5) / 3;
			int sides = 15;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball_hide(GF_WOUNDS, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 21:
		if (name) return _("�A�҂̏�", "Word of Recall");
		if (desc) return _("�n��ɂ���Ƃ��̓_���W�����̍Ő[�K�ցA�_���W�����ɂ���Ƃ��͒n��ւƈړ�����B", "Recalls player from dungeon to town, or from town to the deepest level of dungeon.");

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

	case 22:
		if (name) return _("�^���̍Ւd", "Alter Reality");
		if (desc) return _("���݂̊K���č\������B", "Recreates current dungeon level.");

		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			if (cast)
			{
				alter_reality();
			}
		}
		break;

	case 23:
		if (name) return _("�^�E���E", "Warding True");
		if (desc) return _("�����̂��鏰�Ǝ���8�}�X�̏��̏�ɁA�����X�^�[���ʂ蔲�����菢�����ꂽ�肷�邱�Ƃ��ł��Ȃ��Ȃ郋�[����`���B", "Creates glyphs in all adjacent squares and under you.");

		{
			int rad = 1;

			if (info) return info_radius(rad);

			if (cast)
			{
				warding_glyph();
				glyph_creation();
			}
		}
		break;

	case 24:
		if (name) return _("�s�щ�", "Sterilization");
		if (desc) return _("���̊K�̑��B���郂���X�^�[�����B�ł��Ȃ��Ȃ�B", "Prevents any breeders on current level from breeding.");

		{
			if (cast)
			{
				num_repro += MAX_REPRO;
			}
		}
		break;

	case 25:
		if (name) return _("�S���m", "Detection");
		if (desc) return _("�߂��̑S�Ẵ����X�^�[�A㩁A���A�K�i�A����A�����ăA�C�e�������m����B", "Detects all monsters, traps, doors, stairs, treasures and items in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_all(rad);
			}
		}
		break;

	case 26:
		if (name) return _("�A���f�b�h����", "Annihilate Undead");
		if (desc) return _("�����̎��͂ɂ���A���f�b�h�����݂̊K�����������B��R�����Ɩ����B",
			"Eliminates all nearby undead monsters, exhausting you.  Powerful or unique monsters may be able to resist.");

		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				mass_genocide_undead(power, TRUE);
			}
		}
		break;

	case 27:
		if (name) return _("�痢��", "Clairvoyance");
		if (desc) return _("���̊K�S�̂��i�v�ɏƂ炵�A�_���W���������ׂẴA�C�e�������m����B", "Maps and lights whole dungeon level. Knows all objects location. And gives telepathy for a while.");

		{
			if (cast)
			{
				wiz_lite(FALSE);
			}
		}
		break;

	case 28:
		if (name) return _("�S����", "Restoration");
		if (desc) return _("���ׂẴX�e�[�^�X�ƌo���l���񕜂���B", "Restores all stats and experience.");

		{
			if (cast)
			{
				do_res_stat(A_STR);
				do_res_stat(A_INT);
				do_res_stat(A_WIS);
				do_res_stat(A_DEX);
				do_res_stat(A_CON);
				do_res_stat(A_CHR);
				restore_level();
			}
		}
		break;

	case 29:
		if (name) return _("*�̗͉�*", "Healing True");
		if (desc) return _("�ŋ��̎����̖��@�ŁA�����ƞN�O��Ԃ��S������B", "The greatest healing magic. Heals all HP, cut and stun.");

		{
			int heal = 2000;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				hp_player(heal);
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 30:
		if (name) return _("���Ȃ�r�W����", "Holy Vision");
		if (desc) return _("�A�C�e���̎��\�͂����S�ɒm��B", "*Identifies* an item.");

		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 31:
		if (name) return _("���ɂ̑ϐ�", "Ultimate Resistance");
		if (desc) return _("��莞�ԁA������ϐ���t���AAC�Ɩ��@�h��\�͂��㏸������B", "Gives ultimate resistance, bonus to AC and speed.");

		{
			TIME_EFFECT base = (TIME_EFFECT)plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				TIME_EFFECT v = randint1(base) + base;
				set_fast(v, FALSE);
				set_oppose_acid(v, FALSE);
				set_oppose_elec(v, FALSE);
				set_oppose_fire(v, FALSE);
				set_oppose_cold(v, FALSE);
				set_oppose_pois(v, FALSE);
				set_ultimate_res(v, FALSE);
			}
		}
		break;
	}

	return "";
}
