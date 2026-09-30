#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include "../third-party/stb_image.h"
#include "../third-party/stb_image_write.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include "../include/colorspace.h"

/* helper for JPG file recognition: returns 1 if the file is a .JPG, else 0 */
int isJpg(const char *filepath){

    //opens the file in binary reading mode
    FILE *fd = fopen(filepath, "rb");
    if(!fd) return 0;

    //if there are not magic numbers 
    unsigned char buf[3];
    if(fread(buf, 1, 3, fd) != 3){
        fclose(fd);
        return 0;
    }

    //checks the magic numbers
    if (buf[0] == 0xFF && buf[1] == 0xD8 && buf[2] == 0xFF) {
        fclose(fd);
        return 1;
    }

    fclose(fd);
    return 0;
}

/* helper for raw_bytes_data cleanup */
void cleanup(void **data, ssize_t data_size){

    for (ssize_t i = 0; i < data_size; i++){
        stbi_image_free((unsigned char *)data[i]);
    }

    free(data);
    return;
}



int main(void){

    /* implementing the stp API, in order to load in memory an array of raw byte from a PNG o JPG image */
    int x,y,n, data_size = 3;  
    
    /* list of JPGs' arrays of raw bytes */
    unsigned char **raw_bytes_datas = malloc(data_size * sizeof(unsigned char *));

    if(!raw_bytes_datas){
        printf("malloc() for raw_bytes_data failed\n");
        return 1;
    }

    /* iterates across the dataset dir looking for every file compressable */
    struct dirent *de;                  /* entry of the dir */
    DIR* dr = opendir("./dataset");     /* a pointer to the dir */

    /* opedir failure */
    if(!dr){
        printf("opendir failed\n");
        free(raw_bytes_datas);
        return 1;
    }

    /* dir is empty */
    if(!readdir(dr)){
        printf("Error: /dataset is empty\n");
        free(raw_bytes_datas);
        return 1;
    }

    int count = 0;

    /* iterates across every file tha is JPG format */
    while ((de = readdir(dr)) != NULL) {
        /* checks if is . or .. special dir */
        if(strcmp(de->d_name, ".") == 0|| strcmp(de->d_name, "..") == 0) continue;

        /* building the complete filepath */
        char filepath[1024];
        snprintf(filepath, sizeof(filepath), "./dataset/%s", de->d_name);
        
        /* checks if is a JPG */
        if(isJpg(filepath)){
            /* fill the lists of loaded raw datas */
            if(data_size == (count + 1)){
                data_size += 3;
                /* checks the realloc() function */
                unsigned char **temp = realloc(raw_bytes_datas, (size_t)data_size * sizeof(unsigned char*));
                if(!temp){
                    printf("Realloc of raw_bytes_data failed\n");
                    cleanup((void**)raw_bytes_datas, count);
                    return 1;
                }
                raw_bytes_datas = temp;
            }

            raw_bytes_datas[count] = stbi_load((char *)filepath, &x, &y, &n, 3);

            /* checks the malloc() function in stb_load*/
            if(!raw_bytes_datas[count]){
                printf("stb_load() failed: %s\n", stbi_failure_reason());
                cleanup((void **)raw_bytes_datas, count);
                return 1;
            }
            count++;
        }
    }

    if (count > 0) {
        size_t pixels = (size_t)x * (size_t)y;
        unsigned char *Y = malloc(pixels);
        unsigned char *Cb = malloc(pixels);
        unsigned char *Cr = malloc(pixels);

        if (!Y || !Cb || !Cr) {
            printf("malloc() for Y/Cb/Cr planes failed\n");
            free(Y);
            free(Cb);
            free(Cr);
            cleanup((void **)raw_bytes_datas, count);
            closedir(dr);
            return 1;
        }

        for (int i = 0; i < count; i++) {
            rgb_to_ycbcr(raw_bytes_datas[i], pixels * 3, Y, Cb, Cr);
            subsampling(Cb, Cr, (size_t)x, (size_t)y);
        }

        free(Y);
        free(Cb);
        free(Cr);
    }

    printf("Loaded %d JPG image(s) from ./dataset\n", count);

    closedir(dr);
    cleanup((void **)raw_bytes_datas, count);

    return 0;
}