/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_THE_EYE_H
#define DEF_THE_EYE_H

enum
{
    MAX_ENCOUNTER               = 3,

    TYPE_ALAR                   = 0,
    TYPE_SOLARIAN               = 1,
    TYPE_VOIDREAVER             = 2,

    TYPE_KAELTHAS_PHASE         = 3,                        // not regular encounter, contains phase instead

    //NPC_ASTROMANCER           = 18805,
    NPC_KAELTHAS                = 19622,

    NPC_CAPERNIAN               = 20062,
    NPC_SANGUINAR               = 20060,
    NPC_TELONICUS               = 20063,
    NPC_THALADRED               = 20064,

    // gameobjects
    GO_RAID_DOOR_ALAR_1         = 184329,                   // Alar
    GO_RAID_DOOR_ALAR_2         = 184327,                   // Alar
    GO_ARCANE_DOOR_ASTRO_1      = 184326,                   // Astromancer
    GO_ARCANE_DOOR_ASTRO_2      = 184328,                   // Astromancer
    GO_ARCANE_DOOR_REAVER_1     = 184325,                   // Void Reaver
    GO_ARCANE_DOOR_REAVER_2     = 184324,                   // Void Reaver
    GO_KAEL_STATUE_LEFT         = 184597,
    GO_KAEL_STATUE_RIGHT        = 184596,
};

enum KaelPhases
{
    PHASE_0_NOT_BEGUN           = 0,
    PHASE_1_ADVISOR             = 1,
    PHASE_2_WEAPON              = 2,
    PHASE_3_ADVISOR_ALL         = 3,
    PHASE_4_SOLO                = 4,
    PHASE_5_GRAVITY             = 5,
    PHASE_6_COMPLETE            = 6,
};

class MANGOS_DLL_DECL instance_the_eye : public ScriptedInstance
{
    public:
        instance_the_eye(Map* pMap);

        void Initialize();
        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);

        const char* Save() { return m_strInstData.c_str(); }
        void Load(const char* chrIn);

    private:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string m_strInstData;

        uint32 m_uiKaelthasEventPhase;
};

#endif
