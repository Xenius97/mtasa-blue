/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        Shared/mods/deathmatch/logic/lua/CLuaCompatibility.h
 *  PURPOSE:     Lua version compatibility layer
 *
 *  Multi Theft Auto is available from https://www.multitheftauto.com/
 *
 *****************************************************************************/

#pragma once

// Lua version enumeration for MTA:SA
enum eLuaVersion
{
    LUA_VERSION_UNKNOWN = 0,
    LUA_VERSION_51 = 501,
    LUA_VERSION_54 = 504
};

// Include appropriate Lua headers based on version detection
#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 504
    // Lua 5.4 compatibility definitions for 5.1 APIs
    
    // LUA_GLOBALSINDEX was removed in 5.2+
    // In 5.4, globals are accessed via LUA_REGISTRYINDEX with key LUA_RIDX_GLOBALS
    #define LUA_GLOBALSINDEX_COMPAT    (-10002)
    
    // LUA_ENVIRONINDEX was removed in 5.2+
    #define LUA_ENVIRONINDEX_COMPAT    (-10001)
    
    // Compatibility layer functions
    inline void lua_getglobal_compat(lua_State* L, const char* name, int version) {
        if (version == LUA_VERSION_54) {
            lua_getglobal(L, name);
        } else {
            lua_getfield(L, LUA_GLOBALSINDEX, name);
        }
    }
    
    inline void lua_setglobal_compat(lua_State* L, const char* name, int version) {
        if (version == LUA_VERSION_54) {
            lua_setglobal(L, name);
        } else {
            lua_setfield(L, LUA_GLOBALSINDEX, name);
        }
    }
    
    // lua_getuservalue/lua_setuservalue changed in 5.4 to support multiple values
    inline int lua_getuservalue_compat(lua_State* L, int idx, int version) {
        if (version == LUA_VERSION_54) {
            return lua_getiuservalue(L, idx, 1);  // Get first user value
        } else {
            lua_getuservalue(L, idx);
            return lua_type(L, -1);
        }
    }
    
    inline void lua_setuservalue_compat(lua_State* L, int idx, int version) {
        if (version == LUA_VERSION_54) {
            lua_setiuservalue(L, idx, 1);  // Set first user value
        } else {
            lua_setuservalue(L, idx);
        }
    }

#elif defined(LUA_VERSION_NUM) && LUA_VERSION_NUM == 501
    // Lua 5.1 - use native defines
    #define LUA_GLOBALSINDEX_COMPAT    LUA_GLOBALSINDEX
    #define LUA_ENVIRONINDEX_COMPAT    LUA_ENVIRONINDEX
    
    inline void lua_getglobal_compat(lua_State* L, const char* name, int version) {
        lua_getfield(L, LUA_GLOBALSINDEX, name);
    }
    
    inline void lua_setglobal_compat(lua_State* L, const char* name, int version) {
        lua_setfield(L, LUA_GLOBALSINDEX, name);
    }
    
    inline int lua_getuservalue_compat(lua_State* L, int idx, int version) {
        lua_getuservalue(L, idx);
        return lua_type(L, -1);
    }
    
    inline void lua_setuservalue_compat(lua_State* L, int idx, int version) {
        lua_setuservalue(L, idx);
    }

#else
    #error "Unknown Lua version"
#endif

// Helper function to get Lua version from lua_State
inline eLuaVersion GetLuaVersion(lua_State* L) {
    if (!L) return LUA_VERSION_UNKNOWN;
    
    lua_Number version = lua_version(L);
    if (version == 501) {
        return LUA_VERSION_51;
    } else if (version == 504) {
        return LUA_VERSION_54;
    }
    
    return LUA_VERSION_UNKNOWN;
}

// Helper to convert version number to string
inline const char* GetLuaVersionString(eLuaVersion version) {
    switch (version) {
        case LUA_VERSION_51: return "5.1";
        case LUA_VERSION_54: return "5.4";
        default: return "Unknown";
    }
}

// Helper to parse version string to enum
inline eLuaVersion ParseLuaVersion(const char* versionStr) {
    if (!versionStr) return LUA_VERSION_51;  // Default to 5.1
    
    if (strcmp(versionStr, "5.4") == 0 || strcmp(versionStr, "54") == 0) {
        return LUA_VERSION_54;
    } else if (strcmp(versionStr, "5.1") == 0 || strcmp(versionStr, "51") == 0) {
        return LUA_VERSION_51;
    }
    
    return LUA_VERSION_51;  // Default to 5.1 for backward compatibility
}
