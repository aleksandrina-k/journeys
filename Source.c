#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE ((size_t) 15)

typedef struct Coordinates {
	char* longitude;
	char* latitude;
} t_coord;

typedef struct Journey {
	t_coord* startc;      
	t_coord* endc;        
	char* startname;
	char* endname;
	double velocity;
	double time;
} t_journey;

typedef struct Node {
	struct Journey info;
	struct Node* next;
} t_node;

t_node* NewJourney();
int CheckCoordFormat(char* str, char* dymention);
int CheckForNum(char* str);
int CheckStartEnd(t_journey var);
t_node* CreateList(t_node** p_head);
int CheckForCoord(t_node* p_head, t_coord* coord1, t_coord* coord2);
void PrintOne(t_node* p);
void PrintALL(t_node* p_head);
unsigned CountList(t_node* p_head);
void TopShortestJourneys(t_node* p_head, int top);
void TopLongestJourneys(t_node* p_head, int top);
void Sort(double arr[], int size);
unsigned CountJoureyByCond(t_node* p_head, char* start, char* end);
void FindJourney(t_node* p_head, char* start, char* end);
void SaveInTxtFile(const char* fname, t_node* p_head);
void read_and_create_list(const char* file_name, t_node** p_head);
void FreeList(t_node** p_head);

void TopShortestJourneys1(t_node* p_head, int top);
void TopLongestJourneys1(t_node* p_head, int top);
void SortList(t_journey array[], int size);

int main()
{
	t_node* head = NULL;
	read_and_create_list("data.txt", &head);
	unsigned top, len;
	int command = 6;
	char* s = (char*)malloc(sizeof(char) * MAX_SIZE);
	char* e = (char*)malloc(sizeof(char) * MAX_SIZE);
	if (s == NULL || e == NULL)
	{
		printf("Memory error...\n");
		exit(0);
	}


	while (command)
	{
		printf("\t\t\MENU\n");
		printf("\t0.SAVE changes and EXIT\n");
		printf("\t1.ADD new journey\n");
		printf("\t2.PRINT info\n");
		printf("\t3.SEARCH for specific journey\n");
		printf("\t4.CREATE FILE with the SHORTEST journeys\n");
		printf("\t5.CREATE FILE with the LONGEST journeys\n");
		printf("\t****************************\n");
		printf("\tYour choice:");
		scanf("%d", &command);

		switch (command)
		{
		case 0:
			SaveInTxtFile("data.txt", head);
			FreeList(&head);
			printf("Bye! =)\n");
			break;
		case 1: head = CreateList(&head);
			break;
		case 2:
			if (head == NULL)
			{
				printf("No data.\n\n");
			}
			else
			{
				printf("\n==INFO==\n");
				PrintALL(head);
			}
			break;
		case 3:
			printf("\nInput startpoint of the journey:");
			scanf("%s", s);
			while (CheckForNum(s) == 1)
			{
				printf("Invalid input! (only letters allowed)\n");
				printf("Input startpoint of the journey:");
				scanf("%s", s);
			}
			printf("\nInput endpoint of the journey:");
			scanf("%s", e);
			while (CheckForNum(e) == 1)
			{
				printf("Invalid input! (only letters allowed)\n");
				printf("Input endpoint of the journey:");
				scanf("%s", e);
			}
			len = CountJoureyByCond(head, s, e);
			if (len == 0)
				printf("\nYou haven't been on such a journey.\n");
			else
			{
				printf("\nYou've been on %u such journeys.\n", len);
				FindJourney(head, s, e);
			}
			break;
		case 4:
			printf("\nEnter how many of your shortest journeys do you want to see:");
			scanf("%d", &top);
			len = CountList(head);
			while (top > len)
			{
				printf("There are less than %u journeys.\n", top);
				printf("\nEnter how many of your shortest journeys do you want to see:");
				scanf("%d", &top);
			}
			TopShortestJourneys(head, top);
			break;
		case 5:
			printf("\nEnter how many of your longest journeys do you want to see:");
			scanf("%d", &top);
			len = CountList(head);
			while (top > len)
			{
				printf("There are less than %u journeys.\n", top);
				printf("\nEnter how many of your longest journeys do you want to see:");
				scanf("%d", &top);
			}
			TopLongestJourneys(head, top);
			break;
		default:
			printf("Wrong command!\n\n");
			break;
		}
	}
	free(s);
	free(e);
	s = NULL;
	e = NULL;
	head = NULL;
	return 0;
}

