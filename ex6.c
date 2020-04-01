/******************************************
*Student name: liad cohen
*Student ID:313362394
*Exercise name: ex6
******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LENGTH_ID 10  
#define LENGTH_GENDER 2
#define MAX 128
#define MAX_LENGTH_FIRST_NAME 15
#define MAX_LENGTH_LAST_NAME 15
#define MAX_LENGTH_USER_NAME 15
#define MIN_LENGTH_USER_NAME 3
#define MAX_LENGTH_PASSWORD 15
#define MIN_LENGTH_PASSWORD 3
#define MAX_LENGTH_DESCRIPTION 211
# define MAX_AGE 100
# define MIN_AGE 18
#define FIRST_MASK 0x00000001;
#define SECOND_MASK 0x00000010;
#define THIRD_MASK 0x00000100
#define FOURTH_MASK 0x00001000
#define FIFTH_MASK 0x00010000
#define SIXTH_MASK 0x00100000
#define SEVENTH_MASK 0x01000000
#define EIGHTH_MASK 0x10000000

enum hobbies { Baseball = 1, Basketball, Bicycle, Books, Drawing, Gym, Movies, Poetry };

struct Person {
	char id[LENGTH_ID];
	char* firstName;
	char* lastName;
	int  age;
	char  gender;
	char* userName;
	char* password;
	char* hobbies;
	char* description;
};

typedef struct Person Person;

typedef struct womenItem womenItem;

typedef int(*Comparator)(womenItem * first, womenItem *second);

struct womenItem {
	Person* data;	
	womenItem* next;
};

typedef struct {
	womenItem * head, *last;
} womenList;

//typedef struct womenItem womenItem;


// Declaration on functions

void init_person_data(Person *member, Person *data);
void add_woman_to_list(womenList * women_list, Person * data);
int loadData(Person* users, Person** men, womenList* womenList);
Person* searchTheUser(char *user_name, char *password, Person *men, womenList *women, int men_count);
int womem_list_size(womenList * list);
Person* search_man_user(char *user_name, char *password, Person * men, int menSize);
Person* read_and_add_user(Person* member, womenList* women, Person** men, int count_men);
void displayMainMenu(Person *user, womenList *women, Person* men, int men_count);
void matchForWoman(Person* men, int minAge, int maxAge, char *hobbies, int men_count);
void printMatchForWomens(Person* men, int i);
void matchForMan(womenList *women, int minAge, int maxAge, char *hobbies);
void deleteMan(Person *user, Person *men, int men_count);
void freeMan(Person* men, int index);
//char convertStrToInt(char* hoobies);
int matchOrNot(char hobbies1, char hobbies2);
char turnBits(char* hobbie);
//int matchOrNot1(char hobbies1, char hobbies2);
void bubbleSort(womenList* list, Comparator compare);
int WomenCompare(womenItem * first, womenItem *second);
void switchWomenItems(womenItem* a, womenItem* b);
void printMatchForMan(womenItem * women);
void deleteWoman(womenList* list, Person* woman);
void freeWomanItem(womenItem* iterator);
int WomenPersonCompare(Person * first, Person *second);
int checkId(char* id, Person* men, womenList* women, int numOfMen);
void outputTxt(Person *men, womenList* women, int men_count);

int main()
{	
	// varibles for the main function
	int membersCount, men_count, check=0;
	Person *users = NULL, *member = NULL, *men = NULL;
	Person *user;
	womenList *women;	
	// first allocation for men
	if ((men = (Person *)malloc(sizeof(Person))) == NULL)
	{
		printf("Not enough memory\n");
		exit(1);
	}
	// first allocation for women
	if ((women = (womenList*)malloc(sizeof(womenList))) == NULL)
	{
		printf("Not enough memory\n");
		exit(1);
	}
	// initialized the first node
	women->head = NULL;
	women->last = NULL;
	// read and save the data from the file
	membersCount = loadData(users, &men, women);	
	men_count = membersCount - womem_list_size(women);
	// choice of the user
	int numOfMission;
	char userName[MAX_LENGTH_USER_NAME];
	char password[MAX_LENGTH_PASSWORD];	

	while (1)
	{
		// first menu
		printf("Welcome! please choose an option\n 1 - Log in\n 2 - New member\n 3 - Exit\n ");		
		scanf("%d", &numOfMission);		
		switch (numOfMission)
		{	
		// log in 
		case(1):

			user = searchTheUser(userName, password, men, women, men_count);
			// if not find the user give him another chance
			if (user == NULL)
			{
				printf("User do not exist in the system, please try again\n");
				if ((user = searchTheUser(userName, password, men, women, men_count)) == NULL)
				{
					// return to the first menu
					break;
				}
				else if (user == -1)
				{
					// return to the first menu
					break;
				}
				else
				{
					printf("Hi %s\n", user->firstName);
					// go to main menu
					displayMainMenu(user, women, men, men_count);
				}
			}
			else if (user == -1)
			{
				// return to the first menu
				break;
			}
			else
			{
				printf("Hi %s\n", user->firstName);
				// go to main menu
				displayMainMenu(user, women, men, men_count);
			}
			break;
		// new member
		case(2):
			// alocated new user
			if ((user = (Person*)malloc(sizeof(Person))) == NULL)
			{
				printf("Not enough memory\n");
				exit(1);
			}
			check = read_and_add_user(user, women, &men, men_count);
			if (check == 1)
			{
				// return to the first menu
				break;
			}
			if (user->gender == 'M')
			{
				// if the new user is men need to raise the counter
				men_count += 1;
			}
			displayMainMenu(user, women, men, men_count);
			break;
		// exit from the sistem
		case(3):
			outputTxt(men, women, men_count);
			exit(1);
			break;
		default:
			printf("Bad choice, please try again");
			break;
		}
	}
	return 0;
}

/*    
member - the member to be inited man/woman
data - the data of the member from the file
*/
void init_person_data(Person *member, Person* data)    
{
	strcpy(member->id, data->id);
	member->firstName = data->firstName;
	member->firstName = data->firstName;
	member->lastName = data->lastName;
	member->age= data->age;
	member->gender= data->gender;
	member->userName = data->userName;
	member->password = data->password;
	member->hobbies = data->hobbies;
	member->description = data->description;
}

