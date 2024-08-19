#include "Tetris.h"
#include <time.h> //srand(time(NULL))��Ҫ
#include <stdlib.h>
#include <iostream>
#include "Block.h"
#include <conio.h> //�������̨��������ĺ������û�ͨ�������̲����Ķ�Ӧ����
#include <fstream> //�ļ���д��
#include <graphics.h> //c++ͼ�ο�
#include <mmsystem.h> //��������ͷ�ļ�
#pragma comment(lib,"winmm.lib") //��̬��,����������
#undef UNICODE

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"
#define RECORDER1_FILE "recorder1.txt"

const int SPEED_NORMAL[MAX_LEVEL] = { 500,400,300,200,100 };//�������ٶ�
const int SPEED_QUICK = 50;//�����������½�


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)//���캯��
{
	//��ͼ�� ����,���ϱ߽�,�����С ֻ���ʼ��һ�Σ����Է��ڹ��캯����
	this->rows = rows;
	this->cols = cols;
	leftMargin = left;
	topMargin = top;
	this->blockSize = blockSize;
	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;//�������
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);//���������������һ����
		}
		map.push_back(mapRow);//��һά����mapRow��һ�У��ӵ�map��ͼ��
		//push_back��һ���µ�Ԫ�ؼӵ�vector�������
	}


}

void Tetris::init()
{

	mciSendString("play res/bg1.mp3 repeat", 0, 0, 0);//��������
	//���� (����ָ��, ��ȡ���ص���Ϣ(�ַ���), ������Ϣ�ĳ���, ���ھ��(һ��ΪNULL));

	delay = SPEED_NORMAL[0];
	srand(time(NULL)); //��ϵͳʱ����������ֵ���������������time(NULL)��ȡϵͳʱ�䣬��λΪ��;
	//������Ϸ����
	initgraph(938, 816);//����д����

	//����ͼƬ
	loadimage(&imgBg, "res/bg1.png");//����
	loadimage(&imgWin, "res/win.png");//Ӯ
	loadimage(&imgOver, "res/over.png");//ʧ��
	loadimage(&imgPause, "res/pause.png", 80, 80);//ͼ���IMAGE����ָ�룬ͼƬ��ַ��������,�߶�
	//ͼ���IMAGE����ָ�루&������=������ַ��ָ��ָ��ľ��Ǳ�����ַ��
	//��Ե�ַ��./����Ŀ¼�µ��ļ����з���   ͼƬչʾ������pngҲ������jpg
	loadimage(&imgClear[0], "res/tx/0.png", 36 * 10, 36);//������Чͼ
	loadimage(&imgClear[1], "res/tx/1.png", 36 * 10, 36);
	loadimage(&imgClear[2], "res/tx/2.png", 36 * 10, 36);
	loadimage(&imgClear[3], "res/tx/3.png", 36 * 10, 36);
	loadimage(&imgClear[4], "res/tx/4.png", 36 * 10, 36);
	loadimage(&imgClear[5], "res/tx/5.png", 36 * 10, 36);
	loadimage(&imgClear[6], "res/tx/6.png", 36 * 10, 36);
	loadimage(&imgClear[7], "res/tx/7.png", 36 * 10, 36);
	loadimage(&imgClear[8], "res/tx/8.png", 36 * 10, 36);
	loadimage(&imgClear[9], "res/tx/9.png", 36 * 10, 36);
	loadimage(&imgClear[10], "res/tx/10.png", 36 * 10, 36);
	loadimage(&imgClear[11], "res/tx/11.png", 36 * 10, 36);


	//������һ��ʱ�����³�ʼ����Ϸ��������
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	//��������־�������ĳ�ʼ����
	//��ͷ�ļ���ʼ���Ļ�û�취ÿ�ֶ����³�ʼ��������Ҫд��init������
	score = 0;
	lineCount = 0;
	level = 1;
	gameOver = false;
	clear = false;

	//��ʼ����߷�
	if (endless) {
		ifstream file(RECORDER1_FILE);//�ļ���
		if (!file.is_open()) {//û��
			cout << RECORDER1_FILE << "��ʧ��" << endl;
			endlesshScore = 0;
		}
		else {
			file >> endlesshScore;
		}
		file.close();//�ر��ļ�
	}
	else {
		ifstream file(RECORDER_FILE);//�ļ���
		if (!file.is_open()) {//û��
			cout << RECORDER_FILE << "��ʧ��" << endl;
			highestScore = 0;
		}

		else {
			file >> highestScore;//���ļ���������
		}
		file.close();//�ر��ļ�
	}

}

