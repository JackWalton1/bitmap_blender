#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned short WORD; 
typedef unsigned int DWORD; 
typedef unsigned int LONG; 
typedef unsigned char BYTE;
//typedef uint8_t  BYTE;

typedef struct tagBITMAPFILEHEADER 
 { 
 WORD bfType;  //specifies the file type 
 DWORD bfSize;  //specifies the size in bytes of the bitmap file 
 WORD bfReserved1;  //reserved; must be 0 
 WORD bfReserved2;  //reserved; must be 0 
 DWORD bfOffBits;  //species the offset in bytes from the bitmapfileheader to the bitmap bits 
 }BITMAPFILEHEADER; 

typedef struct tagBITMAPINFOHEADER 
 { 
 DWORD biSize;  //specifies the number of bytes required by the struct 
 LONG biWidth;  //specifies width in pixels 
 LONG biHeight;  //species height in pixels 
 WORD biPlanes; //specifies the number of color planes, must be 1 
 WORD biBitCount; //specifies the number of bit per pixel 
 DWORD biCompression;//spcifies the type of compression 
 DWORD biSizeImage;  //size of image in bytes 
 LONG biXPelsPerMeter;  //number of pixels per meter in x axis 
 LONG biYPelsPerMeter;  //number of pixels per meter in y axis 
 DWORD biClrUsed;  //number of colors used by th ebitmap 
 DWORD biClrImportant;  //number of colors that are important 
 }BITMAPINFOHEADER; 

typedef struct RGBTriple{
    BYTE blue;
    BYTE green;
    BYTE red;
}RGBTriple;

void printManPage();
//TripleNode *tnCreate();
//int pushPixels(TripleNode* tn, RGBTriple triple);
int read_bmp(FILE* fPtr, BITMAPFILEHEADER* bf, BITMAPINFOHEADER* bi, BYTE* bytearray);
int write_bmp(FILE* outPtr, FILE* inPtr, BITMAPFILEHEADER* bf, BITMAPINFOHEADER* bi);
int write_bmp_mix_pixels(float ratio, BITMAPFILEHEADER *bf1, BITMAPINFOHEADER *bi1, FILE* outPtr, BYTE* bytearray1, BYTE* bytearray2);
// make struct that is 2D array of pixels (size is height*width)
// then can make function get_pixel(col, row) that gets pixel in arr
// int argc, char *argv[]
int main(int argc, char *argv[]){
    if(argc!=5){
        printManPage();
        return 1;}

    char *input = argv[1];
    FILE *inPtr1 = fopen(input, "rb");
    if (inPtr1 == NULL){
        fprintf(stderr, "Could not open %s.\n", input);
        printManPage();
        return 1;
    }
    BITMAPFILEHEADER *bf1 = (BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER *));
    BITMAPINFOHEADER *bi1 = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER *));
    BYTE* bytearray1 = (BYTE *)malloc(bf1->bfSize); //biSize tells you how many bytes are in image
    
    read_bmp(inPtr1, bf1, bi1, bytearray1);
