#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int vars[26] = {0}; // initialize variables A-Z to 0
char commands[100][100]; // array to store commands

int getValue(char arg[])
{
    if (arg[0] >= 'A' && arg[0] <= 'Z') // check if the argument is a variable (A-Z)
    {
        return vars[arg[0] - 'A'];
    }

    return atoi(arg); // convert the argument to an integer and return it
}

void calc(char *command, char *arg, int b)
{
    int a = getValue(arg);
    if(strcmp(command, "ADD") == 0)
        {
            vars[arg[0] - 'A'] = a + b;
            printf("%d\n", a + b);
        }
    else if(strcmp(command, "SUB") == 0)
        {
            vars[arg[0] - 'A'] = a - b;
            printf("%d\n", a - b);
        }
    else if(strcmp(command, "MUL") == 0)
        {
            vars[arg[0] - 'A'] = a * b;
            printf("%d\n", a * b);
        }
    else if(strcmp(command, "DIV") == 0)
        {
            vars[arg[0] - 'A'] = a / b;
            printf("%.2f\n", (double)a / b);
        }
    else if(strcmp(command, "MOD") == 0)
        {
            vars[arg[0] - 'A'] = a % b;
            printf("%d\n", a % b);
        }
}

void load(char reg, int val)
{
    vars[reg - 'A'] = val; // store the value in the corresponding reg
    printf("%d LOADED TO %c\n", val, reg);
}

void print(char text)
{
    if (text >= 'A' && text <= 'Z')
    {
        printf("%d\n", vars[text - 'A']); // print the value of the variable
    }
}

void printstr(char *text)
{
    printf("%s\n", text); // print the string
}

int evalSign(int a, char *sign, int b)
{
    if (strcmp(sign, ">") == 0)  return a > b; // evaluate the condition based on the sign
    if (strcmp(sign, "<") == 0)  return a < b;
    if (strcmp(sign, "==") == 0) return a == b;
    if (strcmp(sign, "!=") == 0) return a != b;
    return 0;
}

int condition(int a, char *sign, int b, char *op, int c, char *sign2, int d)
{
    int left = evalSign(a, sign, b); // evaluate the left side of the condition
    int right = evalSign(c, sign2, d); // evaluate the right side of the condition
    int result;

    if (strcmp(op, "") == 0) // if there is no operator, return the left side result
    {
        result = left;
    }
    else if (strcmp(op, "NOT") == 0)
    {
        result = !left && !right;
    }
    else if (strcmp(op, "AND") == 0)
    {
        result = left && right;
    }
    else if (strcmp(op, "OR") == 0)
    {
        result = left || right;
    }

    printf(result ? "True\n" : "False\n"); // print the result of the condition
    return result;
}

int main()
{

    char input[100];
    char command[20];
    int PC = 0; //keep track of the current command being executed
    int lines = 0; // count the number of lines of commands

    while (fgets(commands[lines], 100, stdin)) // read commands from input until END
    {
        commands[lines][strcspn(commands[lines], "\n")] = '\0'; // remove newline from the command

        if (strcmp(commands[lines], "END") == 0)
        {
            break; // stop reading commands after END
        }
        lines++;
    }


    for (PC = 0; PC < lines; PC++)
    {
        sscanf(commands[PC], "%s", command);

        if (strcmp(command, "IF") == 0)
        {
            char arg1[10] = "", arg2[10] = "";
            char arg3[10] = "", arg4[10] = "";
            char sign[10] = "", sign2[10] = "";
            char operator[100] = "";
            int match = sscanf(commands[PC], "%s %s %s %s %s %s %s %s", command, arg1, sign, arg2, operator, arg3, sign2, arg4);
            int a = getValue(arg1);
            int b = getValue(arg2);
            int c = 0, d = 0;
            if (match > 5) // if there is a second condition, get the values of arg3 and arg4
            {
                c = getValue(arg3);
                d = getValue(arg4);
            }
            else // if there is no second condition, set the operator to an empty string
                operator[0] = '\0';
            if (!condition(a, sign, b, operator, c, sign2, d))
            {
                int i = 1;
                while (i > 0 && PC < lines - 1) // skip to ENDIF if the condition is false
                {
                    PC++;
                    char line[20];
                    sscanf(commands[PC], "%s", line);

                    if (strcmp(line, "IF") == 0)
                        i++;
                    else if (strcmp(line, "ENDIF") == 0)
                        i--;
                }
            }
        }
        else if (strcmp(command, "ENDIF") == 0)
        {
            if (PC + 1 < lines) // check if there is a next line after ENDIF
            {
                char nextLine[20];
                sscanf(commands[PC + 1], "%s", nextLine);
                if (strcmp(nextLine, "ELSE") == 0)
                {
                    int j = 1;
                    while (j > 0 && PC < lines - 1) // skip to ENDELSE if there is an ELSE after ENDIF
                    {
                        PC++;
                        char line[20];
                        sscanf(commands[PC], "%s", line);
                        if (strcmp(line, "ELSE") == 0)
                            j++;
                        else if (strcmp(line, "ENDELSE") == 0)
                            j--;
                    }
                }
            }
        }
        
        else if (strcmp(command, "LOAD") == 0)
        {
            char var;
            int val;
            sscanf(commands[PC], "%s %c%d", command, &var, &val);
            load(var, val);
        }
        else if (strcmp(command, "PRINT") == 0)
        {
            char var;
            sscanf(commands[PC], "%s %c", command, &var);
            print(var);
        }
        else if (strcmp(command, "PRINTSTR") == 0)
        {
            char str[100];
            sscanf(commands[PC], "%s %s", command, str);
            printstr(str);
        }
        else if (strcmp(command, "ADD") == 0
            || strcmp(command, "SUB") == 0
            || strcmp(command, "MUL") == 0
            || strcmp(command, "DIV") == 0
            || strcmp(command, "MOD") == 0)
        {
            char arg1[10], arg2[10];
            sscanf(commands[PC], "%s %s %s", command, arg1, arg2);
        
            int b = getValue(arg2);
            calc(command, arg1, b);
        }
    }

    return 0;
}