/*!
 * @brief ���S�E���ށE�ؕ����̉�ʕ\��
 * @date 2020/02/24
 * @author Hourier
 * @details
 * core�Afiles�Aview-mainwindow�̎Q�Ƌ֎~�B�R�[���o�b�N�őΉ����邱��
 */

#include "process-death.h"
#include "world.h"
#include "floor-town.h"
#include "player-inventory.h"
#include "object-flavor.h"
#include "store.h"
#include "term.h"

#define GRAVE_LINE_WIDTH 31

concptr ANGBAND_DIR_FILE; //!< Various extra files (ascii) These files may be portable between platforms

/*!
 * @brief ��΂̐^�񒆂ɕ�������������� /
 * Centers a string within a GRAVE_LINE_WIDTH character string		-JWT-
 * @return �Ȃ�
 * @details
 */
static void center_string(char *buf, concptr str)
{
	int i = strlen(str);
	int j = GRAVE_LINE_WIDTH / 2 - i / 2;
	(void)sprintf(buf, "%*s%s%*s", j, "", str, GRAVE_LINE_WIDTH - i - j, "");
}


/*
 * Redefinable "print_tombstone" action
 */
bool(*tombstone_aux)(void) = NULL;


/*!
 * @brief ��΂̃A�X�L�[�A�[�g�\�� /
 * Display a "tomb-stone"
 * @param creature_ptr �v���[���[�ւ̎Q�ƃ|�C���^
 * @return �Ȃ�
 */
