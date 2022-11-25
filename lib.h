//By Othman RAHAL and Samir TASTIFT     Man-tutored project by M.Romain DUBESSY        Engineering school : Institut SupGalilée
#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#define MAX_LENGTH 99999
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60
typedef struct _NEURON
{
    float* weights;
    float bias;
    float* d_weights;
    float d_bias;
    float delta;
}NEURON;

typedef struct _LAYER
{
    int nb_in;
    float* in;
    float* out;
    int nb_out;
    NEURON* tab_neuron;
    int nb_neuron;
    struct _LAYER* prec;
    struct _LAYER* next;
}LAYER;

typedef struct _NETWORK
{
    LAYER* first;
    LAYER* last;
    float* inputs;
    int nb_layers;
    int nb_inputs;
    int* nb_neuron_per_layer;
    float learning_rate;
}NETWORK;


typedef struct _DATA_DIGIT
{
    int digit_expected;
    unsigned char* pixels;
}DATA_DIGIT;

typedef struct _DATASET
{
    int nb_digit;
    int rows;
    int columns;
    int size_pixels;
    DATA_DIGIT* digits;
}DATASET;
//--------------------------------------------------
/**
 *
 * @brief   Display a progress bar
 *
 * @param   double percentage 
 *
 * @return  void
 *
 **/
void printProgress(double percentage);
//--------------------------------------------------
/**
 *
 * @brief   Empty the buffer
 *
 * @param   void 
 * 
 * @return  void
 * 
 **/
void emptyBuffer(void);
//--------------------------------------------------
/**
 *
 * @brief   Clear terminal
 *
 * @param   void
 *
 * @return  void
 *
 **/
void clearTerminal(void);
//--------------------------------------------------
/**
 *
 * @brief   Wait n secondes
 *
 * @param   int seconde
 *
 * @return  void
 *
 **/
void timer_secondes(int seconde);
//---------------------------------------------------
/**
 *
 * @brief   Decompress and extract database
 *
 * @param   char* filename                      (Filename from decompressing)
 * @param   FILE* database                      (Database loaded)
 * 
 * @return  DATASET decompress_extract_img      (Database decompressed and extracted)
 *
 **/
DATASET decompress_extract_img(char* filename, FILE* file);
//---------------------------------------------------
/**
 *
 * @brief   Free the database
 *
 * @param   DATASET database                    (Database loaded)
 * 
 * @return  void
 *
 **/
void free_database(DATASET database);
//---------------------------------------------------
/**
 *
 * @brief   Display the database
 *
 * @param   DATASET database                    (Database loaded)
 * 
 * @return  void
 *
 **/
void print_database(DATASET database);
//---------------------------------------------------
/**
 *
 * @brief   Save digit of database in .bmp
 *
 * @param   unsigned char data[]                (Digit of database)
 * @param   int rows                            (Size of rows)
 * @param   int cols                            (Size of cols)
 * @param   const char*                         (Filename)
 * 
 * @return  NETWORK* new_network                (Pointer of network's address)
 * 
 **/
void saveBMP(unsigned char data[], int rows, int cols, const char* filename);
//---------------------------------------------------
/**
 *
 * @brief  Create a new network
 *
 * @param   DATASET database                    (Database loaded needeed for network's creation)
 * 
 * @return  NETWORK* new_network                (Pointer of network's address)
 * 
 **/
NETWORK* new_network(DATASET data);
//---------------------------------------------------
/**
 *
 * @brief  Create a new network
 *
 * @param   int nb_inputs                       (Inputs' network)
 * @param   float* tab_inputs                   (Inputs' table of network)
 * @param   int nb_layers                       (Inputs' table of network)
 * @param   int* tab_neuron_per_layer           (Table of neuron per layer)
 * 
 * @return  NETWORK* create_network             (pointer of network's address)
 **/
