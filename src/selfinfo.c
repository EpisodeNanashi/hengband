/*!
 * @file selfinfo.c
 * @brief ���ȕ��͏���/ Self knowledge
 * @date 2018/09/07
 * @author
 * <pre>
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research,
 * and not for profit purposes provided that this copyright and statement
 * are included in all such copies.  Other copyrights may also apply.
 * </pre>
 * 2018 Deskull
 * @details
 * spell2s.c���番��
 */

#include "angband.h"

/*!
 * @brief ���ȕ��͏���(Nethack����̃A�C�f�A) / self-knowledge... idea from nethack.
 * @return �Ȃ�
 * @details
 * <pre>
 * Useful for determining powers and
 * resistences of items.  It saves the screen, clears it, then starts listing
 * attributes, a screenful at a time.  (There are a LOT of attributes to
 * list.  It will probably take 2 or 3 screens for a powerful character whose
 * using several artifacts...) -CFT
 *
 * It is now a lot more efficient. -BEN-
 *
 * See also "identify_fully()".
 *
 * XXX XXX XXX Use the "show_file()" method, perhaps.
 * </pre>
 */
void self_knowledge(void)
{
	int i = 0, j, k;

	int v_nr = 0;
	char v_string [8] [128];
	char s_string [6] [128];

	u32b flgs[TR_FLAG_SIZE];

	object_type *o_ptr;

	char Dummy[80];
	char buf[2][80];

	cptr info[220];

	int plev = p_ptr->lev;

	int percent;

	for (j = 0; j < TR_FLAG_SIZE; j++)
		flgs[j] = 0L;

	p_ptr->knowledge |= (KNOW_STAT | KNOW_HPRATE);

	strcpy(Dummy, "");

	percent = (int)(((long)p_ptr->player_hp[PY_MAX_LEVEL - 1] * 200L) /
		(2 * p_ptr->hitdie +
		((PY_MAX_LEVEL - 1+3) * (p_ptr->hitdie + 1))));

	sprintf(Dummy, _("���݂̗̑̓����N : %d/100", "Your current Life Rating is %d/100."), percent);
	strcpy(buf[0], Dummy);
	info[i++] = buf[0];
	info[i++] = "";

	chg_virtue(V_KNOWLEDGE, 1);
	chg_virtue(V_ENLIGHTEN, 1);

	/* Acquire item flags from equipment */
	for (k = INVEN_RARM; k < INVEN_TOTAL; k++)
	{
		u32b tflgs[TR_FLAG_SIZE];

		o_ptr = &inventory[k];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Extract the flags */
		object_flags(o_ptr, tflgs);

		/* Extract flags */
		for (j = 0; j < TR_FLAG_SIZE; j++)
			flgs[j] |= tflgs[j];
	}

	info[i++] = _("�\�͂̍ő�l", "Limits of maximum stats");

	for (v_nr = 0; v_nr < 6; v_nr++)
	{
		char stat_desc[80];

		sprintf(stat_desc, "%s 18/%d", stat_names[v_nr], p_ptr->stat_max_max[v_nr]-18);

		strcpy(s_string[v_nr], stat_desc);

		info[i++] = s_string[v_nr];
	}
	info[i++] = "";

	sprintf(Dummy, _("���݂̑��� : %s(%ld)", "Your alighnment : %s(%ld)"), your_alignment(), (long int)p_ptr->align);
	strcpy(buf[1], Dummy);
	info[i++] = buf[1];
	for (v_nr = 0; v_nr < 8; v_nr++)
	{
		char vir_name [20];
		char vir_desc[80];
		int tester = p_ptr->virtues[v_nr];
	
		strcpy(vir_name, virtue[(p_ptr->vir_types[v_nr])-1]);
 
		sprintf(vir_desc, _("�����ƁB%s�̏��Ȃ��B", "Oops. No info about %s."), vir_name);
		if (tester < -100)
			sprintf(vir_desc, _("[%s]�̑΋� (%d)", "You are the polar opposite of %s (%d)."),
				vir_name, tester);
		else if (tester < -80)
			sprintf(vir_desc, _("[%s]�̑�G (%d)", "You are an arch-enemy of %s (%d)."),
				vir_name, tester);
		else if (tester < -60)
			sprintf(vir_desc, _("[%s]�̋��G (%d)", "You are a bitter enemy of %s (%d)."),
				vir_name, tester);
		else if (tester < -40)
			sprintf(vir_desc, _("[%s]�̓G (%d)", "You are an enemy of %s (%d)."),
				vir_name, tester);
		else if (tester < -20)
			sprintf(vir_desc, _("[%s]�̍ߎ� (%d)", "You have sinned against %s (%d)."),
				vir_name, tester);
		else if (tester < 0)
			sprintf(vir_desc, _("[%s]�̖����� (%d)", "You have strayed from the path of %s (%d)."),
				vir_name, tester);
		else if (tester == 0)
			sprintf(vir_desc, _("[%s]�̒����� (%d)", "You are neutral to %s (%d)."),
				vir_name, tester);
		else if (tester < 20)
			sprintf(vir_desc, _("[%s]�̏����� (%d)", "You are somewhat virtuous in %s (%d)."),
				vir_name, tester);
		else if (tester < 40)
			sprintf(vir_desc, _("[%s]�̒����� (%d)", "You are virtuous in %s (%d)."),
				vir_name, tester);
		else if (tester < 60)
			sprintf(vir_desc, _("[%s]�̍����� (%d)", "You are very virtuous in %s (%d)."),
				vir_name, tester);
		else if (tester < 80)
			sprintf(vir_desc, _("[%s]�̔e�� (%d)", "You are a champion of %s (%d)."),
				vir_name, tester);
		else if (tester < 100)
			sprintf(vir_desc, _("[%s]�̈̑�Ȕe�� (%d)", "You are a great champion of %s (%d)."),
				vir_name, tester);
		else
			sprintf(vir_desc, _("[%s]�̋�� (%d)", "You are the living embodiment of %s (%d)."),
				vir_name, tester);
	
		strcpy(v_string[v_nr], vir_desc);
	
		info[i++] = v_string[v_nr];
	}
	info[i++] = "";
	
	/* Racial powers... */
	if (p_ptr->mimic_form)
	{
		switch (p_ptr->mimic_form)
		{
			case MIMIC_DEMON:
			case MIMIC_DEMON_LORD:
			sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̒n�����Ή��̃u���X��f�����Ƃ��ł���B(%d MP)", 
							"You can nether breathe, dam. %d (cost %d)."), 3 * plev, 10+plev/3);

				info[i++] = Dummy;
			break;
		case MIMIC_VAMPIRE:
			if (plev > 1)
			{
				sprintf(Dummy, _("���Ȃ��͓G���� %d-%d HP �̐����͂��z���ł���B(%d MP)", 
								"You can steal life from a foe, dam. %d-%d (cost %d)."),
				    plev + MAX(1, plev / 10), plev + plev * MAX(1, plev / 10), 1 + (plev / 3));
				info[i++] = Dummy;
			}
			break;
		}
	}
	else
	{
	switch (p_ptr->prace)
	{
		case RACE_NIBELUNG:
		case RACE_DWARF:
			if (plev > 4)
			info[i++] = _("���Ȃ���㩂ƃh�A�ƊK�i�����m�ł���B(5 MP)", "You can find traps, doors and stairs (cost 5).");
			break;
		case RACE_HOBBIT:
			if (plev > 14)
			{
				info[i++] = _("���Ȃ��͐H���𐶐��ł���B(10 MP)", "You can produce food (cost 10).");
			}
			break;
		case RACE_GNOME:
			if (plev > 4)
			{
				sprintf(Dummy, _("���Ȃ��͔͈� %d �ȓ��Ƀe���|�[�g�ł���B(%d MP)", "You can teleport, range %d (cost %d)."),
				    (1 + plev), (5 + (plev / 5)));
				info[i++] = Dummy;
			}
			break;
		case RACE_HALF_ORC:
			if (plev > 2)
				info[i++] = _("���Ȃ��͋��|�������ł���B(5 MP)", "You can remove fear (cost 5).");
			break;
		case RACE_HALF_TROLL:
			if (plev > 9)
				info[i++] = _("���Ȃ��͋��\�����邱�Ƃ��ł���B(12 MP) ", "You enter berserk fury (cost 12).");
			break;
		case RACE_AMBERITE:
			if (plev > 29)
				info[i++] = _("���Ȃ��̓V���h�E�V�t�g���邱�Ƃ��ł���B(50 MP)", "You can Shift Shadows (cost 50).");

			if (plev > 39)
				info[i++] = _("���Ȃ��́u�p�^�[���v��S�ɕ`���ĕ������Ƃ��ł���B(75 MP)", "You can mentally Walk the Pattern (cost 75).");

			break;
		case RACE_BARBARIAN:
			if (plev > 7)
				info[i++] = _("���Ȃ��͋��\�����邱�Ƃ��ł���B(10 MP) ", "You can enter berserk fury (cost 10).");

			break;
		case RACE_HALF_OGRE:
			if (plev > 24)
				info[i++] = _("���Ȃ��͔����̃��[�����d�|���邱�Ƃ��ł���B(35 MP)", "You can set an Explosive Rune (cost 35).");

			break;
		case RACE_HALF_GIANT:
			if (plev > 19)
				info[i++] = _("���Ȃ��͐΂̕ǂ��󂷂��Ƃ��ł���B(10 MP)", "You can break stone walls (cost 10).");
			break;
		case RACE_HALF_TITAN:
			if (plev > 34)
				info[i++] = _("���Ȃ��̓����X�^�[���X�L�������邱�Ƃ��ł���B(20 MP)", "You can probe monsters (cost 20).");
			break;
		case RACE_CYCLOPS:
			if (plev > 19)
			{
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̊�΂𓊂��邱�Ƃ��ł���B(15 MP)", 
							 "You can throw a boulder, dam. %d (cost 15)."), 3 * plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_YEEK:
			if (plev > 14)
				info[i++] = _("���Ȃ��͋��|���ĂыN�������ѐ��𔭂��邱�Ƃ��ł���B(15 MP)", "You can make a terrifying scream (cost 15).");
			break;
		case RACE_KLACKON:
			if (plev > 8)
			{
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̎_�𐁂������邱�Ƃ��ł���B(9 MP)", "You can spit acid, dam. %d (cost 9)."), plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_KOBOLD:
			if (plev > 11)
			{
				sprintf(Dummy,
						_("���Ȃ��� %d �_���[�W�̓Ŗ�𓊂��邱�Ƃ��ł���B(8 MP)", "You can throw a dart of poison, dam. %d (cost 8)."), plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_DARK_ELF:
			if (plev > 1)
			{
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃}�W�b�N�E�~�T�C���̎������g����B(2 MP)", "You can cast a Magic Missile, dam %d (cost 2)."),
				    (3 + ((plev-1) / 5)));
				info[i++] = Dummy;
			}
			break;
		case RACE_DRACONIAN:
			sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃u���X��f�����Ƃ��ł���B(%d MP)", "You can breathe, dam. %d (cost %d)."), 2 * plev, plev);
			info[i++] = Dummy;
			break;
		case RACE_MIND_FLAYER:
			if (plev > 14)
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̐��_�U�������邱�Ƃ��ł���B(12 MP)", "You can mind blast your enemies, dam %d (cost 12)."), plev);
			info[i++] = Dummy;
			break;
		case RACE_IMP:
			if (plev > 29)
			{
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃t�@�C�A�E�{�[���̎������g����B(15 MP)", "You can cast a Fire Ball, dam. %d (cost 15)."), plev);
				info[i++] = Dummy;
			}
			else if (plev > 8)
			{
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃t�@�C�A�E�{���g�̎������g����B(15 MP)", "You can cast a Fire Bolt, dam. %d (cost 15)."), plev);
				info[i++] = Dummy;
			}
			break;
		case RACE_GOLEM:
			if (plev > 19)
				info[i++] = _("���Ȃ��� d20+30 �^�[���̊Ԕ���΂ɕω���������B(15 MP)", "You can turn your skin to stone, dur d20+30 (cost 15).");
			break;
		case RACE_ZOMBIE:
		case RACE_SKELETON:
			if (plev > 29)
				info[i++] = _("���Ȃ��͎������o���l���񕜂��邱�Ƃ��ł���B(30 MP)", "You can restore lost experience (cost 30).");
			break;
		case RACE_VAMPIRE:
			if (plev > 1)
			{
				sprintf(Dummy, _("���Ȃ��͓G���� %d-%d HP �̐����͂��z���ł���B(%d MP)", "You can steal life from a foe, dam. %d-%d (cost %d)."),
				    plev + MAX(1, plev / 10), plev + plev * MAX(1, plev / 10), 1 + (plev / 3));
				info[i++] = Dummy;
			}
			break;
		case RACE_SPECTRE:
			if (plev > 3)
			{
				info[i++] = _("���Ȃ��͋�������œG�����|�����邱�Ƃ��ł���B(3 MP)", "You can wail to terrify your enemies (cost 3).");
			}
			break;
		case RACE_SPRITE:
			if (plev > 11)
			{
				info[i++] = _("���Ȃ��͓G�𖰂点�閂�@�̕��𓊂��邱�Ƃ��ł���B(12 MP)", "You can throw magical dust which induces sleep (cost 12).");
			}
			break;
		case RACE_DEMON:
			sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̒n�����Ή��̃u���X��f�����Ƃ��ł���B(%d MP)", 
						"You can breathe nether, dam. %d (cost %d)."), 3 * plev, 10+plev/3);

			info[i++] = Dummy;
			break;
		case RACE_KUTAR:
			if (plev > 19)
				info[i++] = _("���Ȃ��� d20+30 �^�[���̊ԉ��ɐL�т邱�Ƃ��ł���B(15 MP)", "You can expand horizontally, dur d20+30 (cost 15).");
			break;
		case RACE_ANDROID:
			if (plev < 10)
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃��C�K���������Ƃ��ł���B(7 MP)", "You can fire a ray gun with damage %d (cost 7)."), 
					(plev + 1) / 2);
			else if (plev < 25)
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃u���X�^�[�������Ƃ��ł���B(13 MP)", "You can fire a blaster with damage %d (cost 13)."), plev);
			else if (plev < 35)
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃o�Y�[�J�������Ƃ��ł���B(26 MP)", "You can fire a bazooka with damage %d (cost 26)."), plev * 2);
			else if (plev < 45)
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃r�[���L���m���������Ƃ��ł���B(40 MP)", 
						"You can fire a beam cannon with damage %d (cost 40)."), plev * 2);
			else
				sprintf(Dummy, _("���Ȃ��� %d �_���[�W�̃��P�b�g�������Ƃ��ł���B(60 MP)", 
						"You can fire a rocket with damage %d (cost 60)."), plev * 5);

			info[i++] = Dummy;
			break;
		default:
			break;
	}
	}

	switch(p_ptr->pclass)
	{
		case CLASS_WARRIOR:
			if (plev > 39)
			{
				info[i++] = _("���Ȃ��̓����_���ȕ����ɑ΂��Đ���U�����邱�Ƃ��ł���B(75 MP)", 
							"You can attack some random directions at a time (cost 75).");
			}
			break;
		case CLASS_HIGH_MAGE:
			if (p_ptr->realm1 == REALM_HEX) break;
		case CLASS_MAGE:
		case CLASS_SORCERER:
			if (plev > 24)
			{
				info[i++] = _("���Ȃ��̓A�C�e���̖��͂��z�����邱�Ƃ��ł���B(1 MP)", 
						"You can absorb charges from an item (cost 1).");
			}
			break;
		case CLASS_PRIEST:
			if (is_good_realm(p_ptr->realm1))
			{
				if (plev > 34)
				{
					info[i++] = _("���Ȃ��͕�����j�����邱�Ƃ��ł���B(70 MP)", "You can bless a weapon (cost 70).");
				}
			}
			else
			{
				if (plev > 41)
				{
					info[i++] = _("���Ȃ��͎���̂��ׂẴ����X�^�[���U�����邱�Ƃ��ł���B(40 MP)", "You can damages all monsters in sight (cost 40).");
				}
			}
			break;
		case CLASS_ROGUE:
			if (plev > 7)
			{
				info[i++] = _("���Ȃ��͍U�����đ����ɓ����邱�Ƃ��ł���B(12 MP)", "You can hit a monster and teleport at a time (cost 12).");
			}
			break;
		case CLASS_RANGER:
			if (plev > 14)
			{
				info[i++] = _("���Ȃ��͉����𒲍����邱�Ƃ��ł���B(20 MP)", "You can prove monsters (cost 20).");
			}
			break;
		case CLASS_PALADIN:
			if (is_good_realm(p_ptr->realm1))
			{
				if (plev > 29)
				{
					info[i++] = _("���Ȃ��͐��Ȃ鑄������Ƃ��ł���B(30 MP)", "You can fires a holy spear (cost 30).");
				}
			}
			else
			{
				if (plev > 29)
				{
					info[i++] = _("���Ȃ��͐����͂����������鑄������Ƃ��ł���B(30 MP)", "You can fires a spear which drains vitality (cost 30).");
				}
			}
			break;
		case CLASS_WARRIOR_MAGE:
			if (plev > 24)
			{
				info[i++] = _("���Ȃ��͂g�o���l�o�ɕϊ����邱�Ƃ��ł���B(0 MP)", "You can convert HP to SP (cost 0).");
				info[i++] = _("���Ȃ��͂l�o���g�o�ɕϊ����邱�Ƃ��ł���B(0 MP)", "You can convert SP to HP (cost 0).");
			}
			break;
		case CLASS_CHAOS_WARRIOR:
			if (plev > 39)
			{
				info[i++] = _("���Ȃ��͎��͂ɉ�����f�킷���𔭐������邱�Ƃ��ł���B(50 MP)",
						"You can radiate light which confuses nearby monsters (cost 50).");
			}
			break;
		case CLASS_MONK:
			if (plev > 24)
			{
				info[i++] = _("���Ȃ��͍\���邱�Ƃ��ł���B(0 MP)", "You can assume a posture of special form (cost 0).");
			}
			if (plev > 29)
			{
				info[i++] = _("���Ȃ��͒ʏ��2�{�̍U�����s�����Ƃ��ł���B(30 MP)", "You can perform double attacks in a time (cost 30).");
			}
			break;
		case CLASS_MINDCRAFTER:
		case CLASS_FORCETRAINER:
			if (plev > 14)
			{
				info[i++] = _("���Ȃ��͐��_���W�����Ăl�o���񕜂����邱�Ƃ��ł���B(0 MP)", "You can concentrate to regenerate your mana (cost 0).");
			}
			break;
		case CLASS_TOURIST:
			info[i++] = _("���Ȃ��͎ʐ^���B�e���邱�Ƃ��ł���B(0 MP)", "You can take a photograph (cost 0).");
			if (plev > 24)
			{
				info[i++] = _("���Ȃ��̓A�C�e�������S�ɊӒ肷�邱�Ƃ��ł���B(20 MP)", "You can *identify* items (cost 20).");
			}
			break;
		case CLASS_IMITATOR:
			if (plev > 29)
			{
				info[i++] = _("���Ȃ��͉����̓���U�����_���[�W2�{�ł܂˂邱�Ƃ��ł���B(100 MP)", 
						"You can imitate monster's special attacks with double damage (cost 100).");
			}
			break;
		case CLASS_BEASTMASTER:
			info[i++] = _("���Ȃ���1�̂̐����̂��郂���X�^�[���x�z���邱�Ƃ��ł���B(���x��/4 MP)", "You can dominate a monster (cost level/4).");
			if (plev > 29)
			{
				info[i++] = _("���Ȃ��͎��E���̐����̂��郂���X�^�[���x�z���邱�Ƃ��ł���B((���x��+20)/2 MP)", 
						"You can dominate living monsters in sight (cost (level+20)/4).");
			}
			break;
		case CLASS_MAGIC_EATER:
			info[i++] = _("���Ȃ��͏�/���@�_/���b�h�̖��͂������̂��̂ɂ��邱�Ƃ��ł���B", "You can absorb a staff, wand or rod itself.");
			break;
		case CLASS_RED_MAGE:
			if (plev > 47)
			{
				info[i++] = _("���Ȃ���1�^�[����2�񖂖@�������邱�Ƃ��ł���B(20 MP)", "You can cast two spells in one time (cost 20).");
			}
			break;
		case CLASS_SAMURAI:
			{
				info[i++] = _("���Ȃ��͐��_���W�����ċC�����𗭂߂邱�Ƃ��ł���B", "You can concentrate to regenerate your mana.");
			}
			if (plev > 24)
			{
				info[i++] = _("���Ȃ��͓���Ȍ^�ō\���邱�Ƃ��ł���B", "You can assume a posture of special form.");
			}
			break;
		case CLASS_BLUE_MAGE:
			info[i++] = _("���Ȃ��͑���Ɏg��ꂽ���@���w�Ԃ��Ƃ��ł���B", "You can study spells which your enemy casts on you.");
			break;
		case CLASS_CAVALRY:
			if (plev > 9)
			{
				info[i++] = _("���Ȃ��̓����X�^�[�ɏ���Ė�����y�b�g�ɂ��邱�Ƃ��ł���B", "You can ride on a hostile monster forcibly to turn it into pet.");
			}
			break;
		case CLASS_BERSERKER:
			if (plev > 9)
			{
				info[i++] = _("���Ȃ��͊X�ƃ_���W�����̊Ԃ��s�������邱�Ƃ��ł���B", "You can travel between town and the depths.");
			}
			break;
		case CLASS_MIRROR_MASTER:
			info[i++] = _("���Ȃ��͋������o�����Ƃ��ł���B(2 MP)", "You can create a Mirror (cost 2).");
			info[i++] = _("���Ȃ��͋������邱�Ƃ��ł���B(0 MP)", "You can break distant Mirrors (cost 0).");
			break;
		case CLASS_NINJA:
			if (plev > 19)
			{
				info[i++] = _("���Ȃ��͑f�����ړ����邱�Ƃ��ł���B", "You can walk extremery fast.");
			}
			break;
	}

	if (p_ptr->muta1)
	{
		if (p_ptr->muta1 & MUT1_SPIT_ACID)
		{
			info[i++] = _("���Ȃ��͎_�𐁂������邱�Ƃ��ł���B(�_���[�W ���x��X1)", "You can spit acid (dam lvl).");
		}
		if (p_ptr->muta1 & MUT1_BR_FIRE)
		{
			info[i++] = _("���Ȃ��͉��̃u���X��f�����Ƃ��ł���B(�_���[�W ���x��X2)", "You can breathe fire (dam lvl * 2).");
		}
		if (p_ptr->muta1 & MUT1_HYPN_GAZE)
		{
			info[i++] = _("���Ȃ����ɂ݂͍Ö����ʂ����B", "Your gaze is hypnotic.");
		}
		if (p_ptr->muta1 & MUT1_TELEKINES)
		{
			info[i++] = _("���Ȃ��͔O���͂������Ă���B", "You are telekinetic.");
		}
		if (p_ptr->muta1 & MUT1_VTELEPORT)
		{
			info[i++] = _("���Ȃ��͎����̈ӎv�Ńe���|�[�g�ł���B", "You can teleport at will.");
		}
		if (p_ptr->muta1 & MUT1_MIND_BLST)
		{
			info[i++] = _("���Ȃ��͐��_�U�����s����B(�_���[�W 3�`12d3)", "You can Mind Blast your enemies (3 to 12d3 dam).");
		}
		if (p_ptr->muta1 & MUT1_RADIATION)
		{
			info[i++] = _("���Ȃ��͎����̈ӎv�ŋ������ː��𔭐����邱�Ƃ��ł���B(�_���[�W ���x��X2)", "You can emit hard radiation at will (dam lvl * 2).");
		}
		if (p_ptr->muta1 & MUT1_VAMPIRISM)
		{
			info[i++] = _("���Ȃ��͋z���S�̂悤�ɓG���琶���͂��z�����邱�Ƃ��ł���B(�_���[�W ���x��X2)", 
						"You can drain life from a foe like a vampire (dam lvl * 2).");
		}
		if (p_ptr->muta1 & MUT1_SMELL_MET)
		{
			info[i++] = _("���Ȃ��͋߂��ɂ���M���������������邱�Ƃ��ł���B", "You can smell nearby precious metal.");
		}
		if (p_ptr->muta1 & MUT1_SMELL_MON)
		{
			info[i++] = _("���Ȃ��͋߂��̃����X�^�[�̑��݂����������邱�Ƃ��ł���B", "You can smell nearby monsters.");
		}
		if (p_ptr->muta1 & MUT1_BLINK)
		{
			info[i++] = _("���Ȃ��͒Z���������e���|�[�g�ł���B", "You can teleport yourself short distances.");
		}
		if (p_ptr->muta1 & MUT1_EAT_ROCK)
		{
			info[i++] = _("���Ȃ��͍d�����H�ׂ邱�Ƃ��ł���B", "You can consume solid rock.");
		}
		if (p_ptr->muta1 & MUT1_SWAP_POS)
		{
			info[i++] = _("���Ȃ��͑��̎҂Əꏊ�����ւ�邱�Ƃ��ł���B", "You can switch locations with another being.");
		}
		if (p_ptr->muta1 & MUT1_SHRIEK)
		{
			info[i++] = _("���Ȃ��͐g�̖т��悾���ѐ��𔭂��邱�Ƃ��ł���B(�_���[�W ���x��X2)", "You can emit a horrible shriek (dam 2 * lvl).");
		}
		if (p_ptr->muta1 & MUT1_ILLUMINE)
		{
			info[i++] = _("���Ȃ��͖��邢��������Ƃ��ł���B", "You can emit bright light.");
		}
		if (p_ptr->muta1 & MUT1_DET_CURSE)
		{
			info[i++] = _("���Ȃ��͎׈��Ȗ��@�̊댯�������Ƃ邱�Ƃ��ł���B", "You can feel the danger of evil magic.");
		}
		if (p_ptr->muta1 & MUT1_BERSERK)
		{
			info[i++] = _("���Ȃ��͎����̈ӎv�ŋ����퓬��ԂɂȂ邱�Ƃ��ł���B", "You can drive yourself into a berserk frenzy.");
		}
		if (p_ptr->muta1 & MUT1_POLYMORPH)
		{
			info[i++] = _("���Ȃ��͎����̈ӎu�ŕω��ł���B", "You can polymorph yourself at will.");
		}
		if (p_ptr->muta1 & MUT1_MIDAS_TCH)
		{
			info[i++] = _("���Ȃ��͒ʏ�A�C�e�������ɕς��邱�Ƃ��ł���B", "You can turn ordinary items to gold.");
		}
		if (p_ptr->muta1 & MUT1_GROW_MOLD)
		{
			info[i++] = _("���Ȃ��͎��͂ɃL�m�R�𐶂₷���Ƃ��ł���B", "You can cause mold to grow near you.");
		}
		if (p_ptr->muta1 & MUT1_RESIST)
		{
			info[i++] = _("���Ȃ��͌��f�̍U���ɑ΂��Đg���d�����邱�Ƃ��ł���B", "You can harden yourself to the ravages of the elements.");
		}
		if (p_ptr->muta1 & MUT1_EARTHQUAKE)
		{
			info[i++] = _("���Ȃ��͎��͂̃_���W��������󂳂��邱�Ƃ��ł���B", "You can bring down the dungeon around your ears.");
		}
		if (p_ptr->muta1 & MUT1_EAT_MAGIC)
		{
			info[i++] = _("���Ȃ��͖��@�̃G�l���M�[�������̕��Ƃ��Ďg�p�ł���B", "You can consume magic energy for your own use.");
		}
		if (p_ptr->muta1 & MUT1_WEIGH_MAG)
		{
			info[i++] = _("���Ȃ��͎����ɉe����^���閂�@�̗͂������邱�Ƃ��ł���B", "You can feel the strength of the magics affecting you.");
		}
		if (p_ptr->muta1 & MUT1_STERILITY)
		{
			info[i++] = _("���Ȃ��͏W�c�I���B�s�\���N�������Ƃ��ł���B", "You can cause mass impotence.");
		}
		if (p_ptr->muta1 & MUT1_PANIC_HIT)
		{
			info[i++] = _("���Ȃ��͍U��������g����邽�ߓ����邱�Ƃ��ł���B", "You can run for your life after hitting something.");
		}
		if (p_ptr->muta1 & MUT1_DAZZLE)
		{
			info[i++] = _("���Ȃ��͍����ƖӖڂ������N�������˔\�𔭐����邱�Ƃ��ł���B ", "You can emit confusing, blinding radiation.");
		}
		if (p_ptr->muta1 & MUT1_LASER_EYE)
		{
			info[i++] = _("���Ȃ��͖ڂ��烌�[�U�[�����𔭂��邱�Ƃ��ł���B(�_���[�W ���x��X2)", "Your eyes can fire laser beams (dam 2 * lvl).");
		}
		if (p_ptr->muta1 & MUT1_RECALL)
		{
			info[i++] = _("���Ȃ��͊X�ƃ_���W�����̊Ԃ��s�������邱�Ƃ��ł���B", "You can travel between town and the depths.");
		}
		if (p_ptr->muta1 & MUT1_BANISH)
		{
			info[i++] = _("���Ȃ��͎׈��ȃ����X�^�[��n���ɗ��Ƃ����Ƃ��ł���B", "You can send evil creatures directly to Hell.");
		}
		if (p_ptr->muta1 & MUT1_COLD_TOUCH)
		{
			info[i++] = _("���Ȃ��͓G��G���ē��点�邱�Ƃ��ł���B(�_���[�W ���x��X3)", "You can freeze things with a touch (dam 3 * lvl).");
		}
		if (p_ptr->muta1 & MUT1_LAUNCHER)
		{
			info[i++] = _("���Ȃ��̓A�C�e����͋��������邱�Ƃ��ł���B", "You can hurl objects with great force.");
		}
	}

	if (p_ptr->muta2)
	{
		if (p_ptr->muta2 & MUT2_BERS_RAGE)
		{
			info[i++] = _("���Ȃ��͋���m���̔�����N�����B", "You are subject to berserker fits.");
		}
		if (p_ptr->muta2 & MUT2_COWARDICE)
		{
			info[i++] = _("���Ȃ��͎��X���a�ɂȂ�B", "You are subject to cowardice.");
		}
		if (p_ptr->muta2 & MUT2_RTELEPORT)
		{
			info[i++] = _("���Ȃ��̓����_���Ƀe���|�[�g����B", "You are teleporting randomly.");
		}
		if (p_ptr->muta2 & MUT2_ALCOHOL)
		{
			info[i++] = _("���Ȃ��̑̂̓A���R�[���𕪔傷��B", "Your body produces alcohol.");
		}
		if (p_ptr->muta2 & MUT2_HALLU)
		{
			info[i++] = _("���Ȃ��͌��o�������N�������_�����ɐN����Ă���B", "You have a hallucinatory insanity.");
		}
		if (p_ptr->muta2 & MUT2_FLATULENT)
		{
			info[i++] = _("���Ȃ��͐���ł��Ȃ�����ț��������B", "You are subject to uncontrollable flatulence.");
		}
		if (p_ptr->muta2 & MUT2_PROD_MANA)
		{
			info[i++] = _("���Ȃ��͐���s�\�Ȗ��@�̃G�l���M�[�𔭂��Ă���B", "You are producing magical energy uncontrollably.");
		}
		if (p_ptr->muta2 & MUT2_ATT_DEMON)
		{
			info[i++] = _("���Ȃ��̓f�[��������������B", "You attract demons.");
		}
		if (p_ptr->muta2 & MUT2_SCOR_TAIL)
		{
			info[i++] = _("���Ȃ��̓T�\���̐K���������Ă���B(�ŁA�_���[�W 3d7)", "You have a scorpion tail (poison, 3d7).");
		}
		if (p_ptr->muta2 & MUT2_HORNS)
		{
			info[i++] = _("���Ȃ��͊p�������Ă���B(�_���[�W 2d6)", "You have horns (dam. 2d6).");
		}
		if (p_ptr->muta2 & MUT2_BEAK)
		{
			info[i++] = _("���Ȃ��̓N�`�o�V�������Ă���B(�_���[�W 2d4)", "You have a beak (dam. 2d4).");
		}
		if (p_ptr->muta2 & MUT2_SPEED_FLUX)
		{
			info[i++] = _("���Ȃ��̓����_���ɑ�����������x���������肷��B", "You move faster or slower randomly.");
		}
		if (p_ptr->muta2 & MUT2_BANISH_ALL)
		{
			info[i++] = _("���Ȃ��͎��X�߂��̃����X�^�[�����ł�����B", "You sometimes cause nearby creatures to vanish.");
		}
		if (p_ptr->muta2 & MUT2_EAT_LIGHT)
		{
			info[i++] = _("���Ȃ��͎��X���͂̌����z�����ĉh�{�ɂ���B", "You sometimes feed off of the light around you.");
		}
		if (p_ptr->muta2 & MUT2_TRUNK)
		{
			info[i++] = _("���Ȃ��͏ۂ̂悤�ȕ@�������Ă���B(�_���[�W 1d4)", "You have an elephantine trunk (dam 1d4).");
		}
		if (p_ptr->muta2 & MUT2_ATT_ANIMAL)
		{
			info[i++] = _("���Ȃ��͓�������������B", "You attract animals.");
		}
		if (p_ptr->muta2 & MUT2_TENTACLES)
		{
			info[i++] = _("���Ȃ��͎׈��ȐG��������Ă���B(�_���[�W 2d5)", "You have evil looking tentacles (dam 2d5).");
		}
		if (p_ptr->muta2 & MUT2_RAW_CHAOS)
		{
			info[i++] = _("���Ȃ��͂��΂��Ώ��J�I�X�ɕ�܂��B", "You occasionally are surrounded with raw chaos.");
		}
		if (p_ptr->muta2 & MUT2_NORMALITY)
		{
			info[i++] = _("���Ȃ��͕ψق��Ă������A�񕜂��Ă��Ă���B", "You may be mutated, but you're recovering.");
		}
		if (p_ptr->muta2 & MUT2_WRAITH)
		{
			info[i++] = _("���Ȃ��̓��̂͗H�̉���������̉������肷��B", "You fade in and out of physical reality.");
		}
		if (p_ptr->muta2 & MUT2_POLY_WOUND)
		{
			info[i++] = _("���Ȃ��̌��N�̓J�I�X�̗͂ɉe�����󂯂�B", "Your health is subject to chaotic forces.");
		}
		if (p_ptr->muta2 & MUT2_WASTING)
		{
			info[i++] = _("���Ȃ��͐��シ�鋰�낵���a�C�ɂ������Ă���B", "You have a horrible wasting disease.");
		}
		if (p_ptr->muta2 & MUT2_ATT_DRAGON)
		{
			info[i++] = _("���Ȃ��̓h���S������������B", "You attract dragons.");
		}
		if (p_ptr->muta2 & MUT2_WEIRD_MIND)
		{
			info[i++] = _("���Ȃ��̐��_�̓����_���Ɋg�債����k�������肵�Ă���B", "Your mind randomly expands and contracts.");
		}
		if (p_ptr->muta2 & MUT2_NAUSEA)
		{
			info[i++] = _("���Ȃ��݂͔̈��ɗ����������Ȃ��B", "You have a seriously upset stomach.");
		}
		if (p_ptr->muta2 & MUT2_CHAOS_GIFT)
		{
			info[i++] = _("���Ȃ��̓J�I�X�̎�숫������J���������Ƃ�B", "Chaos deities give you gifts.");
		}
		if (p_ptr->muta2 & MUT2_WALK_SHAD)
		{
			info[i++] = _("���Ȃ��͂��΂��Α��́u�e�v�ɖ������ށB", "You occasionally stumble into other shadows.");
		}
		if (p_ptr->muta2 & MUT2_WARNING)
		{
			info[i++] = _("���Ȃ��͓G�Ɋւ���x����������B", "You receive warnings about your foes.");
		}
		if (p_ptr->muta2 & MUT2_INVULN)
		{
			info[i++] = _("���Ȃ��͎��X�����m�炸�ȋC���ɂȂ�B", "You occasionally feel invincible.");
		}
		if (p_ptr->muta2 & MUT2_SP_TO_HP)
		{
			info[i++] = _("���Ȃ��͎��X�����ؓ��ɂǂ��Ɨ����B", "Your blood sometimes rushes to your muscles.");
		}
		if (p_ptr->muta2 & MUT2_HP_TO_SP)
		{
			info[i++] = _("���Ȃ��͎��X���Ɍ����ǂ��Ɨ����B", "Your blood sometimes rushes to your head.");
		}
		if (p_ptr->muta2 & MUT2_DISARM)
		{
			info[i++] = _("���Ȃ��͂悭�܂Â��ĕ��𗎂Ƃ��B", "You occasionally stumble and drop things.");
		}
	}

	if (p_ptr->muta3)
	{
		if (p_ptr->muta3 & MUT3_HYPER_STR)
		{
			info[i++] = _("���Ȃ��͒��l�I�ɋ����B(�r��+4)", "You are superhumanly strong (+4 STR).");
		}
		if (p_ptr->muta3 & MUT3_PUNY)
		{
			info[i++] = _("���Ȃ��͋��ゾ�B(�r��-4)", "You are puny (-4 STR).");
		}
		if (p_ptr->muta3 & MUT3_HYPER_INT)
		{
			info[i++] = _("���Ȃ��̔]�͐��̃R���s���[�^���B(�m�\������+4)", "Your brain is a living computer (+4 INT/WIS).");
		}
		if (p_ptr->muta3 & MUT3_MORONIC)
		{
			info[i++] = _("���Ȃ��͐��_���ゾ�B(�m�\������-4)", "You are moronic (-4 INT/WIS).");
		}
		if (p_ptr->muta3 & MUT3_RESILIENT)
		{
			info[i++] = _("���Ȃ��͔��Ƀ^�t���B(�ϋv+4)", "You are very resilient (+4 CON).");
		}
		if (p_ptr->muta3 & MUT3_XTRA_FAT)
		{
			info[i++] = _("���Ȃ��͋ɒ[�ɑ����Ă���B(�ϋv+2,�X�s�[�h-2)", "You are extremely fat (+2 CON, -2 speed).");
		}
		if (p_ptr->muta3 & MUT3_ALBINO)
		{
			info[i++] = _("���Ȃ��̓A���r�m���B(�ϋv-4)", "You are albino (-4 CON).");
		}
		if (p_ptr->muta3 & MUT3_FLESH_ROT)
		{
			info[i++] = _("���Ȃ��̓��͕̂��s���Ă���B(�ϋv-2,����-1)", "Your flesh is rotting (-2 CON, -1 CHR).");
		}
		if (p_ptr->muta3 & MUT3_SILLY_VOI)
		{
			info[i++] = _("���Ȃ��̐��͊Ԕ����ȃL�[�L�[�����B(����-4)", "Your voice is a silly squeak (-4 CHR).");
		}
		if (p_ptr->muta3 & MUT3_BLANK_FAC)
		{
			info[i++] = _("���Ȃ��͂̂��؂�ڂ����B(����-1)", "Your face is featureless (-1 CHR).");
		}
		if (p_ptr->muta3 & MUT3_ILL_NORM)
		{
			info[i++] = _("���Ȃ��͌��e�ɕ����Ă���B", "Your appearance is masked with illusion.");
		}
		if (p_ptr->muta3 & MUT3_XTRA_EYES)
		{
			info[i++] = _("���Ȃ��͗]���ɓ�̖ڂ������Ă���B(�T��+15)", "You have an extra pair of eyes (+15 search).");
		}
		if (p_ptr->muta3 & MUT3_MAGIC_RES)
		{
			info[i++] = _("���Ȃ��͖��@�ւ̑ϐ��������Ă���B", "You are resistant to magic.");
		}
		if (p_ptr->muta3 & MUT3_XTRA_NOIS)
		{
			info[i++] = _("���Ȃ��͕ςȉ��𔭂��Ă���B(�B��-3)", "You make a lot of strange noise (-3 stealth).");
		}
		if (p_ptr->muta3 & MUT3_INFRAVIS)
		{
			info[i++] = _("���Ȃ��͑f���炵���ԊO�����͂������Ă���B(+3)", "You have remarkable infravision (+3).");
		}
		if (p_ptr->muta3 & MUT3_XTRA_LEGS)
		{
			info[i++] = _("���Ȃ��͗]���ɓ�{�̑��������Ă���B(����+3)", "You have an extra pair of legs (+3 speed).");
		}
		if (p_ptr->muta3 & MUT3_SHORT_LEG)
		{
			info[i++] = _("���Ȃ��̑��͒Z���ˋN���B(����-3)", "Your legs are short stubs (-3 speed).");
		}
		if (p_ptr->muta3 & MUT3_ELEC_TOUC)
		{
			info[i++] = _("���Ȃ��̌��ǂɂ͓d��������Ă���B", "Electricity is running through your veins.");
		}
		if (p_ptr->muta3 & MUT3_FIRE_BODY)
		{
			info[i++] = _("���Ȃ��͉̑̂��ɂ܂�Ă���B", "Your body is enveloped in flames.");
		}
		if (p_ptr->muta3 & MUT3_WART_SKIN)
		{
			info[i++] = _("���Ȃ��̔��̓C�{�ɔ���Ă���B(����-2, AC+5)", "Your skin is covered with warts (-2 CHR, +5 AC).");
		}
		if (p_ptr->muta3 & MUT3_SCALES)
		{
			info[i++] = _("���Ȃ��̔��͗؂ɂȂ��Ă���B(����-1, AC+10)", "Your skin has turned into scales (-1 CHR, +10 AC).");
		}
		if (p_ptr->muta3 & MUT3_IRON_SKIN)
		{
			info[i++] = _("���Ȃ��̔��͓S�łł��Ă���B(��p-1, AC+25)", "Your skin is made of steel (-1 DEX, +25 AC).");
		}
		if (p_ptr->muta3 & MUT3_WINGS)
		{
			info[i++] = _("���Ȃ��͉H�������Ă���B", "You have wings.");
		}
		if (p_ptr->muta3 & MUT3_FEARLESS)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_REGEN)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_ESP)
		{
			/* Unnecessary */
		}
		if (p_ptr->muta3 & MUT3_LIMBER)
		{
			info[i++] = _("���Ȃ��͔̑̂��ɂ��Ȃ₩���B(��p+3)", "Your body is very limber (+3 DEX).");
		}
		if (p_ptr->muta3 & MUT3_ARTHRITIS)
		{
			info[i++] = _("���Ȃ��͂����֐߂ɒɂ݂������Ă���B(��p-3)", "Your joints ache constantly (-3 DEX).");
		}
		if (p_ptr->muta3 & MUT3_VULN_ELEM)
		{
			info[i++] = _("���Ȃ��͌��f�̍U���Ɏア�B", "You are susceptible to damage from the elements.");
		}
		if (p_ptr->muta3 & MUT3_MOTION)
		{
			info[i++] = _("���Ȃ��̓���͐��m�ŗ͋����B(�B��+1)", "Your movements are precise and forceful (+1 STL).");
		}
		if (p_ptr->muta3 & MUT3_GOOD_LUCK)
		{
			info[i++] = _("���Ȃ��͔����I�[���ɂ܂�Ă���B", "There is a white aura surrounding you.");
		}
		if (p_ptr->muta3 & MUT3_BAD_LUCK)
		{
			info[i++] = _("���Ȃ��͍����I�[���ɂ܂�Ă���B", "There is a black aura surrounding you.");
		}
	}

	if (p_ptr->blind)
	{
		info[i++] = _("���Ȃ��͖ڂ������Ȃ��B", "You cannot see.");
	}
	if (p_ptr->confused)
	{
		info[i++] = _("���Ȃ��͍������Ă���B", "You are confused.");
	}
	if (p_ptr->afraid)
	{
		info[i++] = _("���Ȃ��͋��|�ɐN����Ă���B", "You are terrified.");
	}
	if (p_ptr->cut)
	{
		info[i++] = _("���Ȃ��͏o�����Ă���B", "You are bleeding.");
	}
	if (p_ptr->stun)
	{
		info[i++] = _("���Ȃ��͂����낤�Ƃ��Ă���B", "You are stunned.");
	}
	if (p_ptr->poisoned)
	{
		info[i++] = _("���Ȃ��͓łɐN����Ă���B", "You are poisoned.");
	}
	if (p_ptr->image)
	{
		info[i++] = _("���Ȃ��͌��o�����Ă���B", "You are hallucinating.");
	}
	if (p_ptr->cursed & TRC_TY_CURSE)
	{
		info[i++] = _("���Ȃ��͎׈��ȉ��O�ɕ�܂�Ă���B", "You carry an ancient foul curse.");
	}
	if (p_ptr->cursed & TRC_AGGRAVATE)
	{
		info[i++] = _("���Ȃ��̓����X�^�[��{�点�Ă���B", "You aggravate monsters.");
	}
	if (p_ptr->cursed & TRC_DRAIN_EXP)
	{
		info[i++] = _("���Ȃ��͌o���l���z���Ă���B", "You are drained.");
	}
	if (p_ptr->cursed & TRC_SLOW_REGEN)
	{
		info[i++] = _("���Ȃ��̉񕜗͔͂��ɒx���B", "You regenerate slowly.");
	}
	if (p_ptr->cursed & TRC_ADD_L_CURSE)
	{
		info[i++] = _("���Ȃ��̎ア�􂢂͑�����B","Your weak curses multiply."); /* �b��I -- henkma */
	}
	if (p_ptr->cursed & TRC_ADD_H_CURSE)
	{
		info[i++] = _("���Ȃ��̋����􂢂͑�����B","Your heavy curses multiply."); /* �b��I -- henkma */
	}
	if (p_ptr->cursed & TRC_CALL_ANIMAL)
	{
		info[i++] = _("���Ȃ��͓����ɑ_���Ă���B", "You attract animals.");
	}
	if (p_ptr->cursed & TRC_CALL_DEMON)
	{
		info[i++] = _("���Ȃ��͈����ɑ_���Ă���B", "You attract demons.");
	}
	if (p_ptr->cursed & TRC_CALL_DRAGON)
	{
		info[i++] = _("���Ȃ��̓h���S���ɑ_���Ă���B", "You attract dragons.");
	}
	if (p_ptr->cursed & TRC_COWARDICE)
	{
		info[i++] = _("���Ȃ��͎��X���a�ɂȂ�B", "You are subject to cowardice.");
	}
	if (p_ptr->cursed & TRC_TELEPORT)
	{
		info[i++] = _("���Ȃ��̈ʒu�͂Ђ��傤�ɕs���肾�B", "Your position is very uncertain.");
	}
	if (p_ptr->cursed & TRC_LOW_MELEE)
	{
		info[i++] = _("���Ȃ��̕���͍U�����O���₷���B", "Your weapon causes you to miss blows.");
	}
	if (p_ptr->cursed & TRC_LOW_AC)
	{
		info[i++] = _("���Ȃ��͍U�����󂯂₷���B", "You are subject to be hit.");
	}
	if (p_ptr->cursed & TRC_LOW_MAGIC)
	{
		info[i++] = _("���Ȃ��͖��@�����s���₷���B", "You are subject to fail spellcasting.");
	}
	if (p_ptr->cursed & TRC_FAST_DIGEST)
	{
		info[i++] = _("���Ȃ��͂����������ւ�B", "You have a good appetite.");
	}
	if (p_ptr->cursed & TRC_DRAIN_HP)
	{
		info[i++] = _("���Ȃ��̗͑͂��z���Ă���B", "You are drained.");
	}
	if (p_ptr->cursed & TRC_DRAIN_MANA)
	{
		info[i++] = _("���Ȃ��͖��͂��z���Ă���B", "You brain is drained.");
	}
	if (IS_BLESSED())
	{
		info[i++] = _("���Ȃ��͍������������Ă���B", "You feel rightous.");
	}
	if (IS_HERO())
	{
		info[i++] = _("���Ȃ��̓q�[���[�C�����B", "You feel heroic.");
	}
	if (p_ptr->shero)
	{
		info[i++] = _("���Ȃ��͐퓬�����B", "You are in a battle rage.");
	}
	if (p_ptr->protevil)
	{
		info[i++] = _("���Ȃ��͎׈��Ȃ鑶�݂������Ă���B", "You are protected from evil.");
	}
	if (p_ptr->shield)
	{
		info[i++] = _("���Ȃ��͐_��̃V�[���h�Ŏ���Ă���B", "You are protected by a mystic shield.");
	}
	if (IS_INVULN())
	{
		info[i++] = _("���Ȃ��͌��ݏ����Ȃ��B", "You are temporarily invulnerable.");
	}
	if (p_ptr->wraith_form)
	{
		info[i++] = _("���Ȃ��͈ꎞ�I�ɗH�̉����Ă���B", "You are temporarily incorporeal.");
	}
	if (p_ptr->special_attack & ATTACK_CONFUSE)
	{
		info[i++] = _("���Ȃ��̎�͐Ԃ��P���Ă���B", "Your hands are glowing dull red.");
	}
	if (p_ptr->special_attack & ATTACK_FIRE)
	{
		info[i++] = _("���Ȃ��̎�͉Ή��ɕ����Ă���B", "You can strike the enemy with flame.");
	}
	if (p_ptr->special_attack & ATTACK_COLD)
	{
		info[i++] = _("���Ȃ��̎�͗�C�ɕ����Ă���B", "You can strike the enemy with cold.");
	}
	if (p_ptr->special_attack & ATTACK_ACID)
	{
		info[i++] = _("���Ȃ��̎�͎_�ɕ����Ă���B", "You can strike the enemy with acid.");
	}
	if (p_ptr->special_attack & ATTACK_ELEC)
	{
		info[i++] = _("���Ȃ��̎�͓d���ɕ����Ă���B", "You can strike the enemy with electoric shock.");
	}
	if (p_ptr->special_attack & ATTACK_POIS)
	{
		info[i++] = _("���Ȃ��̎�͓łɕ����Ă���B", "You can strike the enemy with poison.");
	}
	switch (p_ptr->action)
	{
		case ACTION_SEARCH:
			info[i++] = _("���Ȃ��͂Ђ��傤�ɒ��Ӑ[�����͂����n���Ă���B", "You are looking around very carefully.");
			break;
	}
	if (p_ptr->new_spells)
	{
		info[i++] = _("���Ȃ��͎�����F����w�Ԃ��Ƃ��ł���B", "You can learn some spells/prayers.");
	}
	if (p_ptr->word_recall)
	{
		info[i++] = _("���Ȃ��͂����ɋA�҂��邾�낤�B", "You will soon be recalled.");
	}
	if (p_ptr->alter_reality)
	{
		info[i++] = _("���Ȃ��͂����ɂ��̐��E�𗣂�邾�낤�B", "You will soon be altered.");
	}
	if (p_ptr->see_infra)
	{
		info[i++] = _("���Ȃ��̓��͐ԊO���ɕq���ł���B", "Your eyes are sensitive to infrared light.");
	}
	if (p_ptr->see_inv)
	{
		info[i++] = _("���Ȃ��͓����ȃ����X�^�[�����邱�Ƃ��ł���B", "You can see invisible creatures.");
	}
	if (p_ptr->levitation)
	{
		info[i++] = _("���Ȃ��͔�Ԃ��Ƃ��ł���B", "You can fly.");
	}
	if (p_ptr->free_act)
	{
		info[i++] = _("���Ȃ��͖�გm�炸�̌��ʂ������Ă���B", "You have free action.");
	}
	if (p_ptr->regenerate)
	{
		info[i++] = _("���Ȃ��͑f�����̗͂��񕜂���B", "You regenerate quickly.");
	}
	if (p_ptr->slow_digest)
	{
		info[i++] = _("���Ȃ��͐H�~�����Ȃ��B", "Your appetite is small.");
	}
	if (p_ptr->telepathy)
	{
		info[i++] = _("���Ȃ��̓e���p�V�[�\�͂������Ă���B", "You have ESP.");
	}
	if (p_ptr->esp_animal)
	{
		info[i++] = _("���Ȃ��͎��R�E�̐����̑��݂�������\�͂������Ă���B", "You sense natural creatures.");
	}
	if (p_ptr->esp_undead)
	{
		info[i++] = _("���Ȃ��̓A���f�b�h�̑��݂�������\�͂������Ă���B", "You sense undead.");
	}
	if (p_ptr->esp_demon)
	{
		info[i++] = _("���Ȃ��͈����̑��݂�������\�͂������Ă���B", "You sense demons.");
	}
	if (p_ptr->esp_orc)
	{
		info[i++] = _("���Ȃ��̓I�[�N�̑��݂�������\�͂������Ă���B", "You sense orcs.");
	}
	if (p_ptr->esp_troll)
	{
		info[i++] = _("���Ȃ��̓g�����̑��݂�������\�͂������Ă���B", "You sense trolls.");
	}
	if (p_ptr->esp_giant)
	{
		info[i++] = _("���Ȃ��͋��l�̑��݂�������\�͂������Ă���B", "You sense giants.");
	}
	if (p_ptr->esp_dragon)
	{
		info[i++] = _("���Ȃ��̓h���S���̑��݂�������\�͂������Ă���B", "You sense dragons.");
	}
	if (p_ptr->esp_human)
	{
		info[i++] = _("���Ȃ��͐l�Ԃ̑��݂�������\�͂������Ă���B", "You sense humans.");
	}
	if (p_ptr->esp_evil)
	{
		info[i++] = _("���Ȃ��͎׈��Ȑ������̑��݂�������\�͂������Ă���B", "You sense evil creatures.");
	}
	if (p_ptr->esp_good)
	{
		info[i++] = _("���Ȃ��͑P�ǂȐ������̑��݂�������\�͂������Ă���B", "You sense good creatures.");
	}
	if (p_ptr->esp_nonliving)
	{
		info[i++] = _("���Ȃ��͊������閳�����̂̑��݂�������\�͂������Ă���B", "You sense non-living creatures.");
	}
	if (p_ptr->esp_unique)
	{
		info[i++] = _("���Ȃ��͓��ʂȋ��G�̑��݂�������\�͂������Ă���B", "You sense unique monsters.");
	}
	if (p_ptr->hold_exp)
	{
		info[i++] = _("���Ȃ��͎��Ȃ̌o���l����������ƈێ�����B", "You have a firm hold on your experience.");
	}
	if (p_ptr->reflect)
	{
		info[i++] = _("���Ȃ��͖�̎����𔽎˂���B", "You reflect bolt spells.");
	}
	if (p_ptr->sh_fire)
	{
		info[i++] = _("���Ȃ��͉��̃I�[���ɕ�܂�Ă���B", "You are surrounded with a fiery aura.");
	}
	if (p_ptr->sh_elec)
	{
		info[i++] = _("���Ȃ��͓d�C�ɕ�܂�Ă���B", "You are surrounded with electricity.");
	}
	if (p_ptr->sh_cold)
	{
		info[i++] = _("���Ȃ��͗�C�̃I�[���ɕ�܂�Ă���B", "You are surrounded with an aura of coldness.");
	}
	if (p_ptr->tim_sh_holy)
	{
		info[i++] = _("���Ȃ��͐��Ȃ�I�[���ɕ�܂�Ă���B", "You are surrounded with a holy aura.");
	}
	if (p_ptr->tim_sh_touki)
	{
		info[i++] = _("���Ȃ��͓��C�̃I�[���ɕ�܂�Ă���B", "You are surrounded with a energy aura.");
	}
	if (p_ptr->anti_magic)
	{
		info[i++] = _("���Ȃ��͔����@�V�[���h�ɕ�܂�Ă���B", "You are surrounded by an anti-magic shell.");
	}
	if (p_ptr->anti_tele)
	{
		info[i++] = _("���Ȃ��̓e���|�[�g�ł��Ȃ��B", "You cannot teleport.");
	}
	if (p_ptr->lite)
	{
		info[i++] = _("���Ȃ��̐g�̂͌����Ă���B", "You are carrying a permanent light.");
	}
	if (p_ptr->warning)
	{
		info[i++] = _("���Ȃ��͍s���̑O�Ɋ댯���@�m���邱�Ƃ��ł���B", "You will be warned before dangerous actions.");
	}
	if (p_ptr->dec_mana)
	{
		info[i++] = _("���Ȃ��͏��Ȃ�����͂Ŗ��@�������邱�Ƃ��ł���B", "You can cast spells with fewer mana points.");
	}
	if (p_ptr->easy_spell)
	{
		info[i++] = _("���Ȃ��͒Ⴂ���s���Ŗ��@�������邱�Ƃ��ł���B", "Fail rate of your magic is decreased.");
	}
	if (p_ptr->heavy_spell)
	{
		info[i++] = _("���Ȃ��͍������s���Ŗ��@�������Ȃ���΂����Ȃ��B", "Fail rate of your magic is increased.");
	}
	if (p_ptr->mighty_throw)
	{
		info[i++] = _("���Ȃ��͋������𓊂���B", "You can throw objects powerfully.");
	}

	if (p_ptr->immune_acid)
	{
		info[i++] = _("���Ȃ��͎_�ɑ΂��銮�S�Ȃ�Ɖu�������Ă���B", "You are completely immune to acid.");
	}
	else if (p_ptr->resist_acid && IS_OPPOSE_ACID())
	{
		info[i++] = _("���Ȃ��͎_�ւ̋��͂ȑϐ��������Ă���B", "You resist acid exceptionally well.");
	}
	else if (p_ptr->resist_acid || IS_OPPOSE_ACID())
	{
		info[i++] = _("���Ȃ��͎_�ւ̑ϐ��������Ă���B", "You are resistant to acid.");
	}

	if (p_ptr->immune_elec)
	{
		info[i++] = _("���Ȃ��͓d���ɑ΂��銮�S�Ȃ�Ɖu�������Ă���B", "You are completely immune to lightning.");
	}
	else if (p_ptr->resist_elec && IS_OPPOSE_ELEC())
	{
		info[i++] = _("���Ȃ��͓d���ւ̋��͂ȑϐ��������Ă���B", "You resist lightning exceptionally well.");
	}
	else if (p_ptr->resist_elec || IS_OPPOSE_ELEC())
	{
		info[i++] = _("���Ȃ��͓d���ւ̑ϐ��������Ă���B", "You are resistant to lightning.");
	}

	if (prace_is_(RACE_ANDROID) && !p_ptr->immune_elec)
	{
		info[i++] = _("���Ȃ��͓d���Ɏア�B", "You are susceptible to damage from lightning.");
	}

	if (p_ptr->immune_fire)
	{
		info[i++] = _("���Ȃ��͉΂ɑ΂��銮�S�Ȃ�Ɖu�������Ă���B", "You are completely immune to fire.");
	}
	else if (p_ptr->resist_fire && IS_OPPOSE_FIRE())
	{
		info[i++] = _("���Ȃ��͉΂ւ̋��͂ȑϐ��������Ă���B", "You resist fire exceptionally well.");
	}
	else if (p_ptr->resist_fire || IS_OPPOSE_FIRE())
	{
		info[i++] = _("���Ȃ��͉΂ւ̑ϐ��������Ă���B", "You are resistant to fire.");
	}

	if (prace_is_(RACE_ENT) && !p_ptr->immune_fire)
	{
		info[i++] = _("���Ȃ��͉΂Ɏア�B", "You are susceptible to damage from fire.");
	}

	if (p_ptr->immune_cold)
	{
		info[i++] = _("���Ȃ��͗�C�ɑ΂��銮�S�Ȃ�Ɖu�������Ă���B", "You are completely immune to cold.");
	}
	else if (p_ptr->resist_cold && IS_OPPOSE_COLD())
	{
		info[i++] = _("���Ȃ��͗�C�ւ̋��͂ȑϐ��������Ă���B", "You resist cold exceptionally well.");
	}
	else if (p_ptr->resist_cold || IS_OPPOSE_COLD())
	{
		info[i++] = _("���Ȃ��͗�C�ւ̑ϐ��������Ă���B", "You are resistant to cold.");
	}

	if (p_ptr->resist_pois && IS_OPPOSE_POIS())
	{
		info[i++] = _("���Ȃ��͓łւ̋��͂ȑϐ��������Ă���B", "You resist poison exceptionally well.");
	}
	else if (p_ptr->resist_pois || IS_OPPOSE_POIS())
	{
		info[i++] = _("���Ȃ��͓łւ̑ϐ��������Ă���B", "You are resistant to poison.");
	}

	if (p_ptr->resist_lite)
	{
		info[i++] = _("���Ȃ��͑M���ւ̑ϐ��������Ă���B", "You are resistant to bright light.");
	}

	if (prace_is_(RACE_VAMPIRE) || prace_is_(RACE_S_FAIRY) || (p_ptr->mimic_form == MIMIC_VAMPIRE))
	{
		info[i++] = _("���Ȃ��͑M���Ɏア�B", "You are susceptible to damage from bright light.");
	}

	if (prace_is_(RACE_VAMPIRE) || (p_ptr->mimic_form == MIMIC_VAMPIRE) || p_ptr->wraith_form)
	{
		info[i++] = _("���Ȃ��͈Í��ɑ΂��銮�S�Ȃ�Ɖu�������Ă���B", "You are completely immune to darkness.");
	}

	else if (p_ptr->resist_dark)
	{
		info[i++] = _("���Ȃ��͈Í��ւ̑ϐ��������Ă���B", "You are resistant to darkness.");
	}
	if (p_ptr->resist_conf)
	{
		info[i++] = _("���Ȃ��͍����ւ̑ϐ��������Ă���B", "You are resistant to confusion.");
	}
	if (p_ptr->resist_sound)
	{
		info[i++] = _("���Ȃ��͉��g�̏Ռ��ւ̑ϐ��������Ă���B", "You are resistant to sonic attacks.");
	}
	if (p_ptr->resist_disen)
	{
		info[i++] = _("���Ȃ��͗򉻂ւ̑ϐ��������Ă���B", "You are resistant to disenchantment.");
	}
	if (p_ptr->resist_chaos)
	{
		info[i++] = _("���Ȃ��̓J�I�X�̗͂ւ̑ϐ��������Ă���B", "You are resistant to chaos.");
	}
	if (p_ptr->resist_shard)
	{
		info[i++] = _("���Ȃ��͔j�Ђ̍U���ւ̑ϐ��������Ă���B", "You are resistant to blasts of shards.");
	}
	if (p_ptr->resist_nexus)
	{
		info[i++] = _("���Ȃ��͈��ʍ����̍U���ւ̑ϐ��������Ă���B", "You are resistant to nexus attacks.");
	}

	if (prace_is_(RACE_SPECTRE))
	{
		info[i++] = _("���Ȃ��͒n���̗͂��z���ł���B", "You can drain nether forces.");
	}
	else if (p_ptr->resist_neth)
	{
		info[i++] = _("���Ȃ��͒n���̗͂ւ̑ϐ��������Ă���B", "You are resistant to nether forces.");
	}
	if (p_ptr->resist_fear)
	{
		info[i++] = _("���Ȃ��͑S�����|�������Ȃ��B", "You are completely fearless.");
	}
	if (p_ptr->resist_blind)
	{
		info[i++] = _("���Ȃ��̖ڂ͖Ӗڂւ̑ϐ��������Ă���B", "Your eyes are resistant to blindness.");
	}
	if (p_ptr->resist_time)
	{
		info[i++] = _("���Ȃ��͎��ԋt�]�ւ̑ϐ��������Ă���B", "You are resistant to time.");
	}

	if (p_ptr->sustain_str)
	{
		info[i++] = _("���Ȃ��̘r�͈͂ێ�����Ă���B", "Your strength is sustained.");
	}
	if (p_ptr->sustain_int)
	{
		info[i++] = _("���Ȃ��̒m�\�͈ێ�����Ă���B", "Your intelligence is sustained.");
	}
	if (p_ptr->sustain_wis)
	{
		info[i++] = _("���Ȃ��̌����͈ێ�����Ă���B", "Your wisdom is sustained.");
	}
	if (p_ptr->sustain_con)
	{
		info[i++] = _("���Ȃ��̑ϋv�͈͂ێ�����Ă���B", "Your constitution is sustained.");
	}
	if (p_ptr->sustain_dex)
	{
		info[i++] = _("���Ȃ��̊�p���͈ێ�����Ă���B", "Your dexterity is sustained.");
	}
	if (p_ptr->sustain_chr)
	{
		info[i++] = _("���Ȃ��̖��͈͂ێ�����Ă���B", "Your charisma is sustained.");
	}

	if (have_flag(flgs, TR_STR))
	{
		info[i++] = _("���Ȃ��̘r�͂͑����ɂ���ĉe�����󂯂Ă���B", "Your strength is affected by your equipment.");
	}
	if (have_flag(flgs, TR_INT))
	{
		info[i++] = _("���Ȃ��̒m�\�͑����ɂ���ĉe�����󂯂Ă���B", "Your intelligence is affected by your equipment.");
	}
	if (have_flag(flgs, TR_WIS))
	{
		info[i++] = _("���Ȃ��̌����͑����ɂ���ĉe�����󂯂Ă���B", "Your wisdom is affected by your equipment.");
	}
	if (have_flag(flgs, TR_DEX))
	{
		info[i++] = _("���Ȃ��̊�p���͑����ɂ���ĉe�����󂯂Ă���B", "Your dexterity is affected by your equipment.");
	}
	if (have_flag(flgs, TR_CON))
	{
		info[i++] = _("���Ȃ��̑ϋv�͂͑����ɂ���ĉe�����󂯂Ă���B", "Your constitution is affected by your equipment.");
	}
	if (have_flag(flgs, TR_CHR))
	{
		info[i++] = _("���Ȃ��̖��͂͑����ɂ���ĉe�����󂯂Ă���B", "Your charisma is affected by your equipment.");
	}

	if (have_flag(flgs, TR_STEALTH))
	{
		info[i++] = _("���Ȃ��̉B���s���\�͂͑����ɂ���ĉe�����󂯂Ă���B", "Your stealth is affected by your equipment.");
	}
	if (have_flag(flgs, TR_SEARCH))
	{
		info[i++] = _("���Ȃ��̒T���\�͂͑����ɂ���ĉe�����󂯂Ă���B", "Your searching ability is affected by your equipment.");
	}
	if (have_flag(flgs, TR_INFRA))
	{
		info[i++] = _("���Ȃ��̐ԊO�����͂͑����ɂ���ĉe�����󂯂Ă���B", "Your infravision is affected by your equipment.");
	}
	if (have_flag(flgs, TR_TUNNEL))
	{
		info[i++] = _("���Ȃ��̍̌@�\�͂͑����ɂ���ĉe�����󂯂Ă���B", "Your digging ability is affected by your equipment.");
	}
	if (have_flag(flgs, TR_SPEED))
	{
		info[i++] = _("���Ȃ��̃X�s�[�h�͑����ɂ���ĉe�����󂯂Ă���B", "Your speed is affected by your equipment.");
	}
	if (have_flag(flgs, TR_BLOWS))
	{
		info[i++] = _("���Ȃ��̍U�����x�͑����ɂ���ĉe�����󂯂Ă���B", "Your attack speed is affected by your equipment.");
	}


	/* Access the current weapon */
	o_ptr = &inventory[INVEN_RARM];

	/* Analyze the weapon */
	if (o_ptr->k_idx)
	{
		/* Indicate Blessing */
		if (have_flag(flgs, TR_BLESSED))
		{
			info[i++] = _("���Ȃ��̕���͐_�̏j�����󂯂Ă���B", "Your weapon has been blessed by the gods.");
		}

		if (have_flag(flgs, TR_CHAOTIC))
		{
			info[i++] = _("���Ȃ��̕���̓��O���X�̒��̑��������B", "Your weapon is branded with the Sign of Logrus.");
		}

		/* Hack */
		if (have_flag(flgs, TR_IMPACT))
		{
			info[i++] = _("���Ȃ��̕���͑Ō��Œn�k�𔭐����邱�Ƃ��ł���B", "The impact of your weapon can cause earthquakes.");
		}

		if (have_flag(flgs, TR_VORPAL))
		{
			info[i++] = _("���Ȃ��̕���͔��ɉs���B", "Your weapon is very sharp.");
		}

		if (have_flag(flgs, TR_VAMPIRIC))
		{
			info[i++] = _("���Ȃ��̕���͓G���琶���͂��z������B", "Your weapon drains life from your foes.");
		}

		/* Special "Attack Bonuses" */
		if (have_flag(flgs, TR_BRAND_ACID))
		{
			info[i++] = _("���Ȃ��̕���͓G��n�����B", "Your weapon melts your foes.");
		}
		if (have_flag(flgs, TR_BRAND_ELEC))
		{
			info[i++] = _("���Ȃ��̕���͓G�����d������B", "Your weapon shocks your foes.");
		}
		if (have_flag(flgs, TR_BRAND_FIRE))
		{
			info[i++] = _("���Ȃ��̕���͓G��R�₷�B", "Your weapon burns your foes.");
		}
		if (have_flag(flgs, TR_BRAND_COLD))
		{
			info[i++] = _("���Ȃ��̕���͓G�𓀂点��B", "Your weapon freezes your foes.");
		}
		if (have_flag(flgs, TR_BRAND_POIS))
		{
			info[i++] = _("���Ȃ��̕���͓G��łŐN���B", "Your weapon poisons your foes.");
		}

		/* Special "slay" flags */
		if (have_flag(flgs, TR_KILL_ANIMAL))
		{
			info[i++] = _("���Ȃ��̕���͓����̓V�G�ł���B", "Your weapon is a great bane of animals.");
		}
		else if (have_flag(flgs, TR_SLAY_ANIMAL))
		{
			info[i++] = _("���Ȃ��̕���͓����ɑ΂��ċ����͂𔭊�����B", "Your weapon strikes at animals with extra force.");
		}
		if (have_flag(flgs, TR_KILL_EVIL))
		{
			info[i++] = _("���Ȃ��̕���͎׈��Ȃ鑶�݂̓V�G�ł���B", "Your weapon is a great bane of evil.");
		}
		else if (have_flag(flgs, TR_SLAY_EVIL))
		{
			info[i++] = _("���Ȃ��̕���͎׈��Ȃ鑶�݂ɑ΂��ċ����͂𔭊�����B", "Your weapon strikes at evil with extra force.");
		}
		if (have_flag(flgs, TR_KILL_HUMAN))
		{
			info[i++] = _("���Ȃ��̕���͐l�Ԃ̓V�G�ł���B", "Your weapon is a great bane of humans.");
		}
		else if (have_flag(flgs, TR_SLAY_HUMAN))
		{
			info[i++] = _("���Ȃ��̕���͐l�Ԃɑ΂��ē��ɋ����͂𔭊�����B", "Your weapon is especially deadly against humans.");
		}
		if (have_flag(flgs, TR_KILL_UNDEAD))
		{
			info[i++] = _("���Ȃ��̕���̓A���f�b�h�̓V�G�ł���B", "Your weapon is a great bane of undead.");
		}
		else if (have_flag(flgs, TR_SLAY_UNDEAD))
		{
			info[i++] = _("���Ȃ��̕���̓A���f�b�h�ɑ΂��Đ_���Ȃ�͂𔭊�����B", "Your weapon strikes at undead with holy wrath.");
		}
		if (have_flag(flgs, TR_KILL_DEMON))
		{
			info[i++] = _("���Ȃ��̕���̓f�[�����̓V�G�ł���B", "Your weapon is a great bane of demons.");
		}
		else if (have_flag(flgs, TR_SLAY_DEMON))
		{
			info[i++] = _("���Ȃ��̕���̓f�[�����ɑ΂��Đ_���Ȃ�͂𔭊�����B", "Your weapon strikes at demons with holy wrath.");
		}
		if (have_flag(flgs, TR_KILL_ORC))
		{
			info[i++] = _("���Ȃ��̕���̓I�[�N�̓V�G�ł���B", "Your weapon is a great bane of orcs.");
		}
		else if (have_flag(flgs, TR_SLAY_ORC))
		{
			info[i++] = _("���Ȃ��̕���̓I�[�N�ɑ΂��ē��ɋ����͂𔭊�����B", "Your weapon is especially deadly against orcs.");
		}
		if (have_flag(flgs, TR_KILL_TROLL))
		{
			info[i++] = _("���Ȃ��̕���̓g�����̓V�G�ł���B", "Your weapon is a great bane of trolls.");
		}
		else if (have_flag(flgs, TR_SLAY_TROLL))
		{
			info[i++] = _("���Ȃ��̕���̓g�����ɑ΂��ē��ɋ����͂𔭊�����B", "Your weapon is especially deadly against trolls.");
		}
		if (have_flag(flgs, TR_KILL_GIANT))
		{
			info[i++] = _("���Ȃ��̕���̓W���C�A���g�̓V�G�ł���B", "Your weapon is a great bane of giants.");
		}
		else if (have_flag(flgs, TR_SLAY_GIANT))
		{
			info[i++] = _("���Ȃ��̕���̓W���C�A���g�ɑ΂��ē��ɋ����͂𔭊�����B", "Your weapon is especially deadly against giants.");
		}
		/* Special "kill" flags */
		if (have_flag(flgs, TR_KILL_DRAGON))
		{
			info[i++] = _("���Ȃ��̕���̓h���S���̓V�G�ł���B", "Your weapon is a great bane of dragons.");
		}
		else if (have_flag(flgs, TR_SLAY_DRAGON))
		{
			info[i++] = _("���Ȃ��̕���̓h���S���ɑ΂��ē��ɋ����͂𔭊�����B", "Your weapon is especially deadly against dragons.");
		}

		if (have_flag(flgs, TR_FORCE_WEAPON))
		{
			info[i++] = _("���Ȃ��̕����MP���g���čU������B", "Your weapon causes greate damages using your MP.");
		}
		if (have_flag(flgs, TR_THROW))
		{
			info[i++] = _("���Ȃ��̕���͓����₷���B", "Your weapon can be thrown well.");
		}
	}


	/* Save the screen */
	screen_save();

	/* Erase the screen */
	for (k = 1; k < 24; k++) prt("", k, 13);

	/* Label the information */
	prt(_("        ���Ȃ��̏��:", "     Your Attributes:"), 1, 15);

	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		prt(info[j], k++, 15);

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == 22) && (j+1 < i))
		{
			prt(_("-- ���� --", "-- more --"), k, 15);
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}

	/* Pause */
	prt(_("[�����L�[�������ƃQ�[���ɖ߂�܂�]", "[Press any key to continue]"), k, 13);
	inkey();

	/* Restore the screen */
	screen_load();
}

