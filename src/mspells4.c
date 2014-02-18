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
    {
        msg_format(_("%^s���������϶����Ĥ֤䤤����", "%^s mumbles powerfully."), m_name);
    }
    else
    {
        msg_format(_("%^s�����Ͼõ�μ�ʸ��ǰ������", "%^s invokes a dispel magic."), m_name);
    }
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
    {
        msg_format(_("%^s��������ͤä���", "%^s shoots something."), m_name);
    }
    else
    {
        msg_format(_("%^s�����åȤ�ȯ�ͤ�����", "%^s fires a rocket."), m_name);
    }

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
    {
        msg_format(_("%^s����̯�ʲ���ȯ������", "%^s makes a strange noise."), m_name);
    }
    else
    {
        msg_format(_("%^s��������ä���", "%^s fires an arrow."), m_name);
    }

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