#include "main/info-initializer.h"
#include "info-reader/fixed-map-parser.h"

/*!
 * @brief ��{���ǂݍ��݂̃��C�����[�`�� /
 * Initialize misc. values
 * @param player_ptr �v���[���[�ւ̎Q�ƃ|�C���^
 * @return �G���[�R�[�h
 */
errr init_misc(player_type *player_ptr) { return parse_fixed_map(player_ptr, "misc.txt", 0, 0, 0, 0); }