void Tetris::play()
{
	init();

	//��������
	nextBlock = new Block();
	curBlock = nextBlock;
	nextBlock = new Block();

	int timer = 0;
	while (1) {
		//�����û�������
		keyEvent();
		//ÿ��30ms��Ⱦһ�λ���
		timer += getDelay();
		if (timer > delay) {
			timer = 0;//������֤�´��ָ�30ms��Ⱦ
			drop();//�����½�
			// ��Ⱦ��Ϸ����
			update = true;
		}

		if (update) {//��update����ǿ�����keyEvnet�û�����ʱҲʵ����Ⱦ����
			//������Ϸ����
			update = false;
			updateWindow();
		}

		if (gameOver) {//��Ϸ����
			//�������
			saveScore();
			//������Ϸ��������
			displayOver();
			
			//�û��治����ϵͳ��ͣ
			//system("pause");
			//�������ɲ�д�ˣ�������ûдdisplayOver����ʱ�ŵü����

		}//���Ҫ����ʱ�����·��鸲�Ǿɷ���Ͱ����if(gameOver)���жϷ���if(update)ǰ

	}
}

void Tetris::startGame()
{
	endless = false;//ÿ�ֳ�ʼ�������ܻ���ҳ��㴳���Ǵ��أ����޾����޾�
	loadimage(&imgBegin, "res/beginbg1.png");
	initgraph(938, 816);
	BeginBatchDraw();
	putimage(0, 0, &imgBegin);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "΢���ź�");
	settextcolor(RGB(255, 255, 255));
	char str[] = { "��ӭ��������˧�Ķ���˹������Ϸ" };
	outtextxy(leftMargin - 80, topMargin + 100, str);

	int gap = 40;
	int starty = 330;
	int height = 80;

	TCHAR title[2][50] = { "����ģʽ", "�޾�ģʽ" };

	for (int i = 0; i < 2; i++) {
		button(375, starty + i * height + i * gap, title[i]);
	}
	

	ExMessage msg;
	bool isButtonHovered = false; // ���ڱ������Ƿ���ͣ�ڰ�ť��
	int hoveredButtonIndex = -1; // ���ڼ�¼��ͣ�İ�ť����,�ж���Ӧ�ý�����ģʽ���ǽ��޾�ģʽ

	while (true) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message) {
			case WM_MOUSEMOVE:
				isButtonHovered = false; // ÿ���ƶ�ǰ������Ϊfalse
				hoveredButtonIndex = -1;

				for (int i = 0; i < 2; i++) {
					if (msg.x >= 375 && msg.x <= 575 && msg.y >= starty + i * height + i * gap && msg.y <= starty + i * height + i * gap + 60) {
						setfillcolor(RGB(143, 87, 150));
						isButtonHovered = true;
						hoveredButtonIndex = i;
					}
					else {
						setfillcolor(RGB(56, 158, 154));

					}
					//���Ƴ��������ӵİ�ť
					fillroundrect(375, starty + i * height + i * gap, 575, starty + i * height + i * gap + 60, 10, 10);
					outtextxy(375 + (200 - textwidth(title[i])) / 2, starty + i * height + i * gap + (60 - textheight(title[i])) / 2, title[i]);
					FlushBatchDraw();//��������ͽ����û���Լ��������ɫ��ʱ��ı���ɫ��ʱ�����˸��������
				}
				break;
			case WM_LBUTTONDOWN:
				if (isButtonHovered && (hoveredButtonIndex == 0)) {
					play();
				}
				if (isButtonHovered && (hoveredButtonIndex == 1)) {
					endless = true;
					play();
				}
				break;
			}
		}
	}
	EndBatchDraw();
	closegraph(); // �ڱ�Ҫʱ�ر�ͼ�δ���
}

