//Kenta Festag
//HW2 for COP3402
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constants
#define MAX_SYMBOL_TABLE_SIZE 500
#define MAX_LINES_OF_CODE 20000
#define MAX_TOKEN_TABLE_SIZE 10000
//To see some extra output if needed, make 1 to see effects
int printAllTokens = 0;
int printAfterHalt = 0;

//Global level variable
int Llevel = -1;

//Symbol Table Struct
typedef struct
{ 
    int kind; 		// const = 1, var = 2, proc = 3
    char name[15];	// name up to 11 chars
    int value; 		// number (ASCII value) 
    int level; 		// L level
    int address; 		// M address
    int mark;		// to indicate unavailable or deleted
}symbol;

//Globally initializes symbol table and token list
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int SymbolTableIndex = 0;
char **tokenList;
int tokenCount = 0;
int currentToken = 0;

//initializes token list
void initTokenList(){
    tokenList = (char **)malloc(MAX_TOKEN_TABLE_SIZE * sizeof(char *));
    for (int i = 0; i < 10000; i++) {
        tokenList[i] = (char *)malloc(15 * sizeof(char));
    }
 } 

//frees token list
void freeTokenList(){
    for (int i = 0; i < 10000; i++) {
        free(tokenList[i]);
    }
    free(tokenList);
}

//structs to hold code to emit
typedef struct { 
    char OPC[3];
    int OP; 	
    int L; 	
    int M; 	
} codeLine;

//globally initializes emittable code
codeLine *codeList;
int line = 0;
void initCodeList(){
    codeList = (codeLine *)malloc(MAX_LINES_OF_CODE * sizeof(codeLine));
} 

//frees token list
void freeCodeList(){
    free(codeList);
}

//emits code to the code list
void emit(int line, char OPC[], int OP, int L, int M){
    strcpy(codeList[line].OPC, OPC);
    codeList[line].OP = OP;
    codeList[line].L = L;
    codeList[line].M = M;
}

//outputs the code to cmd and text ELF file
void outputCode(FILE *ELFfile){
    printf("\nLine    OP    L    M\n");
    for (int i = 0; i < line; i++){
        printf("  %*d   %s    %d    %d\n", 2, i, codeList[i].OPC, codeList[i].L, codeList[i].M);
        fprintf(ELFfile, "%d %d %d\n", codeList[i].OP, codeList[i].L, codeList[i].M);
    }
}

//halts program
void halt(){
    if (printAfterHalt == 1){
        FILE *ELFfile = fopen("ELF.txt", "w");
        outputCode(ELFfile);

        printf("\nKind | Name        | Value | Level | Address | Mark\n");
        printf("---------------------------------------------------\n");

        for (int i = 1; i <= SymbolTableIndex; i++){
            printf("   %d | %*s | %*d | %*d | %*d |    %d\n", symbol_table[i].kind, 11, symbol_table[i].name, 5, symbol_table[i].value, 5, symbol_table[i].level, 7, symbol_table[i].address, symbol_table[i].mark);
        }

        freeTokenList();
        freeCodeList();
        fclose(ELFfile);
    }    
    exit(0);
}

