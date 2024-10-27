/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* ScriptData
SDName: Areatrigger_Scripts
SD%Complete: 100
SDComment: Scripts for areatriggers
SDCategory: Areatrigger
EndScriptData */

/* ContentData
at_coilfang_waterfall           4591
at_legion_teleporter            4560 Teleporter TO Invasion Point: Cataclysm
at_stormwright_shelf            q12741
at_last_rites                   q12019
at_sholazar_waygate             q12548
at_nats_landing                 q11209
at_bring_your_orphan_to         q910 q910 q1800 q1479 q1687 q1558 q10951 q10952
at_brewfest
at_area_52_entrance
EndContentData */

#include "AreaTriggerAI.h"
#include "GameTime.h"
#include "InstanceScript.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"

/*######
## at_coilfang_waterfall
######*/

enum eCoilfangGOs
{
    GO_COILFANG_WATERFALL   = 184212
};

class AreaTrigger_at_coilfang_waterfall : public AreaTriggerScript
{
    public:

        AreaTrigger_at_coilfang_waterfall()
            : AreaTriggerScript("at_coilfang_waterfall")
        {
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*enter*/) override
        {
            if (GameObject* go = GetClosestGameObjectWithEntry(player, GO_COILFANG_WATERFALL, 35.0f))
                if (go->getLootState() == GO_READY)
                    go->UseDoorOrButton();

            return false;
        }
};

/*#####
## at_legion_teleporter
#####*/

enum eLegionTeleporter
{
    SPELL_TELE_A_TO         = 37387,
    QUEST_GAINING_ACCESS_A  = 10589,

    SPELL_TELE_H_TO         = 37389,
    QUEST_GAINING_ACCESS_H  = 10604
};

class AreaTrigger_at_legion_teleporter : public AreaTriggerScript
{
    public:

        AreaTrigger_at_legion_teleporter()
            : AreaTriggerScript("at_legion_teleporter")
        {
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*enter*/) override
        {
            if (player->IsAlive() && !player->isInCombat())
            {
                if (player->GetTeam() == ALLIANCE && player->GetQuestRewardStatus(QUEST_GAINING_ACCESS_A))
                {
                    player->CastSpell(player, SPELL_TELE_A_TO, false);
                    return true;
                }

                if (player->GetTeam() == HORDE && player->GetQuestRewardStatus(QUEST_GAINING_ACCESS_H))
                {
                    player->CastSpell(player, SPELL_TELE_H_TO, false);
                    return true;
                }

                return false;
            }
            return false;
        }
};

/*######
## at_stormwright_shelf
######*/

enum eStormwrightShelf
{
    QUEST_STRENGTH_OF_THE_TEMPEST               = 12741,

    SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST      = 53067
};

class AreaTrigger_at_stormwright_shelf : public AreaTriggerScript
{
    public:

        AreaTrigger_at_stormwright_shelf()
            : AreaTriggerScript("at_stormwright_shelf")
        {
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*enter*/) override
        {
            if (!player->isDead() && player->GetQuestStatus(QUEST_STRENGTH_OF_THE_TEMPEST) == QUEST_STATUS_INCOMPLETE)
                player->CastSpell(player, SPELL_CREATE_TRUE_POWER_OF_THE_TEMPEST, false);

            return true;
        }
};

/*######
## at_scent_larkorwi
######*/

enum eScentLarkorwi
{
    QUEST_SCENT_OF_LARKORWI                     = 4291,
    NPC_LARKORWI_MATE                           = 9683
};

class AreaTrigger_at_scent_larkorwi : public AreaTriggerScript
{
    public:

        AreaTrigger_at_scent_larkorwi()
            : AreaTriggerScript("at_scent_larkorwi")
        {
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*enter*/) override
        {
            if (!player->isDead() && player->GetQuestStatus(QUEST_SCENT_OF_LARKORWI) == QUEST_STATUS_INCOMPLETE)
            {
                if (!player->FindNearestCreature(NPC_LARKORWI_MATE, 15))
                    player->SummonCreature(NPC_LARKORWI_MATE, player->GetPositionX()+5, player->GetPositionY(), player->GetPositionZ(), 3.3f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 100000);
            }

            return false;
        }
};

/*#####
## at_last_rites
#####*/

enum eAtLastRites
{
    QUEST_LAST_RITES                          = 12019,
    QUEST_BREAKING_THROUGH                    = 11898,
};

class AreaTrigger_at_last_rites : public AreaTriggerScript
{
    public:

