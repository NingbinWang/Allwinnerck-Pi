# Allwinnerck-pi
这个项目是在LC-CherryPi-F1C200S的基础板上验证软件,后续自行设计一个带摄像头，并带SPI小型屏幕与GPS的核心开发板，需要支持语音识别功能。
设计上会改为EMMC启动，尽量使用4层板来解决硬件。

# 芯片级介绍

![](/1.Docs/4.Images/soc.png)

# 硬件设计
功能设计：
MIPI的CSI摄像机接口
SPI 24 PIN屏幕(带TE脚用于数据同步)
采用邮票式接口


# 软件设计
1.ISP的接入
2.SD卡录像记录

# 参考
