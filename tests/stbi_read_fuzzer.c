#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../stb_image.h"

// Some bugs only show up when the stack is nonzero. So here we set the stack
// to deterministic values so that earlier fuzz invocations don't affect it.
void setup_stack() {
  // A size of 4096+ seems to work for issue 1861.
  volatile stbi_uc stack[8192];
  for (size_t i = 0; i < sizeof(stack); i++) {
    stack[i] = 0xFF;
  }
}

int LLVMFuzzerTestOneInput(const uint8_t* data, size_t size)
{
    setup_stack();
    
    int x, y, channels, requested_channels;

    /* Read the last byte to determine the number of channels to request;
    this lets the fuzzer find bugs like #1452, which only occurs when
    req_comp == 1, 2, or 3. We read the last byte (insted of the first) so
    that many image files are valid fuzzer inputs. */
    if (size < 1 || size > INT_MAX) return 0;
    requested_channels = (int)(data[size - 1] % 5);
    size--;

    if(!stbi_info_from_memory(data, (int)size, &x, &y, &channels)) return 0;

    /* exit if the image is larger than ~80MB */
    if(y && x > (80000000 / 4) / y) return 0;

    unsigned char *img = stbi_load_from_memory(data, (int)size, &x, &y, &channels, requested_channels);

    free(img);

    return 0;
}

#ifdef __cplusplus
}
#endif