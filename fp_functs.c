/* Name: Laraib Pervez
 * GNumber: 01073542
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "fp.h"

#define SIGN_BIT 11

// Pow function 
float mypowerfunction(float A, float B) {
    float answer = A;
    fp_gmu i  = 1;
    while (i < B) {
        answer = answer * A;
        i++;
    }
    return answer;
}
/* input: float value to be represented
 * output: fp_gmu representation of the input float
 * Follow the Project Documentation for Instructions
 */
fp_gmu compute_fp(float val) {

fp_gmu v = 0;

// if zero then return 0
if (val == 0)
{   
    return v;
}
fp_gmu e = 0;
fp_gmu retVal = 0;
// if negative then justify
 if (val < 0) {
    while (val < 0)
    {
      retVal |= (1 << SIGN_BIT);
      val *= -1;
    }
    e *= 1;
}
// calculating the exponent
if (val >= 2)
{   
    while (val >= 2)
    { 
      val = val / 2;
      ++e;
    }
}
val -= 1;
fp_gmu biasedExponent = e + 15;
// Get the mentissa
fp_gmu mantissa = 0;
float Bits = val * (mypowerfunction(2, 6));
mantissa = (fp_gmu)Bits;
//Biased exponents largest number
if (biasedExponent > 30)
{
    return -1;
}

if (biasedExponent < 1)
{
    return 0;
}
// packing the number
retVal |= (biasedExponent<<6);
retVal |= mantissa;
return retVal;
}

/* input: fp_gmu representation of our floats
*  output: float value represented by our fp_gmu encoding
*  If your input fp_gmu is Infinity, return the constant INFINITY
*  If your input fp_gmu is -Infinity, return the constant -INFINITY
*  If your input fp_gmu is NaN, return the constant NAN
*  Follow the Project Documentation for Instructions
*/
float get_fp(fp_gmu val) {

float fraction = ((val & 0x3F) / (float)mypowerfunction(2, 6)) + 1;
fp_gmu exponent = ((val >> 6) & 0x1F);
// demormalize or 0
if (exponent == 0)
{
    return 0;
}
// if special number
if (exponent == 0x1F)
{
    return -1;
}
// get the e value
fp_gmu e = exponent - 15;

// checking if negative
if (e < 0)
{
    fp_gmu i;
    for (i = e; i < 0; ++i)
    {
 fraction = fraction / 2;
    }
}// if e > 0
else
{
    fp_gmu i;
    for (i = 0; i < e; ++i)
    {
      fraction = fraction * 2;
    }

}
if (val & (1 << SIGN_BIT)) fraction *= -1;
return fraction;
}
/* input: Two fp_gmu representations of our floats
 * output: The multiplication result in our fp_gmu representation
 * You must implement this using the algorithm described in class:
 * Xor the signs: S = S1 ^ S2
 * Add the exponents: E = E1 + E2
 * Multiply the Frac Values: M = M1 * M2
 * If M is not in a valid range for normalized, adjust M and E.
 * Follow the Project Documentation for Instructions
 */
fp_gmu mult_vals(fp_gmu source1, fp_gmu source2) {

fp_gmu exponent1 = ((source1 >> 6) & 0x1F);
fp_gmu exponent2 = ((source2 >> 6) & 0x1F);

fp_gmu e1 = exponent1 - 15;
fp_gmu e2 = exponent2 - 15;
fp_gmu e3 = e1 + e2;


float frac1 = ((float) (source1 & 0x3f));
float frac2 = ((float) (source2 & 0x3f));

frac1 = (frac1 / ((float)mypowerfunction(2, 6))) + 1;
frac2 = (frac2 / ((float)mypowerfunction(2, 6))) + 1;
// overflow
if ((e3 + 15) > 30)
{
    return -1;
}

if ((e3 + 15) < 1)
{
    return 0;
}
// Multiplication of the mantissa
float frac = frac1 * frac2;
if (frac >= 2)
{
    frac /= 2;
    ++e3;
}

fp_gmu new_exp = 15 + e3;

frac -= 1;
frac *= (float)mypowerfunction(2, 6);

fp_gmu newFrac = (fp_gmu) frac;

fp_gmu retVal = 0;
retVal |= new_exp;
retVal <<= 6;
retVal |= newFrac;
return retVal;;
}
/* input: Two fp_gmu representations of our floats
 * output: The addition result in our fp_gmu representation
 * You must implement this using the algorithm described in class:
 * If needed, adjust the numbers to get the same exponent E
 * Add the two adjusted Mantissas: M = M1 + M2
 * Adjust M and E so that it is in the correct range for Normalized
 * Follow the Project Documentation for Instructions
 */

fp_gmu add_vals(fp_gmu source1, fp_gmu source2) {

fp_gmu exponent1 = ((source1 >> 6) & 0x1F);
fp_gmu exponent2 = ((source2 >> 6) & 0x1F);

fp_gmu e1 = exponent1 - 15;
fp_gmu e2 = exponent2 - 15;

float frac1 = ((float) (source1 & 0x3f));
float frac2 = ((float) (source2 & 0x3f));

frac1 = (frac1 / ((float)mypowerfunction(2, 6))) + 1;
frac2 = (frac2 / ((float)mypowerfunction(2, 6))) + 1;

if (e1 > e2)
{
    fp_gmu i;
    fp_gmu delta = e1 - e2;
    for (i = 0; i < delta; ++i)
    {
      frac2 = frac2 / 2;
      ++e2;
    }
}
else if (e1 < e2)
{
    fp_gmu i;
    fp_gmu delta = e2 - e1;
    for ( i = 0; i < delta; ++i)
    {
      frac1 = frac1 / 2;
      ++e1;
    }
}
if (source1 & (1 << SIGN_BIT)) {
    frac1 *= -1;
} else if (source2 & (1 << SIGN_BIT)) {
    frac2 *= -1;
}
// adding the mantissas   
float frac = frac2 + frac1;
fp_gmu sign = 0;
if (frac < 0) {
  sign = 1 << SIGN_BIT;
  frac *= -1;
}
while (frac >= 2)
{
    frac = frac / 2;
    ++e1;
}
// decrement if frac < 1
while (frac < 1)
{
    frac = frac * 2;
    --e1;
}
// overflow check
if ((e1 + 15) > 30)
{
    return -1;
}

fp_gmu new_exp = (15 + e1) * (mypowerfunction(2, 6));
frac -= 1;
frac *= (float)mypowerfunction(2, 6);

fp_gmu newFrac = (fp_gmu) frac;
return new_exp + newFrac + sign;
}
  