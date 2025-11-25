/* https://github.com/nothings/stb/issues/1861 */

#include "../../stb_image.h"
#include "stdio.h"

// Due to how this issue works, the stack needs to have nonzero values to
// show the bug. We set the next values on the stack here because earlier
// tests might have cleared it.
void test_1861_setup_stack() {
  // A size of 4096+ seems to work.
  volatile stbi_uc stack[8192];
  for (size_t i = 0; i < sizeof(stack); i++) {
    stack[i] = 0xFF;
  }
}

int test_1861_png() {
  test_1861_setup_stack();

  stbi_uc data[89] = {
      0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D,
      0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0A,
      0x08, 0x03, 0x00, 0x00, 0x00, 0xBA, 0xEC, 0x3F, 0x8F, 0x00, 0x00, 0x00,
      0x03, 0x50, 0x4C, 0x54, 0x45, 0x00, 0x00, 0x00, 0xA7, 0x7A, 0x3D, 0xDA,
      0x00, 0x00, 0x00, 0x11, 0x49, 0x44, 0x41, 0x54, 0x78, 0xDA, 0x63, 0x60,
      0x00, 0x81, 0x14, 0x10, 0x60, 0xA0, 0x3D, 0x13, 0x00, 0x02, 0x0E, 0x13,
      0x89, 0x61, 0xBE, 0x38, 0x07, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4E,
      0x44, 0xAE, 0x42, 0x60, 0x82};
  int size = (int)sizeof(data);

  int w, h, c;
  stbi_uc *image = stbi_load_from_memory(data, size, &w, &h, &c, 0);
  if (!image) {
    return EXIT_FAILURE;
  }

  // This image should only consist of black pixels.
  // However, before this image was fixed, its palette indices would read
  // out-of-bounds values from the stack-allocated palette buffer.
  const int num_pixels = w * h * c;
  int nonzero_pixels = 0;
  for (int i = 0; i < num_pixels; i++) {
    if (image[i] != 0) {
      nonzero_pixels++;
    }
  }
  stbi_image_free(image);

  if (nonzero_pixels != 0) {
    printf("Test for issue #1861 failed: %d out of %d pixels were nonzero!\n",
           nonzero_pixels, num_pixels);
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