t_node* NewJourney()
{
	t_node* new = NULL;
	new = (t_node*)malloc(sizeof(t_node));
	new->info.startname = (char*)malloc(sizeof(char) * MAX_SIZE);
	new->info.startc = (t_coord*)malloc(sizeof(t_coord));
	new->info.startc->latitude = (char*)malloc(sizeof(char) * MAX_SIZE);
	new->info.startc->longitude = (char*)malloc(sizeof(char) * MAX_SIZE);
	new->info.endname = (char*)malloc(sizeof(char) * MAX_SIZE);
	new->info.endc = (t_coord*)malloc(sizeof(t_coord));
	new->info.endc->latitude = (char*)malloc(sizeof(char) * MAX_SIZE);
	new->info.endc->longitude = (char*)malloc(sizeof(char) * MAX_SIZE);
	if (new == NULL|| new->info.startname==NULL|| new->info.startc==NULL || new->info.startc->latitude==NULL||
		new->info.startc->longitude==NULL|| new->info.endname ==NULL || new->info.endc==NULL ||
		new->info.endc->latitude==NULL || new->info.endc->longitude==NULL)
	{
		printf("Memory error...");
		exit(0);
	}

	printf("\nStart point:");
	scanf("%s", new->info.startname);
	while (CheckForNum(new->info.startname) == 1)
	{
		printf("Invalid input!\n(only letters allowed)\n");
		printf("Start point: ");
		scanf("%s", new->info.startname);
	}

	printf("Start coordinates: (if the coordinates are 78 degrees North,the input should be 78N.)\n");
	printf("Latitude [deg] N/S:");
	scanf("%s", new->info.startc->latitude);
	while (CheckCoordFormat(new->info.startc->latitude, "latitude") != 1)
	{
		printf("Latitude [deg] N/S:");
		scanf("%s", new->info.startc->latitude);
	}

	printf("Longitude [deg] E/W:");
	scanf("%s", new->info.startc->longitude);
	while (CheckCoordFormat(new->info.startc->longitude, "longitude") != 1)
	{
		printf("Longitude [deg] E/W:");
		scanf("%s", new->info.startc->longitude);
	}

	printf("End point:");
	scanf("%s", new->info.endname);
	while (CheckForNum(new->info.endname) == 1)
	{
		printf("Invalid input!\n(only letters allowed)\n");
		printf("End point: ");
		scanf("%s", new->info.endname);
	}

	printf("End coordinates:\n");
	printf("Latitude [deg] N/S:");
	scanf("%s", new->info.endc->latitude);
	while (CheckCoordFormat(new->info.endc->latitude, "latitude") != 1)
	{
		printf("Latitude [deg] N/S:");
		scanf("%s", new->info.endc->latitude);
	}

	printf("Longitude [deg] E/W:");
	scanf("%s", new->info.endc->longitude);
	while (CheckCoordFormat(new->info.endc->longitude, "longitude") != 1)
	{
		printf("Longitude [deg] E/W:");
		scanf("%s", new->info.endc->longitude);
	}

	printf("Velocity [km/h]:");
	scanf("%lf", &new->info.velocity);

	printf("Time [h]:");
	scanf("%lf", &new->info.time);

	new->next = NULL;

	return new;
}

int CheckCoordFormat(char* str, char* dymention)
{
	for (int i = 0; i < strlen(str); i++){
		if (i == (strlen(str) - 1)){
			if (strcmp(dymention, "latitude") == 0){
				if (str[i] != 'N' && str[i] != 'S')
				{
					printf("Invalid input!\n");
					return 0;
				}
			}else{
				if (str[i] != 'W' && str[i] != 'E')
				{
					printf("Invalid input!\n");
					return 0;
				}
			}
		}else{
			if (str[i] < '0' || str[i] > '9')
			{
				printf("Invalid input!\n");
				return 0;
			}
		}
	}
	return 1;
} 
//vrushta 0 pri nevaliden format
int CheckForNum(char* str)
{
	for (int i = 0; i < strlen(str); i++)
		if (str[i] >= '0' && str[i] <= '9')
			return 1;
	return 0;
}
//vrushta 1 ako ima cifri
double CheckForDouble()
{
	double n;
	if (scanf("%lf", &n) == 1)
	{
		getchar();
		return n;
	}
	else
		printf("Invalid input!\n(only letters allowed)\n");
	return 0;
}
//vrushta 0 pri greshka, nedovurshena
int CheckStartEnd(t_journey var)
{
	int flag = 0;
	if (strcmp(var.startc->latitude, var.endc->latitude) == 0)
		flag = 1;
	if (strcmp(var.startc->longitude, var.endc->longitude) == 0 && flag == 1)
	{
		printf("The coordinates of the start and the end point can't be the same!\n");
		return 0;
	}
	return 1;
}
//vrushta 0 pri greshno vuvedeni koordinati

