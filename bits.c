/* WARNING: Do not include any other libraries here,
 * otherwise you will get an error while running test.py
 * You can still use printf for debugging without including
 * <stdio.h>, although you might get a compiler warning. In general,
 * it's not good practice to ignore compiler warnings, but in this
 * case it's OK.
 *
 * Using printf will interfere with our script capturing the execution results.
 * At this point, you can only test correctness with ./btest.
 * After confirming everything is correct in ./btest, remove the printf
 * and run the complete tests with test.py.
 */

/*
 * bitAnd - x & y using only ~ and |
 * Example: bitAnd(4, 5) = 4
 * Legal ops: ~ |
 * Max ops: 7
 * Difficulty: 1
 */
int bitAnd(int x, int y) {
    return ~(~x | ~y);
}

/*
 * bitXor - x ^ y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 7
 *   Difficulty: 1
 */
int bitXor(int x, int y) {
    return ~(~x & ~y) & ~(x & y);
}

/*
 * samesign - Determines if two integers have the same sign.
 *   0 is not positive, nor negative
 *   Example: samesign(0, 1) = 0, samesign(0, 0) = 1
 *            samesign(-4, -5) = 1, samesign(-4, 5) = 0
 *   Legal ops: >> << ! ^ && if else &
 *   Max ops: 12
 *   Difficulty: 2
 *
 * Parameters:
 *   x - The first integer.
 *   y - The second integer.
 *
 * Returns:
 *   1 if x and y have the same sign , 0 otherwise.
 */
int samesign(int x, int y) {
    // 对0的情况进行判断
    int x_is_zero=!(x & 0xFFFFFFFF);
    int y_is_zero=!(y & 0xFFFFFFFF);
    if (x_is_zero) {
        if (y_is_zero)
            return 1;

        return 0;
    }
    if (y_is_zero) {
        if (x_is_zero)
            return 1;

        return 0;
    }

    return !((x >> 31) ^ (y >> 31));
    return 0;
}

/*
 * logtwo - Calculate the base-2 logarithm of a positive integer using bit
 *   shifting. (Think about bitCount)
 *   Note: You may assume that v > 0
 *   Example: logtwo(32) = 5
 *   Legal ops: > < >> << |
 *   Max ops: 25
 *   Difficulty: 4
 */
int logtwo(int v) {
    // 注意：不保证都是2的幂次，超过部分取整
    // 问题转化求最高有效位位置
    // bitCount:考虑并行
    int step1=(v>>16)>0; // 可以得到0或者1
    int move1=step1<<4;
    int step2=((v>>move1)>>8)>0;
    int move2=(step2<<3) | move1;// 注意要加上前面的
    int step3=((v>>move2)>>4)>0;
    int move3=(step3<<2) | move2; 
    int step4=((v>>move3)>>2)>0;
    int move4=(step4<<1) | move3;
    int step5=((v>>move4)>>1)>0;
    int move5=step5 | move4;
    return move5;
}

/*
 *  byteSwap - swaps the nth byte and the mth byte
 *    Examples: byteSwap(0x12345678, 1, 3) = 0x56341278
 *              byteSwap(0xDEADBEEF, 0, 2) = 0xDEEFBEAD
 *    Note: You may assume that 0 <= n <= 3, 0 <= m <= 3
 *    Legal ops: ! ~ & ^ | + << >>
 *    Max ops: 17
 *    Difficulty: 2
 */
int byteSwap(int x, int n, int m) {
    int move_n=n<<3;
    int move_m=m<<3;
    int mask1 = 0xFF << move_n;  // 取第n位,注意要*8，不要写成0x11
    int mask2 = 0xFF << move_m;
    // unsigned int byte1=x & mask1;
    // unsigned int byte2=x & mask2;
    // // unsigned int mask3=0x11111111 ^ mask1 ^ mask2;
    int mask3 = ~(mask1 | mask2);
    int x1 = x & mask3;
    // unsigned int mask4=byte1 << ((m-n)<<3);
    // unsigned int mask5=byte2 >> ((m-n)<<3);
    // return x1 | mask4 | mask5;
    // 问题1：n==m时，直接输出0
    // 问题2：没有处理n>m的情况

    // 思考：不一定要交换，就是先把它们移到低位byte，再移动到合适的位置
    int byte1 = (x >> move_n) & 0xFF;
    int byte2 = (x >> move_m) & 0xFF;
    int mask4 = byte1 << move_m;
    int mask5 = byte2 << move_n;
    return x1 | mask4 | mask5;

    // 更清晰的思路，可以并行操作（但其实就是上面的简化版mask12 mask45合并）
}

/*
 * reverse - Reverse the bit order of a 32-bit unsigned integer.
 *   Example: reverse(0xFFFF0000) = 0x0000FFFF reverse(0x80000000)=0x1 reverse(0xA0000000)=0x5
 *   Note: You may assume that an unsigned integer is 32 bits long.
 *   Legal ops: << | & - + >> for while ! ~ (You can define unsigned in this function)
 *   Max ops: 30
 *   Difficulty: 3
 */