void Tetris::startAgain()
{
	loadimage(&imgBegin, "res/beginbg1.png");
	initgraph(938, 816);
	BeginBatchDraw();
	putimage(0, 0, &imgBegin);
	setbkmode(TRANSPARENT);
	settextstyle(70, 0, "΢���ź�");
	settextcolor(RGB(255, 255, 255));
	char str[] = { "��Ϸ����" };
	outtextxy(365, topMargin + 100, str);

	int gap = 40;
	int starty = 330;
	int height = 80;

	TCHAR title[2][50] = { "����һ��", "������ҳ" };

	for (int i = 0; i < 2; i++) {
		button(375, starty + i * height + i * gap, title[i]);
	}

	ExMessage msg;
	bool isButtonHovered = false; // ���ڱ������Ƿ���ͣ�ڰ�ť��
	int hoveredButtonIndex = -1; // ���ڼ�¼��ͣ�İ�ť����,�ж���Ӧ������һ�ֻ����˳���Ϸ

	while (true) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message) {
				case WM_MOUSEMOVE:
					isButtonHovered = false; // ÿ���ƶ�ǰ������Ϊfalse
					hoveredButtonIndex = -1;

					for (int i = 0; i < 2; i++) {
						if (msg.x >= 375 && msg.x <= 575 && msg.y >= starty + i * height + i * gap && msg.y <= starty + i * height + i * gap + 60) {
							setfillcolor(RGB(143, 87, 150));//����rgb��ȡ��Ҫ����ɫ�Ļ��ͻ���˸   Ϊʲô����ģʽ���������޾�ģʽ������
							isButtonHovered = true;
							hoveredButtonIndex = i;
						}
						else {
							setfillcolor(RGB(56, 158, 154));
						}
						//���Ƴ��������ӵİ�ť
						fillroundrect(375, starty + i * height + i * gap, 575, starty + i * height + i * gap + 60, 10, 10);
						outtextxy(375 + (200 - textwidth(title[i])) / 2, starty + i * height + i * gap + (60 - textheight(title[i])) / 2, title[i]);
						FlushBatchDraw();//��������ͽ����û���Լ��������ɫ��ʱ��ı���ɫ��ʱ�����˸��������
					}
					break;
				case WM_LBUTTONDOWN:
					if (isButtonHovered && (hoveredButtonIndex == 0)) { 
						//��ʼ�������¿�ʼ
						init();
						play();//�̳���һ����ʲôģʽ����һ�ξ���ʲôģʽ
					}
					else if (isButtonHovered && (hoveredButtonIndex == 1)) {//������ҳ
						startGame();
					}
					break;
			}
		}
	}
	EndBatchDraw();
	closegraph(); // �ڱ�Ҫʱ�ر�ͼ�δ���
	
}


void Tetris::keyEvent()
{
	unsigned char ch;//�����û������ַ�,char���͵ķ�Χ��-128,127��224�����ⷶΧ��,���ڷ�Χ��0-255
	bool rotateFlag = false;
	int dx = 0;//�����ƶ�����xƫ����

	//�ж��������ް�������,û����жϵĻ���û��������ʱ�Ῠ����δ���
	if (_kbhit()) {
		//getchar();Ҫ�û����·�����󰴻س������������
		ch = _getch();//�û�����һ�����������ٰ��س�����������Ӧ
		switch (ch) {
			case 13:
				nextBlock=new Block();//��Ԥ�淽�������������һ����״
				update = true;
				break;
			case 32: 
				pause();//��ͣ��������ͣ���ֺͻ�����ͣͼƬ��ϵͳ����ͣ
				mciSendString("resume res/bg1.mp3", 0, 0, 0);//���¿�ʼ����
				break;
			case 224:{
				ch = _getch();
				switch (ch) {
				case 72:
					rotateFlag = true;
					break;
				case 80:
					delay = SPEED_QUICK;
					//drop();
					//update = true;
					//��������Ļ����԰����¼��Ϳ���������Ӧ����̫���˶��û����Ѻ�
					break;
				case 75:
					dx = -1;
					break;
				case 77:
					dx = 1;
					break;
				default:
					break;
				}
				break;
			}
		
		}

	}
	if (rotateFlag) {
		//����ת
		rotate();
		update = true;//���ϸ�����Ⱦ
	}
	if (dx != 0) {
		moveLeftRight(dx);
		update = true;//���ϸ�����Ⱦ
	}
}

