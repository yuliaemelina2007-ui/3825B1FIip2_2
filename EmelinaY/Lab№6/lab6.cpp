#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cstring>

const int N = 10;

char letters[] = { 'A','B','C','D','E','F','G','H','I','J' };

struct Point {
    int row;
    int col;
    Point(int r = -1, int c = -1) : row(r), col(c) {}
};

struct Segment {
    int len;
    int posesX[4];
    int posesY[4];
    int damaged[4];
    int live;

    Segment(int l = 0) : len(l), live(l) {
        for (int i = 0; i < 4; i++) {
            posesX[i] = -1;
            posesY[i] = -1;
            damaged[i] = 0;
        }
    }

    void setPos(int num, int x, int y) {
        posesX[num] = x;
        posesY[num] = y;
    }

    int checkHit(int x, int y) {
        for (int i = 0; i < len; i++) {
            if (posesX[i] == x && posesY[i] == y && damaged[i] == 0) {
                damaged[i] = 1;
                live--;
                if (live == 0) return 2;
                return 1;
            }
        }
        return 0;
    }

    int isDead() {
        return live == 0;
    }
};

class Field {
private:
    int map[N][N];
    int marks[N][N];
    Segment boats[10];
    int boatCnt;

    int checkAround(int x, int y) {
        for (int dx = -1; dx <= 1; dx++) {
            for (int dy = -1; dy <= 1; dy++) {
                if (dx == 0 && dy == 0) continue;
                int nx = x + dx;
                int ny = y + dy;
                if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                    if (map[nx][ny] == 1) return 1;
                }
            }
        }
        return 0;
    }

