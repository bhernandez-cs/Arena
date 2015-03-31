#include <iostream>
#include <windows.h>
#include <time.h>
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

const int capStat = 99;
const int capDamage = 9999;
const int capHP = 9999;
int gFile;

struct stats
{
	int strength;
	int ability;
	int reflex;
	int defense;
};

enum enemyType
{
	ENEMY_GOBLIN = 0,
	ENEMY_SKELETON = 1,
	ENEMY_GHOST = 2,
	ENEMY_BANDIT = 3,
	ENEMY_BOSS = 4,
};

class Player
{
public:
	Player();
	void loadStats(string name, stats loadedStats, int level, int exp);
	void changeStats(stats changeStatsBy);
	void changeHealth(int change);
	void incrementLevel(int &exp);
	void printPlayerStats();
	void attack();
	void strongAttack();
	void defend();
	void item();
	void nah();
	void takeAction();
	string getName(){ return playerName; }
	int getLevel(){ return playerLevel; }
	int getExp(){ return playerExperience; }
	int getMaxHP(){ return playerMaxHP; }
	int getCurrentHP(){ return playerCurrentHP; }
	int getStrength(){ return playerStats.strength; }
	int getAbility(){ return playerStats.ability; }
	int getReflex(){ return playerStats.reflex; }
	int getDefense(){ return playerStats.defense; }
	int getWepAtk(){ return playerWepAtk; }
private:
	string playerName;
	stats playerStats;
	int playerWepAtk;
	//Should make a struct, weapon, for weapons, then make an array of weapons that define the name, wep atk, and price buy / price sell
	int playerLevel;
	int playerExperience;
	int playerMaxHP;
	int playerCurrentHP;
	int playerMaxTP;
	int playerCurrentTP;
}gPlayer;

//Define functions.
class Enemy
{
public:
	Enemy();
	int getMaxHP(){ return enemyMaxHP; }
	int getCurrentHP(){ return enemyCurrentHP; }
	int getStrength(){ return enemyStats.strength; }
	int getAbility(){ return enemyStats.ability; }
	int getReflex(){ return enemyStats.reflex; }
	int getDefense(){ return enemyStats.defense; }
	int makeAction();
	void changeCurrentHP(int change);
private:
	stats enemyStats;
	enemyType enemyType;
	int enemyLevel;
	int enemyMaxHP;
	int enemyCurrentHP;
	int enemyExperience;
};


void clearScreen();
bool takeCommand(char check);
void splash();
void loadSave(int file);
void fileLoad();
void fileSave();
void fileSaveChoose();
void fileMenu();
void mainMenu();
void battleArena();
int genAtkDmg(int str, int wepAtk);

//Start of Player Methods

Player::Player()
{
	playerStats = { 10, 10, 10, 10 };
	playerLevel = 1;
	playerMaxHP = 360;
	playerCurrentHP = 360;
}

void Player::loadStats(string name, stats loadedStats, int level, int exp)
{
	playerName = name;
	playerStats = loadedStats;
	playerLevel = level;
	playerExperience = exp;
	int maxHP = static_cast<int>((0.4461*(level*level)) + (55.9651*level) + 86.5889);
	playerMaxHP = maxHP;
	playerCurrentHP = maxHP;
	int maxTP = static_cast<int>((0.0416*(level*level)) + (0.4473*level) + 46.511);
	playerMaxTP = maxTP;
	playerCurrentTP = maxTP;
	playerWepAtk = 10;
	//change this to an actual wep atk loading
}

void Player::changeStats(stats changeStatsBy)
{
	playerStats.strength += changeStatsBy.strength;
	playerStats.ability += changeStatsBy.ability;
	playerStats.reflex += changeStatsBy.reflex;
	playerStats.defense += changeStatsBy.defense;
}


void Player::changeHealth(int change)
{
	playerCurrentHP += change;
}

void Player::incrementLevel(int &exp)
{
	playerLevel += (exp/100);
	exp = exp % 100;
	int playerMaxHP = static_cast<int>((0.4461*(playerLevel*playerLevel)) + (55.9651*playerLevel) + 86.5889);
	playerCurrentHP = playerMaxHP;
	int playerMaxTP = static_cast<int>((0.0416*(playerLevel*playerLevel)) + (0.4473*playerLevel) + 46.511);
	playerCurrentTP = playerMaxTP;
}

void Player::printPlayerStats()
{
	cout << "Name: " << playerName << endl;
	Sleep(333);
	cout << "Level: " << playerLevel << " at " << playerExperience << " exp." << endl;
	Sleep(333);
	cout << "Strength: " << playerStats.strength << endl;
	Sleep(333);
	cout << "Ability: " << playerStats.ability << endl;
	Sleep(333);
	cout << "Reflex: " << playerStats.reflex << endl;
	Sleep(333);
	cout << "Defense: " << playerStats.defense << endl;
	Sleep(333);
	cout << "HP: " << playerCurrentHP << "/" << playerMaxHP << endl;
	Sleep(333);
	cout << "TP: " << playerCurrentTP << "/" << playerMaxTP << endl;
	Sleep(7000);
	clearScreen();
}

