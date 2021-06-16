#ifdef MEM_OVERRIDE_ON

#undef malloc
#undef realloc
#undef calloc
#undef free
#undef _expand
#undef _msize
#undef new
#undef _aligned_malloc
#undef _aligned_free
#undef _malloc_dbg

#undef MEM_OVERRIDE_ON

#endif