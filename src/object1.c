/*!
 * @file object1.c
 * @brief ���֥������Ȥμ��� / Object code, part 1
 * @date 2014/01/10
 * @author
 * Copyright (c) 1997 Ben Harrison, James E. Wilson, Robert A. Koeneke\n
 *\n
 * This software may be copied and distributed for educational, research,\n
 * and not for profit purposes provided that this copyright and statement\n
 * are included in all such copies.  Other copyrights may also apply.\n
 * 2014 Deskull rearranged comment for Doxygen.\n
 */

#include "angband.h"

#if defined(MACINTOSH) || defined(MACH_O_CARBON)
#ifdef verify
#undef verify
#endif
#endif

/*!
 * @brief ���֥������ȡ��Ϸ���ɽ������ܥ�ʤɽ�������� / Reset the "visual" lists
 * @return �ʤ�
 * This involves resetting various things to their "default" state.\n
 *\n
 * If the "prefs" flag is TRUE, then we will also load the appropriate\n
 * "user pref file" based on the current setting of the "use_graphics"\n
 * flag.  This is useful for switching "graphics" on/off.\n
 *\n
 * The features, objects, and monsters, should all be encoded in the\n
 * relevant "font.pref" and/or "graf.prf" files.  XXX XXX XXX\n
 *\n
 * The "prefs" parameter is no longer meaningful.  XXX XXX XXX\n
 */
void reset_visuals(void)
{
	int i, j;

	/* Extract some info about terrain features */
	for (i = 0; i < max_f_idx; i++)
	{
		feature_type *f_ptr = &f_info[i];

		/* Assume we will use the underlying values */
		for (j = 0; j < F_LIT_MAX; j++)
		{
			f_ptr->x_attr[j] = f_ptr->d_attr[j];
			f_ptr->x_char[j] = f_ptr->d_char[j];
		}
	}

	/* Extract default attr/char code for objects */
	for (i = 0; i < max_k_idx; i++)
	{
		object_kind *k_ptr = &k_info[i];

		/* Default attr/char */
		k_ptr->x_attr = k_ptr->d_attr;
		k_ptr->x_char = k_ptr->d_char;
	}

	/* Extract default attr/char code for monsters */
	for (i = 0; i < max_r_idx; i++)
	{
		monster_race *r_ptr = &r_info[i];

		/* Default attr/char */
		r_ptr->x_attr = r_ptr->d_attr;
		r_ptr->x_char = r_ptr->d_char;
	}

	if (use_graphics)
	{
		char buf[1024];

		/* Process "graf.prf" */
		process_pref_file("graf.prf");

		/* Access the "character" pref file */
		sprintf(buf, "graf-%s.prf", player_base);

		/* Process "graf-<playername>.prf" */
		process_pref_file(buf);
	}

	/* Normal symbols */
	else
	{
		char buf[1024];

		/* Process "font.prf" */
		process_pref_file("font.prf");

		/* Access the "character" pref file */
		sprintf(buf, "font-%s.prf", player_base);

		/* Process "font-<playername>.prf" */
		process_pref_file(buf);
	}
}

/*!
 * @brief ���֥������ȤΥե饰��������Ϳ����
 * Obtain the "flags" for an item
 * @param o_ptr �ե饰�������Υ��֥������ȹ�¤�Υݥ���
 * @param flgs �ե饰���������������
 * @return �ʤ�
 */
void object_flags(object_type *o_ptr, u32b flgs[TR_FLAG_SIZE])
{
	object_kind *k_ptr = &k_info[o_ptr->k_idx];
	int i;

	/* Base object */
	for (i = 0; i < TR_FLAG_SIZE; i++)
		flgs[i] = k_ptr->flags[i];

	/* Artifact */
	if (object_is_fixed_artifact(o_ptr))
	{
		artifact_type *a_ptr = &a_info[o_ptr->name1];

		for (i = 0; i < TR_FLAG_SIZE; i++)
			flgs[i] = a_ptr->flags[i];
	}

	/* Ego-item */
	if (object_is_ego(o_ptr))
	{
		ego_item_type *e_ptr = &e_info[o_ptr->name2];

		for (i = 0; i < TR_FLAG_SIZE; i++)
			flgs[i] |= e_ptr->flags[i];

		if ((o_ptr->name2 == EGO_LITE_AURA_FIRE) && !o_ptr->xtra4 && (o_ptr->sval <= SV_LITE_LANTERN))
		{
			remove_flag(flgs, TR_SH_FIRE);
		}
		else if ((o_ptr->name2 == EGO_LITE_INFRA) && !o_ptr->xtra4 && (o_ptr->sval <= SV_LITE_LANTERN))
		{
			remove_flag(flgs, TR_INFRA);
		}
		else if ((o_ptr->name2 == EGO_LITE_EYE) && !o_ptr->xtra4 && (o_ptr->sval <= SV_LITE_LANTERN))
		{
			remove_flag(flgs, TR_RES_BLIND);
			remove_flag(flgs, TR_SEE_INVIS);
		}
	}

	/* Random artifact ! */
	for (i = 0; i < TR_FLAG_SIZE; i++)
		flgs[i] |= o_ptr->art_flags[i];

	if (object_is_smith(o_ptr))
	{
		int add = o_ptr->xtra3 - 1;

		if (add < TR_FLAG_MAX)
		{
			add_flag(flgs, add);
		}
		else if (add == ESSENCE_TMP_RES_ACID)
		{
			add_flag(flgs, TR_RES_ACID);
			add_flag(flgs, TR_ACTIVATE);
		}
		else if (add == ESSENCE_TMP_RES_ELEC)
		{
			add_flag(flgs, TR_RES_ELEC);
			add_flag(flgs, TR_ACTIVATE);
		}
		else if (add == ESSENCE_TMP_RES_FIRE)
		{
			add_flag(flgs, TR_RES_FIRE);
			add_flag(flgs, TR_ACTIVATE);
		}
		else if (add == ESSENCE_TMP_RES_COLD)
		{
			add_flag(flgs, TR_RES_COLD);
			add_flag(flgs, TR_ACTIVATE);
		}
		else if (add == ESSENCE_SH_FIRE)
		{
			add_flag(flgs, TR_RES_FIRE);
			add_flag(flgs, TR_SH_FIRE);
		}
		else if (add == ESSENCE_SH_ELEC)
		{
			add_flag(flgs, TR_RES_ELEC);
			add_flag(flgs, TR_SH_ELEC);
		}
		else if (add == ESSENCE_SH_COLD)
		{
			add_flag(flgs, TR_RES_COLD);
			add_flag(flgs, TR_SH_COLD);
		}
		else if (add == ESSENCE_RESISTANCE)
		{
			add_flag(flgs, TR_RES_ACID);
			add_flag(flgs, TR_RES_ELEC);
			add_flag(flgs, TR_RES_FIRE);
			add_flag(flgs, TR_RES_COLD);
		}
		else if (add == TR_IMPACT)
		{
			add_flag(flgs, TR_ACTIVATE);
		}
	}
}

/*!
 * @brief ���֥������Ȥ���������Ƥ���ե饰����������
 * Obtain the "flags" for an item which are known to the player
 * @param o_ptr �ե饰�������Υ��֥������ȹ�¤�Υݥ���
 * @param flgs �ե饰���������������
 * @return �ʤ�
 */
void object_flags_known(object_type *o_ptr, u32b flgs[TR_FLAG_SIZE])
{
	bool spoil = FALSE;
	int i;

	object_kind *k_ptr = &k_info[o_ptr->k_idx];

	/* Clear */
	for (i = 0; i < TR_FLAG_SIZE; i++)
		flgs[i] = 0;

	if (!object_is_aware(o_ptr)) return;

	/* Base object */
	for (i = 0; i < TR_FLAG_SIZE; i++)
		flgs[i] = k_ptr->flags[i];

	/* Must be identified */
	if (!object_is_known(o_ptr)) return;

	/* Ego-item (known basic flags) */
	if (object_is_ego(o_ptr))
	{
		ego_item_type *e_ptr = &e_info[o_ptr->name2];

		for (i = 0; i < TR_FLAG_SIZE; i++)
			flgs[i] |= e_ptr->flags[i];

		if ((o_ptr->name2 == EGO_LITE_AURA_FIRE) && !o_ptr->xtra4 && (o_ptr->sval <= SV_LITE_LANTERN))
		{
			remove_flag(flgs, TR_SH_FIRE);
		}
		else if ((o_ptr->name2 == EGO_LITE_INFRA) && !o_ptr->xtra4 && (o_ptr->sval <= SV_LITE_LANTERN))
		{
			remove_flag(flgs, TR_INFRA);
		}
		else if ((o_ptr->name2 == EGO_LITE_EYE) && !o_ptr->xtra4 && (o_ptr->sval <= SV_LITE_LANTERN))
		{
			remove_flag(flgs, TR_RES_BLIND);
			remove_flag(flgs, TR_SEE_INVIS);
		}
	}


#ifdef SPOIL_ARTIFACTS
	/* Full knowledge for some artifacts */
	if (object_is_artifact(o_ptr)) spoil = TRUE;
#endif /* SPOIL_ARTIFACTS */

#ifdef SPOIL_EGO_ITEMS
	/* Full knowledge for some ego-items */
	if (object_is_ego(o_ptr)) spoil = TRUE;
#endif /* SPOIL_EGO_ITEMS */

	/* Need full knowledge or spoilers */
	if (spoil || (o_ptr->ident & IDENT_MENTAL))
	{
		/* Artifact */
		if (object_is_fixed_artifact(o_ptr))
		{
			artifact_type *a_ptr = &a_info[o_ptr->name1];

			for (i = 0; i < TR_FLAG_SIZE; i++)
				flgs[i] = a_ptr->flags[i];
		}

		/* Random artifact ! */
		for (i = 0; i < TR_FLAG_SIZE; i++)
			flgs[i] |= o_ptr->art_flags[i];
	}

	if (object_is_smith(o_ptr))
	{
		int add = o_ptr->xtra3 - 1;

		if (add < TR_FLAG_MAX)
		{
			add_flag(flgs, add);
		}
		else if (add == ESSENCE_TMP_RES_ACID)
		{
			add_flag(flgs, TR_RES_ACID);
		}
		else if (add == ESSENCE_TMP_RES_ELEC)
		{
			add_flag(flgs, TR_RES_ELEC);
		}
		else if (add == ESSENCE_TMP_RES_FIRE)
		{
			add_flag(flgs, TR_RES_FIRE);
		}
		else if (add == ESSENCE_TMP_RES_COLD)
		{
			add_flag(flgs, TR_RES_COLD);
		}
		else if (add == ESSENCE_SH_FIRE)
		{
			add_flag(flgs, TR_RES_FIRE);
			add_flag(flgs, TR_SH_FIRE);
		}
		else if (add == ESSENCE_SH_ELEC)
		{
			add_flag(flgs, TR_RES_ELEC);
			add_flag(flgs, TR_SH_ELEC);
		}
		else if (add == ESSENCE_SH_COLD)
		{
			add_flag(flgs, TR_RES_COLD);
			add_flag(flgs, TR_SH_COLD);
		}
		else if (add == ESSENCE_RESISTANCE)
		{
			add_flag(flgs, TR_RES_ACID);
			add_flag(flgs, TR_RES_ELEC);
			add_flag(flgs, TR_RES_FIRE);
			add_flag(flgs, TR_RES_COLD);
		}
	}
}

/*!
 * @brief ���֥������Ȥ�ȯư����̾�Τ��֤��ʥ��֥롼����/�֥쥹��
 * @param o_ptr ̾�Τ�������븵�Υ��֥������ȹ�¤�λ��ȥݥ���
 * @return cptr ȯư̾�Τ��֤�ʸ����ݥ���
 */
static cptr item_activation_dragon_breath(object_type *o_ptr)
{
	static char desc[256];
	u32b flgs[TR_FLAG_SIZE]; /* for resistance flags */
	int i, n = 0;

	object_flags(o_ptr, flgs);
	strcpy(desc, _("", "breath "));

	for (i = 0; dragonbreath_info[i].flag != 0; i++)
	{
		if (have_flag(flgs, dragonbreath_info[i].flag))
		{
			if (n > 0) strcat(desc, _("��", ", "));
			strcat(desc, dragonbreath_info[i].name);
			n++;
		}
	}

	strcat(desc, _("�Υ֥쥹(250)", ""));

	return (desc);
}

/*!
 * @brief ���֥������Ȥ�ȯư����̾�Τ��֤��ʥ��֥롼����/���ѡ�
 * @param o_ptr ̾�Τ�������븵�Υ��֥������ȹ�¤�λ��ȥݥ���
 * @return cptr ȯư̾�Τ��֤�ʸ����ݥ���
 */
static cptr item_activation_aux(object_type *o_ptr)
{
	static char activation_detail[256];
	cptr desc;
	char timeout[32];
	int constant, dice;
	const activation_type* const act_ptr = find_activation_info(o_ptr);

	if (!act_ptr) return _("̤���", "something undefined");

	desc = act_ptr->desc;

	/* Overwrite description if it is special */
	switch (act_ptr->index) {
	case ACT_BR_FIRE:
		if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES))
			desc = _("�б�Υ֥쥹 (200) �ȲФؤ�����", "breath of fire (200) and resist fire");
		break;
	case ACT_BR_COLD:
		if ((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE))
			desc = _("�䵤�Υ֥쥹 (200) ���䵤�ؤ�����", "breath of cold (200) and resist cold");
		break;
	case ACT_BR_DRAGON:
		desc = item_activation_dragon_breath(o_ptr);
		break;
	case ACT_AGGRAVATE:
		if (o_ptr->name1 == ART_HYOUSIGI)
			desc = _("����ڤ��Ǥ��ʤ餹", "beat wooden clappers");
		break;
	case ACT_RESIST_ACID:
		if (((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ACID)) || (o_ptr->name2 == EGO_BRAND_ACID))
			desc = _("�����åɡ��ܡ��� (100) �Ȼ��ؤ�����", "ball of acid (100) and resist acid");
		break;
	case ACT_RESIST_FIRE:
		if (((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES)) || (o_ptr->name2 == EGO_BRAND_FIRE))
			desc = _("�ե��������ܡ��� (100) �ȲФؤ�����", "ball of fire (100) and resist fire");
		break;
	case ACT_RESIST_COLD:
		if (((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE)) || (o_ptr->name2 == EGO_BRAND_COLD))
			desc = _("���������ܡ��� (100) ���䵤�ؤ�����", "ball of cold (100) and resist cold");
		break;
	case ACT_RESIST_ELEC:
		if (((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ELEC)) || (o_ptr->name2 == EGO_BRAND_ELEC))
			desc = _("����������ܡ��� (100) ���ŷ�ؤ�����", "ball of elec (100) and resist elec");
		break;
	case ACT_RESIST_POIS:
		if (o_ptr->name2 == EGO_BRAND_POIS)
			desc = _("������ (100) ���Ǥؤ�����", "ball of poison (100) and resist elec");
		break;
	}

	/* Timeout description */
	constant = act_ptr->timeout.constant;
	dice = act_ptr->timeout.dice;
	if (constant == 0 && dice == 0) {
		/* We can activate it every turn */
		strcpy(timeout, _("���ĤǤ�", "every turn"));
	} else if (constant < 0) {
		/* Activations that have special timeout */
		switch (act_ptr->index) {
		case ACT_BR_FIRE:
			sprintf(timeout, _("%d ��������", "every %d turns"),
				((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_FLAMES)) ? 200 : 250);
			break;
		case ACT_BR_COLD:
			sprintf(timeout, _("%d ��������", "every %d turns"),
				((o_ptr->tval == TV_RING) && (o_ptr->sval == SV_RING_ICE)) ? 200 : 250);
			break;
		case ACT_TERROR:
			strcpy(timeout, _("3*(��٥�+10) ��������", "every 3 * (level+10) turns"));
			break;
		case ACT_MURAMASA:
			strcpy(timeout, _("��Ψ50%�ǲ����", "(destroyed 50%)"));
			break;
		default:
			strcpy(timeout, "undefined");
			break;
		}
	} else {
		/* Normal timeout activations */
		char constant_str[16], dice_str[16];
		sprintf(constant_str, "%d", constant);
		sprintf(dice_str, "d%d", dice);
		sprintf(timeout, _("%s%s%s ��������", "every %s%s%s turns"),
			(constant > 0) ? constant_str : "",
			(constant > 0 && dice > 0) ? "+" : "",
			(dice > 0) ? dice_str : "");
	}

	/* Build detail activate description */
	sprintf(activation_detail, _("%s : %s", "%s %s"), desc, timeout);

	return activation_detail;
}

/*!
 * @brief ���֥������Ȥ�ȯư����̾�Τ��֤��ʥᥤ��롼����� /
 * Determine the "Activation" (if any) for an artifact Return a string, or NULL for "no activation"
 * @param o_ptr ̾�Τ�������븵�Υ��֥������ȹ�¤�λ��ȥݥ���
 * @return cptr ȯư̾�Τ��֤�ʸ����ݥ���
 */
cptr item_activation(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];

	/* Extract the flags */
	object_flags(o_ptr, flgs);

	/* Require activation ability */
#ifdef JP
	if (!(have_flag(flgs, TR_ACTIVATE))) return ("�ʤ�");
#else
	if (!(have_flag(flgs, TR_ACTIVATE))) return ("nothing");
#endif

	/* Get an explain of an activation */
	if (activation_index(o_ptr))
	{
		return item_activation_aux(o_ptr);
	}

	/* Special items */
	if (o_ptr->tval == TV_WHISTLE)
	{
#ifdef JP
return "�ڥåȸƤӴ� : 100+d100��������";
#else
		return "call pet every 100+d100 turns";
#endif
	}

	if (o_ptr->tval == TV_CAPTURE)
	{
#ifdef JP
return "��󥹥������ᤨ�롢���ϲ������롣";
#else
		return "captures or releases a monster.";
#endif
	}

	/* Oops */
#ifdef JP
	return "���ⵯ���ʤ�";
#else
	return "Nothing";
#endif
}


/*!
 * @brief ���֥������Ȥ�*����*���Ƥ�ܽҤ���ɽ������ /
 * Describe a "fully identified" item
 * @param o_ptr *����*�����������븵�Υ��֥������ȹ�¤�λ��ȥݥ���
 * @param mode ɽ�����ץ����
 * @return ��ɮ���٤����󤬰�ĤǤ⤢�ä����TRUE����Ĥ�ʤ�ɽ��������󥻥뤵�줿���FALSE���֤���
 */
