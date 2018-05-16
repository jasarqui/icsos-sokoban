/*
const authors = {
    auth1: 'FERIA, Kiana Marie',
    auth2: 'ARQUILITA, Jasper Ian'
}

{'SOKOBAN GAME for ICS-OS'}
*/

/* put include statements here*/
#include "../../sdk/dexsdk.h"
#include "../../sdk/time.h"
#include "vga_colors.h"

// space
#define S 0
#define SPACE 0
// wall
#define W 1
#define WALL 1
// storage location
#define O 2
#define STORAGE 2
// box
#define B 3
#define BOX 3
// box on storage
#define D 4
#define BOX_ON_STORAGE 4
// keeper
#define K 5
#define KEEPER 5
// keeper on storage
#define X 6
#define KEEPER_ON_STORAGE 6

#define END 1       // use as boolean true
#define ONGOING 0   // use as boolean false
#define TRUE 1		// use as boolean true
#define FALSE 0		// use as boolean false

/* put function prototypes here */

// gui
void draw_box(int type, int x, int y);	// this is to draw a box of 18x18
void show_main_menu();					// this is to show the main menu

void print_game_menu();				// this is to print the game's home menu
void print_end_menu();				// this is to print the end game's menu
void display_about();   			// this is to display the about page
void display_how(int body);			// this is to display the how page
void congratulate();				// this is to display end game screen

// game gui
void display_game_stats();			            // displays simple how, level and moves
void display_board(int level);   	            // displays board given the level
void reload_tile(int **board, int x, int y);    // reloads tiles per move

// -- misc pages
// about
void call_about();	
void call_page();

// how
void page1_how_to();
void page2_how_to();
void page3_how_to();

// helper fxns
void start_game();	// starts the game from main
char get_input();	// this is to get char input from user

// -- main game fxns
// board
int** create_board();                                                            					// creates a new board
void reset_level(int **copy, int src[10][10], int *moveCount, int *row_player, int *col_player);    // resets the level
void copy_board(int src[10][10], int **dest);                                                       // copies the board from src to dest

// player
void move_player(char direction, int **board, int *row_player, int *col_player, int *moveCount);    // moves the player
void set_player_position(int *row_player, int *col_player, int row_value, int col_value);           // sets the row and col location of player
void get_player_position(int *row_player, int *col_player, int **board);                            // determines the location (row and column) of the player

// move
int is_completed(int **board);                                                                                                  // checks if the player has solved the puzzle
int is_valid_move(char direction, int **board, int row_player, int col_player);                                                 // checks if a given move is valid
void replace_blocks(int **board, int row_dest1, int col_dest1, int row_dest2, int col_dest2, int *row_player, int *col_player); // replace blocks 

/* global variables */
/* put global variables here */
int levels[5][10][10] = {
    { // level 1
        {W, W, W, W, W, W, W, W, W, W},
        {W, K, S, S, W, W, W, W, W, W},
        {W, S, B, B, W, W, W, W, W, W},
        {W, S, B, S, W, W, W, O, W, W},
        {W, W, W, S, W, W, W, O, W, W},
        {W, W, W, S, S, S, S, O, W, W},
        {W, W, S, S, S, W, S, S, W, W},
        {W, W, S, S, S, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W}
    },
    { // level 2
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, S, S, W, S, K, W},
        {W, W, W, S, S, S, W, S, S, W},
        {W, W, W, B, S, B, S, B, S, W},
        {W, W, W, S, B, W, W, S, S, W},
        {W, W, W, S, B, S, W, S, W, W},
        {W, O, O, O, O, O, S, S, W, W},
        {W, W, W, W, W, W, W, W, W, W}
    },
    { // level 3
        {W, W, W, W, W, W, W, W, W, W},
        {W, S, S, S, D, S, S, S, W, W},
        {W, S, B, O, W, O, B, S, W, W},
        {W, S, S, B, O, B, S, S, W, W},
        {W, W, W, O, B, O, W, W, W, W},
        {W, W, W, W, K, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
    },
    { // level 4
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, O, S, S, W, W, W},
        {W, S, B, O, B, O, S, S, W, W},
        {W, K, B, W, S, W, B, S, W, W},
        {W, S, B, O, S, O, S, S, W, W},
        {W, W, W, W, B, W, B, S, W, W},
        {W, W, W, O, S, O, S, S, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W}
    },
    { // level 5
        {W, W, W, W, W, W, W, W, W, W},
        {W, O, S, S, S, S, S, O, W, W},
        {W, S, S, S, S, S, S, S, W, W},
        {W, S, S, S, S, S, S, S, W, W},
        {W, S, S, S, K, S, S, S, W, W},
        {W, S, S, S, S, S, S, S, W, W},
        {W, S, S, S, S, S, S, S, W, W},
        {W, O, S, S, S, S, S, O, W, W},
        {W, W, W, W, W, W, W, W, W, W},
        {W, W, W, W, W, W, W, W, W, W}
    }
};

