#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>

#include <vector>
#include <string>
#include <iostream>

#include "Planet.h"
#include "Vektor2d.h"

#include <Gosu/Audio.hpp>

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

struct Plattform
{
	int mittelpunkt_x;
	int mittelpunkt_y;
	int laenge;
	int hoehe;

	int links_x = mittelpunkt_x - 125;
	int unten_y = mittelpunkt_y + 10;
	int rechts_x = mittelpunkt_x - 125;
	int oben_y = mittelpunkt_y - 10;;


	Gosu::Image picture;

	void draw()
	{
		picture.draw_rot(mittelpunkt_x, mittelpunkt_y, -1.0, 0.0, 0.5, 0.5, 1.0, 1.0);
	}

	Plattform(Gosu::Image bild, int mitte_x, int mitte_y, int lang = 250, int hoch = 20) : picture(bild),mittelpunkt_x(mitte_x),mittelpunkt_y(mitte_y),laenge(lang),hoehe(hoch)
	{
		picture.draw_rot(mittelpunkt_x, mittelpunkt_y, -1.0, 0.0, 0.5, 0.5, 1.0, 1.0);
	}

};





class Spieler
{
private:
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

	vector<Gosu::Image> spielfigur;

	public:

		void set_spielfigur(const Gosu::Image& blick_rechts, const Gosu::Image& blick_links, const Gosu::Image& springen_rechts, const Gosu::Image& springen_links)
		{
			this->spielfigur.push_back(blick_rechts);
			this->spielfigur.push_back(blick_links);
			this->spielfigur.push_back(springen_rechts);
			this->spielfigur.push_back(springen_links);
		}
	
		void draw()
		{
			if (blick_spieler_rechts && !blick_spieler_links && beschleunigung_spieler_y == 0)
			{
				spielfigur.at(0).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
			}
			else if(blick_spieler_links && !blick_spieler_rechts && beschleunigung_spieler_y == 0)
			{
				spielfigur.at(1).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
			}
			else if (blick_spieler_rechts && !blick_spieler_links && beschleunigung_spieler_y != 0)
			{
				spielfigur.at(2).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
			}
			else if (blick_spieler_links && !blick_spieler_rechts && beschleunigung_spieler_y != 0)
			{
				spielfigur.at(3).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
			}

		}

		//void plattform_check(const vector<Plattform> plattform_liste)
		//{
		//	for(auto it = plattform_liste.begin(); it != plattform_liste.end(); it++)
		//	if(position_spieler_x > it->links_x && pos )
		//}


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
		void set_geschwindigkeit_spieler_x(int g)
		{
			this->geschwindigkeit_spieler_x = g;
		}
		int get_geschwindigkeit_spieler_x()
		{
			return this->geschwindigkeit_spieler_x;
		}
		void set_geschwindigkeit_spieler_y(int g)
		{
			this->geschwindigkeit_spieler_y = g;
		}
		int get_geschwindigkeit_spieler_y()
		{
			return this->geschwindigkeit_spieler_y;
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

		//Spieler(string n, int position_x, int position_y) : name(n), position_spieler_x(position_x), position_spieler_y(position_y) {};
};


int geschwindigkeit_y(int sprunghoehe)
{
	int count = 0;
	for (int i = 0; i <= sprunghoehe; i++)
	{
		count = count + i;
		if (count >= sprunghoehe)
		{
			return i;
		}
	}
}

int geschwindigkeit_in_y_richtung = geschwindigkeit_y(sprunghoehe);


class GameWindow : public Gosu::Window, public Spieler
{
	Spieler spieler_1;
	Spieler spieler_2;

public:
	Gosu::Image spieler_bild_1;
	Gosu::Image spieler_bild_2;
	Gosu::Image spieler_bild_1_links;
	Gosu::Image spieler_bild_2_links;
	Gosu::Image spieler_bild_1_springen_rechts;
	Gosu::Image spieler_bild_2_springen_rechts;
	Gosu::Image spieler_bild_1_springen_links;
	Gosu::Image spieler_bild_2_springen_links;
	Gosu::Image plattform;
	Gosu::Image hintergrund;

