#include "Tetris.h"
#include <time.h> //srand(time(NULL))需要
#include <stdlib.h>
#include <iostream>
#include "Block.h"
#include <conio.h> //定义控制台输入输出的函数，用户通过按键盘产生的对应操作
#include <fstream> //文件读写的
#include <graphics.h> //c++图形库
#include <mmsystem.h> //播放音乐头文件
#pragma comment(lib,"winmm.lib") //静态库,播放音乐用
#undef UNICODE

#define MAX_LEVEL 5
#define RECORDER_FILE "recorder.txt"
#define RECORDER1_FILE "recorder1.txt"

const int SPEED_NORMAL[MAX_LEVEL] = { 500,400,300,200,100 };//常量，速度
const int SPEED_QUICK = 50;//常量，快速下降


Tetris::Tetris(int rows, int cols, int left, int top, int blockSize)//构造函数
{
	//地图的 行列,左、上边界,方块大小 只需初始化一次，所以放在构造函数里
	this->rows = rows;
	this->cols = cols;
	leftMargin = left;
	topMargin = top;
	this->blockSize = blockSize;
	for (int i = 0; i < rows; i++) {
		vector<int> mapRow;//定义空行
		for (int j = 0; j < cols; j++) {
			mapRow.push_back(0);//在数字最后面增加一个零
		}
		map.push_back(mapRow);//把一维数组mapRow（一行）加到map地图里
		//push_back将一个新的元素加到vector的最后面
	}


}

void Tetris::init()
{

	mciSendString("play res/bg1.mp3 repeat", 0, 0, 0);//背景音乐
	//参数 (操作指令, 获取返回的信息(字符串), 返回信息的长度, 窗口句柄(一般为NULL));

	delay = SPEED_NORMAL[0];
	srand(time(NULL)); //用系统时间来做种子值，真正的随机数；time(NULL)获取系统时间，单位为秒;
	//创建游戏窗口
	initgraph(938, 816);//这里写死了

	//加载图片
	loadimage(&imgBg, "res/bg1.png");//背景
	loadimage(&imgWin, "res/win.png");//赢
	loadimage(&imgOver, "res/over.png");//失败
	loadimage(&imgPause, "res/pause.png", 80, 80);//图像的IMAGE对象指针，图片地址，拉伸宽度,高度
	//图像的IMAGE对象指针（&变量名=变量地址，指针指向的就是变量地址）
	//相对地址“./”本目录下的文件进行访问   图片展示可以是png也可以是jpg
	loadimage(&imgClear[0], "res/tx/0.png", 36 * 10, 36);//消行特效图
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


	//当玩完一局时，重新初始化游戏区的数据
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			map[i][j] = 0;
		}
	}

	//变量（标志变量）的初始化，
	//在头文件初始化的话没办法每局都重新初始化，所以要写在init函数里
	score = 0;
	lineCount = 0;
	level = 1;
	gameOver = false;
	clear = false;

	//初始化最高分
	if (endless) {
		ifstream file(RECORDER1_FILE);//文件名
		if (!file.is_open()) {//没打开
			cout << RECORDER1_FILE << "打开失败" << endl;
			endlesshScore = 0;
		}
		else {
			file >> endlesshScore;
		}
		file.close();//关闭文件
	}
	else {
		ifstream file(RECORDER_FILE);//文件名
		if (!file.is_open()) {//没打开
			cout << RECORDER_FILE << "打开失败" << endl;
			highestScore = 0;
		}

		else {
			file >> highestScore;//从文件输入数据
		}
		file.close();//关闭文件
	}

}

void Tetris::play()
{
	init();

	//创建方块
	nextBlock = new Block();
	curBlock = nextBlock;
	nextBlock = new Block();

	int timer = 0;
	while (1) {
		//接受用户的输入
		keyEvent();
		//每隔30ms渲染一次画面
		timer += getDelay();
		if (timer > delay) {
			timer = 0;//清理，保证下次又隔30ms渲染
			drop();//方块下降
			// 渲染游戏画面
			update = true;
		}

		if (update) {//用update作标记可以在keyEvnet用户输入时也实现渲染更新
			//更新游戏画面
			update = false;
			updateWindow();
		}

		if (gameOver) {//游戏结束
			//保存分数
			saveScore();
			//更新游戏结束界面
			displayOver();
			
			//让画面不动，系统暂停
			//system("pause");
			//现在这句可不写了，当初还没写displayOver界面时才得加这句

		}//如果要结束时不会新方块覆盖旧方块就把这个if(gameOver)的判断放在if(update)前

	}
}

