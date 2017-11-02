#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>
#include <string.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127

int get_float_bits(float);
char get_flt_sign_char(float);
int get_flt_sign_val(float);
char* get_flt_exp_str(float);
int get_flt_exp_val(float);
int get_flt_exp_mode(float);
char* get_flt_man_str(float fl);
float get_flt_man_val(float fl);
char* get_flt_bits_str(float fl);

/*
    Declare a "typedef struct {} flt;" to contain data for a float
    The data should include:
        An integer for the sign
            (1 for positive, -1 for negative)
        An integer for the exponent value
            (should be bit value to integer minus BIAS or -126 for denormalized)
        A float to contain the value of the mantissa
            (Actual float value extracted from the binary value)
        An integer to contain the mode using the defines above
            (NORM, DNORM and SPEC)
    Example after processing: -15.375 = 1 10000010 11101100000000000000000
        sign = -1
        exp = 3
        man = 0.9218750000
        mode = NORM
*/

typedef struct {
    int sign;
    int exponent;
    float mantissa;
    int mode; 
} Float;


Float get_float_val_float(float fl);
void disp_flt_str(Float fl_s);
float get_flt_bits_val(Float fl_s);

int main()
{
    int i = 0x0000290b;
    float x = -15.375, y = *(float*) (&i), z = pow(2,150);
    float vals[] = {x,y,z};
    
    /*
    printf("Modes are: Normalized %d, Denormalized %d, and Special %d\n", NORM, DNORM, SPEC);
    printf("%lf || E = %s, mode = %d\n", x, get_flt_exp_str(x), get_flt_exp_mode(x));
    printf("%lf || E = %s, mode = %d\n", y, get_flt_exp_str(y), get_flt_exp_mode(y));
    printf("%lf || E = %s, mode = %d\n", z, get_flt_exp_str(z), get_flt_exp_mode(z)); */
    
    /*
    printf("%lf || M = %s\n", x, get_flt_man_str(x));
    printf("%lf || M = %s\n", y, get_flt_man_str(y));
    printf("%lf || M = %s\n", z, get_flt_man_str(z));
    */
    
    // printf("x = %lf, M is %s, M evaluates %lf\n",
    //     x, get_flt_man_str(x), get_flt_man_val(x));
    //printf("x = %lf, bits are %s\n", x, get_flt_bits_str(x));
    
    float fl, flb;
    Float flx;
    int index;
    for(index = 0; index < 3; index++){
        fl = vals[index];
        printf("\nFloat Value: %.20f\n", fl);
        flx = get_float_val_float(x);
        disp_flt_str(flx);
    
        flb = get_flt_bits_val(flx);
        printf("Float came back as %lf\n", flb);
        
        if(fl != flb){
            printf("Float corrupted, mismatch of %lf\n", fl-flb);
        }
    }
    
    return EXIT_SUCCESS;
    
}

/*
    Write a function get_flt_bits_int to return an integer with the
    bits copied from a float.
    Example:
        for f = -15.375,
        the bits of int n = 11000001011101100000000000000000
    Look at the slides and code from the float lectures and use the
    indirection trick.  This can easily be done in one line of code.
    The function should accept a float and return an int.
*/

int get_flt_bits_int(float fl)
{
    return *((int*) &fl);
}


/*
    Write a function that returns the sign of a float as a char.
    You should call get_flt_bits_int to get the bits in an int
    and return '1' if the sign is negative else return '0'.  The
    function should accept a float and return a string.
*/
char get_flt_sign_char(float fl)
{
    int i = get_flt_bits_int(fl);
    if (i < 0) return '1';
    else return '0';
}



/*
    Write a function that returns the sign of a float as an integer.
    You should call get_flt_bits_int to get the bits in an int
    and return -1 if the sign is negative else return 1.  The function
    should accept a float and return an int.
*/
int get_flt_sign_val(float fl)
{
    char sign = get_flt_sign_char(fl);
    if(sign == '1') return -1;
    else return 1;
}



