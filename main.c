#include "./shell.c"

int main(int argc, char **argv){
	char *line;
  char **args;
  int status;

  do {
    printf("Insira seu comando: ");
    line = readLine();
    args = splitLine(line);
    status = execute(args);

    free(line);
    free(args);
  } while (status);
  return EXIT_SUCCESS;
}