#undef TRACE_SYSTEM
#define TRACE_SYSTEM test_driver  
  
#if !defined(_TRACE_TEST_DRIVER_H) || defined(TRACE_HEADER_MULTI_READ)  
#define _TRACE_TEST_DRIVER_H  
  
#include <linux/tracepoint.h>  
  
/** 
 * Two methodes can achieve capture trace. 
 * 1. Dedine trace class, can include many events. 
 * 2. Direct definition trace one event. 
 */  
  
/*method 1*/  
DECLARE_EVENT_CLASS(get_cpu_info,  
    TP_PROTO(unsigned long cpu_id, unsigned long curr_freq,  
             unsigned long max_freq),  
    TP_ARGS(cpu_id, curr_freq, max_freq),  
  
    TP_STRUCT__entry(  
        __field(unsigned long, cpu_id    )  
        __field(unsigned long, curr_freq   )  
        __field(unsigned long, max_freq )  
       ),  
  
    TP_fast_assign(  
        __entry->cpu_id = cpu_id;  
        __entry->curr_freq = curr_freq;  
        __entry->max_freq = max_freq;  
    ),  
  
    TP_printk("cpu=%lu curr_freq=%lu max_freq=%lu",  
          __entry->cpu_id, __entry->curr_freq,  
          __entry->max_freq)  
);  
  
DEFINE_EVENT(get_cpu_info, get_test_driver_data,  
    TP_PROTO(unsigned long cpu_id, unsigned long curr_freq,  
         unsigned long max_freq),  
    TP_ARGS(cpu_id, curr_freq, max_freq)  
);  
  
  
#if 0  
/*method 2*/  
TRACE_EVENT(get_test_driver_data,  
    TP_PROTO(unsigned long cpu_id, unsigned long curr_freq,  
        unsigned long max_freq),  
  
    TP_ARGS(cpu_id, curr_freq, max_freq),  
  
    TP_STRUCT__entry(  
        __field(unsigned long, cpu_id    )  
        __field(unsigned long, curr_freq   )  
        __field(unsigned long, max_freq )  
       ),  
  
    TP_fast_assign(  
        __entry->cpu_id = cpu_id;  
        __entry->curr_freq = curr_freq;  
        __entry->max_freq = max_freq;  
    ),  
  
    TP_printk("cpu=%lu curr_freq=%lu max_freq=%lu",  
          __entry->cpu_id, __entry->curr_freq,  
          __entry->max_freq)  
);  
#endif  
  
#endif /* _TRACE_TEST_DRIVER_H */  
  
/* This part must be outside protection */  
#include <trace/define_trace.h>  
