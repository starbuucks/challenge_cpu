#include <stdio.h>
#include <stdlib.h>

char Flag[] = "bob{F+R_47t@ck}";   // bob{###flag###}
char* cache;

void welcome(){
    printf("Hello Programmer!\n");
    printf("You can run the encryption program.\n");
    printf("You can also run your own program.\n");
    printf("Enjoy it ! :laughing:\n");
}

int choice(){
    char buf[4] = {0, };
    printf("\nRun the program\n");
    printf("1. run encryption program\n");
    printf("2. run your own program\n");
    printf("3. Exit\n");
    printf("> ");
    read(0, buf, 2);
    return atoi(buf);
}



int main(){

    int chk = 1;

    cache = (char*)malloc(0x100);

    welcome();

    while(chk){
        switch(choice()){
            case 1:

            break;
            case 2:

            break;
            case 3:
            chk = 0;
            break;
            default:
            printf("Wrong choice\n");
            break;
        }
    }

    printf("Good bye ~ \n");
    return 0;
}