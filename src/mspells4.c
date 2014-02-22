#include "angband.h"

char* monster_name(int m_idx, char* m_name)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_desc(m_name, m_ptr, 0x00);
    return m_name;
}

/* 2 monster each is near by player, return true */
bool monster_near_player(int m_idx, int t_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_type    *t_ptr = &m_list[t_idx];
    return (m_ptr->cdis <= MAX_SIGHT) || (t_ptr->cdis <= MAX_SIGHT);
}

/* player can see monster, return true */
bool see_monster(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    return is_seen(m_ptr);
}

bool spell_learnable(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    /* Extract the "see-able-ness" */
    bool seen = (!p_ptr->blind && m_ptr->ml);

    bool maneable = player_has_los_bold(m_ptr->fy, m_ptr->fx);
    return (seen && maneable && !world_monster);
}

int monster_level_idx(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    int rlev = ((r_ptr->level >= 1) ? r_ptr->level : 1);
    return rlev;
}

bool monster_is_powerful(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    return (r_ptr->flags2 & RF2_POWERFUL);
}

void monster_wakeup(int t_idx)
{
    (void)set_monster_csleep(t_idx, 0);
}

void monster_fear_message(int t_idx)
{
    char t_name[80];
    monster_name(t_idx, t_name);
    if (see_monster(t_idx))
    {
        msg_format(_("%^s�϶��ݤ���ƨ���Ф�����", "%^s flees in terror!"), t_name);
    }
}

void monspell_message_base(int m_idx, int t_idx, cptr msg1, cptr msg2, cptr msg3, cptr msg4, bool msg_flag, int TARGET_TYPE)
{
    bool known = monster_near_player(m_idx, t_idx);
    bool see_either = see_monster(m_idx) || see_monster(t_idx);
    bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
    bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

    if (mon_to_player || (mon_to_mon && known && see_either))
        disturb(1, 1);

    if (msg_flag)
    {
        if (mon_to_player)
            msg_format(msg1, m_name);
        else if (mon_to_mon && known && see_either)
            msg_format(msg2, m_name);
    }
    else
    {
        if (mon_to_player)
        {
            msg_format(msg3, m_name);
        }
        else if (mon_to_mon && known && see_either)
        {
            msg_format(msg4, m_name, t_name);
        }
    }

    if (mon_to_mon && known && !see_either)
        mon_fight = TRUE;
}

void monspell_message(int m_idx, int t_idx, cptr msg1, cptr msg2, cptr msg3, int TARGET_TYPE)
{
    monspell_message_base(m_idx, t_idx, msg1, msg1, msg2, msg3, p_ptr->blind, TARGET_TYPE);
}

void simple_monspell_message(int m_idx, int t_idx, cptr msg1, cptr msg2, int TARGET_TYPE)
{
    monspell_message_base(m_idx, t_idx, msg1, msg2, msg1, msg2, p_ptr->blind, TARGET_TYPE);
}

void spell_RF4_SHRIEK(int m_idx, int t_idx, int TARGET_TYPE)
{
    simple_monspell_message(m_idx, t_idx,
        _("%^s������⤤���ڤ����򤢤�����", "%^s makes a high pitched shriek."),
        _("%^s��%s�˸����äƶ������", "%^s shrieks at %s."),
        TARGET_TYPE);

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        aggravate_monsters(m_idx);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        monster_wakeup(t_idx);
    }
}

void spell_RF4_DISPEL(int m_idx, int t_idx, int TARGET_TYPE)
{
    bool known = monster_near_player(m_idx, t_idx);
    bool see_m = see_monster(m_idx);
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

    monspell_message(m_idx, t_idx,
        _("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
        _("%^s�����Ͼõ�μ�ʸ��ǰ������", "%^s invokes a dispel magic."),
        _("%^s��%s���Ф������Ͼõ�μ�ʸ��ǰ������", "%^s invokes a dispel magic at %s."),
        TARGET_TYPE);

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        dispel_player();
        if (p_ptr->riding) dispel_monster_status(p_ptr->riding);

        if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
            msg_print(_("���䤬�ä��ʡ�", ""));

        learn_spell(MS_DISPEL);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        if (t_idx == p_ptr->riding) dispel_player();
        dispel_monster_status(t_idx);
    }
}

int spell_RF4_ROCKET(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    monster_type    *m_ptr = &m_list[m_idx];

    monspell_message(m_idx, t_idx,
        _("%^s��������ͤä���", "%^s shoots something."),
        _("%^s�����åȤ�ȯ�ͤ�����", "%^s fires a rocket."),
        _("%^s��%s�˥��åȤ�ȯ�ͤ�����", "%^s fires a rocket at %s."),
        TARGET_TYPE);

    dam = (m_ptr->hp / 4) > 800 ? 800 : (m_ptr->hp / 4);
    breath(y, x, m_idx, GF_ROCKET, dam, 2, FALSE, MS_ROCKET, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_SHARD);
    return dam;
}

int spell_RF4_SHOOT(int y, int x, int m_idx, int t_idx,int TARGET_TYPE)
{
    int dam;
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    
    monspell_message(m_idx, t_idx,
        _("%^s����̯�ʲ���ȯ������", "%^s makes a strange noise."),
        _("%^s��������ä���", "%^s fires an arrow."),
        _("%^s��%s��������ä���", "%^s fires an arrow at %s."),
        TARGET_TYPE);

    dam = damroll(r_ptr->blow[0].d_dice, r_ptr->blow[0].d_side);
    bolt(m_idx, y, x, GF_ARROW, dam, MS_SHOOT, TARGET_TYPE);
    sound(SOUND_SHOOT);

    return dam;
}

int spell_RF4_BREATH(int GF_TYPE, int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam, ms_type, drs_type;
    cptr type_s;
    bool smart_learn = TRUE;
    monster_type    *m_ptr = &m_list[m_idx];
    bool known = monster_near_player(m_idx, t_idx);
    bool see_either = see_monster(m_idx) || see_monster(t_idx);
    bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
    bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

    switch (GF_TYPE)
    {
    case GF_ACID:
        dam = ((m_ptr->hp / 3) > 1600 ? 1600 : (m_ptr->hp / 3));
        type_s = _("��", "acid");
        ms_type = MS_BR_ACID;
        drs_type = DRS_ACID;
        break;
    case GF_ELEC:
        dam = ((m_ptr->hp / 3) > 1600 ? 1600 : (m_ptr->hp / 3));
        type_s = _("���", "lightning");
        ms_type = MS_BR_ELEC;
        drs_type = DRS_ELEC;
        break;
    case GF_FIRE:
        dam = ((m_ptr->hp / 3) > 1600 ? 1600 : (m_ptr->hp / 3));
        type_s = _("�б�", "fire");
        ms_type = MS_BR_FIRE;
        drs_type = DRS_FIRE;
        break;
    case GF_COLD:
        dam = ((m_ptr->hp / 3) > 1600 ? 1600 : (m_ptr->hp / 3));
        type_s = _("�䵤", "frost");
        ms_type = MS_BR_COLD;
        drs_type = DRS_COLD;
        break;
    case GF_POIS:
        dam = ((m_ptr->hp / 3) > 800 ? 800 : (m_ptr->hp / 3));
        type_s = _("����", "gas");
        ms_type = MS_BR_POIS;
        drs_type = DRS_POIS;
        break;
    case GF_NETHER:
        dam = ((m_ptr->hp / 6) > 550 ? 550 : (m_ptr->hp / 6));
        type_s = _("�Ϲ�", "nether");
        ms_type = MS_BR_NETHER;
        drs_type = DRS_NETH;
        break;
    case GF_LITE:
        dam = ((m_ptr->hp / 6) > 400 ? 400 : (m_ptr->hp / 6));
        type_s = _("����", "light");
        ms_type = MS_BR_LITE;
        drs_type = DRS_LITE;
        break;
    case GF_DARK:
        dam = ((m_ptr->hp / 6) > 400 ? 400 : (m_ptr->hp / 6));
        type_s = _("�Ź�", "darkness");
        ms_type = MS_BR_DARK;
        drs_type = DRS_DARK;
        break;
    case GF_CONFUSION:
        dam = ((m_ptr->hp / 6) > 450 ? 450 : (m_ptr->hp / 6));
        type_s = _("����", "confusion");
        ms_type = MS_BR_CONF;
        drs_type = DRS_CONF;
        break;
    case GF_SOUND:
        dam = ((m_ptr->hp / 6) > 450 ? 450 : (m_ptr->hp / 6));
        type_s = _("�첻", "sound");
        ms_type = MS_BR_SOUND;
        drs_type = DRS_SOUND;
        break;
    case GF_CHAOS:
        dam = ((m_ptr->hp / 6) > 600 ? 600 : (m_ptr->hp / 6));
        type_s = _("������", "chaos");
        ms_type = MS_BR_CHAOS;
        drs_type = DRS_CHAOS;
        break;
    case GF_DISENCHANT:
        dam = ((m_ptr->hp / 6) > 500 ? 500 : (m_ptr->hp / 6));
        type_s = _("����", "disenchantment");
        ms_type = MS_BR_DISEN;
        drs_type = DRS_DISEN;
        break;
    case GF_NEXUS:
        dam = ((m_ptr->hp / 3) > 250 ? 250 : (m_ptr->hp / 3));
        type_s = _("���̺���", "nexus");
        ms_type = MS_BR_NEXUS;
        drs_type = DRS_NEXUS;
        break;
    case GF_TIME:
        dam = ((m_ptr->hp / 3) > 150 ? 150 : (m_ptr->hp / 3));
        type_s = _("���ֵ�ž", "time");
        ms_type = MS_BR_TIME;
        smart_learn = FALSE;
        break;
    case GF_INERTIA:
        dam = ((m_ptr->hp / 6) > 200 ? 200 : (m_ptr->hp / 6));
        type_s = _("����", "inertia");
        ms_type = MS_BR_INERTIA;
        smart_learn = FALSE;
        break;
    case GF_GRAVITY:
        dam = ((m_ptr->hp / 3) > 200 ? 200 : (m_ptr->hp / 3));
        type_s = _("����", "gravity");
        ms_type = MS_BR_GRAVITY;
        smart_learn = FALSE;
        break;
    case GF_SHARDS:
        dam = ((m_ptr->hp / 6) > 500 ? 500 : (m_ptr->hp / 6));
        type_s = _("����", "shards");
        ms_type = MS_BR_SHARDS;
        drs_type = DRS_SHARD;
        break;
    case GF_PLASMA:
        dam = ((m_ptr->hp / 6) > 150 ? 150 : (m_ptr->hp / 6));
        type_s = _("�ץ饺��", "plasma");
        ms_type = MS_BR_PLASMA;
        smart_learn = FALSE;
        break;
    case GF_FORCE:
        dam = ((m_ptr->hp / 6) > 200 ? 200 : (m_ptr->hp / 6));
        type_s = _("�ե�����", "force");
        ms_type = MS_BR_FORCE;
        smart_learn = FALSE;
        break;
    case GF_MANA:
        dam = ((m_ptr->hp / 3) > 250 ? 250 : (m_ptr->hp / 3));
        type_s = _("����", "mana");
        ms_type = MS_BR_MANA;
        smart_learn = FALSE;
        break;
    case GF_NUKE:
        dam = ((m_ptr->hp / 3) > 800 ? 800 : (m_ptr->hp / 3));
        type_s = _("�������Ѵ�ʪ", "toxic waste");
        ms_type = MS_BR_NUKE;
        drs_type = DRS_POIS;
        break;
    case GF_DISINTEGRATE:
        dam = ((m_ptr->hp / 6) > 150 ? 150 : (m_ptr->hp / 6));
        type_s = _("ʬ��", "disintegration");
        ms_type = MS_BR_DISI;
        smart_learn = FALSE;
        break;
    default:
        break;
    }

    if (mon_to_player || (mon_to_mon && known && see_either))
        disturb(1, 1);

    if (m_ptr->r_idx == MON_JAIAN && GF_TYPE == GF_SOUND)
    {
        msg_format(_("�֥ܥ�����������������", "'Booooeeeeee'"));
    }
    else if (m_ptr->r_idx == MON_BOTEI && GF_TYPE == GF_SHARDS)
    {
        msg_format(_("�֥���ӥ륫�å�����������", "'Boty-Build cutter!!!'"));
    }
    else if (p_ptr->blind)
    {
        if (mon_to_player || (mon_to_mon && known && see_either))
            msg_format(_("%^s�������Υ֥쥹���Ǥ�����", "%^s breathes."), m_name);
    }
    else
    {
        if (mon_to_player)
        {
            msg_format(_("%^s��%^s�Υ֥쥹���Ǥ�����", "%^s breathes %^s."), m_name, type_s);
        }
        else if (mon_to_mon && known && see_either)
        {
            _(msg_format("%^s��%^s��%^s�Υ֥쥹���Ǥ�����", m_name, t_name, type_s),
              msg_format("%^s breathes %^s at %^s.", m_name, type_s, t_name));
        }
    }

    if (mon_to_mon && known && !see_either)
        mon_fight = TRUE;

    sound(SOUND_BREATH);
    breath(y, x, m_idx, GF_TYPE, dam, 0, TRUE, ms_type, TARGET_TYPE);
    if (smart_learn && mon_to_player)
        update_smart_learn(m_idx, drs_type);

    return dam;
}

