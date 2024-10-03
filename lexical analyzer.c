//Kenta Festag
//HW2 for COP3402
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//halts program
void halt(){
    //exit(0);
}

//function that takes in a string and returns the corresponding token number if exists
char* isKeyword(char word[]){
    if (strcmp(word, "fi") == 0) {
        return "8";
    } 
    else if (strcmp(word, "begin") == 0) {
        return "21";
    } 
    else if (strcmp(word, "end") == 0) {
        return "22";
    } 
    else if (strcmp(word, "if") == 0) {
        return "23";
    } 
    else if (strcmp(word, "then") == 0) {
        return "24";
    } 
    else if (strcmp(word, "while") == 0) {
        return "25";
    } 
    else if (strcmp(word, "do") == 0) {
        return "26";
    } 
    else if (strcmp(word, "call") == 0) {
        return "27";
    } 
    else if (strcmp(word, "const") == 0) {
        return "28";
    } 
    else if (strcmp(word, "var") == 0) {
        return "29";
    } 
    else if (strcmp(word, "procedure") == 0) {
        return "30";
    } 
    else if (strcmp(word, "write") == 0) {
        return "31";
    } 
    else if (strcmp(word, "read") == 0) {
        return "32";
    } 
    else if (strcmp(word, "else") == 0) {
        return "33";
    } 
    return 0;
}

int storeWord(char **tokenList, int tokenCount, char word[]){
    if (strlen(word) >=1){
        if (strlen(word) > 15){
            printf("Error, identifier too long\n");
            halt();
        }
        if ((int)isKeyword(word) > 0){
            strcpy(tokenList[tokenCount], isKeyword(word));
            printf("%s\t\t%s\n", word, isKeyword(word));
            memset(word, '\0', strlen(word));
            return tokenCount + 1;
        }
        else{
            tokenList[tokenCount] = "2";
            strcpy(tokenList[tokenCount+1], word);
            if(strlen(word)<=7){
                printf("%s\t\t2\n", word);
            }
            else{
                printf("%s\t2\n", word);
            }
            memset(word, '\0', strlen(word));
            return tokenCount + 2;
        }
    }
    return tokenCount;
}

int storeNum(char **tokenList, int tokenCount, char number[]){
    if (strlen(number) >=1){
        if (strlen(number) > 5){
            printf("Error, number too long\n");
            halt();
        }
        tokenList[tokenCount] = "3";
        strcpy(tokenList[tokenCount+1], number);
        printf("%s\t\t3\n");
        memset(number, '\0', strlen(number));
        return tokenCount + 2;
    }
    return tokenCount;
}

int storeSymbol(char **tokenList, int tokenCount, char c, FILE* file){
    char nextc;
    if (c == '+'){
        tokenList[tokenCount] = "4";
        printf("+\t\t4\n");
        return tokenCount + 1;
    }
    else if (c == '-'){
        tokenList[tokenCount] = "5";
        printf("-\t\t5\n");
        return tokenCount + 1;
    }
    else if (c == '*'){
        tokenList[tokenCount] = "6";
        printf("*\t\t6\n");
        return tokenCount + 1;
    }
    else if (c == '/'){
        tokenList[tokenCount] = "7";
        printf("/\t\t7\n");
        return tokenCount + 1;
    }
    else if (c == '='){
        tokenList[tokenCount] = "9";
        printf("=\t\t9\n");
        return tokenCount + 1;
    }
    else if (c == '<'){
        nextc = fgetc(file);
        if (nextc == '>'){
            tokenList[tokenCount] = "10";
            printf("<>\t\t10\n");
            return tokenCount + 1;  
        }
        else if (nextc == '='){
            tokenList[tokenCount] = "12";
            printf("<=\t\t12\n");
            return tokenCount + 1; 
        }
        tokenList[tokenCount] = "11";
        printf("<\t\t11\n");
        ungetc(nextc, file);
        return tokenCount + 1;
    }
    else if (c == '>'){
        nextc = fgetc(file);
        if (nextc == '='){
            tokenList[tokenCount] = "14";
            printf(">=\t\t14\n");
            return tokenCount + 1;
        }
        tokenList[tokenCount] = "13";
        printf(">\t\t13\n");
        ungetc(nextc, file);
        return tokenCount + 1;
    }
    else if (c == '('){
        tokenList[tokenCount] = "15";
        printf("(\t\t15\n");
        return tokenCount + 1;
    }
    else if (c == ')'){
        tokenList[tokenCount] = "16";
        printf(")\t\t16\n");
        return tokenCount + 1;
    }
    else if (c == ','){
        tokenList[tokenCount] = "17";
        printf(",\t\t17\n");
        return tokenCount + 1;
    }
    else if (c == ';'){
        tokenList[tokenCount] = "18";
        printf(";\t\t18\n");
        return tokenCount + 1;
    }
    else if (c == '.'){
        tokenList[tokenCount] = "19";
        printf(".\t\t19\n");
        return tokenCount + 1;
    }
    else if (c == ':'){
        nextc = fgetc(file);
        if (nextc == '='){
            tokenList[tokenCount] = "20";
            printf(":=\t\t20\n");
            return tokenCount + 1;
        }
        printf("Error, %c is an unidentified symbol\n", c);
        ungetc(nextc, file);
        halt();
        return tokenCount; 
    }
    printf("Error, %c is an unidentified symbol\n", c);
    halt();
    return tokenCount;
}