void print_tomb(player_type *dead_ptr)
{
	bool done = FALSE;
	if (tombstone_aux)
	{
		done = (*tombstone_aux)();
	}

	if (done) return;

#ifdef JP
	int extra_line = 0;
#endif
	Term_clear();
	char buf[1024];
	path_build(buf, sizeof(buf), ANGBAND_DIR_FILE, _("dead_j.txt", "dead.txt"));

	FILE *fp;
	fp = my_fopen(buf, "r");

	if (fp)
	{
		int i = 0;

		/* Dump the file to the screen */
		while (my_fgets(fp, buf, sizeof(buf)) == 0)
		{
			put_str(buf, i++, 0);
		}

		my_fclose(fp);
	}

	concptr p;
	if (current_world_ptr->total_winner || (dead_ptr->lev > PY_MAX_LEVEL))
	{
#ifdef JP
		p = "�̑�Ȃ��";
#else
		p = "Magnificent";
#endif
	}
	else
	{
		p = player_title[dead_ptr->pclass][(dead_ptr->lev - 1) / 5];
	}

	center_string(buf, dead_ptr->name);
	put_str(buf, 6, 11);

#ifdef JP
#else
	center_string(buf, "the");
	put_str(buf, 7, 11);
#endif

	center_string(buf, p);
	put_str(buf, 8, 11);

	center_string(buf, cp_ptr->title);
	put_str(buf, 10, 11);

	char tmp[160];
	(void)sprintf(tmp, _("���x��: %d", "Level: %d"), (int)dead_ptr->lev);
	center_string(buf, tmp);
	put_str(buf, 11, 11);

	(void)sprintf(tmp, _("�o���l: %ld", "Exp: %ld"), (long)dead_ptr->exp);
	center_string(buf, tmp);
	put_str(buf, 12, 11);

	(void)sprintf(tmp, _("������: %ld", "AU: %ld"), (long)dead_ptr->au);
	center_string(buf, tmp);
	put_str(buf, 13, 11);

#ifdef JP
	/* ��ɍ��ތ��t���I���W�i�����ׂ����\�� */
	if (streq(dead_ptr->died_from, "�r���I��"))
	{
		strcpy(tmp, "<���E>");
	}
	else if (streq(dead_ptr->died_from, "ripe"))
	{
		strcpy(tmp, "���ތ�ɓV����S��");
	}
	else if (streq(dead_ptr->died_from, "Seppuku"))
	{
		strcpy(tmp, "�����̌�A�ؕ�");
	}
	else
	{
		roff_to_buf(dead_ptr->died_from, GRAVE_LINE_WIDTH + 1, tmp, sizeof tmp);
		char *t;
		t = tmp + strlen(tmp) + 1;
		if (*t)
		{
			char dummy[80];
			strcpy(dummy, t); /* 2nd line */
			if (*(t + strlen(t) + 1)) /* Does 3rd line exist? */
			{
				for (t = dummy + strlen(dummy) - 2; iskanji(*(t - 1)); t--) /* Loop */;
				strcpy(t, "�c");
			}
			else if (my_strstr(tmp, "�w") && suffix(dummy, "�x"))
			{
				char dummy2[80];
				char *name_head = my_strstr(tmp, "�w");
				sprintf(dummy2, "%s%s", name_head, dummy);
				if (strlen(dummy2) <= GRAVE_LINE_WIDTH)
				{
					strcpy(dummy, dummy2);
					*name_head = '\0';
				}
			}
			else if (my_strstr(tmp, "�u") && suffix(dummy, "�v"))
			{
				char dummy2[80];
				char *name_head = my_strstr(tmp, "�u");
				sprintf(dummy2, "%s%s", name_head, dummy);
				if (strlen(dummy2) <= GRAVE_LINE_WIDTH)
				{
					strcpy(dummy, dummy2);
					*name_head = '\0';
				}
			}

			center_string(buf, dummy);
			put_str(buf, 15, 11);
			extra_line = 1;
		}
	}

	center_string(buf, tmp);
	put_str(buf, 14, 11);

	if (!streq(dead_ptr->died_from, "ripe") && !streq(dead_ptr->died_from, "Seppuku"))
	{
		if (dead_ptr->current_floor_ptr->dun_level == 0)
		{
			concptr field_name = dead_ptr->town_num ? "�X" : "�r��";
			if (streq(dead_ptr->died_from, "�r���I��"))
			{
				sprintf(tmp, "%s�Ŏ���", field_name);
			}
			else
			{
				sprintf(tmp, "��%s�ŎE���ꂽ", field_name);
			}
		}
		else
		{
			if (streq(dead_ptr->died_from, "�r���I��"))
			{
				sprintf(tmp, "�n�� %d �K�Ŏ���", (int)dead_ptr->current_floor_ptr->dun_level);
			}
			else
			{
				sprintf(tmp, "�ɒn�� %d �K�ŎE���ꂽ", (int)dead_ptr->current_floor_ptr->dun_level);
			}
		}

		center_string(buf, tmp);
		put_str(buf, 15 + extra_line, 11);
	}
#else
	(void)sprintf(tmp, "Killed on Level %d", dead_ptr->current_floor_ptr->dun_level);
	center_string(buf, tmp);
	put_str(buf, 14, 11);

	roff_to_buf(format("by %s.", dead_ptr->died_from), GRAVE_LINE_WIDTH + 1, tmp, sizeof tmp);
	center_string(buf, tmp);
	char *t;
	put_str(buf, 15, 11);
	t = tmp + strlen(tmp) + 1;
	if (*t)
	{
		char dummy[80];
		strcpy(dummy, t); /* 2nd line */
		if (*(t + strlen(t) + 1)) /* Does 3rd line exist? */
		{
			int dummy_len = strlen(dummy);
			strcpy(dummy + MIN(dummy_len, GRAVE_LINE_WIDTH - 3), "...");
		}
		center_string(buf, dummy);
		put_str(buf, 16, 11);
	}
#endif
	time_t ct = time((time_t*)0);
	(void)sprintf(tmp, "%-.24s", ctime(&ct));
	center_string(buf, tmp);
	put_str(buf, 17, 11);
	msg_format(_("���悤�Ȃ�A%s!", "Goodbye, %s!"), dead_ptr->name);
}


/*!
 * todo handle_stuff�Adisplay_player�̈����͎b��B�ǂ̂悤�ɐ݌v�������������l����
 * @brief ���S�A���ގ��̊ȈՃX�e�[�^�X�\��
 * @param creature_ptr �v���[���[�ւ̎Q�ƃ|�C���^
 * @param handle_stuff �X�V�����`�F�b�N�ւ̃R�[���o�b�N
 * @param file_character �X�e�[�^�X�_���v�ւ̃R�[���o�b�N
 * @param update_playtime �v���C���ԍX�V�����ւ̃R�[���o�b�N
 * @param display_player �X�e�[�^�X�\���ւ̃R�[���o�b�N
 * @return �Ȃ�
 */