bool screen_object(object_type *o_ptr, u32b mode)
{
	int                     i = 0, j, k;

	u32b flgs[TR_FLAG_SIZE];

	char temp[70 * 20];
	cptr            info[128];
	char o_name[MAX_NLEN];
	int wid, hgt;
	int rad;
	char desc[256];

	int trivial_info = 0;

	/* Extract the flags */
	object_flags(o_ptr, flgs);

	/* Extract the description */
	{
		roff_to_buf(o_ptr->name1 ? (a_text + a_info[o_ptr->name1].text) :
			    (k_text + k_info[o_ptr->k_idx].text),
			    77 - 15, temp, sizeof(temp));
		for (j = 0; temp[j]; j += 1 + strlen(&temp[j]))
		{ info[i] = &temp[j]; i++;}
	}

	if (object_is_equipment(o_ptr))
	{
		/* Descriptions of a basic equipment is just a flavor */
		trivial_info = i;
	}

	/* Mega-Hack -- describe activation */
	if (have_flag(flgs, TR_ACTIVATE))
	{
#ifdef JP
info[i++] = "��ư�����Ȥ��θ���...";
#else
		info[i++] = "It can be activated for...";
#endif

		info[i++] = item_activation(o_ptr);
#ifdef JP
info[i++] = "...�������������Ƥ��ʤ���Фʤ�ʤ���";
#else
		info[i++] = "...if it is being worn.";
#endif

	}

	/* Figurines, a hack */
	if (o_ptr->tval == TV_FIGURINE)
	{
#ifdef JP
info[i++] = "������ꤲ�����ڥåȤ��Ѳ����롣";
#else
		info[i++] = "It will transform into a pet when thrown.";
#endif

	}

	/* Figurines, a hack */
	if (o_ptr->name1 == ART_STONEMASK)
	{
#ifdef JP
info[i++] = "��������������Ԥϵ۷쵴�ˤʤ롣";
#else
		info[i++] = "It makes you turn into a vampire permanently.";
#endif

	}

	if ((o_ptr->tval == TV_SWORD) && (o_ptr->sval == SV_DOKUBARI))
	{
#ifdef JP
info[i++] = "�������������ݤ����Ȥ����롣";
#else
		info[i++] = "It will attempt to kill a monster instantly.";
#endif

	}

	if ((o_ptr->tval == TV_POLEARM) && (o_ptr->sval == SV_DEATH_SCYTHE))
	{
#ifdef JP
info[i++] = "����ϼ�ʬ���Ȥ˹��⤬�֤äƤ��뤳�Ȥ����롣";
#else
		info[i++] = "It causes you to strike yourself sometimes.";
#endif

#ifdef JP
info[i++] = "�����̵Ũ�ΥХꥢ���ڤ�������";
#else
		info[i++] = "It always penetrates invulnerability barriers.";
#endif
	}

	if (o_ptr->name2 == EGO_2WEAPON)
	{
#ifdef JP
info[i++] = "���������ή�Ǥ�̿��Ψ����夵���롣";
#else
		info[i++] = "It affects your ability to hit when you are wielding two weapons.";
#endif

	}

	if (have_flag(flgs, TR_EASY_SPELL))
	{
#ifdef JP
info[i++] = "�������ˡ������٤򲼤��롣";
#else
		info[i++] = "It affects your ability to cast spells.";
#endif
	}

	if (o_ptr->name2 == EGO_AMU_FOOL)
	{
#ifdef JP
info[i++] = "�������ˡ������٤�夲�롣";
#else
		info[i++] = "It interferes with casting spells.";
#endif
	}

	if (o_ptr->name2 == EGO_RING_THROW)
	{
#ifdef JP
info[i++] = "�����ʪ�򶯤��ꤲ�뤳�Ȥ��ǽ�ˤ��롣";
#else
		info[i++] = "It provides great strength when you throw an item.";
#endif
	}

	if (o_ptr->name2 == EGO_AMU_NAIVETY)
	{
#ifdef JP
info[i++] = "�������ˡ���Ϥ򲼤��롣";
#else
		info[i++] = "It decreases your magic resistance.";
#endif
	}

	if (o_ptr->tval == TV_STATUE)
	{
		monster_race *r_ptr = &r_info[o_ptr->pval];

		if (o_ptr->pval == MON_BULLGATES)
#ifdef JP
			info[i++] = "����������˾�����Ѥ���������";
#else
			info[i++] = "It is shameful.";
#endif
		else if ( r_ptr->flags2 & (RF2_ELDRITCH_HORROR))
#ifdef JP
			info[i++] = "����������˾���ȶ�����";
#else
		info[i++] = "It is fearful.";
#endif
		else
#ifdef JP
			info[i++] = "����������˾���ȳڤ�����";
#else
		info[i++] = "It is cheerful.";
#endif
	}
	
	/* Hack -- describe lite's */
	
	if (o_ptr->name2 == EGO_LITE_DARKNESS) info[i++] = _("�������������ʤ���", "It provides no light.");
	
	rad = 0;
	if (have_flag(flgs, TR_LITE_1) && o_ptr->name2 != EGO_LITE_DARKNESS)  rad += 1;
	if (have_flag(flgs, TR_LITE_2) && o_ptr->name2 != EGO_LITE_DARKNESS)  rad += 2;
	if (have_flag(flgs, TR_LITE_3) && o_ptr->name2 != EGO_LITE_DARKNESS)  rad += 3;
	if (have_flag(flgs, TR_LITE_M1)) rad -= 1;
	if (have_flag(flgs, TR_LITE_M2)) rad -= 2;
	if (have_flag(flgs, TR_LITE_M3)) rad -= 3;
	
	if(o_ptr->name2 == EGO_LITE_SHINE) rad++;
		
	if (have_flag(flgs, TR_LITE_FUEL) && o_ptr->name2 != EGO_LITE_DARKNESS)
	{
		if(rad > 0) sprintf(desc, _("�����ǳ�����ˤ�ä�������(Ⱦ�� %d)������롣", "It provides light (radius %d) when fueled."), rad);	
	}
	else
	{
		if(rad > 0) sprintf(desc, _("����ϱʱ�ʤ�������(Ⱦ�� %d)������롣", "It provides light (radius %d) forever."), rad);	
		if(rad < 0) sprintf(desc, _("������������Ⱦ�¤򶹤��(Ⱦ�¤�-%d)��", "It decreases radius of light source by %d."), -rad);
	}
	
	if(rad != 0) info[i++] = desc;

	
	if (o_ptr->name2 == EGO_LITE_LONG)
	{
		info[i++] = _("�����Ĺ�������������������롣", "It provides light for much longer time.");
	}

	/* And then describe it fully */

	if (have_flag(flgs, TR_RIDING))
	{
		if ((o_ptr->tval == TV_POLEARM) && ((o_ptr->sval == SV_LANCE) || (o_ptr->sval == SV_HEAVY_LANCE)))
#ifdef JP
info[i++] = "����Ͼ���������˻Ȥ��䤹����";
#else
			info[i++] = "It is made for use while riding.";
#endif
		else
		{
#ifdef JP
			info[i++] = "����Ͼ�����Ǥ�Ȥ��䤹����";
#else
			info[i++] = "It is suitable for use while riding.";
#endif
			/* This information is not important enough */
			trivial_info++;
		}
	}
	if (have_flag(flgs, TR_STR))
	{
#ifdef JP
info[i++] = "��������Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your strength.";
#endif

	}
	if (have_flag(flgs, TR_INT))
	{
#ifdef JP
info[i++] = "�������ǽ�˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your intelligence.";
#endif

	}
	if (have_flag(flgs, TR_WIS))
	{
#ifdef JP
info[i++] = "����ϸ����˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your wisdom.";
#endif

	}
	if (have_flag(flgs, TR_DEX))
	{
#ifdef JP
info[i++] = "����ϴ��Ѥ��˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your dexterity.";
#endif

	}
	if (have_flag(flgs, TR_CON))
	{
#ifdef JP
info[i++] = "������ѵ��Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your constitution.";
#endif

	}
	if (have_flag(flgs, TR_CHR))
	{
#ifdef JP
info[i++] = "�����̥�Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your charisma.";
#endif

	}

	if (have_flag(flgs, TR_MAGIC_MASTERY))
	{
#ifdef JP
info[i++] = "�������ˡƻ�����ǽ�Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your ability to use magic devices.";
#endif

	}
	if (have_flag(flgs, TR_STEALTH))
	{
#ifdef JP
info[i++] = "����ϱ�̩��ưǽ�Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your stealth.";
#endif

	}
	if (have_flag(flgs, TR_SEARCH))
	{
#ifdef JP
info[i++] = "�����õ��ǽ�Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your searching.";
#endif

	}
	if (have_flag(flgs, TR_INFRA))
	{
#ifdef JP
info[i++] = "������ֳ������Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your infravision.";
#endif

	}
	if (have_flag(flgs, TR_TUNNEL))
	{
#ifdef JP
info[i++] = "����Ϻη�ǽ�Ϥ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your ability to tunnel.";
#endif

	}
	if (have_flag(flgs, TR_SPEED))
	{
#ifdef JP
info[i++] = "����ϥ��ԡ��ɤ˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your speed.";
#endif

	}
	if (have_flag(flgs, TR_BLOWS))
	{
#ifdef JP
info[i++] = "������Ƿ����˱ƶ���ڤܤ���";
#else
		info[i++] = "It affects your attack speed.";
#endif

	}

	if (have_flag(flgs, TR_BRAND_ACID))
	{
#ifdef JP
info[i++] = "����ϻ��ˤ�ä��礭�ʥ��᡼����Ϳ���롣";
#else
		info[i++] = "It does extra damage from acid.";
#endif

	}
	if (have_flag(flgs, TR_BRAND_ELEC))
	{
#ifdef JP
info[i++] = "������ŷ�ˤ�ä��礭�ʥ��᡼����Ϳ���롣";
#else
		info[i++] = "It does extra damage from electricity.";
#endif

	}
	if (have_flag(flgs, TR_BRAND_FIRE))
	{
#ifdef JP
info[i++] = "����ϲб�ˤ�ä��礭�ʥ��᡼����Ϳ���롣";
#else
		info[i++] = "It does extra damage from fire.";
#endif

	}
	if (have_flag(flgs, TR_BRAND_COLD))
	{
#ifdef JP
info[i++] = "������䵤�ˤ�ä��礭�ʥ��᡼����Ϳ���롣";
#else
		info[i++] = "It does extra damage from frost.";
#endif

	}

	if (have_flag(flgs, TR_BRAND_POIS))
	{
#ifdef JP
info[i++] = "�����Ũ���Ǥ��롣";
#else
		info[i++] = "It poisons your foes.";
#endif

	}

	if (have_flag(flgs, TR_CHAOTIC))
	{
#ifdef JP
info[i++] = "����ϥ�����Ū�ʸ��̤�ڤܤ���";
#else
		info[i++] = "It produces chaotic effects.";
#endif

	}

	if (have_flag(flgs, TR_VAMPIRIC))
	{
#ifdef JP
info[i++] = "�����Ũ����ҥåȥݥ���Ȥ�ۼ����롣";
#else
		info[i++] = "It drains life from your foes.";
#endif

	}

	if (have_flag(flgs, TR_IMPACT))
	{
#ifdef JP
info[i++] = "������Ͽ̤򵯤������Ȥ��Ǥ��롣";
#else
		info[i++] = "It can cause earthquakes.";
#endif

	}

	if (have_flag(flgs, TR_VORPAL))
	{
#ifdef JP
info[i++] = "����������ڤ�̣���Ԥ�Ũ�����Ǥ��뤳�Ȥ��Ǥ��롣";
#else
		info[i++] = "It is very sharp and can cut your foes.";
#endif

	}

	if (have_flag(flgs, TR_KILL_DRAGON))
	{
#ifdef JP
info[i++] = "����ϥɥ饴��ˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of dragons.";
#endif

	}
	else if (have_flag(flgs, TR_SLAY_DRAGON))
	{
#ifdef JP
info[i++] = "����ϥɥ饴����Ф����ä˶���٤��Ϥ�ȯ�����롣";
#else
		info[i++] = "It is especially deadly against dragons.";
#endif

	}

	if (have_flag(flgs, TR_KILL_ORC))
	{
#ifdef JP
info[i++] = "����ϥ������ˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of orcs.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_ORC))
	{
#ifdef JP
info[i++] = "����ϥ��������Ф����ä˶���٤��Ϥ�ȯ�����롣";
#else
		info[i++] = "It is especially deadly against orcs.";
#endif

	}

	if (have_flag(flgs, TR_KILL_TROLL))
	{
#ifdef JP
info[i++] = "����ϥȥ��ˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of trolls.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_TROLL))
	{
#ifdef JP
info[i++] = "����ϥȥ����Ф����ä˶���٤��Ϥ�ȯ�����롣";
#else
		info[i++] = "It is especially deadly against trolls.";
#endif

	}

	if (have_flag(flgs, TR_KILL_GIANT))
	{
#ifdef JP
info[i++] = "����ϵ�ͤˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of giants.";
#endif
	}
	else if (have_flag(flgs, TR_SLAY_GIANT))
	{
#ifdef JP
info[i++] = "����ϥ��㥤����Ȥ��Ф����ä˶���٤��Ϥ�ȯ�����롣";
#else
		info[i++] = "It is especially deadly against giants.";
#endif

	}

	if (have_flag(flgs, TR_KILL_DEMON))
	{
#ifdef JP
info[i++] = "����ϥǡ����ˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of demons.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_DEMON))
	{
#ifdef JP
info[i++] = "����ϥǡ������Ф������ʤ��Ϥ�ȯ�����롣";
#else
		info[i++] = "It strikes at demons with holy wrath.";
#endif

	}

	if (have_flag(flgs, TR_KILL_UNDEAD))
	{
#ifdef JP
info[i++] = "����ϥ���ǥåɤˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of undead.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_UNDEAD))
	{
#ifdef JP
info[i++] = "����ϥ���ǥåɤ��Ф������ʤ��Ϥ�ȯ�����롣";
#else
		info[i++] = "It strikes at undead with holy wrath.";
#endif

	}

	if (have_flag(flgs, TR_KILL_EVIL))
	{
#ifdef JP
info[i++] = "����ϼٰ��ʤ�¸�ߤˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of evil monsters.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_EVIL))
	{
#ifdef JP
info[i++] = "����ϼٰ��ʤ�¸�ߤ��Ф������ʤ��Ϥǹ��⤹�롣";
#else
		info[i++] = "It fights against evil with holy fury.";
#endif

	}

	if (have_flag(flgs, TR_KILL_ANIMAL))
	{
#ifdef JP
info[i++] = "����ϼ�������ưʪ�ˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of natural creatures.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_ANIMAL))
	{
#ifdef JP
info[i++] = "����ϼ�������ưʪ���Ф����ä˶���٤��Ϥ�ȯ�����롣";
#else
		info[i++] = "It is especially deadly against natural creatures.";
#endif

	}

	if (have_flag(flgs, TR_KILL_HUMAN))
	{
#ifdef JP
info[i++] = "����Ͽʹ֤ˤȤäƤ�ŷŨ�Ǥ��롣";
#else
		info[i++] = "It is a great bane of humans.";
#endif

	}
	if (have_flag(flgs, TR_SLAY_HUMAN))
	{
#ifdef JP
info[i++] = "����Ͽʹ֤��Ф����ä˶���٤��Ϥ�ȯ�����롣";
#else
		info[i++] = "It is especially deadly against humans.";
#endif

	}

	if (have_flag(flgs, TR_FORCE_WEAPON))
	{
#ifdef JP
info[i++] = "����ϻ��ѼԤ����Ϥ�Ȥäƹ��⤹�롣";
#else
		info[i++] = "It powerfully strikes at a monster using your mana.";
#endif

	}
	if (have_flag(flgs, TR_DEC_MANA))
	{
#ifdef JP
info[i++] = "��������Ϥξ���򲡤����롣";
#else
		info[i++] = "It decreases your mana consumption.";
#endif

	}
	if (have_flag(flgs, TR_SUST_STR))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ������Ϥ�ݻ����롣";
#else
		info[i++] = "It sustains your strength.";
#endif

	}
	if (have_flag(flgs, TR_SUST_INT))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ�����ǽ��ݻ����롣";
#else
		info[i++] = "It sustains your intelligence.";
#endif

	}
	if (have_flag(flgs, TR_SUST_WIS))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ��θ�����ݻ����롣";
#else
		info[i++] = "It sustains your wisdom.";
#endif

	}
	if (have_flag(flgs, TR_SUST_DEX))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ��δ��Ѥ���ݻ����롣";
#else
		info[i++] = "It sustains your dexterity.";
#endif

	}
	if (have_flag(flgs, TR_SUST_CON))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ����ѵ��Ϥ�ݻ����롣";
#else
		info[i++] = "It sustains your constitution.";
#endif

	}
	if (have_flag(flgs, TR_SUST_CHR))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ���̥�Ϥ�ݻ����롣";
#else
		info[i++] = "It sustains your charisma.";
#endif

	}

	if (have_flag(flgs, TR_IM_ACID))
	{
#ifdef JP
info[i++] = "����ϻ����Ф��봰�����ȱ֤�����롣";
#else
		info[i++] = "It provides immunity to acid.";
#endif

	}
	if (have_flag(flgs, TR_IM_ELEC))
	{
#ifdef JP
info[i++] = "������ŷ���Ф��봰�����ȱ֤�����롣";
#else
		info[i++] = "It provides immunity to electricity.";
#endif

	}
	if (have_flag(flgs, TR_IM_FIRE))
	{
#ifdef JP
info[i++] = "����ϲФ��Ф��봰�����ȱ֤�����롣";
#else
		info[i++] = "It provides immunity to fire.";
#endif

	}
	if (have_flag(flgs, TR_IM_COLD))
	{
#ifdef JP
info[i++] = "����ϴ������Ф��봰�����ȱ֤�����롣";
#else
		info[i++] = "It provides immunity to cold.";
#endif

	}

	if (have_flag(flgs, TR_THROW))
	{
#ifdef JP
info[i++] = "�����Ũ���ꤲ���礭�ʥ��᡼����Ϳ���뤳�Ȥ��Ǥ��롣";
#else
		info[i++] = "It is perfectly balanced for throwing.";
#endif
	}

	if (have_flag(flgs, TR_FREE_ACT))
	{
#ifdef JP
info[i++] = "�����������Ф��봰�����ȱ֤�����롣";
#else
		info[i++] = "It provides immunity to paralysis.";
#endif

	}
	if (have_flag(flgs, TR_HOLD_LIFE))
	{
#ifdef JP
info[i++] = "�������̿�ϵۼ����Ф�������������롣";
#else
		info[i++] = "It provides resistance to life draining.";
#endif

	}
	if (have_flag(flgs, TR_RES_FEAR))
	{
#ifdef JP
info[i++] = "����϶��ݤؤδ���������������롣";
#else
		info[i++] = "It makes you completely fearless.";
#endif

	}
	if (have_flag(flgs, TR_RES_ACID))
	{
#ifdef JP
info[i++] = "����ϻ��ؤ�����������롣";
#else
		info[i++] = "It provides resistance to acid.";
#endif

	}
	if (have_flag(flgs, TR_RES_ELEC))
	{
#ifdef JP
info[i++] = "������ŷ�ؤ�����������롣";
#else
		info[i++] = "It provides resistance to electricity.";
#endif

	}
	if (have_flag(flgs, TR_RES_FIRE))
	{
#ifdef JP
info[i++] = "����ϲФؤ�����������롣";
#else
		info[i++] = "It provides resistance to fire.";
#endif

	}
	if (have_flag(flgs, TR_RES_COLD))
	{
#ifdef JP
info[i++] = "����ϴ����ؤ�����������롣";
#else
		info[i++] = "It provides resistance to cold.";
#endif

	}
	if (have_flag(flgs, TR_RES_POIS))
	{
#ifdef JP
info[i++] = "������Ǥؤ�����������롣";
#else
		info[i++] = "It provides resistance to poison.";
#endif

	}

	if (have_flag(flgs, TR_RES_LITE))
	{
#ifdef JP
info[i++] = "����������ؤ�����������롣";
#else
		info[i++] = "It provides resistance to light.";
#endif

	}
	if (have_flag(flgs, TR_RES_DARK))
	{
#ifdef JP
info[i++] = "����ϰŹ��ؤ�����������롣";
#else
		info[i++] = "It provides resistance to dark.";
#endif

	}

	if (have_flag(flgs, TR_RES_BLIND))
	{
#ifdef JP
info[i++] = "��������ܤؤ�����������롣";
#else
		info[i++] = "It provides resistance to blindness.";
#endif

	}
	if (have_flag(flgs, TR_RES_CONF))
	{
#ifdef JP
info[i++] = "����Ϻ���ؤ�����������롣";
#else
		info[i++] = "It provides resistance to confusion.";
#endif

	}
	if (have_flag(flgs, TR_RES_SOUND))
	{
#ifdef JP
info[i++] = "����Ϲ첻�ؤ�����������롣";
#else
		info[i++] = "It provides resistance to sound.";
#endif

	}
	if (have_flag(flgs, TR_RES_SHARDS))
	{
#ifdef JP
info[i++] = "��������Ҥؤ�����������롣";
#else
		info[i++] = "It provides resistance to shards.";
#endif

	}

	if (have_flag(flgs, TR_RES_NETHER))
	{
#ifdef JP
info[i++] = "������Ϲ��ؤ�����������롣";
#else
		info[i++] = "It provides resistance to nether.";
#endif

	}
	if (have_flag(flgs, TR_RES_NEXUS))
	{
#ifdef JP
info[i++] = "����ϰ��̺���ؤ�����������롣";
#else
		info[i++] = "It provides resistance to nexus.";
#endif

	}
	if (have_flag(flgs, TR_RES_CHAOS))
	{
#ifdef JP
info[i++] = "����ϥ������ؤ�����������롣";
#else
		info[i++] = "It provides resistance to chaos.";
#endif

	}
	if (have_flag(flgs, TR_RES_DISEN))
	{
#ifdef JP
info[i++] = "����������ؤ�����������롣";
#else
		info[i++] = "It provides resistance to disenchantment.";
#endif

	}

	if (have_flag(flgs, TR_LEVITATION))
	{
#ifdef JP
info[i++] = "���������⤯���Ȥ��ǽ�ˤ��롣";
#else
		info[i++] = "It allows you to levitate.";
#endif

	}
		
	if (have_flag(flgs, TR_SEE_INVIS))
	{
		info[i++] = _("�����Ʃ���ʥ�󥹥����򸫤뤳�Ȥ��ǽ�ˤ��롣", "It allows you to see invisible monsters.");
	}
	if (have_flag(flgs, TR_TELEPATHY))
	{
#ifdef JP
info[i++] = "����ϥƥ�ѥ���ǽ�Ϥ�����롣";
#else
		info[i++] = "It gives telepathic powers.";
#endif

	}
	if (have_flag(flgs, TR_ESP_ANIMAL))
	{
#ifdef JP
info[i++] = "����ϼ���������ʪ���Τ��롣";
#else
		info[i++] = "It senses natural creatures.";
#endif

	}
	if (have_flag(flgs, TR_ESP_UNDEAD))
	{
#ifdef JP
info[i++] = "����ϥ���ǥåɤ��Τ��롣";
#else
		info[i++] = "It senses undead.";
#endif

	}
	if (have_flag(flgs, TR_ESP_DEMON))
	{
#ifdef JP
info[i++] = "����ϰ�����Τ��롣";
#else
		info[i++] = "It senses demons.";
#endif

	}
	if (have_flag(flgs, TR_ESP_ORC))
	{
#ifdef JP
info[i++] = "����ϥ��������Τ��롣";
#else
		info[i++] = "It senses orcs.";
#endif

	}
	if (have_flag(flgs, TR_ESP_TROLL))
	{
#ifdef JP
info[i++] = "����ϥȥ����Τ��롣";
#else
		info[i++] = "It senses trolls.";
#endif

	}
	if (have_flag(flgs, TR_ESP_GIANT))
	{
#ifdef JP
info[i++] = "����ϵ�ͤ��Τ��롣";
#else
		info[i++] = "It senses giants.";
#endif

	}
	if (have_flag(flgs, TR_ESP_DRAGON))
	{
#ifdef JP
info[i++] = "����ϥɥ饴����Τ��롣";
#else
		info[i++] = "It senses dragons.";
#endif

	}
	if (have_flag(flgs, TR_ESP_HUMAN))
	{
#ifdef JP
info[i++] = "����Ͽʹ֤��Τ��롣";
#else
		info[i++] = "It senses humans.";
#endif

	}
	if (have_flag(flgs, TR_ESP_EVIL))
	{
#ifdef JP
info[i++] = "����ϼٰ���¸�ߤ��Τ��롣";
#else
		info[i++] = "It senses evil creatures.";
#endif

	}
	if (have_flag(flgs, TR_ESP_GOOD))
	{
#ifdef JP
info[i++] = "��������ɤ�¸�ߤ��Τ��롣";
#else
		info[i++] = "It senses good creatures.";
#endif

	}
	if (have_flag(flgs, TR_ESP_NONLIVING))
	{
#ifdef JP
info[i++] = "����ϳ�ư����̵��ʪ�Τ��Τ��롣";
#else
		info[i++] = "It senses non-living creatures.";
#endif

	}
	if (have_flag(flgs, TR_ESP_UNIQUE))
	{
#ifdef JP
info[i++] = "��������̤ʶ�Ũ���Τ��롣";
#else
		info[i++] = "It senses unique monsters.";
#endif

	}
	if (have_flag(flgs, TR_SLOW_DIGEST))
	{
#ifdef JP
info[i++] = "����Ϥ��ʤ��ο�����դ��٤����롣";
#else
		info[i++] = "It slows your metabolism.";
#endif

	}
	if (have_flag(flgs, TR_REGEN))
	{
#ifdef JP
info[i++] = "��������ϲ����Ϥ򶯲����롣";
#else
		info[i++] = "It speeds your regenerative powers.";
#endif

	}
	if (have_flag(flgs, TR_WARNING))
	{
#ifdef JP
info[i++] = "����ϴ����Ф��Ʒٹ��ȯ���롣";
#else
		info[i++] = "It warns you of danger";
#endif

	}
	if (have_flag(flgs, TR_REFLECT))
	{
		info[i++] = _("�������μ�ʸ��ȿ�ͤ��롣", "It reflects bolt spells.");
	}
	if (have_flag(flgs, TR_SH_FIRE))
	{
		info[i++] = _("����ϱ�ΥХꥢ��ĥ�롣", "It produces a fiery sheath.");
	}
	if (have_flag(flgs, TR_SH_ELEC))
	{
		info[i++] = _("������ŵ��ΥХꥢ��ĥ�롣", "It produces an electric sheath.");
	}
	if (have_flag(flgs, TR_SH_COLD))
	{
#ifdef JP
info[i++] = "������䵤�ΥХꥢ��ĥ�롣";
#else
		info[i++] = "It produces a sheath of coldness.";
#endif

	}
	if (have_flag(flgs, TR_NO_MAGIC))
	{
#ifdef JP
info[i++] = "�����ȿ��ˡ�Хꥢ��ĥ�롣";
#else
		info[i++] = "It produces an anti-magic shell.";
#endif

	}
	if (have_flag(flgs, TR_NO_TELE))
	{
#ifdef JP
info[i++] = "����ϥƥ�ݡ��Ȥ���⤹�롣";
#else
		info[i++] = "It prevents teleportation.";
#endif

	}
	if (have_flag(flgs, TR_XTRA_MIGHT))
	{
#ifdef JP
info[i++] = "������𡿥ܥ�ȡ��Ƥ��궯�Ϥ�ȯ�ͤ��뤳�Ȥ��Ǥ��롣";
#else
		info[i++] = "It fires missiles with extra might.";
#endif

	}
	if (have_flag(flgs, TR_XTRA_SHOTS))
	{
#ifdef JP
info[i++] = "������𡿥ܥ�ȡ��Ƥ������᤯ȯ�ͤ��뤳�Ȥ��Ǥ��롣";
#else
		info[i++] = "It fires missiles excessively fast.";
#endif

	}

	if (have_flag(flgs, TR_BLESSED))
	{
#ifdef JP
info[i++] = "����Ͽ��˽�ʡ����Ƥ��롣";
#else
		info[i++] = "It has been blessed by the gods.";
#endif

	}

	if (object_is_cursed(o_ptr))
	{
		if (o_ptr->curse_flags & TRC_PERMA_CURSE)
		{
#ifdef JP
info[i++] = "����ϱʱ�μ������������Ƥ��롣";
#else
			info[i++] = "It is permanently cursed.";
#endif

		}
		else if (o_ptr->curse_flags & TRC_HEAVY_CURSE)
		{
#ifdef JP
info[i++] = "����϶��Ϥʼ������������Ƥ��롣";
#else
			info[i++] = "It is heavily cursed.";
#endif

		}
		else
		{
#ifdef JP
info[i++] = "����ϼ����Ƥ��롣";
#else
			info[i++] = "It is cursed.";
#endif

			/*
			 * It's a trivial infomation since there is
			 * fake inscription {cursed}
			 */
			trivial_info++;
		}
	}

	if ((have_flag(flgs, TR_TY_CURSE)) || (o_ptr->curse_flags & TRC_TY_CURSE))
	{
		info[i++] = _("��������Ťβҡ�������ǰ���ɤäƤ��롣", "It carries an ancient foul curse.");
	}
	if ((have_flag(flgs, TR_AGGRAVATE)) || (o_ptr->curse_flags & TRC_AGGRAVATE))
	{
		info[i++] = _("������ն�Υ�󥹥������ܤ餻�롣", "It aggravates nearby creatures.");
	}
	if ((have_flag(flgs, TR_DRAIN_EXP)) || (o_ptr->curse_flags & TRC_DRAIN_EXP))
	{
		info[i++] = _("����Ϸи��ͤ�ۤ���롣", "It drains experience.");
	}
	if (o_ptr->curse_flags & TRC_SLOW_REGEN)
	{
		info[i++] = _("����ϲ����Ϥ���롣", "It slows your regenerative powers.");
	}
	if ((o_ptr->curse_flags & TRC_ADD_L_CURSE) || have_flag(flgs, TR_ADD_L_CURSE))
	{
		info[i++] = _("����ϼ夤���������䤹��","It adds weak curses.");
	}
	if ((o_ptr->curse_flags & TRC_ADD_H_CURSE) || have_flag(flgs, TR_ADD_H_CURSE))
	{
		info[i++] = _("����϶��Ϥʼ��������䤹��","It adds heavy curses.");
	}
	if ((have_flag(flgs, TR_CALL_ANIMAL)) || (o_ptr->curse_flags & TRC_CALL_ANIMAL))
	{
		info[i++] = _("�����ưʪ��ƤӴ󤻤롣", "It attracts animals.");
	}
	if ((have_flag(flgs, TR_CALL_DEMON)) || (o_ptr->curse_flags & TRC_CALL_DEMON))
	{
		info[i++] = _("����ϰ����ƤӴ󤻤롣", "It attracts demons.");
	}
	if ((have_flag(flgs, TR_CALL_DRAGON)) || (o_ptr->curse_flags & TRC_CALL_DRAGON))
	{
		info[i++] = _("����ϥɥ饴���ƤӴ󤻤롣", "It attracts dragons.");
	}
	if ((have_flag(flgs, TR_CALL_UNDEAD)) || (o_ptr->curse_flags & TRC_CALL_UNDEAD))
	{
		info[i++] = _("����ϻ����ƤӴ󤻤롣", "It attracts undeads.");
	}
	if ((have_flag(flgs, TR_COWARDICE)) ||  (o_ptr->curse_flags & TRC_COWARDICE))
	{
		info[i++] = _("����϶��ݴ��������������", "It makes you subject to cowardice.");
	}
	if ((have_flag(flgs, TR_TELEPORT)) || (o_ptr->curse_flags & TRC_TELEPORT))
	{
		info[i++] = _("����ϥ�����ʥƥ�ݡ��Ȥ������������", "It induces random teleportation.");
	}
	if ((have_flag(flgs, TR_LOW_MELEE)) || o_ptr->curse_flags & TRC_LOW_MELEE)
	{
		info[i++] = _("����Ϲ���򳰤��䤹����", "It causes you to miss blows.");
	}
	if ((have_flag(flgs, TR_LOW_AC)) || (o_ptr->curse_flags & TRC_LOW_AC))
	{
		info[i++] = _("����Ϲ��������䤹����", "It helps your enemies' blows.");
	}
	if ((have_flag(flgs, TR_LOW_MAGIC)) || (o_ptr->curse_flags & TRC_LOW_MAGIC))
	{
		info[i++] = _("�������ˡ�򾧤��ˤ������롣", "It encumbers you while spellcasting.");
	}
	if ((have_flag(flgs, TR_FAST_DIGEST)) || (o_ptr->curse_flags & TRC_FAST_DIGEST))
	{
		info[i++] = _("����Ϥ��ʤ��ο�����դ�®�����롣", "It speeds your metabolism.");
	}
	if ((have_flag(flgs, TR_DRAIN_HP)) || (o_ptr->curse_flags & TRC_DRAIN_HP))
	{
		info[i++] = _("����Ϥ��ʤ������Ϥ�ۤ���롣", "It drains you.");
	}
	if ((have_flag(flgs, TR_DRAIN_MANA)) || (o_ptr->curse_flags & TRC_DRAIN_MANA))
	{
		info[i++] = _("����Ϥ��ʤ������Ϥ�ۤ���롣", "It drains your mana.");
	}

	/* Describe about this kind of object instead of THIS fake object */
	if (mode & SCROBJ_FAKE_OBJECT)
	{
		switch (o_ptr->tval)
		{
		case TV_RING:
			switch (o_ptr->sval)
			{
			case SV_RING_LORDLY:
#ifdef JP
				info[i++] = "����ϴ��Ĥ��Υ����������������롣";
#else
				info[i++] = "It provides some random resistances.";
#endif
				break;
			case SV_RING_WARNING:
#ifdef JP
				info[i++] = "����ϤҤȤĤ�����ESP�������������롣";
#else
				info[i++] = "It may provide a low rank ESP.";
#endif
				break;
			}
			break;

		case TV_AMULET:
			switch (o_ptr->sval)
			{
			case SV_AMULET_RESISTANCE:
#ifdef JP
				info[i++] = "������Ǥؤ������������������롣";
#else
				info[i++] = "It may provides resistance to poison.";
#endif
#ifdef JP
				info[i++] = "����ϥ�����������������������롣";
#else
				info[i++] = "It may provide a random resistances.";
#endif
				break;
			case SV_AMULET_THE_MAGI:
#ifdef JP
				info[i++] = "����Ϻ���ǣ��ĤޤǤ�����ESP������롣";
#else
				info[i++] = "It provides up to three low rank ESPs.";
#endif
				break;
			}
			break;
		}
	}

	if (have_flag(flgs, TR_IGNORE_ACID) &&
	    have_flag(flgs, TR_IGNORE_ELEC) &&
	    have_flag(flgs, TR_IGNORE_FIRE) &&
	    have_flag(flgs, TR_IGNORE_COLD))
	{
#ifdef JP
		info[i++] = "����ϻ����ŷ⡦�бꡦ�䵤�ǤϽ��Ĥ��ʤ���";
#else
		info[i++] = "It cannot be harmed by the elements.";
#endif
	}
	else
	{
		if (have_flag(flgs, TR_IGNORE_ACID))
		{
#ifdef JP
			info[i++] = "����ϻ��ǤϽ��Ĥ��ʤ���";
#else
			info[i++] = "It cannot be harmed by acid.";
#endif
		}
		if (have_flag(flgs, TR_IGNORE_ELEC))
		{
#ifdef JP
			info[i++] = "������ŷ�ǤϽ��Ĥ��ʤ���";
#else
			info[i++] = "It cannot be harmed by electricity.";
#endif
		}
		if (have_flag(flgs, TR_IGNORE_FIRE))
		{
#ifdef JP
			info[i++] = "����ϲб�ǤϽ��Ĥ��ʤ���";
#else
			info[i++] = "It cannot be harmed by fire.";
#endif
		}
		if (have_flag(flgs, TR_IGNORE_COLD))
		{
#ifdef JP
			info[i++] = "������䵤�ǤϽ��Ĥ��ʤ���";
#else
			info[i++] = "It cannot be harmed by cold.";
#endif
		}
	}

	if (mode & SCROBJ_FORCE_DETAIL) trivial_info = 0;

	/* No relevant informations */
	if (i <= trivial_info) return (FALSE);

	/* Save the screen */
	screen_save();

	/* Get size */
	Term_get_size(&wid, &hgt);

	/* Display Item name */
	if (!(mode & SCROBJ_FAKE_OBJECT))
		object_desc(o_name, o_ptr, 0);
	else
		object_desc(o_name, o_ptr, (OD_NAME_ONLY | OD_STORE));

	prt(o_name, 0, 0);

	/* Erase the screen */
	for (k = 1; k < hgt; k++) prt("", k, 13);

	/* Label the information */
	if ((o_ptr->tval == TV_STATUE) && (o_ptr->sval == SV_PHOTO))
	{
		monster_race *r_ptr = &r_info[o_ptr->pval];
		int namelen = strlen(r_name + r_ptr->name);
		prt(format("%s: '", r_name + r_ptr->name), 1, 15);
		Term_queue_bigchar(18 + namelen, 1, r_ptr->x_attr, r_ptr->x_char, 0, 0);
		prt("'", 1, (use_bigtile ? 20 : 19) + namelen);
	}
	else
#ifdef JP
prt("     �����ƥ��ǽ��:", 1, 15);
#else
	prt("     Item Attributes:", 1, 15);
#endif

	/* We will print on top of the map (column 13) */
	for (k = 2, j = 0; j < i; j++)
	{
		/* Show the info */
		prt(info[j], k++, 15);

		/* Every 20 entries (lines 2 to 21), start over */
		if ((k == hgt - 2) && (j+1 < i))
		{
#ifdef JP
prt("-- ³�� --", k, 15);
#else
			prt("-- more --", k, 15);
#endif
			inkey();
			for (; k > 2; k--) prt("", k, 15);
		}
	}

	/* Wait for it */
#ifdef JP
prt("[���������򲡤��ȥ���������ޤ�]", k, 15);
#else
	prt("[Press any key to continue]", k, 15);
#endif

	inkey();

	/* Restore the screen */
	screen_load();

	/* Gave knowledge */
	return (TRUE);
}



