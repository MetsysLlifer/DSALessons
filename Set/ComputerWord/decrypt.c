#include <stdio.h>

int main(){
    printf("00011101: %c\n\n", 00011101);

    int arr[8] = {0,1,0,0,0,0,0,1};

    unsigned char letter = 0;

    printf("Converting - ");
    for(int i = 0; i < 8; i++){
        printf("%d", arr[i]);
        if(arr[i]) letter |= 1 << (7 - i);
    }
    printf("\n\nto - ");
    for(int i = 7; i >= 0; i--){
        printf("%d", (letter >> i) & 1);
    }
    printf("\n%c ", (char)letter);

    return 0;
}