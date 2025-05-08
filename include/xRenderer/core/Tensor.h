#ifndef TENSOR_H
#define TENSOR_H

#include <array>
#include <type_traits>

template <typename number, unsigned int spacedim, unsigned int rank = 1>
class Tensor {
  static_assert(rank > 0, "Rank must be greater than 0");
  using this_rank_tensor = Tensor<number, spacedim, rank>;
  using lower_rank_tensor =
      std::conditional_t<rank == 1, number, Tensor<number, spacedim, rank - 1>>;

  using array_type =
      std::conditional_t<rank == 1, number[(dim != 0) ? dim : 1],
                         typename Tensor<number, spacedim, rank_ - 1>::
                             array_type[(dim != 0) ? dim : 1]>;

public:
  Tensor()                               = default;
  Tensor(const Tensor &other)            = default;
  Tensor &operator=(const Tensor &other) = default;
  Tensor &operator=(Tensor &&other)      = default;
  ~Tensor()                              = default;

  lower_rank_tensor       &operator[](unsigned int i) { return data[i]; }
  const lower_rank_tensor &operator[](unsigned int i) const { return data[i]; }

  Tensor operator+(const this_rank_tensor &other) const {
    Tensor result;
    for (unsigned int i = 0; i < spacedim; ++i) {
      result[i] = data[i] + other[i];
    }
    return result;
  }
  Tensor operator-(const this_rank_tensor &other) const {
    Tensor result;
    for (unsigned int i = 0; i < spacedim; ++i) {
      result[i] = data[i] - other[i];
    }
    return result;
  }
    Tensor operator*(const 

private:
  lower_rank_tensor data[spacedim];
    // Add your tensor operations here
};

#endif // TENSOR_H