void Tetris::startGame()
{
	endless = false;//每局初始化，才能回主页后点闯关是闯关，点无尽是无尽
	loadimage(&imgBegin, "res/beginbg1.png");
	initgraph(938, 816);
	BeginBatchDraw();
	putimage(0, 0, &imgBegin);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, "微软雅黑");
	settextcolor(RGB(255, 255, 255));
	char str[] = { "欢迎来到，很帅的俄罗斯方块游戏" };
	outtextxy(leftMargin - 80, topMargin + 100, str);

	int gap = 40;
	int starty = 330;
	int height = 80;

	TCHAR title[2][50] = { "闯关模式", "无尽模式" };

	for (int i = 0; i < 2; i++) {
		button(375, starty + i * height + i * gap, title[i]);
	}
	

	ExMessage msg;
	bool isButtonHovered = false; // 用于标记鼠标是否悬停在按钮上
	int hoveredButtonIndex = -1; // 用于记录悬停的按钮索引,判断是应该进闯关模式还是进无尽模式

	while (true) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message) {
			case WM_MOUSEMOVE:
				isButtonHovered = false; // 每次移动前先设置为false
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
					//绘制出变完样子的按钮
					fillroundrect(375, starty + i * height + i * gap, 575, starty + i * height + i * gap + 60, 10, 10);
					outtextxy(375 + (200 - textwidth(title[i])) / 2, starty + i * height + i * gap + (60 - textheight(title[i])) / 2, title[i]);
					FlushBatchDraw();//加上这个就解决了没用自己定义的颜色的时候的变颜色的时候会闪烁的问题了
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
	closegraph(); // 在必要时关闭图形窗口
}

void Tetris::startAgain()
{
	loadimage(&imgBegin, "res/beginbg1.png");
	initgraph(938, 816);
	BeginBatchDraw();
	putimage(0, 0, &imgBegin);
	setbkmode(TRANSPARENT);
	settextstyle(70, 0, "微软雅黑");
	settextcolor(RGB(255, 255, 255));
	char str[] = { "游戏结束" };
	outtextxy(365, topMargin + 100, str);

	int gap = 40;
	int starty = 330;
	int height = 80;

	TCHAR title[2][50] = { "再玩一局", "返回主页" };

	for (int i = 0; i < 2; i++) {
		button(375, starty + i * height + i * gap, title[i]);
	}

	ExMessage msg;
	bool isButtonHovered = false; // 用于标记鼠标是否悬停在按钮上
	int hoveredButtonIndex = -1; // 用于记录悬停的按钮索引,判断是应该再玩一局还是退出游戏

	while (true) {
		if (peekmessage(&msg, EM_MOUSE)) {
			switch (msg.message) {
				case WM_MOUSEMOVE:
					isButtonHovered = false; // 每次移动前先设置为false
					hoveredButtonIndex = -1;

					for (int i = 0; i < 2; i++) {
						if (msg.x >= 375 && msg.x <= 575 && msg.y >= starty + i * height + i * gap && msg.y <= starty + i * height + i * gap + 60) {
							setfillcolor(RGB(143, 87, 150));//加上rgb获取想要的颜色的话就会闪烁   为什么闯关模式不会闪，无尽模式会闪？
							isButtonHovered = true;
							hoveredButtonIndex = i;
						}
						else {
							setfillcolor(RGB(56, 158, 154));
						}
						//绘制出变完样子的按钮
						fillroundrect(375, starty + i * height + i * gap, 575, starty + i * height + i * gap + 60, 10, 10);
						outtextxy(375 + (200 - textwidth(title[i])) / 2, starty + i * height + i * gap + (60 - textheight(title[i])) / 2, title[i]);
						FlushBatchDraw();//加上这个就解决了没用自己定义的颜色的时候的变颜色的时候会闪烁的问题了
					}
					break;
				case WM_LBUTTONDOWN:
					if (isButtonHovered && (hoveredButtonIndex == 0)) { 
						//初始化，重新开始
						init();
						play();//继承上一局是什么模式再玩一次就是什么模式
					}
					else if (isButtonHovered && (hoveredButtonIndex == 1)) {//返回主页
						startGame();
					}
					break;
			}
		}
	}
	EndBatchDraw();
	closegraph(); // 在必要时关闭图形窗口
	
}


