//Piotr Grabowski
#include "Morphology.h"
#include <iostream>
#include <vector>

class BitmapExt : public Bitmap
{
public:
	unsigned int dimensionOfX, dimensionOfY, dimensionOfZ;
	bool ***arrayOfPixels;

	BitmapExt(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ)
	{
		dimensionOfX = sizeX;
		dimensionOfY = sizeY;
		dimensionOfZ = sizeZ;

        arrayOfPixels = new bool**[sizeX];
		for(unsigned int i = 0; i < sizeX; i++)
		{
			arrayOfPixels[i]=new bool*[sizeY];
			for(unsigned int j = 0; j < sizeY; j++)
				arrayOfPixels[i][j]=new bool[sizeZ];
		}

		for(unsigned int i = 0; i < sizeX; i++)
			for(unsigned int j = 0; j < sizeY; j++)
				for(unsigned int z = 0; z < sizeZ; z++)
					arrayOfPixels[i][j][z] = false;
	}

	unsigned int sx() const
	{
		return dimensionOfX;
	}

	unsigned int sy() const
	{
		return dimensionOfY;
	}

	unsigned int sz() const
	{
		return dimensionOfZ;
	}

	bool& operator()(unsigned x, unsigned y, unsigned z)
	{
		return arrayOfPixels[x][y][z];
	}

	bool operator()(unsigned x, unsigned y, unsigned z) const
	{
		return arrayOfPixels[x][y][z];
	}

	BitmapExt& operator=(const BitmapExt& w)
	{
		if(this != &w)
		{
			delete[] this -> arrayOfPixels;

			dimensionOfX = w.dimensionOfX;
			dimensionOfY = w.dimensionOfY;
			dimensionOfZ = w.dimensionOfZ;

			arrayOfPixels = new bool**[dimensionOfX];
			for(unsigned int i = 0; i < dimensionOfX; i++)
			{
				arrayOfPixels[i]=new bool*[dimensionOfY];
				for(unsigned int j = 0; j < dimensionOfY; j++)
					arrayOfPixels[i][j]=new bool[dimensionOfZ];
			}

			for(unsigned int i = 0; i < sx(); i++)
				for(unsigned int j = 0; j < sy(); j++)
					for(unsigned int z = 0; z < sz(); z++)
						this -> arrayOfPixels[i][j][z] = w.arrayOfPixels[i][j][z];
		}
        return *this;
    }

	BitmapExt(const BitmapExt& copy_)
	{
		dimensionOfX = copy_.dimensionOfX;
		dimensionOfY = copy_.dimensionOfY;
		dimensionOfZ = copy_.dimensionOfZ;

		arrayOfPixels = new bool**[dimensionOfX];
		for(unsigned int i = 0; i < dimensionOfX; i++)
		{
			arrayOfPixels[i]=new bool*[dimensionOfY];
			for(unsigned int j = 0; j < dimensionOfY; j++)
				arrayOfPixels[i][j]=new bool[dimensionOfZ];
		}

		for(unsigned int i = 0; i < dimensionOfX; i++)
			for(unsigned int j = 0; j < dimensionOfY; j++)
				for(unsigned int z = 0; z < dimensionOfZ; z++)
					arrayOfPixels[i][j][z] = copy_.arrayOfPixels[i][j][z];
	}

	~BitmapExt()
	{
		for(unsigned int i = 0; i < sx(); i++)
			for(unsigned int j = 0; j < sy(); j++)
				delete arrayOfPixels[i][j];

		for(unsigned int i = 0; i < sx(); i++)
			delete arrayOfPixels[i];

		delete[] arrayOfPixels;
	}
};

std::ostream& operator<<(std::ostream& out, const Bitmap& a)
{
	out << "{\n" ;
	for(unsigned int i = 0; i < a.sx(); i++)
	{
		out << " {\n";
		for(unsigned int j = 0; j < a.sy(); j++)
		{
			out << "  {";
			for(unsigned int z = 0; z < a.sz(); z++)
			{
				out << a(i,j,z);
				if(z != a.sz() - 1) out << ",";
			}
			if(j != a.sy() - 1)
				out << "}, \n";
			else
				out << "}\n";
		}
		if(i != a.sx() - 1)
			out << " }, \n";
		else
			out << " }\n";
	}
	out << "}";
	return out;
}

class Inversion : public Transformation
{
public:
	void transform(Bitmap& a)
	{
		for(unsigned int i = 0; i < a.sx(); i++)
		{
			for(unsigned int j = 0; j < a.sy(); j++)
			{
				for(unsigned int z = 0; z < a.sz(); z++)
					if(a(i,j,z) == false)
						a(i,j,z) = true;
					else a(i,j,z) = false;
			}
		}
	}
};

