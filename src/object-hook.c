
#include "angband.h"
#include "object-hook.h"

/*!
* @brief �Ώۂ̃A�C�e�������N���X�{�E�̖�̍ޗ��ɂȂ邩��Ԃ��B/
* Hook to determine if an object is contertible in an arrow/bolt
* @param o_ptr �I�u�W�F�N�g�̍\���̂̎Q�ƃ|�C���^�B
* @return �ޗ��ɂł���Ȃ�TRUE��Ԃ�
*/
bool item_tester_hook_convertible(object_type *o_ptr)
{
	if ((o_ptr->tval == TV_JUNK) || (o_ptr->tval == TV_SKELETON)) return TRUE;

	if ((o_ptr->tval == TV_CORPSE) && (o_ptr->sval == SV_SKELETON)) return TRUE;
	/* Assume not */
	return (FALSE);
}

/*!
* @brief ���폠�́u����v�Ӓ�ΏۂɂȂ邩�𔻒肷��B/ Hook to specify "weapon"
* @param o_ptr �I�u�W�F�N�g�̍\���̂̎Q�ƃ|�C���^�B
* @return �ΏۂɂȂ�Ȃ�TRUE��Ԃ��B
*/
bool item_tester_hook_orthodox_melee_weapons(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
	case TV_HAFTED:
	case TV_POLEARM:
	case TV_DIGGING:
	{
		return (TRUE);
	}
	case TV_SWORD:
	{
		if (o_ptr->sval != SV_DOKUBARI) return (TRUE);
	}
	}

	return (FALSE);
}

/*!
* @brief �I�u�W�F�N�g���E�肩����ɑ����ł��镐�킩�ǂ����̔���
* @param o_ptr ���肷��I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return �E�肩����̕���Ƃ��đ����ł���Ȃ��TRUE��Ԃ��B
*/
bool item_tester_hook_melee_weapon(object_type *o_ptr)
{
	/* Check for a usable slot */
	if ((o_ptr->tval >= TV_DIGGING) && (o_ptr->tval <= TV_SWORD)) return (TRUE);

	/* Assume not wearable */
	return (FALSE);
}


/*!
* @brief ���폠�́u��e�v�Ӓ�ΏۂɂȂ邩�𔻒肷��B/ Hook to specify "weapon"
* @param o_ptr �I�u�W�F�N�g�̍\���̂̎Q�ƃ|�C���^�B
* @return �ΏۂɂȂ�Ȃ�TRUE��Ԃ��B
*/
bool item_tester_hook_ammo(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
	case TV_SHOT:
	case TV_ARROW:
	case TV_BOLT:
	{
		return (TRUE);
	}
	}

	return (FALSE);
}

/*!
* @brief �C���ΏۂƂȂ��ꂽ���킩�𔻒肷��B / Hook to specify "broken weapon"
* @param o_ptr �I�u�W�F�N�g�̍\���̂̎Q�ƃ|�C���^�B
* @return �C���ΏۂɂȂ�Ȃ�TRUE��Ԃ��B
*/
bool item_tester_hook_broken_weapon(object_type *o_ptr)
{
	if (o_ptr->tval != TV_SWORD) return FALSE;

	switch (o_ptr->sval)
	{
	case SV_BROKEN_DAGGER:
	case SV_BROKEN_SWORD:
		return TRUE;
	}

	return FALSE;
}

/*!
* @brief �I�u�W�F�N�g�����ˉ\�ȕ��킩�ǂ�����Ԃ��B
* @param o_ptr ���肷��I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return ���ˉ\�ȕ���Ȃ��TRUE
*/
bool item_tester_hook_boomerang(object_type *o_ptr)
{
	if ((o_ptr->tval == TV_DIGGING) || (o_ptr->tval == TV_SWORD) || (o_ptr->tval == TV_POLEARM) || (o_ptr->tval == TV_HAFTED)) return (TRUE);

	/* Assume not */
	return (FALSE);
}

/*!
* @brief �I�u�W�F�N�g���v���C���[���H�ׂ邱�Ƃ��ł��邩�𔻒肷�� /
* Hook to determine if an object is eatable
* @param o_ptr ���肵�����I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return �H�ׂ邱�Ƃ��\�Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_eatable(object_type *o_ptr)
{
	if (o_ptr->tval == TV_FOOD) return TRUE;

#if 0
	if (prace_is_(RACE_SKELETON))
	{
		if (o_ptr->tval == TV_SKELETON ||
			(o_ptr->tval == TV_CORPSE && o_ptr->sval == SV_SKELETON))
			return TRUE;
	}
	else
#endif

		if (prace_is_(RACE_SKELETON) ||
			prace_is_(RACE_GOLEM) ||
			prace_is_(RACE_ZOMBIE) ||
			prace_is_(RACE_SPECTRE))
		{
			if (o_ptr->tval == TV_STAFF || o_ptr->tval == TV_WAND)
				return TRUE;
		}
		else if (prace_is_(RACE_DEMON) ||
			(mimic_info[p_ptr->mimic_form].MIMIC_FLAGS & MIMIC_IS_DEMON))
		{
			if (o_ptr->tval == TV_CORPSE &&
				o_ptr->sval == SV_CORPSE &&
				my_strchr("pht", r_info[o_ptr->pval].d_char))
				return TRUE;
		}

	/* Assume not */
	return (FALSE);
}