void Tetris::updateWindow()
{
	//�������˸����Ϊ����ûһ��һ������Ⱦ,����һǰһ����Ⱦ��

	/*���䵽�ײ��Ĺ̶�������Ⱦ*/
	IMAGE** imgs = Block::getImages();//�����imgs����������Block���imgs�����ÿ��Ԫ�أ���ЩԪ�ر�����ָ�����ͣ�

	//imgs��һ�����飬ÿ��Ԫ����һ������ָ��,����imgs������IMAGE**,
	//����imgs������7��Ԫ�� 
	BeginBatchDraw();//������ͼ,������˸,�� FlushBatchDraw �� EndBatchDraw ����ʹ��

	putimage(0, 0, &imgBg);//���Ʊ���ͼƬ

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == 0) continue;//��i�е�j�е�λ���ǿհ�λ�ã�˵��û����
			int x = j * blockSize + leftMargin;//�����ˮƽx���꣬�õڼ��м���õ�
			int y = i * blockSize + topMargin;//�����y����
			putimage(x, y, imgs[map[i][j] - 1]);//x���꣬y���꣬�ĸ�������״�ķ���
			//�������imgs��updateWindow��ľֲ�����imgs,��ȡ������������
			//imgs�±��0��ʼ����map��ֵֻ��1-7������Ҫ��1
		}
	}

	//��ǰ������Ⱦ
	curBlock->draw(leftMargin + 36 * 4, topMargin);//д���ˣ�+36*4�պ������м��½���ǰ����
	//��draw(leftMargin, topMargin)���leftMargin��Ӱ�������ƶ���topMargin��Ӱ���½�����ƶ�

	//Ԥ�淽����Ⱦ��д����
	nextBlock->draw(leftMargin + 36 * 11, 70);

	clearLine();//���У�����û�������棬���Ǻ�updateWindowһ�����play��

	drawScore();//���Ʒ���

	//������Ч
	if (clear) {
		for (int j = 0; j < 12; j++) {//12��ͼƬ
			for (int i = 0; i < 20; i++) {
				if (returnLines[i] != 0) {
					putimage(leftMargin, topMargin + 36 * returnLines[i], &imgClear[j]);
				}
				//returnLines[i] = 0; 
				//����������д��ԭ����Ȼj����0��ʱ��returnLines[i]�ͱ���ԭΪ 0�ˣ��ͻ�û��̬ͼЧ��,��Ϊ��ʱ��û��Ⱦ���Ƶ�������
			}
			FlushBatchDraw();//���ϼ���ͼƬ
			Sleep(30);//0.03s����һ֡ͼƬ
		}
		clear = false;//�ָ�
		for (int i = 0; i < 20; i++) {//�ָ�
			returnLines[i] = 0;
		}
	}
	
	EndBatchDraw();

}
//��һ�ε��ã�����0��֮�󷵻ؾ����ϴε��ü���˶���ms
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;//��һ�Σ���̬����ֻ�ܳ�ʼ��һ��

	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {//�ǵ�һ�ε���
		lastTime = currentTime;//����lastTime��ֵ
		return 0;//���ص�delayֵ��0
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;//����lastTime��ֵ
		return ret;
	}
}
void Tetris::drop()
{
	//���︴�ƶ���Ч�����ã����Զ���һ�����ƹ��캯��
	//�������������Ǿ������Զ������ƹ��캯��
	backBlock = *curBlock;//curBlock��ָ�룬ǰ��*��������ָ����﷨
	curBlock->drop();
	/*�÷��鲻���½�����ͼ*/
	if (!curBlock->blockInMap(map)) {//�Ƿ�λ�ã�Ҫô1.����ǽ��2.�����ף�3.���������ѵ��÷����ϣ�
		//�ѷ���̻�
		backBlock.solidify(map);
		/*���ֿ����½����̻���Ҫ�Ĵ���*/  //solidFlag = 1;
		//��ֹ�ڴ�й©
		delete curBlock;
		//ȷ����һ�ֵķ�����״���ٸ���һ�ֵ�nextBlock
		curBlock = nextBlock;
		nextBlock = new Block;

		//�����Ϸ�Ƿ����
		checkOver();

	}
	/*���ֿ����½����̻���Ҫ�Ĵ���*/
	/*�����¼�����һֱ��ס�ͻᱣ�ֿ����½�����غ���һ���·���ָ������ٶ�*/
	//if(solidFlag==1) delay = SPEED_NORMAL;
	delay = SPEED_NORMAL[level - 1];//�½��ٶȻָ��ɵ�ǰ�ؿ��������ٶ�
}

