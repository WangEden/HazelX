-- premake5.lua

workspace "HazelX"
    architecture "x64"
    startproject "Sandbox"

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

include "HazelX/vendor/GLFW"
include "HazelX/vendor/Glad"

project "HazelX"
    location "HazelX"
    kind "SharedLib" -- 动态链接库
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    pchheader "hzpch.h"
    pchsource "HazelX/src/hzpch.cpp"

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "%{prj.name}/vendor/spdlog/include",
        "%{prj.name}/src",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.Glad}"
    }

    links
    {
        "GLFW",
        "Glad",
        "opengl32.lib"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On" -- 静态链接运行时库
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS",
            "HZ_BUILD_DLL",
            "GLFW_INCLUDE_NONE",
            "_WINDLL"
        }

        postbuildcommands
        {
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }
    
    filter "configurations:Debug"
        defines "HZ_DEBUG"
        buildoptions "/MDd"
        symbols "On" -- 开启调试符号

    filter "configurations:Release"
        defines "HZ_RELEASE"
        buildoptions "/MD"
        optimize "On" -- 开启优化

    filter "configurations:Dist"
        defines "HZ_DIST"
        buildoptions "/MD"
        optimize "On" -- 开启优化

project "Sandbox"
    location "Sandbox"
    kind "ConsoleApp" -- 控制台应用程序
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }

    includedirs
    {
        "HazelX/vendor/spdlog/include",
        "HazelX/src"
    }

    links
    {
        "HazelX"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On" -- 静态链接运行时库
        systemversion "latest"

        defines
        {
            "HZ_PLATFORM_WINDOWS"
        }

    filter "configurations:Debug"
        defines "HZ_DEBUG"
        buildoptions "/MDd"
        symbols "On" -- 开启调试符号

    filter "configurations:Release"
        defines "HZ_RELEASE"    
        buildoptions "/MD"
        optimize "On" -- 开启优化

    filter "configurations:Dist"
        defines "HZ_DIST"
        buildoptions "/MD"
        optimize "On" -- 开启优化

    
