-- Example Lua 5.4 client script

addEventHandler("onClientResourceStart", resourceRoot, function()
    outputChatBox("=== Lua 5.4 Test (Client) ===", 0, 255, 0)
    outputChatBox("Lua Version: " .. _VERSION, 255, 255, 255)
    
    -- Test floor division
    local result = 15 // 4
    outputChatBox("Floor division: 15 // 4 = " .. result, 255, 255, 255)
    
    -- Test bitwise operations  
    local mask = 0xFF & 0x0F
    outputChatBox(string.format("Bitwise AND: 0xFF & 0x0F = 0x%02X", mask), 255, 255, 255)
    
    outputChatBox("=== Tests completed! ===", 0, 255, 0)
end)
