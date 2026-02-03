project "Lua54_Server"
	language "C++"
	targetname "lua5.4"
	warnings "Off"

	vpaths {
		["Headers"] = "**.h",
		["Sources"] = "**.c",
		["*"] = "premake5.lua"
	}

	files {
		"premake5.lua",
		"src/**.c",
		"src/**.h",
	}

	-- Exclude lua.c and onelua.c (standalone interpreter files)
	removefiles {
		"src/lua.c",
		"src/onelua.c",
		"src/ltests.c"
	}

	defines { "LUA_BUILD_AS_DLL" }

	filter "system:windows"
		kind "SharedLib"
		targetdir(buildpath("server/mods/deathmatch"))

	filter "system:not windows"
		kind "StaticLib"

	filter {"system:windows", "platforms:x64"}
		targetdir(buildpath("server/x64"))

	filter {"system:windows", "platforms:arm"}
		targetdir(buildpath("server/arm"))

	filter {"system:windows", "platforms:arm64"}
		targetdir(buildpath("server/arm64"))


if os.target() == "windows" then
	project "Lua54_Client"
		language "C++"
		kind "SharedLib"
		targetname "lua5.4c"
		targetdir(buildpath("mods/deathmatch"))

		vpaths {
			["Headers"] = "**.h",
			["Sources"] = "**.c",
			["*"] = "premake5.lua"
		}

		files {
			"premake5.lua",
			"src/**.c",
			"src/**.h",
		}

		-- Exclude lua.c and onelua.c (standalone interpreter files)
		removefiles {
			"src/lua.c",
			"src/onelua.c",
			"src/ltests.c"
		}

		defines {
			"LUA_USE_APICHECK",
			"LUA_BUILD_AS_DLL"
		}

        filter "platforms:not x86"
            flags { "ExcludeFromBuild" }
end
