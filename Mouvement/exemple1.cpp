// Exemple montrant le mouvement basique du contenu d'un std::string à un autre
// Par Jérôme Leclercq pour l'IIM - 2021

#include <iostream>
#include <string>

int main()
{
	// Construction d'un std::string classique
	std::string str = "Bonjour !";

	// Construction d'un std::string à partir du premier
	std::string str2 = std::move(str);
	// À partir d'ici, str2 a récupéré le contenu de str et ce dernier est maintenant dans un état indéterminé (mais destructible)

	std::cout << str2 << std::endl; //< Affichera "Bonjour !"
	std::cout << str << std::endl; //< Pourrait afficher n'importe quoi (rien du tout, "Bonjour !", ou même autre chose)

	return 0;
}
