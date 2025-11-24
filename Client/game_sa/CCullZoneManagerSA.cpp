/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        game_sa/CCullZoneManager.cpp
 *  PURPOSE:     Cull zone manager
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#include "StdInc.h"
#include "CCullZoneManagerSA.h"

CCullZoneManagerSA::CCullZoneManagerSA()
{
    Init();
}

void CCullZoneManagerSA::Init()
{
    CurrentFlags_Player = 0;
    CurrentFlags_Camera = 0;
    NumAttributeZones = 0;
    NumTunnelAttributeZones = 0;
    NumMirrorAttributeZones = 0;
}

void CCullZoneManagerSA::AddCullZone(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ, uint16_t flags)
{
    if (flags & TUNNEL_TRANSITION || flags & TUNNEL)
        AddTunnelAttributeZone(pos, vec1X, vec1Y, minZ, vec2X, vec2Y, maxZ, (eZoneAttributes)flags);

    if (flags)
    {
        CAttributeZone& zone = aAttributeZones[NumAttributeZones++];
        zone.zoneDef.Init(pos, vec1X, vec1Y, minZ, vec2X, vec2Y, maxZ);
        zone.flags = (eZoneAttributes)flags;
    }
}

void CCullZoneManagerSA::AddTunnelAttributeZone(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ,
                                                eZoneAttributes flags)
{
    CAttributeZone& zone = aTunnelAttributeZones[NumTunnelAttributeZones++];
    zone.zoneDef.Init(pos, vec1X, vec1Y, minZ, vec2X, vec2Y, maxZ);
    zone.flags = flags;
}

void CCullZoneManagerSA::AddMirrorAttributeZone(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ,
                                                eZoneAttributes flags, float mirrorV, float mirrorNormalX, float mirrorNormalY, float mirrorNormalZ)
{
    CMirrorAttributeZone& zone = aMirrorAttributeZones[NumMirrorAttributeZones++];
    zone.zoneDef.Init(pos, vec1X, vec1Y, minZ, vec2X, vec2Y, maxZ);
    zone.flags = (eMirrorFlags)flags;
    zone.mirrorV = mirrorV;
    zone.mirrorNormalX = int8_t(mirrorNormalX * 100.0f);
    zone.mirrorNormalY = int8_t(mirrorNormalY * 100.0f);
    zone.mirrorNormalZ = int8_t(mirrorNormalZ * 100.0f);
}

bool CCullZoneManagerSA::CamCloseInForPlayer() const
{
    return CurrentFlags_Player & CAM_CLOSE_IN;
}

bool CCullZoneManagerSA::CamStairsForPlayer() const
{
    return CurrentFlags_Player & STAIRS;
}
bool CCullZoneManagerSA::Cam1stPersonForPlayer() const
{
    return CurrentFlags_Player & STPERSON_1ST;
}

bool CCullZoneManagerSA::NoPolice() const
{
    return CurrentFlags_Player & NO_POLICE;
}

bool CCullZoneManagerSA::PoliceAbandonCars() const
{
    return CurrentFlags_Player & POLICE_ABANDON_CARS;
}

bool CCullZoneManagerSA::InRoomForAudio() const
{
    return CurrentFlags_Camera & IN_ROOM_FOR_AUDIO;
}

bool CCullZoneManagerSA::FewerCars() const
{
    return CurrentFlags_Player & FEWER_CARS;
}

bool CCullZoneManagerSA::FewerPeds() const
{
    return CurrentFlags_Player & FEWER_PEDS;
}

bool CCullZoneManagerSA::CamNoRain() const
{
    return CurrentFlags_Camera & NO_RAIN;
}

bool CCullZoneManagerSA::PlayerNoRain() const
{
    return CurrentFlags_Player & NO_RAIN;
}

bool CCullZoneManagerSA::DoExtraAirResistanceForPlayer() const
{
    return CurrentFlags_Player & EXTRA_AIR_RESISTANCE;
}

bool CCullZoneManagerSA::DoINeedToLoadCollision() const
{
    return CurrentFlags_Player & LOAD_COLLISION;
}
