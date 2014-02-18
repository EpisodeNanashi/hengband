#include "angband.h"

void spell_RF4_SHRIEK(int m_idx, cptr m_name)
{
    disturb(1, 1);
    msg_format(_("%^s�����񍂂����؂萺���������B", "%^s makes a high pitched shriek."), m_name);
    aggravate_monsters(m_idx);
}

void spell_RF4_DISPEL(bool blind, cptr m_name)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������͋����Ԃ₢���B", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s�����͏����̎�����O�����B", "%^s invokes a dispel magic."), m_name);

    dispel_player();
    if (p_ptr->riding) dispel_monster_status(p_ptr->riding);

#ifdef JP
    if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
        msg_print("���₪�����ȁI");
#endif
    learn_spell(MS_DISPEL);
}

int spell_RF4_ROCKET(bool blind, cptr m_name, monster_type* m_ptr, int y, int x, int m_idx, bool learnable)
{
    int dam;

    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������˂����B", "%^s shoots something."), m_name);
    else
        msg_format(_("%^s�����P�b�g�𔭎˂����B", "%^s fires a rocket."), m_name);

    dam = ((m_ptr->hp / 4) > 800 ? 800 : (m_ptr->hp / 4));
    breath(y, x, m_idx, GF_ROCKET,
        dam, 2, FALSE, MS_ROCKET, learnable);
    update_smart_learn(m_idx, DRS_SHARD);
    return dam;
}

