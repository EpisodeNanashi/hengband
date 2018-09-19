#include "angband.h"
#include "gameoption.h"

/*!
 * @brief �I�v�V�����e�[�u�� /
 * Available Options
 */
const option_type option_info[] =
{
	/*** Input Options ***/

	{ &rogue_like_commands,         FALSE, OPT_PAGE_INPUT, 0, 0,
	"rogue_like_commands",          _("���[�O���L�[�z�u���g�p����", "Rogue-like commands") },

	{ &always_pickup,               FALSE, OPT_PAGE_INPUT, 0, 5,
	"always_pickup",                _("��ɃA�C�e�����E��" , "Pick things up by default") },

	{ &carry_query_flag,            FALSE, OPT_PAGE_INPUT, 0, 3,
	"carry_query_flag",             _("�A�C�e�����E���O�Ɋm�F����", "Prompt before picking things up") },

	{ &quick_messages,              TRUE,  OPT_PAGE_INPUT, 0, 1,
	"quick_messages",               _("�N�C�b�N�E���b�Z�[�W���g�p����", "Activate quick messages") },

	{ &auto_more,                   FALSE, OPT_PAGE_INPUT, 2, 6,
	"auto_more",                    _("�L�[�҂����Ȃ��ŘA���Ń��b�Z�[�W��\������", "Automatically clear '-more-' prompts") },

	{ &command_menu,                TRUE,  OPT_PAGE_INPUT, 2, 7,
	"command_menu",                 _("���j���[�ɂ��R�}���h�I����L���ɂ���", "Enable command selection menu") },

	{ &other_query_flag,            FALSE, OPT_PAGE_INPUT, 0, 2,
	"other_query_flag",             _("����̃A�C�e�����g�p����Ƃ��Ɋm�F����", "Prompt for floor item selection") },

	{ &use_old_target,              FALSE, OPT_PAGE_INPUT, 0, 4,
	"use_old_target",               _("��ɈȑO�̃^�[�Q�b�g���w�肷��", "Use old target by default") },

	{ &always_repeat,               TRUE,  OPT_PAGE_INPUT, 0, 6,
	"always_repeat",                _("�R�}���h�����J��Ԃ�", "Repeat obvious commands") },

	{ &confirm_destroy,             FALSE, OPT_PAGE_INPUT, 5, 3,
	"confirm_destroy",              _("�u�����l�v�ȃA�C�e����j�󂷂鎞�m�F����", "Prompt for destruction of known worthless items") },

	{ &confirm_wear,                TRUE,  OPT_PAGE_INPUT, 5, 4,
	"confirm_wear",                 _("���ꂽ���𑕔����鎞�m�F����", "Confirm to wear/wield known cursed items") },

	{ &confirm_quest,               TRUE,  OPT_PAGE_INPUT, 1, 9,
	"confirm_quest",                _("�N�G�X�g����߂ĊK�i�œ�����O�Ɋm�F����", "Prompt before exiting a quest level") },

	{ &target_pet,                  FALSE, OPT_PAGE_INPUT, 2, 5,
	"target_pet",                   _("�y�b�g���^�[�Q�b�g�ɂ���", "Allow targetting pets") },

	#ifdef ALLOW_EASY_OPEN
	{ &easy_open,                   TRUE,  OPT_PAGE_INPUT, 5, 7,
	"easy_open",                    _("�����I�Ƀh�A���J����", "Automatically open doors") },
	#endif /* ALLOW_EASY_OPEN */

	#ifdef ALLOW_EASY_DISARM
	{ &easy_disarm,                 TRUE,  OPT_PAGE_INPUT, 5, 8,
	"easy_disarm",                  _("�����I��㩂���������", "Automatically disarm traps") },
	#endif /* ALLOW_EASY_DISARM */

	#ifdef ALLOW_EASY_FLOOR /* TNB */
	{ &easy_floor,                  FALSE, OPT_PAGE_INPUT, 5, 9,
	"easy_floor",                   _("����ŏd�Ȃ����A�C�e�������X�g����", "Display floor stacks in a list") },
	#endif /* ALLOW_EASY_FLOOR -- TNB */

	{ &use_command,                 FALSE, OPT_PAGE_INPUT, 5, 10,
	"use_command",                  _("�u�g��(a)�v�R�}���h�ŃA�C�e�������ł��g����", "Allow unified use command") },

	{ &over_exert,                  FALSE, OPT_PAGE_INPUT, 0, 29,
	"over_exert",                   _("MP������Ȃ��Ă����@�ɒ��킷��", "Allow casting spells when short of mana") },

	{ &numpad_as_cursorkey,         TRUE, OPT_PAGE_INPUT, 2, 31,
	"numpad_as_cursorkey",          _("�G�f�B�^���Ńe���L�[���J�[�\���L�[�Ƃ��Ďg��", "Use numpad keys as cursor keys in editor mode") },

/*** Map Screen Options ***/

	{ &center_player,               FALSE, OPT_PAGE_MAPSCREEN, 5, 11,
	"center_player",                _("��Ƀv���C���[�𒆐S�ɒu��(*�x��*)", "Center map while walking (*slow*)") },

	{ &center_running,              TRUE,  OPT_PAGE_MAPSCREEN, 5, 12,
	"center_running",               _("�����Ă��鎞�ł����S�ɒu��", "Centering even while running") },

	{ &view_yellow_lite,            TRUE,  OPT_PAGE_MAPSCREEN, 1, 28,
	"view_yellow_lite",             _("������͈̔͂���ʂȐF�ŕ\������", "Use special colors for torch-lit grids") },

	{ &view_bright_lite,            TRUE,  OPT_PAGE_MAPSCREEN, 1, 29,
	"view_bright_lite",             _("���E�͈̔͂���ʂȐF�ŕ\������", "Use special colors for 'viewable' grids") },

	{ &view_granite_lite,           TRUE,  OPT_PAGE_MAPSCREEN, 1, 30,
	"view_granite_lite",            _("�ǂ���ʂȐF�ŕ\������(�d��)", "Use special colors for wall grids (slow)") },

	{ &view_special_lite,           TRUE,  OPT_PAGE_MAPSCREEN, 1, 31,
	"view_special_lite",            _("������ʂȐF�ŕ\������(�d��)", "Use special colors for floor grids (slow)") },

	{ &view_perma_grids,            TRUE,  OPT_PAGE_MAPSCREEN, 1, 6,
	"view_perma_grids",             _("���邢�ꏊ�͂��̂܂܂ɂ���", "Map remembers all perma-lit grids") },

	{ &view_torch_grids,            FALSE, OPT_PAGE_MAPSCREEN, 1, 7,
	"view_torch_grids",             _("������ŏƂ炵���ꏊ�͂��̂܂܂ɂ���", "Map remembers all torch-lit grids") },

	{ &view_unsafe_grids,           FALSE, OPT_PAGE_MAPSCREEN, 1, 8,
	"view_unsafe_grids",            _("�g���b�v���m�ς݂łȂ��ꏊ��\������", "Map marked by detect traps") },

	{ &view_reduce_view,            FALSE, OPT_PAGE_MAPSCREEN, 1, 17,
	"view_reduce_view",             _("�X�ł͎������������", "Reduce view-radius in town") },

	{ &fresh_before,                TRUE,  OPT_PAGE_MAPSCREEN, 1, 23,
	"fresh_before",                 _("�A���R�}���h���ɉ�ʂ��ĕ`�悵������", "Flush output while continuous command") },

	{ &fresh_after,                 FALSE, OPT_PAGE_MAPSCREEN, 1, 24,
	"fresh_after",                  _("�R�}���h��ɉ�ʂ���ɍĕ`�悵������", "Flush output after monster's move") },

	{ &fresh_message,               FALSE, OPT_PAGE_MAPSCREEN, 1, 25,
	"fresh_message",                _("���b�Z�[�W�̌�ɉ�ʂ��ĕ`�悷��", "Flush output after every message") },

	{ &hilite_player,               FALSE, OPT_PAGE_MAPSCREEN, 1, 27,
	"hilite_player",                _("�v���C���[�ɃJ�[�\�������킹��", "Hilite the player with the cursor") },

	{ &display_path,                FALSE, OPT_PAGE_MAPSCREEN, 2, 8,
	"display_path",                 _("���@���̋O�Ղ�\������", "Display actual path before shooting") },

/*** Text Display Options ***/

	{ &plain_descriptions,          TRUE,  OPT_PAGE_TEXT, 5, 1,
	"plain_descriptions",           _("�A�C�e���̋L�q���ȗ��ɂ���", "Plain object descriptions") },

	{ &plain_pickup,                FALSE, OPT_PAGE_TEXT, 6, 6,
	"plain_pickup",                 _("�u�E�����v���b�Z�[�W���ȗ�������", "Plain pickup messages(japanese only)") },

	{ &always_show_list,            TRUE,  OPT_PAGE_TEXT, 4, 0,
	"always_show_list",             _("�I�����ɂ͏�Ɉꗗ��\������", "Always show list when choosing items") },

	{ &depth_in_feet,               FALSE, OPT_PAGE_TEXT, 0, 7,
	"depth_in_feet",                _("�_���W�����̐[�����t�B�[�g�ŕ\������", "Show dungeon level in feet") },

	{ &show_labels,                 TRUE,  OPT_PAGE_TEXT, 0, 10,
	"show_labels",                  _("�����ꗗ�ő����ꏊ��\������", "Show labels in object listings") },

	{ &show_weights,                TRUE,  OPT_PAGE_TEXT, 0, 11,
	"show_weights",                 _("�A�C�e���ꗗ�ŏd�ʂ�\������", "Show weights in object listings") },

	{ &show_item_graph,             TRUE,  OPT_PAGE_TEXT, 2, 0,
	"show_item_graph",              _("�A�C�e���̃V���{����\������", "Show items graphics") },

	{ &equippy_chars,               TRUE,  OPT_PAGE_TEXT, 1, 12,
	"equippy_chars",                _("�X�e�[�^�X�ɕ����ő�����\������", "Display 'equippy' chars") },

	{ &display_mutations,           FALSE, OPT_PAGE_TEXT, 5, 0,
	"display_mutations",            _("'C'�R�}���h�œˑR�ψق�\������", "Display mutations in 'C'haracter Display") },

	{ &compress_savefile,           FALSE, OPT_PAGE_TEXT, 1, 26,
	"compress_savefile",            _("�Z�[�u�E�t�@�C�����̃��b�Z�[�W�����k����", "Compress messages in savefiles") },

	{ &abbrev_extra,                FALSE, OPT_PAGE_TEXT, 2, 10,
	"abbrev_extra",                 _("�A�C�e���ɒǉ��ϐ�/�\�̗͂��̂�����", "Describe obj's extra resistances by abbreviation") },

	{ &abbrev_all,                  FALSE, OPT_PAGE_TEXT, 2, 11,
	"abbrev_all",                   _("�A�C�e���ɑS�Ă̑ϐ�/�\�̗͂��̂�����", "Describe obj's all resistances by abbreviation") },

	{ &exp_need,                    FALSE, OPT_PAGE_TEXT, 2, 12,
	"exp_need",                     _("���̃��x���ɕK�v�Ȍo���l��\������", "Show the experience needed for next level") },

	{ &ignore_unview,               FALSE, OPT_PAGE_TEXT, 2, 13,
	"ignore_unview",                _("���E�O�̃����X�^�[�̍s����\�����Ȃ�", "Ignore whenever any monster does") },

	{ &show_ammo_detail,            TRUE, OPT_PAGE_TEXT, 2, 14,
	"show_ammo_detail",             _("��e�̃_���[�W�̐�����\������", "Show description of ammo damage") },

	{ &show_ammo_no_crit,           FALSE, OPT_PAGE_TEXT, 2, 15,
	"show_ammo_no_crit",            _("��S���l�����Ȃ��ꍇ�̖�e�̃_���[�W��\������", "Show ammo damage with no critical") },

	{ &show_ammo_crit_ratio,        FALSE, OPT_PAGE_TEXT, 2, 16,
	"show_ammo_crit_ratio",         _("��e�̉�S��������\������", "Show critical ratio of ammo") },


/*** Game-Play ***/

	{ &stack_force_notes,           TRUE,  OPT_PAGE_GAMEPLAY, 0, 8,
	"stack_force_notes",            _("�قȂ���̃A�C�e�����܂Ƃ߂�", "Merge inscriptions when stacking") },

	{ &stack_force_costs,           FALSE, OPT_PAGE_GAMEPLAY, 0, 9,
	"stack_force_costs",            _("�قȂ銄���\���̃A�C�e�����܂Ƃ߂�", "Merge discounts when stacking") },

	{ &expand_list,                 TRUE,  OPT_PAGE_GAMEPLAY, 1, 5,
	"expand_list",                  _("�u�ꗗ�v�R�}���h���g������", "Expand the power of the list commands") },

	{ &small_levels,                TRUE,  OPT_PAGE_GAMEPLAY, 0, 30,
	"small_levels",                 _("���ɏ������t���A�̐������\�ɂ���", "Allow unusually small dungeon levels") },

	{ &always_small_levels,         FALSE, OPT_PAGE_GAMEPLAY, 2, 3,
	"always_small_levels",          _("��ɔ��ɏ������t���A�𐶐�����", "Always create unusually small dungeon levels") },

	{ &empty_levels,                TRUE,  OPT_PAGE_GAMEPLAY, 0, 31,
	"empty_levels",                 _("����ۂ́u�A���[�i�v���x���̐������\�ɂ���", "Allow empty 'arena' levels") },

	{ &bound_walls_perm,            FALSE, OPT_PAGE_GAMEPLAY, 2, 1,
	"bound_walls_perm",             _("�_���W�����̊O�ǂ��i�v��ɂ���", "Boundary walls become 'permanent wall'") },

	{ &last_words,                  TRUE,  OPT_PAGE_GAMEPLAY, 0, 28,
	"last_words",                   _("�L�����N�^�[�����񂾎��⌾���̂���", "Leave last words when your character dies") },

#ifdef WORLD_SCORE
	{ &send_score,                  TRUE,  OPT_PAGE_GAMEPLAY, 4, 6,
	"send_score",                   _("�X�R�A�T�[�o�ɃX�R�A�𑗂�", "Send score dump to the world score server") },
#endif

	{ &allow_debug_opts,            FALSE, OPT_PAGE_GAMEPLAY, 6, 11,
	"allow_debug_opts",             _("�f�o�b�O/���\�I�v�V������������", "Allow use of debug/cheat options") },

/*** Disturbance ***/

	{ &find_ignore_stairs,          FALSE, OPT_PAGE_DISTURBANCE, 0, 16,
	"find_ignore_stairs",           _("�K�i�͒ʉ߂���", "Run past stairs") },

	{ &find_ignore_doors,           TRUE,  OPT_PAGE_DISTURBANCE, 0, 17,
	"find_ignore_doors",            _("�h�A�͒ʉ߂���", "Run through open doors") },

	{ &find_cut,                    FALSE, OPT_PAGE_DISTURBANCE, 0, 18,
	"find_cut",                     _("�Ȃ�p���΂߂ɍŒZ�����Œʉ߂���", "Run past known corners") },

	{ &check_abort,                 TRUE,  OPT_PAGE_DISTURBANCE, 1, 18,
	"check_abort",                  _("�A���R�}���h�̓L�[���͂Œ��f����", "Check for user abort while continuous command") },

	{ &flush_failure,               TRUE,  OPT_PAGE_DISTURBANCE, 1, 20,
	"flush_failure",                _("�l�X�ȃ~�X�������ɓ��͂��N���A����", "Flush input on various failures") },

	{ &flush_disturb,               FALSE, OPT_PAGE_DISTURBANCE, 1, 21,
	"flush_disturb",                _("��Q�������ɓ��͂��N���A����", "Flush input whenever disturbed") },

	{ &disturb_move,                FALSE, OPT_PAGE_DISTURBANCE, 0, 20,
	"disturb_move",                 _("�ǂ��̃����X�^�[�������Ă��s���𒆎~����", "Disturb whenever any monster moves") },

	{ &disturb_high,                FALSE, OPT_PAGE_DISTURBANCE, 1, 3,
	"disturb_high",                 _("���x���̍��������X�^�[����������s���𒆎~����", "Disturb whenever high-level monster moves") },

	{ &disturb_near,                TRUE,  OPT_PAGE_DISTURBANCE, 0, 21,
	"disturb_near",                 _("���E���̃����X�^�[����������s���𒆎~����", "Disturb whenever viewable monster moves") },

	{ &disturb_pets,                FALSE, OPT_PAGE_DISTURBANCE, 5, 6,
	"disturb_pets",                 _("���E���̃y�b�g����������s���𒆎~����", "Disturb when visible pets move") },

	{ &disturb_panel,               TRUE,  OPT_PAGE_DISTURBANCE, 0, 22,
	"disturb_panel",                _("��ʃX�N���[�����ɍs���𒆎~����", "Disturb whenever map panel changes") },

	{ &disturb_state,               TRUE,  OPT_PAGE_DISTURBANCE, 0, 23,
	"disturb_state",                _("�����̃X�e�[�^�X�ω����ɍs���𒆎~����", "Disturb whenever player state changes") },

	{ &disturb_minor,               TRUE,  OPT_PAGE_DISTURBANCE, 0, 24,
	"disturb_minor",                _("���ׂȂ��Ƃ��N���Ă��s���𒆎~����", "Disturb whenever boring things happen") },

	{ &ring_bell,                   FALSE, OPT_PAGE_DISTURBANCE, 0, 14,
	"ring_bell",                    _("�G���[���Ƀr�[�v����炷", "Audible bell (on errors, etc)") },

	{ &disturb_trap_detect,         TRUE,  OPT_PAGE_DISTURBANCE, 0, 27,
	"disturb_trap_detect",          _("�g���b�v���m�͈͊O�ɏo�钼�O�ɍs���𒆎~����", "Disturb when leaving trap detected area") },

	{ &alert_trap_detect,           FALSE, OPT_PAGE_DISTURBANCE, 0, 25,
	"alert_trap_detect",            _("�g���b�v���m�͈͊O�ɏo�钼�O�Ɍx������", "Alert when leaving trap detected area") },

/*** Birth Options ***/
	{ &manual_haggle,               FALSE, OPT_PAGE_BIRTH, 1, 0,
	"manual_haggle",                _("�X�Œl�؂��������", "Manually haggle in stores") },

	{ &easy_band,                   FALSE, OPT_PAGE_BIRTH, 6, 31,
	"easy_band",                    _("���S�җp�ȒP���[�h(*)", "Easy Mode (*)") },

	{ &smart_learn,                 TRUE,  OPT_PAGE_BIRTH, 1, 14,
	"smart_learn",                  _("�����X�^�[�͎��s���w�K����(*)", "Monsters learn from their mistakes (*)") },

	{ &smart_cheat,                 FALSE, OPT_PAGE_BIRTH, 1, 15,
	"smart_cheat",                  _("�����X�^�[�̓v���C���[�̎�݂�˂�(*)", "Monsters exploit players weaknesses (*)") },

	{ &vanilla_town,                FALSE, OPT_PAGE_BIRTH, 6, 0,
	"vanilla_town",                 _("���c�̊X/�N�G�X�g�ƍr��Ȃ�", "Use 'vanilla' town without quests and wilderness") },

	{ &lite_town,                   FALSE, OPT_PAGE_BIRTH, 6, 1,
	"lite_town",                    _("���K�͂ȊX/�r��Ȃ�", "Use 'lite' town without a wilderness") },

	{ &ironman_shops,               FALSE, OPT_PAGE_BIRTH, 6, 2,
	"ironman_shops",                _("(�S�l�p)�X���g�p���Ȃ�(*)", "Stores are permanently closed (*)") },

	{ &ironman_small_levels,        FALSE, OPT_PAGE_BIRTH, 6, 3,
	"ironman_small_levels",         _("(�S�l�p)��ɔ��ɏ������t���A�𐶐�(*)", "Always create unusually small dungeon levels (*)") },

	{ &ironman_downward,            FALSE, OPT_PAGE_BIRTH, 6, 4,
	"ironman_downward",             _("(�S�l�p)�A�҂Ə��K�i�Ȃ�(*)", "Disable recall and use of up stairs (*)") },

	{ &ironman_empty_levels,        FALSE, OPT_PAGE_BIRTH, 6, 8,
	"ironman_empty_levels",         _("(�S�l�p)��ɋ���ۂ̃A���[�i���x���𐶐�(*)", "Always create empty 'arena' levels (*)") },

	{ &ironman_rooms,               FALSE, OPT_PAGE_BIRTH, 6, 12,
	"ironman_rooms",                _("(�S�l�p)��ɕ��ʂłȂ������𐶐�����(*)", "Always generate very unusual rooms (*)") },

	{ &ironman_nightmare,           FALSE, OPT_PAGE_BIRTH, 6, 18,
	"ironman_nightmare",            _("(�S�l�p)�������[�h(����͑S���s�𗝂ł��I)(*)", "Nightmare mode(it isn't even remotely fair!)(*)") },

	{ &left_hander,                 FALSE, OPT_PAGE_BIRTH, 6, 13,
	"left_hander",                  _("�������ł���", "Left-Hander") },

	{ &preserve_mode,               TRUE,  OPT_PAGE_BIRTH, 6, 14,
	"preserve_mode",                _("�`���̃A�C�e������蓦���Ă��Đ��������(*)", "Preserve artifacts (*)") },

	{ &autoroller,                  TRUE,  OPT_PAGE_BIRTH, 6, 15,
	"autoroller",                   _("�\�͒l�ɃI�[�g���[���[�g�p(*)", "Allow use of autoroller for stats (*)") },

	{ &autochara,                   FALSE, OPT_PAGE_BIRTH, 6, 16,
	"autochara",                   _("�̊i/�n�ʂɃI�[�g���[���[�g�p", "Autoroll for weight, height and social status") },

	{ &powerup_home,                TRUE,  OPT_PAGE_BIRTH, 4, 3,
	"powerup_home",                 _("�䂪�Ƃ��g������(*)", "Increase capacity of your home (*)") },

/*** Easy Object Auto-Destroyer ***/

	{ &destroy_items,               FALSE, OPT_PAGE_AUTODESTROY, 7, 0,
	"destroy_items",                _("�A�C�e���̊ȈՎ����j����g�p����", "Use easy auto-destroyer") },

	{ &destroy_feeling,             FALSE, OPT_PAGE_AUTODESTROY, 7, 8,
	"destroy_feeling",              _("�ȈՊӒ肵���Ƃ������j���K�p����", "Apply auto-destroy as sense feeling") },

	{ &destroy_identify,            FALSE, OPT_PAGE_AUTODESTROY, 7, 9,
	"destroy_identify",             _("�Ӓ肵���Ƃ������j���K�p����", "Apply auto-destroy as identify an item") },

	{ &leave_worth,                 TRUE,  OPT_PAGE_AUTODESTROY, 7, 2,
	"leave_worth",                  _("���l������A�C�e���͉󂳂Ȃ�", "Auto-destroyer leaves known worthy items") },

	{ &leave_equip,                 FALSE, OPT_PAGE_AUTODESTROY, 7, 3,
	"leave_equip",                  _("����/�h��͉󂳂Ȃ�", "Auto-destroyer leaves weapons and armour") },

	{ &leave_chest,                 TRUE,  OPT_PAGE_AUTODESTROY, 7, 7,
	"leave_chest",                  _("�J������Ă��Ȃ����͉󂳂Ȃ�", "Auto-destroyer leaves closed chests") },

	{ &leave_wanted,                TRUE,  OPT_PAGE_AUTODESTROY, 7, 4,
	"leave_wanted",                 _("�܋���̎���/���͉󂳂Ȃ�", "Auto-destroyer leaves wanted corpses") },

	{ &leave_corpse,                FALSE, OPT_PAGE_AUTODESTROY, 7, 5,
	"leave_corpse",                 _("����/���͉󂳂Ȃ�", "Auto-destroyer leaves corpses and skeletons") },

	{ &leave_junk,                  FALSE, OPT_PAGE_AUTODESTROY, 7, 6,
	"leave_junk",                   _("���炭���͉󂳂Ȃ�", "Auto-destroyer leaves junk") },

	{ &leave_special,               TRUE,  OPT_PAGE_AUTODESTROY, 7, 1,
	"leave_special",                _("�푰/�E�Ƃœ��ʂɕK�v�ȃA�C�e���͉󂳂Ȃ�", "Auto-destroyer leaves items your race/class needs") },

/*** Play-record Options ***/

	{ &record_fix_art,              TRUE,  OPT_PAGE_PLAYRECORD, 4, 11,
	"record_fix_art",               _("�Œ�A�[�e�B�t�@�N�g�̓�����L�^����", "Record fixed artifacts") },

	{ &record_rand_art,             TRUE,  OPT_PAGE_PLAYRECORD, 4, 12,
	"record_rand_art",              _("�����_���A�[�e�B�t�@�N�g�̓�����L�^����", "Record random artifacts") },

	{ &record_destroy_uniq,         TRUE,  OPT_PAGE_PLAYRECORD, 4, 13,
	"record_destroy_uniq",          _("���j�[�N�����X�^�[��|�����Ƃ����L�^����", "Record when destroy unique monster") },

	{ &record_fix_quest,            TRUE,  OPT_PAGE_PLAYRECORD, 4, 14,
	"record_fix_quest",             _("�Œ�N�G�X�g�̒B�����L�^����", "Record fixed quests") },

	{ &record_rand_quest,           TRUE,  OPT_PAGE_PLAYRECORD, 4, 15,
	"record_rand_quest",            _("�����_���N�G�X�g�̒B�����L�^����", "Record random quests") },

	{ &record_maxdepth,             TRUE,  OPT_PAGE_PLAYRECORD, 4, 16,
	"record_maxdepth",              _("�Ő[�K���X�V�����Ƃ��ɋL�^����", "Record movements to deepest level") },

	{ &record_stair,                TRUE,  OPT_PAGE_PLAYRECORD, 4, 17,
	"record_stair",                 _("�K�̈ړ����L�^����", "Record recall and stair movements") },

	{ &record_buy,                  TRUE,  OPT_PAGE_PLAYRECORD, 4, 18,
	"record_buy",                   _("�A�C�e���̍w�����L�^����", "Record purchased items") },

	{ &record_sell,                 FALSE, OPT_PAGE_PLAYRECORD, 4, 19,
	"record_sell",                  _("�A�C�e���̔��p���L�^����", "Record sold items") },

	{ &record_danger,               TRUE,  OPT_PAGE_PLAYRECORD, 4, 20,
	"record_danger",                _("�s���`�ɂȂ����Ƃ����L�^����", "Record hitpoint warning") },

	{ &record_arena,                TRUE,  OPT_PAGE_PLAYRECORD, 4, 21,
	"record_arena",                 _("�A���[�i�ł̏������L�^����", "Record arena victories") },

	{ &record_ident,                TRUE,  OPT_PAGE_PLAYRECORD, 4, 22,
	"record_ident",                 _("�������̃A�C�e���̎��ʂ��L�^����", "Record first identified items") },

	{ &record_named_pet,            FALSE, OPT_PAGE_PLAYRECORD, 4, 23,
	"record_named_pet",             _("���O���y�b�g�̏����L�^����", "Record informations of named pets") },

/*** End of Table ***/

	{ NULL,                         0, 0, 0, 0,
	NULL,                           NULL }
};

