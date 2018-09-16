#include "angband.h"
#include "cmd-spell.h"



/*!
* @brief �j�ח̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_crusade_spell(SPELL_IDX spell, BIT_FLAGS mode)
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
		if (name) return _("����", "Punishment");
		if (desc) return _("�d���̃{���g�������̓r�[������B", "Fires a bolt or beam of lightning.");

		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("�׈����݊��m", "Detect Evil");
		if (desc) return _("�߂��̎׈��ȃ����X�^�[�����m����B", "Detects all evil monsters in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_evil(rad);
			}
		}
		break;

	case 2:
		if (name) return _("���|����", "Remove Fear");
		if (desc) return _("���|����菜���B", "Removes fear.");

		{
			if (cast)
			{
				set_afraid(0);
			}
		}
		break;

	case 3:
		if (name) return _("�Ј�", "Scare Monster");
		if (desc) return _("�����X�^�[1�̂����|������B��R�����Ɩ����B", "Attempts to scare a monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fear_monster(dir, power);
			}
		}
		break;

	case 4:
		if (name) return _("����", "Sanctuary");
		if (desc) return _("�אڂ����S�Ẵ����X�^�[�𖰂点��B��R�����Ɩ����B", "Attempts to sleep monsters in the adjacent squares.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				sleep_monsters_touch();
			}
		}
		break;

	case 5:
		if (name) return _("����", "Portal");
		if (desc) return _("�������̃e���|�[�g������B", "Teleport medium distance.");

		{
			POSITION range = 25 + plev / 2;

			if (info) return info_range(range);

			if (cast)
			{
				teleport_player(range, 0L);
			}
		}
		break;

	case 6:
		if (name) return _("�X�^�[�_�X�g", "Star Dust");
		if (desc) return _("�^�[�Q�b�g�t�߂ɑM���̃{���g��A�˂���B", "Fires many bolts of light near the target.");

		{
			int dice = 3 + (plev - 1) / 9;
			int sides = 2;

			if (info) return info_multi_damage_dice(dice, sides);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_blast(GF_LITE, dir, dice, sides, 10, 3);
			}
		}
		break;

	case 7:
		if (name) return _("�g�̏�", "Purify");
		if (desc) return _("���A�ŁA�N�O����S������B", "Heals all cut, stun and poison status.");

		{
			if (cast)
			{
				set_cut(0);
				set_poisoned(0);
				set_stun(0);
			}
		}
		break;

	case 8:
		if (name) return _("�׈���΂�", "Scatter Evil");
		if (desc) return _("�׈��ȃ����X�^�[1�̂��e���|�[�g������B��R�����Ɩ����B", "Attempts to teleport an evil monster away.");

		{
			int power = MAX_SIGHT * 5;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_AWAY_EVIL, dir, power, 0);
			}
		}
		break;

	case 9:
		if (name) return _("���Ȃ����", "Holy Orb");
		if (desc) return _("���Ȃ�͂���������B�׈��ȃ����X�^�[�ɑ΂��đ傫�ȃ_���[�W��^���邪�A�P�ǂȃ����X�^�[�ɂ͌��ʂ��Ȃ��B",
			"Fires a ball with holy power. Hurts evil monsters greatly, but don't effect good monsters.");

		{
			int dice = 3;
			int sides = 6;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (p_ptr->pclass == CLASS_PRIEST ||
				p_ptr->pclass == CLASS_HIGH_MAGE ||
				p_ptr->pclass == CLASS_SORCERER)
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HOLY_FIRE, dir, damroll(dice, sides) + base, rad);
			}
		}
		break;

	case 10:
		if (name) return _("��������", "Exorcism");
		if (desc) return _("���E���̑S�ẴA���f�b�h�y�ш����Ƀ_���[�W��^���A�׈��ȃ����X�^�[�����|������B",
			"Damages all undead and demons in sight, and scares all evil monsters in sight.");

		{
			int sides = plev;
			int power = plev;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_undead(randint1(sides));
				dispel_demons(randint1(sides));
				turn_evil(power);
			}
		}
		break;

	case 11:
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

	case 12:
		if (name) return _("�������F", "Sense Unseen");
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

	case 13:
		if (name) return _("�Ύ׈����E", "Protection from Evil");
		if (desc) return _("�׈��ȃ����X�^�[�̍U����h���o���A�𒣂�B", "Gives aura which protect you from evil monster's physical attack.");

		{
			int base = 25;
			int sides = 3 * plev;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_protevil(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 14:
		if (name) return _("�ق��̗�", "Judgment Thunder");
		if (desc) return _("���͂ȓd���̃{���g����B", "Fires a powerful bolt of lightning.");

		{
			HIT_POINT dam = plev * 5;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_bolt(GF_ELEC, dir, dam);
			}
		}
		break;

	case 15:
		if (name) return _("���Ȃ�䌾�t", "Holy Word");
		if (desc) return _("���E���̎׈��ȑ��݂ɑ傫�ȃ_���[�W��^���A�̗͂��񕜂��A�ŁA���|�A�N�O��ԁA��������S������B",
			"Damages all evil monsters in sight, heals HP somewhat, and completely heals poison, fear, stun and cut status.");

		{
			int dam_sides = plev * 6;
			int heal = 100;

			if (info) return format(_("��:1d%d/��%d", "dam:d%d/h%d"), dam_sides, heal);
			if (cast)
			{
				dispel_evil(randint1(dam_sides));
				hp_player(heal);
				set_afraid(0);
				set_poisoned(0);
				set_stun(0);
				set_cut(0);
			}
		}
		break;

	case 16:
		if (name) return _("�J���ꂽ��", "Unbarring Ways");
		if (desc) return _("�꒼����̑S�Ă�㩂Ɣ���j�󂷂�B", "Fires a beam which destroy traps and doors.");

		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				destroy_door(dir);
			}
		}
		break;

	case 17:
		if (name) return _("����", "Arrest");
		if (desc) return _("�׈��ȃ����X�^�[�̓������~�߂�B", "Attempts to paralyze an evil monster.");

		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				stasis_evil(dir);
			}
		}
		break;

	case 18:
		if (name) return _("���Ȃ�I�[��", "Holy Aura");
		if (desc) return _("��莞�ԁA�׈��ȃ����X�^�[�������鐹�Ȃ�I�[���𓾂�B",
			"Gives aura of holy power which injures evil monsters which attacked you for a while.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_sh_holy(randint1(base) + base, FALSE);
			}
		}
		break;

	case 19:
		if (name) return _("�A���f�b�h&�����ގU", "Dispel Undead & Demons");
		if (desc) return _("���E���̑S�ẴA���f�b�h�y�ш����Ƀ_���[�W��^����B", "Damages all undead and demons in sight.");

		{
			int sides = plev * 4;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_undead(randint1(sides));
				dispel_demons(randint1(sides));
			}
		}
		break;

	case 20:
		if (name) return _("�׈��ގU", "Dispel Evil");
		if (desc) return _("���E���̑S�Ă̎׈��ȃ����X�^�[�Ƀ_���[�W��^����B", "Damages all evil monsters in sight.");

		{
			int sides = plev * 4;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_evil(randint1(sides));
			}
		}
		break;

	case 21:
		if (name) return _("���Ȃ�n", "Holy Blade");
		if (desc) return _("�ʏ�̕���ɖŎׂ̑���������B", "Makes current weapon especially deadly against evil monsters.");

		{
			if (cast)
			{
				brand_weapon(13);
			}
		}
		break;

	case 22:
		if (name) return _("�X�^�[�o�[�X�g", "Star Burst");
		if (desc) return _("����ȑM���̋�����B", "Fires a huge ball of powerful light.");

		{
			HIT_POINT dam = 100 + plev * 2;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_LITE, dir, dam, rad);
			}
		}
		break;

	case 23:
		if (name) return _("�V�g����", "Summon Angel");
		if (desc) return _("�V�g��1�̏�������B", "Summons an angel.");

		{
			if (cast)
			{
				bool pet = !one_in_(3);
				u32b flg = 0L;

				if (pet) flg |= PM_FORCE_PET;
				else flg |= PM_NO_PET;
				if (!(pet && (plev < 50))) flg |= PM_ALLOW_GROUP;

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, (plev * 3) / 2, SUMMON_ANGEL, flg))
				{
					if (pet)
					{
						msg_print(_("�u���p�ł������܂����A����l�l�v", "'What is thy bidding... Master?'"));
					}
					else
					{
						msg_print(_("�u��͓��̉��l�ɂ��炸�I ���s�҂�A�������߂�I�v", "Mortal! Repent of thy impiousness."));
					}
				}
			}
		}
		break;

	case 24:
		if (name) return _("�m�C���g", "Heroism");
		if (desc) return _("��莞�ԁA�q�[���[�C���ɂȂ�B", "Removes fear, and gives bonus to hit and 10 more HP for a while.");

		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_hero(randint1(base) + base, FALSE);
				hp_player(10);
				set_afraid(0);
			}
		}
		break;

	case 25:
		if (name) return _("�􂢑ގU", "Dispel Curse");
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

	case 26:
		if (name) return _("�׈��Ǖ�", "Banish Evil");
		if (desc) return _("���E���̑S�Ă̎׈��ȃ����X�^�[���e���|�[�g������B��R�����Ɩ����B",
			"Teleports all evil monsters in sight away unless resisted.");

		{
			int power = 100;

			if (info) return info_power(power);

			if (cast)
			{
				if (banish_evil(power))
				{
					msg_print(_("�_���ȗ͂��׈���ł��������I", "The holy power banishes evil!"));
				}
			}
		}
		break;

	case 27:
		if (name) return _("�n���}�Q�h��", "Armageddon");
		if (desc) return _("���ӂ̃A�C�e���A�����X�^�[�A�n�`��j�󂷂�B", "Destroy everything in nearby area.");

		{
			int base = 12;
			int sides = 4;

			if (cast)
			{
				destroy_area(p_ptr->y, p_ptr->x, base + randint1(sides), FALSE);
			}
		}
		break;

	case 28:
		if (name) return _("�ڂɂ͖ڂ�", "An Eye for an Eye");
		if (desc) return _("��莞�ԁA�������_���[�W���󂯂��Ƃ��ɍU�����s���������X�^�[�ɑ΂��ē����̃_���[�W��^����B",
			"Gives special aura for a while. When you are attacked by a monster, the monster are injured with same amount of damage as you take.");

		{
			int base = 10;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_eyeeye(randint1(base) + base, FALSE);
			}
		}
		break;

	case 29:
		if (name) return _("�_�̓{��", "Wrath of the God");
		if (desc) return _("�^�[�Q�b�g�̎��͂ɕ����̋��𑽐����Ƃ��B", "Drops many balls of disintegration near the target.");

		{
			HIT_POINT dam = plev * 3 + 25;
			int rad = 2;

			if (info) return info_multi_damage(dam);

			if (cast)
			{
				if (!cast_wrath_of_the_god(dam, rad)) return NULL;
			}
		}
		break;

	case 30:
		if (name) return _("�_��", "Divine Intervention");
		if (desc) return _("�אڂ��郂���X�^�[�ɐ��Ȃ�_���[�W��^���A���E���̃����X�^�[�Ƀ_���[�W�A�����A�N�O�A�����A���|�A�����^����B����ɑ̗͂��񕜂���B",
			"Damages all adjacent monsters with holy power. Damages and attempt to slow, stun, confuse, scare and freeze all monsters in sight. And heals HP.");

		{
			int b_dam = plev * 11;
			int d_dam = plev * 4;
			int heal = 100;
			int power = plev * 4;

			if (info) return format(_("��%d/��%d+%d", "h%d/dm%d+%d"), heal, d_dam, b_dam / 2);
			if (cast)
			{
				project(0, 1, p_ptr->y, p_ptr->x, b_dam, GF_HOLY_FIRE, PROJECT_KILL, -1);
				dispel_monsters(d_dam);
				slow_monsters(plev);
				stun_monsters(power);
				confuse_monsters(power);
				turn_monsters(power);
				stasis_monsters(power);
				hp_player(heal);
			}
		}
		break;

	case 31:
		if (name) return _("����", "Crusade");
		if (desc) return _("���E���̑P�ǂȃ����X�^�[���y�b�g�ɂ��悤�Ƃ��A�Ȃ�Ȃ������ꍇ�y�ёP�ǂłȂ������X�^�[�����|������B����ɑ����̉������ꂽ�R�m���������A�q�[���[�A�j���A�����A�Ύ׈����E�𓾂�B",
			"Attempts to charm all good monsters in sight, and scare all non-charmed monsters, and summons great number of knights, and gives heroism, bless, speed and protection from evil.");

		{
			if (cast)
			{
				int base = 25;
				int sp_sides = 20 + plev;
				int sp_base = plev;

				int i;
				crusade();
				for (i = 0; i < 12; i++)
				{
					int attempt = 10;
					POSITION my = 0, mx = 0;

					while (attempt--)
					{
						scatter(&my, &mx, p_ptr->y, p_ptr->x, 4, 0);

						/* Require empty grids */
						if (cave_empty_bold2(my, mx)) break;
					}
					if (attempt < 0) continue;
					summon_specific(-1, my, mx, plev, SUMMON_KNIGHTS, (PM_ALLOW_GROUP | PM_FORCE_PET | PM_HASTE));
				}
				set_hero(randint1(base) + base, FALSE);
				set_blessed(randint1(base) + base, FALSE);
				set_fast(randint1(sp_sides) + sp_base, FALSE);
				set_protevil(randint1(base) + base, FALSE);
				set_afraid(0);
			}
		}
		break;
	}

	return "";
}

