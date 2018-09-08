
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