void Tetris::clearLine()
{
	//returnLines[20] = 0;
	int n = 0;//��Ҫ���е��±���Ҫ����������
	int lines = 0;//�������˼���
	int k = rows - 1;//k�±꣺�洢���ݵ�����
	for (int i = rows - 1; i >= 0; i--) {//i�±꣺ɨ�����ݵ�����
		//����i���Ƿ�����
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//��ɨ��ߴ洢,����ʱkֵ�����������մ�Ĳ��������е�����
		}
		if (count < cols) {//û����,�洢
			k--;//�洢�±�--������
		}
		else {
			lines++;//����
			returnLines[n++] = i;
		}
	}
	if (lines > 0) {
		//����÷�
		int addScore[4] = { 10,30,60,80 };//�ӷֻ��ƣ�ͬʱ��1��10�֣�2��30��...
		score += addScore[lines - 1];
		clear = true;
		mciSendString("play res/xiaochu2.mp3", 0, 0, 0);

		update = true;


		if (endless) level = 1;
		//ÿ100��һ���ȼ� 0-100 ��һ�أ� 101-200�ڶ���
		else level = (score + 99) / 100;//���е���ʱ����score=10,���score����0ʱ��Ϊlevel��ʼֵ=1
		//����д���߽�ֵ��100+99��/100=1��(101+99)/100�պ�=2

		if (level > MAX_LEVEL) {//��ʱ����>500��ͨ��
			gameOver = true;
		}
		lineCount += lines;//��������������
	}

}

void Tetris::moveLeftRight(int offset)
{
	//offset��xƫ����
	backBlock = *curBlock;
	curBlock->moveLeftRight(offset);//��������������ƶ�
	//�����ƶ����ܻ��Ƴ��߽�
	if (!curBlock->blockInMap(map)) {//�Ƿ�λ��
		*curBlock = backBlock;//��ԭ����һ�εĺϷ�λ��
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7) return;
	//��������������ξ�ֱ��return;������ת����������ô��ת����һ��

	backBlock = *curBlock;//��¼��һ���Ϸ�λ�ã�*curBlock-Block*ָ������ñ��Block
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {//�Ƿ�λ��
		*curBlock = backBlock;//��ԭ����һ�εĺϷ�λ��
	}
}

