1. **Storage Duration, Lifetime & Object Model**

   * storage duration categories: automatic, static, thread, dynamic
   * object lifetime vs storage lifetime
   * lifetime begin/end rules
   * initialization vs construction
   * destruction sequencing
   * temporary objects and lifetime extension
   * materialization of prvalues (C++17+)
   * active object in storage
   * complete objects vs subobjects
   * most vexing lifetime bugs in interviews

2. **Stack, Heap & Memory Layout**

   * stack frame behavior
   * heap allocation model
   * global/static memory regions
   * text/data/bss segments
   * alignment and padding
   * cache locality basics relevant to allocation
   * object layout guarantees
   * `sizeof`, `alignof`, `alignas`
   * over-aligned types
   * placement in raw storage

3. **RAII & Ownership Semantics**

   * RAII fundamentals
   * deterministic destruction
   * resource ownership models
   * unique ownership
   * shared ownership
   * non-owning references/views
   * ownership transfer
   * exception safety through RAII
   * scope guards
   * common ownership anti-patterns

4. **Copy/Move Semantics & Special Member Functions**

   * Rule of 3/5/0
   * copy ctor/assignment
   * move ctor/assignment
   * deleted/defaulted functions
   * implicit generation rules
   * trivial/non-trivial types
   * shallow vs deep copy
   * moved-from states
   * copy elision and NRVO
   * value categories relevance to lifetime

5. **Dynamic Memory Allocation Internals**

   * `new` / `delete`
   * `new[]` / `delete[]`
   * allocation vs construction
   * deallocation vs destruction
   * placement new
   * operator new/operator delete overloads
   * nothrow new
   * allocation failure handling
   * sized deallocation
   * fragmentation concepts
   * allocator interaction basics

6. **Smart Pointers & Managed Lifetime**

   * `unique_ptr`
   * `shared_ptr`
   * `weak_ptr`
   * custom deleters
   * aliasing constructor
   * control block internals
   * cyclic references
   * `enable_shared_from_this`
   * ownership APIs
   * performance costs
   * smart pointer pitfalls in APIs/interviews

7. **Manual Lifetime Management & Low-Level Memory**

   * raw storage manipulation
   * placement new patterns
   * explicit destructor calls
   * unions and active members
   * `std::launder`
   * object reuse in same storage
   * type punning legality
   * strict aliasing
   * byte-level memory access
   * trivially copyable types
   * memory pools/arenas fundamentals

8. **Initialization Semantics**

   * zero/default/value/direct/list initialization
   * aggregate initialization
   * initialization order
   * static initialization order fiasco
   * constant initialization
   * member initialization order
   * reference member lifetime issues
   * temporary binding rules
   * narrowing conversions in brace init
   * lazy initialization patterns

9. **Concurrency & Memory Lifetime Interactions**

   * thread-local storage lifetime
   * object sharing across threads
   * lifetime races/use-after-free
   * atomic ownership patterns
   * synchronization protecting lifetime
   * destruction ordering in multithreading
   * hazard patterns with detached threads
   * shared_ptr thread-safety guarantees
   * ABA-style lifetime issues (high level)
   * lock-free reclamation awareness

10. **Undefined Behavior & Memory Safety Bugs**

* dangling pointers/references
* use-after-free
* double delete
* invalid frees
* buffer overflows
* uninitialized memory
* lifetime violations
* iterator/reference invalidation
* null dereference nuances
* strict aliasing UB
* alignment UB
* sanitizers: ASan, UBSan, MSan basics
