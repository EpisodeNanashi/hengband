
#include "angband.h"
#include "cmd-spell.h"
#include "spells-summon.h"

/*!
* @brief �����̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_daemon_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	static const char s_dam[] = _("����:", "dam ");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�}�W�b�N�E�~�T�C��", "Magic Missile");
		if (desc) return _("�ア���@�̖����B", "Fires a weak bolt of magic.");

		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance() - 10, GF_MISSILE, dir, damroll(dice, sides));
			}
		}
		break;

	case 1:
		if (name) return _("���������m", "Detect Unlife");
		if (desc) return _("�߂��̐����̂Ȃ������X�^�[�����m����B", "Detects all nonliving monsters in your vicinity.");

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cast)
			{
				detect_monsters_nonliving(rad);
			}
		}
		break;

	case 2:
		if (name) return _("�ׂȂ�j��", "Evil Bless");
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

	case 3:
		if (name) return _("�ωΉ�", "Resist Fire");
		if (desc) return _("��莞�ԁA���ւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to fire, cold and electricity for a while. These resistances can be added to which from equipment for more powerful resistances.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_fire(randint1(base) + base, FALSE);
			}
		}
		break;

	case 4:
		if (name) return _("���Q", "Horrify");
		if (desc) return _("�����X�^�[1�̂����|�����A�N�O������B��R�����Ɩ����B", "Attempts to scare and stun a monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fear_monster(dir, power);
				stun_monster(dir, power);
			}
		}
		break;

	case 5:
		if (name) return _("�n���̖�", "Nether Bolt");
		if (desc) return _("�n���̃{���g�������̓r�[������B", "Fires a bolt or beam of nether.");

		{
			int dice = 6 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_NETHER, dir, damroll(dice, sides));
			}
		}
		break;

	case 6:
		if (name) return _("�Ñ�̎��쏢��", "Summon Manes");
		if (desc) return _("�Ñ�̎������������B", "Summons a manes.");

		{
			if (cast)
			{
				if (!summon_specific(-1, p_ptr->y, p_ptr->x, (plev * 3) / 2, SUMMON_MANES, (PM_ALLOW_GROUP | PM_FORCE_PET)))
				{
					msg_print(_("�Ñ�̎���͌���Ȃ������B", "No Manes arrive."));
				}
			}
		}
		break;

	case 7:
		if (name) return _("�n���̉�", "Hellish Flame");
		if (desc) return _("�׈��ȗ͂����{�[������B�P�ǂȃ����X�^�[�ɂ͑傫�ȃ_���[�W��^����B",
			"Fires a ball of evil power. Hurts good monsters greatly.");

		{
			int dice = 3;
			int sides = 6;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (p_ptr->pclass == CLASS_MAGE ||
				p_ptr->pclass == CLASS_HIGH_MAGE ||
				p_ptr->pclass == CLASS_SORCERER)
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HELL_FIRE, dir, damroll(dice, sides) + base, rad);
			}
		}
		break;

	case 8:
		if (name) return _("�f�[�����x�z", "Dominate Demon");
		if (desc) return _("����1�̂𖣗�����B��R�����Ɩ���", "Attempts to charm a demon.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				control_one_demon(dir, power);
			}
		}
		break;

	case 9:
		if (name) return _("�r�W����", "Vision");
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

	case 10:
		if (name) return _("�ϒn��", "Resist Nether");
		if (desc) return _("��莞�ԁA�n���ւ̑ϐ��𓾂�B", "Gives resistance to nether for a while.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_res_nether(randint1(base) + base, FALSE);
			}
		}
		break;

	case 11:
		if (name) return _("�v���Y�}�E�{���g", "Plasma bolt");
		if (desc) return _("�v���Y�}�̃{���g�������̓r�[������B", "Fires a bolt or beam of plasma.");

		{
			int dice = 11 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_PLASMA, dir, damroll(dice, sides));
			}
		}
		break;

	case 12:
		if (name) return _("�t�@�C�A�E�{�[��", "Fire Ball");
		if (desc) return _("���̋�����B", "Fires a ball of fire.");

		{
			HIT_POINT dam = plev + 55;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_FIRE, dir, dam, rad);
			}
		}
		break;

	case 13:
		if (name) return _("���̐n", "Fire Branding");
		if (desc) return _("����ɉ��̑���������B", "Makes current weapon fire branded.");

		{
			if (cast)
			{
				brand_weapon(1);
			}
		}
		break;

	case 14:
		if (name) return _("�n����", "Nether Ball");
		if (desc) return _("�傫�Ȓn���̋�����B", "Fires a huge ball of nether.");

		{
			HIT_POINT dam = plev * 3 / 2 + 100;
			int rad = plev / 20 + 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_NETHER, dir, dam, rad);
			}
		}
		break;

	case 15:
		if (name) return _("�f�[��������", "Summon Demon");
		if (desc) return _("����1�̂���������B", "Summons a demon.");

		{
			if (cast)
			{
				bool pet = !one_in_(3);
				u32b flg = 0L;

				if (pet) flg |= PM_FORCE_PET;
				else flg |= PM_NO_PET;
				if (!(pet && (plev < 50))) flg |= PM_ALLOW_GROUP;

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, plev * 2 / 3 + randint1(plev / 2), SUMMON_DEMON, flg))
				{
					msg_print(_("�����̈��L���[�������B", "The area fills with a stench of sulphur and brimstone."));

					if (pet)
					{
						msg_print(_("�u���p�ł������܂����A����l�l�v", "'What is thy bidding... Master?'"));
					}
					else
					{
						msg_print(_("�u�ڂ����҂�A��͓��̉��l�ɂ��炸�I ���O�̍��𒸂����I�v",
							"'NON SERVIAM! Wretch! I shall feast on thy mortal soul!'"));
					}
				}
				else
				{
					msg_print(_("�����͌���Ȃ������B", "No demons arrive."));
				}
				break;
			}
		}
		break;

	case 16:
		if (name) return _("�����̖�", "Devilish Eye");
		if (desc) return _("��莞�ԁA�e���p�V�[�\�͂𓾂�B", "Gives telepathy for a while.");

		{
			int base = 30;
			int sides = 25;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_tim_esp(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 17:
		if (name) return _("�����̃N���[�N", "Devil Cloak");
		if (desc) return _("���|����菜���A��莞�ԁA���Ɨ�C�̑ϐ��A���̃I�[���𓾂�B�ϐ��͑����ɂ��ϐ��ɗݐς���B",
			"Removes fear. Gives resistance to fire and cold, and aura of fire. These resistances can be added to which from equipment for more powerful resistances.");

		{
			TIME_EFFECT base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				TIME_EFFECT dur = randint1(base) + base;

				set_oppose_fire(dur, FALSE);
				set_oppose_cold(dur, FALSE);
				set_tim_sh_fire(dur, FALSE);
				set_afraid(0);
				break;
			}
		}
		break;

	case 18:
		if (name) return _("�n�◬", "The Flow of Lava");
		if (desc) return _("�����𒆐S�Ƃ������̋������o���A����n��ɕς���B",
			"Generates a ball of fire centered on you which transforms floors to magma.");

		{
			HIT_POINT dam = (55 + plev) * 2;
			int rad = 3;

			if (info) return info_damage(0, 0, dam / 2);

			if (cast)
			{
				fire_ball(GF_FIRE, 0, dam, rad);
				fire_ball_hide(GF_LAVA_FLOW, 0, 2 + randint1(2), rad);
			}
		}
		break;

	case 19:
		if (name) return _("�v���Y�}��", "Plasma Ball");
		if (desc) return _("�v���Y�}�̋�����B", "Fires a ball of plasma.");

		{
			HIT_POINT dam = plev * 3 / 2 + 80;
			int rad = 2 + plev / 40;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_PLASMA, dir, dam, rad);
			}
		}
		break;

	case 20:
		if (name) return _("�����ω�", "Polymorph Demon");
		if (desc) return _("��莞�ԁA�����ɕω�����B�ω����Ă���Ԃ͖{���̎푰�̔\�͂������A����Ɉ����Ƃ��Ă̔\�͂𓾂�B",
			"Mimic a demon for a while. Loses abilities of original race and gets abilities as a demon.");

		{
			int base = 10 + plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_mimic(base + randint1(base), MIMIC_DEMON, FALSE);
			}
		}
		break;

	case 21:
		if (name) return _("�n���̔g��", "Nather Wave");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�Ƀ_���[�W��^����B�P�ǂȃ����X�^�[�ɓ��ɑ傫�ȃ_���[�W��^����B",
			"Damages all monsters in sight. Hurts good monsters greatly.");

		{
			int sides1 = plev * 2;
			int sides2 = plev * 2;

			if (info) return format("%sd%d+d%d", s_dam, sides1, sides2);

			if (cast)
			{
				dispel_monsters(randint1(sides1));
				dispel_good(randint1(sides2));
			}
		}
		break;

	case 22:
		if (name) return _("�T�L���o�X�̐ڕ�", "Kiss of Succubus");
		if (desc) return _("���ʍ����̋�����B", "Fires a ball of nexus.");

		{
			HIT_POINT dam = 100 + plev * 2;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_NEXUS, dir, dam, rad);
			}
		}
		break;

	case 23:
		if (name) return _("�j�ł̎�", "Doom Hand");
		if (desc) return _("�j�ł̎����B�H����������X�^�[�͂��̂Ƃ���HP�̔����O��̃_���[�W���󂯂�B", "Attempts to make a monster's HP almost half.");

		{
			if (cast)
			{
				if (!get_aim_dir(&dir))
					return NULL;
				else
					msg_print(_("<�j�ł̎�>��������I", "You invoke the Hand of Doom!"));

				fire_ball_hide(GF_HAND_DOOM, dir, plev * 2, 0);
			}
		}
		break;

	case 24:
		if (name) return _("�m�C���g", "Raise the Morale");
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
		if (name) return _("�s�ł̓���", "Immortal Body");
		if (desc) return _("��莞�ԁA���ԋt�]�ւ̑ϐ��𓾂�B", "Gives resistance to time for a while.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_res_time(randint1(base) + base, FALSE);
			}
		}
		break;

	case 26:
		if (name) return _("���C�̉~��", "Insanity Circle");
		if (desc) return _("�����𒆐S�Ƃ����J�I�X�̋��A�����̋��𔭐������A�߂��̃����X�^�[�𖣗�����B",
			"Generate balls of chaos, confusion and charm centered on you.");

		{
			HIT_POINT dam = 50 + plev;
			int power = 20 + plev;
			int rad = 3 + plev / 20;

			if (info) return format("%s%d+%d", s_dam, dam / 2, dam / 2);

			if (cast)
			{
				fire_ball(GF_CHAOS, 0, dam, rad);
				fire_ball(GF_CONFUSION, 0, dam, rad);
				fire_ball(GF_CHARM, 0, power, rad);
			}
		}
		break;

	case 27:
		if (name) return _("�y�b�g���j", "Explode Pets");
		if (desc) return _("�S�Ẵy�b�g�������I�ɔ��j������B", "Makes all pets explode.");

		{
			if (cast)
			{
				discharge_minion();
			}
		}
		break;

	case 28:
		if (name) return _("�O���[�^�[�f�[��������", "Summon Greater Demon");
		if (desc) return _("�㋉�f�[��������������B��������ɂ͐l��('p','h','t'�ŕ\����郂���X�^�[)�̎��̂�����Ȃ���΂Ȃ�Ȃ��B",
			"Summons greater demon. It need to sacrifice a corpse of human ('p','h' or 't').");

		{
			if (cast)
			{
				if (!cast_summon_greater_demon()) return NULL;
			}
		}
		break;

	case 29:
		if (name) return _("�n����", "Nether Storm");
		if (desc) return _("������Ȓn���̋�����B", "Generate a huge ball of nether.");

		{
			HIT_POINT dam = plev * 15;
			int rad = plev / 5;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_NETHER, dir, dam, rad);
			}
		}
		break;

	case 30:
		if (name) return _("���̎�", "Bloody Curse");
		if (desc) return _("�������_���[�W���󂯂邱�Ƃɂ���đΏۂɎ􂢂������A�_���[�W��^���l�X�Ȍ��ʂ������N�����B",
			"Puts blood curse which damages and causes various effects on a monster. You also take damage.");

		{
			HIT_POINT dam = 600;
			int rad = 0;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball_hide(GF_BLOOD_CURSE, dir, dam, rad);
				take_hit(DAMAGE_USELIFE, 20 + randint1(30), _("���̎�", "Blood curse"), -1);
			}
		}
		break;

	case 31:
		if (name) return _("�����ω�", "Polymorph Demonlord");
		if (desc) return _("�����̉��ɕω�����B�ω����Ă���Ԃ͖{���̎푰�̔\�͂������A����Ɉ����̉��Ƃ��Ă̔\�͂𓾁A�ǂ�j�󂵂Ȃ�������B",
			"Mimic a demon lord for a while. Loses abilities of original race and gets great abilities as a demon lord. Even hard walls can't stop your walking.");

		{
			int base = 15;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_mimic(base + randint1(base), MIMIC_DEMON_LORD, FALSE);
			}
		}
		break;
	}

	return "";
}