//Start of Enemy Methods

Enemy::Enemy()
{
	int levelChange = rand() % 10 - 5;
	enemyLevel = gPlayer.getLevel() + levelChange;
	int statCenter = static_cast<int>((0.6612 * enemyLevel) + 9.3378);
	enemyStats.strength = statCenter + rand() % 10;
	enemyStats.ability = statCenter + rand() % 10;
	enemyStats.reflex = statCenter + rand() % 10;
	enemyStats.defense = statCenter + rand() % 10;
	enemyMaxHP = static_cast<int>((0.68*(enemyLevel*enemyLevel)) + (55.9651*enemyLevel));
	enemyCurrentHP = enemyMaxHP;
	enemyExperience = (100 - enemyLevel) + 20;
	//Need to change around the coefficients in this equation. Begin working on the battle engine.
	//rand +- 10 around generated stat center which is calculated with : .6612x + 9.3378 to generate stats
}

void Enemy::changeCurrentHP(int change)
{
	enemyCurrentHP += change;
}

//Start of General Methods

void clearScreen()
{
	cout << string(100, '\n');
}

char takeCommand()
{
	char userCommand;
	cin >> userCommand;
	if(userCommand < 97)
	{
		userCommand += 32;
	}
	return userCommand;
}

void splash()
{
	cout << "<=====Welcome to ARENA : Alternate Reality Engaging Numerous Apparitions=====>\n";
	Sleep(500);
}

void loadSave(ifstream& stream, string file)
{
	string fileToLoad = "File" + file + ".txt";
	stream.open(fileToLoad);
	string loadedName;
	stats loadedStats;
	int loadedLevel;
	int loadedExp;
	stream >> loadedName;
	stream >> loadedLevel;
	stream >> loadedExp;
	stream >> loadedStats.strength;
	stream >> loadedStats.ability;
	stream >> loadedStats.reflex;
	stream >> loadedStats.defense;
	gPlayer.loadStats(loadedName, loadedStats, loadedLevel, loadedExp);
	stream.close();
}

void fileSave()
{
	ofstream fileStat;
	if (gFile == 1)
	{
		fileStat.open("File1.txt");
	}
	else if (gFile == 2)
	{
		fileStat.open("File2.txt");
	}
	else if (gFile == 3)
	{
		fileStat.open("File3.txt");
	}
	fileStat << gPlayer.getName() << " ";
	fileStat << gPlayer.getLevel() << " ";
	fileStat << gPlayer.getExp() << " ";
	fileStat << gPlayer.getStrength() << " ";
	fileStat << gPlayer.getAbility() << " ";
	fileStat << gPlayer.getReflex() << " ";
	fileStat << gPlayer.getDefense() << " ";
	fileStat.close();
}

void fileSaveChoose()
{
	string name1;
	string name2;
	string name3;
	ifstream fileStat;
	//Name 1 saving
	fileStat.open("File1.txt");
	fileStat >> name1;
	fileStat.close();
	//Name 2 saving
	fileStat.open("File2.txt");
	fileStat >> name2;
	fileStat.close();
	//Name 3 saving
	fileStat.open("File3.txt");
	fileStat >> name3;
	fileStat.close();
	cout << "Please choose which save slot to save to. \n";
	Sleep(500);
	cout << "File 1: " << name1 << "  (Enter '1') \n";
	cout << "File 2: " << name2 << "  (Enter '2') \n";
	cout << "File 3: " << name3 << "  (Enter '3') \n";
	char file = takeCommand();
	while (file <= 80 || file >= 84)
	{
		cout << "Incorrect command. \n";
		file = takeCommand();
	}
	clearScreen();
	if (file == 81)
	{
		cout << "File 1 Selected. \n";
		Sleep(500);
		gFile = 1;
	}
	else if (file == 82)
	{
		cout << "File 2 Selected. \n";
		Sleep(500);
		gFile = 2;
	}
	else if (file == 83)
	{
		cout << "File 3 Selected. \n";
		Sleep(500);
		gFile = 3;
	}
}