int spell_RF4_SHOOT(bool blind, cptr m_name, monster_race* r_ptr, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s����ȉ��𔭂����B", "%^s makes a strange noise."), m_name);
    else
        msg_format(_("%^s�����������B", "%^s fires an arrow."), m_name);

    dam = damroll(r_ptr->blow[0].d_dice, r_ptr->blow[0].d_side);
    bolt(m_idx, GF_ARROW, dam, MS_SHOOT, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF4_BREATH(int GF_TYPE, bool blind, cptr m_name, monster_type* m_ptr, int y, int x, int m_idx, bool learnable)
{
    int dam, ms_type, drs_type;
    cptr type_s;
    bool smart_learn = TRUE;

    switch (GF_TYPE)
    {
        case GF_ACID:
            dam = ((m_ptr->hp / 3) > 1600 ? 1600 : (m_ptr->hp / 3));
            type_s = _("�_", "acid");
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
            type_s = _("�Ή�", "fire");
            ms_type = MS_BR_FIRE;
            drs_type = DRS_FIRE;
            break;
        case GF_COLD:
            dam = ((m_ptr->hp / 3) > 1600 ? 1600 : (m_ptr->hp / 3));
            type_s = _("��C", "frost");
            ms_type = MS_BR_COLD;
            drs_type = DRS_COLD;
            break;
        case GF_POIS:
            dam = ((m_ptr->hp / 3) > 800 ? 800 : (m_ptr->hp / 3));
            type_s = _("�K�X", "gas");
            ms_type = MS_BR_POIS;
            drs_type = DRS_POIS;
            break;
        case GF_NETHER:
            dam = ((m_ptr->hp / 6) > 550 ? 550 : (m_ptr->hp / 6));
            type_s = _("�n��", "nether");
            ms_type = MS_BR_NETHER;
            drs_type = DRS_NETH;
            break;
        case GF_LITE:
            dam = ((m_ptr->hp / 6) > 400 ? 400 : (m_ptr->hp / 6));
            type_s = _("�M��", "light");
            ms_type = MS_BR_LITE;
            drs_type = DRS_LITE;
            break;
        case GF_DARK:
            dam = ((m_ptr->hp / 6) > 400 ? 400 : (m_ptr->hp / 6));
            type_s = _("�Í�", "darkness");
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
            type_s = _("����", "sound");
            ms_type = MS_BR_SOUND;
            drs_type = DRS_SOUND;
            break;
        case GF_CHAOS:
            dam = ((m_ptr->hp / 6) > 600 ? 600 : (m_ptr->hp / 6));
            type_s = _("�J�I�X", "chaos");
            ms_type = MS_BR_CHAOS;
            drs_type = DRS_CHAOS;
            break;
        case GF_DISENCHANT:
            dam = ((m_ptr->hp / 6) > 500 ? 500 : (m_ptr->hp / 6));
            type_s = _("��", "disenchantment");
            ms_type = MS_BR_DISEN;
            drs_type = DRS_DISEN;
            break;
        case GF_NEXUS:
            dam = ((m_ptr->hp / 3) > 250 ? 250 : (m_ptr->hp / 3));
            type_s = _("���ʍ���", "nexus");
            ms_type = MS_BR_NEXUS;
            drs_type = DRS_NEXUS;
            break;
        case GF_TIME:
            dam = ((m_ptr->hp / 3) > 150 ? 150 : (m_ptr->hp / 3));
            type_s = _("���ԋt�]", "time");
            ms_type = MS_BR_TIME;
            smart_learn = FALSE;
            break;
        case GF_INERTIA:
            dam = ((m_ptr->hp / 6) > 200 ? 200 : (m_ptr->hp / 6));
            type_s = _("�x��", "inertia");
            ms_type = MS_BR_INERTIA;
            smart_learn = FALSE;
            break;
        case GF_GRAVITY:
            dam = ((m_ptr->hp / 3) > 200 ? 200 : (m_ptr->hp / 3));
            type_s = _("�d��", "gravity");
            ms_type = MS_BR_GRAVITY;
            smart_learn = FALSE;
            break;
        case GF_SHARDS:
            dam = ((m_ptr->hp / 6) > 500 ? 500 : (m_ptr->hp / 6));
            type_s = _("�j��", "shards");
            ms_type = MS_BR_SHARDS;
            drs_type = DRS_SHARD;
            break;
        case GF_PLASMA:
            dam = ((m_ptr->hp / 6) > 150 ? 150 : (m_ptr->hp / 6));
            type_s = _("�v���Y�}", "plasma");
            ms_type = MS_BR_PLASMA;
            smart_learn = FALSE;
            break;
        case GF_FORCE:
            dam = ((m_ptr->hp / 6) > 200 ? 200 : (m_ptr->hp / 6));
            type_s = _("�t�H�[�X", "force");
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
            type_s = _("���ː��p����", "toxic waste");
            ms_type = MS_BR_NUKE;
            drs_type = DRS_POIS;
            break;
        case GF_DISINTEGRATE:
            dam = ((m_ptr->hp / 6) > 150 ? 150 : (m_ptr->hp / 6));
            type_s = _("����", "disintegration");
            ms_type = MS_BR_DISI;
            smart_learn = FALSE;
            break;
        default:
            break;
    }

    disturb(1, 1);
    if (m_ptr->r_idx == MON_JAIAN && GF_TYPE == GF_SOUND)
    {
        msg_format(_("�u�{�H�G�`�`�`�`�`�`�v", "'Booooeeeeee'"));
    }
    else if (m_ptr->r_idx == MON_BOTEI && GF_TYPE == GF_SHARDS)
    {
        msg_format(_("�u�{��r���J�b�^�[�I�I�I�v", "'Boty-Build cutter!!!'"));
    }
    else if (blind)
    {
        msg_format(_("%^s�������̃u���X��f�����B", "%^s breathes."), m_name);
    }
    else
    {
        msg_format(_("%^s��%^s�̃u���X��f�����B", "%^s breathes %^s."), m_name, type_s);
    }

    breath(y, x, m_idx, GF_TYPE, dam, 0, TRUE, ms_type, learnable);
    if (smart_learn) update_smart_learn(m_idx, drs_type);
    return dam;
}

int spell_RF4_BA_CHAO(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s�����낵���ɂԂ₢���B", "%^s mumbles frighteningly."), m_name);
    else
        msg_format(_("%^s�������O���X��������B", "%^s invokes a raw Logrus."), m_name);
    
    dam = ((r_ptr->flags2 & RF2_POWERFUL) ? (rlev * 3) : (rlev * 2)) + damroll(10, 10);

    breath(y, x, m_idx, GF_CHAOS, dam, 4, FALSE, MS_BALL_CHAOS, learnable);
    update_smart_learn(m_idx, DRS_CHAOS);
    return dam;
}

int spell_RF4_BA_NUKE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����˔\����������B", "%^s casts a ball of radiation."), m_name);
    
    dam = (rlev + damroll(10, 6)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);

    breath(y, x, m_idx, GF_NUKE, dam, 2, FALSE, MS_BALL_NUKE, learnable);
    update_smart_learn(m_idx, DRS_POIS);
    return dam;
}

int spell_RF5_BA_ACID(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���A�V�b�h�E�{�[���̎������������B", "%^s casts an acid ball."), m_name);
    
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
    return dam;
}

int spell_RF5_BA_ELEC(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���T���_�[�E�E�{�[���̎������������B", "%^s casts a lightning ball."), m_name);

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
    return dam;
}

int spell_RF5_BA_FIRE(monster_type* m_ptr, bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (m_ptr->r_idx == MON_ROLENTO)
    {
        if (blind)
            msg_format(_("%s�������𓊂����B", "%^s throws something."), m_name);
        else
            msg_format(_("%s�͎�֒e�𓊂����B", "%^s throws a hand grenade."), m_name);
    }
    else
    {
        if (blind)
            msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s���t�@�C�A�E�{�[���̎������������B", "%^s casts a fire ball."), m_name);
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
    return dam;
}

int spell_RF5_BA_COLD(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam, rad;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���A�C�X�E�{�[���̎������������B", "%^s casts a frost ball."), m_name);

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
    return dam;
}

int spell_RF5_BA_POIS(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����L�_�̎������������B", "%^s casts a stinking cloud."), m_name);

    dam = damroll(12, 2) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    breath(y, x, m_idx, GF_POIS, dam, 2, FALSE, MS_BALL_POIS, learnable);
    update_smart_learn(m_idx, DRS_POIS);
    return dam;
}

int spell_RF5_BA_NETH(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���n�����̎������������B", "%^s casts a nether ball."), m_name);

    dam = 50 + damroll(10, 10) + (rlev * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1));
    breath(y, x, m_idx, GF_NETHER, dam, 2, FALSE, MS_BALL_NETHER, learnable);
    update_smart_learn(m_idx, DRS_NETH);
    return dam;
}

int spell_RF5_BA_WATE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�������悤�Ȑg�U��������B", "%^s gestures fluidly."), m_name);

    msg_print(_("���Ȃ��͉Q�����Ɉ��ݍ��܂ꂽ�B", "You are engulfed in a whirlpool."));

    dam = ((r_ptr->flags2 & RF2_POWERFUL) ? randint1(rlev * 3) : randint1(rlev * 2)) + 50;
    breath(y, x, m_idx, GF_WATER, dam, 4, FALSE, MS_BALL_WATER, learnable);
    return dam;
}

int spell_RF5_BA_MANA(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s��������͋����Ԃ₢���B", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s�����̗͂��̎�����O�����B", "%^s invokes a mana storm."), m_name);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_MANA, dam, 4, FALSE, MS_BALL_MANA, learnable);
    return dam;
}

int spell_RF5_BA_DARK(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������͋����Ԃ₢���B", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s���Í��̗��̎�����O�����B", "%^s invokes a darkness storm."), m_name);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_DARK, dam, 4, FALSE, MS_BALL_DARK, learnable);
    update_smart_learn(m_idx, DRS_DARK);
    return dam;
}

int spell_RF5_DRAIN_MANA(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    dam = (randint1(rlev) / 2) + 1;
    breath(y, x, m_idx, GF_DRAIN_MANA, dam, 0, FALSE, MS_DRAIN_MANA, learnable);
    update_smart_learn(m_idx, DRS_MANA);
    return dam;
}

int spell_RF5_MIND_BLAST(bool seen, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (!seen)
        msg_print(_("���������Ȃ��̐��_�ɔO������Ă���悤���B", "You feel something focusing on your mind."));
    else
        msg_format(_("%^s�����Ȃ��̓��������Ƃɂ��ł���B", "%^s gazes deep into your eyes."), m_name);

    dam = damroll(7, 7);
    breath(y, x, m_idx, GF_MIND_BLAST, dam, 0, FALSE, MS_MIND_BLAST, learnable);
    return dam;
}

int spell_RF5_BRAIN_SMASH(bool seen, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (!seen)
        msg_print(_("���������Ȃ��̐��_�ɔO������Ă���悤���B", "You feel something focusing on your mind."));
    else
        msg_format(_("%^s�����Ȃ��̓��������ƌ��Ă���B", "%^s looks deep into your eyes."), m_name);

    dam = damroll(12, 12);
    breath(y, x, m_idx, GF_BRAIN_SMASH, dam, 0, FALSE, MS_BRAIN_SMASH, learnable);
    return dam;
}

int spell_RF5_CAUSE_1(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����Ȃ����w�����Ď�����B", "%^s points at you and curses."), m_name);

    dam = damroll(3, 8);
    breath(y, x, m_idx, GF_CAUSE_1, dam, 0, FALSE, MS_CAUSE_1, learnable);
    return dam;
}

int spell_RF5_CAUSE_2(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����Ȃ����w�����ċ��낵���Ɏ�����B", "%^s points at you and curses horribly."), m_name);

    dam = damroll(8, 8);
    breath(y, x, m_idx, GF_CAUSE_2, dam, 0, FALSE, MS_CAUSE_2, learnable);
    return dam;
}

int spell_RF5_CAUSE_3(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������吺�ŋ��񂾁B", "%^s mumbles loudly."), m_name);
    else
        msg_format(_("%^s�����Ȃ����w�����ċ��낵���Ɏ������������I", "%^s points at you, incanting terribly!"), m_name);

    dam = damroll(10, 15);
    breath(y, x, m_idx, GF_CAUSE_3, dam, 0, FALSE, MS_CAUSE_3, learnable);
    return dam;
}

int spell_RF5_CAUSE_4(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���u���O�͊��Ɏ���ł���v�Ƌ��񂾁B", "%^s screams the word 'DIE!'"), m_name);
    else
        msg_format(_("%^s�����Ȃ��̔�E��˂��āu���O�͊��Ɏ���ł���v�Ƌ��񂾁B",
        "%^s points at you, screaming the word DIE!"), m_name);

    dam = damroll(15, 15);
    breath(y, x, m_idx, GF_CAUSE_4, dam, 0, FALSE, MS_CAUSE_4, learnable);
    return dam;
}

int spell_RF5_BO_ACID(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���A�V�b�h�E�{���g�̎������������B", "%^s casts a acid bolt."), m_name);

    dam = (damroll(7, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_ACID, dam, MS_BOLT_ACID, learnable);
    update_smart_learn(m_idx, DRS_ACID);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_ELEC(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���T���_�[�E�{���g�̎������������B", "%^s casts a lightning bolt."), m_name);

    dam = (damroll(4, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_ELEC, dam, MS_BOLT_ELEC, learnable);
    update_smart_learn(m_idx, DRS_ELEC);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_FIRE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���t�@�C�A�E�{���g�̎������������B", "%^s casts a fire bolt."), m_name);

    dam = (damroll(9, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_FIRE, dam, MS_BOLT_FIRE, learnable);
    update_smart_learn(m_idx, DRS_FIRE);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_COLD(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���A�C�X�E�{���g�̎������������B", "%^s casts a frost bolt."), m_name);

    dam = (damroll(6, 8) + (rlev / 3)) * ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 1);
    bolt(m_idx, GF_COLD, dam, MS_BOLT_COLD, learnable);
    update_smart_learn(m_idx, DRS_COLD);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}


int spell_RF5_BA_LITE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������͋����Ԃ₢���B", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%^s���X�^�[�o�[�X�g�̎�����O�����B", "%^s invokes a starburst."), m_name);

    dam = (rlev * 4) + 50 + damroll(10, 10);
    breath(y, x, m_idx, GF_LITE, dam, 4, FALSE, MS_STARBURST, learnable);
    update_smart_learn(m_idx, DRS_LITE);
    return dam;
}


int spell_RF5_BO_NETH(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���n���̖�̎������������B", "%^s casts a nether bolt."), m_name);

    dam = 30 + damroll(5, 5) + (rlev * 4) / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3);
    bolt(m_idx, GF_NETHER, dam, MS_BOLT_NETHER, learnable);
    update_smart_learn(m_idx, DRS_NETH);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_WATE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���E�H�[�^�[�E�{���g�̎������������B", "%^s casts a water bolt."), m_name);

    dam = damroll(10, 10) + (rlev * 3 / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3));
    bolt(m_idx, GF_WATER, dam, MS_BOLT_WATER, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_MANA(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����̖͂�̎������������B", "%^s casts a mana bolt."), m_name);

    dam = randint1(rlev * 7 / 2) + 50;
    bolt(m_idx, GF_MANA, dam, MS_BOLT_MANA, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_PLAS(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);

    else
        msg_format(_("%^s���v���Y�}�E�{���g�̎������������B", "%^s casts a plasma bolt."), m_name);

    dam = 10 + damroll(8, 7) + (rlev * 3 / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3));
    bolt(m_idx, GF_PLASMA, dam, MS_BOLT_PLASMA, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

int spell_RF5_BO_ICEE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���Ɋ��̖�̎������������B", "%^s casts an ice bolt."), m_name);

    dam = damroll(6, 6) + (rlev * 3 / ((r_ptr->flags2 & RF2_POWERFUL) ? 2 : 3));
    bolt(m_idx, GF_ICE, dam, MS_BOLT_ICE, learnable);
    update_smart_learn(m_idx, DRS_COLD);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}


int spell_RF5_MISSILE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���}�W�b�N�E�~�T�C���̎������������B", "%^s casts a magic missile."), m_name);

    dam = damroll(2, 6) + (rlev / 3);
    bolt(m_idx, GF_MISSILE, dam, MS_MAGIC_MISSILE, learnable);
    update_smart_learn(m_idx, DRS_REFLECT);
    return dam;
}

void spell_RF5_SCARE(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₭�ƁA���낵���ȉ������������B", "%^s mumbles, and you hear scary noises."), m_name);
    else
        msg_format(_("%^s�����낵���Ȍ��o�����o�����B", "%^s casts a fearful illusion."), m_name);

    if (p_ptr->resist_fear)
    {
        msg_print(_("���������|�ɐN����Ȃ������B", "You refuse to be frightened."));
    }
    else if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������|�ɐN����Ȃ������B", "You refuse to be frightened."));
    }
    else
    {
        (void)set_afraid(p_ptr->afraid + randint0(4) + 4);
    }
    learn_spell(MS_SCARE);
    update_smart_learn(m_idx, DRS_FEAR);
}

void spell_RF5_BLIND(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�������������Ă��Ȃ��̖ڂ�����܂����I",
        "%^s casts a spell, burning your eyes!"), m_name);

    if (p_ptr->resist_blind)
    {
        msg_print(_("���������ʂ��Ȃ������I", "You are unaffected!"));
    }
    else if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������͂𒵂˕Ԃ����I", "You resist the effects!"));
    }
    else
    {
        (void)set_blind(12 + randint0(4));
    }
    learn_spell(MS_BLIND);
    update_smart_learn(m_idx, DRS_BLIND);
}

void spell_RF5_CONF(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₭�ƁA����Y�܂����������B",
        "%^s mumbles, and you hear puzzling noises."), m_name);
    else
        msg_format(_("%^s���U�f�I�Ȍ��o�����o�����B",
        "%^s creates a mesmerising illusion."), m_name);

    if (p_ptr->resist_conf)
    {
        msg_print(_("���������o�ɂ͂��܂���Ȃ������B", "You disbelieve the feeble spell."));
    }
    else if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������o�ɂ͂��܂���Ȃ������B", "You disbelieve the feeble spell."));
    }
    else
    {
        (void)set_confused(p_ptr->confused + randint0(4) + 4);
    }
    learn_spell(MS_CONF);
    update_smart_learn(m_idx, DRS_CONF);
}

void spell_RF5_SLOW(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx)
{
    disturb(1, 1);

    msg_format(_("%^s�����Ȃ��̋ؗ͂��z����낤�Ƃ����I",
        "%^s drains power from your muscles!"), m_name);

    if (p_ptr->free_act)
    {
        msg_print(_("���������ʂ��Ȃ������I", "You are unaffected!"));
    }
    else if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������͂𒵂˕Ԃ����I", "You resist the effects!"));
    }
    else
    {
        (void)set_slow(p_ptr->slow + randint0(4) + 4, FALSE);
    }
    learn_spell(MS_SLOW);
    update_smart_learn(m_idx, DRS_FREE);
}

void spell_RF5_HOLD(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int y, int x, int m_idx)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����Ȃ��̖ڂ������ƌ��߂��I", "%^s stares deep into your eyes!"), m_name);

    if (p_ptr->free_act)
    {
        msg_print(_("���������ʂ��Ȃ������I", "You are unaffected!"));
    }
    else if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_format(_("���������͂𒵂˕Ԃ����I", "You resist the effects!"));
    }
    else
    {
        (void)set_paralyzed(p_ptr->paralyzed + randint0(4) + 4);
    }
    learn_spell(MS_SLEEP);
    update_smart_learn(m_idx, DRS_FREE);
}

void spell_RF6_HASTE(bool blind, cptr m_name, monster_type* m_ptr, int y, int x, int m_idx)
{
    disturb(1, 1);
    if (blind)
    {
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    }
    else
    {
        msg_format(_("%^s�������̑̂ɔO�𑗂����B", "%^s concentrates on %s body."), m_name);
    }

    /* Allow quick speed increases to base+10 */
    if (set_monster_fast(m_idx, MON_FAST(m_ptr) + 100))
    {
        msg_format(_("%^s�̓����������Ȃ����B", "%^s starts moving faster."), m_name);
    }
}

int spell_RF6_HAND_DOOM(bool blind, cptr m_name, monster_type* m_ptr, int y, int x, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    msg_format(_("%^s��<�j�ł̎�>��������I", "%^s invokes the Hand of Doom!"), m_name);
    dam = (((s32b)((40 + randint1(20)) * (p_ptr->chp))) / 100);
    breath(y, x, m_idx, GF_HAND_DOOM, dam, 0, FALSE, MS_HAND_DOOM, learnable);
    return dam;
}

void spell_RF6_HEAL(bool blind, bool seen, cptr m_name, monster_type* m_ptr,int rlev, int m_idx)
{
    disturb(1, 1);

    /* Message */
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�������̏��ɏW�������B", "%^s concentrates on %s wounds."), m_name);

    /* Heal some */
    m_ptr->hp += (rlev * 6);

    /* Fully healed */
    if (m_ptr->hp >= m_ptr->maxhp)
    {
        /* Fully healed */
        m_ptr->hp = m_ptr->maxhp;

        /* Message */
        if (seen)
            msg_format(_("%^s�͊��S�Ɏ������I", "%^s looks completely healed!"), m_name);
        else
            msg_format(_("%^s�͊��S�Ɏ������悤���I", "%^s sounds completely healed!"), m_name);
    }

    /* Partially healed */
    else
    {
        /* Message */
        if (seen)
            msg_format(_("%^s�̗͑͂��񕜂����悤���B", "%^s looks healthier."), m_name);
        else
            msg_format(_("%^s�̗͑͂��񕜂����悤���B", "%^s sounds healthier."), m_name);
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
        msg_format(_("%^s�͗E�C�����߂����B", "%^s recovers %s courage."), m_name);
    }
}
void spell_RF6_INVULNER(bool seen, cptr m_name, monster_type* m_ptr, int m_idx)
{
    disturb(1, 1);

    /* Message */
    if (!seen)
        msg_format(_("%^s��������͋����Ԃ₢���B", "%^s mumbles powerfully."), m_name);
    else
        msg_format(_("%s�͖����̋��̎������������B", "%^s casts a Globe of Invulnerability."), m_name);

    if (!MON_INVULNER(m_ptr)) (void)set_monster_invulner(m_idx, randint1(4) + 4, FALSE);
}

void spell_RF6_BLINK(cptr m_name, int m_idx)
{
    disturb(1, 1);
    if (teleport_barrier(m_idx))
    {
        msg_format(_("���@�̃o���A��%^s�̃e���|�[�g���ז������B",
            "Magic barrier obstructs teleporting of %^s."), m_name);
    }
    else
    {
        msg_format(_("%^s���u���ɏ������B", "%^s blinks away."), m_name);
        teleport_away(m_idx, 10, 0L);
        p_ptr->update |= (PU_MONSTERS);
    }
}

void spell_RF6_TPORT(cptr m_name, int m_idx)
{
    disturb(1, 1);
    if (teleport_barrier(m_idx))
    {
        msg_format(_("���@�̃o���A��%^s�̃e���|�[�g���ז������B",
            "Magic barrier obstructs teleporting of %^s."), m_name);
    }
    else
    {
        msg_format(_("%^s���e���|�[�g�����B", "%^s teleports away."), m_name);
        teleport_away_followable(m_idx);
    }
}

int spell_RF6_WORLD(cptr m_name, monster_type* m_ptr, int m_idx)
{
    int who = 0;
    disturb(1, 1);
    if (m_ptr->r_idx == MON_DIO) who = 1;
    else if (m_ptr->r_idx == MON_WONG) who = 3;
    if (!process_the_world(randint1(2) + 2, who, TRUE)) return (FALSE);
    return who;
}

int spell_RF6_SPECIAL(cptr m_name, monster_type* m_ptr, monster_race* r_ptr, u32b mode, bool blind, bool direct, int y, int x, int m_idx)
{
    int k, dam, count=0;

    disturb(1, 1);
    switch (m_ptr->r_idx)
    {
        case MON_OHMU:
            /* Moved to process_monster(), like multiplication */
            return -1;
        case MON_BANORLUPART:
        {
            int dummy_hp = (m_ptr->hp + 1) / 2;
            int dummy_maxhp = m_ptr->maxhp / 2;
            int dummy_y = m_ptr->fy;
            int dummy_x = m_ptr->fx;

            if (p_ptr->inside_arena || p_ptr->inside_battle || !summon_possible(m_ptr->fy, m_ptr->fx)) return -1;
            delete_monster_idx(cave[m_ptr->fy][m_ptr->fx].m_idx);
            summon_named_creature(0, dummy_y, dummy_x, MON_BANOR, mode);
            m_list[hack_m_idx_ii].hp = dummy_hp;
            m_list[hack_m_idx_ii].maxhp = dummy_maxhp;
            summon_named_creature(0, dummy_y, dummy_x, MON_LUPART, mode);
            m_list[hack_m_idx_ii].hp = dummy_hp;
            m_list[hack_m_idx_ii].maxhp = dummy_maxhp;

            msg_print(_("�w�o�[�m�[���E���p�[�g�x�����􂵂��I","Banor=Rupart splits in two person!"));
            break;
        }

        case MON_BANOR:
        case MON_LUPART:
        {
            int dummy_hp = 0;
            int dummy_maxhp = 0;
            int dummy_y = m_ptr->fy;
            int dummy_x = m_ptr->fx;

            if (!r_info[MON_BANOR].cur_num || !r_info[MON_LUPART].cur_num) return (FALSE);
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

            msg_print(_("�w�o�[�m�[���x�Ɓw���p�[�g�x�����̂����I", "Banor and Rupart combine into one!"));
            break;
        }

        case MON_ROLENTO:
            if (blind)
                msg_format(_("%^s��������ʂɓ������B", "%^s spreads something."), m_name);
            else
                msg_format(_("%^s�͎�֒e���΂�܂����B", "%^s throws some hand grenades."), m_name);

            {
                int num = 1 + randint1(3);

                for (k = 0; k < num; k++)
                {
                    count += summon_named_creature(m_idx, y, x, MON_SHURYUUDAN, mode);
                }
            }

            if (blind && count)
                msg_print(_("�����̂��̂��ԋ߂ɂ΂�܂���鉹������B", "You hear many things are scattered nearby."));

            break;

        default:
        if (r_ptr->d_char == 'B')
        {
            disturb(1, 1);
            if (one_in_(3) || !direct)
            {
                msg_format(_("%^s�͓ˑR���E���������!", "%^s suddenly go out of your sight!"), m_name);
                teleport_away(m_idx, 10, TELEPORT_NONMAGICAL);
                p_ptr->update |= (PU_MONSTERS);
            }
            else
            {
                int get_damage = 0;
                bool fear; /* dummy */

                msg_format(_("%^s�����Ȃ���͂�ŋ󒆂��瓊�����Ƃ����B",
                    "%^s holds you, and drops from the sky."), m_name);
                dam = damroll(4, 8);
                teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_NONMAGICAL | TELEPORT_PASSIVE);

                sound(SOUND_FALL);

                if (p_ptr->levitation)
                {
                    msg_print(_("���Ȃ��͐Â��ɒ��n�����B", "You float gently down to the ground."));
                }
                else
                {
                    msg_print(_("���Ȃ��͒n�ʂɒ@������ꂽ�B", "You crashed into the ground."));
                    dam += damroll(6, 8);
                }

                /* Mega hack -- this special action deals damage to the player. Therefore the code of "eyeeye" is necessary.
                -- henkma
                */
                get_damage = take_hit(DAMAGE_NOESCAPE, dam, m_name, -1);
                if (p_ptr->tim_eyeeye && get_damage > 0 && !p_ptr->is_dead)
                {
#ifdef JP
                    msg_format("�U����%s���g���������I", m_name);
#else
                    char m_name_self[80];

                    /* hisself */
                    monster_desc(m_name_self, m_ptr, MD_PRON_VISIBLE | MD_POSSESSIVE | MD_OBJECTIVE);

                    msg_format("The attack of %s has wounded %s!", m_name, m_name_self);
#endif
                    project(0, 0, m_ptr->fy, m_ptr->fx, get_damage, GF_MISSILE, PROJECT_KILL, -1);
                    set_tim_eyeeye(p_ptr->tim_eyeeye - 5, TRUE);
                }

                if (p_ptr->riding) mon_take_hit_mon(p_ptr->riding, dam, &fear, extract_note_dies(real_r_ptr(&m_list[p_ptr->riding])), m_idx);
            }
            break;
        }

        /* Something is wrong */
        else return -1;
    }
    return dam;
}


