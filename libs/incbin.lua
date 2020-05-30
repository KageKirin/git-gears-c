-- genie scaffold for incbin

incbin_script = path.getabsolute(path.getdirectory(_SCRIPT))
incbin_root = path.join(incbin_script, "incbin")

incbin_includedirs = {
	incbin_root,
}

incbin_libdirs = {}
incbin_links = {}
incbin_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { incbin_includedirs }
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
	end,

	_refresh_project = function()
		os.execute("curl -L -o ".. path.join(incbin_root, "incbin.h") .. " https://raw.githubusercontent.com/graphitemaster/incbin/master/incbin.h")
	end,
}

---
