/* Tasneem Shelleh 
1220439
section: 3 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INIT_STR_SIZE 1000
#define OP_INSERT 1
#define OP_REMOVE 0

// ctreat the struct of the queue
typedef struct qnode *queueNode; 
struct qnode {
    char *str;
    queueNode next;
};

struct queueRecord{
    queueNode front;
    queueNode rear;
    int size;
};
typedef struct queueRecord *queue;

// create the struct of stack (linked list)
typedef struct snode *stackNode;
struct snode {
    char *str;
    // 1 = insert
    // 0 = remove
    int op;
    int index;
    stackNode next;
};

struct stackStruct {
    stackNode top;
};
typedef struct stackStruct *stack;

struct textNode {
    char *string;
    int capacity;
};
typedef struct textNode textn;

textn loadFile(queue q);
stack createStack();
queue createQueue();
queueNode createQueueNode(char *str);
void Enqueue(char *str , queue q);
char* appendString(textn text, char* word);
void insertText(textn text, queue q, stack undo);
void push(stack s, char *str, int op, int index);
void pushNode(stack s, stackNode node);
queueNode Dequeue(queue q);
void freeQueueNode(queueNode n);
char* moveTextFromQueue(textn text, queue q, stack undo, int pos);
void removeFromText(char* text, int startIndex, int count);
void addTextAtIndex(textn text, int startIndex, char *stringToAdd);
char* RemoveString(char *text, stack undo);
stackNode pop(stack undo);
void performUndo(textn text, stack undo, stack redo);
void performRedo(textn text, stack undo, stack redo);
void printStack(stack s);
void saveTextToFile(char *text);
void printQueue(queue q);
void freeStack(stack s);
void freeQueue(queue q);
void menu();

// load input text from file to a string 
textn loadFile(queue q) {
    FILE *inputF = fopen("input.txt","r");

    char c;
    int i = 0;
    int textSize = INIT_STR_SIZE;
    textn text; 
    text.string = (char *)malloc(sizeof(char) * INIT_STR_SIZE); //create the string 
    text.capacity = sizeof(char) * INIT_STR_SIZE;
    
    while (( c = fgetc(inputF) ) != EOF) {
        text.string[i++] = c;

        // becuase we don't want a fixed size to the string such that there is no a fixed size for the input file
        // realloc the string with the new size according to what fgets read from file 
        if (i == textSize) {
            textSize = i + INIT_STR_SIZE;
            text.string = realloc(text.string, textSize);
            text.capacity = textSize;
        }
    }
    text.string[i] = '\0';  // to end the string 


    fclose(inputF);
    return text;
}

stack createStack() {
    stack s;
    s = (stack)malloc(sizeof(struct stackStruct));
    s->top = NULL;
    return s;
}

queue createQueue(){
    queue q;
    // if(max < min){printf("the queue size is too small");}
    q = (queue)malloc(sizeof(struct queueRecord));

    //if the compailer be abled to allocate the queue
    if (q == NULL)
    {
        printf("Out of space\n");
    }

    q->front = NULL;
    q->rear = NULL;
    return q;
}

queueNode createQueueNode(char *str) {
    queueNode n = (queueNode) malloc(sizeof(struct qnode));
    n->next = NULL;
    n->str = strdup(str); // to duplicate the string str into n->str 
    return n;
}

void Enqueue(char *str , queue q){
    q->size++;
    if (q->rear == NULL) {
        q->rear = q->front = createQueueNode(str); 
    } else {
        q->rear->next = createQueueNode(str);
        q->rear = q->rear->next;
    }
}

void validateAndIncreaseCapacity(textn text, int wordSize) {
    int textCapacity = text.capacity;
    if (strlen(text.string) + wordSize + 2 > textCapacity) {
        char *newText = (char *)malloc(textCapacity * 2);
        strcpy(newText, text.string); 
        free(text.string);
        text.string = newText;
    }
}

char* appendString(textn text, char* word) {
    validateAndIncreaseCapacity(text, strlen(word)); // works as realloc 

    // char* space = " ";
    // strcat(text.string, space);
    strcat(text.string, word);  //strcat : It will append a copy of the source string in the destination string.and a '\0' (concat 2 strings)
    return text.string;
}

void insertText(textn text, queue q, stack undo) {

    printf("please enter the position (-1 for end of text): ");
    int pos = 0;
    scanf("%d", &pos);   // scanf : read until whitespace , getchar() : remove \0 from readen text 
    getchar();
    if ((pos > -1 && pos >= strlen(text.string)) || pos < -1) {
        printf("Invalid position: %d\n", pos);
        return;
    }

    // the new text stored in a new string with unstable size 
    printf("please enter a text to insert: ");
    int intputTextSize = 50;
    char *inputText = (char *)malloc(sizeof(char) * 50);
    int i = 0;
    char c;
    if (pos != 0) {
        inputText[0] = ' ';  // to add a space before the words 
        i = 1;
    }

    while (( c = getchar() ) != '\n' && c != EOF) {
        inputText[i++] = c;

        if (i == intputTextSize) {
            // increase the finalTextSize by 50 chars
            intputTextSize = i + 50;
            inputText = realloc(inputText, intputTextSize);
        }

        // enqueue (add) the new text to the queue 
        if (inputText[i - 1] == ' ') {

            inputText[i] = '\0';
            Enqueue(inputText, q);
            i = 0;
        }
    }
    inputText[i] = '\0';

    if (i != 0) {
        Enqueue(inputText, q);
    }

    free(inputText);

    moveTextFromQueue(text, q, undo, pos);
}

void push(stack s, char *str, int op, int index) {
    stackNode sn = (stackNode)malloc(sizeof(struct snode));
    sn->next = NULL;
    sn->str = strdup(str); // to duolicate str into sn->str
    sn->index = index;
    sn->op = op;

    if (s->top == NULL) {
        s->top = sn;
    } else {
        sn->next = s->top;
        s->top = sn;
    }
}

void pushNode(stack s, stackNode node) {
    node->next = s->top;
    s->top = node;
}

queueNode Dequeue(queue q) {
    queueNode temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    return temp;
}

void freeQueueNode(queueNode n) {
    free(n->str);
    free(n);
}

char* moveTextFromQueue(textn text, queue q, stack undo, int pos) {
    while(q->front != NULL) {
        queueNode n = Dequeue(q);
        push(undo, n->str, OP_INSERT, pos);
        if (pos < 0) {
            text.string = appendString(text, n->str);  //connect (add) n->str to the end of text.string if pos = -1 
        } else {
            addTextAtIndex(text, pos, n->str);
            pos += strlen(n->str); // the index of the new string
        }
        freeQueueNode(n);
    }
}

void removeFromText(char* text, int startIndex, int count) {
    int ptr1 = startIndex;
    int ptr2 = startIndex + count;

    // shift the string to remove a specific text according to the length of the text 
    while(text[ptr2] != '\0') {
        text[ptr1] = text[ptr2];
        ptr1++;
        ptr2++;
    }
    text[ptr1] = '\0';
}

void addTextAtIndex(textn text, int startIndex, char *stringToAdd) {
    int ptr1 = strlen(text.string);
    int ptr2 = strlen(stringToAdd) + strlen(text.string);
    validateAndIncreaseCapacity(text, ptr2);  //realloc
    int addNull = 0;
    if (ptr1 == startIndex) {
        addNull = 1;
    }

    while (ptr1 >= startIndex) 
    {
        text.string[ptr2] = text.string[ptr1]; // let the index of string be at the index (that equal the length of the new word) 
        ptr1--;
        ptr2--;
    }

    for (int i = startIndex; i < startIndex + strlen(stringToAdd); i++) 
    {
        text.string[i] = stringToAdd[i - startIndex]; // add the new string at a spicific index in the original string
    }

    if (addNull) {
        text.string[strlen(stringToAdd) + startIndex] = '\0';
    }
}

char* RemoveString(char *text, stack undo) {

    printf("Please enter text to remove: ");
    int stringToRemoveSize = 50;
    char *stringToRemove = (char *)malloc(sizeof(char) * 50);
    int i = 0;
    char c;
    while (( c = getchar() ) != '\n' && c != EOF) {
        stringToRemove[i++] = c;

        if (i == stringToRemoveSize) {
            // increase the finalTextSize by 50 chars
            stringToRemoveSize = i + 50;
            stringToRemove = realloc(stringToRemove, stringToRemoveSize);
        }
    }
    stringToRemove[i] = '\0';

    char* stringStartPtr = NULL;
    if ((stringStartPtr = strstr(text, stringToRemove)) != NULL) {  // strstr is a a predefined function used for string matching.it returns a pointer to the first charectar found
        int index = stringStartPtr - text;
        removeFromText(text, index, strlen(stringToRemove));
        push(undo, stringToRemove, OP_REMOVE, index);
    } else {
        printf("string doesn't exist\n");
    }

    free(stringToRemove);
}

stackNode pop(stack undo) {
    stackNode temp = undo->top;
    undo->top = undo->top->next;
    return temp;
}

void performUndo(textn text, stack undo, stack redo) {
    if (undo->top == NULL) {
        printf("nothing to undo\n");
        return;
    }

    stackNode undoNode = pop(undo);
    pushNode(redo, undoNode);
    if (undoNode->op == OP_REMOVE) {
        addTextAtIndex(text, undoNode->index, undoNode->str);
    } else {
        int startIndex;
        if (undoNode->index == -1) {
            startIndex = strlen(text.string) - strlen(undoNode->str);
        } else {
            startIndex = undoNode->index;
        }
        removeFromText(text.string, startIndex, strlen(undoNode->str));
    }
}

void performRedo(textn text, stack undo, stack redo) {
    if (redo->top == NULL) {
        printf("nothing to redo\n");
        return;
    }

    stackNode redoNode = pop(redo);
    pushNode(undo, redoNode);
    if (redoNode->op == OP_REMOVE) {
        removeFromText(text.string, redoNode->index, strlen(redoNode->str));
    } else {
        int startIndex;
        if (redoNode->index == -1) {
            startIndex = strlen(text.string);
        } else {
            startIndex = redoNode->index;
        }
        addTextAtIndex(text, startIndex, redoNode->str);
    }
}

void printStack(stack s) {
    printf("=========================\n");
    printf("%-20s\t%-20s\t%s\n", "Token", "Operation", "Index");
    stackNode ptr = s->top;
    while(ptr != NULL) {
        printf("%-20s\t%-20s\t%d\n", ptr->str, (ptr->op == OP_INSERT ? "Insert" : "Remove"), ptr->index);
        ptr = ptr->next;
    }
    printf("=========================\n");
}

void saveTextToFile(char *text) {
    FILE* out = fopen("Output.txt", "w");
    fprintf(out, "%s", text);
    fclose(out);
}

void printQueue(queue q) {
    queueNode ptr = q->front;
    while(ptr != NULL) {
        printf("%s -> ", ptr->str);
        ptr = ptr->next;
    }
    printf("NULL\n");
}

void freeStack(stack s) {
    while(s->top != NULL) {
        stackNode temp = pop(s);
        free(temp->str);
        free(temp);
    }
    free(s);
}

void freeQueue(queue q) {
    while (q->front != NULL) {
        queueNode temp = Dequeue(q);
        free(temp->str);
        free(temp);
    }
    free(q);
}

void menu()
{
    int op = -1;
    int fileRead = 0;

    stack undo = createStack();
    stack redo = createStack();
    queue q = createQueue();
    textn text;
    text.string = NULL;
    char *originalText = NULL;

    while (1)
    {
        printf("\n");
        printf("1. Load the input file which contains the initial text.\n");
        printf("2. Print the loaded text.\n");
        printf("3. Insert strings to the text.\n");
        printf("4. Remove strings from the text.\n");
        printf("5. Perform Undo operation\n");
        printf("6. Perform Redo operation\n");
        printf("7. Print the Undo Stack and the Redo stack\n");
        printf("8. Save the updated text to the output file.\n");
        printf("9. Print updated text\n");
        printf("10. Exit\n");
        printf("Please choose an option: ");
        scanf("%d", &op);
        getchar(); // read new line char
        printf("\n");
        if (fileRead == 0 && op != 1 && op != 10)
        {
            printf("Please load input file first\n");
            continue;
        }
        switch (op)
        {
        case 1:
            if (fileRead == 0) {
                fileRead = 1;
                text = loadFile(q);
                originalText = strdup(text.string);
                printf("The file loaded successfully.\n");
            } else {
                printf("File already read.\n");
            }
            break;
        case 2:
            printf("Text: %s\n", originalText);
            break;
        case 3:
            insertText(text, q, undo);
            printf("Text: %s\n", text.string);
            break;
        case 4:
            RemoveString(text.string, undo);
            printf("Text: %s\n", text.string);
            break;
        case 5:
            performUndo(text, undo, redo);
            printf("Text: %s\n", text.string);
            break;
        case 6:
            performRedo(text, undo, redo);
            printf("Text: %s\n", text.string);
            break;
        case 7:
            printf("Undo stack: \n");
            printStack(undo);
            printf("Redo stack: \n");
            printStack(redo);
            break;
        case 8:
            saveTextToFile(text.string);
            break;
        case 9:
            printf("Updated text: %s\n", text.string);
            break;
        case 10:
            freeQueue(q);
            freeStack(undo);
            freeStack(redo);
            if (text.string != NULL) {
                free(text.string);
            }
            if (originalText != NULL) {
                free(originalText);
            }
            exit(0);
            break;
        default:
            printf("invalid option\n");
            break;
        }
    }
}




int main() {
    menu();
    // stack undo = createStack();
    // stack redo = createStack();
    // queue q = createQueue();
    // textn text = loadFile(q);
    // printf("%s\n", text.string);
    // insertText(q);
    // moveTextFromQueue(text, q, undo);
    // printf("done\n");

    // printf("%s\n", text.string);
    // // printQueue(q);
    // // RemoveString(text.string, undo);
    // printf("new string: '%s'\n", text.string);
    // printStack(undo);
    // performUndo(text.string, undo, redo);
    // printf("new string after undo: '%s'\n", text.string);
    // performRedo(text.string, undo, redo);
    // printf("new string after redo: '%s'\n", text.string);
    
    // printStack(undo);
    // printStack(redo);
    // saveTextToFile(text.string);
    // // char *test1 = (char *) malloc(sizeof(char) * 100);
    // // test1[0] = '1';
    // // test1[1] = '2';
    // // test1[2] = '3';
    // // test1[3] = '\0';

    // // char *test2 = (char *) malloc(sizeof(char) * 5);
    // // test2[0] = 'a';
    // // test2[1] = 'a';
    // // test2[2] = 'a';
    // // test2[3] = '\0';

    // // printf("%s, %s\n", test1, test2);
    // // test1 = appendString(test1, test2);
    // // printf("%s\n", test1, test2);
    // // TODO free all
}