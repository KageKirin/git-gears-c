-- genie scaffold for curl

curl_script = path.getabsolute(path.getdirectory(_SCRIPT))

local function get_curl_path()
	if os.isdir(path.join(curl_script, "curl")) then
		return path.join(curl_script, "curl")
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

curl_root = get_curl_path()

curl_includedirs = {
	path.join(curl_root, "include")
}

curl_libdirs = {
	path.join(curl_root, "lib")
}

curl_links = {}
curl_defines = {}

----
return {
	_add_includedirs = function()
		includedirs { curl_includedirs }
	end,

	_add_defines = function()
		defines { curl_defines }
	end,

	_add_libdirs = function()
		libdirs { curl_libdirs }
	end,

	_add_external_links = function()
		links { curl_links }
	end,

	_add_self_links = function()
		links { "curl" }
	end,

	_create_projects = function()
	end, -- _create_projects()
}

---
