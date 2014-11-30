//-------------------------------------------------------------
//Console Combat - What programmers do when they're bored
//
//Version:  	ALPHA-NOVEMBER-2012
//Author:		Andrew Pinion
//Website:		andrewpinion.com
//
//This program is FOSS (free open source software). It may
//be edited and redistrubuted by anyone, provided that
//this header is included without any modification.
//
//This program is provided "AS IS", with no warranty
//of any kind, expressed or implied.
//-------------------------------------------------------------

//---------------
//Directives
//---------------

#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <time.h>
using namespace std;

//-----------------------
//Declarations Block
//-----------------------

//player structure
typedef struct
{
	string name;
	int hp;
	int sp;
	int str;
	int def;
	int speed;
	int dex;
	
	int statPts;
	int superPts;
}player;

typedef struct
{
	int numPlayers;
	int isPlayer[4];
	int referenceID[4];
	int isAlive[4];
	int hasItem[4];
	int randomChar[4];
	int numAlivePlayers;
	int winner;
}battle;

typedef struct
{
	string name;
}randPlayer;

//function prototypes
void battleSetup();
void battleMainLoop();
void drawLine();

void createPlayer();
void listPlayers(int);
void updateStats(int);

int attack(int, int);
int randInt(int);
void defend(int);
void randomBattleSetup();
void communism();

//structure pointers
player *savedPlayers;
player *savedCpuPlayers;
player *players;
randPlayer *nameDB;
battle battleData;

//global vars
int playerStates[4];
int numPlayers = 4;
int numSavedPlayers = 0;
int activeUser = 0;
int messageID;
int superUpdated;
int always = 1;
int totalNames;  //total random names
int battleType = 0;

//--------------------
//File Processing
//--------------------

void loadNames()
{
	int i;
	//open the file
	ifstream inputFile;
	inputFile.open("namedb.txt");
	
	inputFile >> totalNames;
	nameDB = new randPlayer[totalNames + 2];
	
	//write output
	for (i = 0; i < totalNames; i++)
	{
		getline (inputFile, nameDB[i].name);
	}
	//close the file
	inputFile.close();
}

void loadData()
{
	int i;
	//open the file
	ifstream inputFile;
	inputFile.open("playerdb.txt");
	
	//write output
	for (i = 0; i < 4; i++)
	{
		getline (inputFile, savedPlayers[i].name);
		inputFile >> savedPlayers[i].hp;
		inputFile >> savedPlayers[i].sp;
		inputFile >> savedPlayers[i].str;
		inputFile >> savedPlayers[i].def;
		inputFile >> savedPlayers[i].speed;
		inputFile >> savedPlayers[i].dex;
		inputFile >> savedPlayers[i].statPts;

		char next;
		while(inputFile.get(next))
			if (next == '\n') 
				break;        
	}
	
	//close the file
	inputFile.close();
}

void loadCpuData()
{
	int i;
	//open the file
	ifstream inputFile;
	inputFile.open("cpuplayerdb.txt");
	
	//write output
	for (i = 0; i < 10; i++)
	{
		getline (inputFile, savedCpuPlayers[i].name);
		inputFile >> savedCpuPlayers[i].hp;
		inputFile >> savedCpuPlayers[i].sp;
		inputFile >> savedCpuPlayers[i].str;
		inputFile >> savedCpuPlayers[i].def;
		inputFile >> savedCpuPlayers[i].speed;
		inputFile >> savedCpuPlayers[i].dex;

		char next;
		while(inputFile.get(next))
			if (next == '\n') 
				break;        
	}
	
	//close the file
	inputFile.close();
}

void saveData()
{
	int i;
	//open the file
	ofstream outputFile;
	outputFile.open("playerdb.txt", ios::trunc);
	
	//write output
	for (i = 0; i < 4; i++)
	{
		outputFile << savedPlayers[i].name;
		outputFile << endl;
		outputFile << savedPlayers[i].hp << " ";
		outputFile << savedPlayers[i].sp << " ";
		outputFile << savedPlayers[i].str << " ";
		outputFile << savedPlayers[i].def << " ";
		outputFile << savedPlayers[i].speed << " ";
		outputFile << savedPlayers[i].dex << " ";
		outputFile << savedPlayers[i].statPts << " ";
		outputFile << endl;
	}
	
	//close the file
	outputFile.close();
}

//--------------
//main menu
//--------------

