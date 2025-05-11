#include<SFML/Graphics.hpp>
#include<iostream>
#include<vector>
#include<utility>
#include<cmath>
#include<string>
#include <ctime>

using namespace std;

//VARIJABLA ZA ODABIR IGRA!
int gameMode = 0; // 0 oznacava da igra nije odabrana - kliknut je [x], 1 ako zelimo Igrac Vs Racunalo, 2 ako zelimo Igrac Vs Igrac
//------------------------

void MENUizborIgre(sf::RenderWindow& window, sf::Font& font)
{
	//pozadina MENU izbornika
	sf::Texture menuTexture;
	menuTexture.loadFromFile("pozadina.png");
	sf::Sprite menuSprite;
	menuSprite.setTexture(menuTexture);

	//napisemo naslov i pravila
	//naslov
	sf::Text naslov("Dobrodosli u glavni izbornik, izaberite mode", font);
	naslov.setCharacterSize(30);
	naslov.setStyle(sf::Text::Bold);
	naslov.setFillColor(sf::Color::Black);
	naslov.setPosition(5, 50);

	sf::Text pravila("Pravila: povezujete tockice tako da napravite linije izmedju njih,\n"
		"naizmjence se izmejuje jedan igrac s drugim. Kada se spoji\n"
		"kutija(kvadrat) igrac koji je to spojio ponovno je na redu.\n"
		"Pobjednik je onaj igrac koji ima vise spojenih kutija(kvadrata)!\n\n"
		"Ukoliko se promasi druga tockica prilikom klika probajte\n"
		"ponovno napraviti nekoliko klikova da se registrira potez", font);
	pravila.setCharacterSize(20);
	pravila.setStyle(sf::Text::Bold);
	pravila.setFillColor(sf::Color::Black);
	pravila.setPosition(10, 400);

	//definiramo dva pravokutnika, jedan za Igrac Vs Racunalo, drugi za Igrac Vs Igrac
	sf::RectangleShape opcijaIgracVSRacunalo(sf::Vector2f(240.f, 80.f));
	opcijaIgracVSRacunalo.setFillColor(sf::Color::Magenta);
	opcijaIgracVSRacunalo.setPosition(200.f, 150.f);

	sf::RectangleShape opcijaIgracVSIgrac(sf::Vector2f(240.f, 80.f));
	opcijaIgracVSIgrac.setFillColor(sf::Color::Yellow);
	opcijaIgracVSIgrac.setPosition(200.f, 250.f);

	sf::Text mode1("Igrac Vs Racunalo", font);
	mode1.setCharacterSize(25);
	mode1.setStyle(sf::Text::Bold);
	mode1.setFillColor(sf::Color::Black);
	mode1.setPosition(210, 170);

	sf::Text mode2("Igrac Vs Igrac", font);
	mode2.setCharacterSize(25);
	mode2.setStyle(sf::Text::Bold);
	mode2.setFillColor(sf::Color::Black);
	mode2.setPosition(230, 270);

	//Sve dok je pocetni prozor otvoren	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close(); //ostaje prilikom event.type == sf::Event::Closed gameMode na 0
				return;
			}
			//moramo pogledat je li korisnik kliknuo na neku od ponudjenih opcija
			if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
			{
				if (opcijaIgracVSRacunalo.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					gameMode = 1;
					return;
				}
				if (opcijaIgracVSIgrac.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y))
				{
					gameMode = 2;
					return;
				}
			}
		}
		//tu ide crtanje i odgovarajuce akcije
		window.clear(sf::Color::White);

		//background
		window.draw(menuSprite);

		window.draw(naslov);
		window.draw(pravila);
		window.draw(opcijaIgracVSRacunalo);
		window.draw(opcijaIgracVSIgrac);
		window.draw(mode1);
		window.draw(mode2);

		window.display();

	}

}

//sve potrebne varijable za igru ukljucujuci i sve sf::Texture koje ce se koristiti
int igrac_na_redu; //moze biti 0 ili 1
int bodoviIgracA; // bodovi za igraca 0
int bodoviIgracB; // bodovi za igraca 1
int broj_zatvorenih_kutija; //koliko je kutija zatvoreno u spajanju linije, 0, 1 ili 2
string igrac_A = "Igrac A";
string igrac_B = "Igrac B";
string tekst = "Igraci: ";
string tkoJeNaRedu = "Na redu je ";
string zavrsenaIgra = "";
sf::Texture macka1Slika;
sf::Texture macka2Slika;
sf::Texture klupkoSlika;
sf::Texture textureIgracA;
sf::Texture textureIgracB;
sf::Texture textureIgracAnaredu;
sf::Texture textureIgracBnaredu;
sf::Texture textureIgracApobjedio;
sf::Texture textureIgracBpobjedio;
sf::Texture pozadinaTexture;
sf::Texture pokazivacTexture;



