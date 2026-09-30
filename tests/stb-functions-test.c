/* 
this file contains some smoke tests for the stb functions which are imported from /third-part directory, but basically shows how theese functions work and what they are  
*/

/* libraries */
#include "../third-party/stb_image.h"
#include "../third-party/stb_image_write.h"




#include <stdio.h>
int main(void){

    /* mplementing the stp API, in order to load in memory an array of raw byte from a PNG o JPG image */
    int x,y,n;  
    const char *file = "./dataset/tognarelli.JPG";
    unsigned char *data = stbi_load(file, &x, &y, &n, 3);

    /* check for the malloc() allocation */
    if(!data){
        printf("sttbi_load() fallita: %s\n", stbi_failure_reason());
        return 1;
    }

    //x and y values
    printf("%d %d\n", x, y);

    //print the entire HEXA matrix about the image
    printf("Stampo a schermo il buffer raw per testare\n");
    for(long i = 0; i < x * y * 3; i++){
        printf("%02x\t", data[i]);
    }

    /* this function creates a JPG file decoding the buffer previously created */
    const char* file_decoded = "tognarelli_decoded.JPG";
    int jpg = stbi_write_jpg(file_decoded, x, y, 3, data, 90);
    
    if(jpg == 0){
        printf("Errore nella generazione del jpg dal buffer esadecimale\n");
        stbi_image_free(data);
        return 1;
    }

    printf("The JPG image was decoded successfully in the /root of the project\n");
    
    /* cleanup */
    stbi_image_free(data);
    printf("All the smoke tests passed for stb-functions\n");
    return 0;
}