void showMenu()
{
	char select;
	while (always)
	{
		system("cls");
		cout << "-------------------------\n";
		cout << "Ranked Games\n";
		cout << "-------------------------\n";
		cout << "1: Start New Game\n";
		cout << "2: Create Player\n";
		cout << "3: List all Players\n";
		cout << "-------------------------\n";
		cout << "Quick Play\n";
		cout << "-------------------------\n";
		cout << "4:Generate Random Game\n";
		cout << endl;
		cout << "[DISABLED] Settings\n";
		cout << "5: Exit\n\n";
		cout << ">";
		cin >> select;
		
		if (select == '1') { battleSetup(); }
		if (select == '2') { createPlayer(); }
		if (select == '3') { listPlayers(2); system("pause"); }
		if (select == '4') { randomBattleSetup(); }
		if (select == '5') { saveData(); system("cls"); exit(0); }
	}
}

//------------------------
//player creation
//------------------------

void initialize(int i)
{
	//initialize arrays to default values for all saved players
	savedPlayers[i].name = "empty slot";
	savedPlayers[i].hp = 100;
	savedPlayers[i].sp = 10;
	savedPlayers[i].str = 5;
	savedPlayers[i].def = 5;
	savedPlayers[i].speed = 5;
	savedPlayers[i].dex = 5;
	savedPlayers[i].statPts = 5;
}

void cpuInitialize(int i)
{
	//initialize arrays to default values for all saved players
	savedCpuPlayers[i].name = "empty slot";
	savedCpuPlayers[i].hp = 100;
	savedCpuPlayers[i].sp = 10;
	savedCpuPlayers[i].str = 5;
	savedCpuPlayers[i].def = 5;
	savedCpuPlayers[i].speed = 5;
	savedCpuPlayers[i].dex = 5;
	savedCpuPlayers[i].statPts = 5;
}

void createPlayer()
{
	int selection = 0;
	int selection2 = 0;
	string charName;
	char menuSelection;
	
	while (always)
	{
		system("cls");
		cout << "------------------------------------\n";
		cout << "           Create Player\n";
		cout << "------------------------------------\n";
		cout << endl;
		
		listPlayers(0);

		cout << "\n>";
		cin >> menuSelection;
		
		if (menuSelection == '0') { selection = 0; break; }
		if (menuSelection == '1') { selection = 1; break; }	
		if (menuSelection == '2') { selection = 2; break; }	
		if (menuSelection == '3') { selection = 3; break; }	
	}
	
	if (savedPlayers[selection].name != "empty slot")
	{
		cout << "slot in use. Type 1 to proceed,\n or any other key to cancel." << endl << ">";
		cin >> selection2;
		if (selection2 != 1) return;
	}
	
	else
		numSavedPlayers++;
	
	do
	{
		cout << "\nEnter a name:";
		cin >> charName;
		//if (charName == "empty slot") { cout << "\nOne step ahead of you."; exit(0); }
	}
	while (charName == "empty slot");
	
	initialize(selection);
	savedPlayers[selection].name = charName;
	
	updateStats(selection);
}

void showStats(int selection)
{
	cout << "------------------------------------\n";
	cout << "Player:" << savedPlayers[selection].name << endl;
	cout << "HP:" << savedPlayers[selection].hp << endl;
	cout << "SP:" << savedPlayers[selection].sp << endl;
	cout << "STR:" << savedPlayers[selection].str << endl;
	cout << "DEF:" << savedPlayers[selection].def << endl;
	cout << "SPEED:" << savedPlayers[selection].speed << endl;
	cout << "DEX:" << savedPlayers[selection].dex << endl;
	cout << "------------------------------------\n";
	cout << "Stat Points Remaining:" << savedPlayers[selection].statPts << endl;
	cout << "------------------------------------\n";
}

void updateStats(int selection)
{
	int selection2 = 0;
	while (savedPlayers[selection].statPts > 0)
	{
		system("cls");
		showStats(selection);
		cout << "1: HP + 10\n";
		cout << "2: SP + 1\n";
		cout << "3: STR + 1\n";
		cout << "4: DEF + 1\n";
		cout << "5: SPEED + 1\n";
		cout << "6: DEX + 1\n";
		cout << ">";
		cin >> selection2;
		
		if (selection2 == 1) { savedPlayers[selection].statPts--; savedPlayers[selection].hp += 10; }
		if (selection2 == 2) { savedPlayers[selection].statPts--; savedPlayers[selection].sp += 1; }
		if (selection2 == 3) { savedPlayers[selection].statPts--; savedPlayers[selection].str += 1; }
		if (selection2 == 4) { savedPlayers[selection].statPts--; savedPlayers[selection].def += 1; }
		if (selection2 == 5) { savedPlayers[selection].statPts--; savedPlayers[selection].speed += 1; }
		if (selection2 == 6) { savedPlayers[selection].statPts--; savedPlayers[selection].dex += 1; }
	}
}