void Tetris::keyEvent()
{
	unsigned char ch;//接收用户输入字符,char类型的范围是-128,127，224不在这范围里,现在范围是0-255
	bool rotateFlag = false;
	int dx = 0;//左右移动——x偏移量

	//判断现在有无按键输入,没这个判断的话当没按键输入时会卡在这段代码
	if (_kbhit()) {
		//getchar();要用户按下方向键后按回车才能输入完成
		ch = _getch();//用户按下一个按键后不用再按回车就能做出反应
		switch (ch) {
			case 13:
				nextBlock=new Block();//给预告方块再重新随机赋一个形状
				update = true;
				break;
			case 32: 
				pause();//暂停，包括暂停音乐和绘制暂停图片和系统的暂停
				mciSendString("resume res/bg1.mp3", 0, 0, 0);//重新开始音乐
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
					//加这两句的话可以按向下键就快速做出反应，但太快了对用户不友好
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
		//做旋转
		rotate();
		update = true;//马上更新渲染
	}
	if (dx != 0) {
		moveLeftRight(dx);
		update = true;//马上更新渲染
	}
}

void Tetris::updateWindow()
{
	//方块会闪烁是因为方块没一起一次性渲染,而是一前一后渲染的

	/*降落到底部的固定方块渲染*/
	IMAGE** imgs = Block::getImages();//这里的imgs数组里存的是Block类的imgs数组的每个元素（这些元素本身是指针类型）

	//imgs是一个数组，每个元素是一个数组指针,所以imgs类型是IMAGE**,
	//所以imgs里存的是7种元素 
	BeginBatchDraw();//批量绘图,消除闪烁,和 FlushBatchDraw 或 EndBatchDraw 配套使用

	putimage(0, 0, &imgBg);//绘制背景图片

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if (map[i][j] == 0) continue;//第i行第j列的位置是空白位置，说明没方块
			int x = j * blockSize + leftMargin;//方块的水平x坐标，用第几列计算得到
			int y = i * blockSize + topMargin;//方块的y坐标
			putimage(x, y, imgs[map[i][j] - 1]);//x坐标，y坐标，哪个类型形状的方块
			//函数里的imgs是updateWindow里的局部变量imgs,是取到的类型数组
			//imgs下标从0开始，而map的值只有1-7，所以要减1
		}
	}

	//当前方块渲染
	curBlock->draw(leftMargin + 36 * 4, topMargin);//写死了，+36*4刚好能在中间下降当前方块
	//改draw(leftMargin, topMargin)里的leftMargin会影响左右移动，topMargin会影响下降后的移动

	//预告方块渲染，写死了
	nextBlock->draw(leftMargin + 36 * 11, 70);

	clearLine();//消行，本来没放这里面，而是和updateWindow一起放在play里

	drawScore();//绘制分数

	//消除特效
	if (clear) {
		for (int j = 0; j < 12; j++) {//12张图片
			for (int i = 0; i < 20; i++) {
				if (returnLines[i] != 0) {
					putimage(leftMargin, topMargin + 36 * returnLines[i], &imgClear[j]);
				}
				//returnLines[i] = 0; 
				//不能在这里写还原，不然j还在0的时候returnLines[i]就被还原为 0了，就会没动态图效果,因为这时还没渲染绘制到画面上
			}
			FlushBatchDraw();//马上加载图片
			Sleep(30);//0.03s加载一帧图片
		}
		clear = false;//恢复
		for (int i = 0; i < 20; i++) {//恢复
			returnLines[i] = 0;
		}
	}
	
	EndBatchDraw();

}
//第一次调用，返回0；之后返回距离上次调用间隔了多少ms
int Tetris::getDelay()
{
	static unsigned long long lastTime = 0;//第一次，静态变量只能初始化一次

	unsigned long long currentTime = GetTickCount();
	if (lastTime == 0) {//是第一次调用
		lastTime = currentTime;//更新lastTime的值
		return 0;//返回的delay值是0
	}
	else {
		int ret = currentTime - lastTime;
		lastTime = currentTime;//更新lastTime的值
		return ret;
	}
}
void Tetris::drop()
{
	//这里复制对象效果不好，可以定义一个复制构造函数
	//定义完后下面的那句语句会自动调复制构造函数
	backBlock = *curBlock;//curBlock是指针，前加*解引用是指针的语法
	curBlock->drop();
	/*让方块不会下降出地图*/
	if (!curBlock->blockInMap(map)) {//非法位置（要么1.碰到墙，2.碰到底，3.碰到其他已调用方块上）
		//把方块固化
		backBlock.solidify(map);
		/*保持快速下降至固化需要的代码*/  //solidFlag = 1;
		//防止内存泄漏
		delete curBlock;
		//确定下一轮的方块形状，再改下一轮的nextBlock
		curBlock = nextBlock;
		nextBlock = new Block;

		//检查游戏是否结束
		checkOver();

	}
	/*保持快速下降至固化需要的代码*/
	/*按向下键后不用一直按住就会保持快速下降至落地后，下一个新方块恢复正常速度*/
	//if(solidFlag==1) delay = SPEED_NORMAL;
	delay = SPEED_NORMAL[level - 1];//下降速度恢复成当前关卡的正常速度
}

