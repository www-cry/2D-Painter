#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Config.hpp> // those were added to make it easier to just use everything possible at hand
#include "PainterMenu.h"
#include "NewFileMenu.h"
#include "Canvas.h" // for class objects
class WindowCore
{
public:
	sf::RenderWindow* win; 
	void initint();
	void initw();
	WindowCore(); // initializers with initiating windows
	paintermenu menu; // object of the menu class
	newfilemenu nmenu; // for setting up the canvas parameters
	virtual ~WindowCore(); // destructor for mainwindow
	void update();
	void render(); // update & render for interaction with the window
	void eventpoll(); // description of events if u press the buttons
	bool getwindowisopen(); // boolean check-up
	
};