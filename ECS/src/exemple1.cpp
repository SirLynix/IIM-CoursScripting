#include <entt/entt.hpp>
#include <iostream>

// On déclare deux composants (du type que l'on souhaite)
struct Position
{
	float x = 0.f;
	float y = 0.f;
};

struct Velocity
{
	float x = 0.f;
	float y = -10.f;
};

int main()
{
	// Le centre d'un ECS est son registre, aussi appelé "world" dans certaines implémentations.
	// C'est lui qui contient toutes les entités et leurs composants
	entt::registry registry;
	
	// On créé une première entité, dans un ECS une entité n'est rien d'autre qu'un nombre
	entt::entity firstEntity = registry.create();

	// Pour lui rattacher des données, on demande au registre d'attacher un composant à notre entité.
	// la fonction emplace permet de créer un composant et de l'associer à notre entité.
	// N'importe quel type peut être utilisé comme composant, mais ils ne peuvent être ajoutés qu'une fois à chaque entité
	registry.emplace<Position>(firstEntity);
	registry.emplace<Velocity>(firstEntity);

	// On peut construire le composant avec des paramètres pour son constructeur, en les passant après l'entité
	// Ici on se sert d'un std::string comme d'un nom que nous donnerions à notre entité (on pourrait également faire un composant Name)
	registry.emplace<std::string>(firstEntity, "Albert");

	// On peut créer une seconde entité avec des composants différents, ici nous allons créer une autre entité sans le composant Velocity
	entt::entity secondEntity = registry.create();

	// L'ordre dans lequel les composants sont ajoutés n'est pas important
	registry.emplace<std::string>(secondEntity, "Jean");

	auto& secondPos = registry.emplace<Position>(secondEntity);
	secondPos.x = 20.0f;
	// Attention: un ECS peut déplacer nos composants en mémoire à sa guise, nous ne devons pas conserver de référence sur les composants
	// à la place nous devons toujours le récupérer au moment d'agir dessus

	// On se sert d'une boucle pour faire avancer le temps dans notre programme
	for (unsigned int i = 0; i < 100; ++i)
	{
		// Une vue est une façon de récupérer uniquement les entités possédant certains composants
		// Nous n'allons donc récupérer que les entités possédant à la fois une Position et une Velocity
		auto posVelView = registry.view<Position, Velocity>();

		// On peut itérer dessus comme un tableau
		for (entt::entity entity : posVelView)
		{
			// On peut ensuite récupérer une référence sur les composants depuis la vue
			auto& pos = posVelView.get<Position>(entity);
			auto& vel = posVelView.get<Velocity>(entity);

			// et les modifier comme bon nous semble
			pos.x += vel.x;
			pos.y += vel.y;
		}

		// On fait de même pour l'association Position/std::string (que toutes nos entités possèdent ici)
		auto posView = registry.view<Position, std::string>();
		for (entt::entity entity : posView)
		{
			auto& pos = posView.get<Position>(entity);
			auto& name = posView.get<std::string>(entity);

			// On affiche le nom de l'entité et sa position actuelle
			std::cout << "Entity " << name << " position: (" << pos.x << ", " << pos.y << ")\n";
		}

		std::cout << "---" << std::endl;
	}
}
