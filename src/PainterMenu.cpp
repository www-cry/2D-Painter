#include "PainterMenu.h"
#include "WindowCore.h"
#include <iostream>
#include <algorithm>

paintermenu::paintermenu() : menufont("MFONT.ttf"), newfile(menufont), savefile(menufont), flush(menufont), currentColorText(menufont), currentToolText(menufont), currentStampText(menufont), leave(menufont)
{

	pallete = { sf::Color::Black, sf::Color(128, 128, 128), sf::Color::Red, sf::Color::Yellow, sf::Color::Green,
	sf::Color::Cyan, sf::Color::Blue, sf::Color::Magenta, sf::Color::White, sf::Color(200,200,200), sf::Color(139, 69, 19), sf::Color(255, 192, 203) };
	// colors for the block

	colorswitchesblock.setFillColor(sf::Color::Transparent);
	colorswitchesblock.setOutlineColor(sf::Color(50, 50, 50));
	colorswitchesblock.setOutlineThickness(1.5f);
	colorswitchesblock.setPosition({ 20.0f, 220.0f });
	// init position of the block itself
	int columns = 5;
	float swSize = 24.0f;
	float spacing = 4.0f;
	float padding = 8.0f;
	float startX = 20.0f;
	float startY = 220.0f;
	int rows = (pallete.size() + columns - 1) / columns;

	float panelWidth = (columns * swSize) + ((columns - 1) * spacing) + (padding * 2);
	float panelHeight = (rows * swSize) + ((rows - 1) * spacing) + (padding * 2);
	colorswitchesblock.setSize(sf::Vector2f(panelWidth, panelHeight));
	colorswitches.clear();
	colorswitches.reserve(pallete.size());
	for (size_t i = 0; i < pallete.size(); i++)
	{
		int col = i % columns;
		int row = i / columns;

		float x = startX + padding + col * (swSize + spacing);
		float y = startY + padding + row * (swSize + spacing);

		sf::RectangleShape swat(sf::Vector2f(swSize, swSize));
		swat.setPosition(sf::Vector2f{ x, y });
		swat.setFillColor(pallete[i]);
		swat.setOutlineColor(sf::Color::White);
		swat.setOutlineThickness(1.5f);

		colorswitches.push_back(swat);
	}
	 // filling the pallete
	this->menufont.openFromFile("MFONT.ttf");
	if (this->canvas.resize({ 800u, 600u }))
	{
		this->canvas.clear(sf::Color::White);
		this->canvas.display();
		this->canvasimage = this->canvas.getTexture().copyToImage();
	}
	 // in case of standart canvas

	
	this->newfile.setString("New file");
	this->newfile.setFillColor(sf::Color::White);
	this->newfile.setCharacterSize(18);
	this->newfile.setPosition({ 35.f, 55.f });

	this->leave.setString("Leave");
	this->leave.setFillColor(sf::Color::White);
	this->leave.setCharacterSize(18);
	this->leave.setPosition({ 35.f, 95.f });

	this->savefile.setString("Save file");
	this->savefile.setFillColor(sf::Color::White);
	this->savefile.setCharacterSize(22);
	this->savefile.setPosition({ 35.f, 55.f });

	this->flush.setString("Flush");
	this->flush.setFillColor(sf::Color::White);
	this->flush.setCharacterSize(18);
	this->flush.setPosition({ 35.f, 95.f });

	this->currentColorText.setString("Current color: ");
	this->currentColorText.setFillColor(sf::Color::White);
	this->currentColorText.setCharacterSize(16);
	this->currentColorText.setPosition({ 555.0f, 19.0f });

	this->currentColorBlock.setFillColor(sf::Color::Black);
	this->currentColorBlock.setSize({ 23.0f, 23.0f });
	this->currentColorBlock.setPosition({ 670.0f, 20.0f });

	this->currentToolBlock.setSize({ 23.0f, 23.0f });
	this->currentToolBlock.setPosition({ 806.0f, 19.0f });

	this->toolsicons.resize(4); // vector error jumpscare
	if (!toolsicons[0].loadFromFile("ToolsImg/Pencil.png")) {};
	toolsicons[1].loadFromFile("ToolsImg/Brush.png");
	toolsicons[2].loadFromFile("ToolsImg/Eraser.png");
	toolsicons[3].loadFromFile("ToolsImg/Pallete.png");

	
	this->toolblock.resize(4);

	this->toolblock[0].setSize({ 32.0f, 32.0f });
	this->toolblock[0].setOutlineThickness(1.5f);
	this->toolblock[0].setOutlineColor(sf::Color::Red);
	this->toolblock[0].setPosition({35.0f, 500.0f}); // will add these later


	this->toolblock[1].setSize({ 32.0f, 32.0f });
	this->toolblock[1].setOutlineThickness(1.5f);
	this->toolblock[1].setOutlineColor(sf::Color::Red);
	this->toolblock[1].setPosition(this->toolblock[0].getPosition() + sf::Vector2f{35.0f, 0.0f}); // go + in x(iirc it's to the right), y axis is pointless to touch 

	this->toolblock[2].setSize({ 32.0f, 32.0f });
	this->toolblock[2].setOutlineThickness(1.5f);
	this->toolblock[2].setOutlineColor(sf::Color::Red);
	this->toolblock[2].setPosition(this->toolblock[1].getPosition() + sf::Vector2f{ 35.0f, 0.0f });

	this->toolblock[3].setSize({ 32.0f, 32.0f });
	this->toolblock[3].setOutlineThickness(1.5f);
	this->toolblock[3].setOutlineColor(sf::Color::Red);
	this->toolblock[3].setPosition(this->toolblock[2].getPosition() + sf::Vector2f{ 35.0f, 0.0f });

	// most of the UI is basically built above

	this->setupUI(); // this one basically sets topBar and sideBar positions

	this->initToolbarUI(); // get all tool textures 

	stamp.setRadius(3.0f);
	stampB.setRadius(5.0f);
	stampE.setRadius(2.0f);
	// stamp - pencil stampB - brush stampE - eraser
	stampD.setRadius(3.0f);
	// stampD - showcase stamp
	stampD.setFillColor(this->currentcolor);
	this->stampD.setPosition({ 1002.0f, 34.0f });

	this->colorspecterimage.resize(sf::Vector2u(120, 120), sf::Color::Black);

	for (unsigned int x = 0; x < 120; x++)
	{
		float h = (static_cast<float>(x) / 120.0f) * 360.0f;
		sf::Color color = HSVtoRGB(h, 1.0f, 1.0f);

		for (unsigned int y = 0; y < 120; y++)
		{
			this->colorspecterimage.setPixel(sf::Vector2u(x, y), color);
		}
	}

	this->colorspecterTexture.loadFromImage(this->colorspecterimage);

	this->colorspecterblock.setSize(sf::Vector2f(120.0f, 120.0f));
	this->colorspecterblock.setTexture(&colorspecterTexture);
	this->colorspecterblock.setPosition({ 35.0f, 550.0f });


	this->currentToolText.setCharacterSize(16);
	this->currentToolText.setPosition({ 700.0f, 23.0f });
	this->currentToolText.setString("Current tool: ");


	this->currentStampText.setCharacterSize(16);
	this->currentStampText.setPosition({840.0f, 25.0f});
	this->currentStampText.setString("Current stamp size: ");
}