void add_woman_to_list(womenList* women_list, Person* data)
{
	womenItem *woman;
		
	if ((woman = (womenItem*)malloc(sizeof(womenItem))) == NULL)
	{
		printf("Not enough memory\n");
		exit(1);
	}

	woman->data = data;
	woman->next = NULL;	
	//check if the list is empty
	if (women_list->head == NULL)
	{
		women_list->head = woman;
		women_list->last = woman;
	}
	women_list->last->next = woman;
	women_list->last = woman;
}

/************************************************************************
* function name: loadData
* The Input:  users - pointer to struct
			  men - pointer to the dynamic array for men
			  womenList - pointer to the first node
* The output: int m - the number of persons in the file
* The Function operation: read and save all the data from the file and than
                          divied it to men and women (with other functions)
*************************************************************************/
int loadData(Person* users, Person** men, womenList* womenList)

{
	int i = 0, dummy, counter = 0, numOfLines;
	int menCounter = 0;
	char a, b;
	char c[1000];
	FILE *fp;
	// open the file
	fp = fopen("input.txt", "r");
	if (fp == NULL)
	{
		printf("failed opening the file\n");
		return;
	}

	char buffer[MAX];
	char *p; 
	int k = 0, m=0;
	Person* member;
	if ((member = (Person*)malloc(sizeof(Person))) == NULL)
	{
		printf("Not enough memory\n");
		exit(1);
	}
	while ((fgets(buffer, MAX, fp)) != NULL)
	{
		//printf(buffer);		
		p = strtok(buffer, ";");
		while (p != NULL)
		{
			//printf("%s\n", p);
			k++;			
		
			switch (k)
			{
			case 1 : 
				strcpy(member[i].id, p);
				break;
			case 2 : 
				member[i].firstName = (char*)malloc(strlen(p) + 1);	
				strcpy(member[i].firstName, p);
				
				break;
			case 3:
				member[i].lastName = (char*)malloc(strlen(p) + 1);
				strcpy(member[i].lastName, p);
				break;
			case 4:
				member[i].age = atoi(p);				
				break;
			case 5:
				member[i].gender =  p[0];
				if (member[i].gender == 'M')
				{
					menCounter++;
				}
				break;
			case 6:
				member[i].userName = (char*)malloc(strlen(p) + 1);
				strcpy(member[i].userName, p);
				break;
			case 7:
				member[i].password = (char*)malloc(strlen(p) + 1);
				strcpy(member[i].password, p);
				break;
			case 8:
				member[i].hobbies = (char*)malloc(strlen(p) + 1);
				strcpy(member[i].hobbies, p);
				break;
			case 9:
				member[i].description = (char*)malloc(strlen(p) + 1);
				strcpy(member[i].description, p);
				break;
			}
			p = strtok(NULL, ";");
		}			
			m++;
			i++;
			if ((member = (Person*)realloc(member, (m + 1) * sizeof(Person))) == NULL)
			{
				printf("Not enough memory - realloc failed\n");
				exit(1);
			}		
		k = 0;		
	}
	
	if ((member = (Person*)realloc(member, (m) * sizeof(Person))) == NULL)
	{
		printf("Not enough memory - realloc failed\n");
		exit(1);
	}
	
	//reallocate the men
	if ((*men = (Person*)realloc(*men, sizeof(Person)*menCounter)) == NULL)
	{
		printf("Not enough memory\n");
		exit(1);
	}

	int j=0;	
	for (i = 0; i < m ; i++)
	{
		
		if (member[i].gender == 'M')

		{			
			init_person_data(&(*men)[j], &member[i]);

			j++;					
		}
		
		else if (member[i].gender == 'F')
		{
			add_woman_to_list(womenList, &member[i]);
		}
	}
	//for (int k = 0; k < m; k++)
	//{
	//	free(&member[k]);
	
//	}
	return m;
	// dont forget to freee !!!!  	
}

