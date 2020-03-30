//Piotr Grabowski
#ifndef _MORPHOLOGY_H_
#define _MORPHOLOGY_H_

class Bitmap{
public:
  virtual unsigned sx() const = 0;
  virtual unsigned sy() const = 0;
  virtual unsigned sz() const = 0;
  virtual bool& operator()(unsigned x, unsigned y, unsigned z) = 0;
  virtual bool operator()(unsigned x, unsigned y, unsigned z) const = 0;
  virtual ~Bitmap(){}
};

class Transformation{
public:
  virtual void transform( Bitmap& ) = 0;
  virtual ~Transformation(){}
};

#endif
