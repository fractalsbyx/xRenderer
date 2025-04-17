#ifndef CANVAS_H
#define CANVAS_H

#include <xRenderer/color/Color.h>
#include <xRenderer/core/Grid2D.h>
#include <xRenderer/core/Layer.h>
#include <xRenderer/core/Meshpoint.h>
#include <xRenderer/core/XYMapping.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/samplers/Sampler.h>
#include <xRenderer/super_samplers/SuperSampler.h>

#include <memory>
#include <vector>

template <typename Color> class Canvas {
public:
  Canvas(const meshInt &_xres, const meshInt &_yres,
         const std::shared_ptr<SuperSampler> &_superSampler =
             std::make_shared<SuperSampler>())
      : xres(_xres), yres(_yres), capacity(xres * yres),
        superSampler(_superSampler) {
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

  void setLocation(const Complex &x, const coordType &s,
                   const coordType &rotation = 0.0) {
    xymapping.setLocation(x, s, rotation);
  }

  void addLayer(
      const std::shared_ptr<Sampler<Color>> &sampler,
      const std::shared_ptr<Mapping> &mapping = std::make_shared<Mapping>()) {
    Layer<Color> layer(sampler, mapping);
    layer.setVisible(true);
    layers.push_back(layer);
  }

  void draw() {
    for (meshInt linear_index = 0; linear_index < capacity; ++linear_index) {
      Meshpoint            mPoint = grid.getMeshPoint(linear_index);
      std::vector<Complex> sample_points =
          superSampler->getSamplePoints(mPoint.toComplex(), xymapping);
      std::vector<Color> sample_colors;
      sample_colors.reserve(sample_points.size());
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
  meshInt                       xres     = 0;
  meshInt                       yres     = 0;
  meshInt                       capacity = 0;
  Grid2D<Complex>               grid;
  Grid2D<Color>                 image;
  XYMapping                     xymapping;
  std::shared_ptr<SuperSampler> superSampler = nullptr;
};

#endif // CANVAS_H