#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_MAZE_WIDTH 500
#define MAX_MAZE_HEIGHT 500
// maze storage:
// '#' == wall
// ' ' == visited by generator
// '.' == visited by pathfinder
int MAZE_WIDTH = 100;
int MAZE_HEIGHT = 50;
int NO_DIAGONALS = 1; // whether pathways can be diagonal from one another.

char maze[MAX_MAZE_WIDTH*MAX_MAZE_HEIGHT];
int nextSolutionCell = 0;

void changeCell(char * cell, char value);
int checkCell(int x, int y, int notIncl);
char * getCell(int x, int y);
void printMaze();
void depthFirstSearchGeneration(int x, int y);
int depthFirstSearchSolver(int x, int y, int endx, int endy, int solution[]);
void initializeMaze();
void printMazeWithSolution (int solution[]);
void findSolutionBetweenEntrances(int solution[]);
void punchHoles(int side);

int main(void) {
  time_t t;
  srand((unsigned) time(&t));
  char command = '-';
  int newInstructions = 1;
  while (command != 'Q') {
    if (newInstructions) {
      printf("Commands:\nG - Generate\nS - Solve\ns - Solve with manual points\nD - Toggle diagonals\nQ - Quit\n");
      newInstructions = 0;
    }
    scanf("%c",&command);
    switch (command) {
      case 'G':
      system("cls");
      printf("Ex: Type 50x50 to generate a 50x50 maze.\n");
      scanf("%dx%d",&MAZE_WIDTH,&MAZE_HEIGHT);
      initializeMaze();
      depthFirstSearchGeneration(1,1);
      punchHoles(rand()%2);
      system("cls");
      printMaze();
      newInstructions = 1;
      break;
      case 'D':
      NO_DIAGONALS = !NO_DIAGONALS;
      printf("Use diagonals: %d\n",NO_DIAGONALS);
      break;
      case 's':
        printf("Ex: Type 1,1,30,30 to find solution from (1,1) to (30,30). (1,1,48,48) would be top left to bottom right on a 50x50 maze.\n");
        int sx, sy, ex, ey;
        scanf("%d,%d,%d,%d",&sx,&sy,&ex,&ey);
        int possiblePath = 0;
        if (*getCell(sx,sy) != '#')
          possiblePath++;
        else
          printf("Starting position is a wall. Try again.\n");
        if (*getCell(ex,ey) != '#')
          possiblePath++;
        else
          printf("Ending position is a wall. Try again.\n");
        if (possiblePath == 2) {
          int solution[MAZE_WIDTH*MAZE_HEIGHT];
          nextSolutionCell = 0;
          for (int i = 0; i < MAZE_WIDTH*MAZE_HEIGHT; i++) {
            if (maze[i] == '.') {
              maze[i] = ' ';
            }
          }
          depthFirstSearchSolver(sx,sy,ex,ey,solution);

          system("cls");
          printMazeWithSolution(solution);
          int exploredTotal = 0;
          for (int i = 0; i < MAZE_WIDTH*MAZE_HEIGHT; i++) {
            if (maze[i] == '.') {
              exploredTotal++;
            }
          }
          printf("%d cells explored ('.'), %d solution length. ('*')\n",exploredTotal, nextSolutionCell);
          newInstructions = 1;

        }
        break;
      case 'S':
        if (1) {
        int solution[MAZE_WIDTH*MAZE_HEIGHT];
        nextSolutionCell = 0;
        for (int i = 0; i < MAZE_WIDTH*MAZE_HEIGHT; i++) {
          if (maze[i] == '.') {
            maze[i] = ' ';
          }
        }

        findSolutionBetweenEntrances(solution);

        system("cls");
        printMazeWithSolution(solution);
        int exploredTotal = 0;
        for (int i = 0; i < MAZE_WIDTH*MAZE_HEIGHT; i++) {
          if (maze[i] == '.') {
            exploredTotal++;
          }
        }
        printf("%d cells explored ('.'), %d solution length. ('*')\n",exploredTotal, nextSolutionCell);
        newInstructions = 1;
        break;
      }
    }


  }
  // initializeMaze();
  // depthFirstSearchGeneration(1,1);
  // printMaze();
  //
  // int solution[MAZE_WIDTH*MAZE_HEIGHT];
  //
  // depthFirstSearchSolver(MAZE_WIDTH-2,MAZE_HEIGHT-2,1,1,solution);
  //
  // printMazeWithSolution(solution);
}

// use randomized version of depth-first-search to generate maze.
void depthFirstSearchGeneration(int x, int y) {
  // set current cell as visited
  changeCell(getCell(x,y),' ');
  int complete = 0;
  int random;

  // randomly select possible neighbors to run depthFirstSearchGeneration on
  while (!complete) {
    random = rand() % 4;
    complete = 1;
    if (checkCell(x+1,y,2)) {
      complete = 0;
      if (random == 0) {
        depthFirstSearchGeneration(x+1,y);
        complete = 0;
      }
    }
    if (checkCell(x-1,y,0)) {
      complete = 0;
      if (random == 1) {
        depthFirstSearchGeneration(x-1,y);
        complete = 0;
      }
    }
    if (checkCell(x,y-1,3)) {
      complete = 0;
      if (random == 2) {
        depthFirstSearchGeneration(x,y-1);
        complete = 0;
      }
    }
    if (checkCell(x,y+1,1)) {
      complete = 0;
      if (random == 3) {
        depthFirstSearchGeneration(x,y+1);
        complete = 0;
      }
    }
  }
}