/*!
 * @brief ���֥�����������������򥢥�ե��٥åȥ�٥���֤� /
 * Convert an inventory index into a one character label
 * @param i �ץ쥤�䡼�ν��/�������֥�������ID
 * @return �б����륢��ե��٥å�
 * @details Note that the label does NOT distinguish inven/equip.
 */
char index_to_label(int i)
{
	/* Indexes for "inven" are easy */
	if (i < INVEN_RARM) return (I2A(i));

	/* Indexes for "equip" are offset */
	return (I2A(i - INVEN_RARM));
}

/*!
 * @brief ���򥢥�ե��٥åȥ�٥뤫��ץ쥤�䡼�ν�����֥�������ID���֤� /
 * Convert a label into the index of an item in the "inven"
 * @return �б�����ID����������åȤ˥��֥������Ȥ�¸�ߤ��ʤ��ä����-1���֤� / Return "-1" if the label does not indicate a real item
 * @details Note that the label does NOT distinguish inven/equip.
 */
s16b label_to_inven(int c)
{
	int i;

	/* Convert */
	i = (islower(c) ? A2I(c) : -1);

	/* Verify the index */
	if ((i < 0) || (i > INVEN_PACK)) return (-1);

	/* Empty slots can never be chosen */
	if (!inventory[i].k_idx) return (-1);

	/* Return the index */
	return (i);
}


/*! See cmd5.c */
extern bool select_ring_slot;


/*!
 * @brief �ץ쥤�䡼�ν��/�������֥�������ID�������Ȥ����֤� /
 * @param i �ץ쥤�䡼�ν��/�������֥�������ID
 * @return �����Ȥʤ��TRUE���֤���
 */
static bool is_ring_slot(int i)
{
	return (i == INVEN_RIGHT) || (i == INVEN_LEFT);
}


/*!
 * @brief ���򥢥�ե��٥åȥ�٥뤫��ץ쥤�䡼���������֥�������ID���֤� /
 * Convert a label into the index of a item in the "equip"
 * @return �б�����ID����������åȤ˥��֥������Ȥ�¸�ߤ��ʤ��ä����-1���֤� / Return "-1" if the label does not indicate a real item
 */