//--------------------
//listing players
//--------------------

void listPlayers(int status)
{
	int i;
	if (status == 2)
	{
		system("cls");
		drawLine();
		cout << endl << "Saved Players";
		drawLine();
		status = 1;
		cout << endl;
	}

	
	if (status == 1)
	{
		for(i = 0; i < numPlayers; i++)
		{
			if (savedPlayers[i].name == "empty slot")
				cout << i << ": " << setw(15) << savedPlayers[i].name << endl;
			else
			{
				cout << i << ": " << setw(15) << savedPlayers[i].name << " ";
				cout <<  "HP:" <<setw(3) << savedPlayers[i].hp << " ";
				cout << "SP:" << setw(3) << savedPlayers[i].sp << " ";
				cout << "STR:" << setw(3) << savedPlayers[i].str << " ";
				cout << "DEF:" << setw(3) << savedPlayers[i].def << " ";
				cout << "SPD:" << setw(3) << savedPlayers[i].speed << " ";
				cout << "DEX:" << setw(3) << savedPlayers[i].dex << " ";
				cout << "EXP:" << setw(3) << savedPlayers[i].statPts << endl;
			}
		}
	}
	
	
	else
	{
		for(i = 0; i < numPlayers; i++)
			cout << i << ": " << savedPlayers[i].name << endl;
	}
}

void listCpuPlayers(int status)
{
	int i;
	cout << endl;
	if (status == 1)
	{
		for(i = 0; i < 10; i++)
		{
			if (savedCpuPlayers[i].name == "empty slot")
				cout << i << ": " << setw(15) << savedCpuPlayers[i].name << endl;
			else
			{
				cout << i << ": " << setw(15) << savedCpuPlayers[i].name << " ";
				cout << "HP:" << savedCpuPlayers[i].hp << " ";
				cout << "SP:" << savedCpuPlayers[i].sp << " ";
				cout << "STR:" << savedCpuPlayers[i].str << " ";
				cout << "DEF:" << savedCpuPlayers[i].def << " ";
				cout << "SPD:" << savedCpuPlayers[i].speed << " ";
				cout << "DEX:" << savedCpuPlayers[i].dex << endl;
			}
		}
	}
	
	else
	{
		for(i = 0; i < numPlayers; i++)
			cout << i << ": " << savedPlayers[i].name << endl;
	}
}

//---------------------------
//battle setup functions
//---------------------------

void drawLine()
{
	cout << "\n---------------------------------------------------------------------------";
}

//this function should only be called AFTER all players have been configured!
void generateRandomChar(int i)
{
	int bestPlayerEXP = 0;
	int curPlayerEXP = 0;
	int random = 0;
	int selection = 0;
	
	//give the player a personality
	selection = randInt(totalNames);
	players[i].name = nameDB[selection].name;
	players[i].superPts = 0;

	//set the num of pts to be distributed to equal the num of pts the strongest player has
	for (int j = 0; j < battleData.numPlayers; j++)
	{
		if (battleData.isPlayer[j] == 1)
		{
			curPlayerEXP = (players[j].hp - 100) / 10;
			curPlayerEXP += players[j].sp - 10;
			curPlayerEXP += players[j].str - 5;
			curPlayerEXP += players[j].def - 5;
			curPlayerEXP += players[j].speed - 5;
			curPlayerEXP += players[j].dex - 5;
			if (curPlayerEXP > bestPlayerEXP) bestPlayerEXP = curPlayerEXP;

		}
	}
	//bestPlayerEXP += 2;
	
	//if no players exist, randomize the numer of points
	if (bestPlayerEXP == 0) bestPlayerEXP = 5 * randInt(bestPlayerEXP);
	
	//initialize the player's storage with the default values
	players[i].hp = 100;
	players[i].sp = 10;
	players[i].str = 5;
	players[i].def = 5;
	players[i].speed = 5;
	players[i].dex = 5;
	
	//distribute the points to the new character randomly
	for (int j = 0; j < bestPlayerEXP; j++)
	{
		random = randInt(1500);
		if (random <= 250) 								players[i].hp 		+= 10;
		else if (random > 250 && random <= 500) 		j--; //players[i].sp 		+= 1;
		else if (random > 500 && random <= 750) 		players[i].str 		+= 1;
		else if (random > 750 && random <= 1000)		players[i].def 		+= 1;
		else if (random > 1000 && random <= 1250)		players[i].speed 	+= 1;	
		else if (random > 1250)							players[i].dex 		+= 1;
	}

}