NETWORK* create_network(int nb_inputs, float* tab_inputs, int nb_layers, int* tab_neuron_per_layer);
//---------------------------------------------------
/**
 *
 * @brief  Save a new network
 *
 * @param   network* network                    (Pointer of network's address)
 * @param   float* tab_inputs                   (Inputs' table of network)
 * @param   int nb_layers                       (Inputs' table of network)
 * @param   int* tab_neuron_per_layer           (Table of neuron per layer)
 * @param   char* filename                      (Filename from  saving)
 *
 * @return  void
 * 
 **/
void saveNETWORK(NETWORK* network, int nb_inputs, int nb_layers, int* nb_neuron_per_layer, char* filename);
//---------------------------------------------------
/**
 *
 * @brief   Load a network from file.csv 
 *
 * @param   char* filename                      (Filename for loading)
 *
 * @return  NETWORK* loadNETWORK                (Network loaded)
 *
 **/
NETWORK* loadNETWORK(char* filename);
//---------------------------------------------------
/**
 *
 * @brief   Create new neurons for parameter layer 
 *
 * @param   LAYER* layer                        (Pointer of layer for neurons' creation)
 * @param   LAYER* index_layer                  (Index of layer)
 *
 * @return  int   return counter of neurons for progress bar
 *
 **/
void new_neurons(LAYER* layer, int index_layer);
//---------------------------------------------------
/**
 *
 * @brief   Add new layer in last layer of network
 *
 * @param   NETWORK* network                    (Pointer of network)
 * @param   int nb_neuron_of_new_layer          (Number of neuron for the new layer)
 * @param   int index_layers                    (Index of the layer)
 *
 * @return  void
 *
 **/
void add_layer(NETWORK* network, int nb_neuron_of_new_layer, int index_layer);
//---------------------------------------------------
/**
 *
 * @brief   Display the network
 *
 * @param   NETWORK* network                    (Pointer of network)
 *
 * @return  void
 *
 **/
void print_network(NETWORK* network);
//---------------------------------------------------
/**
 *
 * @brief   Free the network
 *
 * @param   NETWORK* network                    (Pointer of network)
 *
 * @return  void
 *
 **/
void free_network(NETWORK* network);
//---------------------------------------------------
/**
 *
 * @brief   Random float between A and B
 *
 * @param   float A
 * @param   float B
 *
 * @return  random float
 *
 **/
float random_float(float A, float B);
//---------------------------------------------------
/**
 *
 * @brief   Calculation of the neuron output of the layer
 *
 * @param   LAYER*                              (Pointer of the layer for calculation)
 * @param   int index                           (Index of the layer for calculation)
 *
 * @return  float output_layer                  (Output calculated)
 *
 **/
float output_layer(LAYER* layer, int index);
//---------------------------------------------------
/**
 *
 * @brief   Generate random weights and bias for network
 *
 * @param   NETWORK* network                    (Pointer of network)
 *
 * @return  NETWORK* random_network             (Network with random weights and bias)
 *
 **/
NETWORK* random_network(NETWORK* network);
//---------------------------------------------------
/**
 *
 * @brief   Network training 
 *
 * @param   NETWORK* network                    (Pointer of network for training)
 * @param   DATASET database                    (Pointer of database for training)
 * @param   int nb_img_per_subgroup             (Number of images per subgroup)
 * @param   int nb_epoch                        (Number of epoch for training)
 *
 * @return void
 *
 **/
void training_network(NETWORK* network, DATASET database, int nb_img_per_subgroup, int nb_epoch);
//---------------------------------------------------
/**
 *
 * @brief   Mix the order of database
 *
 * @param   DATASET data                        (Database)
 * @param   int index_first                     (First index of digits' table)
 * @param   int index_last                      (Last index of digits' table)
 *
 * @return void
 *
 **/
void mixDatabase(DATASET database, int index_first, int index_last);
//---------------------------------------------------
/**
 *
 * @brief   Feedforward function to calculate output for outputs' layer from neuron inputs
 *      
 * @param   NETWORK* network                    (Pointer of network)
 * @param   unsigned char* pixels               (Pointer of pixels' digit) 
 *
 * @return void
 *
 **/