// check surrounding neighbors of possible new cell, and return 1 if possible
// add to solutions array if part of the solution path
int depthFirstSearchSolver(int x, int y, int endX, int endY, int solution[]) {
  changeCell(getCell(x,y),'.');
  if (x == endX && y == endY) {
    solution[nextSolutionCell++] = y*MAZE_WIDTH+x;
    return 1;
  }
  int solutionTile = 0;
  if (!solutionTile && *getCell(x+1,y) == ' ') {
    int res = depthFirstSearchSolver(x+1,y,endX,endY,solution);
    if (res == 1) {
      solutionTile = 1;
    }
  }

  if (!solutionTile && *getCell(x-1,y) == ' ') {
    int res = depthFirstSearchSolver(x-1,y,endX,endY,solution);
    if (res == 1) {
      solutionTile = 1;
    }
  }

  if (!solutionTile && *getCell(x,y+1) == ' ') {
    int res = depthFirstSearchSolver(x,y+1,endX,endY,solution);
    if (res == 1) {
      solutionTile = 1;
    }
  }

  if (!solutionTile && *getCell(x,y-1) == ' ') {
    int res = depthFirstSearchSolver(x,y-1,endX,endY,solution);
    if (res == 1) {
      solutionTile = 1;
    }
  }

  if (solutionTile) {
    solution[nextSolutionCell++] = y*MAZE_WIDTH+x;
    return 1;
  }



}

int checkCell(int x, int y, int notIncl) {

  // checking north, south, east, west
  if (x == 0 || x == MAZE_WIDTH-1 || y == 0 || y == MAZE_HEIGHT-1)
  return 0;
  if (*getCell(x+1,y) == ' ' && notIncl != 0)
  return 0;
  if (*getCell(x-1,y) == ' ' && notIncl != 2)
  return 0;
  if (*getCell(x,y+1) == ' ' && notIncl != 3)
  return 0;
  if (*getCell(x,y-1) == ' ' && notIncl != 1)
  return 0;

  // checking diagonals
  if (NO_DIAGONALS == 1) {
    if (*getCell(x+1,y-1) == ' ' && (notIncl != 0 && notIncl != 1))
    return 0;
    if (*getCell(x-1,y-1) == ' ' && (notIncl != 1 && notIncl != 2))
    return 0;
    if (*getCell(x-1,y+1) == ' ' && (notIncl != 2 && notIncl != 3))
    return 0;
    if (*getCell(x+1,y+1) == ' ' && (notIncl != 3 && notIncl != 0))
    return 0;
  }

  // checking itself
  if (*getCell(x,y) == ' ')
  return 0;
  return 1;
}

// initialize maze with all walls, clear any previous info
void initializeMaze() {
  for (int i = 0; i < MAX_MAZE_WIDTH*MAX_MAZE_HEIGHT; i++) {
    if (i < MAZE_WIDTH*MAZE_HEIGHT)
    maze[i] = '#';
    else
    maze[i] = '\0';
  }
}



void printMaze() {
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      printf("%c%c",*getCell(x,y),*getCell(x,y));
    }
    printf("\n");
  }
}

// overlay asteriks to display solution onto the maze.
void printMazeWithSolution (int solution[]) {
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      int tileIsSolution = 0;
      for (int i = 0; i < nextSolutionCell; i++) {
        if (solution[i] == y*MAZE_WIDTH+x) {
          tileIsSolution = 1;
          break;
        }
      }
      if (tileIsSolution) {
        printf("**");
      } else {
        printf("%c%c",*getCell(x,y),*getCell(x,y));
      }
    }
    printf("\n");
  }
}

// make two possible holes to create an "entrance" and "exit" to the maze.
void punchHoles(int side) {
  int sx,sy,ex,ey;

  int validHoles = 0;
  while (!validHoles) {

    if (side == 0) {
      sy = 1+rand()%(MAZE_HEIGHT-1);
      sx = 1;
      ey = 1+rand()%(MAZE_HEIGHT-1);
      ex = MAZE_WIDTH-2;
      if (*getCell(sx,sy) == ' ' && *getCell(ex,ey) == ' ') {
        changeCell(getCell(sx-1,sy),' ');
        changeCell(getCell(ex+1,ey),' ');
        validHoles = 1;
      }
    } else {
      sx = 1+rand()%(MAZE_WIDTH-1);
      sy = 1;
      ex = 1+rand()%(MAZE_WIDTH-1);
      ey = MAZE_HEIGHT-2;
      if (*getCell(sx,sy) == ' ' && *getCell(ex,ey) == ' ') {
        changeCell(getCell(sx,sy-1),' ');
        changeCell(getCell(ex,ey+1),' ');
        validHoles = 1;
      }
    }
  }
}
// Finds "holes" in maze and solves between them.
void findSolutionBetweenEntrances(int solution[]) {
  int sx,sy,ex,ey;

  int step = 0;
  for (int y = 0; y < MAZE_HEIGHT; y++) {
    for (int x = 0; x < MAZE_WIDTH; x++) {
      if ((x == 0 || x == MAZE_WIDTH-1 || y == 0 || y == MAZE_HEIGHT-1) && *getCell(x,y) == ' ') {
        if (step == 0) {
          sx = x;
          sy = y;
          step+=1;
        } else if (step == 1) {
          ex = x;
          ey = y;
          step+=1;
        }
      }
    }
  }
  depthFirstSearchSolver(sx,sy,ex,ey,solution);

}

char * getCell(int x, int y) {
  return &(maze[y*MAZE_WIDTH+x]);
}

void changeCell(char * cell, char value) {
  *cell = value;
}