void randomBattleSetup()
{
	int selection;
	int statPoints = 10;
	int random;
	int temp;
	players = new player[4];
	
	statPoints = 5 * randInt(statPoints);
	
	for (int i=0; i < 4; i++)
	{
		selection = randInt(totalNames);
		players[i].name = nameDB[selection].name;
		battleData.isAlive[i] = 1;
		battleData.referenceID[i] = -1;
		temp = statPoints;
		
		players[i].hp = 200;
		players[i].sp = 10;
		players[i].str = 5;
		players[i].def = 5;
		players[i].speed = 5;
		players[i].dex = 5;
				
		for (int j = 0; j < temp; j++)
		{
			random = randInt(1500);
			if (random <= 250) 								players[i].hp 		+= 10;
			else if (random > 250 && random <= 500) 		players[i].sp 		+= 2;
			else if (random > 500 && random <= 750) 		players[i].str 		+= 1;
			else if (random > 750 && random <= 1000)		players[i].def 		+= 1;
			else if (random > 1000 && random <= 1250)		players[i].speed 	+= 1;	
			else if (random > 1250)							players[i].dex 		+= 1;
		}
		players[i].superPts = 0;
	}
	battleData.numAlivePlayers = 4;
	battleData.numPlayers = 4;
	battleData.isPlayer[0] = 1;
	battleData.isPlayer[1] = 0;
	battleData.isPlayer[2] = 0;
	battleData.isPlayer[3] = 0;
	
	battleData.hasItem[0] = 1;
	battleData.hasItem[1] = 1;
	battleData.hasItem[2] = 1;
	battleData.hasItem[3] = 1;
	
	battleData.winner = -1;
	battleType = 2;
	battleMainLoop();
}

