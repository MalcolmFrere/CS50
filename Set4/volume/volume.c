// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Constante allocation
#define ERROR_OPEN_FILE 1
#define ERROR_ARGUMENTS 1
#define ERROR_MEMORY 10
#define ERROR_READ 20
#define ERROR_WRITE 30
#define ERROR_COPY_HEADER 40
#define ERROR_COPY_DATA 50

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int copy_header(FILE *input, FILE *output);
int update_volume(FILE *input, FILE *output, float factor);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return ERROR_ARGUMENTS;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return ERROR_OPEN_FILE;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return ERROR_OPEN_FILE;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file
    if (copy_header(input, output) != 0)
    {
        printf("Error during copy of header file.\n");
        return ERROR_COPY_HEADER;
    }

    // TODO: Read samples from input file and write updated data to output file
    if (update_volume(input, output, factor) != 0)
    {
        printf("Error during volume update.\n");
        return ERROR_COPY_DATA;
    }

    // Close files
    fclose(input);
    fclose(output);
}


int copy_header(FILE *input, FILE *output)
{
    // Allocate sufficient memory to save header of WAV file
    uint8_t *header = malloc(HEADER_SIZE * sizeof(uint8_t));
    if (header == NULL)
    {
        printf("Error to allocate memory");
        return ERROR_MEMORY;
    }

    // Read header of input WAV file
    if (fread(header, sizeof(uint8_t), HEADER_SIZE, input) != HEADER_SIZE)
    {
        printf("Error to read header data.\n");
        return ERROR_READ;
    }

    // Write header of output WAV file
    if (fwrite(header, sizeof(uint8_t), HEADER_SIZE, output) != HEADER_SIZE)
    {
        printf("Error to write header data.\n");
        return ERROR_WRITE;
    }

    free(header);

    return 0;
}

int update_volume(FILE *input, FILE *output, float factor)
{

    int16_t buffer = 0x0000;

    do
    {
        // Read input WAV file data and store it in buffer of 16 bits
        if (fread(&buffer, sizeof(uint16_t), 1, input) != 1)
        {
            // End of file reached
            break;
        }

        // Apply factor on buffer
        buffer = (int16_t)((float)(buffer) * factor);

        // Write buffer with factor in output WAV file
        if (fwrite(&buffer, sizeof(uint16_t), 1, output) != 1)
        {
            printf("Error to retreive header data.\n");
            return ERROR_WRITE;
        }
    }
    while (1);

    return 0;
}