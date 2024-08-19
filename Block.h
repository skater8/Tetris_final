#pragma once
#include <graphics.h> //需要先安装easyx图形库！！
#include <vector>
using namespace std;

struct Point {//定义一个结构体数组，可以将想要使用的数据打包在一起，表示出方块下落的位置在哪里，第几行第几列，写结构体是为了让开发更简单
    int row;
    int col;
};

class Block
{
    //写出几个对外的接口分别是：下降，左右移动，旋转，画图。
public:
    Block();//无参构造函数，默认构造函数，并且只能有一个默认构造函数，生成方块
    void drop();
    void moveLeftRight(int offset);//左右移
    void rotate();//旋转
    void draw(int leftMargin, int topMargin);
    //因为imgs[]数组是block类型的静态成员变量，Tetris不能直接调用
    //所以要在共有类里面建一个imgs的返回值，然后让Tetris类访问getImages的值
    static IMAGE** getImages();//静态函数才能调用静态变量
    Block& operator = (const Block& other);//Block&表示引用
    //直接判断方块是不是在地图里面
    bool blockInMap(const vector<vector<int>>& map);//二维地图

    void solidify(vector<vector<int>>& map);//固化方块
    int getBlockType();//方块形状

private:
    int blockType;//定义一下方块的类型
    Point smallBlocks[4];//最开始的四个小方块的位置
    IMAGE* img;//因为很多图片都是一样的，所以搞一个指针可以优化代码

    static IMAGE* imgs[7]; //整个方块类共有的图片数组
    static int size;//图片大小，小方块大小

};