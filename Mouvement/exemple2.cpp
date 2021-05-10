// Exemple montrant le fonctionnement du mouvement avec un type maison
// Par Jérôme Leclercq pour l'IIM - 2021

#include <iostream>
#include <string>

// Petite classe pour symboliser les opérations faites dessus
class Foo
{
	public:
		// Constructeur par défaut (ne prenant aucun paramètre)
		Foo()
		{
			std::cout << "Foo()" << std::endl;
		}

		// Constructeur par copie
		Foo(const Foo& str)
		{
			std::cout << "Foo(const Foo&)" << std::endl;
		}

		// Constructeur par mouvement
		Foo(Foo&& str)
		{
			std::cout << "Foo(Foo&&)" << std::endl;
		}

		// Destructeur
		~Foo()
		{
			std::cout << "~Foo()" << std::endl;
		}

		// Opérateur d'assignation par copie
		Foo& operator=(const Foo& str)
		{
			std::cout << "Foo::operator=(const Foo&)" << std::endl;
			return *this; //< this étant un pointeur, déréférencer ce pointeur nous donnera une référence sur l'objet courant
		}

		// Opérateur d'assignation par mouvement
		Foo& operator=(Foo&& str)
		{
			std::cout << "Foo::operator=(Foo&&)" << std::endl;
			return *this; //< this étant un pointeur, déréférencer ce pointeur nous donnera une référence sur l'objet courant
		}
};

void test1()
{
	std::cout << "-- Test1 --" << std::endl;

	Foo f; //< Constructeur par défaut
	Foo f2(f); //< Constructeur par copie
}

void test2()
{
	std::cout << "-- Test2 --" << std::endl;

	Foo f; //< Constructeur par défaut
	Foo f2(std::move(f)); //< Constructeur par mouvement
}

void test3()
{
	std::cout << "-- Test3 --" << std::endl;

	Foo f; //< Constructeur par défaut
	std::move(f); //< Rien du tout (std::move n'effectue pas le mouvement, il ne fait que l'autoriser)
}

void test4()
{
	std::cout << "-- Test4 --" << std::endl;

	Foo f; //< Constructeur par défaut
	Foo f2(static_cast<Foo&&>(f)); //< Constructeur par mouvement
}

int main()
{
	test1();
	test2();
	test3();
	test4();
	return 0;
}
