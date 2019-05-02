#include "angband.h"
#include "player-sex.h"

/*
 * Pointer to the player tables
 * (sex, race, class, magic)
 */
const player_sex *sp_ptr;

/*!
 * @brief ���ʕ\�L /
 * Player Sexes
 * @details
 * <pre>
 *      Title,
 *      Winner
 * </pre>
 */
const player_sex sex_info[MAX_SEXES] =
{
	{
#ifdef JP
		"����",
		"�N�C�[��",
#endif
		"Female",
		"Queen"
	},
	{
#ifdef JP
		"�j��",
		"�L���O",
#endif
		"Male",
		"King"
	}
};

