#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool ensureNoDuplicates(char fname[], char type[], FILE* fp);
int countLines(FILE* fp);
bool fileIsSpecified(FILE* fp, int lineCount, char fname[]);
bool typeIsSpecified(FILE* fp, char type[]);
int numOfTypes(FILE* fp);
void typeDeclare(FILE *fp){
    printf("Enter file name preceded by which type you would like to mark with.\n");
    char fname[100];
    char type[100];
    scanf("%s %s", fname, type);
    /*
    Future improvement: Check to make sure file exists in directory!
    */
    //ensure no duplicates
   if( ensureNoDuplicates(fname, type, fp)) fprintf(fp, "%s %s\n", fname, type);
   //no need to display that current classification exists if false is returned

}
/*
After it is determined, in typeDeclaration(), that the file is specified, then
typeIsSpecified is to determine if the type (given by user) is specified after 
the given file. If so, no writing must be done; if not, then typeDeclaration 
should append a type onto that line. 
*/
bool typeIsSpecified(FILE* fp, char type[]){
    int numberOfTypes = numOfTypes(fp);
    bool typeSpec = false;
    char* typeInFile;
    for(int i = 0; i < numberOfTypes; i++){
        fscanf(fp, "%s", typeInFile);
        if(strcmp(typeInFile, type) == 0) typeSpec = true;
    }
    return typeSpec;
}



int numOfTypes(FILE* fp){
    int typeCount = 0; 
    int charCount = 0; //want to send the file pointer back at the location
    //it started at. For type checking purposes.
    char c;
    fscanf(fp, "%c", &c);
    charCount++;
    while(c != '\n'){
        if(c == ' '){
            typeCount++;
        }
        fscanf(fp, "%c", &c);
        charCount++;
    }
    fseek(fp, 0-charCount, SEEK_CUR); //set pointer to end of filename.
    return typeCount;

}
//checks to see if the filename exists in findex.txt.
//note that this function will return the pointer immediately after the file
//name. this is ideal for writing or reading types attributed.
bool fileIsSpecified(FILE* fp, int lineCount, char fname[]){
    fseek(fp, 0, SEEK_SET); //set pointer to beginning of file
    char wholeLine[100];
    char fileOnLine[50];
    bool isSpecified = false;
    for(int i = 0; i < lineCount; i++){
        fgets(wholeLine, 100, fp);
        sscanf(wholeLine, "%s", fileOnLine);
        if(strcmp(fileOnLine, fname) == 0) isSpecified = true;
    }
    int offSetToEndOfFName = strlen(wholeLine) - strlen(fname);
    fseek(fp, 0 - offSetToEndOfFName, SEEK_CUR); //return to immediately
    //after file name.
    return isSpecified;
}
//counts lines in findex.txt
int countLines(FILE* fp){
    int lineCount = 0; 
    fseek(fp, 0, SEEK_SET); //set pointer to beginning of file
    
    char c;
    bool newLDoubleOccurrence = false; //to track if \n occurs twice
    fscanf(fp, "%c", &c);
    while(c != EOF && c != 0 && (!(newLDoubleOccurrence && (c == '\n')))){ //if not EOF and \n
        //doesn't occur twice
        if(c == '\n'){
            lineCount++;
            newLDoubleOccurrence = true;
        }
        else newLDoubleOccurrence = false;
        fscanf(fp, "%c", &c);
    }
    fseek(fp, 0, SEEK_SET); //set pointer to beginning of file
    return lineCount;
}

bool ensureNoDuplicates(char fname[], char type[], FILE* fp){
    int lineCount = countLines(fp); 
    //if there are no new lines in findex.txt then return true
    if(lineCount == 0) return true;
    //now iterate through and ensure no duplicates
    bool noDups = true;
    bool fileSpec = false;
    bool typeSpec = false;

    if(fileSpec = fileIsSpecified(fp, lineCount, fname))
        typeSpec = typeIsSpecified(fp, type);
    noDups = fileIsSpecified && typeSpec;

    
}

int main(){
    //attempt to open findex.txt. 
    FILE * fp;
    fp = fopen("findex.txt", "r");

    if(fp == NULL){
        printf("findex.txt does not exist. Would you like to create it? [Y, n]");
        char response = getchar();
        if(response == 'Y' || response == 'y'){
            fp = fopen("findex.txt", "w");
            if(fp == NULL){
                perror("Cannot create file.");
                return -1;
            }
        }
        else return 0;
    }else{
        fclose(fp);
        fopen("findex.txt", "a+");
    }

    typeDeclare(fp);
    fclose(fp);
    return 0;
}