/*!
 * @brief ���@���ʎ��Ԃ̃^�[�����Ɋ�Â��ĕ\��ID��Ԃ��B
 * @param dur ���ʃ^�[����
 * @return ���ʎ��Ԃ̕\��ID
 */
static int report_magics_aux(int dur)
{
	if (dur <= 5)
	{
		return 0;
	}
	else if (dur <= 10)
	{
		return 1;
	}
	else if (dur <= 20)
	{
		return 2;
	}
	else if (dur <= 50)
	{
		return 3;
	}
	else if (dur <= 100)
	{
		return 4;
	}
	else if (dur <= 200)
	{
		return 5;
	}
	else
	{
		return 6;
	}
}

static cptr report_magic_durations[] =
{
#ifdef JP
"�����Z����",
"�����̊�",
"���΂炭�̊�",
"����������",
"������",
"���ɒ�����",
"�M����قǒ�����",
"�����X�^�[���U������܂�"
#else
	"for a short time",
	"for a little while",
	"for a while",
	"for a long while",
	"for a long time",
	"for a very long time",
	"for an incredibly long time",
	"until you hit a monster"
#endif

};

/*!
 * @brief ���݂̈ꎞ�I���ʈꗗ��Ԃ� / Report all currently active magical effects.
 * @return �Ȃ�
 */