public:
    Field() : boatCnt(0) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                map[i][j] = 0;
                marks[i][j] = 0;
            }
        }
    }

    void wipe() {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                map[i][j] = 0;
                marks[i][j] = 0;
            }
        }
        boatCnt = 0;
    }

    int canFit(int x, int y, int sz, int dir) {
        if (dir == 0) {
            if (y + sz > N) return 0;
            for (int i = 0; i < sz; i++) {
                if (map[x][y + i] == 1) return 0;
            }
            for (int i = 0; i < sz; i++) {
                if (checkAround(x, y + i)) return 0;
            }
        }
        else {
            if (x + sz > N) return 0;
            for (int i = 0; i < sz; i++) {
                if (map[x + i][y] == 1) return 0;
            }
            for (int i = 0; i < sz; i++) {
                if (checkAround(x + i, y)) return 0;
            }
        }
        return 1;
    }

    void insert(int x, int y, int sz, int dir) {
        Segment newBoat(sz);

        if (dir == 0) {
            for (int i = 0; i < sz; i++) {
                map[x][y + i] = 1;
                newBoat.setPos(i, x, y + i);
            }
        }
        else {
            for (int i = 0; i < sz; i++) {
                map[x + i][y] = 1;
                newBoat.setPos(i, x + i, y);
            }
        }

        boats[boatCnt] = newBoat;
        boatCnt++;
    }

    int fire(int x, int y) {
        if (marks[x][y]) return -1;

        marks[x][y] = 1;

        if (map[x][y] == 1) {
            for (int i = 0; i < boatCnt; i++) {
                int res = boats[i].checkHit(x, y);
                if (res == 2) {
                    surroundDead(i);
                    return 2;
                }
                if (res == 1) return 1;
            }
        }

        return 0;
    }

    void surroundDead(int idx) {
        for (int k = 0; k < boats[idx].len; k++) {
            int x = boats[idx].posesX[k];
            int y = boats[idx].posesY[k];
            map[x][y] = 2;
            marks[x][y] = 1;

            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = x + dx;
                    int ny = y + dy;
                    if (nx >= 0 && nx < N && ny >= 0 && ny < N) {
                        if (map[nx][ny] == 0) {
                            map[nx][ny] = 3;
                            marks[nx][ny] = 1;
                        }
                    }
                }
            }
        }
    }

    int allDead() {
        for (int i = 0; i < boatCnt; i++) {
            if (!boats[i].isDead()) return 0;
        }
        return 1;
    }

    int alreadyShot(int x, int y) {
        return marks[x][y];
    }

    char getSym(int x, int y, int show) {
        if (marks[x][y]) {
            if (map[x][y] == 1 || map[x][y] == 2) return 'X';
            else return 'O';
        }
        if (show && map[x][y] == 1) return '#';
        if (map[x][y] == 2) return 'X';
        if (map[x][y] == 3) return 'O';
        return '.';
    }

    char getEnemySym(int x, int y) {
        if (marks[x][y]) {
            if (map[x][y] == 1 || map[x][y] == 2) return 'X';
            else return 'O';
        }
        return '.';
    }

    void show(int fl) {
        std::cout << "  ";
        for (int i = 1; i <= N; i++) std::cout << i << " ";
        std::cout << std::endl;

        for (int i = 0; i < N; i++) {
            std::cout << letters[i] << " ";
            for (int j = 0; j < N; j++) {
                std::cout << getSym(i, j, fl) << " ";
            }
            std::cout << std::endl;
        }
    }

    void showTwo(Field& opp, const char* nm) {
        std::cout << "\n" << nm << "'s fields:\n";
        std::cout << "Left (my ships):                 Right (enemy ships):\n";

        std::cout << "  ";
        for (int i = 1; i <= N; i++) std::cout << i << " ";
        std::cout << "                     ";
        for (int i = 1; i <= N; i++) std::cout << i << " ";
        std::cout << std::endl;

        for (int i = 0; i < N; i++) {
            std::cout << letters[i] << " ";
            for (int j = 0; j < N; j++) {
                std::cout << getSym(i, j, 1) << " ";
            }

            std::cout << "                  " << letters[i] << " ";

            for (int j = 0; j < N; j++) {
                std::cout << opp.getEnemySym(i, j) << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

int toCoord(const char* str, int& x, int& y) {
    if (strlen(str) < 2) return 0;

    char ch = str[0];
    int r = -1;
    for (int i = 0; i < N; i++) {
        if (letters[i] == ch || letters[i] == ch - 32) {
            r = i;
            break;
        }
    }
    if (r == -1) return 0;

    int c = atoi(str + 1) - 1;
    if (c < 0 || c >= N) return 0;

    x = r;
    y = c;
    return 1;
}

class Gamer {
protected:
    char nick[30];
public:
    Field my;
    Field view;

    Gamer(const char* nm) {
        strcpy(nick, nm);
    }

    virtual ~Gamer() {}

    virtual void deploy() = 0;
    virtual Point attack() = 0;
    virtual void fixResult(int x, int y, int code) {}

    char* getName() { return nick; }
};

class Human : public Gamer {
public:
    Human(const char* nm) : Gamer(nm) {}

    void deploy() {
        int szList[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        std::cout << "\n" << nick << ", place your fleet!\n";
        std::cout << "Example: A1 H or A1 V (for 1-cell: A1)\n";
        std::cout << "Ships must not touch!\n\n";

        for (int idx = 0; idx < 10; idx++) {
            my.show(1);

            int ok = 0;
            while (!ok) {
                char buf[50];
                std::cout << "Place " << szList[idx] << "-deck: ";
                std::cin.getline(buf, 50);

                int x, y;
                if (szList[idx] == 1) {
                    if (!toCoord(buf, x, y)) {
                        std::cout << "Bad coords!\n";
                        continue;
                    }
                    if (my.canFit(x, y, 1, 0)) {
                        my.insert(x, y, 1, 0);
                        ok = 1;
                        std::cout << "OK!\n";
                    }
                    else {
                        std::cout << "Can't place here!\n";
                    }
                }
                else {
                    char cmd[10];
                    char crd[10];
                    int rd = sscanf(buf, "%s %s", crd, cmd);
                    if (rd < 2) {
                        std::cout << "Need direction H or V!\n";
                        continue;
                    }
                    if (!toCoord(crd, x, y)) {
                        std::cout << "Bad coords!\n";
                        continue;
                    }
                    int hor = (cmd[0] == 'H' || cmd[0] == 'h');
                    if (my.canFit(x, y, szList[idx], hor)) {
                        my.insert(x, y, szList[idx], hor);
                        ok = 1;
                        std::cout << "OK!\n";
                    }
                    else {
                        std::cout << "Can't place here!\n";
                    }
                }
            }
        }

        std::cout << "\nDeployment complete!\n";
        my.show(1);
    }

    Point attack() {
        char inp[20];
        int x, y;

        while (1) {
            std::cout << nick << ", fire (like A1): ";
            std::cin.getline(inp, 20);

            if (!toCoord(inp, x, y)) {
                std::cout << "Wrong format!\n";
                continue;
            }

            if (view.alreadyShot(x, y)) {
                std::cout << "Already fired there!\n";
                continue;
            }

            break;
        }

        return Point(x, y);
    }
};

class Machine : public Gamer {
private:
    int targetsX[200];
    int targetsY[200];
    int targetCnt;
    int huntFlag;
    int lastRow, lastCol;

    void addNeighbors(int x, int y) {
        if (x > 0 && !my.alreadyShot(x - 1, y)) {
            targetsX[targetCnt] = x - 1;
            targetsY[targetCnt] = y;
            targetCnt++;
        }
        if (x < N - 1 && !my.alreadyShot(x + 1, y)) {
            targetsX[targetCnt] = x + 1;
            targetsY[targetCnt] = y;
            targetCnt++;
        }
        if (y > 0 && !my.alreadyShot(x, y - 1)) {
            targetsX[targetCnt] = x;
            targetsY[targetCnt] = y - 1;
            targetCnt++;
        }
        if (y < N - 1 && !my.alreadyShot(x, y + 1)) {
            targetsX[targetCnt] = x;
            targetsY[targetCnt] = y + 1;
            targetCnt++;
        }
    }

public:
    Machine(const char* nm) : Gamer(nm), targetCnt(0), huntFlag(0), lastRow(-1), lastCol(-1) {}

    void deploy() {
        int sizes[] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

        for (int step = 0; step < 10; step++) {
            int placed = 0;
            int tries = 0;
            while (!placed && tries < 500) {
                int r = rand() % N;
                int c = rand() % N;
                int d = rand() % 2;
                if (my.canFit(r, c, sizes[step], d)) {
                    my.insert(r, c, sizes[step], d);
                    placed = 1;
                }
                tries++;
            }
            if (!placed) {
                my.wipe();
                step = -1;
            }
        }

        std::cout << "Machine deployed its fleet\n";
    }

    Point attack() {
        Point res;

        if (huntFlag && targetCnt > 0) {
            for (int i = targetCnt - 1; i >= 0; i--) {
                if (!my.alreadyShot(targetsX[i], targetsY[i])) {
                    res.row = targetsX[i];
                    res.col = targetsY[i];
                    targetCnt = i;
                    return res;
                }
            }
            huntFlag = 0;
            targetCnt = 0;
        }

        huntFlag = 0;
        targetCnt = 0;

        do {
            res.row = rand() % N;
            res.col = rand() % N;
        } while (my.alreadyShot(res.row, res.col));

        return res;
    }

    void fixResult(int x, int y, int code) {
        if (code == 1) {
            if (!huntFlag) {
                huntFlag = 1;
                targetCnt = 0;
                addNeighbors(x, y);
            }
            else {
                addNeighbors(x, y);
            }
        }
        else if (code == 2) {
            huntFlag = 0;
            targetCnt = 0;
        }
    }
};

class Match {
private:
    Human user;
    Machine pc;
    int finished;

public:
    Match() : user("Player"), pc("Computer"), finished(0) {
        srand(time(0));
    }

    void start() {
        std::cout << "=== SEA BATTLE ===\n\n";

        user.deploy();
        std::cout << "\n";
        pc.deploy();
        std::cout << "\n";

        int turn = 1;

        while (!finished) {
            if (turn) {
                user.my.showTwo(pc.my, "Player");

                Point sh = user.attack();
                int res = pc.my.fire(sh.row, sh.col);

                std::cout << "Result: ";
                if (res == 0) {
                    std::cout << "MISS\n";
                }
                else if (res == 1) {
                    std::cout << "HIT\n";
                }
                else {
                    std::cout << "HIT! DESTROYED\n";
                }

                user.view.fire(sh.row, sh.col);
                user.fixResult(sh.row, sh.col, res);

                if (pc.my.allDead()) {
                    std::cout << "\n*** PLAYER VICTORY! ***\n";
                    finished = 1;
                }

                if (res == 0) {
                    turn = 0;
                }
            }
            else {
                Point sh = pc.attack();
                std::cout << "Computer fires at " << letters[sh.row] << (sh.col + 1) << ": ";

                int res = user.my.fire(sh.row, sh.col);

                if (res == 0) {
                    std::cout << "MISS\n";
                }
                else if (res == 1) {
                    std::cout << "HIT\n";
                }
                else {
                    std::cout << "HIT! DESTROYED\n";
                }

                pc.view.fire(sh.row, sh.col);
                pc.fixResult(sh.row, sh.col, res);

                if (user.my.allDead()) {
                    std::cout << "\n*** COMPUTER VICTORY! ***\n";
                    finished = 1;
                }

                if (res == 0) {
                    turn = 1;
                }
            }
        }

        std::cout << "\nFinal position:\n";
        user.my.showTwo(pc.my, "Player");
    }
};

int main() {
    Match game;
    game.start();
    return 0;
}