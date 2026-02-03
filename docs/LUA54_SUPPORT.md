# Lua 5.4 Support in MTA:SA

MTA:SA now supports both Lua 5.1 and Lua 5.4 side-by-side, allowing resources to choose which version to use.

## How to Use Lua 5.4

### Resource-Level Version Selection

Add the `lua_version` attribute to the `<info>` tag in your resource's `meta.xml`:

```xml
<meta>
    <info author="YourName" version="1.0" lua_version="5.4" />
    
    <script src="server.lua" type="server" />
    <script src="client.lua" type="client" />
</meta>
```

**Valid values:**
- `"5.1"` or `"51"` - Use Lua 5.1 (default for backward compatibility)
- `"5.4"` or `"54"` - Use Lua 5.4

### Default Behavior

If no `lua_version` is specified, resources will default to Lua 5.1 to ensure backward compatibility.

## Key Differences Between Lua 5.1 and 5.4

### New Features in Lua 5.4

1. **Const variables** - `local x <const> = 10`
2. **To-be-closed variables** - `local f <close> = io.open("file.txt")`  
3. **New operators**:
   - Floor division: `//`
   - Bitwise operators: `&`, `|`, `~`, `<<`, `>>`
4. **Integer subtype** - Better integer handling
5. **Improved garbage collector**

### Migration from Lua 5.1

- Replace `unpack()` with `table.unpack()`
- Avoid `module()` function (use regular tables)
- Use `_ENV` instead of `setfenv`/`getfenv`

## References

- [Lua 5.4 Manual](https://www.lua.org/manual/5.4/)
- [What's new in Lua 5.4](https://www.lua.org/manual/5.4/readme.html#changes)
