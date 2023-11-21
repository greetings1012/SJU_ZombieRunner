#include <stdio.h>
#include <Windows.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

/*
* 10���� ���� ��/�������� moveAllToLeft�� ������ ��������� ����
��         /     ������

��������        ���� id
��ֹ�����     ��ֹ� id

�� : ��������, pc����, ��ֹ�����, ������������ ����
������ : ���° ����������, ���° pc�������, ���° ��ֹ�����, ���° ������������ ����

��ֹ� : 1
���� : 2

���� ��� ������� char�� ������ �����Կ��� string���� �ٲ�ߵ�
����� �������� �����̴� ��/�������� �����ؾߵ����� PC�� ���� ���� ���ص� �ɲ����⵵��
*/

//�⺻
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


		 ���
		 ���     */
	{
		{"10", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{"��", "��", " ", " "},
		{"��", "��", " ", " "} },
	/* Second OBS
		 ���
		 ���
		 ���
		 ���     */
	{
		{"11", " ", " ", " "},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "},
		{"��", "��", " ", " "},
		{"��", "��", " ", " "},
		{"��", "��", " ", " "},
		{"��", "��", " ", " "} },
	/* Third OBS
	   �����
	   �����
	   �����
	   �����
				*/
	{
		{"12", " ", " ", " "},
		{"��", "��", "��", "��"},
		{"��", "��", "��", "��"},
		{"��", "��", "��", "��"},
		{"��", "��", "��", "��"},
		{" ", " ", " ", " "},
		{" ", " ", " ", " "} },
};

char zombieModel[][7][5][2] = {
{
	{"20"," "," "," "," "},
	{" "," "," "," "," "},
	{" "," "," "," "," "},
	{" "," "," "," "," "},
	{"[","#","n","#","]"},
	{"~","|","~","|"," "},
	{" ","|"," ","/"," "}},

{
	{"21"," "," "," "," "},
	{" "," "," "," "," "},
	{" "," "," "," "," "},
	{" "," "," "," "," "},
	{"[","#","n","#","]"},
	{"~","|","~","|"," "},
	{" ","/"," ","|"," "}},

{
	{"22"," "," "," "," "},
	{" "," "," "," "," "},
	{" "," "," "," "," "},
	{" "," "," "," "," "},
	{"[","X","n","X","]"},
	{"~","|"," ","|","~"},
	{" ","|"," ","|"," "}},
};