//Error list
void Error(int ErrorNum){
    switch (ErrorNum){
        case 1:
            printf("Error number %d, use = instead of :=\n", ErrorNum);
            halt();
        case 2:
            printf("Error number %d, = must be followed by a number\n", ErrorNum);
            halt();
        case 3:
            printf("Error number %d, identifier must be followed by =\n", ErrorNum);
            halt();
        case 4:
            printf("Error number %d, const, var, procedure must be followed by identifier\n", ErrorNum);
            halt();
        case 5:
            printf("Error number %d, semicolon or comma missing\n", ErrorNum);
            halt();
        case 6:
            printf("Error number %d, incorrect symbol after procedure declaration.\n", ErrorNum);
            halt();
        case 7:
            printf("Error number %d, statement expected\n", ErrorNum);
            halt();
        case 8:
            printf("Error number %d, incorrect symbol after statement part in block\n", ErrorNum);
            halt();
        case 9:
            printf("Error number %d, period expected\n", ErrorNum);
            halt();
        case 10:
            printf("Error number %d, semicolon between statements missing\n", ErrorNum);
            halt();
        case 11:
            printf("Error number %d, undeclared identifier\n", ErrorNum);
            halt();
        case 12:
            printf("Error number %d, assignment to constant or procedure is not allowed\n", ErrorNum);
            halt();
        case 13:
            printf("Error number %d, assignment operator expected\n", ErrorNum);
            halt();
        case 14:
            printf("Error number %d, call must be followed by an identifier\n", ErrorNum);
            halt();
        case 15:
            printf("Error number %d, call of a constant or variable is meaningless\n", ErrorNum);
            halt();
        case 16:
            printf("Error number %d, then expected\n", ErrorNum);
            halt();
        case 17:
            printf("Error number %d, semicolon or end expected\n", ErrorNum);
            halt();
        case 18:
            printf("Error number %d, do expected\n", ErrorNum);
            halt();
        case 19:
            printf("Error number %d, incorrect symbol following statement\n", ErrorNum);
            halt();
        case 20:
            printf("Error number %d, relational operator expected\n", ErrorNum);
            halt();
        case 21:
            printf("Error number %d, expression must not contain a procedure identifier\n", ErrorNum);
            halt();
        case 22:
            printf("Error number %d, right parenthesis missing\n", ErrorNum);
            halt();
        case 23:
            printf("Error number %d, the preceding factor cannot begin with this symbol\n", ErrorNum);
            halt();
        case 24:
            printf("Error number %d, an expression cannot begin with this symbol\n", ErrorNum);
            halt();
        case 25:
            printf("Error number %d, this number is too large\n", ErrorNum);
            halt();
        case 26:
            printf("Error number %d, identifier too long\n", ErrorNum);
            halt();
        case 27:
            printf("Error number %d, invalid symbol\n", ErrorNum);
            halt();
        default:
            printf("Error number %d, some error appeared\n", ErrorNum);
            halt();
    }
    return;
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
    return "0";
}

int storeWord(char word[]){
    if (strlen(word) >=1){
        if (strlen(word) > 15){
            Error(26);
        }
        if (strcmp(isKeyword(word), "0") != 0){
            strcpy(tokenList[tokenCount], isKeyword(word));
            memset(word, '\0', strlen(word));
            return tokenCount + 1;
        }
        else{
            strcpy(tokenList[tokenCount], "2");
            strcpy(tokenList[tokenCount+1], word);
            memset(word, '\0', strlen(word));
            return tokenCount + 2;
        }
    }
    return tokenCount;
}

int storeNum(char number[]){
    if (strlen(number) >=1){
        if (strlen(number) > 5){
            Error(25);
        }
        strcpy(tokenList[tokenCount], "3");
        strcpy(tokenList[tokenCount+1], number);
        memset(number, '\0', strlen(number));
        return tokenCount + 2;
    }
    return tokenCount;
}

int storeSymbol(char c, FILE* file){
    char nextc;
    if (c == '+'){
        strcpy(tokenList[tokenCount], "4");
        return tokenCount + 1;
    }
    else if (c == '-'){
        strcpy(tokenList[tokenCount], "5");
        return tokenCount + 1;
    }
    else if (c == '*'){
        strcpy(tokenList[tokenCount], "6");
        return tokenCount + 1;
    }
    else if (c == '/'){
        strcpy(tokenList[tokenCount], "7");
        return tokenCount + 1;
    }
    else if (c == '='){
        strcpy(tokenList[tokenCount], "9");
        return tokenCount + 1;
    }
    else if (c == '<'){
        nextc = fgetc(file);
        if (nextc == '>'){
            strcpy(tokenList[tokenCount], "10");
            return tokenCount + 1;  
        }
        else if (nextc == '='){
            strcpy(tokenList[tokenCount], "12");
            return tokenCount + 1; 
        }
        strcpy(tokenList[tokenCount], "11");
        ungetc(nextc, file);
        return tokenCount + 1;
    }
    else if (c == '>'){
        nextc = fgetc(file);
        if (nextc == '='){
            strcpy(tokenList[tokenCount], "14");
            return tokenCount + 1;
        }
        strcpy(tokenList[tokenCount], "13");
        ungetc(nextc, file);
        return tokenCount + 1;
    }
    else if (c == '('){
        strcpy(tokenList[tokenCount], "15");
        return tokenCount + 1;
    }
    else if (c == ')'){
        strcpy(tokenList[tokenCount], "16");
        return tokenCount + 1;
    }
    else if (c == ','){
        strcpy(tokenList[tokenCount], "17");
        return tokenCount + 1;
    }
    else if (c == ';'){
        strcpy(tokenList[tokenCount], "18");
        return tokenCount + 1;
    }
    else if (c == '.'){
        strcpy(tokenList[tokenCount], "19");
        return tokenCount + 1;
    }
    else if (c == ':'){
        nextc = fgetc(file);
        if (nextc == '='){
            strcpy(tokenList[tokenCount], "20");
            return tokenCount + 1;
        }
        //printf("Error: <%c is an unidentified symbol>\n", c);
        Error(27);
        ungetc(nextc, file);
        return tokenCount; 
    }
    //printf("Error: <%c is an unidentified symbol>\n", c);
    Error(27);
    return tokenCount;
}

