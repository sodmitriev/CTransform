/*
 * Copyright (C) 2020 Sviatoslav Dmitriev
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*!
 * @file source_derived_example.h
 * @brief Example of source inheritance
 */

//! @cond EXAMPLES

//***********************************************Header code (.h file)**************************************************

#ifndef CTRANSFORM_SOURCE_DERIVED_EXAMPLE_H
#define CTRANSFORM_SOURCE_DERIVED_EXAMPLE_H

//Include base class header
#include <source.h>

 /*
  * Declare a derived class
  * This project conventions require structures of derived classes to have the following name: `source_<class name>`.
  * Feel free to ignore this convention while creating your own derived class.
  */
typedef struct
{
    //! @cond PRIVATE
    source base;            // Add base class as a FIRST member
    int custom_member1;     // Add any desired additional members
    void* custom_member2;
    //! @endcond
} source_my_source;

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
 * `void source_<class name>_constructor(<custom arguments>, source_my_source *this)`. Feel free to ignore this
 * convention while creating your own derived class.
 */
extern void source_my_source_constructor(int anything, source_my_source *this);

/*
 * Declare all required overrides.
 *
 * Those methods shall have the same return type as a respective method of a base class.
 *
 * All of those methods shall accept a single argument of a derived time
 *
 * This project conventions require overridden methods to have the following signature:
 * `extern <original return type> source_<derived class name>_<original method name>(<derived class type>* this)`.
 * Method and argument names may be altered as user prefers, but return and argument types shall follow the convention.
 */
extern void source_my_source_destructor(source_my_source *this);

extern void source_my_source_send(source_my_source *this);

extern size_t source_my_source_sink_min(const source_my_source *this);

extern bool source_my_source_end(const source_my_source *this);

/*
 * Declare any additional methods you need for derived class
 *
 * This project conventions require those methods methods to have the following signature:
 * `<desired return type> source_<derived class name>_<method name>(<any desired arguments>)`
 * Feel free to ignore this convention while creating your own derived class.
 */
extern void source_my_source_set(void *ptr, int argument, source_my_source *this);

extern int source_my_source_get_result(const source_my_source *this);

#endif // CTRANSFORM_SOURCE_DERIVED_EXAMPLE_H

//***********************************************Source code (.c file)**************************************************


/*
 * Create a virtual method table for your derived class
 *
 * Set all methods to your overrides
 *
 * This project conventions require source virtual method table object to have the following name:
 * `source_call_tab_<class name>`. Feel free to ignore this convention while creating your own derived class.
 */
source_call_tab source_call_tab_my_source =
        {
                .destructor  = (void (*)(source *))source_my_source_destructor,
                .send        = (void (*)(source *))source_my_source_send,
                .sink_min    = (size_t (*)(const source *))source_my_source_sink_min,
                .end         = (bool (*)(const source *))source_my_source_end
        };

/*
 * Define all methods
 */

void source_my_source_constructor(int anything, source_my_source *this)
{
    //Set virtual method table for the base class
    this->base.call_tab = &source_call_tab_my_source;
    //You may init sink to NULL or leave it uninitialized
    //Initializing sink in the constructor with not NULL will break this project's conventions but may be performed
    this->base.sink = NULL;

    //Initialize your custom member variables as required
    this->custom_member1 = anything;
    this->custom_member2 = this;
}

void source_my_source_destructor(source_my_source *this)
{
    //Deinitialize variables
    //You may follow the order opposite to initialization, but this does not provide any noticeable advantages here

    //Clear all allocated memory, deinitialize variables
    this->custom_member2 = NULL;
    this->custom_member1 = 0;

    //call_tab and sink will be set to NULL in base class destructor
}

void source_my_source_send(source_my_source *this)
{
    /*
     * Write data to sink buffer only in this method.
     * We can write 10 bytes without size checking, because controller guarantees sink buffer to have at least
     * source_sink_min(this) of available space to write into.
     *
     * Controller also guarantees that if this method is invoked, source_end(this) equals to false
     *
     * If source_end(this) returns false, send method is required to either modify output buffer write position
     * (by calling buffer_winc method), or throw an exception (via CEasyException). Not fulfilling this condition
     * results in an infinite loop inside of controller working method.
     */
    memset(buffer_wpos(this->base.sink), 1, 10);
    buffer_winc(10, this->base.sink);
    /*
     * You are only allowed to call the following buffer methods from here:
     * - buffer_wpos - get pointer to write to
     * - buffer_winc - increase write pointer position
     * - buffer_write_size - get amount of writable space
     * - buffer_writable - check if there is at least one byte to write into
     */

    //You are also allowed to make any modifications to your custom members in any overridden method
    --this->custom_member1
}

size_t source_my_source_sink_min(const source_my_source *this)
{
    /*
     * Set the requirement for a sink space.
     * Controller guarantees sink buffer to have at least source_sink_min(this) of available space to write into when
     * send method is called.
     *
     * This value shall be higher than zero, and must NOT change as a result of any overridden method invocation
     * (except for destructor). Not fulfilling this condition results in an infinite loop inside of controller working
     * method. Value may change as a result of an invocation of any non overridden method introduced to this class.
     */
    return 10;
}

bool source_my_source_end(const source_my_source *this)
{
    /*
     * Return true if the source has no more data
     * This method may always return false when source is infinite
     */
    return this->custom_member1 == 0;
}

//Implement custom methods as required, only limitation is that you are not allowed to modify base class
void source_my_source_set(void *ptr, int argument, source_my_source *this)
{
    this->custom_member1 = argument;
    this->custom_member2 = ptr;
}

int source_my_source_get_result(const source_my_source *this)
{
    return this->custom_member1;
}

//! @endcond
