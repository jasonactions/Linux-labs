                                               
 #include <linux/init.h> 
 #include <linux/module.h> 
 #include <linux/kernel.h> 

 MODULE_LICENSE("GPL"); 

 static int trace_printk_demo_init(void) 
 { 
	 trace_printk("Can not see this in trace unless loaded for the second time\n"); 
	 return 0; 
 } 

 static void trace_printk_demo_exit(void) 
 { 
	 trace_printk("Module unloading\n"); 
 } 

 module_init(trace_printk_demo_init); 
 module_exit(trace_printk_demo_exit);
