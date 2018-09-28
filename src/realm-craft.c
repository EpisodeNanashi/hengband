#include "angband.h"
#include "cmd-spell.h"
#include "selfinfo.h"



/*!
* @brief ���̈斂�@�̊e�������s��
* @param spell ���@ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_craft_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�ԊO������", "Infravision");
		if (desc) return _("��莞�ԁA�ԊO�����͂����������B", "Gives infravision for a while.");

		{
			int base = 100;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_infra(base + randint1(base), FALSE);
			}
		}
		break;

	case 1:
		if (name) return _("�񕜗͋���", "Regeneration");
		if (desc) return _("��莞�ԁA�񕜗͂����������B", "Gives regeneration ability for a while.");

		{
			int base = 80;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_tim_regen(base + randint1(base), FALSE);
			}
		}
		break;

	case 2:
		if (name) return _("�󕠏[��", "Satisfy Hunger");
		if (desc) return _("�����ɂȂ�B", "Satisfies hunger.");

		{
			if (cast)
			{
				set_food(PY_FOOD_MAX - 1);
			}
		}
		break;

	case 3:
		if (name) return _("�ϗ�C", "Resist Cold");
		if (desc) return _("��莞�ԁA��C�ւ̑ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to cold. This resistance can be added to which from equipment for more powerful resistance.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_oppose_cold(randint1(base) + base, FALSE);
			}
		}
		break;

	case 4:
		if (name) return _("�ωΉ�", "Resist Fire");
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

	case 5:
		if (name) return _("�m�C���g", "Heroism");
		if (desc) return _("��莞�ԁA�q�[���[�C���ɂȂ�B", "Removes fear, and gives bonus to hit and 10 more HP for a while.");

		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				(void)heroism(base);
			}
		}
		break;

	case 6:
		if (name) return _("�ϓd��", "Resist Lightning");
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

	case 7:
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

	case 8:
		if (name) return _("�������F", "See Invisibility");
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

	case 9:
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

	case 10:
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

	case 11:
		if (name) return _("����m��", "Berserk");
		if (desc) return _("����m�����A���|����������B", "Gives bonus to hit and HP, immunity to fear for a while. But decreases AC.");

		{
			int base = 25;

			if (info) return info_duration(base, base);

			if (cast)
			{
				(void)berserk(base + randint1(base));
			}
		}
		break;

	case 12:
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

	case 13:
		if (name) return _("�Ύ׈����E", "Protection from Evil");
		if (desc) return _("�׈��ȃ����X�^�[�̍U����h���o���A�𒣂�B", "Gives aura which protect you from evil monster's physical attack.");

		{
			int base = 3 * plev;
			int sides = 25;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_protevil(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 14:
		if (name) return _("����", "Cure");
		if (desc) return _("�ŁA�N�O��ԁA������S�������A���o�𒼂��B", "Heals poison, stun, cut and hallucination completely.");

		{
			if (cast)
			{
				(void)true_healing(0);
			}
		}
		break;

	case 15:
		if (name) return _("���@��", "Mana Branding");
		if (desc) return _("��莞�ԁA����ɗ�C�A���A�d���A�_�A�ł̂����ꂩ�̑���������B����������Ȃ��Ǝg���Ȃ��B",
			"Makes current weapon some elemental branded. You must wield weapons.");

		{
			int base = plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				if (!choose_ele_attack()) return NULL;
			}
		}
		break;

	case 16:
		if (name) return _("�e���p�V�[", "Telepathy");
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

	case 17:
		if (name) return _("���Ή�", "Stone Skin");
		if (desc) return _("��莞�ԁAAC���㏸������B", "Gives bonus to AC for a while.");

		{
			int base = 30;
			int sides = 20;

			if (info) return info_duration(base, sides);

			if (cast)
			{
				set_shield(randint1(sides) + base, FALSE);
			}
		}
		break;

	case 18:
		if (name) return _("�S�ϐ�", "Resistance");
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

	case 20:
		if (name) return _("�ǔ���", "Walk through Wall");
		if (desc) return _("��莞�ԁA�����������ǂ�ʂ蔲������悤�ɂȂ�B", "Gives ability to pass walls for a while.");

		{
			int base = plev / 2;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_kabenuke(randint1(base) + base, FALSE);
			}
		}
		break;

	case 21:
		if (name) return _("������", "Polish Shield");
		if (desc) return _("���ɔ��˂̑���������B", "Makes a shield a shield of reflection.");

		{
			if (cast)
			{
				pulish_shield();
			}
		}
		break;

	case 22:
		if (name) return _("�S�[��������", "Create Golem");
		if (desc) return _("1�̂̃S�[�����𐻑�����B", "Creates a golem.");

		{
			if (cast)
			{
				if (summon_specific(-1, p_ptr->y, p_ptr->x, plev, SUMMON_GOLEM, PM_FORCE_PET))
				{
					msg_print(_("�S�[������������B", "You make a golem."));
				}
				else
				{
					msg_print(_("���܂��S�[���������Ȃ������B", "No Golems arrive."));
				}
			}
		}
		break;

	case 23:
		if (name) return _("���@�̊Z", "Magical armor");
		if (desc) return _("��莞�ԁA���@�h��͂�AC���オ��A�����ƖӖڂ̑ϐ��A���˔\�́A��გm�炸�A���V�𓾂�B",
			"Gives resistance to magic, bonus to AC, resistance to confusion, blindness, reflection, free action and levitation for a while.");

		{
			int base = 20;

			if (info) return info_duration(base, base);

			if (cast)
			{
				set_magicdef(randint1(base) + base, FALSE);
			}
		}
		break;

	case 24:
		if (name) return _("�������͉�", "Remove Enchantment");
		if (desc) return _("����E�h��ɂ�����ꂽ�����閂�͂����S�ɉ�������B", "Removes all magics completely from any weapon or armor.");

		{
			if (cast)
			{
				if (!mundane_spell(TRUE)) return NULL;
			}
		}
		break;

	case 25:
		if (name) return _("�􂢕���", "Remove All Curse");
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
		if (name) return _("���S�Ȃ�m��", "Knowledge True");
		if (desc) return _("�A�C�e���̎��\�͂����S�ɒm��B", "*Identifies* an item.");

		{
			if (cast)
			{
				if (!identify_fully(FALSE)) return NULL;
			}
		}
		break;

	case 27:
		if (name) return _("���틭��", "Enchant Weapon");
		if (desc) return _("����̖������C���ƃ_���[�W�C������������B", "Attempts to increase +to-hit, +to-dam of a weapon.");

		{
			if (cast)
			{
				if (!enchant_spell(randint0(4) + 1, randint0(4) + 1, 0)) return NULL;
			}
		}
		break;

	case 28:
		if (name) return _("�h���", "Enchant Armor");
		if (desc) return _("�Z�̖h��C������������B", "Attempts to increase +AC of an armor.");

		{
			if (cast)
			{
				if (!enchant_spell(0, 0, randint0(3) + 2)) return NULL;
			}
		}
		break;

	case 29:
		if (name) return _("���푮���t�^", "Brand Weapon");
		if (desc) return _("����Ƀ����_���ɑ���������B", "Makes current weapon a random ego weapon.");

		{
			if (cast)
			{
				brand_weapon(randint0(18));
			}
		}
		break;

	case 30:
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

	case 31:
		if (name) return _("�����ւ̖Ɖu", "Immunity");
		if (desc) return _("��莞�ԁA��C�A���A�d���A�_�̂����ꂩ�ɑ΂���Ɖu�𓾂�B",
			"Gives an immunity to fire, cold, electricity or acid for a while.");

		{
			int base = 13;

			if (info) return info_duration(base, base);

			if (cast)
			{
				if (!choose_ele_immune(base + randint1(base))) return NULL;
			}
		}
		break;
	}

	return "";
}
