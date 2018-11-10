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


int spieler_min_abstand_x = 10;
int spieler_min_abstand_y = 42;

int lebensanzahl_anfang = 3;
int leben_anfang = 100;
int sprunghoehe = 300;
int geschoss_geschwindigkeit = 10;


int start_position_spieler_1_x = 200;
int start_position_spieler_2_x = 800;
int start_position_spieler_1_y = 600;
int start_position_spieler_2_y = 600;

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


struct Geschoss
{
	int position_x;
	int position_y;
	int geschwindigkeit_x = geschoss_geschwindigkeit;
	bool flugrichtung_rechts;
	bool treffer;

	Gosu::Image bild;

	void flugbahn()
	{
		if (flugrichtung_rechts)
		{
			position_x = position_x + geschwindigkeit_x;
		}
		else
		{
			position_x = position_x - geschwindigkeit_x;
		}
	}

	Geschoss(int x_position, int y_position, bool rechts_flugrichtung, const Gosu::Image geschoss_bild)
		:position_x(x_position), position_y(y_position-30), flugrichtung_rechts(rechts_flugrichtung), bild(geschoss_bild)
	{
		if (rechts_flugrichtung)
		{
			position_x = position_x + 15;			//Damit die Geschosse nicht in der Spielfigur starten wird die Hälfte der Breite der Spielfigur (15 Pixel) auf die Startposition drauf addiert.
		}
		else
		{
			position_x = position_x - 15;			//wie oben nur dass hier die 15 Pixel abgezogen werden da blick nach links ist.
		}

	}


};


struct Spieler
{
	int start_position_x;
	int start_position_y;
	//Name des Spielers
	string name;
	//Positionen des Spielers
	int position_spieler_x = start_position_x;
	int position_spieler_y = start_position_y;
	//Geschwindigkeit des Spielers
	int geschwindigkeit_spieler_x;
	int geschwindigkeit_spieler_y;
	//Beschleunigung des Spielers
	int beschleunigung_spieler_x;
	int beschleunigung_spieler_y;
	//Blickrichtung des Spielers
	bool blick_spieler_rechts;
	//Leben und Lebensanzahl der Spieler
	int leben_spieler = leben_anfang;
	int lebensanzahl = lebensanzahl_anfang;
	//Plattform auf der sich der Spieler befindet
	int plattform_nr;
	//Vekto mit den Bildern der Spielfigur
	vector<Gosu::Image> spielfigur;

	//Vektor mit Geschossen
	vector<Geschoss> geschosse;
	//schiessen
	int zaehler = 0;
	bool schiessen_moeglich = true;


	void set_spielfigur(const Gosu::Image& blick_rechts, const Gosu::Image& blick_links, const Gosu::Image& springen_rechts, const Gosu::Image& springen_links)
	{
		this->spielfigur.push_back(blick_rechts);
		this->spielfigur.push_back(blick_links);
		this->spielfigur.push_back(springen_rechts);
		this->spielfigur.push_back(springen_links);
	}
	
	void draw()
	{
		if (blick_spieler_rechts && beschleunigung_spieler_y == 0)
		{
			spielfigur.at(0).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
		}
		else if(blick_spieler_rechts == false && beschleunigung_spieler_y == 0)
		{
			spielfigur.at(1).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
		}
		else if (blick_spieler_rechts && beschleunigung_spieler_y != 0)
		{
			spielfigur.at(2).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
		}
		else if (blick_spieler_rechts == false && beschleunigung_spieler_y != 0)
		{
			spielfigur.at(3).draw_rot(position_spieler_x, position_spieler_y, 0.0, 0.0, 0.5, 1.0, 1.0, 1.0);
		}

	}

	void draw_geschosse()
	{
		for (auto it = geschosse.begin(); it != geschosse.end(); it++)
		{
			it->bild.draw_rot(it->position_x, it->position_y, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0);
		}
	}