/* main function */
int main() {
	/* initialize variables here */
    char user_input = ' ';
    int level_status[5];
    int game_status = ONGOING;

    // this is to initialize all level status as ongoing
    int index = 0;
    for (index = 0; index < 5; index++) {
        level_status[index] = ONGOING;
    }

    /* main game proper */
    while (game_status == ONGOING) {
        /* this is for the user menu */
        set_graphics(VGA_320X200X256);
        clrscr();
        show_main_menu();
        if (user_input > 4 || user_input < 1) user_input = (char) getch();

        /* switch user input */
        switch(user_input) {
            case '1': start_game(); break;          // starts a new game
            case '2': call_page(); break;         // displays how to play page
            case '3': call_about(); break;       // displays the about page
            case '4': game_status = END;            // ends the loop
            default: {};                        // placeholder: character beyond 1-4 cannot be given
        }
    }

	set_graphics(VGA_TEXT80X25X16);
	clrscr();
}

/* ============================ SOKOBAN-GAME ============================ */
/* put function definitions here */
int** create_board(){
    int row;
    int **new = (int**)malloc(sizeof(int*) * 10);
    for(row = 0; row < 10; row++){
        new[row] = (int*)malloc(sizeof(int) * 10);
    }
    return new;
}

void copy_board(int src[10][10], int **dest){
    int row, col;
    for(row = 0; row < 10; row++){
        for(col = 0; col < 10; col++){
            dest[row][col] = src[row][col];
        }
    }
}

void set_player_position(int *row_player, int *col_player, int row_value, int col_value){
    *row_player = row_value;
    *col_player = col_value;
}

void get_player_position(int *row_player, int *col_player, int **board){
    int row, col;
    for(row = 0; row < 10; row++){
        for(col = 0; col < 10; col++){
            if(board[row][col] == K || board[row][col] == X){
                *row_player = row;
                *col_player = col;
            }
        }
    }
}

void reset_level(int **copy, int src[10][10], int *moveCount, int *row_player, int *col_player){
    copy_board(src, copy);
    get_player_position(row_player, col_player, copy);
    *moveCount = 0;
}

int is_completed(int **board){
    int row, col;
    for(row = 0; row < 10; row++){
        for(col = 0; col < 10; col++){
            if(board[row][col] == B)
                return FALSE;
        }
    }
    return TRUE;
}

int is_valid_move(char direction, int **board, int row_player, int col_player){
    int row_dest1, col_dest1, row_dest2, col_dest2;
    switch(direction){
        case 'w':   row_dest1 = row_player - 1;
                    col_dest1 = col_player;
                    row_dest2 = row_player - 2;
                    col_dest2 = col_player;
                    break;
        case 's':   row_dest1 = row_player + 1;
                    col_dest1 = col_player;
                    row_dest2 = row_player + 2;
                    col_dest2 = col_player;
                    break;
        case 'a':   row_dest1 = row_player;
                    col_dest1 = col_player - 1;
                    row_dest2 = row_player;
                    col_dest2 = col_player - 2;
                    break;
        case 'd':   row_dest1 = row_player;
                    col_dest1 = col_player + 1;
                    row_dest2 = row_player;
                    col_dest2 = col_player + 2;
                    break;
    }

    if(board[row_dest1][col_dest1] == S || board[row_dest1][col_dest1] == O) return TRUE;
    else if(board[row_dest1][col_dest1] == B || board[row_dest1][col_dest1] == D)
        if(board[row_dest2][col_dest2] == S || board[row_dest2][col_dest2] == O) return TRUE;
        else return FALSE;
    else return FALSE;
}

