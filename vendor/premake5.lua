project 'imgui'
	kind 'StaticLib'
	language 'C++'
	location '%{wks.location}/vendor/imgui/'

	files
	{
		'%{wks.location}/vendor/imgui/imconfig.h',
		'%{wks.location}/vendor/imgui/imgui.h',
		'%{wks.location}/vendor/imgui/imgui.cpp',
		'%{wks.location}/vendor/imgui/imgui_draw.cpp',
		'%{wks.location}/vendor/imgui/imgui_internal.h',
		'%{wks.location}/vendor/imgui/imgui_tables.cpp',
		'%{wks.location}/vendor/imgui/imgui_widgets.cpp',
		'%{wks.location}/vendor/imgui/imstb_rectpack.h',
		'%{wks.location}/vendor/imgui/imstb_textedit.h',
		'%{wks.location}/vendor/imgui/imstb_truetype.h',
		'%{wks.location}/vendor/imgui/imgui_demo.cpp',

		'%{wks.location}/vendor/imgui/backends/imgui_impl_win32.h',
		'%{wks.location}/vendor/imgui/backends/imgui_impl_win32.cpp',
		'%{wks.location}/vendor/imgui/backends/imgui_impl_opengl3.h',
		'%{wks.location}/vendor/imgui/backends/imgui_impl_opengl3_loader.h',
		'%{wks.location}/vendor/imgui/backends/imgui_impl_opengl3.cpp',
	}

	includedirs {
		'%{wks.location}/vendor/imgui/'
	}

	filter 'system:windows'
		systemversion 'latest'
		cppdialect 'C++17'
		staticruntime 'On'

	filter 'system:linux'
		pic 'On'
		systemversion 'latest'
		cppdialect 'C++17'
		staticruntime 'On'

	filter 'configurations:Debug'
		runtime 'Debug'
		symbols 'on'

	filter 'configurations:Release'
		runtime 'Release'
		optimize 'on'