-- genie scaffold for libpcre2

libpcre2_script = path.getabsolute(path.getdirectory(_SCRIPT))

local function get_curl_path()
	if os.isdir(path.join(libpcre2_script, "pcre2")) then
		return path.join(libpcre2_script, "pcre2")
	elseif os.isdir("/usr/local/opt/pcre2") then
		-- macOS path when installed with brew
		return "/usr/local/opt/pcre2"
	end
end

libpcre2_root = get_curl_path()

libpcre2_includedirs = {
	path.join(libpcre2_root, "include")
}

libpcre2_libdirs = {
	path.join(libpcre2_root, "lib")
}

libpcre2_links = {}
libpcre2_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { libpcre2_includedirs }
	end,

	_add_defines = function()
		defines { libpcre2_defines }
	end,

	_add_libdirs = function()
		libdirs { libpcre2_libdirs }
	end,

	_add_external_links = function()
		links { libpcre2_links }
	end,

	_add_self_links = function()
		links { "pcre2" }
	end,

	_create_projects = function()
	end, -- _create_projects()
}

---
