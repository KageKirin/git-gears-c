-- genie scaffold for jsmn

jsmn_script = path.getabsolute(path.getdirectory(_SCRIPT))
jsmn_root = path.join(jsmn_script, "jsmn")

jsmn_includedirs = {
	jsmn_root,
}

jsmn_libdirs = {}
jsmn_links = {}
jsmn_defines = { "JSMN_HEADER" }

----
return {
	_add_includedirs = function()
		includedirs { jsmn_includedirs }
	end,

	_add_defines = function()
		defines { jsmn_defines }
	end,

	_add_libdirs = function()
	end,

	_add_external_links = function()
	end,

	_add_self_links = function()
		links { "jsmn" }
	end,

	_create_projects = function()
		project "jsmn"
		kind "StaticLib"
		language "C"

		files {
			path.join(jsmn_root, "jsmn.h"),
			path.join(jsmn_root, "jsmn.c"),
		}
	end,

	_refresh_project = function()
		os.execute("curl -L -o ".. path.join(jsmn_root, "jsmn.h") .. " https://raw.githubusercontent.com/zserge/jsmn/master/jsmn.h")
	end,
}

---
