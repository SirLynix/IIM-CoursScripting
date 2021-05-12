#include <entt/entt.hpp>
#include <iostream>

// On d�clare deux composants (du type que l'on souhaite)
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
	// Le centre d'un ECS est son registre, aussi appel� "world" dans certaines impl�mentations.
	// C'est lui qui contient toutes les entit�s et leurs composants
	entt::registry registry;
	
	// On cr�� une premi�re entit�, dans un ECS une entit� n'est rien d'autre qu'un nombre
	entt::entity firstEntity = registry.create();

	// Pour lui rattacher des donn�es, on demande au registre d'attacher un composant � notre entit�.
	// la fonction emplace permet de cr�er un composant et de l'associer � notre entit�.
	// N'importe quel type peut �tre utilis� comme composant, mais ils ne peuvent �tre ajout�s qu'une fois � chaque entit�
	registry.emplace<Position>(firstEntity);
	registry.emplace<Velocity>(firstEntity);

	// On peut construire le composant avec des param�tres pour son constructeur, en les passant apr�s l'entit�
	// Ici on se sert d'un std::string comme d'un nom que nous donnerions � notre entit� (on pourrait �galement faire un composant Name)
	registry.emplace<std::string>(firstEntity, "Albert");

	// On peut cr�er une seconde entit� avec des composants diff�rents, ici nous allons cr�er une autre entit� sans le composant Velocity
	entt::entity secondEntity = registry.create();

	// L'ordre dans lequel les composants sont ajout�s n'est pas important
	registry.emplace<std::string>(secondEntity, "Jean");

	auto& secondPos = registry.emplace<Position>(secondEntity);
	secondPos.x = 20.0f;
	// Attention: un ECS peut d�placer nos composants en m�moire � sa guise, nous ne devons pas conserver de r�f�rence sur les composants
	// � la place nous devons toujours le r�cup�rer au moment d'agir dessus

	// On se sert d'une boucle pour faire avancer le temps dans notre programme
	for (unsigned int i = 0; i < 100; ++i)
	{
		// Une vue est une fa�on de r�cup�rer uniquement les entit�s poss�dant certains composants
		// Nous n'allons donc r�cup�rer que les entit�s poss�dant � la fois une Position et une Velocity
		auto posVelView = registry.view<Position, Velocity>();

		// On peut it�rer dessus comme un tableau
		for (entt::entity entity : posVelView)
		{
			// On peut ensuite r�cup�rer une r�f�rence sur les composants depuis la vue
			auto& pos = posVelView.get<Position>(entity);
			auto& vel = posVelView.get<Velocity>(entity);

			// et les modifier comme bon nous semble
			pos.x += vel.x;
			pos.y += vel.y;
		}

		// On fait de m�me pour l'association Position/std::string (que toutes nos entit�s poss�dent ici)
		auto posView = registry.view<Position, std::string>();
		for (entt::entity entity : posView)
		{
			auto& pos = posView.get<Position>(entity);
			auto& name = posView.get<std::string>(entity);

			// On affiche le nom de l'entit� et sa position actuelle
			std::cout << "Entity " << name << " position: (" << pos.x << ", " << pos.y << ")\n";
		}

		std::cout << "---" << std::endl;
	}
}
