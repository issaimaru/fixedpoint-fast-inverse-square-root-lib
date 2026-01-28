# fixedpoint-fast-inverse-square-root-lib
### 概要
高速逆平方根を固定小数点で実装するVLSIアルゴリズム
VLSI Algorithm for Implementing Fast Inverse Square Root in Fixed-Point Format

### 詳細
逆平方根を計算する際，演算速度向上のためのアルゴリズムとして有名なものに高速逆平方根があります．高速逆平方根は少量の乗算と加算，ビットシフトのみで計算が可能なため，VLSIにおいても有用なアルゴリズムです．<br>
高速逆平方根は浮動小数点専用のアルゴリズムで，固定小数点での実装をしている事例は（調べた限りでは）ありませんでした．<br>
**固定小数点→カスタム浮動小数点→高速逆平方根(カスタム浮動小数点上)→固定小数点**<br>
という手順を踏むことで固定小数点で高速に逆平方根を計算するアルゴリズムを考えてみました．<br>
CORDIC法やテーブル参照でのニュートン法よりは早いと思うけど，最良なのかはわかんないです．<br>
When calculating inverse square roots, the fast inverse square root algorithm is well-known for improving computational speed. Since it requires only a small number of multiplications, additions, and bit shifts, it is a useful algorithm even in VLSI.
The fast inverse square root is a floating-point-specific algorithm; I found no examples (to my knowledge) of its implementation in fixed-point.
**Fixed-point → Custom floating-point → Fast inverse square root (on custom floating-point) → Fixed-point**<br>
Following this sequence, I devised an algorithm to compute inverse square roots efficiently on fixed-point systems.<br>
I believe it's faster than the CORDIC method or Newton's method using table lookups, but I'm not sure if it's the optimal solution.<br>

### テスト例
ニュートン法を2回適用したときの精度
```c
Fast Inverse Square Root Test
input           fast_inv_sqrt   Math            Diff
------------------------------------------------------------
0.0005000000    44.7229652405   44.7213592529   0.0016059875
0.0010000000    31.6229286194   31.6227741241   0.0001544952
0.0049999999    14.1421394348   14.1421365738   0.0000028610
0.0099999998    9.9999446869    10.0000000000   0.0000553131
0.0500000007    4.4721369743    4.4721360207    0.0000009537
0.1000000015    3.1622631550    3.1622776985    0.0000145435
0.5000000000    1.4142132998    1.4142135382    0.0000002384
1.0000000000    0.9999963641    1.0000000000    0.0000036359
2.0000000000    0.7071066499    0.7071067691    0.0000001192
5.0000000000    0.4472135305    0.4472135901    0.0000000596
10.0000000000   0.3162258863    0.3162277639    0.0000018775
20.0000000000   0.2236067653    0.2236067951    0.0000000298
```