int SymbolTableCheck(char varName[]){
    for (int i = SymbolTableIndex; i >= 0; i--){
        if (strcmp(symbol_table[i].name, varName) == 0){
            return i;
        }
    }
    return -1;
}

char* getToken(){
    if (currentToken <= tokenCount){
        currentToken += 1;
        return tokenList[currentToken-1];
    }
    return "0";
}

void unGetToken(){
    currentToken = currentToken - 1;
}

void Expression();

void Factor(){
    char* token = getToken();
    if (strcmp(token, "2") == 0){   //ident
        token = getToken();
        int symIdx = SymbolTableCheck(token);
        if (symIdx == -1){
            //printf("Error: <undeclared identifier %s>\n", token);
            Error(11);
        }
        if (symbol_table[symIdx].kind == 1){
            emit(line++, "LIT", 1, 0, symbol_table[symIdx].value);
        }
        if (symbol_table[symIdx].kind == 3){
            Error(21);
        }
        else{
            symbol_table[symIdx].mark = 1;
            emit(line++, "LOD", 3, Llevel - symbol_table[symIdx].level, symbol_table[symIdx].address);
        }
    }
    else if (strcmp(token, "3") == 0){  //number
        token = getToken();
        emit(line++, "LIT", 1, 0, atoi(token));
    }
    else if (strcmp(token, "15") == 0){ //(
        Expression();
        token = getToken();
        if (strcmp(token, "16") != 0){  //)
            Error(22);
        }
    }
    else{
        //printf("Error: <arithmetic equations must contain operands, parentheses, numbers, or symbols>\n");
        Error(23);
        unGetToken();
    }
}

void Term(){
    Factor();
    char* token = getToken();
    while ((strcmp(token, "6") == 0) || (strcmp(token, "7") == 0)){  //* or /
        if (strcmp(token, "6") == 0){
            Factor();
            emit(line++, "MUL", 2, 0, 3);
            token = getToken();
        }
        else{
            Factor();
            emit(line++, "DIV", 2, 0, 4);
            token = getToken();
        }
    }
    unGetToken();
}

void Expression(){
    Term();
    char* token = getToken();
    while ((strcmp(token, "4") == 0) || (strcmp(token, "5") == 0)){  //+ or -
        if (strcmp(token, "4") == 0){
            Term();
            emit(line++, "ADD", 2, 0, 1);
            token = getToken();
        }
        else{
            Term();
            emit(line++, "SUB", 2, 0, 2);
            token = getToken();
        }
    }
    unGetToken();
}

void Condition(){
    char* token = getToken();
    if (strcmp(token, "1") == 0){   //odd
        Expression();
        emit(line++, "ODD", 2, 0, 11);
    }
    else{
        unGetToken();
        Expression();
        token = getToken();
        if (strcmp(token, "9") == 0){   //=
            Expression();
            emit(line++, "EQL", 2, 0, 5);
        }
        else if (strcmp(token, "10") == 0){ //<>
            Expression();
            emit(line++, "NEQ",2 , 0, 6);
        }
        else if (strcmp(token, "11") == 0){ //<
            Expression();
            emit(line++, "LSS", 2, 0, 7);
        }
        else if (strcmp(token, "12") == 0){ //<=
            Expression();
            emit(line++, "LEQ", 2, 0, 8);
        }
        else if (strcmp(token, "13") == 0){ //>
            Expression();
            emit(line++, "GTR", 2, 0, 9);
        }
        else if (strcmp(token, "14") == 0){ //>=
            Expression();
            emit(line++, "GEQ", 2, 0, 10);
        }
        else{
            Error(20);
        }
    }
}

