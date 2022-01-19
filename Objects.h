#pragma once

struct DrawableObject {
	int x, y;
	int w, h;

	int angle = 0;

	int outlineThickness = 2;

	sf::Color color;
	sf::Color outlineColor;

	sf::RectangleShape objectShape;

	DrawableObject(int x, int y, int w, int h) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;

		objectShape.setPosition({(float) x, (float) y }); 
		objectShape.setSize({ (float) w, (float) h });
		objectShape.setOutlineThickness((float) outlineThickness); 
	}

	DrawableObject(int x, int y, int w, int h, int angle) {
		this->x = x;
		this->y = y;
		this->w = w;
		this->h = h;

		objectShape.setPosition({(float) x, (float) y }); 
		objectShape.setSize({ (float) w, (float) h });
		objectShape.setOutlineThickness((float) outlineThickness); 
		objectShape.setRotation((float) angle); 
	}
};


struct Player {

	bool A_pressed = 0;
	bool D_pressed = 0;
	bool S_pressed = 0;
	bool SPACE_pressed = 0;
	bool M1_pressed = 0;

	int x = 0;
	int y = 0;

	int mPosX = 0;
	int mPosY = 0;

	int w = 35; 
	int h = 55; 

	sf::RectangleShape* playerShape;						// The sfml shape of the player 

	int weaponPosX = 10; 									// position of the weapon
	int weaponPosY = 10; 									// position of the weapon
	int weaponRotation = 0;
	int weaponW = 20; 
	int weaponH = 10; 
	sf::RectangleShape* weapon;								// sfml weapon shape 

	Player() {												// constructor: sets size, colour, outline and position of the player & weapon
		playerShape = new sf::RectangleShape({ 35, 55 });
		weapon = new sf::RectangleShape({ 20, 10 });

	}

	void init() {
		playerShape->setFillColor(sf::Color::Magenta);

		playerShape->setOutlineThickness(2);
		playerShape->setOutlineColor(sf::Color::Black);

		weapon->setFillColor(sf::Color::Black);

		weapon->setPosition({ 10, 10 });
	}

	void updateData() {

		playerShape->setPosition({ (float)x, (float)y }); 
		weapon->setPosition({ (float)weaponPosX, (float)weaponPosY });
		weapon->setRotation(weaponRotation); 
	}

};