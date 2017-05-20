#include <string.h>
#include <ctype.h>
#include <jansson.h>

#include <request.h>


int isId(char * command)
{
    char * ptr = command;
    ptr++;

    while(ptr != NULL && *ptr != '\0')
    {
        if(*ptr == '/' && (*(ptr + 1) >= 48 && *(ptr + 1) <= 57))
        {
            return 1;
        }
        ptr++;
    }

    return 0;
}

int isComplex(char *command)
{
    char *ptr = command;

    if(strlen(command) == 1)
    {
        return 0;
    }

    while (ptr != NULL && *ptr != '\0')
    {
        if (*ptr == '?' || *ptr == '=')
        {
            return 1;
        }
        ptr++;
    }

    return 0;
}

Request parseRequest(const char *str)
{
    Request req = {
        .command = ""};
    int commandEnd = 0;
    while (isalpha(str[commandEnd]) || str[commandEnd] == '/' || str[commandEnd] == '=' || str[commandEnd] == '?' || (str[commandEnd] >= 48 && str[commandEnd] <= 57))
        commandEnd++;
    if (commandEnd >= MAX_COMMAND_LEN)
        commandEnd = MAX_COMMAND_LEN - 1;
    strncpy(req.command, str, commandEnd);
    req.command[commandEnd] = 0;
    return req;
}

RequestHandler getCommandRequestHandler(CommandHandler *handlers, int len, char *command)
{
    char pureCommand[MAX_COMMAND_LEN] = "";
    int sophFlag = -1;
    if (isComplex(command) == 1)
    {
        sophFlag = 1;
        char *ptr = command;
        int i = 0;
        while (*ptr != '=')
        {
            pureCommand[i] = *ptr;
            ptr++;
            i++;
        }
        pureCommand[i] = *ptr;
    }
    else if(isId(command) == 1)
    {
        char * ptr = command;
        int i = 0;
        pureCommand[i] = *ptr;
       
        i++;
        ptr++;

        while(*ptr != '/')
        {
            pureCommand[i] = *ptr;
            ptr++;
            i++;
        }
        pureCommand[i] = *ptr;
        sophFlag = 2;
    }

    for (int i = 0; i < len; i++)
    {
        CommandHandler handler = handlers[i];
        if (sophFlag == 1)
        {
            if (0 == strcmp(handler.command, pureCommand))
            {
                return handler.handler;
            }
        }
        else if(sophFlag == 2)
        {
            if(0 == strcmp(handler.command , pureCommand))
            {
                return handler.handler;
            }
        }
        else
        {
            if (0 == strcmp(handler.command, command))
            {
                return handler.handler;
            }
        }
    }
    return NULL;
}

void processRequest(Request *req, Response *res, CommandHandler *handlers, int len)
{
    RequestHandler handler = getCommandRequestHandler(handlers, len, req->command);
    if (handler == NULL)
    {
        char tmp[100] = "";
        sprintf(tmp, "Command `%s` not found", req->command);
        Response_setError(res, tmp);
        return;
    }
    handler(req, res);
}

void Response_setError(Response *self, const char *error)
{
    self->status = false;
    strcpy(self->error, error);
}

void Response_setItemsType(Response *self, int itemsType)
{
    self->itemsType = itemsType;
}

void Response_addString(Response *self, const char *str)
{
    void *item = malloc(strlen(str) + 1);
    strcpy(item, str);
    List_add(self->data, item);
}

void Response_addNumber(Response *self, int number)
{
    int *item = malloc(sizeof(int));
    *item = number;
    List_add(self->data, item);
}

void Response_init(Response *self)
{
    self->status = true;
    self->itemsType = RES_NUMBERS;
    self->data = List_new();
    strcpy(self->error, "");
}

void Response_cleanup(Response *self)
{
    if (self->data != NULL)
    {
        for (int i = 0; i < List_count(self->data); i++)
        {
            free(List_get(self->data, i));
        }
        List_free(&self->data);
    }
}

void Response_toMessage(Response *res, NetMessage *message)
{
    json_t *jobj = json_object();
    json_object_set(jobj, "status", json_boolean(res->status));
    if (!res->status)
    {
        json_object_set(jobj, "error", json_string(res->error));
    }
    else
    {
        json_t *jarray = json_array();
        for (int i = 0; i < List_count(res->data); i++)
        {
            void *value = List_get(res->data, i);
            json_t *jvalue = (res->itemsType == RES_NUMBERS)
                                 ? json_integer(*(int *)value)
                                 : json_string((char *)value);
            json_array_append_new(jarray, jvalue);
        }
        json_object_set_new(jobj, "data", jarray);
    }
    const char *strDump = json_dumps(jobj, JSON_COMPACT);
    NetMessage_setDataString(message, strDump);
    free((void *)strDump);
    json_decref(jobj);
}