char coinModel[][7][1][2] = {
	// @
{
	{"30"},
	{" "},
	{" "},
	{" "},
	{" "},
	{"@"},
	{" "}},
{
	{"31"},
	{" "},
	{" "},
	{" "},
	{"@"},
	{" "},
	{" "}},
{
	{"32"},
	{" "},
	{" "},
	{"@"},
	{" "},
	{" "},
	{" "}},
{
	{"33"},
	{" "},
	{"@"},
	{" "},
	{" "},
	{" "},
	{" "}},
{
	{"34"},
	{"@"},
	{" "},
	{" "},
	{" "},
	{" "},
	{" "}},
	// 0
{
	{"40"},
	{" "},
	{" "},
	{" "},
	{" "},
	{"O"},
	{" "}},
{
	{"41"},
	{" "},
	{" "},
	{" "},
	{"O"},
	{" "},
	{" "}},
{
	{"42"},
	{" "},
	{" "},
	{"O"},
	{" "},
	{" "},
	{" "}},
{
	{"43"},
	{" "},
	{"O"},
	{" "},
	{" "},
	{" "},
	{" "}},
{
	{"44"},
	{"O"},
	{" "},
	{" "},
	{" "},
	{" "},
	{" "}},
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

int showZombie(char zombieInfo[7][5][2]);
int removeZombie(char zombieInfo[7][5][2]);
void changeColor(int colorNum);
int gameStart();

int drawCoin(char coinInfo[7][1][2]);
int removeCoin(char coinInfo[7][1][2]);

#define LEFT 75
#define RIGHT 77
#define UP 72
#define DOWN 80
#define ENTER 13

#define JUMP 1
#define SLIDE 2
#define HIGHJUMP 3
#define ATTACK 4

int gameBoardInfo[20][100]; //pc�� ������ ��ֹ�, ����, ������, ������ Info
int pcBoardInfo[20][5];  //pc�� Info

int char_x = 3, char_y = 17;//ĳ���ͻ�����ġ
int char_size_x = 5, char_size_y = 3;//ĳ���� ũ��
int obs_x = 92, obs_y = 13;//��ֹ�������ġ
int obs_size_x = 4, obs_size_y = 7;//��ֹ� ũ��
int obs_n = 1;//��ֹ� ��ȣ
int floor_y = 20;//�ٴ���ġ
int cheat = FALSE; //���� TRUE:on, FALSE:off
int HEALTH = 4; //ü��
int CURRENT_STAGE = 1; //���� ��������
int TOTAL_SCORE = 0; //�� ���ھ�
int KILLED_ZOMBIE = 0; //óġ�� ���� ��
int pc_state = 0; //pc ����
int pre_pc_state = 0; //pc ���� ����
int FRAME = 0; //Jump�� HighJump�� ������ �� ���
int isSlide = FALSE; //�����̵������� �ƴ��� Ȯ��


//int zombie_state = 0;
static int zombie_size_x = 5;
static int zombie_size_y = 7;
int obsOrZombie = 0;

int coin_size_x = 1, coin_size_y = 7;
int BIG_COIN = 0;  // @�� ������ ������ ����
int SMALL_COIN = 0;  // 0�� ������ ������ ����

int main() {

	srand((unsigned int)time(NULL));
	system("mode con:cols=100 lines=25"); //�ܼ�â ũ��

	gameStart();


	showMap(); //y=20��ġ�� �ٴ� ���
	RemoveCursor();

	//(char_x, char_y)�� ĳ���� ���
	SetCurrentCursorPos(char_x, char_y);
	showCharacter(pcModel[0]);

	int t = 0;
	while (1)
	{
		if (moveAllToLeft() == FALSE)
			break;
		int obs_id = rand() % 3;
		int coin_id = rand() % 10;

		SetCurrentCursorPos(80, 1);
		printf("HP : %2d", HEALTH);

		SetCurrentCursorPos(80, 2);
		printf("BIG COIN : %2d", BIG_COIN);
		SetCurrentCursorPos(80, 3);
		printf("SMALL COIN : %2d", SMALL_COIN);

		SetCurrentCursorPos(80, 4);
		printf("KILLED ZOMBIE : %2d", KILLED_ZOMBIE);

		SetCurrentCursorPos(2, 1);
		printf("< STAGE %2d >", CURRENT_STAGE);

		Sleep(20);

		t += 1;
		if (t % 30 == 0)
		{
			obsOrZombie = rand() % 8;
			SetCurrentCursorPos(obs_x, obs_y);
			if (obsOrZombie == 7) showZombie(zombieModel[0]); // 0.125 Ȯ���� ��ֹ� ��� ���� ����
			else showObject(obsModel[obs_id]);
		}
		else if (t % 20 == 0) {
			SetCurrentCursorPos(obs_x, obs_y);
			drawCoin(coinModel[coin_id]);
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

//��ֹ��� ����ϴ� �Լ�
int showObject(char obsInfo[7][4][3]) {
	COORD curPos = GetCurrentCursorPos();
	int obsdata = atoi(obsInfo[0][0]); //obsModel�� (0,0)�� �����ִ� ��ֹ��� ������ �޾� ���ڷ� ��ȯ(gameboardInfo�� ������Ʈ�ϱ� ����)
	updateGameBoard(curPos.X, curPos.Y, obsdata); //gameBoardInfo�� ��ֹ��� ������ ����
	for (int y = 1; y < obs_size_y; y++) { //obsModel�� y=0���� ��¥�� ��ֹ��� ������ �����ϱ� ���� ���̴� y=1���� ����
		for (int x = 0; x < obs_size_x; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y); //2ĭ¥�� Ư�������̹Ƿ� x*2��ġ�� ���
			if (strcmp(obsInfo[y][x], " ") != 0) {
				printf("%s", obsInfo[y][x]);
				updateGameBoard(curPos.X + (x * 2), curPos.Y + y, 1); //game��oardInfo�� ��ֹ��� 1�� ����
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
	return TRUE;
}

int removeObject(char obsInfo[7][4][3]) {
	COORD curPos = GetCurrentCursorPos();
	updateGameBoard(curPos.X, curPos.Y, 0); //��ֹ��� ������ ����
	for (int y = 1; y < obs_size_y; y++) {  //obsModel�� y=0���� ��¥�� ��ֹ��� ������ �����ϱ� ���� ���̴� y=1���� ����
		for (int x = 0; x < obs_size_x; x++) {
			SetCurrentCursorPos(curPos.X + (x * 2), curPos.Y + y); //2ĭ¥�� Ư�������̹Ƿ� x*2��ġ�� �ִ� ��ֹ��� ����
			if (strcmp(obsInfo[y][x], " ") != 0) {
				printf("  ");
				updateGameBoard(curPos.X + x * 2, curPos.Y + y, 0);
				updateGameBoard(curPos.X + x * 2 + 1, curPos.Y + y, 0); //�Է��Ҷ��� x*2+1��ġ�� updateGameBoard���� ������Ʈ������, �����Ҷ��� ������Ʈ���� �ʾƼ� �ι� ȣ��
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
				updatePcBoard(curPos.X + x - char_x, curPos.Y + y, 1); //pcBoard�� x�� ũ�Ⱑ pc�� ũ��(5)�̱� ������ ��µǴ� ��ġ���� char_x(3)�� ���ְ� pcBoard�� ���ߵ�
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

void removeCharacter(char charInfo[3][5]) {
	COORD curPos = GetCurrentCursorPos();

	for (int y = 0; y < char_size_y; y++) {
		for (int x = 0; x < char_size_x; x++) {
			SetCurrentCursorPos(curPos.X + x, curPos.Y + y);
			if (charInfo[y][x] != ' ') {
				printf(" ");
				updatePcBoard(curPos.X + x - char_x, curPos.Y + y, 0); //pcBoard�� x�� ũ�Ⱑ pc�� ũ��(5)�̱� ������ ��µǴ� ��ġ���� char_x(3)�� ���ְ� pcBoard�� ���ߵ�
			}
		}
	}
	SetCurrentCursorPos(curPos.X, curPos.Y);
}

int updateGameBoard(int curPosX, int curPosY, int n) {
	gameBoardInfo[curPosY][curPosX] = n;
	if (n == 1) //n�� multibyte_character('��'���� 2byte¥��)�̸� gameBoardInfo�� 2ĭ ������Ʈ //�ٸ� ������Ʈ,�������� multibyte_character ������ ���� �߰� ����
		gameBoardInfo[curPosY][curPosX + 1] = n;
	return 1;
}

void updatePcBoard(int curPosX, int curPosY, int n) {
	pcBoardInfo[curPosY][curPosX] = n;
}

//pc_state(pc ����)�� ���� moveAllToLeft()���� ĳ���͸� � ��ġ�� � ���·� ������� �����ϴ� �Լ�(�߰�)
void drawPc() {
	//pc�� ���� ����(pc_state)�� ���� moveAllToLeft()�� ����ɶ����� pc�� �����
	if (pc_state == 0) { //pc�� �⺻�����϶�
		/* //�ϴ��� ��� �����ؼ� �ּ�ó��
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]); //���������� pc�� ����
		*/
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]); //�⺻������ pc�� ���
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

//PC, �������̽��� �����ϰ� ��ֹ�,����,������,���� ���� ��� �������� ��ĭ�� �Ű� ����ϴ� �Լ�
int moveAllToLeft() {
	drawPc();

	int y = obs_y; //��ֹ��� y���� ��ġ == ��ֹ��� ����(10, 11, 12 ��)�� ��������Ƿ� y=13(20-7)�� ������Ų �� y=13��ġ�� gameBoardInfo���� �˻���
	int obs_id;
	int zom_id;
	int coin_id;

	//obj, char, item�� ���� ���� ������ ����
	if (gameBoardInfo[y][0] != 0) {
		switch (gameBoardInfo[y][0] / 10) {
		case 1: //���� ���� ���� ���� ��ֹ��� ��
			obs_id = gameBoardInfo[y][0] % 10;
			SetCurrentCursorPos(0, y);
			removeObject(obsModel[obs_id]);
			break;
		case 2: //���� ���� ���� ���� ������ ��
			zom_id = gameBoardInfo[y][0] % 10;
			SetCurrentCursorPos(0, y);
			removeZombie(zombieModel[zom_id]);
			break;
		case 3:
			coin_id = gameBoardInfo[y][0] % 10;
			SetCurrentCursorPos(0, y);
			removeCoin(coinModel[coin_id]);
			break;
		case 4:
			coin_id = gameBoardInfo[y][0] % 10 + 5;
			SetCurrentCursorPos(0, y);
			removeCoin(coinModel[coin_id]);
			break;
		}
	}

	//gameBoardInfo�� y=13 ��ġ���� x=0���� x=100���� ���� �˻�
	for (int x = 1; x < 100; x++) {
		//gameBoardInfo[y][x]�� obj/����/item/coin�̶��
		if (gameBoardInfo[y][x] != 0) {
			//�������� ��ĭ ������ ���
			switch (gameBoardInfo[y][x] / 10)
			{ //gameBoardInfo�� y=13�� �ִ� ���� 10���� ���� ������ ���⿡ �ִ°� ��ֹ�(1)���� ����(2)���� ������(3,4,5,...)���� ����
			case 1: //��ֹ��� ���
				obs_id = gameBoardInfo[y][x] % 10; //� ��ֹ������� ���� ������ gameBoardInfo[y][x]�� ����Ǿ� �ִ� ������ 10���� ������ �� ������
				SetCurrentCursorPos(x, y);
				removeObject(obsModel[obs_id]);
				SetCurrentCursorPos(x - 1, y);
				showObject(obsModel[obs_id]);
				break;
			case 2: //������ ���
				zom_id = gameBoardInfo[y][x] % 10; //� ��ֹ������� ���� ������ gameBoardInfo[y][x]�� ����Ǿ� �ִ� ������ 10���� ������ �� ������
				SetCurrentCursorPos(x, y);
				removeZombie(zombieModel[zom_id]);
				switch (zom_id) {
				case 0: zom_id = 1; break;
				case 1: zom_id = 0; break;
				}
				SetCurrentCursorPos(x - 1, y);
				showZombie(zombieModel[zom_id]);
				break;
			case 3: // ���� '@' -> �ӽ÷� �����߽��ϴ�!
				coin_id = gameBoardInfo[y][x] % 10;
				SetCurrentCursorPos(x, y);
				removeCoin(coinModel[coin_id]);
				SetCurrentCursorPos(x - 1, y);
				drawCoin(coinModel[coin_id]);
				break;
			case 4: // ���� '0' -> �ӽ÷� �����صξ����ϴ�!
				coin_id = gameBoardInfo[y][x] % 10 + 5; //coinModel ���� - ����5��
				SetCurrentCursorPos(x, y);
				removeCoin(coinModel[coin_id]);
				SetCurrentCursorPos(x - 1, y);
				drawCoin(coinModel[coin_id]);
				break;
			}
			//PC�� ��ֹ��� ��Ҵٸ� (�Ŀ� switch�� ���� ����) //touchCheck() == 2�̸� ����� ������, touchCheck() == 3, 4, 5,...�̸� �����۰� ���� ��
			switch (touchCheck()) {
			case 1:
			case 2:
				//if �������°� �ƴ϶��
				if (cheat == FALSE) {
					HEALTH--; //ü�� ����
					if (gameOver() == TRUE) { //gameOver�� ������ �����ߴٸ�
						return showResult(); //����� �����ָ� False ����(main���� moveAllToLeft�� False�� ������ �� �����ϰ� �Ǿ�����)
					}
					//�浹 �� ��� �� ����
					system("color 47"); //��� : ����, ���� : ���
					Sleep(50);
					system("color 07"); //��� : ����, ���� : ���
					//�����ð� ���� �ѱ� (��ֹ��� �浹 �� �ѹ��� HEALTH�� ���ҽ�Ű�� ���ؼ�)
					cheat = TRUE;
				}
				break;
			case 3:
				SetCurrentCursorPos(x - 1, y);
				removeCoin(coinModel[coin_id]);
				BIG_COIN++;
				break;
			case 4:
				SetCurrentCursorPos(x - 1, y);
				removeCoin(coinModel[coin_id]);
				SMALL_COIN++;
				break;
			}
			//Pc�� ��ֹ��� �������԰�, ������ ���������� ���� ����
			if (touchCheck() == 0 && cheat == TRUE)
				cheat = FALSE;
		}
	}

	/*
	//gameBoardInfo �׽�Ʈ��
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
			if (pcBoardInfo[y][x] == 1 && gameBoardInfo[y][char_x + x] != 0) //pcBoardInfo�� pc�� �ִ� ���� gameBoardInfo�� ��ֹ�/����/������/������ ��ģ�ٸ�
				return gameBoardInfo[y][char_x + x]; //pc�� ��ģ ��ֹ�/����/������/���ο� �ش��ϴ� ���� ����
		}
	}

	return 0; //�浹���� �ʾ����� 0 ����
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
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0X8000)
	{
		pc_state = ATTACK;
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
	}
}


void pcJump() {
	int height = 6; //�׳� Jump�϶��� HighJump�϶��� ���� ���� �ٸ��� �ϴ� ����
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
	else if (FRAME >= height + 1 && FRAME <= height * 2) {
		SetCurrentCursorPos(char_x, char_y - ((FRAME - 1) - ((FRAME - 1) - height) * 2));
		removeCharacter(pcModel[pc_state]);
		SetCurrentCursorPos(char_x, char_y - (FRAME - (FRAME - height) * 2));
		showCharacter(pcModel[pc_state]);
		FRAME++;
	}
	if (FRAME == height * 2 + 1) {
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
	int length = 4; //4������ ���� ���ݸ�� ����
	if (FRAME == 0) {
		SetCurrentCursorPos(char_x, char_y);
		removeCharacter(pcModel[pre_pc_state]);
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]);
		FRAME++;
	}
	else if (FRAME >= 1 && FRAME <= length) {
		int x = char_x + char_size_x;
		int y = obs_y;
		if (gameBoardInfo[y][x]/10 == 2) {
			SetCurrentCursorPos(x, obs_y);
			int zom_id = gameBoardInfo[obs_y][x];
			removeZombie(zombieModel[zom_id]);

			KILLED_ZOMBIE++;
		}
		SetCurrentCursorPos(char_x, char_y);
		showCharacter(pcModel[pc_state]);
		FRAME++;
	}
	else {
		pc_state = 0;
		FRAME = 0;
	}
}


int showZombie(char zombieInfo[7][5][2])
{
	COORD curPos = GetCurrentCursorPos();

	int zombiedata = atoi(zombieInfo[0][0]);
	updateGameBoard(curPos.X, curPos.Y, zombiedata);
	changeColor(12); //���������� ����

	for (int y = 1; y < zombie_size_y; y++)
	{
		for (int x = 0; x < zombie_size_x; x++)
		{
			SetCurrentCursorPos(curPos.X + x, curPos.Y + y);

			if (strcmp(zombieInfo[y][x], " ") != 0) {
				printf("%s", zombieInfo[y][x]);
				updateGameBoard(curPos.X + x, curPos.Y + y, 2);
			}
		}
	}

	SetCurrentCursorPos(curPos.X, curPos.Y);
	changeColor(7); // ��� (default)�� ����
	/*
	switch (zombie_state)
	{
	case 0:
		zombie_state = 1;
		break;
	case 1:
		zombie_state = 0;
		break;
	case 2:
		break;
	default:
		break;
	}*/
	return TRUE;
}

int removeZombie(char zombieInfo[7][5][2])
{
	COORD curPos = GetCurrentCursorPos();

	updateGameBoard(curPos.X, curPos.Y, 0);
	for (int y = 1; y < zombie_size_y; y++)
	{
		for (int x = 0; x < zombie_size_x; x++)
		{
			SetCurrentCursorPos(curPos.X + (x * 1), curPos.Y + y);
			if (strcmp(zombieInfo[y][x], " ") != 0) {
				printf(" ");
				updateGameBoard(curPos.X + (x * 1), curPos.Y + y, 0);
				updateGameBoard(curPos.X + x * 1 + 1, curPos.Y + y, 0);
			}
		}
	}

	SetCurrentCursorPos(curPos.X, curPos.Y);


	return TRUE;
}

void changeColor(int colorNum)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum);
	//10 : green
	//12 : red
	//14 : yellow
	//9 : blue
	//7 : white(default)
}
int gameStart()
{
	SetCurrentCursorPos(40, 10);
	printf("Zombie Runner");
	SetCurrentCursorPos(36, 12);
	printf("Press Enter to start...");
	while (1)
	{
		int key = _getch();
		if (key == ENTER)
		{
			system("cls");
			return TRUE;
		}
	}
	return FALSE;
}

// ���� ���� �Լ�
int drawCoin(char coinInfo[7][1][2]) {
	COORD curPos = GetCurrentCursorPos();
	int coinData = atoi(coinInfo[0][0]);
	updateGameBoard(curPos.X, curPos.Y, coinData);
	changeColor(14); //��������� ����

	for (int y = 1; y < coin_size_y; y++) {
		for (int x = 0; x < coin_size_x; x++) {
			SetCurrentCursorPos(curPos.X + x, curPos.Y + y);
			if (strcmp(coinInfo[y][x], " ") != 0) {
				printf("%s", coinInfo[y][x]);
				updateGameBoard(curPos.X + x, curPos.Y + y, coinData / 10);
			}
		}
	}

	SetCurrentCursorPos(curPos.X, curPos.Y);
	changeColor(7); // ��� (default)�� ����
	return TRUE;
}

// ���� ���� �Լ�
int removeCoin(char coinInfo[7][1][2]) {
	COORD curPos = GetCurrentCursorPos();
	updateGameBoard(curPos.X, curPos.Y, 0); // ������ ������ ����

	for (int y = 1; y < coin_size_y; y++) {
		for (int x = 0; x < coin_size_x; x++) {
			SetCurrentCursorPos(curPos.X + x, curPos.Y + y);
			if (strcmp(coinInfo[y][x], " ") != 0) {
				printf(" ");
				updateGameBoard(curPos.X + x, curPos.Y + y, 0);
			}
		}
	}

	SetCurrentCursorPos(curPos.X, curPos.Y);
	return TRUE;
}