        AreaTrigger_at_last_rites()
            : AreaTriggerScript("at_last_rites")
        {
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger, bool /*enter*/) override
        {
            if (!(player->GetQuestStatus(QUEST_LAST_RITES) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_LAST_RITES) == QUEST_STATUS_COMPLETE ||
                player->GetQuestStatus(QUEST_BREAKING_THROUGH) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_BREAKING_THROUGH) == QUEST_STATUS_COMPLETE))
                return false;

            WorldLocation pPosition;

            switch (trigger->ID)
            {
                case 5332:
                case 5338:
                    pPosition = WorldLocation(571, 3733.68f, 3563.25f, 290.812f, 3.665192f);
                    break;
                case 5334:
                    pPosition = WorldLocation(571, 3802.38f, 3585.95f, 49.5765f, 0.0f);
                    break;
                case 5340:
                    pPosition = WorldLocation(571, 3687.91f, 3577.28f, 473.342f, 0.0f);
                    break;
                default:
                    return false;
            }

            player->TeleportTo(pPosition);

            return false;
        }
};

/*######
## at_sholazar_waygate
######*/

enum eWaygate
{
    SPELL_SHOLAZAR_TO_UNGORO_TELEPORT           = 52056,
    SPELL_UNGORO_TO_SHOLAZAR_TELEPORT           = 52057,

    AT_SHOLAZAR                                 = 5046,
    AT_UNGORO                                   = 5047,

    QUEST_THE_MAKERS_OVERLOOK                   = 12613,
    QUEST_THE_MAKERS_PERCH                      = 12559,
};

class AreaTrigger_at_sholazar_waygate : public AreaTriggerScript
{
    public:

        AreaTrigger_at_sholazar_waygate()
            : AreaTriggerScript("at_sholazar_waygate")
        {
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger, bool /*enter*/) override
        {
            if (player->GetQuestStatus(QUEST_THE_MAKERS_OVERLOOK) == QUEST_STATUS_REWARDED && !player->isDead() &&
                player->GetQuestStatus(QUEST_THE_MAKERS_PERCH)    == QUEST_STATUS_REWARDED)
            {
                switch (trigger->ID)
                {
                    case AT_SHOLAZAR:
                        player->CastSpell(player, SPELL_SHOLAZAR_TO_UNGORO_TELEPORT, false);
                        break;

                    case AT_UNGORO:
                        player->CastSpell(player, SPELL_UNGORO_TO_SHOLAZAR_TELEPORT, false);
                        break;
                }
            }

            return false;
        }
};

/*######
## at_nats_landing
######*/

enum NatsLanding
{
    QUEST_NATS_BARGAIN = 27220,
    SPELL_FISH_PASTE   = 42644,
    NPC_LURKING_SHARK  = 23928
};

class AreaTrigger_at_nats_landing : public AreaTriggerScript
{
    public:
        AreaTrigger_at_nats_landing() : AreaTriggerScript("at_nats_landing") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/, bool /*enter*/) override
        {
            if (!player->IsAlive() || !player->HasAura(SPELL_FISH_PASTE))
                return false;

            if (player->GetQuestStatus(QUEST_NATS_BARGAIN) == QUEST_STATUS_INCOMPLETE)
            {
                if (!player->FindNearestCreature(NPC_LURKING_SHARK, 20.0f))
                {
                    if (Creature* shark = player->SummonCreature(NPC_LURKING_SHARK, -4246.243f, -3922.356f, -7.488f, 5.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 60000))
                        shark->AI()->AttackStart(player);

                    return false;
                }
            }
            return true;
        }
};

/*######
## at_bring_your_orphan_to
######*/

enum BringYourOrphanTo
{
    QUEST_DOWN_AT_THE_DOCKS         = 910,
    QUEST_GATEWAY_TO_THE_FRONTIER   = 911,
    QUEST_LORDAERON_THRONE_ROOM     = 1800,
    QUEST_BOUGHT_OF_ETERNALS        = 1479,
    QUEST_SPOOKY_LIGHTHOUSE         = 1687,
    QUEST_STONEWROUGHT_DAM          = 1558,
    QUEST_DARK_PORTAL_H             = 10951,
    QUEST_DARK_PORTAL_A             = 10952,

    AT_DOWN_AT_THE_DOCKS            = 3551,
    AT_GATEWAY_TO_THE_FRONTIER      = 3549,
    AT_LORDAERON_THRONE_ROOM        = 3547,
    AT_BOUGHT_OF_ETERNALS           = 3546,
    AT_SPOOKY_LIGHTHOUSE            = 3552,
    AT_STONEWROUGHT_DAM             = 3548,
    AT_DARK_PORTAL                  = 4356,