void spell_RF6_TELE_TO(cptr m_name, monster_type* m_ptr)
{
    disturb(1, 1);
    msg_format(_("%^s�����Ȃ��������߂����B", "%^s commands you to return."), m_name);

    teleport_player_to(m_ptr->fy, m_ptr->fx, TELEPORT_PASSIVE);
    learn_spell(MS_TELE_TO);
}

void spell_RF6_TELE_AWAY(cptr m_name, int m_idx)
{
    disturb(1, 1);

    msg_format(_("%^s�Ƀe���|�[�g������ꂽ�B", "%^s teleports you away."), m_name);
    if ((p_ptr->pseikaku == SEIKAKU_COMBAT) || (inventory[INVEN_BOW].name1 == ART_CRIMSON))
        msg_print(_("�������`", ""));

    learn_spell(MS_TELE_AWAY);
    teleport_player_away(m_idx, 100);
}

void spell_RF6_TELE_LEVEL(bool blind, cptr m_name, int m_idx, int rlev)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s��������Ȍ��t���Ԃ₢���B", "%^s mumbles strangely."), m_name);
    else
        msg_format(_("%^s�����Ȃ��̑����w�������B", "%^s gestures at your feet."), m_name);

    if (p_ptr->resist_nexus)
    {
        msg_print(_("���������ʂ��Ȃ������I", "You are unaffected!"));
    }
    else if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������͂𒵂˕Ԃ����I", "You resist the effects!"));
    }
    else
    {
        teleport_level(0);
    }
    learn_spell(MS_TELE_LEVEL);
    update_smart_learn(m_idx, DRS_NEXUS);
}