void fileLoad()
{
	string name1;
	string name2;
	string name3;
	ifstream fileStat;
	//Name 1 saving
	fileStat.open("File1.txt");
	fileStat >> name1;
	fileStat.close();
	//Name 2 saving
	fileStat.open("File2.txt");
	fileStat >> name2;
	fileStat.close();
	//Name 3 saving
	fileStat.open("File3.txt");
	fileStat >> name3;
	fileStat.close();
	cout << "Please choose which file to load. \n";
	Sleep(500);
	cout << "File 1: " << name1 << "  (Enter '1') \n";
	cout << "File 2: " << name2 << "  (Enter '2') \n";
	cout << "File 3: " << name3 << "  (Enter '3') \n";
	char file = takeCommand();
	clearScreen();
	while (file <= 80 || file >= 84)
	{
		cout << "Incorrect command. \n";
		file = takeCommand();
	}
	if (file == 81)
	{
		cout << "File 1 Selected. \n";
		Sleep(500);
		loadSave(fileStat, "1");
		gFile = 1;
	}
	else if (file == 82)
	{
		cout << "File 2 Selected. \n";
		Sleep(500);
		loadSave(fileStat, "2");
		gFile = 2;
	}
	else if (file == 83)
	{
		cout << "File 3 Selected. \n";
		Sleep(500);
		loadSave(fileStat, "3");
		gFile = 3;
	}
}

void fileMenu()
{
	bool cont = false;
	while (!cont)
	{
		cout << "                              New Game (Enter 'N')" << endl;
		Sleep(500);
		cout << "                              Continue (Enter 'C')" << endl;
		Sleep(1000);
		char menuCom = takeCommand();
		clearScreen();
		if (menuCom == 'n')
		{
			cout << "New Game selected. \n";
			Sleep(1000);
			fileSaveChoose();
			cout << "Please enter your warrior's name. \n";
			Sleep(1000);
			string newName;
			cin >> newName;
			gPlayer.loadStats(newName, { 10, 10, 10, 10 }, 1, 1);
			cont = true;
		}
		else if (menuCom == 'c')
		{
			cout << "Continue selected. \n";
			Sleep(1000);
			fileLoad();
			cont = true;
		}
		else
		{
			cout << "Incorrect command. \n";
		}
	}
	clearScreen();
}

void mainMenu()
{
	bool quit = false;
	while (!quit)
	{
		cout << "Arena ('A') \n";
		Sleep(333);
		cout << "Character Info ('C') \n";
		Sleep(333);
		cout << "Item Shop ('I') \n";
		Sleep(333);
		cout << "Save Game ('S') \n";
		Sleep(333);
		cout << "Exit Game ('E') \n";
		Sleep(333);
		char menuCom = takeCommand();
		clearScreen();
		if (menuCom == 'a')
		{
			cout << "Battle in Arena selected. \n";
			Sleep(1000);
			clearScreen();
			battleArena();
		}
		else if (menuCom == 'c')
		{
			cout << "Character Info selected. \n";
			Sleep(1000);
			clearScreen();
			gPlayer.printPlayerStats();
		}
		else if (menuCom == 's')
		{
			cout << "Save game selected. \n";
			Sleep(1000);
			clearScreen();
			fileSave();
			cout << "Saved game. \n";
		}
		else if (menuCom == 'i')
		{
			cout << "Item Shop selected \n";
			//itemShop();
		}
		else if (menuCom == 'e')
		{
			cout << "Exiting Game \n";
			quit = true;
		}
	}
}

void battleArena()
{
	Enemy challengerE;
	bool dead = false;
	int pCurrentHP;
	int pMaxHP;
	int eCurrentHP;
	int eMaxHP;
	while (!dead)
	{
		pCurrentHP = gPlayer.getCurrentHP();
		pMaxHP = gPlayer.getMaxHP();
		eCurrentHP = challengerE.getCurrentHP();
		eMaxHP = challengerE.getMaxHP();
		cout << "Player HP: " << pCurrentHP << "/" << pMaxHP << endl << endl;
		cout << "Enemy HP: " << eCurrentHP << "/" << eMaxHP << endl;
		char action = takeCommand();
		clearScreen();
		if (action == 'a')
		{
			int playerDamage;
			playerDamage = genAtkDmg(gPlayer.getStrength(), gPlayer.getWepAtk());
			int damageReduc = static_cast<int>(playerDamage*(.35*(challengerE.getDefense() / 100)));
			challengerE.changeCurrentHP(damageReduc - playerDamage);
		}
		else if (action == 's')
		{

		}
		else if (action == 'd')
		{

		}
		else if (action == 'i')
		{

		}
		else if (action == 'n')
		{
			int success = rand() % 100;
			if (success > 49)
			{
				cout << "Successfully escaped! \n";
				Sleep(1500);
				clearScreen();
				mainMenu();
			}
			cout << "Failed to escape! \n";
		}
		else
		{

		}
		if (gPlayer.getCurrentHP() <= 0)
		{
			cout << "Player has been killed. \n";
			dead = true;
		}
		if (challengerE.getCurrentHP() <= 0)
		{
			cout << "Enemy has been killed. \n";
			dead = true;
		}
	}
}

int genAtkDmg(int str, int wepAtk)
{
	int damage;
	damage = ((str * str) + (wepAtk * wepAtk)) - ((str * wepAtk) / 4);
	return damage;
}

//int genPrecAtkDmg(int abi, int wepAtk)
//{
//}

int main(int argc, char* argv[])
{
	srand(time(NULL));
	splash();
	fileMenu();
	mainMenu();
	return 0;
}