#include "angband.h"
#include "status-first-page.h"
#include "artifact.h"
#include "combat/melee.h"
#include "combat/shoot.h"
#include "object-hook.h"
#include "objectkind.h"

static TERM_COLOR likert_color = TERM_WHITE;

/*!
 * @brief �Z�\�����N�̕\������߂�
 * Returns a "rating" of x depending on y
 * @param x �Z�\�l
 * @param y �Z�\�l�ɑ΂��郉���N���
 * @return �Ȃ�
 */
static concptr likert(int x, int y)
{
	static char dummy[20] = "", dummy2[20] = "";
	memset(dummy, 0, strlen(dummy));
	memset(dummy2, 0, strlen(dummy2));
	if (y <= 0) y = 1;

	if (show_actual_value)
	{
		sprintf(dummy, "%3d-", x);
	}

	/* Negative value */
	if (x < 0)
	{
		likert_color = TERM_L_DARK;
		strcat(dummy, _("�Œ�", "Very Bad"));
		return dummy;
	}

	switch ((x / y))
	{
	case 0:
	case 1:
		likert_color = TERM_RED;
		strcat(dummy, _("����", "Bad"));
		break;
	case 2:
		likert_color = TERM_L_RED;
		strcat(dummy, _("���", "Poor"));
		break;

	case 3:
	case 4:
		likert_color = TERM_ORANGE;
		strcat(dummy, _("����", "Fair"));
		break;

	case 5:
		likert_color = TERM_YELLOW;
		strcat(dummy, _("�ǂ�", "Good"));
		break;

	case 6:
		likert_color = TERM_YELLOW;
		strcat(dummy, _("��ϗǂ�", "Very Good"));
		break;

	case 7:
	case 8:
		likert_color = TERM_L_GREEN;
		strcat(dummy, _("��z", "Excellent"));
		break;

	case 9:
	case 10:
	case 11:
	case 12:
	case 13:
		likert_color = TERM_GREEN;
		strcat(dummy, _("���z", "Superb"));
		break;

	case 14:
	case 15:
	case 16:
	case 17:
		likert_color = TERM_BLUE;
		strcat(dummy, _("�p�Y�I", "Heroic"));
		break;

	default:
		likert_color = TERM_VIOLET;
		sprintf(dummy2, _("�`���I[%d]", "Legendary[%d]"),
			(int)((((x / y) - 17) * 5) / 2));
		strcat(dummy, dummy2);

		break;
	}

	return dummy;
}


/*!
 * @brief �v���C���[�X�e�[�^�X��1�y�[�W�ڊe��ڍׂ��܂Ƃ߂ĕ\������
 * Prints ratings on certain abilities
 * @param creature_ptr �v���[���[�ւ̎Q�ƃ|�C���^
 * @return �Ȃ�
 * @details
 * This code is "imitated" elsewhere to "dump" a character sheet.
 */
