-- genie scaffold for stb

stb_script = path.getabsolute(path.getdirectory(_SCRIPT))
stb_root = path.join(stb_script, "stb")

stb_includedirs = {
	stb_root,
}

stb_libdirs = {}
stb_links = {}
stb_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { stb_includedirs }
	end,

	_add_defines = function()
	end,

	_add_libdirs = function()
	end,

	_add_external_links = function()
	end,

	_add_self_links = function()
		links { "stb" }
	end,

	_create_projects = function()
		project "stb"
		kind "StaticLib"
		language "C"

		files {
			path.join(stb_root, "*.h"),
			path.join(stb_root, "*.c"),
		}
	end,
	
	_refresh_project = function()
		os.execute("curl -L -o ".. path.join(stb_root, "stb_printf.h") .. " https://github.com/nothings/stb/raw/master/stb_sprintf.h")
	end,
}

---