void paintermenu::menuevents(sf::RenderWindow& win) // hover effect
{
	if (this->currentstate == menustate::mainmenu)
	{
			sf::Vector2f mpos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
			
			if (this->newfile.getGlobalBounds().contains(mpos))
			{
				this->newfile.setFillColor(sf::Color::Red);
				this->newfile.setOutlineColor(sf::Color::White);
			}
			else
			{
				this->newfile.setFillColor(sf::Color::White);
				this->newfile.setOutlineColor(sf::Color::White);
			}
			if (this->leave.getGlobalBounds().contains(mpos))
			{
				this->leave.setFillColor(sf::Color::Red);
				this->leave.setOutlineColor(sf::Color::White);
			}
			else
			{
				this->leave.setFillColor(sf::Color::White);
				this->leave.setOutlineColor(sf::Color::White);
			}
	}
	if (this->currentstate == paintermenu::menustate::paintmenu)
	{
		sf::Vector2f mpos = win.mapPixelToCoords(sf::Mouse::getPosition(win));
		if (this->flush.getGlobalBounds().contains(mpos))
		{
			this->flush.setFillColor(sf::Color::Red);
			this->flush.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->flush.setFillColor(sf::Color::White);
			this->flush.setOutlineColor(sf::Color::White);
		}
		if (this->savefile.getGlobalBounds().contains(mpos))
		{
			this->savefile.setFillColor(sf::Color::Red);
			this->savefile.setOutlineColor(sf::Color::White);
		}
		else
		{
			this->savefile.setFillColor(sf::Color::White);
			this->savefile.setOutlineColor(sf::Color::White);
		}
		
	}
}