void replace_blocks(int **board, int row_dest1, int col_dest1, int row_dest2, int col_dest2, int *row_player, int *col_player){
    if(board[row_dest1][col_dest1] == S){
        board[row_dest1][col_dest1] = K;
    }
    else if(board[row_dest1][col_dest1] == O){
        board[row_dest1][col_dest1] = X;
    }
    else if(board[row_dest1][col_dest1] == B){
        if(board[row_dest2][col_dest2] == S) board[row_dest2][col_dest2] = B;
        else if(board[row_dest2][col_dest2] == O) board[row_dest2][col_dest2] = D;
        board[row_dest1][col_dest1] = K;
    }
    else if(board[row_dest1][col_dest1] == D){
        if(board[row_dest2][col_dest2] == S) board[row_dest2][col_dest2] = B; 
        else if(board[row_dest2][col_dest2] == O) board[row_dest2][col_dest2] = D;
        board[row_dest1][col_dest1] = X;
    }
    reload_tile(board, row_dest1, col_dest1);
    reload_tile(board, row_dest2, col_dest2);
    board[*row_player][*col_player] = (board[*row_player][*col_player] == K) ? S:O;
    set_player_position(row_player, col_player, row_dest1, col_dest1);
 }   

void move_player(char direction, int **board, int *row_player, int *col_player, int *moveCount){
    switch(direction){
        case 'w':   if(is_valid_move('w', board, *row_player, *col_player)){
                        replace_blocks(board, *row_player - 1, *col_player, *row_player - 2, *col_player, row_player, col_player);
                        (*moveCount)++;
                    }
                    break; 
        case 's':   if(is_valid_move('s', board, *row_player, *col_player)){
                        replace_blocks(board, *row_player + 1, *col_player, *row_player + 2, *col_player, row_player, col_player);
                        (*moveCount)++;
                    }   
                    break;
        case 'a':   if(is_valid_move('a', board, *row_player, *col_player)){
                        replace_blocks(board, *row_player, *col_player - 1, *row_player, *col_player - 2, row_player, col_player);
                        (*moveCount)++;
                    }
                    break;
        case 'd':   if(is_valid_move('d', board, *row_player, *col_player)){
                        replace_blocks(board, *row_player, *col_player + 1, *row_player, *col_player + 2, row_player, col_player);
                        (*moveCount)++;
                    }
                    break;
    }
}

void start_game(){
	set_graphics(VGA_320X200X256);
	clrscr();

    int row_player, col_player, level_number, moveCount, level_status, game_status = ONGOING;
    // iterate over the levels of the game
    for(level_number = 0; level_number < 5; level_number++){
        level_status = ONGOING;
        moveCount = 0;
        // create a new board and copy the initial values of the board and set number of moves to 0
        int **level_board = create_board();
        copy_board(levels[level_number], level_board);
    	display_board(level_number);
		display_game_stats();
        get_player_position(&row_player, &col_player, level_board);
        // while level is not completed, user can enter a move, reset the game, or go back to the main menu 
        while(level_status == ONGOING){
            printf("LEVEL %d\n", level_number + 1);
            printf("Moves: %d\n", moveCount);
            printf(">> ");
            char user_input = getch();
            switch(user_input){
                case 'r':   reset_level(level_board, levels[level_number], &moveCount, &row_player, &col_player);
                            break;
                case 'q':   level_status = END;
                            game_status = END;
                            break;
                case 'w':   move_player('w', level_board, &row_player, &col_player, &moveCount);
                            break;
                case 's':   move_player('s', level_board, &row_player, &col_player, &moveCount);
                            break;
                case 'a':   move_player('a', level_board, &row_player, &col_player, &moveCount);
                            break;
                case 'd':   move_player('d', level_board, &row_player, &col_player, &moveCount);
                            break;
            }
            if(is_completed(level_board)){
                if(level_number == 4 ){
                    // print_end_menu();
                    char input = getch();
                    switch(input){
                        case '1': start_game(); break;
                        case '2': game_status = END; break;
                    }
                }
                level_status = END;
            }
        }
        if(game_status == END) break;   
    }
}

void call_page(){
    page1_how_to();
}

void page1_how_to(){
    set_graphics(VGA_320X200X256);
    clrscr();
    display_how(0);
    printf("Page 1\n");
    printf("[1] Back\n");
    printf("[2] Next\n");
    printf(">> ");
    char input = getch();
    switch(input){
        case '1':  break;
        case '2':  page2_how_to();
                   break; 
    }
}

void page2_how_to(){
    set_graphics(VGA_320X200X256);
    clrscr();
    display_how(1);
    printf("Page 2\n");
    printf("[1] Back\n");
    printf("[2] Next\n");
    printf(">> ");
    char input = getch();
    switch(input){
        case '1':   page1_how_to();
                    break;
        case '2':   page3_how_to();
                    break;      
    }
}

