#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

/*
* 10으로 나눈 몫/나머지로 moveAllToLeft가 무엇을 출력할지를 정함
몫         /     나머지

좀비인지        좀비 id
장애물인지     장애물 id

몫 : 좀비인지, pc인지, 장애물인지, 아이템인지를 구별
나머지 : 몇번째 좀비모습인지, 몇번째 pc모습인지, 몇번째 장애물인지, 몇번째 아이템인지를 구별

장애물 : 1
좀비 : 2

※이 경우 좀비모델이 char로 구현이 가능함에도 string으로 바꿔야됨
좀비는 왼쪽으로 움직이니 몫/나머지로 구현해야되지만 PC는 굳이 구현 안해도 될꺼같기도함
*/

//기본
char pcModel[][3][5] = {
{
	{'(','0','_','0',')'},
	{'/','|','_','|','~'},
	{' ','/',' ','|',' '}},

{
   {'(','O','_','O',')'},
   {'/','|','_','|','~'},
   {' ','*',' ','*',' '}},

{
   {' ',' ',' ',' ',' '},
   {'(','0','_','0',')'},
   {'/','|','_','|','~'}},

{
   {'(','>','_','<',')'},
   {'/','|','_','|','~'},
   {' ','^',' ','^',' '}},

{
   {'(','`','^','`',')'},
   {'|','\\','_','|','\\'},
   {' ','/',' ','|',' '}},
};


char obsModel[][7][4][3] = {
	/* First OBS
		 

		 ▲▲
		 ■■     */
	{
		{"10", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{"▲\0", "▲\0", " ", " "},
		{"■\0", "■\0", " ", " "} },
	/* Second OBS
	     ▲▲
		 ■■
		 ■■
		 ■■     */
	
	{
		{"11", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{"▲\0", "▲\0", " ", " "},
		{"■\0", "■\0", " ", " "},
		{"■\0", "■\0", " ", " "},
		{"■\0", "■\0", " ", " "} },
	/* Third OBS
	   ■■■■
	   ■■■■
	   ■■■■
	   ▼▼▼▼
			    */
	{
		{"12", " ", " ", " "},
		{"■\0", "■\0", "■\0", "■\0"},
		{"■\0", "■\0", "■\0", "■\0"},
		{"■\0", "■\0", "■\0", "■\0"},
		{"▼\0", "▼\0", "▼\0", "▼\0"}, 
		{" ", " ", " ", " "},
		{" ", " ", " ", " "} },
};

void SetCurrentCursorPos(int x, int y);
COORD GetCurrentCursorPos();
void RemoveCursor();

void showMap();
int showObject(char obsInfo[7][4][3]);
int removeObject(char obsInfo[7][4][3]);
void showCharacter(char charInfo[3][5]);
void removeCharacter(char charInfo[3][5]);
int updateGameBoard(int curPosX, int curPosY, int n);
void updatePcBoard(int curPosX, int curPosY, int n);
void drawPc();
int moveAllToLeft();
int touchCheck();
int gameOver();
int showResult();
void ProcessKeyInput();
void pcJump();
void pcSlide();
void pcAttack();

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

#define JUMP 1
#define SLIDE 2
#define HIGHJUMP 3
#define ATTACK 4

int gameBoardInfo[20][100]; //pc를 제외한 장애물, 좀비, 아이템, 코인의 Info
int pcBoardInfo[20][5];  //pc의 Info

int char_x = 3, char_y = 17;//캐릭터생성위치
int char_size_x = 5, char_size_y = 3;//캐릭터 크기
int obs_x = 92, obs_y = 13;//장애물생성위치
int obs_size_x = 4, obs_size_y = 7;//장애물 크기
int obs_n = 1;//장애물 번호
int floor_y = 20;//바닥위치
int cheat = FALSE; //무적 TRUE:on, FALSE:off
int HEALTH = 4; //체력
int CURRENT_STAGE = 1; //현재 스테이지
int TOTAL_SCORE = 0; //총 스코어
int KILLED_ZOMBIE = 0; //처치한 좀비 수
int pc_state = 0; //pc 상태
int pre_pc_state = 0; //pc 이전 상태
int FRAME = 0; //Jump와 HighJump를 구현할 때 사용
int slidingEnabled = FALSE; //슬라이딩 상태 True/False로 구별
int isSlide = FALSE; //슬라이딩중인지 아닌지 확인

int main() {
	
	srand((unsigned int)time(NULL));
	system("mode con:cols=100 lines=25"); //콘솔창 크기

	showMap(); //y=20위치에 바닥 깔기
	
	RemoveCursor();

	//(char_x, char_y)에 캐릭터 출력
	SetCurrentCursorPos(char_x, char_y);
	showCharacter(pcModel[0]);


	int t = 0;
	while (1) {
		if (moveAllToLeft() == FALSE)
			break;
		int obs_id = rand() % 3;

		SetCurrentCursorPos(90, 1);
		printf("hp : %2d", HEALTH);
		Sleep(20);

		t += 1;
		if (t % 30 == 0) {
			SetCurrentCursorPos(obs_x, obs_y);
			showObject(obsModel[obs_id]);
		}
	}

	getchar();
	return 0;
}

void SetCurrentCursorPos(int x, int y) {
	COORD curPos = { x, y };

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), curPos);
}

