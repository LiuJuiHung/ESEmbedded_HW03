HW03
===
This is the hw03 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw03 dir, use:

	* `make` to build.

	* `make clean` to clean the ouput files.

4. Extract `gnu-mcu-eclipse-qemu.zip` into hw03 dir. Under the path of hw03, start emulation with `make qemu`.

	See [Lecture 02 ─ Emulation with QEMU] for more details.

5. The sample is a minimal program for ARM Cortex-M4 devices, which enters `while(1);` after reset. Use gdb to get more details.

	See [ESEmbedded_HW02_Example] for knowing how to do the observation and how to use markdown for taking notes.

# Build Your Own Program

1. Edit main.c.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C Standard library functions like `printf`, `malloc`, etc.

# HW03 Requirements

1. How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

2. Modify main.c to observe what you found.

3. You have to state how you designed the observation (code), and how you performed it.

	Just like how you did in HW02.

3. If there are any official data that define the rules, you can also use them as references.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files or executable files and the qemu bin folder)

[Lecture 02 ─ Emulation with QEMU]: http://www.nc.es.ncku.edu.tw/course/embedded/02/#Emulation-with-QEMU
[ESEmbedded_HW02_Example]: https://github.com/vwxyzjimmy/ESEmbedded_HW02_Example

--------------------

- [ ] **If you volunteer to give the presentation next week, check this.**

--------------------

**★★★ Please take your note here ★★★**
ECE_HW03
===
###### tags: `嵌入式韌體開發與作業系統`

## 實驗題目
How do C functions pass and return parameters? Please describe the related standard used by the Application Binary Interface (ABI) for the ARM architecture.

## [AAPCS Procedure Call Standard for the ARM® Architecture](http://infocenter.arm.com/help/topic/com.arm.doc.ihi0042f/IHI0042F_aapcs.pdf)
THE BASE PROCEDURE CALL STANDARD

