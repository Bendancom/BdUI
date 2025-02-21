add_rules("mode.debug","mode.release")

set_languages("c++latest")

add_requires("vulkan-loader",{debug = is_mode("debug")})

target("BdUI")
	set_kind("shared")
    add_packages("vulkan-loader","vulkan-hpp")
    add_files("module/**.cppm",{public = true})
	add_files("src/**.cpp")
target("test")
    set_kind("binary")
    set_default(false)
    add_deps("BdUI")
    add_files("test/**.cpp")
    add_tests("debug")
