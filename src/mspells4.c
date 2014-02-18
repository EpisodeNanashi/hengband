#include "angband.h"

void spell_RF4_SHRIEK(int m_idx, cptr m_name)
{
    disturb(1, 1);
    msg_format(_("%^s������⤤���ڤ����򤢤�����", "%^s makes a high pitched shriek."), m_name);
    aggravate_monsters(m_idx);
}

void spell_RF4_DISPEL(bool blind, cptr m_name)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s�����Ͼõ�μ�ʸ��ǰ������", "%^s invokes a dispel magic."), m_name);

    dispel_player();
    if (p_ptr->riding) dispel_monster_status(p_ptr->riding);

#ifdef JP
    if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
        msg_print("���䤬�ä��ʡ�");
#endif
    learn_spell(MS_DISPEL);
}

void spell_RF4_ROCKET(bool blind, cptr m_name, monster_type* m_ptr, int y, int x, int m_idx, bool learnable)
{
    int dam;

    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������ͤä���", "%^s shoots something."), m_name);
    else
        msg_format(_("%^s�����åȤ�ȯ�ͤ�����", "%^s fires a rocket."), m_name);

    dam = ((m_ptr->hp / 4) > 800 ? 800 : (m_ptr->hp / 4));
    breath(y, x, m_idx, GF_ROCKET,
        dam, 2, FALSE, MS_ROCKET, learnable);
    update_smart_learn(m_idx, DRS_SHARD);
}

void spell_RF4_SHOOT(bool blind, cptr m_name, monster_race* r_ptr, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s����̯�ʲ���ȯ������", "%^s makes a strange noise."), m_name);
    else
        msg_format(_("%^s��������ä���", "%^s fires an arrow."), m_name);

    dam = damroll(r_ptr->blow[0].d_dice, r_ptr->blow[0].d_side);
    bolt(m_idx, GF_ARROW, dam, MS_SHOOT, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF4_BREATH(int GF_TYPE, bool blind, cptr m_name, monster_type* m_ptr, int y, int x, int m_idx, bool learnable)
{
    int dam, ms_type, drs_type;
    cptr type_s;
    bool smart_learn = TRUE;

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

    disturb(1, 1);
    if (m_ptr->r_idx == MON_JAIAN && GF_TYPE == GF_SOUND)
    {
        msg_format(_("�֥ܥ�����������������", "'Booooeeeeee'"));
    }
    else if (m_ptr->r_idx == MON_BOTEI && GF_TYPE == GF_SHARDS)
    {
        msg_format(_("�֥���ӥ륫�å�����������", "'Boty-Build cutter!!!'"));
    }
    else if (blind)
    {
        msg_format(_("%^s�������Υ֥쥹���Ǥ�����", "%^s breathes."), m_name);
    }
    else
    {
        msg_format(_("%^s��%^s�Υ֥쥹���Ǥ�����", "%^s breathes %^s."), m_name, type_s);
    }

    breath(y, x, m_idx, GF_TYPE, dam, 0, TRUE, ms_type, learnable);
    if (smart_learn) update_smart_learn(m_idx, drs_type);
}

void spell_RF4_BA_CHAO(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������ˤĤ֤䤤����", "%^s mumbles frighteningly."), m_name);
    else
        msg_format(_("%^s������륹�����ä���", "%^s invokes a raw Logrus."), m_name);
    
    dam = ((r_ptr->flags2 & RF2_POWERFUL) ? (rlev * 3) : (rlev * 2)) + damroll(10, 10);

    breath(y, x, m_idx, GF_CHAOS, dam, 4, FALSE, MS_BALL_CHAOS, learnable);
    update_smart_learn(m_idx, DRS_CHAOS);
}

void spell_RF4_BA_NUKE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s������ǽ������ä���", "%^s casts a ball of radiation."), m_name);
    
    dam = (rlev + damroll(10, 6)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);

    breath(y, x, m_idx, GF_NUKE, dam, 2, FALSE, MS_BALL_NUKE, learnable);
    update_smart_learn(m_idx, DRS_POIS);
}

void spell_RF5_BA_ACID(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�������åɡ��ܡ���μ�ʸ�򾧤�����", "%^s casts an acid ball."), m_name);
    
    if (r_ptr->flags2 & RF2_POWERFUL)
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 3) + 15);
    }
    breath(y, x, m_idx, GF_ACID, dam, rad, FALSE, MS_BALL_ACID, learnable);
    update_smart_learn(m_idx, DRS_ACID);
}

void spell_RF5_BA_ELEC(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s��������������ܡ���μ�ʸ�򾧤�����", "%^s casts a lightning ball."), m_name);

    if (r_ptr->flags2 & RF2_POWERFUL)
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 3 / 2) + 8);
    }
    breath(y, x, m_idx, GF_ELEC, dam, rad, FALSE, MS_BALL_ELEC, learnable);
    update_smart_learn(m_idx, DRS_ELEC);
}

