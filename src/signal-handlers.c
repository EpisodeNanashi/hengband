/*!
 * @file signal-handlers.c
 * @brief �V�O�i���n���h���̊Ǘ� / Controlling signal handlers
 * @date 2020/02/23
 * @author Hourier
 */

#include "signal-handlers.h"
#include "core.h"
#include "save.h"
#include "world.h"
#include "term.h"
#include "cmd/cmd-dump.h"
#include "floor-events.h"

s16b signal_count;		/* Hack -- Count interupts */

#ifdef HANDLE_SIGNALS

#include <signal.h>

/*!
 * @brief OS����̃V�O�i�����󂯂ăT�X�y���h��Ԃɓ��� /
 * Handle signals -- suspend
 * @param sig �󂯎�����V�O�i��
 * @details
 * Actually suspend the game, and then resume cleanly
 */
static void handle_signal_suspend(int sig)
{
	(void)signal(sig, SIG_IGN);
#ifdef SIGSTOP
	Term_fresh();
	Term_xtra(TERM_XTRA_ALIVE, 0);
	(void)kill(0, SIGSTOP);
	Term_xtra(TERM_XTRA_ALIVE, 1);
	Term_redraw();
	Term_fresh();
#endif
	(void)signal(sig, handle_signal_suspend);
}


/*!
 * todo ������player_type��ǉ�����Ɗ֐��|�C���^����̎��E�����Ȃ��Ȃ�̂ŕۗ�
 * @brief OS����̃V�O�i�����󂯂Ē��f�A�I������ /
 * Handle signals -- simple (interrupt and quit)
 * @param sig �󂯎�����V�O�i��
 * @details
 * <pre>
 * This function was causing a *huge* number of problems, so it has
 * been simplified greatly.  We keep a global variable which counts
 * the number of times the user attempts to kill the process, and
 * we commit suicide if the user does this a certain number of times.
 * We attempt to give "feedback" to the user as he approaches the
 * suicide thresh-hold, but without penalizing accidental keypresses.
 * To prevent messy accidents, we should reset this global variable
 * whenever the user enters a keypress, or something like that.
 * </pre>
 */
static void handle_signal_simple(int sig)
{
	(void)signal(sig, SIG_IGN);
	if (!current_world_ptr->character_generated || current_world_ptr->character_saved)
		quit(NULL);

	signal_count++;
	if (p_ptr->is_dead)
	{
		(void)strcpy(p_ptr->died_from, _("�����I��", "Abortion"));
		forget_lite(p_ptr->current_floor_ptr);
		forget_view(p_ptr->current_floor_ptr);
		clear_mon_lite(p_ptr->current_floor_ptr);
		close_game(p_ptr);
		quit(_("�����I��", "interrupt"));
	}
	else if (signal_count >= 5)
	{
		(void)strcpy(p_ptr->died_from, _("�����I����", "Interrupting"));
		forget_lite(p_ptr->current_floor_ptr);
		forget_view(p_ptr->current_floor_ptr);
		clear_mon_lite(p_ptr->current_floor_ptr);
		p_ptr->playing = FALSE;
		p_ptr->is_dead = TRUE;
		p_ptr->leaving = TRUE;
		close_game(p_ptr);
		quit(_("�����I��", "interrupt"));
	}
	else if (signal_count >= 4)
	{
		Term_xtra(TERM_XTRA_NOISE, 0);
		Term_erase(0, 0, 255);
		Term_putstr(0, 0, -1, TERM_WHITE, _("�n���̏�̎��E�I", "Contemplating suicide!"));
		Term_fresh();
	}
	else if (signal_count >= 2)
	{
		Term_xtra(TERM_XTRA_NOISE, 0);
	}

	(void)signal(sig, handle_signal_simple);
}


/*!
 * todo ������p_ptr��ǉ�����Ɗ֐��|�C���^����̎��E�����Ȃ��Ȃ�̂ŕۗ�
 * @brief OS����̃V�O�i�����󂯂ċ����I������ /
 * Handle signal -- abort, kill, etc
 * @param sig �󂯎�����V�O�i��
 * @return �Ȃ�
 * @details
 * <pre>
 * This function was causing a *huge* number of problems, so it has
 * been simplified greatly.  We keep a global variable which counts
 * the number of times the user attempts to kill the process, and
 * we commit suicide if the user does this a certain number of times.
 * We attempt to give "feedback" to the user as he approaches the
 * suicide thresh-hold, but without penalizing accidental keypresses.
 * To prevent messy accidents, we should reset this global variable
 * whenever the user enters a keypress, or something like that.
 * </pre>
 */
