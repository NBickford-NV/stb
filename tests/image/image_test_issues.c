#include "1454_gif.h"
#include "1838_gif.h"
#include "1860_png.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../../stb_image.h"
#include "stdio.h"

#define CHECK(x)                                                               \
  {                                                                            \
    int returncode = (x);                                                      \
    if (returncode != EXIT_SUCCESS) {                                          \
      printf("%s failed with code %d!", #x, returncode);                       \
      return returncode;                                                       \
    }                                                                          \
  }

int main() {
  CHECK(test_1454_gif());
  CHECK(test_1838_gif());
  CHECK(test_1860_png());
  return EXIT_SUCCESS;
}
