#include "Block.h"
#include <stdlib.h>

IMAGE * Block::imgs[7] = { NULL, };//��block�������imgsͼƬ���鶼����Ϊ��
int Block::size = 36;

Block::Block()
{
    if (imgs[0] == NULL) {
        //����һ����ʼ�����ô�ͼƬ��Ϊ�˽�ʡ�ռ䣬���ô���ȥ�и�����Ż�
        IMAGE imgTmp;
        loadimage(&imgTmp, "res/tiles.png");//�����ͼƬ

        SetWorkingImage(&imgTmp);//�и�
        for (int i = 0; i < 7; i++) {
            imgs[i] = new IMAGE;//Ϊÿһ������Ԫ�ط���һ���ڴ�ռ����洢image���͵Ķ���
            getimage(imgs[i], i * size, 0, size, size);//�и������꣬������0��Ȼ�󳤺Ϳ���size
        }
        SetWorkingImage();//�ָ�������

    }

    //1���ȰѶ���˹���������д����
    int blocks[7][4] = { //��һ����ά����Ϳ��԰��������������̬��ʾ����
    1,3,5,7, // I
    2,4,5,7, // Z 1       0  1
    3,5,4,6, // Z 2       2  3
    3,5,4,7, // T         4  5
    2,3,5,7, // L         6  7
    3,5,7,6, // J
    2,3,4,5, // ��
    };

    //2��������Ҫ�������һ�ֶ���˹����
    blockType = 1 + rand() % 7;// ��Ϊһ�����ַ��飬���Զ�7ȡ����;+1ֻ����Ϊ�������1~7���Ÿ�˳��һ��

    // ��ʼ�� smallBlocks
    for (int i = 0; i < 4; i++) {
        int value = blocks[blockType - 1][i];
        smallBlocks[i].row = value / 2;
        smallBlocks[i].col = value % 2;
    }

    img = imgs[blockType - 1];

}

void Block::drop()
{
    //��С���������λ���½�������Ϸ���������
    for (int i = 0; i < 4; i++) {
        smallBlocks[i].row++;
    }

}

void Block::moveLeftRight(int offset)//�������dx��ֵ������ֱ��+=����
{
    for (int i = 0; i < 4; i++) {
        smallBlocks[i].col += offset;
    }
}

void Block::rotate()
{
    Point p = smallBlocks[1];//��ת����

    //�ҵĹ�ʽ
    for (int i = 0; i < 4; i++) {
        Point tmp = smallBlocks[i];//�Ƚ���һ������
        smallBlocks[i].col = p.col - tmp.row + p.row;
        smallBlocks[i].row = p.row + tmp.col - p.col;
    }
}

void Block::draw(int leftMargin, int topMargin)
{
    for (int i = 0; i < 4; i++) {
        int x = leftMargin + smallBlocks[i].col * size;//x����
        int y = topMargin + smallBlocks[i].row * size;//y����
        putimage(x, y, img);//ֱ�ӻ���ͼ��

    }
}
//��Ϊimgs[]������block���͵ľ�̬��Ա������Tetris����ֱ�ӵ���
//����Ҫ�ڹ��������潨һ��imgs�ķ���ֵ��Ȼ����Tetris�����getImages��ֵ

IMAGE** Block::getImages()
{
    return imgs;
}

//���ڽ�һ��Block��������ݸ��Ƶ���һ��Block������
Block& Block::operator=(const Block& other)
{
    if (this == &other) return *this;//����Ƿ����Ҹ�ֵ�����thisָ���otherָ��ָ��ͬһ����������ֱ�ӷ��ص�ǰ���������

    this->blockType = other.blockType;

    for (int i = 0; i < 4; i++) {
        this->smallBlocks[i] = other.smallBlocks[i];
    }
    return *this;
    // TODO: �ڴ˴����� return ���
}

bool Block::blockInMap(const vector<vector<int>>& map)
{
    int rows = map.size();
    int cols = map[0].size();
    for (int i = 0;i < 4;i++) {//���α����ĸ�С����λ��
        int realcol = smallBlocks[i].col + 4;
        //col����4�Ļ�����λ�ú��������������ƶ���map��ͼ�����ұ߽������ƫ4��col��λ��
        // ����ƫ������Ϊû��4��col�Ļ��൱��col��ʵ�ʵ�λ������4��
        // ��ʱ��ʵ��col=4ʱ�Ƕ�Ӧ��map���4-4=0��col=0���꣬��map����߽�,map��������������
        // ����ʵ��col=10�Ѿ��ﵽԭ��col�����ֵcolsʱ����map��ֻ�ǵ�10-4=6��col=6����
        // ���Ե�ʵ��col=14ʱ�Ż�ﵽmap��14-4=10���ұ߽磬map�ұ߽�����
        //�·�����м��½�ʱ������4������Ŀ��(��ʵ��col+=4��)
        if (realcol < 0 || realcol >= cols ||
            smallBlocks[i].row < 0 || smallBlocks[i].row >= rows ||
            map[smallBlocks[i].row][realcol] != 0)
            //����ڵ�ͼ��,��ǰ�ĸ��ж�������
            //���߸��ǵ��ѽ���ķ�����,�����һ���ж�����
            //��map��Ϊ0��û���鲻Ϊ0�����з��飩
            return false;//ֱ��false���ж�ΪΥ��λ��
    }
    return true;//�ĸ�С�����λ�ö��Ϸ�����ô�����ںϷ�λ��
}

void Block::solidify(vector<vector<int>>& map)
{
    for (int i = 0;i < 4;i++) {
        //���ñ�ǣ��̻���Ӧ��λ��
        int realcol = smallBlocks[i].col + 4;//�·�����м��½�ʱ������4������Ŀ��(��ʵ��col+=4��)
        map[smallBlocks[i].row][realcol] = blockType;

    }
}

int Block::getBlockType()
{
    return blockType;
}

