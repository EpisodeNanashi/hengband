#include "angband.h"
#include "cmd-spell.h"



/*!
* @brief �Í��̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_death_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	static const char s_dam[] = _("����:", "dam ");
	static const char s_random[] = _("�����_��", "random");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
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

	case 1:
		if (name) return _("��E�e", "Malediction");
		if (desc) return _("���������Ȏ׈��ȗ͂����{�[������B�P�ǂȃ����X�^�[�ɂ͑傫�ȃ_���[�W��^����B",
			"Fires a tiny ball of evil power which hurts good monsters greatly.");

		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;
			int rad = 0;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				/*
				* A radius-0 ball may (1) be aimed at
				* objects etc., and will affect them;
				* (2) may be aimed at ANY visible
				* monster, unlike a 'bolt' which must
				* travel to the monster.
				*/

				fire_ball(GF_HELL_FIRE, dir, damroll(dice, sides), rad);

				if (one_in_(5))
				{
					/* Special effect first */
					int effect = randint1(1000);

					if (effect == 666)
						fire_ball_hide(GF_DEATH_RAY, dir, plev * 200, 0);
					else if (effect < 500)
						fire_ball_hide(GF_TURN_ALL, dir, plev, 0);
					else if (effect < 800)
						fire_ball_hide(GF_OLD_CONF, dir, plev, 0);
					else
						fire_ball_hide(GF_STUN, dir, plev, 0);
				}
			}
		}
		break;

	case 2:
		if (name) return _("�׈����m", "Detect Evil");
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

	case 3:
		if (name) return _("���L�_", "Stinking Cloud");
		if (desc) return _("�ł̋�����B", "Fires a ball of poison.");

		{
			HIT_POINT dam = 10 + plev / 2;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_POIS, dir, dam, rad);
			}
		}
		break;

	case 4:
		if (name) return _("��������", "Black Sleep");
		if (desc) return _("1�̂̃����X�^�[�𖰂点��B��R�����Ɩ����B", "Attempts to sleep a monster.");

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

	case 5:
		if (name) return _("�ϓ�", "Resist Poison");
		if (desc) return _("��莞�ԁA�łւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to poison. This resistance can be added to which from equipment for more powerful resistance.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_pois(randint1(base) + base, FALSE);
			}
		}
		break;

	case 6:
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

	case 7:
		if (name) return _("�A���f�b�h�]��", "Enslave Undead");
		if (desc) return _("�A���f�b�h1�̂𖣗�����B��R�����Ɩ����B", "Attempts to charm an undead monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				control_one_undead(dir, power);
			}
		}
		break;

	case 8:
		if (name) return _("�G���g���s�[�̋�", "Orb of Entropy");
		if (desc) return _("�����̂���҂�HP�ƍő�HP�o���Ƀ_���[�W��^������ʂ̂��鋅����B", "Fires a ball which damages to both HP and MaxHP of living monsters.");

		{
			int dice = 3;
			int sides = 6;
			int rad = (plev < 30) ? 2 : 3;
			int base;

			if (IS_WIZARD_CLASS())
				base = plev + plev / 2;
			else
				base = plev + plev / 4;


			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HYPODYNAMIA, dir, damroll(dice, sides) + base, rad);
			}
		}
		break;

	case 9:
		if (name) return _("�n���̖�", "Nether Bolt");
		if (desc) return _("�n���̃{���g�������̓r�[������B", "Fires a bolt or beam of nether.");

		{
			int dice = 8 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_NETHER, dir, damroll(dice, sides));
			}
		}
		break;

	case 10:
		if (name) return _("�E�C�_", "Cloud kill");
		if (desc) return _("�����𒆐S�Ƃ����ł̋��𔭐�������B", "Generate a ball of poison centered on you.");

		{
			HIT_POINT dam = (30 + plev) * 2;
			int rad = plev / 10 + 2;

			if (info) return info_damage(0, 0, dam / 2);

			if (cast)
			{
				project(0, rad, p_ptr->y, p_ptr->x, dam, GF_POIS, PROJECT_KILL | PROJECT_ITEM, -1);
			}
		}
		break;

	case 11:
		if (name) return _("�����X�^�[����", "Genocide One");
		if (desc) return _("�����X�^�[1�̂���������B�o���l��A�C�e���͎�ɓ���Ȃ��B��R�����Ɩ����B", "Attempts to vanish a monster.");

		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball_hide(GF_GENOCIDE, dir, power, 0);
			}
		}
		break;

	case 12:
		if (name) return _("�ł̐n", "Poison Branding");
		if (desc) return _("����ɓł̑���������B", "Makes current weapon poison branded.");

		{
			if (cast)
			{
				brand_weapon(3);
			}
		}
		break;

	case 13:
		if (name) return _("�z���̖�", "Vampiric Bolt");
		if (desc) return _("�{���g�ɂ�胂���X�^�[1�̂��琶���͂��z���Ƃ�B�z���Ƃ��������͂ɂ���Ė����x���オ��B",
			"Absorbs some HP from a monster and gives them to you by bolt. You will also gain nutritional sustenance from this.");

		{
			int dice = 1;
			int sides = plev * 2;
			int base = plev * 2;

			if (info) return info_damage(dice, sides, base);

			if (cast)
			{
				HIT_POINT dam = base + damroll(dice, sides);

				if (!get_aim_dir(&dir)) return NULL;

				if (hypodynamic_bolt(dir, dam))
				{
					chg_virtue(V_SACRIFICE, -1);
					chg_virtue(V_VITALITY, -1);

					hp_player(dam);

					/*
					* Gain nutritional sustenance:
					* 150/hp drained
					*
					* A Food ration gives 5000
					* food points (by contrast)
					* Don't ever get more than
					* "Full" this way But if we
					* ARE Gorged, it won't cure
					* us
					*/
					dam = p_ptr->food + MIN(5000, 100 * dam);

					/* Not gorged already */
					if (p_ptr->food < PY_FOOD_MAX)
						set_food(dam >= PY_FOOD_MAX ? PY_FOOD_MAX - 1 : dam);
				}
			}
		}
		break;

	case 14:
		if (name) return _("�����̏p", "Animate dead");
		if (desc) return _("���͂̎��̂⍜�𐶂��Ԃ��B", "Resurrects nearby corpse and skeletons. And makes these your pets.");

		{
			if (cast)
			{
				animate_dead(0, p_ptr->y, p_ptr->x);
			}
		}
		break;

	case 15:
		if (name) return _("���E", "Genocide");
		if (desc) return _("�w�肵�������̃����X�^�[�����݂̊K�����������B��R�����Ɩ����B",
			"Eliminates an entire class of monster, exhausting you.  Powerful or unique monsters may resist.");

		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				symbol_genocide(power, TRUE);
			}
		}
		break;

	case 16:
		if (name) return _("����m��", "Berserk");
		if (desc) return _("����m�����A���|����������B", "Gives bonus to hit and HP, immunity to fear for a while. But decreases AC.");

		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_shero(randint1(base) + base, FALSE);
				hp_player(30);
				set_afraid(0);
			}
		}
		break;

	case 17:
		if (name) return _("���쏢��", "Invoke Spirits");
		if (desc) return _("�����_���ŗl�X�Ȍ��ʂ��N����B", "Causes random effects.");

		{
			if (info) return s_random;

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				cast_invoke_spirits(dir);
			}
		}
		break;

	case 18:
		if (name) return _("�Í��̖�", "Dark Bolt");
		if (desc) return _("�Í��̃{���g�������̓r�[������B", "Fires a bolt or beam of darkness.");

		{
			int dice = 4 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_DARK, dir, damroll(dice, sides));
			}
		}
		break;

	case 19:
		if (name) return _("������m", "Battle Frenzy");
		if (desc) return _("����m�����A���|���������A��������B",
			"Gives another bonus to hit and HP, immunity to fear for a while. Hastes you. But decreases AC.");

		{
			int b_base = 25;
			int sp_base = plev / 2;
			int sp_sides = 20 + plev / 2;

			if (info) return info_duration(b_base, b_base);

			if (cast)
			{
				set_shero(randint1(25) + 25, FALSE);
				hp_player(30);
				set_afraid(0);
				set_fast(randint1(sp_sides) + sp_base, FALSE);
			}
		}
		break;

	case 20:
		if (name) return _("�z���̐n", "Vampiric Branding");
		if (desc) return _("����ɋz���̑���������B", "Makes current weapon Vampiric.");

		{
			if (cast)
			{
				brand_weapon(4);
			}
		}
		break;

	case 21:
		if (name) return _("�z���̘A��", "Vampiric Bolts");
		if (desc) return _("3�A�˂̃{���g�ɂ�胂���X�^�[1�̂��琶���͂��z���Ƃ�B�z���Ƃ��������͂ɂ���đ̗͂��񕜂���B",
			"Fires 3 bolts. Each of the bolts absorbs some HP from a monster and gives them to you.");
		{
			HIT_POINT dam = 100;

			if (info) return format("%s3*%d", s_dam, dam);

			if (cast)
			{
				int i;

				if (!get_aim_dir(&dir)) return NULL;

				chg_virtue(V_SACRIFICE, -1);
				chg_virtue(V_VITALITY, -1);

				for (i = 0; i < 3; i++)
				{
					if (hypodynamic_bolt(dir, dam))
						hp_player(dam);
				}
			}
		}
		break;

	case 22:
		if (name) return _("���̌���", "Nether Wave");
		if (desc) return _("���E���̐����̂��郂���X�^�[�Ƀ_���[�W��^����B", "Damages all living monsters in sight.");

		{
			int sides = plev * 3;

			if (info) return info_damage(1, sides, 0);

			if (cast)
			{
				dispel_living(randint1(sides));
			}
		}
		break;

	case 23:
		if (name) return _("�Í��̗�", "Darkness Storm");
		if (desc) return _("����ȈÍ��̋�����B", "Fires a huge ball of darkness.");

		{
			HIT_POINT dam = 100 + plev * 2;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_DARK, dir, dam, rad);
			}
		}
		break;

	case 24:
		if (name) return _("���̌���", "Death Ray");
		if (desc) return _("���̌�������B", "Fires a beam of death.");

		{
			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				death_ray(dir, plev);
			}
		}
		break;

	case 25:
		if (name) return _("���ҏ���", "Raise the Dead");
		if (desc) return _("1�̂̃A���f�b�h����������B", "Summons an undead monster.");

		{
			if (cast)
			{
				int type;
				bool pet = one_in_(3);
				u32b flg = 0L;

				type = (plev > 47 ? SUMMON_HI_UNDEAD : SUMMON_UNDEAD);

				if (!pet || (pet && (plev > 24) && one_in_(3)))
					flg |= PM_ALLOW_GROUP;

				if (pet) flg |= PM_FORCE_PET;
				else flg |= (PM_ALLOW_UNIQUE | PM_NO_PET);

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, (plev * 3) / 2, type, flg))
				{
					msg_print(_("�₽���������Ȃ��̎���ɐ����n�߂��B����͕��s�L���^��ł���...",
						"Cold winds begin to blow around you, carrying with them the stench of decay..."));


					if (pet)
					{
						msg_print(_("�Â��̎�����ҋ������Ȃ��Ɏd���邽�ߓy�����S�����I",
							"Ancient, long-dead forms arise from the ground to serve you!"));
					}
					else
					{
						msg_print(_("���҂��S�����B�����W���邠�Ȃ��𔱂��邽�߂ɁI",
							"'The dead arise... to punish you for disturbing them!'"));
					}

					chg_virtue(V_UNLIFE, 1);
				}
			}
		}
		break;

	case 26:
		if (name) return _("���҂̔�`", "Esoteria");
		if (desc) return _("�A�C�e����1���ʂ���B���x���������ƃA�C�e���̔\�͂����S�ɒm�邱�Ƃ��ł���B",
			"Identifies an item. Or *identifies* an item at higher level.");

		{
			if (cast)
			{
				if (randint1(50) > plev)
				{
					if (!ident_spell(FALSE)) return NULL;
				}
				else
				{
					if (!identify_fully(FALSE)) return NULL;
				}
			}
		}
		break;

	case 27:
		if (name) return _("�z���S�ω�", "Polymorph Vampire");
		if (desc) return _("��莞�ԁA�z���S�ɕω�����B�ω����Ă���Ԃ͖{���̎푰�̔\�͂������A����ɋz���S�Ƃ��Ă̔\�͂𓾂�B",
			"Mimic a vampire for a while. Loses abilities of original race and gets abilities as a vampire.");

		{
			int base = 10 + plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_mimic(base + randint1(base), MIMIC_VAMPIRE, FALSE);
			}
		}
		break;

	case 28:
		if (name) return _("�o���l����", "Restore Life");
		if (desc) return _("�������o���l���񕜂���B", "Restore lost experience.");

		{
			if (cast)
			{
				restore_level();
			}
		}
		break;

	case 29:
		if (name) return _("���Ӗ��E", "Mass Genocide");
		if (desc) return _("�����̎��͂ɂ��郂���X�^�[�����݂̊K�����������B��R�����Ɩ����B",
			"Eliminates all nearby monsters, exhausting you.  Powerful or unique monsters may be able to resist.");

		{
			int power = plev + 50;

			if (info) return info_power(power);

			if (cast)
			{
				mass_genocide(power, TRUE);
			}
		}
		break;

	case 30:
		if (name) return _("�n���̍���", "Hellfire");
		if (desc) return _("�׈��ȗ͂���������B�P�ǂȃ����X�^�[�ɂ͑傫�ȃ_���[�W��^����B",
			"Fires a powerful ball of evil power. Hurts good monsters greatly.");

		{
			HIT_POINT dam = 666;
			int rad = 3;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_HELL_FIRE, dir, dam, rad);
				take_hit(DAMAGE_USELIFE, 20 + randint1(30), _("�n���̍��΂̎�������������J", "the strain of casting Hellfire"), -1);
			}
		}
		break;

	case 31:
		if (name) return _("�H�̉�", "Wraithform");
		if (desc) return _("��莞�ԁA�ǂ�ʂ蔲���邱�Ƃ��ł��󂯂�_���[�W���y�������H�̂̏�Ԃɕϐg����B",
			"Becomes wraith form which gives ability to pass walls and makes all damages half.");

		{
			int base = plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_wraith_form(randint1(base) + base, FALSE);
			}
		}
		break;
	}

	return "";
}
