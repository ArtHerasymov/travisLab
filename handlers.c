#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jansson.h>
#include <sys/stat.h>
#include <errno.h>

#include <request.h>

#define BUFFER_SIZE 20000
#define ARRAY_SIZE 200

char *getList();

off_t fsize(const char *filename)
{
    struct stat st;

    if (stat(filename, &st) == 0)
        return st.st_size;

    fprintf(stderr, "Cannot determine size of %s: %s\n",
            filename, strerror(errno));

    return -1;
}

char *dataToJson(int quantity, int max)
{
    printf("QUANTITY : %i , MAX : %i \n", quantity, max);
    json_t *obj = json_object();

    json_t *jsonArr = json_array();

    json_object_set_new(obj, "quantity", json_integer(quantity));
    json_object_set_new(obj, "max", json_integer(max));

    json_array_append_new(jsonArr, obj);

    char *result = malloc(sizeof(char) * BUFFER_SIZE);

    result = json_dumps(jsonArr, JSON_INDENT(2));

    puts(result);

    return result;
}

char *getById(int id)
{
    char *jsonString = getList("../directors.json");
    json_error_t err;
    json_t *jsonArr = json_loads(jsonString, 0, &err);
    int index = 0;
    json_t *value = NULL;
    json_t *resultArr = json_array();

    json_array_foreach(jsonArr, index, value)
    {
        if (json_integer_value(json_object_get(value, "id")) == id)
        {
            printf("ENTERED ID \n");
            json_t *object = json_object();
            json_object_set_new(object, "name", json_object_get(value, "name"));
            json_object_set_new(object, "id", json_object_get(value, "id"));
            json_object_set_new(object, "signature", json_object_get(value, "signature"));
            json_object_set_new(object, "citizenship", json_object_get(value, "citizenship"));
            json_array_append_new(resultArr, object);
        }
    }

    char *result = malloc(sizeof(char) * BUFFER_SIZE);

    if (json_array_size(resultArr) == 0)
    {
        strcpy(result, "404 Not Found");
        return result;
    }
    result = json_dumps(resultArr, JSON_INDENT(2));

    return result;
}

char *getSignature(char *key)
{
    char *jsonString = getList("../directors.json");
    puts(jsonString);
    json_error_t err;
    json_t *jsonArr = json_loads(jsonString, 0, &err);
    int index = 0;
    json_t *value = NULL;
    json_t *resultArr = json_array();

    json_array_foreach(jsonArr, index, value)
    {
        if (strcmp(json_string_value(json_object_get(value, "citizenship")), key) == 0)
        {
            json_t *object = json_object();
            json_object_set_new(object, "name", json_object_get(value, "name"));
            json_object_set_new(object, "id", json_object_get(value, "id"));
            json_object_set_new(object, "signature", json_object_get(value, "signature"));
            json_object_set_new(object, "citizenship", json_object_get(value, "citizenship"));
            json_array_append_new(resultArr, object);
        }
    }

    char *result = json_dumps(resultArr, JSON_INDENT(2));
    json_decref(jsonArr);
    json_decref(resultArr);
    json_decref(value);
    
    return result;
}

char *fileInfoToJson()
{
    char *str = getList("../data/data.txt");
    puts(str);
    char *result = malloc(sizeof(char) * BUFFER_SIZE);
    json_t *json = json_object();
    int size = fsize("../data/data.txt");
    printf("Size : %i \n", size);
    json_object_set_new(json, "Content", json_string(str));
    json_object_set_new(json, "Size", json_integer(size));
    json_object_set_new(json, "File", json_string("data.txt"));

    result = json_dumps(json, JSON_INDENT(2));
    json_decref(json);

    return result;
}

char *serverinfoToJson()
{
    char *jsonString = malloc(sizeof(char) * BUFFER_SIZE);
    json_t *json = json_object();
    json_object_set_new(json, "title", json_string("Directors server"));
    json_object_set_new(json, "developer", json_string("Artem Herasymov"));
    json_object_set_new(json, "time", json_string("17:30"));

    jsonString = json_dumps(json, JSON_INDENT(0));

    json_decref(json);

    return jsonString;
}

char *getList(char *fileName)
{
    FILE *fin = fopen(fileName, "r");
    if (fin == NULL)
    {
        return NULL;
    }
    char buffer[10000] = "";
    char jsonString[10000] = "";
    while (fgets(buffer, 200, fin))
    {
        strcat(jsonString, buffer);
    }
    fclose(fin);

    char * ptr = &jsonString[0];

    return ptr;
}

void infoHandler(Request *req, Response *res)
{
    Response_setItemsType(res, RES_STRINGS);

    char *serverInfo = serverinfoToJson();
    Response_addString(res, serverInfo);
    free(serverInfo);
}

void favouritesHandler(Request *req, Response *res)
{
    Response_setItemsType(res, RES_STRINGS);
    char *ptr = req->command;
    ptr += 11;

    char *list = getList("../directors.json");
    if (list == NULL)
    {
        Response_setError(res, "File with directors wasn't found \n");
    }
    else
    {
        Response_addString(res, list);
    }
}

void signatureHandler(Request *req, Response *res)
{
    Response_setItemsType(res, RES_STRINGS);

    char *ptr = req->command;
    while (*ptr != '=')
    {
        ptr++;
    }
    ptr++;

    char *signatureList = getSignature(ptr);
    Response_addString(res, signatureList);
    free(signatureList);
    fflush(stdin);
}

void idHandler(Request *req, Response *res)
{
    Response_setItemsType(res, RES_STRINGS);

    char *ptr = req->command;
    ptr++;
    while (*ptr != '/')
    {
        ptr++;
    }
    ptr++;

    char *idString = getById(atoi(ptr));

    Response_addString(res, idString);

    free(idString);
}

void fileHandler(Request *req, Response *res)
{
    Response_setItemsType(res, RES_STRINGS);

    char *result = fileInfoToJson();

    Response_addString(res, result);
}

void dataHandler(Request *req, Response *res)
{
    Response_setItemsType(res, RES_STRINGS);

    char *str = getList("../data/data.txt");

    char *ptr = str;
    int array[ARRAY_SIZE];
    for (int i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = -1;
    }
    int index = 0;
    array[index] = atoi(ptr);
    index++;
    ptr++;
    while (*(ptr + 1) != EOF && *ptr != '\0')
    {
        if (*ptr == ' ' && 1)
        {
            array[index] = atoi(ptr + 1);
            index++;
        }
        ptr++;
    }

    int max = array[0];

    int counter = 0;

    while (array[counter] != -1)
    {
        if (array[counter] > max)
        {
            max = array[counter];
        }
        counter++;
    }
    char *result = dataToJson(index, max);

    Response_addString(res, result);
}