void Statement(){
    char* token = getToken();
    if (strcmp(token, "2") == 0){   //ident
        token = getToken();
        int symIdx = SymbolTableCheck(token);
        if (symIdx == -1){
            //printf("Error: <undeclared identifier %s>\n", token);
            Error(11);
        }
        if (symbol_table[symIdx].kind != 2){
            Error(12);
        }
        symbol_table[symIdx].mark = 1;
        token = getToken();
        if (strcmp(token, "20") != 0){  //:=
            Error(13);
        }
        
        emit(line++, "STO", 4, Llevel - symbol_table[symIdx].level, symbol_table[symIdx].address);
        Expression();
        return;
    }
    else if (strcmp(token, "27") == 0){ //call
        token = getToken();
        if (strcmp(token, "2") != 0){
            Error(14);
        }
        token = getToken();
        int symIdx = SymbolTableCheck(token);
        if (symIdx == -1){
            //printf("Error: <undeclared identifier %s>\n", token);
            Error(11);
        }
        if (symbol_table[symIdx].kind != 3){
            Error(15);
        }
        emit(line++, "CAL", 4, Llevel - symbol_table[SymbolTableCheck(token)].level, symbol_table[SymbolTableCheck(token)].address);
        return;
    }
    else if(strcmp(token, "21") == 0){  //begin
        do{
            Statement();
            token = getToken();
        }while(strcmp(token, "18") == 0);   //;
        if (strcmp(token, "22") != 0){  //end
            Error(17);
        }
        return;
    }
    else if(strcmp(token, "23") == 0){  //if
        Condition();
        int jpcIdx = line;
        emit(line++, "JPC", 8, 0, 0);
        token = getToken();
        if (strcmp(token, "24") != 0){  //then
            Error(16);
        }
        Statement();
        token = getToken();
        if (strcmp(token, "8") != 0){  //fi
            printf("Error: <then must be followed by fi>\n");
            halt();
        }
        //code[jpcIdx].M = current code index
        emit(jpcIdx, "JPC", 8, 0, (line*3)+10);
        return;
    }
    else if(strcmp(token, "25") == 0){  //while
        int loopIdx = line;
        Condition();
        token = getToken();
        if (strcmp(token, "26") != 0){  //do
            printf("Error: <while must be followed by do>\n");
            halt();
        }
        int jpcIdx = line;
        emit(line++, "JPC", 8, 0, 0);
        Statement();
        emit(line++, "JMP", 7, 0, (loopIdx*3)+10);
        //code[jpcIdx].M = current code index
        emit(jpcIdx, "JPC", 8, 0, (line*3)+10);
        return;
    }
    else if(strcmp(token, "32") == 0){  //read
        token = getToken();
        if (strcmp(token, "2") != 0){   //ident
            printf("Error: <read keyword must be followed by identifier>\n");
            halt();
        }
        token = getToken();
        int symIdx = SymbolTableCheck(token);
        if (symIdx == -1){
            //printf("Error: <undeclared identifier %s>\n", token);
            Error(11);
        }
        if (symbol_table[symIdx].kind != 2){
            Error(12);
        }
        emit(line++, "SYS", 9, 0, 2);
        emit(line++, "STO", 4, 0, symbol_table[symIdx].address);
        return;
    }
    else if(strcmp(token, "31") == 0){  //write
        Expression();
        emit(line++, "SYS", 9, 0, 1);
        return;
    }
    else{
        unGetToken();
    }
}

int VarDeclaration(){
    int numVars = 0;
    char* token = getToken();
    if (strcmp(token, "29") == 0){  //var
        do{
            numVars++;
            token = getToken();
            if (strcmp(token, "2") != 0){   //ident
                Error(4);
            }
            token = getToken();
            if (SymbolTableCheck(token) != -1){         
                if (symbol_table[SymbolTableCheck(token)].level == Llevel){
                    printf("Error: <symbol name has already been declared>\n");
                    halt();
                }
            }
            SymbolTableIndex += 1;
            //printf("Inserting %s, at index %d, at level %d\n", token, SymbolTableIndex, Llevel);
            symbol_table[SymbolTableIndex].kind = 2;
            strcpy(symbol_table[SymbolTableIndex].name, token);
            symbol_table[SymbolTableIndex].address = numVars+2;
            symbol_table[SymbolTableIndex].level = Llevel;
            symbol_table[SymbolTableIndex].mark = 0;
            token = getToken();
        }while(strcmp(token, "17") == 0);   //,
        if (strcmp(token, "18") != 0){  //;
            Error(5);
        }
    }
    else{
        unGetToken();
    }
    return numVars;
}

