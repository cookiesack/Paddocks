#include <iostream>
#include <windows.h>
#include <sstream>
#include <string.h>
#include <conio.h>
#include <WinUser.h>

void PrintBoard(int height, int width);
void SetCursor(int x, int y);
void InitializeArrays();
void ChangeCursor(BOOL);
void GetDesktopResolution(int& horizontal, int& vertical);
void PrintBoardPreview(int, char);
void MainMenu();
void ComputerTurn();
void HorizontalLine(HDC, int, int);
void VerticalLine(HDC, int, int);
void FillSquare(HDC hDC, int squareX, int squareY);
void CheckBoxes();
int CheckLines(int size);
void Instructions();
void Options();
void Controls();
int GetKeyName(int keyInMenu);
void DrawFilledLinesBoxes(HDC hWndDC);
int CheckWin(HWND hWnd);
bool RemoveScrollbar();

int currX=0, currY=0, size=0, sizeIn=3, filledBox[24][24], score1=0, score2=0, win=0, checkWin=0, turn=1, scored=1, sizeInChange=0, horRes=0, verRes=0, keyDown=0, menuItemMove=0, menuItemSelected=0, keyMoveUp=72, keyMoveRight=77, keyMoveLeft=75, keyMoveDown=80, keySwitch='z', keyDraw=32, keyMenu=27, keyQuit='q', keyRestart='r';
bool horizontal = true, filledh[25][25], filledv[25][25], space = false, viewMainMenu=true, mouseInput=false, moved=false, compSmart=false, gamePassed=false, changedSize=false;
const char arrowUp=0x18, arrowDown=0x19;
std::string p1In="", p2In="", p1tIn="", p2tIn="", p1="Player 1", p2="Player 2", p1t="P1", p2t="P2", multiplayer="Off";
POINT cursorPos={0};
RECT wndRect={0};
COORD new_size = {0};
CONSOLE_CURSOR_INFO cursor;
CONSOLE_FONT_INFO cfi;
CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
PCONSOLE_SCREEN_BUFFER_INFO pbufferInfo;
HWND hWnd;
HANDLE hout = ::GetStdHandle(STD_OUTPUT_HANDLE);
HANDLE hin = ::GetStdHandle(STD_INPUT_HANDLE);
std::stringstream sStrWinner;