int womem_list_size(womenList* list)
{
	int womenCount = 0;
	womenItem *woman = list->head;
	while (woman != NULL) {
		womenCount++;
		woman = woman->next;
	}
	return womenCount;
}

Person* search_man_user(char *user_name, char *password, Person* men, int menSize)
{
	int i=0;
	while (i< menSize)
	{
		if (strcmp(men[i].userName, user_name) == 0 && strcmp(men[i].password, password) == 0)
		{
			return &men[i];
		}
		i++;
	}
	return NULL;
}

Person* searchWomenUser(char *user_name, char *password, womenList *women)
{
	womenItem* head = women->head;
	while (head != NULL)
	{
		if ((strcmp(head->data->userName, user_name) == 0) && (strcmp(head->data->password, password) == 0))
		{
			return head->data;
		}
		head = head->next;
	}
	return NULL;
}
  

Person * searchTheUser(char *user_name,char *password, Person *men, womenList *women, int men_count)	
{	
	int i = 0;
	int flag = 0;
	Person* user = NULL;
	womenItem* head = women->head;
	printf("Please enter your username:\n");
	scanf("%s", user_name);	
	for(i=0; i<men_count; i++)
	{
		if (strcmp(men[i].userName, user_name) == 0) 
		{
			flag = 1;
			printf("Please enter your password:\n");
			scanf("%s", password);

			if (strcmp(men[i].password, password) == 0)
			{
				return &men[i];
			}
			else
			{
				printf("Wrong password\n");
				return -1;
			}
		}		
	}
	if (flag == 0)
	{		
		while (head != NULL)
		{
			if (strcmp(head->data->userName, user_name) == 0) 
			{
				flag = 1;
				printf("Please enter your password:\n");
				scanf("%s", password);
				if (strcmp(head->data->password, password) == 0)
				{
					return head->data;
				}
				else
				{
					printf("Wrong password\n");
					return -1;
				}
			}
			head = head->next;
		}
	}
	if (flag == 0)
	{
		user = NULL;
		return user;
	}

	/*
	printf("Please enter your password:\n");
	scanf("%s", password);

	if ((user = search_man_user(user_name, password, men, men_count)) == NULL)
	{
		user = searchWomenUser(user_name, password, women);
	}
	return user;
	*/
}

int checkId(char* id, Person *men, womenList* women, int numOfMen)
{
	int i=0;
	womenItem* head = women->head;
	while (i < numOfMen)
	{
		if (strcmp(men[i].id, id) == 0)
		{
			return 1;
		}
		i++;
	}
	while (head != NULL)
	{
		if (strcmp(id, head->data->id) == 0)
		{
			return 1;
		}
		head = head->next;
	}
	return 0;
}



Person * read_and_add_user(Person* member, womenList* women, Person** men, int count_men)

