
#ifndef H_COMMON
#define H_COMMON

#define MAXPATHLEN 1024

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "lv2.h"

#define HVSC_SYSCALL			811                  	// which syscall to overwrite with hvsc redirect
#define HVSC_SYSCALL_ADDR		0x8000000000195540ULL	// where above syscall is in lv2
#define NEW_POKE_SYSCALL		813                  	// which syscall to overwrite with new poke
#define NEW_POKE_SYSCALL_ADDR	0x8000000000195A68ULL	// where above syscall is in lv2
#define SYSCALL_TABLE			0x8000000000346570ULL
#define SYSCALL_PTR(n)			(SYSCALL_TABLE + 8 * (n))

#define HV_BASE					0x8000000014000000ULL	// where in lv2 to map lv1
#define HV_SIZE					0x001000				// 0x1000 (we need 4k from lv1 only)
#define HV_PAGE_SIZE			0x0c					// 4k = 0x1000 (1 << 0x0c)
#define	HV_START_OFFSET			0x363000				// remove lv2 protection
#define HV_OFFSET				0x000a78				// at address 0x363a78

#endif

