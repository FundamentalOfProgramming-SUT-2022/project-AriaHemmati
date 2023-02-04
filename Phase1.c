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
#define N2              (int)1e3 + 10
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

char* FIX2(char* s)
{
    if(s[0] == '/')
    {
        return SUF(SZ(s) - 1, s);
    }
    return s;
}

char* CLONE(char* s)
{
    char* ret = calloc(SZ(s) + 6, sizeof (char));
    strcat(ret, "clone");
    strcat(ret, s);
    return ret;
}

char* STRFROM(int x, char c)
{
    char* ret = calloc(x + 1, sizeof (char));
    for(int i = 0; i < x; i ++)
    {
        ret[i] = c;
    }
    return ret;
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

void READ(char* s, char* S)
{
    FILE* f = fopen(S, "r");
    char c;
    while(1)
    {
        c = fgetc(f);
        if(c == EOF)
        {
            break;
        }
        s[SZ(s)] = c;
    }
    fclose(f);
}

/// global variables unkown to the basic functions above

int Main_sz;

char *cur, *command, *path, *str, *str2;

int at, all, byword, cnt, x, y, dir, SIZE;

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
    Go_Main();
    cur = strtok(s, "/");
    while(cur != NULL)
    {
        mkdir(cur, S_IRWXU);
        chdir(cur);
        cur = strtok(NULL, "/");
    }
    ///printf("Go path checking\n");
    ///print_cwd();
}

int Valid(char* s)
{
    return (s != NULL && SZ(s) >= 5 && E(PRE(5, s), "/root"));
}

int exist_path(char* s)
{    
    FILE *f;
    if((f = fopen(FIX2(s), "r")))
    {
        fclose(f);
        return 1;
    }
    ///printf("here s = %s\n", s);
    return 0;
}

void createfile()
{
    if(!Valid(path))
    {
        INVALID;
        return;
    }
    if(exist_path(path))
    {
        printf("this file already exists\n");
        return;
    }
    int ptr = SZ(path);
    for(; ptr >= 0; ptr --)
    {
        if(path[ptr] == '/')
        {
            break;
        }
    }
    char* DIR = PRE(ptr, path);
    char* DIR2 = PRE(ptr, path);
    char* NAME_OF_FILE = SUF(SZ(path) - ptr - 1, path);
    Go_Path(DIR);
    FILE *f = fopen(NAME_OF_FILE, "w+");
    fclose(f);
    Go_Main();
    Go_Path(CLONE(DIR2));
    FILE *f2 = fopen(NAME_OF_FILE, "w+");
    fclose(f2);
    Go_Main();
}

/// insert

int Index(char* s, int A, int B)
{
    int ptr = 0;
    while(ptr < SZ(s) && (A || B))
    {
        if(s[ptr] == '\n')
        {
            A --;
            Ass(A >= 0);
        }
        else if(A == 0)
        {
            B --;
        }
        ptr ++;
    }
    return ptr;
}

void insertstr()
{
    if(!Valid(path) || !exist_path(path) || x == -1 || y == -1 || str == NULL)
    {
        INVALID;
        return;
    }
    char s[N2] = {"\0"};
    char ret[N2] = {"\0"};
    READ(s, FIX2(path));
    FILE *cl = fopen(CLONE(path), "w");
    fputs(s, cl);
    fclose(cl);
    int Ind = Index(s, x - 1, y);
    strcat(ret, PRE(Ind, s));
    strcat(ret, str);
    strcat(ret, SUF(SZ(s) - Ind, s));
    FILE* f = fopen(FIX2(path), "w");
    fputs(ret, f);
    fclose(f);
}

/// cat

void cat()
{
    if(!Valid(path) || !exist_path(path))
    {
        INVALID;
        return;
    }
    char s[N2] = {"\0"};
    READ(s, FIX2(path));
    printf("%s\n", s);
}

/// removestr

