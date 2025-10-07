#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct player { // defining the player struct type
    int flashlight;
    int health;
    int stamina;
    int row;
    int col;
} player;

void initialBoard (const int ROW, const int COL, char board[ROW][COL], player* stats); // generates the initial board with two exits and borders
void obstacle(char board[20][20]); // populates the board with randomly placed obstacles
void items(char board[20][20]); // populates the board with randomly placed items (health and stamina packs)
void displayBoard(const int ROW, const int COL, char board[ROW][COL], player* stats); // Displays the board to the player at the start of the game and after every turn
void playerMovement(player* stats, char board[20][20]); // controls all player input (move in all four cardinal directions, shine the flashlight, and quit the game)
void interactObstacle(player* stats, char board[20][20]); // updates the players health if they interact with a boulder or a rock
void pickItems(player* stats, char board[20][20]); // updates the player's health and stamina if they bump into an obstacle
void useFlash(player* stats, char board[20][20]); // controls the function of the flashlight, as well as how many times a player can use it
void displayStats(player* stats); //displays the stamina, health, and flashlight charges remaining at the start of the game and after every turn
int checkExit(player* stats, char board[20][20]); //checks if the player has reached one of the two exits
int gameOver(player* stats); //ends the game if the player's stamina or health reaches zero.

int main(){
    player stats; //creates a struct that contains all of the relevant variables for the player
    stats.flashlight = 5;
    stats.health = 100;
    stats.stamina = 100;
    stats.row = 1;
    stats.col = 1;

    const int ROW = 20, COL = 20; //initializes the game board
    char board[ROW][COL];

    srand(time(NULL)); //generates random numbers for the obstacle locations
    initialBoard(ROW, COL, board, &stats); //fills the board with the borders and the exits

    while(1) { //infinite loop so game keeps running until an end-of-game condition is met
        displayBoard(ROW, COL, board, &stats);
        displayStats(&stats);

        if(gameOver(&stats)) {
            break; //ends the program if the player dies or runs out of stamina
        }

        playerMovement(&stats, board); //calls the function that controls the players movement and flashlight usage
        if (board[stats.row][stats.col] == 'X') { //ends the game if the player walks on an exit
            printf("Congrats! You have reached the exit!\n");
            break;
        }
    }
    return 0;
}

void initialBoard (const int ROW, const int COL, char board[ROW][COL], player* stats) { // function that creates the border and exit points for the board
    int i, j;
    i = j = 0;
    for(i = 0; i < ROW; i++) {
        for(j = 0; j < COL; j++) {
            if(i == 0 || i == ROW - 1) { // fills top and bottom border with '-'
                board[i][j] = '-';
            }
            else if(j == 0 || j == COL - 1) { // fills the side border with '|'
                board[i][j] = '|';
            }
            else if((j == 18 && i == 18) || (i == 18 && j == 1)) { // two exit point
                board[i][j] = 'X';
            }
            else { // fills rest with empty spaces
                board[i][j] = ' ';
            }
        }
    }
    obstacle(board); // randomly fills the board with obstacles
    items(board); //same as above, but for stamina and health packs
}

void obstacle(char board[20][20]) { // function that randomly generates obstacles for the board
    int num;
    char obstType;
    int ROW, COL; // int where the objects will be placed on the board
    int i;

    for (i = 0; i < 35; i++) {
        num = rand() % 2;
        if (num == 1) {
            obstType = 'R'; // 'R' = Rock
        }
        else {
            obstType = 'B'; // 'B' = Boulder
        }
        do {
            ROW = rand() % 20; // generates a rand num from the row
            COL = rand() % 20; // generates a rand num from the col
        }
        while(board[ROW][COL] != ' '); // checks to see if space is empty
        board[ROW][COL] = obstType;
    }
}

void items(char board[20][20]) {
    char itemType;
    int ROW, COL, num, i;

    for (i = 0; i < 10; i++) { // limits total amount of items to 10
        num = rand() % 2;
        if (num == 1) {
            itemType = 'S'; // S = stamina
        }
        else {
            itemType = 'H'; // H = health
        }
        do {
            ROW = rand() % 20;
            COL = rand() % 20;
        }
        while(board[ROW][COL] != ' '); // ensures that only empty spaces are populated with items
        board[ROW][COL] = itemType;
    }
}

void displayBoard(const int ROW, const int COL, char board[ROW][COL], player* stats) {
    char displayBoard[ROW][COL]; // generates the board to be displayed
   memcpy(displayBoard, board, sizeof(char) * ROW * COL); // used to copy the original board into the display board

    displayBoard[stats -> row][stats -> col] = 'P'; // generates the player's location on the display board

    int i, j;
    for (i = 0; i < ROW; i++) {
        for (j = 0; j < COL; j++) {
                 if (board[i][j] == 'R' || board[i][j] == 'B' || board[i][j] == 'S' || board[i][j] == 'H') {
                    printf(" "); // hides the items and obstacles
                }
                else {
                    printf("%c", displayBoard[i][j]);
                }  //the commented out lines of code are for hiding the item and obstacles, use when project is finished

           //     printf("%c", displayBoard[i][j]);
            }
        printf("\n");
    }
    printf("\n");
}