int main()
{
	::srand(GetTickCount());
	GetCurrentConsoleFont(hout, false, &cfi);
	COORD unit_size = GetConsoleFontSize(hout, cfi.nFont);
	::SetConsoleTitle("Paddocks  [(c) M. Chulev]");
	hWnd = ::GetConsoleWindow();
	//Get a handle to device context
	//HDC hWndDC = GetDC(hWnd);
restartGame:
	if(viewMainMenu==true)
	{
		::system("cls");
		::ChangeCursor(FALSE);
		::GetDesktopResolution(horRes, verRes);
		new_size.X = 800;
		new_size.Y = 800;
		::SetConsoleScreenBufferSize(hout, new_size);
		::MoveWindow(hWnd,(horRes-530)/2,(verRes-240)/2,530,240,1);
		::RemoveScrollbar();
		::MainMenu();
		viewMainMenu=false;
	}
	if(win==4)
		return 0;
	::system("cls");
	score1=0; score2=0; checkWin=0; turn=1; scored=1; sizeInChange=0; horizontal = true; space = false; keyDown=0, mouseInput=false;
	//Initialize filledh, filledv, filledBox
	::InitializeArrays();
	size=sizeIn+1;

	//Position and resize console window and buffer
	new_size.X = 800;
	new_size.Y = 800;
	::SetConsoleScreenBufferSize(hout, new_size);
	::MoveWindow(hWnd,(horRes-(((size+3)*3+2)*unit_size.X))/2,(verRes-((size+3)*2*unit_size.Y+40))/2,((size+3)*3+2)*unit_size.X,(size+3)*2*unit_size.Y+52,1);
	::RemoveScrollbar();

	const int bottom = (size-1)*2;
	const int right  = (size-1)*3;

	//Print board
	::PrintBoard(size, size);
	::ChangeCursor(TRUE);
	mouseInput=false;

	//Start game
	while(win==0)
	{
		//::FindWindow(0, "Paddocks  [(c) M. Chulev]");

		if(::GetForegroundWindow()==hWnd)
		{
			if(turn==1 || multiplayer=="On")
			{
				keyDown=_getch();
				if(keyDown==keyMoveLeft) // ArrowKey Left
				{
					if((currX!=1) && (currX!=0))
						currX-=3; 
					else if(currX==1)
						currX=right-2;
					else if(currX==0)
						currX=right;
					::SetCursor(currX, currY);
					moved=true;
				}
				if(keyDown==keyMoveUp) // ArrowKey Up
				{
					if((currY!=0) && (currY!=1))
						currY-=2;
					else if(currY==0)
						currY=bottom;
					else if(currY==1)
						currY=bottom-1;
					::SetCursor(currX, currY);
					moved=true;
				}
				if(keyDown==keyMoveRight) // ArrowKey Right
				{
					if((currX!=(right-2)) && (currX!=right))
						currX+=3;
					else if(currX==(right-2))
						currX=1;
					else if(currX==right)
						currX=0;
					::SetCursor(currX, currY);
					moved=true;
				}
				if(keyDown==keyMoveDown) // ArrowKey Down
				{
					if(currY!=bottom && currY!=(bottom-1))
						currY+=2;
					else if(currY==bottom)
						currY=0;
					else if(currY==(bottom-1))
						currY=1;
					::SetCursor(currX, currY);
					moved=true;
				}
				space = false;
				if(keyDown==keyDraw) // Spacebar
				{
					if((horizontal == true) && (filledh[(currX+2)/3-1][currY/2] == false))
					{
						::SetCursor(currX, currY);
						std::cout<<(char)205<<(char)205;
						filledh[(currX+2)/3-1][currY/2] = true;
						space = true;
					}
					else if((horizontal == false) && (filledv[currX/3][(currY-1)/2] == false))
					{
						::SetCursor(currX, currY);
						std::cout<<(char)186;
						filledv[currX/3][(currY-1)/2] = true;
						space = true;
					}
					//::DrawFilledLinesBoxes(hWndDC);
					::SetCursor(currX, currY);
					::Sleep(100);
				}
				if(keyDown==keySwitch) // Z
				{
					if(horizontal==true)
					{
						if(currY!=bottom)
							currY+=1;
						else if(currY==bottom)
							currY-=1;
						currX-=1;
						horizontal = false;
					}
					else if(horizontal==false)
					{
						if(currX!=right)
							currX+=1;
						else if(currX==right)
							currX-=2;
						currY-=1;
						horizontal = true;
					}
					::SetCursor(currX, currY);
					::Sleep(200);
					moved=true;
				}
				if(keyDown==keyQuit) // Q
				{
					if(::MessageBox(hWnd, "Exit to Windows?", "Quit", MB_YESNO | MB_SETFOREGROUND | MB_DEFBUTTON2 | MB_ICONQUESTION)==IDYES)
					{
						win=3;
						return 0;
					}
				}
				if(keyDown==keyRestart) // R
				{
					if(::MessageBox(hWnd, "New game?", "Restart", MB_YESNO | MB_SETFOREGROUND | MB_DEFBUTTON2 | MB_ICONQUESTION)==IDYES)
					{
						goto restartGame;
					}
				}
				if(keyDown==keyMenu) // Esc
				{
					if(::MessageBox(hWnd, "Go back to main menu?", "Exit", MB_YESNO | MB_SETFOREGROUND | MB_DEFBUTTON2 | MB_ICONQUESTION)==IDYES)
					{
						gamePassed=true;
						changedSize=false;
						viewMainMenu=true;
						goto restartGame;
					}
				}
				//}
				//else
				//{
				/*if(GetAsyncKeyState(VK_LBUTTON)<0)
				{
				::GetCursorPos(&cursorPos);
				::GetWindowRect(hWnd, &wndRect);
				if((cursorPos.x>wndRect.left)
				&& (cursorPos.x<wndRect.right-40)
				&& (cursorPos.y>wndRect.top+40)
				&& (cursorPos.y<wndRect.bottom-120))
				//<right-40, >left, >top+40 //(cursorPos.y<wndRect.top+51)
				{
				//std::cout<<"--";
				filledh[(int)((cursorPos.x-wndRect.left-10)/24)][(int)((cursorPos.y-(wndRect.top+34))/24)]=true;
				filledv[(int)((cursorPos.x-wndRect.left-10)/24)][(int)((cursorPos.y-(wndRect.top+34))/24)]=true;
				}
				//::DrawFilledLinesBoxes(hWndDC);
				}*/
			}

			scored=0;

			if(turn==2 && multiplayer=="Off")
			{
				::ChangeCursor(FALSE);
				::ComputerTurn();
				::ChangeCursor(TRUE);
			}
			::CheckBoxes();
			//::DrawFilledLinesBoxes(hWndDC);

			if((score1+score2)==(sizeIn*sizeIn))
				checkWin=1;
			if(::CheckWin(hWnd)!=0)
			{
				win=0;
				viewMainMenu=true;
				menuItemSelected=0;
				gamePassed=true;
				goto restartGame;
			}

			if((scored==0) && (space==true))
			{
				if(turn==2)
				{
					turn=1;
					::SetCursor(0, size*2);
					::SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
					std::cout<< p1 << ":" <<std::endl;
					::SetConsoleTextAttribute(hout, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
					::SetCursor(0, size*2+2);
					std::cout<< p2 << ":" <<std::endl;
					::SetCursor(currX, currY);
				}
				else
				{
					turn=2;
					::SetCursor(0, size*2+2);
					::SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_RED);
					std::cout<< p2 << ":" <<std::endl;
					::SetConsoleTextAttribute(hout, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
					::SetCursor(0, size*2);
					std::cout<< p1 << ":" <<std::endl;
					::SetCursor(currX, currY);
				}
				space=false;
			}
			if(moved==true)
			{
				::Sleep(150);
				moved=false;
			}
		}
	}
	return 0;
}

void CheckBoxes()
{
	HWND hWnd;
	hWnd = ::GetConsoleWindow();
	//Get a handle to device context
	//HDC hWndDC = GetDC(hWnd);
	for(int posY=0; posY<=(size-2); posY++) // max posY<=23
	{
		for(int posX=0; posX<=(size-2); posX++) //max 23x24 and 24x23
		{
			if(((filledh[0+posX][0+posY] == true)
				&& (filledv[1+posX][0+posY] == true)
				&& (filledh[0+posX][1+posY] == true)
				&& (filledv[0+posX][0+posY] == true)
				&& (filledBox[0+posX][0+posY] != 1)
				&& (filledBox[0+posX][0+posY] != 2))==true)
			{
				filledBox[0+posX][0+posY] = turn;
				if(posX==0)
					::SetCursor(posX+1, posY*2+1);
				else
					::SetCursor(posX*3+1, posY*2+1);
				if(turn==1)
				{
					::SetConsoleTextAttribute(hout, FOREGROUND_BLUE | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY | BACKGROUND_INTENSITY | BACKGROUND_BLUE);
					std::cout<<p1t;
					if(p1t.length()==1)
						std::cout<<" ";
					::SetConsoleTextAttribute(hout, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
					score1++;
					::SetCursor(p1.length()+2, size*2);
					std::cout<<score1<<" points"<<std::endl;
					::SetCursor(currX, currY);
				}
				else
				{
					::SetConsoleTextAttribute(hout, BACKGROUND_INTENSITY | BACKGROUND_RED);
					std::cout<<p2t;
					if(p2t.length()==1)
						std::cout<<" ";
					::SetConsoleTextAttribute(hout, BACKGROUND_INTENSITY | BACKGROUND_BLUE | BACKGROUND_RED | BACKGROUND_GREEN);
					score2++;
					::SetCursor(p2.length()+2, size*2+2);
					std::cout<<score2<<" points"<<std::endl;
					::SetCursor(currX, currY);
				}
				if(turn==1 || multiplayer=="On")
					scored=1;
			}
		}
	}
}

int CheckLines(int size)
{
	int linesHor=0, linesVer=0;
	for(int checkLineX=0; checkLineX<size; checkLineX++)
	{
		for(int checkLineY=0; checkLineY<=size; checkLineY++)
		{
			if(filledh[checkLineX][checkLineY]==true)
				linesHor++;
		}
	}

	for(int checkLineX=0; checkLineX<=size; checkLineX++)
	{
		for(int checkLineY=0; checkLineY<size; checkLineY++)
		{
			if(filledv[checkLineX][checkLineY]==true)
				linesVer++;
		}
	}
	return linesHor+linesVer;
}

void PrintBoardPreview(int size, char change)
{
	::SetCursor(0, 2);
	if(change==' ')
	{
		system("cls");
		::SetCursor(0, 2);
		for(int h=0; h<size+1; h++)
		{
			for(int w=0; w<size+1; w++)
			{
				std::cout<<"O";
				if(w<size)
					std::cout<<"  ";
			}
			if(h<size)
				std::cout << std::endl << std::endl;
		}
	}
	if(change=='+')
	{
		for(int h=2;h<(size+2)*2;h+=2)
		{
			SetCursor(size*3,h);
			std::cout<<"O";
		}
		for(int w=0;w<size*3;w+=3)
		{
			SetCursor(w,(size+1)*2);
			std::cout<<"O";
		}
	}
	if(change=='-')
	{
		for(int h=2;h<(size+3)*2;h+=2)
		{
			SetCursor((size+1)*3,h);
			std::cout<<" ";
		}
		for(int w=0;w<(size+1)*3;w+=3)
		{
			SetCursor(w,(size+2)*2);
			std::cout<<" ";
		}
	}
}

void PrintBoard(int height, int width)
{
	::system("cls");
	::SetCursor(0, 0);
	for(int h=0; h<height; h++)
	{
		for(int w=0; w<width; w++)
		{
			std::cout<<"O";
			if(w<width-1)
				std::cout<<"  ";
		}
		std::cout << std::endl << std::endl;
	}
	::SetConsoleTextAttribute(hout, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY | BACKGROUND_BLUE);
	std::cout<< p1 << ":";
	::SetConsoleTextAttribute(hout, BACKGROUND_INTENSITY | BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
	std::cout << " " << score1 << " points" << std::endl << std::endl;
	std::cout<< p2 << ": " << score2 << " points";
	currX=1;
	currY=0;
	::SetCursor(currX, currY);
}

void SetCursor( int x, int y )
{
	COORD p = { x, y };
	::SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), p );
}

void InitializeArrays()
{
	for(int cx=0; cx<25; cx++)
	{
		for(int cy=0; cy<25; cy++)
		{
			filledh[cx][cy]=false;
			filledv[cx][cy]=false;
			if((cx<24) && (cy<24))
				filledBox[cx][cy]=0;
		}
	}
}

void ChangeCursor(BOOL boolVisible)
{
	cursor.dwSize = 100;
	cursor.bVisible = boolVisible;
	::SetConsoleCursorInfo(hout, &cursor);
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	::GetWindowRect(hDesktop, &desktop);
	// The top left corner will have coordinates (0,0)
	// and the bottom right corner will have coordinates
	// (horizontal, vertical)
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

void MainMenu()
{
	::system("color f0");
	::SetCursor(0, 0);
	std::cout<<"__________             .___  .___             __\n\\______   \\_____     __| _/__| _/____   ____ |  | __  ______\n |     ___/\\__  \\   / __ |/ __ |/  _ \\_/ ___\\|  |/ / /  ___/\n |    |     / __ \\_/ /_/ / /_/ (  <_> )  \\___|    <  \\___ \\\n |____|    (____  /\\____ \\____ |\\____/ \\___  >__|_ \\/____  >\n                \\/      \\/    \\/           \\/     \\/     \\/\n";
	std::cout<<"\n\t\t";
	::FlushConsoleInputBuffer(hin);
	::system("cls");
	::SetCursor(0, 0);
	std::cout<<"__________             .___  .___             __\n\\______   \\_____     __| _/__| _/____   ____ |  | __  ______\n |     ___/\\__  \\   / __ |/ __ |/  _ \\_/ ___\\|  |/ / /  ___/\n |    |     / __ \\_/ /_/ / /_/ (  <_> )  \\___|    <  \\___ \\\n |____|    (____  /\\____ \\____ |\\____/ \\___  >__|_ \\/____  >\n                \\/      \\/    \\/           \\/     \\/     \\/\n";
	std::cout<<"\n\n\t\t\tStart game\n\t\t\tInstructions\n\t\t\tOptions\n\t\t\tQuit";
	::SetCursor(23, 8+menuItemSelected);
	std::cout<<(char)16;
	do
	{
		menuItemMove = _getch();
		if(menuItemMove == 0x48) // Up
		{
			// move up
			if(menuItemSelected==0)
			{
				menuItemSelected=3;
				::SetCursor(23, 5+menuItemSelected);
				std::cout<<" ";
			}
			else
			{
				menuItemSelected--;
				::SetCursor(23, 9+menuItemSelected);
				std::cout<<" ";
			}
			::SetCursor(23, 8+menuItemSelected);
			std::cout<<(char)16;
		}
		if(menuItemMove == 0x50) // Down
		{
			// move down
			if(menuItemSelected==3)
			{
				menuItemSelected=0;
				::SetCursor(23, 11+menuItemSelected);
				std::cout<<" ";
			}
			else
			{
				menuItemSelected++;
				::SetCursor(23, 7+menuItemSelected);
				std::cout<<" ";
			}
			::SetCursor(23, 8+menuItemSelected);
			std::cout<<(char)16;
		}
		if(gamePassed==true)
		{
			menuItemSelected=0;
			gamePassed=false;
		}
	}
	while(menuItemMove!=13);// until Enter is pressed
	switch(menuItemSelected)
	{
	case 0:
		return;
		break;
	case 1:
		menuItemSelected=0;
		::Instructions(); break;
	case 2:
		menuItemSelected=0;
		::Options(); break;
	case 3:
		win=4;
		return;
		break;
	}
}

void Instructions()
{
	std::system("cls");
	SetCursor(24, 0);
	std::cout<<"Instructions\n";
	std::cout<<"\n In Paddocks (a.k.a Dots and Boxes) you have to connect dots\n so as to form a square (a box). Each time you form a square\n you get 1 point and you are required to draw another line.\n The game ends when all dots are connected. The player\n with more points at the end of the game wins.\n\n "<<(char)16<<"Back";
	do
	{
		menuItemMove = _getch();
	}
	while(menuItemMove!=0x0D);
	menuItemSelected=1;
	::MainMenu();
}

void Options()
{
	hWnd = ::GetConsoleWindow();
	::GetDesktopResolution(horRes, verRes);
	std::system("cls");
	SetCursor(26, 0);
	std::cout<<"Options\n";
	std::cout<<"\n\t\t    Player 1 name: "<<p1<<"\n\t\t    Player 1 tag: "<<p1t<<"\n\t\t    Player 2 name: "<<p2<<"\n\t\t    Player 2 tag: "<<p2t<<"\n\t\t    Multiplayer: "<<multiplayer<<"\n\t\t    Board size: "<<sizeIn<<" boxes\n\t\t    Controls\n\t\t    Back";
	::SetCursor(0, 2+menuItemSelected);
	std::cout<<"\t\t   "<<(char)16;
	do
	{
		menuItemMove = _getch();
		if(menuItemMove == 0x48) // Up
		{
			// move up
			if(menuItemSelected==0)
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected=7;
			}
			else
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected--;
			}
			::SetCursor(0, 2+menuItemSelected);
			std::cout<<"\t\t   "<<(char)16;
		}
		if(menuItemMove == 0x50) // Down
		{
			// move down
			if(menuItemSelected==7)
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected=0;
			}
			else
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected++;
			}
			::SetCursor(0, 2+menuItemSelected);
			std::cout<<"\t\t   "<<(char)16;
		}
	}
	while(menuItemMove!=13);// until Enter is pressed
	switch(menuItemSelected)
	{
	case 0:
		::system("cls");
		::ChangeCursor(TRUE);
		std::cout<<"Player 1 name (max 25 symbols):[";
		::SetCursor(57, 0);
		std::cout<<"]";
		::SetCursor(32, 0);
		std::getline(std::cin, p1In);
		if((p1In.length()>0 && p1In.length()<26)==false)
		{
			::SetCursor(19, 6);
			std::cout<<"***INVALID INPUT***\n\t        ";
			::system("pause");
		}
		else
			p1=p1In;
		menuItemSelected=0;
		::ChangeCursor(FALSE);
		::Options();
		break;
	case 1:
		::system("cls");
		::ChangeCursor(TRUE);
		std::cout<<"Player 1 tag (max 2 symbols):[";
		::SetCursor(32, 0);
		std::cout<<"]";
		::SetCursor(30, 0);
		std::getline(std::cin, p1tIn);
		if((p1tIn.length()>0 && p1tIn.length()<3)==false)
		{
			::SetCursor(19, 6);
			std::cout<<"***INVALID INPUT***\n\t        ";
			::system("pause");
		}
		else
			p1t=p1tIn;
		menuItemSelected=1;
		::ChangeCursor(FALSE);
		::Options();
		break;
	case 2:
		::system("cls");
		::ChangeCursor(TRUE);
		std::cout<<"Player 2 name (max 25 symbols):[";
		::SetCursor(57, 0);
		std::cout<<"]";
		::SetCursor(32, 0);
		std::getline(std::cin, p2In);
		if((p2In.length()>0 && p2In.length()<26)==false)
		{
			::SetCursor(19, 6);
			std::cout<<"***INVALID INPUT***\n\t        ";
			::system("pause");
		}
		else
			p2=p2In;
		menuItemSelected=2;
		::ChangeCursor(FALSE);
		::Options();
		break;
	case 3:
		::system("cls");
		::ChangeCursor(TRUE);
		std::cout<<"Player 2 tag (max 2 symbols):[";
		::SetCursor(32, 0);
		std::cout<<"]";
		::SetCursor(30, 0);
		std::getline(std::cin, p2tIn);
		if((p2tIn.length()>0 && p2tIn.length()<3)==false)
		{
			::SetCursor(19, 6);
			std::cout<<"***INVALID INPUT***\n\t        ";
			::system("pause");
		}
		else
			p2t=p2tIn;
		menuItemSelected=3;
		::ChangeCursor(FALSE);
		::Options();
		break;
	case 4:
		if(multiplayer=="Off")
			multiplayer="On";
		else
			multiplayer="Off";
		menuItemSelected=4;
		::Options();
		break;
	case 5:
		::system("cls");
		::GetConsoleScreenBufferInfo(hout, &bufferInfo);
		new_size.X = 75;
		new_size.Y = 52;
		::SetConsoleScreenBufferSize(hout, new_size);
		
		::MoveWindow(hWnd,(horRes-620)/2,(verRes-700)/2,620,700,1); // Center
		::PrintBoardPreview(sizeIn, ' ');
		do
		{
			::SetCursor(0, 0);
			std::cout << "Choose board size [3-24]: " << arrowDown << sizeIn << arrowUp << " ";
			sizeInChange=_getch();
			if((sizeInChange==0x48) && (sizeIn<24))// ArrowKey Up Increase
			{
				++sizeIn;
				::PrintBoardPreview(sizeIn, '+');
			}
			if((sizeInChange==0x50) && (sizeIn>3))// ArrowKey Down Decrease
			{
				--sizeIn;
				::PrintBoardPreview(sizeIn, '-');
			}
		}
		while(sizeInChange!=0x0D);// until Enter is pressed

		new_size.X = 800;
		new_size.Y = 800;
		::SetConsoleScreenBufferSize(hout, new_size);
		::MoveWindow(hWnd,(horRes-530)/2,(verRes-240)/2,530,240,1);
		::RemoveScrollbar();

		menuItemSelected=5;
		changedSize=true;
		::Options();
		break;
	case 6:
		menuItemSelected=0;
		::Controls();
		break;
	case 7:
		menuItemSelected=2;
		::MainMenu();
		break;
	}
}

