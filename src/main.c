#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third-party/stb_image.h"
#include "../third-party/stb_image_write.h"
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

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

int main(void){

    /* implementing the stp API, in order to load in memory an array of raw byte from a PNG o JPG image */
    int x,y,n;  
    const char *file = "./dataset/tognarelli.JPG";
    unsigned char *data = stbi_load(file, &x, &y, &n, 3);

    /* check for the malloc() allocation */
    if(!data){
        printf("sttbi_load() fallita: %s\n", stbi_failure_reason());
        return 1;
    }



    /* list of JPGs' arrays of raw bytes */
    unsigned char **raw_bytes_datas = malloc(sizeof(unsigned char *));

    if(!(*raw_bytes_datas)){
        printf("malloc() for raw_bytes_data failed\n");
        stbi_image_free(data);
        return 1;
    }

    /* iterates across the dataset dir looking for every file compressable */
    struct dirent *de;                  /* entry of the dir */
    DIR* dr = opendir("./dataset");     /* a pointer to the dir */

    /* opedir failure */
    if(!dr){
        printf("opendir failed\n");
        stbi_image_free(data);
        return 1;
    }

    /* dir is empty */
    if(!readdir(dr)){
        printf("Error: /dataset is empty\n");
        stbi_image_free(data);
        return 1;
    }


    /* iterates across every file tha is JPG format */
    while ((de = readdir(dr)) != NULL) {
        /* checks if is . or .. special dir */
        if(strcmp(de->d_name, ".") || strcmp(de->d_name, "..")) continue;

        /* check if is a dir */
        /* checks if is a JPG */
        if(isJpg(de->d_name)){
            /* fill the lists of loaded raw datas */
            /* qui praticamente ogni volta deve riallocare una cella dato che la lista si riempie sempre, quindi si realloca direttamente aggiungendo una cella, della quale si tiene conto mediante una variabile contatore che ci si porta appresso da fuori dal while. successivamente si converte il jpg con stb_load e si inserisce nella lista in fondo*/
        }
    }
    
    /* this function creates a JPG file decoding the buffer previously created */
    const char* file_decoded = "tognarelli_decoded.JPG";
    int jpg = stbi_write_jpg(file_decoded, x, y, n, data, 90);

    if(jpg == 0){
        printf("Errore nella generazione del jpg dal buffer esadecimale\n");
        stbi_image_free(data);
        return 1;
    }
    
    /* cleanup */
    stbi_image_free(data);


    return 0;
}