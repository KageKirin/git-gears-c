--
-- PokeAss build configuration script
--
-------------------------------------------------------------------------------
--
-- Maintainer notes
--
-- - we're not using the regular _scaffolding_ for this project
--   mostly b/c it's too bloated
-- - scaffolds are still used though
--
-------------------------------------------------------------------------------
--
-- Option to use regular Lua 5.x instead of LuaJIT 2.1.x
-- default is to use LuaJIT.
--
	newoption {
		trigger = "use-lua5",
		description = "Disable usage of LuaJIT in favor of regular Lua 5.x.",
	}

--
-- Use the --to=path option to control where the project files get generated. I use
-- this to create project files for each supported toolset, each in their own folder,
-- in preparation for deployment.
--
	newoption {
		trigger = "to",
		value   = "path",
		description = "Set the output location for the generated files"
	}

-------------------------------------------------------------------------------
--
-- Pull in dependencies
--
	dofile("functions.lua") -- from scaffolding/system/functions.lua
	dofile("cppsettings.lua") -- from scaffolding/system/settings.lua

-------------------------------------------------------------------------------
--
-- Solution wide settings
--

local thisscriptpath = path.getabsolute(path.getdirectory(_SCRIPT))
local rootpath       = path.join(thisscriptpath, '..')
local locationpath = path.join(os.getcwd(), _OPTIONS["to"] or path.join('build/projects'))
local targetpath   = path.join(locationpath, '../bin')
local objectpath   = path.join(locationpath, '../obj')

	solution "pokeass"
		configurations {
			"Debug",
			"Release"
		}
		location (locationpath)

		configuration { "windows" }
			targetdir (path.join(targetpath, "windows"))
			objdir    (path.join(objectpath, "windows"))

		configuration { "linux" }
			targetdir (path.join(targetpath, "linux"))
			objdir    (path.join(objectpath, "linux"))

		configuration { "bsd" }
			targetdir (path.join(targetpath, "bsd"))
			objdir    (path.join(objectpath, "bsd"))

		configuration { "macosx" }
			targetdir (path.join(targetpath, "darwin"))
			objdir    (path.join(objectpath, "darwin"))

		configuration {}

-------------------------------------------------------------------------------
--
-- External 'scaffold' projects
--

local external_scaffolds = {
	--keep
	--this
	--line
	['libcurl'] = dofile(path.join(rootpath, "libs", "libcurl.lua")),
	--keep
	--this
	--line
	['libgit2'] = dofile(path.join(rootpath, "libs", "libgit2.lua")),
	--keep
	--this
	--line
	['jsmn'] = dofile(path.join(rootpath, "libs", "jsmn.lua")),
	--keep
	--this
	--line
}

create_packages_projects(external_scaffolds)

-------------------------------------------------------------------------------
--
-- Main project
--

core_projects = {
	["gitcgears"] = {
		_add_includedirs = function() end,
		_add_defines = function() end,
		_add_libdirs = function() end,
		_add_external_links = function() end,
		_add_self_links = function() end,
		_create_projects = function()
			startproject "gitcgears"
			group "gitcgears"
			project "gitcgears"
				targetname "git-cgears"
				language "C"
				kind "ConsoleApp"
				flags {
					"ExtraWarnings",
					"No64BitChecks",
					"StaticRuntime"
				}

				add_packages(external_scaffolds)

				files {
					"../src/**.h",
					"../src/**.c",
				}

				buildoptions {
					"-m64",
				}

				configuration { "Debug" }
					defines     { "_DEBUG", "LUA_COMPAT_MODULE" }
					flags       { "Symbols" }

				configuration { "Release" }
					defines     { "NDEBUG", "LUA_COMPAT_MODULE" }
					flags       { "OptimizeSize" }

				configuration { "vs*" }
					defines     { "_CRT_SECURE_NO_WARNINGS" }

				configuration { "windows" }
					targetdir    (path.join(rootpath, "bin/windows"))
					links { "ole32" }

				configuration { "linux" }
					targetdir    (path.join(rootpath, "bin/linux"))
					links       { "dl" }

				configuration { "bsd" }
					targetdir    (path.join(rootpath, "bin/bsd"))

				configuration { "linux or bsd" }
					defines      { "LUA_USE_POSIX", "LUA_USE_DLOPEN" }
					buildoptions { "-Wno-implicit-fallthrough" }
					links        { "m" }
					linkoptions  { "-rdynamic" }

				configuration { "macosx" }
					targetdir    (path.join(rootpath, "bin/darwin"))
					defines      { "LUA_USE_MACOSX" }
					links        { "CoreServices.framework" }
					linkoptions  { "-pagezero_size 10000", "-image_base 100000000" }

				configuration { "macosx", "gmake or ninja" }
					buildoptions { "-mmacosx-version-min=10.14" }
					linkoptions  { "-mmacosx-version-min=10.14" }

				configuration {}
		end, -- _create_projects()
	},
}

create_packages_projects(core_projects)

-------------------------------------------------------------------------------
--
-- A more thorough cleanup.
--

	if _ACTION == "clean" then
		os.rmdir("bin")
		os.rmdir("build")
	end

-------------------------------------------------------------------------------
--
-- Use the release action to prepare source and binary packages for a new release.
-- This action isn't complete yet; a release still requires some manual work.
--
	dofile("release.lua")

	newaction {
		trigger     = "release",
		description = "Prepare a new release (incomplete)",
		execute     = dorelease
	}

-------------------------------------------------------------------------------
