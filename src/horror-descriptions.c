/*!
 * @file horror-descriptions.c
 * @brief �G���h���b�`�z���[�̌`�e���e�[�u����` / Definitions of adjectives on 
 * @date 2020/02/21
 * @author Hourier
 * @deitals
 * �����瑝�₵�Ă��悢���Ahorror_desc_evil ��horror_desc_neutral�̌��͓���ɂ��邱��
 */

#include "angband.h"
#include "horror-descriptions.h"

/*!
 * @var horror_desc_common
 * @brief ELDRITCH HORROR���ʎ��̃����X�^�[�̌`�e���b�Z�[�W (���펞�A�׈��E��������)
 */
concptr horror_desc_common[MAX_SAN_HORROR_COMMON] =
{
#ifdef JP
	"��m���",
	"�j�œI��",
	"�e�͂̂Ȃ�",
	"�����̂悤��",
	"���O�����ɂł��Ȃ�",
#else
	"abysmal",
	"baleful",
	"grisly",
	"nightmarish",
	"unspeakable",
#endif
};

/*!
 * @var horror_desc_evil
 * @brief ELDRITCH HORROR���ʎ��̃����X�^�[�̌`�e���b�Z�[�W (���펞�A�׈�)
 */
concptr horror_desc_evil[MAX_SAN_HORROR_EVIL] =
{
#ifdef JP
	"���܂킵��",
	"�����Ƃ���",
	"�`���I��",
	"�����",
	"���낵��",

	"�s����",
	"�����܂���",
	"�񓹂Ȃ�",
	"�g�̖т��悾��",
	"�n����",

	"�ނ��ނ�����",
	"������������",
	"���������",
	"����",
	"�s���",
#else
	"abominable",
	"appalling",
	"blasphemous",
	"disgusting",
	"dreadful",

	"filthy",
	"hideous",
	"hellish",
	"horrible",
	"infernal",

	"loathsome",
	"repulsive",
	"sacrilegious",
	"terrible",
	"unclean",
#endif
};

/*
 * @var horror_desc_neutral
 * @brief ELDRITCH HORROR���ʎ��̃����X�^�[�̌`�e���b�Z�[�W (���펞�A����)
 */
concptr horror_desc_neutral[MAX_SAN_HORROR_NEUTRAL] =
{
#ifdef JP
	"�傢�Ȃ�ؕ|�ɕ�܂ꂽ",
	"�d�����ȏ΂݂�",
	"����͂��̂Ȃ�",
	"�z�����܂ꂻ����",
	"�܊��𒴉z����",

	"�ʎ����ɕ����яオ����",
	"���o�ƌ������悤��",
	"�����Ɨh��߂���",
	"����̎��݂��^���قǈ��|�I��",
	"�����ł��Ȃ�",

	"�T�C�P�f���b�N��",
	"���C���z�������悤��",
	"�g���b�v���ɖ�����",
	"�����^�����ɂȂ�悤��",
	"�����ɉ�������",
#else
	"causing great awe",
	"laughing insanely",
	"existence, that SHOULD NOT be there, ",
	"sucking",
	"transcending senses",

	"rising on the another dimension",
	"hallucinating",
	"swaying and swaying",
	"overwhelming enough to doubt your own existence",
	"unintelligible",

	"psychedelic",
	"spoiling your life",
	"trippy",
	"going blank",
	"unlimitedly accelerated",
#endif
};

/*!
 * @var funny_desc
 * @brief ELDRITCH HORROR���ʎ��̃����X�^�[�̌`�e���b�Z�[�W (���o��Ԏ�)
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
