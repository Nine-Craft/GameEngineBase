project "Sandbox"
    kind "ConsoleApp"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir("%{wks.location}/bin/"..outputdir.. "/%{prj.name}")
    objdir("%{wks.location}/bin-int/"..outputdir.."/%{prj.name}")

    files
    {
        "src/**.h",
        "src/**.cpp",
    }

    includedirs
    {
        "src",
        "%{wks.location}/Engine/src",

        "%{IncludeDir.spdlog}",
        "%{IncludeDir.SDL}",
        "%{IncludeDir.rttr}",
        --"%{IncludeDir.mono}",
        --"%{IncludeDir.gl3w}",
        "%{IncludeDir.glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.rapidjson}",
        
        "%{IncludeDir.glm}",
        "%{IncludeDir.oom}", --custom math library.

        "%{IncludeDir.VulkanSDK}", -- Temporary until the rendering can standalone

    }

    -- library diretories
    libdirs 
    {        
        "%{LibraryDir.SDL}",
        "%{LibraryDir.rttr}",
        "%{LibraryDir.mono}",
        "%{LibraryDir.oom}", --custom math library.
    }

    links
    {
        "opengl32",
        "SDL2",
        "SDL2main",
        "SDL2test",
        "Engine",
        "mono-2.0-sgen",
        "MonoPosixHelper",
        "oom" --custom math library.
    }

    defines
    {
        "_CRT_SECURE_NO_WARNINGS"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "off"
        systemversion "latest"

        defines
        {
            "ENGINE_PLATFORM_WINDOWS",
            "SANDBOX_PLATFORM_WINDOWS",
        }
        
        --enable this post build command for 64 bit system
        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/sdl2/lib/x64/SDL2.dll ../bin/" .. outputdir .. "/Sandbox"},
            {"{COPY} %{wks.location}Engine/vendor/mono/lib/ ../bin/" .. outputdir .. "/lib"},
            {"{COPY} %{wks.location}Engine/vendor/mono/dll/mono-2.0-sgen.dll ../bin/" .. outputdir .. "/Sandbox"},
            {"{COPY} %{wks.location}Sandbox/test.txt ../bin/" .. outputdir .. "/Sandbox"},
            {"{COPY} %{wks.location}Scripting/ ../bin/" .. outputdir .. "/Scripting"},
            {"{COPY} %{wks.location}Engine/vendor/mono/lib/mono/4.5 ../bin/" .. outputdir .. "/Engine/vendor/mono/lib/mono/4.5"}, -- mono c# libraries
            {"{COPY} %{wks.location}Engine/assets ../bin/" .. outputdir .. "/Engine/assets"}, --  engine's assets
            {"{COPY} %{wks.location}Sandbox/icons ../bin/" .. outputdir .. "/Sandbox/icons"}, -- sandbox icons
            {"{COPY} %{wks.location}Sandbox/imgui.ini ../bin/" .. outputdir .. "/Sandbox"} -- imgui.ini copy 
        }

    filter "configurations:Debug"
        defines "ENGINE_DEBUG"
        symbols "On"

        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/rttr/dll/rttr_core_d.dll ../bin/" .. outputdir .. "/Sandbox"},
        }

        links
        {
            "rttr_core_d"
        }
        
    filter "configurations:Release"
        defines "ENGINE_RELEASE"
        optimize "On"

        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/rttr/dll/rttr_core.dll ../bin/" .. outputdir .. "/Sandbox"}
        }

        links
        {
            "rttr_core"
        }
        
    filter "configurations:Production"
        defines "ENGINE_PRODUCTION"
        optimize "On"
        
        architecture "x86_64"
        postbuildcommands
        {
            {"{COPY} %{wks.location}Engine/vendor/rttr/dll/rttr_core.dll ../bin/" .. outputdir .. "/Sandbox"}
        }

        links
        {
            "rttr_core"
        }
        