workspace 'astra'
    architecture 'x86_64'
    objdir '%{wks.location}/.target/%{prj.name}/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'
    targetdir '%{wks.location}/.target/%{prj.name}/%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}'
    startproject 'astra'

    configurations {
        'Debug',
        'Release'
    }

    flags {
        'MultiProcessorCompile'
    }

group 'core'
    project 'astra'
        kind 'ConsoleApp'
        language 'C++'
        cppdialect 'C++17'
        staticruntime 'off'

        files {
            'include/**.h',
            'source/**cpp'
        }

        includedirs {
            'include',
            '%{wks.location}/vendor/imgui/',
            '%{wks.location}/vendor/wil/include',
            '%{wks.location}/vendor/webview2/build/native/include',
        }

        links {
            'opengl32.lib',

            '%{wks.location}/vendor/webview2/build/native/x64/WebView2Loader.lib',
            'imgui'
        }

        postbuildcommands {
            '{COPY} %{wks.location}/vendor/webview2/build/native/x64/WebView2Loader.dll %{cfg.targetdir}'
        }

        filter 'system:windows'
            systemversion 'latest'

        filter 'configurations:Debug'
            defines 'ASTRA_DEBUG'
            runtime 'Debug'
            symbols 'on'

        filter 'configurations:Release'
            defines 'ASTRA_RELEASE'
            runtime 'Release'
            optimize 'on'
group ''

group 'vendor'
    include 'vendor'
group ''