void Controls()
{
	menuItemMove=0;
	::system("cls");
	::SetCursor(26, 0);
	std::cout<<"Controls";
	::SetCursor(20, 2);
	std::cout<<"Move left: ";
	::GetKeyName(keyMoveLeft);
	std::cout<<"\n\t\t    Move right: ";
	::GetKeyName(keyMoveRight);
	std::cout<<"\n\t\t    Move up: ";
	::GetKeyName(keyMoveUp);
	std::cout<<"\n\t\t    Move down: ";
	::GetKeyName(keyMoveDown);
	std::cout<<"\n\t\t    Draw line: ";
	::GetKeyName(keyDraw);
	std::cout<<"\n\t\t    Rotate cursor: ";
	::GetKeyName(keySwitch);
	std::cout<<"\n\t\t    Return to menu: ";
	::GetKeyName(keyMenu);
	std::cout<<"\n\t\t    Quit: ";
	::GetKeyName(keyQuit);
	std::cout<<"\n\t\t    Restart: ";
	::GetKeyName(keyRestart);
	std::cout<<"\n\t\t    Default\n\t\t    Back";
	::SetCursor(19, menuItemSelected+2);
	std::cout<<(char)16;
	do
	{
		menuItemMove = _getch();
		if(menuItemMove == 0x48) // Up
		{
			// move up
			if(menuItemSelected==0)
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected=10;
			}
			else
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected--;
			}
			::SetCursor(0, 2+menuItemSelected);
			std::cout<<"\t\t   "<<(char)16;
		}
		if(menuItemMove == 0x50) // Down
		{
			// move down
			if(menuItemSelected==10)
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected=0;
			}
			else
			{
				::SetCursor(0, 2+menuItemSelected);
				std::cout<<"\t\t    ";
				menuItemSelected++;
			}
			::SetCursor(0, 2+menuItemSelected);
			std::cout<<"\t\t   "<<(char)16;
		}
	}
	while(menuItemMove!=13);// until Enter is pressed
	switch(menuItemSelected)
	{
	case 0:
		::SetCursor(33, 2);
		keyMoveLeft=::GetKeyName(0);
		::Controls();
		break;
	case 1:
		::SetCursor(34, 3);
		keyMoveRight=::GetKeyName(0);
		::Controls();
		break;
	case 2:
		::SetCursor(31, 4);
		keyMoveUp=::GetKeyName(0);
		::Controls();
		break;
	case 3:
		::SetCursor(33, 5);
		keyMoveDown=::GetKeyName(0);
		::Controls();
		break;
	case 4:
		::SetCursor(33, 6);
		keyDraw=::GetKeyName(0);
		::Controls();
		break;
	case 5:
		::SetCursor(37, 7);
		keySwitch=::GetKeyName(0);
		::Controls();
		break;
	case 6:
		::SetCursor(37, 8);
		keyMenu=::GetKeyName(0);
		::Controls();
		break;
	case 7:
		::SetCursor(37, 9);
		keyQuit=::GetKeyName(0);
		::Controls();
		break;
	case 8:
		::SetCursor(37, 10);
		keyRestart=::GetKeyName(0);
		::Controls();
		break;
	case 9:
		keyMoveUp=72;
		keyMoveRight=77;
		keyMoveLeft=75;
		keyMoveDown=80;
		keySwitch='z';
		keyDraw=32;
		keyMenu=27;
		keyQuit='q';
		keyRestart='r';
		::Controls();
		break;
	case 10:
		menuItemSelected=6;
		::Options();
		break;
	}
}

