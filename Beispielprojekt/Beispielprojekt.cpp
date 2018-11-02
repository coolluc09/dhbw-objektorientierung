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


//struct Plattform
//{
//	int linkes_ende;
//	int rechtes_end;
//	int hoehe_oben;
//	int hoehe_unten;
//};


int spieler_min_abstand_x = 10;
int spieler_min_abstand_y = 42;
int lebensanzahl_anfang = 3;
int leben_anfang = 100;
int sprunghoehe = 325;

struct Spieler
{
	//Name des Spielers
	string name;
	//Positionen des Spielers
	int position_spieler_x;
	int position_spieler_y;
	//Geschwindigkeit des Spielers
	int geschwindigkeit_spieler_x;
	int geschwindigkeit_spieler_y;
	//Beschleunigung des Spielers
	int beschleunigung_spieler_x;
	int beschleunigung_spieler_y;
	//Blickrichtung des Spielers
	bool blick_spieler_rechts;
	bool blick_spieler_links;
	//Leben und Lebensanzahl der Spieler
	int leben_spieler = leben_anfang;
	int lebensanzahl = lebensanzahl_anfang;

	/*{
	public:
		void set_name(const string& n)
		{
			this->name = n;
		}
		string get_name()
		{
			return this->name;
		}
		void set_position_spieler_x(int p)
		{
			this->position_spieler_x = p;
		}
		int get_position_spieler_x()
		{
			return this->position_spieler_x;
		}
		void set_position_spieler_y(int p)
		{
			this->position_spieler_y = p;
		}
		int get_position_spieler_y()
		{
			return this->position_spieler_y;
		}
		void set_beschleunigung_spieler_x(int b)
		{
			this->beschleunigung_spieler_x = b;
		}
		int get_beschleunigung_spieler_x()
		{
			return this->beschleunigung_spieler_x;
		}
		void set_beschleunigung_spieler_y(int b)
		{
			this->beschleunigung_spieler_y = b;
		}
		int get_beschleunigung_spieler_y()
		{
			return this->beschleunigung_spieler_y;
		}
		void set_blick_spieler_rechts(bool b)
		{
			this->blick_spieler_rechts = b;
		}
		bool get_blick_spieler_rechts()
		{
			return this->blick_spieler_rechts;
		}
		void set_blick_spieler_links(bool b)
		{
			this->blick_spieler_links = b;
		}
		bool get_blick_spieler_links()
		{
			return this->blick_spieler_links;
		}
		void set_leben_spieler(int l)
		{
			this->leben_spieler = l;
		}
		int get_leben_spieler()
		{
			return this->leben_spieler;
		}
		void set_lebensanzahl(int l)
		{
			this->lebensanzahl = l;
		}
		int get_lebensanzahl()
		{
			return this->lebensanzahl;
		}

		Spieler(string n, int position_x, int position_y) : name(n), position_spieler_x(position_x), position_spieler_y(position_y) {};
	}*/
};


int geschwindigkeit_y(int sprunghoehe)
{
	int count = 0;
	for (size_t i = 0; i <= sprunghoehe; i++)
	{
		count = count + i;
		if (count >= sprunghoehe)
		{
			return i;
		}
	}
}

int geschwindigkeit_in_y_richtung = geschwindigkeit_y(sprunghoehe);



class GameWindow : public Gosu::Window
{
public:
	Gosu::Image spieler_1;
	Gosu::Image spieler_2;
	Gosu::Font text_spieler_1, text_spieler_2, anzeige_leben_spieler_1, anzeige_leben_spieler_2;

	GameWindow()
		: Window(800, 600),
		spieler_1("Figur.png"),
		spieler_2("rakete.png"), 
		text_spieler_1(30), text_spieler_2(30), anzeige_leben_spieler_1(30), anzeige_leben_spieler_2(30)
	{
		/*vector<Plattform> plattform_liste;
		plattform_liste.push_back({ 350,450,200,220 });
		plattform_liste.push_back({ 150,250,200,220 });
		plattform_liste.push_back({ 150,250,200,220 });*/

		

		//set_caption("Gosu Tutorial Game mit Git");

	}

