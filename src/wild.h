﻿#pragma once

/* wild.c */
extern void set_floor_and_wall(DUNGEON_IDX type);
extern void wilderness_gen(void);
extern void wilderness_gen_small(void);
extern errr init_wilderness(void);
extern void init_wilderness_terrains(void);
extern void seed_wilderness(void);
extern errr parse_line_wilderness(char *buf, int ymin, int xmin, int ymax, int xmax, int *y, int *x);
extern bool change_wild_mode(void);

/*
 * A structure describing a wilderness area
 * with a terrain or a town
 */
typedef struct 
{
	int terrain;
	TOWN_IDX town;
	int road;
	u32b seed;
	DEPTH level;
	byte entrance;
} wilderness_type;

extern wilderness_type **wilderness;