//BYTE color = data[x*3+y*width+i] where i give blue green red
    char *input2 = argv[2];
    FILE *inPtr2 = fopen(input2, "rb");
    if (inPtr2 == NULL){
        fprintf(stderr, "Could not open %s.\n", input);
        printManPage();
        free(bf1);
        free(bi1);
        free(bytearray1);
        return 1;
    }
    BITMAPFILEHEADER *bf2 = (BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER *));
    BITMAPINFOHEADER *bi2 = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER *));
    BYTE* bytearray2 = (BYTE *)malloc(bf1->bfSize);

    read_bmp(inPtr2, bf2, bi2, bytearray2);

    float ratio = atof(argv[3]);

    char *output = argv[4];
    FILE *outPtr = fopen(output, "wb");
    if (outPtr == NULL){
        fprintf(stderr, "Could not open %s.\n", output);
        printManPage();
        free(bf1);
        free(bi1);
        free(bf2);
        free(bi2);
        free(bytearray1);
        free(bytearray2);

        return 1;
    }
    //write_bmp(outPtr, inPtr, bf1, bi1);

    write_bmp_mix_pixels(ratio, bf1, bi1, outPtr, bytearray1, bytearray2);
    free(bf1);
    free(bi1);
    free(bf2);
    free(bi2);
    //free(bytearray1);
    //free(bytearray2);
    
    return 0;
}
/*
TripleNode *tnCreate(){
    TripleNode *pixel_array = (TripleNode *)malloc(sizeof(TripleNode *));
    pixel_array->data = NULL;
    pixel_array->next = NULL;
    return pixel_array;
}
*/
void printManPage(){
    printf("NAME\n     blendimages - a program to blend bmp images\nSYNOPSIS\n     program [bmpFile1] [bmpFile2] [ratio(0-1)] [bmpOutFile]\nEXAMPLE\n     ./blendimages wolf.bmp lion.bmp 0.5 lionWolf.bmp\n");
    return;
}
int write_bmp_interpolate(float ratio, BITMAPFILEHEADER *bf1, BITMAPINFOHEADER *bi1, BITMAPFILEHEADER *bf2, BITMAPINFOHEADER *bi2, FILE* outPtr, FILE* inPtr1, FILE* inPtr2){
    //use size of bigger file, so compare heights/widths
    return 0;
}
int write_bmp_mix_pixels(float ratio, BITMAPFILEHEADER *bf1, BITMAPINFOHEADER *bi1, FILE* outPtr, BYTE* bytearray1, BYTE* bytearray2){
    fwrite(&(bf1->bfType), sizeof(WORD), 1, outPtr);
    fwrite(&(bf1->bfSize), sizeof(DWORD), 1, outPtr);
    fwrite(&(bf1->bfReserved1), sizeof(WORD), 1, outPtr);
    fwrite(&(bf1->bfReserved2), sizeof(WORD), 1, outPtr);
    fwrite(&(bf1->bfOffBits), sizeof(DWORD), 1, outPtr);

    fwrite(&(bi1->biSize), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi1->biWidth), sizeof(LONG), 1, outPtr);
    fwrite(&(bi1->biHeight), sizeof(LONG), 1, outPtr);
    fwrite(&(bi1->biPlanes), sizeof(WORD), 1 , outPtr);
    fwrite(&(bi1->biBitCount), sizeof(WORD), 1, outPtr);
    fwrite(&(bi1->biCompression), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi1->biSizeImage), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi1->biXPelsPerMeter), sizeof(LONG), 1, outPtr);
    fwrite(&(bi1->biYPelsPerMeter), sizeof(LONG), 1, outPtr);
    fwrite(&(bi1->biClrUsed), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi1->biClrImportant), sizeof(DWORD), 1, outPtr);
    int padding = (4 - (bi1->biWidth * sizeof(RGBTriple)) % 4) % 4;

    int i, j, k, biHeight;
    for(i = 0, biHeight = abs(bi1->biHeight); i < biHeight; i+=1)
    {
        for(j = 0; j < bi1->biWidth; j+=1)
        {
            BYTE blue1 = bytearray1[j*3+i*bi1->biWidth];
            BYTE blue2 = bytearray2[j*3+i*bi1->biWidth];
            BYTE result_blue = blue1*ratio + blue2*(1-ratio);
            fwrite(&result_blue, sizeof(BYTE), 1, outPtr);

            BYTE grn1 = bytearray1[j*3+i*bi1->biWidth +1];
            BYTE grn2 = bytearray2[j*3+i*bi1->biWidth +1];
            BYTE result_grn = grn1*ratio + grn2*(1-ratio);
            fwrite(&result_grn, sizeof(BYTE), 1, outPtr);

            BYTE red1 = bytearray1[j*3+i*bi1->biWidth+2];
            BYTE red2 = bytearray2[j*3+i*bi1->biWidth+2];
            BYTE result_red = red1*ratio + red2*(1-ratio);
            fwrite(&result_red, sizeof(BYTE), 1, outPtr);
        }
        for(k = 0; k < padding; k++)
        {
        fputc(0x00, outPtr);
        }
        
    }
    return 0;
    //red_result = red_image1 * ratio + red_image2 * (1-ratio); 
    
    //BITMAPFILEHEADER *bfout = (BITMAPFILEHEADER *)malloc(sizeof(BITMAPFILEHEADER));
    //BITMAPINFOHEADER *biout = (BITMAPINFOHEADER *)malloc(sizeof(BITMAPINFOHEADER));
}

