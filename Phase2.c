#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ncurses.h>
#include "MYLIB.h"

#define DIS         4
#define EXIT        endwin(), exit(0)

int rows, colums, Lr, Lc;

int ID[N2][N2], IDX[N3], IDY[N3];

char *Modes[] = {"NORMAL", "VISUAL", "INSERT", "NULL"};

char Lines[N2][N2];

int ABS(int x)
{
    if(x < 0) return -x;
    return x;
}

void CLEARMARK()
{
    for(int i = 0; i < N3; i ++)
    {
        MARK[i] = 0;
    }
}

char* FREAD(char* path)
{
    char* s = calloc(N2, sizeof (char));
    READ(s, FIX2(path));
    return s;
}

int FULLREAD(char* path)
{
    for(int i = 0; i < N2; i ++)
    {
        for(int j = 0; j < N2; j ++)
        {
            Lines[i][j] = '\0';
            ID[i][j] = -1;
        }
    }
    int n = 0;
    char* s = FREAD(path);
    int j = 0;
    for(int i = 0; i < SZ(s); i ++)
    {
        IDX[i] = n;
        IDY[i] = j;
        Lines[n][j] = s[i];
        ID[n][j] = i;
        if(s[i] == '\n')
        {
            n ++;
            j = 0;
        }
        else
        {
            j ++;
        }
    }
    for(int I = 0; I < N2; I ++)
    {
        for(int J = 0; J < N2; J ++)
        {
            if(ID[I][J] == -1 && J)
            {
                ID[I][J] = ID[I][J - 1];
            }
            if(ID[I][J] == -1 && I)
            {
                ID[I][J] = ID[I - 1][J];
            }
        }
    }
    for(int i = SZ(s); i < N3; i ++)
    {
        IDX[i] = n;
        IDY[i] = j;
    }
    return n + 1;
}

struct Node
{
    int mode, x, y, saved; /// x, y -> shift haye satri va sotoni
    int curx, cury, pos;
    char* path;
} myvim;

int check(int x, int y)
{
    if(Lines[x + myvim.x][y + myvim.y] == '\0')
    {
        return 0;
    }
    return 1;
}

void Print(int i, int j)
{
    if(MARK[ID[i][j]])
    {
        attron(COLOR_PAIR(1));
    }
    printw("%c", Lines[i][j]);
    if(MARK[ID[i][j]])
    {
        attroff(COLOR_PAIR(1));
    }
}

void SHOW()
{
    clear();
    FULLREAD(myvim.path);
    for(int i = 0; i < rows - 2; i ++)
    {
        mvprintw(i, 0, "%3d~", i + myvim.x + 1);
        for(int j = 0; j < Lc; j ++)
        {
            if(check(i, j))
            {
                Print(i + myvim.x, j + myvim.y);
            }
        }
    }
    mvprintw(rows - 2, 0, "%s  %s", Modes[myvim.mode], myvim.path);
    if(!myvim.saved)
    {
        printw(" Not Saved");
    }
    else
    {
        printw(" Saved");
    }
    if(myvim.mode == 0)
    {
        mvprintw(rows - 1, 0, "%s", Input);
    }
    move(myvim.curx, myvim.cury + 4);
    refresh();
}

void NAV(int d) // D, U, L, R
{
    if(d == 0)
    {
        if(myvim.curx + myvim.x + 1 == N2)
        {
            return;
        }
        if(myvim.curx + 1 == Lr)
        {
            myvim.x ++;
        }
        else
        {
            myvim.curx ++;
        }
        int sz = SZ(Lines[myvim.x + myvim.curx]); ///
        int delta = myvim.y + myvim.cury - sz;
        if(sz) delta ++;
        if(delta > 0) /// end line
        {
            int hala = Min(myvim.cury, delta);
            delta -= hala;
            myvim.cury -= hala;
            myvim.y -= delta;
        }
    }
    else if(d == 1)
    {
        if(myvim.curx + myvim.x - 1 == -1)
        {
            return;
        }
        if(myvim.curx - 1 == -1)
        {
            myvim.x --;
        }
        else
        {
            myvim.curx --;
        }
        int sz = SZ(Lines[myvim.x + myvim.curx]); ///
        int delta = myvim.y + myvim.cury - sz;
        if(sz) delta ++;
        if(delta > 0) /// end line
        {
            int hala = Min(myvim.cury, delta);
            delta -= hala;
            myvim.cury -= hala;
            myvim.y -= delta;
        }
    }
    else if(d == 2)
    {
        if(myvim.cury + myvim.y == 0) return;
        if(myvim.cury)
        {
            myvim.cury --;
        }
        else
        {
            myvim.y --;
        }
    }
    else if(d == 3)
    {
        if(myvim.cury + myvim.y + 1 == N2) return;
        if(myvim.cury + 1 != Lc)
        {
            myvim.cury ++;
        }
        else
        {
            myvim.y ++;
        }
    }
    move(myvim.curx, myvim.cury + 4);
}

void DRAW(int l, int r)
{
    CLEARMARK();
    if(l > r)
    {
        int mid = l;
        l = r;
        r = mid;
    }
    for(int i = l; i <= r; i ++)
    {
        MARK[i] = 1;
    }
}

