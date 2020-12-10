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
    // 当前方块类型
    int curBlockIndex;
    // 当前方块所在行和列
    int curBlockRow;
    int curBlockCol;

};