{
	int dummy = 0, i, checkIfExist=0;
	char buffer[MAX], h1,h2,h3,h4, tempHobbies[8];	
	
	printf("Please enter your ID :\n");
	scanf("%s", buffer);
	checkIfExist = checkId(buffer, &(*men)[0], women, count_men);
	if (checkIfExist == 1)
	{
		//exist in the stsyem
		printf("Error: User already exists\n");
		return 1;
	}
	strcpy(&member->id, buffer);

	printf("Please enter your first name :\n");
	scanf("%s", buffer);
	member->firstName = (char*)malloc(strlen(buffer) + 1);
	strcpy(member->firstName, buffer);

	printf("Please enter your last name :\n");
	scanf("%s", buffer);
	member->lastName = (char*)malloc(strlen(buffer) + 1);
	strcpy(member->lastName, buffer);

	printf("Please enter your age(18 to 100) :\n");
	scanf("%s", buffer); 
	if (atoi(buffer) < MIN_AGE || atoi(buffer) > MAX_AGE)
	{
		return 1;
	}
	member->age = atoi(buffer);
		
	printf("Please enter your gender(F - female, M - male) :\n");
	scanf("%s", buffer);
	member->gender = buffer[0];

	printf("Choose a username (3-10 characters):\n");
	scanf("%s", buffer);
	if (strlen(buffer) < MIN_LENGTH_USER_NAME)
	{
		return 1;
	}
	if (((buffer[0]) >= 'a' && buffer[0] <= 'z') || (buffer[0] >= 'A' && buffer[0] <= 'Z'))
	{
		member->userName = (char*)malloc(strlen(buffer) + 1);
		strcpy(member->userName, buffer);
	}
	else
	{
		return 1;
	}

	printf("Choose a password (attention-minimum 3 characters):\n");
	scanf("%s", buffer);
	if (strlen(buffer) < MIN_LENGTH_PASSWORD)
	{
		return 1;
	}

	member->password = (char*)malloc(strlen(buffer) + 1);
	strcpy(member->password, buffer);

	printf("Some words about yourself :\n");
	scanf("%c", &dummy);
	fgets(buffer, MAX_LENGTH_DESCRIPTION, stdin);
	member->description = (char*)malloc(strlen(buffer) + 1);
	strcpy(member->description, buffer);
	
	printf("please choose 4 hobbies: Baseball=1, Basketball=2, Bicycle=3, Books=4, Drawing=5, Gym=6");
	printf(",Movies=7, Poetry=8\n");	
	scanf("%c %c %c %c", &h1, &h2, &h3, &h4);	
	tempHobbies[0] = h1;
	tempHobbies[1] = ',';
	tempHobbies[2] = h2;
	tempHobbies[3] = ',';
	tempHobbies[4] = h3;
	tempHobbies[5] = ',';
	tempHobbies[6] = h4;
	tempHobbies[7] = '\0';
	member->hobbies = (char*)malloc(strlen(tempHobbies) + 1);	
	strcpy(member->hobbies, tempHobbies);	  
	
	if (member->gender == 'M')

	{
		*men = (Person*)realloc(*men, sizeof(Person)*(count_men + 1));
		init_person_data(&(*men)[count_men], member);
	}

	else if (member->gender == 'F')
	{
		add_woman_to_list(women, member);
		// linked list

	}
	printf("Hi %s, lets find love!\n", member->firstName);
	
}

void displayMainMenu(Person *user, womenList *women, Person* men, int men_count)
{		
	int option = 0;
	printf(" 1. Find a match\n 2. I found love, DELETE me\n 3. Log out\n");
	scanf("%d", &option);
	switch (option)
	{

	case 1:
		if (user->gender == 'M')
		{
			int minAge = 0, maxAge = 0;
			printf("Please choose ages range:\n");
			scanf("%d %d", &minAge, &maxAge);
			matchForMan(women, minAge, maxAge, user->hobbies);
		}
		else if (user->gender == 'F')
		{
			int minAge=0, maxAge=0;
			printf("Please choose ages range:\n");
			scanf("%d %d", &minAge, &maxAge);
			matchForWoman(men, minAge, maxAge, user->hobbies, men_count);
		}

		break;
	case 2:
		if (user->gender == 'M')
		{
			deleteMan(user, men, men_count);
		}
		else if (user->gender == 'F')
		{
			deleteWoman(women, user);
		}
		break;
	case 3:  
		// return to the first menu
		break;
	default:
		printf("Bad choice, please try again\n");
		break;
	}
}
void matchForMan(womenList *women, int minAge, int maxAge, char *hobbies)
{
	bubbleSort(women, WomenCompare);
	int i, matualHobbies;
	char hobbiesMan, hobbiesWoman;
	womenItem* head = women->head;
	hobbiesMan = turnBits(hobbies);
	while (head != NULL)
	{
		if (head->data->age >= minAge && head->data->age <= maxAge)
		{			
			hobbiesWoman = turnBits(head->data->hobbies);
			matualHobbies = matchOrNot(hobbiesWoman, hobbiesMan);
			if (matualHobbies >= 2)
			{				
				printMatchForMan(head);
			}
		}
		head = head->next;
	}
}