void report_magics(void)
{
	int     i = 0, j, k;
	char    Dummy[80];
	cptr    info[128];
	int     info2[128];


	if (p_ptr->blind)
	{
		info2[i]  = report_magics_aux(p_ptr->blind);
		info[i++] = _("���Ȃ��͖ڂ������Ȃ�", "You cannot see");
	}
	if (p_ptr->confused)
	{
		info2[i]  = report_magics_aux(p_ptr->confused);
		info[i++] = _("���Ȃ��͍������Ă���", "You are confused");
	}
	if (p_ptr->afraid)
	{
		info2[i]  = report_magics_aux(p_ptr->afraid);
		info[i++] = _("���Ȃ��͋��|�ɐN����Ă���", "You are terrified");
	}
	if (p_ptr->poisoned)
	{
		info2[i]  = report_magics_aux(p_ptr->poisoned);
		info[i++] = _("���Ȃ��͓łɐN����Ă���", "You are poisoned");
	}
	if (p_ptr->image)
	{
		info2[i]  = report_magics_aux(p_ptr->image);
		info[i++] = _("���Ȃ��͌��o�����Ă���", "You are hallucinating");
	}
	if (p_ptr->blessed)
	{
		info2[i]  = report_magics_aux(p_ptr->blessed);
		info[i++] = _("���Ȃ��͍������������Ă���", "You feel rightous");
	}
	if (p_ptr->hero)
	{
		info2[i]  = report_magics_aux(p_ptr->hero);
		info[i++] = _("���Ȃ��̓q�[���[�C����", "You feel heroic");
	}
	if (p_ptr->shero)
	{
		info2[i]  = report_magics_aux(p_ptr->shero);
		info[i++] = _("���Ȃ��͐퓬����", "You are in a battle rage");
	}
	if (p_ptr->protevil)
	{
		info2[i]  = report_magics_aux(p_ptr->protevil);
		info[i++] = _("���Ȃ��͎׈��Ȃ鑶�݂������Ă���", "You are protected from evil");
	}
	if (p_ptr->shield)
	{
		info2[i]  = report_magics_aux(p_ptr->shield);
		info[i++] = _("���Ȃ��͐_��̃V�[���h�Ŏ���Ă���", "You are protected by a mystic shield");
	}
	if (p_ptr->invuln)
	{
		info2[i]  = report_magics_aux(p_ptr->invuln);
		info[i++] = _("���Ȃ��͖��G��", "You are invulnerable");
	}
	if (p_ptr->wraith_form)
	{
		info2[i]  = report_magics_aux(p_ptr->wraith_form);
		info[i++] = _("���Ȃ��͗H�̉����Ă���", "You are incorporeal");
	}
	if (p_ptr->special_attack & ATTACK_CONFUSE)
	{
		info2[i]  = 7;
		info[i++] = _("���Ȃ��̎�͐Ԃ��P���Ă���", "Your hands are glowing dull red.");
	}
	if (p_ptr->word_recall)
	{
		info2[i]  = report_magics_aux(p_ptr->word_recall);
		info[i++] = _("���̌�A�҂̏ق���������", "You are waiting to be recalled");
	}
	if (p_ptr->alter_reality)
	{
		info2[i]  = report_magics_aux(p_ptr->alter_reality);
		info[i++] = _("���̌㌻���ϗe����������", "You waiting to be altered");
	}
	if (p_ptr->oppose_acid)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_acid);
		info[i++] = _("���Ȃ��͎_�ւ̑ϐ��������Ă���", "You are resistant to acid");
	}
	if (p_ptr->oppose_elec)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_elec);
		info[i++] = _("���Ȃ��͓d���ւ̑ϐ��������Ă���", "You are resistant to lightning");
	}
	if (p_ptr->oppose_fire)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_fire);
		info[i++] = _("���Ȃ��͉΂ւ̑ϐ��������Ă���", "You are resistant to fire");
	}
	if (p_ptr->oppose_cold)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_cold);
		info[i++] = _("���Ȃ��͗�C�ւ̑ϐ��������Ă���", "You are resistant to cold");
	}
	if (p_ptr->oppose_pois)
	{
		info2[i]  = report_magics_aux(p_ptr->oppose_pois);
		info[i++] = _("���Ȃ��͓łւ̑ϐ��������Ă���", "You are resistant to poison");
	}

	/* Save the screen */
	screen_save();

	/* Erase the screen */
	for (k = 1; k < 24; k++) prt("", k, 13);

	/* Label the information */
	prt(_("    ���݂������Ă��閂�@     :", "     Your Current Magic:"), 1, 15);


	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		sprintf(Dummy, _("%-28s : ���� - %s ", "%s %s."), info[j],
			report_magic_durations[info2[j]]);
		prt(Dummy, k++, 15);

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == 22) && (j + 1 < i))
		{
			prt(_("-- ���� --", "-- more --"), k, 15);
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}

	/* Pause */
	prt(_("[�����L�[�������ƃQ�[���ɖ߂�܂�]", "[Press any key to continue]"), k, 13);
	inkey();

	/* Restore the screen */
	screen_load();
}
