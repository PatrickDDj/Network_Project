# Network_Project

## 启动方法
    make sender-test 用于启动发送端
    make receiver-test 用于启动接受端
    make clean 用于重置可执行文件
    （如果需要查看datagram（transport layer）、ip package（network layer）信息，只需调用各自‘打印信息’函数即可（提供在头文件中））

## 传输层
    收发UDP datagram

## 网络层
    收发ip package
    分片与组装
## 数据链路层
    收发frame
    （bin文件代替物理层）

## 效果展示
![](1.png)


