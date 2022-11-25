//By Othman RAHAL and Samir TASTIFT     Man-tutored project by M.Romain DUBESSY        Engineering school : Institut SupGalilée
#define _CRT_SECURE_NO_WARNINGS
#define MAX_LENGTH_FILE 1000
#include "lib.h"
#define TRUE 1
#define FALSE 0


int main(void)
{
    srand((unsigned int)time(NULL));
    int DATASET_LOADED = FALSE;
    int NETWORK_LOADED = FALSE;
    int NETWORK_TRAINED = FALSE;
    char keypress;
    int nb_img_per_subgroup = 0;
    int nb_epoch = 0;
    int MENU_CHOOSE = 1;
    char filename_text[MAX_LENGTH_FILE] = "";
    int check_filename = FALSE;
    FILE* file = NULL;
    DATASET database;
    NETWORK* network = NULL;
    float success_rate = 0;


       while (MENU_CHOOSE)
       {      
            printMenu(DATASET_LOADED, NETWORK_LOADED, NETWORK_TRAINED);
            printf("\r\t\t--->Please enter a number about the menu : ");
            scanf("%d", &MENU_CHOOSE);
            emptyBuffer();
            if(MENU_CHOOSE < 9 && MENU_CHOOSE > 0)
            {
                //fflush(stdin);
                            
                check_filename = FALSE;
                if (MENU_CHOOSE == 0)   //  (0) Exit
                    return EXIT_SUCCESS;
                else if(MENU_CHOOSE == 1)   //  (1) Load a database from a file.csv
                {
                                

                    if (DATASET_LOADED)
                    {
                        printf("\r\t\t--->THE OLD DATABASE WILL BE OVERWRITTEN ! \n");
                        printf("\r\t\t--->YOU NEED TO LOADED AGAIN A NETWORK !  \n");
                        free_database(database);
                    }
                    check_filename = FALSE;
                    while (!check_filename)
                    {
                                    
                        printf("\tEnter the filename of database with correct format csv (Ex: images_data.csv) : ");
                        fgets(filename_text, MAX_LENGTH_FILE, stdin);
                        fflush(stdin);
                        filename_text[strlen(filename_text) - 1] = '\0';
                        if ((filename_text[strlen(filename_text) - 1] == 'v') && (filename_text[strlen(filename_text) - 2] == 's') && (filename_text[strlen(filename_text) - 3] == 'c') && (filename_text[strlen(filename_text) - 4] == '.'))
                        {
                            if ((file = fopen(filename_text, "r")) != NULL)
                            {
                                database = decompress_extract_img(filename_text, file);
                                DATASET_LOADED = TRUE;
                                NETWORK_LOADED = FALSE;
                                NETWORK_TRAINED = FALSE;
                                printf("\r\t\t---> DATA CHARGED SUCCESSFULLY ! \n");
                                printf("\r\t\t--->PRESS ENTER ! ");
                                emptyBuffer();
                                check_filename = TRUE;
                            }
                            else
                            {
                                printf("\r\t\t--->ERROR ! CAN'T OPEN FILE \"%s\" -_-\n", filename_text);
                                check_filename = FALSE;
                                DATASET_LOADED = FALSE;
                                NETWORK_LOADED = FALSE;
                                NETWORK_TRAINED = FALSE;
                            }
                        }
                        else
                        {
                            printf("\r\t\t--->ERROR ! YOU NEED TO ENTER THE CORRECT FORMAT CSV  -_-\n");
                            check_filename = FALSE;
                            DATASET_LOADED = FALSE;
                            NETWORK_LOADED = FALSE;
                            NETWORK_TRAINED = FALSE;
                        }
                    }
                }
                else if (MENU_CHOOSE == 2)  //  (2) Create a new neural network
                {
                    if (DATASET_LOADED)
                    {
                        printf("\t--->Creation of network ... \n");
                        if (NETWORK_LOADED)
                        {
                            printf("\r\t\t--->THE OLD NETWORK WILL BE OVERWRITTEN ! \n");
                            printf("\r\t\t--->YOU NEED TO TRAIN AGAIN THE NEW NETWORK !  \n");
                            free_network(network);
                        }
                        network = new_network(database);
                        NETWORK_LOADED = TRUE;
                        NETWORK_TRAINED = FALSE;
                        printf("\r\t\t--->NETWORK CREATED SUCCESSFULLY ! \n");
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else
                    {

                        printf("\n\t-->DATASET NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_LOADED = FALSE;
                        NETWORK_TRAINED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                                

                }
                else if (MENU_CHOOSE == 3)  //  (3) Load a neural network from a file.csv
                {
                    check_filename = FALSE;
                    while (!check_filename)
                    {
                        if (NETWORK_LOADED)
                        {
                            printf("\r\t\t--->THE OLD NETWORK WILL BE OVERWRITTEN ! \n");
                            printf("\r\t\t--->YOU NEED TO TRAIN AGAIN THE NEW NETWORK !  \n");
                            free_network(network);
                        }
                        printf("\tEnter the filename of network with format csv (Ex: Network.csv) : ");
                        fgets(filename_text, MAX_LENGTH_FILE, stdin);
                        fflush(stdin);
                        filename_text[strlen(filename_text) - 1] = '\0';
                        if ((filename_text[strlen(filename_text) - 1] == 'v') && (filename_text[strlen(filename_text) - 2] == 's') && (filename_text[strlen(filename_text) - 3] == 'c') && (filename_text[strlen(filename_text) - 4] == '.'))
                        {
                            if ((file = fopen(filename_text, "r")) != NULL)
                            {
                                fclose(file);
                                check_filename = TRUE;
                                network = loadNETWORK(filename_text);
                                NETWORK_LOADED = TRUE;
                                printf("\r\t\t--->NETWORK LOADED SUCCESSFULLY ! \n");
                                NETWORK_TRAINED = TRUE;
                                printf("\r\t\t--->PRESS ENTER ! ");
                                emptyBuffer();
                            }
                        }
                        else
                        {
                            printf("\r\t\t--->ERROR ! YOU NEED TO ENTER THE CORRECT FORMAT CSV  -_-\n");
                            check_filename = FALSE;
                            NETWORK_LOADED = FALSE;
                            NETWORK_TRAINED = FALSE;
                        }
                    }
                }
                else if (MENU_CHOOSE == 4)  //  (4) Display the neural network
                {
                    if (NETWORK_LOADED)
                    {
                        print_network(network);
                        printf("\n\n\n\r\t\t--->PRESS ENTER ! \n");
                        emptyBuffer();
                    }
                    else
                    {
                        printf("\t----->\tNETWORK NOT LOADED PREVIOUSLY !\n");
                        NETWORK_TRAINED = FALSE;
                        DATASET_LOADED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                }
                else if (MENU_CHOOSE == 5)  //  (5) Train the neural network
                {
                    if (NETWORK_LOADED && DATASET_LOADED)
                    {
                        do
                        {
                            nb_img_per_subgroup = 0;
                            while (!nb_img_per_subgroup)
                            {
                                printf("                                     AI Project by Othman RAHAL (INSTRU 1) & Samir TASTIFT                                \n\n");
                                clearTerminal();
                                printf("                          ________choose among this numbers the number of images per susgroup____\n");
                                printf("                         |                                                                      |\n");
                                printf("                         |  1; 2; 4; 5; 8; 10; 16; 20; 25; 40; 50; 80; 100; 125; 200; 250;      |\n");
                                printf("                         |   400; 500; 625; 1 000; 1 250; 2 000; 2 500; 3 125; 5 000; 6 250;    |\n");
                                printf("                         |                10 000; 12 500; 25 000 et 50 000                      |\n");
                                printf("                         |______________________________________________________________________|\n");
                                printf("\n                              ----->Enter the number of images per susgroup valid : "); // We keep 10 000 digits for the testing network
                                if (scanf("%d", &(nb_img_per_subgroup)) != 1)
                                {
                                    fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                                    fflush(stdin);

                                    exit(EXIT_FAILURE);
                                }
                                emptyBuffer();

                                putchar('\n');
                            }
                        } while (50000 % nb_img_per_subgroup != 0); // We keep 10 000 digits for the testing network
                        do
                        {
                            printf("\t----->\tEnter the number of epochs ( > 0) : ");
                            if (scanf("%d", &nb_epoch) != 1)
                            {
                                fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                                emptyBuffer();
                                exit(EXIT_FAILURE);
                            }
                            emptyBuffer();
                            putchar('\n');
                        } while (nb_epoch <= 0);
                        do
                        {
                            printf("\t----->\tEnter the learning rate(0 < x < 1) : ");
                            if (scanf("%f", &(network->learning_rate)) != 1)
                            {
                                fprintf(stderr, "fonction %s: %s\n", __func__, strerror(errno));
                                emptyBuffer();

                                exit(EXIT_FAILURE);
                            }
                            emptyBuffer();

                            putchar('\n');
                        }while (!((network->learning_rate) <= 1 && (network->learning_rate >= 0)));
                        clearTerminal();
                        NETWORK_TRAINED = TRUE;
                        printMenu(DATASET_LOADED, NETWORK_LOADED, NETWORK_TRAINED);
                        printf("\tStart of the network's training...\n");
                        training_network(network, database, nb_img_per_subgroup, nb_epoch);
                        printf("\n --> End of the network's training...\n");
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();

                    }if (!NETWORK_LOADED)
                    {
                        printf("\t----->\tNETWORK NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        DATASET_LOADED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else if (!DATASET_LOADED)
                    {
                        printf("\t----->\tDATASET NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        NETWORK_LOADED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                                

                }
                else if (MENU_CHOOSE == 6)  //  (6) Test the neural network with database
                {
                    if (NETWORK_LOADED && DATASET_LOADED && NETWORK_TRAINED)
                    {
                                    
                        mixDatabase(database, database.nb_digit - 10000, database.nb_digit);
                        if (DATASET_LOADED && NETWORK_LOADED)
                        {
                            printf("\tTesting with %s...\n", filename_text);
                            success_rate = testNetwork(network, database);
                            printf("\x0d\033[K");
                            printf("\n\aThe success rate of this neural network on the recognition of digits on 10 000 samples is %.2f %%\n", success_rate);
                            printf("\r\t\t--->PRESS ENTER ! ");
                            emptyBuffer();
                        }
                    }
                    else if(!NETWORK_LOADED)
                    {
                        printf("\t----->\tNETWORK NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else if (!DATASET_LOADED)
                    {
                        printf("\t----->\tDATASET NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        NETWORK_LOADED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else if (!NETWORK_TRAINED)
                    {
                        printf("  ----->\tNETWORK NOT TRAINED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                                
                }
                else if (MENU_CHOOSE == 7)      //  (7) Test the neural network with one digit of the database
                {
                    if (NETWORK_LOADED && DATASET_LOADED && NETWORK_TRAINED)
                    {
                        testNETWORK_digit(network, database);
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else if (!NETWORK_LOADED)
                    {
                        printf("\t----->\tNETWORK NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        NETWORK_LOADED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else if (!DATASET_LOADED)
                    {
                        printf("\t----->\tDATASET NOT LOADED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        NETWORK_LOADED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                    else if (!NETWORK_TRAINED)
                    {
                        printf("\t----->\tNETWORK NOT TRAINED PREVIOUSLY ! \n");
                        NETWORK_TRAINED = FALSE;
                        printf("\r\t\t--->PRESS ENTER ! ");
                        emptyBuffer();
                    }
                                

                }
                else if (MENU_CHOOSE == 8)  //  (8) Save a neural network in a file.csv
                {
                        if (NETWORK_LOADED)
                        {
                            check_filename = FALSE;
                            while (!check_filename)
                            {
                                            
                                printf("\tEnter the filename of network with format csv (Ex: Network.csv) : ");
                                fgets(filename_text, MAX_LENGTH_FILE, stdin);
                                filename_text[strlen(filename_text) - 1] = '\0';
                                fflush(stdin);
                                if ((filename_text[strlen(filename_text) - 1] == 'v') && (filename_text[strlen(filename_text) - 2] == 's') && (filename_text[strlen(filename_text) - 3] == 'c') && (filename_text[strlen(filename_text) - 4] == '.'))
                                {
                                    if ((file = fopen(filename_text, "w")) != NULL)
                                    {
                                        check_filename = TRUE;
                                        fclose(file);
                                        saveNETWORK(network, network->nb_inputs, network->nb_layers, network->nb_neuron_per_layer, filename_text);
                                        printf("\n\r\t\t--->NETWORK SAVED SUCCESSFULLY ! \n");
                                        printf("\r\t\t--->PRESS ENTER ! ");
                                        emptyBuffer();
                                    }
                                }
                                else
                                {
                                    printf("\r\t\t--->ERROR ! YOU NEED TO ENTER THE CORRECT FORMAT CSV  -_-\n");
                                    check_filename = FALSE;
                                }
                            }
                                        

                        }
                        else
                        {
                            printf("  ----->\tNETWORK NOT LOADED OR CREATED PREVIOUSLY ! \n");
                            printf("  ----->\tPRESS ENTER ! ");
                            emptyBuffer();
                        }
                }
            }        
            clearTerminal();
       }
       if (NETWORK_LOADED)
       {
           free_network(network);
       }
       if (DATASET_LOADED)
       {
           free_database(database);
       }
    return EXIT_SUCCESS;
}
