#include "angband.h"
#include "cmd-spell.h"

/*!
* @brief �̂̊J�n���������� / Start singing if the player is a Bard
* @param spell �̈斂�@�Ƃ��Ă�ID
* @param song ���@���ʂ�ID
* @return �Ȃ�
*/
static void start_singing(SPELL_IDX spell, MAGIC_NUM1 song)
{
	/* Remember the song index */
	SINGING_SONG_EFFECT(p_ptr) = (MAGIC_NUM1)song;

	/* Remember the index of the spell which activated the song */
	SINGING_SONG_ID(p_ptr) = (MAGIC_NUM2)spell;


	/* Now the player is singing */
	set_action(ACTION_SING);


	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);
}

/*!
* @brief �̂̒�~���������� / Stop singing if the player is a Bard
* @return �Ȃ�
*/
void stop_singing(void)
{
	if (p_ptr->pclass != CLASS_BARD) return;

	/* Are there interupted song? */
	if (INTERUPTING_SONG_EFFECT(p_ptr))
	{
		/* Forget interupted song */
		INTERUPTING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
		return;
	}

	/* The player is singing? */
	if (!SINGING_SONG_EFFECT(p_ptr)) return;

	/* Hack -- if called from set_action(), avoid recursive loop */
	if (p_ptr->action == ACTION_SING) set_action(ACTION_NONE);

	/* Message text of each song or etc. */
	do_spell(REALM_MUSIC, SINGING_SONG_ID(p_ptr), SPELL_STOP);

	SINGING_SONG_EFFECT(p_ptr) = MUSIC_NONE;
	SINGING_SONG_ID(p_ptr) = 0;

	/* Recalculate bonuses */
	p_ptr->update |= (PU_BONUS);

	/* Redraw status bar */
	p_ptr->redraw |= (PR_STATUS);
}