void page3_how_to(){
    set_graphics(VGA_320X200X256);
    clrscr();
    display_how(2);
    printf("Page 3\n");
    printf("[1] Back\n");
    printf("[2] Main Menu\n");
    printf(">> ");
    char input = getch();
    switch(input){
        case '1':   page2_how_to();
                    break;
        case '2':   break;    
    }
}

void call_about(){
    set_graphics(VGA_320X200X256);
    clrscr();
    display_about();
    char input;
    do{
        printf("About\n");
        printf("[1] Back\n");
        printf(">> ");
        input = getch();
    }while(input != '1');   
}

/* ============================ GUI SOKOBAN ============================ */
// see comments in prototypes for fxn use
void draw_box(int type, int x, int y) {
	int color;

	switch (type) {
		case SPACE: color = GRAY; break;
		case WALL: color = TURQUOISE; break;
		case BOX: color = DAFFODIL; break;
		case BOX_ON_STORAGE: color = SCARLET_RED; break;
		case KEEPER: color = WHITE; break;
		case KEEPER_ON_STORAGE: color = WHITE; break;
		case STORAGE: color = LIMESTONE_GREEN; break;
		default: color = BLACK;
	}

	int x_in = 0, y_in = 0;
	for (x_in = 0; x_in < 18; x_in++) {
		for (y_in = 0; y_in < 18; y_in++) {
			if (x_in == 0 || x_in == 17 || 
				y_in == 0 || y_in == 17) write_pixel(x + x_in, y + y_in, BLACK);
			else write_pixel(x + x_in, y + y_in, color);
		}
	}
}

void show_main_menu() {
	int box_start;
	for (box_start = 52; box_start < 268; box_start += 18) {
		draw_box(WALL, box_start, 10);
	}
	for (box_start = 52; box_start < 268; box_start += 18) {
		if (box_start == 52 || box_start == 250) draw_box(WALL, box_start, 28);
		else draw_box(SPACE, box_start, 28);
	}
	for (box_start = 52; box_start < 268; box_start += 18) {
		if (box_start == 52 || box_start == 250) draw_box(WALL, box_start, 46);
		else draw_box(SPACE, box_start, 46);
	}
	for (box_start = 52; box_start < 268; box_start += 18) {
		draw_box(WALL, box_start, 64);
	}

	draw_box(SPACE, 39, 105);
	draw_box(SPACE, 39, 125);
	draw_box(SPACE, 39, 145);
	draw_box(SPACE, 39, 165);
    write_text("SOKOBAN", 130, 40, WHITE, 1); 
	write_text("1  Start", 45, 110, WHITE, 0); 
	write_text("2  How to Play", 45, 130, WHITE, 0);
	write_text("3  About", 45, 150, WHITE, 0);
	write_text("4  Exit", 45, 170, WHITE, 0);
}

void congratulate() {
	write_text("CONGRATULATIONS!", 85, 60, WHITE, 1);

	// controls
	draw_box(SPACE, 44, 115);
	draw_box(SPACE, 44, 135);
	write_text("1  Play again", 50, 120, WHITE, 0);
	write_text("2  Back to Main Menu", 50, 140, WHITE, 0);
}

void display_about() {
	// controls
	draw_box(SPACE, 22, 5);
	write_text("< 1  Back", 10, 10, WHITE, 0);

	// text body
	write_text("ABOUT", 130, 40, ICE_BLUE, 1);
	write_text("Sokoban is a type of transport", 25, 75, WHITE, 0);
	write_text("puzzle in which the player pushes", 12, 90, WHITE, 0);
	write_text("boxes or crates around in a", 35, 105, WHITE, 0);
	write_text("warehouse, trying to get them to", 17, 120, WHITE, 0);
	write_text("storage locations. The puzzle is", 17, 135, WHITE, 0);
	write_text("usually implemented as a", 45, 150, WHITE, 0);
	write_text("video game.", 112, 165, WHITE, 0);	
}

