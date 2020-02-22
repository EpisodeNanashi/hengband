#include "h-type.h"
#include "horror-descriptions.h"

/*!
 * @var horror_desc
 * @brief ELDRITCH HORROR���ʎ��̃����X�^�[�̌`�e���b�Z�[�W�i�ʏ펞�j
 */
concptr horror_desc[MAX_SAN_HORROR] =
{
#ifdef JP
	"���܂킵��",
	"��m���",
	"�����Ƃ���",
	"�j�œI��",
	"�`���I��",

	"�����",
	"���낵��",
	"�s����",
	"�e�͂̂Ȃ�",
	"�����܂���",

	"�n����",
	"�g�̖т��悾��",
	"�n����",
	"���܂킵��",
	"�����̂悤��",

	"������������",
	"���������",
	"����",
	"�s���",
	"�����������܂���",
#else
	"abominable",
	"abysmal",
	"appalling",
	"baleful",
	"blasphemous",

	"disgusting",
	"dreadful",
	"filthy",
	"grisly",
	"hideous",

	"hellish",
	"horrible",
	"infernal",
	"loathsome",
	"nightmarish",

	"repulsive",
	"sacrilegious",
	"terrible",
	"unclean",
	"unspeakable",
#endif
};

/*!
 * @var funny_desc
 * @brief ELDRITCH HORROR���ʎ��̃����X�^�[�̌`�e���b�Z�[�W(���o��Ԏ�)
 */
concptr funny_desc[MAX_SAN_FUNNY] =
{
#ifdef JP
	"�Ԕ�����",
	"���m��",
	"�΂��炵��",
	"����������",
	"�n������",

	"�΂���",
	"�΂��΂�����",
	"�Ԃ��Ƃ�",
	"��������",
	"�|�X�g���_����",

	"�t�@���^�X�e�B�b�N��",
	"�_�_�C�Y���I��",
	"�L���r�Y���I��",
	"�F���I��",
	"��z����",

	"����s�\��",
	"���̂�����",
	"�����ׂ�",
	"�M�����Ȃ�",
	"�J�I�e�B�b�N��",

	"�쐫�I��",
	"��펯��",
#else
	"silly",
	"hilarious",
	"absurd",
	"insipid",
	"ridiculous",

	"laughable",
	"ludicrous",
	"far-out",
	"groovy",
	"postmodern",

	"fantastic",
	"dadaistic",
	"cubistic",
	"cosmic",
	"awesome",

	"incomprehensible",
	"fabulous",
	"amazing",
	"incredible",
	"chaotic",

	"wild",
	"preposterous",
#endif
};

/*!
 * @var funny_comments
 * @brief ELDRITCH HORROR���ʎ��̌��o���ԉ��������������\��
 */
concptr funny_comments[MAX_SAN_COMMENT] =
{
#ifdef JP
	  "�ō������I",
	  "���Ђ�[�I",
	  "���������I",
	  "����΂炵���I",
	  "�Ԃ��Ƃс[�I"
  #else
	  "Wow, cosmic, man!",
	  "Rad!",
	  "Groovy!",
	  "Cool!",
	  "Far out!"
  #endif
};
