
/*! �f�o�b�O����nest�̃����X�^�[�����m�F���邽�߂̍\���� / A struct for nest monster information with cheat_hear */
typedef struct
{
	s16b r_idx;
	bool used;
}
nest_mon_info_type;

/*!
* vault�ɔz�u�\�ȃ����X�^�[�̏������w�肷��}�N�� / Monster validation macro
*
* Line 1 -- forbid town monsters
* Line 2 -- forbid uniques
* Line 3 -- forbid aquatic monsters
*/
#define vault_monster_okay(I) \
	(mon_hook_dungeon(I) && \
	 !(r_info[I].flags1 & RF1_UNIQUE) && \
	 !(r_info[I].flags7 & RF7_UNIQUE2) && \
	 !(r_info[I].flagsr & RFR_RES_ALL) && \
	 !(r_info[I].flags7 & RF7_AQUATIC))

extern bool build_type5(void);
extern bool build_type6(void);
extern bool build_type13(void);
