/*
*	Module to list processes and scheduling information belonging to a
*	specific user.
*/
#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */
#include <linux/timekeeping.h> // for ktime
#include <linux/smp.h> // for smp_processor_id
#include <linux/sched.h> // for struct rq and cpu_rq
#include <linux/bitmap.h> // for test_bit
#include <linux/delay.h> // for ssleep
#define DRIVER_AUTHOR "Trey Manuszak <tmanusza@asu.edu>, Omair Neazi <oneazi@asu.edu>, Aswinraj Ravichandran Prema <aravic18@asu.edu>"
#define DRIVER_DESCRIPTION "Prints processes and scheduling information belonging to a specific user"

/*
 * Static variable declarations
 */
static int uid;	// UID of the user

/*
 * Module parameters
 */
module_param(uid, int, S_IRUGO);	// get the UID of the user as module parameter
MODULE_PARM_DESC(uid, "The UID to print the running processes of"); // Parameter description

// constant array of scheduling policy names
const char * policies[7] = {"SCHED_NORMAL", "SCHED_FIFO", "SCHED_RR", "SCHED_BATCH", "SCHED_ISO", "SCHED_IDLE", "SCHED_DEADLINE"};

// function to print all processes belonging to the given uid
void print_processes(int uid)
{
	struct task_struct * task;
	printk("%-10s%-20s%-8s%-5s\n", "PID", "NAME", "PRIO", "POLICY");
	for_each_process(task)
	{
		if (task->cred->uid.val == uid)
		{
			printk("%-10d%-20s%-8d%-20s\n", task->pid, task->comm, 139-task->prio, policies[task->policy]);
		}
	}
}

void print_real_time_processes(void) {
	int bitmapIndex;
	int cpu = smp_processor_id();
	struct rq *rq = cpu_rq(cpu);
	struct sched_rt_entity *entity;
	struct task_struct *task;

	for (bitmapIndex = 0; bitmapIndex < 100; bitmapIndex++) {
		if (test_bit(rq->rt->active->bitmap, bitmapIndex)) { // If queue[bitmapIndex] is occupied
			list_for_each_entry(entity, &(rq->rt->active->queue[bitmapIndex]), run_list) { // iterate through each task in queue
				task = rt_task_of(entity);
				// print task attributes
			}

		}	
	}
	return;
}

static int __init print_processes_init(void)
{
	//printk(KERN_INFO "This is %d\n", uid); // Testing how parameter works
	
	print_real_time_processes();

	

	return 0;
}

static void __exit print_processes_exit(void)
{
	return;
}



module_init(print_processes_init);
module_exit(print_processes_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESCRIPTION);
MODULE_LICENSE("GPL");