	void update_geschosse()
	{
		for (auto it = geschosse.begin(); it != geschosse.end(); it++)
		{
			it->flugbahn();								//Geschoss bewegt sich in X-Richtung
			if (it->position_x >= 1010 || it->position_x <= -10 || it->treffer == true)
			{
				geschosse.erase(it);					//Geschoss, dass das Spielfeld verlässt wird gelöscht 
				break;									//Nach dem Löschen ist es wichtig, dass ein break kommt und der Iterator it wieder auf das neue beginn() Element setzt, 
														// da sonst ein Fehler auftritt da der Iterator nicht erhöht werden kann, da der Iterator des gelöschten Elements auf kein Element im Vektor zeigt.			
			}
		}
	}



	void rewspan()
	{
		if (position_spieler_y >= 1000 || leben_spieler <= 0)
		{
			leben_spieler = leben_anfang;
			lebensanzahl = lebensanzahl - 1;
			position_spieler_x = start_position_x;
			position_spieler_y = start_position_y;
		}
	}

	void neues_spiel()
	{
		position_spieler_x = start_position_x;
		position_spieler_y = start_position_y;
		lebensanzahl = lebensanzahl_anfang;
		leben_spieler = leben_anfang;
		geschosse.erase(geschosse.begin(), geschosse.end());
	}

	Spieler(int position_x, int position_y, bool nach_rechts_schauen, const Gosu::Image blick_rechts, const Gosu::Image blick_links, const Gosu::Image springen_rechts, const Gosu::Image springen_links)
		: start_position_x(position_x), start_position_y(position_y), blick_spieler_rechts(nach_rechts_schauen)
	{
		set_spielfigur(blick_rechts, blick_links, springen_rechts, springen_links);
	}
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
	int modus = 0;
	int sieger = 0; 
	Spieler spieler_1;
	Spieler spieler_2;
	vector<Plattform> plattformliste;
	string anzeige_startbildschirm = "RETURN zum starten";
	string anzeige_endbildschirm;
	string gewonnen_endbildschirm = "Neues Spiel mit RETURN";

public:
	Gosu::Image plattform;
	Gosu::Image hauptplattform;
	Gosu::Image hintergrund;
	Gosu::Image patrone_spieler_1;
	Gosu::Image patrone_spieler_2;
	Gosu::Image startbildschirm;
	Gosu::Image endbildschirm_unentschieden;
	Gosu::Image endbildschirm_mario;
	Gosu::Image endbildschirm_luigi;

	Gosu::Font text_spieler_1, text_spieler_2, anzeige_leben_spieler_1, anzeige_leben_spieler_2;

	GameWindow()
		: Window(1000, 700),
		plattform("Plattform.png"),
		hauptplattform("Hauptplattform.png"),
		hintergrund("Hintergrund_2.jpg"),
		patrone_spieler_1("Mario_geschoss.png"),
		patrone_spieler_2("Luigi_geschoss.png"),
		startbildschirm("Startbildschirm.png"), endbildschirm_unentschieden("Endbildschirm_unentschieden.png"), endbildschirm_mario("Endbildschirm_mario.png"), endbildschirm_luigi("Endbildschirm_luigi.png"),
		spieler_1(start_position_spieler_1_x, start_position_spieler_1_y, true, Gosu::Image("Mario_Figur_Rechtsblick.png"), Gosu::Image("Mario_Figur_Linksblick.png"), Gosu::Image("Mario_Figur_Springen_Rechts.png"), Gosu::Image("Mario_Figur_Springen_Links.png")),
		spieler_2(start_position_spieler_2_x, start_position_spieler_2_y, false, Gosu::Image("Luigi_Figur_Rechtsblick.png"), Gosu::Image("Luigi_Figur_Linksblick.png"), Gosu::Image("Luigi_Figur_Springen_Rechts.png"), Gosu::Image("Luigi_Figur_Springen_Links.png")),
		text_spieler_1(30), text_spieler_2(30), anzeige_leben_spieler_1(30), anzeige_leben_spieler_2(30)
	{
		spieler_1.name = "Mario";
		spieler_2.name = "Luigi";

		//Textlänge beim Start berechnen -> später läuft das Programm flüssiger wenn der Text aufgerufen wird, da nicht immer die Länge berechnet wird.
		text_spieler_1.text_width(spieler_1.name);
		text_spieler_2.text_width(spieler_2.name);

		//Plattformen erstellen
		plattformliste.push_back(Plattform(plattform, 300, 400, 250, 20));
		plattformliste.push_back(Plattform(plattform, 700, 400, 250, 20));
		plattformliste.push_back(Plattform(plattform, 500, 200, 250, 20));
		plattformliste.push_back(Plattform(hauptplattform, 500, 650, 700, 100));


		set_caption("Benjamin und Luca");
	}


