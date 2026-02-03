# Lua 5.4 Implementation Details

## Overview

This document describes the technical implementation of Lua 5.4 support in MTA:SA, allowing resources to choose between Lua 5.1 and Lua 5.4 at runtime.

## Architecture

### Dual Lua Version Support

MTA:SA now includes both Lua 5.1 and Lua 5.4 compiled as separate libraries:
- **Lua 5.1**: `vendor/lua/` → builds to `lua5.1.dll/so` (server) and `lua5.1c.dll` (client)
- **Lua 5.4**: `vendor/lua54/` → builds to `lua5.4.dll/so` (server) and `lua5.4c.dll` (client)

### Key Components

#### 1. Compatibility Layer (`Shared/mods/deathmatch/logic/lua/CLuaCompatibility.h`)

Provides:
- `eLuaVersion` enum for version identification
- Helper functions to abstract API differences between Lua 5.1 and 5.4
- Version parsing utilities
- Compatibility macros for deprecated APIs

#### 2. CLuaMain Modifications

**Server**: `Server/mods/deathmatch/logic/lua/CLuaMain.{h,cpp}`  
**Client**: `Client/mods/deathmatch/logic/lua/CLuaMain.{h,cpp}`

Changes:
- Added `m_luaVersion` member variable (type: `eLuaVersion`)
- Updated constructor to accept version parameter (defaults to `LUA_VERSION_51`)
- Added `GetLuaVersion()` accessor method

#### 3. CLuaManager Modifications

**Server**: `Server/mods/deathmatch/logic/lua/CLuaManager.{h,cpp}`  
**Client**: `Client/mods/deathmatch/logic/lua/CLuaManager.{h,cpp}`

Changes:
- Updated `CreateVirtualMachine()` to accept `luaVersion` parameter
- Passes version to CLuaMain constructor

#### 4. CResource Modifications

**Location**: `Server/mods/deathmatch/logic/CResource.{h,cpp}`

Changes:
- Added `m_luaVersion` member (defaults to `LUA_VERSION_51`)
- Added `GetLuaVersion()` accessor
- Parse `lua_version` attribute from `<info>` tag in meta.xml
- Pass version to `CreateVirtualMachine()` call

### Version Selection Flow

```
meta.xml <info lua_version="5.4">
    ↓
CResource::Load() parses attribute
    ↓
CResource::m_luaVersion = ParseLuaVersion("5.4")
    ↓
CResource::CreateVM() calls CreateVirtualMachine(this, bOOP, m_luaVersion)
    ↓
CLuaManager creates CLuaMain with specified version
    ↓
CLuaMain::Initialize() uses appropriate Lua library
    ↓
Scripts execute in selected Lua version
```

## Lua 5.4 Source Modifications

To maintain compatibility with MTA's custom Lua 5.1 modifications, Lua 5.4 was modified with:

### Added Functions

1. **`lua_getmtasaowner(lua_State* L)`** - Returns CLuaMain pointer
   - Location: `vendor/lua54/src/lapi.c`
   - Returns `G(L)->mtasaowner`

2. **Modified `lua_newstate()`** - Accepts `mtasaowner` parameter
   - Location: `vendor/lua54/src/lstate.c`
   - Signature: `lua_State* lua_newstate(lua_Alloc f, void *ud, void *mtasaowner)`

3. **Modified `luaL_newstate()`** - Accepts `mtasaowner` parameter
   - Location: `vendor/lua54/src/lauxlib.c`
   - Signature: `lua_State* luaL_newstate(void *mtasaowner)`

### Modified Structures

**global_State** (`vendor/lua54/src/lstate.h`):
```c
typedef struct global_State {
    // ... existing fields ...
    void *mtasaowner;  // pointer to this state's CLuaMain
} global_State;
```

### Added Macro

**lua_open** (`vendor/lua54/src/lua.h`):
```c
#define lua_open(mtasaowner) luaL_newstate(mtasaowner)
```

## API Compatibility

### Lua 5.1 → 5.4 Breaking Changes

The compatibility layer handles:

1. **LUA_GLOBALSINDEX removal**
   - Lua 5.1: `LUA_GLOBALSINDEX` (-10002)
   - Lua 5.4: Use `LUA_RIDX_GLOBALS` from registry
   - Wrapper: `LUA_GLOBALSINDEX_COMPAT`

2. **lua_getuservalue/lua_setuservalue changes**
   - Lua 5.1: Single user value
   - Lua 5.4: Multiple user values via `lua_getiuservalue`/`lua_setiuservalue`
   - Wrappers: `lua_getuservalue_compat()`, `lua_setuservalue_compat()`