void paintermenu::draw(sf::RenderWindow& win) // rendering function
{
	if (this->currentstate == menustate::mainmenu)
	{
		win.draw(this->newfile);
		win.draw(this->leave);
	}
	else if (this->currentstate == menustate::paintmenu)
	{
		win.clear(sf::Color(40, 40, 40));

		this->newcanvas.render(win);
		
		win.draw(this->topBar);
		win.draw(this->sidePanel);
		
		for (const auto& toolsic : this->toolblock)
		{
			win.draw(toolsic);
		}
		win.draw(this->savefile);
		win.draw(this->flush);
		win.draw(colorswitchesblock);

		for (const auto& a : colorswitches)
		{
			win.draw(a);
		}

		win.draw(this->currentColorText);
		win.draw(this->currentColorBlock);
		win.draw(this->currentToolBlock);
		win.draw(this->stampD);
		win.draw(this->colorspecterblock);
		win.draw(this->currentStampText);
		win.draw(this->currentToolText);
	}
}

void paintermenu::drawhandle(tools current, Canvas& canvas, sf::Vector2f p0, sf::Vector2f p1) // actual drawing which takes two coordinates and does the magic
{

	sf::Vector2f localp0 = p0 - canvas.getPosition();
	sf::Vector2f localp1 = p1 - canvas.getPosition();

	sf::RenderTexture& txt = canvas.getTexturefrom();

	sf::Vector2f dir = localp1 - localp0;
	float dist = std::hypot(dir.x, dir.y);
	// calculation of the direction and the distance
	switch (current)
	{
	case tools::pencil:
	{
		this->stamp.setOrigin({ this->stamp.getRadius(), this->stamp.getRadius() });
		this->stamp.setFillColor(this->currentcolor);
		if (dist == 0)
		{
			this->stamp.setPosition(localp0);
			txt.draw(this->stamp);
			break;
		}
		sf::Vector2f unitD = dir / dist; // unitD - unit Direction
		for (float d = 0; d <= dist; d += 1.0f)
		{
			this->stamp.setPosition(localp0 + (unitD * d));
			txt.draw(this->stamp);
		}
		break;
	}
	case tools::brush:
	{
		this->stampB.setOrigin({ this->stampB.getRadius(), this->stampB.getRadius()});
		this->stampB.setFillColor(this->currentcolor);

		if (dist == 0.0f)
		{
			this->stampB.setPosition(localp0);
			txt.draw(this->stampB);
			break;
		}

		
		sf::Vector2f unitD = dir / dist; 
		float currentRadiusOFBrush = this->stampB.getRadius();
		float step = std::max(0.5f, currentRadiusOFBrush * 0.5f);
		for (float d = 0.0f; d <= dist; d += step)
		{
			this->stampB.setPosition(localp0 + (unitD * d));
			txt.draw(this->stampB);
		}
		break;
	}
	case tools::eraser:
	{
		float currRadiusOfEraser = this->stampE.getRadius();
		this->stampE.setOrigin({ currRadiusOfEraser, currRadiusOfEraser });
		
		this->stampE.setFillColor(sf::Color::White); // in reality I planned to put here a switch case in terms of different backgrounds but prolly I'll just leave for white

		if (dist == 0.f)
		{
			this->stampE.setPosition(localp0);
			txt.draw(this->stampE);
			break;
		} 

		sf::Vector2f unitD = dir / dist;
		for (float x = 0.0f; x <= dist; x += 1.0f)
		{
			stampE.setPosition(localp0 + (unitD * x));
			txt.draw(stampE);
		}
		break;
	}
	// here main idea of drawing is realised through just constant drawing of stamps
	case tools::pallete:
	{
		txt.display();
		canvasimage = txt.getTexture().copyToImage(); // quick update of sf::Image
		if (localp0.x >= 0.0f && localp0.y >= 0.0f) // safe check
		{
		
		 sf::Vector2u imgSize = canvasimage.getSize();
		if (localp0.x < imgSize.x && localp0.y < imgSize.y) // another safe check
		{
			unsigned int x0 = static_cast<unsigned int>(localp0.x);
			unsigned int y0 = static_cast<unsigned int>(localp0.y); // converting sf::Vector2FLOAT to integers since for getPixel we need vector2u numbers
			this->currentcolor = canvasimage.getPixel(sf::Vector2u(x0,y0));
			this->currentColorBlock.setFillColor(this->currentcolor);
		}
		}
		break;
	}
	// here we just do colorpicking by sfml's function getPixel
	}
	txt.display();
}