	void bewegung_spieler(Spieler spieler)
	{

		if (input().down(Gosu::KB_RIGHT))					//nach Rechts laufen
		{
			spieler.geschwindigkeit_spieler_x = 5;
			spieler.blick_spieler_rechts = true;
			spieler.blick_spieler_links = false;
		}

		if (input().down(Gosu::KB_LEFT))					//nach Links laufen
		{
			spieler.geschwindigkeit_spieler_x = -5;
			spieler.blick_spieler_rechts = false;
			spieler.blick_spieler_links = true;
		}

		// stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen bet‰tigt
		if ((!input().down(Gosu::KB_RIGHT) && !input().down(Gosu::KB_LEFT)) || (input().down(Gosu::KB_RIGHT) && input().down(Gosu::KB_LEFT)))
		{
			spieler.geschwindigkeit_spieler_x = 0;
		}


		if (input().down(Gosu::KB_UP) && (spieler.beschleunigung_spieler_y == 0))					//Springen
		{
			spieler.geschwindigkeit_spieler_y = - geschwindigkeit_in_y_richtung;
		}
		if (spieler.position_spieler_y >= 600 && !input().down(Gosu::KB_UP))
		{
			spieler.geschwindigkeit_spieler_y = 0;
			spieler.position_spieler_y = 600;
		}
		if (spieler.position_spieler_y < 600)
		{
			spieler.beschleunigung_spieler_y = 1;
			spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
		}
		if (position_spieler_1_y >= 600)
		{
			spieler.beschleunigung_spieler_y = 0;
		}
	}



	//Positionen des Spielers
	int position_spieler_1_x = 0;
	int position_spieler_1_y = 600;
	int position_spieler_2_x = 800;
	int position_spieler_2_y = 600;

	//Abstand der Spieler
	double abstand_spieler_x;
	double abstand_spieler_y;

	//Geschwindigkeit des Spielers
	int geschwindigkeit_spieler_1_x;
	int geschwindigkeit_spieler_1_y = 0;
	int geschwindigkeit_spieler_2_x;
	int geschwindigkeit_spieler_2_y = 0;

	//Beschleunigung des Spielers
	int beschleunigung_spieler_1_x;
	int beschleunigung_spieler_1_y;
	int beschleunigung_spieler_2_x;
	int beschleunigung_spieler_2_y;

	//Blickrichtung des Spielers
	bool blick_spieler_1_rechts = false;
	bool blick_spieler_1_links = false;
	bool blick_spieler_2_rechts = false;
	bool blick_spieler_2_links = false;

	//Leben und Lebensanzahl der Spieler
	int leben_spieler_1 = leben_anfang;
	int leben_spieler_2 = leben_anfang;
	int lebensanzahl_spieler_1 = lebensanzahl_anfang;
	int lebensanzahl_spieler_2 = lebensanzahl_anfang;
	



	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		//weiﬂer Hintergurnd
		graphics().draw_quad
		(
			0, 0, Gosu::Color::BLUE,
			800, 0, Gosu::Color::BLUE,
			800, 600, Gosu::Color::BLUE,
			0, 600, Gosu::Color::BLUE,
			-15.0
		);