COORD GetCurrentCursorPos() {
	COORD curPoint;
	CONSOLE_SCREEN_BUFFER_INFO curInfo;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curPoint.X = curInfo.dwCursorPosition.X;
	curPoint.Y = curInfo.dwCursorPosition.Y;

	return curPoint;
}

void RemoveCursor() {
	CONSOLE_CURSOR_INFO curInfo;
	GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
	curInfo.bVisible = 0;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}

void showMap() {
	SetCurrentCursorPos(0, floor_y);
	for (int i = 0; i < 100; i++)
		printf("-");
}
//장애물을 출력하는 함수
int showObject(char obsInfo[7][4][3]) {
	COORD curPos = GetCurrentCursorPos();
	int obsdata = atoi(obsInfo[0][0]); //obsModel의 (0,0)에 적혀있는 장애물의 정보를 받아 숫자로 변환(gameboardInfo에 업데이트하기 위해)
	updateGameBoard(curPos.X, curPos.Y, obsdata); //gameBoardInfo에 장애물의 정보를 저장
	for (int y = 1; y < obs_size_y; y++) { //obsModel의 y=0줄은 어짜피 장애물의 정보를 저장하기 위한 줄이니 y=1부터 시작
		for (int x = 0; x < obs_size_x; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y); //2칸짜리 특수문자이므로 x*2위치에 출력
			if (strcmp(obsInfo[y][x], " ") != 0) {
				printf("%s", obsInfo[y][x]); 
				updateGameBoard(curPos.X+(x*2), curPos.Y+y, 1); //gameㅠoardInfo에 장애물을 1로 저장
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
	return TRUE;
}

int removeObject(char obsInfo[7][4][3]) {
	COORD curPos = GetCurrentCursorPos();
	updateGameBoard(curPos.X, curPos.Y, 0); //장애물의 정보를 지움
	for (int y = 1; y < obs_size_y; y++) {  //obsModel의 y=0줄은 어짜피 장애물의 정보를 저장하기 위한 줄이니 y=1부터 시작
		for (int x = 0; x < obs_size_x; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y); //2칸짜리 특수문자이므로 x*2위치에 있는 장애물을 지움
			if (strcmp(obsInfo[y][x], " ") != 0) {
				printf("  ");
				updateGameBoard(curPos.X + x*2, curPos.Y + y, 0);
				updateGameBoard(curPos.X + x*2 + 1, curPos.Y + y, 0); //입력할때는 x*2+1위치도 updateGameBoard에서 업데이트하지만, 삭제할때는 업데이트하지 않아서 두번 호출
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
	return TRUE;
}

void showCharacter(char charInfo[3][5]) {
	COORD curPos = GetCurrentCursorPos();
	for (int y = 0; y < char_size_y; y++) {
		for (int x = 0; x < char_size_x; x++) {
			SetCurrentCursorPos(curPos.X + x, curPos.Y + y);
			if (charInfo[y][x] != ' ') {
				printf("%c", charInfo[y][x]);
				updatePcBoard(curPos.X+x-char_x, curPos.Y+y, 1); //pcBoard는 x의 크기가 pc의 크기(5)이기 때문에 출력되는 위치에서 char_x(3)을 빼주고 pcBoard에 들어가야됨
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

void removeCharacter(char charInfo[3][5]) {
	COORD curPos = GetCurrentCursorPos();
	
	for (int y = 0; y < char_size_y; y++) {
		for (int x = 0; x < char_size_x; x++) {
			SetCurrentCursorPos(curPos.X + x, curPos.Y+y);
			if (charInfo[y][x] != ' ') {
				printf(" ");
				updatePcBoard(curPos.X+x-char_x, curPos.Y+y, 0); //pcBoard는 x의 크기가 pc의 크기(5)이기 때문에 출력되는 위치에서 char_x(3)을 빼주고 pcBoard에 들어가야됨
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

int updateGameBoard(int curPosX, int curPosY, int n) {
	gameBoardInfo[curPosY][curPosX] = n;
	if (n == 1) //n이 multibyte_character('■'같은 2byte짜리)이면 gameBoardInfo에 2칸 업데이트 //다른 오브젝트,아이템의 multibyte_character 여부의 따라 추가 가능
		gameBoardInfo[curPosY][curPosX+1] = n;
	return 1;
}

void updatePcBoard(int curPosX, int curPosY, int n) {
	pcBoardInfo[curPosY][curPosX] = n;
}

//pc_state(pc 상태)에 따라 moveAllToLeft()에서 캐릭터를 어떤 위치에 어떤 형태로 출력할지 결정하는 함수(추가)
void drawPc() {
	//pc의 현재 상태(pc_state)에 따라 moveAllToLeft()가 실행될때마다 pc를 출력함
	if (pc_state == 0) { //pc가 기본상태일때
		/* //일단은 없어도 동작해서 주석처리
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]); //이전상태의 pc를 삭제
		*/
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]); //기본상태의 pc를 출력
		ProcessKeyInput();
	}
	else if (pc_state == JUMP || pc_state == HIGHJUMP) {
		pcJump();
	}
	else if (pc_state == SLIDE) {
		pcSlide();
		ProcessKeyInput();
	}
	else if (pc_state == ATTACK) {
		pcAttack();
	}
}

//PC, 인터페이스를 제외하고 장애물,좀비,아이템,코인 등을 모두 왼쪽으로 한칸씩 옮겨 출력하는 함수
int moveAllToLeft() {
	drawPc();

	int y = obs_y; //장애물의 y생성 위치 == 장애물의 정보(10, 11, 12 등)가 담겨있으므로 y=13(20-7)로 고정시킨 후 y=13위치만 gameBoardInfo에서 검사함
	int obs_id;
	//obj, char, item이 왼쪽 끝에 닿으면 삭제
	if (gameBoardInfo[y][0] != 0) {
		switch (gameBoardInfo[y][0] / 10) {
		case 1: //왼쪽 끝에 닿은 것이 장애물일 때
			obs_id = gameBoardInfo[y][0] % 10;
			SetCurrentCursorPos(0, y);
			removeObject(obsModel[obs_id]);
		}
	}

	//gameBoardInfo의 y=13 위치에서 x=0부터 x=100까지 전부 검사
	for (int x = 1; x < 100; x++) {
		//gameBoardInfo[y][x]가 obj/좀비/item이라면
		if (gameBoardInfo[y][x] != 0) {
			//왼쪽으로 한칸 움직여 출력
			switch (gameBoardInfo[y][x] / 10) { //gameBoardInfo의 y=13에 있는 값을 10으로 나눈 몫으로 여기에 있는게 장애물(1)인지 좀비(2)인지 아이템(3,4,5,...)인지 구별
			case 1: //장애물일 경우
				obs_id = gameBoardInfo[y][x] % 10; //어떤 장애물인지에 대한 정보는 gameBoardInfo[y][x]에 저장되어 있는 정보를 10으로 나누면 알 수있음
				SetCurrentCursorPos(x, y);
				removeObject(obsModel[obs_id]);
				SetCurrentCursorPos(x - 1, y);
				showObject(obsModel[obs_id]);
			}
			//PC가 장애물과 닿았다면 (후에 switch로 변경 가능) //touchCheck() == 2이면 좀비와 닿은것, touchCheck() == 3, 4, 5,...이면 아이템과 닿은 것
			if (touchCheck() == 1) {
				//if 무적상태가 아니라면
				if (cheat == FALSE) {
					HEALTH--; //체력 감소
					if (gameOver() == TRUE) { //gameOVer의 조건을 만족했다면
						return showResult(); //결과를 보여주며 False 리턴(main에서 moveAllToLeft가 False를 리턴할 시 종료하게 되어있음)
					}
					//충돌 시 배경 색 변경
					system("color 47"); //배경 : 빨강, 문자 : 흰색
					Sleep(50);
					system("color 07"); //배경 : 검정, 문자 : 흰색
					//일정시간 무적 켜기 (장애물과 충돌 시 한번만 HEALTH를 감소시키기 위해서)
					cheat = TRUE;
				}
			}
			//Pc가 장애물을 빠져나왔고, 무적이 켜져있으면 무적 끄기
			if (touchCheck() == 0 && cheat == TRUE)
				cheat = FALSE;
		}
	}

	/* //gameBoardInfo 테스트용
	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < 100; x++) {
			SetCurrentCursorPos(x, y);
			printf("%d", gameBoardInfo[y][x]);
		}
	}
	*/

	return TRUE;
}

int touchCheck() {
	for (int y = 0; y < 20; y++) {
		for (int x = 0; x < char_size_x; x++) {
			if (pcBoardInfo[y][x] == 1 && gameBoardInfo[y][char_x + x] != 0) //pcBoardInfo의 pc가 있는 곳과 gameBoardInfo의 장애물/좀비/아이템/코인이 겹친다면
				return gameBoardInfo[y][char_x + x]; //pc와 겹친 장애물/좀비/아이템/코인에 해당하는 값을 리턴
		}
	}

	return 0; //충돌하지 않았으면 0 리턴
}

int gameOver()
{
	if (HEALTH <= 0) return TRUE;
	else return FALSE;
}

int showResult() {
	system("cls");

	SetCurrentCursorPos(40, 2);
	if (gameOver()) printf("~ Game Over ~ ");
	else printf("~ Game Clear ~ ");


	SetCurrentCursorPos(40, 4);
	printf("SCORE : %d", TOTAL_SCORE);

	SetCurrentCursorPos(40, 5);
	printf("HP : %d", HEALTH);

	SetCurrentCursorPos(40, 6);
	printf("Killed Zombie : %d", KILLED_ZOMBIE);

	Sleep(2000);
	return FALSE;
}

void ProcessKeyInput() {

	if (GetAsyncKeyState(VK_LEFT) & 0X8000)
	{
		pc_state = HIGHJUMP;
		slidingEnabled = FALSE;
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0X8000)
	{
		pc_state = ATTACK;
		slidingEnabled = FALSE;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0X8000)
	{
		pc_state = SLIDE;
		isSlide = TRUE;
	}
	if (!(GetAsyncKeyState(VK_DOWN) & 0X8000))
	{
		isSlide = FALSE;
	}
	if (GetAsyncKeyState(VK_UP) & 0X8000)
	{
		pc_state = JUMP;
		slidingEnabled = FALSE;
	}
}


void pcJump() {
	int height = 6; //그냥 Jump일때와 HighJump일때의 높이 값을 다르게 하는 변수
	if (pc_state == HIGHJUMP) {
		height += 4;
	}
	if (FRAME == 0) {
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]);
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]);
		pre_pc_state = pc_state;
		FRAME++;
	}
	else if (FRAME >= 1 && FRAME <= height) {
		SetCurrentCursorPos(char_x, char_y - (FRAME - 1));
		removeCharacter(pcModel[pc_state]);
		SetCurrentCursorPos(char_x, char_y - FRAME);
		showCharacter(pcModel[pc_state]);
		FRAME++;
	}
	else if (FRAME >= height+1 && FRAME <= height*2) {
		SetCurrentCursorPos(char_x, char_y - ((FRAME - 1) - ((FRAME - 1) - height) * 2));
		removeCharacter(pcModel[pc_state]);
		SetCurrentCursorPos(char_x, char_y - (FRAME - (FRAME - height) * 2));
		showCharacter(pcModel[pc_state]);
		FRAME++;
	}
	if (FRAME == height*2+1) {
		FRAME = 0;
		pc_state = 0;
	}
}

void pcSlide() {
	if (isSlide == TRUE)
	{
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]);
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]);
		pre_pc_state = pc_state;
	}
	else
	{
		pc_state = 0;
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]);
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]);
		pre_pc_state = pc_state;
	}
}

void pcAttack() {
	int length = 4; //4프레임 동안 공격모션 실행
	if (FRAME == 0) {
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]);
		FRAME++;
	}
	else if (FRAME >= 1 && FRAME <= length) {
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]);
		FRAME++;
	}
	else {
		pc_state = 0;
		FRAME = 0;
	}
}
