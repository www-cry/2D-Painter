#include "NewFileMenu.h"
#include <iostream>

newfilemenu::newfilemenu() : newfilemenufont("MFONT.ttf"), titletext(newfilemenufont), WidthLabel(newfilemenufont), HeightLabel(newfilemenufont), bgLabel(newfilemenufont), newWidth(newfilemenufont), newHeight(newfilemenufont), OK(newfilemenufont), LEAVE(newfilemenufont), WhiteBgText(newfilemenufont), TranspBgText(newfilemenufont)
{
	if (!newfilemenufont.openFromFile("MFONT.ttf"))
	{
		std::cerr << "Failed to load font \n";
	}

	// coloring the buttons and etc
	dPanel.setSize({ 360.0f, 360.0f });
	dPanel.setFillColor(sf::Color(45,45,50)); // grey
	dPanel.setOutlineColor(sf::Color(80, 80, 80));
	dPanel.setOutlineThickness(2.0f);
	// dialogue panel

	bgLabel.setString("Background: ");
	bgLabel.setCharacterSize(16);
	bgLabel.setFillColor(sf::Color::White);
	// background label

	WhiteBgButton.setSize({ 120.0f, 30.0f });
	if (this->SelectedBgColor == sf::Color::White)
	{
		WhiteBgButton.setFillColor(sf::Color(100, 100, 100)); // Active state
	}
	WhiteBgButton.setOutlineThickness(2.0f);
	WhiteBgButton.setOutlineColor(sf::Color::White);

	WhiteBgText.setString("White");
	WhiteBgText.setCharacterSize(14);
	WhiteBgText.setFillColor(sf::Color::Black);


	TranspBgButton.setSize({ 120.0f, 30.0f });
	if (this->SelectedBgColor == sf::Color(0, 0, 0, 0))
	{
		TranspBgButton.setFillColor(sf::Color(100, 100, 100)); // Active state
	}
	TranspBgButton.setOutlineThickness(2.0f);
	TranspBgButton.setOutlineColor(sf::Color::White);

	TranspBgText.setString("Transparent");
	TranspBgText.setCharacterSize(14);
	TranspBgText.setFillColor(sf::Color::Black);


	titletext.setCharacterSize(16);
	titletext.setString("New canvas");
	titletext.setFillColor(sf::Color::White);


	WidthLabel.setString("Width: ");
	WidthLabel.setCharacterSize(16);
	WidthLabel.setFillColor(sf::Color::White);

	HeightLabel.setString("Height: ");
	HeightLabel.setCharacterSize(16);
	HeightLabel.setFillColor(sf::Color::White);
	// Labels


	boxWidth.setSize({ 100.0f, 25.0f });
	boxWidth.setFillColor(sf::Color(25, 25, 25));
	newWidth.setCharacterSize(14);
	newWidth.setFillColor(sf::Color::White);

	boxHeight.setSize({ 100.0f, 25.0f });
	boxHeight.setFillColor(sf::Color(25, 25, 25));
	newHeight.setCharacterSize(14);
	newHeight.setFillColor(sf::Color::White);
	// Input stuff

	// Ok - RectangleShape(box) OK - Text --- Same applies to Leave LEAVE
	Ok.setSize({ 80.0f, 30.0f });
	Ok.setFillColor(sf::Color::Blue);
	OK.setString("Ok");
	OK.setCharacterSize(14);
	OK.setFillColor(sf::Color::White);

	Leave.setSize({ 80.0f, 30.0f });
	Leave.setFillColor(sf::Color::Blue);
	LEAVE.setString("Leave");
	LEAVE.setCharacterSize(14);
	LEAVE.setFillColor(sf::Color::White);
}





void newfilemenu::UISetup(sf::Vector2f winsize)
{
	this->newcanvasparam.declined = false;
	this->newcanvasparam.confirm = false;
	// center the newfilemenu in the center
	sf::Vector2f center = winsize * 0.5f;

	dPanel.setPosition(center - sf::Vector2f((dPanel.getSize() * 0.5f)));

	titletext.setPosition(dPanel.getPosition() + sf::Vector2f({ 20.0f, 15.0f }));

	WidthLabel.setPosition(dPanel.getPosition() + sf::Vector2f({ 20.0f, 65.0f }));
	boxWidth.setPosition(dPanel.getPosition() + sf::Vector2f({ 100.0f, 62.0f }));
	newWidth.setPosition(dPanel.getPosition() + sf::Vector2f({ 105.0f, 65.0f }));

	HeightLabel.setPosition(dPanel.getPosition() + sf::Vector2f({ 20.0f, 110.0f }));
	boxHeight.setPosition(dPanel.getPosition() + sf::Vector2f({ 100.0f, 107.0f }));
	newHeight.setPosition(dPanel.getPosition() + sf::Vector2f({ 105.0f, 110.0f }));


	bgLabel.setPosition(dPanel.getPosition() + sf::Vector2f({ 20.0f, 160.0f }));

	WhiteBgButton.setPosition(dPanel.getPosition() + sf::Vector2f({ 20.0f, 190.0f }));
	WhiteBgText.setPosition(WhiteBgButton.getPosition() + sf::Vector2f({ 35.0f, 5.0f }));

	TranspBgButton.setPosition(dPanel.getPosition() + sf::Vector2f({ 160.0f, 190.0f }));
	TranspBgText.setPosition(TranspBgButton.getPosition() + sf::Vector2f({ 18.0f, 5.0f }));

	Leave.setPosition(dPanel.getPosition() + sf::Vector2f({ 170.0f, 300.0f }));
	LEAVE.setPosition(Leave.getPosition() + sf::Vector2f({18.0f, 5.0f}));

	Ok.setPosition(dPanel.getPosition() + sf::Vector2f({260.0f, 300.0f}));
	OK.setPosition(Ok.getPosition() + sf::Vector2f({ 28.0f, 5.0f }));
}

