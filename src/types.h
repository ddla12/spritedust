#ifndef APP_TYPES_H
#define APP_TYPES_H

typedef struct app_data {
    int pixel_size;
} app_data;

#define APP_DATA(x) (app_data *) (x)

#define DEFAULT_PIXEL_SIZE 16
#define LOWEST_PIXEL_SIZE 4
#define HIGHEST_PIXEL_SIZE 32

#endif /* APP_TYPES_H */