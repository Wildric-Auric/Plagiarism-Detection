workspace "Plagiarism"
	configurations {"Release", "Debug"}
	architecture "x86"
	language "C++"
    cppdialect "C++11"
	targetdir "Bin/%{prj.name}/%{cfg.buildcfg}"
    objdir    "Bin/objs"
    characterset("MBCS")
    buildoptions { "/EHsc" }
	location "./"
	project "Plagiarism"
	kind "ConsoleApp"
	includedirs {

	}
	files {
		"*.c",
		"*.lua"
	}

	filter "configurations:Release"
		optimize "On"
	filter "configurations:Debug" 
		symbols "On"