void display_how(int body) {
	// controls
	draw_box(SPACE, 22, 5);
	draw_box(SPACE, 278, 5);
	write_text("< 1  Back", 10, 10, WHITE, 0);
	write_text("Next  2 >", 230, 10, WHITE, 0);

	// text body
	write_text("HOW TO PLAY", 110, 40, ICE_BLUE, 1);
	if (body == 0) {
		write_text("The player is confined to the", 27, 75, WHITE, 0);
		write_text("board, and may move horizontally", 15, 90, WHITE, 0);
		write_text("or vertically onto empty squares.", 11, 105, WHITE, 0);

		// player controls
		draw_box(SPACE, 151, 139);
		draw_box(SPACE, 133, 157);
		draw_box(SPACE, 151, 157);
		draw_box(SPACE, 169, 157);
		write_text("W", 157, 144, WHITE, 0);
		write_text("A", 139, 162, WHITE, 0);
		write_text("S", 157, 162, WHITE, 0);
		write_text("D", 175, 162, WHITE, 0);
		write_text("UP", 152, 126, WHITE, 0);
		write_text("LEFT", 94, 162, WHITE, 0);
		write_text("RIGHT", 193, 162, WHITE, 0);
		write_text("DOWN", 145, 180, WHITE, 0);
	} else if (body == 1) {
		write_text("The player can also move into a", 20, 75, WHITE, 0);
		write_text("box, which pushes it to the square", 8, 90, WHITE, 0);
		write_text("beyond. Boxes may not be pushed", 19, 105, WHITE, 0);
		write_text("into other boxes or walls, and", 23, 120, WHITE, 0);
		write_text("they cannot be pulled.", 60, 135, WHITE, 0);
	} else if (body == 2) {
		write_text("The number of boxes is equal to", 20, 75, WHITE, 0);
		write_text("the number of storage locations.", 15, 90, WHITE, 0);
		write_text("The puzzle is solved when all", 28, 105, WHITE, 0);
		write_text("boxes are at storage locations.", 22, 120, WHITE, 0);
	}	
}

void display_game_stats() {
	write_text("SOKOBAN", 37, 15, ICE_BLUE, 1);
	
	// char str[10];
	// sprintf(str, "%d", moves);
	// write_text(str, x, y, WHITE, 0);

	// controls
	draw_box(SPACE, 61, 73);
	draw_box(SPACE, 43, 91);
	draw_box(SPACE, 79, 91);
	draw_box(SPACE, 61, 109);
	write_text("w", 67, 78, WHITE, 0);
	write_text("a", 49, 96, WHITE, 0);
	write_text("s", 85, 96, WHITE, 0);
	write_text("d", 67, 114, WHITE, 0);

	// game
	draw_box(SPACE, 5, 155);
	draw_box(SPACE, 5, 175);
	write_text("r  Reset", 10, 160, WHITE, 0);
	write_text("q  Quit", 10, 180, WHITE, 0);
}

void display_board(int level) {
	int row = 0, col = 0;
	for (row = 0; row < 10; row++) {
		for (col = 0; col < 10; col++) {
			switch (levels[level][col][row]) {
				case SPACE: draw_box(SPACE, (130 + (row * 18)), (10 + (col * 18))); break;
				case WALL: draw_box(WALL, (130 + (row * 18)), (10 + (col * 18))); break;
				case KEEPER: draw_box(KEEPER, (130 + (row * 18)), (10 + (col * 18))); break;
				case KEEPER_ON_STORAGE: draw_box(KEEPER_ON_STORAGE, (130 + (row * 18)), (10 + (col * 18))); break;
				case BOX: draw_box(BOX, (130 + (row * 18)), (10 + (col * 18))); break;
				case BOX_ON_STORAGE: draw_box(BOX_ON_STORAGE, (130 + (row * 18)), (10 + (col * 18))); break;
				case STORAGE: draw_box(STORAGE, (130 + (row * 18)), (10 + (col * 18))); break;
				default: {};
			}
		}
	}
}

void reload_tile(int** board, int x, int y) {
    switch (board[y][x]) {
        case SPACE: draw_box(SPACE, (130 + (x * 18)), (10 + (y * 18))); break;
        case WALL: draw_box(WALL, (130 + (x * 18)), (10 + (y * 18))); break;
        case KEEPER: draw_box(KEEPER, (130 + (x * 18)), (10 + (y * 18))); break;
        case KEEPER_ON_STORAGE: draw_box(KEEPER_ON_STORAGE, (130 + (x * 18)), (10 + (y * 18))); break;
        case BOX: draw_box(BOX, (130 + (x * 18)), (10 + (y * 18))); break;
        case BOX_ON_STORAGE: draw_box(BOX_ON_STORAGE, (130 + (x * 18)), (10 + (y * 18))); break;
        case STORAGE: draw_box(STORAGE, (130 + (x * 18)), (10 + (y * 18))); break;
        default: {};
    }
}