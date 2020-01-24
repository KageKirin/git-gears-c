-- genie scaffold for pcre2

pcre2_script = path.getabsolute(path.getdirectory(_SCRIPT))

local function get_curl_path()
	if os.isdir(path.join(pcre2_script, "pcre2")) then
		return path.join(pcre2_script, "pcre2")
	elseif os.isdir("/usr/local/opt/pcre2") then
		-- macOS path when installed with brew
		return "/usr/local/opt/pcre2"
	end
end

pcre2_root = get_curl_path()

pcre2_includedirs = {
	path.join(pcre2_root, "include")
}

pcre2_libdirs = {
	path.join(pcre2_root, "lib")
}

pcre2_links = {}
pcre2_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { pcre2_includedirs }
	end,

	_add_defines = function()
		defines { pcre2_defines }
	end,

	_add_libdirs = function()
		libdirs { pcre2_libdirs }
	end,

	_add_external_links = function()
		links { pcre2_links }
	end,

	_add_self_links = function()
		links { "pcre2" }
	end,

	_create_projects = function()
	end, -- _create_projects()
}

---