	Gosu::Font text_spieler_1, text_spieler_2, anzeige_leben_spieler_1, anzeige_leben_spieler_2;

	GameWindow()
		: Window(1000, 700),
		spieler_bild_1("Mario_Figur_Rechtsblick.png"),
		spieler_bild_2("Luigi_Figur_Rechtsblick.png"),
		spieler_bild_1_links("Mario_Figur_Linksblick.png"),
		spieler_bild_2_links("Luigi_Figur_Linksblick.png"),
		spieler_bild_1_springen_rechts("Luigi_Figur_Springen_Rechts.png"),
		spieler_bild_2_springen_rechts("Luigi_Figur_Springen_Rechts.png"),
		spieler_bild_1_springen_links("Luigi_Figur_Springen_Links.png"),
		spieler_bild_2_springen_links("Luigi_Figur_Springen_Links.png"),
		plattform("Plattform.png"),
		hintergrund("Hintergrund_2.jpg"),
		text_spieler_1(30), text_spieler_2(30), anzeige_leben_spieler_1(30), anzeige_leben_spieler_2(30)
	{
		spieler_1.set_spielfigur(spieler_bild_1, spieler_bild_1_links, spieler_bild_1_springen_rechts, spieler_bild_1_springen_links);
		spieler_1.set_position_spieler_x(0);
		spieler_1.set_position_spieler_y(600);
		spieler_1.set_geschwindigkeit_spieler_y(0);

		spieler_2.set_spielfigur(spieler_bild_2, spieler_bild_2_links, spieler_bild_2_springen_rechts, spieler_bild_2_springen_links);
		spieler_2.set_position_spieler_x(800);
		spieler_2.set_position_spieler_y(600);
		spieler_2.set_geschwindigkeit_spieler_y(0);


		/*vector<Plattform> plattform_liste;
		plattform_liste.push_back({ 350,450,200,220 });
		plattform_liste.push_back({ 150,250,200,220 });
		plattform_liste.push_back({ 150,250,200,220 });*/


		//set_caption("Gosu Tutorial Game mit Git");
	}




	void bewegung_spieler(Spieler& spieler, Gosu::ButtonName oben, Gosu::ButtonName rechts, Gosu::ButtonName links)
	{

		spieler.set_position_spieler_x(spieler.get_position_spieler_x() + spieler.get_geschwindigkeit_spieler_x());
		spieler.set_position_spieler_y(spieler.get_position_spieler_y() + spieler.get_geschwindigkeit_spieler_y());

		if (input().down(rechts))					//nach Rechts laufen
		{
			spieler.set_geschwindigkeit_spieler_x(5);
			spieler.set_blick_spieler_rechts(true);
			spieler.set_blick_spieler_links(false);
		}

		if (input().down(links))					//nach Links laufen
		{
			spieler.set_geschwindigkeit_spieler_x(-5);
			spieler.set_blick_spieler_rechts(false);
			spieler.set_blick_spieler_links(true);
		}

		// stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen bet‰tigt
		if ((!input().down(rechts) && !input().down(links)) || (input().down(rechts) && input().down(links)))
		{
			spieler.set_geschwindigkeit_spieler_x(0);
		}


		if (input().down(oben) && (spieler.get_beschleunigung_spieler_y() == 0))					//Springen
		{
			spieler.set_geschwindigkeit_spieler_y(-geschwindigkeit_in_y_richtung);
		}
		if (spieler.get_position_spieler_y() >= 600 && !input().down(oben))
		{
			spieler.set_geschwindigkeit_spieler_y(0);
			spieler.set_position_spieler_y(600);
		}
		if (spieler.get_position_spieler_y() < 600)
		{
			spieler.set_beschleunigung_spieler_y(1);
			spieler.set_geschwindigkeit_spieler_y(spieler.get_geschwindigkeit_spieler_y() + spieler.get_beschleunigung_spieler_y());
		}
		if (spieler.get_position_spieler_y() >= 600)
		{
			spieler.set_beschleunigung_spieler_y(0);
		}
	}


	//Abstand der Spieler
	double abstand_spieler_x;
	double abstand_spieler_y;

