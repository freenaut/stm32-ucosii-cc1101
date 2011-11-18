/*
*********************************************************************************************************
*                                                uC/OS-II
*                                          The Real-Time Kernel
*                                  uC/OS-II Configuration File for V2.8x
*
*                               (c) Copyright 2005-2009, Micrium, Weston, FL
*                                          All Rights Reserved
*
*
* File    : OS_CFG.H
* By      : Jean J. Labrosse
* Version : V2.91
*
* LICENSING TERMS:
* ---------------
*   uC/OS-II is provided in source form for FREE evaluation, for educational use or for peaceful research.
* If you plan on using  uC/OS-II  in a commercial product you need to contact Micri祄 to properly license
* its use in your product. We provide ALL the source code for your convenience and to help you experience
* uC/OS-II.   The fact that the  source is provided does  NOT  mean that you can use it without  paying a
* licensing fee.
*********************************************************************************************************
*/

#ifndef OS_CFG_H
#define OS_CFG_H


 /* ---------------------- 综合配置 ----------------------- *//* ---------------------- MISCELLANEOUS ----------------------- */
#define OS_APP_HOOKS_EN           0u   /* 应用接口 *//* Application-defined hooks are called from the uC/OS-II hooks */
#define OS_ARG_CHK_EN             1u   /* 竞争检查 *//* Enable (1) or Disable (0) argument checking                  */
#define OS_CPU_HOOKS_EN           1u   /* 处理器接口文件 *//* uC/OS-II hooks are found in the processor port files         */

#define OS_DEBUG_EN               0u   /* 调试变量 *//* Enable(1) debug variables                                    */

#define OS_EVENT_MULTI_EN         1u   /* 使用 OSEventPendMulti() *//* Include code for OSEventPendMulti()                          */
#define OS_EVENT_NAME_EN          1u   /* Enable names for Sem, Mutex, Mbox and Q                      */

#define OS_LOWEST_PRIO           63u   /* 最低优先级*//* Defines the lowest priority that can be assigned ...         */
                                       /* ... MUST NEVER be higher than 254!                           */

#define OS_MAX_EVENTS            10u   /* 最大应用事件控制块数量 *//* Max. number of event control blocks in your application      */
#define OS_MAX_FLAGS              5u   /* 最大应用事件标志数量 *//* Max. number of Event Flag Groups    in your application      */
#define OS_MAX_MEM_PART           5u   /* 最大存储器分配数量 *//* Max. number of memory partitions                             */
#define OS_MAX_QS                 4u   /* 最大应用队列控制块数量 *//* Max. number of queue control blocks in your application      */
#define OS_MAX_TASKS             20u   /* 最大应用任务数量, 一定>=2 *//* Max. number of tasks in your application, MUST be >= 2       */

#define OS_SCHED_LOCK_EN          1u   /* 使用 OSSchedLock() 和 OSSchedUnlock() *//* Include code for OSSchedLock() and OSSchedUnlock()           */

#define OS_TICK_STEP_EN           1u   /* 使能步节拍 *//* Enable tick stepping feature for uC/OS-View                  */
#define OS_TICKS_PER_SEC       1000u   /* 设定一秒节拍的数量 *//* Set the number of ticks in one second                        */


/* --------------------- 任务堆栈大小 ---------------------- *//* --------------------- TASK STACK SIZE ---------------------- */
#define OS_TASK_TMR_STK_SIZE    128u   /* 定时器任务堆栈大小 (如果 OS_STK 使能) *//* Timer      task stack size (# of OS_STK wide entries)        */
#define OS_TASK_STAT_STK_SIZE   128u   /* 统计任务堆栈大小 (如果 OS_STK 使能) *//* Statistics task stack size (# of OS_STK wide entries)        */
#define OS_TASK_IDLE_STK_SIZE   128u   /* 空闲任务堆栈大小 (如果 OS_STK 使能) *//* Idle       task stack size (# of OS_STK wide entries)        */


