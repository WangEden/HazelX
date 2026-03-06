project "ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-int/" .. outputdir .. "/%{prj.name}")

    files
    {
        "imconfig.h",
        "imgui.h",
        "imgui.cpp",
        "imgui_draw.cpp",
        "imgui_internal.h",
        "imgui_widgets.cpp",
        "imgui_tables.cpp",
        "imstb_rectpack.h",
        "imstb_textedit.h",
        "imstb_truetype.h",
        "imgui_demo.cpp",
        -- "imgui_impl_glfw.cpp",
        -- "imgui_impl_glfw.h",
        -- "imgui_impl_opengl3.cpp",
        -- "imgui_impl_opengl3.h",
    }

    defines
    {
        "IMGUI_API=__declspec(dllexport)"
    }

    -- IncludeDir = {}
    -- IncludeDir["GLFW"] = "HazelX/vendor/GLFW/include"
    -- include "HazelX/vendor/GLFW"

    -- includedirs
    -- {
    --     "%{IncludeDir.GLFW}",
    -- }

    filter "system:windows"
        systemversion "latest"
        cppdialect "C++17"

    filter { "system:windows", "configurations:Release" }
        buildoptions "/MT"