	//Leben und Lebensanzahl der Spieler
	int leben_spieler_1 = leben_anfang;
	int leben_spieler_2 = leben_anfang;
	int lebensanzahl_spieler_1 = lebensanzahl_anfang;
	int lebensanzahl_spieler_2 = lebensanzahl_anfang;

	//{
	//	//Positionen des Spielers
	//	int position_spieler_1_x = 0;
	//	int position_spieler_1_y = 600;
	//	int position_spieler_2_x = 800;
	//	int position_spieler_2_y = 600;

	//	//Geschwindigkeit des Spielers
	//	int geschwindigkeit_spieler_1_x;
	//	int geschwindigkeit_spieler_1_y = 0;
	//	int geschwindigkeit_spieler_2_x;
	//	int geschwindigkeit_spieler_2_y = 0;

	//	//Beschleunigung des Spielers
	//	int beschleunigung_spieler_1_x;
	//	int beschleunigung_spieler_1_y;
	//	int beschleunigung_spieler_2_x;
	//	int beschleunigung_spieler_2_y;

	//	//Blickrichtung des Spielers
	//	bool blick_spieler_1_rechts = false;
	//	bool blick_spieler_1_links = false;
	//	bool blick_spieler_2_rechts = false;
	//	bool blick_spieler_2_links = false;
	//}




	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		Plattform plattform_1(plattform, 300, 350, 150, 20);
		Plattform plattform_2(plattform, 700, 350, 150, 20);
		Plattform plattform_3(plattform, 500, 150, 150, 20);

		hintergrund.draw_rot(500, 350, -5.0, 0.0, 0.5, 0.5, 1.25, 1.25);

		//weiﬂer Hintergurnd
		//graphics().draw_quad
		//(
		//	0, 0, Gosu::Color::BLUE,
		//	800, 0, Gosu::Color::BLUE,
		//	800, 600, Gosu::Color::BLUE,
		//	0, 600, Gosu::Color::BLUE,
		//	-15.0
		//);


	

		text_spieler_1.draw("Spieler 1", 20, 20, 0.0, 1, 1, Gosu::Color::RED);
		text_spieler_2.draw(" Spieler 2", 680, 20, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_1.draw(to_string(leben_spieler_1), 20, 50, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_2.draw(to_string(leben_spieler_2), 680, 50, 0.0, 1, 1, Gosu::Color::RED);

		//spieler_bild_1.draw_rot
		//(
		//	position_spieler_1_x,position_spieler_1_y,0.0,1,0.5,1,1.5,1.5
		//);


		spieler_1.draw();
		spieler_2.draw();

		/*spieler_bild_2.draw_rot
		(
			position_spieler_2_x, position_spieler_2_y, 0.0, 1, 0.5, 1, 1.5, 1.5
		);*/
	}


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{

		/*position_spieler_1_x = position_spieler_1_x + geschwindigkeit_spieler_1_x +beschleunigung_spieler_1_x;
		position_spieler_1_y = position_spieler_1_y + geschwindigkeit_spieler_1_y;

		position_spieler_2_x = position_spieler_2_x + geschwindigkeit_spieler_2_x + beschleunigung_spieler_2_x;
		position_spieler_2_y = position_spieler_2_y + geschwindigkeit_spieler_2_y;*/


		bewegung_spieler(spieler_1, Gosu::KB_UP, Gosu::KB_RIGHT, Gosu::KB_LEFT);
		bewegung_spieler(spieler_2, Gosu::KB_W, Gosu::KB_D, Gosu::KB_A);

		//if (position_spieler_1_x <= 0)
		//{
		//	position_spieler_1_x = 0;
		//}
		//else if(position_spieler_1_x >= 800)
		//{
		//	position_spieler_1_x = 800;
		//}



		// Abstandsmessung der zwei Spieler

		/*abstand_spieler_x = sqrt(pow((position_spieler_1_x - position_spieler_2_x), 2));
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
*/





//#########################################################################################

//cout << position_spieler_1_x << "\t" << position_spieler_1_y << "\t" << geschwindigkeit_spieler_1_y << endl;
		cout << spieler_1.get_position_spieler_x() << endl;
	};
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
	
}