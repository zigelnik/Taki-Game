/*
NAME: Tal Zigelnik

This program is designed to imitate the TAKI game.
The program starts by choosing the number of players, then each player will enter his name and will get 4 cards to begin with.
The cards vary from numbers and special cards like changing color, stopping the next turn, adding another card in same turn, etc..
After a player has no more cards left in his hand, his name will be printed.

Enjoy

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 
#include <time.h>	
#include <string.h>
#include <stdbool.h>


#define ONE 1
#define TWO 2
#define THREE 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define SEVEN 7
#define EIGHT 8
#define NINE 9
#define ONE_CARD "1"
#define TWO_CARD "2"
#define THREE_CARD "3"
#define FOUR_CARD "4"
#define FIVE_CARD "5"
#define SIX_CARD "6"
#define SEVEN_CARD "7"
#define EIGHT_CARD "8"
#define NINE_CARD "9"
#define pLUS 10
#define sTOP 11
#define reWIND 12
#define TAKI 13
#define CHANGE_COLOR 14
#define NO_COLOR -1
#define RED 0
#define BLUE 1
#define GREEN 2
#define YELLOW 3
#define MAX_NAME_LEN 21
#define RED_CARD "R"
#define BLUE_CARD "B"
#define YELLOW_CARD "Y"
#define GREEN_CARD "G"
#define NO_COLOR_CARD " "
#define PLUS_CARD "+"
#define STOP_CARD "STOP"
#define REWIND_CARd "<->"
#define COLOR_CARD "COLOR"
#define TAKI_CARD "TAKI"
#define CARDS_IN_HAND 4



typedef struct card
{
	int type;
	int color;
} CARD;

typedef struct player
{
	int logSize;
	int phySize;
	char name[MAX_NAME_LEN];
	CARD* card;
} PLAYER;

CARD createCard();

typedef struct stats
{
	int num;
} STATS;

void Initialize(PLAYER* players, int num_of_players);
void printCard(int type, int color);
void startGame(PLAYER* players, int num_of_players);
void printColorCard(char* color);
void getNames(PLAYER** players, int i);
void dump_card(CARD* arr, int i, int j);
void printWelcome();
void printMemoAllocFailed();
void printPlayerHand(PLAYER* players, int i, int j);
void chooseWhatToDo(int handSize);
void drawNewCard(PLAYER* players, int i);
void PlayerWon(char winner);
bool toggleReverseCard(bool reverse);
CARD pickUpperCard();
CARD* cardsRealloc(CARD* cardsArr, int size, int newSize);
CARD toggleTakiSeries(PLAYER* players, CARD* cards, int handSize, CARD upper_card);


void main()
{
	srand(time(NULL));
	PLAYER* players;

	printWelcome();

	int num_of_players = HowManyPlayers();
	players = (PLAYER*)malloc(sizeof(PLAYER) * num_of_players);
	Initialize(players, num_of_players);

	startGame(players, num_of_players);

	int i;
	for (i = 0; i < num_of_players; i++)
		free(players[i].card);

	free(players);

}

// user will enter how much players will play the game
int HowManyPlayers()
{
	int num;
	printf("Please enter the number of players: ");
	scanf("%d", &num);
	return num;
}

// after number of players has been chosen, each player will enter its name and every player will get 4 cards.
void Initialize(PLAYER* players, int num_of_players)
{
	int i, j;
	for (i = 0; i < num_of_players; i++)
	{
		getNames(&players, i);
		for (j = 0; j < players[i].logSize; j++)
			players[i].card[j] = createCard();
	}
}

void getNames(PLAYER** players, int i)
{

	(*players)[i].logSize = CARDS_IN_HAND;
	(*players)[i].phySize = CARDS_IN_HAND;

	printf("Please enter the first name of player #%d:\n", i + 1);
	scanf("%s", (*players)[i].name);
	(*players)[i].card = (PLAYER*)malloc(sizeof(PLAYER) * (*players)[i].logSize);

	if (!*players)
	{
		printMemoAllocFailed();
		exit(1);
	}

}

// for the game to start we need that the first upper card will be a card number between 1-9 and not a special action card
CARD pickUpperCard()
{
	CARD card = createCard();
	while (card.type > 9)
		card = createCard();

	return card;
}

void chooseWhatToDo(int handSize)
{
	printf("Please enter 0 if you want to take a card from the deck\n"
		"or 1-%d if you want to put one of your cards in the middle:\n", handSize);
}

// if a player chose to draw a card, we will increase the player's hand array so he can hold more cards.
void drawNewCard(PLAYER* players, int i)
{
	if (players[i].logSize == players[i].phySize)
	{
		players[i].phySize *= 2;
		players[i].card = cardsRealloc(players[i].card, players[i].logSize, players[i].phySize);
	}
	players[i].card[players[i].logSize] = createCard();
	players[i].logSize++;
}

void printPlayerHand(PLAYER* players, int i, int j)
{
	printf("Card #%d:\n", j + 1);
	printCard(players[i].card[j].type, players[i].card[j].color);
	printf("\n\n");

}


void startGame(PLAYER* players, int num_of_players)
{
	CARD upper_card;
	CARD correct_card;
	upper_card = pickUpperCard();

	bool reverse = false;


	int option, i = 0;
	while (players[i].logSize != 0)
	{
		int j;
		printf("\nUpper card:\n");
		printCard(upper_card.type, upper_card.color);
		printf("\n\n");

		printf("%s's turn:\n\n", players[i].name);
		for (j = 0; j < players[i].logSize; j++)
			printPlayerHand(players, i, j);

		chooseWhatToDo(players[i].logSize);
		scanf("%d", &option);

		// wrong input will trigger error
		while (option > players[i].logSize)
		{
			printf("Invalid choice! Try again.\n");
			chooseWhatToDo(players[i].logSize);
			scanf("%d", &option);
		}

		// if the card chosen is not the same colour or number or 0(draw card), print invalid card and ask for another option
		while (players[i].card[option - 1].type != upper_card.type && players[i].card[option - 1].color != upper_card.color && option != 0
			&& players[i].card[option - 1].type != CHANGE_COLOR)
		{
			printf("Invalid card! Try again.\n");
			chooseWhatToDo(players[i].logSize);
			scanf("%d", &option);
		}

		//draw card and move turn to the next player
		if (option == 0)
			drawNewCard(players, i);


		if (players[i].card[option - 1].type == upper_card.type || players[i].card[option - 1].color == upper_card.color || players[i].card[option - 1].color == NO_COLOR)
		{
			// if the card chosen is "normal" (number between 1-9)
			if (players[i].card[option - 1].type < 10)
			{
				upper_card.type = players[i].card[option - 1].type;
				upper_card.color = players[i].card[option - 1].color;
				dump_card(players[i].card, option - 1, players[i].logSize - 1);
				players[i].logSize--;
			}


			else if (players[i].card[option - 1].type > 9)
			{
				if (players[i].card[option - 1].type == CHANGE_COLOR) //works
				{
					int changedColor = ChangeColorFunc();
					upper_card.type = CHANGE_COLOR;
					upper_card.color = changedColor;
					dump_card(players[i].card, option - 1, players[i].logSize - 1);
					players[i].logSize--;
				}
				else if (players[i].card[option - 1].type == pLUS) // works
				{
					upper_card.type = pLUS;
					upper_card.color = players[i].card[option - 1].color;
					dump_card(players[i].card, option - 1, players[i].logSize - 1);
					players[i].logSize--;
					if (reverse == false)
						i--;
					else if (reverse == true)
						i++;
				}
				else if (players[i].card[option - 1].type == sTOP) // work
				{
					upper_card.type = sTOP;
					upper_card.color = players[i].card[option - 1].color;
					dump_card(players[i].card, option - 1, players[i].logSize - 1);
					players[i].logSize--;

					if (reverse == false)
						i += 1;
					else
						i -= 1;
				}

				else if (players[i].card[option - 1].type == reWIND) // work!
				{
					reverse = toggleReverseCard(reverse);
					upper_card.type = reWIND;
					upper_card.color = players[i].card[option - 1].color;
					dump_card(players[i].card, option - 1, players[i].logSize - 1);
					players[i].logSize--;
				}
				else if (players[i].card[option - 1].type == TAKI)
				{
					upper_card.type = TAKI;
					upper_card.color = players[i].card[option - 1].color;
					dump_card(players[i].card, option - 1, players[i].logSize - 1);
					players[i].logSize--;
					upper_card = toggleTakiSeries(players, players[i].card, players[i].logSize, upper_card);

					if (reverse == false)
						i--;
					else if (reverse == true)
						i++;

				}
			}
		}

		// if reverse card has been played, the index will go clockwise, otherwise counter-clockwise

		if (reverse == true)
		{
			i--;
			if (i < 0)
				i = num_of_players - 1;
		}

		else
		{
			i++;
			if (i >= num_of_players)
				i = i - num_of_players;
		}

		if (players[i - 1].logSize == 0)
			break;

	}

	PlayerWon(players[i].name);

}

void PlayerWon(char winner)
{
	printf("\nThe winner is... %s! Congratulations!!\n", winner);

}

CARD toggleTakiSeries(PLAYER* players, CARD* cards, int handSize, CARD upper_card)
{
	int choice = 1;

	while (choice != 0)
	{
		int j, i = 0;
		printf("\nUpper card:\n");
		printCard(upper_card.type, upper_card.color);
		printf("\n\n");

		printf("%s's turn:\n\n", players[i].name);
		for (j = 0; j < players[i].logSize; j++)
			printPlayerHand(players, i, j);

		printf("Please enter 0 if you want to finish your turn\n"
			"or 1-%d if you want to put one of your cards in the middle:\n", handSize);
		scanf("%d", &choice);
		while (choice > players[i].logSize)
		{
			printf("Invalid choice! Try again.\n");
			printf("Please enter 0 if you want to finish your turn\n"
				"or 1-%d if you want to put one of your cards in the middle:\n", handSize);
			scanf("%d", &choice);
		}

		// if the card chosen is not the same colour or number or 0(draw card), put invalid card and ask for another option
		while (players[i].card[choice - 1].color != upper_card.color && choice != 0)
		{
			printf("Invalid card! Try again.\n");
			printf("Please enter 0 if you want to finish your turn\n"
				"or 1-%d if you want to put one of your cards in the middle:\n", handSize);
			scanf("%d", &choice);
		}

		if (players[i].card[choice - 1].color == upper_card.color)
		{
			upper_card.color = players[i].card[choice - 1].color;
			upper_card.type = players[i].card[choice - 1].type;
			dump_card(players[i].card, choice - 1, players[i].logSize - 1);
			players[i].logSize--;

		}

	}
	return upper_card;
}

bool toggleReverseCard(bool reverse)
{
	if (reverse == false)
		return true;
	else if (reverse == true)
		return false;
}


// this function is used on the chosen card to put on top of the table, then swapping it with the last card in the array, to minimize the array by 1 after it, thus discarding it
void dump_card(CARD* arr, int i, int j) {
	CARD tmp = arr[i];
	arr[i] = arr[j];
	arr[j] = tmp;
}


int ChangeColorFunc()
{
	int clr;
	printf("Please enter your color choice:\n"
		"1 - Yellow\n"
		"2 - Red\n"
		"3 - Blue\n"
		"4 - Green\n");
	scanf("%d", &clr);
	while (clr > 4 || clr < 1)
	{
		printf("Invalid color! Try again.\n");
		printf("Please enter your color choice:\n"
			"1 - Yellow\n"
			"2 - Red\n"
			"3 - Blue\n"
			"4 - Green\n");
		scanf("%d", &clr);
	}
	switch (clr)
	{
	case 1:
		return YELLOW;
		break;
	case 2:
		return RED;
		break;
	case 3:
		return BLUE;
		break;
	case 4:
		return GREEN;
		break;
	}
}

void printCard(int type, int color)
{
	if (type < 10)
	{
		printf("*********\n"
			"*       *\n"
			"*   %d   *\n", type);
	}
	else if (type == pLUS)
	{
		printf("*********\n"
			"*       *\n"
			"*   %s   *\n", PLUS_CARD);
	}
	else if (type == sTOP)
	{
		printf("*********\n"
			"*       *\n"
			"*  %s *\n", STOP_CARD);
	}
	else if (type == reWIND)
	{
		printf("*********\n"
			"*       *\n"
			"*  %s  *\n", REWIND_CARd);
	}
	else if (type == TAKI)
	{
		printf("*********\n"
			"*       *\n"
			"*  %s *\n", TAKI_CARD);
	}
	else if (type == CHANGE_COLOR)
	{
		printf("*********\n"
			"*       *\n"
			"* %s *\n", COLOR_CARD);
	}

	switch (color)
	{
	case RED:
		printColorCard(RED_CARD);
		break;
	case BLUE:
		printColorCard(BLUE_CARD);
		break;
	case YELLOW:
		printColorCard(YELLOW_CARD);
		break;
	case GREEN:
		printColorCard(GREEN_CARD);
		break;
	case NO_COLOR:
		printColorCard(NO_COLOR_CARD);
		break;
	}
}

void printColorCard(char* color)
{
	printf("*   %s   *\n"
		"*       *\n"
		"*********", color);
}

// creating a card using the random func
CARD createCard()
{
	CARD card;
	card.type = (rand() % 14) + 1;

	if (card.type == CHANGE_COLOR)
		card.color = NO_COLOR;
	else
		card.color = rand() % 4;

	return card;
}

// increasing the player's hand array if a card has been drawn
CARD* cardsRealloc(CARD* cardsArr, int size, int newSize)
{
	CARD* newCardsArr = (CARD*)malloc(newSize * sizeof(CARD));
	int i;
	if (!newCardsArr)
	{
		printMemoAllocFailed();
		exit(1);
	}
	if (newCardsArr)
	{
		for (i = 0; i < size && i < newSize; i++)
		{
			newCardsArr[i] = cardsArr[i];
		}
		free(cardsArr);
	}

	return newCardsArr;
}

void printWelcome() {
	printf("************  Welcome to TAKI game !!! ***********\n");
}

void printMemoAllocFailed()
{
	printf("Memmory allocation failed!");
}