void newfilemenu::drawnewfilemenu(sf::RenderWindow& win) // rendering the init window of new canvas
{
	win.draw(dPanel);
	win.draw(titletext);
	win.draw(WidthLabel);
	win.draw(HeightLabel);
	win.draw(boxHeight);
	win.draw(newHeight);
	win.draw(boxWidth);
	win.draw(newWidth);
	win.draw(Leave);
	win.draw(LEAVE);
	win.draw(Ok);
	win.draw(OK);
	win.draw(bgLabel);
	win.draw(WhiteBgButton);
	win.draw(WhiteBgText);
	win.draw(TranspBgButton);
	win.draw(TranspBgText);
}

canvassettings newfilemenu::updatenewfilemenu(sf::RenderWindow& win, const sf::Event& ev)
{
	if (auto* mpressed = ev.getIf<sf::Event::MouseButtonPressed>())
	{
		if (mpressed->button == sf::Mouse::Button::Left)
		{
			sf::Vector2f mpos = win.mapPixelToCoords(mpressed->position);

			if (Leave.getGlobalBounds().contains(mpos))
			{
				this->newcanvasparam.declined = true;
				this->newcanvasparam.confirm = false;
			}
			else if (Ok.getGlobalBounds().contains(mpos))
			{
				this->newcanvasparam.confirm = true; 
				this->newcanvasparam.declined = false;
				unsigned long parsedWidth = 0;
				unsigned long parsedHeight = 0;

				if (!widthText.empty())
				{
					try{ parsedWidth = std::stoul(widthText); } // if not possible to grab a value from widthText set the parsedWidth to 800 as default
					catch (...) { parsedWidth = 800; }
					
				}
				
				if (!heightText.empty())
				{
					try { parsedHeight = std::stoul(heightText); } // same as with width
					catch (...) { parsedHeight = 600; }
				}
				

				// basically since it won't rly matter if we put higer or lower then might as well just use 1 as a starting point since the size can't be negative
				this->newcanvasparam.Bwidth = static_cast<unsigned int>(std::max(1UL, parsedWidth));
				this->newcanvasparam.Bheight = static_cast<unsigned int>(std::max(1UL, parsedHeight));
				this->newcanvasparam.Bcolor = this->SelectedBgColor; // setting the Bcolor of the canvas
				this->newfilewinisopen = false;
			}
			else if (boxWidth.getGlobalBounds().contains(mpos)) // hover effect on click + typing the stuff in
			{
				currmod = typinmode::Width;
				newWidth.setFillColor(sf::Color::Red);
				newHeight.setFillColor(sf::Color::White);
			}
			else if (boxHeight.getGlobalBounds().contains(mpos))
			{
				currmod = typinmode::Height;
				newHeight.setFillColor(sf::Color::Red);
				newWidth.setFillColor(sf::Color::White);
			}
			else
			{
				currmod = typinmode::None;
				newWidth.setFillColor(sf::Color::White);
				newHeight.setFillColor(sf::Color::White);
			}
			if (WhiteBgButton.getGlobalBounds().contains(mpos))
			{
				this->SelectedBgColor = sf::Color::White;
				WhiteBgButton.setFillColor(sf::Color(180, 180, 180));
				TranspBgButton.setFillColor(sf::Color(100, 100, 100));
			}
			else if (TranspBgButton.getGlobalBounds().contains(mpos))
			{
				this->SelectedBgColor = sf::Color::Transparent;
				TranspBgButton.setFillColor(sf::Color(180, 180, 180));
				WhiteBgButton.setFillColor(sf::Color(100, 100, 100));
			}
		}
	}
	

	if (auto* textenter = ev.getIf<sf::Event::TextEntered>()) // text event for getting custom width and height
	{
		if (currmod != typinmode::None)
		{
			uint32_t unicode = static_cast<int>(textenter->unicode);
			std::string& currentSTRING = (currmod == typinmode::Width) ? widthText : heightText; // a check for if ur typin in width the string is widthText if false it's heightText
			if (unicode == '\b' && !currentSTRING.empty()) // backspace
			{
				currentSTRING.pop_back();
			}
			else if (unicode < 128 && std::isdigit(static_cast<char>(unicode)) && currentSTRING.size() < 4) // adding up a character(max 4 digit length) 
			{
				currentSTRING += static_cast<char>(unicode);
			}
			newWidth.setString(widthText);
			newHeight.setString(heightText);
		}
		
	}
	return this->newcanvasparam;
}