	void schiessen(Spieler& spieler, Gosu::ButtonName schuss, const Gosu::Image bild, int counter, bool schiessen_moeglich)
	{
		if (spieler.schiessen_moeglich == false)
		{
			spieler.zaehler = spieler.zaehler - 1;
		}
		if (spieler.zaehler == 0)
		{
			spieler.schiessen_moeglich = true;
		}
		if (input().down(schuss) && spieler.schiessen_moeglich == true && spieler.geschosse.size() < 5)
		{
			spieler.geschosse.push_back(Geschoss(spieler.position_spieler_x, spieler.position_spieler_y, spieler.blick_spieler_rechts, bild));
			spieler.schiessen_moeglich = false;
			spieler.zaehler = 15;					//0,5 sekunden warten, da die Funktion 30 mal die Sekunde aufgerufen wird.
		}

		//Updaten der Position des Geschosses bzw. schauen ob es gelöscht werden muss
		spieler.update_geschosse();
	}

	void getroffen(Spieler& spieler, Geschoss& patrone)
	{
		if (patrone.position_x >= spieler.position_spieler_x - 20 && patrone.position_x <= spieler.position_spieler_x + 20
			&& patrone.position_y >= spieler.position_spieler_y - 65 && patrone.position_y <= spieler.position_spieler_y + 5)
		{
			patrone.treffer = true;
			spieler.leben_spieler = spieler.leben_spieler - 20;				//pro Treffer -20% Leben
		}
	}

	void geschoss_trifft_plattform(vector<Plattform> plattformliste, Geschoss& patrone)
	{
		for (auto it = plattformliste.begin(); it != plattformliste.end(); it++)
		{
			if(patrone.position_x >= it->links_x - 7 && patrone.position_x <= it->rechts_x + 7 
				&& patrone.position_y <= it->unten_y + 7 && patrone.position_y >= it->oben_y - 7)
			{
				patrone.treffer = true;
			}
		}
	}

	void getroffen_von_geschoss(Spieler& spieler_1, Spieler& spieler_2)
	{
		for (auto it = spieler_2.geschosse.begin(); it != spieler_2.geschosse.end(); it++)
		{
			getroffen(spieler_1, (*it));
			geschoss_trifft_plattform(plattformliste, (*it));
		}
		for (auto it2 = spieler_1.geschosse.begin(); it2 != spieler_1.geschosse.end(); it2++)
		{
			getroffen(spieler_2, (*it2));
			geschoss_trifft_plattform(plattformliste, (*it2));
		}
	}

