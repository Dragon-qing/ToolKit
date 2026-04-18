#ifndef MSGDATA_H
#define MSGDATA_H

class MsgData
{
public:
    enum MsgType{
        REFRESH = 0, //定时器刷新消息
        REDRAW, // 重绘消息
        SETFOCUS, // 设置焦点

        TYPE_NUM // 总数
    };

private:
    MsgData();
};

#endif // MSGDATA_H
