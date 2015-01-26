
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAXTOKENS 100
#define MAXTOKENLEN 64

enum type_tag
{
    IDENTIFIER, QUALIFIER, TYPE
};
struct token
{
    char type;
    char string[MAXTOKENLEN];
};

int top = -1;
/*�ڵ�һ����ʶ����identiier��ǰ�������еı�ǣ�token��*/
struct token stack[MAXTOKENS];
/*����ն���ı��*/
struct token this;
#define pop stack[top--]
#define push(s) stack[++top]=s

enum type_tag
classify_string(void)
/*�жϱ�ʶ��������*/
{
    char *s = this.string;
    if(!strcmp(s, "const"))
    {
        strcpy(s, "read-only");
        return QUALIFIER;
    }
    if(!strcmp(s, "volatile"))
    {
        return QUALIFIER;
    }
    if(!strcmp(s, "void"))
    {
        return TYPE;
    }
    if(!strcmp(s, "char"))
    {
        return TYPE;
    }
    if(!strcmp(s, "signed"))
    {
        return TYPE;
    }
    if(!strcmp(s, "unsigned"))
    {
        return TYPE;
    }
    if(!strcmp(s, "short"))
    {
        return TYPE;
    }
    if(!strcmp(s, "int"))
    {
        return TYPE;
    }
    if(!strcmp(s, "long"))
    {
        return TYPE;
    }
    if(!strcmp(s, "float"))
    {
        return TYPE;
    }
    if(!strcmp(s, "double"))
    {
        return TYPE;
    }
    if(!strcmp(s, "struct"))
    {
        return TYPE;
    }
    if(!strcmp(s, "union"))
    {
        return TYPE;
    }
    if(!strcmp(s, "enum"))
    {
        return TYPE;
    }
    return IDENTIFIER;
};

void gettoken(void)
{
    /*������һ����ǣ�������"this"��*/
    char *p = this.string;
    /*�Թ����пհ��ַ�*/
    while(' ' == (*p = getchar()));
    if(isalnum(*p))
    {
        /*�ڱ�ʶ���ж���A-Z��1-9�ַ�*/
        while(isalnum(*++p = getchar()));
        ungetc(*p, stdin);
        *p = '\0';
        this.type = classify_string();
        //printf("%s %d\n", this.string, this.type);
        return;
    }
    this.string[1] = '\0';
    this.type = *p;
    return;
}

void initialize(), get_array(), get_params(),
get_lparen(), get_ptr_part(), get_type();
void (*nextstate)(void) = initialize;

int main()
/*������״̬��ʵ�ֵ�Cdecl*/
{
/*�ڲ�ͬ��״̬��ת����ֱ��ָ��Ϊnull*/
    while(nextstate != NULL)
    {
        (*nextstate)();
    }
    return 0;
}

void initialize()
{
 //   printf("initialize\n");
    gettoken();
    while(this.type != IDENTIFIER)
    {
        push(this);
        gettoken();
    }
    printf("%s is ", this.string);
    gettoken();
    nextstate = get_array;
}

void get_array()
{
  //  printf("get_array\n");
    nextstate = get_params;
    while('[' == this.type)
    {
        printf("array ");
        gettoken();/*һ�����ֻ�']'*/
        if(isdigit(this.string[0]))
        {
            printf("0..%d ", atoi(this.string)-1);
            gettoken();/*��ȡ']'*/
        }
        gettoken();
        printf("of ");
        nextstate = get_lparen;
    }
}

void get_params()
{
    //printf("get_params\n");
    nextstate = get_lparen;
    if('(' == this.type)
    {
        while(this.type != ')')
        {
            gettoken();
        }
        gettoken();
        printf("function returning ");
    }
}

void get_lparen()
{
    //printf("get_lparen\n");
    nextstate = get_ptr_part;
    if(top >= 0)
    {
        if('(' == stack[top].type)
        {
            pop;
            gettoken();
            nextstate = get_array;
        }
    }
}

void get_ptr_part()
{
    //printf("get_ptr_part\n");
    nextstate = get_type;
    if('*' == stack[top].type)
    {
        printf("pointer to ");
        pop;
        nextstate = get_lparen;
    }
    else if(stack[top].type == QUALIFIER)
    {
        printf("%s ", pop.string);
        nextstate = get_lparen;
    }
}

void get_type()
{
    //printf("get_type\n");
    nextstate = NULL;
    /*�����ڶԱ�ʶ��֮ǰ�����ڶ�ջ������б��*/
    while(top >= 0)
    {
        printf("%s ", pop.string);
    }
    printf("\n");
}