int spell_RF6_PSY_SPEAR(bool blind, cptr m_name, monster_race* r_ptr, int rlev, int m_idx, bool learnable)
{
    int dam;
    disturb(1, 1);
    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����̌���������B", "%^s throw a Psycho-Spear."), m_name);

    dam = (r_ptr->flags2 & RF2_POWERFUL) ? (randint1(rlev * 2) + 150) : (randint1(rlev * 3 / 2) + 100);
    beam(m_idx, GF_PSY_SPEAR, dam, MS_PSY_SPEAR, learnable);
    return dam;
}

void spell_RF6_DARKNESS(bool blind, cptr m_name, bool can_use_lite_area)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else if (can_use_lite_area)
        msg_format(_("%^s���ӂ�𖾂邭�Ƃ炵���B", "%^s cast a spell to light up."), m_name);
    else
        msg_format(_("%^s���Èł̒��Ŏ��U�����B", "%^s gestures in shadow."), m_name);

    if (can_use_lite_area) (void)lite_area(0, 3);
    else
    {
        learn_spell(MS_DARKNESS);
        (void)unlite_area(0, 3);
    }
}

void spell_RF6_TRAPS(bool blind, cptr m_name, int y, int x)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢�Ď׈��ɔ��΂񂾁B",
        "%^s mumbles, and then cackles evilly."), m_name);
    else
        msg_format(_("%^s�������������Ď׈��ɔ��΂񂾁B",
        "%^s casts a spell and cackles evilly."), m_name);

    learn_spell(MS_MAKE_TRAP);
    (void)trap_creation(y, x);
}

