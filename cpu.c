#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char Flag[] = "bob{F+R_47t@ck}";   // bob{###flag###}
char* cache;

void welcome(){
    printf("Hello Programmer!\n");
    printf("This is neither 32-bit nor 64-bit.\n");
    printf("This is an 8-bit cpu *.*\n");
    printf("You can run the encryption program.\n");
    printf("You can also run your own program.\n");
    printf("Enjoy it ! :laughing:\n");
}

int choice(){
    char buf[4] = {0, };
    printf("\nRun the program\n");
    printf("1. run the encryption program\n");
    printf("2. run your own program\n");
    printf("3. Exit\n");
    write(1, "> ", 2);
    read(0, buf, 2);
    return atoi(buf);
}

void make_process(char * program, char* argv[], int argc){
    printf("program : %s\n", program); // test
}

int main(){

    int chk = 1;
    char* arg[3];
    int arc;
    char enc_program[0x80];
    char own_program[0x80];

    char buf[0x100];
    int fd, i, len;
    char tmp;

    cache = (char*)malloc(0x80);

    if(0 < (fd = open("enc.program", O_RDONLY))){
        read(fd, enc_program, 0x80);
        close(fd);
    }
    else{
        printf("FILE OPEN ERROR\n");
        // exit(-1); // test
    }

    for(i=0; i<3; i++)
        arg[i] = (char*)malloc(0x10);

    welcome();

    while(chk){
        for(i=0; i<3; i++)
            memset(arg[i], 0, 0x10);
        memset(buf, 0, 0x100);
        memset(own_program, 0, 0x80);
        switch(choice()){
            case 1:
            write(1, "msg to be encrypted : ", 22);
            read(0, arg[0], 0x10);
            memcpy(arg[1], Flag, strlen(Flag));
            make_process(enc_program, arg, 2);
            break;

            case 2:
            write(1, "program code (in hex) : ", 25);
            memset(buf, 0, 0x100);
            len = read(0, buf, 0x100);
            buf[len-1] = (buf[len-1]=='\n')? 0:buf[len-1];
            if (strlen(buf)%2 !=0 ){
                write(1, "[ERROR] length of code string must be even number\n", 50);
                continue;
            }
            for(i=0; i<len/2; i++){
                sscanf(buf+i*2, "%2hhx", &tmp);
                sprintf(own_program+i, "%c", tmp);
            }
            write(1, "argc (max: 3) : ", 14);
            scanf("%d", &arc);
            len = (arc>3)?3:arc;
            for(i=0; i<len; i++){
                printf("argv[%d] : ", i);
                fflush(stdout);
                read(0, arg[i], 0x10);
            }
            make_process(own_program, arg, arc);
            break;

            case 3:
            chk = 0;
            break;

            default:
            write(1, "Wrong choice\n", 13);
            break;
        }
    }

    write(1, "Good bye ~ \n", 12);
    return 0;
}