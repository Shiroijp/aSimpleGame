//Simple Arduino game

#include <LiquidCrystal.h>
#include <stdlib.h> //RNG purposes
#include <time.h>   //RNG purposes

//Alien definition
byte Alien[8] = {
    B01110,
    B10001,
    B11111,
    B10101,
    B01010,
    B10001,
    B01010,
    B00000
};


struct Joystick
{
	const int Switch_PIN = 8;
	const int X_PIN = 0;
	const int Y_PIN = 1;
};

struct LCD
{
	const int RS = 12;
	const int Enable = 11;
	const int D4 = 5;
	const int D5 = 4;
	const int D6 = 3;
	const int D7 = 2;

	const int columns = 16;
	const int rows = 2;
};

struct Game
{
	bool gameStarted = false;
	bool gameFinished = false;
	int startScreenCount = 0;
	int score = 0;
	int distance = 0;
};

struct Player
{
	int lifes = 3;
	int X = 7;
	int Y = 1;
};

struct Enemy
{
	int character = 183;
	int X = 0;
	int Y = 0;
};

//Objects
Joystick stick;
LCD lcdPins;
LiquidCrystal LCD(lcdPins.RS, lcdPins.Enable, lcdPins.D4, lcdPins.D5, lcdPins.D6, lcdPins.D7);

Game game1;
Player player1;
Enemy enemy1;


void setup()
{
	Serial.begin(9600);

	pinMode(stick.Switch_PIN, INPUT);
	digitalWrite(stick.Switch_PIN, HIGH);

	LCD.begin(lcdPins.columns, lcdPins.rows);

	printStartScreen();
	
}

void loop()
{
	if (!game1.gameStarted)
	{
		if (!digitalRead(stick.Switch_PIN))
		{//Start game
			delay(500);
			game1.gameStarted = true;
			printPlayer();
			createAlien();
			printAlien();


		}
		else
		{//Game Menu
			printStartScreen();
		}
	}
	else
	{//Game running
		if (!digitalRead(stick.Switch_PIN))
		{//End game
			printScoreScreen();
			delay(5000);
			game1.gameFinished = true;
			game1.gameStarted = false;
			Serial.println("Partida finalizada.");
		}
		else
		{//Game core
			movePlayer();
			checkCollision();
		}
	}


	//Debug	
	Serial.print("Joystick lectures: X->");
    Serial.print(X_Axys());
	Serial.print(" Y->");
	Serial.print(analogRead(stick.Y_PIN));
	Serial.print(" Switch->");
	Serial.print(digitalRead(stick.Switch_PIN));
	Serial.print(" Player_X->");
	Serial.print(player1.X);
	Serial.print(" Player_Y->");
	Serial.println(player1.Y);

	if (game1.gameStarted)
	{
		delay(16);
	}
	else
	{
		delay(100);
	}
}

int X_Axys()
{
	return analogRead(stick.X_PIN);
}

int Y_Axys()
{
	return analogRead(stick.Y_PIN);
}

void printPlayer()
{
	LCD.clear();
	LCD.setCursor(player1.X, player1.Y);
	LCD.write(255);
}

void movePlayer()
{
	if (X_Axys() > 600)
	{
		//Move right
		if (player1.X + 1 <= lcdPins.columns - 1)
		{
			player1.X += 1;
			printPlayer();
			printAlien();
			game1.distance += 1;
		}
	}
	else if (X_Axys() < 400)
	{
		//Move left
		if (player1.X - 1 >= 0)
		{			
			player1.X -= 1;
			printPlayer();
			printAlien();
			game1.distance += 1;
		}
	}

	if (Y_Axys() > 600)
	{
		//Move Down
		if (player1.Y == 1)
		{
			player1.Y -= 1;
			printPlayer();
			printAlien();
			game1.distance += 1;
		}

	}
	else if (Y_Axys() < 400)
	{
		//Move UP
		if (player1.Y == 0)
		{
			player1.Y += 1;
			printPlayer();
			printAlien();
			game1.distance += 1;
		}
	}
}

void createAlien()
{
	srand (time(NULL));

	int X = random(0, 16);
	int Y = random(0, 2);

	enemy1.X = X;
	enemy1.Y = Y;

	Serial.println(X);
	Serial.println(Y);
}

void printAlien()
{
	LCD.setCursor(enemy1.X, enemy1.Y);
	LCD.write(enemy1.character);
}

void checkCollision()
{
	if ((player1.X == enemy1.X) && (player1.Y == enemy1.Y))
	{
		printPlayer();
		createAlien();
		printAlien();
		Serial.print("Kill!");

		game1.score += 1;
	}
}

void printStartScreen()
{
	LCD.setCursor(0,0);
	LCD.print(" A simple Game");

	Serial.println(game1.startScreenCount);
	game1.startScreenCount += 50;
	if (game1.startScreenCount > 1000)
	{
		game1.startScreenCount = 0;
	}
	LCD.setCursor(0,1);

	if (game1.startScreenCount < 750)
	{

		LCD.print("Click to start..");
	}
	else
	{
		LCD.print("                ");
	}
}

void printScoreScreen()
{
	LCD.clear();

	LCD.setCursor(0,0);
	LCD.print("Kills: ");
	LCD.setCursor(7, 0);
	LCD.print(game1.score);

	LCD.setCursor(0,1);
	LCD.print("Km: ");
	LCD.setCursor(4, 1);
	LCD.print(game1.distance);
}