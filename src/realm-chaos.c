#include "angband.h"
#include "cmd-spell.h"

/*!
* @brief �J�I�X�̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_chaos_spell(SPELL_IDX spell, BIT_FLAGS mode)
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
		if (name) return _("�}�W�b�N�E�~�T�C��", "Magic Missile");
		if (desc) return _("�ア���@�̖����B", "Fires a weak bolt of magic.");

		{
			int dice = 3 + ((plev - 1) / 5);
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
		if (name) return _("�g���b�v/�h�A�j��", "Trap / Door Destruction");
		if (desc) return _("�אڂ���㩂Ɣ���j�󂷂�B", "Destroys all traps in adjacent squares.");

		{
			int rad = 1;

			if (info) return info_radius(rad);

			if (cast)
			{
				destroy_doors_touch();
			}
		}
		break;

	case 2:
		if (name) return _("�M��", "Flash of Light");
		if (desc) return _("�������Ƃ炵�Ă���͈͂������S�̂��i�v�ɖ��邭����B", "Lights up nearby area and the inside of a room permanently.");

		{
			int dice = 2;
			int sides = plev / 2;
			int rad = (plev / 10) + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 3:
		if (name) return _("�����̎�", "Touch of Confusion");
		if (desc) return _("���������������U�����ł���悤�ɂ���B", "Attempts to confuse the next monster that you hit.");

		{
			if (cast)
			{
				if (!(p_ptr->special_attack & ATTACK_CONFUSE))
				{
					msg_print(_("���Ȃ��̎�͌���n�߂��B", "Your hands start glowing."));
					p_ptr->special_attack |= ATTACK_CONFUSE;
					p_ptr->redraw |= (PR_STATUS);
				}
			}
		}
		break;

	case 4:
		if (name) return _("�����y��", "Mana Burst");
		if (desc) return _("���@�̋�����B", "Fires a ball of magic.");

		{
			int dice = 3;
			int sides = 5;
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

				fire_ball(GF_MISSILE, dir, damroll(dice, sides) + base, rad);

				/*
				* Shouldn't actually use GF_MANA, as
				* it will destroy all items on the
				* floor
				*/
			}
		}
		break;

	case 5:
		if (name) return _("�t�@�C�A�E�{���g", "Fire Bolt");
		if (desc) return _("���̃{���g�������̓r�[������B", "Fires a bolt or beam of fire.");

		{
			int dice = 8 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_FIRE, dir, damroll(dice, sides));
			}
		}
		break;

	case 6:
		if (name) return _("�͂̌�", "Fist of Force");
		if (desc) return _("���������ȕ����̋�����B", "Fires a tiny ball of disintegration.");

		{
			int dice = 8 + ((plev - 5) / 4);
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_DISINTEGRATE, dir, damroll(dice, sides), 0);
			}
		}
		break;

	case 7:
		if (name) return _("�e���|�[�g", "Teleport Self");
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

	case 8:
		if (name) return _("�����_�[", "Wonder");
		if (desc) return _("�����X�^�[�Ƀ����_���Ȍ��ʂ�^����B", "Fires something with random effects.");

		{
			if (info) return s_random;

			if (cast)
			{

				if (!get_aim_dir(&dir)) return NULL;

				cast_wonder(dir);
			}
		}
		break;

	case 9:
		if (name) return _("�J�I�X�E�{���g", "Chaos Bolt");
		if (desc) return _("�J�I�X�̃{���g�������̓r�[������B", "Fires a bolt or ball of chaos.");

		{
			int dice = 10 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt_or_beam(beam_chance(), GF_CHAOS, dir, damroll(dice, sides));
			}
		}
		break;

	case 10:
		if (name) return _("�\�j�b�N�E�u�[��", "Sonic Boom");
		if (desc) return _("�����𒆐S�Ƃ��������̋��𔭐�������B", "Generates a ball of sound centered on you.");

		{
			HIT_POINT dam = 60 + plev;
			int rad = plev / 10 + 2;

			if (info) return info_damage(0, 0, dam / 2);

			if (cast)
			{
				msg_print(_("�h�[���I�������h�ꂽ�I", "BOOM! Shake the room!"));
				project(0, rad, p_ptr->y, p_ptr->x, dam, GF_SOUND, PROJECT_KILL | PROJECT_ITEM, -1);
			}
		}
		break;

	case 11:
		if (name) return _("�j�ł̖�", "Doom Bolt");
		if (desc) return _("�����Ȗ��͂̃r�[������B", "Fires a beam of pure mana.");

		{
			int dice = 11 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_MANA, dir, damroll(dice, sides));
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
		if (name) return _("�e���|�[�g�E�A�E�F�C", "Teleport Other");
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

	case 14:
		if (name) return _("�j��̌��t", "Word of Destruction");
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

	case 15:
		if (name) return _("���O���X����", "Invoke Logrus");
		if (desc) return _("����ȃJ�I�X�̋�����B", "Fires a huge ball of chaos.");

		{
			HIT_POINT dam = plev * 2 + 99;
			int rad = plev / 5;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_CHAOS, dir, dam, rad);
			}
		}
		break;

	case 16:
		if (name) return _("���ҕϗe", "Polymorph Other");
		if (desc) return _("�����X�^�[1�̂�ϐg������B��R�����Ɩ����B", "Attempts to polymorph a monster.");

		{
			int power = plev;

			if (info) return info_power(power);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				poly_monster(dir, plev);
			}
		}
		break;

	case 17:
		if (name) return _("�A�����", "Chain Lightning");
		if (desc) return _("�S�����ɑ΂��ēd���̃r�[������B", "Fires lightning beams in all directions.");

		{
			int dice = 5 + plev / 10;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				for (dir = 0; dir <= 9; dir++)
					fire_beam(GF_ELEC, dir, damroll(dice, sides));
			}
		}
		break;

	case 18:
		if (name) return _("���͕���", "Arcane Binding");
		if (desc) return _("��/���@�_�̏[�U�񐔂𑝂₷���A�[�U���̃��b�h�̏[�U���Ԃ����炷�B", "Recharges staffs, wands or rods.");

		{
			int power = 90;

			if (info) return info_power(power);

			if (cast)
			{
				if (!recharge(power)) return NULL;
			}
		}
		break;

	case 19:
		if (name) return _("���q����", "Disintegrate");
		if (desc) return _("����ȕ����̋�����B", "Fires a huge ball of disintegration.");

		{
			HIT_POINT dam = plev + 70;
			int rad = 3 + plev / 40;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_DISINTEGRATE, dir, dam, rad);
			}
		}
		break;

	case 20:
		if (name) return _("�����ϗe", "Alter Reality");
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

	case 21:
		if (name) return _("�}�W�b�N�E���P�b�g", "Magic Rocket");
		if (desc) return _("���P�b�g�𔭎˂���B", "Fires a magic rocket.");

		{
			HIT_POINT dam = 120 + plev * 2;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				msg_print(_("���P�b�g���ˁI", "You launch a rocket!"));
				fire_rocket(GF_ROCKET, dir, dam, rad);
			}
		}
		break;

	case 22:
		if (name) return _("���ׂ̐n", "Chaos Branding");
		if (desc) return _("����ɃJ�I�X�̑���������B", "Makes current weapon a Chaotic weapon.");

		{
			if (cast)
			{
				brand_weapon(2);
			}
		}
		break;

	case 23:
		if (name) return _("��������", "Summon Demon");
		if (desc) return _("������1�̏�������B", "Summons a demon.");

		{
			if (cast)
			{
				u32b flg = 0L;
				bool pet = !one_in_(3);

				if (pet) flg |= PM_FORCE_PET;
				else flg |= PM_NO_PET;
				if (!(pet && (plev < 50))) flg |= PM_ALLOW_GROUP;

				if (summon_specific((pet ? -1 : 0), p_ptr->y, p_ptr->x, (plev * 3) / 2, SUMMON_DEMON, flg))
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
			}
		}
		break;

	case 24:
		if (name) return _("�d�͌���", "Beam of Gravity");
		if (desc) return _("�d�͂̃r�[������B", "Fires a beam of gravity.");

		{
			int dice = 9 + (plev - 5) / 4;
			int sides = 8;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_GRAVITY, dir, damroll(dice, sides));
			}
		}
		break;

	case 25:
		if (name) return _("�����Q", "Meteor Swarm");
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

	case 26:
		if (name) return _("���̈ꌂ", "Flame Strike");
		if (desc) return _("�����𒆐S�Ƃ���������ȉ��̋��𔭐�������B", "Generate a huge ball of fire centered on you.");

		{
			HIT_POINT dam = 300 + 3 * plev;
			int rad = 8;

			if (info) return info_damage(0, 0, dam / 2);

			if (cast)
			{
				fire_ball(GF_FIRE, 0, dam, rad);
			}
		}
		break;

	case 27:
		if (name) return _("���׏���", "Call Chaos");
		if (desc) return _("�����_���ȑ����̋���r�[���𔭐�������B", "Generate random kind of balls or beams.");

		{
			if (info) return format("%s150 / 250", s_dam);

			if (cast)
			{
				call_chaos();
			}
		}
		break;

	case 28:
		if (name) return _("���ȕϗe", "Polymorph Self");
		if (desc) return _("������ϐg�����悤�Ƃ���B", "Polymorphs yourself.");

		{
			if (cast)
			{
				if (!get_check(_("�ϐg���܂��B��낵���ł����H", "You will polymorph yourself. Are you sure? "))) return NULL;
				do_poly_self();
			}
		}
		break;

	case 29:
		if (name) return _("���̗͂�", "Mana Storm");
		if (desc) return _("���ɋ��͂ŋ���ȏ����Ȗ��͂̋�����B", "Fires an extremely powerful huge ball of pure mana.");

		{
			HIT_POINT dam = 300 + plev * 4;
			int rad = 4;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_MANA, dir, dam, rad);
			}
		}
		break;

	case 30:
		if (name) return _("���O���X�̃u���X", "Breathe Logrus");
		if (desc) return _("���ɋ��͂ȃJ�I�X�̋�����B", "Fires an extremely powerful ball of chaos.");

		{
			HIT_POINT dam = p_ptr->chp;
			int rad = 2;

			if (info) return info_damage(0, 0, dam);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_CHAOS, dir, dam, rad);
			}
		}
		break;

	case 31:
		if (name) return _("��������", "Call the Void");
		if (desc) return _("�����Ɏ��͂Ɍ������āA���P�b�g�A�����Ȗ��͂̋��A���ː��p�����̋�����B�������A�ǂɗאڂ��Ďg�p����ƍL�͈͂�j�󂷂�B",
			"Fires rockets, mana balls and nuclear waste balls in all directions each unless you are not adjacent to any walls. Otherwise *destroys* huge area.");

		{
			if (info) return format("%s3 * 175", s_dam);

			if (cast)
			{
				call_the_();
			}
		}
		break;
	}

	return "";
}