s16b label_to_equip(int c)
{
	int i;

	/* Convert */
	i = (islower(c) ? A2I(c) : -1) + INVEN_RARM;

	/* Verify the index */
	if ((i < INVEN_RARM) || (i >= INVEN_TOTAL)) return (-1);

	if (select_ring_slot) return is_ring_slot(i) ? i : -1;

	/* Empty slots can never be chosen */
	if (!inventory[i].k_idx) return (-1);

	/* Return the index */
	return (i);
}



/*!
 * @brief ���֥������Ȥγ�����������ID���֤� /
 * Determine which equipment slot (if any) an item likes
 * @param o_ptr ̾�Τ�������븵�Υ��֥������ȹ�¤�λ��ȥݥ���
 * @return �б�������������ID
 */
s16b wield_slot(object_type *o_ptr)
{
	/* Slot for equipment */
	switch (o_ptr->tval)
	{
		case TV_DIGGING:
		case TV_HAFTED:
		case TV_POLEARM:
		case TV_SWORD:
		{
			if (!inventory[INVEN_RARM].k_idx) return (INVEN_RARM);
			if (inventory[INVEN_LARM].k_idx) return (INVEN_RARM);
			return (INVEN_LARM);
		}

		case TV_CAPTURE:
		case TV_CARD:
		case TV_SHIELD:
		{
			if (!inventory[INVEN_LARM].k_idx) return (INVEN_LARM);
			if (inventory[INVEN_RARM].k_idx) return (INVEN_LARM);
			return (INVEN_RARM);
		}

		case TV_BOW:
		{
			return (INVEN_BOW);
		}

		case TV_RING:
		{
			/* Use the right hand first */
			if (!inventory[INVEN_RIGHT].k_idx) return (INVEN_RIGHT);

			/* Use the left hand for swapping (by default) */
			return (INVEN_LEFT);
		}

		case TV_AMULET:
		case TV_WHISTLE:
		{
			return (INVEN_NECK);
		}

		case TV_LITE:
		{
			return (INVEN_LITE);
		}

		case TV_DRAG_ARMOR:
		case TV_HARD_ARMOR:
		case TV_SOFT_ARMOR:
		{
			return (INVEN_BODY);
		}

		case TV_CLOAK:
		{
			return (INVEN_OUTER);
		}

		case TV_CROWN:
		case TV_HELM:
		{
			return (INVEN_HEAD);
		}

		case TV_GLOVES:
		{
			return (INVEN_HANDS);
		}

		case TV_BOOTS:
		{
			return (INVEN_FEET);
		}
	}

	/* No slot available */
	return (-1);
}

/*!
 * @brief ���/�������֥�������ID������ɽ�����֤� /
 * Return a string mentioning how a given item is carried
 * @param i ����ɽ�������ץ쥤�䡼�ν��/�������֥�������ID
 * @return ����ɽ����ʸ����ݥ���
 */
cptr mention_use(int i)
{
	cptr p;

	/* Examine the location */
	switch (i)
	{
#ifdef JP
		case INVEN_RARM:  p = p_ptr->heavy_wield[0] ? "������" : ((p_ptr->ryoute && p_ptr->migite) ? " ξ��" : (left_hander ? " ����" : " ����")); break;
#else
		case INVEN_RARM:  p = p_ptr->heavy_wield[0] ? "Just lifting" : (p_ptr->migite ? "Wielding" : "On arm"); break;
#endif

#ifdef JP
		case INVEN_LARM:  p = p_ptr->heavy_wield[1] ? "������" : ((p_ptr->ryoute && p_ptr->hidarite) ? " ξ��" : (left_hander ? " ����" : " ����")); break;
#else
		case INVEN_LARM:  p = p_ptr->heavy_wield[1] ? "Just lifting" : (p_ptr->hidarite ? "Wielding" : "On arm"); break;
#endif

#ifdef JP
		case INVEN_BOW:   p = (adj_str_hold[p_ptr->stat_ind[A_STR]] < inventory[i].weight / 10) ? "������" : "�ͷ���"; break;
#else
		case INVEN_BOW:   p = (adj_str_hold[p_ptr->stat_ind[A_STR]] < inventory[i].weight / 10) ? "Just holding" : "Shooting"; break;
#endif

#ifdef JP
		case INVEN_RIGHT: p = (left_hander ? "�����" : "�����"); break;
#else
		case INVEN_RIGHT: p = (left_hander ? "On left hand" : "On right hand"); break;
#endif

#ifdef JP
		case INVEN_LEFT:  p = (left_hander ? "�����" : "�����"); break;
#else
		case INVEN_LEFT:  p = (left_hander ? "On right hand" : "On left hand"); break;
#endif

#ifdef JP
		case INVEN_NECK:  p = "  ��"; break;
#else
		case INVEN_NECK:  p = "Around neck"; break;
#endif

#ifdef JP
		case INVEN_LITE:  p = " ����"; break;
#else
		case INVEN_LITE:  p = "Light source"; break;
#endif

#ifdef JP
		case INVEN_BODY:  p = "  ��"; break;
#else
		case INVEN_BODY:  p = "On body"; break;
#endif

#ifdef JP
		case INVEN_OUTER: p = "�Τξ�"; break;
#else
		case INVEN_OUTER: p = "About body"; break;
#endif

#ifdef JP
		case INVEN_HEAD:  p = "  Ƭ"; break;
#else
		case INVEN_HEAD:  p = "On head"; break;
#endif

#ifdef JP
		case INVEN_HANDS: p = "  ��"; break;
#else
		case INVEN_HANDS: p = "On hands"; break;
#endif

#ifdef JP
		case INVEN_FEET:  p = "  ­"; break;
#else
		case INVEN_FEET:  p = "On feet"; break;
#endif

#ifdef JP
		default:          p = "���å�"; break;
#else
		default:          p = "In pack"; break;
#endif
	}

	/* Return the result */
	return p;
}


/*!
 * @brief ���/�������֥�������ID�θ��ߤΰ������ξ���ɽ�����֤� /
 * Return a string describing how a given item is being worn.
 * @param i ����ɽ�������ץ쥤�䡼�ν��/�������֥�������ID
 * @return ����ɽ�����Ƥ�ʸ����ݥ���
 * @details
 * Currently, only used for items in the equipment, not inventory.
 */
cptr describe_use(int i)
{
	cptr p;

	switch (i)
	{
#ifdef JP
		case INVEN_RARM:  p = p_ptr->heavy_wield[0] ? "�������" : ((p_ptr->ryoute && p_ptr->migite) ? "ξ����������Ƥ���" : (left_hander ? "������������Ƥ���" : "������������Ƥ���")); break;
#else
		case INVEN_RARM:  p = p_ptr->heavy_wield[0] ? "just lifting" : (p_ptr->migite ? "attacking monsters with" : "wearing on your arm"); break;
#endif

#ifdef JP
		case INVEN_LARM:  p = p_ptr->heavy_wield[1] ? "�������" : ((p_ptr->ryoute && p_ptr->hidarite) ? "ξ����������Ƥ���" : (left_hander ? "������������Ƥ���" : "������������Ƥ���")); break;
#else
		case INVEN_LARM:  p = p_ptr->heavy_wield[1] ? "just lifting" : (p_ptr->hidarite ? "attacking monsters with" : "wearing on your arm"); break;
#endif

#ifdef JP
		case INVEN_BOW:   p = (adj_str_hold[p_ptr->stat_ind[A_STR]] < inventory[i].weight / 10) ? "���Ĥ����������դ�" : "�ͷ��Ѥ��������Ƥ���"; break;
#else
		case INVEN_BOW:   p = (adj_str_hold[p_ptr->stat_ind[A_STR]] < inventory[i].weight / 10) ? "just holding" : "shooting missiles with"; break;
#endif

#ifdef JP
		case INVEN_RIGHT: p = (left_hander ? "����λؤˤϤ�Ƥ���" : "����λؤˤϤ�Ƥ���"); break;
#else
		case INVEN_RIGHT: p = (left_hander ? "wearing on your left hand" : "wearing on your right hand"); break;
#endif

#ifdef JP
		case INVEN_LEFT:  p = (left_hander ? "����λؤˤϤ�Ƥ���" : "����λؤˤϤ�Ƥ���"); break;
#else
		case INVEN_LEFT:  p = (left_hander ? "wearing on your right hand" : "wearing on your left hand"); break;
#endif

#ifdef JP
		case INVEN_NECK:  p = "��ˤ����Ƥ���"; break;
#else
		case INVEN_NECK:  p = "wearing around your neck"; break;
#endif

#ifdef JP
		case INVEN_LITE:  p = "�����ˤ��Ƥ���"; break;
#else
		case INVEN_LITE:  p = "using to light the way"; break;
#endif

#ifdef JP
		case INVEN_BODY:  p = "�Τ���Ƥ���"; break;
#else
		case INVEN_BODY:  p = "wearing on your body"; break;
#endif

#ifdef JP
		case INVEN_OUTER: p = "�ȤˤޤȤäƤ���"; break;
#else
		case INVEN_OUTER: p = "wearing on your back"; break;
#endif

#ifdef JP
		case INVEN_HEAD:  p = "Ƭ�ˤ��֤äƤ���"; break;
#else
		case INVEN_HEAD:  p = "wearing on your head"; break;
#endif

#ifdef JP
		case INVEN_HANDS: p = "��ˤĤ��Ƥ���"; break;
#else
		case INVEN_HANDS: p = "wearing on your hands"; break;
#endif

#ifdef JP
		case INVEN_FEET:  p = "­�ˤϤ��Ƥ���"; break;
#else
		case INVEN_FEET:  p = "wearing on your feet"; break;
#endif

#ifdef JP
		default:          p = "���å������äƤ���"; break;
#else
		default:          p = "carrying in your pack"; break;
#endif
	}

	/* Return the result */
	return p;
}


/*!
 * @brief tval/sval����Υ١��������ƥब�ץ쥤�䡼�λ��Ѳ�ǽ����ˡ�񤫤ɤ������֤� /
 * Hack: Check if a spellbook is one of the realms we can use. -- TY
 * @param book_tval �١��������ƥ��tval
 * @param book_sval �١��������ƥ��sval
 * @return ���Ѳ�ǽ����ˡ��ʤ��TRUE���֤���
 */

bool check_book_realm(const byte book_tval, const byte book_sval)
{
	if (book_tval < TV_LIFE_BOOK) return FALSE;
	if (p_ptr->pclass == CLASS_SORCERER)
	{
		return is_magic(tval2realm(book_tval));
	}
	else if (p_ptr->pclass == CLASS_RED_MAGE)
	{
		if (is_magic(tval2realm(book_tval)))
			return ((book_tval == TV_ARCANE_BOOK) || (book_sval < 2));
	}
	return (REALM1_BOOK == book_tval || REALM2_BOOK == book_tval);
}

/*!
 * @brief �����ƥबitem_tester_hook�����Х�ؿ��ݥ��󥿤ξ����������Ƥ��뤫���֤����Ѵؿ�
 * Check an item against the item tester info
 * @param o_ptr Ƚ���Ԥ��������֥������ȹ�¤�λ��ȥݥ���
 * @return item_tester_hook�λ����衢����¾�����Ĥ����㳰�˱�����TRUE/FALSE���֤���
 */
bool item_tester_okay(object_type *o_ptr)
{
	/* Hack -- allow listing empty slots */
	if (item_tester_full) return (TRUE);

	/* Require an item */
	if (!o_ptr->k_idx) return (FALSE);

	/* Hack -- ignore "gold" */
	if (o_ptr->tval == TV_GOLD)
	{
		/* See xtra2.c */
		extern bool show_gold_on_floor;

		if (!show_gold_on_floor) return (FALSE);
	}

	/* Check the tval */
	if (item_tester_tval)
	{
		/* Is it a spellbook? If so, we need a hack -- TY */
		if ((item_tester_tval <= TV_DEATH_BOOK) &&
			(item_tester_tval >= TV_LIFE_BOOK))
			return check_book_realm(o_ptr->tval, o_ptr->sval);
		else
			if (item_tester_tval != o_ptr->tval) return (FALSE);
	}

	/* Check the hook */
	if (item_tester_hook)
	{
		if (!(*item_tester_hook)(o_ptr)) return (FALSE);
	}

	/* Assume okay */
	return (TRUE);
}


/*!
 * @brief ��������ƥ������ɽ������ /
 * Choice window "shadow" of the "show_inven()" function
 * @return �ʤ�
 */
void display_inven(void)
{
	register        int i, n, z = 0;
	object_type     *o_ptr;
	byte            attr = TERM_WHITE;
	char            tmp_val[80];
	char            o_name[MAX_NLEN];
	int             wid, hgt;

	/* Get size */
	Term_get_size(&wid, &hgt);

	/* Find the "final" slot */
	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Track */
		z = i + 1;
	}

	/* Display the pack */
	for (i = 0; i < z; i++)
	{
		/* Examine the item */
		o_ptr = &inventory[i];

		/* Start with an empty "index" */
		tmp_val[0] = tmp_val[1] = tmp_val[2] = ' ';

		/* Is this item "acceptable"? */
		if (item_tester_okay(o_ptr))
		{
			/* Prepare an "index" */
			tmp_val[0] = index_to_label(i);

			/* Bracket the "index" --(-- */
			tmp_val[1] = ')';
		}

		/* Display the index (or blank space) */
		Term_putstr(0, i, 3, TERM_WHITE, tmp_val);

		/* Obtain an item description */
		object_desc(o_name, o_ptr, 0);

		/* Obtain the length of the description */
		n = strlen(o_name);

		/* Get a color */
		attr = tval_to_attr[o_ptr->tval % 128];

		/* Grey out charging items */
		if (o_ptr->timeout)
		{
			attr = TERM_L_DARK;
		}

		/* Display the entry itself */
		Term_putstr(3, i, n, attr, o_name);

		/* Erase the rest of the line */
		Term_erase(3+n, i, 255);

		/* Display the weight if needed */
		if (show_weights)
		{
			int wgt = o_ptr->weight * o_ptr->number;
#ifdef JP
			sprintf(tmp_val, "%3d.%1d kg", lbtokg1(wgt),lbtokg2(wgt) );
#else
			sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
#endif

			prt(tmp_val, i, wid - 9);
		}
	}

	/* Erase the rest of the window */
	for (i = z; i < hgt; i++)
	{
		/* Erase the line */
		Term_erase(0, i, 255);
	}
}



/*!
 * @brief ���������ƥ������ɽ������ /
 * Choice window "shadow" of the "show_equip()" function
 * @return �ʤ�
 */
void display_equip(void)
{
	register        int i, n;
	object_type     *o_ptr;
	byte            attr = TERM_WHITE;
	char            tmp_val[80];
	char            o_name[MAX_NLEN];
	int             wid, hgt;

	/* Get size */
	Term_get_size(&wid, &hgt);

	/* Display the equipment */
	for (i = INVEN_RARM; i < INVEN_TOTAL; i++)
	{
		/* Examine the item */
		o_ptr = &inventory[i];

		/* Start with an empty "index" */
		tmp_val[0] = tmp_val[1] = tmp_val[2] = ' ';

		/* Is this item "acceptable"? */
		if (select_ring_slot ? is_ring_slot(i) : item_tester_okay(o_ptr))
		{
			/* Prepare an "index" */
			tmp_val[0] = index_to_label(i);

			/* Bracket the "index" --(-- */
			tmp_val[1] = ')';
		}

		/* Display the index (or blank space) */
		Term_putstr(0, i - INVEN_RARM, 3, TERM_WHITE, tmp_val);

		/* Obtain an item description */
		if ((((i == INVEN_RARM) && p_ptr->hidarite) || ((i == INVEN_LARM) && p_ptr->migite)) && p_ptr->ryoute)
		{
#ifdef JP
			strcpy(o_name, "(����ξ�����)");
#else
			strcpy(o_name, "(wielding with two-hands)");
#endif
			attr = TERM_WHITE;
		}
		else
		{
			object_desc(o_name, o_ptr, 0);
			attr = tval_to_attr[o_ptr->tval % 128];
		}

		/* Obtain the length of the description */
		n = strlen(o_name);

		/* Grey out charging items */
		if (o_ptr->timeout)
		{
			attr = TERM_L_DARK;
		}

		/* Display the entry itself */
		Term_putstr(3, i - INVEN_RARM, n, attr, o_name);

		/* Erase the rest of the line */
		Term_erase(3+n, i - INVEN_RARM, 255);

		/* Display the weight (if needed) */
		if (show_weights)
		{
			int wgt = o_ptr->weight * o_ptr->number;
#ifdef JP
			sprintf(tmp_val, "%3d.%1d kg", lbtokg1(wgt) , lbtokg2(wgt));
#else
			sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
#endif

			prt(tmp_val, i - INVEN_RARM, wid - (show_labels ? 28 : 9));
		}

		/* Display the slot description (if needed) */
		if (show_labels)
		{
			Term_putstr(wid - 20, i - INVEN_RARM, -1, TERM_WHITE, " <-- ");
			prt(mention_use(i), i - INVEN_RARM, wid - 15);
		}
	}

	/* Erase the rest of the window */
	for (i = INVEN_TOTAL - INVEN_RARM; i < hgt; i++)
	{
		/* Clear that line */
		Term_erase(0, i, 255);
	}
}


/*!
 * @brief ���/�������֥������Ȥ����򥿥���Ϳ����/�����˳������륪�֥������Ȥ����뤫���֤� /
 * Find the "first" inventory object with the given "tag".
 * @param cp �б����륿��ID��Ϳ���뻲�ȥݥ���
 * @param tag �������륪�֥������Ȥ����뤫��Ĵ�٤�������
 * @param mode ������������ڤ��ؤ�
 * @return �����˳������륪�֥������Ȥ�����ʤ�TRUE���֤�
 * @detail 
 * A "tag" is a numeral "n" appearing as "@n" anywhere in the\n
 * inscription of an object.  Alphabetical characters don't work as a\n
 * tag in this form.\n
 *\n
 * Also, the tag "@xn" will work as well, where "n" is a any tag-char,\n
 * and "x" is the "current" command_cmd code.\n
 */
static bool get_tag(int *cp, char tag, int mode)
{
	int i, start, end;
	cptr s;

	/* Extract index from mode */
	switch (mode)
	{
	case USE_EQUIP:
		start = INVEN_RARM;
		end = INVEN_TOTAL - 1;
		break;

	case USE_INVEN:
		start = 0;
		end = INVEN_PACK - 1;
		break;

	default:
		return FALSE;
	}

	/**** Find a tag in the form of {@x#} (allow alphabet tag) ***/

	/* Check every inventory object */
	for (i = start; i <= end; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Skip empty inscriptions */
		if (!o_ptr->inscription) continue;

		/* Skip non-choice */
		if (!item_tester_okay(o_ptr)) continue;

		/* Find a '@' */
		s = my_strchr(quark_str(o_ptr->inscription), '@');

		/* Process all tags */
		while (s)
		{
			/* Check the special tags */
			if ((s[1] == command_cmd) && (s[2] == tag))
			{
				/* Save the actual inventory ID */
				*cp = i;

				/* Success */
				return (TRUE);
			}

			/* Find another '@' */
			s = my_strchr(s + 1, '@');
		}
	}


	/**** Find a tag in the form of {@#} (allows only numerals)  ***/

	/* Don't allow {@#} with '#' being alphabet */
	if (tag < '0' || '9' < tag)
	{
		/* No such tag */
		return FALSE;
	}

	/* Check every object */
	for (i = start; i <= end; i++)
	{
		object_type *o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Skip empty inscriptions */
		if (!o_ptr->inscription) continue;

		/* Skip non-choice */
		if (!item_tester_okay(o_ptr)) continue;

		/* Find a '@' */
		s = my_strchr(quark_str(o_ptr->inscription), '@');

		/* Process all tags */
		while (s)
		{
			/* Check the normal tags */
			if (s[1] == tag)
			{
				/* Save the actual inventory ID */
				*cp = i;

				/* Success */
				return (TRUE);
			}

			/* Find another '@' */
			s = my_strchr(s + 1, '@');
		}
	}

	/* No such tag */
	return (FALSE);
}