    AURA_ORPHAN_OUT                 = 58818,
};

class AreaTrigger_at_bring_your_orphan_to : public AreaTriggerScript
{
    public:
        AreaTrigger_at_bring_your_orphan_to() : AreaTriggerScript("at_bring_your_orphan_to") { }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger, bool /*enter*/) override
        {
            uint32 questId = 0;

            if (player->isDead() || !player->HasAura(AURA_ORPHAN_OUT))
                return false;

            switch (trigger->ID)
            {
                case AT_DOWN_AT_THE_DOCKS:
                    questId = QUEST_DOWN_AT_THE_DOCKS;
                    break;
                case AT_GATEWAY_TO_THE_FRONTIER:
                    questId = QUEST_GATEWAY_TO_THE_FRONTIER;
                    break;
                case AT_LORDAERON_THRONE_ROOM:
                    questId = QUEST_LORDAERON_THRONE_ROOM;
                    break;
                case AT_BOUGHT_OF_ETERNALS:
                    questId = QUEST_BOUGHT_OF_ETERNALS;
                    break;
                case AT_SPOOKY_LIGHTHOUSE:
                    questId = QUEST_SPOOKY_LIGHTHOUSE;
                    break;
                case AT_STONEWROUGHT_DAM:
                    questId = QUEST_STONEWROUGHT_DAM;
                    break;
                case AT_DARK_PORTAL:
                    questId = player->GetTeam() == ALLIANCE ? QUEST_DARK_PORTAL_A : QUEST_DARK_PORTAL_H;
                    break;
            }

            if (questId && player->GetQuestStatus(questId) == QUEST_STATUS_INCOMPLETE)
                player->AreaExploredOrEventHappens(questId);

            return true;
        }
};

/*######
## at_brewfest
######*/

enum Brewfest
{
    NPC_TAPPER_SWINDLEKEG       = 24711,
    NPC_IPFELKOFER_IRONKEG      = 24710,

    AT_BREWFEST_DUROTAR         = 4829,
    AT_BREWFEST_DUN_MOROGH      = 4820,

    SAY_WELCOME                 = 4,

    AREATRIGGER_TALK_COOLDOWN   = 5, // in seconds
};

class AreaTrigger_at_brewfest : public AreaTriggerScript
{
    public:
        AreaTrigger_at_brewfest() : AreaTriggerScript("at_brewfest")
        {
            // Initialize for cooldown
            _triggerTimes[AT_BREWFEST_DUROTAR] = _triggerTimes[AT_BREWFEST_DUN_MOROGH] = 0;
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger, bool /*enter*/) override
        {
            uint32 triggerId = trigger->ID;
            // Second trigger happened too early after first, skip for now
            if (GameTime::GetGameTime() - _triggerTimes[triggerId] < AREATRIGGER_TALK_COOLDOWN)
                return false;

            switch (triggerId)
            {
                case AT_BREWFEST_DUROTAR:
                    if (Creature* tapper = player->FindNearestCreature(NPC_TAPPER_SWINDLEKEG, 20.0f))
                        tapper->AI()->Talk(SAY_WELCOME, player->GetGUID());
                    break;
                case AT_BREWFEST_DUN_MOROGH:
                    if (Creature* ipfelkofer = player->FindNearestCreature(NPC_IPFELKOFER_IRONKEG, 20.0f))
                        ipfelkofer->AI()->Talk(SAY_WELCOME, player->GetGUID());
                    break;
                default:
                    break;
            }

            _triggerTimes[triggerId] = GameTime::GetGameTime();
            return false;
        }

    private:
        std::map<uint32, time_t> _triggerTimes;
};

/*######
## at_area_52_entrance
######*/

enum Area52Entrance
{
    SPELL_A52_NEURALYZER  = 34400,
    NPC_SPOTLIGHT         = 19913,
    SUMMON_COOLDOWN       = 5,

    AT_AREA_52_SOUTH      = 4472,
    AT_AREA_52_NORTH      = 4466,
    AT_AREA_52_WEST       = 4471,
    AT_AREA_52_EAST       = 4422,
};

class AreaTrigger_at_area_52_entrance : public AreaTriggerScript
{
    public:
        AreaTrigger_at_area_52_entrance() : AreaTriggerScript("at_area_52_entrance")
        {
            _triggerTimes[AT_AREA_52_SOUTH] = _triggerTimes[AT_AREA_52_NORTH] = _triggerTimes[AT_AREA_52_WEST] = _triggerTimes[AT_AREA_52_EAST] = 0;
        }

