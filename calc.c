#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

#define PI 3.14159265
#define MAXLINE 1000
#define BUFSIZE 100
#define MAXVAL 100
#define MAXOP 100
#define NUMBER '0'
#define MATHLIB '1'
#define VARIABLE '2'

int getch(void);
void ungetch(int);
int getop(char []);
void push(double);
double pop(void);
void math(char s[]);

double vars[26];
double last;
int sp = 0;
double val[MAXVAL];
char buf[BUFSIZE];
int bufp = 0;

int main(void){
    int type, v;
    double op2;
    char s[MAXOP];

    while ((type = getop(s)) != EOF) {
        switch (type) {
        case NUMBER:
            push(atof(s));
            break;
        case MATHLIB:
            math(s);
            break;
        case '+':
            push(pop() + pop());
            break;
        case '*':
            push(pop() * pop());
            break;
        case '-':
            op2 = pop();
            push(pop() - op2);
            break;
        case '/':
            op2 = pop();
            if (op2 != 0.0)
                push(pop() / op2);
            else
                printf("error: zero divisor\n");
            break;
        case '%':
            op2 = pop();
            if (op2 != 0.0)
                push(fmod(pop(), op2));
            else
                printf("error: zero divisor\n");
            break;
        case VARIABLE:
            v = tolower(s[0]);
            last = vars[v - 'a'] = pop();
            push(last);
            break;
        case '~':
            printf("exit function~~\n");
            exit(0);
            break;
        case '\n':
            printf("= %g\n", pop());
            break;
        default:
            printf("error: unknown command %s\n", s);
        break;
        }
    }
    return 0;
}


void push(double f){
    if (sp < MAXVAL)
        val[sp++] = f;
    else
        printf("error: stack full, can't push %g\n", f);
}


double pop(void){
    if (sp > 0)
        return val[--sp];
    else {
        printf("error: stack empty\n");
        return 0.0;
    }
}


int getop(char s[]){
    int i =0, c, c2;

    while ((s[0] = c = getch()) == ' ' || c == '\t'){}
    s[1] = '\0';

    if (isalpha(c)) {
        while (isalpha(s[++i] = c = getch()))
            ;
        s[i] = '\0';
        if (c != EOF)
            ungetch(c);
        return MATHLIB;
    }

    if (!isdigit(c) && c != '.' && c != '-') {
        if (c == '=') {
            if (isalpha(c2 = getch())) {
                s[0] = c2;
                return VARIABLE;
            } else if (c2 != EOF) {
                ungetch(c2);
            }
        }
        return c;
    }

    if (c == '-') {
        c2 = getch();
        if (c2 != EOF)
            ungetch(c2);
        if (!isdigit(c2) && c2 != '.')
            return c;
    }

    if (isdigit(c) || c == '-')
        while (isdigit(s[++i] = c = getch()))
            ;
    if (c == '.')
        while (isdigit(s[++i] = c = getch()))
            ;
    s[i] = '\0';
    if (c != EOF)
        ungetch(c);
    return NUMBER;
}

int getch(void){
    return (bufp > 0) ? buf[--bufp] : getchar();
}

void ungetch(int c){
    if (bufp >= BUFSIZE)
        printf("ungetch: too many characters\n");
    else
        buf[bufp++] = c;
}

void math(char s[]){
    double op1,op2,result = 0;
    double pi = PI/180;
    
    if(strcmp(s,"pow")==0){
        op1 = pop();
        op2 = pop();
        result = pow(op1,op2);
    } else if( strcmp(s,"sin")==0){
        result = sin(pop()*pi);
    } else if( strcmp(s,"cos")==0){
        result = cos(pop()*pi);
    } else if( strcmp(s,"tan")==0){
        result = tan(pop()*pi);
    } else if( strcmp(s,"sqrt")==0){
        result = sqrt(pop());
    } else if( strcmp(s,"last")==0){
        printf("the most recently printed value is %g\n", last);
    } else{
        printf("error: unknown command %s\n", s);
    }
    push(result);

}