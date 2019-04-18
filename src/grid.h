﻿/*!
 * @file grid.h
 * @brief ダンジョンの生成処理の基幹部分ヘッダーファイル
 * @date 2014/08/15
 * @details
 * Purpose: header file for grid.c, used only in dungeon generation
 * files (generate.c, rooms.c)
 * @author
 * Copyright (c) 1989 James E. Wilson, Robert A. Koeneke
 * This software may be copied and distributed for educational, research, and
 * not for profit purposes provided that this copyright and statement are
 * included in all such copies.
 */

/* Macros */

#define set_cave_feat(Y,X,F)    (current_floor_ptr->grid_array[(Y)][(X)].feat = (F))
#define add_cave_info(Y,X,I)    (current_floor_ptr->grid_array[(Y)][(X)].info |= (I))

/* This should not be used */
/*#define set_cave_info(Y,X,I)    (current_floor_ptr->grid_array[(Y)][(X)].info = (I)) */

/*!
 * @brief 指定座標が瓦礫かどうかを返す
 * @param Y 指定Y座標
 * @param X 指定X座標
 * @return 瓦礫ならばTRUE
 */
#define place_rubble(Y,X)       set_cave_feat(Y,X,feat_rubble)

/*!
 * @brief 指定座標が上り階段かどうかを返す
 * @param Y 指定Y座標
 * @param X 指定X座標
 * @return 上り階段ならばTRUE
 */
#define place_up_stairs(Y,X)    set_cave_feat(Y,X,feat_up_stair)

/*!
 * @brief 指定座標が下り階段かどうかを返す
 * @param Y 指定Y座標
 * @param X 指定X座標
 * @return 下り階段ならばTRUE
 */
#define place_down_stairs(Y,X)  set_cave_feat(Y,X,feat_down_stair)

/*!
 * @brief 指定座標がFLOOR属性を持ったマスかどうかを返す
 * @param Y 指定Y座標
 * @param X 指定X座標
 * @return FLOOR属性を持っているならばTRUE
 */
#define is_floor_bold(Y,X) (current_floor_ptr->grid_array[Y][X].info & CAVE_FLOOR)
#define is_extra_bold(Y,X) (current_floor_ptr->grid_array[Y][X].info & CAVE_EXTRA)

#define is_inner_bold(Y,X) (current_floor_ptr->grid_array[Y][X].info & CAVE_INNER)
#define is_outer_bold(Y,X) (current_floor_ptr->grid_array[Y][X].info & CAVE_OUTER)
#define is_solid_bold(Y,X) (current_floor_ptr->grid_array[Y][X].info & CAVE_SOLID)

#define is_floor_grid(C) ((C)->info & CAVE_FLOOR)
#define is_extra_grid(C) ((C)->info & CAVE_EXTRA)
#define is_inner_grid(C) ((C)->info & CAVE_INNER)
#define is_outer_grid(C) ((C)->info & CAVE_OUTER)
#define is_solid_grid(C) ((C)->info & CAVE_SOLID)

#define place_floor_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_ground_type[randint0(100)]); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_FLOOR); \
	delete_monster(Y, X); \
}

#define place_floor_grid(C) \
{ \
	(C)->feat = feat_ground_type[randint0(100)]; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_FLOOR; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_extra_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_wall_type[randint0(100)]); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_EXTRA); \
	delete_monster(Y, X); \
}

#define place_extra_grid(C) \
{ \
	(C)->feat = feat_wall_type[randint0(100)]; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_EXTRA; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_extra_perm_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_permanent); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_EXTRA); \
	delete_monster(Y, X); \
}

#define place_extra_perm_grid(C) \
{ \
	(C)->feat = feat_permanent; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_EXTRA; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_extra_noperm_bold(Y, X) \
{ \
	feature_type *_f_ptr; \
	set_cave_feat(Y,X,feat_wall_type[randint0(100)]); \
	_f_ptr = &f_info[current_floor_ptr->grid_array[Y][X].feat]; \
	if (permanent_wall(_f_ptr)) current_floor_ptr->grid_array[Y][X].feat = feat_state(current_floor_ptr->grid_array[Y][X].feat, FF_UNPERM); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_EXTRA); \
	delete_monster(Y, X); \
}

#define place_inner_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_wall_inner); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_INNER); \
	delete_monster(Y, X); \
}

#define place_inner_grid(C) \
{ \
	(C)->feat = feat_wall_inner; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_INNER; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_inner_perm_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_permanent); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_INNER); \
	delete_monster(Y, X); \
}

#define place_inner_perm_grid(C) \
{ \
	(C)->feat = feat_permanent; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_INNER; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_outer_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_wall_outer); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_OUTER); \
	delete_monster(Y, X); \
}