	void bewegung_spieler(Spieler& spieler, Gosu::ButtonName oben, Gosu::ButtonName rechts, Gosu::ButtonName links)
	{
		//Abfrage auf welcher Plattform sich der spieler befindet: 0-linke, 1-rechte, 2-obere, 3-Haupt Plattform
		//Wenn sich der Spieler auf keiner Plattform befindet wir als plattform_nr 4 gespeichert, was später bei der y-Bewegung abgefragt wird
		for (int i = 0; i <= 3; i++)
		{

				if (spieler.position_spieler_y <= (plattformliste.at(i).oben_y) && spieler.position_spieler_y >= (plattformliste.at(i).oben_y - 40))
				{
					if (spieler.position_spieler_x >= plattformliste.at(i).links_x && spieler.position_spieler_x <= plattformliste.at(i).rechts_x)
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
		spieler.position_spieler_x = spieler.position_spieler_x + spieler.geschwindigkeit_spieler_x + spieler.beschleunigung_spieler_x;
		spieler.position_spieler_y = spieler.position_spieler_y + spieler.geschwindigkeit_spieler_y;

		//Spielfigur kann nicht über den Rand hinauslaufen
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
		}

		//nach Links laufen
		if (input().down(links))					
		{
			spieler.geschwindigkeit_spieler_x = -5;
			spieler.blick_spieler_rechts  = false;
		}

		//stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen betätigt
		if ((!input().down(rechts) && !input().down(links)) || (input().down(rechts) && input().down(links)))
		{
			spieler.geschwindigkeit_spieler_x = 0;
		}


		//Springen
		//Wenn der Spieler auf einer Plattform ist wird nachfolgend ausgeführt.
		if (spieler.plattform_nr <= 3)
		{
			//oben Taste wird betätigt und die Beschleunigung des Spielers ist 0 -> er steht auf einer Plattform
			if (input().down(oben) && (spieler.beschleunigung_spieler_y == 0))
			{
				spieler.geschwindigkeit_spieler_y = -geschwindigkeit_in_y_richtung;			//setze Y-Geschwindigkeit auf den in der geschwindigkeit_y Funktion, durch angabe der maximalen Sprunghöhe, errechneten Wert.
			}

			//Ist der Spieler auf einer Plattform und die Springen-Taste wird nicht betätigt, so wird die aktuelle Y-Geschwindigkeit auf 0 gesetzt und der Y-Wert auf den oberen Y-Wert der Plattform.
			//Es wird bewusst abgefragt ob die Y-Position des Spielers >= der der Plattformoberfläche ist, da diese Funktion nur begrenzt pro Sekunde aufgerufen wird, kann es passiere, dass der Spieler in der Plattform ist.
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y && !input().down(oben))
			{
			spieler.geschwindigkeit_spieler_y = 0;
			spieler.position_spieler_y = plattformliste.at(spieler.plattform_nr).oben_y;
			}

			//Ist der Spieler auf einer Plattform und die Springen-Taste wird betätigt, so wird die aktuelle Y-Geschwindigkeit auf 0, sodass die Figur nicht in der Plattform drin ist.
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y && input().down(oben))
			{
				spieler.position_spieler_y = plattformliste.at(spieler.plattform_nr).oben_y;
			}

			//Ist der Spieler auf einer Plattform so wird seine Beschleunigung 0 gesetzt und somit das entsprechende Bild geladen.
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y)
			{
				spieler.beschleunigung_spieler_y = 0;
			}