unsigned reverse(unsigned v) {
    // 方法一
    // int p=16;
    // while(p)
    // {
    //     int mask1=1<<(15+p);
    //     int mask2=1<<(16-p);
    //     unsigned bit1=v & mask1;
    //     unsigned bit2=v & mask2;
    //     v=v & ~mask1 & ~mask2;
    //     bit1=bit1>>((p<<1)-1);// 这边涉及到右移，需要定义unsigned，否则可能会是算术右移
    //     bit2=bit2<<((p<<1)-1);
    //     v=v | bit1 | bit2;
    //     p=p-1;
    // }
    // return v;
    // 方法二：分组交换逐层翻转
    v = (v >> 16) | (v << 16);

    v = ((v >> 8) & 0x00FF00FF) |
        ((v & 0x00FF00FF) << 8);

    v = ((v >> 4) & 0x0F0F0F0F) |
        ((v & 0x0F0F0F0F) << 4);

    v = ((v >> 2) & 0x33333333) |
        ((v & 0x33333333) << 2);

    v = ((v >> 1) & 0x55555555) |
        ((v & 0x55555555) << 1);

    return v;
}

/*
 * logicalShift - shift x to the right by n, using a logical shift
 *   Examples: logicalShift(0x87654321,4) = 0x08765432
 *   Note: You can assume that 0 <= n <= 31
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 20
 *   Difficulty: 3
 */
int logicalShift(int x, int n) {
    // 和算术右移区别
    // unsigned ux = x;这样不符合要求
    x = x >> n;
    // 我需要知道x的位数？
    // 思考，最多有n位可能是错误的，然后总共32位是已知的
    // int mask=~0+(1<<((32-n)));
    // int mask=~((0x80000000>>n)<<1);
    // 0x80000000解释成unsigned
    int mask = ~(((1 << 31) >> n) << 1);
    // 但是处理不了n等于0的情况
    return x & mask;
}

/*
 * leftBitCount - returns count of number of consective 1's in left-hand (most) end of word.
 *   Examples: leftBitCount(-1) = 32, leftBitCount(0xFFF0F0F0) = 12,
 *             leftBitCount(0xFE00FF0F) = 7
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 50
 *   Difficulty: 4
 */
int leftBitCount(int x) {
    // 每个数都可以表示成二进制，所以还是从16位往下判断（加上一个特判）
    // 如何判断最高16位都是1呢？（反向判断都是0）
    int ans=0;
    int bit_16=!(~(x & 0xFFFF0000)>>16)<<4;
    ans=ans+bit_16;
    x=x<<bit_16;
    int bit_8=!(~(x & 0xFF000000)>>24)<<3;
    ans=ans+bit_8;
    x=x<<bit_8;
    int bit_4=!(~(x & 0xF0000000)>>28)<<2;
    ans=ans+bit_4;
    x=x<<bit_4;
    int bit_2=!(~(x & 0xC0000000)>>30)<<1;
    ans=ans+bit_2;
    x=x<<bit_2;
    int bit_1=!(~(x & 0x80000000)>>31);
    ans=ans+bit_1;
    x=x<<bit_1;
    int is_neg_1=x>>31 &1;
    ans=ans+is_neg_1;
    return ans;
}