void battleSetup()
{
	//declarations
	char menuSelection;
	int selection;
	int i;
	
	//select the number of players.
	while (always)
	{
		system("cls");
		cout << "\nSelect Mode\n-----------";
		cout << "\n1: Two Fighter Match";
		cout << "\n2: Three Fighter Match";
		cout << "\n3: Four Fighter Match\n>";
		cin >> menuSelection;
		
		if (menuSelection == '1') { selection = 1; battleData.numPlayers = 2; break; }
		if (menuSelection == '2') { selection = 2; battleData.numPlayers = 3; break; }
		if (menuSelection == '3') { selection = 3; battleData.numPlayers = 4; break; }
	}

	battleData.numAlivePlayers = battleData.numPlayers;
	
	for (int i = 0; i < 4; i++) { battleData.isAlive[i] = 0; battleData.randomChar[i] = 0; }
	
	//setup each player, one per for() iteration.
	for(int i = 0; i < battleData.numPlayers; i++)
	{
		battleData.isAlive[i] = 1;
		
		while (always)
		{
			system("cls");
			drawLine();
			cout << "\n Player " << i << " setup";
			drawLine();
			cout << "\n1: Saved Human Player";
			cout << "\n2: Standard CPU Player";
			cout << "\n3: Unknown CPU Player\n>";
			cin >> menuSelection;
			
			if (menuSelection == '1') { selection = 1; break; }
			if (menuSelection == '2') { selection = 2; break; }
			if (menuSelection == '3') { selection = 3; break; }
		}
		
		//setup a new human fighter
		if (selection == 1) 
		{
			battleData.isPlayer[i] = 1;
			
			while (always)
			{	
				system("cls");
				drawLine();
				cout << "\n Choose player " << i+1 << "'s Fighter";
				drawLine(); cout << endl;
				listPlayers(1);
				cin >> menuSelection;

				if (menuSelection == '0' && savedPlayers[0].name != "empty slot") { selection = 0; break; }				
				if (menuSelection == '1' && savedPlayers[1].name != "empty slot") { selection = 1; break; }
				if (menuSelection == '2' && savedPlayers[2].name != "empty slot") { selection = 2; break; }
				if (menuSelection == '3' && savedPlayers[3].name != "empty slot") { selection = 3; break; }
			}
			//while ((selection < 0) && (selection > numPlayers) && (savedPlayers[selection].name == "empty slot"));

			//store the selected player's stats in a temp object
			battleData.referenceID[i] = selection;
			players[i].name = 	savedPlayers[selection].name;
			players[i].hp = 	savedPlayers[selection].hp;
			players[i].sp = 	savedPlayers[selection].sp;
			players[i].str = 	savedPlayers[selection].str;
			players[i].def = 	savedPlayers[selection].def;
			players[i].speed = 	savedPlayers[selection].speed;	
			players[i].dex = 	savedPlayers[selection].dex;
			players[i].superPts = 0;
			battleData.hasItem[i] = 1;
		}
		
		//setup a new cpu fighter
		else if (selection == 2) 
		{
			battleData.isPlayer[i] = 0;

			while (always)
			{	
				system("cls");
				drawLine();
				cout << "\n Choose player " << i+1 << "'s Fighter";
				drawLine(); cout << endl;
				listCpuPlayers(1);
				cin >> menuSelection;
				
				if (menuSelection == '0' && savedCpuPlayers[0].name != "empty slot") { selection = 0; break; }
				if (menuSelection == '1' && savedCpuPlayers[1].name != "empty slot") { selection = 1; break; }	
				if (menuSelection == '2' && savedCpuPlayers[2].name != "empty slot") { selection = 2; break; }	
				if (menuSelection == '3' && savedCpuPlayers[3].name != "empty slot") { selection = 3; break; }
				if (menuSelection == '4' && savedCpuPlayers[4].name != "empty slot") { selection = 4; break; }	
				if (menuSelection == '5' && savedCpuPlayers[5].name != "empty slot") { selection = 5; break; }	
				if (menuSelection == '6' && savedCpuPlayers[6].name != "empty slot") { selection = 6; break; }	
				if (menuSelection == '7' && savedCpuPlayers[7].name != "empty slot") { selection = 7; break; }	
				if (menuSelection == '8' && savedCpuPlayers[8].name != "empty slot") { selection = 8; break; }
				if (menuSelection == '9' && savedCpuPlayers[0].name != "empty slot") { selection = 9; break; }	
			}
			//while ((selection < 0) || (selection > 10) || (savedCpuPlayers[selection].name == "empty slot"));
			
			//store the selected player's stats in a temp object
			battleData.referenceID[i] = selection;
			players[i].name = 	savedCpuPlayers[selection].name;
			players[i].hp = 	savedCpuPlayers[selection].hp;
			players[i].sp = 	savedCpuPlayers[selection].sp;
			players[i].str = 	savedCpuPlayers[selection].str;
			players[i].def = 	savedCpuPlayers[selection].def;
			players[i].speed = 	savedCpuPlayers[selection].speed;	
			players[i].dex = 	savedCpuPlayers[selection].dex;
			players[i].superPts = 0;
		}
		
		else if (selection == 3)
		{
			//flag this char to randomly generate a CPU Player
			battleData.randomChar[i] = 1;
			battleData.isPlayer[i] = 0;
			battleData.referenceID[i] = -1;
		}
	}
	//generate your random chars here
	for (int i = 0; i < battleData.numPlayers; i++)
		if (battleData.randomChar[i]) generateRandomChar(i);
	battleType = 1;
	battleMainLoop();
}

void loadCpuPlayer()
{/*
	if (selection == 1)
	{
		players[1].name = "Buckethead";
		players[1].hp = 100;
		players[1].sp = 10;
		players[1].str = 5;
		players[1].def = 5;
		players[1].speed = 5;
		players[1].dex = 5;
	}
	if (selection == 2)
	{
		players[1].name = "ManBearPig";
		players[1].hp = 150;
		players[1].sp = 10;
		players[1].str = 5;
		players[1].def = 5;
		players[1].speed = 5;
		players[1].dex = 5;
	}
	if (selection == 3)
	{
		players[1].name = "Rocketman";
		players[1].hp = 100;
		players[1].sp = 10;
		players[1].str = 15;
		players[1].def = 5;
		players[1].speed = 5;
		players[1].dex = 5;
	}
	if (selection == 4)
	{
		players[1].name = "Golfer";
		players[1].hp = 100;
		players[1].sp = 10;
		players[1].str = 5;
		players[1].def = 20;
		players[1].speed = 5;
		players[1].dex = 5;
	}
	if (selection == 5)
	{
		players[1].name = "RoboMallCop";
		players[1].hp = 150;
		players[1].sp = 15;
		players[1].str = 10;
		players[1].def = 10;
		players[1].speed = 5;
		players[1].dex = 5;
	}
	players[1].superPts = 0;*/
}