int spell_RF4_BA_NUKE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s������ǽ������ä���", "%^s casts a ball of radiation."),
        _("%^s��%s������ǽ������ä���", "%^s casts a ball of radiation at %s."),
        TARGET_TYPE);

    dam = (rlev + damroll(10, 6)) * (monster_is_powerful(m_idx) ? 2 : 1);
    breath(y, x, m_idx, GF_NUKE, dam, 2, FALSE, MS_BALL_NUKE, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_POIS);

    return dam;
}

int spell_RF4_BA_CHAO(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s���������ˤĤ֤䤤����", "%^s mumbles frighteningly."),
        _("%^s������륹�����ä���", "%^s invokes a raw Logrus."),
        _("%^s��%s�˽���륹�����ä���", "%^s invokes raw Logrus upon %s."),
        TARGET_TYPE);

    dam = (monster_is_powerful(m_idx) ? (rlev * 3) : (rlev * 2)) + damroll(10, 10);

    breath(y, x, m_idx, GF_CHAOS, dam, 4, FALSE, MS_BALL_CHAOS, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_CHAOS);

    return dam;
}

int spell_RF5_BA_ACID(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam, rad;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s�������åɡ��ܡ���μ�ʸ�򾧤�����", "%^s casts an acid ball."),
        _("%^s��%s�˸����äƥ����åɡ��ܡ���μ�ʸ�򾧤�����",
          "%^s casts an acid ball at %s."),
        TARGET_TYPE);

    if (monster_is_powerful(m_idx))
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 3) + 15);
    }

    breath(y, x, m_idx, GF_ACID, dam, rad, FALSE, MS_BALL_ACID, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_ACID);

    return dam;
}

int spell_RF5_BA_ELEC(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam, rad;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s��������������ܡ���μ�ʸ�򾧤�����", "%^s casts a lightning ball."),
        _("%^s��%s�˸����äƥ���������ܡ���μ�ʸ�򾧤�����", 
          "%^s casts a lightning ball at %s."),
        TARGET_TYPE);

    if (monster_is_powerful(m_idx))
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 3 / 2) + 8);
    }
    
    breath(y, x, m_idx, GF_ELEC, dam, rad, FALSE, MS_BALL_ELEC, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_ELEC);

    return dam;
}

int spell_RF5_BA_FIRE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam, rad;
    monster_type    *m_ptr = &m_list[m_idx];
    int rlev = monster_level_idx(m_idx);

    if (m_ptr->r_idx == MON_ROLENTO)
    {
        monspell_message(m_idx, t_idx,
            _("%s���������ꤲ����", "%^s throws something."),
            _("%s�ϼ����Ƥ��ꤲ����", "%^s throws a hand grenade."),
            _("%^s��%^s�˸����äƼ����Ƥ��ꤲ����", "%^s throws a hand grenade."),
            TARGET_TYPE);
    }
    else
    {
        monspell_message(m_idx, t_idx,
            _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
            _("%^s���ե��������ܡ���μ�ʸ�򾧤�����", "%^s casts a fire ball."),
            _("%^s��%s�˸����äƥե��������ܡ���μ�ʸ�򾧤�����",
            "%^s casts a fire ball at %s."),
            TARGET_TYPE);
    }

    if (monster_is_powerful(m_idx))
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 7 / 2) + 10);
    }

    breath(y, x, m_idx, GF_FIRE, dam, rad, FALSE, MS_BALL_FIRE, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_FIRE);

    return dam;
}

int spell_RF5_BA_COLD(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam, rad;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s�����������ܡ���μ�ʸ�򾧤�����", "%^s casts a frost ball."),
        _("%^s��%s�˸����äƥ��������ܡ���μ�ʸ�򾧤�����",
        "%^s casts a frost ball at %s."),
        TARGET_TYPE);

    if (monster_is_powerful(m_idx))
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 3 / 2) + 10);
    }

    breath(y, x, m_idx, GF_COLD, dam, rad, FALSE, MS_BALL_COLD, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_COLD);

    return dam;
}


int spell_RF5_BA_POIS(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���������μ�ʸ�򾧤�����", "%^s casts a stinking cloud."),
        _("%^s��%s�˸����äư������μ�ʸ�򾧤�����", "%^s casts a stinking cloud at %s."),
        TARGET_TYPE);

    dam = damroll(12, 2) * (monster_is_powerful(m_idx) ? 2 : 1);
    breath(y, x, m_idx, GF_POIS, dam, 2, FALSE, MS_BALL_POIS, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_POIS);

    return dam;
}

int spell_RF5_BA_NETH(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���Ϲ���μ�ʸ�򾧤�����", "%^s casts a nether ball."),
        _("%^s��%s�˸����ä��Ϲ���μ�ʸ�򾧤�����", "%^s casts a nether ball at %s."),
        TARGET_TYPE);

    dam = 50 + damroll(10, 10) + (rlev * (monster_is_powerful(m_idx) ? 2 : 1));
    breath(y, x, m_idx, GF_NETHER, dam, 2, FALSE, MS_BALL_NETHER, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_NETH);

    return dam;
}

int spell_RF5_BA_WATE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);
    bool known = monster_near_player(m_idx, t_idx);
    bool see_either = see_monster(m_idx) || see_monster(t_idx);
    bool mon_to_mon = (TARGET_TYPE == MONSTER_TO_MONSTER);
    bool mon_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	char t_name[80];
    monster_name(t_idx, t_name);


    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s��ή���褦�ʿȿ���򤷤���", "%^s gestures fluidly."),
        _("%^s��%s���Ф���ή���褦�ʿȿ���򤷤���", "%^s gestures fluidly at %s."),
        TARGET_TYPE);

    if (mon_to_player)
    {
        msg_format(_("���ʤ��ϱ������˰��߹��ޤ줿��", "You are engulfed in a whirlpool."));
    }
    else if (mon_to_mon && known && see_either && !p_ptr->blind)
    {
        msg_format(_("%^s�ϱ����˰��߹��ޤ줿��", "%^s is engulfed in a whirlpool."), t_name);
    }

    dam = (monster_is_powerful(m_idx) ? randint1(rlev * 3) : randint1(rlev * 2)) + 50;
    breath(y, x, m_idx, GF_WATER, dam, 4, FALSE, MS_BALL_WATER, TARGET_TYPE);
    return dam;
}


int spell_RF5_BA_MANA(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
        _("%^s�����Ϥ���μ�ʸ��ǰ������", "%^s invokes a mana storm."),
        _("%^s��%s���Ф������Ϥ���μ�ʸ��ǰ������", "%^s invokes a mana storm upon %s."),
        TARGET_TYPE);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_MANA, dam, 4, FALSE, MS_BALL_MANA, TARGET_TYPE);
    return dam;
}

int spell_RF5_BA_DARK(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
        _("%^s���Ź�����μ�ʸ��ǰ������", "%^s invokes a darkness storm."),
        _("%^s��%s���Ф��ưŹ�����μ�ʸ��ǰ������", "%^s invokes a darkness storm upon %s."),
        TARGET_TYPE);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_DARK, dam, 4, FALSE, MS_BALL_DARK, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_DARK);

    return dam;
}

int spell_RF5_DRAIN_MANA(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int rlev = monster_level_idx(m_idx);
    int dam;
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);


    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        disturb(1, 1);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER && see_monster(m_idx))
    { 
        /* Basic message */
        msg_format(_("%^s���������ͥ륮����%s����ۤ��Ȥä���", "%^s draws psychic energy from %s."), m_name, t_name);
    }

    dam = ((randint1(rlev) / 2) + 1);
    breath(y, x, m_idx, GF_DRAIN_MANA, dam, 0, FALSE, MS_DRAIN_MANA, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_MANA);
    
    return dam;
}

int spell_RF5_MIND_BLAST(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    bool seen = (!p_ptr->blind && m_ptr->ml);
    int rlev = monster_level_idx(m_idx);
    int dam;
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);


    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        disturb(1, 1);
        if (!seen)
            msg_print(_("���������ʤ���������ǰ�����äƤ���褦����", "You feel something focusing on your mind."));
        else
            msg_format(_("%^s�����ʤ���Ʒ�򤸤äȤˤ��Ǥ��롣", "%^s gazes deep into your eyes."), m_name);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER && see_monster(m_idx))
    {
        msg_format(_("%^s��%s�򤸤ä��ˤ����", "%^s gazes intently at %s."), m_name, t_name);
    }

    dam = damroll(7, 7);
    breath(y, x, m_idx, GF_MIND_BLAST, dam, 0, FALSE, MS_MIND_BLAST, TARGET_TYPE);
    return dam;
}

int spell_RF5_BRAIN_SMASH(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    bool seen = (!p_ptr->blind && m_ptr->ml);
    int rlev = monster_level_idx(m_idx);
    int dam;
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);


    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        disturb(1, 1);
        if (!seen)
            msg_print(_("���������ʤ���������ǰ�����äƤ���褦����", "You feel something focusing on your mind."));
        else
            msg_format(_("%^s�����ʤ���Ʒ�򤸤äȤˤ��Ǥ��롣", "%^s gazes deep into your eyes."), m_name);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER && see_monster(m_idx))
    {
        msg_format(_("%^s��%s�򤸤ä��ˤ����", "%^s gazes intently at %s."), m_name, t_name);
    }

    dam = damroll(12, 12);
    breath(y, x, m_idx, GF_BRAIN_SMASH, dam, 0, FALSE, MS_BRAIN_SMASH, TARGET_TYPE);
    return dam;
}