/*!
* @brief �I�u�W�F�N�g���ǂ���̎�ɂ������ł��镐�킩�ǂ����̔���
* @param o_ptr ���肷��I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return ���E�����̎�ő����ł���Ȃ��TRUE��Ԃ��B
*/
bool item_tester_hook_mochikae(object_type *o_ptr)
{
	/* Check for a usable slot */
	if (((o_ptr->tval >= TV_DIGGING) && (o_ptr->tval <= TV_SWORD)) ||
		(o_ptr->tval == TV_SHIELD) || (o_ptr->tval == TV_CAPTURE) ||
		(o_ptr->tval == TV_CARD)) return (TRUE);

	/* Assume not wearable */
	return (FALSE);
}

/*!
* @brief �I�u�W�F�N�g���v���C���[��������Ƃ��Ĕ����ł��邩�𔻒肷�� /
* Hook to determine if an object is activatable
* @param o_ptr ���肵�����I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return ������Ƃ��Ĕ����\�Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_activate(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];

	/* Not known */
	if (!object_is_known(o_ptr)) return (FALSE);

	/* Extract the flags */
	object_flags(o_ptr, flgs);

	/* Check activation flag */
	if (have_flag(flgs, TR_ACTIVATE)) return (TRUE);

	/* Assume not */
	return (FALSE);
}

/*!
* @brief �I�u�W�F�N�g��h��Ƃ��đ����ł��邩�̔��� / The "wearable" tester
* @param o_ptr ���肷��I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return �I�u�W�F�N�g���h��Ƃ��đ����ł���Ȃ�TRUE��Ԃ��B
*/
bool item_tester_hook_wear(object_type *o_ptr)
{
	if ((o_ptr->tval == TV_SOFT_ARMOR) && (o_ptr->sval == SV_ABUNAI_MIZUGI))
		if (p_ptr->psex == SEX_MALE) return FALSE;

	/* Check for a usable slot */
	if (wield_slot(o_ptr) >= INVEN_RARM) return (TRUE);

	/* Assume not wearable */
	return (FALSE);
}


/*!
* @brief �I�u�W�F�N�g���v���C���[���ȈՎg�p�R�}���h�ŗ��p�ł��邩�𔻒肷�� /
* Hook to determine if an object is useable
* @param o_ptr ���肵�����I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return ���p�\�Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_use(object_type *o_ptr)
{
	u32b flgs[TR_FLAG_SIZE];

	/* Ammo */
	if (o_ptr->tval == p_ptr->tval_ammo)
		return (TRUE);

	/* Useable object */
	switch (o_ptr->tval)
	{
	case TV_SPIKE:
	case TV_STAFF:
	case TV_WAND:
	case TV_ROD:
	case TV_SCROLL:
	case TV_POTION:
	case TV_FOOD:
	{
		return (TRUE);
	}

	default:
	{
		int i;

		/* Not known */
		if (!object_is_known(o_ptr)) return (FALSE);

		/* HACK - only items from the equipment can be activated */
		for (i = INVEN_RARM; i < INVEN_TOTAL; i++)
		{
			if (&inventory[i] == o_ptr)
			{
				/* Extract the flags */
				object_flags(o_ptr, flgs);

				/* Check activation flag */
				if (have_flag(flgs, TR_ACTIVATE)) return (TRUE);
			}
		}
	}
	}

	/* Assume not */
	return (FALSE);
}


/*!
* @brief �I�u�W�F�N�g���v���C���[�����ނ��Ƃ��ł��邩�𔻒肷�� /
* Hook to determine if an object can be quaffed
* @param o_ptr ���肵�����I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return ���ނ��Ƃ��\�Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_quaff(object_type *o_ptr)
{
	if (o_ptr->tval == TV_POTION) return TRUE;

	if (prace_is_(RACE_ANDROID))
	{
		if (o_ptr->tval == TV_FLASK && o_ptr->sval == SV_FLASK_OIL)
			return TRUE;
	}
	return FALSE;
}


/*!
* @brief �I�u�W�F�N�g���v���C���[���ǂނ��Ƃ��ł��邩�𔻒肷�� /
* Hook to determine if an object is readable
* @param o_ptr ���肵�����I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return �ǂނ��Ƃ��\�Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_readable(object_type *o_ptr)
{
	if ((o_ptr->tval == TV_SCROLL) || (o_ptr->tval == TV_PARCHMENT) || (o_ptr->name1 == ART_GHB) || (o_ptr->name1 == ART_POWER)) return (TRUE);

	/* Assume not */
	return (FALSE);
}


