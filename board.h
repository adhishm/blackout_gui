/**
 *@file board.h
 *@author Adhish Majumdar
 *@version 0.0.0
 *@date 14/03/2013
 *@brief File with definition of the class Board.
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

#ifndef BOARD_H
#define BOARD_H

#include "tools.h"

#ifndef DEFAULT_BOARDSIZE
#define DEFAULT_BOARDSIZE 3
#endif

/**
 * The Board class to represent the playing board.
 */
class Board
{
 private:
  /**
   * Representing the two dimensional square board with cells of values 0 or 1.
   */
  int **cell;
  /**
   * Stores the initial state of the cell in case a reset is required.
   */
  int **initialState;
  /**
   * The length of the side of the square board.
   */
  int boardSize;

 public:
  /**
   * Constructor for the board. Creates the board with the side given in sideLength.
   * @param sideLength The side of the square. The default value is 3.
   */
  Board (int sideLength=DEFAULT_BOARDSIZE);
  /**
   * Sets the value in the cell pointed to by x and y to value. If the co-ordinates are valid, the return is true, else false.
   * @param x Row number of the cell.
   * @param y Column number of the cell.
   * @param value The value the cell is supposed to be set to.
   */
  bool setCellValue (int x, int y, int value);
  /**
   * Returns in the pointer value the value of the cell at (x, y). If the co-ordinates are valid, the return is true, else false.
   * @param x Row number of the cell.
   * @param y Column number of the cell.
   * @param value Pointer to the location in memory where the value of the cell is copied.
   */
  bool getCellValue (int x, int y, int *value);
  /**
   * Sets the value in the initialState cell pointed to by x and y to value. If the co-ordinates are valid, the return is true, else false.
   * @param x Row number of the cell.
   * @param y Column number of the cell.
   * @param value The value the cell is supposed to be set to.
   */
  bool setInitialCellValue (int x, int y, int value);
  /**
   * Returns in the pointer value the value of the initialState cell at (x, y). If the co-ordinates are valid, the return is true, else false.
   * @param x Row number of the cell.
   * @param y Column number of the cell.
   * @param value Pointer to the location in memory where the value of the cell is copied.
   */
  bool getInitialCellValue (int x, int y, int *value);
  /**
   * Resets the board to it's initial value.
   */
  void resetBoard();
  /**
   * Sets the intial state of the board and saves it for future reference. It essentially saves the current state snapshot into the matrix intialState.
   */
  void setInitialState ();
  /**
   * Checks the sanity of the coordinates entered. If 0 or negative values, as well as those exceeding the board dimensions are entered, the function returns false, else the return value is true.
   * @param x Row number of the cell.
   * @param y Column number of the cell.
   */
  bool checkCoordinateSanity (int x, int y);
  /**
   * Returns the sum of the elements on the board.
   */
  int sum ();
};

#endif
