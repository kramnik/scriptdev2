/* Copyright (C) 2006 - 2012 ScriptDev2 <http://www.scriptdev2.com/>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: Instance_The_Eye
SD%Complete: 100
SDComment:
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "precompiled.h"
#include "the_eye.h"

instance_the_eye::instance_the_eye(Map* pMap) : ScriptedInstance(pMap),
    m_uiKaelthasEventPhase(0)
{
    Initialize();
}

void instance_the_eye::Initialize()
{
    memset(&m_auiEncounter, 0, sizeof(m_auiEncounter));
}

bool instance_the_eye::IsEncounterInProgress() const
{
    for (uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            return true;
    }

    if (PHASE_1_ADVISOR <= m_uiKaelthasEventPhase  && m_uiKaelthasEventPhase <= PHASE_5_GRAVITY)
        return true;

    return false;
}

void instance_the_eye::OnCreatureCreate(Creature* pCreature)
{
    switch (pCreature->GetEntry())
    {
        case NPC_THALADRED:
        case NPC_TELONICUS:
        case NPC_CAPERNIAN:
        case NPC_SANGUINAR:
        case NPC_KAELTHAS:
            m_mNpcEntryGuidStore[pCreature->GetEntry()] = pCreature->GetObjectGuid();
            break;
    }
}

void instance_the_eye::OnObjectCreate(GameObject* pGo)
{
    switch(pGo->GetEntry())
    {
        case GO_RAID_DOOR_ALAR_1:
        case GO_RAID_DOOR_ALAR_2:
            if (m_auiEncounter[0] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARCANE_DOOR_ASTRO_1:
        case GO_ARCANE_DOOR_ASTRO_2:
            if (m_auiEncounter[1] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        case GO_ARCANE_DOOR_REAVER_1:
        case GO_ARCANE_DOOR_REAVER_2:
            if (m_auiEncounter[2] == DONE)
                pGo->SetGoState(GO_STATE_ACTIVE);
            break;
        default:
            return;
    }
    m_mGoEntryGuidStore[pGo->GetEntry()] = pGo->GetObjectGuid();
}

void instance_the_eye::SetData(uint32 uiType, uint32 uiData)
{
    switch (uiType)
    {
        case TYPE_ALAR:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_RAID_DOOR_ALAR_1);
                DoUseDoorOrButton(GO_RAID_DOOR_ALAR_2);
            }
            break;
        case TYPE_SOLARIAN:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARCANE_DOOR_ASTRO_1);
                DoUseDoorOrButton(GO_ARCANE_DOOR_ASTRO_2);
            }
            break;
        case TYPE_VOIDREAVER:
            m_auiEncounter[uiType] = uiData;
            if (uiData == DONE)
            {
                DoUseDoorOrButton(GO_ARCANE_DOOR_REAVER_1);
                DoUseDoorOrButton(GO_ARCANE_DOOR_REAVER_2);
            }
            break;

        case TYPE_KAELTHAS_PHASE:
            m_uiKaelthasEventPhase = uiData;
            break;
    }

    if (uiData == DONE)
    {
        OUT_SAVE_INST_DATA;

        std::ostringstream saveStream;

        saveStream << m_auiEncounter[0] << " " << m_auiEncounter[1] << " " << m_auiEncounter[2] << " "
            << m_auiEncounter[3];

        m_strInstData = saveStream.str();

        SaveToDB();
        OUT_SAVE_INST_DATA_COMPLETE;
    }
}

uint32 instance_the_eye::GetData(uint32 uiType)
{
    switch(uiType)
    {
        case TYPE_ALAR:
        case TYPE_SOLARIAN:
        case TYPE_VOIDREAVER:
            return m_auiEncounter[uiType];
        case TYPE_KAELTHAS_PHASE:
            return m_uiKaelthasEventPhase;

        default:
            return 0;
    }
}

void instance_the_eye::Load(const char* chrIn)
{
    if (!chrIn)
    {
        OUT_LOAD_INST_DATA_FAIL;
        return;
    }

    OUT_LOAD_INST_DATA(chrIn);

    std::istringstream loadStream(chrIn);
    loadStream >> m_auiEncounter[0] >> m_auiEncounter[1] >> m_auiEncounter[2] >> m_auiEncounter[3];

    for(uint8 i = 0; i < MAX_ENCOUNTER; ++i)
    {
        if (m_auiEncounter[i] == IN_PROGRESS)
            m_auiEncounter[i] = NOT_STARTED;
    }

    OUT_LOAD_INST_DATA_COMPLETE;
}

InstanceData* GetInstanceData_instance_the_eye(Map* pMap)
{
    return new instance_the_eye(pMap);
}

void AddSC_instance_the_eye()
{
    Script* pNewScript;

    pNewScript = new Script;
    pNewScript->Name = "instance_the_eye";
    pNewScript->GetInstanceData = &GetInstanceData_instance_the_eye;
    pNewScript->RegisterSelf();
}