t_node* CreateList(t_node** p_head)
{
	t_node* temp = *p_head;
	if (*p_head == NULL)
	{
		temp = NewJourney();
		while (CheckStartEnd(temp->info) != 1)
			temp = NewJourney();
		return temp;
	}

	t_node* new = NewJourney();
	while (CheckStartEnd(new->info) != 1)
		new = NewJourney();

	while (CheckForCoord(*p_head, new->info.startc, new->info.endc) == 1)
	{
		printf("\nThere is already a journey with the same coordinates\n");
		new = NewJourney();
		while (CheckStartEnd(new->info) != 1)
			new = NewJourney();
	}

	new->next = temp;
	temp = new;

	return temp;
}
int CheckForCoord(t_node* p_head, t_coord* coord1, t_coord* coord2)//vrushta 1 pri suotvetstvie
{
	t_node* cur = p_head;
	for (; cur != NULL; cur = cur->next)
		if (strcmp(cur->info.startc->latitude, coord1->latitude) == 0 && strcmp(cur->info.startc->longitude, coord1->longitude) == 0 &&
			strcmp(cur->info.endc->latitude, coord2->latitude) == 0 && strcmp(cur->info.endc->longitude, coord2->longitude) == 0)
		{
			return 1;
		}
	return 0;
}
//vrushta 1 pri suotvetstvie

void PrintOne(t_node* p)
{
	printf("------------------\n");
	printf("Start point: %s ", p->info.startname);
	printf("{lat:%s\tlong:%s}\n", p->info.startc->latitude, p->info.startc->longitude);
	printf("End point: %s ", p->info.endname);
	printf("{lat:%s\tlong:%s}\n", p->info.endc->latitude, p->info.endc->longitude);
	printf("Velocity: %.2lf km/h\n", p->info.velocity);
	printf("Time: %.2lf h\n", p->info.time);
	printf("------------------\n");
}
void PrintALL(t_node* p_head)
{
	t_node* cur = p_head;
	for (; cur != NULL; cur = cur->next)
		PrintOne(cur);
}

unsigned CountList(t_node* p_head)
{
	t_node* cur = p_head;
	unsigned cnt = 0;

	for (; cur != NULL; cur = cur->next)
		cnt++;
	return cnt;
}
void TopShortestJourneys(t_node* p_head, int top)
{
	t_node* cur = p_head;

	double s[MAX_SIZE] = { 0 };
	int i = 0;
	for (; cur != NULL; i++, cur = cur->next)
	{
		s[i] = cur->info.velocity * cur->info.time;
	}
	Sort(s, i);

	FILE* fp = fopen("shortest journeys.txt", "w");
	if (fp != NULL)
	{
		(void)fprintf(fp, "==TOP %d SHORTEST JOURNEYS==\n", top);
		for (int j = 0; j < top; j++)
		{
			(void)fprintf(fp, "%d) %.2lf km\n", j + 1, s[j]);
		}
		printf("File Saved!\n");
		fclose(fp);
	}
	else
	{
		perror(NULL);
		printf("Error opening file!\n");
	}
	fp = NULL;
}
void TopLongestJourneys(t_node* p_head, int top)
{
	t_node* cur = p_head;

	double s[MAX_SIZE] = { 0 };
	int i = 0;
	for (; cur != NULL; i++, cur = cur->next)
	{
		s[i] = cur->info.velocity * cur->info.time;
	}
	Sort(s, i);

	FILE* fp = fopen("longest journeys.txt", "w");
	if (fp != NULL)
	{
		(void)fprintf(fp, "==TOP %d LONGEST JOURNEYS==\n", top);
		for (int j = 1; j <= top; j++, i--)
		{
			(void)fprintf(fp, "%d) %.2lf km\n", j, s[i - 1]);
		}
		printf("File Saved!\n");
		fclose(fp);
	}
	else
	{
		perror(NULL);
		printf("Error opening file!\n");
	}
	fp = NULL;
}
void Sort(double arr[], int size)
{
	double tmp;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (arr[j] > arr[j + 1]) {
				tmp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = tmp;
			}
		}
	}
}