void paintermenu::handlevent(const sf::Event& ev, sf::RenderWindow& win) // menu click events that also calls drawing functions and ctrl s ctrl z stuff
{
	
	sf::Vector2f mpos;
	if (auto mpressed = ev.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mpressed->button == sf::Mouse::Button::Left)
		{
			mpos = static_cast<sf::Vector2f>(win.mapPixelToCoords(mpressed->position));
			bool clickedOnUi = false;
			for (auto& sw : colorswitches)
			{
				if (sw.getGlobalBounds().contains(mpos))
				{
					this->currentcolor = sw.getFillColor();
					this->currentColorBlock.setFillColor(this->currentcolor);
					this->stampD.setFillColor(this->currentcolor);
					clickedOnUi = true;
					break;
				}
			}
			// picking a color from color pallete
			for (size_t i = 0; i < toolblock.size(); i++)
			{
				if (this->toolblock[i].getGlobalBounds().contains(mpos))
				{
					this->currenttool = static_cast<tools>(i); // 0 - pencil, 1 - brush, 2 - eraser, 3 - pallete
					this->currentToolBlock.setTexture(this->toolblock[i].getTexture()); // UI current tool block
					switch (this->currenttool)
					{
					case tools::pencil:
					{
						this->stampD.setRadius(this->stamp.getRadius());
						break;
					}
					case tools::brush:
					{
						this->stampD.setRadius(this->stampB.getRadius());
						break;
					}
					case tools::eraser:
					{
						this->stampD.setRadius(this->stampE.getRadius());
						break;
					}
					default: break;
					}
					float rad = this->stampD.getRadius();
					this->stampD.setOrigin({ rad, rad });

					clickedOnUi = true;
					break;
				}
			}
			// changin current tool
			if (this->flush.getGlobalBounds().contains(mpos))
			{
				this->push_undo_image();
				this->newcanvas.canvasTexture.clear(this->canvasPARAMSFROMNEWFILEMENU.SelectedBgColor); // or smt like this->newcanvas.bcolor or wherever that one was
				this->newcanvas.canvasTexture.display();
				this->newcanvas.canvasSprite.setTexture(this->newcanvas.canvasTexture.getTexture(), true);
				this->canvasimage = this->newcanvas.toImage();
				return;
			}
			// flushing function
			if (this->savefile.getGlobalBounds().contains(mpos))
			{
				this->canvasimage.saveToFile("Saved.jpg");
			}
			if (this->colorspecterblock.getGlobalBounds().contains(mpos))
			{
				sf::Vector2f colorspecterlocalpos = mpos - this->colorspecterblock.getPosition(); // get real relevant position
				sf::Vector2u pixelatedCoords(static_cast<unsigned int>(std::clamp(colorspecterlocalpos.x, 0.0f, 119.0f)),
					static_cast<unsigned int>(std::clamp(colorspecterlocalpos.y, 0.0f, 119.0f)));
				this->currentcolor = this->colorspecterimage.getPixel(sf::Vector2u(pixelatedCoords));
				this->currentColorBlock.setFillColor(this->currentcolor);
			}
			// colorpicking from color spectrum block
			if(!clickedOnUi)
			{
				if (this->newcanvas.getBounds().contains(mpos))
				{
					if (!this->isdrawing)
					{
						this->isdrawing = true;
						this->push_undo_image();
					}
					
					this->prevpos = static_cast<sf::Vector2i>(mpos); // p0 in future
					sf::Vector2f drawpos = static_cast<sf::Vector2f>(this->prevpos);
					this->drawhandle(this->currenttool, this->newcanvas, drawpos, drawpos); 
				}

			}
			// drawing if the coords match in newcanvas's area
		}
	}
	if (auto mmoved = ev.getIf<sf::Event::MouseMoved>() )
	{
		if (this->isdrawing)
		{
			sf::Vector2f newcurrpos = win.mapPixelToCoords(mmoved->position);
			sf::Vector2f newprevpos = static_cast<sf::Vector2f>(this->prevpos);
			if (this->newcanvas.getBounds().contains(newcurrpos))
			{
				this->drawhandle(this->currenttool, this->newcanvas, newprevpos, newcurrpos);
				this->prevpos = static_cast<sf::Vector2i>(newcurrpos);
			}
		}
		// continuous drawing
	}
	if (auto mreleased = ev.getIf<sf::Event::MouseButtonReleased>())
	{
		if (mreleased->button == sf::Mouse::Button::Left && this->isdrawing)
		{
			this->updatedrawing();
			this->isdrawing = false;
			
		}
	}
	if (const auto* combopressed = ev.getIf<sf::Event::KeyPressed>())
	{
		if (combopressed->code == sf::Keyboard::Key::Z && combopressed->control) // CTRL + Z
		{
			if (!undobuffer.empty())
			{
				sf::Image tmpImg = undobuffer.back();
				undobuffer.pop_back(); // destroy the last element
				this->newcanvas.restoreImg(tmpImg);
				this->canvasimage = tmpImg;
				this->newcanvas.canvasSprite.setTexture(this->newcanvas.canvasTexture.getTexture(), true);
			}
		}
		if (combopressed->code == sf::Keyboard::Key::S && (combopressed->control)) // CTRL + S
		{
			this->updatedrawing();
			this->canvasimage.saveToFile("Saved");
		}
	}
	if (const auto kpressed = ev.getIf<sf::Event::KeyPressed>())
	{
		sf::CircleShape* currentStamp = nullptr;
		float maxRad = 64.0f;
		float minRad = 1.0f;

		if (this->currenttool == paintermenu::tools::pencil)
		{
			currentStamp = &this->stamp;
			minRad = 0.5f;
			maxRad = 20.0f;
		}
		else if (this->currenttool == paintermenu::tools::brush)
		{
			currentStamp = &this->stampB;
			minRad = 1.0f;
			maxRad = 64.0f;
		}
		else if (this->currenttool == paintermenu::tools::eraser)
		{
			currentStamp = &this->stampE;
			minRad = 2.0f;
			maxRad = 64.0f;
		}
		if(currentStamp != nullptr)
		{
			float rad = currentStamp->getRadius();
			if (kpressed->code == sf::Keyboard::Key::Add)
			{
				rad = std::min(maxRad, rad + 0.5f);
			}
			else if (kpressed->code == sf::Keyboard::Key::Subtract)
			{
				rad = std::max(minRad, rad - 0.5f);
			}

			currentStamp->setRadius(rad);
			currentStamp->setOrigin({ rad, rad });
			this->stampD.setRadius(currentStamp->getRadius());
			this->stampD.setFillColor(this->currentcolor);
		}
		// changin the sizes of stamps (works rly strangely, maybe I should've just increased it by 2-4 times)
	}
	
}

void paintermenu::initToolbarUI()
{
	this->tooltxt.resize(this->toolsicons.size());

	for (size_t i = 0; i < toolsicons.size(); i++)
	{
		this->tooltxt[i].loadFromImage(this->toolsicons[i]);
		this->tooltxt[i].setSmooth(true);
		this->toolblock[i].setTexture(&this->tooltxt[i], false);
	}
	if (!this->tooltxt.empty())
    {
        this->currentToolBlock.setTexture(&this->tooltxt[0]); // Pencil texture
    }
}

void paintermenu::setupUI() // setting the more OUTER stuff
{
	topBar.setSize({ 1280.0f, 50.0f });
	topBar.setPosition({ 0.0f,0.0f }); // TOP LEFT
	topBar.setFillColor(sf::Color(45, 45, 48)); // Dark grey

	sidePanel.setSize({ 200.0f, 670.0f });
	sidePanel.setPosition({ 0.0f, 50.0f });
	sidePanel.setFillColor({ 30,30,30 });

}


void paintermenu::updatedrawing()
{
	this->canvasimage = this->newcanvas.toImage(); 
} // saving the image