void spell_RF5_BA_FIRE(monster_type* m_ptr, bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (m_ptr->r_idx == MON_ROLENTO)
    {
        if (blind)
            msg_format(_("%s���������ꤲ����", "%^s throws something."), m_name);
        else
            msg_format(_("%s�ϼ����Ƥ��ꤲ����", "%^s throws a hand grenade."), m_name);
    }
    else
    {
        if (blind)
            msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s���ե��������ܡ���μ�ʸ�򾧤�����", "%^s casts a fire ball."), m_name);
    }

    if (r_ptr->flags2 & RF2_POWERFUL)
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 7 / 2) + 10);
    }
    breath(y, x, m_idx, GF_FIRE, dam, rad, FALSE, MS_BALL_FIRE, learnable);
    update_smart_learn(m_idx, DRS_FIRE);
}

void spell_RF5_BA_COLD(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����������ܡ���μ�ʸ�򾧤�����", "%^s casts a frost ball."), m_name);

    if (r_ptr->flags2 & RF2_POWERFUL)
    {
        rad = 4;
        dam = (rlev * 4) + 50 + damroll(10, 10);
    }
    else
    {
        rad = 2;
        dam = (randint1(rlev * 3 / 2) + 10);
    }
    breath(y, x, m_idx, GF_COLD, dam, rad, FALSE, MS_BALL_COLD, learnable);
    update_smart_learn(m_idx, DRS_COLD);
}

void spell_RF5_BA_POIS(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���������μ�ʸ�򾧤�����", "%^s casts a stinking cloud."), m_name);

    dam = damroll(12, 2) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    breath(y, x, m_idx, GF_POIS, dam, 2, FALSE, MS_BALL_POIS, learnable);
    update_smart_learn(m_idx, DRS_POIS);
}

void spell_RF5_BA_NETH(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���Ϲ���μ�ʸ�򾧤�����", "%^s casts a nether ball."), m_name);

    dam = 50 + damroll(10, 10) + (rlev * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1));
    breath(y, x, m_idx, GF_NETHER, dam, 2, FALSE, MS_BALL_NETHER, learnable);
    update_smart_learn(m_idx, DRS_NETH);
}

void spell_RF5_BA_WATE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s��ή���褦�ʿȿ���򤷤���", "%^s gestures fluidly."), m_name);

    msg_print(_("���ʤ��ϱ������˰��߹��ޤ줿��", "You are engulfed in a whirlpool."));

    dam = ((r_ptr->flags2 & RF2_POWERFUL) ? randint1(rlev * 3) : randint1(rlev * 2)) + 50;
    breath(y, x, m_idx, GF_WATER, dam, 4, FALSE, MS_BALL_WATER, learnable);
}

void spell_RF5_BA_MANA(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s�����Ϥ���μ�ʸ��ǰ������", "%^s invokes a mana storm."), m_name);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_MANA, dam, 4, FALSE, MS_BALL_MANA, learnable);
}

void spell_RF5_BA_DARK(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s���Ź�����μ�ʸ��ǰ������", "%^s invokes a darkness storm."), m_name);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_DARK, dam, 4, FALSE, MS_BALL_DARK, learnable);
    update_smart_learn(m_idx, DRS_DARK);
}

void spell_RF5_DRAIN_MANA(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    dam = (randint1(rlev) / 2) + 1;
    breath(y, x, m_idx, GF_DRAIN_MANA, dam, 0, FALSE, MS_DRAIN_MANA, learnable);
    update_smart_learn(m_idx, DRS_MANA);
}

void spell_RF5_MIND_BLAST(bool seen, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (!seen)
        msg_print(_("���������ʤ���������ǰ�����äƤ���褦����", "You feel something focusing on your mind."));
    else
        msg_format(_("%^s�����ʤ���Ʒ�򤸤äȤˤ��Ǥ��롣", "%^s gazes deep into your eyes."), m_name);

    dam = damroll(7, 7);
    breath(y, x, m_idx, GF_MIND_BLAST, dam, 0, FALSE, MS_MIND_BLAST, learnable);
}

void spell_RF5_BRAIN_SMASH(bool seen, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (!seen)
        msg_print(_("���������ʤ���������ǰ�����äƤ���褦����", "You feel something focusing on your mind."));
    else
        msg_format(_("%^s�����ʤ���Ʒ�򤸤äȸ��Ƥ��롣", "%^s looks deep into your eyes."), m_name);

    dam = damroll(12, 12);
}

