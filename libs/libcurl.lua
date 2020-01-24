-- genie scaffold for libcurl

libcurl_script = path.getabsolute(path.getdirectory(_SCRIPT))

local function get_curl_path()
	if os.isdir(path.join(libcurl_script, "curl")) then
		return path.join(libcurl_script, "curl")
	elseif os.isdir("/usr/local/opt/curl") then
		-- macOS path when installed with brew
		return "/usr/local/opt/curl"
	elseif os.isdir("/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr") then
		--macOS command line tools
		return "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/usr"
	elseif os.isdir("/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr") then
		--macOS XCode
		return "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/usr"
	end
end

libcurl_root = get_curl_path()

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
