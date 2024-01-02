# CS2-Demo-and-CFG-quick-placing

## 简介

本项目用于自动快速放置Counter-Strike 2/CS2游戏配置文件以及回放录制文件（例如autoexec.cfg）。
该程序能够读取正在运行的中的cs2.exe进程，定位到其所在文件夹，并将可执行文件所在的文件夹内的.cfg/.dem文件复制到csgo/cfg目录下。

## 功能

- 读取正在运行的Counter-Strike 2窗口
- 定位到Counter-Strike 2窗口进程所在的文件夹
- 复制.dem文件到 ..\Counter-Strike Global Offensive\csgo目录
- 复制.cfg文件到 ..\Counter-Strike Global Offensive\csgo\cfg目录

## 使用方法

1. 从Steam运行CS2。
2. 在任意目录下新建一个文件夹，用来存放CS2DACQP.exe和需要被复制的cfg/dem文件。
3. 打开CMD，把CS2DACQP.exe拖入CMD窗口并回车即可运行。
4. 如果程序发现正在运行的Counter-Strike 2窗口，将会开始文件复制的过程。
5. 若程序所在文件夹拥有cfg和/或dem文件，程序会正常运行。
6. 若目标文件夹已存在相同文件名文件，程序会询问是否覆盖。

## 注意事项

- 确保CS2正在运行中，且CS2的窗口名称为Counter-Strike 2。（我这边游戏调成中文窗口名字还是Counter-Strike 2，但是似乎CSGO时代国服CSGO窗口名称叫反恐精英：全球攻势。。？总之如果不行就调成英文。
- 避免文件夹路径出现特殊字符。（应该没人搞一堆-；：‘什么乱七八糟的文件夹名字吧。。。

## 错误处理

- 如果找不到正在运行的 Counter-Strike 2 进程，程序将报错并终止运行。
- 如果程序所在文件夹同时缺少 cfg 和 dem 文件，程序将报错并终止运行。
- 其他运行时错误将被显示在控制台。