void playerMovement(player* stats, char board[20][20]) {
    char input;
    printf("Use 'w', 'a', 's', or 'd' key to move\n");
    printf("Use 'q' to quit the game\n");
    printf("Use 'f' to use the flashlight");
    scanf(" %c", &input);

    int newRow = stats->row; // These two lines are used to update the players location
    int newCol = stats->col;
    int prevCol = stats->col; //These two lines are used to save the player's original location in case they run into an obstacle (they will revert back to their old location in this case)
    int prevRow = stats->row;

    if (input == 'w' && stats->row > 1) { //These four lines respond to WASD input while ensuring that the player does not run out of bounds
        newRow--;
        stats->stamina -= 2;
    }
    else if (input == 's' && stats->row < 18) {
        newRow++;
        stats->stamina -= 2;
    }
    else if (input == 'a' && stats->col > 1) {
        newCol--;
        stats->stamina -= 2;
    }

    else if (input == 'd' && stats->col < 18) {
        newCol++;
        stats->stamina -= 2;
    }
    else if (input == 'f') { //uses a flashlight if player presses the f key
        useFlash(stats, board);
    }
    else if (input == 'q') exit(0); //else case for illegal moves
    else {
        printf("Invalid move! Out of bounds or blocked by obstacle.\n");
        return;
    }

    if (board[newRow][newCol] == ' ' || board[newRow][newCol] == 'H' || board[newRow][newCol] == 'S' || board[newRow][newCol] == 'X') {
        stats->row = newRow; //players can step on an item, so the location is updated and the
        stats->col = newCol;
        pickItems(stats, board);
    } if(board[newRow][newCol] == 'R' || board[newRow][newCol] == 'B') {
        stats->row = newRow;
        stats->col = newCol;

        interactObstacle(stats, board);
        stats->row = prevRow;
        stats->col = prevCol;

    }
}

void interactObstacle(player* stats, char board[20][20]) {

    if (stats->health <= 100 && stats->health > 0) { // checks if the player is in within the range of 1 - 100
        // checks to see if the players location is in the location of the bolder
        if (board[stats->row][stats->col] == 'B') {
            stats->health -= 10; // minus 10 HP from player
            printf("You hit a boulder! Health decreased by 10.\n");
        }
        // checks to see if the players location is in the location of the rock
        else if (board[stats->row][stats->col] == 'R') {
            stats->health -= 5; // minus 5 HP from player
            printf("You hit a rock! Health decreased by 5.\n");
        }
    }
}

void pickItems(player* stats, char board[20][20]) {
    if (board[stats->row][stats->col] == 'H') { // checks if player location is in the location of health pack
        stats->health += 10; // add 10 to HP
        board[stats->row][stats->col] = ' '; // replaces the health pack with an empty space
    }
    if (stats->health > 100) {
        // checks to see if HP is over 100
        stats->health = 100; // if over 100 then keep it as 100
    }

    if (board[stats->row][stats->col] == 'S') { // checks if player location is in the location of health pack
            stats->stamina += 10; // add 10 to stamina
            board[stats->row][stats->col] = ' '; // replaces the stamina pack with an empty space
        }
    if (stats->stamina > 100) { // checks to see if stamina is over 100
        stats->stamina = 100; // if over 100 then keep it as 100
    }
}

void useFlash(player* stats, char board[20][20]) { //function for activating the flashlight
    int i, j;

    if (stats->flashlight >= 1) {
        stats->flashlight -= 1;
        printf("Flashlight activated! Showing a 5x5 area around you:\n");

        for (i = stats->row - 2; i <= stats->row + 2; i++) {
            for (j = stats->col - 2; j <= stats->col + 2; j++) {
                if (i >= 0 && i < 20 && j >= 0 && j < 20) {  // Ensure indices are within bounds
                    if (i == stats->row && j == stats->col) {
                        printf("P");  // Display player position as 'P'
                    } else {
                        printf("%c", board[i][j]);  // Display the cell content
                    }
                } else {
                    printf(" ");  // Display empty space if out of bounds
                }
            }
            printf("\n");
        }
        printf("\n");
    } else {
        printf("No flashlight charges available.\n");
    }
}

void displayStats(player* stats) { //Displays the health, stamina, and flashlight charge of the player
    printf("Player Stats:\n");
    printf("Health: %d\n", stats->health);
    printf("Stamina: %d\n", stats->stamina);
    printf("Flashlight Charges:%d\n\n", stats->flashlight);
}

int checkExit(player* stats, char board[20][20]) { //Checks if the player has reached an exit, or 'X'
    if (board[stats->row][stats->col] == 'X') {
        printf("Congrats!! You have reached the exited!");
        return 1;
    }

        return 0;
}

int gameOver(player* stats) { //Ends the game if the players health or stamina reaches zerod

    if (stats->health <= 0) {
        stats->health = 0;
        printf("Game Over.\n");
        printf("You have died!\n");
        return 1;
    }
    return 0;
}
