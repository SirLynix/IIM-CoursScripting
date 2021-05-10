La compilation en C++ est un processus en plusieurs étapes, dont trois sont majoritairement retenus :

# La compilation

## Le préprocesseur

La première étape consiste à s'occuper des lignes commençant par un #, appelées "directives de préprocesseur".
La plus connue et utile d'entre-elles est #include, son rôle est de copier-coller le contenu d'un fichier (header) dans le .cpp.

En ligne de commande, on peut utiliser -E avec Clang et GCC pour obtenir la sortie du préprocesseur (mais cela n'a que peu d'intérêt).

## La traduction de code

La traduction de code est l'ensemble des étapes qui traduisent le C++ en langage binaire (lexing, parsing, optimisations, génération de l'assembleur, etc.).
C'est là que la majorité du travail est faite, le produit de la compilation sont des fichiers intermédiaires (.obj/.o selon le compilateur)

Le préprocesseur et la traduction de code sont déclenchés pour chaque "unitée de compilation" (les fichiers .c/.cpp), les headers ne sont compilés que par l'intermédiaire de leur inclusion dans les .cpp (et sont donc compilés plusieurs fois).

## L'édition des liens

L'une des particularité de la traduction de code est de pouvoir faire appel à des fonctions déclarées dans le même fichier mais définies dans une autre unité de compilation.

Exemple :

**Fichier main.cpp**:
```cpp
void sayHelloWorld(); //< Déclaration de sayHelloWorld() - Notez qu'en général c'est le rôle des headers de faire ces déclarations

int main()
{
    // On peut appeler la fonction sayHelloWorld
    sayHelloWorld();
}
```

**Fichier helloworld.cpp**:
```cpp
#include <iostream>

// Définition de la fonction sayHelloWorld
void sayHelloWorld()
{
    std::cout << "Hello world! " << std::endl;
}
```

La compilation va générer un fichier main.o et helloworld.o, sans lien les uns avec les autres.

Le rôle de l'édition de liens est de faire le lien (!) entre tous les .o/.obj pour produire l'exécutable final (.exe/.dll/.so/etc.) en résolvant les appels à des fonctions déclarées dans des unités de compilation différente.

Dans l'exemple ci-dessus, l'éditeur de lien (aussi appelé *linker*) va faire correspondre l'appel à `sayHelloWorld` avec sa déclaration dans helloworld.cpp, ce lien est fait par un nom attribué à la fonction (dépendant de son nom et de ses paramètres).

L'éditeur de lien, dernière étape de la compilation d'un projet, produira donc des erreurs s'il n'arrive pas à résoudre certaines fonctions ou si plusieurs définitions d'une même fonction sont en conflits (ce qui arrive si on définit une fonction dans un header sans `inline` par exemple).

# Les bibliothèques

Il est à noter également que l'éditeur de lien peut produire et se servir d'ensemble compilé de fonctions (appelés bibliothèques).  

Il existe deux catégories de bibliothèques, les statiques et les dynamiques.

## Les bibliothèques statiques (.lib/.a/etc.)

Une bibliothèque statique est un ensemble de produits de traduction de code (.o/.obj) mis ensembles dans une archive, et destiné à la compilation d'un autre exécutable.

Lorsqu'un éditeur de lien se sert d'une bibliothèque statique, il incorpore les unités de compilation supplémentaires au processus, il en résulte que le code des fonctions se retrouve embarqué dans l'exécutable final.

La résolution des symboles se fait à la compilation.

## Les bibliothèques dynamiques (.dll/.so/etc.)

À contrario, une bibliothèque dynamique est très proche du format exécutable classique, à la différence près qu'elle ne possède pas de point d'entrée (exemple : la fonction `main`) et ne peut donc pas être exécutée par le système.

La résolution de noms se fait en deux temps, une première fois à la compilation en prenant un fichier descriptif en référence (parfois même la bibliothèque dynamique elle-même) et une seconde fois au lancement de l'application.

# En ligne de commande

*Note, les lignes de commande passées ici ne sont qu'un exemple et ne sont pas optimales.*

Avec Clang/GCC, la compilation d'un seul fichier peut se faire simplement en utilisant

```
g++ main.cpp -o monprogramme
```
De cette façon toutes les étapes sont exécutées.

On peut également passer plusieurs fichiers:

```
g++ a.cpp b.cpp c.pp -o monprogramme
```

Néanmoins, cela va compiler tous les fichiers à chaque fois, ce qui peut vite devenir problématique.  
Pour éviter cela, on peut séparer la compilation en plusieurs appels:
```
g++ -c a.cpp
g++ -c b.cpp
g++ -c c.cpp
```
Le paramètre `-c` indique que le compilateur ne doit pas déclencher l'édition des liens et produira donc nos fichiers intermédiaires (a.o, b.o et c.o).

