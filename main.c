#include <stdio.h>

#include <stdlib.h>
#include <string.h>

char* ExtendArray(char*buffer,int32_t *size) {
    *size= *size*2+1;
    char* temp = realloc(buffer, *size);
    if (temp != NULL) {
        buffer = temp;
        return buffer;
    }
    printf("Out of memory\n");
    return NULL;
}

char* AddTextToEnd(char** document, u_int16_t currentLine) {
    int32_t capacity = 0;
    char myLetter;
    char* buffer = document[currentLine];
    int32_t size=(int32_t)strlen(buffer)+10;

    uint16_t findEnd=0;

    while (buffer[findEnd]!='\0') {
        capacity++;
        findEnd++;
    }
    printf("Enter text: ");

    while (1) {
        if (capacity >= size - 1) {
            buffer=ExtendArray(buffer, &size);
        }
        myLetter = getchar();
        if (myLetter == '\n') {
            break;
        }
        buffer[capacity] = myLetter;
        capacity++;
    }


    if (capacity<size) {
        buffer[capacity] = '\0';
    }else {
        size = size+10;
        char* temp = realloc(buffer, size);
        if (temp != NULL) {
            buffer = temp;
            buffer[capacity]='\0';
        }
    }
    return buffer;
}


void startFromNewLine(char** document,u_int16_t* currentLine) {
    *currentLine+=1;
    document[*currentLine]=malloc(sizeof(char) * 10);
    document[*currentLine][0]='\0';
}

void WritetextToFile(char** document, u_int16_t currentLine) {
    FILE *file = fopen("..//myfile.txt", "w");
    if (file != NULL)
    {
        for (int16_t i=1;i<currentLine+1;i++) {
            fputs(document[i],file);
            fputs("\n",file);
           }
        fclose(file);
        }
}


// void ReadteaxtFromfile(char** document,u_int16_t* currentLine) {
//     FILE* file;
//     char mystring[1000];
//     file = fopen("..//myfile.txt", "r");
//     if (file == NULL)
//     {
//         printf("Error opening file");
//     }
//     else
//     {
//         while (fgets(mystring, 100, file) != NULL){
//             printf("%s", mystring);
//             // AddTextToEnd(document,*currentLine);
//             document[*currentLine]=mystring;
//             printf("\n");
//         }
//         fclose(file);
//     }
// }



// void ReadteaxtFromfile(char** document, u_int16_t* currentLine) {
//     FILE* file;
//     char mystring[1000];
//     file = fopen("..//myfile.txt", "r");
//     if (file == NULL) {
//         printf("Error opening file");
//         return;
//     }
//     // *currentLine = 0;
//     startFromNewLine(document, currentLine);
//
//     while (fgets(mystring, sizeof(mystring), file) != NULL) {
//         // Strip the trailing newline that fgets keeps
//         size_t len = strlen(mystring);
//         if (len > 0 && mystring[len - 1] == '\n') {
//             mystring[len - 1] = '\0';
//             len--;
//         }
//
//         // Allocate a proper heap copy and assign to current line
//         char* lineCopy = malloc(len + 1);
//         if (lineCopy == NULL) {
//             printf("Out of memory\n");
//             break;
//         }
//         strcpy(lineCopy, mystring);
//         document[*currentLine] = lineCopy;
//
//         // Move to the next line for the next iteration (but not past EOF)
//         if (!feof(file)) {
//             startFromNewLine(document, currentLine);
//         }
//     }
//
//     fclose(file);
//
//     // Print loaded content
//     printf("Loaded from file:\n");
//     for (uint16_t i = 1; i < *currentLine + 1; i++) {
//         printf("%d. %s\n", i, document[i]);
//     }
// }