#define place_outer_grid(C) \
{ \
	(C)->feat = feat_wall_outer; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_OUTER; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_outer_perm_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_permanent); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_OUTER); \
	delete_monster(Y, X); \
}

#define place_outer_perm_grid(C) \
{ \
	(C)->feat = feat_permanent; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_OUTER; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_outer_noperm_bold(Y, X) \
{ \
	feature_type *_f_ptr = &f_info[feat_wall_outer]; \
	if (permanent_wall(_f_ptr)) set_cave_feat(Y, X, (s16b)feat_state(feat_wall_outer, FF_UNPERM)); \
	else set_cave_feat(Y,X,feat_wall_outer); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,(CAVE_OUTER | CAVE_VAULT)); \
	delete_monster(Y, X); \
}

#define place_outer_noperm_grid(C) \
{ \
	feature_type *_f_ptr = &f_info[feat_wall_outer]; \
	if (permanent_wall(_f_ptr)) (C)->feat = (s16b)feat_state(feat_wall_outer, FF_UNPERM); \
	else (C)->feat = feat_wall_outer; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= (CAVE_OUTER | CAVE_VAULT); \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_solid_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_wall_solid); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_SOLID); \
	delete_monster(Y, X); \
}

#define place_solid_grid(C) \
{ \
	(C)->feat = feat_wall_solid; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_SOLID; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_solid_perm_bold(Y, X) \
{ \
	set_cave_feat(Y,X,feat_permanent); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_SOLID); \
	delete_monster(Y, X); \
}

#define place_solid_perm_grid(C) \
{ \
	(C)->feat = feat_permanent; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_SOLID; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}

#define place_solid_noperm_bold(Y, X) \
{ \
	feature_type *_f_ptr = &f_info[feat_wall_solid]; \
	if ((current_floor_ptr->grid_array[Y][X].info & CAVE_VAULT) && permanent_wall(_f_ptr)) \
		set_cave_feat(Y, X, feat_state(feat_wall_solid, FF_UNPERM)); \
	else set_cave_feat(Y,X,feat_wall_solid); \
	current_floor_ptr->grid_array[Y][X].info &= ~(CAVE_MASK); \
	add_cave_info(Y,X,CAVE_SOLID); \
	delete_monster(Y, X); \
}

#define place_solid_noperm_grid(C) \
{ \
	feature_type *_f_ptr = &f_info[feat_wall_solid]; \
	if (((C)->info & CAVE_VAULT) && permanent_wall(_f_ptr)) \
		(C)->feat = feat_state(feat_wall_solid, FF_UNPERM); \
	else (C)->feat = feat_wall_solid; \
	(C)->info &= ~(CAVE_MASK); \
	(C)->info |= CAVE_SOLID; \
	if ((C)->m_idx) delete_monster_idx((C)->m_idx); \
}


/*
 * 特殊なマス状態フラグ / Special grid flags
 */
#define CAVE_MARK       0x0001    /*!< 現在プレイヤーの記憶に収まっている / memorized feature */
#define CAVE_GLOW       0x0002    /*!< マス自体が光源を持っている / self-illuminating */
#define CAVE_ICKY       0x0004    /*!< 生成されたVaultの一部である / part of a vault */
#define CAVE_ROOM       0x0008    /*!< 生成された部屋の一部である / part of a room */
#define CAVE_LITE       0x0010    /*!< 現在光に照らされている / lite flag  */
#define CAVE_VIEW       0x0020    /*!< 現在プレイヤーの視界に収まっている / view flag */
#define CAVE_TEMP       0x0040    /*!< 光源に関する処理のアルゴリズム用記録フラグ / temp flag */
#define CAVE_XTRA       0x0080    /*!< 視界に関する処理のアルゴリズム用記録フラグ(update_view()等参照) / misc flag */
#define CAVE_MNLT       0x0100    /*!< モンスターの光源によって照らされている / Illuminated by monster */
#define CAVE_MNDK       0x8000    /*!< モンスターの暗源によって暗闇になっている / Darken by monster */

 /* Used only while current_floor_ptr->grid_array generation */
#define CAVE_FLOOR      0x0200	/*!< フロア属性のあるマス */
#define CAVE_EXTRA      0x0400
#define CAVE_INNER      0x0800
#define CAVE_OUTER      0x1000
#define CAVE_SOLID      0x2000
#define CAVE_VAULT      0x4000
#define CAVE_MASK (CAVE_FLOOR | CAVE_EXTRA | CAVE_INNER | CAVE_OUTER | CAVE_SOLID | CAVE_VAULT)

