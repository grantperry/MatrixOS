// ordered_array.c -- Implementation for creating, inserting and deleting
//					from ordered arrays.

#include "ordered_array.h"

/*
// A standard less than predicate.
*/
s8int standard_lessthan_predicate(type_t a, type_t b)
{
	return (a<b)?1:0;
}

/*
// Create an ordered array.
*/
ordered_array_t create_ordered_array(u32int max_size, lessthan_predicate_t less_than)
{
	ordered_array_t to_ret;
	to_ret.array = (void*)kmalloc(max_size*sizeof(type_t));
	memset(to_ret.array, 0, max_size*sizeof(type_t));
	to_ret.size = 0;
	to_ret.max_size = max_size;
	to_ret.less_than = less_than;
	return to_ret;
}

ordered_array_t place_ordered_array(void *addr, u32int max_size, lessthan_predicate_t less_than)
{
	ordered_array_t to_ret;
	to_ret.array = (type_t*)addr;
	memset(to_ret.array, 0, max_size*sizeof(type_t));
	to_ret.size = 0;
	to_ret.max_size = max_size;
	to_ret.less_than = less_than;
	return to_ret;
}

/*
// Destroy an ordered array.
*/
void destroy_ordered_array(ordered_array_t *array)
{
//	kfree(array->array);
}

/*
// Add an item into the array.
*/
void insert_ordered_array(type_t item, ordered_array_t *array)
{
	ASSERT(array->less_than);
	u32int iterator = 0;
	while (iterator < array->size && array->less_than(array->array[iterator], item))
		iterator++;
	if (iterator == array->size) // just add at the end of the array.
		array->array[array->size++] = item;
	else
	{
		type_t tmp = array->array[iterator];
		array->array[iterator] = item;
		while (iterator < array->size)
		{
			iterator++;
			type_t tmp2 = array->array[iterator];
			array->array[iterator] = tmp;
			tmp = tmp2;
		}
		array->size++;
	}
}

/*
// Lookup the item at index i.
*/
type_t lookup_ordered_array(u32int i, ordered_array_t *array)
{
	ASSERT(i < array->size);
	return array->array[i];
}

/*
// Deletes the item at location i from the array.
*/
void remove_ordered_array(u32int i, ordered_array_t *array)
{
	while (i < array->size)
	{
		array->array[i] = array->array[i+1];
		i++;
	}
	array->size--;
}