/*!
 * @brief �����֥������Ȥ����򥿥���Ϳ����/�����˳������륪�֥������Ȥ����뤫���֤� /
 * Find the "first" inventory object with the given "tag".
 * @param cp �б����륿��ID��Ϳ���뻲�ȥݥ���
 * @param tag �������륪�֥������Ȥ����뤫��Ĵ�٤�������
 * @param floor_list ���奢���ƥ������
 * @param floor_num  ���奢���ƥ������ID
 * @return �����˳������륪�֥������Ȥ�����ʤ�TRUE���֤�
 * @detail 
 * A "tag" is a numeral "n" appearing as "@n" anywhere in the\n
 * inscription of an object.  Alphabetical characters don't work as a\n
 * tag in this form.\n
 *\n
 * Also, the tag "@xn" will work as well, where "n" is a any tag-char,\n
 * and "x" is the "current" command_cmd code.\n
 */
static bool get_tag_floor(int *cp, char tag, int floor_list[], int floor_num)
{
	int i;
	cptr s;

	/**** Find a tag in the form of {@x#} (allow alphabet tag) ***/

	/* Check every object in the grid */
	for (i = 0; i < floor_num && i < 23; i++)
	{
		object_type *o_ptr = &o_list[floor_list[i]];

		/* Skip empty inscriptions */
		if (!o_ptr->inscription) continue;

		/* Find a '@' */
		s = my_strchr(quark_str(o_ptr->inscription), '@');

		/* Process all tags */
		while (s)
		{
			/* Check the special tags */
			if ((s[1] == command_cmd) && (s[2] == tag))
			{
				/* Save the actual floor object ID */
				*cp = i;

				/* Success */
				return (TRUE);
			}

			/* Find another '@' */
			s = my_strchr(s + 1, '@');
		}
	}


	/**** Find a tag in the form of {@#} (allows only numerals)  ***/

	/* Don't allow {@#} with '#' being alphabet */
	if (tag < '0' || '9' < tag)
	{
		/* No such tag */
		return FALSE;
	}

	/* Check every object in the grid */
	for (i = 0; i < floor_num && i < 23; i++)
	{
		object_type *o_ptr = &o_list[floor_list[i]];

		/* Skip empty inscriptions */
		if (!o_ptr->inscription) continue;

		/* Find a '@' */
		s = my_strchr(quark_str(o_ptr->inscription), '@');

		/* Process all tags */
		while (s)
		{
			/* Check the normal tags */
			if (s[1] == tag)
			{
				/* Save the floor object ID */
				*cp = i;

				/* Success */
				return (TRUE);
			}

			/* Find another '@' */
			s = my_strchr(s + 1, '@');
		}
	}

	/* No such tag */
	return (FALSE);
}


/*!
 * @brief ����ID�ˤ��碌�ƥ�������ե��٥åȤΥꥹ�Ȥ��֤� /
 * Move around label characters with correspond tags
 * @param label ��٥�ꥹ�Ȥ��������ʸ���󻲾ȥݥ���
 * @param mode ����ʥꥹ�Ȥ������ʥꥹ�Ȥ����ڤ��ؤ�
 * @return �ʤ�
 */
static void prepare_label_string(char *label, int mode)
{
	cptr alphabet_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int  offset = (mode == USE_EQUIP) ? INVEN_RARM : 0;
	int  i;

	/* Prepare normal labels */
	strcpy(label, alphabet_chars);

	/* Move each label */
	for (i = 0; i < 52; i++)
	{
		int index;
		char c = alphabet_chars[i];

		/* Find a tag with this label */
		if (get_tag(&index, c, mode))
		{
			/* Delete the overwritten label */
			if (label[i] == c) label[i] = ' ';

			/* Move the label to the place of corresponding tag */
			label[index - offset] = c;
		}
	}
}


/*!
 * @brief ����ID�ˤ��碌�ƥ�������ե��٥åȤΥꥹ�Ȥ��֤�(���奢���ƥ���) /
 * Move around label characters with correspond tags (floor version)
 * @param label ��٥�ꥹ�Ȥ��������ʸ���󻲾ȥݥ���
 * @param floor_list ���奢���ƥ������
 * @param floor_num  ���奢���ƥ������ID
 * @return �ʤ�
 */
/*
 */
static void prepare_label_string_floor(char *label, int floor_list[], int floor_num)
{
	cptr alphabet_chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int  i;

	/* Prepare normal labels */
	strcpy(label, alphabet_chars);

	/* Move each label */
	for (i = 0; i < 52; i++)
	{
		int index;
		char c = alphabet_chars[i];

		/* Find a tag with this label */
		if (get_tag_floor(&index, c, floor_list, floor_num))
		{
			/* Delete the overwritten label */
			if (label[i] == c) label[i] = ' ';

			/* Move the label to the place of corresponding tag */
			label[index] = c;
		}
	}
}


/*!
 * @brief ��������ƥ��ɽ����Ԥ� /
 * Display the inventory.
 * @param target_item �����ƥ�����������Ԥ����ݤ���
 * @return ���򤷤������ƥ�Υ���
 * @details
 * Hack -- do not display "trailing" empty slots
 */
int show_inven(int target_item)
{
	int             i, j, k, l, z = 0;
	int             col, cur_col, len;
	object_type     *o_ptr;
	char            o_name[MAX_NLEN];
	char            tmp_val[80];
	int             out_index[23];
	byte            out_color[23];
	char            out_desc[23][MAX_NLEN];
	int             target_item_label = 0;
	int             wid, hgt;
	char            inven_label[52 + 1];

	/* Starting column */
	col = command_gap;

	/* Get size */
	Term_get_size(&wid, &hgt);

	/* Default "max-length" */
	len = wid - col - 1;


	/* Find the "final" slot */
	for (i = 0; i < INVEN_PACK; i++)
	{
		o_ptr = &inventory[i];

		/* Skip non-objects */
		if (!o_ptr->k_idx) continue;

		/* Track */
		z = i + 1;
	}

	prepare_label_string(inven_label, USE_INVEN);

	/* Display the inventory */
	for (k = 0, i = 0; i < z; i++)
	{
		o_ptr = &inventory[i];

		/* Is this item acceptable? */
		if (!item_tester_okay(o_ptr)) continue;

		/* Describe the object */
		object_desc(o_name, o_ptr, 0);

		/* Save the object index, color, and description */
		out_index[k] = i;
		out_color[k] = tval_to_attr[o_ptr->tval % 128];

		/* Grey out charging items */
		if (o_ptr->timeout)
		{
			out_color[k] = TERM_L_DARK;
		}

		(void)strcpy(out_desc[k], o_name);

		/* Find the predicted "line length" */
		l = strlen(out_desc[k]) + 5;

		/* Be sure to account for the weight */
		if (show_weights) l += 9;

		/* Account for icon if displayed */
		if (show_item_graph)
		{
			l += 2;
			if (use_bigtile) l++;
		}

		/* Maintain the maximum length */
		if (l > len) len = l;

		/* Advance to next "line" */
		k++;
	}

	/* Find the column to start in */
	col = (len > wid - 4) ? 0 : (wid - len - 1);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = out_index[j];

		/* Get the item */
		o_ptr = &inventory[i];

		/* Clear the line */
		prt("", j + 1, col ? col - 2 : col);

		if (use_menu && target_item)
		{
			if (j == (target_item-1))
			{
#ifdef JP
				strcpy(tmp_val, "��");
#else
				strcpy(tmp_val, "> ");
#endif
				target_item_label = i;
			}
			else strcpy(tmp_val, "  ");
		}
		else if (i <= INVEN_PACK)
		{
			/* Prepare an index --(-- */
			sprintf(tmp_val, "%c)", inven_label[i]);
		}
		else
		{
			/* Prepare an index --(-- */
			sprintf(tmp_val, "%c)", index_to_label(i));
		}

		/* Clear the line with the (possibly indented) index */
		put_str(tmp_val, j + 1, col);

		cur_col = col + 3;

		/* Display graphics for object, if desired */
		if (show_item_graph)
		{
			byte  a = object_attr(o_ptr);
			char c = object_char(o_ptr);

#ifdef AMIGA
			if (a & 0x80) a |= 0x40;
#endif

			Term_queue_bigchar(cur_col, j + 1, a, c, 0, 0);
			if (use_bigtile) cur_col++;

			cur_col += 2;
		}


		/* Display the entry itself */
		c_put_str(out_color[j], out_desc[j], j + 1, cur_col);

		/* Display the weight if needed */
		if (show_weights)
		{
			int wgt = o_ptr->weight * o_ptr->number;
#ifdef JP
			(void)sprintf(tmp_val, "%3d.%1d kg", lbtokg1(wgt) , lbtokg2(wgt) );
#else
			(void)sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
#endif

			prt(tmp_val, j + 1, wid - 9);
		}
	}

	/* Make a "shadow" below the list (only if needed) */
	if (j && (j < 23)) prt("", j + 1, col ? col - 2 : col);

	/* Save the new column */
	command_gap = col;

	return target_item_label;
}


/*!
 * @brief ���������ƥ��ɽ����Ԥ� /
 * Display the equipment.
 * @param target_item �����ƥ�����������Ԥ����ݤ���
 * @return ���򤷤������ƥ�Υ���
 */
int show_equip(int target_item)
{
	int             i, j, k, l;
	int             col, cur_col, len;
	object_type     *o_ptr;
	char            tmp_val[80];
	char            o_name[MAX_NLEN];
	int             out_index[23];
	byte            out_color[23];
	char            out_desc[23][MAX_NLEN];
	int             target_item_label = 0;
	int             wid, hgt;
	char            equip_label[52 + 1];

	/* Starting column */
	col = command_gap;

	/* Get size */
	Term_get_size(&wid, &hgt);

	/* Maximal length */
	len = wid - col - 1;


	/* Scan the equipment list */
	for (k = 0, i = INVEN_RARM; i < INVEN_TOTAL; i++)
	{
		o_ptr = &inventory[i];

		/* Is this item acceptable? */
		if (!(select_ring_slot ? is_ring_slot(i) : item_tester_okay(o_ptr)) &&
		    (!((((i == INVEN_RARM) && p_ptr->hidarite) || ((i == INVEN_LARM) && p_ptr->migite)) && p_ptr->ryoute) ||
		     item_tester_no_ryoute)) continue;

		/* Description */
		object_desc(o_name, o_ptr, 0);

		if ((((i == INVEN_RARM) && p_ptr->hidarite) || ((i == INVEN_LARM) && p_ptr->migite)) && p_ptr->ryoute)
		{
#ifdef JP
			(void)strcpy(out_desc[k],"(����ξ�����)");
#else
			(void)strcpy(out_desc[k],"(wielding with two-hands)");
#endif
			out_color[k] = TERM_WHITE;
		}
		else
		{
			(void)strcpy(out_desc[k], o_name);
			out_color[k] = tval_to_attr[o_ptr->tval % 128];
		}

		out_index[k] = i;
		/* Grey out charging items */
		if (o_ptr->timeout)
		{
			out_color[k] = TERM_L_DARK;
		}

		/* Extract the maximal length (see below) */
#ifdef JP
		l = strlen(out_desc[k]) + (2 + 1);
#else
		l = strlen(out_desc[k]) + (2 + 3);
#endif


		/* Increase length for labels (if needed) */
#ifdef JP
		if (show_labels) l += (7 + 2);
#else
		if (show_labels) l += (14 + 2);
#endif


		/* Increase length for weight (if needed) */
		if (show_weights) l += 9;

		if (show_item_graph) l += 2;

		/* Maintain the max-length */
		if (l > len) len = l;

		/* Advance the entry */
		k++;
	}

	/* Hack -- Find a column to start in */
#ifdef JP
	col = (len > wid - 6) ? 0 : (wid - len - 1);
#else
	col = (len > wid - 4) ? 0 : (wid - len - 1);
#endif

	prepare_label_string(equip_label, USE_EQUIP);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = out_index[j];

		/* Get the item */
		o_ptr = &inventory[i];

		/* Clear the line */
		prt("", j + 1, col ? col - 2 : col);

		if (use_menu && target_item)
		{
			if (j == (target_item-1))
			{
#ifdef JP
				strcpy(tmp_val, "��");
#else
				strcpy(tmp_val, "> ");
#endif
				target_item_label = i;
			}
			else strcpy(tmp_val, "  ");
		}
		else if (i >= INVEN_RARM)
		{
			/* Prepare an index --(-- */
			sprintf(tmp_val, "%c)", equip_label[i - INVEN_RARM]);
		}
		else /* Paranoia */
		{
			/* Prepare an index --(-- */
			sprintf(tmp_val, "%c)", index_to_label(i));
		}

		/* Clear the line with the (possibly indented) index */
		put_str(tmp_val, j+1, col);

		cur_col = col + 3;

		/* Display graphics for object, if desired */
		if (show_item_graph)
		{
			byte a = object_attr(o_ptr);
			char c = object_char(o_ptr);

#ifdef AMIGA
			if (a & 0x80) a |= 0x40;
#endif

			Term_queue_bigchar(cur_col, j + 1, a, c, 0, 0);
			if (use_bigtile) cur_col++;

			cur_col += 2;
		}

		/* Use labels */
		if (show_labels)
		{
			/* Mention the use */
#ifdef JP
			(void)sprintf(tmp_val, "%-7s: ", mention_use(i));
#else
			(void)sprintf(tmp_val, "%-14s: ", mention_use(i));
#endif

			put_str(tmp_val, j+1, cur_col);

			/* Display the entry itself */
#ifdef JP
			c_put_str(out_color[j], out_desc[j], j+1, cur_col + 9);
#else
			c_put_str(out_color[j], out_desc[j], j+1, cur_col + 16);
#endif
		}

		/* No labels */
		else
		{
			/* Display the entry itself */
			c_put_str(out_color[j], out_desc[j], j+1, cur_col);
		}

		/* Display the weight if needed */
		if (show_weights)
		{
			int wgt = o_ptr->weight * o_ptr->number;
#ifdef JP
			(void)sprintf(tmp_val, "%3d.%1d kg", lbtokg1(wgt) , lbtokg2(wgt) );
#else
			(void)sprintf(tmp_val, "%3d.%d lb", wgt / 10, wgt % 10);
#endif

			prt(tmp_val, j + 1, wid - 9);
		}
	}

	/* Make a "shadow" below the list (only if needed) */
	if (j && (j < 23)) prt("", j + 1, col ? col - 2 : col);

	/* Save the new column */
	command_gap = col;

	return target_item_label;
}

/*!
 * @brief ���֥�����ɥ��˽���ʡ������ʥꥹ�Ȥ�ɽ����Ԥ� /
 * Flip "inven" and "equip" in any sub-windows
 * @return �ʤ�
 */
void toggle_inven_equip(void)
{
	int j;

	/* Scan windows */
	for (j = 0; j < 8; j++)
	{
		/* Unused */
		if (!angband_term[j]) continue;

		/* Flip inven to equip */
		if (window_flag[j] & (PW_INVEN))
		{
			/* Flip flags */
			window_flag[j] &= ~(PW_INVEN);
			window_flag[j] |= (PW_EQUIP);

			/* Window stuff */
			p_ptr->window |= (PW_EQUIP);
		}

		/* Flip inven to equip */
		else if (window_flag[j] & (PW_EQUIP))
		{
			/* Flip flags */
			window_flag[j] &= ~(PW_EQUIP);
			window_flag[j] |= (PW_INVEN);

			/* Window stuff */
			p_ptr->window |= (PW_INVEN);
		}
	}
}

/*!
 * @brief ���򤷤������ƥ�γ�ǧ��������� /
 * Verify the choice of an item.
 * @param item ���򥢥��ƥ�ID
 * @return ��ǧ��Yes�ʤ�TRUE���֤���
 * @details The item can be negative to mean "item on floor".
 */
static bool verify(cptr prompt, int item)
{
	char        o_name[MAX_NLEN];
	char        out_val[MAX_NLEN+20];
	object_type *o_ptr;


	/* Inventory */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Floor */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	/* Describe */
	object_desc(o_name, o_ptr, 0);

	/* Prompt */
#ifdef JP
(void)sprintf(out_val, "%s%s�Ǥ���? ", prompt, o_name);
#else
	(void)sprintf(out_val, "%s %s? ", prompt, o_name);
#endif


	/* Query */
	return (get_check(out_val));
}


/*!
 * @brief ���򤷤������ƥ�γ�ǧ�����Υᥤ��롼���� /
 * @praram item ���򥢥��ƥ�ID
 * @return ��ǧ��Yes�ʤ�TRUE���֤���
 * @details The item can be negative to mean "item on floor".
 * Hack -- allow user to "prevent" certain choices
 */
static bool get_item_allow(int item)
{
	cptr s;

	object_type *o_ptr;

	if (!command_cmd) return TRUE; /* command_cmd is no longer effective */

	/* Inventory */
	if (item >= 0)
	{
		o_ptr = &inventory[item];
	}

	/* Floor */
	else
	{
		o_ptr = &o_list[0 - item];
	}

	/* No inscription */
	if (!o_ptr->inscription) return (TRUE);

	/* Find a '!' */
	s = my_strchr(quark_str(o_ptr->inscription), '!');

	/* Process preventions */
	while (s)
	{
		/* Check the "restriction" */
		if ((s[1] == command_cmd) || (s[1] == '*'))
		{
			/* Verify the choice */
#ifdef JP
if (!verify("������", item)) return (FALSE);
#else
			if (!verify("Really try", item)) return (FALSE);
#endif

		}

		/* Find another '!' */
		s = my_strchr(s + 1, '!');
	}

	/* Allow it */
	return (TRUE);
}


/*!
 * @brief �ץ쥤�䡼�ν��/�������֥������Ȥ������Τ�Τ����֤� /
 * Auxiliary function for "get_item()" -- test an index
 * @param item ���򥢥��ƥ�ID
 * @return ������ID�ʤ��TRUE���֤���
 */
static bool get_item_okay(int i)
{
	/* Illegal items */
	if ((i < 0) || (i >= INVEN_TOTAL)) return (FALSE);

	if (select_ring_slot) return is_ring_slot(i);

	/* Verify the item */
	if (!item_tester_okay(&inventory[i])) return (FALSE);

	/* Assume okay */
	return (TRUE);
}

/*!
 * @brief �ץ쥤�䡼�����֥������Ȥ򽦤����Ȥ��Ǥ�����֤����֤� /
 * Determine whether get_item() can get some item or not
 * @param item ���򥢥��ƥ�ID
 * @return �����ƥ�򽦤���ʤ��TRUE���֤���
 * @details assuming mode = (USE_EQUIP | USE_INVEN | USE_FLOOR).
 */
bool can_get_item(void)
{
	int j, floor_list[23], floor_num = 0;

	for (j = 0; j < INVEN_TOTAL; j++)
		if (item_tester_okay(&inventory[j]))
			return TRUE;

	floor_num = scan_floor(floor_list, py, px, 0x03);
	if (floor_num)
		return TRUE;

	return FALSE;
}