void spell_RF6_FORGET(cptr m_name, int rlev)
{
    disturb(1, 1);

    msg_format(_("%^s�����Ȃ��̋L�����������悤�Ƃ��Ă���B",
        "%^s tries to blank your mind."), m_name);

    if (randint0(100 + rlev / 2) < p_ptr->skill_sav)
    {
        msg_print(_("���������͂𒵂˕Ԃ����I", "You resist the effects!"));
    }
    else if (lose_all_info())
    {
        msg_print(_("�L��������Ă��܂����B", "Your memories fade away."));
    }
    learn_spell(MS_FORGET);
}

void spell_RF6_RAISE_DEAD(bool blind, cptr m_name, int m_idx, monster_type* m_ptr)
{
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����ҕ����̎������������B",
        "%^s casts a spell to revive corpses."), m_name);

    animate_dead(m_idx, m_ptr->fy, m_ptr->fx);
}

void spell_RF6_S_KIN(bool blind, cptr m_name, monster_type* m_ptr, monster_race* r_ptr, int m_idx, int y, int x, int rlev, u32b mode)
{
    int count = 0, k;
    disturb(1, 1);
    if (m_ptr->r_idx == MON_SERPENT || m_ptr->r_idx == MON_ZOMBI_SERPENT)
    {
        if (blind)
            msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s���_���W�����̎�����������B",
            "%^s magically summons guardians of dungeons."), m_name);
    }
    else
    {
        if (blind)
            msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
        else
#ifdef JP
            msg_format("%^s�͖��@��%s�����������B",
            m_name,
            ((r_ptr->flags1) & RF1_UNIQUE ?
            "�艺" : "����"));
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
                msg_print(_("�n�ʂ��琅�������o�����I", "Water blew off from the ground!"));
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
    
    if (blind && count)
        msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
}

