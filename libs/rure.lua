-- genie scaffold for rure Rust-Regex C-API

rure_script = path.getabsolute(path.getdirectory(_SCRIPT))

local function get_rure_path()
	if os.isdir(path.join(rure_script, "rure")) then
		return path.join(rure_script, "rure")
	elseif os.isdir(path.join(rure_script, "regex", "regex-capi")) then
		return path.join(rure_script, "regex", "regex-capi")
	end
end

rure_root = get_rure_path()

rure_includedirs = {
	path.join(rure_root, "include")
}

rure_libdirs = {
	path.join(rure_root, "..", "target", "debug")
}

rure_links = {}
rure_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { rure_includedirs }
	end,

	_add_defines = function()
		defines { rure_defines }
	end,

	_add_libdirs = function()
		libdirs { rure_libdirs }
	end,

	_add_external_links = function()
		links { rure_links }
	end,

	_add_self_links = function()
		links { "rure" }
	end,

	_create_projects = function()
	end, -- _create_projects()
}

---
