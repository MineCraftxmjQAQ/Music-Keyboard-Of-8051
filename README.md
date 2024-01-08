# Music-Keyboard-Of-8051
在8051单片机上运行的音乐键盘  
本程序使用普中A2开发板套件运行，单片机使用STC89C516RD+  
矩阵键盘定义：  
00 01 02 03  
04 05 06 07  
08 09 0A 0B  
0C 0D 0E 0F  
音阶顺序为：0C 0D 0E 0F 08 09 0A 0B 04 05 06 07  
使用方法：  
1.启动开发板电源，初始时为中音段  
2.短按[02]将降低八度，短按[03]将升高八度  
3.按下[04~0F]时蜂鸣器会响起对应音阶的音