/*!
 * @brief ���֥���������������Ѵؿ� /
 * Let the user select an item, save its "index"
 * @param cp ���򤷤����֥������Ȥ�ID���֤���
 * @param pmt ������Ū�Υ�å�����
 * @param str ����Ǥ��륪�֥������Ȥ��ʤ����Υ���󥻥��å�����
 * @praram mode ���ץ����ե饰
 * @return �ץ쥤�䡼�ˤ�ꥢ���ƥब���򤵤줿�ʤ�TRUE���֤���/
 * Return TRUE only if an acceptable item was chosen by the user.\n
 * @details
 * The selected item must satisfy the "item_tester_hook()" function,\n
 * if that hook is set, and the "item_tester_tval", if that value is set.\n
 *\n
 * All "item_tester" restrictions are cleared before this function returns.\n
 *\n
 * The user is allowed to choose acceptable items from the equipment,\n
 * inventory, or floor, respectively, if the proper flag was given,\n
 * and there are any acceptable items in that location.\n
 *\n
 * The equipment or inventory are displayed (even if no acceptable\n
 * items are in that location) if the proper flag was given.\n
 *\n
 * If there are no acceptable items available anywhere, and "str" is\n
 * not NULL, then it will be used as the text of a warning message\n
 * before the function returns.\n
 *\n
 * Note that the user must press "-" to specify the item on the floor,\n
 * and there is no way to "examine" the item on the floor, while the\n
 * use of "capital" letters will "examine" an inventory/equipment item,\n
 * and prompt for its use.\n
 *\n
 * If a legal item is selected from the inventory, we save it in "cp"\n
 * directly (0 to 35), and return TRUE.\n
 *\n
 * If a legal item is selected from the floor, we save it in "cp" as\n
 * a negative (-1 to -511), and return TRUE.\n
 *\n
 * If no item is available, we do nothing to "cp", and we display a\n
 * warning message, using "str" if available, and return FALSE.\n
 *\n
 * If no item is selected, we do nothing to "cp", and return FALSE.\n
 *\n
 * Global "p_ptr->command_new" is used when viewing the inventory or equipment\n
 * to allow the user to enter a command while viewing those screens, and\n
 * also to induce "auto-enter" of stores, and other such stuff.\n
 *\n
 * Global "p_ptr->command_see" may be set before calling this function to start\n
 * out in "browse" mode.  It is cleared before this function returns.\n
 *\n
 * Global "p_ptr->command_wrk" is used to choose between equip/inven listings.\n
 * If it is TRUE then we are viewing inventory, else equipment.\n
 *\n
 * We always erase the prompt when we are done, leaving a blank line,\n
 * or a warning message, if appropriate, if no items are available.\n
 */
