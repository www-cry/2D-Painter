#include "WindowCore.h"
#include "PainterMenu.h"
#include <optional>
#include <iostream>
void WindowCore::initint()
{
	this->win = nullptr; // setting the window pointer to null at first
}

void WindowCore::initw()
{
	this->win = new sf::RenderWindow(sf::VideoMode({ 800, 600 }), "Test Window", sf::Style::Close | sf::Style::Resize); // base settings of the window
	this->win->setFramerateLimit(60);
}

WindowCore::~WindowCore()
{
	delete this->win; // destroying the win pointer
}

void WindowCore::update() // for updating the windows by the states
{
		this->eventpoll();
		this->menu.menuevents(*this->win);
		// if (this->menu.currentstate == paintermenu::menustate::paintmenu)
		// {
		// 	this->menu.updatedrawing();
		// }	// old bone for drawing
		bool stopif = false;
}

void WindowCore::render()
{
	this->win->clear(sf::Color::Black);
	this->menu.draw(*this->win);
	if (this->menu.currentstate == paintermenu::menustate::newfilemenu)
	{
		this->nmenu.drawnewfilemenu(*this->win);
	}
	this->win->display();
}

void WindowCore::eventpoll()
{
	while (const std::optional ev = this->win->pollEvent()) // the standart event loop from sfml
	{
		if (auto* resize = ev->getIf<sf::Event::Resized>())
		{
			unsigned int newWidth = resize->size.x;
			unsigned int finalWidth= 800;
			unsigned int finalHeight = 600;

			if (this->menu.currentstate == paintermenu::menustate::paintmenu)
			{
				if (newWidth < 1440) // changin minimum window size for paintermenu
				{
					finalWidth = 1280;
					finalHeight = 720;
				}
				else
				{
					finalWidth = 1600;
					finalHeight = 900;
				}
			}
			else
			{
				finalWidth = 800;
				finalHeight = 600;
			}

			if (finalWidth != resize->size.x || finalHeight != resize->size.y)
			{
				this->win->setSize({ finalWidth, finalHeight });
				sf::View view(sf::FloatRect({ 0.0f, 0.0f }, { static_cast<float>(finalWidth), static_cast<float>(finalHeight) }));
				this->win->setView(view);
				// right here view is basically camera pos
			}
			this->menu.menuRESIZEevent(*this->win); // for resetting the positions on resize
		}
		if (ev->is<sf::Event::Closed>())
		{
			this->win->close(); // usual close event
		}
		if (this->menu.currentstate == paintermenu::menustate::newfilemenu)
		{
			this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam = this->nmenu.updatenewfilemenu(*this->win, *ev);

			if (this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.confirm) // equialent of pressing Ok button in NMenu
			{
				sf::Vector2u canvasSize = this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.getSizeofCanvas();
				const float toolbarWidth = 60.0f;
				const float toolbarHeight = 40.0f;

				unsigned int newWinWidth = canvasSize.x + static_cast<unsigned int>(toolbarWidth);
				unsigned int newWinHeight = canvasSize.y + static_cast<unsigned int>(toolbarHeight);

				newWinWidth = std::max(newWinWidth, 800u);
				newWinHeight = std::max(newWinHeight, 600u);

				this->win->setSize(sf::Vector2u(newWinWidth, newWinHeight));
				this->win->setView(sf::View(sf::FloatRect({ 0.0f,0.0f }, { static_cast<float>(newWinWidth), static_cast<float>(newWinHeight) })));

				// parsing values from newfilemenu
				this->menu.newcanvas.resize(this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.getSizeofCanvas(), this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.Bcolor, &this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.currimg);
				this->menu.canvasPARAMSFROMNEWFILEMENU.SelectedBgColor = this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.Bcolor; // getting Bcolor for FLUSH button in paintermenu
				this->menu.newcanvas.CanvasBgForFunc = this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.Bcolor; // getting Bcolor for Ctrl Z combination in paintermenu

				this->menu.newcanvas.toImage(); // saving in img

				nmenu.setNmenuState(false);
				this->menu.currentstate = paintermenu::menustate::paintmenu;
			}
		
			else if (this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.declined)
			{
				nmenu.setNmenuState(false);
				this->menu.currentstate = paintermenu::menustate::mainmenu; // if declined just flip the bool
			}
			return;
		}
		else if (this->menu.currentstate == paintermenu::menustate::mainmenu)
		{
			// main menu shenanigans
			if (const auto* mpressed = ev->getIf<sf::Event::MouseButtonPressed>()) // on button pressed we create an auto to deduce the coordinates of the click for the menu interactions
			{
				if (mpressed->button == sf::Mouse::Button::Left)
				{
					sf::Vector2f mousepos = this->win->mapPixelToCoords(sf::Mouse::getPosition(*this->win));
					if (this->menu.newfile.getGlobalBounds().contains(mousepos))
					{
						this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.declined = false; 
						this->menu.canvasPARAMSFROMNEWFILEMENU.newcanvasparam.confirm = false; // pre-resetting the flags for the moment if user leaves the nmenu
						sf::Vector2f fornew = this->win->getView().getSize(); // here we set getView first cuz getSize was still reading default size
						this->nmenu.UISetup(fornew); // positions in dialogue box for newcanvas creation
						this->menu.currentstate = paintermenu::menustate::newfilemenu;
					}
					if(this->menu.leave.getGlobalBounds().contains(mousepos))
					{
						this->win->close();
					}
				}
			}
		}
		if (this->menu.currentstate == paintermenu::menustate::paintmenu)
		{
			this->menu.handlevent(*ev, *this->win); // menu event handler( selecting colors, tools etc)
		}
		
	}
}

bool WindowCore::getwindowisopen()
{
	if (this->win->isOpen()) // safe boolean for main loop
	{
		return true;
	}
}

WindowCore::WindowCore() // constructor
{
	this->initint();
	this->initw();
	this->render();
	this->update();
}