void display_player_various(player_type *creature_ptr, void(*display_player_one_line)(int, concptr, TERM_COLOR))
{
	int muta_att = 0;
	if (creature_ptr->muta2 & MUT2_HORNS)     muta_att++;
	if (creature_ptr->muta2 & MUT2_SCOR_TAIL) muta_att++;
	if (creature_ptr->muta2 & MUT2_BEAK)      muta_att++;
	if (creature_ptr->muta2 & MUT2_TRUNK)     muta_att++;
	if (creature_ptr->muta2 & MUT2_TENTACLES) muta_att++;

	int xthn = creature_ptr->skill_thn + (creature_ptr->to_h_m * BTH_PLUS_ADJ);

	/* Shooting Skill (with current bow and normal missile) */
	object_type *o_ptr;
	o_ptr = &creature_ptr->inventory_list[INVEN_BOW];
	int tmp = creature_ptr->to_h_b + o_ptr->to_h;
	int	xthb = creature_ptr->skill_thb + (tmp * BTH_PLUS_ADJ);

	/* If the player is wielding one? */
	int	shots;
	int shot_frac;
	if (o_ptr->k_idx)
	{
		ENERGY energy_fire = bow_energy(o_ptr->sval);

		/* Calculate shots per round */
		shots = creature_ptr->num_fire * 100;
		shot_frac = (shots * 100 / energy_fire) % 100;
		shots = shots / energy_fire;
		if (o_ptr->name1 == ART_CRIMSON)
		{
			shots = 1;
			shot_frac = 0;
			if (creature_ptr->pclass == CLASS_ARCHER)
			{
				/* Extra shot at level 10 */
				if (creature_ptr->lev >= 10) shots++;

				/* Extra shot at level 30 */
				if (creature_ptr->lev >= 30) shots++;

				/* Extra shot at level 45 */
				if (creature_ptr->lev >= 45) shots++;
			}
		}
	}
	else
	{
		shots = 0;
		shot_frac = 0;
	}

	int damage[2];
	int to_h[2];
	int basedam;
	BIT_FLAGS flgs[TR_FLAG_SIZE];
	bool poison_needle;
	for (int i = 0; i < 2; i++)
	{
		damage[i] = creature_ptr->dis_to_d[i] * 100;
		if (((creature_ptr->pclass == CLASS_MONK) || (creature_ptr->pclass == CLASS_FORCETRAINER)) && (empty_hands(creature_ptr, TRUE) & EMPTY_HAND_RARM))
		{
			PLAYER_LEVEL level = creature_ptr->lev;
			if (i)
			{
				damage[i] = 0;
				break;
			}

			if (creature_ptr->pclass == CLASS_FORCETRAINER) level = MAX(1, level - 3);
			if (creature_ptr->special_defense & KAMAE_BYAKKO)
				basedam = monk_ave_damage[level][1];
			else if (creature_ptr->special_defense & (KAMAE_GENBU | KAMAE_SUZAKU))
				basedam = monk_ave_damage[level][2];
			else
				basedam = monk_ave_damage[level][0];

			damage[i] += basedam;
			if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_POISON_NEEDLE)) damage[i] = 1;
			if (damage[i] < 0) damage[i] = 0;
			continue;
		}

		o_ptr = &creature_ptr->inventory_list[INVEN_RARM + i];

		/* Average damage per round */
		if (o_ptr->k_idx == 0)
		{
			basedam = 0;
			damage[i] += basedam;
			if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_POISON_NEEDLE)) damage[i] = 1;
			if (damage[i] < 0) damage[i] = 0;
			continue;
		}

		to_h[i] = 0;
		poison_needle = FALSE;

		if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_POISON_NEEDLE)) poison_needle = TRUE;
		if (object_is_known(o_ptr))
		{
			damage[i] += o_ptr->to_d * 100;
			to_h[i] += o_ptr->to_h;
		}

		basedam = ((o_ptr->dd + creature_ptr->to_dd[i]) * (o_ptr->ds + creature_ptr->to_ds[i] + 1)) * 50;
		object_flags_known(o_ptr, flgs);

		basedam = calc_expect_crit(creature_ptr, o_ptr->weight, to_h[i], basedam, creature_ptr->dis_to_h[i], poison_needle);
		if (OBJECT_IS_FULL_KNOWN(o_ptr) && ((o_ptr->name1 == ART_VORPAL_BLADE) || (o_ptr->name1 == ART_CHAINSWORD)))
		{
			/* vorpal blade */
			basedam *= 5;
			basedam /= 3;
		}
		else if (have_flag(flgs, TR_VORPAL))
		{
			/* vorpal flag only */
			basedam *= 11;
			basedam /= 9;
		}

		if ((creature_ptr->pclass != CLASS_SAMURAI) && have_flag(flgs, TR_FORCE_WEAPON) && (creature_ptr->csp > (o_ptr->dd * o_ptr->ds / 5)))
			basedam = basedam * 7 / 2;
		damage[i] += basedam;
		if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_POISON_NEEDLE)) damage[i] = 1;
		if (damage[i] < 0) damage[i] = 0;
	}

	int blows1 = creature_ptr->migite ? creature_ptr->num_blow[0] : 0;
	int blows2 = creature_ptr->hidarite ? creature_ptr->num_blow[1] : 0;

	/* Basic abilities */

	int xdis = creature_ptr->skill_dis;
	int xdev = creature_ptr->skill_dev;
	int xsav = creature_ptr->skill_sav;
	int xstl = creature_ptr->skill_stl;
	int	xsrh = creature_ptr->skill_srh;
	int	xfos = creature_ptr->skill_fos;
	int xdig = creature_ptr->skill_dig;

	concptr desc = likert(xthn, 12);
	(*display_player_one_line)(ENTRY_SKILL_FIGHT, desc, likert_color);

	desc = likert(xthb, 12);
	(*display_player_one_line)(ENTRY_SKILL_SHOOT, desc, likert_color);

	desc = likert(xsav, 7);
	(*display_player_one_line)(ENTRY_SKILL_SAVING, desc, likert_color);

	desc = likert((xstl > 0) ? xstl : -1, 1);
	(*display_player_one_line)(ENTRY_SKILL_STEALTH, desc, likert_color);

	desc = likert(xfos, 6);
	(*display_player_one_line)(ENTRY_SKILL_PERCEP, desc, likert_color);

	desc = likert(xsrh, 6);
	(*display_player_one_line)(ENTRY_SKILL_SEARCH, desc, likert_color);

	desc = likert(xdis, 8);
	(*display_player_one_line)(ENTRY_SKILL_DISARM, desc, likert_color);

	desc = likert(xdev, 6);
	(*display_player_one_line)(ENTRY_SKILL_DEVICE, desc, likert_color);

	desc = likert(xdev, 6);
	(*display_player_one_line)(ENTRY_SKILL_DEVICE, desc, likert_color);

	desc = likert(xdig, 4);
	(*display_player_one_line)(ENTRY_SKILL_DIG, desc, likert_color);

	if (!muta_att)
		(*display_player_one_line)(ENTRY_BLOWS, format("%d+%d", blows1, blows2), TERM_L_BLUE);
	else
		(*display_player_one_line)(ENTRY_BLOWS, format("%d+%d+%d", blows1, blows2, muta_att), TERM_L_BLUE);

	(*display_player_one_line)(ENTRY_SHOTS, format("%d.%02d", shots, shot_frac), TERM_L_BLUE);

	if ((damage[0] + damage[1]) == 0)
		desc = "nil!";
	else
		desc = format("%d+%d", blows1 * damage[0] / 100, blows2 * damage[1] / 100);

	(*display_player_one_line)(ENTRY_AVG_DMG, desc, TERM_L_BLUE);

	(*display_player_one_line)(ENTRY_INFRA, format("%d feet", creature_ptr->see_infra * 10), TERM_WHITE);
}
