/*!
 * @file sink_derived_example.h
 * @brief Example of sink inheritance
 */

//! @code sink_inheritance_example

//***********************************************Header code (.h file)**************************************************

#ifndef CTRANSFORM_SINK_DERIVED_EXAMPLE_H
#define CTRANSFORM_SINK_DERIVED_EXAMPLE_H

//Include base class header
#include <sink.h>

 /*
  * Declare a derived class
  * This project conventions require structures of derived classes to have the following name: `sink_<class name>`.
  * Feel free to ignore this convention while creating your own derived class.
  */
typedef struct
{
    //! @cond PRIVATE
    sink base;            // Add base class as a FIRST member
    int custom_member1;     // Add any desired additional members
    void* custom_member2;
    //! @endcond
} sink_my_sink;

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
 * `void sink_<class name>_constructor(<custom arguments>, sink_my_sink *this)`. Feel free to ignore this
 * convention while creating your own derived class.
 */
extern void sink_my_sink_constructor(int anything, sink_my_sink *this);

/*
 * Declare all required overrides.
 *
 * Those methods shall have the same return type as a respective method of a base class.
 *
 * All of those methods shall accept a single argument of a derived time
 *
 * This project conventions require overridden methods to have the following signature:
 * `extern <original return type> sink_<derived class name>_<original method name>(<derived class type>* this)`.
 * Method and argument names may be altered as user prefers, but return and argument types shall follow the convention.
 */
extern void sink_my_sink_destructor(sink_my_sink *this);

extern void sink_my_sink_send(sink_my_sink *this);

extern size_t sink_my_sink_source_min(const sink_my_sink *this);

extern bool sink_my_sink_end(const sink_my_sink *this);

/*
 * Declare any additional methods you need for derived class
 *
 * This project conventions require those methods methods to have the following signature:
 * `<desired return type> sink_<derived class name>_<method name>(<any desired arguments>)`
 * Feel free to ignore this convention while creating your own derived class.
 */
extern void sink_my_sink_set(void *ptr, int argument, sink_my_sink *this);

extern int sink_my_sink_get_result(const sink_my_sink *this);

#endif // CTRANSFORM_SINK_DERIVED_EXAMPLE_H

//***********************************************Source code (.c file)**************************************************


/*
 * Create a virtual method table for your derived class
 *
 * Set all methods to your overrides
 *
 * This project conventions require sink virtual method table object to have the following name:
 * `sink_call_tab_<class name>`. Feel free to ignore this convention while creating your own derived class.
 */
sink_call_tab sink_call_tab_my_sink =
        {
                .destructor  = (void (*)(sink *))sink_my_sink_destructor,
                .send        = (void (*)(sink *))sink_my_sink_send,
                .source_min    = (size_t (*)(const sink *))sink_my_sink_source_min,
                .end         = (bool (*)(const sink *))sink_my_sink_end
        };

/*
 * Define all methods
 */

void sink_my_sink_constructor(int anything, sink_my_sink *this)
{
    //Set virtual method table for the base class
    this->base.call_tab = &sink_call_tab_my_sink;
    //You may init sink to NULL or leave it uninitialized
    //Initializing source in the constructor with not NULL will break this project's conventions but may be performed
    this->base.source = NULL;

    //Initialize your custom member variables as required
    this->custom_member1 = anything;
    this->custom_member2 = this;
}

void sink_my_sink_destructor(sink_my_sink *this)
{
    //Deinitialize variables
    //You may follow the order opposite to initialization, but this does not provide any noticeable advantages here

    //Clear all allocated memory, deinitialize variables
    this->custom_member2 = NULL;
    this->custom_member1 = 0;

    //You may set call_tab and sink to NULL, or may leave them expired
    this->base.source = NULL;
    this->base.call_tab = &sink_call_tab_my_sink;
}

void sink_my_sink_send(sink_my_sink *this)
{
    /*
     * Read data from source buffer only in this method.
     * We can read 10 bytes without size checking, because controller guarantees source buffer to have at least
     * sink_source_min(this) of available space to read from.
     *
     * Controller also guarantees that if this method is invoked, sink_end(this) equals to false
     *
     * If sink_end(this) returns false, send method is required to either modify input buffer read position
     * (by calling buffer_rinc method), or throw an exception (via CEasyException). Not fulfilling this condition
     * results in an infinite loop inside of controller working method.
     */
    memcpy(buffer_rpos(this->base.source), 1, 10);
    buffer_rinc(10, this->base.source);
    /*
     * You are only allowed to call the following buffer methods from here:
     * - buffer_rpos - get pointer to read from
     * - buffer_rpos_w - get non const pointer to read from (writing to that pointer results in undefined behaviour),
     *                   it can be used to pass pointer to methods with non const pointer argument, if those methods
     *                   will not write into the pointer
     * - buffer_rinc - increase read pointer position
     * - buffer_read_size - get amount of readable space
     * - buffer_readable - check if there is at least one byte to read
     */

    //You are also allowed to make any modifications to your custom members in any overridden method
    --this->custom_member1
}

size_t sink_my_sink_source_min(const sink_my_sink *this)
{
    /*
     * Set the requirement for a source space.
     * Controller guarantees source buffer to have at least sink_source_min(this) of available space to read from when
     * send method is called.
     *
     * This value shall be higher than zero, and must NOT change as a result of any overridden method invocation
     * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
     * method. Value may change as a result of an invocation of any non overridden method introduced to this class.
     */
    return 10;
}

bool sink_my_sink_end(const sink_my_sink *this)
{
    /*
     * Return true if the sink can't fit any more data
     * This method may always return false when sink is infinite
     */
    return this->custom_member1 == 0;
}

//Implement custom methods as required, only limitation is that you are not allowed to modify base class
void sink_my_sink_set(void *ptr, int argument, sink_my_sink *this)
{
    this->custom_member1 = argument;
    this->custom_member2 = ptr;
}

int sink_my_sink_get_result(const sink_my_sink *this)
{
    return this->custom_member1;
}