int GetKeyName(int keyInMenu)
{
	int keyPressed;
	if(keyInMenu==0)
	{
		switch (keyPressed = _getch())
		{
		case 0:
		case 224:
			switch (keyPressed = _getch())
			{
			case 72: std::cout << "Up Arrow        "; break;
			case 75: std::cout << "Left Arrow      "; break;
			case 77: std::cout << "Right Arrow     "; break;
			case 80: std::cout << "Down Arrow      "; break;
			case 82: std::cout << "Insert          "; break;
			case 71: std::cout << "Home            "; break;
			case 73: std::cout << "Page Up         "; break;
			case 83: std::cout << "Delete          "; break;
			case 79: std::cout << "End             "; break;
			case 81: std::cout << "Page Down       "; break;
			case 59: std::cout << "F1              "; break;
			case 60: std::cout << "F2              "; break;
			case 61: std::cout << "F3              "; break;
			case 62: std::cout << "F4              "; break;
			case 63: std::cout << "F5              "; break;
			case 64: std::cout << "F6              "; break;
			case 65: std::cout << "F7              "; break;
			case 66: std::cout << "F8              "; break;
			case 67: std::cout << "F9              "; break;
			case 68: std::cout << "F10             "; break;
			case 133: std::cout << "F11            "; break;
			case 134: std::cout << "F12            "; break;
			default: std::cout << "Extended Key " << keyPressed;
			}
			break;
		case 13: std::cout << "Enter           "; break;
		case 8: std::cout << "Backspace       "; break;
		case 27: std::cout << "Escape          "; break;
		case 9: std::cout << "Tab             "; break;
		case 32: std::cout << "Space           "; break;
		default: std::cout << (char)toupper(keyPressed)<<"               ";
		}
		return keyPressed;
	}
	else
	{
		switch (keyPressed = keyInMenu)
		{
		case 72: std::cout << "Up Arrow               "; break;
		case 75: std::cout << "Left Arrow               "; break;
		case 77: std::cout << "Right Arrow               "; break;
		case 80: std::cout << "Down Arrow               "; break;
		case 82: std::cout << "Insert               "; break;
		case 71: std::cout << "Home               "; break;
		case 73: std::cout << "Page Up               "; break;
		case 83: std::cout << "Delete               "; break;
		case 79: std::cout << "End               "; break;
		case 81: std::cout << "Page Down               "; break;
		case 59: std::cout << "F1               "; break;
		case 60: std::cout << "F2               "; break;
		case 61: std::cout << "F3               "; break;
		case 62: std::cout << "F4               "; break;
		case 63: std::cout << "F5               "; break;
		case 64: std::cout << "F6               "; break;
		case 65: std::cout << "F7               "; break;
		case 66: std::cout << "F8               "; break;
		case 67: std::cout << "F9               "; break;
		case 68: std::cout << "F10               "; break;
		case 133: std::cout << "F11               "; break;
		case 134: std::cout << "F12               "; break;
		case 13: std::cout << "Enter               "; break;
		case 8: std::cout << "Backspace               "; break;
		case 27: std::cout << "Escape               "; break;
		case 9: std::cout << "Tab               "; break;
		case 32: std::cout << "Space               "; break;
		default: std::cout << (char)toupper(keyPressed)<<"               ";
		}
		return keyPressed;
	}
}