//slijede klase za definiranje objekata - Tocke, linije, kutije (kvadrati)
class Tocka {
public:
	Tocka(int r, int s)
	{
		oznacena = false;
		radijus = 20.f;
		float razmak = (640 - (6 * radijus * 2)) / 7;
		x = razmak + radijus + s * (2 * radijus + razmak);
		y = razmak + radijus + r * (2 * radijus + razmak);
		redak = r;
		stupac = s;
	}

	sf::CircleShape krug;
	int x, y;
	float radijus;
	bool oznacena;
	int redak, stupac;
};

class Linija {
public:
	Linija(int r1, int s1, int r2, int s2) : a(r1, s1), b(r2, s2)
	{
		oznacena_crveno = false;
		oznacena_zeleno = false;
	}

	sf::RectangleShape line;
	float duljina = 94.f;
	float sirina = 8.f;
	Tocka a, b;
	bool oznacena_crveno;
	bool oznacena_zeleno;
};

class Kutija {
public:
	Kutija(int r, int s)
	{
		redak = r;
		stupac = s;
		float radijus = 20.f;
		float razmak = (640 - (6 * radijus * 2)) / 7;
		x = razmak + radijus + s * (2 * radijus + razmak);
		y = razmak + radijus + r * (2 * radijus + razmak);
		oznaceno_crveno = false;
		oznaceno_zeleno = false;
		stranica = razmak * 2 - radijus / 2;
	}

	sf::RectangleShape kvadrat;
	float stranica;
	int redak, stupac;
	int x, y;
	bool oznaceno_crveno;
	bool oznaceno_zeleno;
};
//--------------------------------------------------

//Slijede funkcije za crtanje tocaka, linija, kutija

//funkcija nacrtaj tockicu
void nacrtajTockicu(Tocka A, sf::RenderWindow& window)
{
	A.krug.setRadius(A.radijus);
	if (A.oznacena)
		A.krug.setTexture(&klupkoSlika);
	else
		A.krug.setFillColor(sf::Color::Black);
	A.krug.setOrigin(A.radijus, A.radijus);
	A.krug.setPosition(A.x, A.y);
	window.draw(A.krug);
}

//funkcija nacrtaj liniju
void nacrtajLiniju(Linija A, sf::RenderWindow& window)
{
	if (A.a.x == A.b.x)
		A.line.setSize(sf::Vector2f(A.sirina, A.duljina));
	else
		A.line.setSize(sf::Vector2f(A.duljina, A.sirina));

	A.line.setPosition(A.a.x - 3.f, A.a.y - 3.f);

	if (A.oznacena_crveno)
		A.line.setFillColor(sf::Color::Red);
	else if (A.oznacena_zeleno)
		A.line.setFillColor(sf::Color::Color(0, 153, 0, 255));
	else
		A.line.setFillColor(sf::Color::Color(0, 0, 0, 0));

	window.draw(A.line);
}

//funkcija nacrtaj kutiju
void nacrtajKutiju(Kutija A, sf::RenderWindow& window) {
	A.kvadrat.setSize(sf::Vector2f(A.stranica, A.stranica));
	if (A.oznaceno_crveno) {
		A.kvadrat.setTexture(&macka1Slika);
	}
	else if (A.oznaceno_zeleno) {
		A.kvadrat.setTexture(&macka2Slika);
	}
	else {
		A.kvadrat.setFillColor(sf::Color::Color(0, 0, 0, 0));
		A.kvadrat.setTexture(nullptr);
	}
	A.kvadrat.setPosition(A.x, A.y);
	window.draw(A.kvadrat);
}

//----------------------------------------------------------

//Slijede odgovarajuce provjere

