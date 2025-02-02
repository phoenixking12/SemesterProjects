#include<stdio.h>
#include<conio.h>
#include<dos.h>
#include<windows.h>
#include<time.h>
#include<stdbool.h>
#include<MMsystem.h>
#include<unistd.h>

#define GAMELENGTH 70		//to fix length of playable area
#define SCREENLENGTH 90		//total console length
#define GAPSIZE 7			
#define OBSTACLEGAP 45		//gap between  successive obstacles
#define SCREENHEIGHT 26		//height of console
#define MENULENGTH 20		//length of menu area

COORD CursorPosition;		//COORD structure to hold coordinates

int obsPos[2];	
int obsFlag[2], gapPos[2];					
char bird[2][6] = {
 				   	'/','-','-','o','\\',' ',
					'|','_','_','_',' ','>' 
				  };
int yPos = 6, xPos = 2, score = 0;

void coordinate(int x, int y)
{
	//handle console to access buffer
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);	//to retrieve output
	CursorPosition.X = x;
	CursorPosition.Y = y;
	SetConsoleCursorPosition(console, CursorPosition);
}

void createbird()
{
	int i, j;
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 6; j++)
		{
			coordinate(j + xPos, i + yPos); 
			printf("%c", bird[i][j]);
		}
	}
}

void eraseBird()
{
	int i, j;
	for(i = 0; i < 2; i++)
	{
		for(j = 0; j < 6; j++)
		{
			coordinate(j + xPos, i + yPos);
			 printf(" ");
		}
	}
}

void Bound()
{ 
	int i;
	for(i = 0; i < SCREENLENGTH; i++)		//horizontal borders
	{
		coordinate(i, 0);
		printf("±");
		coordinate(i, SCREENHEIGHT);
		printf("±");
	}
	
	for(i = 0; i < SCREENHEIGHT; i++)	//vertical borders
	{
		coordinate(0, i); 
		printf("±");
		coordinate(SCREENLENGTH,i); 
		printf("±");
	}
	for(i = 0; i < SCREENHEIGHT; i++)
	{
		coordinate(GAMELENGTH, i); 
		printf("±");
	}
}

void genObstacle(int index)		//for obstacle height  
{
	gapPos[index] = 3 + rand() % 14; //3 min 16 max 
}

void createObstacle(int index)
{	
	int num, i;
	srand(time(0));
	num = rand() % 100000;
	if(obsFlag[index] == true)
	{
		for(i = 0; i < gapPos[index]; i++)		//creating obstacle above gap
		{ 	
			if (num < 15000)
			{
				coordinate(GAMELENGTH - obsPos[index], i + 1); 
				printf("***"); 
			}
			else if (num < 50000 )
			{
				coordinate(GAMELENGTH - obsPos[index], i + 1); 
				printf("###"); 
			}
			else
			{
				coordinate(GAMELENGTH - obsPos[index], i + 1); 
				printf("$$$"); 
			}
		}
				
		for(i = gapPos[index] + GAPSIZE; i < SCREENHEIGHT - 1; i++)	//creating obstacle below gap
		{
			if (num < 15000)
			{
				coordinate(GAMELENGTH - obsPos[index], i + 1); 
				printf("$$$"); 
			}
			else if (num < 40000)
			{
				coordinate(GAMELENGTH - obsPos[index], i + 1); 
				printf("***"); 
			}
			else
			{
				coordinate(GAMELENGTH - obsPos[index], i + 1); 
				printf("###"); 
			}			
		}
	} 
}

void eraseobstacle(int index)	//erasing obstacle as bird progresses
{
	int i;
	if(obsFlag[index] == true)
	{
		for(i = 0; i < gapPos[index]; i++)	//gives top y coordinate
		{ 
			coordinate(GAMELENGTH - obsPos[index], i + 1); // x coordinates
			printf("   "); 
		}
		for(i = gapPos[index] + GAPSIZE; i < SCREENHEIGHT - 1; i++)
		{ 
			coordinate(GAMELENGTH - obsPos[index], i + 1); 
			printf("   "); 
		}
	}
}

int crash()
{
	if( obsPos[0] >= 61)		//checking for symbols that make bird. Condition is true if bird has collided
	{
		if( yPos < gapPos[0] + 1 || yPos > gapPos[0] + GAPSIZE)	//checking bird's y position
		{
			return 1;
		}
	}
	return 0;
}

void highscore(int score)
{
	int highscore;
	FILE *fp;
	fp=fopen("highscore.txt","r");
    fscanf(fp, "%d", &highscore);
    fclose(fp);
    
    if(score > highscore)
	{
        printf("  New HighScore %d!!!!!!\n\n",score);
        system("pause");
        fp =fopen("highscore.txt","w");
        fprintf(fp, "%d", score);
        fclose(fp);
    }

}

void rules()
{
	
	system("cls");
	printf("Instructions");
	printf("\n----------------");
	printf("\n Press D to make the bird fly and S to make it fall");
	printf("\n Press F to go right(Pressing it more than 3 times does nothing)\n Press A to go left");
	printf("\n\nPress any key to go back to menu");
	getch();
	system("cls");
}

