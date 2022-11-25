//By Othman RAHAL and Samir TASTIFT     Man-tutored project by M.Romain DUBESSY        Engineering school : Institut SupGalilée
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "lib.h"
#include <errno.h>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
        -> source of progress bar https://stackoverflow.com/questions/14539867/how-to-display-a-progress-indicator-in-pure-c-c-cout-printf
*/
#define PBSTR "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||"
#define PBWIDTH 60

void printProgress(double percentage)
{
    int val = (int)(percentage * 100);
    int lpad = (int)(percentage * PBWIDTH);
    int rpad = PBWIDTH - lpad;
    printf("\r%3d%% [%.*s%*s]", val, lpad, PBSTR, rpad, "");
    fflush(stdout);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void emptyBuffer(void)
{
    int c;
    while ((c = getchar()) != EOF && c != '\n');
}

void clearTerminal(void)
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif
}

void timer_secondes(int seconde)
{

    int current_time_init = (int)time(NULL);

    int current_time = (int)time(NULL);

    while (current_time <= (current_time_init + seconde))

    {

        current_time = (int)time(NULL);

    }

}

DATASET decompress_extract_img(char* filename, FILE* file)
{
    if (file == NULL)
    {
        printf("ERROR %s: ! CAN'T OPEN FILE '%s-_-\n\n\n", __func__, filename);
        exit(EXIT_FAILURE);
    }
    float counter = 1.0;
    DATASET database;
    database.nb_digit = database.rows =  database.columns  = database.size_pixels = 0;
    database.digits = NULL;
    printf("\t\t--->Decompressing and extracting the filename '%s' : \n\t\t",filename);
    char* text[MAX_LENGTH];
    int k = 0;
    fgets(text, MAX_LENGTH, file);  // Retrieve the file line
    database.nb_digit = atoi(strtok(text, ";")); // 60 000 digits
    database.rows = atoi(strtok(NULL, ";")); // 28 rows
    database.columns = atoi(strtok(NULL, ";")); // 28 cloumns
    database.size_pixels = database.columns * database.rows; // 784 pixels
    database.digits = calloc(database.nb_digit, sizeof(DATA_DIGIT));
    if (database.digits == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    else
    {
        for (int ijk = 0; ijk < database.nb_digit; ijk++)
        {
            fgets(text, MAX_LENGTH, file);
            int digit = atoi(strtok(text, ";"));
            database.digits[ijk].digit_expected = digit;
            char imgName[20];
            sprintf(imgName, "%d_%d.bmp", digit, ijk + 1);
            char* tmp; 
            int index_pixel = 0;
            tmp = strtok(NULL, ";");
            database.digits[ijk].pixels = calloc(database.size_pixels, sizeof(unsigned char));
            if (database.digits[ijk].pixels == NULL)
            {
                fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
            }
            else
            {
                while (tmp != NULL)
                {

                    if (strstr(tmp, ",") != NULL)
                    {
                        int nb_digit_after_comma = atoi(tmp + 2);
                        for (size_t i = 0; i < nb_digit_after_comma; i++)
                        {
                            database.digits[ijk].pixels[index_pixel] = tmp[0] - 48;
                            index_pixel++;
                        }
                    }
                    else
                    {
                        database.digits[ijk].pixels[index_pixel] = (unsigned char)atoi(tmp);
                        index_pixel++;
                    }
                    tmp = strtok(NULL, ";");
                }
            }
            counter++;
            printProgress((counter) / database.nb_digit);
        }
        fclose(file);
        timer_secondes(1);
        printf("\x0d\033[K");   // Delete the line
        printf("\t\t-->Database decompressed and extracted !\n");
        return database;
    }
}

void free_database(DATASET database)
{
    for (size_t i = 0; i < database.nb_digit; i++)
    {
        free(database.digits[i].pixels);
    }
    free(database.digits);
}

void print_database(DATASET database)
{
    printf("\n.\n.\n.\nPrint of database ...\n");
    printf("NB OF DIGITS : %d\n", database.nb_digit);
    printf("NB OF ROWS : %d\n", database.rows);
    printf("NB OF COLUMNS : %d\n", database.columns);
    printf("NB OF SIZE OF DIGITS : %d\n.\n.\n.\n.\n.\n.\n.\n", database.size_pixels);
    for (int i = 0; i < database.nb_digit; i++)
    {
        printf("%dth DIGIT :\n", (i + 1));
        printf("DIGIT EXPECTED : %d\n", database.digits[i].digit_expected);
        for (int j = 0; j < database.size_pixels; j++)
        {
            if (database.digits[i].pixels[j] == 0)
            {
                printf(".");
                if ((j + 1) % 28 == 0)
                {
                    printf("\n");
                }
            }
            if (database.digits[i].pixels[j] != 0)
            {
                printf("0");
                if ((j + 1) % 28 == 0)
                {
                    printf("\n");
                }
            }  
        }
        printf("\n.\n.\n");
    }
}

void saveBMP(unsigned char data[], int rows, int cols, const char* filename)
{
    int width = rows, height = cols;
    int pad = (4 - (3 * width) % 4) % 4;
    int filesize = 54 + (3 * width + pad) * height;
    unsigned char header[54] = { 'B','M', 0,0,0,0, 0,0,0,0, 54,0,0,0, 40,0,0,0,
                                         0,0,0,0, 0,0,0,0, 1,0,24,0 };
    char padding[3] = { 0,0,0 };
    unsigned char* img;
    FILE* file;
    int  i;
    for (i = 0; i < 4; i++)
    {
        header[2 + i] = (unsigned char)((filesize >> (8 * i)) & 255);
        header[18 + i] = (unsigned char)((width >> (8 * i)) & 255);
        header[22 + i] = (unsigned char)((height >> (8 * i)) & 255);
    }
    img = malloc(sizeof(char) * 3 * (size_t)width * (size_t)height);
    if (img == NULL)
        printf("Error could not allocate memory !\n");
    else
    {
        for (i = 0; i < width * height; i++)
        {
            int color = data[i];
            img[3 * i + 2] = img[3 * i + 1] = img[3 * i] = (unsigned char)(color & 0xFF);
        }
        file = fopen(filename, "wb");
        if (file == NULL)
            printf("Error: could not open file '%s' !\n", filename);
        else
        {
            fwrite(header, 54, 1, file);
            for (i = height - 1; i >= 0; i--)
            {
                fwrite((char*)(img + 3 * width * i), 3 * (size_t)width, 1, file);
                fwrite(padding, (unsigned int)pad, 1, file);
            }
            fclose(file);
        }
        free(img);
    }
}

NETWORK* new_network(DATASET data)
{
    int nb_inputs = data.size_pixels;
    float* tab_inputs = calloc(data.size_pixels, sizeof(double));
    if (tab_inputs == NULL)
    { 
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    int nb_layers;
    printf("\t--->Numbers of layers of your network : ");
    if (scanf("%d", &nb_layers) != 1) {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    emptyBuffer();
    int* nb_neuron_per_layer = calloc(nb_layers, sizeof(int));
    if (nb_neuron_per_layer == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nb_layers; i++)
    {
        if(i == nb_layers-1)
        {
            while (*(nb_neuron_per_layer + i) != 10)
            {
                printf("\t---> Numbers of neurons of your %d layer (you need to put 10 neurones for output layer) : ", i + 1);
                if (scanf("%d", nb_neuron_per_layer + i) != 1)
                {
                fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
                }
                emptyBuffer();
            }
        }
        else
        {
            printf("\t---> Numbers of neurons of your %d layer : ", i + 1);
            if (scanf("%d", nb_neuron_per_layer + i) != 1)
            {
                fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
            }
            emptyBuffer();
        }
    }
    NETWORK* network = create_network(nb_inputs, tab_inputs, nb_layers, nb_neuron_per_layer);
    return network;

}

void new_neurons(LAYER* layer, int index_layer)
{
    if (layer == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    NEURON* neuron = calloc(layer->nb_neuron, sizeof(NEURON));
    if (neuron == NULL)
    {
        printf("Error of the table of neuron with the %d layer", index_layer + 1);
        exit(EXIT_FAILURE);
    }
    layer->tab_neuron = neuron;
    layer->nb_out = layer->nb_neuron;
    layer->out = calloc(layer->nb_out, sizeof(float));
    if (layer->out == NULL)
    {
        printf("Error of the table of outputs with the %d layer", index_layer + 1);
        exit(EXIT_FAILURE);
    }
    if (index_layer != 0)
    {   // Hidden layer (to give the inputs to layers)
        layer->nb_in = layer->prec->nb_out;
        layer->in = layer->prec->out;
    }
    for (int i = 0; i < layer->nb_neuron; i++)
    {
        layer->tab_neuron[i].weights = calloc(layer->nb_in, sizeof(float));
        if (layer->tab_neuron[i].weights == NULL)
        {
            printf("Error of the table of weights of the %dth layer with the %d neuron", index_layer + 1, i + 1);
            exit(EXIT_FAILURE);
        }
        //Ajout de sensibilite
        layer->tab_neuron[i].d_weights = calloc(layer->nb_in, sizeof(float));
        if (layer->tab_neuron[i].d_weights == NULL)
        {
            printf("ERROR: table of delta_neuron  of the %dth layer with the %d neuron", index_layer + 1, i + 1);
            exit(EXIT_FAILURE);
        }
    }
}

void add_layer(NETWORK* network, int nb_neuron_of_new_layer, int index_layer)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    LAYER* temp_layer = calloc(1, sizeof(LAYER));
    if (temp_layer == NULL)
    {
        printf("Error of the calloc with the %d layer", index_layer + 1);
        exit(EXIT_FAILURE);
    }
    network->last->next = temp_layer;
    temp_layer->prec = network->last;
    network->last = temp_layer;
    temp_layer->next = NULL;
    temp_layer->nb_neuron = nb_neuron_of_new_layer;
    new_neurons(temp_layer, index_layer);
}

NETWORK* create_network(int nb_inputs, float* tab_inputs, int nb_layers, int* tab_neuron_per_layer)
{
    ////////////////// creation of the neuron network /////////////////////////////////
    NETWORK* network = calloc(1, sizeof(NETWORK));
    if (network == NULL)
    {
        printf("Error of the calloc with the network");
        exit(EXIT_FAILURE);
    }
    network->nb_layers = nb_layers;
    network->nb_inputs = nb_inputs;
    network->inputs = tab_inputs;
    int index_layer = 0;
    // Input Layer
    LAYER* first_layer = calloc(1, sizeof(LAYER));
    if (first_layer == NULL)
    {
        printf("Error of the calloc with the %d layer", index_layer + 1);
        exit(EXIT_FAILURE);
    }
    first_layer->in = network->inputs;
    first_layer->nb_in = network->nb_inputs;
    network->first = first_layer;
    network->first->prec = NULL;
    network->last = first_layer;
    network->last->next = NULL;
    network->nb_neuron_per_layer = tab_neuron_per_layer;
    first_layer->nb_neuron = tab_neuron_per_layer[index_layer];
    new_neurons(first_layer, index_layer);
    index_layer++;

    while (index_layer != nb_layers)
    {
        //  Hidden Layer
        add_layer(network, tab_neuron_per_layer[index_layer], index_layer);
        index_layer++;
    } 
    network = random_network(network);
    return network;
}

void saveNETWORK(NETWORK* network, int nb_inputs, int nb_layers, int* nb_neuron_per_layer, char* filename)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    float counter = 1.0;
    int nb_neurons = 0;
    for (int i = 0; i < nb_inputs; i++)
    {
        nb_neurons += nb_neuron_per_layer[i];
    }
    LAYER* temp_layer = network->first;
    FILE* file = NULL;
    file = fopen(filename, "w");
    if (file == NULL)
    {
        printf("Error with creation of csv file for writing");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    while (temp_layer != NULL)
    {
        fprintf(file, "%d;%d\n", temp_layer->nb_in, nb_neuron_per_layer[index]); //Writing of number of inputs and layers
        for (int i = 0; i < temp_layer->nb_neuron; i++)
        {
            fprintf(file, "%f;", temp_layer->tab_neuron[i].bias);
            for (int j = 0; j < temp_layer->nb_in; j++)
            {
                fprintf(file, "%f", temp_layer->tab_neuron[i].weights[j]);//Writing of weights of neurons of the layer
                if (j < temp_layer->nb_in - 1)
                {
                    fprintf(file, ";");
                }
            }
            fprintf(file, "\n");
            printProgress(counter/nb_neurons);
            counter++;
        }
        index++;
        temp_layer = temp_layer->next;
    }
    fclose(file);
    printf("\n\r\t\t--->NETWORK SAVED SUCCESSFULLY ! ");
}

NETWORK* loadNETWORK(char* filename)
{
    FILE* file = fopen(filename, "r");
    if (file == NULL)
    { 
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    NETWORK* network = calloc(1, sizeof(NETWORK));
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    char* text[MAX_LENGTH];
    int index_layer = 0;
    int nb_neuron_of_new_layer = 0;
    int input = 0;
    LAYER* temp_layer = calloc(1, sizeof(LAYER));
    if (temp_layer == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    while (fgets(text, MAX_LENGTH, file) != NULL)
    {
        if (index_layer == 0)
        {
            network->first = temp_layer;
            network->last = temp_layer;
            network->last->next = NULL;
            network->first->prec = NULL;
            network->nb_inputs = atoi(strtok(text, ";"));
            network->last->nb_in = network->nb_inputs;
            network->inputs = calloc(network->nb_inputs, sizeof(float));
            if (network->inputs == NULL)
            {
                fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                exit(EXIT_FAILURE);
            }
            temp_layer->in = network->inputs;
            temp_layer->nb_neuron = atoi(strtok(NULL, ";"));
            new_neurons(temp_layer, index_layer);
            index_layer++;
        }
        else
        {
            input = atoi(strtok(text, ";"));
            nb_neuron_of_new_layer = atoi(strtok(NULL, ";"));
            add_layer(network, nb_neuron_of_new_layer, index_layer);
            index_layer++;
        }
        for (int i = 0; i < network->last->nb_neuron; i++)
        {
            fgets(text, MAX_LENGTH, file);
            network->last->tab_neuron[i].bias = (float)atof(strtok(text, ";"));
            for (int j = 0; j < network->last->nb_in; j++)
            {
                network->last->tab_neuron[i].weights[j] = (float)atof(strtok(NULL, ";"));
            }
        }
    }
    fclose(file);
    temp_layer = network->first;
    int* tab_neuron_per_layer = calloc(index_layer, sizeof(int));
    if (tab_neuron_per_layer == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    for (int index = 0; index < index_layer; index++)
    {
        tab_neuron_per_layer[index] = temp_layer->nb_neuron;
        temp_layer = temp_layer->next;
    }
    network->nb_neuron_per_layer = tab_neuron_per_layer;
    return network;
}

void print_network(NETWORK* network)
{
    printf("\n\t\t\t\tNetwork :\n\n");
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("\n\tLearning rate of the network : %f\n", network->learning_rate);
    printf("\n\t\tInputs of network :  ");
    for (int index_inputs_network = 0; index_inputs_network < network->nb_inputs; index_inputs_network++)
    {
        printf("%.3f ", network->inputs[index_inputs_network]);
    }
    printf("\n");
    LAYER* layer = network->first;
    int index = 1;
    while (layer != NULL)
    {
        printf("/////////////////////////////////////////////////////////////////////////////////////////////////\n");
        printf("Layer %d : \n", index);
        printf("\tInputs of layer %d : ", index);
        for (int index_in = 0; index_in < layer->nb_in; index_in++)
        {
            printf("%f ", layer->in[index_in]);
        }
        for (int index_neuron = 0; index_neuron < layer->nb_neuron; index_neuron++)
        {
            printf("\n**********************************************************\n");
            printf("\t\tNeuron %d : \n\t\t\tWeights : ", index_neuron + 1);

            for (int index_weights = 0; index_weights < layer->nb_in; index_weights++)
            {
                printf("%3f ", layer->tab_neuron[index_neuron].weights[index_weights]);
            }
            printf("\n\t\t\tBias : %f", layer->tab_neuron[index_neuron].bias);
        }

        printf("\n\tOuputs of layer %d : ", index);
        for (int index_out = 0; index_out < layer->nb_out; index_out++)
        {
            printf("%f ", layer->out[index_out]);
        }
        printf("\n");
        layer = layer->next;
        index++;
    }
}

void free_network(NETWORK* network)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    LAYER* layer = network->last;
    LAYER* temp = NULL;
    for (int index = 0; index < network->nb_layers; index++)
    {
        for (int i = 0; i < layer->nb_neuron; i++)
        {
            free(layer->tab_neuron[i].weights);
            free(layer->tab_neuron[i].d_weights);
        }
        free(layer->tab_neuron);
        temp = layer;
        layer = layer->prec;
        free(temp);
    }
    free(network->inputs);
    free(network->nb_neuron_per_layer);
    free(network);
}

float random_float(float A, float B)
{
    return ((float)rand() * (B - A)) / (float)RAND_MAX + A;
}

float output_layer(LAYER* layer, int index)
{
    if (layer == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    return sigmoid(scalarProduct(layer->in, layer->tab_neuron[index].weights, layer->nb_in) + layer->tab_neuron[index].bias);
}

NETWORK* random_network(NETWORK* network)
{
    LAYER* temp_layer = network->first;
    while (temp_layer != NULL)
    {
        for (int i = 0; i < temp_layer->nb_neuron; i++)
        {
            for (int j = 0; j < temp_layer->nb_in; j++)
            {
                temp_layer->tab_neuron[i].weights[j] = random_float(-2, 2);
            }
            temp_layer->tab_neuron[i].bias = random_float(-2, 2);
        }
        temp_layer = temp_layer->next;
    }
    return network;
}

void mixDatabase(DATASET database, int index_first, int index_last)
{
    int i, j, k;
    DATA_DIGIT temp;
    for (i = 0; i < index_last / 2; i++)
    {
        j = rand_int(index_first, index_last-1);  //Random index between 0 and index_n_max
        k = rand_int(index_first, index_last-1);  //Random index between 0 and index_n_max
        // Exchange data of digit[i] with data of digits [j]
        temp = database.digits[j];
        database.digits[j] = database.digits[k];    
        database.digits[k] = temp;
    }
}

int rand_int(int a, int b)
{
    return a + rand() % (b + 1 - a);
}

void training_network(NETWORK* network, DATASET database, int nb_img_per_subgroup, int nb_epoch)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    float counter = 0.0;
    mixDatabase(database,0,database.nb_digit);    // Database mixed before the training
    int nb_susgroup = (database.nb_digit - 10000)/nb_img_per_subgroup; // nb_img_per_subgroup;   
                                                                            //  nb_susgroup = 50000 / nb_img_per_susgroup (50000 data because i keep the 10000 last data for testing)
    for (int i = 0; i < nb_epoch; i++)      // At each epoch, I mix my database
    {
        for (int j = 0; j < nb_susgroup; j++)
        {
            for (int k = 0; k < nb_img_per_subgroup; k++)
            {
                feedForward(network, database.digits[nb_img_per_subgroup * j + k].pixels);
                backPropagation(network, database.digits[nb_img_per_subgroup * j + k].digit_expected);
                counter++;
                printProgress(counter/(nb_susgroup*nb_img_per_subgroup*nb_epoch));
            }
            gradientDescent(network, nb_img_per_subgroup);
        }
        mixDatabase(database,0,database.nb_digit - 10000);
    }
}

void feedForward(NETWORK* network, unsigned char* pixels)
{
    if (network == NULL || pixels == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    LAYER* layer = network->first;
    for (int i = 0; i < layer->nb_in; i++)
    {
        layer->in[i] = ((float)pixels[i]) / 255;
    }
    for (int i = 0; i < layer->nb_out; i++)
    {
        layer->out[i] = output_layer(layer, i);
    }
    while (layer->next != NULL)
    {
        layer = layer->next;
        for (int i = 0; i < layer->nb_in; i++)
        {
            layer->in[i] = layer->prec->out[i];
        }
        for (int i = 0; i < layer->nb_out; i++)
        {
            layer->out[i] = output_layer(layer,i);
        }
    }

}

float scalarProduct(float* in, float* weights, int nb_in)
{
    float sum = 0;
    for (int i = 0; i < nb_in; i++)
    {
        sum += in[i] * weights[i];
    }
    return sum;
}

float sigmoid(float output_neuron)
{
    return (float)(1.0 / (1.0 + (float)exp(-output_neuron)));
}

void backPropagation(NETWORK* network, int digit_expected)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    dL(network, digit_expected);
    dl(network);
}

void gradientDescent(NETWORK* network, int nb_img_per_subgroup)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    LAYER* layer = network->last;
    while (layer != NULL)
    {
        for (int i = 0; i < layer->nb_neuron; i++)
        {
            layer->tab_neuron[i].bias -= (network->learning_rate / nb_img_per_subgroup) * layer->tab_neuron[i].d_bias;
            layer->tab_neuron[i].d_bias = 0;
            for (int j = 0; j < layer->nb_in; j++)
            {
                layer->tab_neuron[i].weights[j] -= (network->learning_rate / nb_img_per_subgroup) * layer->tab_neuron[i].d_weights[j];
                layer->tab_neuron[i].d_weights[j] = 0;
            }
        }
        layer = layer->prec;
    }
}

void dL(NETWORK* network, int digit_expected)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    LAYER* layer = network->last;
    for (int i = 0; i < layer->nb_neuron; i++)
    {
        layer->tab_neuron[i].delta = (layer->out[i] - (i == digit_expected)) * (layer->out[i] * (1 - layer->out[i]));
        layer->tab_neuron[i].d_bias += layer->tab_neuron[i].delta;
        for (int j = 0; j < layer->nb_in; j++)
        {
            layer->tab_neuron[i].d_weights[j] += layer->in[j] * layer->tab_neuron[i].delta; 
        }
    }
}

void dl(NETWORK* network)
{
    if (network == NULL)
    {
        fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
        exit(EXIT_FAILURE);
    }
    LAYER* layer = network->last->prec;
    while (layer != NULL)
    {
        for (int i = 0; i < layer->nb_neuron; i++)
        {
            layer->tab_neuron[i].delta = 0;
            for (int j = 0; j < layer->next->nb_neuron; j++)
            {
                layer->tab_neuron[i].delta += layer->next->tab_neuron[j].delta * layer->next->tab_neuron[j].weights[i];
            }
            layer->tab_neuron[i].delta = layer->tab_neuron[i].delta * (layer->out[i] * (1 - layer->out[i]));
            layer->tab_neuron[i].d_bias += layer->tab_neuron[i].delta;
            for (int k = 0; k < layer->nb_in; k++)
            {
                layer->tab_neuron[i].d_weights[k] += layer->in[k] * layer->tab_neuron[i].delta;
            }
        }
        layer = layer->prec;
    }
}

float testNetwork(NETWORK* network, DATASET database)
{
    if (network == NULL)
    {
            fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
            exit(EXIT_FAILURE);
    }
    int nb_success = 0;
    float success_rate = 0.0;
    int digit_read = 0;
    float counter = 0.0;
    LAYER* layer;
    for (int i = database.nb_digit - 10000; i < database.nb_digit; i++)
    {
        printProgress(counter / 10000);
        feedForward(network, database.digits[i].pixels);
        layer = network->last;
        digit_read = find_OutputMax(layer->out, layer->nb_out);
        if (digit_read == database.digits[i].digit_expected)
        {
            nb_success++;
        }
        layer = network->first;
        counter++;
    }
    success_rate = (float)nb_success / 10000;
    return success_rate * 100;
}

int find_OutputMax(float* outputs, int nb_out)
{
    int outputMax_index = 0;
    for (int i = 1; i < nb_out; i++)
    {
        if (outputs[i] > outputs[outputMax_index])
        {
            outputMax_index = i;
        }
    }
    return outputMax_index;
}

void print_digit_expected(unsigned char* pixels, int nb_pixels)
{
    for (int i = 0; i < nb_pixels; i++)
    {
        if (pixels[i] == 0)
        {
            printf(".");
            if ((i + 1) % 28 == 0)
            {
                printf("\n");
            }
        }
        if (pixels[i] != 0)
        {
            printf("0");
            if ((i + 1) % 28 == 0)
            {
                printf("\n");
            }
        }
    }
}

void testNETWORK_digit(NETWORK* network, DATASET database)
{
    int digit_read = 0;
    int rand_digit = rand_int(database.size_pixels - 1, database.size_pixels - 9999);
    LAYER* layer;
    feedForward(network, database.digits[rand_digit].pixels);
    layer = network->last;        
    digit_read = find_OutputMax(layer->out, layer->nb_out);
    printf("\n***************************************************************\n");
    printf("Digit predicted by the network  : %d\n", digit_read);
    print_digit_expected(database.digits[rand_digit].pixels, database.size_pixels);
    printf("***************************************************************\n");
}

void printMenu(int DATASET_LOADED, int NETWORK_LOADED, int NETWORK_TRAINED)
{
    printf("                 AI Project by Othman RAHAL (INSTRU 1) & Samir TASTIFT (TELECOM 1)             \n");
    printf("                           Man - tutored project by M.Romain DUBESSY                           \n");       
    printf("                                Engineering school : SupGalilee                                \n"); 
    putchar('\n');
    printf("                           Welcome to our number recognition program                           \n");
    putchar('\n');
    printf("     ******************************************************************************************\n");
    printf("      ........................................................................................ \n");
    printf("      .......(1) Load a database from a file.csv.............................................. \n");
    printf("      .......(2) Create a new neural network.................................................. \n");
    printf("      .......(3) Load a neural network from a file.csv........................................ \n");
    printf("      .......(4) Display the neural network................................................... \n");
    printf("      .......(5) Train the neural network..................................................... \n");
    printf("      .......(6) Test the neural network with database........................................ \n");
    printf("      .......(7) Test the neural network with one digit of the database....................... \n");
    printf("      .......(8) Save a neural network in a file.csv.......................................... \n");
    printf("      .......(0) Exit......................................................................... \n");
    printf("      ...................................................By.Othman.RAHAL.&.Samir.TASTIFT...... \n");
    printf("      .......................................................Tutor.project :M.Romain.DUBESSY.. \n");
    printf("     ******************************************************************************************\n");
    putchar('\n');
    if (DATASET_LOADED)
    {
        if (NETWORK_LOADED)
        {
            if (NETWORK_TRAINED)
            {
                printf("        --------------             --------------               ---------------           \n");
                printf("       |DATASET LOADED|           |NETWORK LOADED|             |NETWORK TRAINED|          \n");
                printf("        --------------             --------------               ---------------           \n");
            }
            else
            {
                printf("        --------------             --------------                                         \n");
                printf("       |DATASET LOADED|           |NETWORK LOADED|            NETWORK NOT TRAINED         \n");
                printf("        --------------             --------------                                         \n");
            }
        }
        else
        {
            printf("            --------------                                                                    \n");
            printf("           |DATASET LOADED|          NETWORK NOT LOADED           NETWORK NOT TRAINED         \n");
            printf("            --------------                                                                    \n");
        }
    }
    else
    {
        if (NETWORK_LOADED)
        { 
            if (NETWORK_TRAINED)
            {
                printf("                                          --------------             ---------------          \n");
                printf("               DATASET NOT LOADED        |NETWORK LOADED|           |NETWORK TRAINED|         \n");
                printf("                                          --------------             ---------------          \n");
            }
            else
            {
                printf("                                          --------------                                      \n");
                printf("               DATASET NOT LOADED        |NETWORK LOADED|            NETWORK NOT TRAINED      \n");
                printf("                                          --------------                                      \n");
            }  
        }
        else
        {
            printf("                                                                                                  \n");
            printf("               DATASET NOT LOADED         NETWORK NOT LOADED             NETWORK NOT TRAINED      \n");
            printf("                                                                                                  \n");
        }
    }


}