/* --------------------- 任务管理 ---------------------- *//* --------------------- TASK MANAGEMENT ---------------------- */
#define OS_TASK_CHANGE_PRIO_EN    1u   /* 使用 OSTaskChangePrio() *//*     Include code for OSTaskChangePrio()                      */
#define OS_TASK_CREATE_EN         1u   /* 使用 OSTaskCreate() *//*     Include code for OSTaskCreate()                          */
#define OS_TASK_CREATE_EXT_EN     1u   /* 使用 OSTaskCreateExt() *//*     Include code for OSTaskCreateExt()                       */
#define OS_TASK_DEL_EN            1u   /* 使用 OSTaskDel() *//*     Include code for OSTaskDel()                             */
#define OS_TASK_NAME_EN           1u   /*     Enable task names                                        */
#define OS_TASK_PROFILE_EN        1u   /* OS_TCB 的包含变量 *//*     Include variables in OS_TCB for profiling                */
#define OS_TASK_QUERY_EN          1u   /* 使用 OSTaskQuery() *//*     Include code for OSTaskQuery()                           */
#define OS_TASK_REG_TBL_SIZE      1u   /*     Size of task variables array (#of INT32U entries)        */
#define OS_TASK_STAT_EN           1u   /* 使能(1)或禁止(0)统计任务 *//*     Enable (1) or Disable(0) the statistics task             */
#define OS_TASK_STAT_STK_CHK_EN   1u   /* 从统计任务堆栈检查任务  *//*     Check task stacks from statistic task                    */
#define OS_TASK_SUSPEND_EN        1u   /* 使用 OSTaskSuspend() 和 OSTaskResume() *//*     Include code for OSTaskSuspend() and OSTaskResume()      */
#define OS_TASK_SW_HOOK_EN        1u   /* 使用 OSTaskSwHook() *//*     Include code for OSTaskSwHook()                          */


/* ----------------------- 事件标志 ------------------------ *//* ----------------------- EVENT FLAGS ------------------------ */
#define OS_FLAG_EN                1u   /* 使能(1)或禁止(0)事件标志 *//* Enable (1) or Disable (0) code generation for EVENT FLAGS    */
#define OS_FLAG_ACCEPT_EN         1u   /* 使用 OSFlagAccept() *//*     Include code for OSFlagAccept()                          */
#define OS_FLAG_DEL_EN            1u   /* 使用 OSFlagDel() *//*     Include code for OSFlagDel()                             */
#define OS_FLAG_NAME_EN           1u   /*     Enable names for event flag group                        */
#define OS_FLAG_QUERY_EN          1u   /* 使用 OSFlagQuery() *//*     Include code for OSFlagQuery()                           */
#define OS_FLAG_WAIT_CLR_EN       1u   /* 等待清除事件标志 *//* Include code for Wait on Clear EVENT FLAGS                   */
#define OS_FLAGS_NBITS           16u   /* OS_FLAGS 数据类型(8,16或32位) *//* Size in #bits of OS_FLAGS data type (8, 16 or 32)            */


/* -------------------- 信息邮箱 --------------------- *//* -------------------- MESSAGE MAILBOXES --------------------- */
#define OS_MBOX_EN                1u   /* 使能(1)或禁止(0)信息邮箱 *//* Enable (1) or Disable (0) code generation for MAILBOXES      */
#define OS_MBOX_ACCEPT_EN         1u   /* 使用 OSMboxAccept() *//*     Include code for OSMboxAccept()                          */
#define OS_MBOX_DEL_EN            1u   /* 使用 OSMboxDel() *//*     Include code for OSMboxDel()                             */
#define OS_MBOX_PEND_ABORT_EN     1u   /* 使用 OSMboxPendAbort() *//*     Include code for OSMboxPendAbort()                       */
#define OS_MBOX_POST_EN           1u   /* 使用 OSMboxPost() *//*     Include code for OSMboxPost()                            */
#define OS_MBOX_POST_OPT_EN       1u   /* 使用 OSMboxPostOpt() *//*     Include code for OSMboxPostOpt()                         */
#define OS_MBOX_QUERY_EN          1u   /* 使用 OSMboxQuery() *//*     Include code for OSMboxQuery()                           */


/* --------------------- 存储管理 -------------------- *//* --------------------- MEMORY MANAGEMENT -------------------- */
#define OS_MEM_EN                 1u   /* 使能(1)或禁止(0)存储管理 *//* Enable (1) or Disable (0) code generation for MEMORY MANAGER */
#define OS_MEM_NAME_EN            1u   /*     Enable memory partition names                            */
#define OS_MEM_QUERY_EN           1u   /* 使用 OSMemQuery() *//*     Include code for OSMemQuery()                            */


/* ---------------- 互斥信号量 --------------- *//* ---------------- MUTUAL EXCLUSION SEMAPHORES --------------- */
#define OS_MUTEX_EN               1u   /* 使能(1)或禁止(0)互斥信号量 *//* Enable (1) or Disable (0) code generation for MUTEX          */
#define OS_MUTEX_ACCEPT_EN        1u   /* 使用 OSMutexAccept() *//*     Include code for OSMutexAccept()                         */
#define OS_MUTEX_DEL_EN           1u   /* 使用 OSMutexDel() *//*     Include code for OSMutexDel()                            */
#define OS_MUTEX_QUERY_EN         1u   /* 使用 OSMutexQuery() *//*     Include code for OSMutexQuery()                          */


