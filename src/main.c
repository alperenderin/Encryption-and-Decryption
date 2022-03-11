/**
* @file main.c
* @description Decode and Encode a file using Red-Black Trees and Huffmann Algorithm 
               One of the purposes of this program is to quickly gain experience with the field.h, jval.h and jrb.h libraries.
* @date 16.05.2021
* @author Alperen Derin - alperenderinn@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <stdbool.h>
#include "jrb.h"
#include "fields.h"

void InputControl(int, char **);               // function that controls input commands
void ParseAndAddIntoJRB(IS, JRB, char **);     // the file to be read is taken as a parameter, ıt parsed and placed on the tree in encode or decode format.

char *StrConcatenate(char *, char *, char *);  // function that joins three strings one after the other.
void EncodeOrDecode(IS, JRB, char *);          // it finds the exported file in the tree and produces the correct output to the output file.

IS OpenInputFolderUsingLibfdr(char *);         // opens and returns the input file to be read using the libfdr library.
FILE *CreateOutputFolder(char *inputFileName); // opens and returns the file to be outcome.

int main(int argc, char **argv)
{
    InputControl(argc, argv);
    IS inputFile, ISKey = new_inputstruct(".kilit");
    JRB jrb = make_jrb();

    if (ISKey == NULL)
    {
        perror(".kilit");
        exit(1);
    }

    ParseAndAddIntoJRB(ISKey, jrb, argv); //We parsed according to argv (encode or decode?).
    inputFile = OpenInputFolderUsingLibfdr(argv[2]);
    EncodeOrDecode(inputFile, jrb, argv[3]);
}

/**
 * @brief this function makes encode or decode related to Huffmann Algorithm
 * 
 * @param inputFile input file
 * @param jrb red-black tree where we will store the data we have extracted from the file
 * @param outputFileName name of output file
 */
void EncodeOrDecode(IS inputFile, JRB jrb, char *outputFileName)
{
    FILE *outputFile = CreateOutputFolder(outputFileName); 
    JRB tmp;
    while (get_line(inputFile) >= 0)
    {
        for (int i = 0; i < inputFile->NF; i++)
        {
            tmp = NULL;
            tmp = jrb_find_str(jrb, inputFile->fields[i]);
            if (tmp != NULL)
            {
                if (strcmp(tmp->val.s, ""))
                    fprintf(outputFile, "%s ", tmp->val.s);             // Print the found value to output file
                else
                    fprintf(outputFile, "%s ", inputFile->fields[i]);   // There is no word equivalent, it prints the same word
            }
            else if(tmp == NULL){                                       // In case the tree is empty or the word is not in the tree, the same word is output.
                fprintf(outputFile, "%s ", inputFile->fields[i]);
            }
        }
    }
    printf("Process is done...\n");
    fclose(outputFile);
}

/**
 * @brief this function creates the input file
 * 
 * @param inputFileName filename that is a string
 * @return IS an input struct 
 */
IS OpenInputFolderUsingLibfdr(char *inputFileName)
{
    IS inputFile, inputFileNoEx;
    char *inputFilePath = StrConcatenate("", inputFileName, ".txt");

    inputFile = new_inputstruct(inputFilePath);
    if (inputFile == NULL)
    {
        //Maybe file has no extension
        inputFilePath = StrConcatenate("", inputFileName, "");
        inputFileNoEx = new_inputstruct(inputFilePath);

        if (inputFileNoEx == NULL)
        {
            perror(inputFileName);
            printf("Given as your file path for input file:%s\n", inputFilePath);
            exit(1);
        }
        return inputFileNoEx;
    }
    printf("Your input file is succesfully opened for encode process.\n");

    return inputFile;
}

/**
 * @brief Create a Output File
 * 
 * @param outputFileName file name as string
 * @return FILE* the generated file
 */
FILE *CreateOutputFolder(char *outputFileName)
{
    FILE *outputFile;
    char *outputFilePath = StrConcatenate("", outputFileName, "");

    outputFile = fopen(outputFilePath, "w");
    if (outputFile == NULL)
    {
        perror(outputFilePath);
        printf("Given as your file path for output file:%s\n", outputFilePath);
        exit(1);
    }
    printf("Your output file is succesfully created for process.\n");

    return outputFile;
}

/**
 * @brief this function adds the words that read from the file to the red-black tree
 * 
 * @param is input struct from .fields library, I use this for the file operations 
 * @param jrb a red-black three node
 * @param argv argument vector parameter
 */
void ParseAndAddIntoJRB(IS is, JRB jrb, char **argv)
{
    char _value[100], _trash[100], _key[100];
    char *value, *key;

    while (get_line(is) >= 0)
    {   
        sscanf(is->text1, "%[^\"]\"%[^\"]\"%[^\"]\"%[^\"]", _trash, _value, _trash, _key);
        
        value = (char*)(malloc(sizeof(strlen(_value)))); // In order not to lose memory when loading the tree, space is created in the length of the string.
        key = (char*)(malloc(sizeof(strlen(_key))));
        strcpy(value, _value);
        strcpy(key, _key);

        if (!(strcmp(argv[1], "-e")))                   // If the encode operation is to be performed, the value of the lock file is placed in the key field of the tree, and the key of the lock file is placed in the jval field.
            jrb_insert_str(jrb, strdup(value), new_jval_s(strdup(key)));
        else
            jrb_insert_str(jrb, strdup(key), new_jval_s(strdup(value)));

        strcpy(_value, "");
        strcpy(_key, "");
    }
    printf("Parsing and Placing process has been done.\n");
}

/**
 * @brief check the correctness of the parameters entered in the command prompt
 * 
 * @param argc argument count parameter
 * @param argv argument vector parameter
 */
void InputControl(int argc, char **argv)
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./kripto -d or -e  input_file_name output_file_name\n");
        exit(1);
    }
    else if (!((!strcmp(argv[1], "-d")) || (!strcmp(argv[1], "-e"))))   //control the second argument, (strcmp returns zero if strings is equal)
    {
        fprintf(stderr, "You don't give the first argument correctly (-d or -e) Your argument: %s\n", argv[1]);
        exit(1);
    }
}


/**
 * @brief Concatenates three strings
 * 
 * @param a any simple string
 * @param b any simple string
 * @param c any simple string
 * @return char* is the outcome of concatenation process
 */
char *StrConcatenate(char *a, char *b, char *c)
{
    int size = strlen(a) + strlen(b) + strlen(c) + 1;
    char *str = malloc(size);
    strcpy(str, a);
    strcat(str, b);
    strcat(str, c);

    return str;
}