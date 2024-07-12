#include "hw2_test.h"
#include <stdio.h>
#include <iostream>
#include <cassert>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;

void test_get_weight_and_set_weight();
void test_get_heaviest_sibling();
void test_get_path_sum();
void printTree();
int main() {
	test_get_weight_and_set_weight();
	test_get_heaviest_sibling();
	test_get_path_sum();
}
void test_get_weight_and_set_weight()
{
	/*
	tests for get_weight and set_weight
	*/
	int x = get_weight();
	assert(x == 0 && "basic_get_weight_check");
    x = set_weight(5);
	assert(x == 0 && "basic_set_weight_check");
    x = get_weight();
	assert(x == 5 && "check_weight_changed");
	assert(get_weight()==x && "check_return_value_of_get_weight_again");
	assert(set_weight(-1)<0 && "negative_weight_check");
	assert(set_weight(0)==0 && "zero_weight_check");
	assert(get_weight()==0  && "zero_weight_change_check");
	int num=fork();
	if(num==0) //child
	{
		assert(get_weight()==0 && "check_that_child_inherits_weight");
		assert(set_weight(4)==0 && "change_weight_of_child");
		assert(get_weight()==4 && "check_change_of_weight_in_fork");
		int num2=fork();
		if(num2==0)
		{
			assert(get_weight()==4 && "check_that_child_of_child_inherits_weight");
			int num3=fork();
			if(num3==0)
			{
				assert(get_weight()==4 && "check_that_child_of_child_of_child_inherits_weight");
				int prev=get_weight();
				assert(set_weight(-1)<0  && "negative_weight_after_fork_check");
				assert(prev==get_weight() && "negative_weight_no_effect_after_fork_check");
				exit(0);
			}
			exit(0);
		}
		exit(0);
	}    
	cout << "===== SUCCESS get_weight and set_weight =====" << endl;
}
void test_get_heaviest_sibling()
{
	/*
	tests for get_heaviest_sibling
	*/
	int firstPid=getpid();
	assert(get_heaviest_sibling()==firstPid && "no_siblings_heaviest_child_1");
	set_weight(10);
	assert(get_heaviest_sibling()==firstPid && "no_siblings_heaviest_child_2");
	set_weight(8);
	assert(get_heaviest_sibling()==firstPid && "no_siblings_heaviest_child_2");
	set_weight(-1);
	assert(get_heaviest_sibling()==firstPid && "negative_weight_doesnt_effect_heaviest_sibling");
	std::cout << "waiting for all children to finish" << std::endl;
	int num=fork();
	if(num==0)
	{
		set_weight(7);
		assert(get_heaviest_sibling()==getpid() && "heaviest_sibling_change_1");
		sleep(10);
		exit(0);
	}
	sleep(3);
	num=fork();
	if(num==0)
	{
		set_weight(8);
		assert(get_heaviest_sibling()==getpid() && "heaviest_sibling_change_2");
		sleep(10);
		exit(0);
	}
	sleep(2);
	num=fork();
	if(num==0)
	{
		set_weight(11);
		assert(get_heaviest_sibling()==getpid() && "heaviest_sibling_change_3");
		sleep(10);
		exit(0);
	}
	sleep(2);
	num=fork();
	if(num==0)
	{
		set_weight(10);
		assert(get_heaviest_sibling()!=getpid() && "heaviest_sibling_doesnt_change");
		sleep(10);
		assert(get_heaviest_sibling()==getpid() && "heaviest_sibling_expired_change");
		exit(0);
	}
	while(wait(NULL)>0)
	{

	}
	cout << "===== SUCCESS get_heaviest_sibling =====" << endl;
}
void test_get_path_sum()
{
	set_weight(1);
	assert(get_path_sum(getpid())==1 && "check_path_of_size_1");
	assert(get_path_sum(1)<0 && "no_path_to_init");
	int pid=fork();
	if(pid==0)
	{
		set_weight(5);
		std::cout << "process B is pid " << getpid() << std::endl;
		pid=fork();
		if(pid==0)
		{
			set_weight(8);
			std::cout << "process C is pid " << getpid() << std::endl;
			pid=fork();
			if(pid==0)
			{
				set_weight(12);
				std::cout << "process D is pid " << getpid() << std::endl;
				while(true);
			}
			while(true);
		}
		pid=fork();
		if(pid==0)
		{
			set_weight(0);
			std::cout << "process E is pid " << getpid() << std::endl;
			pid=fork();
			if(pid==0)
			{
				set_weight(1);
				std::cout << "process F is pid " << getpid() << std::endl;
				pid=fork();
				if(pid==0)
				{
					set_weight(13);
					std::cout << "process G is pid " << getpid() << std::endl;
					pid=fork();
					if(pid==0)
					{
						set_weight(7);
						std::cout << "process H is pid " << getpid() << std::endl;
						while(true);
					}
					while(true);
				}
				while(true);
			}
			while(true);
		}
		while(true);
	}
	std::cout << "setting all the processes" << std::endl;
	std::cout << "process A is pid " << getpid() << std::endl;
	sleep(10);
	printTree();
	while(true)
	{
		std::cout << "select the pid you want for target" << std::endl;
		std::cout << "will return the path from process A to the chosen process" << std::endl;
		int target;
		std::cin >> target;
		std::cout << "get_path_sum returned: " << get_path_sum(target) << std::endl;
	}
}
void printTree()
{
	std::cout << "Process Tree:" << std::endl;
    std::cout << "Root Process (A, Weight: 1)" << std::endl;;
    std::cout << "-----Child Process (B, Weight: 5)" << std::endl;
   	std::cout << "---------Grandchild Process (C, Weight: 8)" << std::endl;
   	std::cout << "-------------Great-grandchild Process (D, Weight: 12)" << std::endl;
	std::cout << "---------Grandchild Process (E, Weight: 0)" << std::endl;
   	std::cout << "-------------Great-grandchild Process (F, Weight: 1)" << std::endl;
	std::cout << "--------------------Great-great-grandchild (G, Weight: 13)" << std::endl;
   	std::cout << "--------------------------Great-greate-great-grandchild Process (H, Weight: 7)" << std::endl;
}