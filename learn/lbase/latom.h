#ifndef LATOM_H
#define LATOM_H

extern int latom_length (const char *str);
extern const char *latom_new (const char *str, int len);
extern const char *latom_string (const char *str);
extern const char *latom_int (long n);

#endif
