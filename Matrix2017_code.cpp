#include "Matrix2017.h"

int max(int a, int b)
{
	return (a > b ? a : b);
}

/*Matrix class implementation*/
Matrix::Matrix() //default constructor
{
	nRows = nColumns = 0;
	values = NULL;
}
Matrix::~Matrix() //default destructor
{
	reset();
}

Matrix::Matrix(int nRows, int nColumns, int initialization, double initializationValue)
{
	this->nRows = nRows;
	this->nColumns = nColumns;
	if ((nRows*nColumns) == 0)
	{
		values = NULL;
		return;
	}
	values = new double*[nRows];
	for (int iR = 0;iR<nRows;iR++)
	{
		values[iR] = new double[nColumns];
		for (int iC = 0;iC<nColumns;iC++)
		{
			switch (initialization)
			{
			case MI_ZEROS:
				values[iR][iC] = 0;
				break;
			case MI_ONES:
				values[iR][iC] = 1;
				break;
			case MI_EYE:
				values[iR][iC] = (iR == iC) ? 1 : 0;
				break;
			case MI_RAND: values[iR][iC] = (rand() % 1000000) / 1000000.0;
				break;
			case MI_VALUE:
				values[iR][iC] = initializationValue;
				break;
			}
		}
	}
}

Matrix::Matrix(int nRows, int nColumns, double first, ...)
{
	this->nRows = nRows;
	this->nColumns = nColumns;
	if ((nRows*nColumns) == 0)
	{
		values = NULL;
		return;
	}
	values = new double*[nRows];
	va_list va;
	va_start(va, first);
	for (int iR = 0;iR<nRows;iR++)
	{
		values[iR] = new double[nColumns];
		for (int iC = 0;iC<nColumns;iC++)
		{
			values[iR][iC] = (iC == 0 && iR == 0) ? first : va_arg(va, double);
		}
	}
	va_end(va);
}

Matrix::Matrix(Matrix& m)
{
	nRows = nColumns = 0;
	values = NULL;
	copy(m);
}

Matrix::Matrix(string s)
{
	nRows = nColumns = 0;
	values = NULL;
	copy(s);
}

Matrix::Matrix(double d)
{
	nRows = nColumns = 0;
	values = NULL;
	copy(d);
}

void Matrix::copy(Matrix& m)
{
	reset();
	this->nRows = m.nRows;
	this->nColumns = m.nColumns;
	if ((nRows*nColumns) == 0)
	{
		values = NULL;
		return;
	}
	values = new double*[nRows];
	for (int iR = 0;iR<nRows;iR++)
	{
		values[iR] = new double[nColumns];
		for (int iC = 0;iC<nColumns;iC++)
		{
			values[iR][iC] = m.values[iR][iC];
		}
	}
}

void Matrix::copy(double d)
{
	reset();
	this->nRows = 1;
	this->nColumns = 1;
	values = new double*[1];
	values[0] = new double[1];
	values[0][0] = d;
}

void Matrix::copy(string s)
{
	reset();
	char* buffer = new char[s.length() + 1];
	strcpy_s(buffer, s.length() + 1, s.c_str());
	char* lineContext;
	char* lineSeparators = ";\r\n";
	char* line = strtok_s(buffer, lineSeparators, &lineContext);
	while (line)
	{
		Matrix row; char* context;char* separators = " []";
		char* token = strtok_s(line, separators, &context);
		while (token)
		{
			Matrix item = atof(token);
			row.addColumn(item);
			token = strtok_s(NULL, separators, &context);
		}
		if (row.nColumns>0 && (row.nColumns == nColumns || nRows == 0))
			addRow(row);line = strtok_s(NULL, lineSeparators, &lineContext);
	}
	delete[] buffer;
}

void Matrix::reset()
{
	if (values)
	{
		for (int i = 0;i<nRows;i++)
			delete[] values[i];
		delete[] values;
	}
	nRows = nColumns = 0;
	values = NULL;
}