void spell_RF6_S_CYBER(bool blind, cptr m_name, int m_idx, int y, int x)
{
    int count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���T�C�o�[�f�[���������������I",
        "%^s magically summons Cyberdemons!"), m_name);

    count = summon_cyber(m_idx, y, x);

    if (blind && count)
        msg_print(_("�d���ȑ������߂��ŕ�������B", "You hear heavy steps nearby."));
}

void spell_RF6_S_MONSTER(bool blind, cptr m_name, int m_idx, int y, int x, int rlev)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�Œ��Ԃ����������I", "%^s magically summons help!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }
    if (blind && count)
        msg_print(_("�������ԋ߂Ɍ��ꂽ��������B", "You hear something appear nearby."));
}

void spell_RF6_S_MONSTERS(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_6)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�Ń����X�^�[�����������I", "%^s magically summons monsters!"), m_name);

    for (k = 0; k < s_num_6; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, 0, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }

    if (blind && count)
        msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
}

void spell_RF6_S_ANT(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_6)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�ŃA�������������B", "%^s magically summons ants."), m_name);

    for (k = 0; k < s_num_6; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_ANT, PM_ALLOW_GROUP);
    }

    if (blind && count)
        msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
}

void spell_RF6_S_SPIDER(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_6)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�ŃN�������������B", "%^s magically summons spiders."), m_name);

    for (k = 0; k < s_num_6; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_SPIDER, PM_ALLOW_GROUP);
    }

    if (blind && count)
        msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
}