void spell_RF5_CAUSE(int GF_TYPE, int dam, int y, int x, int m_idx, int t_idx, cptr msg1, cptr msg2, cptr msg3, int MS_TYPE, int TARGET_TYPE)
{
    int rlev = monster_level_idx(m_idx);
	char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
	monster_name(t_idx, t_name);

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        disturb(1, 1);
        if (p_ptr->blind)
            msg_format(msg1, m_name);
        else
            msg_format(msg2, m_name);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        if (see_monster(m_idx))
        {
            msg_format(msg3, m_name, t_name);
        }
        else
        {
            mon_fight = TRUE;
        }
    }
    breath(y, x, m_idx, GF_TYPE, dam, 0, FALSE, MS_TYPE, TARGET_TYPE);
}

int spell_RF5_CAUSE_1(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    cptr msg1, msg2, msg3;
    int dam = damroll(3, 8);

    msg1 = _("%^s��������Ĥ֤䤤����", "%^s mumbles.");
    msg2 = _("%^s�����ʤ���ؤ����Ƽ��ä���", "%^s points at you and curses.");
    msg3 = _("%^s��%s��ؤ����Ƽ����򤫤�����", "%^s points at %s and curses.");
    
    spell_RF5_CAUSE(GF_CAUSE_1, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_1, TARGET_TYPE);
    return dam;
}

int spell_RF5_CAUSE_2(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    cptr msg1, msg2, msg3;
    int dam = damroll(8, 8);

    msg1 = _("%^s��������Ĥ֤䤤����", "%^s mumbles.");
    msg2 = _("%^s�����ʤ���ؤ����ƶ������˼��ä���", "%^s points at you and curses horribly.");
    msg3 = _("%^s��%s��ؤ����ƶ������˼����򤫤�����", "%^s points at %s and curses horribly.");

    spell_RF5_CAUSE(GF_CAUSE_2, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_2, TARGET_TYPE);
    return dam;
}

int spell_RF5_CAUSE_3(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    cptr msg1, msg2, msg3;
    int dam = damroll(10, 15);

    msg1 = _("%^s�������������Ƕ������", "%^s mumbles loudly.");
    msg2 = _("%^s�����ʤ���ؤ����ƶ������˼�ʸ�򾧤�����", "%^s points at you, incanting terribly!");
    msg3 = _("%^s��%s��ؤ������������˼�ʸ�򾧤�����", "%^s points at %s, incanting terribly!");

    spell_RF5_CAUSE(GF_CAUSE_3, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_3, TARGET_TYPE);
    return dam;
}

int spell_RF5_CAUSE_4(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    cptr msg1, msg2, msg3;
    int dam = damroll(15, 15);

    msg1 = _("%^s���֤����ϴ��˻��Ǥ���פȶ������", "%^s screams the word 'DIE!'");
    msg2 = _("%^s�����ʤ����빦���ͤ��ơ֤����ϴ��˻��Ǥ���פȶ������", "%^s points at you, screaming the word DIE!");
    msg3 = _("%^s��%s���빦���ͤ��ơ��֤����ϴ��˻��Ǥ���פȶ������", "%^s points at %s, screaming the word, 'DIE!'");

    spell_RF5_CAUSE(GF_CAUSE_4, dam, y, x, m_idx, t_idx, msg1, msg2, msg3, MS_CAUSE_4, TARGET_TYPE);
    return dam;
}

int spell_RF5_BO_ACID(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s�������åɡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a acid bolt."),
        _("%s��%s�˸����äƥ����åɡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts an acid bolt at %s."),
        TARGET_TYPE);

    dam = (damroll(7, 8) + (rlev / 3)) * (monster_is_powerful(m_idx) ? 2 : 1);
    bolt(m_idx, y, x, GF_ACID, dam, MS_BOLT_ACID, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_ACID);
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_ELEC(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a lightning bolt."),
        _("%^s��%s�˸����äƥ���������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a lightning bolt at %s."),
        TARGET_TYPE);

    dam = (damroll(4, 8) + (rlev / 3)) * (monster_is_powerful(m_idx) ? 2 : 1);
    bolt(m_idx, y, x, GF_ELEC, dam, MS_BOLT_ELEC, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_ELEC);
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_FIRE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���ե��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a fire bolt."),
        _("%^s��%s�˸����äƥե��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a fire bolt at %s."),
        TARGET_TYPE);

    dam = (damroll(9, 8) + (rlev / 3)) * (monster_is_powerful(m_idx) ? 2 : 1);
    bolt(m_idx, y, x, GF_FIRE, dam, MS_BOLT_FIRE, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_FIRE);
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_COLD(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s�����������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a frost bolt."),
        _("%^s��%s�˸����äƥ��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a frost bolt at %s."),
        TARGET_TYPE);

    dam = (damroll(6, 8) + (rlev / 3)) * (monster_is_powerful(m_idx) ? 2 : 1);
    bolt(m_idx, y, x, GF_COLD, dam, MS_BOLT_COLD, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_COLD);
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BA_LITE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
        _("%^s���������С����Ȥμ�ʸ��ǰ������", "%^s invokes a starburst."),
        _("%^s��%s���Ф��ƥ������С����Ȥμ�ʸ��ǰ������", "%^s invokes a starburst upon %s."),
        TARGET_TYPE);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_LITE, dam, 4, FALSE, MS_STARBURST, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
        update_smart_learn(m_idx, DRS_LITE);

    return dam;
}


int spell_RF5_BO_NETH(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���Ϲ�����μ�ʸ�򾧤�����", "%^s casts a nether bolt."),
        _("%^s��%s�˸����ä��Ϲ�����μ�ʸ�򾧤�����", "%^s casts a nether bolt at %s."),
        TARGET_TYPE);

    dam = 30 + damroll(5, 5) + (rlev * 4) / (monster_is_powerful(m_idx) ? 2 : 3);
    bolt(m_idx, y, x, GF_NETHER, dam, MS_BOLT_NETHER, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_NETH);
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_WATE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a water bolt."),
        _("%^s��%s�˸����äƥ������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a water bolt at %s."),
        TARGET_TYPE);

    dam = damroll(10, 10) + (rlev * 3 / (monster_is_powerful(m_idx) ? 2 : 3));
    bolt(m_idx, y, x, GF_WATER, dam, MS_BOLT_WATER, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_MANA(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s�����Ϥ���μ�ʸ�򾧤�����", "%^s casts a mana bolt."),
        _("%^s��%s�˸����ä����Ϥ���μ�ʸ�򾧤�����", "%^s casts a mana bolt at %s."),
        TARGET_TYPE);

    dam = randint1(rlev * 7 / 2) + 50;
    bolt(m_idx, y, x, GF_MANA, dam, MS_BOLT_MANA, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_PLAS(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���ץ饺�ޡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a plasma bolt."),
        _("%^s��%s�˸����äƥץ饺�ޡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a plasma bolt at %s."),
        TARGET_TYPE);

    dam = 10 + damroll(8, 7) + (rlev * 3 / (monster_is_powerful(m_idx) ? 2 : 3));
    bolt(m_idx, y, x, GF_PLASMA, dam, MS_BOLT_PLASMA, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_BO_ICEE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���˴�����μ�ʸ�򾧤�����", "%^s casts an ice bolt."),
        _("%^s��%s�˸����äƶ˴�����μ�ʸ�򾧤�����", "%^s casts an ice bolt at %s."),
        TARGET_TYPE);

    dam = damroll(6, 6) + (rlev * 3 / (monster_is_powerful(m_idx) ? 2 : 3));
    bolt(m_idx, y, x, GF_ICE, dam, MS_BOLT_ICE, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_COLD);
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

int spell_RF5_MISSILE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;
    int rlev = monster_level_idx(m_idx);

    monspell_message(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s���ޥ��å����ߥ�����μ�ʸ�򾧤�����", "%^s casts a magic missile."),
        _("%^s��%s�˸����äƥޥ��å����ߥ�����μ�ʸ�򾧤�����", "%^s casts a magic missile at %s."),
        TARGET_TYPE);

    dam = damroll(2, 6) + (rlev / 3);
    bolt(m_idx, y, x, GF_MISSILE, dam, MS_MAGIC_MISSILE, TARGET_TYPE);
    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        update_smart_learn(m_idx, DRS_REFLECT);
    }
    return dam;
}

void spell_badstatus_message(int m_idx, int t_idx, cptr msg1, cptr msg2, cptr msg3, cptr msg4, bool resist, bool saving_throw, int TARGET_TYPE)
{
    bool see_either = see_monster(m_idx) || see_monster(t_idx);
    bool see_t = see_monster(t_idx);
    bool known = monster_near_player(m_idx, t_idx);
    char m_name[80], t_name[80];
    monster_name(m_idx, m_name);
    monster_name(t_idx, t_name);

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        disturb(1, 1);
        if (p_ptr->blind)
            msg_format(msg1, m_name);
        else
            msg_format(msg2, m_name);

        if (resist)
        {
            msg_print(msg3);
        }
        else if (saving_throw)
        {
            msg_print(msg4);
        }
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        if (known)
        {
            if (see_either)
            {
                msg_format(msg1, m_name, t_name);
            }
            else
            {
                mon_fight = TRUE;
            }
        }

        if (resist)
        {
            if (see_t) msg_format(msg2, t_name);
        }
        else if (saving_throw)
        {
            if (see_t) msg_format(msg3, t_name);
        }
        else
        {
            if (see_t) msg_format(msg4, t_name);
        }
        monster_wakeup(t_idx);
    }
}

void spell_RF5_SCARE(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    bool resist, saving_throw;

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        resist = p_ptr->resist_fear;
        saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
        spell_badstatus_message(m_idx, t_idx,
            _("%^s��������Ĥ֤䤯�ȡ��������ʲ���ʹ��������", "%^s mumbles, and you hear scary noises."),
            _("%^s���������ʸ��Ф���Ф�����", "%^s casts a fearful illusion."),
            _("���������ݤ˿�����ʤ��ä���", "You refuse to be frightened."),
            _("���������ݤ˿�����ʤ��ä���", "You refuse to be frightened."),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_afraid(p_ptr->afraid + randint0(4) + 4);
        }
        learn_spell(MS_SCARE);
        update_smart_learn(m_idx, DRS_FEAR);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        resist = tr_ptr->flags3 & RF3_NO_FEAR;
        saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

        spell_badstatus_message(m_idx, t_idx, 
            _("%^s���������ʸ��Ф���Ф�����", "%^s casts a fearful illusion in front of %s."),
            _("%^s�϶��ݤ򴶤��ʤ���", "%^s refuses to be frightened."),
            _("%^s�϶��ݤ򴶤��ʤ���", "%^s refuses to be frightened."),
            _("%^s�϶��ݤ���ƨ���Ф�����", "%^s flees in terror!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            set_monster_monfear(t_idx, MON_MONFEAR(t_ptr) + randint0(4) + 4);
        }
    }
}

