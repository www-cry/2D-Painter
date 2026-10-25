#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <string>
#include "Canvas.h"
#include "NewFileMenu.h"
#include <cmath>
class paintermenu
{
	private:
	std::vector<sf::Image> undobuffer; // buffer for ctrl - z ( maybe we need to put a size for it in case someone tries to put 30000 stuff on it )
	size_t undosize = 20; // to prevent overflow stacking 
	std::vector<sf::RectangleShape> colorswitches; // for pallet blocks
	std::vector<sf::Color> pallete;
	sf::Color currentcolor = sf::Color::Black; // default color at start
	sf::RectangleShape colorspecterblock; // for color specter
	sf::Image colorspecterimage; // same as csb
	sf::Texture colorspecterTexture;
	sf::Vector2i prevpos; // for drawing from prevposition to currposition of da mouse based on click of LMB(p0 p1 in future)
	sf::Vector2i currpos; 
	public:

	sf::Text newfile;
	sf::Text leave; 
	sf::Text savefile;
	sf::Text flush;
	sf::Font menufont; // buttons and font of the painterMenu

	Canvas newcanvas; // for referencing the canvas
	newfilemenu canvasPARAMSFROMNEWFILEMENU; // for pulling the parameters from the struct in the newfilemenu class
	sf::Image canvasimage;
	sf::RenderTexture canvas;
	// Canvas creating shenanigans(NOTE: I DELETED THE STANDART CONSTRUCTOR SO IT NO LONGER WORKED)
	
	sf::RectangleShape currentColorBlock;
	sf::RectangleShape currentToolBlock;
	sf::Text currentColorText;
	sf::Text currentToolText;
	sf::Text currentStampText;
	// current stuf bs

	sf::RectangleShape topBar;
	sf::RectangleShape sidePanel;
	sf::RectangleShape colorswitchesblock; // in idea to make it like base transparent but outline dark-gray?
	sf::RectangleShape gradientblock; // kinda the same as colorswitchesblock
	sf::RectangleShape PencilBlock;
	sf::RectangleShape BrushBlock;
	sf::RectangleShape EraserBlock;
	sf::RectangleShape PalleteBlock;
	
	bool isdrawing = false; // boolean for keepping the drawing
	enum class menustate{mainmenu, openfilemenu, savemenu, newfilemenu, paintmenu}; // changins the states
	std::vector<sf::Image> toolsicons; // for installing images of the icons
	std::vector<sf::Texture> tooltxt; // for getting their textures from images
	std::vector<sf::RectangleShape> toolblock; // we set it as vector as I want to separate each active tool used(mb I'll also need the booleans for it to work without a fail)
	enum class tools{pencil, brush, eraser, pallete}; // changing the tools
	tools currenttool = tools::pencil; // default tool
	sf::CircleShape stamp; // stamp for pencil
	sf::CircleShape stampB; // stamp for brush
	sf::CircleShape stampE; // stamp for Eraser
	sf::CircleShape stampD; // stamp for demonstration
	menustate currentstate = menustate::mainmenu; // default state
	paintermenu(); // constructor

	void setupUI(); // tie coordinates

	void menuevents(sf::RenderWindow& win); // for now it's only hovereffect for the buttons of the paintermenu(will be changed once I get my hands on normal rendering tool)

	void draw(sf::RenderWindow& win); // Render function(prolly should've named it render then)

	void drawhandle(tools current, Canvas& canvas, sf::Vector2f p0, sf::Vector2f p1); // basically a switch painter which checks currenttool

	void updatedrawing(); // for saving the image 

	void handlevent(const sf::Event& ev, sf::RenderWindow& win); // main operational window

	void push_undo_image() // calling the function to pushin the stuff back NOTE: Do not call display here ever
	{
		if (!undobuffer.empty() && undobuffer.size() >= undosize)
		{
			undobuffer.erase(undobuffer.begin());
		}
		this->canvasimage = this->newcanvas.toImage();
		undobuffer.push_back(this->canvasimage);
		
	}

	void initToolbarUI(); // an init function for toolbar textures

	sf::Color HSVtoRGB(float hue, float saturation, float val)
	{
		float c = val * saturation; // Chroma = how colorful is the value
		float x = c * (1.0f - std::abs(fmod(hue / 60.0f, 2.0f) - 1.0f)); // determines the bleeding of secondary color into the current one
		float m = val - c; // brightness of the color
		float r = 0, g = 0, b = 0;

		if (hue >= 0 && hue < 60) // RED dominant Green rizing Blue dead
		{
			r = c; g = x; b = 0;
		}
		else if (hue >= 60 && hue < 120) // GREEN dominant Red rizing Blue dead
		{
			r = x; g = c; b = 0;
		}
		else if (hue >= 120 && hue < 180) // GREEN dominant Blue rizing Red dead
		{
			r = 0; g = c; b = x;
		}
		else if (hue >= 180 && hue < 240) // BLUE dominant Green is falling Red dead
		{
			r = 0; g = x; b = c;
		}
		else if (hue >= 240 && hue < 300) // BLUE dominant Red rizing Green dead
		{
			r = x; g = 0; b = c;
		}
		else //	Red is dominant blue is falling Green dead
		{
			r = c; g = 0; b = x;
		}

		return sf::Color(static_cast<uint8_t>((r + m) * 255),
			static_cast<uint8_t>((g + m) * 255),
			static_cast<uint8_t>((b + m) * 255));
		// takes each color channel, adds the brightness and multiplies the whole thing by 255
	}
	void menuRESIZEevent(const sf::RenderWindow& win) // basically a WINDOW resize event(it's done incorrectly for paintermenu still)
	{
		unsigned int width = win.getSize().x;
		unsigned int height = win.getSize().y;
		if (width < 1040)
		{
			if (this->currentstate == menustate::mainmenu)
			{
				this->newfile.setPosition({ 35.f, 55.f });
			}
			if (this->currentstate == menustate::newfilemenu)
			{
				// copied from initUI in nmenu
				// basically saying they won't be affected at all
				float centerX = width / 2;
				float centerY = win.getSize().y / 2;
				sf::Vector2f center({ centerX, centerY });
				this->canvasPARAMSFROMNEWFILEMENU.dPanel.setPosition(center - (this->canvasPARAMSFROMNEWFILEMENU.dPanel.getSize() * 0.5f));

				this->canvasPARAMSFROMNEWFILEMENU.titletext.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 20.0f, 15.0f }));

