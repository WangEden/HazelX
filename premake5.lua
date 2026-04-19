-- premake5.lua

workspace "HazelX"
    architecture "x64"
    startproject "Hazelnut"

    configurations
    {
        "Debug", -- 调试版本，包含日志和调试信息
        "Release", -- 发布版本，包含日志
        "Dist" -- Distribution 完全没有日志的发布版本
    }

    filter "system:windows"
        buildoptions { "/utf-8" }

    outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

    IncludeDir = {}
    IncludeDir["GLFW"] = "HazelX/vendor/GLFW/include"
    IncludeDir["Glad"] = "HazelX/vendor/Glad/include"
    IncludeDir["ImGui"] = "HazelX/vendor/imgui"
    IncludeDir["GLM"] = "HazelX/vendor/glm"
    IncludeDir["entt"] = "HazelX/Vendor/entt/include"
    IncludeDir["FastNoise"] = "HazelX/vendor/FastNoise"
    IncludeDir["mono"] = "HazelX/vendor/mono/include"

    LibraryDir = {}
    LibraryDir["mono"] = "vendor/mono/lib/Debug/mono-2.0-sgen.lib"

group "Denpendencies"
    include "HazelX/vendor/GLFW" -- 链接到GLFW子模块中的premake5.lua
    include "HazelX/vendor/Glad"
    include "HazelX/vendor/imgui"
group ""

group "Core"
project "HazelX"
    location "HazelX"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        pchheader "hzpch.h"
        pchsource "HazelX/src/hzpch.cpp"


    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c", 
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp", 
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb",

        "%{prj.name}/vendor/yaml-cpp/include/**.h",
        "%{prj.name}/vendor/yaml-cpp/src/**.cpp",
        "%{prj.name}/vendor/yaml-cpp/src/**.h",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    externalincludedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/assimp/include",
        "%{prj.name}/vendor/yaml-cpp/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.entt}",
		"%{IncludeDir.mono}",
		"%{IncludeDir.FastNoise}",
    }

    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/stb/include",
    }

    links
    {
        "GLFW",
        "Glad",
        "ImGui",
    }

    filter "files:HazelX/vendor/yaml-cpp/src/**.cpp"
        buildoptions { "/Y-" }

    filter "system:windows"
        systemversion "latest"
        links 
        { 
            "opengl32.lib",
            "%{LibraryDir.mono}",
        }

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            -- "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "YAML_CPP_STATIC_DEFINE",
        }

    filter "system:macosx"
        staticruntime "off" -- Mac 上通常不强制静态链接系统库
        links
        {
            "OpenGL.framework",
            "Cocoa.framework",
            "IOKit.framework",
            "CoreVideo.framework",
            "QuartzCore.framework"
        }
        defines
        {
            "HZ_PLATFORM_MACOS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "HZ_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "on"
    
project "Hazel-ScriptCore"
	location "Hazel-ScriptCore"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}
group ""

group "Tools"
project "Hazelnut"
    location "Hazelnut"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    externalincludedirs
    {
        "HazelX/vendor/spdlog/include",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.entt}",
        "HazelX/vendor"
    }

    includedirs
    {
        "%{prj.name}/src",
        "HazelX/src"
    }

    links
    {
        "HazelX"
    }

    filter { "system:windows", "configurations:Debug" }
        systemversion "latest"
        runtime "Debug"
        symbols "on"

        defines
        {
            "HZ_DEBUG",
            "HZ_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\""),

            ("{COPY} \"$(SolutionDir)HazelX/vendor/mono/bin/Debug/mono-2.0-sgen.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/mono/bin/Debug/mono-2.0-sgen.pdb\" \"%{cfg.targetdir}\""),
        }

        links
        {
			"HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
        }

    filter { "system:windows", "configurations:Release" }
        systemversion "latest"
        runtime "Release"
        optimize "on"

        defines 
        {
            "HZ_RELEASE",
            "HZ_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\""),

            ("{COPY} \"$(SolutionDir)HazelX/vendor/mono/bin/Release/mono-2.0-sgen.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/mono/bin/Release/mono-2.0-sgen.pdb\" \"%{cfg.targetdir}\""),
        }
        
        links
		{
			"HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
		}

    filter { "system:windows", "configurations:Dist" }
        systemversion "latest"
        runtime "Release"
        optimize "on"

        defines 
        {
            "HZ_DIST",
            "HZ_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\""),

            ("{COPY} \"$(SolutionDir)HazelX/vendor/mono/bin/Release/mono-2.0-sgen.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/mono/bin/Release/mono-2.0-sgen.pdb\" \"%{cfg.targetdir}\""),
        }

        links
		{
			"HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
		}

    filter "system:macosx"
        defines { "HZ_PLATFORM_MACOS" }
group ""

workspace "Sandbox"
    architecture "x64"
    targetdir "build"

    configurations
    {
        "Debug",
        "Release",
        "Dist"
    }

project "Hazel-ScriptCore"
	location "Hazel-ScriptCore"
	kind "SharedLib"
	language "C#"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}

project "ExampleApp"
	location "ExampleApp"
	kind "SharedLib"
	language "C#"

	targetdir ("Hazelnut/assets/scripts")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}

	links
	{
		"Hazel-ScriptCore"
	}
