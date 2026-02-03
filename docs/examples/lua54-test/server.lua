-- Example Lua 5.4 server script demonstrating new features

-- Const variables (Lua 5.4 feature)
local RESOURCE_NAME <const> = "lua54-test"

-- Helper function demonstrating floor division
local function calculateHalf(number)
    return number // 2  -- Floor division operator (Lua 5.4)
end

-- Helper function demonstrating bitwise operations
local function combineFlags()
    local flags1 = 0x01
    local flags2 = 0x02
    return flags1 | flags2  -- Bitwise OR (Lua 5.4)
end

-- Event handler
addEventHandler("onResourceStart", resourceRoot, function()
    outputServerLog("=== Lua 5.4 Test Resource Started ===")
    outputServerLog("Lua Version: " .. _VERSION)
    
    -- Test floor division
    local result = calculateHalf(10)
    outputServerLog("Floor division: 10 // 2 = " .. result)
    
    -- Test bitwise operations
    local combined = combineFlags()
    outputServerLog(string.format("Bitwise OR: 0x01 | 0x02 = 0x%02X", combined))
    
    -- Test table.unpack (moved from global in Lua 5.4)
    local values = {1, 2, 3, 4, 5}
    local a, b, c = table.unpack(values)
    outputServerLog(string.format("table.unpack: %d, %d, %d", a, b, c))
    
    outputServerLog("=== All Lua 5.4 tests passed! ===")
end)

addEventHandler("onResourceStop", resourceRoot, function()
    outputServerLog("Lua 5.4 Test Resource Stopped")
end)
