#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    uint8_t *data; // Array di byte che rappresenta la bitmap
    int data_size; // Dimensione dell'array in bytes
    int bits;      // Numero totale di bit nella bitmap
} Bitmap;

// Inizializza la bitmap con tutti i bit a 0 (liberi)
void Bitmap_init(Bitmap *map, uint8_t *data, int bits);

// Imposta il bit specificato nella bitmap a 1 (occupato)
void Bitmap_setBit(Bitmap *bitmap, int bit);

// Imposta il bit specificato nella bitmap a 0 (libero)
void clearBit(Bitmap *bitmap, int bit);

// Verifica lo stato del bit specificato
int testBit(Bitmap *bitmap, int bit);