string Matrix::getString()
{
	string s="";
	for (int iR = 0; iR<nRows; iR++)
	{
		for (int iC = 0; iC<nColumns; iC++)
		{
			//cout << values[iR][iC] << " ";

			char buffer[50]="";
			sprintf_s(buffer, 50, "%g\t", values[iR][iC]);
			s += buffer;
		}
		//cout << endl;
		s += "\n";
	}
	return s;
}

Matrix Matrix::operator=(Matrix& m)
{
	copy(m);
	return *this;
}

Matrix Matrix::operator=(double d) { copy(d);return *this; }
Matrix Matrix::operator=(string s) { copy(s);return *this; }

void Matrix::add(Matrix& m)
{
	if (nRows != m.nRows || nColumns != m.nColumns)
		throw("Invalid matrix dimension");
	for (int iR = 0; iR < nRows; iR++)
	{
		for (int iC = 0;iC < nColumns;iC++)
			values[iR][iC] += m.values[iR][iC];
	}
}
Matrix Matrix::operator+(Matrix& m) { Matrix r = *this;r += m;return r; }
/*
 it is used to:
 add two matrices and return a matrix

 example:
 Matrix a,b,c;
 a=b+c;
 //or
 a=a+b;

 implementation description:
 it takes matrix (m) by reference
 then creates class variable (r) of type matrix and saves the value of (this) pointer in it ((this) pointer refers to the object that called the operator)
 then it adds the elements of (m) to the elements of (r) by using (+=) which uses (add) function in its implementation
 then it returns r
*/
Matrix Matrix::operator+(double d) { Matrix r = *this;r += d;return r; }

/*
it used to:
add a value to each element of the matrix the called the operator then outputs it

example:
double d;
Matrix a,b;
a=a+d;
//or
a=b+d;

implementation description:
it creates class variable (r) of type matrix and saves the value of (this) pointer in it ((this) pointer refers to the object that called the operator)
then it adds the (d) to the elements of (r) by using (+=) which uses (add) function in its implementation
then it returns r
*/

void Matrix::sub(Matrix& m)
{
	if (nRows != m.nRows || nColumns != m.nColumns)
		throw("Invalid matrix dimension");
	for (int iR = 0;iR < nRows;iR++)
	{
		for (int iC = 0;iC < nColumns;iC++)
			values[iR][iC] -= m.values[iR][iC];
	}
}
/*
it is used to:
subtract a matrix elements' values from another matrix

example:
Matrix a,b;
a.sub(b);

implementation description:
first it checked that the dimension of the two matrices are equal
if not equal -> it throws error message
if equal -> it for loops on the whole matrix
and subtract the matrix n from (this) matrix by using (-=)
so it doesn't have to return anything

*/
void Matrix::operator-=(Matrix& m) { sub(m); }
/*
it is used to:
subtract matrix elements (m) from another matrix elements (this matrix)//the obj that called the operator

example:
Matrix a,b;
a-=b;

implementation description:
it takes the matrix m by reference then it calls the sub function

*/


void Matrix::operator-=(double d) { sub(Matrix(nRows, nColumns, MI_VALUE, d)); }
/*
it is used to:
subtract a double (d) from the matrix elements (this)

example:
Matrix a;
double d;
a-=d;

implementation description:
it calls the (sub) function which takes Matrix parameter
and in it, it calls Matrix constructor to create a Matrix with same dimensions as (this)
and with MI_VALUE enum to initialize all elements of this matrix with the value of double d
then it passes the created matrix to (sub) which subtracts the elements of the passed Matrix from the (this) Matrix
so it doesn't return anything

*/
Matrix Matrix::operator-(Matrix& m) { Matrix r = *this;r -= m;return r; }
/*
it is used to:
subtract two matrices,, and return the result matrix

example:
Matrix a,b,c;
a=b-c;
//or
a=a-b;

implementation description:
it creates an object variable of type Matrix (r) then gives it the reference of (this) (the object that called the operator)
then uses the (-=) operator to subtract elements of (m) from (r)(this)
then returns the matrix (r)
*/

