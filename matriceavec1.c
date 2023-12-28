#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n1 3 // Taille de la premi�re dimension de la matrice B
#define m1 3 // Taille de la deuxi�me dimension de la matrice B
#define n2 3 // Taille de la premi�re dimension de la matrice C
#define m2 3 // Taille de la deuxi�me dimension de la matrice C
#define N (n1 * m2) // Taille du tampon

int B[n1][m1];
int C[n2][m2];
int A[n1][m2];
int T[N];

void *producer(void *arg) {
    int row = *((int *)arg);

    for (int j = 0; j < m2; j++) {
        int result = 0;
        for (int k = 0; k < m1; k++) {
            result += B[row][k] * C[k][j];
        }
        T[row * m2 + j] = result;
    }

    pthread_exit(NULL);
}

void *consumer(void *arg) {
    int index = *((int *)arg);
    int row = index / m2;
    int col = index % m2;

    A[row][col] = T[index];

    pthread_exit(NULL);
}

void print_matrix(char *matrix_name, int rows, int cols, int matrix[rows][cols]) {
    printf("%s :\n", matrix_name);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }
}

int main() {
    // Initialisation de B et C avec la valeur 1 et 1 respectivement
    for (int i = 0; i < n1; i++) {
        for (int j = 0; j < m1; j++) {
            B[i][j] = 1;
        }
    }
    printf("Matrice B initialis�e avec la valeur 1 :\n");
    print_matrix("Matrice B", n1, m1, B);

    for (int c = 0; c < n2; c++) {
        for (int d = 0; d < m2; d++) {
            C[c][d] = 1;
        }
    }
    printf("\nMatrice C initialis�e avec la valeur 1 :\n");
    print_matrix("Matrice C", n2, m2, C);

    pthread_t producers[n1], consumers[N];

    for (int i = 0; i < n1; i++) {
        pthread_create(&producers[i], NULL, producer, &i);
    }

    for (int i = 0; i < N; i++) {
        pthread_create(&consumers[i], NULL, consumer, &i);
    }

    for (int i = 0; i < n1; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(consumers[i], NULL);
    }

    // Affichage du contenu du tampon T apr�s le remplissage
    printf("\nContenu du tampon T apr�s le remplissage :\n");
    for (int i = 0; i < N; i++) {
        printf("%d ", T[i]);
    }
    printf("\n");

    // Affichage de la matrice r�sultante A
    printf("\nMatrice r�sultante A :\n");
    print_matrix("Matrice r�sultante A", n1, m2, A);

    getchar();
    getchar();

    return 0;
}

