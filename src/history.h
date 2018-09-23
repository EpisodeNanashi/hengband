
/*!
* �����������b�Z�[�W�e�[�u�� / Forward declare
*/
typedef struct hist_type hist_type;

/*!
* �����������b�Z�[�W�e�[�u���̍\���̒�` / Player background information
*/
struct hist_type
{
	cptr info;			    /*!> ���b�Z�[�W�{�� / Textual History */

	byte roll;			    /*!> �m���̏d�� / Frequency of this entry */
	byte chart;			    /*!> �����������b�Z�[�W�̗���������`���[�gID / Chart index */
	byte next;			    /*!> ���̃`���[�gID */
	byte bonus;			    /*!> ���b�Z�[�W�ɔ����Љ�I�n�ʂ̕ω���(50����l) / Social Class Bonus + 50 */
};

struct hist_type;
extern hist_type bg[];
