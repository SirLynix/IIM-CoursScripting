-- xmake.lua utilisant la SDL

-- Configuration des modes debug et release
add_rules("mode.debug", "mode.release")
-- Activation de l'auto-régénération de projet vsxmake à la modification
add_rules("plugin.vsxmake.autoupdate")

-- On enregistre les trois bibliothèques que nous allons utiliser
add_requires("libsdl", "libsdl_image", "libsdl_ttf")

-- On précise où notre exécutable final doit être compilé
set_targetdir("./bin")

-- On déclare notre projet
target("sdl_playground")
    set_kind("binary") -- Nous précisons faire un exécutable

    -- On enregistre les fichiers de notre application
    add_headerfiles("src/**.h", "src/**.hpp")
    add_files("src/**.cpp")

    -- On rajoute les bibliothèques que notre projet utilise
    add_packages("libsdl", "libsdl_image", "libsdl_ttf")

    -- On passe une option au compilateur pour forcer le point d'entrée de notre programme
    -- (problème lié à la SDL qui utilise un #define main pour faire son propre point d'entrée)
    if is_plat("windows") then
        add_ldflags("/subsystem:console")
    end