void ConstDeclaration(){
    char* token = getToken();
    if (strcmp(token, "28") == 0){  //const
        do{
            token = getToken();
            if (strcmp(token, "2") != 0){   //ident
                Error(4);
            }
            token = getToken();
            if (SymbolTableCheck(token) != -1){
                printf("Error: <symbol name has already been declared>\n");
                halt();
            }
            char nameOfVar[15];
            strcpy(nameOfVar, token);
            token = getToken();
            if (strcmp(token, "9") != 0){   //=
                Error(1);
            }
            token = getToken();
            if (strcmp(token, "3") != 0){   //number
                printf("Error: <constants must be assigned an integer value>\n");
                halt();
            }
            token = getToken();
            SymbolTableIndex += 1;
            symbol_table[SymbolTableIndex].kind = 1;
            strcpy(symbol_table[SymbolTableIndex].name, nameOfVar);
            symbol_table[SymbolTableIndex].value = atoi(token);
            symbol_table[SymbolTableIndex].level = Llevel;
            token = getToken();
        }while (strcmp(token, "17") == 0);  //,
        if (strcmp(token, "18") != 0){  //;
            Error(5);
        }
    }
    else{
        unGetToken();
    }
}

void Block(){
    Llevel += 1;
    int prev_sx = SymbolTableIndex;
    int space = 3;
    int jmpIdx = line;
    emit(line++, "JMP", 7, 0, 666);
    ConstDeclaration();
    int numVars = VarDeclaration();
    emit(line++, "INC", 6, 0, space += numVars);
    char* token = getToken();
    int run = 0;
    if (strcmp(token, "30") == 0){  //procedure
        do{
            token = getToken();
            if (strcmp(token, "2") != 0){   //ident
                Error(6);
            }
            token = getToken();
            //Declare procedure variable in symbol table
            SymbolTableIndex += 1;
            symbol_table[SymbolTableIndex].kind = 3;
            strcpy(symbol_table[SymbolTableIndex].name, token);
            symbol_table[SymbolTableIndex].level = Llevel;
            symbol_table[SymbolTableIndex].address = (line*3)+10;
            token = getToken();
            if (strcmp(token, "18") != 0){  //;
                Error(5);
            }
            Block();
            token = getToken();
            if (strcmp(token, "18") != 0){  //;
                Error(17);
            }
            emit(line++, "RTN", 2, 0, 0);
            token = getToken();
        }while(strcmp(token, "30") == 0);
        unGetToken();
    }
    else{
        unGetToken();
    }
    emit(jmpIdx, "JMP", 7, 0, (line*3)+10);
    //emit(line++, "INC", 6, 0, space);
    Statement();
    SymbolTableIndex = prev_sx;
    Llevel -= 1;
}

void Program(){
    Block();
    char* token = getToken();
    if (strcmp(token, "19") != 0){
        printf("Error: <program must end with period>\n");
        halt();
    }
    emit(line++, "SYS", 9,  0, 3);
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

    initTokenList();

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
                    printf("Error: <comment never finished>\n");
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
            if (c == ' ' |c == '\n' |c == '\t' | c == 13){
                tokenCount = storeNum(number);
                tokenCount = storeWord(word);
                flag = 1;
            }
            //If there is symbol, process symbol and word/num prior
            if (flag == 0){
                tokenCount = storeNum(number);
                tokenCount = storeWord(word);
                tokenCount = storeSymbol(c, file);
            }
        }

        else{
            tokenCount = storeNum(number);
            tokenCount = storeWord(word);
        }
    }

    FILE *ELFfile = fopen("ELF.txt", "w");

    //Prints Tokens if needed
    if (printAllTokens == 1){
        for (int i = 0; i < tokenCount; i++){
            printf("Token %d: (%s)\n", i+1, tokenList[i]);
        }
    }
    
    initCodeList();

    printf("\nCode Generation:\n");
    emit(line++, "JMP", 7, 0, 13);
    Program();
    outputCode(ELFfile);

    /*printf("\nKind | Name        | Value | Level | Address | Mark\n");
    printf("---------------------------------------------------\n");

    for (int i = 1; i <= SymbolTableIndex; i++){
        printf("   %d | %*s | %*d | %*d | %*d |    %d\n", symbol_table[i].kind, 11, symbol_table[i].name, 5, symbol_table[i].value, 5, symbol_table[i].level, 7, symbol_table[i].address, symbol_table[i].mark);
    }*/

    freeTokenList();
    freeCodeList();
    fclose(ELFfile);
    fclose(file);
}