void spell_RF6_S_HOUND(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_4)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�Ńn�E���h�����������B", "%^s magically summons hounds."), m_name);

    for (k = 0; k < s_num_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_HOUND, PM_ALLOW_GROUP);
    }

    if (blind && count)
        msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
}

void spell_RF6_S_HYDRA(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_4)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�Ńq�h�������������B", "%^s magically summons hydras."), m_name);

    for (k = 0; k < s_num_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_HYDRA, PM_ALLOW_GROUP);
    }
    if (blind && count)
        msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
}

void spell_RF6_S_ANGEL(bool blind, cptr m_name, monster_race* r_ptr, int m_idx, int y, int x, int rlev)
{
    int k, count = 0;
    int num = 1;

    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�œV�g�����������I", "%^s magically summons an angel!"), m_name);

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
        if (blind && count)
            msg_print(_("�������ԋ߂Ɍ��ꂽ��������B", "You hear something appear nearby."));
    }
    else
    {
        if (blind)
            msg_print(_("�����̂��̂��ԋ߂Ɍ��ꂽ��������B", "You hear many things appear nearby."));
    }
}

void spell_RF6_S_DEMON(bool blind, cptr m_name, int m_idx, int y, int x, int rlev)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�͖��@�ō��ׂ̋{�삩�爫�������������I",
        "%^s magically summons a demon from the Courts of Chaos!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_DEMON, PM_ALLOW_GROUP);
    }

    if (blind && count)
        msg_print(_("�������ԋ߂Ɍ��ꂽ��������B", "You hear something appear nearby."));
}