int check_neigh(BitmapExt& a, int i,  int j,  int z, bool bool_, int& counter)
{
	if( (z - 1 >= 0) )
	{
		if( a(i,j,z-1) == bool_ )
			counter++;
	}
	if( z + 1 <= a.sz() - 1 )
	{
		if( a(i,j,z+1) == bool_ )
			counter++;
	}
	if( (j - 1 >= 0) )
	{
		if( a(i,j-1,z) == bool_ )
			counter++;
	}
	if( j + 1 <= a.sy() - 1 )
	{
		if( a(i,j+1,z) == bool_ )
			counter++;
	}
	if( (i - 1 >= 0) )
	{
		if( a(i-1,j,z) == bool_ )
			counter++;
	}
	if( (i + 1 <= a.sx() - 1) )
	{
		if( a(i+1,j,z) == bool_ )
			counter++;
	}
	return counter;
}

class Erosion : public Transformation
{
public:
	void transform(Bitmap& a)
	{
		int counter1 = 0;
		int counter = 0;
		BitmapExt b(a.sx(), a.sy(), a.sz());

		for( int i = 0; i < a.sx(); i++)
		{
			for( int j = 0; j < a.sy(); j++)
			{
				for( int z = 0; z < a.sz(); z++)
					b.arrayOfPixels[i][j][z] = a(i,j,z);
			}
		}

		for(int i = 0; i < a.sx(); i++)
		{
			for(int j = 0; j < a.sy(); j++)
			{
				for(int z = 0; z < a.sz(); z++)
				{
					if(b(i,j,z) == true)
					{
						counter1 = check_neigh(b, i, j, z, false, counter);
						if(counter1 >= 1) a(i,j,z) = false;
						counter1 = 0;
						counter = 0;
					}
				}
			}
		}
	}
};

class Dilatation : public Transformation
{
public:
	void transform(Bitmap& a)
	{
		int counter1 = 0;
		int counter = 0;
		BitmapExt b(a.sx(), a.sy(), a.sz());

		for( int i = 0; i < a.sx(); i++)
		{
			for( int j = 0; j < a.sy(); j++)
			{
				for( int z = 0; z < a.sz(); z++)
					b.arrayOfPixels[i][j][z] = a(i,j,z);
			}
		}

		for(int i = 0; i < a.sx(); i++)
		{
			for(int j = 0; j < a.sy(); j++)
			{
				for(int z = 0; z < a.sz(); z++)
				{
					if(b(i,j,z) == false)
					{
						counter1 = check_neigh(b, i, j, z, true, counter);
						if(counter1 >= 1) a(i,j,z) = true;
						counter1 = 0;
						counter = 0;
					}
				}
			}
		}
	}
};

class Reset : public Transformation
{
public:
	void transform(Bitmap& a)
	{
		for(unsigned int i = 0; i < a.sx(); i++)
		{
			for(unsigned int j = 0; j < a.sy(); j++)
			{
				for(unsigned int z = 0; z < a.sz(); z++)
					a(i,j,z) = false;
			}
		}
	}
};

class Averaging : public Transformation
{
public:
	void transform(Bitmap& a)
	{
		int counter1 = 0;
		int counter = 0;
		BitmapExt b(a.sx(), a.sy(), a.sz());

		for( int i = 0; i < a.sx(); i++)
		{
			for( int j = 0; j < a.sy(); j++)
			{
				for( int z = 0; z < a.sz(); z++)
					b.arrayOfPixels[i][j][z] = a(i,j,z);
			}
		}

		for(int i = 0; i < a.sx(); i++)
		{
			for(int j = 0; j < a.sy(); j++)
			{
				for(int z = 0; z < a.sz(); z++)
				{
					if(b(i,j,z) == false)
					{
						counter1 = check_neigh(b, i, j, z, true, counter);
						if(counter1 >= 4) a(i,j,z) = true;
						counter1 = 0;
						counter = 0;
					}

					if(b(i,j,z) == true)
					{
						counter1 = check_neigh(b, i, j, z, false, counter);
						if(counter1 >= 4) a(i,j,z) = false;
						counter1 = 0;
						counter = 0;
					}
				}
			}
		}
	}
};

class TransformationComposition : public Transformation
{
public:
	std::vector <Transformation*> arr;

	void addTransformation (Transformation* p){
		arr.push_back(p);
	}

	void transform(Bitmap& a){
		for(int i = 0; i < arr.size(); i++)
			arr[i] -> transform(a);
	}
};

/* #include "Morfologia.cpp" int main(){ BitmapExt b(3,4,5); b(0,1,0)= b(0,2,2) = b(0,2,3) = b(0,1,2)= true; b(1,2,1) = b(1,2,2) = b(1,2,3) = b(1,1,1) = b(2,1,2)= true; b(2,2,1) = b(2,2,2) = b(2,2,3) = b(2,1,1) = true; std::cout << "Oryginalna Bitmap:\n" << b << "\n\n"; Averaging u; Dilatation d; ZlozeniePrzeksztalcen z; z.addTransformation(&u); z.addTransformation(&d); // Transformation 'z' najpierw wykona Averaging, a potem dylatacje z.transform(b); std::cout << "Przeksztalcona Bitmap:\n" << b << "\n"; }
*/
