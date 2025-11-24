/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        game_sa/CCullZoneManager.h
 *  PURPOSE:     Header file for cull zone class
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once
#include <cstdint>

struct Vector3
{
    float x, y, z;
};

enum eZoneAttributes : uint16_t
{
    ZA_NONE = 0x0,
    CAM_CLOSE_IN = 0x1,
    STAIRS = 0x2,
    STPERSON_1ST = 0x4,
    NO_RAIN = 0x8,
    NO_POLICE = 0x10,
    LOAD_COLLISION = 0x40,
    TUNNEL_TRANSITION = 0x80,
    POLICE_ABANDON_CARS = 0x100,
    IN_ROOM_FOR_AUDIO = 0x200,
    FEWER_PEDS = 0x400,
    TUNNEL = 0x800,
    MILITARY_ZONE = 0x1000,
    EXTRA_AIR_RESISTANCE = 0x4000,
    FEWER_CARS = 0x8000
};

enum eMirrorFlags : uint8_t
{
    MF_NONE = 0x0,
    MF_MIRROR = 0x1,
    MF_SCREENS_1 = 0x2,
    MF_SCREENS_2 = 0x4,
    MF_SCREENS_3 = 0x8,
    MF_SCREENS_4 = 0x10,
    MF_SCREENS_5 = 0x20,
    MF_SCREENS_6 = 0x40
};

struct CZoneDef
{
    int16_t m_cornerX, m_cornerY;
    int16_t m_vec1X, m_vec1Y;
    int16_t m_vec2X, m_vec2Y;
    int16_t m_minZ, m_maxZ;

    void Init(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ)
    {
        m_cornerX = int16_t(pos.x - vec1X - vec2X);
        m_cornerY = int16_t(pos.y - vec1Y - vec2Y);
        m_vec1X = int16_t(vec1X + vec1X);
        m_vec1Y = int16_t(vec1Y + vec1Y);
        m_vec2X = int16_t(vec2X + vec2X);
        m_vec2Y = int16_t(vec2Y + vec2Y);
        m_minZ = int16_t(minZ);
        m_maxZ = int16_t(maxZ);
    }

    bool IsPointWithin(const Vector3& point) const
    {
        if ((float)m_minZ >= point.z || (float)m_maxZ <= point.z)
            return false;

        float dx = point.x - (float)m_cornerX;
        float dy = point.y - (float)m_cornerY;

        float sqMag0 = (float)m_vec1X * dx + (float)m_vec1Y * dy;
        if (sqMag0 < 0.0f || sqMag0 > ((float)m_vec1X * (float)m_vec1X + (float)m_vec1Y * (float)m_vec1Y))
            return false;

        float sqMag1 = (float)m_vec2X * dx + (float)m_vec2Y * dy;
        if (sqMag1 < 0.0f || sqMag1 > ((float)m_vec2X * (float)m_vec2X + (float)m_vec2Y * (float)m_vec2Y))
            return false;

        return true;
    }
};

struct CMirrorAttributeZone
{
    CZoneDef     zoneDef;
    float        mirrorV;
    int8_t       mirrorNormalX, mirrorNormalY, mirrorNormalZ;
    eMirrorFlags flags;

    bool IsPointWithin(const Vector3& point) const { return zoneDef.IsPointWithin(point); }
};

struct CAttributeZone
{
    CZoneDef        zoneDef;
    eZoneAttributes flags;

    bool IsPointWithin(const Vector3& point) const { return zoneDef.IsPointWithin(point); }
};

class CCullZoneManagerSA
{
public:
    static inline CAttributeZone       aAttributeZones[1300];
    static inline CAttributeZone       aTunnelAttributeZones[40];
    static inline CMirrorAttributeZone aMirrorAttributeZones[72];

    static inline int32_t NumAttributeZones = 0;
    static inline int32_t NumTunnelAttributeZones = 0;
    static inline int32_t NumMirrorAttributeZones = 0;

    int32_t CurrentFlags_Player = 0;
    int32_t CurrentFlags_Camera = 0;

public:
    CCullZoneManagerSA();
    void Init();

    void AddCullZone(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ, uint16_t flags);
    void AddTunnelAttributeZone(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ, eZoneAttributes flags);
    void AddMirrorAttributeZone(const Vector3& pos, float vec1X, float vec1Y, float minZ, float vec2X, float vec2Y, float maxZ, eZoneAttributes flags,
                                float mirrorV, float mirrorNormalX, float mirrorNormalY, float mirrorNormalZ);

    bool CamCloseInForPlayer() const;
    bool CamStairsForPlayer() const;
    bool Cam1stPersonForPlayer() const;
    bool NoPolice() const;
    bool PoliceAbandonCars() const;
    bool InRoomForAudio() const;
    bool FewerCars() const;
    bool FewerPeds() const;
    bool CamNoRain() const;
    bool PlayerNoRain() const;
    bool DoExtraAirResistanceForPlayer() const;
    bool DoINeedToLoadCollision() const;
};
