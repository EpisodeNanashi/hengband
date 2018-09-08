
/*!
* @brief �x�[�X�A�C�e�������K�w�����Z�����m��
* @details
* There is a 1/10 (10%) chance of inflating the requested object_level
* during the creation of an object (see "get_obj_num()" in "object.c").
* Lower values yield better objects more often.
*/
#define GREAT_OBJ       10

/*!
* @brief �[�w�����X�^�[�����������(NASTY����)�̊�{�m��(1/x)
* @details
* There is a 1/25 (4%) chance of inflating the requested monster_level
* during the creation of a monsters (see "get_mon_num()" in "monster.c").
* Lower values yield harder monsters more often.
*/
#define NASTY_MON_BASE     25
#define NASTY_MON_MAX      3	/*!< �[�w�����X�^�[��1�t���A�ɐ��������ő吔  */
#define NASTY_MON_PLUS_MAX 25	/*!< �[�w�����X�^�[�̊K�w���Z�ő�� */

#define PENETRATE_INVULNERABILITY 13 /*!< ���G�����j����m��(1/x) / 1/x chance of hurting even if invulnerable! */

#define MAX_TELEPORT_DISTANCE 200 /*!< �e���|�[�g�ő勗�� */

/*
* Refueling constants
*/
#define FUEL_TORCH      5000    /*!< �����̊�{�����l / Maximum amount of fuel in a torch */
#define FUEL_LAMP       15000   /*!< �����^���̊�{�����l / Maximum amount of fuel in a lantern */

/*
* More maximum values
*/
#define MAX_SIGHT       20      /*!< �v���C���[�̍ő压�E�͈�(�}�X) / Maximum view distance */
#define MAX_RANGE       (p_ptr->inside_battle ? 36 : 18)      /*!< �v���C���[�̍U���˒�(�}�X) / Maximum range (spells, etc) */
#define AAF_LIMIT       100     /*!< �����X�^�[�̌��E���m�͈�(�}�X) Limit of sensing radius */

#define MIN_M_ALLOC_TD          4 /*!< �X(����)�̍Œ�Z�l�z�u�� / The town starts out with 4 residents during the day */
#define MIN_M_ALLOC_TN          8 /*!< �X(���)�̍Œ�Z�l�z�u�� / The town starts out with 8 residents during the night */

/*!
* @brief �����X�^�[���B�̍ő吔
* @details
* A monster can only "multiply" (reproduce) if there are fewer than 100
* monsters on the level capable of such spontaneous reproduction.  This
* is a hack which prevents the "m_list[]" array from exploding due to
* reproducing monsters.  Messy, but necessary.
*/
#define MAX_REPRO       100

#define MAX_VAMPIRIC_DRAIN 50 /*!< �z�������̍ő��HP */

/*
* Dungeon generation values
*/
#define DUN_UNUSUAL 250 /*!< �ʏ�ł͂Ȃ�����������������{�m��(���x��/�萔) / Level/chance of unusual room (was 200) */
#define DUN_DEST    18  /*!< �j��n�`���t���A�ɔ��������{�m��(1/�萔) / 1/chance of having a destroyed level */
#define SMALL_LEVEL 3   /*!< �������t���A�̐���������{�m��(1/�萔) / 1/chance of smaller size (3) */
#define EMPTY_LEVEL 24  /*!< �A���[�i���x��(�O�ǂ̂Ȃ��t���A)�̐���������{�m��(1/�萔) / 1/chance of being 'empty' (15) */
#define LAKE_LEVEL  24  /*!< ���΂̂���t���A�̐��������m��(1/�萔) / 1/chance of being a lake on the level */
#define DARK_EMPTY  5   /*!< �t���A�S�̂��Â��\���̊�{�m��(1/�萔) / 1/chance of arena level NOT being lit (2) */
#define DUN_CAVERN  20	/*!< ���A��̃_���W����������������{�m��(1/�萔) / 1/chance of having a cavern level */

/*
 * Dungeon tunnel generation values
 */
