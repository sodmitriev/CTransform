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
 * @file transformation_hash.h
 * Declaration of hash transformation class and methods
 */

#ifndef CTRANSFORM_TRANSFORMATION_HASH_H
#define CTRANSFORM_TRANSFORMATION_HASH_H

#include <transformation.h>

/*!
 * @class transformation_hash
 * @brief A class that implements hash transformation
 * @inherit transformation
 *
 * This transformation reads data from the source, and puts hash sum of that data into the sink
 */
typedef struct
{
    transformation base;
    struct evp_md_ctx_st *ctx;
    size_t md_size;
} transformation_hash;

/*!
 * @fn transformation_hash_constructor(const char *cipher, const char *digest, const char *key,
                                          transformation_hash *this)
 * @brief Construct an hash sum transformation
 * @memberof transformation_hash
 * @param digest Digest (hash sum) name to use
 * @param this Pointer to "this" object
 * @throws EINVAL Cipher or digest does not exist
 * @throws EINVAL Key is too long
 * @throws ENOMEM Not enough memory to initialize internal state
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * To list ciphers available on your machine execute the following command:
 * `openssl list -cipher-algorithms`
 *
 * Refer to openssl documentation for more info on available digests.
 */
extern void transformation_hash_constructor(const char *digest, transformation_hash *this);

/*!
 * @fn transformation_hash_destructor(transformation_hash *this)
 * @brief Destructs this transformation
 * @memberof transformation_hash
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_hash_destructor(transformation_hash *this);

/*!
 * @fn transformation_hash_transform(transformation_hash *this)
 * @brief Transform one block of data
 * @memberof transformation_hash
 * @param this Pointer to "this" object
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_hash_transform(transformation_hash *this);

/*!
 * @fn transformation_hash_finalize(transformation_hash *this)
 * @brief Flushes internal buffers
 * @memberof transformation_hash
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_hash_finalize(transformation_hash *this);

/*!
 * @fn transformation_hash_sink_min(const transformation_hash *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_hash
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_hash_sink_min(const transformation_hash *this);

/*!
 * @fn transformation_hash_source_min(const transformation_hash *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_hash
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_hash_source_min(const transformation_hash *this);

#endif //CTRANSFORM_TRANSFORMATION_HASH_H
