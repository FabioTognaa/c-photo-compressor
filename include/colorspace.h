#include <stddef.h>

/* RGB to YCbCr: converts the pixels' RGB format into TYCbCr using ITU-R BT.601 standard.
* returns planar arrays of Y, Cb and Cr parmateres:
|--...-|------...-|-------|
YYYY...CbCbCbCb...CrCrCrCr 
*/
void rgb_to_ycbcr(unsigned char *rgb, size_t rgb_size, unsigned char *y, unsigned char *cb, unsigned char *cr);

/* subsampling function: reduces the crominance channels resolution by doing an average between 2x2 pixel blocks' Cb and Cr
*/
void subsampling(unsigned char *cb, unsigned char *cr, size_t width, size_t height);