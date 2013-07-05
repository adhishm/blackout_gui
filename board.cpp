/**
 *@file board.cpp
 *@author Adhish Majumdar
 *@version 0.0.0
 *@date 14/03/2013
 *@brief File with definition of member functions of the class Board.
 *@details The Board class represent the playing board for the game.
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

#include "board.h"

/**
 * Constructor for the board. Creates the board with the side given in sideLength.
 * @param sideLength The side of the square. The default value is 3.
 */
Board::Board (int sideLength)
{
  int i;

  this->boardSize = sideLength;

  this->cell = new int* [sideLength];
  this->initialState = new int* [sideLength];

  for (i=0; i<sideLength; i++)
    {
      this->cell[i] = new int[sideLength];
      this->initialState[i] = new int[sideLength];
    }
}

/**
 * Sets the value in the cell pointed to by x and y to value. If the co-ordinates are valid, the return is true, else false.
 * @param x Row number of the cell.
 * @param y Column number of the cell.
 * @param value The value the cell is supposed to be set to.
 */
bool Board::setCellValue (int x, int y, int value)
{
  if (this->checkCoordinateSanity(x, y))
    {
      this->cell[x-1][y-1] = value;
      return (true);
    }
  else
    {
      return (false);
    }
}

/**
 * Returns in the pointer value the value of the cell at (x, y). If the co-ordinates are valid, the return is true, else false.
 * @param x Row number of the cell.
 * @param y Column number of the cell.
 * @param value Pointer to the location in memory where the value of the cell is copied.
 */
bool Board::getCellValue (int x, int y, int *value)
{
  if (this->checkCoordinateSanity(x, y))
    {
      *value = this->cell[x-1][y-1];
      return (true);
    }
  else
    {
      return (false);
    }
}

/**
 * Sets the value in the initialState cell pointed to by x and y to value. If the co-ordinates are valid, the return is true, else false.
 * @param x Row number of the cell.
 * @param y Column number of the cell.
 * @param value The value the cell is supposed to be set to.
   */
bool Board::setInitialCellValue (int x, int y, int value)
{
  if (this->checkCoordinateSanity(x, y))
    {
      this->initialState[x-1][y-1] = value;
      return (true);
    }
  else
    {
      return (false);
    }
}

/**
 * Returns in the pointer value the value of the initialState cell at (x, y). If the co-ordinates are valid, the return is true, else false.
 * @param x Row number of the cell.
 * @param y Column number of the cell.
 * @param value Pointer to the location in memory where the value of the cell is copied.
 */
bool Board::getInitialCellValue (int x, int y, int *value)
{
  if (this->checkCoordinateSanity(x, y))
    {
      *value = this->initialState[x-1][y-1];
      return (true);
    }
  else
    {
      return (false);
    }
}

/**
 * Checks the sanity of the coordinates entered. If 0 or negative values, as well as those exceeding the board dimensions are entered, the function returns false, else the return value is true.
 * @param x Row number of the cell.
 * @param y Column number of the cell.
 */
bool Board::checkCoordinateSanity (int x, int y)
{
  if (x <= 0)
    {
      return (false);
    }

  if (y <= 0)
    {
      return (false);
    }

  if (x > this->boardSize)
    {
      return (false);
    }

  if (y > this->boardSize)
    {
      return (false);
    }

  // We are still here, so the co-ordinates are alright.
  return (true);
}

/**
 * Sets the intial state of the board and saves it for future reference. It essentially saves the current state snapshot into the matrix intialState.
 */
void Board::setInitialState ()
{
  int i, j;

  for (i=0; i<this->boardSize; i++)
    {
      for (j=0; j<this->boardSize; j++)
	{
	  this->initialState[i][j] = this->cell[i][j];
	}
    }
}

/**
 * Resets the board to it's initial value.
 */
void Board::resetBoard()
{
  int i, j;

  for (i=0; i<this->boardSize; i++)
    {
      for (j=0; j<this->boardSize; j++)
	{
	  this->cell[i][j] = this->initialState[i][j];
	}
    }
}

/**
 * Returns the sum of the elements on the board.
 */
int Board::sum ()
{
  int s=0;
  int i, j;

  for (i=0; i<boardSize; i++)
    {
      for (j=0; j<boardSize; j++)
	{
	  s += this->cell[i][j];
	}
    }

  return (s);
}