static void handle_signal_abort(int sig)
{
	int wid, hgt;
	Term_get_size(&wid, &hgt);

	(void)signal(sig, SIG_IGN);
	if (!current_world_ptr->character_generated || current_world_ptr->character_saved) quit(NULL);

	forget_lite(p_ptr->current_floor_ptr);
	forget_view(p_ptr->current_floor_ptr);
	clear_mon_lite(p_ptr->current_floor_ptr);

	Term_erase(0, hgt - 1, 255);
	Term_putstr(0, hgt - 1, -1, TERM_RED,
		_("���낵���\�t�g�̃o�O����т������Ă����I", "A gruesome software bug LEAPS out at you!"));

	Term_putstr(45, hgt - 1, -1, TERM_RED, _("�ً}�Z�[�u...", "Panic save..."));

	exe_write_diary(p_ptr, DIARY_GAMESTART, 0, _("----�Q�[���ُ�I��----", "---- Panic Save and Abort Game ----"));
	Term_fresh();

	p_ptr->panic_save = 1;
	(void)strcpy(p_ptr->died_from, _("(�ً}�Z�[�u)", "(panic save)"));

	signals_ignore_tstp();

	if (save_player(p_ptr))
	{
		Term_putstr(45, hgt - 1, -1, TERM_RED, _("�ً}�Z�[�u�����I", "Panic save succeeded!"));
	}
	else
	{
		Term_putstr(45, hgt - 1, -1, TERM_RED, _("�ً}�Z�[�u���s�I", "Panic save failed!"));
	}

	Term_fresh();
	quit(_("�\�t�g�̃o�O", "software bug"));
}


/*!
 * @brief OS�����SIGTSTP�V�O�i���𖳎�����֐� /
 * Ignore SIGTSTP signals (keyboard suspend)
 * @return �Ȃ�
 * @details
 */
void signals_ignore_tstp(void)
{
#ifdef SIGTSTP
	(void)signal(SIGTSTP, SIG_IGN);
#endif
}


/*!
 * @brief OS�����SIGTSTP�V�O�i���n���h�� /
 * Handle SIGTSTP signals (keyboard suspend)
 * @return �Ȃ�
 * @details
 */
void signals_handle_tstp(void)
{
#ifdef SIGTSTP
	(void)signal(SIGTSTP, handle_signal_suspend);
#endif
}


/*!
 * @brief OS����̃V�O�i���n���h�������������� /
 * Prepare to handle the relevant signals
 * @return �Ȃ�
 * @details
 */
void signals_init(void)
{
#ifdef SIGHUP
	(void)signal(SIGHUP, SIG_IGN);
#endif

#ifdef SIGTSTP
	(void)signal(SIGTSTP, handle_signal_suspend);
#endif

#ifdef SIGINT
	(void)signal(SIGINT, handle_signal_simple);
#endif

#ifdef SIGQUIT
	(void)signal(SIGQUIT, handle_signal_simple);
#endif

#ifdef SIGFPE
	(void)signal(SIGFPE, handle_signal_abort);
#endif

#ifdef SIGILL
	(void)signal(SIGILL, handle_signal_abort);
#endif

#ifdef SIGTRAP
	(void)signal(SIGTRAP, handle_signal_abort);
#endif

#ifdef SIGIOT
	(void)signal(SIGIOT, handle_signal_abort);
#endif

#ifdef SIGKILL
	(void)signal(SIGKILL, handle_signal_abort);
#endif

#ifdef SIGBUS
	(void)signal(SIGBUS, handle_signal_abort);
#endif

#ifdef SIGSEGV
	(void)signal(SIGSEGV, handle_signal_abort);
#endif

#ifdef SIGTERM
	(void)signal(SIGTERM, handle_signal_abort);
#endif

#ifdef SIGPIPE
	(void)signal(SIGPIPE, handle_signal_abort);
#endif

#ifdef SIGEMT
	(void)signal(SIGEMT, handle_signal_abort);
#endif

#ifdef SIGDANGER
	(void)signal(SIGDANGER, handle_signal_abort);
#endif

#ifdef SIGSYS
	(void)signal(SIGSYS, handle_signal_abort);
#endif

#ifdef SIGXCPU
	(void)signal(SIGXCPU, handle_signal_abort);
#endif

#ifdef SIGPWR
	(void)signal(SIGPWR, handle_signal_abort);
#endif
}

#else

/*!
 * @brief �_�~�[ /
 * Do nothing
 */
void signals_ignore_tstp(void)
{
}


/*!
 * @brief �_�~�[ /
 * Do nothing
 */
void signals_handle_tstp(void)
{
}


/*!
 * @brief �_�~�[ /
 * Do nothing
 */
void signals_init(void)
{
}
#endif
