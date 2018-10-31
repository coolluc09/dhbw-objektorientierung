#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Planet.h"
#include "Vektor2d.h"

using namespace std;

// Simulationsgeschwindigkeit
const double DT = 100.0;

Gosu::Image spieler;


class GameWindow : public Gosu::Window
{
public:
	Gosu::Image spieler;
	GameWindow()
		: Window(800, 600),
		spieler("rakete.png")
		
	{
		//set_caption("Gosu Tutorial Game mit Git");

		//// Erzeuge Planeten
		//planets.push_back(Planet({ 200.0, 200.0 }, 0.1, "planet1.png"));
		//planets.push_back(Planet({ 600.0, 200.0 }, 0.1, "planet2.png"));
		//planets.push_back(Planet({ 400.0, 500.0 }, 0.1, "planet3.png"));
	}

	int position_spieler_x = 400;
	int position_spieler_y = 200;

	//Hallo Welt



	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		graphics().draw_quad
		(
			350, 200, Gosu::Color::WHITE,
			450, 200, Gosu::Color::WHITE,
			350, 220, Gosu::Color::WHITE,
			450, 220, Gosu::Color::WHITE,
			0.0
		);

		spieler.draw_rot
		(
			position_spieler_x,position_spieler_y,0.0,1,0.5,1,1,1
		);


	}

	
	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		if (input().down(Gosu::KB_RIGHT))
		{
			position_spieler_x = position_spieler_x + 3;
		}
		if (input().down(Gosu::KB_LEFT))
		{
			position_spieler_x = position_spieler_x - 3;
		}
	/*	if (input().down(Gosu::KB_UP))
		{
			int zwischenspeicher = position_spieler_y;
			while (position_spieler_y > zwischenspeicher - 10)
			{
				position_spieler_y = position_spieler_y - 2;
			}
			position_spieler_y = zwischenspeicher;
		}*/

		if ((position_spieler_x > 450 || position_spieler_x < 350) && (position_spieler_y >= 200) && (position_spieler_y < 600))
		{
			position_spieler_y = position_spieler_y + 10;	
		}

		cout << position_spieler_x << "\t" << position_spieler_y << endl;
	
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}