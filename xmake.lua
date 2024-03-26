add_rules("mode.release","mode.debug")
set_languages("cxx23")

set_config("kind","shared")

target("bdui")
	set_kind("$(kind)")
	add_files("src/**.ixx",{install = true})
	add_files("src/**.cpp")
target("test")
 	set_kind("binary")
	add_deps("bdui")
	add_files("test/test.cpp")