/* wordle.txt */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ResultGreen        1
#define ResultYellow       2
#define ResultRed          4
#define max                5195

typedef char Result;

struct s_words {
  char **arr;
  int n;
};
typedef struct s_words Words;

bool isin(char, char *);
void Example_print_results(Result *);
Result *cw(char *, char *);
Result cc(char, int, char *);
int main(int, char **);

// * Function to print result
void Example_print_results(Result* res) {
  int i;
  for (i = 0; i < 5; i++) {
    switch (res[i])
    {
    case ResultGreen:
      printf("%s\n", "Green");
      break;
    case ResultYellow:
      printf("%s\n", "Yellow");
      break;
    case ResultRed:
      printf("%s\n", "Red");
      break;
    default:
      printf("Unknown: %d", res[i]);
      break;
    }    
  }
}

// * checks if a character is in the given string
bool isin(char c, char *str) {
  bool ret;
  int i, size;

  ret = false;
  size = strlen(str);
  for (i = 0; i < size; i++) {
    if(str[i] == c) {
      ret = true;
      break;
    }
  }

  return ret;
}

// * check char (cc)
Result cc(char guess, int idx, char *word) {
  char correct;
  correct = word[idx];
  if(guess == correct) {
    return ResultGreen;
  }
  else if(isin(guess, word)) {
    return ResultYellow;
  }
  return ResultRed;
}

Words readFile(char *filename) {
  char buf[8];
  int i, size;
  FILE *fd;
  // char **ret;
  static char ret[max][5];

  fd = fopen(filename, "r");
  if(!fd) {
    perror("fopen");
    Words words = {
      .arr = (char **)0,
      .n = 0
    };
    return words;
  }

  size = max * 5;

  i = 0;
  memset(buf, 0, 8);
  while(fgets(buf, 7, fd)) {
    size = strlen(buf);
    if(size < 1) {
      memset(buf, 0, 8);
      continue;
    }
    // * nylon\n\0
    // * 123456
    // * strlen => 6
    size--;
    buf[size] = 0;

    if(size != 5) {
      memset(buf, 0, 8);
      continue;
    }

    ret[i][0] = buf[0];
    ret[i][1] = buf[1];
    ret[i][2] = buf[2];
    ret[i][3] = buf[3];
    ret[i][4] = buf[4];

    memset(buf, 0, 8);
    i++;
    if(max <= i) {
      break;
    }
  }

  fclose(fd);
  Words words = {
    .arr = (char **)&ret,
    .n = i
  };

  return words;

}

// * check word (cw)
Result* cw(char *guess, char *word) {
  static Result res[5];
  int i;

  for (i = 0; i < 5; i++ ) {
    res[i] = cc(guess[i], i, word);
  }

  return res;
}

int main(int argc, char *argv[]) {
  char *correct, *guess;
  Result *res;

  if(argc < 3) {
    fprintf(stderr, "Usage: %s CORRECTWORD GUESSWORD\n", argv[0]);
    return -1;
  }

  correct = argv[1];
  guess = argv[2];

  res = cw(guess, correct);
  Example_print_results(res);

  return 0;
}

// * Run the code 
// * gcc -Wall wordle.c -o wordle && ./wordle


/*
struct - blueprint for user defined data structure
*/