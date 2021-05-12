add_requires("entt", "libsdl_image", "libsdl_ttf")
add_requires("libsdl", { configs = { use_sdlmain = false } })

-- Configuration des modes debug et release
add_rules("mode.debug", "mode.release")
-- Activation de l'auto-régénération de projet vsxmake à la modification
add_rules("plugin.vsxmake.autoupdate")

set_languages("c++17")

set_targetdir("./bin")

target("sdlcpp")
    set_kind("static")
    add_headerfiles("src/sdlcpp/**.hpp")
    add_files("src/sdlcpp/**.cpp")
    add_packages("libsdl", "libsdl_image", "libsdl_ttf", { public = true })

target("Exemple1")
    set_kind("binary")
    add_files("src/exemple1.cpp")
    add_packages("entt")

target("Exemple2")
    set_kind("binary")
    add_files("src/exemple2.cpp")
    add_packages("entt")
    add_deps("sdlcpp")

target("Exemple3")
    set_kind("binary")
    add_files("src/exemple3.cpp")
    add_packages("entt")
    add_deps("sdlcpp")
