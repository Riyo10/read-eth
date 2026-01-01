#ifndef JSON_H
#define JSON_H

typedef struct {
    char *data;
    int size;
    int capacity;
} JSONBuffer;

JSONBuffer* json_create_object();
JSONBuffer* json_create_array();
void json_add_string(JSONBuffer *json, const char *key, const char *value);
void json_add_number(JSONBuffer *json, const char *key, double value);
void json_add_integer(JSONBuffer *json, const char *key, int value);
void json_add_boolean(JSONBuffer *json, const char *key, int value);
void json_add_object(JSONBuffer *parent, const char *key, JSONBuffer *child);
const char* json_get_string(JSONBuffer *json);
void json_free(JSONBuffer *json);

#endif // JSON_H
