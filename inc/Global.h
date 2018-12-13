#ifndef GLOBAL_H
#define GLOBAL_H

/**
 * @mainpage MIPS Assembler
 *
 * @section sec0 Command line invocation
 *
 * Usage: <br/>
 * <br/>
 * ./assembler.exe
 *
 * To know makefile's rules, please use in the terminal inside the project folder:
 * make help
 *
 * You can read the Readme for more information.
 *
 *
 * @section sec1 What works
 *
 * - The lexical analysis <br/>
 * - The Grammatical analysis excepted pseudo-instructions <br/>
 * - Others functions <br/>
 *
 */

#define fileNameSize 256

#define HASHVALUE 31

#define HASHLENGTH 20

#define HASHCHAR 3

#define OP_INSTRUCT_MAX 3

#define OP_PSEUDO_MAX 9

#endif