		text_spieler_1.draw("Spieler 1", 20, 20, 0.0, 1, 1, Gosu::Color::RED);
		text_spieler_2.draw(" Spieler 2", 680, 20, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_1.draw(to_string(leben_spieler_1), 20, 50, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_2.draw(to_string(leben_spieler_2), 680, 50, 0.0, 1, 1, Gosu::Color::RED);

		spieler_1.draw_rot
		(
			position_spieler_1_x,position_spieler_1_y,0.0,1,0.5,1,1.5,1.5
		);
		spieler_2.draw_rot
		(
			position_spieler_2_x, position_spieler_2_y, 0.0, 1, 0.5, 1, 1.5, 1.5
		);
	}

	
	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		position_spieler_1_x = position_spieler_1_x + geschwindigkeit_spieler_1_x +beschleunigung_spieler_1_x;
		position_spieler_1_y = position_spieler_1_y + geschwindigkeit_spieler_1_y;

		position_spieler_2_x = position_spieler_2_x + geschwindigkeit_spieler_2_x + beschleunigung_spieler_2_x;
		position_spieler_2_y = position_spieler_2_y + geschwindigkeit_spieler_2_y;


		if (position_spieler_1_x <= 0)
		{
			position_spieler_1_x = 0;
		}
		else if(position_spieler_1_x >= 800)
		{
			position_spieler_1_x = 800;
		}




		//Bewegungssteuerung Spieler 1

		if (input().down(Gosu::KB_RIGHT))					//nach Rechts laufen
		{
			geschwindigkeit_spieler_1_x = 5;
			blick_spieler_1_rechts = true;
			blick_spieler_1_links = false;
		}

		if (input().down(Gosu::KB_LEFT))					//nach Links laufen
		{
			blick_spieler_1_rechts = false;
			blick_spieler_1_links = true;
			geschwindigkeit_spieler_1_x = -5;
		}

		// stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen bet‰tigt
		if ((!input().down(Gosu::KB_RIGHT) && !input().down(Gosu::KB_LEFT)) || (input().down(Gosu::KB_RIGHT) && input().down(Gosu::KB_LEFT)))	
		{
			geschwindigkeit_spieler_1_x = 0;
		}


		if (input().down(Gosu::KB_UP) && (beschleunigung_spieler_1_y == 0))					//Springen
		{
			geschwindigkeit_spieler_1_y = -geschwindigkeit_in_y_richtung;
		}
		if (position_spieler_1_y >= 600 && !input().down(Gosu::KB_UP))
		{
			geschwindigkeit_spieler_1_y = 0;
			position_spieler_1_y = 600;
		}
		if (position_spieler_1_y < 600)
		{
			beschleunigung_spieler_1_y = 1;
			geschwindigkeit_spieler_1_y = geschwindigkeit_spieler_1_y + beschleunigung_spieler_1_y;
		}
		if (position_spieler_1_y >= 600)
		{
			beschleunigung_spieler_1_y = 0;
		}


		//Bewegungssteuerung Spieler 2

		if (input().down(Gosu::KB_D))					//nach Rechts laufen
		{
			geschwindigkeit_spieler_2_x = 5;
			blick_spieler_2_rechts = true;
			blick_spieler_2_links = false;
		}

		if (input().down(Gosu::KB_A))					//nach Links laufen
		{
			blick_spieler_2_rechts = false;
			blick_spieler_2_links = true;
			geschwindigkeit_spieler_2_x = -5;
		}

		// stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen bet‰tigt
		if ((!input().down(Gosu::KB_D) && !input().down(Gosu::KB_A)) || (input().down(Gosu::KB_D) && input().down(Gosu::KB_A)))
		{
			geschwindigkeit_spieler_2_x = 0;
		}


		if (input().down(Gosu::KB_W) && (beschleunigung_spieler_2_y == 0))					//Springen
		{
			geschwindigkeit_spieler_2_y = -25;
		}
		if (position_spieler_2_y >= 600 && !input().down(Gosu::KB_W))
		{
			geschwindigkeit_spieler_2_y = 0;
			position_spieler_2_y = 600;
		}
		if (position_spieler_2_y < 600)
		{
			beschleunigung_spieler_2_y = 1;
			geschwindigkeit_spieler_2_y = geschwindigkeit_spieler_2_y + beschleunigung_spieler_2_y;
		}
		if (position_spieler_2_y >= 600)
		{
			beschleunigung_spieler_2_y = 0;
		}


		// Abstandsmessung der zwei Spieler

		abstand_spieler_x = sqrt(pow((position_spieler_1_x - position_spieler_2_x), 2));
		abstand_spieler_y = sqrt(pow((position_spieler_1_y - position_spieler_2_y), 2));

		if (abstand_spieler_x <= spieler_min_abstand_x && abstand_spieler_y <= spieler_min_abstand_y)
		{
			if (position_spieler_1_x > position_spieler_2_x)
			{
				beschleunigung_spieler_1_x = 100 * int(200 / leben_spieler_1);
				beschleunigung_spieler_2_x = -100 * int(200 / leben_spieler_1);
				if (leben_spieler_1 >= 35)
				{
					leben_spieler_1 = leben_spieler_1 - 5;
				}
				if (leben_spieler_2 >= 35)
				{
					leben_spieler_2 = leben_spieler_2 - 5;
				}
			}
			else
			{
				beschleunigung_spieler_1_x = -100 * int(90 / leben_spieler_1);
				beschleunigung_spieler_2_x = 100 * int(90 / leben_spieler_1);
				if (leben_spieler_1 >= 35)
				{
					leben_spieler_1 = leben_spieler_1 - 5;
				}
				if (leben_spieler_2 >= 35)
				{
					leben_spieler_2 = leben_spieler_2 - 5;
				}
			}

		}
		else
		{
			beschleunigung_spieler_1_x = 0;
			beschleunigung_spieler_2_x = 0;
		}






		//#########################################################################################

		cout << position_spieler_1_x << "\t" << position_spieler_1_y << "\t" << geschwindigkeit_spieler_1_y << endl;
	
	}
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
	
}