void show_info(player_type *creature_ptr, void(*handle_stuff)(player_type*), errr(*file_character)(player_type*, concptr), void(*update_playtime)(void), void(*display_player)(player_type*, int))
{
	object_type *o_ptr;
	for (int i = 0; i < INVEN_TOTAL; i++)
	{
		o_ptr = &creature_ptr->inventory_list[i];
		if (!o_ptr->k_idx) continue;

		object_aware(creature_ptr, o_ptr);
		object_known(o_ptr);
	}

	store_type *st_ptr;
	for (int i = 1; i < max_towns; i++)
	{
		st_ptr = &town_info[i].store[STORE_HOME];

		/* Hack -- Know everything in the home */
		for (int j = 0; j < st_ptr->stock_num; j++)
		{
			o_ptr = &st_ptr->stock[j];
			if (!o_ptr->k_idx) continue;

			/* Aware and Known */
			object_aware(creature_ptr, o_ptr);
			object_known(o_ptr);
		}
	}

	creature_ptr->update |= (PU_BONUS);
	handle_stuff(creature_ptr);
	flush();
	msg_erase();
	prt(_("�L�����N�^�[�̋L�^���t�@�C���ɏ����o�����Ƃ��ł��܂��B", "You may now dump a character record to one or more files."), 21, 0);
	prt(_("���^�[���L�[�ŃL�����N�^�[�����܂��BESC�Œ��f���܂��B", "Then, hit RETURN to see the character, or ESC to abort."), 22, 0);
	while (TRUE)
	{
		char out_val[160];
		put_str(_("�t�@�C���l�[��: ", "Filename: "), 23, 0);
		strcpy(out_val, "");
		if (!askfor(out_val, 60)) return;
		if (!out_val[0]) break;
		screen_save();
		(void)file_character(creature_ptr, out_val);
		screen_load();
	}

	update_playtime();
	display_player(creature_ptr, 0);
	prt(_("�����L�[�������Ƃ���ɏ�񂪑����܂� (ESC�Œ��f): ", "Hit any key to see more information (ESC to abort): "), 23, 0);
	if (inkey() == ESCAPE) return;

	if (creature_ptr->equip_cnt)
	{
		Term_clear();
		(void)show_equipment(creature_ptr, 0, USE_FULL, 0);
		prt(_("�������Ă����A�C�e��: -����-", "You are using: -more-"), 0, 0);
		if (inkey() == ESCAPE) return;
	}

	if (creature_ptr->inven_cnt)
	{
		Term_clear();
		(void)show_inventory(creature_ptr, 0, USE_FULL, 0);
		prt(_("�����Ă����A�C�e��: -����-", "You are carrying: -more-"), 0, 0);

		if (inkey() == ESCAPE) return;
	}

	for (int l = 1; l < max_towns; l++)
	{
		st_ptr = &town_info[l].store[STORE_HOME];
		if (st_ptr->stock_num == 0) continue;
		for (int i = 0, k = 0; i < st_ptr->stock_num; k++)
		{
			Term_clear();
			for (int j = 0; (j < 12) && (i < st_ptr->stock_num); j++, i++)
			{
				GAME_TEXT o_name[MAX_NLEN];
				char tmp_val[80];
				o_ptr = &st_ptr->stock[i];
				sprintf(tmp_val, "%c) ", I2A(j));
				prt(tmp_val, j + 2, 4);
				object_desc(creature_ptr, o_name, o_ptr, 0);
				c_put_str(tval_to_attr[o_ptr->tval], o_name, j + 2, 7);
			}

			prt(format(_("�䂪�Ƃɒu���Ă������A�C�e�� ( %d �y�[�W): -����-", "Your home contains (page %d): -more-"), k + 1), 0, 0);
			if (inkey() == ESCAPE) return;
		}
	}
}
