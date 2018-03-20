#ifndef CALC_PI_H
#define CALC_PI_H

class CalculatePi  
{
public:
	static int get_nth_digit(int n);
	static int res ;
	static int cur_n ;


private:
	static int inv_mod(int x, int y);
	static int pow_mod(int a, int b, int m);
	static int is_prime(int n);
	static int next_prime(int n);
	static int mul_mod(int a,int b,int m);
	static int get_first_digit_of_double(double num);

};


#endif