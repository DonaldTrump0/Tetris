#pragma once

class CBlock
{
public:
    CBlock();
    int GetCurBlockIndex();
    int GetCurBlockRow();
    int GetCurBlockCol();
    void SetCurBlockIndex(int blockIndex);
    void SetCurBlockRow(int blockRow);
    void SetCurBlockCol(int blockCol);

public:
    const static char blocks[][4][4];

private:
    // ��ǰ��������
    int curBlockIndex;
    // ��ǰ���������к���
    int curBlockRow;
    int curBlockCol;

};

