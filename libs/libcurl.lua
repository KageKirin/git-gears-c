-- genie scaffold for libcurl

libcurl_script = path.getabsolute(path.getdirectory(_SCRIPT))
libcurl_root = "/usr/local/opt/curl" -- macOS path when installed with brew

libcurl_includedirs = {
	path.join(libcurl_root, "include")
}

libcurl_libdirs = {
	path.join(libcurl_root, "lib")
}

libcurl_links = {}
libcurl_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { libcurl_includedirs }
	end,

	_add_defines = function()
		defines { libcurl_defines }
	end,

	_add_libdirs = function()
		libdirs { libcurl_libdirs }
	end,

	_add_external_links = function()
		links { libcurl_links }
	end,

	_add_self_links = function()
		links { "curl" }
	end,

	_create_projects = function()
	end, -- _create_projects()
}

---