void Tetris::drawScore()//������Ϸ�߿�����ʾ������/����˵��
{
	char scoreText[32];//�������飬������������������ؿ���

	//��ѧ������
	//����
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(255, 255, 255));
	//��������ϸ��
	LOGFONT f;
	gettextstyle(&f);//��ȡ��ǰ����
	f.lfHeight = 60;//�߶�
	f.lfWidth = 30;//���
	f.lfQuality = ANTIALIASED_QUALITY;//��������Ϊ"�����"Ч���������Ե������������

	//ǿת��ǰ����
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);//��ɵ�ǰ����
	setbkmode(TRANSPARENT);//����ı�����Ϊ͸��
	//����������ʾ��д����
	outtextxy(leftMargin + 36 * 11 + 10, topMargin + 36 * 15, scoreText);//x,y��ʼλ��

	//��߷�
	if (endless) {
		sprintf_s(scoreText, sizeof(scoreText), "%d", endlesshScore);
		outtextxy(leftMargin + 36 * 11 + 10, topMargin + 36 * 18 + 15, scoreText);//д����
	}
	else {
		sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
		outtextxy(leftMargin + 36 * 11 + 10, topMargin + 36 * 18 + 15, scoreText);//д����
	}

	//��������
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);//��ȡ���壬Ϊ������ȡ����Ŀ��
	int xPos = leftMargin - 36 - f.lfWidth * strlen(scoreText);//д����
	outtextxy(xPos, topMargin + 36 * 18 + 15, scoreText);//д����

	//�ؿ�
	if (endless) {
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, "����");
		settextcolor(RGB(255, 255, 255));
		char str[] = { "�޾�ģʽ" };
		outtextxy(120, topMargin + 36 * 15 + 15, str);
	}
	else {
		sprintf_s(scoreText, sizeof(scoreText), "%d", level);
		outtextxy(xPos, topMargin + 36 * 15, scoreText);//д����
	}

	//����˵��
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, "����");
	settextcolor(RGB(255, 255, 255));
	char str0[] = { "����˵����" };
	char str1[] = { "[��]����ת��ǰ���鷽��" };
	char str2[] = { "[��]�������ӿ������ٶ�" };
	char str3[] = { "[��]�������ƶ�����" };
	char str4[] = { "[��]�������ƶ�����" };
	char str5[] = { "[Enter]������Ԥ�淽��" };
	char str6[] = { "[�ո��]����ͣ��Ϸ" };

	outtextxy(42, 70, str0);
	outtextxy(42, 100, str1);
	outtextxy(42, 130, str2);
	outtextxy(42, 160, str3);
	outtextxy(42, 190, str4);
	outtextxy(42, 220, str5);
	outtextxy(42, 250, str6);

	
}

void Tetris::checkOver()
{
	gameOver = (curBlock->blockInMap(map) == false);//���ںϷ�λ��->��Ϸ����
}

void Tetris::saveScore()
{
	if (endless == true && score > endlesshScore) {

		endlesshScore = score;
		ofstream file(RECORDER1_FILE);//ifstream--ofstream��д
		file << endlesshScore;//д��
		file.close();

	}
	if (endless==false && score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);//ifstream--ofstream��д
		file << highestScore;//д��
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg1.mp3", 0, 0, 0);

	
	//ʤ����������ʧ�ܽ���
	if (level <= MAX_LEVEL) {
		putimage(leftMargin, 361, &imgOver);//д����
		mciSendString("play res/over.mp3", 0, 0, 0);
		Sleep(1700);
		startAgain();
	}
	else {
		putimage(leftMargin, 361, &imgWin);//д����
		mciSendString("play res/win.mp3", 0, 0, 0);
		Sleep(1700);
		startAgain(); 
	}
	
	
}

void Tetris::pause()
{
	mciSendString("pause res/bg1.mp3", 0, 0, 0);
	putimage(leftMargin + 4 * 36, 361, &imgPause);//д����
	system("pause");
}

void Tetris::button(int x, int y, TCHAR* text)
{
	int w = 200, h = 60;
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(56, 158, 154));
	fillroundrect(x, y, x + w, y + h, 10, 10);
	TCHAR s1[] = "΢���ź�";
	settextstyle(40, 0, s1);
	settextcolor(WHITE);

	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	outtextxy(tx, ty, text);
}
