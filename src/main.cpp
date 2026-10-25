#include "WindowCore.h"
int main()
{
	WindowCore winda;

	while (winda.getwindowisopen() == 1) // while the window is open basically
	{
		winda.eventpoll();
		winda.render();
		winda.update();
		
	}
}