void spell_RF6_S_UNDEAD(bool blind, cptr m_name, int m_idx, int y, int x, int rlev)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�ŃA���f�b�h�̋��G�����������I",
        "%^s magically summons an undead adversary!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_UNDEAD, PM_ALLOW_GROUP);
    }

    if (blind && count)
        msg_print(_("�������ԋ߂Ɍ��ꂽ��������B", "You hear something appear nearby."));
}

void spell_RF6_S_DRAGON(bool blind, cptr m_name, int m_idx, int y, int x, int rlev)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�Ńh���S�������������I", "%^s magically summons a dragon!"), m_name);

    for (k = 0; k < 1; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_DRAGON, PM_ALLOW_GROUP);
    }
    if (blind && count)
        msg_print(_("�������ԋ߂Ɍ��ꂽ��������B", "You hear something appear nearby."));
}

void spell_RF6_S_HI_UNDEAD(monster_type* m_ptr, bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_6, u32b mode)
{
    int k, count = 0;
    disturb(1, 1);

    if (((m_ptr->r_idx == MON_MORGOTH) || (m_ptr->r_idx == MON_SAURON) || (m_ptr->r_idx == MON_ANGMAR)) && ((r_info[MON_NAZGUL].cur_num + 2) < r_info[MON_NAZGUL].max_num))
    {
        int cy = y;
        int cx = x;

        if (blind)
            msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s�����@�ŗH�S��������������I", "%^s magically summons rangers of Nazgul!"), m_name);

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
                    msg_format(_("�u�H�S���%d���A�i�Y�O���E�u���b�N�I�v",
                    "A Nazgul says 'Nazgul-Rangers Number %d, Nazgul-Black!'"), count);
                else
                    msg_format(_("�u������%d���A�i�Y�O���E�u���b�N�I�v",
                    "Another one says 'Number %d, Nazgul-Black!'"), count);

                msg_print(NULL);
            }
        }
        msg_format(_("�u%d�l������āA�����O�����W���[�I�v",
            "They say 'The %d meets! We are the Ring-Ranger!'."), count);
        msg_print(NULL);
    }
    else
    {
        if (blind)
            msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
        else
            msg_format(_("%^s�����@�ŋ��͂ȃA���f�b�h�����������I",
            "%^s magically summons greater undead!"), m_name);

        for (k = 0; k < s_num_6; k++)
        {
            count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_UNDEAD, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
        }
    }
    if (blind && count)
    {
        msg_print(_("�ԋ߂ŉ��������̂��̂�������鉹����������B",
            "You hear many creepy things appear nearby."));
    }
}


void spell_RF6_S_HI_DRAGON(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_4)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�ŌÑ�h���S�������������I", "%^s magically summons ancient dragons!"), m_name);

    for (k = 0; k < s_num_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_HI_DRAGON, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }
    if (blind && count)
    {
        msg_print(_("�����̗͋������̂��ԋ߂Ɍ��ꂽ������������B",
            "You hear many powerful things appear nearby."));
    }
}

void spell_RF6_S_AMBERITES(bool blind, cptr m_name, int m_idx, int y, int x, int rlev, int s_num_4)
{
    int k, count = 0;
    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s���A���o�[�̉��������������I", "%^s magically summons Lords of Amber!"), m_name);

    for (k = 0; k < s_num_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_AMBERITES, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }
    if (blind && count)
    {
        msg_print(_("�s���̎҂��߂��Ɍ����̂����������B", "You hear immortal beings appear nearby."));
    }
}

void spell_RF6_S_UNIQUE(bool blind, cptr m_name, int m_idx, monster_type* m_ptr, int y, int x, int rlev, int s_num_4)
{
    int k, count = 0;
    bool uniques_are_summoned = FALSE;
    int non_unique_type = SUMMON_HI_UNDEAD;

    disturb(1, 1);

    if (blind)
        msg_format(_("%^s���������Ԃ₢���B", "%^s mumbles."), m_name);
    else
        msg_format(_("%^s�����@�œ��ʂȋ��G�����������I", "%^s magically summons special opponents!"), m_name);

    for (k = 0; k < s_num_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, SUMMON_UNIQUE, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }

    if (count) uniques_are_summoned = TRUE;

    if ((m_ptr->sub_align & (SUB_ALIGN_GOOD | SUB_ALIGN_EVIL)) == (SUB_ALIGN_GOOD | SUB_ALIGN_EVIL))
        non_unique_type = 0;
    else if (m_ptr->sub_align & SUB_ALIGN_GOOD)
        non_unique_type = SUMMON_ANGEL;

    for (k = count; k < s_num_4; k++)
    {
        count += summon_specific(m_idx, y, x, rlev, non_unique_type, (PM_ALLOW_GROUP | PM_ALLOW_UNIQUE));
    }

    if (blind && count)
    {
        msg_format(_("������%s���ԋ߂Ɍ��ꂽ������������B", "You hear many %s appear nearby."),
            uniques_are_summoned ? _("�͋�������", "powerful things") : _("����", "things"));
    }
}