				this->canvasPARAMSFROMNEWFILEMENU.WidthLabel.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 20.0f, 65.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.boxWidth.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 100.0f, 62.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.newWidth.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 105.0f, 65.0f }));

				this->canvasPARAMSFROMNEWFILEMENU.HeightLabel.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 20.0f, 110.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.boxHeight.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 100.0f, 107.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.newHeight.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 105.0f, 110.0f }));


				this->canvasPARAMSFROMNEWFILEMENU.bgLabel.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 20.0f, 160.0f }));

				this->canvasPARAMSFROMNEWFILEMENU.WhiteBgButton.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 20.0f, 190.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.WhiteBgText.setPosition(this->canvasPARAMSFROMNEWFILEMENU.WhiteBgButton.getPosition() + sf::Vector2f({ 35.0f, 5.0f }));

				this->canvasPARAMSFROMNEWFILEMENU.TranspBgButton.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 160.0f, 190.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.TranspBgText.setPosition(this->canvasPARAMSFROMNEWFILEMENU.TranspBgButton.getPosition() + sf::Vector2f({ 18.0f, 5.0f }));

				this->canvasPARAMSFROMNEWFILEMENU.Leave.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 170.0f, 300.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.LEAVE.setPosition(this->canvasPARAMSFROMNEWFILEMENU.Leave.getPosition() + sf::Vector2f({ 18.0f, 5.0f }));

				this->canvasPARAMSFROMNEWFILEMENU.Ok.setPosition(this->canvasPARAMSFROMNEWFILEMENU.dPanel.getPosition() + sf::Vector2f({ 260.0f, 300.0f }));
				this->canvasPARAMSFROMNEWFILEMENU.OK.setPosition(this->canvasPARAMSFROMNEWFILEMENU.Ok.getPosition() + sf::Vector2f({ 28.0f, 5.0f }));
			}
		}
		else
		{
			this->newfile.setPosition({ 35.f, 55.f });
			
			this->savefile.setPosition({ 35.f, 55.f });

			this->flush.setPosition({ 35.f, 95.f });

			this->currentColorText.setPosition({ 555.0f, 19.0f });

			this->currentColorBlock.setPosition({ 670.0f, 20.0f });

			this->currentToolText.setPosition({ 700.0f, 23.0f });

			this->currentToolBlock.setPosition({ 806.0f, 19.0f });

			this->currentStampText.setPosition({ 840.0f, 25.0f });

			this->stampD.setPosition({ 1002.0f, 34.0f });

			this->toolblock[0].setPosition({ 35.0f, 500.0f });

			this->toolblock[1].setPosition(this->toolblock[0].getPosition() + sf::Vector2f{ 35.0f, 0.0f });

			this->toolblock[2].setPosition(this->toolblock[1].getPosition() + sf::Vector2f{ 35.0f, 0.0f });

			this->toolblock[3].setPosition(this->toolblock[2].getPosition() + sf::Vector2f{ 35.0f, 0.0f });

			this->colorspecterblock.setPosition({ 30.0f, 550.0f }); // fked up the placement, was guessin
			
			// Sizes for Top and Side we leave here because in case we want to minimize the window again to let it deal with itself

			this->topBar.setSize({ static_cast<float>(width), 50.0f }); // adjusting the top-side bars to the sizes of resized window(doesn't work in minimize thing)
			this->topBar.setPosition({ 0.0f,0.0f }); // TOP LEFT
			this->topBar.setFillColor(sf::Color(45, 45, 48)); // Dark grey

			this->sidePanel.setSize({ 200.0f, static_cast<float>(height - 50)});
			this->sidePanel.setPosition({ 0.0f, 50.0f });
			this->sidePanel.setFillColor({ 30,30,30 });

			sf::Vector2f canvasSize = this->newcanvas.getSize();

			float availableWidth = static_cast<float>(width) - 200.0f;
			float availableHeight = static_cast<float>(height) - 50.0f;

			float canvasPosX = 200.0f + std::max(0.0f, (availableWidth - static_cast<float>(canvasSize.x)) / 2.0f);
			float canvasPosY = 50.0f + std::max(0.0f, (availableHeight - static_cast<float>(canvasSize.y)) / 2.0f);
			this->newcanvas.setPosition({ canvasPosX, canvasPosY }); // setting the position of the canvas
		}
	}
};