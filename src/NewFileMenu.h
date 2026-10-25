#pragma once
#include <SFML/Graphics.hpp>
#include "Canvas.h"
#include <string>
struct canvassettings
{
	unsigned int Bwidth = 800;
	unsigned int Bheight = 600;
	sf::Color Bcolor = sf::Color::White;
	// base params of the canvas
	bool confirm = false;
	bool declined = false;
	// booleans for OK and LEAVE buttons
	sf::Image currimg; // for resize in Canvas.h
	sf::Vector2u getSizeofCanvas() const { return sf::Vector2u(Bwidth, Bheight); } // getter function for canvas parsed params
};

class newfilemenu
{
private:
	bool newfilewinisopen = false;
public:
	void setNmenuState(bool state) { newfilewinisopen = state; }
	bool getNmenuState() const { return newfilewinisopen; }
	newfilemenu();

	sf::Font newfilemenufont; // the font for the dialogue window(prolly should've just use the one in the painterMenu class ngl)

	sf::RectangleShape dPanel;
	sf::Text titletext;
	sf::Text WidthLabel;
	sf::Text HeightLabel;
	sf::Text bgLabel;
	// User interface for the new file

	sf::RectangleShape boxWidth;
	sf::RectangleShape boxHeight;
	sf::Text newWidth;
	sf::Text newHeight;
	// input width and height

	sf::RectangleShape WhiteBgButton;
	sf::RectangleShape TranspBgButton;
	sf::Text WhiteBgText;
	sf::Text TranspBgText;
	sf::Color SelectedBgColor = sf::Color::White; // quite self-explanatory
	// input background setting

	sf::RectangleShape Ok;
	sf::RectangleShape Leave;
	sf::Text OK;
	sf::Text LEAVE;
	// Control buttons

	std::string widthText = "800";
	std::string heightText = "600";
	enum class typinmode { None, Width, Height }; // for texting
	typinmode currmod = typinmode::None;
	// for inputting width and height
	canvassettings newcanvasparam; // object for the struct for changin values
	Canvas newcanvas; // object of canvas class for resize function
	const canvassettings& getResult() const { return this->newcanvasparam; } // result getter
	void UISetup(sf::Vector2f winsize); // setting positions of each object in nmenu

	void drawnewfilemenu(sf::RenderWindow& win); // basically a render function

	canvassettings updatenewfilemenu(sf::RenderWindow& win, const sf::Event& ev); // for the menu interactions
};