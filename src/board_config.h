/*
 * board_config.h
 *
 *  Created on: Jul 27, 2015
 *      Author: szilveszterb
 */

#ifndef BOARD_CONFIG_H_
#define BOARD_CONFIG_H_

extern unsigned int _heap __attribute__((section(".data")));
extern unsigned int _splim __attribute__((section(".data")));
#define HEAP_START &_heap
#define HEAP_END   (&_splim - 1)

#endif /* BOARD_CONFIG_H_ */
