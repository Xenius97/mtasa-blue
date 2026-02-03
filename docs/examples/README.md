# MTA:SA Lua 5.4 Examples

This directory contains example resources demonstrating Lua 5.4 features in MTA:SA.

## lua54-test

A test resource that demonstrates:
- Const variables
- Floor division operator (`//`)
- Bitwise operators (`&`, `|`, `~`, `<<`, `>>`)
- `table.unpack()` (replacement for global `unpack()`)

### Usage

1. Copy the `lua54-test` folder to your server's `resources` directory
2. Add `<resource src="lua54-test" />` to your `mtaserver.conf`
3. Start your server and start the resource with: `start lua54-test`
4. Check the server console for test output

### Expected Output

```
=== Lua 5.4 Test Resource Started ===
Lua Version: Lua 5.4
Floor division: 10 // 2 = 5
Bitwise OR: 0x01 | 0x02 = 0x03
table.unpack: 1, 2, 3
=== All Lua 5.4 tests passed! ===
```

## Creating Your Own Lua 5.4 Resource

1. Create a new resource folder
2. Add `lua_version="5.4"` to the `<info>` tag in `meta.xml`:
   ```xml
   <meta>
       <info author="YourName" version="1.0" lua_version="5.4" />
       <script src="yourscript.lua" type="server" />
   </meta>
   ```
3. Use Lua 5.4 features in your scripts
4. Test thoroughly before deploying to production

## Notes

- Resources without `lua_version` attribute default to Lua 5.1
- All MTA functions work the same in both Lua versions
- See `docs/LUA54_SUPPORT.md` for full documentation