/*!
 * �`�[�g�I�v�V�����̒�`�e�[�u�� / Cheating options
 */
const option_type cheat_info[CHEAT_MAX] =
{
	{ &cheat_peek,		FALSE,	255,	0x01, 0x00,
	"cheat_peek",		_("�A�C�e���̐������̂�������", "Peek into object creation")
	},

	{ &cheat_hear,		FALSE,	255,	0x02, 0x00,
	"cheat_hear",		_("�����X�^�[�̐������̂�������", "Peek into monster creation")
	},

	{ &cheat_room,		FALSE,	255,	0x04, 0x00,
	"cheat_room",		_("�_���W�����̐������̂�������", "Peek into dungeon creation")
	},

	{ &cheat_xtra,		FALSE,	255,	0x08, 0x00,
	"cheat_xtra",		_("���̑��̎����̂�������", "Peek into something else")
	},

	{ &cheat_know,		FALSE,	255,	0x10, 0x00,
	"cheat_know",		_("���S�ȃ����X�^�[�̎v���o��m��", "Know complete monster info")
	},

	{ &cheat_live,		FALSE,	255,	0x20, 0x00,
	"cheat_live",		_("����������邱�Ƃ��\�ɂ���", "Allow player to avoid death")
	},

	{ &cheat_save,		FALSE,	255,	0x40, 0x00,
	"cheat_save",		_("���񂾎��Z�[�u���邩�m�F����", "Ask for saving death")
	},

	{ &cheat_diary_output,	FALSE,	255,	0x80, 0x00,
	"cheat_diary_output",	_("�E�B�U�[�h���O����L�ɏo�͂���", "Output wizard log to diary.")
	},

	{ &cheat_turn,	FALSE,	255,	0x81, 0x00,
	"cheat_turn",	_("�Q�[�����b�Z�[�W�Ƀ^�[���\�����s��", "Put turn to game message.")
	},

	{ &cheat_sight,	FALSE,	255,	0x82, 0x00,
	"cheat_sight",	_("�u����v�R�}���h���g������B", "Expand \"L\"ook command.")
	}
};


/*!
* �����Z�[�u�I�v�V�����e�[�u��
*/
const option_type autosave_info[2] =
{
	{ &autosave_l,      FALSE, 255, 0x01, 0x00,
	"autosave_l",    _("�V�����K�ɓ���x�Ɏ����Z�[�u����", "Autosave when entering new levels") },

	{ &autosave_t,      FALSE, 255, 0x02, 0x00,
	"autosave_t",   _("���^�[�����Ɏ����Z�[�u����", "Timed autosave") },
};