void removestr()
{
    char* Path = FIX2(path);
    if(!Valid(path) || !exist_path(path) || x == -1 || y == -1 || dir == 0 || SIZE == -1)
    {
        INVALID;
        return;
    }
    if(SIZE == 0) return;
    char s[N2] = {"\0"};
    READ(s, Path);
    FILE *cl = fopen(CLONE(path), "w");
    fputs(s, cl);
    fclose(cl);
    int Ind = Index(s, x - 1, y);
    Ass(0 <= Ind + (SIZE - 1) * dir && Ind + (SIZE - 1) * dir < SZ(s));
    int l = Ind + (SIZE - 1) * dir, r = Ind;
    if(l > r) /// swap
    {
        int mid = r;
        r = l;
        l = mid;
    }
    char ret[N2] = {"\0"};
    strcat(ret, PRE(l, s));
    strcat(ret, SUF(SZ(s) - r - 1, s));
    FILE* f = fopen(Path, "w");
    fputs(ret, f);
    fclose(f);
}

/// copy paste cut

void copystr()
{
    char* Path = FIX2(path);
    if(!Valid(path) || !exist_path(path) || x == -1 || y == -1 || dir == 0 || SIZE == -1)
    {
        INVALID;
        return;
    }
    char s[N2] = {"\0"};
    READ(s, Path);
    int Ind = Index(s, x - 1, y);
    Ass(0 <= Ind + (SIZE - 1) * dir && Ind + (SIZE - 1) * dir < SZ(s));
    int l = Ind + (SIZE - 1) * dir, r = Ind;
    if(l > r) /// swap
    {
        int mid = r;
        r = l;
        l = mid;
    }
    char* ret = BAZE(l, r, s);
    FILE *f = fopen("Clip.txt", "w");
    fputs(ret, f);
    fclose(f);
}

void cutstr()
{
    copystr();
    removestr();
}

void pastestr()
{
    char* Path = FIX2(path);
    if(!Valid(path) || !exist_path(path) || x == -1 || y == -1)
    {
        INVALID;
        return;
    }
    char* s = calloc(N2, sizeof (char));
    READ(s, "Clip.txt");
    str = s;
    insertstr();
}

/// find, replace, undo

void find()
{
    char* Path = FIX2(path);
    if(!(Valid(path) || !exist_path(path) || str == NULL))
    {
        INVALID;
        return;
    }
    ///printf("just checking all = %d at = %d cnt = %d byword = %d str = %s\n", all, at, cnt, byword, str);
    char s[N2] = {"\0"};
    READ(s, Path);
    int arr[N] = {0};
    int ans[N] = {-1};
    int ptr = 0, n = SZ(str);
    for(int i = 1; i < n; i ++)
    {
        arr[i] = arr[i - 1];
        while(arr[i] && str[arr[i]] != str[i])
        {
            arr[i] = arr[arr[i] - 1];
        }
        arr[i] += (int)(str[arr[i]] == str[i]);
    }
    int Words = 0, flag = 0, j = 0;
    for(int i = 0; i < SZ(s); i ++)
    {
        while(j && (str[j] != s[i]))
        {
            j = arr[j - 1];
        }
        j += (int)(str[j] == s[i]);
        if(j == SZ(str))
        {
            ans[ptr ++] = (byword ? Words : i);
            j = 0;
        }
        if(s[i] != ' ' && s[i] != '\n')
        {
            flag = 1;
        }
        else
        {
            if(flag) Words ++;
            flag = 0;
        }
    }
    if(cnt)
    {
        printf("Count : %d\n", ptr);
    }
    if(at)
    {
        if(at > ptr)
        {
            printf("not enough matches :(  -1\n");
        }
        else
        {
            printf("%d\n", ans[at - 1]);
        }
        return;
    }
    if(all)
    {
        for(int i = 0; i < ptr; i ++)
        {
            printf("%d", ans[i]);
            if(i + 1 != ptr)
            {
                printf(", ");
            }
        }
        printf("\n");
    }
    return;
}

