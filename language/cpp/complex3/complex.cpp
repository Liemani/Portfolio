#include <sstream>
#include <iostream>
#include <cassert>
#include "complex.h"

int Complex::numberOfComplex;
const int Complex::BLOCK_SIZE = 256;
Complex *Complex::headOfFreeList = NULL;

std::istream& operator>>(std::istream& in, Complex& rhs)
{
	double re = 0.0;
	double im = 0.0;
	char c = 0;
	
	in >> c;
	if (c == '(') {
		in >> re >> c;
		if (c == ',') in >> im >> c;
		if (c == 'i') in >> c;
		if (c != ')') in.clear(std::ios::failbit);
	} else {
		in.putback(c);
		in >> re;
	}

	if (in) rhs = Complex(re, im);
	
	return in;
}
/*
std::ostream& operator<<(std::ostream& out, const Complex& rhs)
{
	out << "(" << rhs.re_ << ", " << rhs.im_ << "i)";
	
	return out;
}
*/
std::ostream& operator<<(std::ostream& out, const Complex& rhs)
{
	std::ostringstream buf;
	buf.flags(out.flags());
	buf.precision(out.precision());
	
	buf << "("<< rhs.re_ << "," << rhs.im_ << "i)" << std::ends;
	out << buf.str();
	
	return out;
}

// static member func.
void* Complex::operator new(size_t size)
{
	if (size != sizeof(Complex))
		return ::operator new(size);
	
	Complex* ptr = headOfFreeList;
	
	if (ptr) {
		headOfFreeList = ptr->next;
	} else {
		Complex* pArr = static_cast<Complex*>(::operator new(BLOCK_SIZE * sizeof(Complex)));
		assert(pArr);
		
		for (int i = 1; i < BLOCK_SIZE - 1; ++i)
			pArr[i].next = &pArr[i + 1];
		
		pArr[BLOCK_SIZE - 1].next = NULL;
		
		headOfFreeList = &pArr[1];
		
		ptr = pArr;
	}
	
	return ptr;
}

void Complex::operator delete(void* ptr, size_t size)
{
	if (ptr == NULL) return;
	
	if (size != sizeof(Complex)) {
		::operator delete(ptr);
		return;
	}
	
	Complex *deleteNode = static_cast<Complex*>(ptr);
	
	deleteNode->next = headOfFreeList;
	headOfFreeList = deleteNode;
}

int Complex::getNumberOfComplex()
{
	return Complex::numberOfComplex;
}

Complex::Complex(double re, double im)
: re_(re), im_(im)						// constructor initialization list (reference or constant must)
{
	++::Complex::numberOfComplex;
}

Complex::Complex(const Complex& rhs)
: re_(rhs.re_), im_(rhs.im_)
{
	++::Complex::numberOfComplex;
}

Complex::~Complex()
{
	--Complex::numberOfComplex;
}

bool Complex::operator==(const Complex& rhs) const
{
	return re_ == rhs.re_ && im_ == rhs.im_;
}

Complex Complex::operator+(const Complex& rhs) const
{
	double re = re_ + rhs.re_;
	double im = im_ + rhs.im_;
	
	return Complex(re, im);
}

Complex Complex::operator-(const Complex& rhs) const
{
	double re = re_ - rhs.re_;
	double im = im_ - rhs.im_;
	
	return Complex(re, im);
}

double Complex::real() const
{
	return re_;
}

double Complex::imag() const
{
	return im_;
}

void Complex::real(double re)
{
	re_ = re;
}

void Complex::imag(double im)
{
	im_ = im;
}

