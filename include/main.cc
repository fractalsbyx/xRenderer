#include <Sampler.h>
class mySapmpler : Sampler {
}

int main(){
  // Create a Canvas object
  Canvas<10, 10> canvas;

  // Access the Complex and image Grid2Ds
  auto &ComplexGrid2D = canvas.Complex;
  auto &imageGrid2D = canvas.image;

  // Set some values in the Complex Grid2D
  ComplexGrid2D(0, 0) = 1.0;
  ComplexGrid2D(1, 1) = 2.0;

  // Set some values in the image Grid2D
  imageGrid2D(0, 0) = Color(255, 0, 0); // Red
  imageGrid2D(1, 1) = Color(0, 255, 0); // Green

  return 0;
}