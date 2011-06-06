
#include <stdint.h>
#include <sys/synchronization.h>

#if defined(__cplusplus)
#define INLINE inline
#elif defined(__GNUC__)
#define INLINE __inline__
#else
#error "Error"
#endif

typedef struct {
    uint32_t value;
    sys_mutex_t mutex;
    sys_cond_t cond;
} sem_t;

typedef struct {
    unsigned value : 1;
    sys_mutex_t mutex;
    sys_cond_t cond;
} bsem_t;

INLINE static int32_t
sem_init (sem_t *sem, uint32_t value)
{
    int32_t ret;
    sys_mutex_attribute_t mutex_attr;
    sys_cond_attribute_t cond_attr;

    sem->value = value;

    sys_mutex_attribute_initialize (mutex_attr);
    sys_cond_attribute_initialize (cond_attr);
    if ((ret = sys_mutex_create (&(sem->mutex), &mutex_attr)) != CELL_OK) {
	return ret;
    }
    if ((ret = sys_cond_create (&(sem->cond), sem->mutex, &cond_attr))
	!= CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
sem_destroy (sem_t *sem)
{
    int32_t ret;

    if ((ret = sys_cond_destroy (sem->cond)) != CELL_OK) {
	return ret;
    }
    if ((ret = sys_mutex_destroy (sem->mutex)) != CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
sem_wait (sem_t *sem)
{
    int32_t ret;

    if ((ret = sys_mutex_lock (sem->mutex, 0)) != CELL_OK) {
	return ret;
    }
    if (sem->value == 0) {
	if ((ret = sys_cond_wait (sem->cond, 0)) != CELL_OK) {
	    return ret;
	}
    }
    --sem->value;
    if ((ret = sys_mutex_unlock (sem->mutex)) != CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
sem_post (sem_t *sem)
{
    int32_t ret;

    if ((ret = sys_mutex_lock (sem->mutex, 0)) != CELL_OK) {
	return ret;
    }
    ++sem->value;
    if ((ret = sys_mutex_unlock (sem->mutex)) != CELL_OK) {
	return ret;
    }
    if ((ret = sys_cond_signal (sem->cond)) != CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
bsem_init (bsem_t *sem, uint32_t value)
{
    int32_t ret;
    sys_mutex_attribute_t mutex_attr;
    sys_cond_attribute_t cond_attr;

    if ((value & ~1) != 0) {
	return EINVAL;
    }

    sem->value = value;

    sys_mutex_attribute_initialize (mutex_attr);
    sys_cond_attribute_initialize (cond_attr);
    if ((ret = sys_mutex_create (&(sem->mutex), &mutex_attr)) != CELL_OK) {
	return ret;
    }
    if ((ret = sys_cond_create (&(sem->cond), sem->mutex, &cond_attr))
	!= CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
bsem_destroy (bsem_t *sem)
{
    int32_t ret;

    if ((ret = sys_cond_destroy (sem->cond)) != CELL_OK) {
	return ret;
    }
    if ((ret = sys_mutex_destroy (sem->mutex)) != CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
bsem_wait (bsem_t *sem)
{
    int32_t ret;

    if ((ret = sys_mutex_lock (sem->mutex, 0)) != CELL_OK) {
	return ret;
    }
    if (sem->value == 0) {
	if ((ret = sys_cond_wait (sem->cond, 0)) != CELL_OK) {
	    return ret;
	}
    }
    sem->value = 0;
    if ((ret = sys_mutex_unlock (sem->mutex)) != CELL_OK) {
	return ret;
    }

    return CELL_OK;
}

INLINE static int32_t
bsem_post (bsem_t *sem)
{
    int32_t ret;

    if ((ret = sys_mutex_lock (sem->mutex, 0)) != CELL_OK) {
	return ret;
    }
    sem->value = 1;
    if ((ret = sys_mutex_unlock (sem->mutex)) != CELL_OK) {
	return ret;
    }
    if ((ret = sys_cond_signal (sem->cond)) != CELL_OK) {
	return ret;
    }

    return CELL_OK;
}