/*
    Write a function to return a string containing the
    actual binary value of the exponent of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are "10000010"
    The function should accept a float and return a string.
*/
char* get_flt_exp_str(float fl)
{
    char *exp_str = malloc(20);
    exp_str[8] = '\0'; // allow printing with %s
    unsigned int bit_check = pow(2,30);//1000...Check from second place on
    unsigned int has_bit;
    int fl_int = get_flt_bits_int(fl);
    int index;
    for(index = 0; index < 8; index++){
        has_bit = bit_check & fl_int;
        bit_check /= 2; //bit shift 1 right
        if(has_bit){
            exp_str[index] = '1';
        } else {
            exp_str[index] = '0';
        }
    }
    return exp_str;
}



/*
    Write a function to return an integer containing the
    actual integer value of the exponent of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the exponent value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the actual value of the exponent is 3
    The function should accept a float and return an int.
*/
int get_flt_exp_val(float fl)
{
    int expint = get_flt_bits_int(fl);
    int j, count=0, sum=0;
    //expint = expint & (int) pow(2,8) - 1;
    
    printf("Get E value has string %x, for float %lf\n", expint, fl);
    
    for (j=23; j <31; j++){
        
        sum = sum + ( (expint>>j)&1) * pow(2,count);
        printf("Reached digit %d, digit is %d, 2^digit = %f, accumulated E = %d\n",
            count, (expint>>j)&1, pow(2,count), sum);
        count++;
    }
    printf("sum = %d\n", sum);
    /* int bit_check = pow(2, 7);
    int index, E = 0;
    for(index = 0; index < 8; index++){
        
    } */
    
    return sum - BIAS;
}



/*
    Write a function to return an integer containing the
    mode of the exponent of a float.  You should call
    get_flt_exp_val to get the bits in an int and return
    the int with the mode value. denormalized
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the mode is NORM
    The function should accept a float and return an int.
*/

int get_flt_exp_mode(float fl)
{
    //get bits
    int expint = BIAS + get_flt_exp_val(fl);
    
    // DB-DL
    printf("e in function is %x\n", expint);
    
    if(expint == 0){
        puts("Reached denormalized case");
        return DNORM;
    }
    if(expint == 0xff){
        puts("Reached special case");
        return SPEC;
    }
    puts("Reached Normal case");
    return NORM;
}



/*
    Write a function to return a string containing the
    actual binary value of the mantissa of a float in a
    char array.  You should call get_flt_bits_int to get
    the bits in an int and return the string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are "11101100000000000000000"
    The function should accept a float and return a string.
*/
char* get_flt_man_str(float fl)
{
    const int len = 23;
    char *exp_str = malloc(len + 1);
    exp_str[len] = '\0'; // allow printing with %s
    unsigned int bit_check = pow(2,(len-1));//1000...Check from second place on
    unsigned int has_bit;
    int fl_int = get_flt_bits_int(fl);
    int index;
    for(index = 0; index < len; index++){
        has_bit = bit_check & fl_int;
        bit_check = bit_check >> 1;
        if(has_bit){
            exp_str[index] = '1';
        } else {
            exp_str[index] = '0';
        }
    }
    return exp_str;  
}



/*
    Write a function to return a float containing the
    actual float value of the mantissa of a float.  You
    should call get_flt_bits_int to get the bits in an int
    and return the int with the mantissa value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the mantissa bits are 11101100000000000000000
            the actual value of the mantissa is 0.9218750000
    The function should accept a float and return an int.
*/
float get_flt_man_val(float fl)
{
    const len = 23;
    float place_val = 1, sum = 0;
    int expint = get_flt_bits_int(fl), index;
    
    for(index = len - 1; index >= 0; index--){
        place_val /= 2; //simulates bit-shift right, past decimal point
        sum += (((expint >> index) &1 ) * place_val); //much better than pow
        //TODO: take out all pow and use rolling place-value variable
        //printf("Working on digit %d, running total %lf\n", index, sum);
    }
    
    return sum;
}



