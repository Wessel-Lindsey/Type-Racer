/*
SE 185: Final Project Template
Team xx
Team member 1 Mason | 33.33%
Team member 2 Nathaniel | 33.33%
Team member 3 Lindsey| 33.33%
*/
#include <ctype.h>
#include <ncurses/ncurses.h>
#include <stdio.h> //// add more here////
#include <stdlib.h>
#include <string.h>
#include <sys/time.h> //used to incrament time
#include <time.h>
#define GRIDY 50
#define GRIDX 100
#define MAXWORDS 1100
#define WORDLENGTH 30

/*					STRUCT */
typedef struct word {
  char word[30];
  int x;
  int y;
} word;

/*			User Defined Functions Prototype						*/
void draw_character(int y, int x, char use); // Outputs to screenchar at y,x
const char *pickRandom(char words[][WORDLENGTH], int numWords);
int getLinesGenerated(int time, int numCalled);
void printBoard(); // literaly prints board

int main() {
  int minWordLen, numRead, pas, addWordLength = 0;
  char startInput = 'z';
  char wordList[MAXWORDS][WORDLENGTH], addWord[20][20];
  FILE *fp = fopen("wordList.txt", "r");
  int i = 0, total = 0, pastTime = 0, currentTime = 1; // =0 x2
  struct word
      wordsInPlay[(GRIDY) *
                  (GRIDX / 5)]; // i thnk this lenght is wrong (there can be
                                // more words than the length or the width)
  //(Y val time the (x val devided by the avg word length))  should = the max
  // amount of words

  // time values
  struct timeval beforeT, afterT, resultT;
  int sec = 0;
  // word wordsInPlay[78];	//use for words on screen

  // put list of words from file in an array

  while (fgets(wordList[i], WORDLENGTH, fp)) {
    // remove \n's
    wordList[i][strlen(wordList[i]) - 1] = '\0';
    i++;
  }
  fclose(fp);
  total = i;
  // so now wordList contains every word from file

  printf("Hello, this is a typing game. You will need to type the words as "
         "they appear.\nWould you like to play or add a word? You currently "
         "have %d words that can be used. (p/w)\n",
         total);

  fflush(stdout);
  while (startInput != 'p') {
    scanf("%c", &startInput);
    if (startInput == 'p') {
      printf("What would you like your minimun word length to be?\n");
      scanf("%d", &minWordLen);
      if (minWordLen <= 0) {
        printf("Enter a number greater than 0. \n");
        startInput = '\0'; // error
      }
    } else if (startInput == 'w') {
      // add words to word list
      i = 0;
      char tempWord[20];
      while (strcmp(tempWord, "x") != 0 && i <= 20) {
        printf(
            "Type a word you would like to add to the list, \"x\" to stop: ");
        scanf("%s", tempWord);
        if (strcmp(tempWord, "x") != 0) {
          strcpy(addWord[i], tempWord);
          addWordLength++;
        }
        i++;
      }
    } else {
      printf("Please enter either \'p\' or \'w\'\n");
    }
  } // minwordlen is defined

  char minLenthWL[MAXWORDS][WORDLENGTH]; // array of usable words with min length
  int tempIndex = 0;
  for (i = 0; i < total; i++) {
    if (strlen(wordList[i]) > minWordLen) {
      strcpy(minLenthWL[tempIndex], wordList[i]);
      tempIndex++;
    }
  }
  // check user inputed words
  int count = 0;
  for (i = 0; i < addWordLength; i++) {
    // printf("Cycling through addedWords:\n");
    printf("%s\n", addWord[i]);
    if (strlen(addWord[i]) > minWordLen) {
      strcpy(minLenthWL[tempIndex + count], addWord[i]);
      // printf("Added to minLenthWL: %s\n", addWord[i]);
      count++;
    }
  }

  for (i = 0; i < tempIndex + 1; i++) {
    printf("%s\n", minLenthWL[i]);
  }
  // start game
  // Board Set Up
  initscr();
  printBoard();
  refresh();

  //get one random word
  strcpy(wordsInPlay[0].word, pickRandom(minLenthWL, tempIndex));
  wordsInPlay[0].y = 2;
  wordsInPlay[0].x = rand() % ((GRIDX)-strlen(wordsInPlay[0].word)) + 2;
  mvprintw(wordsInPlay[0].y, wordsInPlay[0].x, "%s", wordsInPlay[0].word);
  //loop variable
  int lose = 0;
  int numWordsInPlay =  1; // decrements when word typed, increases when word appears
  char wordTyped[WORDLENGTH];
  int found;                    // used to check if used typed word on screen
  time_t startTime; //using <time.h>
  time_t endTime;
  int timeElaspedTyping, numIterations =0, numLinesToGenerate;
  srand(time(NULL));

  while (!lose) {
    numIterations++;

    mvprintw((GRIDY + 2), 0, "Type Here: "); // indicates where to type
    startTime = time(NULL);
    scanw("%s", wordTyped);  
    endTime = time(NULL);
    timeElaspedTyping = endTime - startTime;
    numLinesToGenerate = getLinesGenerated(timeElaspedTyping, numIterations);

    for (i = 0; i < numLinesToGenerate; i++) {
      // Game logic here
      // shift if total words on screen is greater than 0
      if (numWordsInPlay > 0) {
        // shift all elments right 1
        for (int i = numWordsInPlay; i > 0; i--) {
          strcpy(wordsInPlay[i].word, wordsInPlay[i - 1].word);
          wordsInPlay[i].x = wordsInPlay[i - 1].x;
          wordsInPlay[i].y = wordsInPlay[i - 1].y;
        }
      }
      // chose random word and set strut word memeber to this
      strcpy(wordsInPlay[0].word, pickRandom(minLenthWL, tempIndex));
      // set y cord to top of board
      wordsInPlay[0].y = 2;
      // randomly generate place for x-cord with word length in mind
      wordsInPlay[0].x =
          rand() % ((GRIDX)-strlen(wordsInPlay[0].word)) + 2; // 78-strlen

      for (int i = 1; i <= numWordsInPlay; i++) {
        // update all words to move down cords by one
        wordsInPlay[i].y = wordsInPlay[i].y + 1;
      }
      numWordsInPlay++;
    }
    //										screen
    // clear
    



    /*			Implement logic for succesful word typed
     */
    found = -1;
	int place = 0;
    for (int j = numWordsInPlay - 1; j > 0; j--, place++) {
		mvprintw((GRIDY + 5+place), 5, "%d  %d   %s", j,strcmp(wordsInPlay[j].word, wordTyped),wordsInPlay[j].word); 
      if (strcasecmp(wordsInPlay[j].word, wordTyped) == 13) {   
        found = j; // index where word that has been typed is found
		mvprintw((GRIDY + 6), 0, "d: %d", j);
		break; 
      }
    }

    // shift struct array left starting at found to effectively delete
    if (found != -1) {
      for (int i = found; i < numWordsInPlay;
           i++) { // numWordsInPlay was wordsInPlay
        strcpy(wordsInPlay[i].word, wordsInPlay[i + 1].word);
        wordsInPlay[i].x = wordsInPlay[i + 1].x;
        wordsInPlay[i].y = wordsInPlay[i + 1].y;
      }
      numWordsInPlay--;
    }

	clear();
    printBoard();
    for (int i = 0; i < numWordsInPlay; i++) {
      mvprintw(wordsInPlay[i].y, wordsInPlay[i].x, "%s", wordsInPlay[i].word);
      if(wordsInPlay[i].y >= GRIDY)
      lose = 1;
    }
	mvprintw((GRIDY + 4), 0, "Past Word: %s", wordTyped); 

    refresh();
  }
  // NOT FINNISHED
  clear();
  endwin();
  printf("Game Over!\n");
  return 0;
}

