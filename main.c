#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define max 100

typedef union
{
    char cData;
    float fData;
} Item;

typedef struct
{
    Item items[max];
    int top;
} stack;

stack* initialize()
{
    stack *s=malloc(sizeof(stack));
    s->top=0;
    return s;
}

void push(stack *s, Item value)
{
    s->items[s->top++] = value;
}

Item pop(stack *s)
{
    s->top--;
    return s->items[s->top];
}

Item top(stack *s)
{
    return s->items[s->top-1];
}

int isEmpty(stack *s)
{
    if (s->top == 0)
        return 1;
    else
        return 0;
}

int myIsDigit(char *token)
{
    int i;
    for(i=0; i<strlen(token); i++)
    {
        if(isdigit(token[i]))
            return 1;
    }
    return 0;
}

int priority(char op)
{
    switch(op)
    {
    case '(':
        return 4;
        break;
    case '^':
        return 3;
    case '/':
    case '*':
    case '%':
        return 2;
        break;
    case '+':
    case '-':
        return 1;
        break;
    }
}

float evaluate(float x, float y, char op)
{
    switch(op)
    {
    case '+':
        return x+y;
        break;
    case '-':
        return x-y;
        break;
    case '*':
        return x*y;
        break;
    case '/':
        return x/y;
        break;
    case '^':
        return pow(x,y);
    }
}
void infixToPostfix(char* infix, char* postfix)
{
    postfix[0]= '\0';
    stack *s = initialize();
    char *token = strtok(infix, " ");
    while(token!=NULL)
    {
        if(myIsDigit(token))
        {
            strcat(postfix, token);
            strcat(postfix, " ");
        }
        else if(isEmpty(s))
        {
            Item x;
            x.cData = token[0];
            push(s, x);
        }
        else if((top(s).cData == '(')&&( token[0]!= ')'))
        {
            Item x;
            x.cData = token[0];
            push(s, x);
        }
        else if (token[0] == ')')
        {
            while(top(s).cData != '(')
            {
                Item x = pop(s);
                strncat(postfix, &x.cData, 1);
                strcat(postfix, " ");
            }
            pop(s);
        }
        else
        {
            while(priority(top(s).cData)>=priority(token[0]) && (top(s).cData) != '(' && !isEmpty(s))
            {
                Item x = pop(s);
                strncat(postfix, &x.cData, 1);
                strcat(postfix, " ");
            }
            Item x;
            x.cData = token[0];
            push(s, x);
        }
        token = strtok(NULL, " ");
    }
    while(!isEmpty(s))
    {
        Item x = pop(s);
        strncat(postfix, &x.cData, 1);
        strcat(postfix, " ");
    }
}

float evaluatePostfix(char *postfix)
{
    stack *s = initialize();
    char *token = strtok(postfix, " ");
    while(token!=NULL)
    {
        if(myIsDigit(token))
        {
            float x = atof(token);
            Item y;
            y.fData = x;
            push(s, y);
        }
        else{
            float x = pop(s).fData;
            float y = pop(s).fData;
            float result = evaluate(y, x, token[0]);
            Item z;
            z.fData = result;
            push(s, z);
        }
        token = strtok(NULL, " ");
    }
    float result = pop(s).fData;
    return result;
    }

void replaceNewLineBySpace(char *s)
{
    char *s1 = s;
    while((s1 = strstr(s1, "\n")) != NULL)
        *s1 = ' ';
}

int main(int argc, char**argv)
{
    char infixExpr[256] = "";
    char postfixExpr[256] = "";
    printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    while(fgets(infixExpr, 255, stdin) != NULL)
    {
        replaceNewLineBySpace(infixExpr);
        infixToPostfix(infixExpr, postfixExpr);
        printf("Postfix : %s\n", postfixExpr);
        float result = evaluatePostfix(postfixExpr);
        printf("Result: %.2f\n\n", result);
        printf("Enter an expression you want to evaluate or Ctrl+Z to exit: ");
    }
}
