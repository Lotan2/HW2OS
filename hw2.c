#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/sched/task.h>
#include <linux/syscalls.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage long sys_set_weight(int newweight) {
	if(newweight < 0) {
		return -EINVAL;	
	}	
	current->weight = newweight;
	return 0;
}

asmlinkage long sys_get_weight(void) {
	return current->weight;
}

asmlinkage long sys_get_path_sum(pid_t target) {
	struct task_struct *target_task;
	struct task_struct *current_task = current;
	struct task_struct *temp_task;
	long sum = 0;

	rcu_read_lock();
	target_task = pid_task(find_vpid(target), PIDTYPE_PID);
	if(!target_task) {
		rcu_read_unlock();
		return -ECHILD;
	}
	temp_task = target_task;
	while(temp_task != &init_task)
	{
		if(temp_task == current_task) { 
			// found the path now sum up the weights
			temp_task = target_task;
			sum= current_task->weight;			
			while(temp_task != current_task)
			{
				sum+=temp_task->weight;
				temp_task = temp_task->parent;
			}
			rcu_read_unlock;
			return sum;		
		}
		// continue to move up the tree until we reach the current
		temp_task= temp_task->parent;
	}
	// we didnt reach the current so its not descendant of current process
	rcu_read_unlock();
	return -ECHILD;
}

asmlinkage pid_t sys_get_heaviest_sibling(void) {
	pid_t minPID = current->pid;
	long minWeight = current->weight;
	struct task_struct *potSib;
	rcu_read_lock();	
	for_each_process( potSib )
	{
		if(potSib->real_parent->pid == current->real_parent->pid && potSib->weight >= minWeight)
		{ 
			//sibling and he is heavier
			if(potSib->weight == minWeight)
			{	
				if(potSib->pid < minPID)
				{	
					minPID=potSib->pid;
					minWeight=potSib->weight;
				}
			}
			else
			{
			minPID=potSib->pid;
			minWeight=potSib->weight;
			}
		}
	}
	rcu_read_unlock();
	return minPID;
}