/*!
* @brief �̂̊e�������s��
* @param spell ��ID
* @param mode �������e (SPELL_NAME / SPELL_DESC / SPELL_INFO / SPELL_CAST / SPELL_FAIL / SPELL_CONT / SPELL_STOP)
* @return SPELL_NAME / SPELL_DESC / SPELL_INFO ���ɂ͕�����|�C���^��Ԃ��BSPELL_CAST / SPELL_FAIL / SPELL_CONT / SPELL_STOP ����NULL�������Ԃ��B
*/
cptr do_music_spell(SPELL_IDX spell, BIT_FLAGS mode)
{
	bool name = (mode == SPELL_NAME) ? TRUE : FALSE;
	bool desc = (mode == SPELL_DESC) ? TRUE : FALSE;
	bool info = (mode == SPELL_INFO) ? TRUE : FALSE;
	bool cast = (mode == SPELL_CAST) ? TRUE : FALSE;
	bool fail = (mode == SPELL_FAIL) ? TRUE : FALSE;
	bool cont = (mode == SPELL_CONT) ? TRUE : FALSE;
	bool stop = (mode == SPELL_STOP) ? TRUE : FALSE;
	static const char s_dam[] = _("����:", "dam ");

	int dir;
	int plev = p_ptr->lev;

	switch (spell)
	{
	case 0:
		if (name) return _("�x�݂̉�", "Song of Holding");
		if (desc) return _("���E���̑S�Ẵ����X�^�[������������B��R�����Ɩ����B", "Attempts to slow all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�������Ƃ��������f�B���������ݎn�߂��D�D�D", "You start humming a slow, steady melody..."));
			start_singing(spell, MUSIC_SLOW);
		}

		{
			int power = plev;

			if (info) return info_power(power);

			if (cont)
			{
				slow_monsters(plev);
			}
		}
		break;

	case 1:
		if (name) return _("�j���̉�", "Song of Blessing");
		if (desc) return _("��������AC�̃{�[�i�X�𓾂�B", "Gives bonus to hit and AC for a few turns.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�����ȃ����f�B��t�Ŏn�߂��D�D�D", "The holy power of the Music of the Ainur enters you..."));
			start_singing(spell, MUSIC_BLESS);
		}

		if (stop)
		{
			if (!p_ptr->blessed)
			{
				msg_print(_("�����ȋC���������������B", "The prayer has expired."));
			}
		}

		break;

	case 2:
		if (name) return _("����̉��F", "Wrecking Note");
		if (desc) return _("�����̃{���g����B", "Fires a bolt of sound.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		{
			int dice = 4 + (plev - 1) / 5;
			int sides = 4;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_bolt(GF_SOUND, dir, damroll(dice, sides));
			}
		}
		break;

	case 3:
		if (name) return _("�N�O�̐���", "Stun Pattern");
		if (desc) return _("���E���̑S�Ẵ����X�^�[��N�O������B��R�����Ɩ����B", "Attempts to stun all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("Ῐf�����郁���f�B��t�Ŏn�߂��D�D�D", "You weave a pattern of sounds to bewilder and daze..."));
			start_singing(spell, MUSIC_STUN);
		}

		{
			int dice = plev / 10;
			int sides = 2;

			if (info) return info_power_dice(dice, sides);

			if (cont)
			{
				stun_monsters(damroll(dice, sides));
			}
		}

		break;

	case 4:
		if (name) return _("�����̗���", "Flow of Life");
		if (desc) return _("�̗͂������񕜂�����B", "Heals HP a little.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�̂�ʂ��đ̂Ɋ��C���߂��Ă����D�D�D", "Life flows through you as you sing a song of healing..."));
			start_singing(spell, MUSIC_L_LIFE);
		}

		{
			int dice = 2;
			int sides = 6;

			if (info) return info_heal(dice, sides, 0);

			if (cont)
			{
				hp_player(damroll(dice, sides));
			}
		}

		break;

	case 5:
		if (name) return _("���z�̉�", "Song of the Sun");
		if (desc) return _("�������Ƃ炵�Ă���͈͂������S�̂��i�v�ɖ��邭����B", "Lights up nearby area and the inside of a room permanently.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		{
			int dice = 2;
			int sides = plev / 2;
			int rad = plev / 10 + 1;

			if (info) return info_damage(dice, sides, 0);

			if (cast)
			{
				msg_print(_("����P���̂��ӂ���Ƃ炵���B", "Your uplifting song brings brightness to dark places..."));
				lite_area(damroll(dice, sides), rad);
			}
		}
		break;

	case 6:
		if (name) return _("���|�̉�", "Song of Fear");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�����|������B��R�����Ɩ����B", "Attempts to scare all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���ǂ남�ǂ낵�������f�B��t�Ŏn�߂��D�D�D", "You start weaving a fearful pattern..."));
			start_singing(spell, MUSIC_FEAR);
		}

		{
			int power = plev;

			if (info) return info_power(power);

			if (cont)
			{
				project_hack(GF_TURN_ALL, power);
			}
		}

		break;

	case 7:
		if (name) return _("�킢�̉�", "Heroic Ballad");
		if (desc) return _("�q�[���[�C���ɂȂ�B", "Removes fear, and gives bonus to hit and 10 more HP for a while.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�������킢�̉̂��̂����D�D�D", "You start singing a song of intense fighting..."));

			(void)hp_player(10);
			(void)set_afraid(0);

			/* Recalculate hitpoints */
			p_ptr->update |= (PU_HP);

			start_singing(spell, MUSIC_HERO);
		}

		if (stop)
		{
			if (!p_ptr->hero)
			{
				msg_print(_("�q�[���[�̋C���������������B", "The heroism wears off."));
				/* Recalculate hitpoints */
				p_ptr->update |= (PU_HP);
			}
		}

		break;

	case 8:
		if (name) return _("��I�m�o", "Clairaudience");
		if (desc) return _("�߂����/��/�K�i�����m����B���x��15�őS�Ẵ����X�^�[�A20�ō���ƃA�C�e�������m�ł���悤�ɂȂ�B���x��25�Ŏ��ӂ̒n�`�����m���A40�ł��̊K�S�̂��i�v�ɏƂ炵�A�_���W�������̂��ׂẴA�C�e�������m����B���̌��ʂ͉̂������邱�Ƃŏ��ɋN����B",
			"Detects traps, doors and stairs in your vicinity. And detects all monsters at level 15, treasures and items at level 20. Maps nearby area at level 25. Lights and know the whole level at level 40. These effects occurs by turns while this song continues.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�Â��ȉ��y�����o���������܂������D�D�D", "Your quiet music sharpens your sense of hearing..."));
			/* Hack -- Initialize the turn count */
			SINGING_COUNT(p_ptr) = 0;
			start_singing(spell, MUSIC_DETECT);
		}

		{
			int rad = DETECT_RAD_DEFAULT;

			if (info) return info_radius(rad);

			if (cont)
			{
				int count = SINGING_COUNT(p_ptr);

				if (count >= 19) wiz_lite(FALSE);
				if (count >= 11)
				{
					map_area(rad);
					if (plev > 39 && count < 19)
						SINGING_COUNT(p_ptr) = count + 1;
				}
				if (count >= 6)
				{
					/* There are too many hidden treasure.  So... */
					/* detect_treasure(rad); */
					detect_objects_gold(rad);
					detect_objects_normal(rad);

					if (plev > 24 && count < 11)
						SINGING_COUNT(p_ptr) = count + 1;
				}
				if (count >= 3)
				{
					detect_monsters_invis(rad);
					detect_monsters_normal(rad);

					if (plev > 19 && count < 6)
						SINGING_COUNT(p_ptr) = count + 1;
				}
				detect_traps(rad, TRUE);
				detect_doors(rad);
				detect_stairs(rad);

				if (plev > 14 && count < 3)
					SINGING_COUNT(p_ptr) = count + 1;
			}
		}

		break;

	case 9:
		if (name) return _("���̉�", "Soul Shriek");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�ɑ΂��Đ��_�U�����s���B", "Damages all monsters in sight with PSI damages.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���_��P���Ȃ���̂��̂����D�D�D", "You start singing a song of soul in pain..."));
			start_singing(spell, MUSIC_PSI);
		}

		{
			int dice = 1;
			int sides = plev * 3 / 2;

			if (info) return info_damage(dice, sides, 0);

			if (cont)
			{
				project_hack(GF_PSI, damroll(dice, sides));
			}
		}

		break;

	case 10:
		if (name) return _("�m���̉�", "Song of Lore");
		if (desc) return _("�����̂���}�X�Ɨׂ�̃}�X�ɗ����Ă���A�C�e�����Ӓ肷��B", "Identifies all items which are in the adjacent squares.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���̐��E�̒m�������ꍞ��ł����D�D�D", "You recall the rich lore of the world..."));
			start_singing(spell, MUSIC_ID);
		}

		{
			int rad = 1;

			if (info) return info_radius(rad);

			/*
			* �̂̊J�n���ɂ����ʔ����F
			* MP�s���ŊӒ肪���������O�ɉ̂����f���Ă��܂��̂�h�~�B
			*/
			if (cont || cast)
			{
				project(0, rad, p_ptr->y, p_ptr->x, 0, GF_IDENTIFY, PROJECT_ITEM, -1);
			}
		}

		break;

	case 11:
		if (name) return _("�B�ق̉�", "Hiding Tune");
		if (desc) return _("�B���s���\�͂��㏸������B", "Gives improved stealth.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���Ȃ��̎p���i�F�ɂƂ�����ł������D�D�D", "Your song carries you beyond the sight of mortal eyes..."));
			start_singing(spell, MUSIC_STEALTH);
		}

		if (stop)
		{
			if (!p_ptr->tim_stealth)
			{
				msg_print(_("�p���͂�����ƌ�����悤�ɂȂ����B", "You are no longer hided."));
			}
		}

		break;

	case 12:
		if (name) return _("���e�̐���", "Illusion Pattern");
		if (desc) return _("���E���̑S�Ẵ����X�^�[������������B��R�����Ɩ����B", "Attempts to confuse all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�ӂ��ʂɌ��e�����ꂽ�D�D�D", "You weave a pattern of sounds to beguile and confuse..."));
			start_singing(spell, MUSIC_CONF);
		}

		{
			int power = plev * 2;

			if (info) return info_power(power);

			if (cont)
			{
				confuse_monsters(power);
			}
		}

		break;

	case 13:
		if (name) return _("�j�ł̋���", "Doomcall");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�ɑ΂��č����U�����s���B", "Damages all monsters in sight with booming sound.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�������������D�D�D", "The fury of the Downfall of Numenor lashes out..."));
			start_singing(spell, MUSIC_SOUND);
		}

		{
			int dice = 10 + plev / 5;
			int sides = 7;

			if (info) return info_damage(dice, sides, 0);

			if (cont)
			{
				project_hack(GF_SOUND, damroll(dice, sides));
			}
		}

		break;

	case 14:
		if (name) return _("�t�B���G���̉�", "Firiel's Song");
		if (desc) return _("���͂̎��̂⍜�𐶂��Ԃ��B", "Resurrects nearby corpse and skeletons. And makes these your pets.");

		{
			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�����ƕ����̃e�[�}��t�Ŏn�߂��D�D�D", "The themes of life and revival are woven into your song..."));
				animate_dead(0, p_ptr->y, p_ptr->x);
			}
		}
		break;

	case 15:
		if (name) return _("���̒���", "Fellowship Chant");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�𖣗�����B��R�����Ɩ����B", "Attempts to charm all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���炩�ȃ����f�B��t�Ŏn�߂��D�D�D", "You weave a slow, soothing melody of imploration..."));
			start_singing(spell, MUSIC_CHARM);
		}

		{
			int dice = 10 + plev / 15;
			int sides = 6;

			if (info) return info_power_dice(dice, sides);

			if (cont)
			{
				charm_monsters(damroll(dice, sides));
			}
		}

		break;

	case 16:
		if (name) return _("�������g", "Sound of disintegration");
		if (desc) return _("�ǂ��@��i�ށB�����̑����̃A�C�e���͏�������B", "Makes you be able to burrow into walls. Objects under your feet evaporate.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���ӂ��郁���f�B��t�Ŏn�߂��D�D�D", "You weave a violent pattern of sounds to break wall."));
			start_singing(spell, MUSIC_WALL);
		}

		{
			/*
			* �̂̊J�n���ɂ����ʔ����F
			* MP�s���Ō��ʂ����������O�ɉ̂����f���Ă��܂��̂�h�~�B
			*/
			if (cont || cast)
			{
				project(0, 0, p_ptr->y, p_ptr->x,
					0, GF_DISINTEGRATE, PROJECT_KILL | PROJECT_ITEM | PROJECT_HIDE, -1);
			}
		}
		break;

	case 17:
		if (name) return _("���f�ϐ�", "Finrod's Resistance");
		if (desc) return _("�_�A�d���A���A��C�A�łɑ΂���ϐ��𓾂�B�����ɂ��ϐ��ɗݐς���B",
			"Gives resistance to fire, cold, electricity, acid and poison. These resistances can be added to which from equipment for more powerful resistances.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("���f�̗͂ɑ΂���E�ς̉̂��̂����B", "You sing a song of perseverance against powers..."));
			start_singing(spell, MUSIC_RESIST);
		}

		if (stop)
		{
			if (!p_ptr->oppose_acid)
			{
				msg_print(_("�_�ւ̑ϐ������ꂽ�C������B", "You feel less resistant to acid."));
			}

			if (!p_ptr->oppose_elec)
			{
				msg_print(_("�d���ւ̑ϐ������ꂽ�C������B", "You feel less resistant to elec."));
			}

			if (!p_ptr->oppose_fire)
			{
				msg_print(_("�΂ւ̑ϐ������ꂽ�C������B", "You feel less resistant to fire."));
			}

			if (!p_ptr->oppose_cold)
			{
				msg_print(_("��C�ւ̑ϐ������ꂽ�C������B", "You feel less resistant to cold."));
			}

			if (!p_ptr->oppose_pois)
			{
				msg_print(_("�łւ̑ϐ������ꂽ�C������B", "You feel less resistant to pois."));
			}
		}

		break;

	case 18:
		if (name) return _("�z�r�b�g�̃����f�B", "Hobbit Melodies");
		if (desc) return _("��������B", "Hastes you.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�y���ȉ̂��������ݎn�߂��D�D�D", "You start singing joyful pop song..."));
			start_singing(spell, MUSIC_SPEED);
		}

		if (stop)
		{
			if (!p_ptr->fast)
			{
				msg_print(_("�����̑f�������Ȃ��Ȃ����悤���B", "You feel yourself slow down."));
			}
		}

		break;

	case 19:
		if (name) return _("�c�񂾐��E", "World Contortion");
		if (desc) return _("�߂��̃����X�^�[���e���|�[�g������B��R�����Ɩ����B", "Teleports all nearby monsters away unless resisted.");

		{
			int rad = plev / 15 + 1;
			int power = plev * 3 + 1;

			if (info) return info_radius(rad);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�̂���Ԃ�c�߂��D�D�D", "Reality whirls wildly as you sing a dizzying melody..."));
				project(0, rad, p_ptr->y, p_ptr->x, power, GF_AWAY_ALL, PROJECT_KILL, -1);
			}
		}
		break;

	case 20:
		if (name) return _("�ގU�̉�", "Dispelling chant");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�Ƀ_���[�W��^����B�׈��ȃ����X�^�[�ɓ��ɑ傫�ȃ_���[�W��^����B",
			"Damages all monsters in sight. Hurts evil monsters greatly.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�ς����Ȃ��s���a�����G��ӂߗ��Ă��D�D�D", "You cry out in an ear-wracking voice..."));
			start_singing(spell, MUSIC_DISPEL);
		}

		{
			int m_sides = plev * 3;
			int e_sides = plev * 3;

			if (info) return format("%s1d%d+1d%d", s_dam, m_sides, e_sides);

			if (cont)
			{
				dispel_monsters(randint1(m_sides));
				dispel_evil(randint1(e_sides));
			}
		}
		break;

	case 21:
		if (name) return _("�T���}���̊Ì�", "The Voice of Saruman");
		if (desc) return _("���E���̑S�Ẵ����X�^�[�����������A���点�悤�Ƃ���B��R�����Ɩ����B", "Attempts to slow and sleep all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�D�����A���͓I�ȉ̂��������ݎn�߂��D�D�D", "You start humming a gentle and attractive song..."));
			start_singing(spell, MUSIC_SARUMAN);
		}

		{
			int power = plev;

			if (info) return info_power(power);

			if (cont)
			{
				slow_monsters(plev);
				sleep_monsters(plev);
			}
		}

		break;

	case 22:
		if (name) return _("���̉��F", "Song of the Tempest");
		if (desc) return _("�����̃r�[������B", "Fires a beam of sound.");

		{
			int dice = 15 + (plev - 1) / 2;
			int sides = 10;

			if (info) return info_damage(dice, sides, 0);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_beam(GF_SOUND, dir, damroll(dice, sides));
			}
		}
		break;

	case 23:
		if (name) return _("������̐��E", "Ambarkanta");
		if (desc) return _("���݂̊K���č\������B", "Recreates current dungeon level.");

		{
			int base = 15;
			int sides = 20;

			if (info) return info_delay(base, sides);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("���͂��ω����n�߂��D�D�D", "You sing of the primeval shaping of Middle-earth..."));
				alter_reality();
			}
		}
		break;

	case 24:
		if (name) return _("�j��̐���", "Wrecking Pattern");
		if (desc) return _("���͂̃_���W������h�炵�A�ǂƏ��������_���ɓ���ς���B",
			"Shakes dungeon structure, and results in random swapping of floors and walls.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�j��I�ȉ̂������킽�����D�D�D", "You weave a pattern of sounds to contort and shatter..."));
			start_singing(spell, MUSIC_QUAKE);
		}

		{
			int rad = 10;

			if (info) return info_radius(rad);

			if (cont)
			{
				earthquake(p_ptr->y, p_ptr->x, 10);
			}
		}

		break;


	case 25:
		if (name) return _("��؂̉�", "Stationary Shriek");
		if (desc) return _("���E���̑S�Ẵ����X�^�[��Ⴢ����悤�Ƃ���B��R�����Ɩ����B", "Attempts to freeze all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�������Ƃ��������f�B��t�Ŏn�߂��D�D�D", "You weave a very slow pattern which is almost likely to stop..."));
			start_singing(spell, MUSIC_STASIS);
		}

		{
			int power = plev * 4;

			if (info) return info_power(power);

			if (cont)
			{
				stasis_monsters(power);
			}
		}

		break;

	case 26:
		if (name) return _("���̉�", "Endurance");
		if (desc) return _("�����̂��鏰�̏�ɁA�����X�^�[���ʂ蔲�����菢�����ꂽ�肷�邱�Ƃ��ł��Ȃ��Ȃ郋�[����`���B",
			"Sets a glyph on the floor beneath you. Monsters cannot attack you if you are on a glyph, but can try to break glyph.");

		{
			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�̂��_���ȏ�����o�����D�D�D", "The holy power of the Music is creating sacred field..."));
				warding_glyph();
			}
		}
		break;

	case 27:
		if (name) return _("�p�Y�̎�", "The Hero's Poem");
		if (desc) return _("�������A�q�[���[�C���ɂȂ�A���E���̑S�Ẵ����X�^�[�Ƀ_���[�W��^����B",
			"Hastes you. Gives heroism. Damages all monsters in sight.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�p�Y�̉̂��������񂾁D�D�D", "You chant a powerful, heroic call to arms..."));
			(void)hp_player(10);
			(void)set_afraid(0);

			/* Recalculate hitpoints */
			p_ptr->update |= (PU_HP);

			start_singing(spell, MUSIC_SHERO);
		}

		if (stop)
		{
			if (!p_ptr->hero)
			{
				msg_print(_("�q�[���[�̋C���������������B", "The heroism wears off."));
				/* Recalculate hitpoints */
				p_ptr->update |= (PU_HP);
			}

			if (!p_ptr->fast)
			{
				msg_print(_("�����̑f�������Ȃ��Ȃ����悤���B", "You feel yourself slow down."));
			}
		}

		{
			int dice = 1;
			int sides = plev * 3;

			if (info) return info_damage(dice, sides, 0);

			if (cont)
			{
				dispel_monsters(damroll(dice, sides));
			}
		}
		break;

	case 28:
		if (name) return _("�����@���i�̏���", "Relief of Yavanna");
		if (desc) return _("���͂ȉ񕜂̉̂ŁA�����ƞN�O��Ԃ��S������B", "Powerful healing song. Also heals cut and stun completely.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�̂�ʂ��đ̂Ɋ��C���߂��Ă����D�D�D", "Life flows through you as you sing the song..."));
			start_singing(spell, MUSIC_H_LIFE);
		}

		{
			int dice = 15;
			int sides = 10;

			if (info) return info_heal(dice, sides, 0);

			if (cont)
			{
				hp_player(damroll(dice, sides));
				set_stun(0);
				set_cut(0);
			}
		}

		break;

	case 29:
		if (name) return _("�Đ��̉�", "Goddess' rebirth");
		if (desc) return _("���ׂẴX�e�[�^�X�ƌo���l���񕜂���B", "Restores all stats and experience.");

		{
			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				msg_print(_("�Í��̒��Ɍ��Ɣ����ӂ�܂����B�̂����̊��͂����߂����B",
					"You strewed light and beauty in the dark as you sing. You feel refreshed."));
				(void)do_res_stat(A_STR);
				(void)do_res_stat(A_INT);
				(void)do_res_stat(A_WIS);
				(void)do_res_stat(A_DEX);
				(void)do_res_stat(A_CON);
				(void)do_res_stat(A_CHR);
				(void)restore_level();
			}
		}
		break;

	case 30:
		if (name) return _("�T�E�����̖��p", "Wizardry of Sauron");
		if (desc) return _("���ɋ��͂ł��������������̋�����B", "Fires an extremely powerful tiny ball of sound.");

		{
			int dice = 50 + plev;
			int sides = 10;
			int rad = 0;

			if (info) return info_damage(dice, sides, 0);

			/* Stop singing before start another */
			if (cast || fail) stop_singing();

			if (cast)
			{
				if (!get_aim_dir(&dir)) return NULL;

				fire_ball(GF_SOUND, dir, damroll(dice, sides), rad);
			}
		}
		break;

	case 31:
		if (name) return _("�t�B���S���t�B���̒���", "Fingolfin's Challenge");
		if (desc) return _("�_���[�W���󂯂Ȃ��Ȃ�o���A�𒣂�B",
			"Generates barrier which completely protect you from almost all damages. Takes a few your turns when the barrier breaks.");

		/* Stop singing before start another */
		if (cast || fail) stop_singing();

		if (cast)
		{
			msg_print(_("�t�B���S���t�B���̖����ւ̒�����̂����D�D�D",
				"You recall the valor of Fingolfin's challenge to the Dark Lord..."));

			/* Redraw map */
			p_ptr->redraw |= (PR_MAP);

			/* Update monsters */
			p_ptr->update |= (PU_MONSTERS);

			/* Window stuff */
			p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);

			start_singing(spell, MUSIC_INVULN);
		}

		if (stop)
		{
			if (!p_ptr->invuln)
			{
				msg_print(_("���G�ł͂Ȃ��Ȃ����B", "The invulnerability wears off."));
				/* Redraw map */
				p_ptr->redraw |= (PR_MAP);

				/* Update monsters */
				p_ptr->update |= (PU_MONSTERS);

				/* Window stuff */
				p_ptr->window |= (PW_OVERHEAD | PW_DUNGEON);
			}
		}

		break;
	}

	return "";
}

