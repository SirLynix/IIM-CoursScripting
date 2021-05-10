// Exemple montrant l'impact de la bonne gestion des exceptions avec la gestion de mémoire manuelle
// Par Jérôme Leclercq pour l'IIM - 2021

// ------------------------------ 
// Fonction originale: aucune gestion des exceptions
unsigned char* DrawBoDes1(unsigned int width, unsigned int height)
{
    assert(width > 0);
    assert(height > 0);

    unsigned char* img = new unsigned char[width * height * 4];
    
    if (!FillImage(img, Color(135, 206, 235)))
    {
        delete[] img;
        
        return nullptr;
    }
    
    // Chargeons une image de soleil à mettre dans le coin haut-gauche
    unsigned char* sunImg = LoadImgFromFile("sun.png");
    if (!sunImg)
    {
        delete[] img;
        
        return nullptr;
    }
    
    if (!BlitImage(img, sunImg, 0, 0))
    {
        delete[] sunImg;
        delete[] img;
        
        return nullptr;
    }
    
    delete[] sunImg; //< Nous avons terminé avec le soleil, on le libère

    // Maintenant chargeons une image de maison
    unsigned char* houseImg = LoadImgFromFile("house.png");
    if (!houseImg)
    {
        delete[] img;
        
        return nullptr;
    }
    
    if (!BlitImage(img, houseImg, 300, 200))
    {
        delete[] houseImg;
        delete[] img;
        
        return nullptr;
    }
    
    delete[] houseImg; //< Nous avons fini avec la maison, on la libère

    // Autres opération similaires (dessin de nuages, colline, etc.)
    
    return img;
}

// ------------------------------ 
// La même fonction mais gérant correctement les exceptions
unsigned char* DrawBoDes1(unsigned int width, unsigned int height)
{
    assert(width > 0);
    assert(height > 0);
 
    // Pas besoin de protéger le new ici: s'il déclenche une exception nous n'avons aucune mémoire à libérer
    unsigned char* img = new unsigned char[width * height * 4];
 
    try
    {
        if (!FillImage(img, Color(135, 206, 235)))
        {
            delete[] img;
 
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        delete[] img;
 
        throw; //< On relance l'exception
    }
 
    // Chargeons une image de soleil à mettre dans le coin haut-gauche
    unsigned char* sunImg;
    try
    {
        sunImg = LoadImgFromFile("sun.png");
        if (!sunImg)
        {
            delete[] img;
 
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        delete[] img;
 
        throw;
    }
 
    try
    {
        if (!BlitImage(img, sunImg, 0, 0))
        {
            delete[] sunImg;
            delete[] img;
 
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        delete[] sunImg;
        delete[] img;
 
        return nullptr;
    }
 
    delete[] sunImg; //< Nous avons terminé avec le soleil, on le libère
 
    // Maintenant chargeons une image de maison
    unsigned char* houseImg;
    try
    {
        houseImg = LoadImgFromFile("sun.png");
        if (!houseImg)
        {
            delete[] img;
 
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        delete[] img;
 
        throw;
    }
 
    try
    {
        if (!BlitImage(img, houseImg, 300, 2000))
        {
            delete[] houseImg;
            delete[] img;
 
            return nullptr;
        }
    }
    catch (const std::exception& e)
    {
        delete[] houseImg;
        delete[] img;
 
        return nullptr;
    }
 
    delete[] houseImg; //< Nous avons fini avec la maison, on la libère
 
    // Autres opération similaires (dessin de nuages, colline, etc.)
 
    return img;
}

// ------------------------------ 
// La même fonction mais utilisant des conteneurs

std::vector<unsigned char> DrawBoDes1(unsigned int width, unsigned int height)
{
    assert(width > 0);
    assert(height > 0);

    std::vector<unsigned char> img(width * height * 4);
    if (!FillImage(img, Color(135, 206, 235)))
        return {};
    
    std::vector<unsigned char> fileImg;

    // Chargeons une image de soleil à mettre dans le coin haut-gauche
    if (!LoadImgFromFile("sun.png", fileImg))
        return {};
    
    if (!BlitImage(img, fileImg, 0, 0))
        return {};
    
    fileImg.clear();
    
    // Maintenant chargeons une image de maison
    if (!LoadImgFromFile("house.png", fileImg))
        return {};
    
    if (!BlitImage(img, fileImg, 300, 200))
        return {};

    // Autres opération similaires (dessin de nuages, colline, etc.)
    
    return img;
}