        bool OnTrigger(Player* player, AreaTriggerEntry const* trigger, bool /*enter*/) override
        {
            float x = 0.0f, y = 0.0f, z = 0.0f;

            if (!player->IsAlive())
                return false;

            uint32 triggerId = trigger->ID;
            if (GameTime::GetGameTime() - _triggerTimes[triggerId] < SUMMON_COOLDOWN)
                return false;

            switch (triggerId)
            {
                case AT_AREA_52_EAST:
                    x = 3044.176f;
                    y = 3610.692f;
                    z = 143.61f;
                    break;
                case AT_AREA_52_NORTH:
                    x = 3114.87f;
                    y = 3687.619f;
                    z = 143.62f;
                    break;
                case AT_AREA_52_WEST:
                    x = 3017.79f;
                    y = 3746.806f;
                    z = 144.27f;
                    break;
                case AT_AREA_52_SOUTH:
                    x = 2950.63f;
                    y = 3719.905f;
                    z = 143.33f;
                    break;
            }

            player->SummonCreature(NPC_SPOTLIGHT, x, y, z, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 5000);
            player->AddAura(SPELL_A52_NEURALYZER, player);
            _triggerTimes[trigger->ID] = GameTime::GetGameTime();
            return false;
        }

    private:
        std::map<uint32, time_t> _triggerTimes;
};

//9464
class areatrigger_at_dinner_party : public AreaTriggerScript
{
    public:
        areatrigger_at_dinner_party() : AreaTriggerScript("areatrigger_at_dinner_party") {}

    struct areatrigger_at_dinner_partyAI : AreaTriggerAI
    {
        areatrigger_at_dinner_partyAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

        bool IsValidTarget(Unit* caster, Unit* target, AreaTriggerActionMoment actionM) override
        {
            if (!target->ToCreature())
                return false;

            switch (target->GetEntry())
            {
                case 114316:
                case 114317:
                case 114318:
                case 114319:
                case 114320:
                case 114321:
                    return true;
                default:
                    return false;
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_at_dinner_partyAI(areatrigger);
    }
};

//51600
class areatrigger_skorpyron_scorpid_swarm : public AreaTriggerScript
{
    public:
        areatrigger_skorpyron_scorpid_swarm() : AreaTriggerScript("areatrigger_skorpyron_scorpid_swarm") {}

    struct areatrigger_skorpyron_scorpid_swarmAI : AreaTriggerAI
    {
        areatrigger_skorpyron_scorpid_swarmAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

        bool IsValidTarget(Unit* caster, Unit* target, AreaTriggerActionMoment actionM) override
        {
            if (!target->ToCreature())
                return false;

            switch (target->GetEntry())
            {
                case 103217:
                case 103224:
                case 103225:
                    return true;
                default:
                    return false;
            }
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new areatrigger_skorpyron_scorpid_swarmAI(areatrigger);
    }
};

//12916
class at_medivh_flame_wreath : public AreaTriggerScript
{
    public:
        at_medivh_flame_wreath() : AreaTriggerScript("at_medivh_flame_wreath") {}

    struct at_medivh_flame_wreathAI : AreaTriggerAI
    {
        at_medivh_flame_wreathAI(AreaTrigger* areatrigger) : AreaTriggerAI(areatrigger) {}

        bool IsValidTarget(Unit* caster, Unit* target, AreaTriggerActionMoment actionM) override
        {
            if (caster && caster->GetAI()->GetObjectData(target->GetGUID()))
                return false;

            return true;
        }
    };

    AreaTriggerAI* GetAI(AreaTrigger* areatrigger) const override
    {
        return new at_medivh_flame_wreathAI(areatrigger);
    }
};

void AddSC_areatrigger_scripts()
{
    new AreaTrigger_at_coilfang_waterfall();
    new AreaTrigger_at_legion_teleporter();
    new AreaTrigger_at_stormwright_shelf();
    new AreaTrigger_at_scent_larkorwi();
    new AreaTrigger_at_last_rites();
    new AreaTrigger_at_sholazar_waygate();
    new AreaTrigger_at_nats_landing();
    new AreaTrigger_at_bring_your_orphan_to();
    new AreaTrigger_at_brewfest();
    new AreaTrigger_at_area_52_entrance();
    new areatrigger_at_dinner_party();
    new areatrigger_skorpyron_scorpid_swarm();
    new at_medivh_flame_wreath();
}
