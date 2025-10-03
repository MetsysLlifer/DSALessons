#include <stdio.h>
#include <string.h>
#define MAX 10

int main(){
    char word[MAX] = "WORD";
    size_t wordLength = strlen(word);
    printf("%zu", wordLength);
}