/*		User Defined Functions' Definition		 */
void draw_character(int y, int x, char use) {
  mvaddch(y, x, use);
  refresh();
}

// pick random word
const char *pickRandom(char words[][WORDLENGTH], int numWords) {
  int elementPick = rand() % numWords;
  return words[elementPick];
}

void printBoard() {
  for (int row = 1; row <= GRIDY; row++) {
    for (int col = 1; col <= GRIDX; col++) {
      if (row == 1) {
        mvprintw(row, col, "_");
      } else if (row == GRIDY) {
        mvprintw(row, col, "_");
        mvprintw(row + 1, col, "_");
      } else if (col == 1) {
        mvprintw(row, col, "|");
      } else if (col == GRIDX) {
        mvprintw(row, col, "|");
      } else {
        mvprintw(row, col, " ");
      }
    }
  }
}

int getLinesGenerated(int time, int numCalled) {
  int temp;
  if (numCalled < 10) { //10 words typed
    temp = 0;
  }
  else if (numCalled < 20) { //20 words typed "level 2"
    temp = 1;
  }
  else if (numCalled < 30) {  //30 words
    temp = 2;
  }
  else {
    temp = 4;
  }
  if (time <= 1) { // 1 second or less
    return 1 + temp;
  } 
  else if (time < 2) {//2 seconds or less
    return 2 + temp;
  }
  else if (time < 3) { //3 seconds or less
    return 3 + temp;
  }
  return 4 + temp; //4 seconds or less

}