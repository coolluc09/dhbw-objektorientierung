#include "stdafx.h"

#include <Gosu/Gosu.hpp>
#include <Gosu/AutoLink.hpp>
#include <Gosu/Window.hpp>

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
int sprunghoehe = 300;

struct Plattform
{
	int mittelpunkt_x;
	int mittelpunkt_y;
	int laenge;
	int hoehe;

	int links_x = mittelpunkt_x - (laenge / 2);
	int rechts_x = mittelpunkt_x + (laenge / 2);
	int unten_y = mittelpunkt_y + (hoehe / 2);
	int oben_y = mittelpunkt_y - (hoehe / 2);


	Gosu::Image picture;

	void draw()
	{
		picture.draw_rot(mittelpunkt_x, mittelpunkt_y, -1.0, 0.0, 0.5, 0.5, 1.0, 1.0);
	}

	Plattform(Gosu::Image bild, int mitte_x, int mitte_y, int lang = 250, int hoch = 20) : picture(bild),mittelpunkt_x(mitte_x),mittelpunkt_y(mitte_y),laenge(lang),hoehe(hoch)
	{
	}

};


struct Spieler
{
	int plattform_nr;

//private:
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


	Spieler(int position_x, int position_y, const Gosu::Image& blick_rechts, const Gosu::Image& blick_links,
		const Gosu::Image& springen_rechts, const Gosu::Image& springen_links)
		//: position_spieler_x(position_x), position_spieler_y(position_y)
	{
		position_spieler_x = position_x;
		position_spieler_y = position_y;
		set_spielfigur(blick_rechts, blick_links, springen_rechts, springen_links);
	}
	Spieler() 
	{

	};
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


class GameWindow : public Gosu::Window
{
	//bool spiel_fortsetzen = true;
	//int plattform_nr = 3;
	Spieler spieler_1;
	Spieler spieler_2;
	vector<Plattform> plattformliste;

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
	Gosu::Image hauptplattform;
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
		hauptplattform("Hauptplattform.png"),
		hintergrund("Hintergrund_2.jpg"),
		text_spieler_1(30), text_spieler_2(30), anzeige_leben_spieler_1(30), anzeige_leben_spieler_2(30)
	{
		//Spieler spieler_1 = Spieler(1, 600, spieler_bild_1, spieler_bild_1_links, spieler_bild_1_springen_rechts, spieler_bild_1_springen_links);
		//Spieler spieler_2 = Spieler(800, 600, spieler_bild_2, spieler_bild_2_links, spieler_bild_2_springen_rechts, spieler_bild_2_springen_links);

		//Spieler 1 mit Werten belegen
		spieler_1.position_spieler_x = 235;
		spieler_1.position_spieler_y = 600;
		spieler_1.blick_spieler_rechts = true;
		spieler_1.blick_spieler_links = false;
		spieler_1.set_spielfigur(spieler_bild_1, spieler_bild_1_links, spieler_bild_1_springen_rechts, spieler_bild_1_springen_links);

		//Spieler 2 mit Werten belegen
		spieler_2.position_spieler_x = 735;
		spieler_2.position_spieler_y = 600;
		spieler_2.blick_spieler_rechts = false;
		spieler_2.blick_spieler_links = true;
		spieler_2.set_spielfigur(spieler_bild_2, spieler_bild_2_links, spieler_bild_2_springen_rechts, spieler_bild_2_springen_links);

		//Plattformen erstellen
		plattformliste.push_back(Plattform(plattform, 300, 400, 250, 20));
		plattformliste.push_back(Plattform(plattform, 700, 400, 250, 20));
		plattformliste.push_back(Plattform(plattform, 500, 200, 250, 20));
		plattformliste.push_back(Plattform(hauptplattform, 500, 650, 700, 100));


		set_caption("Benjamin und Luca");
	}

	//bool auf_plattform(vector<Plattform> plattformen, Spieler spieler)
	//{
	//	for (int i = 0; i <= 3; i++)
	//	{
	//		if (spieler.position_spieler_x >= plattformen.at(i).links_x && spieler.position_spieler_x <= plattformen.at(i).rechts_x)
	//		{
	//			if (spieler.position_spieler_y >= (plattformen.at(i).unten_y - 10))
	//			{
	//				spieler.plattform_nr = i;
	//				return true;
	//			}
	//		}
	//	}