Matrix Matrix::operator-(double d) { Matrix r = *this;r -= d;return r; }
/*
it is used to:
subtract double from matrix,, and return the result matrix

example:
double d;
Matrix a,b;
b=a-d;

implementation description:
it creates an object variable of type Matrix (r) then gives it the reference of (this) (the object that called the operator)
then uses the (-=) operator to subtract double d from elements of (r)(this)
then returns the matrix (r)
*/

void Matrix::mul(Matrix& m)
{
	if (nRows != m.nRows || nColumns != m.nColumns)
	throw("Invalid matrix dimension");
	Matrix r(nRows, m.nColumns); // can change m.nColumns -> nColumns
	for (int iR = 0;iR<r.nRows;iR++)
	{
		for (int iC = 0;iC < r.nColumns;iC++)
		{
			r.values[iR][iC] = 0; // can delete this but use constructor with enum MI_ZEROS
			for (int k = 0;k < m.nColumns;k++)
				r.values[iR][iC] += values[iR][k] * m.values[k][iC];
		}
	}
	copy(r);
}
/*
it is used to:
multiply (this) Matrix to another Matrix (m) then saves the result in (this) Matrix
so it doesn't return anything

example:
Matrix a,b;
a.mul(b);

implementation description:
first it checks that the Matrix (m) has the same dimension as (this) Matrix
if not -> it throws error message
then it creates a new Matrix (r) with the same dimension as them
for each element of r (iR intersecting iC) :
it takes every element of iR row in (this) Matrix and multiplies it with the corrosponding iC column in (m) Matrix
then adds the results to r.values[iR][iC]

then it uses the copy function to put the (r) Matrix in (this) Matrix

*/
void Matrix::operator*=(Matrix& m) { mul(m); }
/*
it is used to:
multiply (this) Matrix to a Matrix (m)
and then saves the result in (this) Matrix
so it doesn't return anything

example:
Matrix a,b;
a*=b;

implementation description:
it uses the (mul) function to multiply (this) Matrix by (m) Matrix then saves the result at (this) Matrix
*/
void Matrix::operator*=(double d)
{
	for (int iR = 0;iR<nRows;iR++)
		for (int iC = 0;iC<nColumns;iC++)
			values[iR][iC] *= d;
}
/*
it is used to:
multiply each element of (this) Matrix by double d then saves the result in (this) Matrix
so it doesn't return anything

example:
double d;
Matrix a;
a*=d;

implementation description:
it loops on the each elements of (this) Matrix and multiplies it with d then saves the result in the element
*/

Matrix Matrix::operator*(Matrix& m) { Matrix r = *this;r *= m;return r; }
/*
it is used to:
multiply (this) Matrix by another Matrix (m) and returns the result

example:
Matrix a,b,c;
a=b*c;
//or
a=a*b;

implementation description:
it creates class variable of type Matrix (r) and make it refer to (this) Matrix
then uses the operator (*=) to multiply (r) by Matrix (m) and save the result in (r)
then returns the Matrix (r)
*/

Matrix Matrix::operator*(double d) { Matrix r = *this;r *= d;return r; }
/*
it is used to:
multiply elements of (this) Matrix by double (d) and return the result Matrix

example:
double d;
Matrix a,b;
a=b*d;
//or
a=a*d;

implementation description:
it creates class variable of type Matrix (r) and make it refer to (this) Matrix
then uses the operator (*=) to multiply elements of (r) by double (d) and save the result in (r)
then returns the Matrix (r)
*/

Matrix Matrix::operator/(Matrix& m) //C = A/B where C, A and B are all matrices
{
	Matrix r = *this;
	r = r.div(m);
	return r;
}