int main()
{
    myvim.pos = -1;
    myvim.path = "/root/T1/T2/file.txt";
    myvim.mode = 3;
    myvim.x = myvim.y = 0;
    myvim.curx = myvim.cury = 0;
    myvim.saved = 1;
    initscr();
    start_color();
    noecho();
    cbreak();
    keypad(stdscr, true);
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
    init_pair(2, COLOR_BLACK, COLOR_GREEN);
    init_pair(3, COLOR_GREEN, COLOR_BLACK);
    init_pair(4, COLOR_RED, COLOR_BLACK);
    init_pair(5, COLOR_CYAN, COLOR_BLACK);
    getmaxyx(stdscr, rows, colums);
    Lr = rows - 2; /// the last 2 rows
    Lc = colums - 4; /// the first 4 colums
    SHOW();
    while(1)
    {
        int key = getch();
        ///mvprintw(41, 4, "%d", key);
        refresh();
        if(myvim.mode == 3) /// mode NULL
        {
            if(key == 'i')              // insert
            {
                myvim.mode = 2;
            }
            else if(key == 'v')         // visual
            {
                myvim.mode = 1;
            }
            else if(key == 'n')         // normal
            {
                while(SZ(Input))
                {
                    Input[SZ(Input) - 1] = '\0';
                }
                myvim.mode = 0;
            }
            else if(key == 'u')         // undo
            {
                undo(myvim.path);
            }
            else if(key == 'a')         // auto-indent
            {
                auto_indent(myvim.path);
            }
            else if(key == 's')         // save
            {
                myvim.saved = 1;
            }
            else if(key == 'e')         // exit
            {
                endwin();
                exit(0);
            }
            SHOW();
            continue;
        }
        if(myvim.mode == 0) /// normal
        {
            if(key == 27)
            {
                CLEARMARK();
                myvim.mode = 3;
                continue;
            }
            if(key == 10) /// enter
            {
                if(myvim.mode == 0)
                {
                    clear();
                    int ret = Digest();
                    if(ret == 1)
                    {
                        myvim.path = path;
                        SHOW();
                    }
                    if(ret == 2)
                    {
                        myvim.saved = 1;
                    }
                    if(ret == 3)
                    {
                        createfile(path);
                        FILE *f = fopen(FIX2(path), "w");
                        fputs(FREAD(myvim.path), f);
                        fclose(f);
                        myvim.path = path;
                    }
                    refresh();
                    while(SZ(Input))
                    {
                        Input[SZ(Input) - 1] = '\0';
                    }
                    if(E(command, "find") || E(command, "replace"))
                    {
                        SHOW();
                        continue;
                    }
                    getch();
                    continue;
                }
            }
            else
            {
                Input[SZ(Input)] = key;
            }
            SHOW();
            continue;
        }
        if(myvim.mode == 2) /// Insert
        {
            if(key == 27)
            {
                CLEARMARK();
                myvim.mode = 3;
                continue;
            }
            if(258 <= key && key <= 261)
            {
                NAV(key - 258);
                continue;
            }
            if(key == '~')
            {
                pastestr(myvim.path, myvim.x + myvim.curx + 1, myvim.y + myvim.cury);
                myvim.saved = 0;
                SHOW();
                continue;
            }
            insertstr(myvim.path, myvim.x + myvim.curx + 1, myvim.y + myvim.cury, STRFROM(1, key));
            if(key == '\n')
            {
                NAV(0);
            }
            else { NAV(3); }
            myvim.saved = 0;
            SHOW();
            continue;
        }
        if(myvim.mode == 1)     /// visual
        {
            if(myvim.pos == -1)
            {
                myvim.pos = ID[myvim.curx + myvim.x][myvim.cury + myvim.y];
            }
            if(key == 27)
            {
                CLEARMARK();
                myvim.pos = -1;
                myvim.mode = 3;
                continue;
            }
            int side = 0;
            if(myvim.pos <= ID[myvim.curx + myvim.x][myvim.cury + myvim.y])
            {
                side = -1;
            }
            else
            {
                side = 1;
            }
            int SIZE = ABS(ID[myvim.curx + myvim.x][myvim.cury + myvim.y] - myvim.pos) + 1;
            if(key == 'r')          /// delete
            {
                removestr(myvim.path, myvim.curx + myvim.x + 1, myvim.cury + myvim.y, side, SIZE);
                CLEARMARK();
                myvim.saved = 0;
                SHOW();
                getch();
                myvim.mode = 3;
                SHOW();
                continue;
            }
            if(key == 'c')          /// copy
            {
                copystr(myvim.path, myvim.curx + myvim.x + 1, myvim.cury + myvim.y, side, SIZE);
                CLEARMARK();
                myvim.saved = 0;
                SHOW();
                getch();
                myvim.mode = 3;
                SHOW();
                continue;
            }
            if(key == 'q')          /// cut
            {
                cutstr(myvim.path, myvim.curx + myvim.x + 1, myvim.cury + myvim.y, side, SIZE);
                CLEARMARK();
                myvim.saved = 0;
                SHOW();
                getch();
                myvim.mode = 3;
                SHOW();
                continue;
            }
            if(258 <= key && key <= 261)
            {
                NAV(key - 258);
            }
            DRAW(myvim.pos, ID[myvim.curx + myvim.x][myvim.cury + myvim.y]);
            SHOW();
        }
    }
    endwin();
    return 0;
}