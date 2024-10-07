#include <stdio.h>
#include <string.h>
#include <ctype.h>

int checkJKP (char* kangaroo, char* joey) {
    int i = 0;
    int j = 0;

    while (i < strlen(kangaroo) && j < strlen(joey))
    {
        if(tolower(kangaroo[i]) == tolower(joey[j])) {
            j++;
        }
        i++;
    }

    return (j == strlen(joey)) ? 1 : 0;
}

int isJKP(char* word1, char* word2) {
    if (strlen(word1) > strlen(word2))
    {
        return checkJKP(word1, word2);
    }
    else if (strlen(word1) < strlen(word2)) {
        return checkJKP(word2, word1);
    }
    else {
        return 0;
    }
}

int main(int argc, char* argv[]) {
    printf("%d\n", isJKP(argv[1], argv[2]));
}