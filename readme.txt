how to flash:
st-flash --format ihex write stm32.hex

enable semihosting:
1.  add in linker
    "PROVIDE ( __end__ = . );" in ._user_heap_stack
2. add in CMakeLists
    set(LINKER_FLAGS "-lc -l rdimon --specs=rdimon.specs -Wl,--print-memory-usage")
    instead of
    set(LINKER_FLAGS "--specs=nosys.specs -Wl,--print-memory-usage")
3. add in main.cpp
    extern "C" void initialise_monitor_handles(void);
    and call initialise_monitor_handles();
4. use printf
5. use "monitor arm semihosting enable" in GDB


