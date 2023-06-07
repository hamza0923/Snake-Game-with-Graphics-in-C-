#include<graphics.h>
#include<conio.h>
#include<windows.h>
#include<fstream>
using namespace std;

#define EASY 			1
#define HARD 			2
#define RESTART			3
#define KEY_LEFT        75
#define KEY_RIGHT       77
#define KEY_UP        	72
#define KEY_DOWN        80

//---------------------------------------------------------------------------------------------------------------
		const int width = 27;
		const int height = 34;
		const int res=19;
		const int ofsetx=120,ofsety=50;
//---------------------------------------------------------------------------------------------------------------
		bool gameover;
		int x, y, fruitX, fruitY, score;
		int tailX[width*height], tailY[width*height]; //coordinates for max snake tail lenght
		int nTail;
		enum Directon {STOP=0, LEFT,RIGHT, UP, DOWN}; 
		Directon dir;		//controls
		int Gx[width], Gy[height];	//coordinates for GUI grid
		fstream Score_Backup;
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Processing 
{
	private:
		int high_score;
		POINT cursor;				//point is a built in class, cursor is an object containing x,y;
			int xp,yp;
	public:
		Processing()
		{
			xp=0;yp=0;
		}
		int choice_input1()				//function to take input from the mouse
		{
							
			while(1)
			{											
				GetCursorPos(&cursor);	//every time the address of the object cursor is passed to this functions
						xp=cursor.x;	//the variables in the cursor gets assigned the x,y corrdinates of the mouse
						yp=cursor.y;	
				if(GetAsyncKeyState(VK_LBUTTON)&&xp>550&&xp<670&&yp>425&&yp<485)	//GetAsynckeystate() is a function that 
				{																	//is used to detect if the key, passed as an argument
					return EASY;													// is pressed or not.
					break;
				}
				if(GetAsyncKeyState(VK_LBUTTON)&&xp>700&&xp<820&&yp>425&&yp<485)
				{
					return HARD;
					break;
				}
				if(GetAsyncKeyState(VK_LBUTTON)&&xp>1000&&xp<1140&&yp>600&&yp<645)
				{
					return RESTART;
					break;
				}
				delay(1);
			}
		}
		void Setup()			//function to initialize the values of all the variables
		{
			gameover=false;
			dir=STOP;
			x=width/2;
			y=height/2;
			fruitX=rand()%width; //display fruit in a random place
			fruitY=rand()%height; 
			score=0;
			int tempx=0,tempy=0;		
			for(int i=0;i<=width;i++)	//following code is being used for creating the grid of the snake console
			{
				Gx[i]=tempx+ofsetx;
				tempx=tempx+res; 
			}
			for(int i=0;i<=height;i++)
			{
				Gy[i]=tempy+ofsety;
				tempy=tempy+res;
			}
		}
		void algorithm(int m)		//the fuction does all the logical work required to increase the lenght of the snake,
		{							//assign the values of the direction, and detecting gameover conditions.
			int prev2X,prev2Y;
			int prevX=tailX[0];		//following code is being used to assign the previous locations of the snake body
			int prevY=tailY[0];		//in an array which will be
			
			tailX[0]=x;
			tailY[0]=y;
			for(int i=1;i<nTail;i++) 
			{
				prev2X=tailX[i];
				prev2Y=tailY[i];
				tailX[i]=prevX;
				tailY[i]=prevY;
				prevX=prev2X;
				prevY=prev2Y ;
			}
			switch (dir) 	//following code is being used to move the snake  according to the input directions
			{
				case LEFT:
					x--;
				break;
				case RIGHT:
					x++;
				break;
				case UP:
					y--;
				break;
				case DOWN:
					y++;
				break;
				default:
				break;
			}
			if(m==HARD)			//code being used to gameover if the snake head crash in the console wall
				if(x>width-1||y>height-1||x<0||y<0)
				{
					gameover=true;
				}
			if(m==EASY)			//followiing code is being used to jump the snake from one side to the opposite
			{					//side of the console
				if(x>=width)
					x=0;
				else if(x<0)
					x=width-1;
				if(y>=height) 
					y=0; 
				else if(y<0) 
					y=height-1;
			}
			for(int i=0;i<nTail;i++)	//code being used to gameover if snake bumps into it self
				if (tailX[i]==x&&tailY[i]==y)
					gameover=true;
			if (x==fruitX&&y==fruitY) 	//code being used to increase the score, snake size and puting fruit in 
			{							//in new place
				score+=10;
				fruitX=rand()%width;
				fruitY=rand()%height;
				nTail++;
			}
		}
		void key_input()		//following code is being used to take the arrow key inputs
		{
			if(kbhit())		//kbhit() detects if a key is pressed or not
			{
				for(int i=0;i<2;i++)
				{
    				switch(getch()) //getch() to recieve the ASCII code of the keys
					{
    					case KEY_UP:
    						if(dir!=DOWN&&dir!=UP)
								dir=UP;  //key up
     				    break;
    					case KEY_DOWN:
							if(dir!=DOWN&&dir!=UP) 
								dir=DOWN;  // key down
        				break;
						case KEY_LEFT:
							if(dir!=LEFT&&dir!=RIGHT)
								dir=LEFT;  // key left
        				break;
						case KEY_RIGHT:
							if(dir!=LEFT&&dir!=RIGHT)
								dir=RIGHT;  // key right
						break;
    				}
				}
			}
		}
		int file_handle(int s)			//function being used for saving the high score in the file
		{
			Score_Backup.open("hs.txt",ios::in);
			Score_Backup>>high_score;
			Score_Backup.close();
			
			if(s>high_score)
			{
				Score_Backup.open("hs.txt",ios::out);
				high_score=s;
				Score_Backup<<high_score;
				Score_Backup.close();
			}
			return high_score;
		}
		void speed(int m)	//give appropriate delay in the loop to create the effect of controling snake speed
		{
			if(m==EASY)
				delay(70);
			if(m==HARD)
			{
				delay(15);
				delay(60-(score/10));
			}
		}
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Graphics
{
	private:
		long int X_PIXELS,Y_PIXELS;
	public:
		Graphics()
		{
			X_PIXELS=GetSystemMetrics(SM_CXSCREEN);	//function to get horizontal pixel count of the system [1365 px]
			Y_PIXELS=GetSystemMetrics(SM_CYSCREEN);//function to get the vertical pixel count of the system [767 px]
			char arr[25]="SNAKE GAME CONSOLE";
			initwindow(X_PIXELS,Y_PIXELS,arr);  //function creates the window
		}
		void set_background1()   //function creates the intro background
		{
			cleardevice();							//clears the device screen
			setbkcolor(LIGHTCYAN);					//puts background color
			setfillstyle(LTBKSLASH_FILL,WHITE);		//fills the given area with color and texture
			bar(0,0,X_PIXELS,Y_PIXELS);				//creates a rectangle filled with color at the given location
			setfillstyle(SOLID_FILL,CYAN);			
			bar(30,30,X_PIXELS-30,Y_PIXELS-60);
		}
		void set_background2(int m)		//function to create the playing screen background
		{
			setbkcolor(BLACK);
			setcolor(YELLOW);						//sets a specific color for every further thing that is to be drawn
			rectangle(ofsetx-3,ofsety-3,width*res+ofsetx+3,height*res+ofsety+3);
			rectangle(ofsetx-10,ofsety-10,width*res+ofsetx+10,height*res+ofsety+10);
			if(m==EASY)
				setfillstyle(SLASH_FILL,YELLOW);	
			if(m==HARD)
				setfillstyle(SOLID_FILL,YELLOW);
			floodfill(ofsetx-9,ofsety-9,YELLOW);
			rectangle(-1,-1,X_PIXELS+1,Y_PIXELS+1);
			setfillstyle(SOLID_FILL,LIGHTCYAN);
			floodfill(1,1,YELLOW);
		}
		void write_title_name()		//function to draw the title name of the game on background1
		{
			settextstyle(BOLD_FONT,0,20);
			setbkcolor(CYAN);
			char arr[10]="SNAKE";
			outtextxy(X_PIXELS/3+37,100,arr);
		}	
		void draw_title_underline() //function to draw the line under the title
		{
			setlinestyle(DASHED_LINE,0,10);
			line(X_PIXELS-1165,250,X_PIXELS-200,250);
		}
		void selection_menu()	//function to draw the hard/easy mode choice selection menu
		{
			settextstyle(BOLD_FONT,0,3);					//sets the text font,size,direction for every text to be
			char arr1[20]="Select Game Mode";				//further drawn
			char arr2[5]="EASY";
			char arr3[5]="HARD";
			outtextxy(X_PIXELS/3+120,320,arr1);				//outputs the char string at given corrdinates
			rectangle(550,400,670,450);		//option Box 1
			rectangle(700,400,820,450);		//option Box 2
			setcolor(DARKGRAY);
			settextstyle(SANS_SERIF_FONT,0,2);
			outtextxy(582,413,arr2);
			outtextxy(730,413,arr3);
		}
		void Draw_Snake()	//function draws the body of the snake and the fruite
		{
			for(int i=0;i<height;i++) 
			{
				for(int j=0;j<width;j++) 
				{
					if(i==y&&j==x)
					{
						if(gameover==true)
							{
							setcolor(LIGHTRED);
							setfillstyle(XHATCH_FILL,RED);
							}
						setcolor(LIGHTGREEN);
						setfillstyle(XHATCH_FILL,GREEN);
						if(dir==LEFT||dir==RIGHT||dir==STOP)
							fillellipse(Gx[x]+res/2,Gy[y]+res/2,res+4,res/2+4);
						if(dir==UP||dir==DOWN)
							fillellipse(Gx[x]+res/2,Gy[y]+res/2,res/2+4,res+4);
					}// snake head
					else if(i==fruitY&&j==fruitX)
					{
						setcolor(YELLOW);
						setlinestyle(SOLID_LINE,0,2);
						arc(Gx[fruitX],Gy[fruitY]+5,260,50,res/2+5+1);
						arc(Gx[fruitX],Gy[fruitY]+5,250,60,res/2+5);
						arc(Gx[fruitX],Gy[fruitY]+5,260,50,res/2+5-1);
						arc(Gx[fruitX],Gy[fruitY]+5,270,40,res/2+5-2);
						arc(Gx[fruitX],Gy[fruitY]+5,280,30,res/2+5-4);
						arc(Gx[fruitX],Gy[fruitY]+5,290,20,res/2+5-6);
					} //fruit
					else 
					{
						for(int k=0;k<nTail;k++) 
						{
							if(tailX[k]==j&&tailY[k]==i) 
							{
								setcolor(LIGHTGREEN);
								setfillstyle(XHATCH_FILL,GREEN);
								fillellipse(Gx[tailX[k]]+res/2,Gy[tailY[k]]+res/2,res/2+3,res/2+3);
								
							} //tail
						}				
					}			
				} //end of for loop
			} //end of for loop
		}//end of Draw_Snake() func
		void Draw_Score(Processing P)	//function to draw score and highscore
		{
			setcolor(BLACK);
			setlinestyle(0,0,4);
			line(X_PIXELS-480,80+10,X_PIXELS-480,600);
			setlinestyle(0,0,2);
			char S[50];
			sprintf(S,"Score: %d",score);  //%d is a formate specifier
			setcolor(RED);
			setbkcolor(LIGHTCYAN);
			settextstyle(BOLD_FONT,0,6);
			outtextxy(X_PIXELS-470,Y_PIXELS/3+30,S);
			char HS[50];
			settextstyle(BOLD_FONT,0,3);
			sprintf(HS,"Highest Score: %d",P.file_handle(score)); //%d is formate specifier
			outtextxy(X_PIXELS-483,70,HS);
			setbkcolor(BLACK);
		}		
		void Draw_Gameover()	//function to draw the game over message
		{
			int count=0;
			char arr[15]="GAME OVER!";
			setcolor(BLACK);
			setbkcolor(LIGHTRED);
			settextstyle(SANS_SERIF_FONT,0,5);
			outtextxy(((width*res)/2),((height*res)/2)+ofsety-35,arr);
			setbkcolor(LIGHTRED);
		}
		void highlight_button(int x,int y)	//function to high light the clicked button according to the coordinates recieved
		{
			setfillstyle(SOLID_FILL,LIGHTGRAY);
			floodfill(x,y,WHITE);		//highlighting button
		}
		void restart_button()
		{
			setlinestyle(0,0,4);
			setcolor(RED);
			setlinestyle(SOLID_LINE,0,5);
			rectangle(1000,600,1140,645);
			setlinestyle(SOLID_LINE,0,1);
			char arr[10]="RESTART";
			setbkcolor(LIGHTCYAN);
			settextstyle(SANS_SERIF_FONT,0,3);
			outtextxy(1013,608,arr);
			setbkcolor(BLACK);
		}

};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Sequence 
{
	public:
		Sequence(Processing pr)		//calls the setup function of the proccessing class
		{
			pr.Setup();
		}
		void easy_game_sequence(Graphics graphics,Processing processing)	//calls the functions with required EASY arguments
		{
			graphics.highlight_button(560,410);
			delay(200);
			while(1)
			{
				int page=1;
				setvisualpage(page);		//code for output screen double buffering
				setactivepage(1-page);
				cleardevice();
				graphics.set_background2(EASY);
				graphics.Draw_Snake();
				graphics.Draw_Score(processing);
				if(!gameover)
				{
					processing.key_input();
					processing.algorithm(EASY);
				}
				if(gameover)
				{
					graphics.Draw_Gameover();
					graphics.restart_button();
				//	if(processing.choice_input1()==RESTART)
				//	{
				//		graphics.highlight_button(1005,605);
				//	}
				}
				processing.speed(EASY);
				page=page-1;
			}
		}
		void hard_game_sequence(Graphics graphics,Processing processing)
		{
			graphics.highlight_button(710,410);
			delay(200);		
			while(1)
			{
				int page=1;
				setvisualpage(page);
				setactivepage(1-page);
				cleardevice();
				graphics.set_background2(HARD);
				graphics.Draw_Snake();
				graphics.Draw_Score(processing);
				if(!gameover)
				{
					processing.key_input();
					processing.algorithm(HARD);
				}
				if(gameover)
				{
					graphics.Draw_Gameover();
					graphics.restart_button();
				}
				processing.speed(HARD);
				page=page-1;
			}
		}
};

int main()
{ 
	Graphics Interface;
	Processing Logic;
	Sequence Play(Logic);
	bool reset=false;
	do
	{
		Interface.set_background1();
		Interface.write_title_name();
		Interface.draw_title_underline();
		Interface.selection_menu();
		switch(Logic.choice_input1())
		{
			case EASY:	
				Play.easy_game_sequence(Interface,Logic);
			break;
			case HARD:
				Play.hard_game_sequence(Interface,Logic);
			break;
		}
		if(gameover&&Logic.choice_input1()==RESTART)
			reset==true;
	}while(reset==true);
	getch();
	closegraph();
}
