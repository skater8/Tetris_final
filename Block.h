#pragma once
#include <graphics.h> //��Ҫ�Ȱ�װeasyxͼ�ο⣡��
#include <vector>
using namespace std;

struct Point {//����һ���ṹ�����飬���Խ���Ҫʹ�õ����ݴ����һ�𣬱�ʾ�����������λ��������ڼ��еڼ��У�д�ṹ����Ϊ���ÿ�������
    int row;
    int col;
};

class Block
{
    //д����������Ľӿڷֱ��ǣ��½��������ƶ�����ת����ͼ��
public:
    Block();//�޲ι��캯����Ĭ�Ϲ��캯��������ֻ����һ��Ĭ�Ϲ��캯�������ɷ���
    void drop();
    void moveLeftRight(int offset);//������
    void rotate();//��ת
    void draw(int leftMargin, int topMargin);
    //��Ϊimgs[]������block���͵ľ�̬��Ա������Tetris����ֱ�ӵ���
    //����Ҫ�ڹ��������潨һ��imgs�ķ���ֵ��Ȼ����Tetris�����getImages��ֵ
    static IMAGE** getImages();//��̬�������ܵ��þ�̬����
    Block& operator = (const Block& other);//Block&��ʾ����
    //ֱ���жϷ����ǲ����ڵ�ͼ����
    bool blockInMap(const vector<vector<int>>& map);//��ά��ͼ

    void solidify(vector<vector<int>>& map);//�̻�����
    int getBlockType();//������״

private:
    int blockType;//����һ�·��������
    Point smallBlocks[4];//�ʼ���ĸ�С�����λ��
    IMAGE* img;//��Ϊ�ܶ�ͼƬ����һ���ģ����Ը�һ��ָ������Ż�����

    static IMAGE* imgs[7]; //���������๲�е�ͼƬ����
    static int size;//ͼƬ��С��С�����С

};