void end()
{
	system("cls");
	puts("");
	puts("\t\t--------------------------");
	puts("\t\t-------- Game Over -------");
	puts("\t\t--------------------------");
	puts("\t\tPress any key to go back to menu.");
	getch();
	system("cls");
}

void play()
{
	char ch;
	yPos = 4, score = 0;
	obsFlag[0] = 1; 			//first flag is 1 to always create the obstacle
	obsFlag[1] = 0;				//second flag is 0 until conditionis true where obPos has increased to 41
	obsPos[0] = obsPos[1] = 4;
	xPos = 2;
	
	system("cls"); 
	Bound();
	genObstacle(0);
	coordinate(GAMELENGTH + 7, 5);
	printf("Score: %d", score);
	
	coordinate(GAMELENGTH + 5, 2);
	printf("FLAPPY BIRD");
	coordinate(GAMELENGTH + 6, 4);
	printf("----------");
	coordinate(GAMELENGTH + 6, 6);
	printf("----------");
	coordinate(GAMELENGTH + 7, 12);
	printf("Control ");
	coordinate(GAMELENGTH + 7, 13);
	printf("-------- ");
	coordinate(GAMELENGTH + 2, 14);
	printf("D=jump");
	coordinate(GAMELENGTH + 2, 15);
	printf("S=down");
	coordinate(GAMELENGTH + 2, 16);
	printf("A=left");
	coordinate(GAMELENGTH + 2, 17);
	printf("F=right");
	coordinate(GAMELENGTH + 7, 18);
	printf("WARNING");
	coordinate(GAMELENGTH + 2, 19);
	printf("F can't be pressed");
	coordinate(GAMELENGTH + 2, 20);
	printf("more than 3 times");
	coordinate(10, 5);
	printf("Press any key to start");
	getch();
	coordinate(10, 5);
	printf("                      ");
	
	while(1)
	{	 
		if(kbhit())
		{
			 ch = getch();
			if(ch == 100)
			{
				if( yPos > 3)
					yPos -= 3;	//d
			} 
			else if (ch == 115)
			{
				if(yPos > 3)
					yPos += 3;	//s
			}
			else if (ch == 102 && xPos < 8)
			{
				xPos += 2;//f
				if (crash() == 1)
				{
					highscore(score);
					end();
					return;
				}
			}
			else if (ch == 97)
			{
				xPos -= 3;
				if (xPos < 2)//a
				{
					end();
					return;
				}
			}
			if(ch == 27)
				break;//esc
		}
		
		createbird();
		createObstacle(0);		//creates first obstacle
		createObstacle(1);		//creates second obstacle
		if( crash() == 1)
		{
			highscore(score);
			end();
			return;
		}
		Sleep(100);
		eraseBird();		//erasing bird after every jump
		eraseobstacle(0);
		eraseobstacle(1);
		yPos += 1;  //to bring the bird down
		
		if(yPos > SCREENHEIGHT - 2)		//ends at lower border
		{
			end();
			return;
		}
		
		if (obsFlag[0] == 1)		
			obsPos[0] += 2;
		
		if(obsFlag[1] == 1)
			obsPos[1] += 2;
		
		if(obsPos[0] >= 40 && obsPos[0] < 42)
		{
			obsFlag[1] = 1;
			obsPos[1] = 4;
			genObstacle(1);
		}
		if(obsPos[0] > 68)
		{
			score++;
			coordinate(GAMELENGTH + 7, 5);
			printf("Score: %d", score);
			obsPos[0] = obsPos[1];
			gapPos[0] = gapPos[1];
		}		
	}	 
}

int main()
{
	char ch;
	int num;
	
	puts("Enter 1 for I ain't worried by One republic\nPress 2 for Heaven I know by Gordi\nPress 3 NCS high background music\n Press any key else for no music");
	scanf("%d", &num);
	system("cls");
	switch(num)
	{
		case 1:
			PlaySound(TEXT("onerepublic.wav"), NULL, SND_ASYNC);
			break;
		case 2:
			PlaySound(TEXT("heaven.wav"), NULL, SND_ASYNC);
			break;
		case 3:
			PlaySound(TEXT("ncshigh.wav"), NULL, SND_ASYNC);
			break;
		default:
			break;
	}

	do
	{
		coordinate(10,5); 
		puts(" -------------------------- "); 
		coordinate(10,6); 
		puts(" |      Flappy Bird       | "); 
		coordinate(10,7); 
		puts(" --------------------------");
		coordinate(10,9); 
		puts("1. Start Game");
		coordinate(10,10); 
		puts("2. Instructions");	 
		coordinate(10,11); 
		puts("3. Quit");
		coordinate(10,13); 
		puts("Select option: ");
		ch = getche();
		
		if( ch == '1')
		{	
		 	play();
		} 
		else if( ch == '2')
			rules();
		else if( ch == '3')
			exit(0);		
	}
	while(1);
	
	return 0;
}