void ComputerTurn()
{
	int fillBoxX=0, fillBoxY=0, compHor = 1, repeat=0, compScored=0;
	//do
	//{
loop:
	//compScored=0;
	::Sleep(800);
	for(; fillBoxX<=(size-2); fillBoxX++) // max size-2=23
	{
		fillBoxY=0;
		for(; fillBoxY<=(size-2); fillBoxY++) //max 23x24 and 24x23 _|
		{
			if(((filledh[0+fillBoxX][0+fillBoxY]==false) && (filledv[1+fillBoxX][0+fillBoxY]==true) && (filledh[0+fillBoxX][1+fillBoxY]==true) && (filledv[0+fillBoxX][0+fillBoxY]==true))==true)
			{
				filledh[0+fillBoxX][0+fillBoxY] = true;
				::SetCursor(fillBoxX*3+1, fillBoxY*2);
				std::cout << (char)205 << (char)205;
				::SetCursor(currX, currY);
				fillBoxX=0;
				fillBoxY=0;
				space = true;
				::CheckBoxes();
				goto loop;
			}
			if(((filledh[0+fillBoxX][0+fillBoxY]==true) && (filledv[1+fillBoxX][0+fillBoxY]==false) && (filledh[0+fillBoxX][1+fillBoxY]==true) && (filledv[0+fillBoxX][0+fillBoxY]==true))==true)
			{
				filledv[1+fillBoxX][0+fillBoxY] = true;
				::SetCursor((1+fillBoxX)*3, fillBoxY*2+1);
				std::cout << (char)186;
				::SetCursor(currX, currY);
				fillBoxX=0;
				fillBoxY=0;
				space = true;
				::CheckBoxes();
				goto loop;
			}
			if(((filledh[0+fillBoxX][0+fillBoxY]==true) && (filledv[1+fillBoxX][0+fillBoxY]==true) && (filledh[0+fillBoxX][1+fillBoxY]==false) && (filledv[0+fillBoxX][0+fillBoxY]==true))==true)
			{
				filledh[0+fillBoxX][1+fillBoxY] = true;
				::SetCursor(fillBoxX*3+1, (1+fillBoxY)*2);
				std::cout << (char)205 << (char)205;
				::SetCursor(currX, currY);
				fillBoxX=0;
				fillBoxY=0;
				space = true;
				::CheckBoxes();
				goto loop;
			}
			if(((filledh[0+fillBoxX][0+fillBoxY]==true) && (filledv[1+fillBoxX][0+fillBoxY]==true) && (filledh[0+fillBoxX][1+fillBoxY]==true) && (filledv[0+fillBoxX][0+fillBoxY]==false))==true)
			{
				filledv[0+fillBoxX][0+fillBoxY] = true;
				::SetCursor(fillBoxX*3, fillBoxY*2+1);
				std::cout << (char)186;
				::SetCursor(currX, currY);
				fillBoxX=0;
				fillBoxY=0;
				space = true;
				::CheckBoxes();
				goto loop;
			}
		}
	}
	do
	{
		repeat=0;
		compHor = ::rand()%2;
		if(compHor==1)
		{
			fillBoxX = ::rand()%(size-1);
			fillBoxY = ::rand()%(size);
			if(filledh[fillBoxX][fillBoxY] == false)
			{
				filledh[fillBoxX][fillBoxY] = true;
				::SetCursor(fillBoxX*3+1, fillBoxY*2);
				std::cout << (char)205 << (char)205;
				space = true;
				repeat=0;
				break;
			}
			else
				repeat=1;
		}
		else
		{
			fillBoxX = rand()%(size);
			fillBoxY = rand()%(size-1);
			if(filledv[fillBoxX][fillBoxY] == false)
			{
				filledv[fillBoxX][fillBoxY] = true;
				::SetCursor(fillBoxX*3, fillBoxY*2+1);
				std::cout << (char)186;
				space = true;
				repeat=0;
				break;
			}
			else
				repeat=1;
		}
		if(::CheckLines(size-1)==2*size*(size-1))
			repeat=0;
	}
	while(repeat==1);
	return;
}

