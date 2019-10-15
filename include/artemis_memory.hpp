#ifndef ARTEMIS_MEMORY_HPP
#define ARTEMIS_MEMORY_HPP

#include <stddef.h>
#include <cstdint>

//I don't really have a clue what I'm doing with this memory stuff,
//place holding it for now with pure virtual classes

//Probably end up as the main game allocator
class IArena {
  public:
  virtual ~IArena() = default;
  virtual void* allocate(size_t) = 0; 
  virtual void deallocate(void*) = 0;
};

class StupidArena : virtual public IArena {
  public:
  StupidArena() = default;
  ~StupidArena() = default;
  void* allocate(size_t);
  void deallocate(void*);
  private:
  StupidArena(StupidArena&&) = delete;
  StupidArena(const StupidArena&) = delete;
};

IArena* newArena(size_t);

void destroyArena(IArena*);

#endif