3. **Environment tables**
   - Lua 5.1: `LUA_ENVIRONINDEX`, `setfenv`/`getfenv`
   - Lua 5.4: Uses `_ENV` upvalue
   - Note: Scripts should migrate to `_ENV` pattern

## Build System

### Premake Configuration

**Main premake5.lua**:
```lua
include "vendor/lua"     -- Lua 5.1
include "vendor/lua54"   -- Lua 5.4
```

**Lua 5.4 Build** (`vendor/lua54/premake5.lua`):
- Project: `Lua54_Server` → `lua5.4.dll/so`
- Project: `Lua54_Client` → `lua5.4c.dll`
- Excludes: `lua.c`, `onelua.c`, `ltests.c` (standalone tools)

### Library Linking

Both Lua libraries are built but currently, runtime version selection is prepared at the infrastructure level. The actual runtime loading mechanism (if using dynamic loading) would need platform-specific implementation.

## Current Limitations

### Known Limitations

1. **Single Lua Version per Process**
   - Current implementation: Infrastructure supports version tracking
   - Full runtime switching would require dynamic library loading
   - Resources can specify version, but actual runtime dispatch needs completion

2. **Compiled Scripts**
   - Lua 5.1 and 5.4 use different bytecode formats
   - Compiled scripts are not compatible between versions
   - Scripts must be recompiled for the target version

3. **C Modules**
   - Lua C modules must be recompiled for each Lua version
   - Binary compatibility is not guaranteed

## Testing Strategy

### Unit Tests

1. **Version Parsing**
   - Test `ParseLuaVersion()` with various inputs
   - Verify default behavior (5.1)

2. **Version Tracking**
   - Verify CLuaMain stores correct version
   - Check CResource passes version correctly

3. **Meta.xml Parsing**
   - Test various `lua_version` attribute values
   - Verify backward compatibility (missing attribute)

### Integration Tests

1. **Resource Loading**
   - Load resource with `lua_version="5.4"`
   - Verify VM created with correct version
   - Check logs for version confirmation

2. **Feature Tests**
   - Test Lua 5.4 features (floor division, bitwise ops)
   - Verify Lua 5.1 resources still work
   - Test mixed resources (5.1 and 5.4 running simultaneously)

### Performance Tests

1. **Startup Time**
   - Measure resource loading time
   - Compare Lua 5.1 vs 5.4 initialization

2. **Runtime Performance**
   - Benchmark common operations
   - Compare GC performance

## Future Enhancements

### Planned Features

1. **Per-Script Version Selection**
   - Allow individual scripts to specify version
   - Syntax: `<script src="file.lua" type="server" lua_version="5.4" />`

2. **Cross-Version Exports**
   - Enable function exports between versions
   - Implement argument marshalling

3. **Automatic Migration Tools**
   - Script analyzer for compatibility issues
   - Automated code updates (unpack → table.unpack)

4. **API Versioning**
   - Version MTA-specific functions
   - Handle breaking changes gracefully

5. **JIT Support**
   - Consider LuaJIT integration
   - Performance optimization options

## Maintenance

### Adding Future Lua Versions

To add Lua X.Y support:

1. Copy Lua source to `vendor/luaXY/`
2. Apply mtasaowner modifications
3. Create `premake5.lua` with appropriate project names
4. Add to main `premake5.lua` includes
5. Update `eLuaVersion` enum
6. Add parsing in `ParseLuaVersion()`
7. Test compatibility layer
8. Update documentation

### Debugging Tips

1. **Check Lua Version**
   ```lua
   print("Lua version: " .. _VERSION)
   ```

2. **Verify Resource Version**
   ```lua
   -- In server/client code
   local version = getResourceInfo(getThisResource(), "lua_version") or "5.1"
   print("Resource Lua version: " .. version)
   ```

3. **Log VM Creation**
   - Add logging in `CLuaManager::CreateVirtualMachine()`
   - Log version parameter and CLuaMain creation

## Security Considerations

1. **Script Validation**
   - Validate Lua version before execution
   - Prevent version mismatch exploits

2. **Bytecode Security**
   - Verify bytecode version matches VM
   - Reject incompatible bytecode

3. **Resource Isolation**
   - Ensure resources can't access each other's VMs inappropriately
   - Maintain separate memory spaces

## References

- [Lua 5.4 Reference Manual](https://www.lua.org/manual/5.4/)
- [Lua 5.4 vs 5.1 Changes](https://www.lua.org/manual/5.4/readme.html)
- [MTA:SA Lua Documentation](https://wiki.multitheftauto.com/wiki/Scripting_Introduction)

## Contributors

Implementation by MTA:SA development team with community input.

## License

This implementation follows MTA:SA's existing license for all modifications and additions.
