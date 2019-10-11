#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFSIZE 64
#define TOKENDELIMITER " \t\r\n\a"
#define READLINEBUFSIZE 1024

int cdCommnand(char **args);
int helpCommand(char **args);
int exitCommand(char **args);

char *buildinStrings[] = {
  "cd",
  "help",
  "finalizar"
};

int (*buildinCommands[]) (char **) = {
  &cdCommnand,
  &helpCommand,
  &exitCommand
};

int buildinNums() {
  return sizeof(buildinStrings) / sizeof(char *);
}

int cdCommnand(char **args)
{
  if (args[1] == NULL) {
    fprintf(stderr, "ERROR: um argumento é esperado \"cd\"\n");
  } else {
    if (chdir(args[1]) != 0) {
      perror("lsh");
    }
  }
  return 1;
}

int helpCommand(char **args)
{
  int i;
  printf("SHELL EM C\n");
  printf("FEITO POR HIGOR ALVE E THIAGO SOUTO.\n");

  printf("PARA SAIR DO PROGRAMA BASTA DIGITAR finalizar.\n");
  return 1;
}

int exitCommand(char **args)
{
  return 0;
}

int lauch(char **args)
{
  pid_t pid;
  int status;

  pid = fork();
  if (pid == 0) {
    if (execvp(args[0], args) == -1) {
      perror("lsh");
    }
    exit(EXIT_FAILURE);
  } else if (pid < 0) {
    perror("lsh");
  } else {
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }

  return 1;
}

int executeProgram(char **args)
{
  int i;

  if (args[0] == NULL) {
    return 1;
  }

  for (i = 0; i < buildinNums(); i++) {
    if (strcmp(args[0], buildinStrings[i]) == 0) {
      return (*buildinCommands[i])(args);
    }
  }

  return lauch(args);
}


char *readLine(void)
{
  int bufsize = READLINEBUFSIZE;
  int position = 0;
  char *buffer = malloc(sizeof(char) * bufsize);
  int c;

  if (!buffer) {
    fprintf(stderr, "ERROR: Nçao foi possivel alocar\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    c = getchar();

    if (c == EOF) {
      exit(EXIT_SUCCESS);
    } else if (c == '\n') {
      buffer[position] = '\0';
      return buffer;
    } else {
      buffer[position] = c;
    }
    position++;

    if (position >= bufsize) {
      bufsize += READLINEBUFSIZE;
      buffer = realloc(buffer, bufsize);
      if (!buffer) {
        fprintf(stderr, "ERROR: Nçao foi possivel alocar\n");
        exit(EXIT_FAILURE);
      }
    }
  }
}

char **splitLine(char *line)
{
  int bufsize = BUFSIZE, position = 0;
  char **tokens = malloc(bufsize * sizeof(char*));
  char *token, **tokens_backup;

  if (!tokens) {
    fprintf(stderr, "ERROR: Erro na alocação\n");
    exit(EXIT_FAILURE);
  }

  token = strtok(line, TOKENDELIMITER);
  while (token != NULL) {
    tokens[position] = token;
    position++;

    if (position >= bufsize) {
      bufsize += BUFSIZE;
      tokens_backup = tokens;
      tokens = realloc(tokens, bufsize * sizeof(char*));
      if (!tokens) {
		free(tokens_backup);
        fprintf(stderr, "ERROR: Erro na alocação\n");
        exit(EXIT_FAILURE);
      }
    }

    token = strtok(NULL, TOKENDELIMITER);
  }
  tokens[position] = NULL;
  return tokens;
}

void shellLoop(void)
{
  char *line;
  char **args;
  int status;

  do {
    printf("Insira seu comando: ");
    line = readLine();
    args = splitLine(line);
    status = executeProgram(args);

    free(line);
    free(args);
  } while (status);
}