/*
    Write a function to return a string containing the
    actual binary value of a float in a char array.  You
    should call get_flt_sign_char, get_flt_exp_str and
    get_flt_man_str to get the bits in an char and two
    strings and return the concatenated string.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            The sign is '1'
            the exponent is "10000010"
            and the mantissa bits are "11101100000000000000000"
            The string should be formatted as:
                "1 10000010 11101100000000000000000" to clearly
                separate the 3 parts.
    The function should accept a float and return a string.
*/
char* get_flt_bits_str(float fl)
{
    char* bit_str = malloc(35);
    bit_str[34] = '\0';
    printf("Buffer Contents: %s\n", bit_str);
    
    bit_str[0] = get_flt_sign_char(fl);
    bit_str[1] = ' ';
    
    char *buf = get_flt_exp_str(fl);
    strcpy(bit_str + 2, buf);
    bit_str[10] = ' ';
    
    buf = get_flt_man_str(fl);
    strcpy(bit_str + 11, buf);
    
    return bit_str;
}



/*
    Write a function to separate the parts of a float
    into a flt struct as described above.  You should
    call get_flt_sign_val, get_flt_exp_mode,
    get_flt_exp_val and get_flt_man_val.
    Hint:  make sure to set exponent to -126 for
    DNORM mode.
*/
Float get_float_val_float(float fl)
{
    Float fl_s;
    
    fl_s.sign = get_flt_sign_val(fl);
    
    fl_s.mantissa = get_flt_man_val(fl);
    fl_s.mode = get_flt_exp_mode(fl);
    
    if(fl_s.mode == DNORM){
        fl_s.exponent = -126;
    } else {
        fl_s.exponent = BIAS + get_flt_exp_val(fl);
    }
    
    return fl_s;
}


/*
    Write a function to print a flt struct to screen.
    It should accept a flt struct and return nothing.
    Hint: Use if statement to print mode.
*/

void disp_flt_str(Float fl_s)
{
    char mode_name[13];
    switch(fl_s.mode){
        case NORM:
            strcpy(mode_name, "Normalized");
            break;
        case DNORM:
            strcpy(mode_name, "Denormalized");
            break;
        case SPEC:
            strcpy(mode_name, "Special");
            break;
        default:
            strcpy(mode_name, "Error");
    }
    
    printf("Float Sign: %d\nFloat E: %d\nFloat Mantissa: %lf\nFloat Mode: %s\n",
        fl_s.sign, fl_s.exponent, fl_s.mantissa, mode_name);
}




/*
    Write a function to get the actual float value back
    out of a flt struct.
    Hints:
        The float value produced will depend on the mode.
        To set a float to infinity use the math library constant INFINITY
        To set a float to not-a-number use the math library constant NAN
        Check the slides and text for conditions for NORN, DNORM and SPEC
        You need to return (sign) * M * 2^e
*/
float get_flt_bits_val(Float fl_s)
{
    printf("Struct mode in display func is %d\n", fl_s.mode);
    switch(fl_s.mode){
        case SPEC:
            if(fl_s.mantissa == 0) return INFINITY * fl_s.sign;
            return NAN;
        case NORM:
            printf("Reached code for calculating normalized\n");
            return fl_s.sign * fl_s.mantissa * pow(2, fl_s.exponent - BIAS);
        case DNORM:
            return fl_s.sign * fl_s.mantissa * pow(2, 1 - BIAS);
        default:
            printf("Error unpacking Float structure.\n");
            return NAN;
    }
}



/*
    Write a main function that calls an prints results for
    each function when completed.
    get_flt_sign_char
    get_flt_sign_val

    get_flt_exp_str
    get_flt_exp_val

    get_flt_man_str
    get_flt_man_val

    get_flt_bits_str

    get_flt_val_flt
    print_flt

    get_flt_bits_val

int main(){




    return 0;
}
*/