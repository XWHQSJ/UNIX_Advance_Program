#include "apue.h"

#define TOK_ADD 5

void do_line(char *);
void cmd_add(void);
int get_token(void);

int main(void)
{
    char line[MAXLINE];

    while(fgets(line, MAXLINE, stdin) != NULL)
    {
        do_line(line);
    }

    exit(0);
}

// global pointer for get_token()
char *tok_ptr;

// process one line of input
void do_line(char *ptr)
{
    int cmd;

    tok_ptr = ptr;
    while((cmd = get_token()) > 0)
    {
        // one case for each command
        switch (cmd)
        {
        case TOK_ADD:
            cmd_add();
            break;
        default:
            break;
        }
    }
}

void cmd_add(void)
{
    int token;

    token = get_token();

    /* rest of processing for this command */
}

int get_token(void)
{
    /* fetch next token from line pointed to by tok_ptr */
}