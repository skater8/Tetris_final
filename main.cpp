/*开发日志
* 1.创建项目
* 2.导入素材，把素材放到项目的文件目录下面
* 3.c++开发
* 4.设计模块：
* 设计类：二者相互关联
* block方块类
* Tetris游戏类
* 5.设计各个模块的主要接口
* 6.写出函数，启动游戏
*/

#include"Tetris.h"
#include <graphics.h>


int main(void) {
	Tetris game(20, 10, 298.5, 39, 36);//列数，行数，左边界，上边界，方块大小
	game.startGame();
	return 0;
}