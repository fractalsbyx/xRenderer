#ifndef CANVAS_H
#define CANVAS_H

#include "Color.h"
#include "Grid2D.h"
#include "Layer.h"
#include "Meshpoint.h"
#include "Sampler.h"
#include "SuperSampler.h"
#include "XYMapping.h"
#include "typedefs.h"
#include <memory>
#include <vector>

template <typename Color> class Canvas {
public:
  Canvas(const meshInt &_xres, const meshInt &_yres,
         const std::shared_ptr<SuperSampler> &_superSampler =
             std::make_shared<SuperSampler>())
      : xres(_xres), yres(_yres), superSampler(_superSampler) {
    setResolution(xres, yres);
  }

  void setResolution(const meshInt &x, const meshInt &y) {
    grid.resize(x, y);
    image.resize(x, y);
    xymapping.resize(x, y);
    xres     = x;
    yres     = y;
    capacity = xres * yres;
  }

  void setSuperSampler(const std::shared_ptr<SuperSampler> &sampler) {
    superSampler = sampler;
  }

  void setLocation(const Complex &x, const coordType &rotation) {
    xymapping.setLocation(x, rotation);
  }

  void addLayer(
      const std::shared_ptr<Sampler<Color>> &sampler,
      const std::shared_ptr<Mapping> &mapping = std::make_shared<Mapping>()) {
    Layer layer(sampler, mapping);
    layer.setVisible(true);
    layers.push_back(layer);
  }

  void draw() {
    for (meshInt linear_index = 0; linear_index < capacity; ++linear_index) {
      Meshpoint            mPoint = grid.getMeshPoint(linear_index);
      std::vector<Complex> sample_points =
          superSampler->getSamplePoints(mPoint.toComplex(), xymapping);
      std::vector<Color> sample_colors(sample_points.size());
      for (const Complex &sample_pt : sample_points) {
        Color sample_color = Color::blank();
        for (auto &layer : layers) {
          if (layer.isVisible()) {
            Color layer_color = layer.sample(sample_pt);
            sample_color =
                Color::applyLayer(sample_color, layer_color, layer.getOpacity(),
                                  layer.getMixingMode());
          }
        }
        sample_colors.push_back(sample_color);
      }
      image(linear_index) = Color::blend(sample_colors);
    }
  }

  const Grid2D<Color> &getImage() const { return image; }

  std::vector<Layer<Color>> layers;

private:
  Grid2D<Complex>               grid;
  Grid2D<Color>                 image;
  XYMapping                     xymapping;
  std::shared_ptr<SuperSampler> superSampler = nullptr;

  meshInt xres     = 0;
  meshInt yres     = 0;
  meshInt capacity = xres * yres;
};

#endif // CANVAS_H