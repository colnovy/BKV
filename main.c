#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Макрос для проверки бита
#define ISBIT(n, x) (((01 << (n)) & (x)) ? 1 : 0)

// Вывод байта в двоичном виде
void outbyte(char byte) {
    for (int i = 7; i >= 0; i--)
        putchar(ISBIT(i, byte) ? '*' : ' ');
    fflush(stdout);
}

// Вывод массива байтов в двоичном виде
void outbytes(int n, char *byte) {
    for (int i = 0; i < n; i++)
        outbyte(byte[i]);
    putchar('\n');
}

// Структура для хранения данных глифа
typedef struct img {
    int w;          // Ширина
    int h;          // Высота
    int dx;         // Расстояние до следующего глифа
    int count;      // Количество черных пикселей
    int id;         // Идентификатор
    int bytes;      // Количество байтов в битовой карте
    double density; // Плотность
    int diam;       // Диаметр
    int perim;      // Периметр
    int conn;       // Связность
    unsigned char *data; // Битовая карта
} IMG;

// Подсчет установленных битов
int popcnt64(unsigned long long w) {
    w -= (w >> 1) & 0x5555555555555555ULL;
    w = (w & 0x3333333333333333ULL) + ((w >> 2) & 0x3333333333333333ULL);
    w = (w + (w >> 4)) & 0x0f0f0f0f0f0f0f0fULL;
    return (int)((w * 0x0101010101010101ULL) >> 56);
}

int popcnt8(unsigned char i) {
    int count = 0;
    while (i) {
        ++count;
        i = (i - 1) & i;
    }
    return count;
}

// Создание пустого глифа
IMG *init_img(int id, int w, int h) {
    IMG *t = (IMG *)malloc(sizeof(IMG));
    t->w = w;
    t->h = h;
    t->dx = 0;
    t->count = 0;
    t->id = id;
    t->bytes = (((w + 7) / 8) * h);
    t->data = (unsigned char *)calloc(t->bytes, 1);
    return t;
}

// Загрузка глифа из файла
IMG *load_img(int id, const char *file) {
    FILE *F = fopen(file, "rb");
    if (!F) {
        perror("Error opening file");
        exit(1);
    }

    IMG *I = (IMG *)malloc(sizeof(IMG));
    fread(&I->w, sizeof(int), 1, F);
    fread(&I->h, sizeof(int), 1, F);
    fread(&I->dx, sizeof(int), 1, F);
    fread(&I->count, sizeof(int), 1, F);
    fread(&I->id, sizeof(int), 1, F);
    fread(&I->bytes, sizeof(int), 1, F);
    I->data = (unsigned char *)calloc(I->bytes, 1);
    fread(I->data, 1, I->bytes, F);
    fclose(F);
    return I;
}

// Вычисление плотности
double calculate_density(IMG *img) {
    return (double)img->count / (img->w * img->h);
}

// Вычисление диаметра
int calculate_diameter(IMG *img) {
    int max_dist = 0;
    for (int i1 = 0; i1 < img->h; i1++) {
        for (int j1 = 0; j1 < img->w; j1++) {
            if (ISBIT(7 - (j1 % 8), img->data[i1 * ((img->w + 7) / 8) + (j1 / 8)])) {
                for (int i2 = 0; i2 < img->h; i2++) {
                    for (int j2 = 0; j2 < img->w; j2++) {
                        if (ISBIT(7 - (j2 % 8), img->data[i2 * ((img->w + 7) / 8) + (j2 / 8)])) {
                            int dist = abs(i1 - i2) + abs(j1 - j2);
                            if (dist > max_dist) max_dist = dist;
                        }
                    }
                }
            }
        }
    }
    return max_dist;
}

// Вычисление периметра
int calculate_perimeter(IMG *img) {
    int perimeter = 0;
    for (int i = 0; i < img->h; i++) {
        for (int j = 0; j < img->w; j++) {
            if (ISBIT(7 - (j % 8), img->data[i * ((img->w + 7) / 8) + (j / 8)])) {
                // Проверяем соседние пиксели
                int neighbors = 0;
                if (i > 0 && ISBIT(7 - (j % 8), img->data[(i - 1) * ((img->w + 7) / 8) + (j / 8)])) neighbors++;
                if (i < img->h - 1 && ISBIT(7 - (j % 8), img->data[(i + 1) * ((img->w + 7) / 8) + (j / 8)])) neighbors++;
                if (j > 0 && ISBIT(7 - ((j - 1) % 8), img->data[i * ((img->w + 7) / 8) + ((j - 1) / 8)])) neighbors++;
                if (j < img->w - 1 && ISBIT(7 - ((j + 1) % 8), img->data[i * ((img->w + 7) / 8) + ((j + 1) / 8)])) neighbors++;
                if (neighbors < 4) perimeter++;
            }
        }
    }
    return perimeter;
}

// Сравнение глифов
int compare_glyphs(IMG *img1, IMG *img2) {
    if (img1->w != img2->w || img1->h != img2->h) return 0;

    int diff = 0;
    for (int i = 0; i < img1->h; i++) {
        for (int j = 0; j < img1->w; j++) {
            int bit1 = ISBIT(7 - (j % 8), img1->data[i * ((img1->w + 7) / 8) + (j / 8)]);
            int bit2 = ISBIT(7 - (j % 8), img2->data[i * ((img2->w + 7) / 8) + (j / 8)]);
            if (bit1 != bit2) diff++;
        }
    }

    double threshold = 0.1 * img1->w * img1->h;
    return diff <= threshold;
}

// Основная функция
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <glyph_file_1> <glyph_file_2> ...\n", argv[0]);
        return 1;
    }

    IMG *G[50000];
    int N = argc - 1;

    // Загрузка глифов
    for (int i = 0; i < N; i++) {
        G[i] = load_img(i, argv[i + 1]);
        G[i]->density = calculate_density(G[i]);
        G[i]->diam = calculate_diameter(G[i]);
        G[i]->perim = calculate_perimeter(G[i]);

        printf("Glyph %d: w=%d, h=%d, count=%d, density=%.2f, diam=%d, perim=%d\n",
               G[i]->id, G[i]->w, G[i]->h, G[i]->count, G[i]->density, G[i]->diam, G[i]->perim);
    }

    // Сравнение глифов
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (compare_glyphs(G[i], G[j])) {
                printf("Glyphs %d and %d are similar.\n", G[i]->id, G[j]->id);
            }
        }
    }

    // Освобождение памяти
    for (int i = 0; i < N; i++) {
        free(G[i]->data);
        free(G[i]);
    }

    return 0;
}