void spell_RF5_BLIND(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    bool resist, saving_throw;

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        resist = p_ptr->resist_blind;
        saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
        spell_badstatus_message(m_idx, t_idx,
            _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
            _("%^s����ʸ�򾧤��Ƥ��ʤ����ܤ򤯤�ޤ�����", "%^s casts a spell, burning your eyes!"),
            _("���������̤��ʤ��ä���", "You are unaffected!"),
            _("���������Ϥ�ķ���֤�����", "You resist the effects!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_blind(12 + randint0(4));
        }
        learn_spell(MS_BLIND);
        update_smart_learn(m_idx, DRS_BLIND);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        cptr msg1;
        char t_name[80];
        monster_name(t_idx, t_name);
        
        if (streq(t_name, "it"))
        {
            msg1 = _("%s�ϼ�ʸ�򾧤���%s���ܤ�Ƥ��դ�������", "%^s casts a spell, burning %ss eyes.");
        }
        else
        {
            msg1 = _("%s�ϼ�ʸ�򾧤���%s���ܤ�Ƥ��դ�������", "%^s casts a spell, burning %s's eyes.");
        }

        resist = tr_ptr->flags3 & RF3_NO_CONF;
        saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

        spell_badstatus_message(m_idx, t_idx,
            msg1,
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
            _("%^s���ܤ������ʤ��ʤä��� ", "%^s is blinded!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_monster_confused(t_idx, MON_CONFUSED(t_ptr) + 12 + randint0(4));
        }
    }
}

void spell_RF5_CONF(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    bool resist, saving_throw;

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        resist = p_ptr->resist_conf;
        saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
        spell_badstatus_message(m_idx, t_idx,
            _("%^s��������Ĥ֤䤯�ȡ�Ƭ��Ǻ�ޤ�����������", "%^s mumbles, and you hear puzzling noises."),
            _("%^s��Ͷ��Ū�ʸ��Ф���Ф�����", "%^s creates a mesmerising illusion."),
            _("���������ФˤϤ��ޤ���ʤ��ä���", "You disbelieve the feeble spell."),
            _("���������ФˤϤ��ޤ���ʤ��ä���", "You disbelieve the feeble spell."),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_confused(p_ptr->confused + randint0(4) + 4);
        }
        learn_spell(MS_CONF);
        update_smart_learn(m_idx, DRS_CONF);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        resist = tr_ptr->flags3 & RF3_NO_CONF;
        saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

        spell_badstatus_message(m_idx, t_idx,
            _("%^s��%s�����˸���Ū�ʸ���Ĥ���Ф�����", "%^s casts a mesmerizing illusion in front of %s."),
            _("%^s���Ǥ蘆��ʤ��ä���", "%^s disbelieves the feeble spell."),
            _("%^s���Ǥ蘆��ʤ��ä���", "%^s disbelieves the feeble spell."),
            _("%^s�Ϻ��𤷤��褦����", "%^s seems confused."),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_monster_confused(t_idx, MON_CONFUSED(t_ptr) + 12 + randint0(4));
        }
    }
}


void spell_RF5_SLOW(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    bool resist, saving_throw;

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        resist = p_ptr->resist_conf;
        saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
        spell_badstatus_message(m_idx, t_idx,
            _("%^s�����ʤ��ζ��Ϥ�ۤ�����Ȥ�����", "%^s drains power from your muscles!"),
            _("%^s�����ʤ��ζ��Ϥ�ۤ�����Ȥ�����", "%^s drains power from your muscles!"),
            _("���������̤��ʤ��ä���", "You are unaffected!"),
            _("���������Ϥ�ķ���֤�����", "You resist the effects!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_slow(p_ptr->slow + randint0(4) + 4, FALSE);
        }
        learn_spell(MS_SLOW);
        update_smart_learn(m_idx, DRS_FREE);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        cptr msg1;
        char t_name[80];
        monster_name(t_idx, t_name);

        if (streq(t_name, "it"))
        {
            msg1 = _("%s��%s�ζ��������Ϥ�ۤ��Ȥä���", "%^s drains power from %ss muscles.");
        }
        else
        {
            msg1 = _("%s��%s�ζ��������Ϥ�ۤ��Ȥä���", "%^s drains power from %s's muscles.");
        }

        resist = tr_ptr->flags1 & RF1_UNIQUE;
        saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

        spell_badstatus_message(m_idx, t_idx,
            msg1,
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
            _("%s��ư�����٤��ʤä���", "%^s starts moving slower."),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            set_monster_slow(t_idx, MON_SLOW(t_ptr) + 50);
        }
    }
}