unsigned CountJoureyByCond(t_node* p_head, char* start, char* end)
{
	t_node* cur = p_head;
	unsigned cnt = 0;
	for (; cur != NULL; cur = cur->next)
		if (strcmp(start, cur->info.startname) == 0 && strcmp(end, cur->info.endname) == 0)
			cnt++;
	return cnt;
}
void FindJourney(t_node* p_head, char* start, char* end)
{
	t_node* cur = p_head;
	for (; cur != NULL; cur = cur->next)
		if (strcmp(start, cur->info.startname) == 0 && strcmp(end, cur->info.endname) == 0)
			PrintOne(cur);
}

void SaveInTxtFile(const char* fname, t_node* p_head)
{
	FILE* fp = fopen(fname, "w");
	t_node* cur = p_head;
	if (fp == NULL)
	{
		printf("Error opening file.\n");
		exit(0);
	}
	for (; cur != NULL; cur = cur->next)
	{
		(void)fprintf(fp, "%s ", cur->info.startname);
		(void)fprintf(fp, "{%s, %s}\n", cur->info.startc->latitude, cur->info.startc->longitude);
		(void)fprintf(fp, "%s ", cur->info.endname);
		(void)fprintf(fp, "{%s, %s}\n", cur->info.endc->latitude, cur->info.endc->longitude);
		(void)fprintf(fp, "V= %.2lf km/h\n", cur->info.velocity);
		(void)fprintf(fp, "t: %.2lf h\n\n", cur->info.time);
	}
	fclose(fp);
	fp = NULL;
}
void read_and_create_list(const char* file_name, t_node** p_head)
{
	t_node* temp = NULL;

	FILE* fp = fopen(file_name, "r");
	if (fp == NULL) 
	{
		perror("Error");
		exit(0);
	}

	while (!feof(fp)) {
		if (*p_head == NULL) 
		{   
			*p_head = (t_node*)malloc(sizeof(t_node));
			(*p_head)->info.startc = (t_coord*)malloc(sizeof(t_coord));                  
			(*p_head)->info.startc->longitude = (char*)malloc(sizeof(char) * MAX_SIZE);  
			(*p_head)->info.startc->latitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			(*p_head)->info.endc = (t_coord*)malloc(sizeof(t_coord));
			(*p_head)->info.endc->longitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			(*p_head)->info.endc->latitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			(*p_head)->info.startname = (char*)malloc(sizeof(char) * MAX_SIZE);
			(*p_head)->info.endname = (char*)malloc(sizeof(char) * MAX_SIZE);
			if (*p_head == NULL || (*p_head)->info.startc == NULL || (*p_head)->info.startc->longitude == NULL ||
				(*p_head)->info.startc->latitude == NULL || (*p_head)->info.endc == NULL || (*p_head)->info.endc->longitude == NULL ||
				(*p_head)->info.endc->latitude == NULL || (*p_head)->info.startname == NULL || (*p_head)->info.endname == NULL) {
				printf("Memory error...\n");
				exit(0);
			}

			(void)fscanf(fp, "%s {%3s, %3s}\n", (*p_head)->info.startname, (*p_head)->info.startc->latitude, (*p_head)->info.startc->longitude);                            
			(void)fscanf(fp, "%s {%3s, %3s}\n", (*p_head)->info.endname, (*p_head)->info.endc->latitude, (*p_head)->info.endc->longitude);
			(void)fscanf(fp, "V= %lf km/h\n", &(*p_head)->info.velocity);    
			(void)fscanf(fp, "t: %lf h\n\n", &(*p_head)->info.time);         

			(*p_head)->next = NULL;

			temp = *p_head;
		}
		else 
		{                                                                      
			temp->next = (t_node*)malloc(sizeof(t_node));                      
			temp->next->info.startc = (t_coord*)malloc(sizeof(t_coord));       
			temp->next->info.startc->longitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			temp->next->info.startc->latitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			temp->next->info.endc = (t_coord*)malloc(sizeof(t_coord));
			temp->next->info.endc->longitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			temp->next->info.endc->latitude = (char*)malloc(sizeof(char) * MAX_SIZE);
			temp->next->info.startname = (char*)malloc(sizeof(char) * MAX_SIZE);
			temp->next->info.endname = (char*)malloc(sizeof(char) * MAX_SIZE);
			if (temp->next == NULL || temp->next->info.startc == NULL || temp->next->info.startc->longitude == NULL ||
				temp->next->info.startc->latitude == NULL || temp->next->info.endc == NULL || temp->next->info.endc->longitude == NULL ||
				temp->next->info.endc->latitude == NULL || temp->next->info.startname == NULL || temp->next->info.endname == NULL) {
				(void)printf("Memory error...\n");
				FreeList(&p_head); 
				exit(0);                 
			}                        

			(void)fscanf(fp, "%s {%3s, %3s}\n", temp->next->info.startname, temp->next->info.startc->latitude, temp->next->info.startc->longitude);
			(void)fscanf(fp, "%s {%3s, %3s}\n", temp->next->info.endname, temp->next->info.endc->latitude, temp->next->info.endc->longitude);
			(void)fscanf(fp, "V= %lf km/h\n", &temp->next->info.velocity);
			(void)fscanf(fp, "t: %lf h\n\n", &temp->next->info.time);

			temp->next->next = NULL;

			temp = temp->next;
		}
	}
	temp = NULL;                            
	fclose(fp);
	fp = NULL;
}

