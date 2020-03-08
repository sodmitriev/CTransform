/*!
 * @file transformation_derived_example.h
 * @brief Example of transformation inheritance
 */

//! @code transformation_inheritance_example

//***********************************************Header code (.h file)**************************************************

#ifndef CTRANSFORM_TRANSFORMATION_DERIVED_EXAMPLE_H
#define CTRANSFORM_TRANSFORMATION_DERIVED_EXAMPLE_H

//Include base class header
#include <transformation.h>

 /*
  * Declare a derived class
  * This project conventions require structures of derived classes to have the following name: 
  * `transformation_<class name>`.
  * Feel free to ignore this convention while creating your own derived class.
  */
typedef struct
{
    //! @cond PRIVATE
    transformation base;            // Add base class as a FIRST member
    int custom_member1;             // Add any desired additional members
    void* custom_member2;
    //! @endcond
} transformation_my_transformation;

/*
 * Declare all required methods.
 *
 * Methods are declared extern because declarations are stored in a separate file. This is not a requirement, so feel
 * free to come up with any other declaration-definition pattern.
 */

/*
 * Declare constructor method.
 *
 * Specify any parameters that your constructor requires.
 *
 * This project conventions require derived class constructor to have the following signature:
 * `void transformation_<class name>_constructor(<custom arguments>, transformation_my_transformation *this)`. 
 * Feel free to ignore this convention while creating your own derived class.
 */
extern void transformation_my_transformation_constructor(int anything, transformation_my_transformation *this);

/*
 * Declare all required overrides.
 *
 * Those methods shall have the same return type as a respective method of a base class.
 *
 * All of those methods shall accept a single argument of a derived time
 *
 * This project conventions require overridden methods to have the following signature:
 * `extern <original return type> transformation_<derived class name>_<original method name>(<derived class type>* this)`.
 * Method and argument names may be altered as user prefers, but return and argument types shall follow the convention.
 */
extern void transformation_my_transformation_destructor(transformation_my_transformation *this);

extern void transformation_my_transformation_transform(transformation_my_transformation *this);

extern bool transformation_my_transformation_finalize(transformation_my_transformation *this);

extern size_t transformation_my_transformation_sink_min(const transformation_my_transformation *this);

extern size_t transformation_my_transformation_source_min(const transformation_my_transformation *this);

/*
 * Declare any additional methods you need for derived class
 *
 * This project conventions require those methods methods to have the following signature:
 * `<desired return type> transformation_<derived class name>_<method name>(<any desired arguments>)`
 * Feel free to ignore this convention while creating your own derived class.
 */
extern void transformation_my_transformation_set(void *ptr, int argument, transformation_my_transformation *this);

extern int transformation_my_transformation_get_result(const transformation_my_transformation *this);

#endif // CTRANSFORM_TRANSFORMATION_DERIVED_EXAMPLE_H

//***********************************************Source code (.c file)**************************************************


/*
 * Create a virtual method table for your derived class
 *
 * Set all methods to your overrides
 *
 * This project conventions require transformation virtual method table object to have the following name:
 * `transformation_call_tab_<class name>`. Feel free to ignore this convention while creating your own derived class.
 */
transformation_call_tab transformation_call_tab_my_transformation =
        {
                .destructor = (void (*)(transformation *))transformation_my_transformation_destructor,
                .transform = (void (*)(transformation *))transformation_my_transformation_transform,
                .finalize = (bool (*)(transformation *))transformation_my_transformation_finalize,
                .sink_min = (size_t (*)(const transformation *))transformation_my_transformation_sink_min,
                .source_min = (size_t (*)(const transformation *))transformation_my_transformation_source_min
        };

/*
 * Define all methods
 */

void transformation_my_transformation_constructor(int anything, transformation_my_transformation *this)
{
    //Set virtual method table for the base class
    this->base.call_tab = &transformation_call_tab_my_transformation;
    //You may init source and sink to NULL or leave it uninitialized
    //Initializing source sink in the constructor with not NULL will break this project's conventions but may be performed
    this->base.source = NULL;
    this->base.sink = NULL;

    //Initialize your custom member variables as required
    this->custom_member1 = anything;
    this->custom_member2 = this;
}

void transformation_my_transformation_destructor(transformation_my_transformation *this)
{
    //Deinitialize variables
    //You may follow the order opposite to initialization, but this does not provide any noticeable advantages here

    //Clear all allocated memory, deinitialize variables
    this->custom_member2 = NULL;
    this->custom_member1 = 0;

    //call_tab, source and sink will be set to NULL in base class destructor
}

