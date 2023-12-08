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

line *head = NULL, *tail = NULL; // 전역 변수로 head와 tail 선언

void drawStatusBar() {
  int max_y, max_x;
  getmaxyx(stdscr, max_y, max_x);

  // 현재 커서 위치 저장
  int cur_y, cur_x;
  getyx(stdscr, cur_y, cur_x);

  // 상태 바의 첫 번째 줄 그리기
  move(max_y - STATUS_LINES, 0);
  clrtoeol();
  if (filename) {
    printw("[%s] - %d lines", filename, node_count);
  } else {
    printw("[No Name] - %d lines", node_count);
  }

  // 커서 위치 표시
  int cursor_info_len = snprintf(NULL, 0, " | Cursor: %d/%d", cur_y + 1, cur_x);
  move(max_y - STATUS_LINES, max_x - cursor_info_len);
  printw(" | Cursor: %d/%d", cur_y + 1, cur_x);

  // 상태 바의 두 번째 줄 그리기
  move(max_y - 1, 0);
  clrtoeol();
  printw("help : ctrl+q");

  // 원래 커서 위치로 복원
  move(cur_y, cur_x);

  refresh();
}

void init() {
  head = (line *)malloc(sizeof(line));
  tail = (line *)malloc(sizeof(line));
  head->text = strdup(""); // 빈 문자열로 초기화
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

  node_count--;  // 라인 수 감소
}

line *addLine(line *current, const char *text) {
  line *newLine = (line *)malloc(sizeof(line));
  newLine->text = strdup(text);
  newLine->next = NULL;
  newLine->prev = current;

  if (current) {
    current->next = newLine;
  }

  node_count++;  // 라인 수 증가
  return newLine;
}

void printFileContent(const char *filename) {
  FILE *file = fopen(filename, "r");
  if (!file) {
    perror("파일을 열 수 없습니다");
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

  // 화면에 출력
  for (line *current = head; current != NULL; current = current->next) {
    printw("%s", current->text);
  }

  // 리스트 해제
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

    ch = getch(); // 키 입력 받기

    switch (ch) {
    case KEY_UP:
      if (y > 0) {
        currentLine = currentLine->prev; // 이전 라인으로 이동
        y--;
        x = strlen(currentLine->text);
        move(y, x);
      }
      break;

    case KEY_DOWN:
      if (currentLine->next && currentLine->next != tail) {
        currentLine = currentLine->next; // 다음 라인으로 이동
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
    // 새 라인 추가
    y++;
    line *newLine = addLine(currentLine, "");// y 값을 업데이트

    // 새 라인의 첫 번째 열에서 물결 표시 삭제
    mvaddch(y, 0, ' ');
    move(y, 0); // 다음 라인으로 커서 이동
    x = 0;
    currentLine = newLine;

    // 현재 라인이 비어있는 경우, 이전 라인의 물결 제거
    if (currentLine->prev && strlen(currentLine->prev->text) == 0) {
        mvaddch(y-1, 0, ' ');
    }
    break;
    case KEY_BACKSPACE:
    case '\b':
    case 127: // 백스페이스 키 처리
      if (x > 0) {
          int len = strlen(currentLine->text);
          if (len > 0) {
            currentLine->text[len - 1] = '\0';
            move(y, --x);
            delch();
          }
          if (strlen(currentLine->text) == 0) {
            // 라인이 비었으면 물결 추가
            mvaddch(y, 0, '~');
          }
        } else if (y > 0) {
          // 이전 라인으로 이동
          currentLine = currentLine->prev;
          y--;
          x = strlen(currentLine->text);
          move(y, x);
          if (x == 0) {
            // 이전 라인이 비었으면 물결 추가
            mvaddch(y, 0, '~');
          }
        }
        break;


      //            case 27: // ESC key
      //                break;
    default:
      // 현재 라인에 문자 추가
      int len = strlen(currentLine->text);
      char *newText =
          (char *)realloc(currentLine->text, len + 2); // 문자열 재할당
          if (strlen(currentLine->text) == 0) {
          // 라인이 비어있는 경우 물결 표시 삭제
          mvaddch(y, 0, ' ');
        }
      // 커서 위치 이후의 문자들을 오른쪽으로 이동
      memmove(newText + x + 1, newText + x, len - x + 1);
      newText[x] = ch; // 커서 위치에 새 문자 삽입
      currentLine->text = newText;
      // 화면 갱신을 위해 현재 라인을 다시 출력
      move(y, 0);
      printw("%s", currentLine->text);
      x++; // 커서 위치 업데이트
      move(y, x);
      break;
    }drawStatusBar();
  }
}

int main(int argc, char *argv[]) {
  initscr(); // curses 모드 시작
  init();
  cbreak();  // 버퍼링 없이 각 키 입력을 즉시 받음
  noecho();
  keypad(stdscr, TRUE); // 특수 키 입력 활성화
  resize_term(24, 80);  // PDCurses 윈도우 크기 조정

  // 파일명을 결정하고, 상태 바를 그립니다.
  if (argc == 1) {
    filename = "[No Name]";
  } else if (argc == 2) {
    filename = argv[1];
    FILE *file = fopen(filename, "r");
    if (file) {
      printFileContent(filename); // 파일 내용 출력
      fclose(file);               // 파일 닫기
    } else {
      printw("파일을 열 수 없습니다: %s\n", strerror(errno)); // 오류 메시지 출력
    }
    refresh();
  }

  // 커서를 (0, 0)으로 이동
  move(0, 0);

  // 상태 바 그리기
  drawStatusBar();

  // 키 입력 처리 시작
  processKeyInput();

  endwin(); // curses 모드 종료
  return 0;
}