/*
void HorizontalLine(HDC hDC, int startX, int startY)
{
for(int linePxlX=5; linePxlX<25; linePxlX++)
{
for(int linePxlY=2; linePxlY<8; linePxlY++)
{
SetPixel(hDC, startX+linePxlX, startY+linePxlY, (0,0,0));
}
}
}

void VerticalLine(HDC hDC, int startX, int startY)
{
for(int linePxlX=1; linePxlX<7; linePxlX++)
{
for(int linePxlY=8; linePxlY<27; linePxlY++)
{
SetPixel(hDC, startX+linePxlX, startY+linePxlY, (0,0,0));
}
}
}

void FillSquare(HDC hDC, int squareX, int squareY)
{
for(int linePxlX=6; linePxlX<25; linePxlX++)
{
for(int linePxlY=8; linePxlY<27; linePxlY++)
{
if((linePxlY<12 || linePxlY>22) || ((linePxlY>11 || linePxlY<23) && (linePxlX<8 || linePxlX>20)))
SetPixel(hDC, squareX*3*8+linePxlX, squareY*3*8+linePxlY, (0,0,0));
}
}
}

void DrawFilledLinesBoxes(HDC hWndDC)
{
for(int drawX=0;drawX<25;drawX++)
{
for(int drawY=0;drawY<25;drawY++)
{
if(filledh[drawX][drawY]!=0)
HorizontalLine(hWndDC, drawX*3*8, drawY*3*8);
if(filledv[drawX][drawY]!=0)
VerticalLine(hWndDC, drawX*3*8, drawY*3*8);
if(filledBox[drawX][drawY]!=0 && drawX<24 && drawY<24)
{
::FillSquare(hWndDC, drawX, drawY);
}
}
}
}
*/
int CheckWin(HWND hWnd)
{
	win=0;
	if(checkWin>0)
	{
		if(score1>score2)
			win=1;
		else if(score1<score2)
			win=2;
		else
			win=4;
	}
	if(win==1)
	{
		sStrWinner.str(std::string());
		sStrWinner << p1 << " (" << p1t << ") wins!           ";
		std::string strWinner = sStrWinner.str();
		::MessageBox(hWnd, (LPCSTR)strWinner.c_str(), "Game Over", MB_OK | MB_SETFOREGROUND);
		return 1;
	}
	else if(win==2)
	{
		sStrWinner.str(std::string());
		sStrWinner << p2 << " (" << p2t << ") wins!           ";
		std::string strWinner = sStrWinner.str();
		::MessageBox(hWnd, (LPCSTR)strWinner.c_str(), "Game Over", MB_OK | MB_SETFOREGROUND);
		return 2;
	}
	else if(win==4)
	{
		sStrWinner.str(std::string());
		sStrWinner << "Tie!                    ";
		std::string strWinner = sStrWinner.str();
		::MessageBox(hWnd, (LPCSTR)strWinner.c_str(), "Game Over", MB_OK | MB_SETFOREGROUND);
		return 4;
	}
	return 0;
}

