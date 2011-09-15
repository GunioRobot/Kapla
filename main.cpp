/*
 Le code source pr�sent sur cette page est utilisable � volont�, m�me pour utilisation commerciale.
 La reproduction de l'article est autoris�e avec l'accord de l'auteur.
 */


//#include "main.h"
#include "fonctions.h"


// Notre application SFML
sf::Window	Application;



int main ( int argc, char **argv )
{
	// On cr�e notre fen�tre gr�ce � SFML
	Application.Create( sf::VideoMode( 800, 500, 32 ), "SFML : Bullet physics", sf::Style::Titlebar | sf::Style::Resize | sf::Style::Close );
	
    //Variable pour calculer le delta de d�placement de la souris quand les clicks droit et gauche de la souris sont enfonc� pour manipuler la cam�ra
    unsigned int previousX, previousY;
    // Pour d�clancher la chute d'un seul kapla quand la touche Espace est relach�e
	bool trigger = 0; 
    double deltaX = 0;
    double deltaY = 0;
	
	/// Initialisation d'OpenGL
	
	// On active la lumi�re 0
	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
	
	// Quelques autres options OpenGL
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR);
	
	// Couleur de fond d'�cran
    glClearColor(0.7,0.7,0.7,0);
    //On initialise une cam�ra qui sera plac� par d�faut par le constructeur
    Camera camcam;
    
	// l'espace d'affichage
	glViewport( 0, 0, 800, 500 );
    
    // une instance de curseur en forme de kapla qui se d�place au dessus de la construction, il peut �tre retourn� pour avoir une des 3 surfaces diff�rentes face au sol. On peut orienter l'angle du kapla dans le plan du sol
	Cursor cursor;
    //cr�e un "monde" Bullet, n�cessaire pour mettre les kaplas et les faire int�ragir. Pour fonctionner plusieur doivent �tre cr��s pour que "btDiscreteDynamicWorld" cr�e un objet world dans lequel ajouter les objets Kapla.
    //pour simplifier l'initialisation, j'ai pens� r�unir la cr�ation des diff�rents objets n�cessaire dans un class world
    
    //probl�me pour initialiser le monde, il y a divers objet � g�n�rer pour le mettre en place mais ca ne semble pas marcher 
    World monde;
    // d�claration d'un conteneur pour les objet de type Kapla
    std::vector<Kapla> keva;
    
    

 // pour avoir les infos clavier en temps r�el
	const sf::Input& Input = Application.GetInput();
	// Notre boucle d'affichage
	while(Application.IsOpened() )
	{

        // r�f�rence vers l'entr�e associ�e � une fen�tre (pour r�cup�rer les donn�s clavier en temps r�el
        
        sf::Event Event;
        while (Application.GetEvent(Event))
        {
            if (Event.Type == sf::Event::Resized)
                glViewport(0, 0, Event.Size.Width, Event.Size.Height);
            // Fen�tre ferm�e
            if (Event.Type == sf::Event::Closed)
                Application.Close();
            
            // Touche 'echap' appuy�e
            if ((Event.Type == sf::Event::KeyPressed) && (Event.Key.Code == sf::Key::Escape))
                Application.Close();
        }
            //Utilise les fl�che pour d�placer le Kapla qui va �tre lach�
            // Get some useless input states, just to illustrate the tutorial
            bool         LeftKeyDown     = Input.IsKeyDown(sf::Key::Left);
            bool         RightKeyDown     = Input.IsKeyDown(sf::Key::Right);
            bool         UpKeyDown     = Input.IsKeyDown(sf::Key::Up);
            bool         DownKeyDown     = Input.IsKeyDown(sf::Key::Down);
            bool         RightButtonDown = Input.IsMouseButtonDown(sf::Mouse::Right);
            bool         LeftButtonDown = Input.IsMouseButtonDown(sf::Mouse::Left);
            bool         Espace     = Input.IsKeyDown(sf::Key::Space);

            unsigned int MouseX          = Input.GetMouseX();
            unsigned int MouseY          = Input.GetMouseY();
        
       // en approchant le curseur Kapla du bord la cam�ra d�zoom pour laisser voir tous les Kapla (je pr�sume que la souris est en coordon�e absolue par rapport � la fen�tre avec le 0 en haut � gauche.
        if (MouseX<20) {

            camcam.extendScene(MouseX,0.0,0.0,0.0);
        }
        else if (MouseX > (Event.Size.Width - 20)) {
            camcam.extendScene(0.0,(MouseX-(Event.Size.Width - 20)),0.0,0.0);
        }
        
        
        if (MouseY<20) {
            
            camcam.extendScene(0.0, 0.0,MouseX,0.0);
        }else if (MouseY > (Event.Size.Height - 20)) {
            camcam.extendScene(0.0,0.0,0.0,(MouseY-(Event.Size.Height - 20)));
        }
        
        //Tant que la souris bouge la souris bouge
        
        while ((previousY=!MouseY)||(previousX=!MouseX))
        {
            //si les deux boutons sont enfonc�s
            if ((RightButtonDown)&&(LeftButtonDown))
            {

                //utiliser le delta de d�placement pour d�placer la cam�ra autour de la sc�ne 
            deltaX = MouseX - previousX;
            deltaY = MouseY - previousY;
            
            camcam.set(deltaX,deltaY);
            }
            
        }
        //NEED convertiseur coord souris -> coord du plan visible par la cam�ra
        // le d�placement sur le plan de la fen�tre est proportionel � celui du plan de construction
        cursor.set(MouseX, MouseY);
        // dessin  le curseur
        cursor.drawKapla(5,15,1); 
        
        
        //touche espace enfonc�e et relach�e
        if ((trigger)&&!(Espace))
        {
            // ajoute un kapla dans le monde au coordon�e et orientation et direction du curseur.
            keva.push_back(Kapla());
        
            trigger=0;
        }else if ((Espace)&&!(trigger))
        {
            trigger=1;
        }                
                                     
            previousX=MouseX;
            previousY=MouseY; 

        
		// Update dynamics

		        monde.simuStep();

		
		// On a	ffiche le sol;
		box(10,1,10);
		

		
		// swap buffers, etc
		Application.Display();
	}
}