/* ---------------------- 消息队列 ---------------------- *//* ---------------------- MESSAGE QUEUES ---------------------- */
#define OS_Q_EN                   1u   /* 使能(1)或禁止(0)消息队列 *//* Enable (1) or Disable (0) code generation for QUEUES         */
#define OS_Q_ACCEPT_EN            1u   /* 使用 OSQAccept() *//*     Include code for OSQAccept()                             */
#define OS_Q_DEL_EN               1u   /* 使用 OSQDel() *//*     Include code for OSQDel()                                */
#define OS_Q_FLUSH_EN             1u   /* 使用 OSQFlush() *//*     Include code for OSQFlush()                              */
#define OS_Q_PEND_ABORT_EN        1u   /* 使用 OSQPendAbort() *//*     Include code for OSQPendAbort()                          */
#define OS_Q_POST_EN              1u   /* 使用 OSQPost() *//*     Include code for OSQPost()                               */
#define OS_Q_POST_FRONT_EN        1u   /* 使用 OSQPostFront() *//*     Include code for OSQPostFront()                          */
#define OS_Q_POST_OPT_EN          1u   /* 使用 OSQPostOpt() *//*     Include code for OSQPostOpt()                            */
#define OS_Q_QUERY_EN             1u   /* 使用 OSQQuery() *//*     Include code for OSQQuery()                              */


/* ------------------------ 信号量 ------------------------ *//* ------------------------ SEMAPHORES ------------------------ */
#define OS_SEM_EN                 1u   /* 使能(1)或禁止(0)信号量 *//* Enable (1) or Disable (0) code generation for SEMAPHORES     */
#define OS_SEM_ACCEPT_EN          1u   /* 使用 OSSemAccept() *//*    Include code for OSSemAccept()                            */
#define OS_SEM_DEL_EN             1u   /* 使用 OSSemDel() *//*    Include code for OSSemDel()                               */
#define OS_SEM_PEND_ABORT_EN      1u   /* 使用 OSSemPendAbort() *//*    Include code for OSSemPendAbort()                         */
#define OS_SEM_QUERY_EN           1u   /* 使用 OSSemQuery() *//*    Include code for OSSemQuery()                             */
#define OS_SEM_SET_EN             1u   /* 使用 OSSemSet() *//*    Include code for OSSemSet()                               */


/* --------------------- 时间管理 ---------------------- *//* --------------------- TIME MANAGEMENT ---------------------- */
#define OS_TIME_DLY_HMSM_EN       1u   /* 使用 OSTimeDlyHMSM() *//*     Include code for OSTimeDlyHMSM()                         */
#define OS_TIME_DLY_RESUME_EN     1u   /* 使用 OSTimeDlyResume() *//*     Include code for OSTimeDlyResume()                       */
#define OS_TIME_GET_SET_EN        1u   /* 使用 OSTimeGet() and OSTimeSet() *//*     Include code for OSTimeGet() and OSTimeSet()             */
#define OS_TIME_TICK_HOOK_EN      1u   /* 使用 OSTimeTickHook() *//*     Include code for OSTimeTickHook()                        */


/* --------------------- 定时器管理 --------------------- *//* --------------------- TIMER MANAGEMENT --------------------- */
#define OS_TMR_EN                 1u   /* 使能(1)或禁止(0)定时器管理 *//* Enable (1) or Disable (0) code generation for TIMERS         */
#define OS_TMR_CFG_MAX           16u   /* 定时器最大值 *//*     Maximum number of timers                                 */
#define OS_TMR_CFG_NAME_EN        1u   /* 定时器名字大小  *//*     Determine timer names                                    */
#define OS_TMR_CFG_WHEEL_SIZE     8u   /* 定时器轮大小 *//*     Size of timer wheel (#Spokes)                            */
#define OS_TMR_CFG_TICKS_PER_SEC 10u   /* 定时器管理任务运行速度(Hz) *//*     Rate at which timer management task runs (Hz)            */

#endif
	 	   	  		 			 	    		   		 		 	 	 			 	    		   	 			 	  	 		 				 		  			 		 					 	  	  		      		  	   		      		  	 		 	      		   		 		  	 		 	      		  		  		  
