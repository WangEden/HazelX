-- premake5.lua

workspace "Ripple"
    architecture "x64"
    startproject "Editor"

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
    IncludeDir["GLFW"] = "Engine/vendor/GLFW/include"
    IncludeDir["Glad"] = "Engine/vendor/Glad/include"
    IncludeDir["ImGui"] = "Engine/vendor/imgui"
    IncludeDir["GLM"] = "Engine/vendor/glm"
    IncludeDir["entt"] = "Engine/Vendor/entt/include"
    IncludeDir["FastNoise"] = "Engine/vendor/FastNoise"
    IncludeDir["mono"] = "Engine/vendor/mono/include"

    LibraryDir = {}
    LibraryDir["mono"] = "vendor/mono/lib/Debug/mono-2.0-sgen.lib"

group "Denpendencies"
    include "Engine/vendor/GLFW" -- 链接到GLFW子模块中的premake5.lua
    include "Engine/vendor/Glad"
    include "Engine/vendor/imgui"
group ""

group "Core"
project "Engine"
    location "Engine"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    filter "system:windows"
        pchheader "hzpch.h"
        pchsource "Engine/src/hzpch.cpp"

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

    filter "files:Engine/vendor/yaml-cpp/src/**.cpp"
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
            "RP_PLATFORM_WINDOWS",
            -- "RP_BUILD_DLL",
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
            "RP_PLATFORM_MACOS",
            "GLFW_INCLUDE_NONE"
        }

    filter "configurations:Debug"
        defines "RP_DEBUG"
        runtime "Debug"
        symbols "on"

    filter "configurations:Release"
        defines "RP_RELEASE"
        runtime "Release"
        optimize "on"

    filter "configurations:Dist"
        defines "RP_DIST"
        runtime "Release"
        optimize "on"
    
project "ScriptCore"
	location "ScriptCore"
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
project "Editor"
    location "Editor"
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
        "Engine/vendor/spdlog/include",
        "%{IncludeDir.GLM}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.entt}",
        "Engine/vendor"
    }

    includedirs
    {
        "%{prj.name}/src",
        "Engine/src"
    }

    links
    {
        "Engine"
    }

    filter { "system:windows", "configurations:Debug" }
        systemversion "latest"
        runtime "Debug"
        symbols "on"

        defines
        {
            "RP_DEBUG",
            "RP_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)Engine/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)Engine/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\""),

            ("{COPY} \"$(SolutionDir)Engine/vendor/mono/bin/Debug/mono-2.0-sgen.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)Engine/vendor/mono/bin/Debug/mono-2.0-sgen.pdb\" \"%{cfg.targetdir}\""),
        }

        links
        {
			"Engine/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
        }

    filter { "system:windows", "configurations:Release" }
        systemversion "latest"
        runtime "Release"
        optimize "on"

        defines 
        {
            "RP_RELEASE",
            "RP_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)Engine/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)Engine/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\""),

            ("{COPY} \"$(SolutionDir)Engine/vendor/mono/bin/Release/mono-2.0-sgen.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)Engine/vendor/mono/bin/Release/mono-2.0-sgen.pdb\" \"%{cfg.targetdir}\""),
        }
        
        links
		{
			"Engine/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
		}

    filter { "system:windows", "configurations:Dist" }
        systemversion "latest"
        runtime "Release"
        optimize "on"

        defines 
        {
            "RP_DIST",
            "RP_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)Engine/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)Engine/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\""),

            ("{COPY} \"$(SolutionDir)Engine/vendor/mono/bin/Release/mono-2.0-sgen.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)Engine/vendor/mono/bin/Release/mono-2.0-sgen.pdb\" \"%{cfg.targetdir}\""),
        }

        links
		{
			"Engine/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
		}

    filter "system:macosx"
        defines { "RP_PLATFORM_MACOS" }
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

project "ScriptCore"
	location "ScriptCore"
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

	targetdir ("Editor/assets/scripts")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"%{prj.name}/src/**.cs", 
	}

	links
	{
		"ScriptCore"
	}
