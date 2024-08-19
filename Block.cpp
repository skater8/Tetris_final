#include "Block.h"
#include <stdlib.h>

IMAGE * Block::imgs[7] = { NULL, };//把block类里面的imgs图片数组都设置为空
int Block::size = 36;

Block::Block()
{
    if (imgs[0] == NULL) {
        //进行一个初始化，用大图片是为了节省空间，利用代码去切割进行优化
        IMAGE imgTmp;
        loadimage(&imgTmp, "res/tiles.png");//方块的图片

        SetWorkingImage(&imgTmp);//切割
        for (int i = 0; i < 7; i++) {
            imgs[i] = new IMAGE;//为每一个数组元素分配一块内存空间来存储image类型的对象
            getimage(imgs[i], i * size, 0, size, size);//切割点的坐标，纵向都是0，然后长和宽都是size
        }
        SetWorkingImage();//恢复工作区

    }

    //1、先把俄罗斯方块的样子写出来
    int blocks[7][4] = { //用一个二维数组就可以把最基本的七种形态表示出来
    1,3,5,7, // I
    2,4,5,7, // Z 1       0  1
    3,5,4,6, // Z 2       2  3
    3,5,4,7, // T         4  5
    2,3,5,7, // L         6  7
    3,5,7,6, // J
    2,3,4,5, // 田
    };

    //2、接下来要随机生成一种俄罗斯方块
    blockType = 1 + rand() % 7;// 因为一共七种方块，所以对7取余数;+1只是因为让他变成1~7看着更顺眼一点

    // 初始化 smallBlocks
    for (int i = 0; i < 4; i++) {
        int value = blocks[blockType - 1][i];
        smallBlocks[i].row = value / 2;
        smallBlocks[i].col = value % 2;
    }

    img = imgs[blockType - 1];

}

void Block::drop()
{
    //让小方块里面的位置下降，在游戏类里面调用
    for (int i = 0; i < 4; i++) {
        smallBlocks[i].row++;
    }

}

void Block::moveLeftRight(int offset)//传入的是dx的值，所以直接+=即可
{
    for (int i = 0; i < 4; i++) {
        smallBlocks[i].col += offset;
    }
}

void Block::rotate()
{
    Point p = smallBlocks[1];//旋转中心

    //找的公式
    for (int i = 0; i < 4; i++) {
        Point tmp = smallBlocks[i];//先进行一个备份
        smallBlocks[i].col = p.col - tmp.row + p.row;
        smallBlocks[i].row = p.row + tmp.col - p.col;
    }
}

void Block::draw(int leftMargin, int topMargin)
{
    for (int i = 0; i < 4; i++) {
        int x = leftMargin + smallBlocks[i].col * size;//x坐标
        int y = topMargin + smallBlocks[i].row * size;//y坐标
        putimage(x, y, img);//直接绘制图像

    }
}
//因为imgs[]数组是block类型的静态成员变量，Tetris不能直接调用
//所以要在共有类里面建一个imgs的返回值，然后让Tetris类访问getImages的值

IMAGE** Block::getImages()
{
    return imgs;
}

//用于将一个Block对象的内容复制到另一个Block对象中
Block& Block::operator=(const Block& other)
{
    if (this == &other) return *this;//检查是否自我赋值，如果this指针和other指针指向同一个对象，则则直接返回当前对象的引用

    this->blockType = other.blockType;

    for (int i = 0; i < 4; i++) {
        this->smallBlocks[i] = other.smallBlocks[i];
    }
    return *this;
    // TODO: 在此处插入 return 语句
}

bool Block::blockInMap(const vector<vector<int>>& map)
{
    int rows = map.size();
    int cols = map[0].size();
    for (int i = 0;i < 4;i++) {//依次遍历四个小方块位置
        int realcol = smallBlocks[i].col + 4;
        //col不加4的话方块位置和整个框柱方块移动的map地图的左右边界会往右偏4个col的位置
        // 往右偏移是因为没加4个col的话相当于col比实际的位置少了4个
        // 这时当实际col=4时是对应的map里的4-4=0的col=0坐标，即map的左边界,map左坐标向右移了
        // 而当实际col=10已经达到原本col的最大值cols时，在map里只是第10-4=6的col=6坐标
        // 所以当实际col=14时才会达到map里14-4=10的右边界，map右边界右移
        //新方块从中间下降时右移了4个方块的宽度(即实际col+=4了)
        if (realcol < 0 || realcol >= cols ||
            smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
            map[smallBlocks[i].row][realcol] != 0)
            //如果在地图外,即前四个判断条件，
            //或者覆盖到已降落的方块上,即最后一个判断条件
            //（map上为0是没方块不为0是已有方块）
            return false;//直接false，判断为违法位置
    }
    return true;//四个小方块的位置都合法，那么方块在合法位置
}

void Block::solidify(vector<vector<int>>& map)
{
    for (int i = 0;i < 4;i++) {
        //设置标记，固化对应的位置
        int realcol = smallBlocks[i].col + 4;//新方块从中间下降时右移了4个方块的宽度(即实际col+=4了)
        map[smallBlocks[i].row][realcol] = blockType;

    }
}

int Block::getBlockType()
{
    return blockType;
}