#define DUN_TUN_RND_MIN	 5 /* Chance of random direction (was 10) */
#define DUN_TUN_RND_MAX	20
#define DUN_TUN_CHG_MIN	20 /* Chance of changing direction (was 30) */
#define DUN_TUN_CHG_MAX	60
#define DUN_TUN_CON_MIN 10 /* Chance of extra tunneling (was 15) */
#define DUN_TUN_CON_MAX	40
#define DUN_TUN_PEN_MIN 30 /* Chance of doors at room entrances (was 25) */
#define DUN_TUN_PEN_MAX 70
#define DUN_TUN_JCT_MIN 60 /* Chance of doors at tunnel junctions (was 90) */
#define DUN_TUN_JCT_MAX 90

/*
 * Dungeon streamer generation values
 */
#define DUN_STR_DEN     5	/* Density of streamers */
#define DUN_STR_RNG     5	/* Width of streamers */
#define DUN_STR_MAG     6	/* Number of magma streamers */
#define DUN_STR_MC     30	/* 1/chance of treasure per magma */
#define DUN_STR_QUA     4	/* Number of quartz streamers */
#define DUN_STR_QC     15	/* 1/chance of treasure per quartz */
#define DUN_STR_WLW     1	/* Width of lava & water streamers -KMW- */
#define DUN_STR_DWLW    8	/* Density of water & lava streams -KMW- */

#define DUN_MOS_DEN	2	/* Density of moss streamers */
#define DUN_MOS_RNG	10	/* Width of moss streamers */
#define DUN_STR_MOS	2	/* Number of moss streamers */
#define DUN_WAT_DEN	15	/* Density of rivers */
#define DUN_WAT_RNG	2	/* Width of rivers */
#define DUN_STR_WAT	3	/* Max number of rivers */
#define DUN_WAT_CHG	50	/* 1 in 50 chance of junction in river */

/*
 * Dungeon treausre allocation values
 */
#define DUN_AMT_ROOM	9	/* Amount of objects for rooms */
#define DUN_AMT_ITEM	3	/* Amount of objects for rooms/corridors */
#define DUN_AMT_GOLD	3	/* Amount of treasure for rooms/corridors */
#define DUN_AMT_INVIS	3	/* Amount of invisible walls for rooms/corridors */

 /* Chance of using syllables to form the name instead of the "template" files */
#define SINDARIN_NAME   10 /*!< �����_���A�[�e�B�t�@�N�g�ɃV���_������������m�� */
#define TABLE_NAME      20 /*!< �����_���A�[�e�B�t�@�N�g�Ɋ�����������m��(���m�ɂ� TABLE_NAME - SINDARIN_NAME %)�ƂȂ� */
#define A_CURSED        13 /*!< 1/n�̊m���Ő����̊����ȊO�̃����_���A�[�e�B�t�@�N�g���􂢂��ɂȂ�B */
#define WEIRD_LUCK      12 /*!< 1/n�̊m����random_resistance()�̏������o�C�A�X�O�̑ϐ������Acreate_artifact��4�𒴂���pval���������B*/
#define BIAS_LUCK       20 /*!< 1/n�̊m����random_resistance()�ŕt�����錳�f�ϐ����Ɖu�ɂȂ� */
#define IM_LUCK         7 /*!< 1/n�̊m����random_resistance()�ŕ����Ɖu�̏����������Ə������ */

 /*! @note
 * Bias luck needs to be higher than weird luck,
 * since it is usually tested several times...
 */

#define ACTIVATION_CHANCE 3 /*!< 1/n�̊m���Ń����_���A�[�e�B�t�@�N�g�ɔ������t�������B�������h��͂����1/2 */

#define TY_CURSE_CHANCE 200 /*!<���Ẩ��O��1�^�[�����̔����m��(1/n)*/
#define CHAINSWORD_NOISE 100 /*!<�`�F���\�[��1�^�[�����̔����m��(1/n)*/

#define SPEAK_CHANCE 8
#define GRINDNOISE 20
#define CYBERNOISE 20

#define GROUP_MAX 32 /*!< place_monster_group() �֐��ɂ�郂���X�^�[��GROUP�������̔z�u�ő吔 / Maximum size of a group of monsters */

 /* ToDo: Make this global */
#define HURT_CHANCE 16 /*!< �����U�����󂯂��ۂɔ\�͒l�ቺ���N�����m��(1/X) / 1/x chance of reducing stats (for elemental attacks) */
