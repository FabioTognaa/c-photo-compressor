

#include <stddef.h>

/* RGB to YCbCr: converts the pixels' RGB format into TYCbCr using ITU-R BT.601 standard.
* returns planar arrays of Y, Cb and Cr parmateres:
|--...-|------...-|-------|
YYYY...CbCbCbCb...CrCrCrCr 
*/
void rgb_to_ycbcr(unsigned char *rgb, size_t rgb_size, unsigned char *y, unsigned char *cb, unsigned char *cr){

/* conversion table:
    Y  = 0.299R + 0.587G + 0.114B
    Cb = 128 − 0.168736R − 0.331264G + 0.5B
    Cr = 128+0.5R−0.418688G−0.081312B
*/
    /* we can assume that rgb_size in not null beacuse we surely have a jpeg image in input */
    for (size_t i = 0, p = 0; i + 3 <= rgb_size; i += 3, p++){
        unsigned char R = rgb[i];
        unsigned char G = rgb[i+1];
        unsigned char B = rgb[i+2];
        y[p] =  (unsigned char)(0.299 * (float)R + 0.587 * (float)G + 0.114 * (float)B);
        cb[p] =  (unsigned char)(128 - 0.168736 * (float)R - 0.331264 * (float)G + 0.5 * (float)B);
        cr[p] =  (unsigned char)(128 + 0.5 * (float)R - 0.418688 * (float)G + 0.081312 * (float)B);
    }
    return;
}

/* subsampling function: reduces the crominance channels resolution by doing an average between 2x2 pixel blocks' Cb and Cr
*/
void subsampling(unsigned char *cb, unsigned char *cr, size_t width, size_t height){

    // scorre le righe e tralascia quelle dispari; bordi con width/height dispari restano intatti
    for (size_t row = 0; (row + 1) < height; row += 2) {
        for (size_t col = 0; col + 1 < width; col += 2) {

            // media dei cb e dei cr sul blocco 2x2
            size_t Cbm = (cb[row * width + col] + cb[row * width + col + 1]
                + cb[row * width + col + width] + cb[row * width + col + width + 1]) / 4;
            size_t Crm = (cr[row * width + col] + cr[row * width + col + 1]
                + cr[row * width + col + width] + cr[row * width + col + width + 1]) / 4;

            cb[row * width + col] = (unsigned char)Cbm;
            cb[row * width + col + 1] = (unsigned char)Cbm;
            cb[row * width + col + width] = (unsigned char)Cbm;
            cb[row * width + col + width + 1] = (unsigned char)Cbm;

            cr[row * width + col] = (unsigned char)Crm;
            cr[row * width + col + 1] = (unsigned char)Crm;
            cr[row * width + col + width] = (unsigned char)Crm;
            cr[row * width + col + width + 1] = (unsigned char)Crm;
        }
    }

    return;
}

/* upsampling funciton: reverts the subsampling one : fare poi */