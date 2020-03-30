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
 * @file transformation_decrypt.h
 * Declaration of decrypt transformation class and methods
 */

#ifndef CTRANSFORM_TRANSFORMATION_DECRYPT_H
#define CTRANSFORM_TRANSFORMATION_DECRYPT_H

#include <transformation.h>

/*!
 * @class transformation_decrypt
 * @brief A class that implements data decryption transformation
 * @inherit transformation
 */
typedef struct
{
    //! @cond PRIVATE
    transformation base;
    struct evp_cipher_ctx_st *ctx;
    size_t block_size;
    bool empty;
    //! @endcond
} transformation_decrypt;

/*!
 * @fn transformation_decrypt_constructor(const char *cipher, const char *digest, const char *key,
                                          transformation_decrypt *this)
 * @brief Construct an decryption transformation
 * @memberof transformation_decrypt
 * @param cipher Cipher name to use
 * @param digest Digest (hash sum) name to use
 * @param key Decryption key string
 * @param this Pointer to "this" object
 * @throws EINVAL Cipher or digest does not exist
 * @throws EINVAL Key is too long
 * @throws ENOMEM Not enough memory to initialize internal state
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Variable length key is transformed into a fixed length key with provided digest
 *
 * To list ciphers available on your machine execute the following command:
 * `openssl list -cipher-algorithms`
 * 
 * To list digests available on your machine execute the following command:
 * `openssl list -digest-algorithms`
 *
 * Refer to openssl documentation for more info on available ciphers and digests.
 */
extern void transformation_decrypt_constructor(const char *cipher, const char *digest, const char *key,
                                               transformation_decrypt *this);

/*!
 * @fn transformation_decrypt_destructor(transformation_decrypt *this)
 * @brief Destructs this transformation
 * @memberof transformation_decrypt
 * @param this Pointer to "this" object
 *
 * Overrides @ref transformation_destructor(transformation *) method.
 */
extern void transformation_decrypt_destructor(transformation_decrypt *this);

/*!
 * @fn transformation_decrypt_transform(transformation_decrypt *this)
 * @brief Transform one block of data
 * @memberof transformation_decrypt
 * @param this Pointer to "this" object
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Overrides @ref transformation_transform(transformation *) method.
 */
extern void transformation_decrypt_transform(transformation_decrypt *this);

/*!
 * @fn transformation_decrypt_finalize(transformation_decrypt *this)
 * @brief Flushes internal buffers
 * @memberof transformation_decrypt
 * @param this Pointer to "this" object
 * @return True if all buffers were flushed, false otherwise
 * @throws ENOANO Any openssl error (call ERR_print_errors_fp(stderr) from <openssl/err.h> to print the error)
 *
 * Overrides @ref transformation_finalize(transformation *) method.
 */
extern bool transformation_decrypt_finalize(transformation_decrypt *this);

/*!
 * @fn transformation_decrypt_sink_min(const transformation_decrypt *this)
 * @brief Returns minimum required amount of writable space in sink
 * @memberof transformation_decrypt
 * @param this Pointer to "this" object
 * @return Minimum required amount of writable space in sink
 *
 * Overrides @ref transformation_sink_min(const transformation *) method.
 */
extern size_t transformation_decrypt_sink_min(const transformation_decrypt *this);

/*!
 * @fn transformation_decrypt_source_min(const transformation_decrypt *this)
 * @brief Returns minimum required amount of readable space in source
 * @memberof transformation_decrypt
 * @param this Pointer to "this" object
 * @return Minimum required amount of readable space in source
 *
 * Overrides @ref transformation_source_min(const transformation *) method.
 */
extern size_t transformation_decrypt_source_min(const transformation_decrypt *this);

#endif //CTRANSFORM_TRANSFORMATION_DECRYPT_H
