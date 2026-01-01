#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/json.h"

#define JSON_INITIAL_CAPACITY 1024

JSONBuffer* json_create_object() {
    JSONBuffer *json = (JSONBuffer *)malloc(sizeof(JSONBuffer));
    json->capacity = JSON_INITIAL_CAPACITY;
    json->data = (char *)malloc(json->capacity);
    json->size = 0;
    strcpy(json->data, "{");
    json->size = 1;
    return json;
}

JSONBuffer* json_create_array() {
    JSONBuffer *json = (JSONBuffer *)malloc(sizeof(JSONBuffer));
    json->capacity = JSON_INITIAL_CAPACITY;
    json->data = (char *)malloc(json->capacity);
    json->size = 0;
    strcpy(json->data, "[");
    json->size = 1;
    return json;
}

static void json_ensure_capacity(JSONBuffer *json, int needed) {
    while (json->size + needed >= json->capacity) {
        json->capacity *= 2;
        json->data = (char *)realloc(json->data, json->capacity);
    }
}

static void json_append(JSONBuffer *json, const char *str) {
    int len = strlen(str);
    json_ensure_capacity(json, len + 1);
    strcpy(json->data + json->size, str);
    json->size += len;
}

void json_add_string(JSONBuffer *json, const char *key, const char *value) {
    if (json->data[json->size - 1] != '{' && json->data[json->size - 1] != '[') {
        json_append(json, ",");
    }
    
    // Handle NULL values
    if (value == NULL) {
        value = "";
    }
    
    char buffer[2048];
    // Escape quotes and backslashes in value
    char escaped[1024] = {0};
    int j = 0;
    for (int i = 0; value[i] && j < sizeof(escaped) - 2; i++) {
        if (value[i] == '"') {
            escaped[j++] = '\\';
            escaped[j++] = '"';
        } else if (value[i] == '\\') {
            escaped[j++] = '\\';
            escaped[j++] = '\\';
        } else if (value[i] == '\n') {
            escaped[j++] = '\\';
            escaped[j++] = 'n';
        } else if (value[i] == '\r') {
            escaped[j++] = '\\';
            escaped[j++] = 'r';
        } else if (value[i] == '\t') {
            escaped[j++] = '\\';
            escaped[j++] = 't';
        } else {
            escaped[j++] = value[i];
        }
    }
    escaped[j] = '\0';
    
    snprintf(buffer, sizeof(buffer), "\"%s\":\"%s\"", key, escaped);
    json_append(json, buffer);
}

void json_add_number(JSONBuffer *json, const char *key, double value) {
    if (json->data[json->size - 1] != '{' && json->data[json->size - 1] != '[') {
        json_append(json, ",");
    }
    
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "\"%s\":%.2f", key, value);
    json_append(json, buffer);
}

void json_add_integer(JSONBuffer *json, const char *key, int value) {
    if (json->data[json->size - 1] != '{' && json->data[json->size - 1] != '[') {
        json_append(json, ",");
    }
    
    char buffer[512];
    snprintf(buffer, sizeof(buffer), "\"%s\":%d", key, value);
    json_append(json, buffer);
}

void json_add_boolean(JSONBuffer *json, const char *key, int value) {
    if (json->data[json->size - 1] != '{' && json->data[json->size - 1] != '[') {
        json_append(json, ",");
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "\"%s\":%s", key, value ? "true" : "false");
    json_append(json, buffer);
}

void json_add_object(JSONBuffer *parent, const char *key, JSONBuffer *child) {
    if (parent->data[parent->size - 1] != '{' && parent->data[parent->size - 1] != '[') {
        json_append(parent, ",");
    }
    
    // Close the child object/array
    if (child->data[child->size - 1] == '{') {
        json_append(child, "}");
    } else if (child->data[child->size - 1] == '[') {
        json_append(child, "]");
    } else if (child->data[child->size - 1] != '}' && child->data[child->size - 1] != ']') {
        if (child->data[0] == '{') {
            json_append(child, "}");
        } else {
            json_append(child, "]");
        }
    }
    
    char buffer[256];
    snprintf(buffer, sizeof(buffer), "\"%s\":%s", key, child->data);
    json_append(parent, buffer);
}

const char* json_get_string(JSONBuffer *json) {
    // Check if the JSON starts with '{' or '[' and close appropriately
    if (json->data[0] == '{') {
        // Only add closing brace if not already closed
        if (json->data[json->size - 1] != '}') {
            json_append(json, "}");
        }
    } else if (json->data[0] == '[') {
        // Only add closing bracket if not already closed
        if (json->data[json->size - 1] != ']') {
            json_append(json, "]");
        }
    }
    return json->data;
}

void json_free(JSONBuffer *json) {
    if (json) {
        if (json->data)
            free(json->data);
        free(json);
    }
}
