#pragma once

/* ELDRITCH_HORROR�ɂ��sanity blast�����Ɋւ��郁�b�Z�[�W�̍ő吔 / Number of entries in the sanity-blast descriptions */
#define MAX_SAN_HORROR_COMMON 5 /*!< ���낵���Ώۂ̌`�e��(���펞�A�׈��E��������) */
#define MAX_SAN_HORROR_EVIL 15 /*!< ���낵���Ώۂ̌`�e��(���펞�A�׈�) */
#define MAX_SAN_HORROR_NEUTRAL 15 /*!< ���낵���Ώۂ̌`�e��(���펞�A����) */
#define MAX_SAN_FUNNY 22  /*!< ���낵���Ώۂ̌`�e��(���o��) */
#define MAX_SAN_COMMENT 5 /*!< ���낵���Ώۂ������Ƃ��̐⋩���b�Z�[�W��(���o��) */

concptr horror_desc_common[MAX_SAN_HORROR_COMMON];
concptr horror_desc_evil[MAX_SAN_HORROR_EVIL];
concptr funny_desc[MAX_SAN_FUNNY];
concptr funny_comments[MAX_SAN_COMMENT];
