#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

struct Item
{
	int key;
	int val;
	struct Item* next;
	struct Item* prev;
};
struct Item* Head = NULL;
struct Item* Tail = NULL;

struct HeaderFile
{
	int version;
	int itemCount;
	char reserve[100];
};
struct HeaderFile fileHeader;


//Insert items into the list: key and val
void Add(int key, int val)
{
	struct Item* currentItem;
	currentItem = (struct Item*)malloc(sizeof(struct Item));
	if (currentItem == NULL)
	{
		printf("ERROR - Not enough memory");
		return;
	}
	else
	{
		currentItem->key = key;
		currentItem->val = val;

		if (Head == NULL)
		{
			Head = currentItem;
			Tail = currentItem;
			currentItem->next = NULL;
			currentItem->prev = NULL;
		}
		else
		{
			currentItem->prev = Tail;
			currentItem->next = NULL;
			Tail->next = currentItem;
			Tail = currentItem;
		}
	}
}

//Search a key in the linked list
struct Item* SearchKey(int key)
{
	struct Item* currentItem = Head;
	struct Item* ret = NULL;

	while (currentItem != NULL)
	{
		if (currentItem->key == key)
		{
			ret = currentItem;
			return ret;
		}

		currentItem = currentItem->next;
	}

	//If there is no such key in the linked list return NULL 
	printf("*** (There is no such key in the list)\n");
	return NULL;
}

//Remove item from the list
void RemoveItemFromList(int key)
{
	struct Item* currentItem = SearchKey(key);

	if (!currentItem) //If there is no such key->cuurentItem==NULL and return
	{
		currentItem = NULL;
		return;
	}
	else if (currentItem == Head && currentItem == Tail)
	{
		free(currentItem);
		Head = NULL;
		Tail = NULL;
		return;
	}
	else if (currentItem == Head)
	{
		Head->next->prev = NULL;
		Head = Head->next;
		free(currentItem);
		return;
	}
	else if (currentItem == Tail)
	{
		Tail->prev->next = NULL;
		Tail = Tail->prev;
		free(currentItem);
		return;
	}
	else
	{
		currentItem->next->prev = currentItem->prev;
		currentItem->prev->next = currentItem->next;
		free(currentItem);
		return;
	}
}

//Update:enter a key and update its value
void UpdateValue(int key, int val)
{
	struct Item* sameKey = Head;

	while (sameKey != NULL)
	{
		if (sameKey->key == key)
		{
			sameKey->val = val;
			return;
		}

		sameKey = sameKey->next;
	}

	printf("There is no such key in the list\n");
	return;
}

//Print the linked list
void PrintList()
{
	struct Item* currentItem = Head;

	printf("The resulting linked list:\n");
	while (currentItem != NULL)
	{
		printf("Key: %d Value: %d\n", currentItem->key, currentItem->val);
		currentItem = currentItem->next;
	}
}

//Save the list to file
void SaveFile()
{
	struct Item* currentItem = Head;

	FILE* f = fopen("Data1.bin", "w");
	if (f == NULL)
	{
		printf("ERROR");
		return;
	}
	else
	{
		fwrite(&fileHeader, sizeof(struct HeaderFile), 1, f); //Write the header into the file

		for (int i = 0; i < fileHeader.itemCount; i++) //write the struct Item into the file
		{
			fwrite(currentItem, sizeof(struct Item), 1, f);
			currentItem = currentItem->next;
		}

		fclose(f);
	}
}

//Load the list from file
void LoadFile()
{
	FILE* f = fopen("Data1.bin", "r");
	if (f == NULL)
	{
		printf("ERROR");
		return;
	}
	else
	{
		int read = fread(&fileHeader, sizeof(struct HeaderFile), 1, f); //Read the header from the file
		if (read == 0)
		{
			printf("ERROR");
			return;
		}

		for (int i = 0; i < fileHeader.itemCount; i++) //Read the struct Item from the file
		{
			struct Item* currentItem = (struct Item*)malloc(sizeof(struct Item));
			if (currentItem == NULL)
			{
				printf("ERROR - Not enough memory");
				return;
			}

			//Build the list
			read = fread(currentItem, sizeof(struct Item), 1, f);
			Add(currentItem->key, currentItem->val);
		}

		fclose(f);
	}
}

//Free the linked list, we will do this after saving the list in the file
//The Head and Tail are reset in preparation for reading the file and creating the new linked list
void FreeList()
{
	struct Item* currentItem = Head;
	struct Item* release;

	while (currentItem != NULL)
	{
		release = currentItem;
		currentItem = currentItem->next;
		free(release);
	}

	Head = NULL;
	Tail = NULL;
}


int main()
{
	//initialization of struct HeaderFile 
	fileHeader.itemCount = 0;
	fileHeader.version = 1;
	/////////////////////////////////////

	int num1, num2, num3;
	struct Item* KeyeFound;

	char userResponse = NULL;
	while (userResponse != 'E')
	{
		printf("\nPlease choose one of the following options:\n'A'-Insert a new value into the list: key and value\n'F'-Search a key\n'U'-Update: enter a key and update its value\n'P'-Print the linked list\n'D'-Remove from list\n'S'-Save file\n'L'-Load file\n'E'-Exit the program\n");
		userResponse = getch();
		switch (userResponse)
		{
		case 'A': //Insert items to the linked list
			printf("Please enter key and value: ");
			scanf("%d %d", &num1, &num2);
			Add(num1, num2);
			fileHeader.itemCount++;
			break;
		case 'F': //Search a key and return value, if the key is not found return return ***
			printf("Please enter the key you are looking for: ");
			scanf("%d", &num3);
			KeyeFound = SearchKey(num3);

			if (KeyeFound == NULL) //If there is no such key in the list break
			{
				break;
			}

			printf("The item is: key: %d value: %d \n", KeyeFound->key, KeyeFound->val);
			break;
		case 'U': //Update:enter a key and update its value
			printf("Please enter the key whose value you want to change and the new value: ");
			scanf("%d %d", &num1, &num2);
			UpdateValue(num1, num2);
			break;
		case 'D': //Remove an item from the list
			printf("Please enter the key you want to remove: ");
			scanf("%d", &num3);
			RemoveItemFromList(num3);
			fileHeader.itemCount--;
			break;
		case 'P': //Print the linked list
			PrintList();
			break;
		case 'S':
			SaveFile(); //Save file
			FreeList(); //Free the linked list, we will do this after saving the list in the file
			break;
		case 'L':
			LoadFile(); //Load file
			break;
		case 'E': //Exit - exit the program
			printf("Thank you for using our program.\n");
			break;

		default:
			printf("Wrong selection, please try again\n");
			break;
		}
	}

	return 0;
}