/* Used only after current_floor_ptr->grid_array generation */
#define CAVE_KNOWN      0x0200    /* Directly viewed or map detected flag */
#define CAVE_NOTE       0x0400    /* Flag for delayed visual update (needs note_spot()) */
#define CAVE_REDRAW     0x0800    /* Flag for delayed visual update (needs lite_spot()) */
#define CAVE_OBJECT     0x1000    /* Mirror, glyph, etc. */
#define CAVE_UNSAFE     0x2000    /* Might have trap */
#define CAVE_IN_DETECT  0x4000    /* trap detected area (inner circle only) */

/* Externs */

extern bool new_player_spot(void);

extern void place_random_stairs(POSITION y, POSITION x);
extern void place_random_door(POSITION y, POSITION x, bool room);
extern void place_closed_door(POSITION y, POSITION x, int type);
extern void add_door(POSITION x, POSITION y);
extern void place_secret_door(POSITION y, POSITION x, int type);
extern void place_locked_door(POSITION y, POSITION x);
extern void try_door(POSITION y, POSITION x);
extern void place_floor(POSITION x1, POSITION x2, POSITION y1, POSITION y2, bool light);
extern void place_room(POSITION x1, POSITION x2, POSITION y1, POSITION y2, bool light);
extern void vault_monsters(POSITION y1, POSITION x1, int num);
extern void vault_objects(POSITION y, POSITION x, int num);
extern void vault_trap_aux(POSITION y, POSITION x, POSITION yd, POSITION xd);
extern void vault_traps(POSITION y, POSITION x, POSITION yd, POSITION xd, int num);

extern bool get_is_floor(POSITION x, POSITION y);
extern void set_floor(POSITION x, POSITION y);
extern void place_bound_perm_wall(grid_type *g_ptr);

extern bool is_known_trap(grid_type *g_ptr);
extern bool is_hidden_door(grid_type *g_ptr);
extern bool is_mirror_grid(grid_type *g_ptr);
extern bool is_glyph_grid(grid_type *g_ptr);
extern bool is_explosive_rune_grid(grid_type *g_ptr);

extern bool player_can_enter(FEAT_IDX feature, BIT_FLAGS16 mode);

/*!
 * マス構造体のspecial要素を利用する地形かどうかを判定するマクロ / Is this feature has special meaning (except floor_id) with g_ptr->special?
 */
#define feat_uses_special(F) (have_flag(f_info[(F)].flags, FF_SPECIAL))

/* grids.c */
extern POSITION distance(POSITION y1, POSITION x1, POSITION y2, POSITION x2);
extern bool los(POSITION y1, POSITION x1, POSITION y2, POSITION x2);
extern void update_local_illumination(POSITION y, POSITION x);
extern bool player_can_see_bold(POSITION y, POSITION x);
extern bool cave_valid_bold(POSITION y, POSITION x);
extern bool no_lite(void);
extern void map_info(POSITION y, POSITION x, TERM_COLOR *ap, SYMBOL_CODE *cp, TERM_COLOR *tap, SYMBOL_CODE *tcp);
extern void move_cursor_relative(int row, int col);
extern void print_rel(SYMBOL_CODE c, TERM_COLOR a, TERM_LEN y, TERM_LEN x);
extern void note_spot(POSITION y, POSITION x);
extern void display_dungeon(void);
extern void lite_spot(POSITION y, POSITION x);
extern void prt_map(void);
extern void prt_path(POSITION y, POSITION x);
extern void display_map(int *cy, int *cx);
extern void forget_lite(void);
extern void update_lite(void);
extern void forget_view(void);
extern void update_view(void);
extern void update_mon_lite(void);
extern void clear_mon_lite(void);
extern void delayed_visual_update(void);
extern void forget_flow(void);
extern void update_flow(void);
extern void update_smell(void);
extern void map_area(POSITION range);
extern void cave_set_feat(POSITION y, POSITION x, FEAT_IDX feat);
extern FEAT_IDX conv_dungeon_feat(FEAT_IDX newfeat);
extern FEAT_IDX feat_state(FEAT_IDX feat, int action);
extern void cave_alter_feat(POSITION y, POSITION x, int action);
extern void remove_mirror(POSITION y, POSITION x);
extern void lite_spot(POSITION y, POSITION x);
extern bool is_open(FEAT_IDX feat);
extern bool check_local_illumination(POSITION y, POSITION x);

/*!
 * モンスターにより照明が消されている地形か否かを判定する。 / Is this grid "darkened" by monster?
 */
#define darkened_grid(C) \
	((((C)->info & (CAVE_VIEW | CAVE_LITE | CAVE_MNLT | CAVE_MNDK)) == (CAVE_VIEW | CAVE_MNDK)) && \
	!p_ptr->see_nocto)

 /*
  * Get feature mimic from f_info[] (applying "mimic" field)
  */
#define get_feat_mimic(C) \
	(f_info[(C)->mimic ? (C)->mimic : (C)->feat].mimic)
