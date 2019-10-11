#include "./shell.c"

int main(int argc, char **argv){
	char *line;
  char **args;
	int argc2;
  int status;

  do {
    printf("Insira seu comando: ");
    line = readLine();
    args = splitLine(&argc2,line);
    status = execute(argc2, args);

    free(line);
    free(args);
  } while (status);
  return EXIT_SUCCESS;
}