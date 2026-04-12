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

    include "HazelX/vendor/GLFW" -- 链接到GLFW子模块中的premake5.lua
    include "HazelX/vendor/Glad"
    include "HazelX/vendor/imgui"

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

    -- filter "system:macosx"
    -- filter "" -- 重置过滤

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.c", 
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp", 
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/stb",
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    externalincludedirs
    {
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.GLM}",
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/vendor/assimp/include",
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

    filter "system:windows"
        systemversion "latest"
        links { "opengl32.lib" }

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "_WINDLL"
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
        "HazelX/vendor"
    }

    includedirs
    {
        "HazelX/src"
    }

    links
    {
        "HazelX"
    }

    filter "system:windows"
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS"
        }

        postbuildcommands 
        {
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.dll\" \"%{cfg.targetdir}\""),
            ("{COPY} \"$(SolutionDir)HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.pdb\" \"%{cfg.targetdir}\"")
        }

    filter "system:macosx"
        defines { "HZ_PLATFORM_MACOS" }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        runtime "Debug"
        symbols "on"

        links
        {
			"HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
        }

    filter "configurations:Release"
        defines "HZ_RELEASE"    
        runtime "Release"
        optimize "on"

        links
		{
			"HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
		}

    filter "configurations:Dist"
        defines "HZ_DIST"
        runtime "Release"
        optimize "on"

        links
		{
			"HazelX/vendor/assimp/lib/x64/assimp-vc143-mt.lib"
		}