int main(void){

    printf("Hello, It's text editor\n");
    uint16_t repeat=1;
    u_int16_t currentLine=0;
    int32_t size=5;
    char** document = malloc(sizeof(char*) * size);

    startFromNewLine(document,&currentLine);

    while (repeat) {
        printf("Command 1 is to append text symbols to the end\n"
               "Command 2 is to start the new line\n"
               "Command 3 is to use files to save the information\n"
               "Command 4 is to use files to load\n"
               "Command 5 is to print the current text to the console\n"
               "Command 6 is to insert the text by line and symbol index\n"
               "Command 7 is to search\n"
               "Command 8 is to exit\n");

        printf("Choose the command: ");
        char choose;
        scanf(" %c", &choose);
        getchar();

        switch (choose) {
            case '1':
                printf("%d\n",currentLine);
                document[currentLine]=AddTextToEnd(document,currentLine);
                break;
            case '2':
                printf("Command 2 to start the new line\n\n");
                if (currentLine<size-1) {
                    startFromNewLine(document,&currentLine);
                    printf("%d\n",currentLine);
                }else {
                    size=size*2;
                    char** temp = realloc(document, sizeof(char*)*size );
                    if (temp != NULL) {
                        document=temp;
                        startFromNewLine(document,&currentLine);
                        printf("%d\n",currentLine);
                    }else {
                        printf("Out of memory\n");
                    }

                }
                break;
            case '3':
                printf("Command 3 is to use files to save the information\n\n");
                WritetextToFile(document,currentLine);
                repeat=1;
                continue;
            case '4':
                printf("Command 4 is to use files to load\n\n");
                // ReadteaxtFromfile(document,&currentLine);
                repeat=1;
                continue;
            case '5':
                printf("Command 5 is to print the current text to the console\n\n");
                for (int16_t i=1;i<currentLine+1;i++) {
                    printf("%d.",i);
                    printf("You entered: %s", document[i]);
                    printf("\n");
                }
                printf("\n");
                repeat=1;
                continue;
            case '6':
                int32_t line;
                int32_t index;
                char** documentTemplate = malloc(sizeof(char*) * size);
                printf("Command 6 is to insert the text by line and symbol index\n\n");
                printf("Choose line and index: ");
                scanf("%d %d", &line, &index);
                getchar();

                u_int16_t activeLine=0;
                startFromNewLine(documentTemplate,&activeLine);
                char* result =AddTextToEnd(documentTemplate,activeLine);
                printf("Preparing to insert '%s' into line %d at index %d...\n\n", result, line, index);
                result[strlen(result)]=' ';

                uint32_t sizeofupdate=sizeof(document[line])+strlen(result)+1;

                if (strlen(document[line])+strlen(result)+1>sizeof(document[line]) ) {
                    document[line]=ExtendArray(document[line],&sizeofupdate);
                }

                u_int16_t docLen = strlen(document[line]);
                u_int16_t wordLen = strlen(result);

                for (int i = docLen; i >= index; i--) {
                    document[line][i + wordLen] = document[line][i];
                }

                for (int i = 0; i < wordLen; i++) {
                    document[line][index + i] = result[i];
                }

                printf("%s",document[line]);
                printf("\n");
                repeat=1;
                free(documentTemplate);
                continue;
           case '7': {
                printf("Command 7 is to search\n\n");

               char** documentTemplates = malloc(sizeof(char*) * 10);
               u_int16_t activeLines=0;
               startFromNewLine(documentTemplates,&activeLines);
               char* results =AddTextToEnd(documentTemplates,activeLines);

               printf("%s",results);
               u_int32_t currentchar=0;
               u_int32_t match=0;
               u_int32_t** arrayformatching = malloc(sizeof(u_int32_t*) * 100);
               u_int32_t HowManyMatching=0;

                for (int i = 1; i <= currentLine; i++) {
                    for (int j = 0; j < strlen(document[i]); j++) {
                        if (document[i][j]==' ' || document[i][j]=='\0') {
                            if (match==strlen(results)) {
                                u_int32_t* pairofresult = malloc(sizeof(u_int32_t) * 2);
                                pairofresult[0] = i; // Line
                                pairofresult[1] = j-strlen(results); // Index
                                arrayformatching[HowManyMatching]=pairofresult;
                                HowManyMatching++;
                            }
                            currentchar=0;
                            match=0;
                        }else {
                            if (document[i][j]==results[currentchar]) {
                                match++;
                            }
                            currentchar++;

                        }
                    }
                }
               printf("result: ");
               for (int i = 0; i < HowManyMatching; i++) {
                   printf("%d ",arrayformatching[i][0]);
                   printf("%d",arrayformatching[i][1]);
                   printf("\n");
               }
               // printf("%p",arrayformatching);

                printf("\n\n");
                repeat = 1;
               free(documentTemplates);
               free(arrayformatching);
                continue;

            }
            case '8':
                printf("Thanks bye");
                repeat =0;
                break;
            default:
                printf("You wrote incorrect number\n");
                printf("write again\n\n");

                repeat=1;
        }
    }

    for (int32_t i = 1; i < currentLine; i++) {
        free(document[i]);
    }
    // free(document);
    return 0;
}

