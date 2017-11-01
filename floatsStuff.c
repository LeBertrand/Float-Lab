#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include <float.h>

#define NORM 0
#define DNORM 1
#define SPEC 2
#define BIAS 127

int get_float_bits(float);
char get_flt_sign_char(float);
int get_flt_sign_val(float);
char* get_flt_exp_str(float);
int get_flt_exp_val(float);

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

typedef struct flt{
    int sign;
    int exponent;
    float mantissa;
    int mode; 
} Float;

int main()
{
    int x = -15.375;
    printf("E = %d\n", get_flt_exp_val(x));
    get_flt_exp_val(x);
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
    int expint = get_flt_bits_int(fl); // sign propogation shouldn't matter
    int j, count=0, sum=0;
    //expint = expint & (int) pow(2,8) - 1;
    
    for (j=23; j <31; j++){
        printf("%d %.2x\n", (expint>>j)&1, expint);
        sum = sum + ((expint>>j)&1)*pow(2,count);
        count++;
    }
    printf("sum = %d\n", sum);
    /* int bit_check = pow(2, 7);
    int index, E = 0;
    for(index = 0; index < 8; index++){
        
    } */
    
    return expint;
}



/*
    Write a function to return an integer containing the
    mode of the exponent of a float.  You should call
    get_flt_exp_val to get the bits in an int and return
    the int with the mode value.
    Example:
        for f = -15.375
            n = 11000001011101100000000000000000
            the exponent bits are 10000010
            the mode is NORM
    The function should accept a float and return an int.
*/




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




/*
    Write a function to separate the parts of a float
    into a flt struct as described above.  You should
    call get_flt_sign_val, get_flt_exp_mode,
    get_flt_exp_val and get_flt_man_val.
    Hint:  make sure to set exponent to -126 for
    DNORM mode.
*/




/*
    Write a function to print a flt struct to screen.
    It should accept a flt struct and return nothing.
    Hint: Use if statement to print mode.
*/




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