void spell_RF5_HOLD(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    bool resist, saving_throw;

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        resist = p_ptr->free_act;
        saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
        spell_badstatus_message(m_idx, t_idx,
            _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
            _("%^s�����ʤ����ܤ򤸤äȸ��Ĥ᤿��", "%^s stares deep into your eyes!"),
            _("���������̤��ʤ��ä���", "You are unaffected!"),
            _("���������Ϥ�ķ���֤�����", "You resist the effects!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_paralyzed(p_ptr->paralyzed + randint0(4) + 4);
        }
        learn_spell(MS_SLEEP);
        update_smart_learn(m_idx, DRS_FREE);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        resist = (tr_ptr->flags1 & RF1_UNIQUE) || (tr_ptr->flags3 & RF3_NO_STUN);
        saving_throw = (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

        spell_badstatus_message(m_idx, t_idx,
            _("%^s��%s�򤸤äȸ��Ĥ᤿��", "%^s stares intently at %s."),
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."),
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected."), 
            _("%^s�����㤷����", "%^s is paralyzed!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
            (void)set_monster_stunned(t_idx, MON_STUNNED(t_ptr) + randint1(4) + 4);
        }
    }
}

void spell_RF6_HASTE(int m_idx, int t_idx, int TARGET_TYPE)
{
    bool see_m = see_monster(m_idx);
    monster_type    *m_ptr = &m_list[m_idx];
	char m_name[80];
    monster_name(m_idx, m_name);

    monspell_message_base(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s����ʬ���Τ�ǰ�����ä���", "%^s concentrates on %s body."),
        _("%^s����ʬ���Τ�ǰ�����ä���", "%^s concentrates on %s body."),
        _("%^s����ʬ���Τ�ǰ�����ä���", "%^s concentrates on %s body."),
        p_ptr->blind, TARGET_TYPE);

    /* Allow quick speed increases to base+10 */
    if (set_monster_fast(m_idx, MON_FAST(m_ptr) + 100))
    {
        if (TARGET_TYPE == MONSTER_TO_PLAYER ||
            (TARGET_TYPE == MONSTER_TO_MONSTER && see_m))
            msg_format(_("%^s��ư����®���ʤä���", "%^s starts moving faster."), m_name);
    }
}

int spell_RF6_HAND_DOOM(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    int dam;

    simple_monspell_message(m_idx, t_idx,
        _("%^s��<���Ǥμ�>�����ä���", "%^s invokes the Hand of Doom!"),
        _("%^s��%s��<���Ǥμ�>�����ä���", "%^s invokes the Hand of Doom upon %s!"),
        TARGET_TYPE);

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        dam = (((s32b)((40 + randint1(20)) * (p_ptr->chp))) / 100);
        breath(y, x, m_idx, GF_HAND_DOOM, dam, 0, FALSE, MS_HAND_DOOM, MONSTER_TO_PLAYER);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        dam = 20; /* Dummy power */
        breath(y, x, m_idx, GF_HAND_DOOM, dam, 0, FALSE, MS_HAND_DOOM, MONSTER_TO_MONSTER);
    }
    return dam;
}

void spell_RF6_HEAL(int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    int rlev = monster_level_idx(m_idx);
    bool seen = (!p_ptr->blind && m_ptr->ml);
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    /* Message */
    monspell_message_base(m_idx, t_idx,
        _("%^s��������Ĥ֤䤤����", "%^s mumbles."),
        _("%^s�ϼ�ʬ�ν���ǰ���椷����", "%^s concentrates on %s wounds."),
        _("%^s����ʬ�ν��˽��椷����", "%^s concentrates on %s wounds."),
        _("%^s�ϼ�ʬ�ν���ǰ���椷����", "%^s concentrates on %s wounds."),
        p_ptr->blind, TARGET_TYPE);

    /* Heal some */
    m_ptr->hp += (rlev * 6);

    /* Fully healed */
    if (m_ptr->hp >= m_ptr->maxhp)
    {
        /* Fully healed */
        m_ptr->hp = m_ptr->maxhp;

        /* Message */
        monspell_message_base(m_idx, t_idx,
            _("%^s�ϴ����˼��ä��褦����", "%^s sounds completely healed!"),
            _("%^s�ϴ����˼��ä��褦����", "%^s sounds completely healed!"),
            _("%^s�ϴ����˼��ä���", "%^s looks completely healed!"),
            _("%^s�ϴ����˼��ä���", "%^s looks completely healed!"),
            !seen, TARGET_TYPE);
    }

    /* Partially healed */
    else
    {
        /* Message */
        monspell_message_base(m_idx, t_idx,
            _("%^s�����Ϥ���������褦����", "%^s sounds healthier."),
            _("%^s�����Ϥ���������褦����", "%^s sounds healthier."),
            _("%^s�����Ϥ���������褦����", "%^s looks healthier."),
            _("%^s�����Ϥ���������褦����", "%^s looks healthier."),
            !seen, TARGET_TYPE);
    }

    /* Redraw (later) if needed */
    if (p_ptr->health_who == m_idx) p_ptr->redraw |= (PR_HEALTH);
    if (p_ptr->riding == m_idx) p_ptr->redraw |= (PR_UHEALTH);

    /* Cancel fear */
    if (MON_MONFEAR(m_ptr))
    {
        /* Cancel fear */
        (void)set_monster_monfear(m_idx, 0);

        /* Message */
        if (see_monster(m_idx))
            msg_format(_("%^s��ͦ�������ᤷ����", "%^s recovers %s courage."), m_name);
    }
}
void spell_RF6_INVULNER(int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    bool seen = (!p_ptr->blind && m_ptr->ml);

    /* Message */
	monspell_message_base(m_idx, t_idx,
            _("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
            _("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."),
            _("%s��̵���ε�μ�ʸ�򾧤�����", "%^s casts a Globe of Invulnerability."),
            _("%s��̵���ε�μ�ʸ�򾧤�����", "%^s casts a Globe of Invulnerability."),
            !seen, TARGET_TYPE);

    if (!MON_INVULNER(m_ptr)) (void)set_monster_invulner(m_idx, randint1(4) + 4, FALSE);
}

void spell_RF6_BLINK(int m_idx, int TARGET_TYPE)
{
	char m_name[80];
    monster_name(m_idx, m_name);
	
	if (TARGET_TYPE==MONSTER_TO_PLAYER)
		disturb(1, 1);

    if (teleport_barrier(m_idx))
    {
		if(see_monster(m_idx))
	        msg_format(_("��ˡ�ΥХꥢ��%^s�Υƥ�ݡ��Ȥ���⤷����",
						 "Magic barrier obstructs teleporting of %^s."), m_name);
    }
    else
    {
		if(see_monster(m_idx))
	        msg_format(_("%^s���ֻ��˾ä�����", "%^s blinks away."), m_name);

        teleport_away(m_idx, 10, 0L);

		if (TARGET_TYPE==MONSTER_TO_PLAYER)
	        p_ptr->update |= (PU_MONSTERS);
    }
}

void spell_RF6_TPORT(int m_idx, int TARGET_TYPE)
{	
	char m_name[80];
    monster_name(m_idx, m_name);
	
	if (TARGET_TYPE==MONSTER_TO_PLAYER)
		disturb(1, 1);
    if (teleport_barrier(m_idx))
    {
		if(see_monster(m_idx))
			msg_format(_("��ˡ�ΥХꥢ��%^s�Υƥ�ݡ��Ȥ���⤷����",
						 "Magic barrier obstructs teleporting of %^s."), m_name);
    }
    else
    {
		if(see_monster(m_idx))
			msg_format(_("%^s���ƥ�ݡ��Ȥ�����", "%^s teleports away."), m_name);

        teleport_away_followable(m_idx);
    }
}

int spell_RF6_WORLD(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    int who = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);
    if (m_ptr->r_idx == MON_DIO) who = 1;
    else if (m_ptr->r_idx == MON_WONG) who = 3;
    if (!process_the_world(randint1(2) + 2, who, TRUE)) return (FALSE);
    return who;
}

int spell_RF6_SPECIAL_BANORLUPART(int m_idx, int mode)
{
    monster_type    *m_ptr = &m_list[m_idx];
	int dummy_hp, dummy_maxhp, k;
	int dummy_y = m_ptr->fy;
	int dummy_x = m_ptr->fx;

	switch(m_ptr->r_idx)
	{
		case MON_BANORLUPART:
			dummy_hp = (m_ptr->hp + 1) / 2;
			dummy_maxhp = m_ptr->maxhp / 2;
			
			if (p_ptr->inside_arena || p_ptr->inside_battle || !summon_possible(m_ptr->fy, m_ptr->fx)) 
				return -1;

			delete_monster_idx(cave[m_ptr->fy][m_ptr->fx].m_idx);
			summon_named_creature(0, dummy_y, dummy_x, MON_BANOR, mode);
			m_list[hack_m_idx_ii].hp = dummy_hp;
			m_list[hack_m_idx_ii].maxhp = dummy_maxhp;
			summon_named_creature(0, dummy_y, dummy_x, MON_LUPART, mode);
			m_list[hack_m_idx_ii].hp = dummy_hp;
			m_list[hack_m_idx_ii].maxhp = dummy_maxhp;

			msg_print(_("�إС��Ρ��롦��ѡ��ȡ٤�ʬ��������","Banor=Rupart splits in two person!"));
			break;
		
        case MON_BANOR:
        case MON_LUPART:
            dummy_hp = 0;
            dummy_maxhp = 0;

            if (!r_info[MON_BANOR].cur_num || !r_info[MON_LUPART].cur_num) 
				return -1;

            for (k = 1; k < m_max; k++)
            {
                if (m_list[k].r_idx == MON_BANOR || m_list[k].r_idx == MON_LUPART)
                {
                    dummy_hp += m_list[k].hp;
                    dummy_maxhp += m_list[k].maxhp;
                    if (m_list[k].r_idx != m_ptr->r_idx)
                    {
                        dummy_y = m_list[k].fy;
                        dummy_x = m_list[k].fx;
                    }
                    delete_monster_idx(k);
                }
            }
            summon_named_creature(0, dummy_y, dummy_x, MON_BANORLUPART, mode);
            m_list[hack_m_idx_ii].hp = dummy_hp;
            m_list[hack_m_idx_ii].maxhp = dummy_maxhp;

            msg_print(_("�إС��Ρ���٤ȡإ�ѡ��ȡ٤����Τ�����", "Banor and Rupart combine into one!"));
            break;
	}
	return 0;
}


int spell_RF6_SPECIAL_ROLENTO(int y, int x, int m_idx, int t_idx, int mode, int TARGET_TYPE)
{
	int count = 0, k;
	int num = 1 + randint1(3);
	
	monspell_message(m_idx, t_idx,
		_("%^s���������̤��ꤲ����", "%^s spreads something."),
		_("%^s�ϼ����Ƥ�Ф�ޤ�����", "%^s throws some hand grenades."),
        _("%^s�ϼ����Ƥ�Ф�ޤ�����", "%^s throws some hand grenades."),
        TARGET_TYPE);

	for (k = 0; k < num; k++)
	{
		count += summon_named_creature(m_idx, y, x, MON_SHURYUUDAN, mode);
	}
	
	if (p_ptr->blind && count)
		msg_print(_("¿���Τ�Τ��ֶ�ˤФ�ޤ���벻�����롣", "You hear many things are scattered nearby."));
	
	return 0;
}

int spell_RF6_SPECIAL_B(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
	int dam;
    monster_type    *m_ptr = &m_list[m_idx];
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
	bool monster_to_player = (TARGET_TYPE == MONSTER_TO_PLAYER);
	bool monster_to_monster = (TARGET_TYPE == MONSTER_TO_MONSTER);
    bool direct = player_bold(y, x);
	char m_name[80];
    monster_name(m_idx, m_name);

	disturb(1, 1);
	if (one_in_(3) || !direct)
	{		
		simple_monspell_message(m_idx, t_idx,
			_("%^s�������볦����ä���!", "%^s suddenly go out of your sight!"),
			_("%^s�������޾徺���ƻ볦����ä���!", "%^s suddenly go out of your sight!"),
			TARGET_TYPE);
				
		teleport_away(m_idx, 10, TELEPORT_NONMAGICAL);
		p_ptr->update |= (PU_MONSTERS);
	}
	else
	{
		int get_damage = 0;
		bool fear; /* dummy */
	
		simple_monspell_message(m_idx, t_idx,
			_("%^s�����ʤ����Ϥ�Ƕ��椫���ꤲ��Ȥ�����", "%^s holds you, and drops from the sky."),
			_("%^s��%s���Ϥ�Ƕ��椫���ꤲ��Ȥ�����", "%^s holds %s, and drops from the sky."),
			TARGET_TYPE);

		dam = damroll(4, 8);

		if (monster_to_player || t_idx == p_ptr->riding)
			teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_NONMAGICAL | TELEPORT_PASSIVE);
		else 
			teleport_monster_to(t_idx, m_ptr->fy, m_ptr->fx, 100, TELEPORT_NONMAGICAL | TELEPORT_PASSIVE);

		sound(SOUND_FALL);

		if ((monster_to_player && p_ptr->levitation) ||
			(monster_to_monster && (tr_ptr->flags7 & RF7_CAN_FLY)))
		{
			simple_monspell_message(m_idx, t_idx,
				_("���ʤ����Ť������Ϥ�����", "You float gently down to the ground."),
				_("%^s���Ť������Ϥ�����", "%^s floats gently down to the ground."),
				TARGET_TYPE);
		}
		else
		{
			simple_monspell_message(m_idx, t_idx,
				_("���ʤ������̤�á���Ĥ���줿��", "You crashed into the ground."),
				_("%^s�����̤�á���Ĥ���줿��", "%^s crashed into the ground."),
				TARGET_TYPE);
			dam += damroll(6, 8);
		}

		if(monster_to_player ||
		   (monster_to_monster && p_ptr->riding == t_idx))
		{
			/* Mega hack -- this special action deals damage to the player. Therefore the code of "eyeeye" is necessary.
			-- henkma
			*/
			get_damage = take_hit(DAMAGE_NOESCAPE, dam, m_name, -1);
			if (p_ptr->tim_eyeeye && get_damage > 0 && !p_ptr->is_dead)
			{
				char m_name_self[80];
				/* hisself */
				monster_desc(m_name_self, m_ptr, MD_PRON_VISIBLE | MD_POSSESSIVE | MD_OBJECTIVE);

				msg_format(_("���⤬%s���Ȥ���Ĥ�����", "The attack of %s has wounded %s!"), m_name, m_name_self);

				project(0, 0, m_ptr->fy, m_ptr->fx, get_damage, GF_MISSILE, PROJECT_KILL, -1);
				set_tim_eyeeye(p_ptr->tim_eyeeye - 5, TRUE);
			}
		}

		if(monster_to_player && p_ptr->riding)
			mon_take_hit_mon(p_ptr->riding, dam, &fear, extract_note_dies(real_r_ptr(&m_list[p_ptr->riding])), m_idx);

		if(monster_to_monster)
			mon_take_hit_mon(t_idx, dam, &fear, extract_note_dies(real_r_ptr(t_ptr)), m_idx);
	}
	return dam;
}

int spell_RF6_SPECIAL(int y, int x, int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    u32b mode = 0L;
    int count=0;

    disturb(1, 1);
    switch (m_ptr->r_idx)
    {
        case MON_OHMU:
            /* Moved to process_monster(), like multiplication */
            return -1;

        case MON_BANORLUPART:
        case MON_BANOR:
        case MON_LUPART:
			return spell_RF6_SPECIAL_BANORLUPART(m_idx, mode);

        case MON_ROLENTO:
			return spell_RF6_SPECIAL_ROLENTO(y, x, m_idx, t_idx, mode, TARGET_TYPE);
            break;

        default:
        if (r_ptr->d_char == 'B')
        {
            return spell_RF6_SPECIAL_B(y, x, m_idx, t_idx, TARGET_TYPE);
            break;
        }

        /* Something is wrong */
        else return -1;
    }
}

void spell_RF6_TELE_TO(int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];

	simple_monspell_message(m_idx, t_idx,
		_("%^s�����ʤ�������ᤷ����", "%^s commands you to return."),
		_("%^s��%s������ᤷ����", "%^s commands %s to return."),
		TARGET_TYPE);
	
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_PASSIVE);
		learn_spell(MS_TELE_TO);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		bool resists_tele = FALSE;
		char t_name[80];
		monster_name(t_idx, t_name);

		if (tr_ptr->flagsr & RFR_RES_TELE)
		{
			if ((tr_ptr->flags1 & RF1_UNIQUE) || (tr_ptr->flagsr & RFR_RES_ALL))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected!"), t_name);
				}
				resists_tele = TRUE;
			}
			else if (tr_ptr->level > randint1(100))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s����������äƤ��롪", "%^s resists!"), t_name);
				}
				resists_tele = TRUE;
			}
		}

		if (!resists_tele)
		{
			if (t_idx == p_ptr->riding) 
				teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_PASSIVE);
			else 
				teleport_monster_to(t_idx, m_ptr->fy, m_ptr->fx, 100, TELEPORT_PASSIVE);
		}
        monster_wakeup(t_idx);
	}
}

void spell_RF6_TELE_AWAY(int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];

	simple_monspell_message(m_idx, t_idx,
		_("%^s�˥ƥ�ݡ��Ȥ�����줿��", "%^s teleports you away."),
		_("%^s��%s��ƥ�ݡ��Ȥ�������", "%^s teleports %s away."),
		TARGET_TYPE);
	
	if (TARGET_TYPE == MONSTER_TO_PLAYER)
	{
		if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
			msg_print(_("���ä���", ""));
		
		learn_spell(MS_TELE_AWAY);
		teleport_player_away(m_idx, 100);
	}
	else if (TARGET_TYPE == MONSTER_TO_MONSTER)
	{
		bool resists_tele = FALSE;
		char t_name[80];
		monster_name(t_idx, t_name);

		if (tr_ptr->flagsr & RFR_RES_TELE)
		{
			if ((tr_ptr->flags1 & RF1_UNIQUE) || (tr_ptr->flagsr & RFR_RES_ALL))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected!"), t_name);
				}
				resists_tele = TRUE;
			}
			else if (tr_ptr->level > randint1(100))
			{
				if (is_original_ap_and_seen(t_ptr)) tr_ptr->r_flagsr |= RFR_RES_TELE;
				if (see_monster(t_idx))
				{
					msg_format(_("%^s����������äƤ��롪", "%^s resists!"), t_name);
				}
				resists_tele = TRUE;
			}
		}

		if (!resists_tele)
		{
			if (t_idx == p_ptr->riding) 
				teleport_player_away(m_idx, MAX_SIGHT * 2 + 5);
			else 
				teleport_away(t_idx, MAX_SIGHT * 2 + 5, TELEPORT_PASSIVE);
		}
        monster_wakeup(t_idx);
	}
}