void Tetris::clearLine()
{
	//returnLines[20] = 0;
	int n = 0;//存要消行的下标需要的数组索引
	int lines = 0;//计算消了几行
	int k = rows - 1;//k下标：存储数据的行数
	for (int i = rows - 1; i >= 0; i--) {//i下标：扫描数据的行数
		//检查第i行是否满行
		int count = 0;
		for (int j = 0; j < cols; j++) {
			if (map[i][j]) {
				count++;
			}
			map[k][j] = map[i][j];//边扫描边存储,满行时k值不变所以最终存的不会是满行的数据
		}
		if (count < cols) {//没满行,存储
			k--;//存储下标--往上走
		}
		else {
			lines++;//满行
			returnLines[n++] = i;
		}
	}
	if (lines > 0) {
		//计算得分
		int addScore[4] = { 10,30,60,80 };//加分机制，同时消1行10分，2行30分...
		score += addScore[lines - 1];
		clear = true;
		mciSendString("play res/xiaochu2.mp3", 0, 0, 0);

		update = true;


		if (endless) level = 1;
		//每100分一个等级 0-100 第一关， 101-200第二关
		else level = (score + 99) / 100;//运行到这时至少score=10,因此score等于0时仍为level初始值=1
		//这样写，边界值（100+99）/100=1，(101+99)/100刚好=2

		if (level > MAX_LEVEL) {//这时分数>500，通关
			gameOver = true;
		}
		lineCount += lines;//更新总消除行数
	}

}

void Tetris::moveLeftRight(int offset)
{
	//offset是x偏移量
	backBlock = *curBlock;
	curBlock->moveLeftRight(offset);//调方块类的左右移动
	//左右移动可能会移出边界
	if (!curBlock->blockInMap(map)) {//非法位置
		*curBlock = backBlock;//还原到上一次的合法位置
	}
}

void Tetris::rotate()
{
	if (curBlock->getBlockType() == 7) return;
	//如果类型是田字形就直接return;不用旋转，田字形怎么旋转都长一样

	backBlock = *curBlock;//记录上一个合法位置，*curBlock-Block*指针解引用变成Block
	curBlock->rotate();

	if (!curBlock->blockInMap(map)) {//非法位置
		*curBlock = backBlock;//还原到上一次的合法位置
	}
}

