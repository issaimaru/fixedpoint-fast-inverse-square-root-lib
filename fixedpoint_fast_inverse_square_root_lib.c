/**
 * @file fixedpoint_fast_inverse_square_root_lib.c
 * @author issaimaru (takeuchi.i.5b78@m.isct.ac.jp)
 * @brief 高速逆平方根を固定小数点で実装する手法を提案するライブラリ
 * @version 0.1
 * @date 2026-01-28
 * 
 * @copyright Copyright (c) 2026
 * 
 */

//固定小数点はQ8.24形式で実装している

#include <stdio.h>
#include "fixedpoint_fast_inverse_square_root_lib.h"

//乗算1回と条件分岐1回で32bitのMSBを求める関数
//参考：https://qiita.com/naoppy/items/338695351772a4b46bc1
static const unsigned char magic_index[32] = {
    0,  1, 28,  2, 29, 19, 24,  3, 30, 22, 20, 10, 25, 12, 15,  4, 
	31, 27, 18, 23, 21,  9, 11, 14, 26, 17,  8, 13, 16,  7,  6,  5 
}; 

/**
 * @brief 32ビット整数のMSBを求める関数(乗算1回と条件分岐1回)
 * 
 * @param value 32ビット整数
 * @return int MSBの位置
 */
int GetMSB32(int value) {
    if(value == 0) return -1; // MSBが存在しない場合の処理

    value |= (value >> 1);
    value |= (value >> 2);
    value |= (value >> 4);
    value |= (value >> 8);
    value |= (value >> 16);

    value >>= 1;
    value ++;

    value = (0x07D6E531 * value) >> 27;
    return (int)magic_index[value & 0x1F];
}

/**
 * @brief 固定小数点からカスタム浮動小数点に変換する関数(高速逆平方根用)
 * 
 * @param input 固定小数点数（Q8.24形式）
 * @return int カスタム浮動小数点数
 */
int fix_to_custom_float(int input) {
    int msb = GetMSB32(input);
    int E = msb + 3;//カスタム浮動小数点の指数部
    int S;
    if(msb >= 26) {
        S = (input >> (msb - 26))&0x03FFFFFF;
    }else{
        S = (input << (26 - msb))&0x03FFFFFF;
    }
    int custom_float = (E << 26) | S;
    return custom_float;
}

/**
 * @brief カスタム浮動小数点から固定小数点に変換する関数(高速逆平方根用)
 * 
 * @param custom_float カスタム浮動小数点数
 * @return int 固定小数点数（Q8.24形式）
 */
int custom_float_to_fix(int custom_float) {
    int E = (custom_float >> 26) & 0x3F;//指数部
    int S = custom_float & 0x03FFFFFF;//仮数部
    int fix;
    if(E <= 29){
        fix = (1 << 26 | S) >> (29 - E);
    }else{
        fix = (1 << 26 | S) << (E - 29);
    }
    return fix;
}

/**
 * @brief 32bit固定小数点の乗算を行う関数
 * 
 * @param a_fixed 乗数1
 * @param b_fixed 乗数2
 * @return int 乗算結果
 */
int mul_fixed(int a_fixed, int b_fixed){
    long long temp = (long long)a_fixed * (long long)b_fixed;
    //Q8.24形式なので，結果を24ビット右シフトして調整する
    temp = temp >> SHIFT;
    return (int)temp;
} 

/**
 * @brief 高速逆平方根を固定小数点で計算する関数(Q8.24形式)
 * 
 * @param input 入力値(固定小数点)
 * @return int 結果(固定小数点)
 */
int fixedpoint_fast_inverse_square_root(int input) {
    //入力が0の場合はエラー値を返す
    if(input == 0) return -1;

    //入力が1未満の場合，誤差を抑えるためにシフトする
    int msb = GetMSB32(input);
    int shift = 0;
    if(msb <= 24) {
        shift = 24 - msb;
        if(shift % 2 == 1){
            shift++;
        }
        input = input << shift;
    }
    
    //高速逆平方根を適用するためにカスタム浮動小数点に変換
    int L = fix_to_custom_float(input);

    //初期近似値
    int a;
    a = 0xA1BDE5B0 - (L >> 1);

    //固定小数点での計算に戻す
    int approximation = custom_float_to_fix(a);

    //ニュートン法による補正
    int norm_half_input = input >> 1;
    int three_halfs = FLOAT2FIX(1.5f);

    for(int i = 0; i < 2; i++){
        approximation = mul_fixed(approximation, (three_halfs - mul_fixed(approximation, mul_fixed(norm_half_input, approximation))));
    }

    //シフトした分を戻す
    approximation = approximation << (shift / 2);

    return approximation;
}