void spell_RF6_TELE_LEVEL(int m_idx, int t_idx, int TARGET_TYPE)
{
    monster_type    *t_ptr = &m_list[t_idx];
    monster_race    *tr_ptr = &r_info[t_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    bool resist, saving_throw;

    if (TARGET_TYPE == MONSTER_TO_PLAYER)
    {
        resist = p_ptr->resist_nexus;
        saving_throw = (randint0(100 + rlev / 2) < p_ptr->skill_sav);
        spell_badstatus_message(m_idx, t_idx,
            _("%^s��������̯�ʸ��դ�Ĥ֤䤤����", "%^s mumbles strangely."),
            _("%^s�����ʤ���­��ؤ�������", "%^s gestures at your feet."),
            _("���������̤��ʤ��ä���", "You are unaffected!"),
            _("���������Ϥ�ķ���֤�����", "You resist the effects!"),
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
			teleport_level(0);
        }
		learn_spell(MS_TELE_LEVEL);
		update_smart_learn(m_idx, DRS_NEXUS);
    }
    else if (TARGET_TYPE == MONSTER_TO_MONSTER)
    {
        resist = tr_ptr->flagsr & (RFR_EFF_RES_NEXU_MASK | RFR_RES_TELE);
        saving_throw = (tr_ptr->flags1 & RF1_QUESTOR) ||
			           (tr_ptr->level > randint1((rlev - 10) < 1 ? 1 : (rlev - 10)) + 10);

        spell_badstatus_message(m_idx, t_idx, 
            _("%^s��%s��­��ؤ�������", "%^s gestures at %s's feet."),
            _("%^s�ˤϸ��̤��ʤ��ä���", "%^s is unaffected!"),
            _("%^s�ϸ��Ϥ�ķ���֤�����", "%^s resist the effects!"),
            "",
            resist, saving_throw, TARGET_TYPE);

        if (!resist && !saving_throw)
        {
			teleport_level((t_idx == p_ptr->riding) ? 0 : t_idx);
        }
    }
}

int spell_RF6_PSY_SPEAR(int y, int x, int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
	int rlev = monster_level_idx(m_idx);
    int dam;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);
    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����η������ä���", "%^s throw a Psycho-Spear."), m_name);

    dam = (r_ptr->flags2 & RF2_POWERFUL) ? (randint1(rlev * 2) + 150) : (randint1(rlev * 3 / 2) + 100);
    beam(m_idx, y, x, GF_PSY_SPEAR, dam, MS_PSY_SPEAR, MONSTER_TO_PLAYER);
    return dam;
}

void spell_RF6_DARKNESS(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    bool can_use_lite_area = FALSE;
	char m_name[80];
    monster_name(m_idx, m_name);

    if ((p_ptr->pclass == CLASS_NINJA) &&
        !(r_ptr->flags3 & (RF3_UNDEAD | RF3_HURT_LITE)) &&
        !(r_ptr->flags7 & RF7_DARK_MASK))
        can_use_lite_area = TRUE;
    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else if (can_use_lite_area)
        msg_format(_("%^s���դ�����뤯�Ȥ餷����", "%^s cast a spell to light up."), m_name);
    else
        msg_format(_("%^s���ŰǤ���Ǽ�򿶤ä���", "%^s gestures in shadow."), m_name);

    if (can_use_lite_area)
    {
        (void)lite_area(0, 3);
    }
    else
    {
        learn_spell(MS_DARKNESS);
        (void)unlite_area(0, 3);
    }
}

void spell_RF6_TRAPS(int y, int x, int m_idx)
{
	char m_name[80];
    monster_name(m_idx, m_name);
    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤�Ƽٰ������Ф����",
        "%^s mumbles, and then cackles evilly."), m_name);
    else
        msg_format(_("%^s����ʸ�򾧤��Ƽٰ������Ф����",
        "%^s casts a spell and cackles evilly."), m_name);

    learn_spell(MS_MAKE_TRAP);
    (void)trap_creation(y, x);
}

void spell_RF6_FORGET(int m_idx)
{
    int rlev = monster_level_idx(m_idx);
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    msg_format(_("%^s�����ʤ��ε�����õ�褦�Ȥ��Ƥ��롣",
        "%^s tries to blank your mind."), m_name);

    if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������Ϥ�ķ���֤�����", "You resist the effects!"));
    }
    else if (lose_all_info())
    {
        msg_print(_("����������Ƥ��ޤä���", "Your memories fade away."));
    }
    learn_spell(MS_FORGET);
}

void spell_RF6_RAISE_DEAD(int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���������μ�ʸ�򾧤�����",
        "%^s casts a spell to revive corpses."), m_name);

    animate_dead(m_idx, m_ptr->fy, m_ptr->fx);
}

void spell_RF6_S_KIN(int y, int x, int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    int count = 0, k;
    u32b mode = 0L;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);
    if (m_ptr->r_idx == MON_SERPENT || m_ptr->r_idx == MON_ZOMBI_SERPENT)
    {
        if (p_ptr->blind)
            msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s�����󥸥��μ�򾤴�������",
            "%^s magically summons guardians of dungeons."), m_name);
    }
    else
    {
        if (p_ptr->blind)
            msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
        else
#ifdef JP
            msg_format("%^s����ˡ��%s�򾤴�������",
            m_name,
            ((r_ptr->flags1) & RF1_UNIQUE ?
            "�겼" : "���"));
#else
            msg_format("%^s magically summons %s %s.",
            m_name, m_poss,
            ((r_ptr->flags1) & RF1_UNIQUE ?
            "minions" : "kin"));
#endif
    }

    switch (m_ptr->r_idx)
    {
        case MON_MENELDOR:
        case MON_GWAIHIR:
        case MON_THORONDOR:
        {
            int num = 4 + randint1(3);
            for (k = 0; k < num; k++)
            {
                count += summon_specific(m_idx, y, x, rlev, SUMMON_EAGLES, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
            }
        }
        break;

        case MON_BULLGATES:
        {
            int num = 2 + randint1(3);
            for (k = 0; k < num; k++)
            {
                count += summon_named_creature(m_idx, y, x, MON_IE, mode);
            }
        }
        break;

        case MON_SERPENT:
        case MON_ZOMBI_SERPENT:
        {
            int num = 2 + randint1(3);

            if (r_info[MON_JORMUNGAND].cur_num < r_info[MON_JORMUNGAND].max_num && one_in_(6))
            {
                msg_print(_("���̤���夬�᤭�Ф�����", "Water blew off from the ground!"));
                fire_ball_hide(GF_WATER_FLOW, 0, 3, 8);
            }

            for (k = 0; k < num; k++)
            {
                count += summon_specific(m_idx, y, x, rlev, SUMMON_GUARDIANS, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
            }
        }
        break;

        case MON_CALDARM:
        {
            int num = randint1(3);
            for (k = 0; k < num; k++)
            {
                count += summon_named_creature(m_idx, y, x, MON_LOCKE_CLONE, mode);
            }
        }
        break;

        case MON_LOUSY:
        {
            int num = 2 + randint1(3);
            for (k = 0; k < num; k++)
            {
                count += summon_specific(m_idx, y, x, rlev, SUMMON_LOUSE, PM_ALLOW_GROUP);
            }
        }
        break;

        default:
        summon_kin_type = r_ptr->d_char; /* Big hack */

        for (k = 0; k < 4; k++)
        {
            count += summon_specific(m_idx, y, x, rlev, SUMMON_KIN, PM_ALLOW_GROUP);
        }
        break;
    }
    
    if (p_ptr->blind && count)
        msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
}

void spell_RF6_S_CYBER(int y, int x, int m_idx)
{
    int count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�������С��ǡ����򾤴�������",
        "%^s magically summons Cyberdemons!"), m_name);

    count = summon_cyber(m_idx, y, x);

    if (p_ptr->blind && count)
        msg_print(_("�Ÿ���­�����᤯��ʹ�����롣", "You hear heavy steps nearby."));
}

void spell_RF6_S_MONSTER(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ����֤򾤴�������", "%^s magically summons help!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }
    if (p_ptr->blind && count)
        msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
}

void spell_RF6_S_MONSTERS(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥ�󥹥����򾤴�������", "%^s magically summons monsters!"), m_name);

    for (k = 0; k < S_NUM_6; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }

    if (p_ptr->blind && count)
        msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
}

void spell_RF6_S_ANT(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥ���򾤴�������", "%^s magically summons ants."), m_name);

    for (k = 0; k < S_NUM_6; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_ANT, PM_ALLOW_GROUP);
    }

    if (p_ptr->blind && count)
        msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
}

void spell_RF6_S_SPIDER(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥ���򾤴�������", "%^s magically summons spiders."), m_name);

    for (k = 0; k < S_NUM_6; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_SPIDER, PM_ALLOW_GROUP);
    }

    if (p_ptr->blind && count)
        msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
}

void spell_RF6_S_HOUND(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥϥ���ɤ򾤴�������", "%^s magically summons hounds."), m_name);

    for (k = 0; k < S_NUM_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_HOUND, PM_ALLOW_GROUP);
    }

    if (p_ptr->blind && count)
        msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
}

void spell_RF6_S_HYDRA(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥҥɥ�򾤴�������", "%^s magically summons hydras."), m_name);

    for (k = 0; k < S_NUM_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_HYDRA, PM_ALLOW_GROUP);
    }
    if (p_ptr->blind && count)
        msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
}

void spell_RF6_S_ANGEL(int y, int x, int m_idx)
{
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
    int num = 1;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ��ŷ�Ȥ򾤴�������", "%^s magically summons an angel!"), m_name);

    if ((r_ptr->flags1 & RF1_UNIQUE) && !easy_band)
    {
        num += r_ptr->level / 40;
    }

    for (k = 0; k < num; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_ANGEL, PM_ALLOW_GROUP);
    }

    if (count < 2)
    {
        if (p_ptr->blind && count)
            msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
    }
    else
    {
        if (p_ptr->blind)
            msg_print(_("¿���Τ�Τ��ֶ�˸��줿�������롣", "You hear many things appear nearby."));
    }
}

void spell_RF6_S_DEMON(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�Ǻ��٤ε���鰭��򾤴�������",
        "%^s magically summons a demon from the Courts of Chaos!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_DEMON, PM_ALLOW_GROUP);
    }

    if (p_ptr->blind && count)
        msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
}

void spell_RF6_S_UNDEAD(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥ���ǥåɤζ�Ũ�򾤴�������",
        "%^s magically summons an undead adversary!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_UNDEAD, PM_ALLOW_GROUP);
    }

    if (p_ptr->blind && count)
        msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
}

void spell_RF6_S_DRAGON(int y, int x, int m_idx)
{
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
	char m_name[80];
    monster_name(m_idx, m_name);

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�ǥɥ饴��򾤴�������", "%^s magically summons a dragon!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_DRAGON, PM_ALLOW_GROUP);
    }
    if (p_ptr->blind && count)
        msg_print(_("�������ֶ�˸��줿�������롣", "You hear something appear nearby."));
}

