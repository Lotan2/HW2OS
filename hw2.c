#include <linux/kernel.h>

asmlinkage long sys_hello(void)
{
	printk("Hello, World!\n");
	return 0;
}

asmlinkage int sys_set_weight(int newweight)
{
	if (newweight < 0)
	{
		return -EINVAL;
	}
	weight = newweight;
	return 0;
}

asmlinkage int sys_get_weight(void)
{
	return weight;
}

asmlinkage int sys_get_path_sum(pid_t target)
{
	struct task_struct *target_task;
	struct task_struct *current_task = current;
	int sum = 0;

	rcu_read_lock();
	target_task = pid_task(find_vpid(target), PIDTYPE_PID);
	if (!target_task)
	{
		rcu_read_unlock();
		return -ECHILD;
	}
	struct task_struct *temp_task;
	while (temp_task != &init_task)
	{
		if (temp_task == current_task)
		{
			// found the path now sum up the weights
			temp_task = target_task;
			sum = current_task->weight;
			while (temp_task != current_task)
			{
				sum += temp_task->weight;
				temp_task = temp_task->parent;
			}
			rcu_read_unlock;
			return sum;
		}
		// continue to move up the tree until we reach the current
		target_task = target_task->parent;
	}
	// we didnt reach the current so its not descendant of current process
	rcu_read_unlock();
	return -ECHILD;
}

asmlinkage pid_t sys_get_heaviest_sibling(void)
{
	rcu_read_lock();
	pid_t maxPID = current->pid;
	int maxWeight = current->weight;
	struct task_struct *potSib;

	for_each_process(potSib)
	{
		if (potSib->real_parent->pid == current->real_parent->pid && potSib->weight >= maxWeight)
		{
			// sibling and he is heavier
			if (potSib->weight == maxWeight)
			{
				if (potSib->pid < maxPid)
				{
					maxPID = potSib->pid;
					maxWeight = potSib->weight;
				}
			}
			else
			{
				maxPID = potSib->pid;
				minWeight = potSib->weight;
			}
		}
	}
	rcu_read_unlock();
	return maxPID;
}