bool RemoveScrollbar()
{
	if(!GetConsoleScreenBufferInfo(hout, &bufferInfo)) return false;
	COORD new_size = 
	{
		bufferInfo.srWindow.Right - bufferInfo.srWindow.Left + 1,
		bufferInfo.srWindow.Bottom - bufferInfo.srWindow.Top + 1
	};
	if(!SetConsoleScreenBufferSize(hout, new_size))
		return false;
	return true;
}

/*
add key change [+default]
mouse horizontal/vertical
pixelize
add better computer logic
*/

//#include <windows.h>
//
//#include <cmath>
//
//#define ROUND(a) ((int) (a + 0.5))
//
///* set window handle */
//
//static HWND sHwnd;
//
//static COLORREF redColor=RGB(255,0,0);
//
//static COLORREF blueColor=RGB(0,0,255);
//
//static COLORREF greenColor=RGB(0,255,0);
//
//
//void SetWindowHandle(HWND hwnd){
//
//sHwnd=hwnd;
//
//}
//
///* SetPixel */
//
//void setPixel(int x,int y,COLORREF& color=redColor){
//
//if(sHwnd==NULL){
//
//    MessageBox(NULL,"sHwnd was not initialized !","Error",MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
//
//    exit(0);
//
//}
//
//HDC hdc=GetDC(sHwnd);
//
//SetPixel(hdc,x,y,color);
//
//ReleaseDC(sHwnd,hdc);
//
//return;
//
//// NEVERREACH //
//
//}
//
//
//void drawLineDDA(int xa, int ya, int xb, int yb){
//
//   int dx = xb - xa, dy = yb - ya, steps, k;
//
//   float xIncrement, yIncrement, x = xa, y = ya;
//
//   if(abs(dx) > abs(dy)) steps = abs(dx);
//
//   else steps = abs(dy);
//
//   xIncrement = dx / (float) steps;
//
//   yIncrement = dy / (float) steps;
//
//   setPixel(ROUND(x), ROUND(y));
//
//   for(int k = 0; k < steps; k++){
//
//    x += xIncrement;
//
//    y += yIncrement;
//
//    setPixel(x, y);
//
// }
//
//}
//
///* Window Procedure WndProc */
//
//LRESULT CALLBACK WndProc(HWND hwnd,UINT message,WPARAM wParam,LPARAM lParam){
//
// switch(message){
//
//    case WM_PAINT:
//
//        SetWindowHandle(hwnd);
//
//        drawLineDDA(10, 20, 250, 300);
//
//        break;
//
//    case WM_CLOSE: // FAIL THROUGH to call DefWindowProc
//
//        break;
//
//    case WM_DESTROY:
//
//        PostQuitMessage(0);
//
//        return 0;
//
//    default:
//
//    break; // FAIL to call DefWindowProc //
//
//  }
//
// return DefWindowProc(hwnd,message,wParam,lParam);
//
//}
//
//int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int      iCmdShow){
//
//static TCHAR szAppName[] = TEXT("Straight Line");
//
//WNDCLASS wndclass;
//
//wndclass.style         = CS_HREDRAW|CS_VREDRAW ;
//
//wndclass.lpfnWndProc   = WndProc ;
//
//wndclass.cbClsExtra    = 0 ;
//
//wndclass.cbWndExtra    = 0 ;
//
//wndclass.hInstance     = hInstance ;
//
//wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
//
//wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
//
//wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
//
//wndclass.lpszMenuName  = NULL ;
//
//wndclass.lpszClassName = szAppName ;
//
//// Register the window //
//
//if(!RegisterClass(&wndclass)){
//
//    MessageBox(NULL,"Registering the class failed","Error",MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
//
//    exit(0);
//
//}
//
//// CreateWindow //
//
//HWND hwnd=CreateWindow(szAppName,"DDA - Programming Techniques",
//
//            WS_OVERLAPPEDWINDOW,
//
//             CW_USEDEFAULT,
//
//             CW_USEDEFAULT,
//
//             CW_USEDEFAULT,
//
//             CW_USEDEFAULT,
//
//             NULL,
//
//             NULL,
//
//             hInstance,
//
//             NULL);
//
//if(!hwnd){
//
//    MessageBox(NULL,"Window Creation Failed!","Error",MB_OK | MB_SETFOREGROUND);
//
//    exit(0);
//
//  }
//
//  // ShowWindow and UpdateWindow //
//
//  ShowWindow(hwnd,iCmdShow);
//
// UpdateWindow(hwnd);
//
// // Message Loop //
//
// MSG msg;
//
// while(GetMessage(&msg,NULL,0,0)){
//
//    TranslateMessage(&msg);
//
//    DispatchMessage(&msg);
//
// }
//
//  /* return no error to the operating system */
//
//  return 0;
//
//}