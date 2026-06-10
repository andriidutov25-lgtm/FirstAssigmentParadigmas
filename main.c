#include <stdio.h>

#include <stdlib.h>
#include <string.h>

typedef struct {
    char **data;
    int32_t count;
    int32_t capacity;
} Stack;

void StackInit(Stack *s, int32_t capacity) {
    s->data     = malloc(sizeof(char *) * capacity);
    s->count    = 0;
    s->capacity = capacity;
}

void StackPush(Stack *s, const char *line) {
    if (s->count == s->capacity) {
        free(s->data[s->capacity - 1]);
        s->count = s->capacity - 1;
    }
    for (int32_t i = s->count; i > 0; i--) {
        s->data[i] = s->data[i - 1];
    }
    s->data[0] = strdup(line);
    s->count++;
}

char *StackPop(Stack *s) {
    if (s->count == 0) return NULL;
    char *top = s->data[0];
    for (int32_t i = 0; i < s->count - 1; i++) {
        s->data[i] = s->data[i + 1];
    }
    s->data[s->count - 1] = NULL;
    s->count--;
    return top;
}

void StackFree(Stack *s) {
    for (int32_t i = 0; i < s->count; i++) free(s->data[i]);
    free(s->data);
    s->count = s->capacity = 0;
}


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

int count_digits(int num) {
    int count = 0;
    if (num == 0) {
        return 1;
    }
    while (num != 0) {
        num /= 10;
        count++;
    }

    return count;
}


int32_t GetNumberOfString(char* lines){
    return (int32_t)atoi(lines);
}


