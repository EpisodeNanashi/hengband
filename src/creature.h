#pragma once

/*! @brief �����s���G�l���M�[�l�𐳋K�����ŕԂ�(����100/���U25) / Random energy */
#define ENERGY_NEED() (randnor(100, 25))

/*! @brief �����l�ɉ�������{�s���G�l���M�[����ʂ�Ԃ� / Extract energy from speed (Assumes that SPEED is unsigned) */
#define SPEED_TO_ENERGY(SPEED) (((SPEED) > 199) ? 49 : extract_energy[(SPEED)])

extern const byte extract_energy[200];