void spell_RF6_S_HI_UNDEAD(int y, int x, int m_idx)
{
    char* m_name = monster_name(m_idx, "");
    monster_type    *m_ptr = &m_list[m_idx];
    monster_race    *r_ptr = &r_info[m_ptr->r_idx];
    int rlev = monster_level_idx(m_idx);
    u32b mode = 0L;
    int k, count = 0;
    disturb(1, 1);

    if (((m_ptr->r_idx == MON_MORGOTH) || (m_ptr->r_idx == MON_SAURON) || (m_ptr->r_idx == MON_ANGMAR)) && ((r_info[MON_NAZGUL].cur_num + 2) < r_info[MON_NAZGUL].max_num))
    {
        int cy = y;
        int cx = x;

        if (p_ptr->blind)
            msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s����ˡ��ͩ������򾤴�������", "%^s magically summons rangers of Nazgul!"), m_name);

        msg_print(NULL);

        for (k = 0; k < 30; k++)
        {
            if (!summon_possible(cy, cx) || !cave_empty_bold(cy, cx))
            {
                int j;
                for (j = 100; j > 0; j--)
                {
                    scatter(&cy, &cx, y, x, 2, 0);
                    if (cave_empty_bold(cy, cx)) break;
                }
                if (!j) break;
            }
            if (!cave_empty_bold(cy, cx)) continue;

            if (summon_named_creature(m_idx, cy, cx, MON_NAZGUL, mode))
            {
                y = cy;
                x = cx;
                count++;
                if (count == 1)
                    msg_format(_("��ͩ������%d�桢�ʥ����롦�֥�å�����",
                    "A Nazgul says 'Nazgul-Rangers Number %d, Nazgul-Black!'"), count);
                else
                    msg_format(_("��Ʊ����%d�桢�ʥ����롦�֥�å�����",
                    "Another one says 'Number %d, Nazgul-Black!'"), count);

                msg_print(NULL);
            }
        }
        msg_format(_("��%d�ͤ���äơ���󥰥�󥸥㡼����",
            "They say 'The %d meets! We are the Ring-Ranger!'."), count);
        msg_print(NULL);
    }
    else
    {
        if (p_ptr->blind)
            msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s����ˡ�Ƕ��Ϥʥ���ǥåɤ򾤴�������",
            "%^s magically summons greater undead!"), m_name);

        for (k = 0; k < S_NUM_6; k++)
        {
            count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_UNDEAD, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
        }
    }
    if (p_ptr->blind && count)
    {
        msg_print(_("�ֶ�ǲ���¿���Τ�Τ��礤��벻��ʹ�����롣",
            "You hear many creepy things appear nearby."));
    }
}


void spell_RF6_S_HI_DRAGON(int y, int x, int m_idx)
{
    char* m_name = monster_name(m_idx, "");
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�Ǹ���ɥ饴��򾤴�������", "%^s magically summons ancient dragons!"), m_name);

    for (k = 0; k < S_NUM_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_DRAGON, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }
    if (p_ptr->blind && count)
    {
        msg_print(_("¿�����϶�����Τ��ֶ�˸��줿����ʹ�����롣",
            "You hear many powerful things appear nearby."));
    }
}

void spell_RF6_S_AMBERITES(int y, int x, int m_idx)
{
    char* m_name = monster_name(m_idx, "");
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s������С��β�²�򾤴�������", "%^s magically summons Lords of Amber!"), m_name);

    for (k = 0; k < S_NUM_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_AMBERITES, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }
    if (p_ptr->blind && count)
    {
        msg_print(_("�Ի�μԤ��᤯�˸����Τ�ʹ��������", "You hear immortal beings appear nearby."));
    }
}

