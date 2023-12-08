#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STATUS_LINES 2  // Number of status lines

// Global state variables
char *filename = NULL;  // Filename to display
int node_count = 0;     // The current number of nodes (lines)

#ifdef WIN32
#include <curses.h>
#else
#include <ncurses.h>
#endif

typedef struct line {
  char *text;
  struct line *prev;
  struct line *next;
} line;

line *head = NULL, *tail = NULL; // ���� ������ head�� tail ����

void drawStatusBar() {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  // ���� Ŀ�� ��ġ ����
  int cur_y, cur_x;
  getyx(stdscr, cur_y, cur_x);

  // ���� ���� ù ��° �� �׸���
  move(max_y - STATUS_LINES, 0);
  clrtoeol();
  if (filename) {
    printw("[%s] - %d lines", filename, node_count);
  } else {
    printw("[No Name] - %d lines", node_count);
  }

  // Ŀ�� ��ġ ǥ��
  int cursor_info_len = snprintf(NULL, 0, " | Cursor: %d/%d", cur_y + 1, cur_x);
  move(max_y - STATUS_LINES, max_x - cursor_info_len);
  printw(" | Cursor: %d/%d", cur_y + 1, cur_x);

  // ���� ���� �� ��° �� �׸���
  move(max_y - 1, 0);
  clrtoeol();
  printw("help : ctrl+q");

  // ���� Ŀ�� ��ġ�� ����
  move(cur_y, cur_x);

  refresh();
}

void init() {
  head = (line *)malloc(sizeof(line));
  tail = (line *)malloc(sizeof(line));
  head->text = strdup(""); // �� ���ڿ��� �ʱ�ȭ
  head->prev = NULL;
  head->next = tail;
  tail->prev = head;
  tail->next = NULL;
  for (int i = 1; i < 24; i++) {
    mvaddch(i, 0, '~');
  }
}

void deleteLine(line *current) {
  if (current == NULL) return;

  if (current->prev) {
    current->prev->next = current->next;
  }
  if (current->next) {
    current->next->prev = current->prev;
  }
  free(current->text);
  free(current);

  node_count--;  // ���� �� ����
}

line *addLine(line *current, const char *text) {
  line *newLine = (line *)malloc(sizeof(line));
  newLine->text = strdup(text);
  newLine->next = NULL;
  newLine->prev = current;

  if (current) {
    current->next = newLine;
  }

  node_count++;  // ���� �� ����
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

void processKeyInput() {
  int ch;
  int x, y;
  getyx(stdscr, y, x);
  move(y, x);
  line *currentLine = head;

  while (1) {

    ch = getch(); // Ű �Է� �ޱ�

    switch (ch) {
    case KEY_UP:
      if (y > 0) {
        currentLine = currentLine->prev; // ���� �������� �̵�
        y--;
        x = strlen(currentLine->text);
        move(y, x);
      }
      break;

    case KEY_DOWN:
      if (currentLine->next && currentLine->next != tail) {
        currentLine = currentLine->next; // ���� �������� �̵�
        y++;
        x = strlen(currentLine->text);
        move(y, x);
      }
      break;
    case KEY_LEFT:
      if (x > 0) {
        x--;
        move(y, x);
      }
      break;
    case KEY_RIGHT:
      if (x < strlen(currentLine->text)) {
        x++;
        move(y, x);
      }
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
    y++;
    line *newLine = addLine(currentLine, "");// y ���� ������Ʈ

    // �� ������ ù ��° ������ ���� ǥ�� ����
    mvaddch(y, 0, ' ');
    move(y, 0); // ���� �������� Ŀ�� �̵�
    x = 0;
    currentLine = newLine;

    // ���� ������ ����ִ� ���, ���� ������ ���� ����
    if (currentLine->prev && strlen(currentLine->prev->text) == 0) {
        mvaddch(y-1, 0, ' ');
    }
    break;
    case KEY_BACKSPACE:
    case '\b':
    case 127: // �齺���̽� Ű ó��
      if (x > 0) {
          int len = strlen(currentLine->text);
          if (len > 0) {
            currentLine->text[len - 1] = '\0';
            move(y, --x);
            delch();
          }
          if (strlen(currentLine->text) == 0) {
            // ������ ������� ���� �߰�
            mvaddch(y, 0, '~');
          }
        } else if (y > 0) {
          // ���� �������� �̵�
          currentLine = currentLine->prev;
          y--;
          x = strlen(currentLine->text);
          move(y, x);
          if (x == 0) {
            // ���� ������ ������� ���� �߰�
            mvaddch(y, 0, '~');
          }
        }
        break;


      //            case 27: // ESC key
      //                break;
    default:
      // ���� ���ο� ���� �߰�
      int len = strlen(currentLine->text);
      char *newText =
          (char *)realloc(currentLine->text, len + 2); // ���ڿ� ���Ҵ�
          if (strlen(currentLine->text) == 0) {
          // ������ ����ִ� ��� ���� ǥ�� ����
          mvaddch(y, 0, ' ');
        }
      // Ŀ�� ��ġ ������ ���ڵ��� ���������� �̵�
      memmove(newText + x + 1, newText + x, len - x + 1);
      newText[x] = ch; // Ŀ�� ��ġ�� �� ���� ����
      currentLine->text = newText;
      // ȭ�� ������ ���� ���� ������ �ٽ� ���
      move(y, 0);
      printw("%s", currentLine->text);
      x++; // Ŀ�� ��ġ ������Ʈ
      move(y, x);
      break;
    }drawStatusBar();
  }
}

int main(int argc, char *argv[]) {
  initscr(); // curses ��� ����
  init();
  cbreak();  // ���۸� ���� �� Ű �Է��� ��� ����
  noecho();
  keypad(stdscr, TRUE); // Ư�� Ű �Է� Ȱ��ȭ
  resize_term(24, 80);  // PDCurses ������ ũ�� ����

  // ���ϸ��� �����ϰ�, ���� �ٸ� �׸��ϴ�.
  if (argc == 1) {
    filename = "[No Name]";
  } else if (argc == 2) {
    filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file) {
      printFileContent(filename); // ���� ���� ���
      fclose(file);               // ���� �ݱ�
    } else {
      printw("������ �� �� �����ϴ�: %s\n", strerror(errno)); // ���� �޽��� ���
    }
    refresh();
  }

  // Ŀ���� (0, 0)���� �̵�
  move(0, 0);

  // ���� �� �׸���
  drawStatusBar();

  // Ű �Է� ó�� ����
  processKeyInput();

  endwin(); // curses ��� ����
  return 0;
}
