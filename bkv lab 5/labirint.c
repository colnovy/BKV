#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int x, y;
} Point;

typedef struct {
    Point point;
    int shagi;
    Point put[100];
    int put_dlina;
} QueueNode;

int M, N;
char labirint[100][100];
int proydeno[100][100];

const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, 1, 0, -1};

int preobrazovanie(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    return 0;
}

int est_stena(int x, int y, int dir) {
    return (preobrazovanie(labirint[x][y]) >> dir) & 1;
}

void print_labirint_with_put(Point* put, int dlina) {
    char labirint_copy[100][100];
    memcpy(labirint_copy, labirint, sizeof(labirint_copy));
    int i;
    for (i = 0; i < dlina; i++) {
        labirint_copy[put[i].x][put[i].y] = '*';
    }
    
    FILE* out = fopen("output.txt", "w");
    if (!out) {
        perror("ошибка создания output.txt");
        return;
    }
    
    for (i = 0; i < M; i++) {
    	int j;
        for (j = 0; j < N; j++) {
            fprintf(out, "%c", labirint_copy[i][j]);
        }
        fprintf(out, "\n");
    }
    fclose(out);
}

void bfs() {
    QueueNode ochered[10000];
    int start = 0, finish = 0;
    
    ochered[finish].point = (Point){0, 0};
    ochered[finish].shagi = 0;
    ochered[finish].put_dlina = 1;
    ochered[finish].put[0] = (Point){0, 0};
    finish++;
    proydeno[0][0] = 1;
    
    while (start < finish) {
        QueueNode current = ochered[start++];
        Point p = current.point;

        if (p.x == M-1 && p.y == N-1) {
            printf("%d\n", current.shagi);
            print_labirint_with_put(current.put, current.put_dlina);
            return;
        }
        int dir;
        for (dir = 0; dir < 4; dir++) {
            int nx = p.x + dx[dir];
            int ny = p.y + dy[dir];
            
            if (nx >= 0 && nx < M && ny >= 0 && ny < N && !proydeno[nx][ny]) {
                if (!est_stena(p.x, p.y, dir)) {
                    proydeno[nx][ny] = 1;
                    
                    ochered[finish].point = (Point){nx, ny};
                    ochered[finish].shagi = current.shagi + 1;
                    ochered[finish].put_dlina = current.put_dlina;
                    memcpy(ochered[finish].put, current.put, current.put_dlina * sizeof(Point));
                    ochered[finish].put[ochered[finish].put_dlina++] = (Point){nx, ny};
                    finish++;
                }
            }
        }
    }
    
    printf("путь не найден\n");
}

int main() {
    FILE* file = fopen("3x4.txt", "r");
    if (!file) {
        perror("ошибка открытия 3x4.txt");
        return 1;
    }
    
    if (fscanf(file, "%d %d", &M, &N) != 2) {
        printf("неверный размер\n");
        fclose(file);
        return 1;
    }
    int i;
    for (i = 0; i < M; i++) {
        if (fscanf(file, "%s", labirint[i]) != 1) {
            printf("ошибка чтения строки %d\n", i);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    
    memset(proydeno, 0, sizeof(proydeno));
    bfs();
    
    return 0;
}