int provjeraZatvorenostiKutije(vector<Kutija>& kutije, const vector<Linija>& linije)
{
	int br_zatvorenih = 0;

	for (auto& kutija : kutije) {

		if (kutija.oznaceno_crveno || kutija.oznaceno_zeleno)
			continue; //u slucaju da je vec neka kutija obojana idemo na iduæu

		bool lijeva = false, desna = false, gornja = false, donja = false;

		for (const auto& linija : linije)
		{
			if ((linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
				linija.b.redak == kutija.redak && linija.b.stupac == kutija.stupac + 1 &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
					linija.b.redak == kutija.redak && linija.b.stupac == kutija.stupac + 1 &&
					linija.oznacena_zeleno))
			{
				gornja = true;
			}
			if ((linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac + 1 &&
				linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac + 1 &&
					linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
					linija.oznacena_zeleno))
			{
				desna = true;
			}
			if ((linija.a.redak == kutija.redak + 1 && linija.a.stupac == kutija.stupac &&
				linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak + 1 && linija.a.stupac == kutija.stupac &&
					linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
					linija.oznacena_zeleno))
			{
				donja = true;
			}
			if ((linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
				linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
					linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac &&
					linija.oznacena_zeleno))
			{
				lijeva = true;
			}
		}
		//ako su sve linije zatvorene oko odredjene kutije ovisno o igracu oznacimo tu kutiju odg. bojom
		// te povecamo broj zatvorenih
		if (lijeva && desna && gornja && donja) //treba dodat igraca 
		{
			if (igrac_na_redu == 0)
			{
				kutija.oznaceno_crveno = true; //treba vidjeti jos samo koji ce kasnije igrac biti na redu!
				br_zatvorenih++;
			}
			else
			{
				kutija.oznaceno_zeleno = true;
				br_zatvorenih++;
			}

		}
	}
	return br_zatvorenih;
}