void Tetris::drawScore()//绘制游戏边框外显示的文字/操作说明
{
	char scoreText[32];//数字数组，公用与分数，行数，关卡等

	//需学！！！
	//分数
	sprintf_s(scoreText, sizeof(scoreText), "%d", score);
	setcolor(RGB(255, 255, 255));
	//设置字体细节
	LOGFONT f;
	gettextstyle(&f);//获取当前字体
	f.lfHeight = 60;//高度
	f.lfWidth = 30;//宽度
	f.lfQuality = ANTIALIASED_QUALITY;//设置字体为"抗锯齿"效果，不会边缘挡道其他东西

	//强转当前字体
	strcpy_s(f.lfFaceName, sizeof(f.lfFaceName), _T("Segoe UI Black"));
	settextstyle(&f);//设成当前字体
	setbkmode(TRANSPARENT);//字体的背景设为透明
	//分数绘制显示，写死了
	outtextxy(leftMargin + 36 * 11 + 10, topMargin + 36 * 15, scoreText);//x,y起始位置

	//最高分
	if (endless) {
		sprintf_s(scoreText, sizeof(scoreText), "%d", endlesshScore);
		outtextxy(leftMargin + 36 * 11 + 10, topMargin + 36 * 18 + 15, scoreText);//写死了
	}
	else {
		sprintf_s(scoreText, sizeof(scoreText), "%d", highestScore);
		outtextxy(leftMargin + 36 * 11 + 10, topMargin + 36 * 18 + 15, scoreText);//写死了
	}

	//消灭行数
	sprintf_s(scoreText, sizeof(scoreText), "%d", lineCount);
	gettextstyle(&f);//获取字体，为了下行取字体的宽度
	int xPos = leftMargin - 36 - f.lfWidth * strlen(scoreText);//写死了
	outtextxy(xPos, topMargin + 36 * 18 + 15, scoreText);//写死了

	//关卡
	if (endless) {
		setbkmode(TRANSPARENT);
		settextstyle(40, 0, "黑体");
		settextcolor(RGB(255, 255, 255));
		char str[] = { "无尽模式" };
		outtextxy(120, topMargin + 36 * 15 + 15, str);
	}
	else {
		sprintf_s(scoreText, sizeof(scoreText), "%d", level);
		outtextxy(xPos, topMargin + 36 * 15, scoreText);//写死了
	}

	//操作说明
	setbkmode(TRANSPARENT);
	settextstyle(20, 0, "宋体");
	settextcolor(RGB(255, 255, 255));
	char str0[] = { "操作说明：" };
	char str1[] = { "[↑]：旋转当前方块方向" };
	char str2[] = { "[↓]：长按加快下落速度" };
	char str3[] = { "[←]：向左移动方块" };
	char str4[] = { "[→]：向右移动方块" };
	char str5[] = { "[Enter]：更改预告方块" };
	char str6[] = { "[空格键]：暂停游戏" };

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
	gameOver = (curBlock->blockInMap(map) == false);//不在合法位置->游戏结束
}

void Tetris::saveScore()
{
	if (endless == true && score > endlesshScore) {

		endlesshScore = score;
		ofstream file(RECORDER1_FILE);//ifstream--ofstream读写
		file << endlesshScore;//写入
		file.close();

	}
	if (endless==false && score > highestScore) {
		highestScore = score;
		ofstream file(RECORDER_FILE);//ifstream--ofstream读写
		file << highestScore;//写入
		file.close();
	}
}

void Tetris::displayOver()
{
	mciSendString("stop res/bg1.mp3", 0, 0, 0);

	
	//胜利结束还是失败结束
	if (level <= MAX_LEVEL) {
		putimage(leftMargin, 361, &imgOver);//写死了
		mciSendString("play res/over.mp3", 0, 0, 0);
		Sleep(1700);
		startAgain();
	}
	else {
		putimage(leftMargin, 361, &imgWin);//写死了
		mciSendString("play res/win.mp3", 0, 0, 0);
		Sleep(1700);
		startAgain(); 
	}
	
	
}

void Tetris::pause()
{
	mciSendString("pause res/bg1.mp3", 0, 0, 0);
	putimage(leftMargin + 4 * 36, 361, &imgPause);//写死了
	system("pause");
}

void Tetris::button(int x, int y, TCHAR* text)
{
	int w = 200, h = 60;
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(56, 158, 154));
	fillroundrect(x, y, x + w, y + h, 10, 10);
	TCHAR s1[] = "微软雅黑";
	settextstyle(40, 0, s1);
	settextcolor(WHITE);

	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	outtextxy(tx, ty, text);
}