/*!
* @brief �G�b�Z���X�̕t���\�ȕ�����e����Ԃ�
* @param o_ptr �`�F�b�N�������I�u�W�F�N�g�̍\���̎Q�ƃ|�C���^
* @return �G�b�Z���X�̕t���\�ȕ��킩��e�Ȃ��TRUE��Ԃ��B
*/
bool item_tester_hook_melee_ammo(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
	case TV_HAFTED:
	case TV_POLEARM:
	case TV_DIGGING:
	case TV_BOLT:
	case TV_ARROW:
	case TV_SHOT:
	{
		return (TRUE);
	}
	case TV_SWORD:
	{
		if (o_ptr->sval != SV_DOKUBARI) return (TRUE);
	}
	}

	return (FALSE);
}

/*!
* @brief ��p�̈�̕�������̑Ώۂɂł��镐�킩�ǂ�����Ԃ��B / An "item_tester_hook" for offer
* @param o_ptr �I�u�W�F�N�g�\���̂̎Q�ƃ|�C���^
* @return �����\�ȕ���Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_weapon_except_bow(object_type *o_ptr)
{
	switch (o_ptr->tval)
	{
	case TV_SWORD:
	case TV_HAFTED:
	case TV_POLEARM:
	case TV_DIGGING:
	{
		return (TRUE);
	}
	}

	return (FALSE);
}

/*!
* @brief ��p�̈�̊e�����Ɏg������ꂽ�������ǂ�����Ԃ��B / An "item_tester_hook" for offer
* @param o_ptr �I�u�W�F�N�g�\���̂̎Q�ƃ|�C���^
* @return �g���鑕���Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_cursed(object_type *o_ptr)
{
	return (bool)(object_is_cursed(o_ptr));
}


/*!
* @brief �A�C�e�������̉��l�̃A�C�e�����ǂ������肷�� /
* Check if an object is nameless weapon or armour
* @param o_ptr ���肷��A�C�e���̏��Q�ƃ|�C���^
* @return ���Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_nameless_weapon_armour(object_type *o_ptr)
{
	/* Require weapon or armour */
	if (!object_is_weapon_armour_ammo(o_ptr)) return FALSE;

	/* Require nameless object if the object is well known */
	if (object_is_known(o_ptr) && !object_is_nameless(o_ptr))
		return FALSE;

	return TRUE;
}


/*!
* @brief �A�C�e�����Ӓ�ς݂��𔻒肷�� /
* @param o_ptr ���肷��A�C�e���̏��Q�ƃ|�C���^
* @return ���ۂɊӒ�ς݂Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_identify(object_type *o_ptr)
{
	return (bool)!object_is_known(o_ptr);
}

/*!
* @brief �A�C�e�����Ӓ�ς݂̕���h��𔻒肷�� /
* @param o_ptr ���肷��A�C�e���̏��Q�ƃ|�C���^
* @return ���ۂɊӒ�ς݂Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_identify_weapon_armour(object_type *o_ptr)
{
	if (object_is_known(o_ptr))
		return FALSE;
	return object_is_weapon_armour_ammo(o_ptr);
}

/*!
* @brief �A�C�e����*�Ӓ�*�ς݂��𔻒肷�� /
* @param o_ptr ���肷��A�C�e���̏��Q�ƃ|�C���^
* @return ���ۂɊӒ�ς݂Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_identify_fully(object_type *o_ptr)
{
	return (bool)(!object_is_known(o_ptr) || !(o_ptr->ident & IDENT_MENTAL));
}

/*!
* @brief �A�C�e����*�Ӓ�*�ς݂̕���h��𔻒肷�� /
* @param o_ptr ���肷��A�C�e���̏��Q�ƃ|�C���^
* @return ���ۂɊӒ�ς݂Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_identify_fully_weapon_armour(object_type *o_ptr)
{
	if (!item_tester_hook_identify_fully(o_ptr))
		return FALSE;
	return object_is_weapon_armour_ammo(o_ptr);
}


/*!
* @brief ���͏[�U���\�ȃA�C�e�����ǂ������肷�� /
* Hook for "get_item()".  Determine if something is rechargable.
* @param o_ptr ���肷��A�C�e���̏��Q�ƃ|�C���^
* @return ���͏[�U���\�Ȃ��TRUE��Ԃ�
*/
bool item_tester_hook_recharge(object_type *o_ptr)
{
	/* Recharge staffs */
	if (o_ptr->tval == TV_STAFF) return (TRUE);

	/* Recharge wands */
	if (o_ptr->tval == TV_WAND) return (TRUE);

	/* Hack -- Recharge rods */
	if (o_ptr->tval == TV_ROD) return (TRUE);

	/* Nope */
	return (FALSE);
}
