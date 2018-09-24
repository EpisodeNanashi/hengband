#include "angband.h"
#include "monster-hook.h"


/*!
* @brief �����X�^�[���N�G�X�g�̓����Ώۂɐ��蓾�邩��Ԃ� / Hook function for quest monsters
* @param r_idx �����X�^�[�h�c
* @return �����Ώۂɂł���Ȃ�TRUE��Ԃ��B
*/
bool mon_hook_quest(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	/* Random quests are in the dungeon */
	if (r_ptr->flags8 & RF8_WILD_ONLY) return FALSE;

	/* No random quests for aquatic monsters */
	if (r_ptr->flags7 & RF7_AQUATIC) return FALSE;

	/* No random quests for multiplying monsters */
	if (r_ptr->flags2 & RF2_MULTIPLY) return FALSE;

	/* No quests to kill friendly monsters */
	if (r_ptr->flags7 & RF7_FRIENDLY) return FALSE;

	return TRUE;
}


/*!
* @brief �����X�^�[���_���W�����ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �_���W�����ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_dungeon(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!(r_ptr->flags8 & RF8_WILD_ONLY))
		return TRUE;
	else
	{
		dungeon_info_type *d_ptr = &d_info[dungeon_type];
		if ((d_ptr->mflags8 & RF8_WILD_MOUNTAIN) &&
			(r_ptr->flags8 & RF8_WILD_MOUNTAIN)) return TRUE;
		return FALSE;
	}
}


/*!
* @brief �����X�^�[���C�m�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �C�m�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_ocean(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_OCEAN)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[���C�݂ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �C�݂ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_shore(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_SHORE)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[���r�n�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �r�n�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_waste(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_WASTE | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[�����ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �r�n�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_town(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_TOWN | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[���X�тɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �X�тɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_wood(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_WOOD | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[���ΎR�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �ΎR�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_volcano(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_VOLCANO)
		return TRUE;
	else
		return FALSE;
}

/*!
* @brief �����X�^�[���R�n�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �R�n�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_mountain(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & RF8_WILD_MOUNTAIN)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[�������ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �X�тɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_grass(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (r_ptr->flags8 & (RF8_WILD_GRASS | RF8_WILD_ALL))
		return TRUE;
	else
		return FALSE;
}

/*!
* @brief �����X�^�[���[�����n�`�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �[�����n�`�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_deep_water(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (r_ptr->flags7 & RF7_AQUATIC)
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[���󂢐��n�`�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �󂢐��n�`�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_shallow_water(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (r_ptr->flags2 & RF2_AURA_FIRE)
		return FALSE;
	else
		return TRUE;
}


/*!
* @brief �����X�^�[���n��n�`�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �n��n�`�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_lava(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!mon_hook_dungeon(r_idx)) return FALSE;

	if (((r_ptr->flagsr & RFR_EFF_IM_FIRE_MASK) ||
		(r_ptr->flags7 & RF7_CAN_FLY)) &&
		!(r_ptr->flags3 & RF3_AURA_COLD))
		return TRUE;
	else
		return FALSE;
}


/*!
* @brief �����X�^�[���ʏ�̏��n�`�ɏo�����邩�ǂ�����Ԃ�
* @param r_idx ���肷�郂���X�^�[�̎푰ID
* @return �ʏ�̏��n�`�ɏo������Ȃ��TRUE��Ԃ�
*/
bool mon_hook_floor(MONRACE_IDX r_idx)
{
	monster_race *r_ptr = &r_info[r_idx];

	if (!(r_ptr->flags7 & RF7_AQUATIC) ||
		(r_ptr->flags7 & RF7_CAN_FLY))
		return TRUE;
	else
		return FALSE;
}
