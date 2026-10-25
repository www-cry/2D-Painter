#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
//#include "PainterMenu.h"
class Canvas
{
public:
	sf::RenderTexture canvasTexture;
	sf::Sprite canvasSprite;
	// for resizing the texture and updating sprite
	sf::Color CanvasBgForFunc = sf::Color::White;
	sf::Vector2f position{ 200.0f, 50.0f }; // default position of the canvas in paintermenu

	sf::RenderTexture& getTexturefrom() { return canvasTexture; } 
	sf::Vector2f getPosition() const { return position; }
	sf::Vector2f getSize() const { return sf::Vector2f(canvasTexture.getSize()); }
	sf::FloatRect getBounds() const { return canvasSprite.getGlobalBounds(); }
	sf::Image toImage() { return this->canvasTexture.getTexture().copyToImage(); }
	// Getter functions

	void setPosition(const sf::Vector2f pos) 
	{ 
		this->position = pos; 
		this->canvasSprite.setPosition(pos); 
	}
	// for menuRESIZEevent to change the canvas position

	void restoreImg(const sf::Image& img) {
		sf::Texture tmptxt;
		if (tmptxt.loadFromImage(img))
		{
			sf::Sprite restoredSprite(tmptxt);
			canvasTexture.clear(this->CanvasBgForFunc); // TO DO: Get here the BColor of the canvas
			canvasTexture.draw(restoredSprite);
			canvasTexture.display();
			this->canvasSprite.setTexture(this->canvasTexture.getTexture());
		}
	}
	// used for ctrl + z interaction

	void render(sf::RenderWindow& win)
	{
		win.draw(canvasSprite);
	}
	// simple render

	bool resize(sf::Vector2u newSize, sf::Color bgcolor = sf::Color::White, const sf::Image* copyImg = nullptr);
	// this one get's triggered when we hit ok in the newfilemenu

	Canvas() : canvasTexture(), canvasSprite(canvasTexture.getTexture()) {}
	// default constructor(currently goes with 0,0 if I want to change it set canvasTexture desired size
};