			//Else ist hier, der Spieler ist über einer Plattform in der Luft -> seine Y-Beschleunigung Richtung Boden wird auf 1 gesetzt und seine Y-Geschwindigkeit mit der Beschleunigung addiert.
			else
			{
				spieler.beschleunigung_spieler_y = 1;
				spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
			}
		}
		//ist der Spieler auf keiner Plattform (also irgendwo in der Luft), so wird er in y Richtung beschleunigt und fällt hinunter, bis er auf einer Plattform ist
		else
		{
			spieler.beschleunigung_spieler_y = 1;
			spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
		}

	}

	void spieler_stossen_sich_ab(Spieler& spieler_1, Spieler& spieler_2)
	{
		int abstand_spieler_x = sqrt(pow((spieler_1.position_spieler_x - spieler_2.position_spieler_x), 2));
		int abstand_spieler_y = sqrt(pow((spieler_1.position_spieler_y - spieler_2.position_spieler_y), 2));

		if (abstand_spieler_x <= spieler_min_abstand_x && abstand_spieler_y <= spieler_min_abstand_y)
		{
		if (spieler_1.position_spieler_x > spieler_2.position_spieler_x)
		{
			spieler_1.beschleunigung_spieler_x = 100 ;
			spieler_2.beschleunigung_spieler_x = -100;
		}
		else
		{
			spieler_1.beschleunigung_spieler_x = -100;
			spieler_2.beschleunigung_spieler_x = 100;
		}
		if (spieler_1.leben_spieler >= 45)
		{
			spieler_1.leben_spieler = spieler_1.leben_spieler - 5;
		}
		if (spieler_2.leben_spieler >= 45)
		{
			spieler_2.leben_spieler = spieler_2.leben_spieler - 5;
		}
		}
		else
		{
			spieler_1.beschleunigung_spieler_x = 0;
			spieler_2.beschleunigung_spieler_x = 0;
		}
	}



	//Abstand der Spieler
	double abstand_spieler_x;
	double abstand_spieler_y;



	// wird bis zu 60x pro Sekunde aufgerufen.
	// Wenn die Grafikkarte oder der Prozessor nicht mehr hinterherkommen,
	// dann werden `draw` Aufrufe ausgelassen und die Framerate sinkt
	void draw() override
	{

		switch (modus)
		{
			case 0:
				startbildschirm.draw_rot(500, 350, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0);
				break;

			case 1:
				hintergrund.draw_rot(500, 350, -5.0, 0.0, 0.5, 0.5, 1.25, 1.25);

				for (auto it = plattformliste.begin(); it != plattformliste.end(); it++)
				{
					it->draw();
				}

				text_spieler_1.draw(spieler_1.name, 20, 20, 0.0, 1, 1, Gosu::Color::RED);
				text_spieler_2.draw(spieler_2.name, 890, 20, 0.0, 1, 1, Gosu::Color::GREEN);
				anzeige_leben_spieler_1.draw(to_string(spieler_1.lebensanzahl) + " Leben", 20, 50, 0.0, 1, 1, Gosu::Color::RED);
				anzeige_leben_spieler_2.draw(to_string(spieler_2.lebensanzahl) + " Leben", 890, 50, 0.0, 1, 1, Gosu::Color::GREEN);
				anzeige_leben_spieler_1.draw(to_string(spieler_1.leben_spieler) + "%", 20, 80, 0.0, 1, 1, Gosu::Color::RED);
				anzeige_leben_spieler_2.draw(to_string(spieler_2.leben_spieler) + "%", 890, 80, 0.0, 1, 1, Gosu::Color::GREEN);

				spieler_1.draw();
				spieler_2.draw();

				spieler_1.draw_geschosse();
				spieler_2.draw_geschosse();

				break;

			case 2:

				if (sieger == 0)
				{
					endbildschirm_unentschieden.draw_rot(500, 350, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0);
				}
				else if (sieger == 1)
				{
					endbildschirm_mario.draw_rot(500, 350, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0);
				}
				else if (sieger == 2)
				{
					endbildschirm_luigi.draw_rot(500, 350, 0.0, 0.0, 0.5, 0.5, 1.0, 1.0);
				}
				break;

			default:
				break;
		}

	}


	// Wird 60x pro Sekunde aufgerufen
	void update() override
	{
		//bei betätigen von esc wird das Programm geschlossen
		if (Window::input().down(Gosu::KB_ESCAPE))
		{
			cout << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nSpiel beendet\nBenjamin Schuppel und Luca Valenziano\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" << endl;
			Window::close();
		}


		switch (modus) 
		{
			case 0:
				if (input().down(Gosu::KB_RETURN))
				{
					modus = 1;
				}
				break;

			case 1:
				bewegung_spieler(spieler_1, Gosu::KB_UP, Gosu::KB_RIGHT, Gosu::KB_LEFT);
				bewegung_spieler(spieler_2, Gosu::KB_W, Gosu::KB_D, Gosu::KB_A);

				schiessen(spieler_1, Gosu::KB_RIGHT_ALT, patrone_spieler_1, spieler_1.zaehler, spieler_1.schiessen_moeglich);
				schiessen(spieler_2, Gosu::KB_SPACE, patrone_spieler_2, spieler_2.zaehler, spieler_2.schiessen_moeglich);

				spieler_stossen_sich_ab(spieler_1, spieler_2);
				getroffen_von_geschoss(spieler_1, spieler_2);

				spieler_1.rewspan();
				spieler_2.rewspan();

				if (spieler_1.lebensanzahl == 0 || spieler_2.lebensanzahl == 0)
				{
					if (spieler_1.lebensanzahl == 0)
					{
						sieger = 2;
					}
					else if (spieler_2.lebensanzahl == 0)
					{
						sieger = 1;
					}
					else
					{
						sieger = 0;
					}

					modus = 2;
				}
				break;

			case 2:
				if (input().down(Gosu::KB_RETURN))
				{
					spieler_1.neues_spiel();
					spieler_2.neues_spiel();
					modus = 1;
				}
				break;

			default:
				modus = 0;
				break;
		}	

//#########################################################################################

		//cout << spieler_1.position_spieler_x <<"\t" << spieler_1.position_spieler_y << endl;
	};
};

// C++ Hauptprogramm
int main()
{
	GameWindow window;
	window.show();
}