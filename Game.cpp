#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Client.h"
#include "Objects.h"

void gameLoop(); 
void draw();
void events(); 
void getInput();
void getData();
void sendInput();

void calculateWeaponPos(Player&); 

namespace ob {

	int WIDTH = 800; 
	int HEIGHT = 1000; 

	sf::RenderWindow *window; 
	sf::Color bgCol; 

	std::vector<DrawableObject> objects;
	Player player1; 
	Player player2; 
}

namespace net {
	ClientClass *client; 
	ClientClass *client2; 
}

namespace buttons {
	bool A_pressed = 0; 
	bool D_pressed = 0; 
	bool S_pressed = 0; 
	bool SPACE_pressed = 0; 
	bool M1_pressed = 0; 

	int mx = 0;
	int my = 0;

}

void init() {

	ob::window = new sf::RenderWindow(sf::VideoMode(ob::WIDTH, ob::HEIGHT), "Gun Game");
	ob::window->setFramerateLimit(20);
	ob::window->setKeyRepeatEnabled(false);

	ob::bgCol.r = 130;
	ob::bgCol.g = 113;
	ob::bgCol.b = 96;

	net::client = new ClientClass("127.0.0.1", 8080);  //77.20.133.201
	net::client2 = new ClientClass("127.0.0.1", 8081); 

	std::cout << "\nEnd of Initialization\n" << std::endl; 

	ob::player1.init(); 
	ob::player2.init(); 

}

void gameLoop() {
	static bool gameRunning = true;

	init(); 

	while (gameRunning) {
		events(); 
		getInput(); 
		sendInput(); 
		getData(); 
		draw(); 
		
	}
}

void events() {
	sf::Event event; 

	while (ob::window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			ob::window->close();
		}
	}
}

void draw() {

	ob::window->clear(ob::bgCol); 

	for (DrawableObject& drob : ob::objects) {
		ob::window->draw(drob.objectShape); 
	}

	ob::window->draw(*ob::player1.playerShape);
	ob::window->draw(*ob::player2.playerShape);

	ob::window->draw(*ob::player1.weapon);
	ob::window->draw(*ob::player2.weapon);

	ob::window->display(); 
}

void getInput() {
	buttons::A_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
	buttons::D_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
	buttons::S_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::S);
	buttons::SPACE_pressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	buttons::M1_pressed = sf::Mouse::isButtonPressed(sf::Mouse::Left);

	auto mousePos = sf::Mouse::getPosition(*ob::window); 

	buttons::mx = mousePos.x; 
	buttons::my = mousePos.y; 

	sf::Event events; 
	while (ob::window->pollEvent(events)) {
		if (events.type == sf::Event::Closed)
			ob::window->close();
	}
}

void getData() {

	// get Data from server
	std::string dat = net::client2->recieve(); 

	// parse data
	std::vector<int> numbers;
	
	int i = 0; 
	
	while (i < dat.size()) {
		if (dat[i] == ';') {
			int nextSem = dat.find(';', i + 1);

			std::string s = dat.substr(i + 1, nextSem - i - 1);
			
			if (s != "!") {
				//printf("%s\n", s.c_str()); 
				numbers.push_back(std::atoi(s.c_str()));

			}

			i = nextSem; 
		}
		else {
			i++; 
		}

	}

	ob::objects.clear(); 
	
	// create players
	ob::player1.x = numbers[0]; 
	ob::player1.y = numbers[1]; 

	ob::player2.x = numbers[4]; 
	ob::player2.y = numbers[5]; 

	// calculate weapon angle for player 1
	ob::player1.mPosX = numbers[2]; 
	ob::player1.mPosY = numbers[3]; 

	ob::player2.mPosX = buttons::mx; 
	ob::player2.mPosY = buttons::my; 


	calculateWeaponPos(ob::player1); 
	calculateWeaponPos(ob::player2); 

	ob::player1.updateData(); 
	ob::player2.updateData(); 

	//std::cout << "nbr6: " << numbers[6] << std::endl; 

	// create Bullets
	for (int i = 0; i < numbers[6]; i++) {
		ob::objects.push_back(DrawableObject(numbers[6+2*i + 1], numbers[6+2*i + 2], 5, 5)); 
	}

	int index = numbers[6] * 2 + 7;

	// create Walls 
	for (int i = 0; i < numbers[index]; i++) {
		ob::objects.push_back(DrawableObject(numbers[index + 5 * i + 1], numbers[index + 5 * i + 2], numbers[index + 5 * i + 3], numbers[index + 5 * i + 4]));
	}

}

void sendInput() {
	std::string s = std::to_string(buttons::A_pressed) + std::to_string(buttons::D_pressed) +
					std::to_string(buttons::S_pressed) + std::to_string(buttons::SPACE_pressed) +
					std::to_string(buttons::M1_pressed) + ';' + std::to_string(buttons::mx) +
					';' +  std::to_string(buttons::my) + "!";

	//std::cout << s << std::endl; 

	net::client->send(s);
}

void calculateWeaponPos(Player& p) {
	double dx = p.mPosX - (p.x + p.w / 2.0);
	double dy = p.mPosY - (p.y + p.h / 3.0);

	double nevl = 1 / sqrt(dx * dx + dy * dy);

	double angle = -acos(dx / sqrt(dx * dx + dy * dy)) * 180.0 / 3.1415;


	//std::cout << "dx: " << dx << "  dy: " << dy << "  angle: " << angle << std::endl; 

	if (dy > 0) {
		angle = 360 - angle;
	}

	dx *= nevl * 30; // normaleneinheitsvektor * 30 X
	dy *= nevl * 30; // normaleneinheitsvektor * 30 Y

	p.weaponPosX = (p.x + p.w / 2.0) + dx;
	p.weaponPosY = (p.y + p.h / 3.0) - 10 + dy;

	p.weaponRotation = angle; 

}