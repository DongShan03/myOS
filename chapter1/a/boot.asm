        org     07c00h      ;告诉编译器程序加载到7c00处
        mov     ax, cs      
        mov     ds, ax
        mov     es, ax
        call    DispStr     ;调用显示字符串例程
        jmp     $           ;无限循环

DispStr:
        mov     ax, BootMessage
        mov     bp, ax      ;ES:BP=串地址
        mov     cx, 16      ;CX = 串长度
        mov     ax, 01301h  ;AH=13, AL=01h
        mov     bx, 000ch   ;页号为0(BH=0) 黑底红字(BL=0Ch, 高亮)
        mov     dl, 0
        int     10h
        ret

BootMessage:        db      "Hello, OS world!"
times   510-($-$$)  db         0        ;填充剩下的区域,使生成的二进制文件大小为512B
dw      0xaa55              ;解释标志



;作为一个引导磁盘,首先需要以0xAA55结束,并且必须为512B