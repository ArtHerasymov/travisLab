[![Travis-CI][travis-badge]][travis-builds]

[travis-badge]: https://travis-ci.org/ArtHerasymov/lab8.svg?branch=master
[travis-builds]: https://travis-ci.org/ArtHerasymov/lab8/builds
# Lab8(progbase)
## Goal : learn how to create a simple web server, using http interaction.
## Task1 : Create TCP-server that would listen to TCP-clients' requests and interacts with them using HTTP protocol. Server should process only HTTP GET request and return adequetly formattted HTTP responses.
### Requests :
* '/' - Returns json object that contains info about server
* '/favourites' - Returns json object that contains data about your favourite direcotrs
* '/favourites?{key}={value}' - Returns json object that contains data about your favourite directors based on searching criterion
* '/favourites/{id}' - Returns json object that contains data about your favourite director by the idnex
* '/file' - Returns the data about name, size and contents of the file
* '/file/data' - Returns the quantity of numbers and maximum value
## Task2 : Create documentation using Doxygen
## Task3 : Implement Continuous Integration