int main(int argc, char *argv[])
{
    //Init Variables
    FILE *file;
    char *filename;

    char c;
    char nextc;
    int inComment = 0;

    char word[100] = "";
    char number[100] = "";

    char lalpha[] = "abcdefghijklmnopqrstuvwxyz";
    char ualpha[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    char numlist[] = "0123456789";

    //Init Token List Array
    char **tokenList = (char **)malloc(10000 * sizeof(char *));
    for (int i = 0; i < 10000; i++) {
        tokenList[i] = (char *)malloc(15 * sizeof(char));
    }
    int tokenCount = 0;

    //Init File
    filename = argv[1];
    file = fopen(filename, "r");

    //Print Original File Text
    printf("Source Program:\n");
    while ((c = fgetc(file)) != EOF) {
        putchar(c);
    }

    //Reset File Pointer
    fseek(file, 0, SEEK_SET);
    printf("\n\nLexeme Table:\n");
    printf("\nlexeme\t\ttoken type\n");

    //While loop through file
    while(!feof(file)){
        //obtain first character in text file
        c = fgetc(file);
        //Checks for comments
        if (c == '/'){
            nextc = fgetc(file);
            if (nextc == '*'){
                inComment = 1;
                while(!feof(file)){
                    nextc = fgetc(file);
                    //printf("IC");
                    if (nextc == '*'){
                        nextc = fgetc(file);
                        if (nextc == '/'){
                            inComment = 0;
                            c = fgetc(file);
                            break;
                        }
                        ungetc(nextc, file);    //in case of repeating *
                    }
                }
                if (inComment == 1){
                    printf("Error, comment never finished");
                }
            }
            else{   //if next character wasnt '*', then put char back
                ungetc(nextc, file);
            }
        }

        //Finding what c is a part of
        if(!feof(file)){
            int flag = 0;
            //If c is part of the alphabet
            for (int i = 0; i < strlen(ualpha); i++){      
                if (c == ualpha[i] || c == lalpha[i]){
                    word[strlen(word)] = c;
                    word[strlen(word)+1] = '\0';
                    flag = 1;
                }
            }

            //If c is part of numbers
            for (int i = 0; i < 10; i++){
                if (c == numlist[i]){
                    if (strlen(word) == 0){ //if it isnt a part of word
                        number[strlen(number)] = c;
                        number[strlen(number)+1] = '\0';
                    }
                    else{   //if it is part of word
                        word[strlen(word)] = c;
                        word[strlen(word)+1] = '\0';
                    }
                    flag = 1;
                }
            }

            //If c is white space, newline, newtab
            if (c == ' ' |c == '\n' |c == '\t'){
                tokenCount = storeWord(tokenList, tokenCount, word);
                tokenCount = storeNum(tokenList, tokenCount, number);
                flag = 1;
            }
            //If there is symbol, process symbol and word/num prior
            if (flag == 0){
                tokenCount = storeWord(tokenList, tokenCount, word);
                tokenCount = storeNum(tokenList, tokenCount, number);
                tokenCount = storeSymbol(tokenList, tokenCount, c, file);
            }
        }

        else{
            tokenCount = storeWord(tokenList, tokenCount, word);
            tokenCount = storeNum(tokenList, tokenCount, number);
        }
    }

    //Printing Token List
    printf("\n\nToken List:\n");
    for (int j = 0; j < tokenCount; j++){
        printf("%s ", tokenList[j]);
    }

    //Freeing Token List
    for (int k = 0; k < 10000; k++) {
        free(tokenList[k]);
    }
    free(tokenList);
}