void printMatchForMan(womenItem * women)
{
	char hobbies[MAX] = { 0 };
	int success = build_hobbies_str(women->data->hobbies, hobbies);
	printf("Name: %s %s Age: %d Hobbies: %s\n", women->data->firstName, women->data->lastName, women->data->age, hobbies);
	printf("Description: %s\n\n", women->data->description);
}


void matchForWoman(Person* men, int minAge, int maxAge,char *hobbies, int men_count)
{
	int i, matualHobbies;
	char hobbiesMan, hobbiesWoman;
	hobbiesWoman = turnBits(hobbies);
	for (i = 0; i < men_count; i++)
	{
		if (men[i].age>= minAge && men[i].age <= maxAge)
		{
			hobbiesMan = turnBits(men[i].hobbies);
			matualHobbies = matchOrNot(hobbiesWoman, hobbiesMan);
			if (matualHobbies >= 2) 
			{
				printMatchForWomens(men,i);
			}
		}
	}
}

int build_hobbies_str(char * hobbies, char *str_recv)
{
	char *hobbies_str[] = {"Baseball", "Basketball", "Bicycle", "Books", "Drawing", "Gym", "Movies", "Poetry"};
	int arr_length = 0,i;
	for (i = 0; i < strlen(hobbies); i += 2)
	{
		char idx = hobbies[i] - '0';
		strcpy(&str_recv[arr_length], hobbies_str[idx-1]);
		arr_length += strlen(hobbies_str[idx - 1]);
		strcpy(&str_recv[arr_length], ", ");
		arr_length += 2;
	}
	str_recv[arr_length - 2] = 0;
	return arr_length;
}

void printMatchForWomens(Person* men, int i)
{
	char hobbies[MAX] = { 0 };	 
	int success = build_hobbies_str(men[i].hobbies, hobbies);
	printf("Name: %s %s Age: %d Hobbies: %s\n", men[i].firstName, men[i].lastName, men[i].age, hobbies);
	printf("Description: %s\n\n", men[i].description);	
}

void deleteMan(Person *user, Person *men, int men_count)
{
	int i, index=-1;

	for (i = 0; i < men_count; i++)
	{
		if (strcmp(men[i].firstName, user->firstName) == 0)
		{
			index = i;
		}
	}
	
	if (index != -1)
	{
		freeMan(men, index);

		// check if the man that need to delete is not in the end of array
		if (index != men_count - 1)
		{
			int j;
			for (j = index; j < men_count; j++)
			{
				men[index] = men[index + 1];
			}
		}
	}
}

void freeMan(Person* men, int index)
{
	free(men[index].firstName);
	free(men[index].lastName);
	free(men[index].password);
	free(men[index].userName);
	free(men[index].hobbies);
	free(men[index].description);
}

  /*
char convertStrToInt(char* hoobies)
{
	char retval, mask = 0x01, shift;
	for (int i = 0; i < strlen(hoobies); i += 2)
	{
		shift = hoobies[i] - '0';
		mask = mask << shift - 1;
		retval |= mask;
	}
	return retval;
}
*/
int matchOrNot(char hobbies1, char hobbies2)
{
	char temp, mask = 0x01;
	int numOfBits = 8, matualHobbies = 0,j;
	temp = hobbies1 & hobbies2;
	for (j = 0; j < numOfBits; j++)
	{
		if (temp & mask)
		{
			matualHobbies++;
		}
		mask = mask << 1;
	}
	return matualHobbies;
}

char turnBits(char* hobbie)
{
	char choice_list = choice_list & 0x0;
	int i;
	for (i = 0; i < strlen(hobbie); i += 2)
	{
		if (hobbie[i] == '1')
		{
			choice_list = choice_list | (1 << (Baseball - 1));
		}
		else if (hobbie[i] == '2')
		{
			choice_list = choice_list | (1 << (Basketball - 1));
		}
		else if (hobbie[i] == '3')
		{
			
			choice_list = choice_list | (1 << (Bicycle - 1));
		}
		else if (hobbie[i] == '4')
		{
			choice_list = choice_list | (1 << (Books - 1));
		}
		else if (hobbie[i] == '5')
		{
			choice_list = choice_list | (1 << (Drawing - 1));	
		}
		else if (hobbie[i] == '6')
		{
			choice_list = choice_list | (1 << (Gym - 1));
		}
		else if (hobbie[i] == '7')
		{
			choice_list = choice_list | (1 << (Movies - 1));
		}
		else if (hobbie[i] == '8')
		{
			choice_list = choice_list | (1 << (Poetry - 1));
		}
	}
	return choice_list;
}

