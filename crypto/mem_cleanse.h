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
 * @file mem_cleanse.h
 * Declaration of memory buffer cleanup function
 */

#ifndef CTRANSFORM_MEM_CLEANSE_H
#define CTRANSFORM_MEM_CLEANSE_H

#include <stddef.h>

/*!
 * @fn mem_cleanse(void* ptr, size_t len)
 * @brief Cleans memory buffer in a secure way, should be used to clean memory occupied by
 * secure data (e.g. encryption key)
 * @param ptr Pointer to memory buffer
 * @param len Memory buffer length
 */
void mem_cleanse(void* ptr, size_t len);

#endif //CTRANSFORM_MEM_CLEANSE_H