//--------------------------------
//battle processing functions
//--------------------------------

void dispSuperGuage(int i)
{
	cout << " SUPER:[";
	if (players[i].superPts >= 5) cout << "-----]";
	if (players[i].superPts == 4) cout << "---- ]";
	if (players[i].superPts == 3) cout << "---  ]";
	if (players[i].superPts == 2) cout << "--   ]";
	if (players[i].superPts == 1) cout << "-    ]";
	if (players[i].superPts == 0) cout << "     ]";
	cout << endl;
}
void dispStatus(int i)
{
	cout << setw(15) << players[i].name << " ";
	if (players[i].hp <= 0) cout << "<KO>" << endl;
	else
	{
		cout << "HP:" << setw(3) << players[i].hp << " ";
		cout << "SP:" << setw(3) << players[i].sp << " ";
		cout << "STR:" << setw(3) << players[i].str << " ";
		cout << "DEF:" << setw(3) << players[i].def << " ";
		cout << "SPD:" << setw(3) << players[i].speed << " ";
		cout << "DEX:" << setw(3) << players[i].dex;
		dispSuperGuage(i);
	}
}

void showItemMenu()
{

}

void drawBattleHeader()
{
	cout << "---------------------------------------------------------------------------\n";
	dispStatus(0);
	dispStatus(1);
	if (battleData.numPlayers >= 3) dispStatus(2);
	if (battleData.numPlayers == 4) dispStatus(3);
	cout << "---------------------------------------------------------------------------";
}

void drawBattleMenu()
{
	cout << endl << players[activeUser].name << "'s turn\n";
	cout << "---------------------------------------------------------------------------";
	cout << "\n1:Attack";
	if (battleData.hasItem[activeUser]) cout << "\n2:Items"; else cout << "\n<no items>";
	cout << "\n3:Defend";
	if (players[0].sp <= 6 )cout << "\n4:Rest";
	if (players[0].superPts >= 5) cout << "\n5: Super Attack";
	//else cout << "\n[Locked] Super Attack";
	cout << endl << ">";
}

int randInt(int range)
{ 
	int random;
	random = rand() % range + 1;
	return random;
}

void superPoint(int i)
{
	int rand;
	rand = randInt(2);
	if ((rand == 1) && (players[i].superPts != 5))
	{
		players[i].superPts++;
		superUpdated = i;
		messageID = 1;
		//cout << endl << players[i].name <<  " earned a super point.";
	}
}

//int numPlayers = 0;
//bool isPlayer[4];


int chooseTarget()
{
	int selection; 
	int rand;
	
	if (battleData.isPlayer[activeUser] && (battleData.numPlayers > 2))
	{
		do
		{
			cout << "\n---------------";
			cout << "\nSelect a Target";
			cout << "\n---------------";
			for(int i=0; i< battleData.numPlayers; i++)
				if (activeUser != i && battleData.isAlive[i]) cout << endl << i << " " << players[i].name;
			cout << "\n>";
			cin >> selection;
		}
		while (selection > battleData.numPlayers || selection < 0 || selection == activeUser || battleData.isAlive[selection] == 0);
	}
	else if (battleData.isPlayer[activeUser] && (battleData.numPlayers = 2))
	{
		if (activeUser == 1) selection = 0;
		if (activeUser == 0) selection = 1;
	}
	else
	{
		selection = activeUser;
		do
		{
			rand = randInt(1000);
			if (battleData.numPlayers == 2 && activeUser == 0) selection = 1; 
			else if (battleData.numPlayers == 2 && activeUser == 1) selection = 0;
			
			else if (battleData.numPlayers == 3 && rand <= 333 && battleData.isAlive[0]) selection = 0; 
			else if (battleData.numPlayers == 3 && rand > 333 && rand <= 666 && battleData.isAlive[1]) selection = 1; 
			else if (battleData.numPlayers == 3 && rand > 666 && battleData.isAlive[2]) selection = 2; 
			
			else if (battleData.numPlayers == 4 && rand <= 250 && battleData.isAlive[0]) selection = 0;
			else if (battleData.numPlayers == 4 && rand > 250 && rand <= 500 && battleData.isAlive[1]) selection = 1; 
			else if (battleData.numPlayers == 4 && rand > 500 && rand <= 750 && battleData.isAlive[2]) selection = 2; 
			else if (battleData.numPlayers == 4 && rand > 750 && battleData.isAlive[3]) selection = 3; 
			//cout << "\n attacker is " << activeUser << "selection is: " << selection; system("pause");
			
		}
		while (selection == activeUser);
	}
	return selection;
}

