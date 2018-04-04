# 编译器课程设计--C_Compiler
## 开发环境
CodeBlocks16.01
## 工程简介
一个C++编写的C编译器，支持扩充的C0文法（具体文法在源代码中的global.h中有说明），编译生成可执行的mips汇编代码，可在Mars4.5（MIPS模拟器）上运行。
## 调试
使用CodeBlocks打开Compiler文件夹下Compiler.cbp即可运行调试。
## 运行说明
运行编译器后在控制台输入需要编译的源代码文本文件（.txt）（C0文法编写）路径，之后键入是否优化（0/1），编译后将在程序同目录下生成MIPS汇编文件MIPS.asm（若选择优化将同时生成OptedMIPS.asm），可直接使用MARS打开运行。
## 输入示例
输入：test.txt 0（编译test.txt文件，不需优化）