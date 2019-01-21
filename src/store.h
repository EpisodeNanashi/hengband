
/*
 * Store types
 */
#define STORE_GENERAL   0 /*!< �X�܂̎��: �G�݉� */
#define STORE_ARMOURY   1 /*!< �X�܂̎��: �h� */
#define STORE_WEAPON    2 /*!< �X�܂̎��: ���퉮 */
#define STORE_TEMPLE    3 /*!< �X�܂̎��: ���@ */
#define STORE_ALCHEMIST 4 /*!< �X�܂̎��: �B���p�̓X */
#define STORE_MAGIC     5 /*!< �X�܂̎��: ����� */
#define STORE_BLACK     6 /*!< �X�܂̎��: �u���b�N�E�}�[�P�b�g */
#define STORE_HOME      7 /*!< �X�܂̎��: �䂪�� */
#define STORE_BOOK      8 /*!< �X�܂̎��: ���X */
#define STORE_MUSEUM    9 /*!< �X�܂̎��: ������ */
#define MAX_STORES 10 /*!< store.c�p�̓X�܂̎�ލő吔 / Total number of stores (see "store.c", etc) */

/* store.c */
extern bool combine_and_reorder_home(int store_num);
extern void do_cmd_store(void);
extern void store_shuffle(int which);
extern void store_maint(int town_num, int store_num);
extern void store_init(int town_num, int store_num);
extern void move_to_black_market(object_type * o_ptr);

extern const owner_type owners[MAX_STORES][MAX_OWNERS];

extern byte store_table[MAX_STORES][STORE_CHOICES][2];



