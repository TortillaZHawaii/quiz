#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define BUTTONS_COUNT 4

typedef struct question {
  char* question;
  char** answers;
  int correctAnswer;
} question;

void printQuestion(question* question) 
{
  printf("%s\n", question->question);
  for (int i = 0; i < 4; i++) {
    printf("SW%d) %s\n", i + 1, question->answers[i]);
  }
}

void blink(struct gpiod_line* line, int seconds)
{
  gpiod_line_set_value(line, 1);
  sleep(seconds);
  gpiod_line_set_value(line, 0);
}

void signalCorrectAnswer(struct gpiod_line* lineGreen) 
{
  printf("Correct!\n");
  blink(lineGreen, 1);
}

void signalWrongAnswer(struct gpiod_line* lineRed) 
{
  printf("Wrong!\n");
  blink(lineRed, 1);
}

int getAnswer(struct gpiod_line_bulk* lineButtons)
{

  int pressedButton = -1;
  int rv;
  do
  {
    struct gpiod_line_bulk event;
    rv = gpiod_line_event_wait_bulk(lineButtons, NULL, &event);
    
    if (rv == -1) 
    {
      printf("Error waiting for event\n");
      exit(EXIT_FAILURE);
    }

    if (rv >= 1)
    {
      for(int i = 0; i < lineButtons->num_lines; ++i)
      {
        if(event.lines[0] == lineButtons->lines[i])
        {
          pressedButton = i;
          break;
        }
      }
    }
  } 
  while (rv == 0);

  return pressedButton;
}

void playQuiz(struct gpiod_line* lineRed, 
  struct gpiod_line* lineGreen, struct gpiod_line_bulk* lineButtons,
  question** questions, int questionsCount)
{
  int score = 0;
  
  printf("Staring quiz, press the button to answer\n");

  for(int currentQuestion = 0; currentQuestion < questionsCount; ++currentQuestion)
  {
    printQuestion(questions[currentQuestion]);
    
    int pressedButton = getAnswer(lineButtons);
    
    int isCorrect = questions[currentQuestion]->correctAnswer == pressedButton;
    if (isCorrect)
    {
      ++score;
      signalCorrectAnswer(lineGreen);
    } 
    else
    {
      signalWrongAnswer(lineRed);
    }
  }

  printf("You scored %d out of %d\n", score, questionsCount);
}


int main(int argc, char **argv)
{
  const char *chipname = "gpiochip0";
  struct gpiod_chip *chip;
  struct gpiod_line *lineRed;    // Red LED
  struct gpiod_line *lineGreen;  // Green LED
  struct gpiod_line_bulk* lineButtons = NULL; // Pushbuttons

  // Open GPIO chip
  chip = gpiod_chip_open_by_name(chipname);

  // Open GPIO lines
  // MAKE SURE TO CHANGE GPIO NUMBERS IN YOUR CODE
  lineRed = gpiod_chip_get_line(chip, 27);
  if(lineRed == NULL)
  {
    fprintf(stderr, "Error opening line 27\n");
    exit(EXIT_FAILURE);
  }

  lineGreen = gpiod_chip_get_line(chip, 22);
  if(lineGreen == NULL)
  {
    fprintf(stderr, "Error opening line 22\n");
    exit(EXIT_FAILURE);
  }

  unsigned int buttonOffsets[] = {18, 17, 10, 25};
  if(gpiod_chip_get_lines(chip, buttonOffsets, BUTTONS_COUNT, lineButtons) < 0)
  {
    fprintf(stderr, "Error opening GPIO button lines\n");
    exit(EXIT_FAILURE);
  }
  
  // Open LED lines for output
  gpiod_line_request_output(lineRed, "quiz", 0);
  gpiod_line_request_output(lineGreen, "quiz", 0);

  // Open button lines for falling edge input
  gpiod_line_request_bulk_falling_edge_events(lineButtons, "quiz");

  question question1 = {
    "What is the capital of France?",
    (char*[]){"Paris", "London", "Berlin", "Madrid"},
    0
  };
  question question2 = {
    "What is the capital of Germany?",
    (char*[]){"Paris", "London", "Berlin", "Madrid"},
    2
  };
  question question3 = {
    "What is the capital of Spain?",
    (char*[]){"Paris", "London", "Berlin", "Madrid"},
    3
  };
  question* questions[] = {&question1, &question2, &question3};

  playQuiz(lineRed, lineGreen, lineButtons, questions, 3);

  // Release lines and chip
  gpiod_line_release(lineRed);
  gpiod_line_release(lineGreen);
  gpiod_line_release_bulk(lineButtons);
  gpiod_chip_close(chip);
  return 0;
}