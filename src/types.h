#ifndef APP_TYPES_H
#define APP_TYPES_H

typedef struct app_data {
    int pixel_size;
} app_data;

#define APP_DATA(x) (app_data *) (x)

#endif /* APP_TYPES_H */