/*
int matchOrNot1(char hobbies1, char hobbies2)
{	
	int andOperatorHobbies = hobbies1 & hobbies2, mask = 0x01; // ??? or char
	int numOfBits = 8, matualHobbies = 0, i;
	for (int i = 0; i < numOfBits; i++)
	{
		if (andOperatorHobbies & mask > 0)
		{
			matualHobbies++;
		}

		mask = mask << 1;
	} 
	return matualHobbies;
}
*/
  

//hobbiesCompare(char *, char*)

int WomenCompare(womenItem * first, womenItem *second)
{
	return strcmp(first->data->lastName, second->data->lastName);
}


void switchWomenItems(womenItem* a, womenItem* b) {
	Person * temp = a->data;
	a->data = b->data;
	b->data = temp;
}

void bubbleSort(womenList* list, Comparator compare)
{
	womenItem* i;
	int switched = 1;
	while (switched != 0) {
		switched = 0;
		i = list->head;
		while (i->next != NULL) {
			if (compare(i, i->next) > 0) {
				switched = 1;
				switchWomenItems(i, i->next);
			}
			i = i->next;
		}
	}
}

void deleteWoman (womenList* list, Person* woman) {
	womenItem* iterator = list->head->next;
	womenItem* previous = list->head;
	while (iterator != NULL && WomenPersonCompare(iterator->data, woman) != 0) {
		previous = iterator;
		iterator = iterator->next;
	}

	// We either found the woman or we got to the end of the list
	if (WomenPersonCompare(iterator->data, woman) == 0) {
		previous->next = iterator->next;
		iterator->next = NULL;
		freeWomanItem(iterator);
		
		// If this is the first woman
		if (iterator == list->head)
		{
			list->head = list->head->next;
		}
		// If this is the last woman
		else if (iterator == list->last)
		{
			list->last = previous;
		}
	}
}int WomenPersonCompare(Person * first, Person *second)
{
	return strcmp(first->lastName, second->lastName);
}

void freeWomanItem(womenItem* iterator)
{
	free(iterator->data->firstName);
	free(iterator->data->lastName);
	free(iterator->data->userName);
	free(iterator->data->password);
	free(iterator->data->hobbies);
	free(iterator->data->description);

	free(iterator);
}

void outputTxt(Person *men, womenList* women, int men_count)
{
	int i;
	FILE* f;
	f = fopen("output.txt", "w+");
	womenItem* head = women->head;
	for (i = 0; i < men_count; i++)
	{
		fprintf(f, "%s", men[i].id);
		fprintf(f, ";");
		fprintf(f, "%s", men[i].firstName);
		fprintf(f, ";");
		fprintf(f, "%s", men[i].lastName);
		fprintf(f, ";");
		fprintf(f, "%d", men[i].age);
		fprintf(f, ";");
		fprintf(f, "%c", men[i].gender);
		fprintf(f, ";");
		fprintf(f, "%s", men[i].userName);
		fprintf(f, ";");
		fprintf(f, "%s", men[i].password);
		fprintf(f, ";");
		fprintf(f, "%s", men[i].hobbies);
		fprintf(f, ";");
		fprintf(f, "%s", men[i].description);
		fprintf(f, "\n");
	}
	bubbleSort(women, WomenCompare);
	while (head != NULL)
	{
		fprintf(f, "%s", head->data->id);
		fprintf(f, ";");
		fprintf(f, "%s", head->data->firstName);
		fprintf(f, ";");
		fprintf(f, "%s", head->data->lastName);
		fprintf(f, ";");
		fprintf(f, "%d", head->data->age);
		fprintf(f, ";");
		fprintf(f, "%c", head->data->gender);
		fprintf(f, ";");
		fprintf(f, "%s", head->data->userName);
		fprintf(f, ";");
		fprintf(f, "%s", head->data->password);
		fprintf(f, ";");
		fprintf(f, "%s", head->data->hobbies);
		fprintf(f, ";");
		fprintf(f, "%s", head->data->description);
		fprintf(f, "\n");

		head = head->next;
	}
	fclose(f);
}