void replace()
{
    char* Path = FIX2(path);
    if(!(Valid(path) || !exist_path(path) || str == NULL || str2 == NULL))
    {
        INVALID;
        return;
    }
    char s[N2] = {"\0"};
    READ(s, Path);
    FILE *cl = fopen(CLONE(path), "w");
    fputs(s, cl);
    fclose(cl);
    int arr[N] = {0};
    int ptr = 0, n = SZ(str);
    for(int i = 1; i < n; i ++)
    {
        arr[i] = arr[i - 1];
        while(arr[i] && (str[arr[i]] != str[i]))
        {
            arr[i] = arr[arr[i] - 1];
        }
        arr[i] += (int)(str[arr[i]] == str[i]);
    }
    int j = 0;
    char* ret = calloc(N2 + 1, sizeof (char));
    for(int i = 0; i < N2; i ++)
    {
        ret[i] = '\0';
    }
    for(int i = 0; i < SZ(s); i ++)
    {
        strncat(ret, &s[i], 1);
        while(j && (str[j] != s[i]))
        {
            j = arr[j - 1];
        }
        j += (int)(str[j] == s[i]);
        if(j == SZ(str))
        {
            ptr ++;
            if(ptr == at || all)
            {
                for(int k = 0; k < SZ(str); k ++)
                {
                    ret[SZ(ret) - 1] = '\0';
                }
                strcat(ret, str2);
            }
            j = 0;
        }
    }
    FLUSH;
    FILE *f = fopen(Path, "w");
    fputs(ret, f);
    fclose(f);
}

void undo()
{
    char* Path = FIX2(path);
    if(!Valid(path) || !exist_path(path))
    {
        INVALID;
        return;
    }
    char s[N2] = {"/0"};
    READ(s, CLONE(path));
    FILE *f = fopen(Path, "w");
    fputs(s, f);
    fclose(f);
}

/// grep

void grep()
{
    if(str == NULL)
    {
        INVALID;
        return;
    }
    int arr[N] = {0};
    int ptr = 0, n = SZ(str);
    for(int i = 1; i < n; i ++)
    {
        arr[i] = arr[i - 1];
        while(arr[i] && str[arr[i]] != str[i])
        {
            arr[i] = arr[arr[i] - 1];
        }
        arr[i] += (int)(str[arr[i]] == str[i]);
    }
    char* CUR = strtok(path, " ");
    while(CUR != NULL)
    {
        char* now = FIX(CUR);
        if(!E(now, ""))
        {
            if(!Valid(now) || !exist_path(now))
            {
                INVALID;
                return;
            }
            char s[N2] = {"\0"};
            READ(s, FIX2(now));
            int j = 0;
            for(int i = 0; i < SZ(s); i ++)
            {
                while(j && s[i] != str[j])
                {
                    j = arr[j - 1];
                }
                j += (s[i] == str[j]);
                if(j == SZ(str))
                {
                    printf("Found Match i = %d file : %s\n", i, now);
                    j = 0;
                }
            }
        }
        CUR = strtok(NULL, " ");
    }
}

/// auto indent

void auto_indent()
{
    char* Path = FIX2(path);
    if(!Valid(path) || !exist_path(path))
    {
        INVALID;
        return;
    }
    char s[N2] = {"\0"};
    char ret[N2] = {"\0"};
    READ(s, Path);
    int T = 0;
    for(int i = 0; i < SZ(s); i ++)
    {
        if(s[i] == '{')
        {
            strcat(ret, "\n");
            strcat(ret, STRFROM(4 * T, ' '));
            strcat(ret, "{\n");
            strcat(ret, STRFROM(4 * T + 4, ' '));
            T ++;
        }
        else if(s[i] == '}')
        {
            T --;
            strcat(ret, "\n");
            strcat(ret, STRFROM(4 * T, ' '));
            strcat(ret, "}\n");
            strcat(ret, STRFROM(4 * T, ' '));
            if(T < 0)
            {
                printf("Not correct sequence for auto-indent :(\n");
                return;
            }
        }
        else
        {
            strcat(ret, STRFROM(1, s[i]));
            if(s[i] == '\n')
            {
                strcat(ret, STRFROM(4 * T, ' '));
            }
        }
    }
    if(T != 0)
    {
        printf("Not correct sequence for auto-indent :(\n");
        return;
    }
    FILE *f = fopen(Path, "w");
    fputs(ret, f);
    fclose(f);
}