void spell_RF6_S_UNIQUE(int y, int x, int m_idx)
{
    char* m_name = monster_name(m_idx, "");
    monster_type    *m_ptr = &m_list[m_idx];
    int rlev = monster_level_idx(m_idx);
    int k, count = 0;
    bool uniques_are_summoned = FALSE;
    int non_unique_type = SUMMON_HI_UNDEAD;

    disturb(1, 1);

    if (p_ptr->blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s����ˡ�����̤ʶ�Ũ�򾤴�������", "%^s magically summons special opponents!"), m_name);

    for (k = 0; k < S_NUM_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_UNIQUE, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }

    if (count) uniques_are_summoned = TRUE;

    if ((m_ptr->sub_align & (SUB_ALIGN_GOOD | SUB_ALIGN_EVIL)) == (SUB_ALIGN_GOOD | SUB_ALIGN_EVIL))
        non_unique_type = 0;
    else if (m_ptr->sub_align & SUB_ALIGN_GOOD)
        non_unique_type = SUMMON_ANGEL;

    for (k = count; k < S_NUM_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, non_unique_type, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }

    if (p_ptr->blind && count)
    {
        msg_format(_("¿����%s���ֶ�˸��줿����ʹ�����롣", "You hear many %s appear nearby."),
            uniques_are_summoned ? _("�϶������", "powerful things") : _("���", "things"));
    }
}


int monspell_to_player(int SPELL_NUM, int y, int x, int m_idx)
{
    switch (SPELL_NUM)
    {
    case 96 + 0:   spell_RF4_SHRIEK(m_idx, 0, MONSTER_TO_PLAYER); break;    /* RF4_SHRIEK */
    case 96 + 1:   break;   /* RF4_XXX1 */
    case 96 + 2:   spell_RF4_DISPEL(m_idx, 0, MONSTER_TO_PLAYER); break;    /* RF4_DISPEL */
    case 96 + 3:   return spell_RF4_ROCKET(y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_ROCKET */
    case 96 + 4:   return spell_RF4_SHOOT(y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_SHOOT */
    case 96 + 5:   break;   /* RF4_XXX2 */
    case 96 + 6:   break;   /* RF4_XXX3 */
    case 96 + 7:   break;   /* RF4_XXX4 */
    case 96 + 8:   return spell_RF4_BREATH(GF_ACID, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_ACID */
    case 96 + 9:   return spell_RF4_BREATH(GF_ELEC, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_ELEC */
    case 96 + 10:  return spell_RF4_BREATH(GF_FIRE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_FIRE */
    case 96 + 11:  return spell_RF4_BREATH(GF_COLD, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_COLD */
    case 96 + 12:  return spell_RF4_BREATH(GF_POIS, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_POIS */
    case 96 + 13:  return spell_RF4_BREATH(GF_NETHER, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_NETH */
    case 96 + 14:  return spell_RF4_BREATH(GF_LITE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_LITE */
    case 96 + 15:  return spell_RF4_BREATH(GF_DARK, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_DARK */
    case 96 + 16:  return spell_RF4_BREATH(GF_CONFUSION, y, x, m_idx, 0, MONSTER_TO_PLAYER);    /* RF4_BR_CONF */
    case 96 + 17:  return spell_RF4_BREATH(GF_SOUND, y, x, m_idx, 0, MONSTER_TO_PLAYER);    /* RF4_BR_SOUN */
    case 96 + 18:  return spell_RF4_BREATH(GF_CHAOS, y, x, m_idx, 0, MONSTER_TO_PLAYER);    /* RF4_BR_CHAO */
    case 96 + 19:  return spell_RF4_BREATH(GF_DISENCHANT, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_DISE */
    case 96 + 20:  return spell_RF4_BREATH(GF_NEXUS, y, x, m_idx, 0, MONSTER_TO_PLAYER);    /* RF4_BR_NEXU */
    case 96 + 21:  return spell_RF4_BREATH(GF_TIME, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_TIME */
    case 96 + 22:  return spell_RF4_BREATH(GF_INERTIA, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_BR_INER */
    case 96 + 23:  return spell_RF4_BREATH(GF_GRAVITY, y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF4_BR_GRAV */
    case 96 + 24:  return spell_RF4_BREATH(GF_SHARDS, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_SHAR */
    case 96 + 25:  return spell_RF4_BREATH(GF_PLASMA, y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF4_BR_PLAS */
    case 96 + 26:  return spell_RF4_BREATH(GF_FORCE, y, x, m_idx, 0, MONSTER_TO_PLAYER);    /* RF4_BR_WALL */
    case 96 + 27:  return spell_RF4_BREATH(GF_MANA, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_MANA */
    case 96 + 28:  return spell_RF4_BA_NUKE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BA_NUKE */
    case 96 + 29:  return spell_RF4_BREATH(GF_NUKE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_NUKE */
    case 96 + 30:  return spell_RF4_BA_CHAO(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BA_CHAO */
    case 96 + 31:  return spell_RF4_BREATH(GF_DISINTEGRATE, y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF4_BR_DISI */
    case 128 + 0:  return spell_RF5_BA_ACID(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_ACID */
    case 128 + 1:  return spell_RF5_BA_ELEC(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_ELEC */
    case 128 + 2:  return spell_RF5_BA_FIRE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_FIRE */
    case 128 + 3:  return spell_RF5_BA_COLD(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_COLD */
    case 128 + 4:  return spell_RF5_BA_POIS(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_POIS */
    case 128 + 5:  return spell_RF5_BA_NETH(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_NETH */
    case 128 + 6:  return spell_RF5_BA_WATE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_WATE */
    case 128 + 7:  return spell_RF5_BA_MANA(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_MANA */
    case 128 + 8:  return spell_RF5_BA_DARK(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_DARK */
    case 128 + 9:  return spell_RF5_DRAIN_MANA(y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF5_DRAIN_MANA */
    case 128 + 10: return spell_RF5_MIND_BLAST(y, x, m_idx, 0, MONSTER_TO_PLAYER);  /* RF5_MIND_BLAST */
    case 128 + 11: return spell_RF5_BRAIN_SMASH(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_MIND_BLAST */
    case 128 + 12: return spell_RF5_CAUSE_1(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_1 */
    case 128 + 13: return spell_RF5_CAUSE_2(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_2 */
    case 128 + 14: return spell_RF5_CAUSE_3(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_3 */
    case 128 + 15: return spell_RF5_CAUSE_4(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_CAUSE_4 */
    case 128 + 16: return spell_RF5_BO_ACID(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ACID */
    case 128 + 17: return spell_RF5_BO_ELEC(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ELEC */
    case 128 + 18: return spell_RF5_BO_FIRE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_FIRE */
    case 128 + 19: return spell_RF5_BO_COLD(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_COLD */
    case 128 + 20: return spell_RF5_BA_LITE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BA_LITE */
    case 128 + 21: return spell_RF5_BO_NETH(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_NETH */
    case 128 + 22: return spell_RF5_BO_WATE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_WATE */
    case 128 + 23: return spell_RF5_BO_MANA(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_MANA */
    case 128 + 24: return spell_RF5_BO_PLAS(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_PLAS */
    case 128 + 25: return spell_RF5_BO_ICEE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_BO_ICEE */
    case 128 + 26: return spell_RF5_MISSILE(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF5_MISSILE */
    case 128 + 27: spell_RF5_SCARE(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF5_SCARE */
    case 128 + 28: spell_RF5_BLIND(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF5_BLIND */
    case 128 + 29: spell_RF5_CONF(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF5_CONF */
    case 128 + 30: spell_RF5_SLOW(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF5_SLOW */
    case 128 + 31: spell_RF5_HOLD(y, x, m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF5_HOLD */
    case 160 + 0:  spell_RF6_HASTE(m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_HASTE */
    case 160 + 1:  return spell_RF6_HAND_DOOM(y, x, m_idx, 0, MONSTER_TO_PLAYER); /* RF6_HAND_DOOM */
    case 160 + 2:  spell_RF6_HEAL(m_idx, 0, MONSTER_TO_PLAYER); break;    /* RF6_HEAL */
    case 160 + 3:  spell_RF6_INVULNER(m_idx, 0, MONSTER_TO_PLAYER); break;    /* RF6_INVULNER */
    case 160 + 4:  spell_RF6_BLINK(m_idx, MONSTER_TO_PLAYER); break;   /* RF6_BLINK */
    case 160 + 5:  spell_RF6_TPORT(m_idx, MONSTER_TO_PLAYER); break;   /* RF6_TPORT */
    case 160 + 6:  return spell_RF6_WORLD(m_idx); break;    /* RF6_WORLD */
    case 160 + 7:  return spell_RF6_SPECIAL(y, x, m_idx, 0, MONSTER_TO_PLAYER);   /* RF6_SPECIAL */
    case 160 + 8:  spell_RF6_TELE_TO(m_idx, 0, MONSTER_TO_PLAYER); break; /* RF6_TELE_TO */
    case 160 + 9:  spell_RF6_TELE_AWAY(m_idx, 0, MONSTER_TO_PLAYER); break;   /* RF6_TELE_AWAY */
    case 160 + 10: spell_RF6_TELE_LEVEL(m_idx, 0, MONSTER_TO_PLAYER); break;  /* RF6_TELE_LEVEL */
    case 160 + 11: spell_RF6_PSY_SPEAR(y, x, m_idx); break; /* RF6_PSY_SPEAR */
    case 160 + 12: spell_RF6_DARKNESS(m_idx); break;    /* RF6_DARKNESS */
    case 160 + 13: spell_RF6_TRAPS(y, x, m_idx); break; /* RF6_TRAPS */
    case 160 + 14: spell_RF6_FORGET(m_idx); break;  /* RF6_FORGET */
    case 160 + 15: spell_RF6_RAISE_DEAD(m_idx); break;  /* RF6_RAISE_DEAD */
    case 160 + 16: spell_RF6_S_KIN(y, x, m_idx); break; /* RF6_S_KIN */
    case 160 + 17: spell_RF6_S_CYBER(y, x, m_idx); break;   /* RF6_S_CYBER */
    case 160 + 18: spell_RF6_S_MONSTER(y, x, m_idx); break; /* RF6_S_MONSTER */
    case 160 + 19: spell_RF6_S_MONSTERS(y, x, m_idx); break;    /* RF6_S_MONSTER */
    case 160 + 20: spell_RF6_S_ANT(y, x, m_idx); break; /* RF6_S_ANT */
    case 160 + 21: spell_RF6_S_SPIDER(y, x, m_idx); break;  /* RF6_S_SPIDER */
    case 160 + 22: spell_RF6_S_HOUND(y, x, m_idx); break;   /* RF6_S_HOUND */
    case 160 + 23: spell_RF6_S_HYDRA(y, x, m_idx); break;   /* RF6_S_HYDRA */
    case 160 + 24: spell_RF6_S_ANGEL(y, x, m_idx); break;   /* RF6_S_ANGEL */
    case 160 + 25: spell_RF6_S_DEMON(y, x, m_idx); break;   /* RF6_S_DEMON */
    case 160 + 26: spell_RF6_S_UNDEAD(y, x, m_idx); break;  /* RF6_S_UNDEAD */
    case 160 + 27: spell_RF6_S_DRAGON(y, x, m_idx); break;  /* RF6_S_DRAGON */
    case 160 + 28: spell_RF6_S_HI_UNDEAD(y, x, m_idx); break;   /* RF6_S_HI_UNDEAD */
    case 160 + 29: spell_RF6_S_HI_DRAGON(y, x, m_idx); break;   /* RF6_S_HI_DRAGON */
    case 160 + 30: spell_RF6_S_AMBERITES(y, x, m_idx); break;   /* RF6_S_AMBERITES */
    case 160 + 31: spell_RF6_S_UNIQUE(y, x, m_idx); break;  /* RF6_S_UNIQUE */
    }
    return 0;
}

int monspell_to_monster(int SPELL_NUM, int y, int x, int m_idx, int t_idx)
{
    switch (SPELL_NUM)
    {
    case 96 + 0:   spell_RF4_SHRIEK(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF4_SHRIEK */
    case 96 + 1:   return -1;   /* RF4_XXX1 */
    case 96 + 2:   spell_RF4_DISPEL(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF4_DISPEL */
    case 96 + 3:   return spell_RF4_ROCKET(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_ROCKET */
    case 96 + 4:   return spell_RF4_SHOOT(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_SHOOT */
    case 96 + 5:   return -1;   /* RF4_XXX2 */
    case 96 + 6:   return -1;   /* RF4_XXX3 */
    case 96 + 7:   return -1;   /* RF4_XXX4 */
    case 96 + 8:   return spell_RF4_BREATH(GF_ACID, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_ACID */
    case 96 + 9:   return spell_RF4_BREATH(GF_ELEC, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_ELEC */
    case 96 + 10:  return spell_RF4_BREATH(GF_FIRE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_FIRE */
    case 96 + 11:  return spell_RF4_BREATH(GF_COLD, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_COLD */
    case 96 + 12:  return spell_RF4_BREATH(GF_POIS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_POIS */
    case 96 + 13:  return spell_RF4_BREATH(GF_NETHER, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_NETH */
    case 96 + 14:  return spell_RF4_BREATH(GF_LITE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_LITE */
    case 96 + 15:  return spell_RF4_BREATH(GF_DARK, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_DARK */
    case 96 + 16:  return spell_RF4_BREATH(GF_CONFUSION, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_CONF */
    case 96 + 17:  return spell_RF4_BREATH(GF_SOUND, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_SOUN */
    case 96 + 18:  return spell_RF4_BREATH(GF_CHAOS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_CHAO */
    case 96 + 19:  return spell_RF4_BREATH(GF_DISENCHANT, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_DISE */
    case 96 + 20:  return spell_RF4_BREATH(GF_NEXUS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_NEXU */
    case 96 + 21:  return spell_RF4_BREATH(GF_TIME, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_TIME */
    case 96 + 22:  return spell_RF4_BREATH(GF_INERTIA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_INER */
    case 96 + 23:  return spell_RF4_BREATH(GF_GRAVITY, y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF4_BR_GRAV */
    case 96 + 24:  return spell_RF4_BREATH(GF_SHARDS, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_SHAR */
    case 96 + 25:  return spell_RF4_BREATH(GF_PLASMA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);  /* RF4_BR_PLAS */
    case 96 + 26:  return spell_RF4_BREATH(GF_FORCE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF4_BR_WALL */
    case 96 + 27:  return spell_RF4_BREATH(GF_MANA, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_MANA */
    case 96 + 28:  return spell_RF4_BA_NUKE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BA_NUKE */
    case 96 + 29:  return spell_RF4_BREATH(GF_NUKE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_NUKE */
    case 96 + 30:  return spell_RF4_BA_CHAO(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BA_CHAO */
    case 96 + 31:  return spell_RF4_BREATH(GF_DISINTEGRATE, y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF4_BR_DISI */
    case 128 + 0:  return spell_RF5_BA_ACID(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_ACID */
    case 128 + 1:  return spell_RF5_BA_ELEC(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_ELEC */
    case 128 + 2:  return spell_RF5_BA_FIRE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_FIRE */
    case 128 + 3:  return spell_RF5_BA_COLD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_COLD */
    case 128 + 4:  return spell_RF5_BA_POIS(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_POIS */
    case 128 + 5:  return spell_RF5_BA_NETH(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_NETH */
    case 128 + 6:  return spell_RF5_BA_WATE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_WATE */
    case 128 + 7:  return spell_RF5_BA_MANA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_MANA */
    case 128 + 8:  return spell_RF5_BA_DARK(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_DARK */
    case 128 + 9:  return spell_RF5_DRAIN_MANA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_DRAIN_MANA */
    case 128 + 10: return spell_RF5_MIND_BLAST(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF5_MIND_BLAST */
    case 128 + 11: return spell_RF5_BRAIN_SMASH(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BRAIN_SMASH */
    case 128 + 12: return spell_RF5_CAUSE_1(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_CAUSE_1 */
    case 128 + 13: return spell_RF5_CAUSE_2(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_CAUSE_2 */
    case 128 + 14: return spell_RF5_CAUSE_3(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_CAUSE_3 */
    case 128 + 15: return spell_RF5_CAUSE_4(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_CAUSE_4 */
    case 128 + 16: return spell_RF5_BO_ACID(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_ACID */
    case 128 + 17: return spell_RF5_BO_ELEC(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_ELEC */
    case 128 + 18: return spell_RF5_BO_FIRE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_FIRE */
    case 128 + 19: return spell_RF5_BO_COLD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_COLD */
    case 128 + 20: return spell_RF5_BA_LITE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BA_LITE */
    case 128 + 21: return spell_RF5_BO_NETH(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_NETH */
    case 128 + 22: return spell_RF5_BO_WATE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_WATE */
    case 128 + 23: return spell_RF5_BO_MANA(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_MANA */
    case 128 + 24: return spell_RF5_BO_PLAS(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_PLAS */
    case 128 + 25: return spell_RF5_BO_ICEE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_BO_ICEE */
    case 128 + 26: return spell_RF5_MISSILE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);    /* RF5_MISSILE */
    case 128 + 27: spell_RF5_SCARE(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF5_SCARE */
    case 128 + 28: spell_RF5_BLIND(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF5_BLIND */
    case 128 + 29: spell_RF5_CONF(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF5_CONF */
    case 128 + 30: spell_RF5_SLOW(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF5_SLOW */
    case 128 + 31: spell_RF5_HOLD(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); break;  /* RF5_HOLD */
    case 160 + 0:  spell_RF6_HASTE(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_HASTE */
    case 160 + 1:  return spell_RF6_HAND_DOOM(y, x, m_idx, t_idx, MONSTER_TO_MONSTER); /* RF6_HAND_DOOM */
    case 160 + 2:  spell_RF6_HEAL(m_idx, t_idx, MONSTER_TO_MONSTER); break;    /* RF6_HEAL */
    case 160 + 3:  spell_RF6_INVULNER(m_idx, t_idx, MONSTER_TO_MONSTER); break;    /* RF6_INVULNER */
    case 160 + 4:  spell_RF6_BLINK(m_idx, MONSTER_TO_MONSTER); break;   /* RF6_BLINK */
    case 160 + 5:  spell_RF6_TPORT(m_idx, MONSTER_TO_MONSTER); break;   /* RF6_TPORT */
    case 160 + 6:  return -1; break;    /* RF6_WORLD */
    case 160 + 7:  return spell_RF6_SPECIAL(y, x, m_idx, t_idx, MONSTER_TO_MONSTER);   /* RF6_SPECIAL */
    case 160 + 8:  spell_RF6_TELE_TO(m_idx, t_idx, MONSTER_TO_MONSTER); break; /* RF6_TELE_TO */
    case 160 + 9:  spell_RF6_TELE_AWAY(m_idx, t_idx, MONSTER_TO_MONSTER); break;   /* RF6_TELE_AWAY */
    case 160 + 10: spell_RF6_TELE_LEVEL(m_idx, t_idx, MONSTER_TO_PLAYER); break;  /* RF6_TELE_LEVEL */
    }
    return 0;
}