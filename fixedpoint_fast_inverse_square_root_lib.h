#define SHIFT 24

#define ONE (1 << SHIFT)

#define FLOAT2FIX(x) ((int)(((x) * ONE)))
#define FIX2FLOAT(x) ((float)(x) / ONE)

static const unsigned char magic_index[32];
int GetMSB32(int value);

int fix_to_custom_float(int input);
int custom_float_to_fix(int custom_float);
int mul_fixed(int a_fixed, int b_fixed);

int fixedpoint_fast_inverse_square_root(int input);
