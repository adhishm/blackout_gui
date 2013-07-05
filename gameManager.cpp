/**
 *@file gameManager.cpp
 *@author Adhish Majumdar
 *@version 0.0.0
 *@date 21/03/2013
 *@brief Function definitions for the game manager
 *@details This file contains function definitions for the game manager. The game manager manages the gameplay by calling appropriate functions depending on the interaction with the player.
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

#include "gameManager.h"

/**
 * Function to start the game. This is called by the main() function.
 */
void play ()
{
  Blackout *bl;  // Stores all game information
  int choice;    // Stores the choice of the user from the menu
  bool menuContinue = true;
  int boardSize;

  while (menuContinue)
    {
      // Get choice
      choice = showMenu ();
      
      switch (choice)
	{
	case 0:
	  // Exit the game
	  if (!bl)
	    {
	      // Clear memory
	      delete (bl);
	      bl = NULL;
	      return;
	    }
	  menuContinue = false;
	  break;

	case 1:
	  // New game with default
	  boardSize = DEFAULT_GAMESQUARESIZE;
	  bl = new Blackout (boardSize);
	  if (!bl->generateGame())
	    {
	      std::cout << "\nFailed to generate game\n";
	      delete (bl);
	      bl = NULL;
	    }
	  else
	    {
	      game (bl);
	    }
	  menuContinue = true;
	  break;

	case 2:
	  // New game with custom board size
	  std::cout << "Board size (" << DEFAULT_GAMESQUARESIZE << "-" << MAX_GAMESQUARESIZE << ")? ";
	  std::cin >> boardSize;

	  bl = new Blackout (boardSize);
	  if (!bl->generateGame())
	    {
	      std::cout << "\nFailed to generate game\n";
	      delete (bl);
	      bl = NULL;
	    }
	  else
	    {
	      game (bl);
	    }
	  menuContinue = true;
	  break;

	case 3:
	  // Load previous game
	  bl = new Blackout ();
	  if (loadGameData (bl))
	    {
	      game (bl);
	    }
	  else
	    {
	      std::cout << "Error loading file.\n";
	      // Clear memory
	      delete (bl);
	      bl = NULL;
	    }
	  menuContinue = true;
	  break;

	default:
	  // Exit the game
	  if (!bl)
	    {
	      // Clear memory
	      delete (bl);
	      bl = NULL;
	      return;
	    }
	  menuContinue = false;
	  break;
	}
    }
}

/**
 * Shows the game menu and returns the option chosen by the user.
 */
int showMenu ()
{
  std::cout << std::endl;
  std::cout << "Blackout\n========\n";
  std::cout << "Main menu:\n";
  std::cout << "1. New game (default " << DEFAULT_GAMESQUARESIZE << "x" << DEFAULT_GAMESQUARESIZE << "board)\n";
  std::cout << "2. New game (custom board size)\n";
  std::cout << "3. Load saved game\n";
  std::cout << "Enter choice (anything other than 1, 2 or 3 to exit): ";

  int ch;

  std::cin >> ch;
  return (ch);
}

/**
 * Loads the game data into the memory pointed to by Blackout *bl. The return value is true of the load is successful, else it is false.
 * @param bl Pointer to the object of the Blackout class into which game data is to be loaded.
 */
bool loadGameData (Blackout *bl)
{
  std::string fName;

  std::cout << "Enter file name: ";
  std::cin >> fName;

  return (bl->loadGame(fName.c_str()));
}

/**
 * Plays the game.
 * @param bl Pointer to the object of the Blackout class that contains all the game data.
 */
void game (Blackout *bl)
{
  std::string m;
  int x, y, xy;
  bool gameContinue = true;
  
  while (gameContinue)
    {
      if (bl->show())
	{
	  std::cout << "\nNumber of moves: " << bl->getMoves ();
	  std::cout << "\nr:reset|s:save|l:load|q:quit";
	  std::cout << "\nYour move (rowcolumn): ";
	  std::cin >> m;

	  xy = decipherInput (m);

	  switch (xy)
	    {
	    case 0:
	      // Quit game
	      if (quit (bl))
		{
		  return;
		}
	      break;
	    case -1:
	      // Reset game
	      reset (bl);
	      break;
	    case -2:
	      // Save game
	      save (bl, "");
	      break;
	    case -3:
	      // Load game
	      load (bl);
	      break;
	    default:
	      // Standard move
	      x = xy/10;     // Extracting coordinates from the move
	      y = xy%10;
	      if (!(bl->applyMove (x, y)))
		{
		  // incomplete information
		  std::cout << "\nSorry I could not understand.\n";
		}
	      else
		{
		  gameContinue = !(bl->checkWinCondition());
		}
	      break;
	    }
	}
      else
	{
	  std::cout << "\nError displaying game state.\n";
	  break;
	}
    }
  while (gameContinue);

  if (bl->getWin())
    {
      // The game was won!
      std::cout << "\nCongratulations! You cracked the game in " << bl->getMoves() << " moves!\n";
    }
}

/**
 * Deciphers the input from the user.
 * Return value:
 *  0: Quit game
 * -1: Reset game
 * -2: Save game
 * -3: Load game
 * None of the above: move
 */
int decipherInput (std::string m)
{
  const char *input = m.c_str();

  if (input[0] == 'q')
    {
      // Quit game
      return (0);
    }

  if (input[0] == 'r')
    {
      // Reset game
      return (-1);
    }

  if (input[0] == 's')
    {
      // Save game
      return (-2);
    }

  if (input[0] == 'l')
    {
      // Load new game
      return (-3);
    }

  // If we are still here, then it is a move
  int move = atoi(input);

  return (move);
}

/**
 * Save the game.
 * @param bl Pointer to game data
 * @param question The question to be asked
 */
void save (Blackout *bl, std::string question)
{
  char ans;
  if (!question.empty())
    {
      std::cout << question;
      std::cin  >> ans;

      if (ans == 'n' || ans=='N')
	{
	  // No save is required
	  return;
	}
    }

  std::string fileName;

  std::cout << "Filename: ";
  std::cin >> fileName;
  if (bl->saveGame (fileName.c_str()))
    {
      std::cout << "\nSave successful!\n";
    }
  else
    {
      std::cout << "\nSave unsuccessful!\n";
    }
}

/**
 * Loads a new game.
 * @param bl Pointer to game data
 */
void load (Blackout *bl)
{
  std::string fileName;

  std::cout << "Filename: ";
  std::cin >> fileName;
  if (bl->loadGame (fileName.c_str()))
    {
      std::cout << "\nLoad successful!\n";
		}
  else
    {
      std::cout << "\nLoad unsuccessful!\n";
    }
}
  
/**
 * Resets the game.
 * @param bl Pointer to game data
 */
void reset (Blackout *bl)
{
  char c;

  std::cout << "\nReally reset (y/n)";
  std::cin >> c;
  if (c == 'y' || c == 'Y')
    {
      save (bl, "Save before reset (y/n)?");
      bl->reset ();
    }
}
  
/**
 * Quits the game.
 * @param bl Pointer to game data
 */
bool quit (Blackout *bl)
{
  char c;
  std::cout << "\nReally quit (y/n)";
  std::cin >> c;
  if (c == 'y' || c == 'Y')
    {
      save (bl, "Save before leaving (y/n)?");
      delete (bl);
      bl = NULL;
      return (true);
    }
  else
    {
      return (false);
    }
}