	//}


	void bewegung_spieler(Spieler& spieler, Gosu::ButtonName oben, Gosu::ButtonName rechts, Gosu::ButtonName links, vector<Plattform> plattformen)
	{
		for (int i = 0; i <= 3; i++)
		{

				if (spieler.position_spieler_y <= (plattformen.at(i).oben_y) && spieler.position_spieler_y >= (plattformen.at(i).oben_y - 20))
				{
					if (spieler.position_spieler_x >= plattformen.at(i).links_x && spieler.position_spieler_x <= plattformen.at(i).rechts_x)
					{
						spieler.plattform_nr = i;
						break;
					}
					else
					{
						spieler.plattform_nr = 4;
					}
				}
		
		}

		//Position des Spielers aktualisieren
		spieler.position_spieler_x = spieler.position_spieler_x + spieler.geschwindigkeit_spieler_x;
		spieler.position_spieler_y = spieler.position_spieler_y + spieler.geschwindigkeit_spieler_y;

		//Spielfigur kann nicht �ber den Rand hinauslaufen
		if (spieler.position_spieler_x <= 15)
		{
			spieler.position_spieler_x = 15;
		}
		else if (spieler.position_spieler_x >= 985)
		{
			spieler.position_spieler_x = 985;
		}

		//nach Rechts laufen
		if (input().down(rechts))					
		{
			spieler.geschwindigkeit_spieler_x = 5;
			spieler.blick_spieler_rechts = true;
			spieler.blick_spieler_links = false;
		}

		//nach Links laufen
		if (input().down(links))					
		{
			spieler.geschwindigkeit_spieler_x = -5;
			spieler.blick_spieler_rechts  = false;
			spieler.blick_spieler_links  = true;
		}

		//stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen bet�tigt
		if ((!input().down(rechts) && !input().down(links)) || (input().down(rechts) && input().down(links)))
		{
			spieler.geschwindigkeit_spieler_x = 0;
		}


		//Springen
		if (spieler.plattform_nr <= 3)
		{
			if (input().down(oben) && (spieler.beschleunigung_spieler_y == 0))
			{
				spieler.geschwindigkeit_spieler_y = -geschwindigkeit_in_y_richtung;
			}
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y && !input().down(oben))
			{
				spieler.geschwindigkeit_spieler_y = 0;
				spieler.position_spieler_y = plattformliste.at(spieler.plattform_nr).oben_y;
			}
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y)
			{
				spieler.beschleunigung_spieler_y = 0;
			}
			else
			{
				spieler.beschleunigung_spieler_y = 1;
				spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
			}
		}
		else
		{
			spieler.beschleunigung_spieler_y = 1;
			spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
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



	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		hintergrund.draw_rot(500, 350, -5.0, 0.0, 0.5, 0.5, 1.25, 1.25);

		for (auto it = plattformliste.begin(); it != plattformliste.end(); it++)
		{
			it->draw();
		}


		text_spieler_1.draw("Spieler 1", 20, 20, 0.0, 1, 1, Gosu::Color::RED);
		text_spieler_2.draw(" Spieler 2", 680, 20, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_1.draw(to_string(leben_spieler_1), 20, 50, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_2.draw(to_string(leben_spieler_2), 680, 50, 0.0, 1, 1, Gosu::Color::RED);


		spieler_1.draw();
		spieler_2.draw();
	}


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//bei bet�tigen von esc wird das Programm geschlossen
		if (Window::input().down(Gosu::KB_ESCAPE))
		{
			Window::close();
		}

		//auf_plattform(plattformliste, spieler_1);

		bewegung_spieler(spieler_1, Gosu::KB_UP, Gosu::KB_RIGHT, Gosu::KB_LEFT, plattformliste);
		bewegung_spieler(spieler_2, Gosu::KB_W, Gosu::KB_D, Gosu::KB_A, plattformliste);


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
		cout << spieler_1.position_spieler_x <<"\t" << spieler_1.position_spieler_y << endl;
	};
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();

}