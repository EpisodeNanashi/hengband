#include "angband.h"

/*!
* @brief �v���C���[���烂���X�^�[�ւ̎ˌ��������� /
* Determine if the player "hits" a monster (normal combat).
* @param chance ��{�����l
* @param m_ptr �����X�^�[�̍\���̎Q�ƃ|�C���^
* @param vis �ڕW�����E�ɕ߂炦�Ă���Ȃ��TRUE���w��
* @param o_name ���b�Z�[�W�\�����̃����X�^�[��
* @return �����Ɣ��肳�ꂽ�ꍇTRUE��Ԃ�
* @note Always miss 5%, always hit 5%, otherwise random.
*/
bool test_hit_fire(int chance, monster_type *m_ptr, int vis, char* o_name)
{
	int k, ac;
	monster_race *r_ptr = &r_info[m_ptr->r_idx];

	/* Percentile dice */
	k = randint1(100);

	/* Snipers with high-concentration reduce instant miss percentage.*/
	k += p_ptr->concent;

	/* Hack -- Instant miss or hit */
	if (k <= 5) return (FALSE);
	if (k > 95) return (TRUE);

	if (p_ptr->pseikaku == SEIKAKU_NAMAKE)
		if (one_in_(20)) return (FALSE);

	/* Never hit */
	if (chance <= 0) return (FALSE);

	ac = r_ptr->ac;
	if (p_ptr->concent)
	{
		ac *= (8 - p_ptr->concent);
		ac /= 8;
	}

	if (m_ptr->r_idx == MON_GOEMON && !MON_CSLEEP(m_ptr)) ac *= 3;

	/* Invisible monsters are harder to hit */
	if (!vis) chance = (chance + 1) / 2;

	/* Power competes against armor */
	if (randint0(chance) < (ac * 3 / 4))
	{
		if (m_ptr->r_idx == MON_GOEMON && !MON_CSLEEP(m_ptr))
		{
			char m_name[80];

			/* Extract monster name */
			monster_desc(m_name, m_ptr, 0);
			msg_format(_("%s��%s���a��̂Ă��I", "%s cuts down %s!"), m_name, o_name);
		}
		return (FALSE);
	}

	/* Assume hit */
	return (TRUE);
}




/*!
* @brief �v���C���[���烂���X�^�[�ւ̎ˌ��N���e�B�J������ /
* Critical hits (from objects thrown by player) Factor in item weight, total plusses, and player level.
* @param weight ��e�̏d��
* @param plus_ammo ��e�̖����C��
* @param plus_bow �|�̖����C��
* @param dam ���ݎZ�o���̃_���[�W�l
* @return �N���e�B�J���C�����������_���[�W�l
*/
HIT_POINT critical_shot(int weight, int plus_ammo, int plus_bow, HIT_POINT dam)
{
	int i, k;
	object_type *j_ptr = &inventory[INVEN_BOW];

	/* Extract "shot" power */
	i = p_ptr->to_h_b + plus_ammo;

	if (p_ptr->tval_ammo == TV_BOLT)
		i = (p_ptr->skill_thb + (p_ptr->weapon_exp[0][j_ptr->sval] / 400 + i) * BTH_PLUS_ADJ);
	else
		i = (p_ptr->skill_thb + ((p_ptr->weapon_exp[0][j_ptr->sval] - (WEAPON_EXP_MASTER / 2)) / 200 + i) * BTH_PLUS_ADJ);


	/* Snipers can shot more critically with crossbows */
	if (p_ptr->concent) i += ((i * p_ptr->concent) / 5);
	if ((p_ptr->pclass == CLASS_SNIPER) && (p_ptr->tval_ammo == TV_BOLT)) i *= 2;

	/* Good bow makes more critical */
	i += plus_bow * 8 * (p_ptr->concent ? p_ptr->concent + 5 : 5);

	/* Critical hit */
	if (randint1(10000) <= i)
	{
		k = weight * randint1(500);

		if (k < 900)
		{
			msg_print(_("�育�������������I", "It was a good hit!"));
			dam += (dam / 2);
		}
		else if (k < 1350)
		{
			msg_print(_("���Ȃ�̎育�������������I", "It was a great hit!"));
			dam *= 2;
		}
		else
		{
			msg_print(_("��S�̈ꌂ���I", "It was a superb hit!"));
			dam *= 3;
		}
	}

	return (dam);
}


