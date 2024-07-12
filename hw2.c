#include <linux/kernel.h>

asmlinkage long sys_hello(void) {
	printk("Hello, World!\n");
	return 0;
}

asmlinkage int set_weight(int newweight) {
	if(newweight < 0) {
		return -EINVAL;	
	}	
	weight = newweight;
	return 0;
}

asmlinkage int get_weight(void) {
	return weight;
}

asmlinkage int get_path_sum(pid_t target) {
	struct task_struct *target_task;
	struct task_struct *current_task = current;
	int sum = 0;

	rcu_read_lock();
	target_task = pid_task(find_vpid(target), PIDTYPE_PID);
	if(!target_task) {
		rcu_read_unlock();
		return -ECHILD;
	}
	struct task_struct *temp_task;
	while(temp_task != &init_task)
	{
		if(temp_task == current_task) { 
			// found the path now sum up the weights
			temp_task = target_task;
			sum= current_task->weight;			
			while(target_task != current_task)
			{
				sum+=temp_task->weight;
				temp_task = temp_task->parent;
			}
			rcu_read_unlock;
			return sum;		
		}
		// continue to move up the tree until we reach the current
		target_task= target_task->parent;
	}
	// we didnt reach the current so its not descendant of current process
	rcu_read_unlock();
	return -ECHILD;
}

asmlinkage pid_t get_heaviest_sibling(void) {
	pid_t minPID = current->pid;
	int minWeight = current->weight;	
	for_each_process( potSib )
	{
		if(potSib->real_parent->pid == current->real_parent->pid && potSib->weight >= minWeight)
		{ 
			//sibling and he is heavier
			if(potSib->weight == minWeight)
			{	
				if(potSib->pid < minPid)
				{	
					minPID=potSib->pid;
					minWeight=potSib->weight;
				}
			}
		}
		else
		{
			minPID=potSib->pid;
			minWeight=potSib->weight;
		}
	}
	return minPID;
}