bool get_item(int *cp, cptr pmt, cptr str, int mode)
{
	s16b this_o_idx, next_o_idx = 0;

	char which = ' ';

	int j, k, i1, i2, e1, e2;

	bool done, item;

	bool oops = FALSE;

	bool equip = FALSE;
	bool inven = FALSE;
	bool floor = FALSE;

	bool allow_floor = FALSE;

	bool toggle = FALSE;

	char tmp_val[160];
	char out_val[160];

	/* See cmd5.c */
	extern bool select_the_force;

	int menu_line = (use_menu ? 1 : 0);
	int max_inven = 0;
	int max_equip = 0;

#ifdef ALLOW_REPEAT

	static char prev_tag = '\0';
	char cur_tag = '\0';

#endif /* ALLOW_REPEAT */

#ifdef ALLOW_EASY_FLOOR /* TNB */

	if (easy_floor || use_menu) return get_item_floor(cp, pmt, str, mode);

#endif /* ALLOW_EASY_FLOOR -- TNB */

	/* Extract args */
	if (mode & USE_EQUIP) equip = TRUE;
	if (mode & USE_INVEN) inven = TRUE;
	if (mode & USE_FLOOR) floor = TRUE;

#ifdef ALLOW_REPEAT

	/* Get the item index */
	if (repeat_pull(cp))
	{
		/* the_force */
		if (select_the_force && (*cp == INVEN_FORCE))
		{
			item_tester_tval = 0;
			item_tester_hook = NULL;
			command_cmd = 0; /* Hack -- command_cmd is no longer effective */
			return (TRUE);
		}

		/* Floor item? */
		else if (floor && (*cp < 0))
		{
			object_type *o_ptr;

			/* Special index */
			k = 0 - (*cp);

			/* Acquire object */
			o_ptr = &o_list[k];

			/* Validate the item */
			if (item_tester_okay(o_ptr))
			{
				/* Forget restrictions */
				item_tester_tval = 0;
				item_tester_hook = NULL;
				command_cmd = 0; /* Hack -- command_cmd is no longer effective */

				/* Success */
				return TRUE;
			}
		}

		else if ((inven && (*cp >= 0) && (*cp < INVEN_PACK)) ||
		         (equip && (*cp >= INVEN_RARM) && (*cp < INVEN_TOTAL)))
		{
			if (prev_tag && command_cmd)
			{
				/* Look up the tag and validate the item */
				if (!get_tag(&k, prev_tag, (*cp >= INVEN_RARM) ? USE_EQUIP : USE_INVEN)) /* Reject */;
				else if ((k < INVEN_RARM) ? !inven : !equip) /* Reject */;
				else if (!get_item_okay(k)) /* Reject */;
				else
				{
					/* Accept that choice */
					(*cp) = k;

					/* Forget restrictions */
					item_tester_tval = 0;
					item_tester_hook = NULL;
					command_cmd = 0; /* Hack -- command_cmd is no longer effective */

					/* Success */
					return TRUE;
				}

				prev_tag = '\0'; /* prev_tag is no longer effective */
			}

			/* Verify the item */
			else if (get_item_okay(*cp))
			{
				/* Forget restrictions */
				item_tester_tval = 0;
				item_tester_hook = NULL;
				command_cmd = 0; /* Hack -- command_cmd is no longer effective */

				/* Success */
				return TRUE;
			}
		}
	}

#endif /* ALLOW_REPEAT */


	/* Paranoia XXX XXX XXX */
	msg_print(NULL);


	/* Not done */
	done = FALSE;

	/* No item selected */
	item = FALSE;


	/* Full inventory */
	i1 = 0;
	i2 = INVEN_PACK - 1;

	/* Forbid inventory */
	if (!inven) i2 = -1;
	else if (use_menu)
	{
		for (j = 0; j < INVEN_PACK; j++)
			if (item_tester_okay(&inventory[j])) max_inven++;
	}

	/* Restrict inventory indexes */
	while ((i1 <= i2) && (!get_item_okay(i1))) i1++;
	while ((i1 <= i2) && (!get_item_okay(i2))) i2--;


	/* Full equipment */
	e1 = INVEN_RARM;
	e2 = INVEN_TOTAL - 1;

	/* Forbid equipment */
	if (!equip) e2 = -1;
	else if (use_menu)
	{
		for (j = INVEN_RARM; j < INVEN_TOTAL; j++)
			if (select_ring_slot ? is_ring_slot(j) : item_tester_okay(&inventory[j])) max_equip++;
		if (p_ptr->ryoute && !item_tester_no_ryoute) max_equip++;
	}

	/* Restrict equipment indexes */
	while ((e1 <= e2) && (!get_item_okay(e1))) e1++;
	while ((e1 <= e2) && (!get_item_okay(e2))) e2--;

	if (equip && p_ptr->ryoute && !item_tester_no_ryoute)
	{
		if (p_ptr->migite)
		{
			if (e2 < INVEN_LARM) e2 = INVEN_LARM;
		}
		else if (p_ptr->hidarite) e1 = INVEN_RARM;
	}


	/* Restrict floor usage */
	if (floor)
	{
		/* Scan all objects in the grid */
		for (this_o_idx = cave[py][px].o_idx; this_o_idx; this_o_idx = next_o_idx)
		{
			object_type *o_ptr;

			/* Acquire object */
			o_ptr = &o_list[this_o_idx];

			/* Acquire next object */
			next_o_idx = o_ptr->next_o_idx;

			/* Accept the item on the floor if legal */
			if (item_tester_okay(o_ptr) && (o_ptr->marked & OM_FOUND)) allow_floor = TRUE;
		}
	}

	/* Require at least one legal choice */
	if (!allow_floor && (i1 > i2) && (e1 > e2))
	{
		/* Cancel p_ptr->command_see */
		command_see = FALSE;

		/* Oops */
		oops = TRUE;

		/* Done */
		done = TRUE;

		if (select_the_force) {
		    *cp = INVEN_FORCE;
		    item = TRUE;
		}
	}

	/* Analyze choices */
	else
	{
		/* Hack -- Start on equipment if requested */
		if (command_see && command_wrk && equip)
		{
			command_wrk = TRUE;
		}

		/* Use inventory if allowed */
		else if (inven)
		{
			command_wrk = FALSE;
		}

		/* Use equipment if allowed */
		else if (equip)
		{
			command_wrk = TRUE;
		}

		/* Use inventory for floor */
		else
		{
			command_wrk = FALSE;
		}
	}


	/*
	 * �ɲå��ץ����(always_show_list)�����ꤵ��Ƥ�����Ͼ�˰�����ɽ������
	 */
	if ((always_show_list == TRUE) || use_menu) command_see = TRUE;

	/* Hack -- start out in "display" mode */
	if (command_see)
	{
		/* Save screen */
		screen_save();
	}


	/* Repeat until done */
	while (!done)
	{
		int get_item_label = 0;

		/* Show choices */
		int ni = 0;
		int ne = 0;

		/* Scan windows */
		for (j = 0; j < 8; j++)
		{
			/* Unused */
			if (!angband_term[j]) continue;

			/* Count windows displaying inven */
			if (window_flag[j] & (PW_INVEN)) ni++;

			/* Count windows displaying equip */
			if (window_flag[j] & (PW_EQUIP)) ne++;
		}

		/* Toggle if needed */
		if ((command_wrk && ni && !ne) ||
		    (!command_wrk && !ni && ne))
		{
			/* Toggle */
			toggle_inven_equip();

			/* Track toggles */
			toggle = !toggle;
		}

		/* Update */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Redraw windows */
		window_stuff();


		/* Inventory screen */
		if (!command_wrk)
		{
			/* Redraw if needed */
			if (command_see) get_item_label = show_inven(menu_line);
		}

		/* Equipment screen */
		else
		{
			/* Redraw if needed */
			if (command_see) get_item_label = show_equip(menu_line);
		}

		/* Viewing inventory */
		if (!command_wrk)
		{
			/* Begin the prompt */
#ifdef JP
			sprintf(out_val, "����ʪ:");
#else
			sprintf(out_val, "Inven:");
#endif

			/* Some legal items */
			if ((i1 <= i2) && !use_menu)
			{
				/* Build the prompt */
#ifdef JP
				sprintf(tmp_val, "%c-%c,'(',')',",
#else
				sprintf(tmp_val, " %c-%c,'(',')',",
#endif
					index_to_label(i1), index_to_label(i2));

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
#ifdef JP
			if (!command_see && !use_menu) strcat(out_val, " '*'����,");
#else
			if (!command_see && !use_menu) strcat(out_val, " * to see,");
#endif

			/* Append */
#ifdef JP
			if (equip) strcat(out_val, format(" %s ������,", use_menu ? "'4'or'6'" : "'/'"));
#else
			if (equip) strcat(out_val, format(" %s for Equip,", use_menu ? "4 or 6" : "/"));
#endif
		}

		/* Viewing equipment */
		else
		{
			/* Begin the prompt */
#ifdef JP
			sprintf(out_val, "������:");
#else
			sprintf(out_val, "Equip:");
#endif

			/* Some legal items */
			if ((e1 <= e2) && !use_menu)
			{
				/* Build the prompt */
#ifdef JP
				sprintf(tmp_val, "%c-%c,'(',')',",
#else
				sprintf(tmp_val, " %c-%c,'(',')',",
#endif
					index_to_label(e1), index_to_label(e2));

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
#ifdef JP
			if (!command_see && !use_menu) strcat(out_val, " '*'����,");
#else
			if (!command_see) strcat(out_val, " * to see,");
#endif

			/* Append */
#ifdef JP
			if (inven) strcat(out_val, format(" %s ����ʪ,", use_menu ? "'4'or'6'" : "'/'"));
#else
			if (inven) strcat(out_val, format(" %s for Inven,", use_menu ? "4 or 6" : "'/'"));
#endif
		}

		/* Indicate legality of the "floor" item */
#ifdef JP
		if (allow_floor) strcat(out_val, " '-'����,");
		if (select_the_force) strcat(out_val, " 'w'������,");
#else
		if (allow_floor) strcat(out_val, " - for floor,");
		if (select_the_force) strcat(out_val, " w for the Force,");
#endif

		/* Finish the prompt */
		strcat(out_val, " ESC");

		/* Build the prompt */
		sprintf(tmp_val, "(%s) %s", out_val, pmt);

		/* Show the prompt */
		prt(tmp_val, 0, 0);

		/* Get a key */
		which = inkey();

		if (use_menu)
		{
		int max_line = (command_wrk ? max_equip : max_inven);
		switch (which)
		{
			case ESCAPE:
			case 'z':
			case 'Z':
			case '0':
			{
				done = TRUE;
				break;
			}

			case '8':
			case 'k':
			case 'K':
			{
				menu_line += (max_line - 1);
				break;
			}

			case '2':
			case 'j':
			case 'J':
			{
				menu_line++;
				break;
			}

			case '4':
			case '6':
			case 'h':
			case 'H':
			case 'l':
			case 'L':
			{
				/* Verify legality */
				if (!inven || !equip)
				{
					bell();
					break;
				}

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				/* Switch inven/equip */
				command_wrk = !command_wrk;
				max_line = (command_wrk ? max_equip : max_inven);
				if (menu_line > max_line) menu_line = max_line;

				/* Need to redraw */
				break;
			}

			case 'x':
			case 'X':
			case '\r':
			case '\n':
			{
				if (command_wrk == USE_FLOOR)
				{
					/* Special index */
					(*cp) = -get_item_label;
				}
				else
				{
					/* Validate the item */
					if (!get_item_okay(get_item_label))
					{
						bell();
						break;
					}

					/* Allow player to "refuse" certain actions */
					if (!get_item_allow(get_item_label))
					{
						done = TRUE;
						break;
					}

					/* Accept that choice */
					(*cp) = get_item_label;
				}

				item = TRUE;
				done = TRUE;
				break;
			}
			case 'w':
			{
				if (select_the_force) {
					*cp = INVEN_FORCE;
					item = TRUE;
					done = TRUE;
					break;
				}
			}
		}
		if (menu_line > max_line) menu_line -= max_line;
		}
		else
		{
		/* Parse it */
		switch (which)
		{
			case ESCAPE:
			{
				done = TRUE;
				break;
			}

			case '*':
			case '?':
			case ' ':
			{
				/* Hide the list */
				if (command_see)
				{
					/* Flip flag */
					command_see = FALSE;

					/* Load screen */
					screen_load();
				}

				/* Show the list */
				else
				{
					/* Save screen */
					screen_save();

					/* Flip flag */
					command_see = TRUE;
				}
				break;
			}

			case '/':
			{
				/* Verify legality */
				if (!inven || !equip)
				{
					bell();
					break;
				}

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				/* Switch inven/equip */
				command_wrk = !command_wrk;

				/* Need to redraw */
				break;
			}

			case '-':
			{
				/* Use floor item */
				if (allow_floor)
				{
					/* Scan all objects in the grid */
					for (this_o_idx = cave[py][px].o_idx; this_o_idx; this_o_idx = next_o_idx)
					{
						object_type *o_ptr;

						/* Acquire object */
						o_ptr = &o_list[this_o_idx];

						/* Acquire next object */
						next_o_idx = o_ptr->next_o_idx;

						/* Validate the item */
						if (!item_tester_okay(o_ptr)) continue;

						/* Special index */
						k = 0 - this_o_idx;

						/* Verify the item (if required) */
#ifdef JP
if (other_query_flag && !verify("������", k)) continue;
#else
						if (other_query_flag && !verify("Try", k)) continue;
#endif


						/* Allow player to "refuse" certain actions */
						if (!get_item_allow(k)) continue;

						/* Accept that choice */
						(*cp) = k;
						item = TRUE;
						done = TRUE;
						break;
					}

					/* Outer break */
					if (done) break;
				}

				/* Oops */
				bell();
				break;
			}

			case '0':
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			{
				/* Look up the tag */
				if (!get_tag(&k, which, command_wrk ? USE_EQUIP : USE_INVEN))
				{
					bell();
					break;
				}

				/* Hack -- Validate the item */
				if ((k < INVEN_RARM) ? !inven : !equip)
				{
					bell();
					break;
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
#ifdef ALLOW_REPEAT
				cur_tag = which;
#endif /* ALLOW_REPEAT */
				break;
			}

#if 0
			case '\n':
			case '\r':
			{
				/* Choose "default" inventory item */
				if (!command_wrk)
				{
					k = ((i1 == i2) ? i1 : -1);
				}

				/* Choose "default" equipment item */
				else
				{
					k = ((e1 == e2) ? e1 : -1);
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}
#endif

			case 'w':
			{
				if (select_the_force) {
					*cp = INVEN_FORCE;
					item = TRUE;
					done = TRUE;
					break;
				}

				/* Fall through */
			}

			default:
			{
				int ver;
				bool not_found = FALSE;

				/* Look up the alphabetical tag */
				if (!get_tag(&k, which, command_wrk ? USE_EQUIP : USE_INVEN))
				{
					not_found = TRUE;
				}

				/* Hack -- Validate the item */
				else if ((k < INVEN_RARM) ? !inven : !equip)
				{
					not_found = TRUE;
				}

				/* Validate the item */
				else if (!get_item_okay(k))
				{
					not_found = TRUE;
				}

				if (!not_found)
				{
					/* Accept that choice */
					(*cp) = k;
					item = TRUE;
					done = TRUE;
#ifdef ALLOW_REPEAT
					cur_tag = which;
#endif /* ALLOW_REPEAT */
					break;
				}

				/* Extract "query" setting */
				ver = isupper(which);
				which = tolower(which);

				/* Convert letter to inventory index */
				if (!command_wrk)
				{
					if (which == '(') k = i1;
					else if (which == ')') k = i2;
					else k = label_to_inven(which);
				}

				/* Convert letter to equipment index */
				else
				{
					if (which == '(') k = e1;
					else if (which == ')') k = e2;
					else k = label_to_equip(which);
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Verify the item */
#ifdef JP
if (ver && !verify("������", k))
#else
				if (ver && !verify("Try", k))
#endif

				{
					done = TRUE;
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}
		}
		}
	}


	/* Fix the screen if necessary */
	if (command_see)
	{
		/* Load screen */
		screen_load();

		/* Hack -- Cancel "display" */
		command_see = FALSE;
	}


	/* Forget the item_tester_tval restriction */
	item_tester_tval = 0;

	item_tester_no_ryoute = FALSE;

	/* Forget the item_tester_hook restriction */
	item_tester_hook = NULL;


	/* Clean up  'show choices' */
	/* Toggle again if needed */
	if (toggle) toggle_inven_equip();

	/* Update */
	p_ptr->window |= (PW_INVEN | PW_EQUIP);

	/* Window stuff */
	window_stuff();


	/* Clear the prompt line */
	prt("", 0, 0);

	/* Warning if needed */
	if (oops && str) msg_print(str);

	if (item)
	{
#ifdef ALLOW_REPEAT
		repeat_push(*cp);
		if (command_cmd) prev_tag = cur_tag;
#endif /* ALLOW_REPEAT */

		command_cmd = 0; /* Hack -- command_cmd is no longer effective */
	}

	/* Result */
	return (item);
}


#ifdef ALLOW_EASY_FLOOR

/*!
 * @brief ����������Ƥ��륪�֥������Ȥο����֤� / scan_floor
 * @param items ���֥������Ȥ�ID�ꥹ�Ȥ��֤���������󻲾ȥݥ���
 * @param y ��������ե���Y��ɸ
 * @param x ��������ե���X��ɸ
 * @param mode ���ץ����ե饰
 * @return �оݤΥޥ�������Ƥ��륢���ƥ��
 * @details
 * Return a list of o_list[] indexes of items at the given cave
 * location. Valid flags are:
 *
 *		mode & 0x01 -- Item tester
 *		mode & 0x02 -- Marked items only
 *		mode & 0x04 -- Stop after first
 */
int scan_floor(int *items, int y, int x, int mode)
{
	int this_o_idx, next_o_idx;

	int num = 0;

	/* Sanity */
	if (!in_bounds(y, x)) return 0;

	/* Scan all objects in the grid */
	for (this_o_idx = cave[y][x].o_idx; this_o_idx; this_o_idx = next_o_idx)
	{
		object_type *o_ptr;

		/* Acquire object */
		o_ptr = &o_list[this_o_idx];

		/* Acquire next object */
		next_o_idx = o_ptr->next_o_idx;

		/* Item tester */
		if ((mode & 0x01) && !item_tester_okay(o_ptr)) continue;

		/* Marked */
		if ((mode & 0x02) && !(o_ptr->marked & OM_FOUND)) continue;

		/* Accept this item */
		/* XXX Hack -- Enforce limit */
		if (num < 23)
			items[num] = this_o_idx;

		num++;

		/* Only one */
		if (mode & 0x04) break;
	}

	/* Result */
	return num;
}


/*!
 * @brief ����������Ƥ��륢���ƥ�ΰ������֤� / Display a list of the items on the floor at the given location.
 * @param items ���֥������Ȥ�ID�ꥹ�Ȥ��֤���������󻲾ȥݥ���
 * @param target_item ��������ν����
 * @param y ��������ե���Y��ɸ
 * @param x ��������ե���X��ɸ
 * @param min_width ɽ����Ĺ��
 * @return ���򤷤������ƥ��ID
 * @details
 */
int show_floor(int target_item, int y, int x, int *min_width)
{
	int i, j, k, l;
	int col, len;

	object_type *o_ptr;

	char o_name[MAX_NLEN];

	char tmp_val[80];

	int out_index[23];
	byte out_color[23];
	char out_desc[23][MAX_NLEN];
	int target_item_label = 0;

	int floor_list[23], floor_num;
	int wid, hgt;
	char floor_label[52 + 1];

	bool dont_need_to_show_weights = TRUE;

	/* Get size */
	Term_get_size(&wid, &hgt);

	/* Default length */
	len = MAX((*min_width), 20);


	/* Scan for objects in the grid, using item_tester_okay() */
	floor_num = scan_floor(floor_list, y, x, 0x03);

	/* Display the floor objects */
	for (k = 0, i = 0; i < floor_num && i < 23; i++)
	{
		o_ptr = &o_list[floor_list[i]];

		/* Describe the object */
		object_desc(o_name, o_ptr, 0);

		/* Save the index */
		out_index[k] = i;

		/* Acquire inventory color */
		out_color[k] = tval_to_attr[o_ptr->tval & 0x7F];

		/* Save the object description */
		strcpy(out_desc[k], o_name);

		/* Find the predicted "line length" */
		l = strlen(out_desc[k]) + 5;

		/* Be sure to account for the weight */
		if (show_weights) l += 9;

		if (o_ptr->tval != TV_GOLD) dont_need_to_show_weights = FALSE;

		/* Maintain the maximum length */
		if (l > len) len = l;

		/* Advance to next "line" */
		k++;
	}

	if (show_weights && dont_need_to_show_weights) len -= 9;

	/* Save width */
	*min_width = len;

	/* Find the column to start in */
	col = (len > wid - 4) ? 0 : (wid - len - 1);

	prepare_label_string_floor(floor_label, floor_list, floor_num);

	/* Output each entry */
	for (j = 0; j < k; j++)
	{
		/* Get the index */
		i = floor_list[out_index[j]];

		/* Get the item */
		o_ptr = &o_list[i];

		/* Clear the line */
		prt("", j + 1, col ? col - 2 : col);

		if (use_menu && target_item)
		{
			if (j == (target_item-1))
			{
#ifdef JP
				strcpy(tmp_val, "��");
#else
				strcpy(tmp_val, "> ");
#endif
				target_item_label = i;
			}
			else strcpy(tmp_val, "   ");
		}
		else
		{
			/* Prepare an index --(-- */
			sprintf(tmp_val, "%c)", floor_label[j]);
		}

		/* Clear the line with the (possibly indented) index */
		put_str(tmp_val, j + 1, col);

		/* Display the entry itself */
		c_put_str(out_color[j], out_desc[j], j + 1, col + 3);

		/* Display the weight if needed */
		if (show_weights && (o_ptr->tval != TV_GOLD))
		{
			int wgt = o_ptr->weight * o_ptr->number;
#ifdef JP
			sprintf(tmp_val, "%3d.%1d kg", lbtokg1(wgt) , lbtokg2(wgt) );
#else
			sprintf(tmp_val, "%3d.%1d lb", wgt / 10, wgt % 10);
#endif

			prt(tmp_val, j + 1, wid - 9);
		}
	}

	/* Make a "shadow" below the list (only if needed) */
	if (j && (j < 23)) prt("", j + 1, col ? col - 2 : col);

	return target_item_label;
}

/*!
 * @brief ���֥���������������Ѵؿ�(���奢���ƥ���) /
 * Let the user select an item, save its "index"
 * @param cp ���򤷤����֥������Ȥ�ID���֤���
 * @param pmt ������Ū�Υ�å�����
 * @param str ����Ǥ��륪�֥������Ȥ��ʤ����Υ���󥻥��å�����
 * @praram mode ���ץ����ե饰
 * @return �ץ쥤�䡼�ˤ�ꥢ���ƥब���򤵤줿�ʤ�TRUE���֤���/
 */
bool get_item_floor(int *cp, cptr pmt, cptr str, int mode)
{
	char n1 = ' ', n2 = ' ', which = ' ';

	int j, k, i1, i2, e1, e2;

	bool done, item;

	bool oops = FALSE;

	/* Extract args */
	bool equip = (mode & USE_EQUIP) ? TRUE : FALSE;
	bool inven = (mode & USE_INVEN) ? TRUE : FALSE;
	bool floor = (mode & USE_FLOOR) ? TRUE : FALSE;

	bool allow_equip = FALSE;
	bool allow_inven = FALSE;
	bool allow_floor = FALSE;

	bool toggle = FALSE;

	char tmp_val[160];
	char out_val[160];

	int floor_num, floor_list[23], floor_top = 0;
	int min_width = 0;

	extern bool select_the_force;

	int menu_line = (use_menu ? 1 : 0);
	int max_inven = 0;
	int max_equip = 0;

#ifdef ALLOW_REPEAT

	static char prev_tag = '\0';
	char cur_tag = '\0';

	/* Get the item index */
	if (repeat_pull(cp))
	{
		/* the_force */
		if (select_the_force && (*cp == INVEN_FORCE))
		{
			item_tester_tval = 0;
			item_tester_hook = NULL;
			command_cmd = 0; /* Hack -- command_cmd is no longer effective */
			return (TRUE);
		}

		/* Floor item? */
		else if (floor && (*cp < 0))
		{
			if (prev_tag && command_cmd)
			{
				/* Scan all objects in the grid */
				floor_num = scan_floor(floor_list, py, px, 0x03);

				/* Look up the tag */
				if (get_tag_floor(&k, prev_tag, floor_list, floor_num))
				{
					/* Accept that choice */
					(*cp) = 0 - floor_list[k];

					/* Forget restrictions */
					item_tester_tval = 0;
					item_tester_hook = NULL;
					command_cmd = 0; /* Hack -- command_cmd is no longer effective */

					/* Success */
					return TRUE;
				}

				prev_tag = '\0'; /* prev_tag is no longer effective */
			}

			/* Validate the item */
			else if (item_tester_okay(&o_list[0 - (*cp)]))
			{
				/* Forget restrictions */
				item_tester_tval = 0;
				item_tester_hook = NULL;
				command_cmd = 0; /* Hack -- command_cmd is no longer effective */

				/* Success */
				return TRUE;
			}
		}

		else if ((inven && (*cp >= 0) && (*cp < INVEN_PACK)) ||
		         (equip && (*cp >= INVEN_RARM) && (*cp < INVEN_TOTAL)))
		{
			if (prev_tag && command_cmd)
			{
				/* Look up the tag and validate the item */
				if (!get_tag(&k, prev_tag, (*cp >= INVEN_RARM) ? USE_EQUIP : USE_INVEN)) /* Reject */;
				else if ((k < INVEN_RARM) ? !inven : !equip) /* Reject */;
				else if (!get_item_okay(k)) /* Reject */;
				else
				{
					/* Accept that choice */
					(*cp) = k;

					/* Forget restrictions */
					item_tester_tval = 0;
					item_tester_hook = NULL;
					command_cmd = 0; /* Hack -- command_cmd is no longer effective */

					/* Success */
					return TRUE;
				}

				prev_tag = '\0'; /* prev_tag is no longer effective */
			}

			/* Verify the item */
			else if (get_item_okay(*cp))
			{
				/* Forget restrictions */
				item_tester_tval = 0;
				item_tester_hook = NULL;
				command_cmd = 0; /* Hack -- command_cmd is no longer effective */

				/* Success */
				return TRUE;
			}
		}
	}

#endif /* ALLOW_REPEAT */


	/* Paranoia XXX XXX XXX */
	msg_print(NULL);


	/* Not done */
	done = FALSE;

	/* No item selected */
	item = FALSE;


	/* Full inventory */
	i1 = 0;
	i2 = INVEN_PACK - 1;

	/* Forbid inventory */
	if (!inven) i2 = -1;
	else if (use_menu)
	{
		for (j = 0; j < INVEN_PACK; j++)
			if (item_tester_okay(&inventory[j])) max_inven++;
	}

	/* Restrict inventory indexes */
	while ((i1 <= i2) && (!get_item_okay(i1))) i1++;
	while ((i1 <= i2) && (!get_item_okay(i2))) i2--;


	/* Full equipment */
	e1 = INVEN_RARM;
	e2 = INVEN_TOTAL - 1;

	/* Forbid equipment */
	if (!equip) e2 = -1;
	else if (use_menu)
	{
		for (j = INVEN_RARM; j < INVEN_TOTAL; j++)
			if (select_ring_slot ? is_ring_slot(j) : item_tester_okay(&inventory[j])) max_equip++;
		if (p_ptr->ryoute && !item_tester_no_ryoute) max_equip++;
	}

	/* Restrict equipment indexes */
	while ((e1 <= e2) && (!get_item_okay(e1))) e1++;
	while ((e1 <= e2) && (!get_item_okay(e2))) e2--;

	if (equip && p_ptr->ryoute && !item_tester_no_ryoute)
	{
		if (p_ptr->migite)
		{
			if (e2 < INVEN_LARM) e2 = INVEN_LARM;
		}
		else if (p_ptr->hidarite) e1 = INVEN_RARM;
	}


	/* Count "okay" floor items */
	floor_num = 0;

	/* Restrict floor usage */
	if (floor)
	{
		/* Scan all objects in the grid */
		floor_num = scan_floor(floor_list, py, px, 0x03);
	}

	/* Accept inventory */
	if (i1 <= i2) allow_inven = TRUE;

	/* Accept equipment */
	if (e1 <= e2) allow_equip = TRUE;

	/* Accept floor */
	if (floor_num) allow_floor = TRUE;

	/* Require at least one legal choice */
	if (!allow_inven && !allow_equip && !allow_floor)
	{
		/* Cancel p_ptr->command_see */
		command_see = FALSE;

		/* Oops */
		oops = TRUE;

		/* Done */
		done = TRUE;

		if (select_the_force) {
		    *cp = INVEN_FORCE;
		    item = TRUE;
		}
	}

	/* Analyze choices */
	else
	{
		/* Hack -- Start on equipment if requested */
		if (command_see && (command_wrk == (USE_EQUIP))
			&& allow_equip)
		{
			command_wrk = (USE_EQUIP);
		}

		/* Use inventory if allowed */
		else if (allow_inven)
		{
			command_wrk = (USE_INVEN);
		}

		/* Use equipment if allowed */
		else if (allow_equip)
		{
			command_wrk = (USE_EQUIP);
		}

		/* Use floor if allowed */
		else if (allow_floor)
		{
			command_wrk = (USE_FLOOR);
		}
	}

	/*
	 * �ɲå��ץ����(always_show_list)�����ꤵ��Ƥ�����Ͼ�˰�����ɽ������
	 */
	if ((always_show_list == TRUE) || use_menu) command_see = TRUE;

	/* Hack -- start out in "display" mode */
	if (command_see)
	{
		/* Save screen */
		screen_save();
	}

	/* Repeat until done */
	while (!done)
	{
		int get_item_label = 0;

		/* Show choices */
		int ni = 0;
		int ne = 0;

		/* Scan windows */
		for (j = 0; j < 8; j++)
		{
			/* Unused */
			if (!angband_term[j]) continue;

			/* Count windows displaying inven */
			if (window_flag[j] & (PW_INVEN)) ni++;

			/* Count windows displaying equip */
			if (window_flag[j] & (PW_EQUIP)) ne++;
		}

		/* Toggle if needed */
		if ((command_wrk == (USE_EQUIP) && ni && !ne) ||
		    (command_wrk == (USE_INVEN) && !ni && ne))
		{
			/* Toggle */
			toggle_inven_equip();

			/* Track toggles */
			toggle = !toggle;
		}

		/* Update */
		p_ptr->window |= (PW_INVEN | PW_EQUIP);

		/* Redraw windows */
		window_stuff();

		/* Inventory screen */
		if (command_wrk == (USE_INVEN))
		{
			/* Extract the legal requests */
			n1 = I2A(i1);
			n2 = I2A(i2);

			/* Redraw if needed */
			if (command_see) get_item_label = show_inven(menu_line);
		}

		/* Equipment screen */
		else if (command_wrk == (USE_EQUIP))
		{
			/* Extract the legal requests */
			n1 = I2A(e1 - INVEN_RARM);
			n2 = I2A(e2 - INVEN_RARM);

			/* Redraw if needed */
			if (command_see) get_item_label = show_equip(menu_line);
		}

		/* Floor screen */
		else if (command_wrk == (USE_FLOOR))
		{
			j = floor_top;
			k = MIN(floor_top + 23, floor_num) - 1;

			/* Extract the legal requests */
			n1 = I2A(j - floor_top);
			n2 = I2A(k - floor_top);

			/* Redraw if needed */
			if (command_see) get_item_label = show_floor(menu_line, py, px, &min_width);
		}

		/* Viewing inventory */
		if (command_wrk == (USE_INVEN))
		{
			/* Begin the prompt */
#ifdef JP
			sprintf(out_val, "����ʪ:");
#else
			sprintf(out_val, "Inven:");
#endif

			if (!use_menu)
			{
				/* Build the prompt */
#ifdef JP
				sprintf(tmp_val, "%c-%c,'(',')',",
#else
				sprintf(tmp_val, " %c-%c,'(',')',",
#endif
					index_to_label(i1), index_to_label(i2));

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
#ifdef JP
			if (!command_see && !use_menu) strcat(out_val, " '*'����,");
#else
			if (!command_see && !use_menu) strcat(out_val, " * to see,");
#endif

			/* Append */
			if (allow_equip)
			{
#ifdef JP
				if (!use_menu)
					strcat(out_val, " '/' ������,");
				else if (allow_floor)
					strcat(out_val, " '6' ������,");
				else
					strcat(out_val, " '4'or'6' ������,");
#else
				if (!use_menu)
					strcat(out_val, " / for Equip,");
				else if (allow_floor)
					strcat(out_val, " 6 for Equip,");
				else
					strcat(out_val, " 4 or 6 for Equip,");
#endif
			}

			/* Append */
			if (allow_floor)
			{
#ifdef JP
				if (!use_menu)
					strcat(out_val, " '-'����,");
				else if (allow_equip)
					strcat(out_val, " '4' ����,");
				else
					strcat(out_val, " '4'or'6' ����,");
#else
				if (!use_menu)
					strcat(out_val, " - for floor,");
				else if (allow_equip)
					strcat(out_val, " 4 for floor,");
				else
					strcat(out_val, " 4 or 6 for floor,");
#endif
			}
		}

		/* Viewing equipment */
		else if (command_wrk == (USE_EQUIP))
		{
			/* Begin the prompt */
#ifdef JP
			sprintf(out_val, "������:");
#else
			sprintf(out_val, "Equip:");
#endif

			if (!use_menu)
			{
				/* Build the prompt */
#ifdef JP
				sprintf(tmp_val, "%c-%c,'(',')',",
#else
				sprintf(tmp_val, " %c-%c,'(',')',",
#endif
					index_to_label(e1), index_to_label(e2));

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
#ifdef JP
			if (!command_see && !use_menu) strcat(out_val, " '*'����,");
#else
			if (!command_see && !use_menu) strcat(out_val, " * to see,");
#endif

			/* Append */
			if (allow_inven)
			{
#ifdef JP
				if (!use_menu)
					strcat(out_val, " '/' ����ʪ,");
				else if (allow_floor)
					strcat(out_val, " '4' ����ʪ,");
				else
					strcat(out_val, " '4'or'6' ����ʪ,");
#else
				if (!use_menu)
					strcat(out_val, " / for Inven,");
				else if (allow_floor)
					strcat(out_val, " 4 for Inven,");
				else
					strcat(out_val, " 4 or 6 for Inven,");
#endif
			}

			/* Append */
			if (allow_floor)
			{
#ifdef JP
				if (!use_menu)
					strcat(out_val, " '-'����,");
				else if (allow_inven)
					strcat(out_val, " '6' ����,");
				else
					strcat(out_val, " '4'or'6' ����,");
#else
				if (!use_menu)
					strcat(out_val, " - for floor,");
				else if (allow_inven)
					strcat(out_val, " 6 for floor,");
				else
					strcat(out_val, " 4 or 6 for floor,");
#endif
			}
		}

		/* Viewing floor */
		else if (command_wrk == (USE_FLOOR))
		{
			/* Begin the prompt */
#ifdef JP
			sprintf(out_val, "����:");
#else
			sprintf(out_val, "Floor:");
#endif

			if (!use_menu)
			{
				/* Build the prompt */
#ifdef JP
				sprintf(tmp_val, "%c-%c,'(',')',", n1, n2);
#else
				sprintf(tmp_val, " %c-%c,'(',')',", n1, n2);
#endif

				/* Append */
				strcat(out_val, tmp_val);
			}

			/* Indicate ability to "view" */
#ifdef JP
			if (!command_see && !use_menu) strcat(out_val, " '*'����,");
#else
			if (!command_see && !use_menu) strcat(out_val, " * to see,");
#endif

			if (use_menu)
			{
				if (allow_inven && allow_equip)
				{
#ifdef JP
					strcat(out_val, " '4' ������, '6' ����ʪ,");
#else
					strcat(out_val, " 4 for Equip, 6 for Inven,");
#endif
				}
				else if (allow_inven)
				{
#ifdef JP
					strcat(out_val, " '4'or'6' ����ʪ,");
#else
					strcat(out_val, " 4 or 6 for Inven,");
#endif
				}
				else if (allow_equip)
				{
#ifdef JP
					strcat(out_val, " '4'or'6' ������,");
#else
					strcat(out_val, " 4 or 6 for Equip,");
#endif
				}
			}
			/* Append */
			else if (allow_inven)
			{
#ifdef JP
				strcat(out_val, " '/' ����ʪ,");
#else
				strcat(out_val, " / for Inven,");
#endif
			}
			else if (allow_equip)
			{
#ifdef JP
				strcat(out_val, " '/'������,");
#else
				strcat(out_val, " / for Equip,");
#endif
			}

			/* Append */
			if (command_see && !use_menu)
			{
#ifdef JP
				strcat(out_val, " Enter ��,");
#else
				strcat(out_val, " Enter for scroll down,");
#endif
			}
		}

		/* Append */
#ifdef JP
		if (select_the_force) strcat(out_val, " 'w'������,");
#else
		if (select_the_force) strcat(out_val, " w for the Force,");
#endif

		/* Finish the prompt */
		strcat(out_val, " ESC");

		/* Build the prompt */
		sprintf(tmp_val, "(%s) %s", out_val, pmt);

		/* Show the prompt */
		prt(tmp_val, 0, 0);

		/* Get a key */
		which = inkey();

		if (use_menu)
		{
		int max_line = 1;
		if (command_wrk == USE_INVEN) max_line = max_inven;
		else if (command_wrk == USE_EQUIP) max_line = max_equip;
		else if (command_wrk == USE_FLOOR) max_line = MIN(23, floor_num);
		switch (which)
		{
			case ESCAPE:
			case 'z':
			case 'Z':
			case '0':
			{
				done = TRUE;
				break;
			}

			case '8':
			case 'k':
			case 'K':
			{
				menu_line += (max_line - 1);
				break;
			}

			case '2':
			case 'j':
			case 'J':
			{
				menu_line++;
				break;
			}

			case '4':
			case 'h':
			case 'H':
			{
				/* Verify legality */
				if (command_wrk == (USE_INVEN))
				{
					if (allow_floor) command_wrk = USE_FLOOR;
					else if (allow_equip) command_wrk = USE_EQUIP;
					else
					{
						bell();
						break;
					}
				}
				else if (command_wrk == (USE_EQUIP))
				{
					if (allow_inven) command_wrk = USE_INVEN;
					else if (allow_floor) command_wrk = USE_FLOOR;
					else
					{
						bell();
						break;
					}
				}
				else if (command_wrk == (USE_FLOOR))
				{
					if (allow_equip) command_wrk = USE_EQUIP;
					else if (allow_inven) command_wrk = USE_INVEN;
					else
					{
						bell();
						break;
					}
				}
				else
				{
					bell();
					break;
				}

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				/* Switch inven/equip */
				if (command_wrk == USE_INVEN) max_line = max_inven;
				else if (command_wrk == USE_EQUIP) max_line = max_equip;
				else if (command_wrk == USE_FLOOR) max_line = MIN(23, floor_num);
				if (menu_line > max_line) menu_line = max_line;

				/* Need to redraw */
				break;
			}

			case '6':
			case 'l':
			case 'L':
			{
				/* Verify legality */
				if (command_wrk == (USE_INVEN))
				{
					if (allow_equip) command_wrk = USE_EQUIP;
					else if (allow_floor) command_wrk = USE_FLOOR;
					else
					{
						bell();
						break;
					}
				}
				else if (command_wrk == (USE_EQUIP))
				{
					if (allow_floor) command_wrk = USE_FLOOR;
					else if (allow_inven) command_wrk = USE_INVEN;
					else
					{
						bell();
						break;
					}
				}
				else if (command_wrk == (USE_FLOOR))
				{
					if (allow_inven) command_wrk = USE_INVEN;
					else if (allow_equip) command_wrk = USE_EQUIP;
					else
					{
						bell();
						break;
					}
				}
				else
				{
					bell();
					break;
				}

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				/* Switch inven/equip */
				if (command_wrk == USE_INVEN) max_line = max_inven;
				else if (command_wrk == USE_EQUIP) max_line = max_equip;
				else if (command_wrk == USE_FLOOR) max_line = MIN(23, floor_num);
				if (menu_line > max_line) menu_line = max_line;

				/* Need to redraw */
				break;
			}

			case 'x':
			case 'X':
			case '\r':
			case '\n':
			{
				if (command_wrk == USE_FLOOR)
				{
					/* Special index */
					(*cp) = -get_item_label;
				}
				else
				{
					/* Validate the item */
					if (!get_item_okay(get_item_label))
					{
						bell();
						break;
					}

					/* Allow player to "refuse" certain actions */
					if (!get_item_allow(get_item_label))
					{
						done = TRUE;
						break;
					}

					/* Accept that choice */
					(*cp) = get_item_label;
				}

				item = TRUE;
				done = TRUE;
				break;
			}
			case 'w':
			{
				if (select_the_force) {
					*cp = INVEN_FORCE;
					item = TRUE;
					done = TRUE;
					break;
				}
			}
		}
		if (menu_line > max_line) menu_line -= max_line;
		}
		else
		{
		/* Parse it */
		switch (which)
		{
			case ESCAPE:
			{
				done = TRUE;
				break;
			}

			case '*':
			case '?':
			case ' ':
			{
				/* Hide the list */
				if (command_see)
				{
					/* Flip flag */
					command_see = FALSE;

					/* Load screen */
					screen_load();
				}

				/* Show the list */
				else
				{
					/* Save screen */
					screen_save();

					/* Flip flag */
					command_see = TRUE;
				}
				break;
			}

			case '\n':
			case '\r':
			case '+':
			{
				int i, o_idx;
				cave_type *c_ptr = &cave[py][px];

				if (command_wrk != (USE_FLOOR)) break;

				/* Get the object being moved. */
				o_idx = c_ptr->o_idx;

				/* Only rotate a pile of two or more objects. */
				if (!(o_idx && o_list[o_idx].next_o_idx)) break;

				/* Remove the first object from the list. */
				excise_object_idx(o_idx);

				/* Find end of the list. */
				i = c_ptr->o_idx;
				while (o_list[i].next_o_idx)
					i = o_list[i].next_o_idx;

				/* Add after the last object. */
				o_list[i].next_o_idx = o_idx;

				/* Re-scan floor list */ 
				floor_num = scan_floor(floor_list, py, px, 0x03);

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				break;
			}

			case '/':
			{
				if (command_wrk == (USE_INVEN))
				{
					if (!allow_equip)
					{
						bell();
						break;
					}
					command_wrk = (USE_EQUIP);
				}
				else if (command_wrk == (USE_EQUIP))
				{
					if (!allow_inven)
					{
						bell();
						break;
					}
					command_wrk = (USE_INVEN);
				}
				else if (command_wrk == (USE_FLOOR))
				{
					if (allow_inven)
					{
						command_wrk = (USE_INVEN);
					}
					else if (allow_equip)
					{
						command_wrk = (USE_EQUIP);
					}
					else
					{
						bell();
						break;
					}
				}

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				/* Need to redraw */
				break;
			}

			case '-':
			{
				if (!allow_floor)
				{
					bell();
					break;
				}

				/*
				 * If we are already examining the floor, and there
				 * is only one item, we will always select it.
				 * If we aren't examining the floor and there is only
				 * one item, we will select it if floor_query_flag
				 * is FALSE.
				 */
				if (floor_num == 1)
				{
					if ((command_wrk == (USE_FLOOR)) || (!carry_query_flag))
					{
						/* Special index */
						k = 0 - floor_list[0];

						/* Allow player to "refuse" certain actions */
						if (!get_item_allow(k))
						{
							done = TRUE;
							break;
						}

						/* Accept that choice */
						(*cp) = k;
						item = TRUE;
						done = TRUE;

						break;
					}
				}

				/* Hack -- Fix screen */
				if (command_see)
				{
					/* Load screen */
					screen_load();

					/* Save screen */
					screen_save();
				}

				command_wrk = (USE_FLOOR);

				break;
			}

			case '0':
			case '1': case '2': case '3':
			case '4': case '5': case '6':
			case '7': case '8': case '9':
			{
				if (command_wrk != USE_FLOOR)
				{
					/* Look up the tag */
					if (!get_tag(&k, which, command_wrk))
					{
						bell();
						break;
					}

					/* Hack -- Validate the item */
					if ((k < INVEN_RARM) ? !inven : !equip)
					{
						bell();
						break;
					}

					/* Validate the item */
					if (!get_item_okay(k))
					{
						bell();
						break;
					}
				}
				else
				{
					/* Look up the alphabetical tag */
					if (get_tag_floor(&k, which, floor_list, floor_num))
					{
						/* Special index */
						k = 0 - floor_list[k];
					}
					else
					{
						bell();
						break;
					}
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
#ifdef ALLOW_REPEAT
				cur_tag = which;
#endif /* ALLOW_REPEAT */
				break;
			}

#if 0
			case '\n':
			case '\r':
			{
				/* Choose "default" inventory item */
				if (command_wrk == (USE_INVEN))
				{
					k = ((i1 == i2) ? i1 : -1);
				}

				/* Choose "default" equipment item */
				else if (command_wrk == (USE_EQUIP))
				{
					k = ((e1 == e2) ? e1 : -1);
				}

				/* Choose "default" floor item */
				else if (command_wrk == (USE_FLOOR))
				{
					if (floor_num == 1)
					{
						/* Special index */
						k = 0 - floor_list[0];

						/* Allow player to "refuse" certain actions */
						if (!get_item_allow(k))
						{
							done = TRUE;
							break;
						}

						/* Accept that choice */
						(*cp) = k;
						item = TRUE;
						done = TRUE;
					}
					break;
				}

				/* Validate the item */
				if (!get_item_okay(k))
				{
					bell();
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}
#endif

			case 'w':
			{
				if (select_the_force) {
					*cp = INVEN_FORCE;
					item = TRUE;
					done = TRUE;
					break;
				}

				/* Fall through */
			}

			default:
			{
				int ver;

				if (command_wrk != USE_FLOOR)
				{
					bool not_found = FALSE;

					/* Look up the alphabetical tag */
					if (!get_tag(&k, which, command_wrk))
					{
						not_found = TRUE;
					}

					/* Hack -- Validate the item */
					else if ((k < INVEN_RARM) ? !inven : !equip)
					{
						not_found = TRUE;
					}

					/* Validate the item */
					else if (!get_item_okay(k))
					{
						not_found = TRUE;
					}

					if (!not_found)
					{
						/* Accept that choice */
						(*cp) = k;
						item = TRUE;
						done = TRUE;
#ifdef ALLOW_REPEAT
						cur_tag = which;
#endif /* ALLOW_REPEAT */
						break;
					}
				}
				else
				{
					/* Look up the alphabetical tag */
					if (get_tag_floor(&k, which, floor_list, floor_num))
					{
						/* Special index */
						k = 0 - floor_list[k];

						/* Accept that choice */
						(*cp) = k;
						item = TRUE;
						done = TRUE;
#ifdef ALLOW_REPEAT
						cur_tag = which;
#endif /* ALLOW_REPEAT */
						break;
					}
				}

				/* Extract "query" setting */
				ver = isupper(which);
				which = tolower(which);

				/* Convert letter to inventory index */
				if (command_wrk == (USE_INVEN))
				{
					if (which == '(') k = i1;
					else if (which == ')') k = i2;
					else k = label_to_inven(which);
				}

				/* Convert letter to equipment index */
				else if (command_wrk == (USE_EQUIP))
				{
					if (which == '(') k = e1;
					else if (which == ')') k = e2;
					else k = label_to_equip(which);
				}

				/* Convert letter to floor index */
				else if (command_wrk == USE_FLOOR)
				{
					if (which == '(') k = 0;
					else if (which == ')') k = floor_num - 1;
					else k = islower(which) ? A2I(which) : -1;
					if (k < 0 || k >= floor_num || k >= 23)
					{
						bell();
						break;
					}

					/* Special index */
					k = 0 - floor_list[k];
				}

				/* Validate the item */
				if ((command_wrk != USE_FLOOR) && !get_item_okay(k))
				{
					bell();
					break;
				}

				/* Verify the item */
#ifdef JP
if (ver && !verify("������", k))
#else
				if (ver && !verify("Try", k))
#endif

				{
					done = TRUE;
					break;
				}

				/* Allow player to "refuse" certain actions */
				if (!get_item_allow(k))
				{
					done = TRUE;
					break;
				}

				/* Accept that choice */
				(*cp) = k;
				item = TRUE;
				done = TRUE;
				break;
			}
		}
		}
	}

	/* Fix the screen if necessary */
	if (command_see)
	{
		/* Load screen */
		screen_load();

		/* Hack -- Cancel "display" */
		command_see = FALSE;
	}


	/* Forget the item_tester_tval restriction */
	item_tester_tval = 0;

	/* Forget the item_tester_hook restriction */
	item_tester_hook = NULL;


	/* Clean up  'show choices' */
	/* Toggle again if needed */
	if (toggle) toggle_inven_equip();

	/* Update */
	p_ptr->window |= (PW_INVEN | PW_EQUIP);

	/* Window stuff */
	window_stuff();


	/* Clear the prompt line */
	prt("", 0, 0);

	/* Warning if needed */
	if (oops && str) msg_print(str);

	if (item)
	{
#ifdef ALLOW_REPEAT
		repeat_push(*cp);
		if (command_cmd) prev_tag = cur_tag;
#endif /* ALLOW_REPEAT */

		command_cmd = 0; /* Hack -- command_cmd is no longer effective */
	}

	/* Result */
	return (item);
}

/*!
 * @brief ����Υ����ƥ�򽦤������ѥ��֥롼���� 
 * @return �ץ쥤�䡼�ˤ�ꥢ���ƥब���򤵤줿�ʤ�TRUE���֤���
 */
static bool py_pickup_floor_aux(void)
{
	s16b this_o_idx;

	cptr q, s;

	int item;

	/* Restrict the choices */
	item_tester_hook = inven_carry_okay;

	/* Get an object */
#ifdef JP
	q = "�ɤ�򽦤��ޤ�����";
	s = "�⤦���å��ˤϾ��ˤ���ɤΥ����ƥ������ʤ���";
#else
	q = "Get which item? ";
	s = "You no longer have any room for the objects on the floor.";
#endif

	if (get_item(&item, q, s, (USE_FLOOR)))
	{
		this_o_idx = 0 - item;
	}
	else
	{
		return (FALSE);
	}

	/* Pick up the object */
	py_pickup_aux(this_o_idx);

	return (TRUE);
}

/*!
 * @brief ����Υ����ƥ�򽦤��ᥤ�����
 * @param pick FALSE�ʤ�����μ�ư�����Τߤ�Ԥ�/ FALSE then only gold will be picked up
 * @return �ʤ�
 * @details
 * This is called by py_pickup() when easy_floor is TRUE.
 */
void py_pickup_floor(bool pickup)
{
	s16b this_o_idx, next_o_idx = 0;

	char o_name[MAX_NLEN];
	object_type *o_ptr;

	int floor_num = 0, floor_o_idx = 0;

	int can_pickup = 0;

	/* Scan the pile of objects */
	for (this_o_idx = cave[py][px].o_idx; this_o_idx; this_o_idx = next_o_idx)
	{
		object_type *o_ptr;

		/* Access the object */
		o_ptr = &o_list[this_o_idx];

		/* Describe the object */
		object_desc(o_name, o_ptr, 0);

		/* Access the next object */
		next_o_idx = o_ptr->next_o_idx;

		/* Hack -- disturb */
		disturb(0, 0);

		/* Pick up gold */
		if (o_ptr->tval == TV_GOLD)
		{
			/* Message */
#ifdef JP
		msg_format(" $%ld �β��ͤ�����%s�򸫤Ĥ�����",
			   (long)o_ptr->pval, o_name);
#else
			msg_format("You have found %ld gold pieces worth of %s.",
				(long) o_ptr->pval, o_name);
#endif


			/* Collect the gold */
			p_ptr->au += o_ptr->pval;

			/* Redraw gold */
			p_ptr->redraw |= (PR_GOLD);

			/* Window stuff */
			p_ptr->window |= (PW_PLAYER);

			/* Delete the gold */
			delete_object_idx(this_o_idx);

			/* Check the next object */
			continue;
		}
		else if (o_ptr->marked & OM_NOMSG)
		{
			/* If 0 or 1 non-NOMSG items are in the pile, the NOMSG ones are
			 * ignored. Otherwise, they are included in the prompt. */
			o_ptr->marked &= ~(OM_NOMSG);
			continue;
		}

		/* Count non-gold objects that can be picked up. */
		if (inven_carry_okay(o_ptr))
		{
			can_pickup++;
		}

		/* Count non-gold objects */
		floor_num++;

		/* Remember this index */
		floor_o_idx = this_o_idx;
	}

	/* There are no non-gold objects */
	if (!floor_num)
		return;

	/* Mention the number of objects */
	if (!pickup)
	{
		/* One object */
		if (floor_num == 1)
		{
			/* Access the object */
			o_ptr = &o_list[floor_o_idx];

#ifdef ALLOW_EASY_SENSE

			/* Option: Make object sensing easy */
			if (easy_sense)
			{
				/* Sense the object */
				(void) sense_object(o_ptr);
			}

#endif /* ALLOW_EASY_SENSE */

			/* Describe the object */
			object_desc(o_name, o_ptr, 0);

			/* Message */
#ifdef JP
				msg_format("%s�����롣", o_name);
#else
			msg_format("You see %s.", o_name);
#endif

		}

		/* Multiple objects */
		else
		{
			/* Message */
#ifdef JP
			msg_format("%d �ĤΥ����ƥ�λ������롣", floor_num);
#else
			msg_format("You see a pile of %d items.", floor_num);
#endif

		}

		/* Done */
		return;
	}

	/* The player has no room for anything on the floor. */
	if (!can_pickup)
	{
		/* One object */
		if (floor_num == 1)
		{
			/* Access the object */
			o_ptr = &o_list[floor_o_idx];

#ifdef ALLOW_EASY_SENSE

			/* Option: Make object sensing easy */
			if (easy_sense)
			{
				/* Sense the object */
				(void) sense_object(o_ptr);
			}

#endif /* ALLOW_EASY_SENSE */

			/* Describe the object */
			object_desc(o_name, o_ptr, 0);

			/* Message */
#ifdef JP
				msg_format("���å��ˤ�%s��������֤��ʤ���", o_name);
#else
			msg_format("You have no room for %s.", o_name);
#endif

		}

		/* Multiple objects */
		else
		{
			/* Message */
#ifdef JP
			msg_format("���å��ˤϾ��ˤ���ɤΥ����ƥ������ʤ���", o_name);
#else
			msg_print("You have no room for any of the objects on the floor.");
#endif

		}

		/* Done */
		return;
	}

	/* One object */
	if (floor_num == 1)
	{
		/* Hack -- query every object */
		if (carry_query_flag)
		{
			char out_val[MAX_NLEN+20];

			/* Access the object */
			o_ptr = &o_list[floor_o_idx];

#ifdef ALLOW_EASY_SENSE

			/* Option: Make object sensing easy */
			if (easy_sense)
			{
				/* Sense the object */
				(void) sense_object(o_ptr);
			}

#endif /* ALLOW_EASY_SENSE */

			/* Describe the object */
			object_desc(o_name, o_ptr, 0);

			/* Build a prompt */
#ifdef JP
			(void) sprintf(out_val, "%s�򽦤��ޤ���? ", o_name);
#else
			(void) sprintf(out_val, "Pick up %s? ", o_name);
#endif


			/* Ask the user to confirm */
			if (!get_check(out_val))
			{
				/* Done */
				return;
			}
		}

		/* Access the object */
		o_ptr = &o_list[floor_o_idx];

#ifdef ALLOW_EASY_SENSE

		/* Option: Make object sensing easy */
		if (easy_sense)
		{
			/* Sense the object */
			(void) sense_object(o_ptr);
		}

#endif /* ALLOW_EASY_SENSE */

		/* Pick up the object */
		py_pickup_aux(floor_o_idx);
	}

	/* Allow the user to choose an object */
	else
	{
		while (can_pickup--)
		{
			if (!py_pickup_floor_aux()) break;
		}
	}
}

#endif /* ALLOW_EASY_FLOOR */
