// Exemple montrant l'implémentation du mouvement avec un type maison
// Par Jérôme Leclercq pour l'IIM - 2021

#include <cstring> // std::memcpy, std::strlen
#include <iostream>
#include <string>

// Petite classe pour représenter le comportement de std::string et le mouvement
class MyStr
{
	public:
		// Constructeur par défaut (ne prenant aucun paramètre)
		MyStr() :
		m_length(0),
		m_str(nullptr)
		{
		}

		// Constructeur prenant un paramètre
		MyStr(const char* str)
		{
			m_length = std::strlen(str);
			m_str = new char[m_length];
			std::memcpy(m_str, str, m_length);
		}

		// Constructeur par copie
		MyStr(const MyStr& str) :
		m_length(str.m_length)
		{
			m_str = new char[m_length];
			std::memcpy(m_str, str.m_str, m_length);
		}

		// Constructeur par mouvement : on copie le pointeur de l'objet source et on modifie ce dernier pour qu'il ne pointe plus vers ce pointeur
		MyStr(MyStr&& str) :
		m_length(str.m_length),
		m_str(str.m_str)
		{
			str.m_str = nullptr;
		}

		~MyStr()
		{
			// Si m_str est nullptr, delete[] ne fait rien
			delete[] m_str;
		}

		// Opérateur d'assignation par copie
		MyStr& operator=(const MyStr& str)
		{
			// On libère la chaine précédente, si elle existe
			delete[] m_str;

			// Allocation d'une nouvelle chaine de caractère et copie de celle de l'objet source
			m_length = str.m_length;
			m_str = new char[m_length];
			std::memcpy(m_str, str.m_str, m_length);

			return *this; //< this étant un pointeur, déréférencer ce pointeur nous donnera une référence sur l'objet courant
		}

		// Opérateur d'assignation par mouvement
		MyStr& operator=(MyStr&& str)
		{
			// On libère la chaine précédente, si elle existe
			delete[] m_str;

			// Pas d'allocation ici, nous volons/vampirisons l'objet source pour nous approprier son contenu
			m_length = str.m_length;
			m_str = str.m_str;
			str.m_str = nullptr; // Modification de la chaine possédée par l'objet source pour qu'elle n'essaie pas de le libérer lors de sa destruction

			return *this; //< this étant un pointeur, déréférencer ce pointeur nous donnera une référence sur l'objet courant
		}

	private:
		std::size_t m_length;
		char* m_str;
};

MyStr func()
{
	MyStr str = "Bonjour le monde !";
	return str; //< Note: pas de std::move lors du return, celui-ci est inutile (le compilateur le prend déjà en compte)
}

int main()
{
	MyStr str = "Bonjour"; //< Appel au constructeur prenant un const char*
	MyStr str2 = std::move(str); //< Mouvement de str vers vers2
	// À partir d'ici str ne contient plus rien et peut être détruit

	MyStr str3 = func(); //< Constructeur par mouvement avec le retour de la fonction (qui est déjà une rvalue)

	return 0;
}
