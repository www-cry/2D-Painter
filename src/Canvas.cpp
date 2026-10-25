#include "Canvas.h"
#include <iostream>
bool Canvas::resize(sf::Vector2u newSize, sf::Color bgcolor, const sf::Image* copyImg)
{
	if (!this->canvasTexture.resize(newSize))
	{
		return false; // safe check
	}

	this->canvasTexture.clear(bgcolor); // clearing w/e could be there

	if (copyImg != nullptr && copyImg->getSize().x > 0 && copyImg->getSize().y > 0) // if the image exists and it's params aren't negative somehow
	{
		sf::Texture tmpTxt;
		
		if (tmpTxt.loadFromImage(*copyImg))
		{
			sf::Sprite tmpSpr(tmpTxt);
			this->canvasTexture.draw(tmpSpr);
		}
	}
	this->canvasTexture.display();
	this->canvasSprite.setTexture(this->canvasTexture.getTexture(), true);
	canvasSprite.setTextureRect(sf::IntRect({ 0, 0 }, sf::Vector2i(newSize)));
	return true;
}
// the function basically sets the new canvas with the parameters from the struct in newfilemenu which are modified in the WindowCore.cpp event handler