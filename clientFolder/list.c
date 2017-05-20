/** @file
    @brief Generic list implementation
*/
#include <stdio.h>
#include <list.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define empty(MEM, SIZE) memset(MEM, 0, SIZE)
#define throw(MSG) assert(0 && MSG);

struct List
{
    size_t capacity;
    size_t size;
    void **items; /**< holds a pointer to items array */
};

static const int initialCapacity = 4;

static void __ensureCapacity(List *self, size_t min);

List *List_new(void)
{
    List *self = malloc(sizeof(struct List));
    self->capacity = initialCapacity;
    self->size = 0;
    self->items = malloc(0);
    __ensureCapacity(self, 0);
    return self;
}

void List_free(List **selfPtr)
{
    if (selfPtr == NULL)
        throw("Null pointer on free()");
    List *self = *selfPtr;
    free(self->items);
    free(self);
    *selfPtr = NULL;
}

void *List_get(List *self, int index)
{
    if (index < 0 || index >= self->size)
        throw("Index out of bounds");
    return self->items[index];
}
void List_set(List *self, int index, void *ref)
{
    if (ref == NULL)
        throw("NULL reference");
    if (index < 0 || index >= self->size)
        throw("Index out of bounds");

    self->items[index] = ref;
}
void List_add(List *self, void *ref)
{
    if (ref == NULL)
        throw("NULL reference");
    if (self->size <= self->capacity)
    {
        __ensureCapacity(self, self->size + 1);
    }
    List_set(self, self->size++, ref);
}
void List_insert(List *self, int index, void *ref)
{
    // if (ref == NULL) throw("NULL reference");
    // if (index > self->size) throw("Argument out of range");
    // if (self->size >= self->capacity - 1) {
    //     __ensureCapacity(self, self->size + 1);
    // }
    // if (index < self->size) {
    //     Array items = {
    //         .itemSize = sizeof(void *),
    //         .items = (char *)self->items,
    //         .length = self->size
    //     };
    //     Array_copy(items, index, items, index + 1, self->size - index);
    // }
    // List_set(self, index, ref);
    // self->size++;
    // self->version++;
}

int List_indexOf(List *self, void *ref)
{
    if (ref == NULL)
        throw("NULL reference");
    for (int i = 0; i < self->size; i++)
    {
        if (self->items[i] == ref)
        {
            return i;
        }
    }
    return -1;
}

bool List_contains(List *self, void *ref)
{
    if (ref == NULL)
        throw("NULL reference");
    return List_indexOf(self, ref) >= 0;
}

bool List_remove(List *self, void *ref)
{
    if (ref == NULL)
        throw("NULL reference");
    int index = List_indexOf(self, ref);
    if (index >= 0)
    {
        List_removeAt(self, index);
        return true;
    }
    return false;
}

void List_removeAt(List *self, int index)
{
    // if (index < 0 || index >= self->size) throw("Index out of bounds");
    // self->size--;
    // if (index < self->size) {
    //     Array items = {
    //         .itemSize = sizeof(void *),
    //         .items = (char *)self->items,
    //         .length = self->size
    //     };
    //     Array_copy(items, index + 1, items, index, self->size - index);
    // }
    // self->version++;
}
bool List_isEmpty(List *self)
{
    return self->size == 0;
}
int List_count(List *self)
{
    return self->size;
}
void List_copyTo(List *self, void *array, int arrayIndex)
{
    if (arrayIndex < 0 || arrayIndex >= self->size)
        throw("Index out of bounds");
    memcpy(
        array,
        self->items[arrayIndex],
        (self->size - arrayIndex) * sizeof(void *));
}

void List_clear(List *self)
{
    if (self->size > 0)
    {
        self->size = 0;
    }
}

static void __ensureCapacity(List *self, size_t min)
{
    if (self->size <= min)
    {
        int newCapacity = self->size == 0 ? initialCapacity : self->size * 2;
        if (newCapacity < min)
            newCapacity = min;
        self->capacity = newCapacity;
        size_t newSize = sizeof(void *) * self->capacity;
        self->items = realloc(self->items, newSize);
    }
}

void List_print(List *self)
{
    int counter = 0;

    if (self->size != 0)
    {
        while (self->size != counter)
        {
            printf("%i ", *(int *)self->items[counter]);
            counter++;
        }
    }

    puts("");
}

// static int toDecimal(int * chunk)
// {
//     int power = 7;
//     int result = 0;
//     int * ptr = chunk;
//     while(power >= 0)
//     {
//         result += *ptr * pow(2 , power);
//         power--;
//         ptr++;
//     }

//     return result;
// }

int toDecimal(int *chunk)
{
    int power = 7;
    int result = 0;
    int *ptr = chunk;

    while (power >= 0)
    {
        result += *ptr * pow(2, power);
        power--;
        ptr++;
    }

    return result;
}

List *List_transform(List *list)
{
    List *trList = List_new();

    int numOfChunks = list->size / 8;
    int chunkArray[numOfChunks][8];

    int counter = 0;
    int chunkCounter = 0;

    while (chunkCounter != numOfChunks)
    {
        int cellCounter = 0;
        while (cellCounter < 8)
        {
            if (*(int *)list->items[counter] > 0)
            {
                chunkArray[chunkCounter][cellCounter] = 1;
            }
            else
            {
                chunkArray[chunkCounter][cellCounter] = 0;
            }
            cellCounter++;
            counter++;
        }
        chunkCounter++;
    }

    int k = 0;
    int valueArr[chunkCounter];
    while (k < chunkCounter)
    {
        valueArr[k] = toDecimal(&chunkArray[k][0]);
        List_add(trList, (void *)&valueArr[k]);
        k++;
    }

    return trList;
}