Matrix Matrix::operator/(double d) //C = A/B where C, A are matrices and B is a double
{
	Matrix r = *this;
	for (int iR = 0; iR<r.nRows; iR++)
		for (int iC = 0; iC<r.nColumns; iC++)
			r.values[iR][iC] /= d;
	return r;
}

void Matrix::operator/=(Matrix& m) // Divides by m and stores the result in the calling function
{
	div(m);
}

void Matrix::operator/=(double d) // Divides by d (element wise) and stores the result in the calling function
{
	for (int iR = 0; iR<nRows; iR++)
		for (int iC = 0; iC<nColumns; iC++)
			values[iR][iC] /= d;
}

Matrix Matrix::div(Matrix m)//div C = A/B = A * B.getInverse();
{
	if (nColumns != m.nRows)
		throw("First matrix must have the same number of columns as the rows in the second matrix.");
	Matrix t = m.getInverse(); // get the inverse of B
	Matrix r = *this;
	r *= t;
	return r; // return r the result of the division process
}

Matrix Matrix::operator++() { add(Matrix(nRows, nColumns, MI_VALUE, 1.0));return *this; }

Matrix Matrix::operator++() { add(Matrix(nRows, nColumns, MI_VALUE, 1.0));return *this; }
/*
it is used to:
increase the value of elements of (this) Matrix by 1
and returns the new value

example:
Matrix a,b;
a++;
//or
b=a++;

implementation description:
it uses the (add) function which adds the passed Matrix to (this) Matrix
and it created the passed Matrix with constructor and enum MI_VALUE and initializing value 1
so it adds 1 to every element of (this) Matrix

*/

Matrix Matrix::operator++(int) { Matrix C = *this;add(Matrix(nRows, nColumns, MI_VALUE, 1.0));return C; }
/*
it is used to:
increase the value of elements of (this) Matrix by int 1
but it returns the old value
the int is used to differentiate between it and +++()
the old value is saved in C

example:
int x
Matrix a,b;
a++x;
//or
b=a++x;

implementation description:
it uses the (add) function which adds the passed Matrix to (this) Matrix
and it created the passed Matrix with constructor and enum MI_VALUE and initializing value 1
so it adds 1 to every element of (this) Matrix
*/
Matrix Matrix::operator--() { add(Matrix(nRows, nColumns, MI_VALUE, -1.0));return *this; }
/*
it is used to:
decrease the value of elements of (this) Matrix by 1
and return the new value

example:
Matrix a,b;
a--;
//or
b=a--;

implementation description:
it uses the (add) function which adds the passed Matrix to (this) Matrix
and it created the passed Matrix with constructor and enum MI_VALUE and initializing value -1
so it add -1 to every element of (this) Matrix
*/
Matrix Matrix::operator--(int ) { Matrix r = *this;add(Matrix(nRows, nColumns, MI_VALUE, -1.0));return r; }
/*
it is used to:
decrease the value of elements of (this) Matrix by 1
and returns the old value
the old value is saved in r

example:
Matrix a,b;
int x;
a--x;
//or
b=a--x;

implementation description:
it uses the (add) function which adds the passed Matrix to (this) Matrix
and it created the passed Matrix with constructor and enum MI_VALUE and initializing value -1
so it adds -1 to every element of (this) Matrix
*/
Matrix Matrix::operator-()
{
	for (int iR = 0;iR < nRows;iR++)
	{
		for (int iC = 0;iC < nColumns;iC++)
			values[iR][iC] = -values[iR][iC];
	}
	return *this;
}
/*
it is used to:
to multiply each element of (this) Matrix with -1
it returns a Matrix just to allow this line (a=-b;)
but you can use it like this (-a;)

example:
Matrix a,b;
a=-b;
//or
-a;

implementation description:
it loops on the whole (this) Matrix and multiply each element - and save it in the same element

*/

Matrix Matrix::operator+() { return *this; }

