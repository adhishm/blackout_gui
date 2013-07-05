/**
 *@file blackout.cpp
 *@author Adhish Majumdar
 *@version 0.0.0
 *@date 14/03/2013
 *@brief File with definition of member functions of the class Blackout.
 *@details The Blackout class handles data and events required for the game.
 */

/*
    Blackout
    Classes and functions to play the game of blackout.
    Copyright (C) 2013  Adhish Majumdar

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "blackout.h"

/**
 * Constructor that initializes the game.
 */
Blackout::Blackout (int boardSize)
{
  // Initialize the board
  this->b = new Board(boardSize);
  this->nPoints = boardSize;

  // Fill the board with 0's
  int i, j;
  for (i=1; i<=boardSize; i++)
    {
      for (j=1; j<=boardSize; j++)
	{
	  this->b->setCellValue (i, j, 0);
	}
    }
}

/**
 * Carries out the move at the position given by x and y.
 * @param x Row number of the point where the move is carried out.
 * @param y Column number of the point where the move is carried out.
 */
bool Blackout::applyMove (int x, int y)
{
  if (x<=0 || x>this->nPoints || y <=0 || y>this->nPoints)
  {
    return (false);
  }
  
  this->flipCell (x, y);

  // Flip above
  if (x!=1)
    {
      this->flipCell (x-1, y);
    }
  // Flip below
  if (x!=this->nPoints)
    {
      this->flipCell (x+1, y);
    }
  // Flip left
  if (y!=1)
    {
      this->flipCell (x, y-1);
    }
  // Flip right
  if (y!=this->nPoints)
    {
      this->flipCell (x, y+1);
    }

  (this->nMoves)++;
  return (true);
}

/**
 * Checks for the win condition - that is if the board is composed of only one kind of item, all 0's or all 1's.
 */
bool Blackout::checkWinCondition ()
{
  int boardSum = this->b->sum ();
  // For the win condition, all cells must be 0 or 1
  this->win = (boardSum==0) || (boardSum==(this->nPoints * this->nPoints));
  return ( this->win );
}

/**
 * This function generates a new game. THis is done by basically starting from a full square, and performing the number of moves specified by the nInitializationLoops variable, and taking the final state as the beginning of the game. This ensures that we get a 'solvable' puzzle.
 * @param nInitializationLoops Number of moves used initially carried out to jumble up the puzzle. Default value: DEFAULT_INITIALIZATION_LOOPS.
 */
bool Blackout::generateGame (int nInitializationLoops, int count)
{
  int x, y, i;

  srand ( time (NULL) );  // Set the seed for the random number generation

  for (i=0; i<nInitializationLoops; i++)
    {
      // Get random position on the board
      x = getRandomInteger (1, this->nPoints);
      y = getRandomInteger (1, this->nPoints);

      if (!this->applyMove (x, y))
	{
	  // Trouble applying move. Abort.
	  return (false);
	}
    }

  // Check for not getting a winning condition right at the beginning.
  // Because that would be just plain absurd, won't it?
  if (this->checkWinCondition())
    {
      // This is not working. Game generation failed
      if (count==GAME_RECURSION_LIMIT)
	{
	  return (false);
	}
      // Let's try generating with a different number of initialization loops
      this->generateGame(nInitializationLoops * rand(), ++count);
    }

  // If we reach here, then the game generation was successful
  this->nMoves = 0;
  this->win = false;
  this->b->setInitialState ();
  return (true);
}

/**
 * Saves the current state of the game to file.
 * @param fileName Name of the file to which the game is to be saved.
 */
bool Blackout::saveGame (const char* fileName)
{
  // Open the file
  std::ofstream outFile;
  outFile.open (fileName);

  if (outFile.is_open())
    {
      int i, j, cellValue;

      outFile << "Board size: " << this->nPoints << std::endl;
      outFile << "Moves: " << this->nMoves << std::endl;

      outFile << "Board state:";
      for (i=0; i<this->nPoints; i++)
	{
	  outFile << "\n";
	  for (j=0; j<this->nPoints; j++)
	    {
	      this->b->getCellValue (i+1, j+1, &cellValue);
	      outFile << cellValue << " ";
	    }
	}

      outFile << "\nInitial state:";
      for (i=0; i<this->nPoints; i++)
	{
	  outFile << "\n";
	  for (j=0; j<this->nPoints; j++)
	    {
	      this->b->getInitialCellValue (i+1, j+1, &cellValue);
	      outFile << cellValue << " ";
	    }
	}
      outFile << std::endl;
      outFile.close ();
      return (true);
    }
  else
    {
      return (false);
    }
}

/**
 * Loads the game from a file.
 * @param fileName File of the file from which to load the game.
 */
