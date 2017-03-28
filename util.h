#ifndef UTIL_H_
#define UTIL_H_

/* Drop the \n at the end */
void dropLineFeed(char *msg);

/* Concatenate the src to the destination, while first performing a realloc
   on the destination to make sute it's large enough */
bool safeCat(char **dest, size_t *dest_size, char *src, size_t src_size);

/* Replace - with _ */
void replaceDash(char *msg);

/* Uppercase all the characters in the string */
void toUpperCase(char *msg);

#endif /* UTIL_H */
