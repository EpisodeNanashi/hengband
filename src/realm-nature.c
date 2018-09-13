#include "angband.h"
#include "cmd-spell.h"


/*!
* @brief ���R�̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_nature_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	static const char s_dam[] = _("����:", "dam ");
	static const char s_rng[] = _("�˒�", "rng ");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�����X�^�[���m", "Detect Creatures");
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
		if (name) return _("���", "Lightning");
		if (desc) return _("�d���̒Z���r�[������B", "Fires a short beam of lightning.");

		{
			int dice = 3 + (plev - 1) / 5;
			int sides = 4;
			POSITION range = plev / 6 + 2;

			if (info) return format("%s%dd%d %s%d", s_dam, dice, sides, s_rng, range);

			if (cast)
			{
				project_length = range;

				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 2:
		if (name) return _("㩂Ɣ����m", "Detect Doors and Traps");
		if (desc) return _("�߂��̑S�Ă�㩂Ɣ������m����B", "Detects traps, doors, and stairs in your vicinity.");

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
		if (name) return _("�H�Ɛ���", "Produce Food");
		if (desc) return _("�H��������o���B", "Produces a Ration of Food.");

		{
			if (cast)
			{
				object_type forge, *q_ptr = &forge;
				msg_print(_("�H���𐶐������B", "A food ration is produced."));

				/* Create the food ration */
				object_prep(q_ptr, lookup_kind(TV_FOOD, SV_FOOD_RATION));

				/* Drop the object from heaven */
				drop_near(q_ptr, -1, p_ptr->y, p_ptr->x);
			}
		}
		break;

	case 4:
		if (name) return _("���̌�", "Daylight");
		if (desc) return _("�������Ƃ炵�Ă���͈͂������S�̂��i�v�ɖ��邭����B", "Lights up nearby area and the inside of a room permanently.");

		{
			int dice = 2;
			int sides = plev / 2;
			int rad = (plev / 10) + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);

				if ((prace_is_(RACE_VAMPIRE) || (p_ptr->mimic_form == MIMIC_VAMPIRE)) && !p_ptr->resist_lite)
				{
					msg_print(_("���̌������Ȃ��̓��̂��ł������I", "The daylight scorches your flesh!"));
					take_hit(DAMAGE_NOESCAPE, damroll(2, 2), _("���̌�", "daylight"), -1);
				}
			}
		}
		break;

	case 5:
		if (name) return _("�����K��", "Animal Taming");
		if (desc) return _("����1�̂𖣗�����B��R�����Ɩ����B", "Attempts to charm an animal.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				charm_animal(dir, power);
			}
		}
		break;

	case 6:
		if (name) return _("���ւ̑ϐ�", "Resist Environment");
		if (desc) return _("��莞�ԁA��C�A���A�d���ɑ΂���ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to fire, cold and electricity for a while. These resistances can be added to which from equipment for more powerful resistances.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
				set_oppose_elec(randint1(base) + base, FALSE);
			}
		}
		break;

	case 7:
		if (name) return _("���ƓŎ���", "Cure Wounds & Poison");
		if (desc) return _("�����S�������A�ł�̂��犮�S�Ɏ�菜���A�̗͂������񕜂�����B", "Heals all cut and poison status. Heals HP a little.");

		{
			int dice = 2;
			int sides = 8;

			if (info) return info_heal(dice, sides, 0);

			if (cast)
			{
				hp_player(damroll(dice, sides));
				set_cut(0);
				set_poisoned(0);
			}
		}
		break;

	case 8:
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

	case 9:
		if (name) return _("�A�C�X�E�{���g", "Frost Bolt");
		if (desc) return _("��C�̃{���g�������̓r�[������B", "Fires a bolt or beam of cold.");

		{
			int dice = 3 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_bolt_or_beam(beam_chance() - 10, GF_COLD, dir, damroll(dice, sides));
			}
		}
		break;

	case 10:
		if (name) return _("���R�̊o��", "Nature Awareness");
		if (desc) return _("���ӂ̒n�`�����m���A�߂���㩁A���A�K�i�A�S�Ẵ����X�^�[�����m����B",
			"Maps nearby area. Detects all monsters, traps, doors and stairs.");

		{
			int rad1 = DETECT_RAD_MAP;
			int rad2 = DETECT_RAD_DEFAULT;

			if (info) return info_radius(MAX(rad1, rad2));

			if (cast)
			{
				map_area(rad1);
				detect_traps(rad2, TRUE);
				detect_doors(rad2);
				detect_stairs(rad2);
				detect_monsters_normal(rad2);
			}
		}
		break;

	case 11:
		if (name) return _("�t�@�C�A�E�{���g", "Fire Bolt");
		if (desc) return _("�Ή��̃{���g�������̓r�[������B", "Fires a bolt or beam of fire.");

		{
			int dice = 5 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_bolt_or_beam(beam_chance() - 10, GF_FIRE, dir, damroll(dice, sides));
			}
		}
		break;

	case 12:
		if (name) return _("���z����", "Ray of Sunlight");
		if (desc) return _("��������B��������������X�^�[�Ɍ��ʂ�����B", "Fires a beam of light which damages to light-sensitive monsters.");

		{
			int dice = 6;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				msg_print(_("���z���������ꂽ�B", "A line of sunlight appears."));
				lite_line(dir, damroll(6, 8));
			}
		}
		break;

	case 13:
		if (name) return _("������", "Entangle");
		if (desc) return _("���E���̑S�Ẵ����X�^�[������������B��R�����Ɩ����B", "Attempts to slow all monsters in sight.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				slow_monsters(plev);
			}
		}
		break;

	case 14:
		if (name) return _("��������", "Summon Animal");
		if (desc) return _("������1�̏�������B", "Summons an animal.");

		{
			if (cast)
			{
				if (!(summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_ANIMAL_RANGER, (PM_ALLOW_GROUP | PM_FORCE_PET))))
				{
					msg_print(_("�����͌���Ȃ������B", "No animals arrive."));
				}
				break;
			}
		}
		break;

	case 15:
		if (name) return _("�򑐎���", "Herbal Healing");
		if (desc) return _("�̗͂�啝�ɉ񕜂����A�����A�N�O��ԁA�ł���S������B", "Heals HP greatly. And heals cut, stun and poison completely.");

		{
			int heal = 500;

			if (info) return info_heal(0, 0, heal);

			if (cast)
			{
				hp_player(heal);
				set_stun(0);
				set_cut(0);
				set_poisoned(0);
			}
		}
		break;

	case 16:
		if (name) return _("�K�i����", "Stair Building");
		if (desc) return _("�����̂���ʒu�ɊK�i�����B", "Creates a stair which goes down or up.");

		{
			if (cast)
			{
				stair_creation();
			}
		}
		break;

	case 17:
		if (name) return _("���Ή�", "Stone Skin");
		if (desc) return _("��莞�ԁAAC���㏸������B", "Gives bonus to AC for a while.");

		{
			int base = 20;
			int sides = 30;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_shield(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 18:
		if (name) return _("�^�E�ϐ�", "Resistance True");
		if (desc) return _("��莞�ԁA�_�A�d���A���A��C�A�łɑ΂���ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to fire, cold, electricity, acid and poison for a while. These resistances can be added to which from equipment for more powerful resistances.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_acid(randint1(base) + base, FALSE);
				set_oppose_elec(randint1(base) + base, FALSE);
				set_oppose_fire(randint1(base) + base, FALSE);
				set_oppose_cold(randint1(base) + base, FALSE);
				set_oppose_pois(randint1(base) + base, FALSE);
			}
		}
		break;

	case 19:
		if (name) return _("�X�ёn��", "Forest Creation");
		if (desc) return _("���͂ɖ؂����o���B", "Creates trees in all adjacent squares.");

		{
			if (cast)
			{
				tree_creation();
			}
		}
		break;

	case 20:
		if (name) return _("�����F�a", "Animal Friendship");
		if (desc) return _("���E���̑S�Ă̓����𖣗�����B��R�����Ɩ����B", "Attempts to charm all animals in sight.");

		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cast)
			{
				charm_animals(power);
			}
		}
		break;

	case 21:
		if (name) return _("������", "Stone Tell");
		if (desc) return _("�A�C�e���̎��\�͂����S�ɒm��B", "*Identifies* an item.");

		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 22:
		if (name) return _("�΂̕�", "Wall of Stone");
		if (desc) return _("�����̎��͂ɉԛ���̕ǂ����B", "Creates granite walls in all adjacent squares.");

		{
			if (cast)
			{
				wall_stone();
			}
		}
		break;

	case 23:
		if (name) return _("���H�h�~", "Protect from Corrosion");
		if (desc) return _("�A�C�e�����_�ŏ����Ȃ��悤���H����B", "Makes an equipment acid-proof.");

		{
			if (cast)
			{
				if (!rustproof()) return NULL;
			}
		}
		break;

	case 24:
		if (name) return _("�n�k", "Earthquake");
		if (desc) return _("���͂̃_���W������h�炵�A�ǂƏ��������_���ɓ���ς���B",
			"Shakes dungeon structure, and results in random swapping of floors and walls.");

		{
			int rad = 10;

			if (info) return info_radius(rad);

			if (cast)
			{
				earthquake(p_ptr->y, p_ptr->x, rad);
			}
		}
		break;

	case 25:
		if (name) return _("�J�}�C�^�`", "Cyclone");
		if (desc) return _("�S�����Ɍ������čU������B", "Attacks all adjacent monsters.");

		{
			if (cast)
			{
				int y = 0, x = 0;
				cave_type       *c_ptr;
				monster_type    *m_ptr;

				for (dir = 0; dir < 8; dir++)
				{
					y = p_ptr->y + ddy_ddd[dir];
					x = p_ptr->x + ddx_ddd[dir];
					c_ptr = &cave[y][x];

					/* Get the monster */
					m_ptr = &m_list[c_ptr->m_idx];

					/* Hack -- attack monsters */
					if (c_ptr->m_idx && (m_ptr->ml || cave_have_flag_bold(y, x, FF_PROJECT)))
						py_attack(y, x, 0);
				}
			}
		}
		break;

	case 26:
		if (name) return _("�u���U�[�h", "Blizzard");
		if (desc) return _("����ȗ�C�̋�����B", "Fires a huge ball of cold.");

		{
			HIT_POINT dam = 70 + plev * 3 / 2;
			int rad = plev / 12 + 1;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_COLD, dir, dam, rad);
			}
		}
		break;

	case 27:
		if (name) return _("��ȗ�", "Lightning Storm");
		if (desc) return _("����ȓd���̋�����B", "Fires a huge electric ball.");

		{
			HIT_POINT dam = 90 + plev * 3 / 2;
			int rad = plev / 12 + 1;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_ELEC, dir, dam, rad);
				break;
			}
		}
		break;

	case 28:
		if (name) return _("�Q��", "Whirlpool");
		if (desc) return _("����Ȑ��̋�����B", "Fires a huge ball of water.");

		{
			HIT_POINT dam = 100 + plev * 3 / 2;
			int rad = plev / 12 + 1;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;
				fire_ball(GF_WATER, dir, dam, rad);
			}
		}
		break;

	case 29:
		if (name) return _("�z������", "Call Sunlight");
		if (desc) return _("�����𒆐S�Ƃ������̋��𔭐�������B����ɁA���̊K�S�̂��i�v�ɏƂ炵�A�_���W���������ׂẴA�C�e�������m����B",
			"Generates ball of light centered on you. Maps and lights whole dungeon level. Knows all objects location.");

		{
			HIT_POINT dam = 150;
			int rad = 8;

			if (info) return info_damage(0, 0, dam / 2);

			if (cast)
			{
				fire_ball(GF_LITE, 0, dam, rad);
				chg_virtue(V_KNOWLEDGE, 1);
				chg_virtue(V_ENLIGHTEN, 1);
				wiz_lite(FALSE);

				if ((prace_is_(RACE_VAMPIRE) || (p_ptr->mimic_form == MIMIC_VAMPIRE)) && !p_ptr->resist_lite)
				{
					msg_print(_("���������Ȃ��̓��̂��ł������I", "The sunlight scorches your flesh!"));
					take_hit(DAMAGE_NOESCAPE, 50, _("����", "sunlight"), -1);
				}
			}
		}
		break;

	case 30:
		if (name) return _("����̐n", "Elemental Branding");
		if (desc) return _("����ɉ�����C�̑���������B", "Makes current weapon fire or frost branded.");

		{
			if (cast)
			{
				brand_weapon(randint0(2));
			}
		}
		break;

	case 31:
		if (name) return _("���R�̋���", "Nature's Wrath");
		if (desc) return _("�߂��̑S�Ẵ����X�^�[�Ƀ_���[�W��^���A�n�k���N�����A�����𒆐S�Ƃ��������̋��𔭐�������B",
			"Damages all monsters in sight. Makes quake. Generates disintegration ball centered on you.");

		{
			int d_dam = 4 * plev;
			int b_dam = (100 + plev) * 2;
			int b_rad = 1 + plev / 12;
			int q_rad = 20 + plev / 2;

			if (info) return format("%s%d+%d", s_dam, d_dam, b_dam / 2);

			if (cast)
			{
				dispel_monsters(d_dam);
				earthquake(p_ptr->y, p_ptr->x, q_rad);
				project(0, b_rad, p_ptr->y, p_ptr->x, b_dam, GF_DISINTEGRATE, PROJECT_KILL | PROJECT_ITEM, -1);
			}
		}
		break;
	}

	return "";
}

