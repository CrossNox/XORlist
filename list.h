#include <stdbool.h>
#include <stdlib.h>

struct list;
typedef struct list list_t;
struct iterator;
typedef struct list_iter list_iter_t;

//**********Basic Primitives**********//

//Creates a new list
//Pos: Returns a new empty list
list_t *list_create(void);

// Returns true or false depending on wether the list has or not elements, respectively.
// Pre: the list was previously created
bool list_is_empty(const list_t *list);

// Adds a new value to the list. Returns false if any problem arises.
// Pre: The list was previously created.
// Post: A new element was added to the list at the first position.
bool list_insert_first(list_t *list, void *data);

// Adds a new value to the list. Returns false if any problem arises.
// Pre: The list was previously created.
// Post: A new element was added to the list at the last position.
bool list_insert_last(list_t *list, void *data);

// Removes and returns the first element from the list. If the list were empty, it returns NULL.
// Pre: the list was previously created.
// Post: if the list was not empty, returns the first value and the list contains one less item.
void *list_delete_first(list_t *list);

// Removes and returns the last element from the list. If the list were empty, it returns NULL.
// Pre: the list was previously created.
// Post: if the list was not empty, returns the last value and the list contains one less item.
void *list_delete_last(list_t *list);

// Obtiene el primer valor la list. Si la list tiene elementos,
// se devuelve el primer valor. Si está vacía devuelve NULL.
// Pre: la list fue creada.
// Post: se devolvió el primer valor de la list, cuando la list no está
// vacía. NULL en caso contrario.

// Returns the first element from the list. If the list were empty, returns NULL.
// Pre: the list was created.
// Post: The first element from the list, or NULL if it were empty, is returned.
// No side effects. 
void *list_peek_first(const list_t *list);

// Returns the last element from the list. If the list were empty, returns NULL.
// Pre: the list was created.
// Post: The last element from the list, or NULL if it were empty, is returned.
// No side effects. 
void *list_peek_last(const list_t *list);

// Pre: The list has been created.
// Pos: returns the amount of elements the list contains
size_t list_length(const list_t *list);

// Destroys the list. If destroy_data is non-null, it is called for each element in the list.
// Pre: the list was created. destroy_data, if passed, is a function capable of destroying the
// data stored in the list.
// Post: Every element in the list was destroyed.
void list_destroy(list_t *list, void destroy_data(void *));

//**********Iteration primitives**********//

// Returns a new iterator
// Pos: Returns a new iterator allocated on the heap pointing to the first element
list_iter_t *list_iter_create_start(list_t *list);

// Returns a new iterator
// Pos: Returns a new iterator allocated on the heap pointing to the last element
list_iter_t *list_iter_create_end(list_t *list);

// Forwards the iterator one position over the list.
// Pre: The iterator and the list have been created
// Pos: The iterator is located on position forward on the list. If there were no 
// more elements after the current one, returns false. Else, returns true.
bool list_iter_forward(list_iter_t *iter);

// Forwards the iterator one position over the list.
// Pre: The iterator and the list have been created
// Pos: The iterator is located on position back on the list. If there were no 
// more elements after the current one, returns false. Else, returns true.
bool list_iter_backward(list_iter_t *iter);

// Returns the data at the current position
// Pre: The iterator and the list have been created
// Pos: The data at the current position is returned if there are elements and the
// iterator is not at the end. Else, returns NULL.
void *list_iter_peek_current(const list_iter_t *iter);

// Evaluates if the iterator is at the end of the list
// Pre: The iterator and the list have been created
// Pos: Returns true if the iterator is at the end of the list. Else, returns false.
bool list_iter_at_last(const list_iter_t *iter);

// Evaluates if the iterator is at the start of the list
// Pre: The iterator and the list have been created
// Pos: Returns true if the iterator is at the start of the list. Else, returns false.
bool list_iter_at_start(const list_iter_t *iter);

// Destroys the iterator
// Pre: The iterator has been created
void list_iter_destroy(list_iter_t *iter);

//**********List with iterator primitives**********//

// Adds a new element to the list at the iterator's current position
// Pre: The iterator and the list have been created
// Pos: Adds a new element to the list at the iterator's current position. Returns true
// upon success and the iterator's position's data is the new data.
// Returns false on failure.
bool list_iter_insert(list_iter_t *iter, void *data);

// Takes away the iterator's current position's data from the list
// Pre: The iterator and the list have been created
void *list_iter_delete(list_iter_t *iter);

//**********Internal iterator primitives**********//

// Iterates the list from the start while callback returns true
// The callback function takes the data and an extra pointer and returns true
// if iteration must go on.
// Pre: The iterator and the list have been created
void list_iterate_start(list_t *list, bool (*callback)(void *data, void *extra), void *extra);

// Iterates the list from the end while callback returns true
// The callback function takes the data and an extra pointer and returns true
// if iteration must go on.
// Pre: The iterator and the list have been created
void list_iterate_end(list_t *list, bool (*callback)(void *data, void *extra), void *extra);