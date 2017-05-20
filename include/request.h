/** @file
	@brief Module responsible for processing requests
*/

#pragma once

#include <progbase/net.h>
#include <progbase/list.h>

#define MAX_COMMAND_LEN 100
#define MAX_ERROR_LEN 256

/**
	@struct Request
	@brief Defines Request structure
*/
typedef struct {
	char command[MAX_COMMAND_LEN];
} Request;

/**
	@brief Identifies command
	@param str - string consisting command
*/
Request parseRequest(const char * str);

/**
	@struct Response
	@brief Defines Response structure
*/
typedef struct {
	bool status;
	enum {
		RES_NUMBERS,
		RES_STRINGS
	} itemsType;
	List * data;
	char error[MAX_ERROR_LEN];
} Response;

/**
	@brief Frees memory, allocated by response
	@param self - pointer to response
*/
void Response_cleanup(Response * self);

/**
	@brief Initializes response list
	@param self - pointer to response
*/
void Response_init(Response * self);

/**
	@brief Adds number to response
	@param self - pointer to response
	@param number - number to insert
*/
void Response_addNumber(Response * self, int number);

/**
	@brief Adds string to response
	@param self - pointer to response
	@param str - string to insert
*/
void Response_addString(Response * self, const char * str);


/**
	@brief Adds error to respnse
	@param self - pointer to response
	@param error - error to insert
*/
void Response_setError(Response * self, const char * error);

/**
	@brief Sets the type of item that would be stored in response message
	@param self - pointer to response
	@param itemsType - the type of item
*/
void Response_setItemsType(Response * self, int itemsType);

/**
	@brief Convertes response structure NetMessage structure
	@param res - pointer to response
	@param message - desired message
*/
void Response_toMessage(Response * res, NetMessage * message);

/**
	@brief Callback function that is invoked upon initialization
	@param req - pointer to request
	@param res - pointer to response
*/
typedef void (*RequestHandler)(Request * req, Response * res);


/**
	@struct CommandHandler
	@brief Defines CommandHandler structure
*/
typedef struct {
	char command[MAX_COMMAND_LEN];
	RequestHandler handler;
} CommandHandler;

/**
	@brief Determines what command to proceed with
	@param req - pointer to request
	@param res - pointer to response
	@param handlers - pointer to the array of handler to choose from
	@param len - the length of handlers array
*/
void processRequest(Request * req, Response * res, CommandHandler * handlers, int len);


/**
	@brief Function that determines id-base command
	@param command - string consisting entire command
*/
int isId(char * command);

/**
	@brief Function that determines whether command is a complex favourite command
	@param command - string consisting entire command
*/
int isComplex(char *command);