int read_bmp(FILE* inPtr, BITMAPFILEHEADER* bf, BITMAPINFOHEADER* bi, BYTE* bytearray){

    // bf
    fread(&(bf->bfType), sizeof(WORD), 1, inPtr);
    fread(&(bf->bfSize), sizeof(DWORD), 1, inPtr);
    fread(&(bf->bfReserved1), sizeof(WORD), 1, inPtr);
    fread(&(bf->bfReserved2), sizeof(WORD), 1, inPtr);
    fread(&(bf->bfOffBits), sizeof(DWORD), 1, inPtr);
    // bi
    fread(&(bi->biSize), sizeof(DWORD), 1, inPtr);
    fread(&(bi->biWidth), sizeof(LONG), 1, inPtr);
    fread(&(bi->biHeight), sizeof(LONG), 1, inPtr);
    fread(&(bi->biPlanes), sizeof(WORD), 1, inPtr);
    fread(&(bi->biBitCount), sizeof(WORD), 1, inPtr);
    fread(&(bi->biCompression), sizeof(DWORD), 1, inPtr);
    fread(&(bi->biSizeImage), sizeof(DWORD), 1, inPtr);
    fread(&(bi->biXPelsPerMeter), sizeof(LONG), 1, inPtr);
    fread(&(bi->biYPelsPerMeter), sizeof(LONG), 1, inPtr);
    fread(&(bi->biClrUsed), sizeof(DWORD), 1, inPtr);
    fread(&(bi->biClrImportant), sizeof(DWORD), 1, inPtr);
    
    fread(bytearray, bf->bfSize, 1, inPtr);
    
    return 0;
    
}
/*
int pushPixels(TripleNode* tn, RGBTriple triple){
    TripleNode *insert_tn = tnCreate();
    (insert_tn->data) = &triple;
    TripleNode* temp_tn = tn;
    //checks if empty
    if(tn->data == NULL){
        tn->data = insert_tn->data;
        free(insert_tn);
        return 0;
    }
    //if next is not the node for insert, go to next node
    while (temp_tn->next != NULL){
        temp_tn = temp_tn->next;
    }
    //if next is node for insert, insert at next node
    if(temp_tn->next == NULL){
        temp_tn->next = insert_tn;
        //free(insert_le);
        return 0;
    }
    //if func fails to insert
    free(insert_tn);
    return 1;
} 
*/
int write_bmp(FILE* outPtr, FILE* inPtr, BITMAPFILEHEADER* bf, BITMAPINFOHEADER* bi){
    fwrite(&(bf->bfType), sizeof(WORD), 1, outPtr);
    fwrite(&(bf->bfSize), sizeof(DWORD), 1, outPtr);
    fwrite(&(bf->bfReserved1), sizeof(WORD), 1, outPtr);
    fwrite(&(bf->bfReserved2), sizeof(WORD), 1, outPtr);
    fwrite(&(bf->bfOffBits), sizeof(DWORD), 1, outPtr);

    fwrite(&(bi->biSize), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi->biWidth), sizeof(LONG), 1, outPtr);
    fwrite(&(bi->biHeight), sizeof(LONG), 1, outPtr);
    fwrite(&(bi->biPlanes), sizeof(WORD), 1 , outPtr);
    fwrite(&(bi->biBitCount), sizeof(WORD), 1, outPtr);
    fwrite(&(bi->biCompression), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi->biSizeImage), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi->biXPelsPerMeter), sizeof(LONG), 1, outPtr);
    fwrite(&(bi->biYPelsPerMeter), sizeof(LONG), 1, outPtr);
    fwrite(&(bi->biClrUsed), sizeof(DWORD), 1, outPtr);
    fwrite(&(bi->biClrImportant), sizeof(DWORD), 1, outPtr);
    int padding = (4 - (bi->biWidth * sizeof(RGBTriple)) % 4) % 4;

    int i, j, k, biHeight;

    for(i = 0, biHeight = abs(bi->biHeight); i < biHeight; i++)
    {
        for(j = 0; j < bi->biWidth; j++)
        {
        RGBTriple triple;
        fread(&triple, sizeof(RGBTriple), 1, inPtr);        
        fwrite(&(triple.blue), sizeof(BYTE), 1, outPtr);
        fwrite(&(triple.green), sizeof(BYTE), 1, outPtr);
        fwrite(&(triple.red), sizeof(BYTE), 1, outPtr);
        
        }
        for(k = 0; k < padding; k++)
        {
        fputc(0x00, outPtr);
        }
        fseek(inPtr, padding, SEEK_CUR);
    }
    return 0;
}