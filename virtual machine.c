//Kenta Festag
//HW1 for COP3402


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ARRAY_SIZE = 500;

int main(int argc, char *argv[])
{
    //Init Variables
    FILE *filePointer;
    char *filename;
    int count = 10;
    int pas[ARRAY_SIZE];
    int pc = 10;
    int sp = 500;
    int bp = 499;
    int ir[3];
    int running = 1;
    char OP[] = "";
    

    //Initialize PAS with zeroes
    for (int i = 0; i < ARRAY_SIZE; i++) {
        pas[i] = 0;
    }

    //Take file input
    filename = argv[1];
    filePointer = fopen(filename, "r");
    while (!feof(filePointer)){
        fscanf(filePointer, "%d", &pas[count]);
        count++;
    }

    //Initialize first values and print
    sp = 500;
    bp = 499;
    printf("\t\t\tPC\tBP\tSP\tstack\n");
    printf("Initial values:\t\t%d\t%d\t%d\n", pc, bp, sp);
    fclose(filePointer);    //close text file

    //CPU
    while(running > 0){
        //Fetch cycle
        ir[0] = pas[pc];    //OP   
        ir[1] = pas[pc+1];  //L
        ir[2] = pas[pc+2];  //M
        pc = pc+3;
        int arb;
        
        //Execution cycle
        //I made a switch case of the different OPs following the given ISA code for each case
        //I added the base(bp, L) function of sorts with the arb variable for cases 3,4 and 5
        switch(ir[0]){
            case 1:
                strcpy(OP, "LIT");
                sp = sp - 1;
                pas[sp] = ir[2];
                break;
            case 2:
                if (ir[2] == 0){
                    strcpy(OP, "RTN");
                    sp = bp + 1;
                    bp = pas[sp-2];
                    pc = pas[sp-3];
                }
                if (ir[2] == 1){
                    strcpy(OP, "ADD");
                    pas[sp+1] = (pas[sp+1] + pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 2){
                    strcpy(OP, "SUB");
                    pas[sp+1] = (pas[sp+1] - pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 3){
                    strcpy(OP, "MUL");
                    pas[sp+1] = (pas[sp+1] * pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 4){
                    strcpy(OP, "DIV");
                    pas[sp+1] = (pas[sp+1] / pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 5){
                    strcpy(OP, "EQL");
                    pas[sp+1] = (pas[sp+1] == pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 6){
                    strcpy(OP, "NEQ");
                    pas[sp+1] = (pas[sp+1] != pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 7){
                    strcpy(OP, "LSS");
                    pas[sp+1] = (pas[sp+1] < pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 8){
                    strcpy(OP, "LEQ");
                    pas[sp+1] = (pas[sp+1] <= pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 9){
                    strcpy(OP, "GTR");
                    pas[sp+1] = (pas[sp+1] > pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 10){
                    strcpy(OP, "GEQ");
                    pas[sp+1] = (pas[sp+1] >= pas[sp]);
                    sp = sp + 1;
                }
                break;
            case 3:
                strcpy(OP, "LOD");
                sp = sp - 1;
                arb = bp;
                while (ir[1] > 0){
                    arb = pas[arb];
                    ir[1]--;
                }
                pas[sp] = pas[arb-ir[2]];
                break;
            case 4:
                strcpy(OP, "STO");
                arb = bp;
                while (ir[1] > 0){
                    arb = pas[arb];
                    ir[1]--;
                }
                pas[arb-ir[2]] = pas[sp];
                sp = sp + 1;
                break;
            case 5:
                strcpy(OP, "CAL");
                arb = bp;
                while (ir[1] > 0){
                    arb = pas[arb];
                    ir[1]--;
                }
                pas[sp-1] = arb;
                pas[sp-2] = bp;
                pas[sp-3] = pc;
                bp = sp - 1;
                pc = ir[2];
                break;
            case 6:
                strcpy(OP, "INC");
                sp = sp - ir[2];
                break;
            case 7:
                strcpy(OP, "JMP");
                pc = ir[2];
                break;
            case 8:
                strcpy(OP, "JPC");
                if (pas[sp] == 0){
                    pc = ir[2];
                }
                sp = sp + 1;
                break;
            case 9:
                strcpy(OP, "SYS");
                if (ir[2] == 1){
                    printf("Output result is: %d\n", pas[sp]);
                    sp = sp + 1;
                }
                if (ir[2] == 2){
                    sp = sp - 1;
                    int temp = 0;
                    printf("Please Enter an Integer: ");
                    scanf("%d", &temp);
                    pas[sp] = temp;
                }
                if (ir[2] == 3){
                    running = 0;
                }
                break;
            default:
                //Some way to stop the program if there is a OP outside of the given 9
                printf("Not the right OP");
                running = 0;
        }

        //Print PAS
        printf("%s\t%d\t%d\t%d\t%d\t%d", OP, ir[1], ir[2], pc, bp, sp);
        printf("\t");
        //For loop to print the stack
        for (int j = 499; j >= sp; j--){
            if(j == bp & j != 499){
                printf(" | ");
            }
            printf("%d ", pas[j]);
        }
        printf("\n");
    }
    return 0;
}