void transformation_my_transformation_transform(transformation_my_transformation *this)
{
    /*
     * Read chunk of data from source buffer, transform it and write to sink buffer in this method.
     * We can read and write 10 bytes without size checking, because controller guarantees source and sink buffer to
     * have at least transformation_source_min(this) and transformation_sink_min(this) of available space respectively.
     *
     * Controller also guarantees that if this method is invoked, transformation_finalize(this) was never invoked on
     * on this object.
     *
     * This method is required to either modify input buffer read position (by calling buffer_rinc method),
     * modify output buffer write position (by calling buffer_winc method), or throw an exception (via CEasyException).
     * Not fulfilling this condition results in an infinite loop inside of controller working method.
     */
    memcpy(this->custom_member2, buffer_rpos(this->base.source), 10);
    buffer_rinc(10, this->base.source);
    memset(buffer_wpos(this->base.sink), 1, 10);
    buffer_winc(10, this->base.sink);
    /*
     * You are only allowed to call the following buffer methods from here:
     * - buffer_rpos(&this->base.source) - get pointer to read from
     * - buffer_rpos_w(&this->base.source) - get non const pointer to read from (writing to that pointer results in
     *                   undefined behaviour), it can be used to pass pointer to methods with non const pointer
     *                   argument, if those methods will not write into the pointer
     * - buffer_rinc(&this->base.source) - increase read pointer position
     * - buffer_read_size(&this->base.source) - get amount of readable space
     * - buffer_readable(&this->base.source) - check if there is at least one byte to read
     *
     * - buffer_wpos(&this->base.sink) - get pointer to write to
     * - buffer_winc(&this->base.sink) - increase write pointer position
     * - buffer_write_size(&this->base.sink) - get amount of writable space
     * - buffer_writable(&this->base.sink) - check if there is at least one byte to write into
     */

    //You are also allowed to make any modifications to your custom members in any overridden method
    ++this->custom_member1
}

bool transformation_my_transformation_finalize(transformation_my_transformation *this)
{
    /*
     * Flush internally buffered data into sink buffer in this method.
     * We can write 10 bytes without size checking, because controller guarantees sink buffer to
     * have at least transformation_sink_min(this) of available writable space.
     */
    if(this->custom_member1 > 0)
    {
        memset(buffer_wpos(this->base.sink), 1, 10);
        buffer_winc(10, this->base.sink);
    }
    /*
     * You are only allowed to call the following buffer methods from here:
     * - buffer_wpos(&this->base.sink) - get pointer to write to
     * - buffer_winc(&this->base.sink) - increase write pointer position
     * - buffer_write_size(&this->base.sink) - get amount of writable space
     * - buffer_writable(&this->base.sink) - check if there is at least one byte to write into
     */

    //You are also allowed to make any modifications to your custom members in any overridden method
    --this->custom_member1
     /*
      * Return true if transformation was successfully finalized.
      * Return false if more invocations of this method are required to finalize the transformation.
      *
      * If this method is infinitely invoked in a loop, it shall eventually return true.
      * Not fulfilling this condition results in an infinite loop inside of controller working method.
      */
    return this->custom_member1 == 0;
}

size_t transformation_my_transformation_sink_min(const transformation_my_transformation *this)
{
    /*
     * Set the requirement for a sink space.
     * Controller guarantees sink buffer to have at least transformation_sink_min(this) of available space to write into
     * when transform and finalize methods are invoked.
     *
     * This value shall be higher than zero, and must NOT change as a result of any overridden method invocation
     * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
     * method. Value may change as a result of an invocation of any non overridden method introduced to this class.
     */
    return 10;
}

size_t transformation_my_transformation_source_min(const transformation_my_transformation *this)
{
    /*
     * Set the requirement for a source space.
     * Controller guarantees sink buffer to have at least transformation_source_min(this) of available space to read
     * from when transform method is invoked, and less than transformation_source_min(this) of available space to read
     * from when finalize method is invoked.
     *
     * This value shall be higher than zero, and must NOT change as a result of any overridden method invocation
     * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
     * method. Value may change as a result of an invocation of any non overridden method introduced to this class.
     */
    return 10;
}

//Implement custom methods as required, only limitation is that you are not allowed to modify base class
void transformation_my_transformation_set(void *ptr, int argument, transformation_my_transformation *this)
{
    this->custom_member1 = argument;
    this->custom_member2 = ptr;
}

int transformation_my_transformation_get_result(const transformation_my_transformation *this)
{
    return this->custom_member1;
}