void feedForward(NETWORK* network, unsigned char* pixels);
//---------------------------------------------------
/**
 *
 * @brief   Random int between a and b
 *
 * @param   int a
 * @param   int b
 *
 * @return  void
 *
 **/
int rand_int(int a, int b);
//---------------------------------------------------
/**
 *
 * @brief   Calculation of the scalar product between vectA and vectB
 *
 * @param   float* vectA                        (Pointer of vectA)
 * @param   float* vectB                        (Pointer of vectB)
 * @param   int size_vect                       (Size of vect)
 *
 * @return  float   scalarProduct               (Result of scalar product between vectA and vectB)    
 *
 **/
float scalarProduct(float* vectA, float* vectB, int size_vect);
//---------------------------------------------------
/**
 *
 * @brief   Sigmoid function ---> f(x) = 1 / (1 + exp(- x))
 *
 * @param   float output_neuron                 (Output of neuron --> x for inject in sigmoid function f(x) )
 *
 * @return  float   sigmoid                     (f(output_neuron) = 1 / (1 + exp(- output_neuron)))
 *
 **/
float sigmoid(float output_neuron);
//---------------------------------------------------
/**
 *
 * @brief   BackPropagation function consists in updating the weights of each neuron from the last layer to the first
 *
 * @param   NETWORK* network                    (Pointer of network)
 * @param   int digit_expected                  (Digit expected)
 *
 * @return  void
 *
 **/
void backPropagation(NETWORK* network, int digit_expected);
//---------------------------------------------------
/**
 *
 * @brief   Gradient Descent algorithm in supervised learning problems to minimize the cost function
 * 
 * @param   NETWORK* network                    (Pointer of network)
 * @param   int nb_ing_per_susgroup             (Number of images per subgroup)
 *
 * @return  void
 *
 **/
void gradientDescent(NETWORK* network, int nb_img_per_subgroup);
//---------------------------------------------------
/**
 *
 * @brief   Dl
 * 
 * @param   NETWORK* network                    (Pointer of network)
 *
 * @return  void
 *
 **/
void dl(NETWORK* network);
//---------------------------------------------------
/**
 *
 * @brief   DL
 * 
 * @param   NETWORK* network                    (Pointer of network)
 * @param   int digit_expected                  (Digit expected)
 *
 * @return  void
 *
 **/
void dL(NETWORK* network, int digit_expected);
//---------------------------------------------------
/**
 *
 * @brief   Testing the trained network with new digits
 * 
 * @param   NETWORK* network                    (Pointer of network)
 * @param   DATASET database                    (Database)
 *
 * @return  void
 *
 **/
float testNetwork(NETWORK* network, DATASET database);
//---------------------------------------------------
/**
 *
 * @brief   Find index of output maximun of outputs' table
 * 
 * @param   float* outputs                      (Pointer of outputs's table)
 * @param   int nb_out                          (Number of outputs)
 *
 * @return  float   succcess rate               (Success rate)
 *
 **/
int find_OutputMax(float* outputs, int nb_out);
//---------------------------------------------------
/**
 *
 * @brief   Display digit for testing
 *
 * @param   unsigned char* pixels               (Pixels of digits)
 * @param   int nb_pixels                       (Number of pixels)
 *
 * @return  void
 *
 **/
void print_digit_expected(unsigned char* pixels, int nb_pixels);
//---------------------------------------------------
/**
 *
 * @brief   Testing the network with random digits of database
 *
 * @param   NETWORK* network                    (Pointer of network)
 * @param   DATASET database                    (Database)
 *
 * @return  void
 *
 **/
void testNETWORK_digit(NETWORK* network, DATASET database);
//---------------------------------------------------
/**
 *
 * @brief   Display menu
 *
 * @param   int DATASET_LOADED  (TRUE OR FALSE)
 * @param   int NETWORK_LOADED  (TRUE OR FALSE)
 * @param   int NETWORK_TRAINED (TRUE OR FALSE)
 *
 * @return  void
 *
 **/
void printMenu(int DATASET_LOADED, int NETWORK_LOADED, int NETWORK_TRAINED);