void startFromNewLine(char** document,u_int16_t* currentLine,int32_t numereishion) {
    *currentLine+=1;
    document[*currentLine]=malloc(sizeof(char) * 10);

    if (numereishion) {
        snprintf(document[*currentLine], sizeof(document[*currentLine]), "%d", *currentLine);
        document[*currentLine][count_digits(*currentLine)]='.';
        document[*currentLine][count_digits(*currentLine)+1]='\0';
    }else {
        document[*currentLine][count_digits(*currentLine)]='\0';

    }


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


int GetCommand() {
    int myNumber;
    printf("Choose the command: ");
    scanf("%d", &myNumber);
    getchar();
    return myNumber;
}

static void SaveForUndo(Stack *undo, Stack *redo, const char *line) {
    StackFree(redo);
    StackInit(redo, redo->capacity ? redo->capacity : 20);
    StackPush(undo, line);
}

char* Delete(char ***document, uint16_t currentLine,
             Stack *undo, Stack *redo) {
    int32_t lineForDelete;
    int32_t indexForDelete;
    int32_t sizefordelete;
    printf("Choose line and index: ");
    scanf("%d %d %d", &lineForDelete, &indexForDelete,&sizefordelete);
    getchar();

    sizefordelete--;
    char *line = (*document)[lineForDelete];

    char* Cut_result = malloc((sizefordelete + 1) * sizeof(char));
    if (Cut_result == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }

    if (lineForDelete >= 1 && lineForDelete  <= currentLine &&
        indexForDelete+sizefordelete <= strlen((*document)[lineForDelete])) {

        SaveForUndo(undo, redo, line);

        printf("%s",(*document)[lineForDelete]);
        indexForDelete+=GetNumberOfString((*document)[lineForDelete])+1;

        for (int32_t i =indexForDelete; i < indexForDelete+sizefordelete; i++) {
            Cut_result[i-indexForDelete]=(*document)[lineForDelete][i];
            (*document)[lineForDelete][i]=' ';
        }
        for (int32_t i =indexForDelete; i <strlen((*document)[lineForDelete]); i++) {
            (*document)[lineForDelete][i]=(*document)[lineForDelete][i+sizefordelete];
        }
        printf("%s",(*document)[lineForDelete]);

        return Cut_result;
    }

    printf("Index or line out of range\n\n");
    return NULL;
}

void InsertAndPaste(char ***document, uint16_t currentLine, int32_t size,
                    char *text, Stack *undo, Stack *redo) {
    int32_t line;
    int32_t index;
    char** documentTemplate = malloc(sizeof(char*) * size);
    printf("Choose line and index: ");

    scanf("%d %d", &line, &index);
    getchar();
    if (line >= 1 && line <= currentLine && index <= strlen((*document)[line])) {
        char *lineStr  = (*document)[line];
        SaveForUndo(undo, redo, lineStr);

        u_int16_t activeLine=0;
        index+=GetNumberOfString((*document)[line])+1;
        startFromNewLine(documentTemplate,&activeLine,0);
        char* result;
        if (text==NULL) {
            result =AddTextToEnd(documentTemplate,activeLine);
        }else {
            result =text;
        }

        printf("Preparing to insert '%s' into line %d at index %d...\n\n", result, line, index);

        int32_t sizeofupdate=strlen((*document)[line])+strlen(result)+1;

        if (sizeofupdate > (int32_t)strlen((*document)[line]) + 1) {
            (*document)[line] = ExtendArray((*document)[line], &sizeofupdate);
        }

        size_t docLen = strlen((*document)[line]);
        size_t wordLen = strlen(result);

        for (int i = docLen; i >= index; i--) {
            (*document)[line][i + wordLen] = (*document)[line][i];
        }

        for (int i = 0; i < wordLen; i++) {
            (*document)[line][index + i] = result[i];
        }

        printf("%s",(*document)[line]);
        printf("\n");
        free(documentTemplate);
    }else {
        printf("Index or line out of range\n\n");
    }
}




char* Copy(char** document, u_int16_t currentLine) {
    int32_t LineToCopy;
    int32_t IndexToCopy;
    int32_t SizeCopy;

    printf("Choose line and index: ");

    scanf("%d %d %d", &LineToCopy, &IndexToCopy,&SizeCopy);
    getchar();
    char *Copy_result= malloc((SizeCopy + 1) * sizeof(char));

    if (LineToCopy >= 1 && LineToCopy <= currentLine && IndexToCopy <= strlen(document[LineToCopy])) {
        IndexToCopy+=GetNumberOfString(document[LineToCopy])+1;
        for (int32_t i =IndexToCopy; i < IndexToCopy+SizeCopy; i++) {
            Copy_result[i-IndexToCopy]=document[LineToCopy][i];
        }
        return Copy_result;
    }
    printf("Index or line out of range\n\n");
    return NULL;
}


void Replacement(char ***document, uint16_t currentLine,
                 Stack *undo, Stack *redo){
    int32_t lineToReplace;
    int32_t IndexToReplace;
    char** documentTemplate = malloc(sizeof(char*) * 5);
    printf("Choose line and index: ");

    scanf("%d %d", &lineToReplace, &IndexToReplace);
    getchar();
    // AddToStack(&StackForUndo,(*document)[lineToReplace],&countfullig);
    IndexToReplace+=GetNumberOfString((*document)[lineToReplace])+1;

    if (lineToReplace >= 1 && lineToReplace <= currentLine && IndexToReplace <= strlen((*document)[lineToReplace])) {
        u_int16_t activeLine=0;
        char *lineHHH   = (*document)[lineToReplace];

        SaveForUndo(undo, redo, lineHHH);

        startFromNewLine(documentTemplate,&activeLine,0);
        char* resultToReplace=AddTextToEnd(documentTemplate,activeLine);
        resultToReplace[strlen(resultToReplace)]=' ';
        // printf("%d",strlen(resultToReplace));
        for (int i = IndexToReplace; i < IndexToReplace+strlen(resultToReplace)-1; i++) {
            (*document)[lineToReplace][i] = resultToReplace[i-IndexToReplace];
        }
        printf("%s",(*document)[lineToReplace]);
        printf("\n");
        free(documentTemplate);
    }else {
        printf("Index or line out of range\n\n");
    }
}
void Undo(char ***document, Stack *undo, Stack *redo) {
    char *saved = StackPop(undo);
    if (!saved) { printf("Nothing to undo.\n"); return; }

    int32_t lineNum = GetNumberOfString(saved);
    if (lineNum < 1) { printf("Corrupt undo entry.\n"); free(saved); return; }

    /* Before restoring, push the CURRENT state onto the redo stack. */
    StackPush(redo, (*document)[lineNum]);

    free((*document)[lineNum]);
    (*document)[lineNum] = saved;   /* stack gave us ownership */
    printf("Undo: %s\n\n", (*document)[lineNum]);
}


void Redo(char ***document, Stack *undo, Stack *redo) {
    char *saved = StackPop(redo);
    if (!saved) { printf("Nothing to redo.\n"); return; }

    int32_t lineNum = GetNumberOfString(saved);
    if (lineNum < 1) { printf("Corrupt redo entry.\n"); free(saved); return; }

    /* Push the current state onto undo so the user can undo the redo. */
    StackPush(undo, (*document)[lineNum]);

    free((*document)[lineNum]);
    (*document)[lineNum] = saved;
    printf("Redo: %s\n\n", (*document)[lineNum]);
}




int main(void){

    printf("Hello, It's text editor\n");
    uint16_t repeat=1;
    u_int16_t currentLine=0;
    int32_t size=5;
    char** StackForUndo=malloc(sizeof(char*) * size);
    int32_t countfullig=0;
    char* BufferToPaste=NULL;
    char** document = malloc(sizeof(char*) * size);
    Stack undoStack, redoStack;
    StackInit(&undoStack, 20);
    StackInit(&redoStack, 20);

    startFromNewLine(document,&currentLine,1);

    while (repeat) {
        printf("Command 1 is to append text symbols to the end\n"
               "Command 2 is to start the new line\n"
               "Command 3 is to use files to save the information\n"
               "Command 4 is to use files to load\n"
               "Command 5 is to print the current text to the console\n"
               "Command 6 is to insert the text by line and symbol index\n"
               "Command 7 is to search\n"
               "Command 8 is delete command\n"
               "Command 9 is undo command \n"
               "Command 10 is redo command \n"
               "Command 11 is cut command \n"
               "Command 12 is paste command \n"
               "Command 13 is copy command \n"
               "Command 14 is replacement command \n"
               "Command 0 is to exit\n");

        int choose=GetCommand();
        switch (choose) {
            case 1:
                printf("%d\n",currentLine);
                document[currentLine]=AddTextToEnd(document,currentLine);
                break;
            case 2:
                printf("Command 2 to start the new line\n\n");
                if (currentLine<size-1) {
                    startFromNewLine(document,&currentLine,1);
                    printf("%d\n",currentLine);
                }else {
                    size=size*2;
                    char** temp = realloc(document, sizeof(char*)*size );
                    if (temp != NULL) {
                        document=temp;
                        startFromNewLine(document,&currentLine,1);
                        printf("%d\n",currentLine);
                    }else {
                        printf("Out of memory\n");
                    }

                }
                break;
            case 3:
                printf("Command 3 is to use files to save the information\n\n");
                WritetextToFile(document,currentLine);
                repeat=1;
                continue;
            case 4:
                printf("Command 4 is to use files to load is not implimenting\n\n");
                repeat=1;
                continue;
            case 5:
                printf("Command 5 is to print the current text to the console\n\n");
                for (int16_t i=1;i<currentLine+1;i++) {
                    // printf("%d.",i);
                    printf(" %s", document[i]);
                    printf("\n");
                }
                printf("\n");
                repeat=1;
                continue;
            case 6:
                printf("Command 6 is to insert the text by line and symbol index\n\n");
                // InsertAndPaste(&document,currentLine,size,NULL);
                InsertAndPaste(&document, currentLine, size,
                            NULL, &undoStack, &redoStack);

                repeat=1;
                continue;
           case 7: {
                printf("Command 7 is to search\n\n");

               char** documentTemplates = malloc(sizeof(char*) * 10);
               u_int16_t activeLines=0;
               startFromNewLine(documentTemplates,&activeLines,0);
               char* results =AddTextToEnd(documentTemplates,activeLines);

               printf("%s",results);
               u_int32_t currentchar=0;
               u_int32_t match=0;
               u_int32_t** arrayformatching = malloc(sizeof(u_int32_t*) * 100);
               u_int32_t HowManyMatching=0;

                for (int i = 1; i <= currentLine; i++) {
                    for (int j = 0; j <= strlen(document[i]); j++) {
                        if (document[i][j]==' ' || document[i][j]=='\0') {
                            if (match==strlen(results)) {
                                u_int32_t* pairofresult = malloc(sizeof(u_int32_t) * 2);
                                pairofresult[0] = i;
                                pairofresult[1] = j-strlen(results);
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
               if (!HowManyMatching) {
                   printf("Not Found");
               }else {
                   printf("result: ");

                   for (int i = 0; i < HowManyMatching; i++) {
                       printf("%d ",arrayformatching[i][0]);
                       printf("%d",arrayformatching[i][1]);
                       printf("\n");
                   }
               }


                printf("\n\n");
                repeat = 1;
               free(documentTemplates);
               free(arrayformatching);
                continue;
            }
            case 8:
                printf("Command 8 is to delete the text by line and symbol index\n\n");
                char* BufferToPaste=Delete(&document,currentLine,&undoStack,&redoStack);
                    // Delete(&document,currentLine,&StackForUndo,&countfullig);
                repeat =1;
                continue;

            case 9:
                printf("Command 9 is to undo the changes\n\n");
                Undo(&document, &undoStack, &redoStack);
                repeat =1;
                continue;
            case 10:
                printf("Command 9 is to todo the changes\n\n");
                Redo(&document, &undoStack, &redoStack);
                repeat =1;
                continue;

            case 11:
                printf("Command 11 is to cut the text by line and symbol index and size\n\n");
                BufferToPaste=Delete(&document,currentLine,&undoStack,&redoStack);
                repeat =1;
                continue;
            case 12:
                printf("Command 12 is to paste the text by line and symbol index\n\n");
                if (BufferToPaste==NULL) {
                    printf("Buffer is empty");
                }else {
                    InsertAndPaste(&document, currentLine, size,BufferToPaste , &undoStack, &redoStack);
                }
                repeat =1;
                continue;
            case 13:
                printf("Command 13 is to copy the text by line and symbol index\n\n");
                BufferToPaste=Copy(document,currentLine);
                repeat =1;
                continue;
            case 14:
                printf("Command 14 is to replacement the text by line and symbol index\n\n");
                // Replacement(&document,currentLine,size);
                Replacement(&document, currentLine, &undoStack, &redoStack);

                repeat =1;
                continue;
            case 0:
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
    return 0;
}