1. Core registers
![](https://i.imgur.com/M68B7rx.png)

2. The first four registers r0-r3 (a1-a4) are used to pass argument values into a subroutine and to return a result value from a function. They may also be used to hold intermediate values within a routine (but, in general, only between subroutine calls).

3. Subroutine Calls
Both the ARM and Thumb instruction sets contain a primitive subroutine call instruction, BL, which performs a branch-with-link operation. The effect of executing BL is to transfer the sequentially next value of the program counter —the return address—into the link register (LR) and the destination address into the program counter (PC). Bit 0 of the link register will be set to 1 if the BL instruction was executed from Thumb state, and to 0 if executed from ARM state. The result is to transfer control to the destination address, passing the return address in LR as an additional parameter to the called subroutine.

4. Result Return
A Half-precision Floating Point Type is returned in the least significant 16 bits of r0.
A Fundamental Data Type that is smaller than 4 bytes is zero- or sign-extended to a word and returned in r0.

5. Parameter Passing
The base standard provides for passing arguments in core registers (r0-r3) and on the stack. For subroutines that take a small number of parameters, only registers are used, greatly reducing the overhead of a call.



## 實驗步驟
1. 編輯 main.c 後，`$make` 編譯
    * 使用一個 `func(3 , 6)` 此 func 裡是做 `+` 法運算，透過 `gnu-mcu-eclipse-qemu` 觀察傳遞參數時，register 如何運作
    * main.c
    ```C=1
    void reset_handler(void)
    {
        int a = 3;
        int b = 6;
        int c = 15;
        int d = 4;
        int e = 23;
        sum(a, b);
        func(a, b, c, d, e);
        while (1)
            ;
    }

    int sum(int d, int e)
    {
        int f = d + e;
        return f;
    }

    int func(int aa, int bb, int cc, int dd, int ee)
    {
        int ff = aa + bb + cc + dd + ee;
        return ff;
    }
    ```
    
    * make 後
    ``` 
    Disassembly of section .mytext:
    
    00000000 <reset_handler-0x8>:
       0:   20000100        andcs   r0, r0, r0, lsl #2
       4:   00000009        andeq   r0, r0, r9

    00000008 <reset_handler>:
       8:   b580            push    {r7, lr}
       a:   b088            sub     sp, #32
       c:   af02            add     r7, sp, #8
       e:   2303            movs    r3, #3
      10:   617b            str     r3, [r7, #20]
      12:   2306            movs    r3, #6
      14:   613b            str     r3, [r7, #16]
      16:   230f            movs    r3, #15
      18:   60fb            str     r3, [r7, #12]
      1a:   2304            movs    r3, #4
      1c:   60bb            str     r3, [r7, #8]
      1e:   2317            movs    r3, #23
      20:   607b            str     r3, [r7, #4]
      22:   6978            ldr     r0, [r7, #20]
      24:   6939            ldr     r1, [r7, #16]
      26:   f000 f809       bl      3c <sum>
      2a:   687b            ldr     r3, [r7, #4]
      2c:   9300            str     r3, [sp, #0]
      2e:   6978            ldr     r0, [r7, #20]
      30:   6939            ldr     r1, [r7, #16]
      32:   68fa            ldr     r2, [r7, #12]
      34:   68bb            ldr     r3, [r7, #8]
      36:   f000 f811       bl      5c <func>
      3a:   e7fe            b.n     3a <reset_handler+0x32>

    0000003c <sum>:
      3c:   b480            push    {r7}
      3e:   b085            sub     sp, #20
      40:   af00            add     r7, sp, #0
      42:   6078            str     r0, [r7, #4]
      44:   6039            str     r1, [r7, #0]
      46:   687a            ldr     r2, [r7, #4]
      48:   683b            ldr     r3, [r7, #0]
      4a:   4413            add     r3, r2
      4c:   60fb            str     r3, [r7, #12]
      4e:   68fb            ldr     r3, [r7, #12]
      50:   4618            mov     r0, r3
      52:   3714            adds    r7, #20
      54:   46bd            mov     sp, r7
      56:   f85d 7b04       ldr.w   r7, [sp], #4
      5a:   4770            bx      lr

    0000005c <func>:
      5c:   b480            push    {r7}
      5e:   b087            sub     sp, #28
      60:   af00            add     r7, sp, #0
      62:   60f8            str     r0, [r7, #12]
      64:   60b9            str     r1, [r7, #8]
      66:   607a            str     r2, [r7, #4]
      68:   603b            str     r3, [r7, #0]
      6a:   68fa            ldr     r2, [r7, #12]
      6c:   68bb            ldr     r3, [r7, #8]
      6e:   441a            add     r2, r3
      70:   687b            ldr     r3, [r7, #4]
      72:   441a            add     r2, r3
      74:   683b            ldr     r3, [r7, #0]
      76:   441a            add     r2, r3
      78:   6a3b            ldr     r3, [r7, #32]
      7a:   4413            add     r3, r2
      7c:   617b            str     r3, [r7, #20]
      7e:   697b            ldr     r3, [r7, #20]
      80:   4618            mov     r0, r3
      82:   371c            adds    r7, #28
      84:   46bd            mov     sp, r7
      86:   f85d 7b04       ldr.w   r7, [sp], #4
      8a:   4770            bx      lr

    Disassembly of section .comment:

    00000000 <.comment>:
       0:   3a434347        bcc     10d0d24 <func+0x10d0cc8>
       4:   35312820        ldrcc   r2, [r1, #-2080]!       ; 0xfffff7e0
       8:   392e343a        stmdbcc lr!, {r1, r3, r4, r5, sl, ip, sp}
       c:   732b332e                        ; <UNDEFINED> instruction: 0x732b332e
      10:   33326e76        teqcc   r2, #1888       ; 0x760
      14:   37373131                        ; <UNDEFINED> instruction: 0x37373131
      18:   2029312d        eorcs   r3, r9, sp, lsr #2
      1c:   2e392e34        mrccs   14, 1, r2, cr9, cr4, {1}
      20:   30322033        eorscc  r2, r2, r3, lsr r0
      24:   35303531        ldrcc   r3, [r0, #-1329]!       ; 0xfffffacf
      28:   28203932        stmdacs r0!, {r1, r4, r5, r8, fp, ip, sp}
      2c:   72657270        rsbvc   r7, r5, #112, 4
      30:   61656c65        cmnvs   r5, r5, ror #24
      34:   00296573        eoreq   r6, r9, r3, ror r5

    Disassembly of section .ARM.attributes:

    00000000 <.ARM.attributes>:
       0:   00003041        andeq   r3, r0, r1, asr #32
       4:   61656100        cmnvs   r5, r0, lsl #2
       8:   01006962        tsteq   r0, r2, ror #18
       c:   00000026        andeq   r0, r0, r6, lsr #32
      10:   726f4305        rsbvc   r4, pc, #335544320      ; 0x14000000
      14:   2d786574        cfldr64cs       mvdx6, [r8, #-464]!     ; 0xfffffe30
      18:   0600344d        streq   r3, [r0], -sp, asr #8
      1c:   094d070d        stmdbeq sp, {r0, r2, r3, r8, r9, sl}^
      20:   14041202        strne   r1, [r4], #-514 ; 0xfffffdfe
      24:   17011501        strne   r1, [r1, -r1, lsl #10]
      28:   1a011803        bne     4603c <func+0x45fe0>
      2c:   22061e01        andcs   r1, r6, #1, 28
      30:   Address 0x0000000000000030 is out of bounds.
      ```
2. 將資料夾 gnu-mcu-eclipse-qemu 完整複製到 ESEmbedded_HW03 資料夾中

3. 在 ESEmbedded_HW02 底下 `$make qemu`，再開啟另一個 Terminal 連線 `$arm-none-eabi-gdb`，輸入 `$target remote 127.0.0.1:1234` 連接，輸入兩次的 `ctrl + x` 和數字 `2`，開啟 Register 以及指令，並且輸入 `si` 單步執行觀察。

4. 程式執行分析
* reset_handler
    ```
    00000008 <reset_handler>:
       8:   b580            push    {r7, lr}
       a:   b088            sub     sp, #32
       c:   af02            add     r7, sp, #8
       e:   2303            movs    r3, #3
      10:   617b            str     r3, [r7, #20]
      12:   2306            movs    r3, #6
      14:   613b            str     r3, [r7, #16]
      16:   230f            movs    r3, #15
      18:   60fb            str     r3, [r7, #12]
      1a:   2304            movs    r3, #4
      1c:   60bb            str     r3, [r7, #8]
      1e:   2317            movs    r3, #23
      20:   607b            str     r3, [r7, #4]
      22:   6978            ldr     r0, [r7, #20]
      24:   6939            ldr     r1, [r7, #16]
      26:   f000 f809       bl      3c <sum>
      2a:   687b            ldr     r3, [r7, #4]
      2c:   9300            str     r3, [sp, #0]
      2e:   6978            ldr     r0, [r7, #20]
      30:   6939            ldr     r1, [r7, #16]
      32:   68fa            ldr     r2, [r7, #12]
      34:   68bb            ldr     r3, [r7, #8]
      36:   f000 f811       bl      5c <func>
      3a:   e7fe            b.n     3a <reset_handler+0x32>
    ```
    1. 先 push {r7, lr}
    2. 根據定義的變數，讓 stack 空出一塊區塊
    3. 將一開始宣告的變數放到 register，再 store 到 memory 裡
    4. 再依序執行 `sum`、`func` 兩個 function 中執行

* sum
    ```
    0000003c <sum>:
      3c:   b480            push    {r7}
      3e:   b085            sub     sp, #20
      40:   af00            add     r7, sp, #0
      42:   6078            str     r0, [r7, #4]
      44:   6039            str     r1, [r7, #0]
      46:   687a            ldr     r2, [r7, #4]
      48:   683b            ldr     r3, [r7, #0]
      4a:   4413            add     r3, r2
      4c:   60fb            str     r3, [r7, #12]
      4e:   68fb            ldr     r3, [r7, #12]
      50:   4618            mov     r0, r3
      52:   3714            adds    r7, #20
      54:   46bd            mov     sp, r7
      56:   f85d 7b04       ldr.w   r7, [sp], #4
      5a:   4770            bx      lr
    ```
    1. 先把 r7 push 到 stack 
    2. `sp - 20` 是為了給 sum 這個 function 一個空間
    3. `sp` 存放到 `r7` 裡
    4. 將 r0、r1 的值 store 到 memory 裡面
    5. 將 memory 的值 load 到 register r2 r3裡面
    6. r2 add r3 後存到 r3
    7. 將 r3 的值 store 到 memory 裡面
    8. 將 memory 的值 load 到 register r3裡面
    9. 從 r3 的值 return 到 r0
    10. `sp - 20` 是為了釋放 sum 這個 function 的空間
    11. branch 回到 reset_handler。

* func
    ```
    0000005c <func>:
      5c:   b480            push    {r7}
      5e:   b087            sub     sp, #28
      60:   af00            add     r7, sp, #0
      62:   60f8            str     r0, [r7, #12]
      64:   60b9            str     r1, [r7, #8]
      66:   607a            str     r2, [r7, #4]
      68:   603b            str     r3, [r7, #0]
      6a:   68fa            ldr     r2, [r7, #12]
      6c:   68bb            ldr     r3, [r7, #8]
      6e:   441a            add     r2, r3
      70:   687b            ldr     r3, [r7, #4]
      72:   441a            add     r2, r3
      74:   683b            ldr     r3, [r7, #0]
      76:   441a            add     r2, r3
      78:   6a3b            ldr     r3, [r7, #32]
      7a:   4413            add     r3, r2
      7c:   617b            str     r3, [r7, #20]
      7e:   697b            ldr     r3, [r7, #20]
      80:   4618            mov     r0, r3
      82:   371c            adds    r7, #28
      84:   46bd            mov     sp, r7
      86:   f85d 7b04       ldr.w   r7, [sp], #4
      8a:   4770            bx      lr
    ```
    1. 先把 r7 push 到 stack 
    2. 將 function 要使用到的 stack 空間空出來 ，將 sp 的值給r7。
    3. `sp` 存放到 `r7` 裡
    4. 將 r0、r1、r2、r3 的值 store 到 memory 裡面
    5. 從 memory 中取出兩個值到 r2、r3 再將相加後的結果放到 r2
    6. 再從 memory 中取出一個值到 r3 再與 r2 相加放入 r2
    7. 最後把結果放到 r3
    8. 將 memory 的值 load 到 register r3裡面，從 r3 的值 return 到 r0
    9. 釋放 func 這個 function 的空間
    10. branch 回到 reset_handler。

## 結果與討論
* 主程式執行會先 push {r7, lr}
* 根據AAPCS，傳遞參數會先使用r0 r1 r2 r3，如果不夠用會使用到stack。
* 執行function時，會先push {r7}，stack 會空出空間給 function，並將 sp 的值給r7
* 使用傳遞的參數時，先將參數從 stack load 出來，放入暫存器
* 運算時最後的結果會被放到 r3 中
* 回傳參數時，會把結果放到 r3，再把 r3 的值給 r0 進行回傳