void FreeList(t_node** p_head)
{
	t_node* temp = NULL;

	for (; *p_head != NULL; *p_head = temp) 
	{
		temp = (*p_head)->next;                               
		free((*p_head)->info.startc->longitude);              
		free((*p_head)->info.startc->latitude);
		free((*p_head)->info.startc);                         
		free((*p_head)->info.endc->longitude);
		free((*p_head)->info.endc->latitude);
		free((*p_head)->info.endc);
		free((*p_head)->info.startname);
		free((*p_head)->info.endname);
		free(*p_head);                                       
	}
	*p_head = NULL;
	temp = NULL;
}


void TopShortestJourneys1(t_node* p_head, int top)
{
	t_node* cur = p_head;

	t_journey ar[MAX_SIZE] = { 0 };
	double s[MAX_SIZE] = { 0 };

	int i = 0;
	for (; cur != NULL; i++, cur = cur->next)
	{
		ar[i].startname = (char*)malloc(sizeof(char) * MAX_SIZE);
		ar[i].endname = (char*)malloc(sizeof(char) * MAX_SIZE);
		if (ar[i].startname == NULL || ar[i].endname == NULL)
		{
			printf("Memory error...\n");
			exit(0);
		}
		s[i]= cur->info.velocity * cur->info.time;
		ar[i] = cur->info;
	}
	Sort(s, i);
	SortList(ar, i);

	FILE* fp = fopen("shortest journeys.txt", "w");
	if (fp != NULL)
	{
		(void)fprintf(fp, "==TOP %d SHORTEST JOURNEYS==\n", top);
		for (int j = 0; j < top; j++)
		{
			(void)fprintf(fp, "%d) %s -> %s\n",j + 1,ar[j].startname,ar[j].endname);
			(void)fprintf(fp, "%.2lf km\n\n",s[j]);
		}
		printf("File Saved!\n");
		fclose(fp);
	}
	else
	{
		perror(NULL);
		printf("Error opening file!\n");
	}
	for (int j = 0; j < i; j++){
		free(ar[j].startname);
		free(ar[j].endname);
		ar[j].startname = NULL;
		ar[j].endname = NULL;
	}
	fp = NULL;
}
void TopLongestJourneys1(t_node* p_head, int top)
{
	t_node* cur = p_head;

	t_journey arr[MAX_SIZE] = { 0 };
	double s[MAX_SIZE] = { 0 };

	int i = 0,size;
	for (; cur != NULL; i++, cur = cur->next)
	{
		arr[i].startname = (char*)malloc(sizeof(char) * MAX_SIZE);
		arr[i].endname = (char*)malloc(sizeof(char) * MAX_SIZE);
		if (arr[i].startname == NULL || arr[i].endname == NULL)
		{
			printf("Memory error...\n");
			exit(0);
		}
		s[i] = cur->info.velocity * cur->info.time;
		arr[i] = cur->info;
	}
	size = i;
	Sort(s, i);
	SortList(arr, i);

	FILE* fp = fopen("longest journeys.txt", "w");
	if (fp != NULL)
	{
		(void)fprintf(fp, "==TOP %d LONGEST JOURNEYS==\n", top);
		for (int j = 1; j <= top; j++, i--)
		{
			(void)fprintf(fp, "%d) %s -> %s\n", j, arr[i-1].startname, arr[i-1].endname);
			(void)fprintf(fp, "%.2lf km\n\n", s[i-1]);
		}
		printf("File Saved!\n");
		fclose(fp);
	}
	else
	{
		perror(NULL);
		printf("Error opening file!\n");
	}
	for (int j = 0; j < size; j++) {
		free(arr[j].startname);
		free(arr[j].endname);
		arr[j].startname = NULL;
		arr[j].endname = NULL;
	}
	fp = NULL;
}
void SortList(t_journey array[], int size)
{
	t_journey tmp;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size - i - 1; j++) {
			if (array[j].velocity*array[j].time > array[j + 1].velocity*array[j+1].time) {
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
		}
	}
}