Pour faire tourner l'édition des liens séparément, on peut ensuite utiliser
```
g++ a.o b.o c.o -o monprogramme
```

L'avantage de séparer la compilation de cette façon est qu'en cas de modification d'un fichier .cpp (ou d'un header qu'il inclut !) on peut alors ne recompiler que celui-ci et réexecuter l'édition des liens

# Intégrer une bibliothèque dans son programme

En C comme en C++, intégrer une bibliothèque dans son programme consiste à donner trois informations cruciales au compilateur :

1. Où trouver les headers de la bibliothèque (pour les directives `#include`)
```
g++ -I/home/lynix/chemin/vers/ma/lib/include
```
2. Quelles bibliothèques (statiques ou dynamiques) lier à l'exécutable? (avec Clang et GCC l'ordre est important et doit être l'ordre inverse de dépendance)
```
g++ -lmalibA -lmalibB
```
3. Où trouver les fichier de bibliothèques?
```
g++ -L/home/lynix/chemin/vers/ma/lib/bin
```

Notes: certaines bibliothèques sont intégralement implémentées dans leurs headers et n'ont donc pas besoin des étapes 2 et 3

# Utiliser un outil de build

Face à la complexité de la compilation en C++, il est plus que recommandé de passer par un outil capable de donner des instructions au compilateur, et gérer les bibliothèques pour nous.

**Visual Studio** gère correctement le compilateur mais ne nous aidera que très peu pour les dépendances, et il ne fonctionne que pour Windows.

Si nous désirons toucher le plus de personnes et permettre le plus facilement un support multi-plateforme, nous pouvons alors employer des outils comme cmake, meson, premake, xmake, etc.

Celui que nous allons utiliser est `xmake`, il s'agit d'un gestionnaire de build assez récent utilisant le langage Lua pour décrire des projets.

Par exemple :
```lua
target("monprogramme") -- Nous déclarons un nouveau projet
    set_kind("binary") -- Nous définissons le type du projet
    add_files("src/*.cpp") -- Nous ajoutons quelques fichiers au projet
```

Nous pouvons ensuite lancer la compilatio en ligne de commande avec un simple `xmake` :
```
lynix@SirLynixVanDesktop:/test_project$ xmake
[ 16%]: ccache compiling.release src/A.cpp
[ 16%]: ccache compiling.release src/B.cpp
[ 16%]: ccache compiling.release src/main.cpp
[ 66%]: linking.release test_multifiles
[100%]: build ok!
```

En réexécutant la commande, nous observons que rien ne se produit, car aucun fichier n'a été touché :
```
lynix@SirLynixVanDesktop:/test_project$ xmake
[100%]: build ok!
```

En modifiant un fichier et en relançant la commande (avec `-v` pour afficher les commandes exécutées par xmake), on peut alors se rendre compte de ce que l'outil fait réellement pour nous :

```
lynix@SirLynixVanDesktop:$ touch src/A.cpp
lynix@SirLynixVanDesktop:$ xmake -v
[ 16%]: ccache compiling.release src/A.cpp
/usr/bin/ccache /usr/bin/gcc -c -m64 -fvisibility=hidden -fvisibility-inlines-hidden -O3 -DNDEBUG -o build/.objs/test_multifiles/linux/x86_64/release/src/A.cpp.o src/A.cpp
[ 66%]: linking.release test_multifiles
/usr/bin/g++ -o build/linux/x86_64/release/test_multifiles build/.objs/test_multifiles/linux/x86_64/release/src/A.cpp.o build/.objs/test_multifiles/linux/x86_64/release/src/B.cpp.o build/.objs/test_multifiles/linux/x86_64/release/src/main.cpp.o -m64 -s
[100%]: build o
```

L'un des avantages à utiliser un système de build est de pouvoir s'affranchir du compilateur derrière, en effet si nous utilisons xmake sur Windows celui-ci va plutôt chercher à utiliser le compilateur de Visual Studio:

```
lynix@SirLynixVanDesktop:/mnt/c/Users/Lynix/Documents/GitHub/IIM-CoursScripting/Compilation$ xmake.exe -v
checking for platform ... windows
checking for architecture ... x64
checking for vswhere.exe ... C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe
checking for cl.exe ... C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\HostX64\x64\cl.exe
checking for Microsoft Visual Studio (x64) version ... 2019
checking for dmd ... no
checking for ldc2 ... no
checking for gdc ... no
checking for zig ... no
checking for zig ... no
checking for link.exe ... C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\HostX64\x64\link.exe
checking for the linker (ld) ... link.exe
checking for cl.exe ... C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\HostX64\x64\cl.exe
checking for the c++ compiler (cxx) ... cl.exe
checking for the c++ compiler (cxx) ... cl.exe
checking for the c++ compiler (cxx) ... cl.exe
checking for C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\14.28.29910\bin\HostX64\x64\cl.exe ... ok
checking for flags (-Ox -fp:fast) ... ok
checking for flags (-DNDEBUG) ... ok
checking for ccache ... no
[ 16%]: compiling.release src\A.cpp
"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\bin\\HostX64\\x64\\cl.exe" -c /EHsc -nologo -Ox -fp:fast -DNDEBUG -Fobuild\.objs\test_multifiles\windows\x64\release\src\A.cpp.obj src\A.cpp
[ 16%]: compiling.release src\B.cpp
"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\bin\\HostX64\\x64\\cl.exe" -c /EHsc -nologo -Ox -fp:fast -DNDEBUG -Fobuild\.objs\test_multifiles\windows\x64\release\src\B.cpp.obj src\B.cpp
[ 16%]: compiling.release src\main.cpp
"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\bin\\HostX64\\x64\\cl.exe" -c /EHsc -nologo -Ox -fp:fast -DNDEBUG -Fobuild\.objs\test_multifiles\windows\x64\release\src\main.cpp.obj src\main.cpp
checking for flags (cl_sourceDependencies) ... ok
[ 66%]: linking.release test_multifiles.exe
"C:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\VC\\Tools\\MSVC\\14.28.29910\\bin\\HostX64\\x64\\link.exe" -nologo -dynamicbase -nxcompat -machine:x64 /opt:ref /opt:icf -out:build\windows\x64\release\test_multifiles.exe build\.ob
js\test_multifiles\windows\x64\release\src\A.cpp.obj build\.objs\test_multifiles\windows\x64\release\src\B.cpp.obj build\.objs\test_multifiles\windows\x64\release\src\main.cpp.obj
[100%]: build ok!
```

Cet outil gère donc pour nous notre projet de façon agnostique à la plateforme et peut faire correspondre nos diverses options aux options correctes du compilateur.

En plus de générer notre projet pour nous, il peut également générer des fichiers pour d'autres outils :

- CMake : `xmake project -k cmake`
- Makefile : `xmake project -k make`
- XCode : `xmake project -k xcode`
- VisualStudio : `xmake project -k vsxmake`

Mais la véritable force de XMake est sa capacité à gérer les bibliothèques externes pour nous.

Par exemple, intégrons la bibliothèque `fmt` à un projet simple :

```lua
add_requires("fmt") -- Liste la bibliothèque fmt comme une dépendance de façon générale (utile pour la configuration)

target("monprogramme") -- Nous déclarons un nouveau projet
    set_kind("binary") -- Nous définissons le type du projet
    add_files("src/*.cpp") -- Nous ajoutons quelques fichiers au projet
    add_packages("fmt") -- Nous disons que notre programme va utiliser la bibliothèque fmt
```

avec le code source suivant :  
**src/main.cpp:**
```cpp
#include <fmt/core.h>

int main()
{
    fmt::print("Hello, world!\n");
}
```

Nous lançons xmake:
```
lynix@SirLynixVanDesktop:$ xmake
checking for platform ... linux
checking for architecture ... x86_64
note: try installing these packages (pass -y to skip confirm)?
in xmake-repo:
  -> fmt 7.1.3
please input: y (y/n)
y
  => download https://github.com/fmtlib/fmt/releases/download/7.1.3/fmt-7.1.3.zip .. ok
  => install fmt 7.1.3 .. ok
[ 25%]: ccache compiling.release src/main.cpp
[ 50%]: linking.release monprogramme
[100%]: build ok!
lynix@SirLynixVanDesktop:$ xmake run
Hello, world!
```

La véritable force de xmake est d'être capable d'installer automatiquement des bibliothèques pour les intégrer à notre programme si elles ne sont pas trouvables sur notre système !

Si nous forçons une regénération (-r) et activons le mode verbeux (-v) nous pouvons voir de quelle façon la bibliothèque a été intégrée :
```
lynix@SirLynixVanDesktop:$ xmake -rv
[ 25%]: ccache compiling.release src/main.cpp
/usr/bin/ccache /usr/bin/gcc -c -m64 -isystem /home/lynix/.xmake/packages/f/fmt/7.1.3/ee402a251c6d4a11919a394127fd0afa/include -o build/.objs/monprogramme/linux/x86_64/release/src/main.cpp.o src/main.cpp
[ 50%]: linking.release monprogramme
/usr/bin/g++ -o build/linux/x86_64/release/monprogramme build/.objs/monprogramme/linux/x86_64/release/src/main.cpp.o -m64 -L/home/lynix/.xmake/packages/f/fmt/7.1.3/ee402a251c6d4a11919a394127fd0afa/lib -lfmt
[100%]: build ok!
```

(note: -isystem est équivalent à -I à l'exception qu'il précise au compilateur qu'il ne faut pas générer de warnings venant de ces headers, afin de nous éviter des warnings venant de parties de notre programme que nous ne contrôlons pas).

Presque aussi simple qu'en Rust !
