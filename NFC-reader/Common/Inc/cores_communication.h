/*
 * cores_communication.h
 *
 *  Created on: 23. 05. 2024
 *      Author: Petr Kucera
 *              petr@khome.cz
 *
 *
 * Copyright (c) 2024 Petr Kucera
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 all
 * copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef CORES_COMMUNICATION_H_
#define CORES_COMMUNICATION_H_

#ifndef BUFFSHAREDSIZE
#define BUFFSHAREDSIZE 128
#endif

/**
 * @brief Used to clear the buffer locks and their size holders
 *        It has to be called in only one of the cores
 */
void core_share_init(void);

/**
 * @brief Send data from M7 to M4
 * @param buffer
 * @param size
 * @return -1 if lock is not acquired, otherwise how many items were transferred
 */
int put_to_m4(const int *const restrict buffer, const unsigned int size);

/**
 * @brief Get data from M4
 * @param buffer
 * @param size
 * @return -1 if lock is not acquired, otherwise how many items were read
 */
int get_from_m4(int *const restrict buffer, unsigned int size);

/**
 * @brief Send data from M4 to M7
 * @param buffer
 * @param size
 * @return -1 if lock is not acquired, otherwise how many items were transferred
 */
int put_to_m7(const int *const restrict buffer, const unsigned int size);

/**
 * @brief Get data from M7
 * @param buffer
 * @param size
 * @return -1 if lock is not acquired, otherwise how many items were read
 */
int get_from_m7(int *const restrict buffer, unsigned int size);

#endif /* CORES_COMMUNICATION_H_ */