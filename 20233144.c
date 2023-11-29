#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WIN32
    #include <curses.h>
#else
    #include <ncurses.h>
#endif

typedef struct line{
	char* text;
	struct line* prev;
	struct line* next;
}line;

line *head = NULL, *tail = NULL; // ���� ������ head�� tail ����

void init() {
    head = (line*)malloc(sizeof(line));
    tail = (line*)malloc(sizeof(line));
    head->text = strdup(""); // �� ���ڿ��� �ʱ�ȭ
    head->prev = NULL;
    head->next = tail;
    tail->prev = head;
    tail->next = NULL;
}

line* addLine(line *tail, const char *text) {
    line *newLine = (line *)malloc(sizeof(line));
    newLine->text = strdup(text); // ���ڿ� ����
    newLine->next = NULL;
    newLine->prev = tail;

    if (tail) {
        tail->next = newLine;
    }

    return newLine;
}

void printFileContent(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("������ �� �� �����ϴ�");
        return;
    }

    line *head = NULL, *tail = NULL;
    char buffer[1024];

    while (fgets(buffer, sizeof(buffer), file)) {
        if (!head) {
            head = addLine(NULL, buffer);
            tail = head;
        } else {
            tail = addLine(tail, buffer);
        }
    }

    fclose(file);

    // ȭ�鿡 ���
    for (line *current = head; current != NULL; current = current->next) {
        printw("%s", current->text);
    }

    // ����Ʈ ����
    while (head) {
        line *temp = head;
        head = head->next;
        free(temp->text);
        free(temp);
    }
}


void processKeyInput(int *x, int *y) {
    int ch;
    line *currentLine = head;

    while (1) {
        ch = getch(); // Ű �Է� �ޱ�
		
        switch (ch) {
            case KEY_UP:
                printw("Up arrow\n");
                break;
            case KEY_DOWN:
                printw("Down arrow\n");
                break;
            case KEY_LEFT:
                printw("Left arrow\n");
                break;
            case KEY_RIGHT:
                printw("Right arrow\n");
                break;
            case KEY_HOME:
                printw("Home key\n");
                break;
            case KEY_END:
                printw("End key\n");
                break;
            case KEY_PPAGE:
                printw("Page Up\n");
                break;
            case KEY_NPAGE:
                printw("Page Down\n");
                break;
            case '\n': // Enter key
                // �� ���� �߰�
                currentLine = addLine(currentLine, "");
                (*y)++; // y ���� ������Ʈ
                move(*y, 0); // ���� �������� Ŀ�� �̵�
                break;
            case KEY_BACKSPACE:
            case '\b':
            case 127: // �齺���̽� Ű ó��
                if (*x > 0) {
                    // ���� ���ο��� ���� ����
                    int len = strlen(currentLine->text);
                    if (len > 0) {
                        currentLine->text[len - 1] = '\0';
                        move(*y, --(*x));
                        delch();
                    }
                } else if (*y > 0) {
                    // ���� �������� �̵�
                    currentLine = currentLine->prev;
                    *y = *y - 1;
                    *x = strlen(currentLine->text);
                    move(*y, *x);
                }
                
                break;


//            case 27: // ESC key
//                break;
            default:
                // ���� ���ο� ���� �߰�
                int len = strlen(currentLine->text);
                currentLine->text = realloc(currentLine->text, len + 2);
                currentLine->text[len] = ch;
                currentLine->text[len + 1] = '\0';
                printw("%c", ch); // ȭ�鿡 ���� ���
                break;
        }
    }
}

void insertnode(){
	line* newnode;
	newnode = (line*)malloc(sizeof(line));
	//scanf("%s", &newnode->text);
	//newnode->next=tail;
	
//	if(head->next==tail){
//		head->next=newnode;
//		newnode->prev=head;
//		newnode->next=tail;
//		tail->prev=newnode;
//		return;
//	}	
}

int main(int argc, char*argv[]){
	int x=0,y=0;
	FILE *file;
	init();
	initscr();            // curses ��� ����
    cbreak();             // ���۸� ���� �� Ű �Է��� ��� ����
    noecho();    
    getyx(stdscr, x,y);
    move(x,y);
	if(argc==1){
		
		processKeyInput(&x, &y); 
	}else if(argc==2){
		printFileContent(argv[1]);
        refresh();
        getch();
        if (file == NULL) {
    	printw("������ �� �� �����ϴ�: %s\n", strerror(errno)); // ���� �޽��� ���
        refresh();
        endwin();
        exit(EXIT_FAILURE);
        }
        keypad(stdscr, TRUE); // Ư�� Ű �Է� Ȱ��ȭ
		processKeyInput(&x, &y); 
        // ���� ������ �а� ó���ϴ� �ڵ带 ���⿡ �߰��ϼ���.
        fclose(file);
	}
    endwin();             // curses ��� ����
}