/*
 * float_i2f - Return bit-level equivalent of expression (float) x
 *   Result is returned as unsigned int, but it is to be interpreted as
 *   the bit-level representation of a single-precision floating point values.
 *   Legal ops: if else while for & | ~ + - >> << < > ! ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned float_i2f(int x) {
    // 先模拟整数转化为单精度浮点数的过程
    // 要处理特殊情况0
    // if (x == 0)
    //     return 0;
    // // 确认sign
    // int sign = 0;
    // unsigned ux = x;  // 要进入unsigned状态
    // if (x < 0) {
    //     sign = 1;
    //     // x=-x;
    //     // 考虑TMin边界条件，在int语境下会溢出，需要在unsigned语境下
    //     ux = ~ux + 1;
    // }
    // // 从最高位往下扫描，确认exp
    // int p = 31;
    // while (!(ux >> p)) {
    //     p = p - 1;
    // }
    // // 确认frac
    // int frac;
    // if (p < 24) {
    //     int mask1 = 0;
    //     int temp = p - 1;
    //     while (temp > -1) {
    //         mask1 = mask1 + (1 << temp);
    //         temp = temp - 1;
    //     }
    //     frac = (ux & mask1) << (23 - p);

    // } else {
    //     int move = p - 23;
    //     int mask2 = 1 << (move - 1);
    //     int bit_24 = ux & mask2;  // 24位数
    //     frac = (ux & (0x7FFFFF << move)) >> move;
    //     if (bit_24) {
    //         // 后面没有1，向偶数舍入
    //         if ((ux & -ux) == (1 << (move - 1))) {
    //             if (frac & 1) {
    //                 frac = frac + 1;
    //                 if (frac & 0x800000) {
    //                     frac = 0;
    //                     p = p + 1;
    //                 }
    //             }
    //         }
    //         // 后面有1，直接进位
    //         else {
    //             frac = frac + 1;
    //             if (frac & 0x800000) {
    //                 frac = 0;
    //                 p = p + 1;
    //             }
    //         }
    //     }
    // }
    // return (sign << 31) + ((p + 127) << 23) + frac;

    // 第二版
    if (x == 0)  // 1
        return 0;
    // 确认sign
    int sign = 0;
    unsigned ux = x;  // 要进入unsigned状态
    if (x < 0) {      // 2
        sign = 1;
        // x=-x;
        // 考虑TMin边界条件，在int语境下会溢出，需要在unsigned语境下
        ux = ~ux + 1;  // 3,4
    }
    // 从最高位往下扫描，确认exp
    int p = 31;
    while (!(ux >> p)) {  // 5,6
        p = p - 1;        // 7
    }
    // 确认frac
    int frac;
    if (p < 24) {  // 8
        // int mask1 = 0;
        // int temp = p - 1;
        // while (temp > -1) {
        //     mask1 = mask1 + (1 << temp);
        //     temp = temp - 1;
        // }
        // frac = (ux & mask1) << (23 - p);
        frac = (ux << (23 - p)) & 0x7FFFFF;  // 优化1，9,10,11,12

    } else {
        int move = p - 23;                         // 13
        int mask2 = 1 << (move - 1);               // 14,15
        int bit_24 = ux & mask2;                   // 24位数,16
        frac = (ux & (0x7FFFFF << move)) >> move;  // 17,18,19
        // 或者frac=(ux>>move) &0x7FFFFF
        if (bit_24) {
            // 后面没有1，向偶数舍入
            if ((ux & -ux) == mask2) {  // 20,21,22
                if (frac & 1) {         // 23
                    frac = frac + 1;    // 24
                    // 不需要，直接进位
                    // if (frac == 0x800000) {
                    //     frac = 0;
                    //     p = p + 1;
                    // }
                }
            }
            // 后面有1，直接进位
            else {
                frac = frac + 1;  // 26
                // if (frac == 0x800000) {
                //     frac = 0;
                //     p = p + 1;
                // }
            }
        }
    }
    return (sign << 31) + ((p + 127) << 23) + frac;  // 27,28,29,30
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: & >> << | if > < >= <= ! ~ else + ==
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatScale2(unsigned uf) {
    // 这里的uf直接按照IEEE 754解读
    unsigned sign = uf & 0x80000000;
    unsigned exp = (uf >> 23) & 0xFF;
    unsigned frac = uf & 0x7FFFFF;
    if (exp >= 1) {
        if (exp <= 254) {
            exp = exp + 1;
            // 注意这种情况:变成INF
            if (exp == 255) {
                frac = 0;
            }
        }
    }
    if (exp == 0) {
        frac = frac << 1;
        if ((frac >> 23) & 1) {
            exp = 1;
            frac = frac & 0x7FFFFF;
        }
    }
    return sign + (exp << 23) + frac;
}
/*
 * float64_f2i - Convert a 64-bit IEEE 754 floating-point number to a 32-bit signed integer.
 *   The conversion rounds towards zero.
 *   Note: Assumes IEEE 754 representation and standard two's complement integer format.
 *   Parameters:
 *     uf1 - The lower 32 bits of the 64-bit floating-point number.
 *     uf2 - The higher 32 bits of the 64-bit floating-point number.
 *   Returns:
 *     The converted integer value, or 0x80000000 on overflow, or 0 on underflow.
 *   Legal ops: >> << | & ~ ! + - > < >= <= if else
 *   Max ops: 60
 *   Difficulty: 3
 */
int float64_f2i(unsigned uf1, unsigned uf2) {
    int sign = 0x80000000 & uf2;
    int exp = (uf2 & 0x7FF00000) >> 20;
    int E = exp - (1 << 10) + 1;
    unsigned frac1=uf2 & 0x000FFFFF;
    unsigned frac2=uf1 & 0x7FF00000; 
    unsigned frac=(1<<31)| (frac1<<11) | (frac2>>21);
    if (E < 0)
        return 0;

    if (E >= 31)
        return 0x80000000;
    
    int res=frac >> (31-E);

    if(sign)
    res=~res+1;

    return res; 
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: < > <= >= << >> + - & | ~ ! if else &&
 *   Max ops: 30
 *   Difficulty: 4
 */
unsigned floatPower2(int x) {
    if (x < -149)
        return 0;
    if (x >= 128)
        return 0x7F800000;
    if (x >= -126 && x <= 127)
        return (x + 127) << 23;
    else {
        return 0x800000 >> (-126 - x);
    }
    return 2;
}
