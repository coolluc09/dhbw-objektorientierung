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

//struct Plattform
//{
//	int linkes_ende;
//	int rechtes_end;
//	int hoehe_oben;
//	int hoehe_unten;
//};


class GameWindow : public Gosu::Window
{
public:
	Gosu::Image spieler;
	GameWindow()
		: Window(800, 600),
		spieler("rakete.png")
		
	{
		/*vector<Plattform> plattform_liste;
		plattform_liste.push_back({ 350,450,200,220 });
		plattform_liste.push_back({ 150,250,200,220 });
		plattform_liste.push_back({ 150,250,200,220 });*/




		//set_caption("Gosu Tutorial Game mit Git");


	}



	//Positionen des Spielers
	int position_spieler_x = 0;
	int position_spieler_y = 600;

	//Geschwindigkeit des Spielers
	int geschwindigkeit_spieler_x;
	int geschwindigkeit_spieler_y = 0;

	//Beschleunigung des Spielers
	int beschleunigung_spieler_x;
	int beschleunigung_spieler_y;

	//Hallo Welt



	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{
		/*graphics().draw_quad
		(
			350, 200, Gosu::Color::WHITE,
			450, 200, Gosu::Color::WHITE,
			350, 220, Gosu::Color::WHITE,
			450, 220, Gosu::Color::WHITE,
			0.0
		);*/

		spieler.draw_rot
		(
			position_spieler_x,position_spieler_y,0.0,1,0.5,1,1,1
		);
	}

	
	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		position_spieler_x = position_spieler_x + geschwindigkeit_spieler_x;
		position_spieler_y = position_spieler_y + geschwindigkeit_spieler_y;



		//Bewegungssteuerung

		if (input().down(Gosu::KB_RIGHT))					//nach Rechts laufen
		{
			geschwindigkeit_spieler_x = 5;
		}

		if (input().down(Gosu::KB_LEFT))					//nach Links laufen
		{
			geschwindigkeit_spieler_x = -5;
		}

		// stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen betätigt
		if ((!input().down(Gosu::KB_RIGHT) && !input().down(Gosu::KB_LEFT)) || (input().down(Gosu::KB_RIGHT) && input().down(Gosu::KB_LEFT)))	
		{
			geschwindigkeit_spieler_x = 0;
		}


		if (input().down(Gosu::KB_UP) && (beschleunigung_spieler_y == 0))					//Springen
		{
			geschwindigkeit_spieler_y = -20;
		}
		if (position_spieler_y >= 600 && !input().down(Gosu::KB_UP))
		{
			geschwindigkeit_spieler_y = 0;
			position_spieler_y = 600;
		}
		if (position_spieler_y < 600)
		{
			beschleunigung_spieler_y = 1;
			geschwindigkeit_spieler_y = geschwindigkeit_spieler_y + beschleunigung_spieler_y;
		}
		if (position_spieler_y >= 600)
		{
			beschleunigung_spieler_y = 0;
		}



		

		//#########################################################################################





	/*	if ((position_spieler_x > 450 || position_spieler_x < 350) && (position_spieler_y >= 200) && (position_spieler_y < 600))
		{
			position_spieler_y = position_spieler_y + 10;	
		}*/

		cout << position_spieler_x << "\t" << position_spieler_y << "\t" << geschwindigkeit_spieler_y << endl;
	
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
	
}