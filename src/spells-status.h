﻿
extern bool heal_monster(DIRECTION dir, HIT_POINT dam);
extern bool speed_monster(DIRECTION dir, int power);
extern bool slow_monster(DIRECTION dir, int power);
extern bool sleep_monster(DIRECTION dir, int power);
extern bool stasis_monster(DIRECTION dir);    /* Like sleep, affects undead as well */
extern bool stasis_evil(DIRECTION dir);    /* Like sleep, affects undead as well */
extern bool confuse_monster(DIRECTION dir, PLAYER_LEVEL plev);
extern bool stun_monster(DIRECTION dir, PLAYER_LEVEL plev);
extern bool fear_monster(DIRECTION dir, PLAYER_LEVEL plev);
extern bool poly_monster(DIRECTION dir, int power);
extern bool clone_monster(DIRECTION dir);
extern void stop_singing(void);
extern bool time_walk(player_type *creature_ptr);
extern void roll_hitdice(player_type *creature_ptr, SPOP_FLAGS options);
extern bool_hack life_stream(bool_hack message, bool_hack virtue_change);
extern bool_hack heroism(int base);
extern bool_hack berserk(int base);
extern bool_hack cure_light_wounds(DICE_NUMBER dice, DICE_SID sides);
extern bool_hack cure_serious_wounds(DICE_NUMBER dice, DICE_SID sides);
extern bool_hack cure_critical_wounds(HIT_POINT pow);
extern bool_hack true_healing(HIT_POINT pow);
extern bool_hack restore_mana(bool_hack magic_eater);
extern bool restore_all_status(void);

