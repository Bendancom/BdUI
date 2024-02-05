add_rules("mode.debug","mode.release")

set_languages("c++23")


add_requires("vulkan-loader",{debug = is_mode("debug")})

target("BdUI")
    add_includedirs("include",{public = true})
    add_packages("vulkan-loader","vulkan-hpp",{public = true})
    add_files("modules/**.ixx","src/**.cpp")
target("test")
    set_kind("binary")
    set_default(false)
    add_deps("BdUI")
    add_files("test/**.cpp")
    add_tests("debug")