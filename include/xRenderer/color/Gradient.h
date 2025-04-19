#ifndef GRADIENT_H
#define GRADIENT_H

#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>
#include <vector>

#include <xRenderer/color/Color.h>
#include <xRenderer/core/typedefs.h>
#include <xRenderer/core/util.h>

constexpr float color_loop_length = 360.0;
template <typename Color> class Gradient {
public:
  class Node {
  public:
    enum class InterpolationMethod {
      LINEAR,
      SMOOTH
    };
    Node(const Color &c, const float &p) : color(c), position(p) {}
    Color               color;
    float               position;
    InterpolationMethod left_interp  = InterpolationMethod::LINEAR;
    InterpolationMethod right_interp = InterpolationMethod::LINEAR;

    bool operator<(const Node &other) const {
      return position < other.position;
    }
  };

  void addNode(const Color &color, const float &position) {
    if (position < 0 || position > color_loop_length) {
      throw std::out_of_range("Position out of range");
    }
    nodes.insert(Node(color, position));
    reCalculate();
  }

  void clearNodes() { nodes.clear(); }

  Color interpolate(const Node &lower, const Node &upper,
                    const float &fraction) {
    float interp_val = fraction; // To be implemented
    return Color::interpolate(lower.color, upper.color, interp_val);
  }

  /**
   * @brief Calculates a color at an arbitrary position in the color loop.
   * @param position The position in the color loop.
   * @return The color at the given position.
   */
  Color calculateColor(const float &position) {
    if (nodes.empty()) { return Color::black(); }
    const float pos = std::fmod(position, color_loop_length);
    auto upper_it = lower_bound(nodes.begin(), nodes.end(), Node(Color(), pos));
    if (upper_it == nodes.end()) { upper_it = nodes.begin(); }
    auto lower_it =
        lower_bound(nodes.rbegin(), nodes.rend(), Node(Color(), position));
    if (lower_it == nodes.rend()) { lower_it = nodes.rbegin(); }
    float dist_lower = directed_periodic_difference(
        position, lower_it->position, color_loop_length);
    float dist_upper = directed_periodic_difference(
        upper_it->position, position, color_loop_length);
    return interpolate(*lower_it, *upper_it,
                       dist_lower / (dist_lower + dist_upper));
  }

  /**
   * @brief Calculates a color for the index of the lookup table.
   * @param index The index of the color in the lookup table.
   * @return The color at the given index.
   */
  Color calculateColor(const unsigned int &index) {
    if (index >= resolution) { throw std::out_of_range("Index out of range"); }
    float position = color_loop_length * (float(index) / float(resolution));
    return calculateColor(position);
  }

  void reCalculate() {
    lookup.clear();
    lookup.reserve(resolution);
    for (unsigned int i = 0; i < resolution; ++i) {
      lookup.push_back(calculateColor(i));
    }
  }

  /**
   * @brief Gets a color from the lookup table.
   * @param position The position within the color loop.
   * @return The estimated color at the given position.
   */
  Color getColor(const coordType &position) const {
    return lookup[static_cast<unsigned int>(
        resolution * (std::fmod(position * speed + shift, color_loop_length) /
                      color_loop_length))];
  }

  coordType speed = 1.0;
  coordType shift = 0.0;

private:
  std::set<Node>     nodes;
  std::vector<Color> lookup;
  unsigned int resolution = 360; // Number of colors in the gradient lookup
};

#endif // GRADIENT_H