float udaljenost(float x1, float y1, float x2, float y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//provjera jesu li sve kutije ispunjene, kako je rijec o 6x6 => 25 kutija
int zavrseno()
{
	if ((bodoviIgracA + bodoviIgracB) != 25)
		return 0;
	else
		return 1;
}


//Slijede funkcije za Racunalo kao igraca!

Linija* odaberi(vector<Linija>& linije) {
	vector<Linija*> neoznacene;
	for (auto& linija : linije) {
		if (!linija.oznacena_crveno && !linija.oznacena_zeleno) {
			neoznacene.push_back(&linija);
		}
	}
	if (!neoznacene.empty()) {
		int ind = rand() % neoznacene.size();
		return neoznacene[ind];
	}
	return nullptr;
}

void oznaciTocke(Linija* linija, vector<Tocka>& tocke) {
	for (auto& T : tocke) {
		if ((T.redak == linija->a.redak && T.stupac == linija->a.stupac) ||
			(T.redak == linija->b.redak && T.stupac == linija->b.stupac)) {
			T.oznacena = true;
		}
	}
}
//---------------------------------------------------------------
//MINiMAX
int provjeraZatvorenostiMinMax(vector<Kutija>& kutije,vector<Linija>& linije, vector<Kutija*>& zatvorene)
{
	zatvorene.clear();//imamo prazan vektor -> fja ga vraca
	int br_zatvorenih = 0;

	for (auto& kutija : kutije) {

		if (kutija.oznaceno_crveno || kutija.oznaceno_zeleno)
			continue; //u slucaju da je vec neka kutija obojana idemo na iduæu

		bool lijeva = false, desna = false, gornja = false, donja = false; //ponovna provjera je li kutija zatvorena

		for (const auto& linija : linije)
		{
			if ((linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
				linija.b.redak == kutija.redak && linija.b.stupac == kutija.stupac + 1 &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
					linija.b.redak == kutija.redak && linija.b.stupac == kutija.stupac + 1 &&
					linija.oznacena_zeleno))
			{
				gornja = true;
			}
			if ((linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac + 1 &&
				linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac + 1 &&
					linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
					linija.oznacena_zeleno))
			{
				desna = true;
			}
			if ((linija.a.redak == kutija.redak + 1 && linija.a.stupac == kutija.stupac &&
				linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak + 1 && linija.a.stupac == kutija.stupac &&
					linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac + 1 &&
					linija.oznacena_zeleno))
			{
				donja = true;
			}
			if ((linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
				linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac &&
				linija.oznacena_crveno) || (linija.a.redak == kutija.redak && linija.a.stupac == kutija.stupac &&
					linija.b.redak == kutija.redak + 1 && linija.b.stupac == kutija.stupac &&
					linija.oznacena_zeleno))
			{
				lijeva = true;
			}
		}

		if (lijeva && desna && gornja && donja) //treba dodat igraca 
		{
			if (igrac_na_redu == 0)
			{
				kutija.oznaceno_crveno = true; //treba vidjeti jos samo koji ce kasnije igrac biti na redu!
				br_zatvorenih++;
				zatvorene.push_back(&kutija);
			}
			else
			{
				kutija.oznaceno_zeleno = true;
				br_zatvorenih++;
				zatvorene.push_back(&kutija);
			}

		}
	}
	return br_zatvorenih;
}

int gotovaIgra()
{
	if ((bodoviIgracA + bodoviIgracB) == 25)
		return 1;
	else
		return 0;
}

int tkoJePobijedio()
{
	if (bodoviIgracA >= 13)
		return 1; //pobijedio igrac Igrac
	if (bodoviIgracB >= 13)
		return 2; //pobijedio igrac Racunalo
	return 0; //igra jos nije gotova
}

int minimax(std::vector<Linija>& linije, std::vector<Kutija>& kutije, int depth, bool tko)
{
	vector<Kutija*> zatvorene;
	int bestVal;

	if (!tko)
		bestVal = std::numeric_limits<int>::max();
	else
		bestVal = std::numeric_limits<int>::min();

	int koliko;
	int pobijedio = 0;
	int moveVal;
	if (gotovaIgra() == 1)
	{
		pobijedio = tkoJePobijedio();
		if (pobijedio == 2)
			return 1000;
		else if (pobijedio == 1)
			return -1000;
	}

	if (depth == 0)
		return 0;

	for (auto& linija : linije)
	{
		if (!linija.oznacena_crveno && !linija.oznacena_zeleno) {

			if (tko)
				linija.oznacena_zeleno = true;
			else
				linija.oznacena_crveno = true;

			koliko = provjeraZatvorenostiMinMax(kutije, linije, zatvorene);

			if (koliko == 0)
				moveVal = minimax(linije, kutije, depth - 1, !tko);
			else
				moveVal = minimax(linije, kutije, depth - 1, tko);

			linija.oznacena_zeleno = false;
			linija.oznacena_crveno = false;

			for (auto& zatvorena : zatvorene)
			{
				zatvorena->oznaceno_crveno = false;
				zatvorena->oznaceno_zeleno = false;
			}

			if (tko && moveVal + koliko > bestVal) {
				bestVal = moveVal + koliko;
			}
			else if (!tko && moveVal + koliko < bestVal) {
				bestVal = moveVal + koliko;
			}
		}
	}
	return bestVal;
}
//trazenje najboljeg poteza => ovo ce se pozivati kada je racunalo na redu
Linija* findBestMove(std::vector<Linija>& linije, std::vector<Kutija>& kutije, int depth) {
	int bestVal = std::numeric_limits<int>::min();
	int koliko = 0;
	int moveVal;
	vector<Kutija*> zatvorene;
	Linija* bestMove = nullptr;

	for (auto& linija : linije) {
		if (!linija.oznacena_crveno && !linija.oznacena_zeleno) {
			linija.oznacena_zeleno = true;
			koliko = provjeraZatvorenostiMinMax(kutije, linije, zatvorene);

			if(koliko == 0)
				moveVal = minimax(linije, kutije, depth - 1, false);
			else
				moveVal = minimax(linije, kutije, depth - 1, true);

			linija.oznacena_zeleno = false;
			for (auto& zatvorena : zatvorene)
			{
				zatvorena->oznaceno_crveno = false;
				zatvorena->oznaceno_zeleno = false;
			}
			if (moveVal + koliko > bestVal) {
				bestMove = &linija;
				bestVal = moveVal + koliko;
			}
		}
	}

	return bestMove;
}

//MAIN (GLAVNI) DIO:
int main() {

	sf::RenderWindow prozor;
	prozor.create(sf::VideoMode(640, 640), "Zatvaranje kvadrata");

	//za racunalo
	srand(static_cast<unsigned int>(time(0)));

	//definiramo sve potrebno, koji je igrac na redu i ostalo
	igrac_na_redu = 0;
	bodoviIgracA = 0;
	bodoviIgracB = 0;
	broj_zatvorenih_kutija = 0;
	//-------------------------------------------------------

	//custom stvari

	//FONT
	sf::Font font;
	font.loadFromFile("arial.ttf");

	if (!font.loadFromFile("arial.ttf")) {
		// doslo je do greske prilikom ucitavanja fonta
		return -1;
	}

	MENUizborIgre(prozor, font);

	if (!prozor.isOpen())
		return 0;

	//IKONICA MISA
	if (!pokazivacTexture.loadFromFile("sapica.png"))
	{
		return -1;
	}
	sf::Sprite pokazivacSprite;
	pokazivacSprite.setTexture(pokazivacTexture);

	//sakrivanje default pokazivaca misa
	prozor.setMouseCursorVisible(false);

	//za slicice koje ce biti unutar oznacenih kutija
	if (!macka1Slika.loadFromFile("macka1.png")) {
		return -1;
	}

	if (!macka2Slika.loadFromFile("macka2.png")) {
		return -1;
	}

	//za klupko koje ce nastati umjesto tocke kada se napravi linija
	if (!klupkoSlika.loadFromFile("klupko.png")) {
		return -1;
	}

	//pozadinska slika igre (background)
	if (!pozadinaTexture.loadFromFile("pozadina.png")) {
		return -1;
	}
	sf::Sprite pozadinaSprite;
	pozadinaSprite.setTexture(pozadinaTexture);

	//za ikonice igraca pored kojih ce biti odgovarajuci bodovi
	if (!textureIgracA.loadFromFile("igracA.png")) {
		return -1;
	}

	if (!textureIgracB.loadFromFile("igracB.png")) {
		return -1;
	}

	sf::Sprite spriteIgracA(textureIgracA);
	sf::Sprite spriteIgracB(textureIgracB);

	spriteIgracA.setPosition(150.f, -5.f); // Poèetna pozicija slièice Igrac A
	spriteIgracB.setPosition(250.f, 0.f); // Poèetna pozicija slièice Igrac B

	//za ikonice igraca tko je na redu

	if (!textureIgracAnaredu.loadFromFile("igracA.png")) {
		return -1;
	}

	if (!textureIgracBnaredu.loadFromFile("igracB.png")) {
		return -1;
	}

	sf::Sprite spriteIgracAnaredu(textureIgracAnaredu);
	sf::Sprite spriteIgracBnaredu(textureIgracBnaredu);

	spriteIgracAnaredu.setPosition(550.f, -5.f); // Poèetna pozicija slièice Igrac A
	spriteIgracBnaredu.setPosition(550.f, 0.f); // Poèetna pozicija slièice Igrac B

	//za ikonicu igraca koja ce se pokazati ovisno o tome ko pobjedi
	if (!textureIgracApobjedio.loadFromFile("igracA.png")) {
		return -1;
	}

	if (!textureIgracBpobjedio.loadFromFile("igracB.png")) {
		return -1;
	}

	sf::Sprite spriteIgracApobjedio(textureIgracApobjedio);
	sf::Sprite spriteIgracBpobjedio(textureIgracBpobjedio);

	spriteIgracApobjedio.setPosition(580.f, 570.f); // Poèetna pozicija slièice Igrac A
	spriteIgracBpobjedio.setPosition(580.f, 570.f); // Poèetna pozicija slièice Igrac B


	//--------------------------------------------------------
	//odgovarajuca inicijalizacija

	pair<int, int> odabir_tocke;
	vector<pair<int, int>> odabir_duzine;
	int odabir_redak;
	int odabir_stupac;


	vector<Tocka> tocke;
	vector<Linija> linije;
	vector<Kutija> kutije;

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; j++) {
			tocke.push_back(Tocka(i, j));
		}
	}

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 5; j++) {
			linije.push_back(Linija(i, j, i, j + 1));
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 6; j++) {
			linije.push_back(Linija(i, j, i + 1, j));
		}
	}

	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 5; j++) {
			kutije.push_back(Kutija(i, j));
		}
	}

	while (prozor.isOpen())
	{
		sf::Event d;
		while (prozor.pollEvent(d))
		{
			if (d.type == sf::Event::Closed)
				prozor.close();
			//Igrac prvi zapocinje u svakom slucaju igru (Nikad racunalo)
			if (igrac_na_redu == 0)
			{
				sf::Vector2i pozicijaMisa = sf::Mouse::getPosition(prozor);
				pokazivacSprite.setPosition(static_cast<float>(pozicijaMisa.x), static_cast<float>(pozicijaMisa.y));

				if (d.type == sf::Event::MouseButtonPressed && d.mouseButton.button == sf::Mouse::Left)
				{
					for (auto& T : tocke)
					{
						if (udaljenost(T.x, T.y, d.mouseButton.x, d.mouseButton.y) <= T.radijus)
						{
							odabir_redak = T.redak;
							odabir_stupac = T.stupac;
							odabir_tocke = make_pair(odabir_redak, odabir_stupac);
							odabir_duzine.push_back(odabir_tocke);

							if (odabir_duzine.size() == 2)
							{
								for (auto& L : linije)
								{
									if (((L.a.redak == odabir_duzine[0].first && L.a.stupac == odabir_duzine[0].second) &&
										(L.b.redak == odabir_duzine[1].first && L.b.stupac == odabir_duzine[1].second)) ||
										((L.b.redak == odabir_duzine[0].first && L.b.stupac == odabir_duzine[0].second) &&
											(L.a.redak == odabir_duzine[1].first && L.a.stupac == odabir_duzine[1].second)))
									{

										if (L.oznacena_crveno || L.oznacena_zeleno)
										{
											odabir_duzine.clear();
											break;
										}

										L.oznacena_crveno = true;

										for (auto& T : tocke)
										{
											if ((T.redak == odabir_duzine[0].first && T.stupac == odabir_duzine[0].second) ||
												(T.redak == odabir_duzine[1].first && T.stupac == odabir_duzine[1].second))
											{
												T.oznacena = true;
											}
										}

										broj_zatvorenih_kutija = provjeraZatvorenostiKutije(kutije, linije);
										bodoviIgracA += broj_zatvorenih_kutija;

										if (broj_zatvorenih_kutija > 0)
										{
											odabir_duzine.clear();
											igrac_na_redu = 0;
											break;
										}
										else
										{
											odabir_duzine.clear();
											igrac_na_redu = 1;
											break;
										}
									}
								}
								odabir_duzine.clear();
							}
						}
					}
				}
			} // ovdje zavrsava igrac_na_redu == 0

			else
			{	//u slucaju da je MODE igre igrac vs igrac
				if (igrac_na_redu == 1 && gameMode == 2)
				{
					sf::Vector2i pozicijaMisa = sf::Mouse::getPosition(prozor);
					pokazivacSprite.setPosition(static_cast<float>(pozicijaMisa.x), static_cast<float>(pozicijaMisa.y));
					//igrac na redu = 1
					if (d.type == sf::Event::MouseButtonPressed && d.mouseButton.button == sf::Mouse::Left)
					{
						for (auto& T : tocke)
						{
							if (udaljenost(T.x, T.y, d.mouseButton.x, d.mouseButton.y) <= T.radijus)
							{
								odabir_redak = T.redak;
								odabir_stupac = T.stupac;
								odabir_tocke = make_pair(odabir_redak, odabir_stupac);
								odabir_duzine.push_back(odabir_tocke);

								if (odabir_duzine.size() == 2)
								{
									for (auto& L : linije)
									{
										if (((L.a.redak == odabir_duzine[0].first && L.a.stupac == odabir_duzine[0].second) &&
											(L.b.redak == odabir_duzine[1].first && L.b.stupac == odabir_duzine[1].second)) ||
											((L.b.redak == odabir_duzine[0].first && L.b.stupac == odabir_duzine[0].second) &&
												(L.a.redak == odabir_duzine[1].first && L.a.stupac == odabir_duzine[1].second)))
										{

											if (L.oznacena_crveno || L.oznacena_zeleno)
											{
												odabir_duzine.clear();
												break;
											}

											L.oznacena_zeleno = true;

											for (auto& T : tocke)
											{
												if ((T.redak == odabir_duzine[0].first && T.stupac == odabir_duzine[0].second) ||
													(T.redak == odabir_duzine[1].first && T.stupac == odabir_duzine[1].second))
												{
													T.oznacena = true;
												}
											}

											broj_zatvorenih_kutija = provjeraZatvorenostiKutije(kutije, linije);
											bodoviIgracB += broj_zatvorenih_kutija;

											if (broj_zatvorenih_kutija > 0)
											{
												odabir_duzine.clear();
												igrac_na_redu = 1;
												break;
											}
											else
											{
												odabir_duzine.clear();
												igrac_na_redu = 0;
												break;
											}
										}
									}
									odabir_duzine.clear();
								}
							}
						}
					}
				}
				//igrac vs racunalo
				if (igrac_na_redu == 1 && gameMode == 1)
				{
					Linija* odabrana = findBestMove(linije, kutije, 2); //poziv fje koja ukljucuje minimax a vraca odg. liniju
					if (odabrana != nullptr) {
						odabrana->oznacena_zeleno = true;

						// Oznaèavanje toèaka linije
						oznaciTocke(odabrana, tocke);

						broj_zatvorenih_kutija = provjeraZatvorenostiKutije(kutije, linije);
						bodoviIgracB += broj_zatvorenih_kutija;

						if (broj_zatvorenih_kutija == 0) {
							igrac_na_redu = 0;
						}
					}
				}

			}
		}

		prozor.clear(sf::Color::White);


		// Prikaz bodova i slièica
		sf::Text textBodoviA(std::to_string(bodoviIgracA), font);
		textBodoviA.setCharacterSize(45);
		textBodoviA.setStyle(sf::Text::Bold);
		textBodoviA.setFillColor(sf::Color::Red);
		textBodoviA.setPosition(190.f, 0.f); // Pozicija teksta za bodove igraèa A

		sf::Text textBodoviB(std::to_string(bodoviIgracB), font);
		textBodoviB.setCharacterSize(45);
		textBodoviB.setStyle(sf::Text::Bold);
		textBodoviB.setFillColor(sf::Color::Color(0, 153, 0, 255));
		textBodoviB.setPosition(290.f, 0.f); // Pozicija teksta za bodove igraèa B

		sf::Text text(tekst, font);
		text.setCharacterSize(35);
		text.setStyle(sf::Text::Bold);
		text.setFillColor(sf::Color::Color(51, 51, 153, 255));
		text.setPosition(40.f, 5.f);

		sf::Text textNaRedu(tkoJeNaRedu, font);
		textNaRedu.setCharacterSize(35);
		textNaRedu.setStyle(sf::Text::Bold);
		textNaRedu.setFillColor(sf::Color::Color(51, 51, 153, 255));
		textNaRedu.setPosition(370.f, 5.f);


		//prvo pozadina
		prozor.draw(pozadinaSprite);

		//pa na nju ikonice i bodovi
		prozor.draw(spriteIgracA);
		prozor.draw(textBodoviA);
		prozor.draw(spriteIgracB);
		prozor.draw(textBodoviB);
		prozor.draw(text);
		prozor.draw(textNaRedu);

		if (igrac_na_redu == 0)
			prozor.draw(spriteIgracAnaredu);
		else
			prozor.draw(spriteIgracBnaredu);

		for (auto& K : kutije)
			nacrtajKutiju(K, prozor);
		for (auto& L : linije)
			nacrtajLiniju(L, prozor);
		for (auto& T : tocke)
			nacrtajTockicu(T, prozor);

		if (zavrseno())
		{
			sf::Text zavrsenaText;
			zavrsenaText.setFont(font);
			if (bodoviIgracA > bodoviIgracB)
			{
				zavrsenaText.setString("Igra je zavrsena, pobijedio je igrac: ");
				zavrsenaText.setFillColor(sf::Color::Blue);
				zavrsenaText.setCharacterSize(35);
				zavrsenaText.setPosition(30, 580);
				prozor.draw(zavrsenaText);
				prozor.draw(spriteIgracApobjedio);
			}
			else if (bodoviIgracB > bodoviIgracA)
			{
				zavrsenaText.setString("Igra je zavrsena, pobijedio je igrac: ");
				zavrsenaText.setFillColor(sf::Color::Blue);
				zavrsenaText.setCharacterSize(35);
				zavrsenaText.setPosition(30, 580);
				prozor.draw(zavrsenaText);
				prozor.draw(spriteIgracBpobjedio);
			}

			else
			{
				zavrsenaText.setString("Igra je zavrsena, nerijeseno je!");
				zavrsenaText.setFillColor(sf::Color::Blue);
				zavrsenaText.setCharacterSize(35);
				zavrsenaText.setPosition(70, 580);
				prozor.draw(zavrsenaText);
			}
		} 

		//crtanje ikonica misa
		prozor.draw(pokazivacSprite);

		prozor.display();
	}

	return 0;
}