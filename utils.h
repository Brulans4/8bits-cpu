#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>

/**
 * @brief Allocates memory and checks if the allocation was successful.
 * 
 * @param size The size of the memory to be allocated.
 * @return void* The pointer to the allocated memory.
*/
void *smalloc(size_t size);

/**
 * @brief Checks if a condition is true.
 * 
 * @param cond The condition to be checked.
 * @param msg The message to be printed if the condition is false.
*/
void checkCond(bool cond, char *msg);

/**
 * @brief Checks if a value is negative.
 * 
 * @param res The value to be checked.
 * @param msg The message to be printed if the value is negative.
*/
void checkNeg(int res, char *msg);

/**
 * @brief Checks if a value is NULL.
 * 
 * @param res The value to be checked.
 * @param msg The message to be printed if the value is NULL.
*/
void checkNull(void *res, char *msg);

#endif
