#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// simple function for loading text files for shader, return the char *shader given the right path.
const char *loadText(const char *textFile){
    FILE *file = fopen(textFile, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *content = (char *)malloc(file_size + 1);
    if (content == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return " ";
    }
    fread(content, 1, file_size, file);
    content[file_size] = '\0';
    fclose(file);
    return content;
}
