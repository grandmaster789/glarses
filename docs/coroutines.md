# Assorted Notes on Coroutines

C++20 coroutines are stackless, with *many* customization points. A function becomes a coroutine if it mentions one of the 3 coroutine keywords (co_await, co_yield and co_return). It also requires many details to be specified in the return type.

Upon suspending a coroutine, its stack frame is stored on the heap. When resuming, a stored stack frame is copied back. The stack frame that was stored on the heap needs to be destroyed at some point - this is also a customization point.

All the customization is bundled in the return type, which also specifies what 'promise' to use. 

Given a function:

    Task some_work() {
        cudaStream_t stream;
        cudaStreamCreate(stream);

        gpu_matmul<<<8, 256, 0>>>(matA, matB);

        while (cudaStreamQuery(stream) != cudaSuccess)
            co_await std::suspend_always{};

        cudaStreamDestroy(stream);
    }

This will be rewritten (kind of) to the following:

    Task some_work() {
        Task::promise_type some_promise;                       // we use the promise type as specified in the return type

        auto return_object = some_promise.get_return_object();

        try {
            co_await some_promise.initial_suspend();

            // function body from original is put here
            cudaStream_t stream;
            cudaStreamCreate(stream);
    
            gpu_matmul<<<8, 256, 0>>>(matA, matB);
    
            while (cudaStreamQuery(stream) != cudaSuccess)
                co_await std::suspend_always{};
    
            cudaStreamDestroy(stream);
        }
        catch (...) {
            some_promise.unhandled_exception();
        }

        co_await some_promise.final_suspend();
    }

Another line in here:
    
    co_await std::suspend_always{};

Will expand to:

    auto&& awaiter = std::suspend_always{};
    
    if (!awaiter.await_ready())
        awaiter.await_suspend(std::coroutine_handle<> the_generated_coroutine_handle);
    
    awaiter.await_resume();

co_await needs an Awaitable, which holds additional customization points, but for your own sanity just use either *std::suspend_always* or *std::suspend_never*.

Actual implementation of suspend_always:

    namespace std {
        struct suspend_always {
            constexpr bool await_ready()                     const noexcept { return false; }
            constexpr void await_suspend(coroutine_handle<>) const noexcept {}
            constexpr void await_resume()                    const noexcept {}
        };
    }

These do not *need* to be constexpr, but when they are they allow for additional optimizations. Also, the await_suspend has 3 possible signatures:

    void await_suspend(...)               -> always suspend
    bool await_suspend(...)               -> suspend if this returns true
    coroutine_handle<> await_suspend(...) -> resume the returned coroutine (symmetric coroutine transfer)

The promise controls coroutine behaviour:
- initial_suspend
- final_suspend
- exception handling

The Awaitable controls suspension point behaviour
- co_await std::suspend_always{};

The coroutine handle is like a pointer to the coroutine. It also allows access to both the promise type and the raw pointer to the coroutine frame address.
- std::coroutine_handle<PromiseType>::fromPromise(promise_instance); //  == (promise reference -> coroutine handle)
- void* ptr = handle.address();                                       // == (coroutine handle to raw memory address)
- std::coroutine_handle<>::from_address(ptr);                         // == (raw memory location to coroutine handle)
- handle.promise();                                                   // == (coroutine handle to promise instance)

Other functions on the coroutine handle:
- handle.resume();  // resume a suspended routine
- handle.done();    // check if the coroutine has completed
- handle.destroy(); // destroy the coroutine (delete the stack frame)

# References
- Introduction to C++ Coroutines through a Thread scheduling demonstration https://www.youtube.com/watch?v=kIPzED3VD3w
- Implementing a C++ Coroutine Task from Scratch - Dietmar Kühl - ACCU 2023 https://www.youtube.com/watch?v=Npiw4cYElng&t=1s