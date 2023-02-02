#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef long long           ll;
typedef long double         ld;

#define F               first
#define S               second
#define INVALID         printf("Invalid Command\n")
#define N               (int)1e2 + 10
#define FLUSH           fflush(stdout)
#define SZ(x)           (int)strlen(x)

char Input[N], CWD[N];

/// Basic Math Functions

int Min(int a, int b)
{
    if(a <= b) return a;
    return b;
}

int Max(int a, int b)
{
    if(a <= b) return b;
    return a;
}

/// Errors

void Ass(int condition)
{
    if(condition == 0)
    {
        printf("Assert is fucked up!");
    }
}

/// String crops && useful functions

int E(char* a, char* b)
{
    return (strcmp(a, b) == 0);
}

char* PRE(int x, char* s)
{
    Ass(x <= strlen(s));
    char* ret = calloc(x + 1, sizeof (char));
    for(int i = 0; i < x; i ++)
    {
        ret[i] = s[i];
    }
    return ret;
}

char* SUF(int x, char* s)
{
    Ass(x <= SZ(s));
    char* ret = calloc(x + 1, sizeof (char));
    for(int i = 0; i < x; i ++)
    {
        ret[i] = s[SZ(s) - x + i];
    }
    return ret;
}

char* BAZE(int l, int r, char* s)
{
    Ass(r < strlen(s) && l <= r);
    char* ret = calloc(r - l + 2, sizeof (char));
    for(int i = l; i <= r; i ++)
    {
        ret[i - l] = s[i];
    }
    return ret;
}

char* FIX(char* s)
{
    int n = SZ(s), l = 0, r = n - 1;
    while(l < n && s[l] == ' ')
    {
        l ++;
    }
    while(r >= 0 && s[r] == ' ')
    {
        r --;
    }
    if(l > r) return NULL;
    Ass(l < n && r >= 0 && l <= r);
    if(s[l] == '"') l ++;
    if(s[r] == '"') r --;
    /// printf("in fix : l = %d r = %d ret = %s\n", l, r, BAZE(l, r, s));
    return BAZE(l, r, s);
}

int TO_INT(char* s)
{
    int n = SZ(s);
    int ret = 0;
    for(int i = 0; i < n; i ++)
    {
        Ass('0' <= s[i] && s[i] <= '9');
        ret *= 10;
        ret += (int)(s[i] - '0');
    }
    return ret;
}

/// global variables unkown to the basic functions above

int Main_sz;

char *cur, *command, *path, *str;

int x, y, dir, SIZE;

/// Working with Directory and CWD

int GET()
{
    getcwd(CWD, N);
    return strlen(CWD);
}

void print_cwd()
{
    GET();
    printf("%s\n", CWD);
}

void Go_Main()
{
    int sz = GET();
    while(sz > Main_sz)
    {
        /*print_cwd();
        FLUSH;*/
        chdir("..");
        sz = GET();
    }
}

void Go_Path(char* s)
{
    
}

int Valid(char* s)
{
    return (s != NULL && SZ(s) >= 5 && E(PRE(5, s), "/root"));
}

int exist_path(char* s)
{
    FILE *f = fopen(s, "r");
    if(f != NULL)
    {
        fclose(f);
        return 1;
    }
    return 0;
}

void createfile()
{

}

/// Digesting the input to known elements

int _file()
{
    if(SZ(cur) >= 4 && E(PRE(4, cur), "file"))
    {
        /*printf("Ive maid it\n");
        FLUSH;*/
        cur += 4;
        path = FIX(cur);
        return 1;
    }
    return 0;
}

int _str()
{
    if(SZ(cur) >= 3 && E(PRE(3, cur), "str"))
    {
        cur += 3;
        str = FIX(cur);
        return 1;
    }
    return 0;
}

int _pos()
{
    if(SZ(cur) >= 3 && E(PRE(3, cur), "pos"))
    {
        cur += 3;
        cur = FIX(cur);
        for(int i = 0; i < SZ(cur); i ++)
        {
            if(cur[i] == ':')
            {
                x = TO_INT(PRE(i, cur));
                y = TO_INT(SUF(SZ(cur) - i - 1, cur));
                return 1;
            }
        }
    }
    return 0;
}

int _size()
{
    if(SZ(cur) >= 4 && E(PRE(4, cur), "size"))
    {
        cur += 4;
        cur = FIX(cur);
        SIZE = TO_INT(cur);
        return 1;
    }
    return 0;
}

void Digest()
{
    cur = str = path = command = NULL;
    dir = x = y = SIZE = -1;
    cur = strtok(Input, "-");
    command = cur;
    command[SZ(cur) - 1] = '\0';
    cur = strtok(NULL, "-");
    while(cur != NULL)
    {
        /*printf("now cur is : %s\n", cur);
        FLUSH;*/
        if(!(_file() || _size() || _pos() || _str()))
        {
            INVALID;
            break;
        }
        cur = strtok(NULL, "-");
    }
    /*printf("path = %s str = %s command = %s x = %d y = %d SIZE = %d\n", path, str, command, x, y, SIZE);
    FLUSH;*/
}

int main()
{
    Main_sz = GET();
    printf("Ariya's VIM   Powered by ~Kc~\n");
    do
    {
        printf("> ");
        scanf("%[^\n]%*c", Input);
        /*printf("this is the input:\n%s\n", Input);
        FLUSH;*/
        Digest();
        if (E(command, "createfile"))     /// createfile -file <addresss/name>
        {
            createfile();
        }
        else if (E(command, "insertstr"))
        {
            // insertstr();
        } 
        else if (E(command, "cat"))
        {
            // cat();
        }
        else if (E(command, "removetstr"))
        {
            // removestr();
        }
        else if (E(command, "copy"))
        {
            // copystr();
        }
        else if (E(command, "cut"))
        {
            // cutstr();
        }
        else if (E(command, "pastestr"))
        {
            // pastestr();
        }
        else if (E(command, "find"))
        {
            // find();
        }
        else
        {
            // INVALID;
        }
    } while (strcmp(Input, "exit"));
    return 0;
}