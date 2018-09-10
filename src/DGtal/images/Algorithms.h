#pragma once

#include <type_traits>

namespace DGtal
{

namespace details
{

template <
  typename InputImage,
  typename OutputImage
>
void image_copy_dispath_same_type(InputImage && input, OutputImage & output, std::true_type)
  {
    output = std::forward<InputImage>(input);
  }

} // namespace details

template <
  typename InputImage,
  typename OutputImage
>
void image_copy(InputImage && input, OutputImage & output)
  {
    details::image_copy_dispatch_same_type(
        std::forward<InputImage>(input),
        output,
        std::is_same< typename std::decay<InputImage>::type, typename std::decay<OutputImage>::type >
    );
  }

} // namespace DGtal
