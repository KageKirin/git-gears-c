-- genie scaffold for jsmn

jsmn_script = path.getabsolute(path.getdirectory(_SCRIPT))
jsmn_root = path.join(jsmn_script, "jsmn")

jsmn_includedirs = {
	jsmn_root,
}

jsmn_libdirs = {}
jsmn_links = {}
jsmn_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { jsmn_includedirs }
	end,

	_add_defines = function()
	end,

	_add_libdirs = function()
	end,

	_add_external_links = function()
	end,

	_add_self_links = function()
	end,

	_create_projects = function()
		os.execute("curl -o ".. path.join(jsmn_root, "jsmn.h") .. " https://raw.githubusercontent.com/zserge/jsmn/master/jsmn.h")
	end, -- _create_projects()
}

---