/// Digesting the input to known elements

int _file()
{
    if(SZ(cur) >= 5 && E(PRE(4, cur), "file"))
    {
        cur += 5;
        path = FIX(cur);
        return 1;
    }
    return 0;
}

int _str()
{
    if(SZ(cur) >= 4 && E(PRE(4, cur), "str1"))
    {
        cur += 4;
        str = FIX(cur);
        return 1;
    }
    if(SZ(cur) >= 4 && E(PRE(4, cur), "str2"))
    {
        cur += 4;
        str2 = FIX(cur);
        return 1;
    }
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

int _flag()
{
    if(SZ(FIX(cur)) == 1)
    {
        char* idk = FIX(cur);
        if(idk[0] == 'b')
        {
            dir = -1;
        }
        if(idk[0] == 'f')
        {
            dir = 1;
        }
        return 1;
    }
    return 0;
}

int _atallbyword()
{
    if(SZ(cur) >= 3)
    {
        if(E(PRE(2, cur), "at"))
        {
            cur += 2;
            at = TO_INT(FIX(cur));
            return 1;
        }
        if(E(PRE(3, cur), "all"))
        {
            all = 1;
            return 1;
        }
    }
    if(SZ(cur) >= 5 && E(PRE(5, cur), "count"))
    {
        cnt = 1;
        return 1;
    }
    if(SZ(cur) >= 6 && E(PRE(6, cur), "byword"))
    {
        byword = 1;
        return 1;
    }
    return 0;
}

int _indent()
{
    if(SZ(cur) >= 6 && E(PRE(6, cur), "indent"))
    {
        cur += 6;
        path = FIX(cur);
        return 1;
    }
    return 0;
}

void Digest()
{
    cur = str = str2 = path = command = NULL;
    cnt = at = all = byword = dir = 0;
    x = y = SIZE = -1;
    cur = strtok(Input, "-");
    command = cur;
    command[SZ(cur) - 1] = '\0';
    cur = strtok(NULL, "-");
    while(cur != NULL)
    {
        if(!(_file() || _size() || _pos() || _str() || _flag() || _atallbyword() || _indent()))
        {
            INVALID;
            break;
        }
        cur = strtok(NULL, "-");
    }
    if(at && all)
    {
        INVALID;
        return;
    }
    if (E(command, "createfile"))
    {
        createfile();
    }
    else if (E(command, "insertstr"))
    {
        insertstr();
    } 
    else if (E(command, "cat"))
    {
        cat();
    }
    else if (E(command, "removestr"))
    {
        removestr();
    }
    else if (E(command, "copystr"))
    {
        copystr();
    }
    else if (E(command, "cutstr"))
    {
        cutstr();
    }
    else if (E(command, "pastestr"))
    {
        pastestr();
    }
    else if (E(command, "find"))
    {
        find();
    }
    else if (E(command, "replace"))
    {
        replace();
    }
    else if (E(command, "grep"))
    {
        grep();
    }
    else if (E(command, "undo"))
    {
        undo();
    }
    else if (E(command, "aut"))
    {
        auto_indent();
    }
    else if (E(command, "exi"))
    {
        exit(0);
    }
    else
    {
        INVALID;
    }
}

int main()
{
    Main_sz = GET();
    printf("Ariya's VIM   Powered by ~Kaycee~\n");
    do
    {
        printf("> ");
        scanf("%[^\n]%*c", Input);
        Digest();
    } while (1);
    return 0;
}