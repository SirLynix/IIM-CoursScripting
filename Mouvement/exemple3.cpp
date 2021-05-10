// Exemple montrant le mouvement d'un std::string d'une fonction vers un objet (changement de propriétaire)
// Par Jérôme Leclercq pour l'IIM - 2021

#include <iostream>
#include <string>

// Classe symbolisant un joueur dans une application
class Player
{
	public:
		// Un accesseur permettant de récupérer une référence vers le nom du joueur
		const std::string& GetName() const
		{ 
			return m_name;
		}

		// Fonction permettant de changer le nom
		void UpdateName(std::string name)
		{
			m_name = std::move(name);
		}

	private:
		std::string m_name;
};

void test1()
{
	// Premier test : passons un std::string par valeur
	std::string nick = "SirLynixVanFrietjes";

	Player player;
	player.UpdateName(nick); //< Copie de nick vers name et mouvement de celui-ci vers m_name
	
	// La valeur de nick n'est pas altéré
}

void test2()
{
	// Second test : passons un std::string par mouvement
	std::string nick = "SirLynixVanFrietjes";

	Player player;
	player.UpdateName(std::move(nick)); //< Mouvement de nick vers le paramètre name, et mouvement de celui-ci vers m_name

	// La valeur de nick est altérée, nous ne pouvons plus l'utiliser
}

void test3()
{
	// Troisième test : passons un std::string par construction implicite
	Player player;
	player.UpdateName("SirLynixVanFrietjes"); //< Construction d'un std::string temporaire et mouvement de celle-ci vers m_name
}

int main()
{
	test1();
	test2();
	test3();

	return 0;
}

/*
Note: Nous pourrions ici implémenter deux versions d'UpdateName, l'une prenant une référence constante et l'autre prenant une référence de rvalue
de cette façon:
void UpdateName(const std::string& name)
{
	m_name = name; // Copie de name vers m_name
}

void UpdateName(std::string&& name)
{
	m_name = std::move(name); // Mouvement de name vers m_name
}

Avec un objet comme std::string cela est très légèrement plus efficace (voir https://quick-bench.com/q/GteHG2e_-j4rOrSLgVTuds7U2w8), mais augmente
la complexité du code de façon exponentielle en fonction du nombre de paramètres.

Par exemple :

struct Foo {
    std::string A;
    std::string B;
    std::string C;

    Foo(std::string _A, std::string _B, std::string _C) : 
	A(std::move(_A)), 
	B(std::move(_B)), 
	C(std::move(_C)) 
	{
	}
}

VS

struct Foo 
{
	std::string A;
	std::string B;
	std::string C;

	Foo(const std::string& _A, const std::string& _B, const std::string& _C) :
	A(_A),
	B(_B),
	C(_C)
	{
	}

	Foo(std::string&& _A, const std::string& _B, const std::string& _C) :
	A(std::move(_A)),
	B(_B),
	C(_C)
	{
	}

	Foo(const std::string& _A, std::string&& _B, const std::string& _C) :
	A(_A),
	B(std::move(_B)),
	C(_C)
	{
	}

	Foo(const std::string& _A, const std::string& _B, std::string&& _C) :
	A(_A),
	B(_B),
	C(std::move(_C))
	{
	}

	Foo(std::string&& _A, std::string&& _B, const std::string& _C) :
	A(std::move(_A)),
	B(std::move(_B)),
	C(_C)
	{
	}

	Foo(std::string&& _A, const std::string& _B, std::string&& _C) :
	A(std::move(_A)),
	B(_B),
	C(std::move(_C))
	{
	}

	Foo(std::string&& _A, std::string&& _B, std::string&& _C) :
	A(std::move(_A)),
	B(std::move(_B)),
	C(std::move(_C))
	{
	}
}

Pour cette raison on passera donc par référence constante si nous n'avons pas besoin de nous approprier l'objet et par valeur + mouvement lorsque nous désirons nous l'approprier.
Quelques liens :
https://stackoverflow.com/questions/7592630/is-pass-by-value-a-reasonable-default-in-c11
https://stackoverflow.com/questions/2139224/how-to-pass-objects-to-functions-in-c
https://sirlynixvanfrietjes.be/2018/02/25/cpp-moderne-le-passage-dobjets-par-argument/

*/
