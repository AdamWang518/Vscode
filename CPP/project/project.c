#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char *shell_read_line()
{
    char *line = NULL;
    ssize_t bufsize = 0; // 利用 getline 來自動分配緩衝區，將輸入放入line
    getline(&line, &bufsize, stdin);
    return line;
};
char **shell_split_line(char *line){
    int bufsize = TOKEN_BUFSIZE, position = 0;

};
int shell_execute(char **args)
{

};
int shell_launch(char **args)
{

};
void shell_loop()
{
    char *line;
    char **args;
    int status = 1;
    while (status)
    {
        printf("B0829039's Shell> ");
        line = shell_read_line();
        args = shell_split_line(line);
        status = shell_execute(args);
        free(line);
        free(args);
    }
};
int main()
{
    shell_loop();
    return 0;
}