//Piotr Grabowski
#include "Morphology.cpp"
int main(){
	BitmapExt bitmapA(3,4,5);
	bitmapA(0,1,0)= bitmapA(0,2,2) = bitmapA(0,2,3) = bitmapA(0,1,2)= true;
	bitmapA(1,2,1) = bitmapA(1,2,2) = bitmapA(1,2,3) = bitmapA(1,1,1) = bitmapA(2,1,2)= true;
	bitmapA(2,2,1) = bitmapA(2,2,2) = bitmapA(2,2,3) = bitmapA(2,1,1) = true;
	std::cout << "Oryginal bitmap:\n" << bitmapA << "\n\n";

	Averaging averaging;
	Dilatation dilatation;
	Erosion erosion;
	Reset reset;

	TransformationComposition transfcomp1;
	transfcomp1.addTransformation(&averaging);
	transfcomp1.addTransformation(&dilatation);
	transfcomp1.transform(bitmapA);
	std::cout << "Reformatted bitmap after averaging and dilatation:\n" << bitmapA << "\n";

	TransformationComposition transfcomp2;
	transfcomp2.addTransformation(&erosion);
	transfcomp2.transform(bitmapA);
	std::cout << "Reformatted bitmap after erosion additionally:\n" << bitmapA << "\n";

	TransformationComposition transfcomp3;
	transfcomp3.addTransformation(&reset);
	transfcomp3.transform(bitmapA);
	std::cout << "Reformatted bitmap after reset:\n" << bitmapA << "\n";
}