void battleMainLoop()
{
	int selection;
	int damage;
	int target;
	activeUser = 0;
	
	do
	{
		//reset variables for the turn
		messageID = 0;
		superUpdated = 0;
		damage = 0;
		selection = -1;
		target = -1;
		
		//clear defend state if it is active
		if (playerStates[activeUser] == 1) 
		{
			playerStates[activeUser] = 0;
			players[activeUser].def /= 2; 
		}
		
		//player turn
		if (battleData.isPlayer[activeUser] && battleData.isAlive[activeUser])
		{
			do
			{
				system("cls");
				drawBattleHeader();
				drawBattleMenu();
				cin >> selection;
				if (selection == 2 && battleData.hasItem[activeUser] != 1) selection = -1;
				if (selection == 5 && players[activeUser].superPts < 5) selection = -1;
				if (players[activeUser].sp > 6 && selection == 4) selection = -1;
			}
			while (selection < 1 || selection > 5);
		}
		
		//cpu turn
		else if (battleData.isAlive[activeUser])
		{	
			system("cls");
			drawBattleHeader();

			if (players[activeUser].superPts == 5)
				selection = 5;
			else if (players[activeUser].sp < 2)
				selection = 4;
			else 
				selection = 1;
		}
		
		//standard attack handler
		if (selection == 1 && players[activeUser].sp >= 2) 
		{
			target = chooseTarget();
			damage = attack(activeUser,target);
			
			//successful hit of standard attack
			if (damage > 0)
			{
				players[target].hp -= damage;
				players[activeUser].sp -= 2;
				
				system("cls");
				superPoint(activeUser);
				drawBattleHeader();
				cout << endl << players[activeUser].name << " deals " << damage << " damage to " << players[target].name << "!";
			}
			
			//successful block of attack
			if (damage == 0)
			{
				system("cls");
				players[target].def -= 1;
				players[activeUser].sp -= 2;
				superPoint(target);
				drawBattleHeader();
				cout << endl << players[target].name << "'s defense absorbed " << players[activeUser].name << "'s attack!";
			}
			
			//successful dodge
			if (damage == -3)
			{
				players[target].speed -= 1;
				players[activeUser].sp -= 2;
				superPoint(target);
				
				system("cls");
				drawBattleHeader();
				cout << endl << players[target].name << " dodged " << players[activeUser].name << "'s attack!";

			}
		}
		
		if (selection == 2)
		{	
			communism();
			battleData.hasItem[activeUser] = 0;
			system("cls");
			drawBattleHeader();
			messageID = 2;
		}
		
		//defend handler
		if (selection == 3)
		{
			playerStates[activeUser] = 1;
			players[activeUser].def *= 2;
			
			system("cls");
			drawBattleHeader();
			players[activeUser].sp -= 1;
			cout << endl << players[activeUser].name << " defends!";
		}
		
		//rest handler
		if (selection == 4)
		{
			players[activeUser].sp += 3;
			system("cls");
			drawBattleHeader();
			cout << endl << players[activeUser].name << " rests to restore stamina!";
//set speed to 0 with a state
		}
		
		//super handler
		if (selection == 5) 
		{
			target = chooseTarget();
			damage = attack(activeUser,target);
			if (damage < 8) damage = 8;
			players[target].hp -= damage * 6;
			players[activeUser].sp = 0;
				
			system("cls");
			players[activeUser].superPts = 0;
			drawBattleHeader();
			cout << endl << players[activeUser].name << " used super attack [Multi-Hit] on " << players[target].name << "!";
			cout << endl << damage*6 << " [" << damage << " * 6] damage done to " << players[target].name << "!";
			players[activeUser].str = players[activeUser].str * 0.8;
		}
		
		//KO handler
		if (players[target].hp <= 0 && target != -1)
		{
			cout << endl << players[target].name << " has been defeated by " << players[activeUser].name << "!";
			battleData.isAlive[target] = 0;
			battleData.numAlivePlayers--;
			players[activeUser].superPts++;
			
			//stat point update
			if (battleData.isPlayer[activeUser])
			{
				if (battleType == 1) cout << endl << players[activeUser].name << " earned a stat point!";
				savedPlayers[battleData.referenceID[activeUser]].statPts++;
			}
			
			//battle win condition
			if (battleData.numAlivePlayers == 1)
			{ 
				cout << endl << players[activeUser].name << " has won the game!";
				battleData.winner = activeUser;
				break;
			}
		}
		
		//message handling
		if (messageID == 1)	cout << endl << players[superUpdated].name <<  "'s super charge increased!";
		if (messageID == 2)	cout << endl << players[superUpdated].name <<  " used some communism!\n All remaining HP divided evenly!";
		if (players[activeUser].sp < 2 && damage == 0 && battleData.isAlive[activeUser] && playerStates[activeUser] == 0) cout << endl << players[activeUser].name << " doesn't have enough stamina to attack!";
		
		//the turn is over, move on to the next player.
		if (battleData.isAlive[activeUser]) players[activeUser].sp++;
		if (activeUser == battleData.numPlayers - 1) activeUser = 0;
		else activeUser++;
		if (battleData.isAlive[activeUser]) drawLine();
		cout << endl;
		if (battleData.isAlive[activeUser]) system("pause");
	}
	while (1 != 2);
	
	if (battleType == 1) 
	{
		//levelup handler
		if (savedPlayers[battleData.referenceID[battleData.winner]].statPts >= 5 && battleData.isPlayer[activeUser])
		{	
			cout << endl << players[battleData.winner].name << " has leveled up!";
			drawLine();	cout << endl;
			system("pause");
			updateStats(battleData.winner);
			saveData();
		}
		
		//normal end handler
		else
		{
			drawLine();	cout << endl;
			system("pause");
		}
	}
}

