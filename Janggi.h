#pragma once

#include <qwidget.h>
#include <qpainter.h>
#include <vector>

enum JanggiTeam
{
    Red,
    Blue
};

//궁 1, 사 2, 차 3, 포 4, 마 5, 상 6, 병 7
enum JanggiPeiceType
{
    Gung,
    Sa,
    Cha,
    Po,
    Ma,
    Sang,
    Byeong
};

class JanggiPosition
{
public:
    JanggiPosition();
    JanggiPosition(int row, int column);

    bool operator==(const JanggiPosition& pos) const;
    bool operator!=(const JanggiPosition& pos) const;
    JanggiPosition operator+(const JanggiPosition& pos) const;
    JanggiPosition& operator+=(const JanggiPosition& pos) ;

public:
    int row;
    int column;
};

class Janggi;

class JanggiPeice
{
public:
    JanggiPeice(Janggi *janggi, JanggiPosition pos, JanggiTeam team, JanggiPeiceType type);
    ~JanggiPeice();

    bool canMove(JanggiPosition pos);
    bool canPlace(JanggiPosition pos);
    bool canDestroy(JanggiPosition pos);
    bool canPlaceOrDestroy(JanggiPosition pos);
    std::vector<JanggiPosition> getMoves();

    void move(JanggiPosition pos);

    JanggiTeam getTeam() const;
    JanggiPeiceType getType() const;

private:
    JanggiPosition pos;
    Janggi* janggi;
    JanggiTeam team;
    JanggiPeiceType type;
};


class Janggi
{
public:
    Janggi();
    ~Janggi();

public:
    void reset(int redToggle = 0, int blueToggle = 0);
    JanggiPeice* getPeice(JanggiPosition pos);

private:
    void move(JanggiPosition from, JanggiPosition to);

public:
    bool isValid(JanggiPosition pos);
    bool isCastle(JanggiPosition pos);
    bool isCastle(JanggiPosition pos, JanggiTeam team);
    bool isCastleCenter(JanggiPosition pos);
    bool isCastleCenter(JanggiPosition pos, JanggiTeam team);
    bool isCastleVert(JanggiPosition pos);
    bool isCastleVert(JanggiPosition pos, JanggiTeam team);

public:
    const static int GridRow = 10;
    const static int GridColumn = 9;
private:
    JanggiPeice* peices[GridRow][GridColumn];

    friend class JanggiPeice;
};