void Matrix::setSubMatrix(int r, int c, Matrix& m)
{
	if ((r + m.nRows)>nRows || (c + m.nColumns)>nColumns)
		throw("Invalid matrix dimension");
	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
			values[r + iR][c + iC] = m.values[iR][iC];
}

Matrix Matrix::getSubMatrix(int r, int c, int nRows, int nColumns)
{
	if ((r + nRows)>nRows || (c + nColumns)>nColumns)
		throw("Invalid matrix dimension");
	Matrix m(nRows, nColumns);
	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
			m.values[iR][iC] = values[r + iR][c + iC];
	return m;
}

/*void Matrix::setMatrix()
{

}*/
void Matrix::addColumn(Matrix& m)
{
	Matrix n(max(nRows, m.nRows), nColumns + m.nColumns);
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(0, nColumns, m);
	*this = n;
}

void Matrix::addRow(Matrix& m)
{
	Matrix n(nRows + m.nRows, max(nColumns, m.nColumns));
	n.setSubMatrix(0, 0, *this);
	n.setSubMatrix(nRows, 0, m);
	*this = n;
}

Matrix Matrix::getCofactor(int r, int c)
{
	if (nRows <= 1 && nColumns <= 1)
		throw("Invalid matrix dimension");
	Matrix m(nRows - 1, nColumns - 1);
	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
		{
			int sR = (iR<r) ? iR : iR + 1;
			int sC = (iC<c) ? iC : iC + 1;
			m.values[iR][iC] = values[sR][sC];
		}
	return m;
}

double Matrix::getDeterminant()
{
	if (nRows != nColumns)
		throw("Invalid matrix dimension");
	if (nRows == 1 && nColumns == 1)return values[0][0];
	double value = 0, m = 1;
	for (int iR = 0;iR<nRows;iR++)
	{
		value += m * values[0][iR] * getCofactor(0, iR).getDeterminant();
		m *= -1;
	}
	return value;
}

istream& operator >> (istream &is, Matrix& m) //inputs the matrix through "cin>>" example: Matrix myMatrix; cin>>myMatrix;
{
	string s;
	getline(is, s, ']'); //] is the delimiter at which the getline knows this is the end of the string
	s += "]"; //because it wasn't saved in the actual string and the constructor uses it
	m = Matrix(s); //uses the constructor which takes an input string
	return is;
}
ostream& operator << (ostream &os, Matrix& m) //prints out the matrix elements using "cout<<"
{
	os << m.getString(); 
	return os;
}

Matrix Matrix::getInverse()//inverse=(1/determinant)*transpose of cofactor matrix
{
	if (nRows != nColumns)//inverse can only be done on square matrices
		throw("Invalid Matrix Dimension");
	Matrix n(nRows, nColumns);// copy matrix
	for (int iR = 0;iR<n.nRows;iR++)
		for (int iC = 0;iC<n.nColumns;iC++)
		{
			n.values[iR][iC] = values[iR][iC];
		}
	double det_value = n.getDeterminant();//determinant value of the matrix

	Matrix m(nRows, nColumns);//cofactor matrix
	int sign = 1;

	for (int iR = 0;iR<m.nRows;iR++)
		for (int iC = 0;iC<m.nColumns;iC++)
		{
			m.values[iR][iC] = sign*n.getCofactor(iR, iC).getDeterminant();//getting detreminant values of cofactor matrix
			sign *= -1;//following sign rule in matrices
		}

	m.getTranspose();//transpose of cofactor matrix
	m *= (1 / det_value);
	return m;
}


Matrix Matrix::getTranspose() {
	Matrix x(nColumns, nRows);
	for (int ir = 0; ir<x.nRows;ir++) {
		for (int ic = 0; ic<x.nColumns;ic++) {
			x.values[ir][ic] = values[ic][ir];
		}
	}
	return x;
}



//void mul_dot_astrisk(Matrix& m)
//{
//	if (nRows != m.nRows || nColumns != m.nColumns)
//		throw("Invalid matrix dimension");
//
//}