//---------------------------------
//battle action functions
//---------------------------------

int attack(int user, int target)
{
	if (players[user].sp >= 2)
	{
		int randDamage = randInt(20);
		
		int randSpeed = randInt(players[target].speed);
		if (randSpeed < (players[target].speed * 0.75)) randSpeed = (players[target].speed * 0.75);
		randSpeed = randSpeed + (randInt(players[target].speed) / 4);
		
		int randDex = randInt(players[user].dex);
		if (randDex < (players[user].dex * 0.75)) randDex = (players[user].dex * 0.75);
		randDex = randDex + (randInt(players[user].dex) / 4);

		int damage = 0;
		if (randDex > randSpeed)
		{	
			damage = randDamage + (players[user].str - players[target].def);
			if (damage <= 0) damage = 0;
				return damage;
		}
		else
		{
			return -3;
		}
			
	}
	else
		//not enough sp
		return -2;
}

void defend(int user)
{
	players[user].def *= 2;
}

void unDefend(int user)
{
	players[user].def /= 2;
}

void communism()
{
	//calculate newHP
	int newHP;
	if (battleData.numAlivePlayers == 2)	newHP = (players[0].hp + players[1].hp) / 2;
	if (battleData.numAlivePlayers == 3)	newHP = (players[0].hp + players[1].hp + players[2].hp) / 3;
	if (battleData.numAlivePlayers == 4)	newHP = (players[0].hp + players[1].hp + players[2].hp + players[3].hp) / 4;
	
	//set newHP
	for (int i = 0; i < battleData.numPlayers; i++)
		if (battleData.isAlive[i]) players[i].hp = newHP;
}

//------------
//Main
//------------

int main()
{
	system("cls");
	srand ( time(0) );
	//initialize the player data with default values
	savedPlayers = new player[4];
	savedCpuPlayers = new player[10];
	players = new player[4];

	for (int i=0; i<4; i++)
	{
		initialize(i);
		playerStates[i] = 0;
	}

	for (int i=0; i<10; i++)
		cpuInitialize(i);
	
	//replace player and cpu data with file data, if it exists.
	loadData();
	loadCpuData();
	loadNames();
	//show the main menu
	//randomBattleSetup();
	showMenu();
	
	//save all players to file
	saveData();
	
	//shutdown the program
	system("cls");
	return 0;
}

