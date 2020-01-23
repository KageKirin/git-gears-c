--- language-specific compiler settings

--- C++ options

-- C++03 settings
local cpp03_buildoptions = {
	clang = {
		"-std=c++03",
	},
	vs = {
		"/std:c++03"
	}
}

local cpp03_linkoptions = {
	clang = {
		"-stdlib=libc++",
	},
	vs = {

	}
}

-- C++11 settings
local cpp11_buildoptions = {
	clang = {
		"-std=c++11",
	},
	vs = {
		"/std:c++11"
	}
}

local cpp11_linkoptions = {
	clang = {
		"-stdlib=libc++",
	},
	vs = {

	}
}

-- C++14 settings
local cpp14_buildoptions = {
	clang = {
		"-std=c++14",
	},
	vs = {
		"/std:c++14"
	}
}

local cpp14_linkoptions = {
	clang = {
		"-stdlib=libc++",
	},
	vs = {

	}
}

-- C++17 settings
local cpp17_buildoptions = {
	clang = {
		"-std=c++1z",
	},
	vs = {
		"/std:c++latest"
	}
}

local cpp17_linkoptions = {
	clang = {
		"-stdlib=libc++",
	},
	vs = {

	}
}

-- forward-compatible C++XY settings
-- i.e. aliasing for single place to change
local cpp_fwd_buildoptions = cpp17_buildoptions
local cpp_fwd_linkoptions = cpp17_linkoptions

--- C options (K&R C language)

-- C89 settings
local c89_buildoptions = {
	clang = {
		"-std=c89",
	},
	vs = {

	}
}

local c89_linkoptions = {
	clang = {},
	vs = {},
}

-- C99 settings
local c99_buildoptions = {
	clang = {
		"-std=c99",
	},
	vs = {

	}
}

local c99_linkoptions = {
	clang = {},
	vs = {},
}

-- C11 settings
local c11_buildoptions = {
	clang = {
		"-std=c11",
	},
	vs = {

	}
}

local c11_linkoptions = {
	clang = {},
	vs = {},
}

---

local force_buildoptions = function(_project, _buildoptions, _linkoptions)
	project (_project)

	configuration {}

	configuration { "linux-steamlink" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-gcc* or linux-clang*" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "vs*-clang" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "asmjs" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "mingw-*" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-gcc" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-mips-gcc" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-arm-gcc" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "android-*" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "osx" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "macosx" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "ios*" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "orbis" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "qnx-arm" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "rpi" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "riscv" }
		buildoptions { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }


	configuration { "vs*", "not vs*-clang" }
		buildoptions_cpp { _buildoptions.vs, }
		linkoptions { _linkoptions.vs, }

	configuration { "vs*", "vs*-clang" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "vs*-clang" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration {}
end

---

force_cppbuildoptions = function(_project, _buildoptions, _linkoptions)
	if not _project then
		local prj = project()
		assert(prj)
		_project= prj.name
	end
	project (_project)

	configuration {}

	configuration { "linux-steamlink" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-gcc* or linux-clang*" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "asmjs" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "mingw-*" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-gcc" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-mips-gcc" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "linux-arm-gcc" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "android-*" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "osx" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "macosx" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "ios*" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "orbis" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "qnx-arm" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "rpi" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "riscv" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }


	configuration { "vs*", "not vs*-clang" }
		buildoptions_cpp { _buildoptions.vs, }
		linkoptions { _linkoptions.vs, }

	configuration { "vs*", "vs*-clang" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration { "vs*-clang" }
		buildoptions_cpp { _buildoptions.clang, }
		linkoptions { _linkoptions.clang, }

	configuration {}
end

---

build_cpp03 = function(_project)
	force_cppbuildoptions(_project, cpp03_buildoptions, cpp03_linkoptions)
end

build_cpp11 = function(_project)
	force_cppbuildoptions(_project, cpp11_buildoptions, cpp11_linkoptions)
end

build_cpp14 = function(_project)
	force_cppbuildoptions(_project, cpp14_buildoptions, cpp14_linkoptions)
end

build_cpp17 = function(_project)
	force_cppbuildoptions(_project, cpp17_buildoptions, cpp17_linkoptions)
end


build_cppfwd = function(_project)
	force_cppbuildoptions(_project, cpp_fwd_buildoptions, cpp_fwd_linkoptions)
end

---


---

build_c89 = function(_project)
	force_buildoptions(_project, c89_buildoptions, c89_linkoptions)
end

---

build_c99 = function(_project)
	force_buildoptions(_project, c99_buildoptions, c99_linkoptions)
end

---

build_c11 = function(_project)
	force_buildoptions(_project, c11_buildoptions, c11_linkoptions)
end

---

