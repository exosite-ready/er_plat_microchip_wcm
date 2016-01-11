/*
 * malloc_wrapper.c
 *
 *  Created on: Jul 27, 2015
 *      Author: szilveszterb
 */
#include <lib/dlmalloc.h>

/* Don't use newlib module from modules/newlib for now;
 * this is a local copy of the uip_sbrk function*/

// _sbrk_r (newlib) / uip_sbrk (multiple)
static char *heap_ptr;
static int mem_index;

#ifdef USE_MULTIPLE_ALLOCATOR
void* uip_sbrk( ptrdiff_t incr )
#else
void* _sbrk_r( struct _reent* r, ptrdiff_t incr )
#endif
{
  void* ptr;

  // If increment is negative, return -1
  if( incr < 0 )
    return ( void* )-1;

  // Otherwise ask the platform about our memory space (if needed)
  // We do this for all our memory spaces
  while( 1 )
  {
    if( heap_ptr == NULL )
    {
      if( ( heap_ptr = ( char* )platform_get_first_free_ram( mem_index ) ) == NULL )
      {
        ptr = ( void* )-1;
        break;
      }
    }

    // Do we have space in the current memory space?
    if( heap_ptr + incr > ( char* )platform_get_last_free_ram( mem_index )  )
    {
      // We don't, so try the next memory space
      heap_ptr = NULL;
      mem_index ++;
    }
    else
    {
      // Memory found in the current space
      ptr = heap_ptr;
      heap_ptr += incr;
      break;
    }
  }

  return ptr;
}

#ifndef PCEMU
// mallinfo()
struct mallinfo mallinfo( void )
{
#ifdef USE_MULTIPLE_ALLOCATOR
  return dlmallinfo();
#else
  return _mallinfo_r( _REENT );
#endif
}
#endif

#if defined( USE_MULTIPLE_ALLOCATOR ) || defined( USE_SIMPLE_ALLOCATOR )
// Redirect all allocator calls to our dlmalloc/salloc

#ifdef USE_MULTIPLE_ALLOCATOR
#define CNAME( func ) dl##func
#else
#define CNAME( func ) s##func
#endif

/*
void* _malloc_r( struct _reent* r, size_t size )
{
  return CNAME( malloc )( size );
}

void* _calloc_r( struct _reent* r, size_t nelem, size_t elem_size )
{
  return CNAME( calloc )( nelem, elem_size );
}

void _free_r( struct _reent* r, void* ptr )
{
  CNAME( free )( ptr );
}

void* _realloc_r( struct _reent* r, void* ptr, size_t size )
{
  return CNAME( realloc )( ptr, size );
}
*/
#endif

/*
 * Use microchip xc32's wrapper mechanism:
 * --wrap malloc --wrap free --wrap_realloc and --wrap calloc is used
 * This way the compiler substitutes all malloc, etc call with __wrap_malloc, etc
 * call
 * */
/*
 * USE DLMALLOC!!!!
 * !!!!Don't use microchip malloc!!!
 * Microchip realloc has a serious bug: it can overwrite our buffer if realloc is called
 * It happens at specific constellations, the details are not yet clear, but it was caught
 * with USE_ADVANCED_MEM_CHECK. It usually happens after the 4th realloc
 * */
#define MICROCHIP_USE_DLMALLOC

#ifdef MICROCHIP_USE_DLMALLOC
void* __wrap_malloc(size_t size)
{
  return dlmalloc(size);
}

void __wrap_free(void *ptr)
{
  dlfree(ptr);
}

void* __wrap_realloc(void *ptr, size_t size)
{
  return dlrealloc(ptr, size);
}

void* __wrap_calloc(size_t num, size_t size)
{
  return dlcalloc(num, size);
}
#else
void* __wrap_malloc(size_t size)
{
  return __real_malloc(size);
}

void __wrap_free(void *ptr)
{
  __real_free(ptr);
}

void* __wrap_realloc(void *ptr, size_t size)
{
  return __real_realloc(ptr, size);
}

void* __wrap_calloc(size_t num, size_t size)
{
  return __real_calloc(num, size);
}
#endif