/*

//team_check가 0이면 모두 검사, 팀을 넣어 그 팀은 검사 안함
int pos_available(int x, int y, int team_check) {
    if (!pos_boundary(x, y)) return 0;
    if (!team_check) {
        if (grid[x][y] != 0) return 0;
    }
    else
        if (grid[x][y] * team_check > 0) return 0;
    return 1;
}


//team이 0일 시 둘다 허용
int pos_castle(int x, int y, int team) {
    if (!pos_boundary(x, y)) return 0;
    if ((team == 1 || !team) && 3 <= y && y <= 5 && x <= 2)
        return 1;
    else if ((team == -1 || !team) && 3 <= y && y <= 5 && x >= 7)
        return 1;
    return 0;
}

//team이 0일 시 둘다 허용
int pos_castle_center(int x, int y, int team) {
    if (!pos_boundary(x, y)) return 0;
    if ((team == 1 || !team) && 4 == y && x == 1)
        return 1;
    else if ((team == -1 || !team) && 4 == y && x == 8)
        return 1;
    return 0;
}

//team이 0일 시 둘다 허용
int pos_castle_vert(int x, int y, int team) {
    if (!pos_boundary(x, y)) return 0;
    if ((team == 1 || !team) && (3 == y || y == 5) && (x == 2 || x == 0))
        return 1;
    else if ((team == -1 || !team) && (3 == y || y == 5) && (x == 7 || x == 9))
        return 1;
    return 0;
}

int pos_boundary(int x, int y) {
    if (x < 0 || y < 0 || x >= GRID_R || y >= GRID_C) return 0;
    return 1;
}

int peice_action(int x, int y) {
    int team = grid[x][y] > 0 ? 1 : -1;
    int id = grid[x][y] * team;

    COORD peice_arr[GRID_R * GRID_C];
    int count = 0;

    int dx[] = { 0,0,-1,1,1,1,-1,-1 };
    int dy[] = { 1,-1,0,0,-1,1,-1,1 };
    int turn[] = { 1,-1 };

    //궁 1, 사 2, 차 3, 포 4, 마 5, 상 6, 병 7
    switch (id)
    {
    case 1:
    case 2:
        for (int i = 0; i < ((pos_castle_center(x, y, team) || pos_castle_vert(x, y, team)) ? 8 : 4); i++)
        {
            int tx = x + dx[i];
            int ty = y + dy[i];
            if (pos_available(tx, ty, team) && pos_castle(tx, ty, team))
                peice_arr[count++] = (COORD){ tx, ty };
        }
        break;
    case 3:
        for (int i = 0; i < ((pos_castle_center(x, y, 0) || pos_castle_vert(x, y, 0)) ? 8 : 4); i++)
        {
            int tx = x + dx[i];
            int ty = y + dy[i];
            while (pos_available(tx, ty, 0) && (i < 4 || pos_castle(tx, ty, 0)))
            {
                peice_arr[count++] = (COORD){ tx, ty };
                tx += dx[i];
                ty += dy[i];
            }
            if (pos_available(tx, ty, team))
                peice_arr[count++] = (COORD){ tx, ty };
        }
        break;
    case 4:
        for (int i = 0; i < 4; i++)
        {
            int tx = x + dx[i];
            int ty = y + dy[i];
            if (pos_boundary(tx, ty, 0) && abs(grid[tx][ty]) != id && abs(grid[tx][ty]) != 0)
            {
                while (tx += dx[i], ty += dy[i], pos_available(tx, ty, 0))
                {
                    peice_arr[count++] = (COORD){ tx, ty };
                }
                if (pos_available(tx, ty, team) && abs(grid[tx][ty]) != id)
                    peice_arr[count++] = (COORD){ tx, ty };
            }
        }
        break;
    case 5:
    case 6:
        for (int i = 0; i < 4; i++)
        {
            for (int t = 0; t < 2; t++)
            {
                int tx = x + dx[i];
                int ty = y + dy[i];
                int can_reach = 1;

                for (int chk = 0; chk < id - 4; chk++)
                {
                    if (!pos_available(tx, ty, 0)) {
                        can_reach = 0;
                        break;
                    }
                    tx += dx[i];
                    ty += dy[i];
                    if (dx[i])
                        ty += turn[t];
                    else
                        tx += turn[t];
                }
                if (can_reach && pos_available(tx, ty, team))
                    peice_arr[count++] = (COORD){ tx, ty };
            }
        }
        break;
    case 7:
        ; COORD target[] = { {x + team, y}, {x ,y + 1},{x ,y - 1}, {x + team, y + 1} ,{x + team, y - 1} };
        for (int i = 0; i < ((pos_castle_center(x, y, -team) || pos_castle_vert(x, y, -team)) ? 5 : 3); i++)
        {
            if (pos_available(target[i].X, target[i].Y, team) && (i < 3 || pos_castle(target[i].X, target[i].Y, -team))) {
                peice_arr[count++] = target[i];
            }
        }
        break;
    }

    print_peice_pos(x, y, 3);
    for (int i = 0; i < count; i++)
    {
        print_peice_pos(peice_arr[i].X, peice_arr[i].Y, 1);
    }

    int input;
    COORD m_pos;
    COORD grid_pos = { -1,-1 };
    do
    {
        if (grid_pos.X != -1)
            print_peice_pos(grid_pos.X, grid_pos.Y, 2);

        input = mouse_pos(&m_pos);

        if (grid_pos.X != -1)
            print_peice_pos(grid_pos.X, grid_pos.Y, 1);

        grid_pos.X = -1;

        for (int i = 0; i < count; i++)
        {
            if (m_pos.Y % 2 == 1 && m_pos.X / 2 % 2 == 1 && pos_boundary(m_pos.Y / 2, m_pos.X / 4) && peice_arr[i].X == m_pos.Y / 2 && peice_arr[i].Y == m_pos.X / 4) {
                grid_pos.X = m_pos.Y / 2;
                grid_pos.Y = m_pos.X / 4;
                break;
            }
        }

    } while (!input);

    print_peice_pos(x, y, 0);
    for (int i = 0; i < count; i++)
    {
        print_peice_pos(peice_arr[i].X, peice_arr[i].Y, 0);
    }

    //취소 명령
    if (grid_pos.X == -1) return 0;

    int isking = grid[grid_pos.X][grid_pos.Y] == -1 * team;
    grid[grid_pos.X][grid_pos.Y] = grid[x][y];
    print_peice_pos(grid_pos.X, grid_pos.Y, 0);
    grid[x][y] = 0;
    print_peice_pos(x, y, 0);

    return isking ? -1 : 1;
}


void game_play() {
    gotoXY(50, 4);
    wprintf(L"<게임 설명>");
    gotoXY(50, 5);
    wprintf(L"말 선택 후 명령");

    int turn = -1; //1은 레드 -1은 블루
    while (1)
    {
        gotoXY(50, 2);
        if (turn == 1) {
            wprintf(L"한나라 턴");
        }
        else {
            wprintf(L"초나라 턴");
        }


        int result;
        int pos = 0;
        COORD m_pos;
        COORD grid_pos = { -1,-1 };
        do
        {
            int input;
            do
            {
                if (grid_pos.X != -1)
                    print_peice_pos(grid_pos.X, grid_pos.Y, 1);

                input = mouse_pos(&m_pos);

                if (grid_pos.X != -1)
                    print_peice_pos(grid_pos.X, grid_pos.Y, 0);

                if (m_pos.Y % 2 == 1 && m_pos.X / 2 % 2 == 1 && pos_boundary(m_pos.Y / 2, m_pos.X / 4) && grid[m_pos.Y / 2][m_pos.X / 4] * turn > 0) {
                    grid_pos.X = m_pos.Y / 2;
                    grid_pos.Y = m_pos.X / 4;
                }
                else
                    grid_pos.X = -1;

            } while (!input || grid_pos.X == -1);

        } while (result = peice_action(grid_pos.X, grid_pos.Y), !result);


        if (result == -1) {//왕이 죽으면?
            break;
        }

        turn = -turn;
    }


    gotoXY(50, 4);
    wprintf(L"       ");
    gotoXY(50, 5);
    wprintf(L"               ");
}

void game_menu() {
    while (1)
    {
        gotoXY(50, 7);
        wprintf(L"[게임 시작]");
        gotoXY(50, 8);
        wprintf(L"[끝내기]");
        gotoXY(50, 4);
        wprintf(L"<게임 설명>");
        gotoXY(50, 5);
        wprintf(L"마우스 클릭으로 조작");

        int pos = -1;
        int input;
        COORD m_pos;
        do
        {
            if (pos != -1) {
                gotoXY(45, 7 + pos);
                wprintf(L" -> ");
            }

            input = mouse_pos(&m_pos);
            if (pos != -1) {
                gotoXY(45, 7 + pos);
                wprintf(L"    ");
            }

            pos = -1;
            if (50 <= m_pos.X && m_pos.X <= 60) {
                if (m_pos.Y == 7)
                    pos = 0;
                else if (m_pos.Y == 8)
                    pos = 1;
            }

        } while (!input || pos == -1);



        gotoXY(50, 7);
        wprintf(L"                     ");
        gotoXY(50, 8);
        wprintf(L"                     ");
        gotoXY(50, 4);
        wprintf(L"                     ");
        gotoXY(50, 5);
        wprintf(L"                     ");

        switch (pos)
        {
        case 0:
            reset_peice();
            print_grid();
            game_play();
            break;
        case 1:
            gotoXY(0, 21);
            return;
        }
    }
}


int main()
{
    SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS);
    _setmode(_fileno(stdout), 0x20000);
    hidecursor();

    reset_peice();
    print_grid();

    game_menu();

    return 0;
}
*/