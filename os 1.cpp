#include <iostream>

struct process
{
	char process_id;
	bool active;
	char arrival_time;
	char total_cpu_time;
	char remaining_cpu_time;
	char turnaround_time;
};

static inline int next(uint64_t* seed, const int bits)
{
	*seed = (*seed * 0x5deece66d + 0xb) & ((1ULL << 48) - 1);
	return (int)((int64_t)*seed >> (48 - bits));
}

static inline int nextInt(uint64_t* seed, const int n)
{
	int bits, val;
	const int m = n - 1;

	if ((m & n) == 0) {
		uint64_t x = n * (uint64_t)next(seed, 31);
		return (int)((int64_t)x >> 31);
	}

	do {
		bits = next(seed, 31);
		val = bits % n;
	} while (bits - val + m < 0);
	return val;
}

void start(process* sim_table, uint64_t* seed)
{
	char ctr = 0;
	for (int i = 0; i < 5; i++)
	{
		sim_table[i].process_id = ctr++;
		sim_table[i].arrival_time = nextInt(seed, 20);
		sim_table[i].total_cpu_time = nextInt(seed, 20) + 5;
		sim_table[i].remaining_cpu_time = sim_table[i].total_cpu_time;
		sim_table[i].active = sim_table[i].arrival_time == 0 ? 1 : 0;
	}
}

void print(process* sim_table)
{
	for (int i = 0; i < 5; i++)
	{
		printf("Process ID: %d, Arrival Time: %d, Total CPU Time: %d\n", sim_table[i].process_id, sim_table[i].arrival_time, sim_table[i].total_cpu_time);
	}
}

void print_table(process* sim_table, char current_time)
{
	printf("Time: %d\n", current_time);
	printf("ID | Arrival Time | Total CPU Time | Remaining CPU Time | Turnaround Time\n");
	for (int i = 0; i < 5; i++)
	{
		printf("%2d | %12d | %14d | %17d | %14d\n", sim_table[i].process_id, sim_table[i].arrival_time, sim_table[i].total_cpu_time, sim_table[i].remaining_cpu_time, sim_table[i].turnaround_time);
	}
	printf("\n");
}

void step(process* sim_table, char* current_time)
{
	int total_turnaround_time = 0;
	int completed_processes = 0;
	int total_cpu_time = 0;

	while (completed_processes < 5)
	{
		int shortest_remaining_time = INT_MAX;
		int selected_process = -1;

		for (int i = 0; i < 5; i++)
		{
			if (sim_table[i].active && sim_table[i].remaining_cpu_time < shortest_remaining_time)
			{
				shortest_remaining_time = sim_table[i].remaining_cpu_time;
				selected_process = i;
			}
		}

		if (selected_process == -1)
		{
			// No active process, increment time and activate any new arrivals
			(*current_time)++;
			for (int i = 0; i < 5; i++)
			{
				if (sim_table[i].arrival_time == *current_time)
				{
					sim_table[i].active = true;
				}
			}
		}
		else
		{
			// Execute the selected process for 1 time unit
			sim_table[selected_process].remaining_cpu_time--;

			if (sim_table[selected_process].remaining_cpu_time == 0)
			{
				sim_table[selected_process].active = false;
				sim_table[selected_process].turnaround_time = *current_time - sim_table[selected_process].arrival_time + 1;
				total_turnaround_time += sim_table[selected_process].turnaround_time;
				completed_processes++;
			}

			// Increment time and activate any new arrivals
			(*current_time)++;
			for (int i = 0; i < 5; i++)
			{
				if (sim_table[i].arrival_time == *current_time)
				{
					sim_table[i].active = true;
				}
			}

			// Print table
			print_table(sim_table, *current_time);
		}
	}

	// Calculate and print average total CPU time and average turnaround time
	for (int i = 0; i < 5; i++)
	{
		total_cpu_time += sim_table[i].total_cpu_time;
	}
	double avg_total_cpu_time = total_cpu_time / 5.0;
	double avg_turnaround_time = total_turnaround_time / 5.0;
	printf("Average Total CPU Time: %.2f\n", avg_total_cpu_time);
	printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);
}

int main()
{
	/*

	Instructions:

	Implement only the Shortest Remaining Time (SRT) scheduling algorithm.
	Assume that all processes will arrive within the first 20 time units (k = 20).
	Arrival times may be purely random (they do not need to be normally distributed).
	Assume 5 total processes (n = 5), all of which should have a total CPU time (Ti) between 5 and 25 determined at random (they do not need to be normally distributed, so you may disregard the variables d and v).
	The output should consist of:
		The process ID(0 through 4), arrival time, and total CPU time of each process
		The average total CPU time of all processes
		The ATT
	You may find it helpful to display the simulation table after each time unit, as this will help you determine if processes are being preempted correctly according to SRT. (This would certainly help your professor evaluate your simulation.)

	*/

	/*

	NOTE: CHAR IS BEING USED FOR 4 BITS. This is really not needed, I just like using specific bit values for performance/optimizations (not much changes in this code with them)
	 
	 */

	process sim_table[5] = {};
	uint64_t seed = 42;

	start(sim_table, &seed);
	print(sim_table);
	char current_time = 0;
	step(sim_table, &current_time);
}