bool Blackout::loadGame (const char* fileName)
{
  // Open the file
  std::ifstream inFile;
  inFile.open (fileName);

  if (inFile.is_open())
    {
      // Before the file data is read directly into the class object, it must be verified.
      // The following variables store temporarily until all information in the file is read.
      int boardSize, numMoves;
      int **boardData;
      int i, j;

      std::string t1, t2;    // To store the titles
      std::string line;      // To read entire lines
      // char newline;
      inFile >> t1 >> t2 >> boardSize;   // Board size:
      inFile >> t1 >> numMoves;          // Moves:

      // Read current board state
      std::getline (inFile, line);       // Newline
      std::getline (inFile, line);       // Board state:

      // Allocate memory to boardData
      boardData = new int* [boardSize];
      if (!boardData)
	{
	  return (false);
	}
      else
	{
	  for (i=0; i<boardSize; i++)
	    {
	      boardData[i] = new int [boardSize];
	      if (!boardData[i])
		{
		  // Clear previous data
		  for (j=0; i<i; j++)
		    {
		      delete (boardData[j]);
		      boardData[j] = NULL;
		    }
		  delete (boardData);
		  boardData = NULL;

		  inFile.close ();
		  return (false);
		}
	      else
		{
		  // Data allocation done - populate the array
		  for (j=0; j<boardSize; j++)
		    {
		      inFile >> boardData[i][j];
		    }
		  // inFile >> newline;
		}
	    }
	  // All data is read - it can be transferred now
	  for (i=0; i<boardSize; i++)
	    {
	      for (j=0; j<boardSize; j++)
		{
		  this->b->setCellValue(i+1, j+1, boardData[i][j]);
		}
	    }
	  // Free data from boardData
	  for (i=0; i<boardSize; i++)
	    {
	      delete (boardData[i]);
	      boardData[i] = NULL;
	    }
	  delete (boardData);
	  boardData = NULL;
	}

      // Read initial board state
      std::getline (inFile, line);      // Newline
      std::getline (inFile, line);      // Initial state:

      // Allocate memory to boardData
      boardData = new int* [boardSize];
      if (!boardData)
	{
	  return (false);
	}
      else
	{
	  for (i=0; i<boardSize; i++)
	    {
	      boardData[i] = new int [boardSize];
	      if (!boardData[i])
		{
		  // Clear previous data
		  for (j=0; i<i; j++)
		    {
		      delete (boardData[j]);
		      boardData[j] = NULL;
		    }
		  delete (boardData);
		  boardData = NULL;

		  inFile.close ();
		  return (false);
		}
	      else
		{
		  // Data allocation done - populate the array
		  for (j=0; j<boardSize; j++)
		    {
		      inFile >> boardData[i][j];
		    }
		  // inFile >> newline;
		}
	    }
	  // All data is read - it can be transferred now
	  for (i=0; i<boardSize; i++)
	    {
	      for (j=0; j<boardSize; j++)
		{
		  this->b->setInitialCellValue(i+1, j+1, boardData[i][j]);
		}
	    }
	  // Free data from boardData
	  for (i=0; i<boardSize; i++)
	    {
	      delete (boardData[i]);
	      boardData[i] = NULL;
	    }
	  delete (boardData);
	  boardData = NULL;
	}

      // All data is read - close the file and return
      inFile.close ();      
      return (true);
    }
  else
    {
      return (false);
    }
}

/**
 * Resets the game to its initial state.
 */
void Blackout::reset ()
{
  this->b->resetBoard ();
  this->nMoves = 0;
}

/**
 * Flips the value of the cell at position x, y.
 * @param x Row number of the point where the move is carried out.
 * @param y Column number of the point where the move is carried out.
 */
void Blackout::flipCell (int x, int y)
{
  int cellValue;
  this->b->getCellValue (x, y, &cellValue);

  if (cellValue==0)
    {
      this->b->setCellValue (x, y, 1);
    }
  else
    {
      this->b->setCellValue (x, y, 0);
    }
}

/**
 * Displays the game board.
 */
bool Blackout::show ()
{
  int i, j;
  int value;
  
  for (i=1; i<=this->nPoints; i++)
  {
    std::cout << std::endl;
    for (j=1; j<=this->nPoints; j++)
    {
      if (this->b->getCellValue (i, j, &value))
      {
	std::cout << value << " ";
      }
      else
      {
	std::cout << "\nError reading game state.";
	return (false);
      }
    }
  }
  
  return (true);
}

/**
 * Returns the number of moves carried out.
 */
int Blackout::getMoves ()
{
  return (this->nMoves);
}

/**
 * Returns the win state.
 */
bool Blackout::getWin()
{
  return (this->win);
}
