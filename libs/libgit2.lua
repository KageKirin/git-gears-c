-- genie scaffold for libgit2

libgit2_script = path.getabsolute(path.getdirectory(_SCRIPT))

local function get_libgit2_path()
	if os.isdir(path.join(libgit2_script, "libgit2")) then
		return path.join(libgit2_script, "libgit2")
	elseif os.isdir("/usr/local/opt/libgit2") then
		return "/usr/local/opt/libgit2" -- macOS path when installed with brew
	end
end

libgit2_root = get_libgit2_path()

libgit2_includedirs = {
	path.join(libgit2_root, "include")
}

libgit2_libdirs = {
	path.join(libgit2_root, "lib")
}

libgit2_links = {}
libgit2_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { libgit2_includedirs }
	end,

	_add_defines = function()
		defines { libgit2_defines }
	end,

	_add_libdirs = function()
		libdirs { libgit2_libdirs }
	end,

	_add_external_links = function()
		links { libgit2_links }
	end,

	_add_self_links = function()
		links { "git2" }
	end,

	_create_projects = function()
	end, -- _create_projects()
}

---