void spell_RF5_CAUSE_1(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����ʤ���ؤ����Ƽ��ä���", "%^s points at you and curses."), m_name);

    dam = damroll(3, 8);
    breath(y, x, m_idx, GF_CAUSE_1, dam, 0, FALSE, MS_CAUSE_1, learnable);
}

void spell_RF5_CAUSE_2(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����ʤ���ؤ����ƶ������˼��ä���", "%^s points at you and curses horribly."), m_name);

    dam = damroll(8, 8);
    breath(y, x, m_idx, GF_CAUSE_2, dam, 0, FALSE, MS_CAUSE_2, learnable);
}

void spell_RF5_CAUSE_3(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s�������������Ƕ������", "%^s mumbles loudly."), m_name);
    else
        msg_format(_("%^s�����ʤ���ؤ����ƶ������˼�ʸ�򾧤�����", "%^s points at you, incanting terribly!"), m_name);

    dam = damroll(10, 15);
    breath(y, x, m_idx, GF_CAUSE_3, dam, 0, FALSE, MS_CAUSE_3, learnable);
}

void spell_RF5_CAUSE_4(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���֤����ϴ��˻��Ǥ���פȶ������", "%^s screams the word 'DIE!'"), m_name);
    else
        msg_format(_("%^s�����ʤ����빦���ͤ��ơ֤����ϴ��˻��Ǥ���פȶ������",
        "%^s points at you, screaming the word DIE!"), m_name);

    dam = damroll(15, 15);
    breath(y, x, m_idx, GF_CAUSE_4, dam, 0, FALSE, MS_CAUSE_4, learnable);
}

void spell_RF5_BO_ACID(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�������åɡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a acid bolt."), m_name);

    dam = (damroll(7, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_ACID, dam, MS_BOLT_ACID, learnable);
    update_smart_learn(m_idx, DRS_ACID);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_ELEC(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a lightning bolt."), m_name);

    dam = (damroll(4, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_ELEC, dam, MS_BOLT_ELEC, learnable);
    update_smart_learn(m_idx, DRS_ELEC);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_FIRE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���ե��������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a fire bolt."), m_name);

    dam = (damroll(9, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_FIRE, dam, MS_BOLT_FIRE, learnable);
    update_smart_learn(m_idx, DRS_FIRE);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_COLD(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a frost bolt."), m_name);

    dam = (damroll(6, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_COLD, dam, MS_BOLT_COLD, learnable);
    update_smart_learn(m_idx, DRS_COLD);
    update_smart_learn(m_idx, DRS_REFLECT);
}


void spell_RF5_BA_LITE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s���������С����Ȥμ�ʸ��ǰ������", "%^s invokes a starburst."), m_name);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_LITE, dam, 4, FALSE, MS_STARBURST, learnable);
    update_smart_learn(m_idx, DRS_LITE);
}


void spell_RF5_BO_NETH(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���Ϲ�����μ�ʸ�򾧤�����", "%^s casts a nether bolt."), m_name);

    dam = 30 + damroll(5, 5) + (rlev * 4) / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3);
    bolt(m_idx, GF_NETHER, dam, MS_BOLT_NETHER, learnable);
    update_smart_learn(m_idx, DRS_NETH);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_WATE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���������������ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a water bolt."), m_name);

    dam = damroll(10, 10) + (rlev * 3 / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3));
    bolt(m_idx, GF_WATER, dam, MS_BOLT_WATER, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_MANA(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����Ϥ���μ�ʸ�򾧤�����", "%^s casts a mana bolt."), m_name);

    dam = randint1(rlev * 7 / 2) + 50;
    bolt(m_idx, GF_MANA, dam, MS_BOLT_MANA, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_PLAS(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);

    else
        msg_format(_("%^s���ץ饺�ޡ��ܥ�Ȥμ�ʸ�򾧤�����", "%^s casts a plasma bolt."), m_name);

    dam = 10 + damroll(8, 7) + (rlev * 3 / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3));
    bolt(m_idx, GF_PLASMA, dam, MS_BOLT_PLASMA, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
}

void spell_RF5_BO_ICEE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���˴�����μ�ʸ�򾧤�����", "%^s casts an ice bolt."), m_name);

    dam = damroll(6, 6) + (rlev * 3 / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3));
    bolt(m_idx, GF_ICE, dam, MS_BOLT_ICE, learnable);
    update_smart_learn(m_idx, DRS_COLD);
    update_smart_learn(m_idx, DRS_REFLECT);
}


void spell_RF5_MISSILE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������Ĥ֤䤤����", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���ޥ��å����ߥ�����μ�ʸ�򾧤�����", "%^s casts a magic missile."), m_name);

    dam = damroll(2, 6) + (rlev / 3);
    bolt(m_idx, GF_MISSILE, dam, MS_MAGIC_MISSILE, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
}