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


struct Spieler
{
	int plattform_nr;

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


	void rewspan_nach_out_of_map()
	{
		if (position_spieler_y >= 900)
		{
			lebensanzahl = lebensanzahl - 1;
			leben_spieler = leben_anfang;

			position_spieler_x = start_position_x;
			position_spieler_y = start_position_y;
		}
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
	Spieler spieler_1;
	Spieler spieler_2;
	vector<Plattform> plattformliste;

public:
	Gosu::Image plattform;
	Gosu::Image hauptplattform;
	Gosu::Image hintergrund;

	Gosu::Font text_spieler_1, text_spieler_2, anzeige_leben_spieler_1, anzeige_leben_spieler_2;

	GameWindow()
		: Window(1000, 700),
		plattform("Plattform.png"),
		hauptplattform("Hauptplattform.png"),
		hintergrund("Hintergrund_2.jpg"),
		spieler_1(start_position_spieler_1_x, start_position_spieler_1_y, true, Gosu::Image("Mario_Figur_Rechtsblick.png"), Gosu::Image("Mario_Figur_Linksblick.png"), Gosu::Image("Luigi_Figur_Springen_Rechts.png"), Gosu::Image("Luigi_Figur_Springen_Links.png")),
		spieler_2(start_position_spieler_2_x, start_position_spieler_2_y, false, Gosu::Image("Luigi_Figur_Rechtsblick.png"), Gosu::Image("Luigi_Figur_Linksblick.png"), Gosu::Image("Luigi_Figur_Springen_Rechts.png"), Gosu::Image("Luigi_Figur_Springen_Links.png")),
		text_spieler_1(30), text_spieler_2(30), anzeige_leben_spieler_1(30), anzeige_leben_spieler_2(30)
	{
		spieler_1.name = "Mario";
		spieler_2.name = "Luigi";

		//Plattformen erstellen
		plattformliste.push_back(Plattform(plattform, 300, 400, 250, 20));
		plattformliste.push_back(Plattform(plattform, 700, 400, 250, 20));
		plattformliste.push_back(Plattform(plattform, 500, 200, 250, 20));
		plattformliste.push_back(Plattform(hauptplattform, 500, 650, 700, 100));


		set_caption("Benjamin und Luca");
	}



	void bewegung_spieler(Spieler& spieler, Gosu::ButtonName oben, Gosu::ButtonName rechts, Gosu::ButtonName links)
	{
		//Abfrage auf welcher Plattform sich der spieler befindet: 0-linke, 1-rechte, 2-obere, 3-Haupt Plattform
		//Wenn sich der Spieler auf keiner Plattform befindet wir als plattform_nr 4 gespeichert, was sp�ter bei der y-Bewegung abgefragt wird
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
		}

		//nach Links laufen
		if (input().down(links))					
		{
			spieler.geschwindigkeit_spieler_x = -5;
			spieler.blick_spieler_rechts  = false;
		}

		//stehen bleiben, wenn keine Richtung eingegeben wird bzw. wenn beide Richtungen bet�tigt
		if ((!input().down(rechts) && !input().down(links)) || (input().down(rechts) && input().down(links)))
		{
			spieler.geschwindigkeit_spieler_x = 0;
		}


		//Springen
		//Wenn der Spieler auf einer Plattform ist wird nachfolgend ausgef�hrt.
		if (spieler.plattform_nr <= 3)
		{
			//oben Taste wird bet�tigt und die Beschleunigung des Spielers ist 0 -> er steht auf einer Plattform
			if (input().down(oben) && (spieler.beschleunigung_spieler_y == 0))
			{
				spieler.geschwindigkeit_spieler_y = -geschwindigkeit_in_y_richtung;			//setze Y-Geschwindigkeit auf den in der geschwindigkeit_y Funktion, durch angabe der maximalen Sprungh�he, errechneten Wert.
			}

			//Ist der Spieler auf einer Plattform und die Springen-Taste wird nicht bet�tigt, so wird die aktuelle Y-Geschwindigkeit auf 0 gesetzt und der Y-Wert auf den oberen Y-Wert der Plattform.
			//Es wird bewusst abgefragt ob die Y-Position des Spielers >= der der Plattformoberfl�che ist, da diese Funktion nur begrenzt pro Sekunde aufgerufen wird, kann es passiere, dass der Spieler in der Plattform ist.
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y && !input().down(oben))
			{
				spieler.geschwindigkeit_spieler_y = 0;
				spieler.position_spieler_y = plattformliste.at(spieler.plattform_nr).oben_y;
			}

			//Ist der Spieler auf einer Plattform und die Springen-Taste wird bet�tigt, so wird die aktuelle Y-Geschwindigkeit auf 0, sodass die Figur nicht in der Plattform drin ist.
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y && input().down(oben))
			{
				spieler.position_spieler_y = plattformliste.at(spieler.plattform_nr).oben_y;
			}

			//Ist der Spieler auf einer Plattform so wird seine Beschleunigung 0 gesetzt und somit das entsprechende Bild geladen.
			if (spieler.position_spieler_y >= plattformliste.at(spieler.plattform_nr).oben_y)
			{
				spieler.beschleunigung_spieler_y = 0;
			}

			//Else ist hier, der Spieler ist �ber einer Plattform in der Luft -> seine Y-Beschleunigung Richtung Boden wird auf 1 gesetzt und seine Y-Geschwindigkeit mit der Beschleunigung addiert.
			else
			{
				spieler.beschleunigung_spieler_y = 1;
				spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
			}
		}
		//ist der Spieler auf keiner Plattform (also irgendwo in der Luft), so wird er in y Richtung beschleunigt und f�llt hinunter, bis er auf einer Plattform ist
		else
		{
			spieler.beschleunigung_spieler_y = 1;
			spieler.geschwindigkeit_spieler_y = spieler.geschwindigkeit_spieler_y + spieler.beschleunigung_spieler_y;
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

		hintergrund.draw_rot(500, 350, -5.0, 0.0, 0.5, 0.5, 1.25, 1.25);

		for (auto it = plattformliste.begin(); it != plattformliste.end(); it++)
		{
			it->draw();
		}


		text_spieler_1.draw("Spieler 1", 20, 20, 0.0, 1, 1, Gosu::Color::RED);
		text_spieler_2.draw(" Spieler 2", 680, 20, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_1.draw(to_string(spieler_1.lebensanzahl), 20, 50, 0.0, 1, 1, Gosu::Color::RED);
		anzeige_leben_spieler_2.draw(to_string(spieler_2.lebensanzahl), 680, 50, 0.0, 1, 1, Gosu::Color::RED);


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



		bewegung_spieler(spieler_1, Gosu::KB_UP, Gosu::KB_RIGHT, Gosu::KB_LEFT);
		bewegung_spieler(spieler_2, Gosu::KB_W, Gosu::KB_D, Gosu::KB_A);


		spieler_1.rewspan_nach_out_of_map();
		spieler_2.rewspan_nach_out_of_map();



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