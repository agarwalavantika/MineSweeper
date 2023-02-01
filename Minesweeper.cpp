#include<iostream>
#include<cstring>
#include<cstdlib>
#include<ctime>
using namespace std;
int row, column, mines, revealed, margin, temp_mines;

void gameOver(char *realBoard, char *board)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < margin; j++)
		{
			cout << " ";
		}
		for (int j = 0; j < column; j++)
		{
			if (*(realBoard + i * column + j) == '*')
			cout << *(realBoard + i * column + j) << " ";
			else
			cout << *(board + i * column + j) << " ";
		}
		cout << endl;
	}
}

void showRules()
{
	cout << "----------------------------------------------------------------------------------------------------------------------\n";
	cout << "\t\t\t\t\tWELCOME TO THE MINESWEEPER!!!!\n";
	cout << "-> First select the difficulty level of the Puzzle.\n";
	cout << "-> For each move you have to select the move type either you want to mark the cell as suspicious or reveal the cell.\n";
	cout << "-> To indicate which cell you want to reveal or mark you have to give input as row number starting from 0 separated by \n   a space a column number starting from 0.\n";
	cout << "-> Below the board, it shows the number of mines remaining according to the player which increases or decreases when a \n   cell is marked or unmarked.\n";
	cout << "----------------------------------------------------------------------------------------------------------------------\n\n\n\n";
}

void showBoard(char *board)
{	
	cout << endl;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < margin; j++)
		{
			cout << " ";
		}
		for (int j = 0; j < column; j++)
		{
			cout << *(board + i * column + j) << " ";
		}
		cout << endl;
	}
}

inline void mark(char *board, int r, int c)
{
	*(board + r * column + c) = '?';
}
inline void unmark(char *board, int r, int c)
{
	*(board + r * column + c) = '_';
}

void re_arrange(char *realBoard, int i, int j)
{
	srand (time (0));
	*(realBoard + i * column + j) = '_';
	int r = rand() % row;
	while (r == i)
	{
		r = rand() % row;
	}
	int c = rand() % column;
	while (c == j)
	{
		c = rand() % column;
	}
	*(realBoard + r * column + c) = '*';
}

int countMines(char *realBoard, int r, int c)
{
	if (*(realBoard + r * column + c) == '*') return -1;
	
	int count = 0, start_column = c-1, end_column = c+1, start_row = r-1, end_row = r+1;
	if (r == 0)
	{
		start_row = 0;
	}
	else if (r == row - 1)
	{
		end_row = row - 1;
	}
	if (c == 0)
	{
		start_column = 0;
	}
	else if (c == column - 1)
	{
		end_column = column - 1;
	}
	for (int i = start_row; i <= end_row; i++)
	{
		for (int j = start_column; j <= end_column; j++)
		{
			if (*(realBoard + i * column + j) == '*') count++;
		}
	}
	return count;
}

void reveal(char *realBoard, char **board, int c, int r)
{
	if (c < 0 || c >= column || r < 0 || r >= row) return;
	if (*(*board + (r * column) + c) != '_') return;
	
	int val = countMines (realBoard, r, c);
	if (val != -1) *(*board + (r * column) + c) = char(48+val);
	revealed++;
	if (val != 0) return;
	
	reveal (realBoard, board, c, r-1);
	reveal (realBoard, board, c, r+1);
	reveal (realBoard, board, c-1, r);
	reveal (realBoard, board, c+1, r);
}

void PrepareBoard(char *realBoard, char *board)
{
	temp_mines = 0;
	revealed = 0;
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			*(board + i * column + j) = '_';
			*(realBoard + i * column + j) = '_';
		}
	}
	srand (time (0));
	int minescount = 0;
	while (minescount < mines)
	{
		int r = rand() % row;
		int c = rand() % column;
		while (*(realBoard + r * column + c) == '*')
		{
			r = rand() % row;
			c = rand() % column;
		}
		*(realBoard + r * column + c) = '*';
		minescount++;
	}
}

int Play(char *realBoard, char *board)
{
	int result = 0, moves = 0, i, j, type, mines_remaining = 0;
	showBoard(board);
	for (int j = 0; j < (margin + 2 * column / 3); j++)
	{
		cout << " ";
	}
	mines_remaining = mines - temp_mines;
	cout << "Mines : " << mines_remaining << endl;
	
	while (result == 0)
	{
		cout << "Enter move type : \n1. Reveal the cell\n2. Mark the cell\n3. Unmark the cell\n4. To Start a New Game\n0. To Exit the Game\n";
		cin >> type;
		
		if (type == 0)
		exit(0);
		
		if (type == 4)
		{
			char same;
			cout << "Enter y to play new game on same board or n for a new board : ";
			cin >> same;
			cout << endl << endl << endl;
			
			if (same == 'n')
				return result;
			else
				return 2;
		}
		
		cout << "Enter Move row,col : ";
		cin >> i >> j;
		
		if (type == 1 && *(board + i * column + j) != '?')
		{
			if (*(realBoard + i * column + j) == '*' && moves > 0)
			{
				*(board + i * column + j) = '*';
				gameOver(realBoard, board);
				result = -1;
				break;
			}
			
			if (*(realBoard + i * column + j) == '*' && moves == 0)
			{
				re_arrange (realBoard, i, j);
			}
			
			reveal (realBoard, &(board), j, i);
			int assumed_mines = (row * column) - revealed;
			if (mines == assumed_mines)
			{
				result = 1;
				break;
			}
			moves++;
		}
		
		else if (type == 2)
		{
			temp_mines++;
			mark (board, i, j);
		}
		
		else if (type == 3 && *(board + i * column + j) == '?')
		{
			temp_mines--;
			unmark (board, i, j);
		}
		
		
		showBoard(board);
		for (int j = 0; j < (margin + 2 * column / 3); j++)
		{
			cout << " ";
		}
		mines_remaining = mines - temp_mines;
		cout << "Mines : " << mines_remaining << endl;
	}
	
	return result;
}

void prepareOnlyLiveBoard(char *board)
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < column; j++)
		{
			*(board + i * column + j) = '_';
		}
	}
}

int main()
{
	char again = 'y';
	showRules();
	while (again == 'y')
	{
		int difficulty;
		
		cout << "Select Difficulty : \n1. Easy(10 mines and 9x9 matrix)\n2. Medium(40 mines and 16x16 matrix)\n3. Hard(99 mines and 16x30 matrix)\n";
		cin >> difficulty;
		
		if (difficulty == 1)
		{
			row = 9;
			column = 9;
			mines = 10;
			margin = 45;
		}
		else if (difficulty == 2)
		{
			row = 16;
			column = 16;
			mines = 40;
			margin = 40;
		}
		else if (difficulty == 3)
		{
			column = 30;
			row = 16;
			mines = 99;
			margin = 30;
		}
		else
		exit(0);
		
		char realBoard[row][column], board[row][column];
		PrepareBoard(*realBoard, *board);
		int result = Play(*realBoard, *board);
		while (result == 2)
		{
			prepareOnlyLiveBoard(*board);
			result = Play(*realBoard, *board);
		}
		
		if (result == -1)
		{
			cout << "YOU LOSE.......\nGAME OVER\n";
		}
		else if (result == 1)
		{
			cout << "CONGRATULATIONS.......\nYOU WON!!!!\n";
		}
		else if (result == 0)
		{
			again = 'y';
			continue;
		}
		
		
		cout << "Enter y to play again or n to exit : ";
		cin >> again;
		cout << endl << endl << endl;
	}
}
