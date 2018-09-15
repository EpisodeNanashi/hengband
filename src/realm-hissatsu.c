#include "angband.h"
#include "cmd-spell.h"

/*!
* @brief ���p�̊e�������s��
* @param spell ���pID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_CAST)
* @return SPELL_NAME / SPELL_DESC ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST����NULL�������Ԃ��B
*/
cptr do_hissatsu_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("��эj", "Tobi-Izuna");
		if (desc) return _("2�}�X���ꂽ�Ƃ���ɂ��郂���X�^�[���U������B", "Attacks a two squares distant monster.");

		if (cast)
		{
			project_length = 2;
			if (!get_aim_dir(&dir)) return NULL;

			project_hook(GF_ATTACK, dir, HISSATSU_2, PROJECT_STOP | PROJECT_KILL);
		}
		break;

	case 1:
		if (name) return _("�܌��J�a��", "3-Way Attack");
		if (desc) return _("3�����ɑ΂��čU������B", "Attacks in 3 directions in one time.");

		if (cast)
		{
			int cdir;
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			for (cdir = 0; cdir < 8; cdir++)
			{
				if (cdd[cdir] == dir) break;
			}

			if (cdir == 8) return NULL;

			y = p_ptr->y + ddy_cdd[cdir];
			x = p_ptr->x + ddx_cdd[cdir];
			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
				msg_print(_("�U���͋��؂����B", "You attack the empty air."));

			y = p_ptr->y + ddy_cdd[(cdir + 7) % 8];
			x = p_ptr->x + ddx_cdd[(cdir + 7) % 8];
			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
				msg_print(_("�U���͋��؂����B", "You attack the empty air."));

			y = p_ptr->y + ddy_cdd[(cdir + 1) % 8];
			x = p_ptr->x + ddx_cdd[(cdir + 1) % 8];
			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
				msg_print(_("�U���͋��؂����B", "You attack the empty air."));
		}
		break;

	case 2:
		if (name) return _("�u�[������", "Boomerang");
		if (desc) return _("������茳�ɖ߂��Ă���悤�ɓ�����B�߂��Ă��Ȃ����Ƃ�����B",
			"Throws current weapon. And it'll return to your hand unless failed.");

		if (cast)
		{
			if (!do_cmd_throw(1, TRUE, -1)) return NULL;
		}
		break;

	case 3:
		if (name) return _("����", "Burning Strike");
		if (desc) return _("�Ή��ϐ��̂Ȃ������X�^�[�ɑ�_���[�W��^����B", "Attacks a monster with more damage unless it has resistance to fire.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_FIRE);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 4:
		if (name) return _("�E�C���m", "Detect Ferocity");
		if (desc) return _("�߂��̎v�l���邱�Ƃ��ł��郂���X�^�[�����m����B", "Detects all monsters except mindless in your vicinity.");

		if (cast)
		{
			detect_monsters_mind(DETECT_RAD_DEFAULT);
		}
		break;

	case 5:
		if (name) return _("�݂ˑł�", "Strike to Stun");
		if (desc) return _("����Ƀ_���[�W��^���Ȃ����A�N�O�Ƃ�����B", "Attempts to stun a monster in the adjacent.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_MINEUCHI);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 6:
		if (name) return _("�J�E���^�[", "Counter");
		if (desc) return _("����ɍU�����ꂽ�Ƃ��ɔ�������B�������邽�т�MP������B",
			"Prepares to counterattack. When attack by a monster, strikes back using SP each time.");

		if (cast)
		{
			if (p_ptr->riding)
			{
				msg_print(_("��n���ɂ͖������B", "You cannot do it when riding."));
				return NULL;
			}
			msg_print(_("����̍U���ɑ΂��Đg�\�����B", "You prepare to counter blow."));
			p_ptr->counter = TRUE;
		}
		break;

	case 7:
		if (name) return _("��������", "Harainuke");
		if (desc) return _("�U��������A���Α��ɔ�����B",
			"Attacks monster with your weapons normally, then move through counter side of the monster.");

		if (cast)
		{
			POSITION y, x;

			if (p_ptr->riding)
			{
				msg_print(_("��n���ɂ͖������B", "You cannot do it when riding."));
				return NULL;
			}

			if (!get_rep_dir2(&dir)) return NULL;

			if (dir == 5) return NULL;
			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (!cave[y][x].m_idx)
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}

			py_attack(y, x, 0);

			if (!player_can_enter(cave[y][x].feat, 0) || is_trap(cave[y][x].feat))
				break;

			y += ddy[dir];
			x += ddx[dir];

			if (player_can_enter(cave[y][x].feat, 0) && !is_trap(cave[y][x].feat) && !cave[y][x].m_idx)
			{
				msg_print(NULL);

				/* Move the player */
				(void)move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP);
			}
		}
		break;

	case 8:
		if (name) return _("�T�[�y���c�^��", "Serpent's Tongue");
		if (desc) return _("�őϐ��̂Ȃ������X�^�[�ɑ�_���[�W��^����B", "Attacks a monster with more damage unless it has resistance to poison.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_POISON);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 9:
		if (name) return _("�a������̑���", "Zammaken");
		if (desc) return _("�����̂Ȃ��׈��ȃ����X�^�[�ɑ�_���[�W��^���邪�A���̃����X�^�[�ɂ͑S�����ʂ��Ȃ��B",
			"Attacks an evil unliving monster with great damage. No effect to other  monsters.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_ZANMA);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 10:
		if (name) return _("������", "Wind Blast");
		if (desc) return _("�U���������������֐�����΂��B", "Attacks an adjacent monster, and blow it away.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, 0);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				return "";
			}
			if (cave[y][x].m_idx)
			{
				int i;
				POSITION ty = y, tx = x;
				POSITION oy = y, ox = x;
				MONSTER_IDX m_idx = cave[y][x].m_idx;
				monster_type *m_ptr = &m_list[m_idx];
				char m_name[80];

				monster_desc(m_name, m_ptr, 0);

				for (i = 0; i < 5; i++)
				{
					y += ddy[dir];
					x += ddx[dir];
					if (cave_empty_bold(y, x))
					{
						ty = y;
						tx = x;
					}
					else break;
				}
				if ((ty != oy) || (tx != ox))
				{
					msg_format(_("%s�𐁂���΂����I", "You blow %s away!"), m_name);
					cave[oy][ox].m_idx = 0;
					cave[ty][tx].m_idx = m_idx;
					m_ptr->fy = ty;
					m_ptr->fx = tx;

					update_mon(m_idx, TRUE);
					lite_spot(oy, ox);
					lite_spot(ty, tx);

					if (r_info[m_ptr->r_idx].flags7 & (RF7_LITE_MASK | RF7_DARK_MASK))
						p_ptr->update |= (PU_MON_LITE);
				}
			}
		}
		break;

	case 11:
		if (name) return _("�����̖ڗ���", "Judge");
		if (desc) return _("����E�h���1���ʂ���B���x��45�ȏ�ŕ���E�h��̔\�͂����S�ɒm�邱�Ƃ��ł���B",
			"Identifies a weapon or armor. Or *identifies* these at level 45.");

		if (cast)
		{
			if (plev > 44)
			{
				if (!identify_fully(TRUE)) return NULL;
			}
			else
			{
				if (!ident_spell(TRUE)) return NULL;
			}
		}
		break;

	case 12:
		if (name) return _("�j��a", "Rock Smash");
		if (desc) return _("����󂵁A��Όn�̃����X�^�[�ɑ�_���[�W��^����B", "Breaks rock. Or greatly damage a monster made by rocks.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_HAGAN);

			if (!cave_have_flag_bold(y, x, FF_HURT_ROCK)) break;

			/* Destroy the feature */
			cave_alter_feat(y, x, FF_HURT_ROCK);

			/* Update some things */
			p_ptr->update |= (PU_FLOW);
		}
		break;

	case 13:
		if (name) return _("����ጎ��", "Midare-Setsugekka");
		if (desc) return _("�U���񐔂������A��C�ϐ��̂Ȃ������X�^�[�ɑ�_���[�W��^����B",
			"Attacks a monster with increased number of attacks and more damage unless it has resistance to cold.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_COLD);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 14:
		if (name) return _("�}���˂�", "Spot Aiming");
		if (desc) return _("�����X�^�[���ꌂ�œ|���U�����J��o���B���s�����1�_�����_���[�W��^�����Ȃ��B",
			"Attempts to kill a monster instantly. If failed cause only 1HP of damage.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_KYUSHO);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 15:
		if (name) return _("���_�a��", "Majingiri");
		if (desc) return _("��S�̈ꌂ�ōU������B�U�������킳��₷���B",
			"Attempts to attack with critical hit. But this attack is easy to evade for a monster.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_MAJIN);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 16:
		if (name) return _("�̂Đg", "Desperate Attack");
		if (desc) return _("���͂ȍU�����J��o���B���̃^�[���܂ł̊ԁA�H�炤�_���[�W��������B",
			"Attacks with all of your power. But all damages you take will be doubled for one turn.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_SUTEMI);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
			p_ptr->sutemi = TRUE;
		}
		break;

	case 17:
		if (name) return _("�����h�܎a", "Lightning Eagle");
		if (desc) return _("�d���ϐ��̂Ȃ������X�^�[�ɔ��ɑ傫���_���[�W��^����B",
			"Attacks a monster with more damage unless it has resistance to electricity.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_ELEC);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 18:
		if (name) return _("���g", "Rush Attack");
		if (desc) return _("�f��������ɋߊ��U������B", "Steps close to a monster and attacks at a time.");

		if (cast)
		{
			if (!rush_attack(NULL)) return NULL;
		}
		break;

	case 19:
		if (name) return _("�ԗ��Q", "Bloody Maelstrom");
		if (desc) return _("�������g���������A���̏����[���قǑ傫���З͂őS�����̓G���U���ł���B�����Ă��Ȃ������X�^�[�ɂ͌��ʂ��Ȃ��B",
			"Attacks all adjacent monsters with power corresponding to your cut status. Then increases your cut status. No effect to unliving monsters.");

		if (cast)
		{
			int y = 0, x = 0;

			cave_type       *c_ptr;
			monster_type    *m_ptr;

			if (p_ptr->cut < 300)
				set_cut(p_ptr->cut + 300);
			else
				set_cut(p_ptr->cut * 2);

			for (dir = 0; dir < 8; dir++)
			{
				y = p_ptr->y + ddy_ddd[dir];
				x = p_ptr->x + ddx_ddd[dir];
				c_ptr = &cave[y][x];

				/* Get the monster */
				m_ptr = &m_list[c_ptr->m_idx];

				/* Hack -- attack monsters */
				if (c_ptr->m_idx && (m_ptr->ml || cave_have_flag_bold(y, x, FF_PROJECT)))
				{
					if (!monster_living(&r_info[m_ptr->r_idx]))
					{
						char m_name[80];

						monster_desc(m_name, m_ptr, 0);
						msg_format(_("%s�ɂ͌��ʂ��Ȃ��I", "%s is unharmed!"), m_name);
					}
					else py_attack(y, x, HISSATSU_SEKIRYUKA);
				}
			}
		}
		break;

	case 20:
		if (name) return _("���k��", "Earthquake Blow");
		if (desc) return _("�n�k���N�����B", "Shakes dungeon structure, and results in random swapping of floors and walls.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_QUAKE);
			else
				earthquake(p_ptr->y, p_ptr->x, 10);
		}
		break;

	case 21:
		if (name) return _("�n����", "Crack");
		if (desc) return _("�Ռ��g�̃r�[������B", "Fires a beam of shock wave.");

		if (cast)
		{
			int total_damage = 0, basedam, i;
			u32b flgs[TR_FLAG_SIZE];
			object_type *o_ptr;
			if (!get_aim_dir(&dir)) return NULL;
			msg_print(_("�����傫���U�艺�낵���B", "You swing your weapon downward."));
			for (i = 0; i < 2; i++)
			{
				int damage;

				if (!buki_motteruka(INVEN_RARM + i)) break;
				o_ptr = &inventory[INVEN_RARM + i];
				basedam = (o_ptr->dd * (o_ptr->ds + 1)) * 50;
				damage = o_ptr->to_d * 100;
				object_flags(o_ptr, flgs);
				if ((o_ptr->name1 == ART_VORPAL_BLADE) || (o_ptr->name1 == ART_CHAINSWORD))
				{
					/* vorpal blade */
					basedam *= 5;
					basedam /= 3;
				}
				else if (have_flag(flgs, TR_VORPAL))
				{
					/* vorpal flag only */
					basedam *= 11;
					basedam /= 9;
				}
				damage += basedam;
				damage *= p_ptr->num_blow[i];
				total_damage += damage / 200;
				if (i) total_damage = total_damage * 7 / 10;
			}
			fire_beam(GF_FORCE, dir, total_damage);
		}
		break;

	case 22:
		if (name) return _("�C���̗Y����", "War Cry");
		if (desc) return _("���E���̑S�����X�^�[�ɑ΂��č����̍U�����s���B����ɁA�߂��ɂ��郂���X�^�[��{�点��B",
			"Damages all monsters in sight with sound. Aggravate nearby monsters.");

		if (cast)
		{
			msg_print(_("�Y���т��������I", "You roar out!"));
			project_hack(GF_SOUND, randint1(plev * 3));
			aggravate_monsters(0);
		}
		break;

	case 23:
		if (name) return _("���o�O�i", "Musou-Sandan");
		if (desc) return _("���͂�3�i�U�����J��o���B", "Attacks with powerful 3 strikes.");

		if (cast)
		{
			int i;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			for (i = 0; i < 3; i++)
			{
				POSITION y, x;
				POSITION ny, nx;
				MONSTER_IDX m_idx;
				cave_type *c_ptr;
				monster_type *m_ptr;

				y = p_ptr->y + ddy[dir];
				x = p_ptr->x + ddx[dir];
				c_ptr = &cave[y][x];

				if (c_ptr->m_idx)
					py_attack(y, x, HISSATSU_3DAN);
				else
				{
					msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
					return NULL;
				}

				if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
				{
					return "";
				}

				/* Monster is dead? */
				if (!c_ptr->m_idx) break;

				ny = y + ddy[dir];
				nx = x + ddx[dir];
				m_idx = c_ptr->m_idx;
				m_ptr = &m_list[m_idx];

				/* Monster cannot move back? */
				if (!monster_can_enter(ny, nx, &r_info[m_ptr->r_idx], 0))
				{
					/* -more- */
					if (i < 2) msg_print(NULL);
					continue;
				}

				c_ptr->m_idx = 0;
				cave[ny][nx].m_idx = m_idx;
				m_ptr->fy = ny;
				m_ptr->fx = nx;

				update_mon(m_idx, TRUE);

				/* Redraw the old spot */
				lite_spot(y, x);

				/* Redraw the new spot */
				lite_spot(ny, nx);

				/* Player can move forward? */
				if (player_can_enter(c_ptr->feat, 0))
				{
					/* Move the player */
					if (!move_player_effect(y, x, MPE_FORGET_FLOW | MPE_HANDLE_STUFF | MPE_DONT_PICKUP)) break;
				}
				else
				{
					break;
				}

				/* -more- */
				if (i < 2) msg_print(NULL);
			}
		}
		break;

	case 24:
		if (name) return _("�z���S�̉�", "Vampire's Fang");
		if (desc) return _("�U����������̗̑͂��z���Ƃ�A�����̗̑͂��񕜂�����B�����������Ȃ������X�^�[�ɂ͒ʂ��Ȃ��B",
			"Attacks with vampiric strikes which absorbs HP from a monster and gives them to you. No effect to unliving monsters.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_DRAIN);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
		}
		break;

	case 25:
		if (name) return _("���f", "Moon Dazzling");
		if (desc) return _("���E���̋N���Ă���S�����X�^�[�ɞN�O�A�����A�����^���悤�Ƃ���B", "Attempts to stun, confuse and sleep all waking monsters.");

		if (cast)
		{
			msg_print(_("�����s�K���ɗh�炵���D�D�D", "You irregularly wave your weapon..."));
			project_hack(GF_ENGETSU, plev * 4);
			project_hack(GF_ENGETSU, plev * 4);
			project_hack(GF_ENGETSU, plev * 4);
		}
		break;

	case 26:
		if (name) return _("�S�l�a��", "Hundred Slaughter");
		if (desc) return _("�A�����ē��g�Ń����X�^�[���U������B�U�����邽�т�MP������BMP���Ȃ��Ȃ邩�A�����X�^�[��|���Ȃ�������S�l�a��͏I������B",
			"Performs a series of rush attacks. The series continues while killing each monster in a time and SP remains.");

		if (cast)
		{
			const int mana_cost_per_monster = 8;
			bool is_new = TRUE;
			bool mdeath;

			do
			{
				if (!rush_attack(&mdeath)) break;
				if (is_new)
				{
					/* Reserve needed mana point */
					p_ptr->csp -= technic_info[REALM_HISSATSU - MIN_TECHNIC][26].smana;
					is_new = FALSE;
				}
				else
					p_ptr->csp -= mana_cost_per_monster;

				if (!mdeath) break;
				command_dir = 0;

				p_ptr->redraw |= PR_MANA;
				handle_stuff();
			} while (p_ptr->csp > mana_cost_per_monster);

			if (is_new) return NULL;

			/* Restore reserved mana */
			p_ptr->csp += technic_info[REALM_HISSATSU - MIN_TECHNIC][26].smana;
		}
		break;

	case 27:
		if (name) return _("�V�ė��M", "Dragonic Flash");
		if (desc) return _("���E���̏ꏊ���w�肵�āA���̏ꏊ�Ǝ����̊Ԃɂ���S�����X�^�[���U�����A���̏ꏊ�Ɉړ�����B",
			"Runs toward given location while attacking all monsters on the path.");

		if (cast)
		{
			POSITION y, x;

			if (!tgt_pt(&x, &y)) return NULL;

			if (!cave_player_teleportable_bold(y, x, 0L) ||
				(distance(y, x, p_ptr->y, p_ptr->x) > MAX_SIGHT / 2) ||
				!projectable(p_ptr->y, p_ptr->x, y, x))
			{
				msg_print(_("���s�I", "You cannot move to that place!"));
				break;
			}
			if (p_ptr->anti_tele)
			{
				msg_print(_("�s�v�c�ȗ͂��e���|�[�g��h�����I", "A mysterious force prevents you from teleporting!"));
				break;
			}
			project(0, 0, y, x, HISSATSU_ISSEN, GF_ATTACK, PROJECT_BEAM | PROJECT_KILL, -1);
			teleport_player_to(y, x, 0L);
		}
		break;

	case 28:
		if (name) return _("��d�̌���", "Twin Slash");
		if (desc) return _("1�^�[����2�x�U�����s���B", "double attacks at a time.");

		if (cast)
		{
			int x, y;

			if (!get_rep_dir(&dir, FALSE)) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
			{
				py_attack(y, x, 0);
				if (cave[y][x].m_idx)
				{
					handle_stuff();
					py_attack(y, x, 0);
				}
			}
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "You don't see any monster in this direction"));
				return NULL;
			}
		}
		break;

	case 29:
		if (name) return _("�Օ��Ⓛ��", "Kofuku-Zettousei");
		if (desc) return _("���͂ȍU�����s���A�߂��̏ꏊ�ɂ����ʂ��y�ԁB", "Performs a powerful attack which even effect nearby monsters.");

		if (cast)
		{
			int total_damage = 0, basedam, i;
			int y, x;
			u32b flgs[TR_FLAG_SIZE];
			object_type *o_ptr;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (d_info[dungeon_type].flags1 & DF1_NO_MELEE)
			{
				msg_print(_("�Ȃ����U�����邱�Ƃ��ł��Ȃ��B", "Something prevent you from attacking."));
				return "";
			}
			msg_print(_("�����傫���U�艺�낵���B", "You swing your weapon downward."));
			for (i = 0; i < 2; i++)
			{
				int damage;
				if (!buki_motteruka(INVEN_RARM + i)) break;
				o_ptr = &inventory[INVEN_RARM + i];
				basedam = (o_ptr->dd * (o_ptr->ds + 1)) * 50;
				damage = o_ptr->to_d * 100;
				object_flags(o_ptr, flgs);
				if ((o_ptr->name1 == ART_VORPAL_BLADE) || (o_ptr->name1 == ART_CHAINSWORD))
				{
					/* vorpal blade */
					basedam *= 5;
					basedam /= 3;
				}
				else if (have_flag(flgs, TR_VORPAL))
				{
					/* vorpal flag only */
					basedam *= 11;
					basedam /= 9;
				}
				damage += basedam;
				damage += p_ptr->to_d[i] * 100;
				damage *= p_ptr->num_blow[i];
				total_damage += (damage / 100);
			}
			project(0, (cave_have_flag_bold(y, x, FF_PROJECT) ? 5 : 0), y, x, total_damage * 3 / 2, GF_METEOR, PROJECT_KILL | PROJECT_JUMP | PROJECT_ITEM, -1);
		}
		break;

	case 30:
		if (name) return _("�c�_�S�E��", "Keiun-Kininken");
		if (desc) return _("�������_���[�W�����炤���A����ɔ��ɑ傫�ȃ_���[�W��^����B�A���f�b�h�ɂ͓��Ɍ��ʂ�����B",
			"Attacks a monster with extremely powerful damage. But you also takes some damages. Hurts a undead monster greatly.");

		if (cast)
		{
			int y, x;

			if (!get_rep_dir2(&dir)) return NULL;
			if (dir == 5) return NULL;

			y = p_ptr->y + ddy[dir];
			x = p_ptr->x + ddx[dir];

			if (cave[y][x].m_idx)
				py_attack(y, x, HISSATSU_UNDEAD);
			else
			{
				msg_print(_("���̕����ɂ̓����X�^�[�͂��܂���B", "There is no monster."));
				return NULL;
			}
			take_hit(DAMAGE_NOESCAPE, 100 + randint1(100), _("�c�_�S�E�����g�����Ռ�", "exhaustion on using Keiun-Kininken"), -1);
		}
		break;

	case 31:
		if (name) return _("�ؕ�", "Harakiri");
		if (desc) return _("�u���m���Ƃ́A���ʂ��Ƃƌ�������B�v", "'Busido is found in death'");

		if (cast)
		{
			int i;
			if (!get_check(_("�{���Ɏ��E���܂����H", "Do you really want to commit suicide? "))) return NULL;
			/* Special Verification for suicide */
			prt(_("�m�F�̂��� '@' �������ĉ������B", "Please verify SUICIDE by typing the '@' sign: "), 0, 0);

			flush();
			i = inkey();
			prt("", 0, 0);
			if (i != '@') return NULL;
			if (p_ptr->total_winner)
			{
				take_hit(DAMAGE_FORCE, 9999, "Seppuku", -1);
				p_ptr->total_winner = TRUE;
			}
			else
			{
				msg_print(_("���m���Ƃ́A���ʂ��Ƃƌ�������B", "Meaning of Bushi-do is found in the death."));
				take_hit(DAMAGE_FORCE, 9999, "Seppuku", -1);
			}
		}
		break;
	}

	return "";
}
