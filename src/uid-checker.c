#include "uid-checker.h"
#ifdef SET_UID
# ifdef SAFE_SETUID
#  ifdef SAFE_SETUID_POSIX
#   include "util.h"
#  endif
#  endif
#endif

/*!
 * @brief �t�@�C���̃h���b�v�p�[�~�b�V�����`�F�b�N / Hack -- drop permissions
 */
void safe_setuid_drop(void)
{
#ifdef SET_UID
# ifdef SAFE_SETUID
#  ifdef SAFE_SETUID_POSIX

	if (setuid(getuid()) != 0)
	{
		quit(_("setuid(): �������������܂���I", "setuid(): cannot set permissions correctly!"));
	}
	if (setgid(getgid()) != 0)
	{
		quit(_("setgid(): �������������܂���I", "setgid(): cannot set permissions correctly!"));
	}
#   else
	if (setreuid(geteuid(), getuid()) != 0)
	{
		quit(_("setreuid(): �������������܂���I", "setreuid(): cannot set permissions correctly!"));
	}
	if (setregid(getegid(), getgid()) != 0)
	{
		quit(_("setregid(): �������������܂���I", "setregid(): cannot set permissions correctly!"));
	}
#  endif
# endif
#endif
}


/*!
 * @brief �t�@�C���̃O���u�p�[�~�b�V�����`�F�b�N / Hack -- grab permissions
 */
void safe_setuid_grab(void)
{
#ifdef SET_UID
# ifdef SAFE_SETUID
#  ifdef SAFE_SETUID_POSIX

	if (setuid(p_ptr->player_egid) != 0)
	{
		quit(_("setuid(): �������������܂���I", "setuid(): cannot set permissions correctly!"));
	}
	if (setgid(p_ptr->player_egid) != 0)
	{
		quit(_("setgid(): �������������܂���I", "setgid(): cannot set permissions correctly!"));
	}
#  else
	if (setreuid(geteuid(), getuid()) != 0)
	{
		quit(_("setreuid(): �������������܂���I", "setreuid(): cannot set permissions correctly!"));
	}
	if (setregid(getegid(), getgid()) != 0)
	{
		quit(_("setregid(): �������������܂���I", "setregid(): cannot set permissions correctly!"));
	}
#  endif
# endif
#endif
}
